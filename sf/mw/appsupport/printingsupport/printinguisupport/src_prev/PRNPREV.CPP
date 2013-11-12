// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#include <e32std.h>
#include <e32base.h>

#include <w32std.h>
#include <prnprev.h>
#include "PRNDEV.H"
#include "PREVSTD.H"
#include "PREVPRI.H"

const TInt KPreviewArrayGranularity(1);
const TInt KPreviewPanelBorderWidth(1);

EXPORT_C void InternalizeL(CPrintPreviewImage::TMarginState& aThing,RReadStream& aStream)
	{
	TInt thing=aStream.ReadUint8L();
	if (thing>CPrintPreviewImage::EHideMargins) // or whatever the range check is
		User::Leave(KErrCorrupt);
	aThing=CPrintPreviewImage::TMarginState(thing); 
	}


//
// CPrintPreviewImage
//

EXPORT_C CPrintPreviewImage* CPrintPreviewImage::NewL(CPrintSetup& aPrintSetup,MPageRegionPrinter& aBodyPrinter,
											RWsSession& aWsSession,RWindow& aWindow,const TSize& aMaxWindowSize,
											const TPrintPreviewFormat& aStaticFormat)
/** Static constructor for a CPrintPreviewImage object.

@param aPrintSetup The printer setup.
@param aBodyPrinter A page region printer. Defines a function to print the 
body of each page band by band.
@param aWsSession A window server session. This may be the GUI framework session 
accessed through iEikonEnv->WsSession().
@param aWindow The window in which the image is to be drawn. Typically this 
is the window of the owning control.
@param aMaxWindowSize The maximum size of the window aWindow.
@param aStaticFormat The print preview format.
@return The newly created print preview image. */
	{
	CPrintPreviewImage* self=new(ELeave) CPrintPreviewImage(aPrintSetup,aBodyPrinter,aWsSession,aWindow,
														aMaxWindowSize,aStaticFormat);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


CPrintPreviewImage::CPrintPreviewImage(CPrintSetup& aPrintSetup,MPageRegionPrinter& aBodyPrinter,
											RWsSession& aWsSession,RWindow& aWindow,const TSize& aWindowSize,
											const TPrintPreviewFormat& aFormat)
	:iPrintSetup(&aPrintSetup),
	iAppBodyPrinter(&aBodyPrinter),
	iWsSession(&aWsSession),
	iWindow(&aWindow),
	iFormat(aFormat),
	iWinSize(aWindowSize),
	iNumBands(1)
	{	
	__DECLARE_NAME(_S("CPrintPreviewImage"));
	}



void CPrintPreviewImage::ConstructL()
// Constructs PreviewPageArea array and also preview device
	{
	iPanelArray = new(ELeave) CArrayFixSeg<TPreviewPanel>(KPreviewArrayGranularity);
	CWsScreenDevice* winDev=new(ELeave) CWsScreenDevice(*iWsSession);
	CleanupStack::PushL(winDev);
	User::LeaveIfError(winDev->Construct());
	iDevice = CPrintPreviewDevice::NewL(winDev); // takes ownership of winDev
	CleanupStack::Pop(winDev);
	iDevice->SetWindow(iWindow);
	// create a font for the labels
	User::LeaveIfError(iDevice->GetNearestFontToDesignHeightInPixels(iLabelFont,iFormat.iLabelFontSpecInPixels));
	}

EXPORT_C CPrintPreviewImage::~CPrintPreviewImage()
/** Destructor.

This ends printing, and frees all resources owned by the CPrintPreviewImage 
object, prior to its destruction. */
	{
	iPrintSetup->EndPrint(); // make sure previewing doesn't continue after I die
	if (iLabelFont && iDevice)
		iDevice->ReleaseFont(iLabelFont);
	delete iPanelArray;
	delete iDevice;
	}

EXPORT_C void CPrintPreviewImage::SetMargins(TMarginState aState)
/** Sets whether or not margins are displayed. 

@param aState The margin state - on or off. */
	{
	iMarginState = aState;
	for (TInt i=0 ; i<iPanelArray->Count() ; i++)
		(*iPanelArray)[i].iFullyDrawn = EFalse;
	}

EXPORT_C void CPrintPreviewImage::SetObserver(MPrintProcessObserver& aObserver)
/** Sets the process observer.

The observer is informed of events occurring during print previewing. It allows 
error dialogs to be displayed by the GUI print preview dialogs if there are 
problems during previewing.

@param aObserver The preview process observer. */
	{
	iObserver = &aObserver;
	}

EXPORT_C void CPrintPreviewImage::SetNumBands(TInt aNum)
/** Sets the number of print bands to be drawn in the preview.

On construction, the number of bands is initialised to to 1.

@param aNum The number of print bands. */
	{
	iNumBands = aNum;
	}

EXPORT_C void CPrintPreviewImage::FormatL(TInt aNumPagesInDoc,TInt aFirstPageToDisplay,TInt aNumPagesToView)
/** Sets preview formatting information.

This function can be called prior to previewing, or dynamically during previewing. 
It is used to alter the display parameters, e.g. turn margin indicator lines 
on or off, show/hide labels, change the number of panels viewed, or scroll 
the preview to show different pages.

Note:

The panels are fitted to the maximum window area available. This means that 
the print preview dialog will always be the same size irrespective of the 
number of panels displayed. 

@param aNumPagesInDoc The number of pages in the document.
@param aFirstPageToDisplay The first page to display.
@param aNumPagesToView The number of pages to preview.
@panic 6 If the number of pages in the document or the number of pages to be 
previewed is not greater than zero.
@panic 7 If aFirstPageToDisplay is not less than aNumPagesInDoc or if the first 
page to display is less than zero. */
	{// recalculates the size and position of panels and sets the zoom factor
	// only 1-4 pages or facing pages allowed
	__ASSERT_ALWAYS( (aNumPagesInDoc>0), Panic(EPanicIllegalNumberOfPages));
	__ASSERT_ALWAYS( (aNumPagesToView>0), Panic(EPanicIllegalNumberOfPages));
	__ASSERT_ALWAYS( aFirstPageToDisplay<aNumPagesInDoc, Panic(EPanicIllegalPageNumber));
	__ASSERT_ALWAYS( aFirstPageToDisplay>=0, Panic(EPanicIllegalPageNumber));

	iNumPagesInDoc = aNumPagesInDoc;
	//
	// set the label width
	SetLabelWidth();
	//
	// record the old print range for scrolling purposes
	iDrawRange.iFrom = iPrintParams.iFirstPage;
	iDrawRange.iTo = iPrintParams.iLastPage;
	TSize oldGutter(iActualGutter);
	//
	// Initialise the panel array
	iPrintParams.iFirstPage = aFirstPageToDisplay;
	iPrintParams.iLastPage = SetPanelArrayL(aFirstPageToDisplay,aNumPagesToView);
	//
	// invalidate the scrolling if the gutter size has changed
	if (iActualGutter!=oldGutter)
		iDrawRange.iFrom = KErrGeneral;
	//
	// invalidate the scrolling if the range is the same as before
	//if (iPrintParams.iFirstPage==iDrawRange.iFrom && iPrintParams.iLastPage==iDrawRange.iTo)
	//	iDrawRange.iFrom = KErrGeneral;
	//
	// set the pixel to twip mapping
	TPageSpec pageSpec = iPrintSetup->PrinterDevice()->CurrentPageSpecInTwips();
	iDevice->SetTwipToPixelMapping(iPanelSizeInPixels,pageSpec.OrientedPageSize());
	}


void CPrintPreviewImage::SetLabelWidth()
	{
	__ASSERT_DEBUG( (iNumPagesInDoc>0), Panic(EPanicIllegalNumberOfPages));
	//
	TInt num = iNumPagesInDoc+iPrintSetup->iNumOfFirstPage-1;
	TInt i;
	for (i=0 ; num>0 ; i++)
		num /= 10;
	if (iPrintSetup->iNumOfFirstPage<0)
		i++; // account for '-' sign
	iLabelSize.iHeight = iLabelFont->AscentInPixels()+1; // numbers never have a decender
	iLabelSize.iWidth = iLabelFont->MaxNormalCharWidthInPixels()*i;
	}


TInt CPrintPreviewImage::SetPanelArrayL(const TInt aFirstPage,const TInt aNumPagesToView)
// Scales the required number of panels to fill the window, centres these panels in the window,
//  and puts only those panels in the array.
// Returns number of last page in the preview 
//
	{
	TPageSpec pageSpecInTwips=iPrintSetup->PrinterDevice()->CurrentPageSpecInTwips();
	__ASSERT_ALWAYS (pageSpecInTwips.OrientedPageSize().iHeight>0 && pageSpecInTwips.OrientedPageSize().iWidth>0,Panic(EPanicPageTooSmall));
	//
	TInt width=Max(iFormat.iMinPanelGutterInPixels.iWidth,
					iLabelSize.iWidth+iFormat.iPanelShadowWidthInPixels+2*iFormat.iLabelOffsetInPixels);
	iActualGutter = TSize(width,iFormat.iMinPanelGutterInPixels.iHeight);
	//
	// calculate final panel size, scaling height up to fit
	iPanelSizeInPixels.iHeight = iWinSize.iHeight-(2*iActualGutter.iHeight)/*-iLabelFont->HeightInPixels()*/;
	iPanelSizeInPixels.iWidth = (iPanelSizeInPixels.iHeight*pageSpecInTwips.OrientedPageSize().iWidth)
		/pageSpecInTwips.OrientedPageSize().iHeight;
	// check it still fits widthways, if not then scale down to width
	if ( ((aNumPagesToView*(iPanelSizeInPixels.iWidth+iActualGutter.iWidth))+iActualGutter.iWidth) > iWinSize.iWidth)
		{// too wide so shrink
		iPanelSizeInPixels.iWidth = (iWinSize.iWidth-((aNumPagesToView+1)*iActualGutter.iWidth))/aNumPagesToView;
		iPanelSizeInPixels.iHeight = (iPanelSizeInPixels.iWidth*pageSpecInTwips.OrientedPageSize().iHeight)
			/pageSpecInTwips.OrientedPageSize().iWidth;
		}
	//
	// calculate the iActualGutter width and height such that the panels are evenly spread over the whole window area
	iActualGutter.iWidth = (iWinSize.iWidth-(iPanelSizeInPixels.iWidth*aNumPagesToView))/(aNumPagesToView+1);
	iRndError.iWidth = iWinSize.iWidth-(iActualGutter.iWidth*(aNumPagesToView+1)+(iPanelSizeInPixels.iWidth*aNumPagesToView));
	iActualGutter.iHeight = (iWinSize.iHeight-iPanelSizeInPixels.iHeight)/2;
	iRndError.iHeight = iWinSize.iHeight-iPanelSizeInPixels.iHeight-iActualGutter.iHeight*2;
	//
	// work out how many panels *will* be displayed
	TInt numPagesToView = (aNumPagesToView>(iNumPagesInDoc-aFirstPage))? iNumPagesInDoc-aFirstPage : aNumPagesToView;
	//
	// shrink the panel to take the borders into account
	iPanelSizeInPixels.iWidth -= (2*KPreviewPanelBorderWidth); //+KPreviewPanelShadowWidth
	iPanelSizeInPixels.iHeight -= (2*KPreviewPanelBorderWidth); //+KPreviewPanelShadowWidth
	TInt hOffsetPerPanel = iPanelSizeInPixels.iWidth+iActualGutter.iWidth+2*KPreviewPanelBorderWidth;
	//
	// fill the array
	TRange iFullyDrawn;
	iFullyDrawn.iFrom = KErrGeneral;
	iFullyDrawn.iTo = iFullyDrawn.iFrom;
	for (TInt i=0 ; i<iPanelArray->Count() ; i++)
		{
		if ((*iPanelArray)[i].iFullyDrawn)
			{
			iFullyDrawn.iTo = iDrawRange.iFrom+i;
			if (iFullyDrawn.iFrom == KErrGeneral)
				iFullyDrawn.iFrom = iFullyDrawn.iTo;
			}
		}
	iPanelArray->Reset();
	TPreviewPanel panel;
	for (TInt currentCol=0 ; currentCol<numPagesToView ; currentCol++)
		{ // lay up a row
		panel.iPanelPos.iX = iActualGutter.iWidth+hOffsetPerPanel*currentCol+KPreviewPanelBorderWidth;
		panel.iPanelPos.iY = iActualGutter.iHeight+KPreviewPanelBorderWidth;
		TInt pageNum=currentCol+aFirstPage;
		panel.iFullyDrawn = (iFullyDrawn.iFrom!=KErrGeneral && pageNum>=iFullyDrawn.iFrom && pageNum<=iFullyDrawn.iTo);
		iPanelArray->AppendL(panel);
		}

	iPanelArray->Compress(); // we dont want to waste any space
	__ASSERT_DEBUG(iPanelArray->Count()==numPagesToView,Panic(EPanicInconsistentNumberOfPanels));
	return aFirstPage+numPagesToView-1;
	}



void CPrintPreviewImage::NotifyPrintStarted(TPrintParameters aPrintParams)
	{
	if (iObserver)
		iObserver->NotifyPrintStarted(aPrintParams);
	}



void CPrintPreviewImage::NotifyBandPrinted(TInt aPercentageOfPagePrinted, TInt aCurrentPageNum, TInt aCurrentCopyNum)
	{
	if (iObserver)
		iObserver->NotifyBandPrinted(aPercentageOfPagePrinted,aCurrentPageNum,aCurrentCopyNum);	
	}



void CPrintPreviewImage::NotifyPrintEnded(TInt anError)
// Flushes the window buffer to ensure that the whole preview has been output
//
	{
	iWsSession->Flush();
	if (iObserver)
		iObserver->NotifyPrintEnded(anError);
	}

EXPORT_C void CPrintPreviewImage::DrawL(const TPoint& aOrigin,const TRect& aInvalidRegion)
// Draws the preview by calling PreviewPrint with a page range that covers the visible panels 
// aInvalidRegion is assumed to be relative to aOrigin
// 
/** Draws the preview.

This function should be called every time the window needs to be refreshed. 

Note that typically this function is called in the owning control's Draw() 
function. Since DrawL() can leave it must be called within a trap harness. 
The normal behaviour is to clear the image rectangle if a leave occurs. 

@param aOrigin The drawing origin.
@param aInvalidRegion The invalid region. This is assumed to be relative to 
aOrigin.
@leave KErrNoMemory If there is an out of memory error.
@panic 4 If the panel array has not been initialised. Call FormatL() to avoid this. */
	{
	__ASSERT_ALWAYS(iPanelArray->Count()>0, Panic(EPanicNotFormatted));

	iOrigin = aOrigin;
	iPrintSetup->EndPrint();
	TRect headerRect; TRect footerRect;
	CalculateHeaderFooterRects(headerRect,footerRect);
	//
	// check any invalid area
	if (!aInvalidRegion.IsEmpty())
		{
		if (iDrawRange.iFrom==iPrintParams.iFirstPage && iDrawRange.iTo==iPrintParams.iLastPage)
			{// no scrolling need take place
			TSize panelSize(iPanelSizeInPixels.iWidth+iActualGutter.iWidth,iWinSize.iHeight); // size of panel+gutter
			TPoint offset(iActualGutter.iWidth,iActualGutter.iHeight); // offset of panel corner from corner of it's gutter
			// mark any pages that intersect the invalid region as undrawn
			for (TInt i=0 ; i<iPanelArray->Count() ; i++)
				{
				TRect panelRect((*iPanelArray)[i].iPanelPos-offset,panelSize);
				if (aInvalidRegion.Intersects(panelRect))
					(*iPanelArray)[i].iFullyDrawn = EFalse;
				}
			}
		else
			iDrawRange.iFrom = KErrGeneral; // redraw everything
		}
	else if (iDrawRange.iFrom!=KErrGeneral)
		ScrollPanels(); // carry out scrolling if possible
	//
	if (iDrawRange.iFrom==KErrGeneral)
		{
		// no scrolling possible - redraw all panels
		iDrawRange.iFrom = iPrintParams.iFirstPage;
		iDrawRange.iTo = iPrintParams.iLastPage;
		// set all pages in the draw range invalid
		for (TInt x=iDrawRange.iFrom ; x<=iDrawRange.iTo ; x++)
			(*iPanelArray)[x-iPrintParams.iFirstPage].iFullyDrawn = EFalse;
		}
	//
	// draw the "background" bits synchronously
	CGraphicsContext* gc;
	iDevice->SetPanelPos(iOrigin);
	User::LeaveIfError( iDevice->CreateContext(gc) );
	((CPrintPreviewGc*)gc)->CWindowGc::CancelClippingRect(); // so drawing isn't clipped at all
	DrawExtremities(gc);
	delete gc;
	gc = NULL;
	for (TInt i=iDrawRange.iFrom ; i<=iDrawRange.iTo ; i++)
		{// draws all panels surrounds
		iDevice->SetPanelPos((*iPanelArray)[i-iPrintParams.iFirstPage].iPanelPos+iOrigin);
		User::LeaveIfError( iDevice->CreateContext(gc) );
		((CPrintPreviewGc*)gc)->CWindowGc::CancelClippingRect(); // so drawing isn't clipped at all
		DrawToGutter(gc,i);
		if (!(*iPanelArray)[i-iPrintParams.iFirstPage].iFullyDrawn)
			DrawBorder(gc);  // only draws borders of invalid panels
		DrawLabel(gc,i);
		delete gc;
		gc = NULL;
		}
	//
	// draw the content (only the unscrolled panels)
	TPrintParameters params(iPrintParams);
	params.iFirstPage = iPrintParams.iFirstPage;
	params.iLastPage = iPrintParams.iLastPage;
	iPrintSetup->StartPreviewPrintL(params,*this,this,*iDevice,headerRect,footerRect,iNumBands);
	}


void CPrintPreviewImage::ScrollPanels()
	{
	TRange scrollRange(iDrawRange);
	scrollRange.iFrom = (iDrawRange.iFrom<iPrintParams.iFirstPage)? iPrintParams.iFirstPage : iDrawRange.iFrom;
	scrollRange.iTo = (iDrawRange.iTo>iPrintParams.iLastPage)? iPrintParams.iLastPage : iDrawRange.iTo;
	while (scrollRange.iFrom<=scrollRange.iTo && !(*iPanelArray)[scrollRange.iFrom-iPrintParams.iFirstPage].iFullyDrawn)
		scrollRange.iFrom++; // don't scroll panels that aren't completely drawn
	while (scrollRange.iTo>=scrollRange.iFrom && !(*iPanelArray)[scrollRange.iTo-iPrintParams.iFirstPage].iFullyDrawn)
		scrollRange.iTo--; // don't scroll panels that aren't completely drawn
	if (scrollRange.iFrom>scrollRange.iTo)
		{
		iDrawRange.iFrom = KErrGeneral;
		return;
		}
	//
	// work out the area we can scroll
	TRect scrollRect;
	scrollRect.iTl.iY = 0;
	scrollRect.iBr.iY = iWinSize.iHeight;
	scrollRect.iTl.iX = (scrollRange.iFrom-iDrawRange.iFrom)*(iActualGutter.iWidth+iPanelSizeInPixels.iWidth)+iActualGutter.iWidth/2;
	scrollRect.iBr.iX = (scrollRange.iTo-iDrawRange.iFrom+1)*(iActualGutter.iWidth+iPanelSizeInPixels.iWidth)+iActualGutter.iWidth/2;
	//
	// work out the scroll offset
	TPoint offset(0,0);
	offset.iX = (iDrawRange.iFrom-iPrintParams.iFirstPage)*(iActualGutter.iWidth+iPanelSizeInPixels.iWidth+2);
	//
	// scroll
	iWindow->Scroll(offset,scrollRect);
	//
	// set the pages that still have to be drawn (inc. marking them undrawn)
	iDrawRange.iFrom = iPrintParams.iFirstPage;
	iDrawRange.iTo = iPrintParams.iLastPage;
	for (TInt i=iPrintParams.iFirstPage ; i<=iPrintParams.iLastPage ; i++)
		{
		if (i<scrollRange.iFrom)
			{
			iDrawRange.iFrom = iPrintParams.iFirstPage;
			iDrawRange.iTo = scrollRange.iFrom-1;
			(*iPanelArray)[i-iPrintParams.iFirstPage].iFullyDrawn = EFalse;
			}
		else if (i>scrollRange.iTo)
			{
			iDrawRange.iFrom = scrollRange.iTo+1;
			iDrawRange.iTo = iPrintParams.iLastPage;
			(*iPanelArray)[i-iPrintParams.iFirstPage].iFullyDrawn = EFalse;
			}
		}
	}


void CPrintPreviewImage::DrawBorder(CGraphicsContext* aGc)
	{
	aGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
	// draw page border
	aGc->SetBrushColor(iFormat.iPreviewBackgroundColor);
	TRect border(iPanelSizeInPixels);
	border.Grow(KPreviewPanelBorderWidth,KPreviewPanelBorderWidth);
	aGc->DrawRect(border);
	// draw shadow
	TRect sideShadow=border;
	sideShadow.iTl.iX += border.Width();
	sideShadow.iTl.iY += iFormat.iPanelShadowWidthInPixels;
	sideShadow.iBr.iX += iFormat.iPanelShadowWidthInPixels;
	TRect bottomShadow=border;
	bottomShadow.iTl.iY += border.Height();
	bottomShadow.iTl.iX += iFormat.iPanelShadowWidthInPixels;
	bottomShadow.iBr.iX += iFormat.iPanelShadowWidthInPixels;
	bottomShadow.iBr.iY += iFormat.iPanelShadowWidthInPixels;
	aGc->SetBrushColor(iFormat.iPanelShadowColor);
	aGc->DrawRect(sideShadow);
	aGc->DrawRect(bottomShadow);
	aGc->SetBrushColor(iFormat.iPreviewBackgroundColor);
	aGc->SetBrushStyle(CGraphicsContext::ENullBrush); // back to normal
	}


void CPrintPreviewImage::CalculateHeaderFooterRects(TRect& aHeaderRectInPixels,TRect& aFooterRectInPixels)
	{
	TRect pageRect(iPanelSizeInPixels);
	TRect marginRect(pageRect);
	marginRect.iTl.iX += iDevice->HorizontalTwipsToPixels(iPrintSetup->iPageMarginsInTwips.iMargins.iLeft);
	marginRect.iTl.iY += iDevice->VerticalTwipsToPixels(iPrintSetup->iPageMarginsInTwips.iMargins.iTop)-1;
	marginRect.iBr.iX -= iDevice->HorizontalTwipsToPixels(iPrintSetup->iPageMarginsInTwips.iMargins.iRight);
	marginRect.iBr.iY -= iDevice->VerticalTwipsToPixels(iPrintSetup->iPageMarginsInTwips.iMargins.iBottom);
	
	// header
	TPoint from = TPoint(marginRect.iTl.iX,pageRect.iTl.iY+iDevice->VerticalTwipsToPixels(iPrintSetup->iPageMarginsInTwips.iHeaderOffset));
	TPoint to = TPoint(marginRect.iBr.iX,marginRect.iTl.iY);
	aHeaderRectInPixels = TRect(from,to);
	// footer
	from.iY = marginRect.iBr.iY+iDevice->VerticalTwipsToPixels(iPrintSetup->iPageMarginsInTwips.iFooterOffset);
	to.iY = from.iY;
	aFooterRectInPixels = TRect(from,TPoint(to.iX,pageRect.iBr.iY));
	}


void CPrintPreviewImage::DrawMargins(CGraphicsContext* aGc)
	{
	TRect pageRect(iPanelSizeInPixels);
	TRect marginRect(pageRect);
	marginRect.iTl.iX += iDevice->HorizontalTwipsToPixels(iPrintSetup->iPageMarginsInTwips.iMargins.iLeft)-1; // -1 keeps line on outside
	marginRect.iTl.iY += iDevice->VerticalTwipsToPixels(iPrintSetup->iPageMarginsInTwips.iMargins.iTop)-1;
	marginRect.iBr.iX -= iDevice->HorizontalTwipsToPixels(iPrintSetup->iPageMarginsInTwips.iMargins.iRight);
	marginRect.iBr.iY -= iDevice->VerticalTwipsToPixels(iPrintSetup->iPageMarginsInTwips.iMargins.iBottom);
	// draw the margins one at a time
	//aGc->SetPenStyle(CGraphicsContext::EDottedPen); // dotty lines for margins
	aGc->SetPenColor(iFormat.iPanelMarginColor); // grey lines for margins
	TPoint from = pageRect.iTl;
	TPoint to = pageRect.iBr;
	// left margin
	from.iX = marginRect.iTl.iX;
	to.iX = from.iX;
	aGc->DrawLine(from,to);
	// right margin
	from.iX = marginRect.iBr.iX;
	to.iX = from.iX;
	aGc->DrawLine(from,to);
	// top margin
	from = TPoint(pageRect.iTl.iX,marginRect.iTl.iY);
	to = TPoint(pageRect.iBr.iX,marginRect.iTl.iY);
	aGc->DrawLine(from,to);
	// bottom margin
	from.iY = marginRect.iBr.iY;
	to.iY = from.iY;
	aGc->DrawLine(from,to);

	// header
	aGc->SetDrawMode(CGraphicsContext::EDrawModeAND);
	from = TPoint(marginRect.iTl.iX,pageRect.iTl.iY+iDevice->VerticalTwipsToPixels(iPrintSetup->iPageMarginsInTwips.iHeaderOffset));
	to = TPoint(marginRect.iBr.iX,from.iY);
	aGc->DrawLine(from,to);
	// footer
	from.iY = marginRect.iBr.iY+iDevice->VerticalTwipsToPixels(iPrintSetup->iPageMarginsInTwips.iFooterOffset);
	to.iY = from.iY;
	aGc->DrawLine(from,to);

	// reset
	aGc->SetPenStyle(CGraphicsContext::ESolidPen); // back to normal
	aGc->SetPenColor(TRgb(0,0,0)); // black
	}


void CPrintPreviewImage::DrawLabel(CGraphicsContext* aGc,TInt aPageNo)
	{
	TBuf<10> outputBuf; // magic number!
	outputBuf.Num(aPageNo+iPrintSetup->iNumOfFirstPage);
	aGc->UseFont(iLabelFont);

	TPoint tl; // for the label to be bottom left
	tl.iX = -iFormat.iLabelOffsetInPixels-iLabelSize.iWidth;
	tl.iY = iPanelSizeInPixels.iHeight-iLabelSize.iHeight+1; // label rect is purposely oversized by 1 pixel

	TRect rect(tl,iLabelSize);

	/*
	TRect rect(iPanelSizeInPixels);
	rect.iTl.iY += iPanelSizeInPixels.iHeight+KPreviewPanelBorderWidth+iFormat.iPanelShadowWidthInPixels;
	rect.iBr.iY = rect.iTl.iY+iFormat.iMinPanelGutterInPixels.iHeight+iLabelFont->HeightInPixels()-iFormat.iPanelShadowWidthInPixels;
	rect.Grow(iFormat.iMinPanelGutterInPixels.iWidth/2,0);
	TInt ascent = iLabelFont->AscentInPixels()+(iFormat.iMinPanelGutterInPixels.iHeight/2);
	*/

	aGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
	aGc->DrawText(outputBuf,rect,iLabelFont->AscentInPixels(),CGraphicsContext::ERight,0);
	aGc->SetBrushStyle(CGraphicsContext::ENullBrush);
	}



void CPrintPreviewImage::PrintBandL(CGraphicsDevice* /*aLayoutDevice*/,TInt aPageNo,const TBandAttributes& aBand)
// First decide how big my band should be, then...
// draw the panel border, then the margins, then its contents (band by band), then the label
//
	{
	// only print the page if it's invalidated
	if ((*iPanelArray)[aPageNo-iPrintParams.iFirstPage].iFullyDrawn)
		return;
	//
	// set the band in the device
	TBandAttributes band = aBand;
	iDevice->SetBandRect(band.iRect);
	//
	// see if this is the last band on the page
	TBool lastBand = EFalse;
	if (band.iTextIsIgnored)
		{// sneaky use of otherwise superfluous bool
		lastBand = ETrue;
		band.iTextIsIgnored = EFalse;
		}
	//
	// set the device to the correct panel pos, adjusting for the offset
	iDevice->SetPanelPos((*iPanelArray)[aPageNo-iPrintParams.iFirstPage].iPanelPos+iOrigin); // iDevice *is* aLayoutDevice
	// create a gc for me to draw with
	CGraphicsContext* gc;
	User::LeaveIfError( iDevice->CreateContext(gc) );
	CleanupStack::PushL(gc);
	//
	// draw the band
	iAppBodyPrinter->PrintBandL(iDevice,aPageNo,band);
	if (lastBand&&(iMarginState==EShowMargins))
		{
		// the margins are the last thing drawn on each page
		((CPrintPreviewGc*)gc)->SetBandRect(TRect(iPanelSizeInPixels));
		DrawMargins(gc);
		}
	if (lastBand)
		(*iPanelArray)[aPageNo-iPrintParams.iFirstPage].iFullyDrawn = ETrue;
	//
	CleanupStack::PopAndDestroy(); // gc
	}


void CPrintPreviewImage::DrawExtremities(CGraphicsContext* aGc)
// draws to the very left and right areas of the screen that are not assigned to any panel in particular
	{
	aGc->SetPenStyle(CGraphicsContext::ENullPen);
	aGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
	aGc->SetBrushColor(iFormat.iPreviewBackgroundColor);
	//
	TRect panelRect(iPanelSizeInPixels);
	panelRect.Grow(KPreviewPanelBorderWidth,KPreviewPanelBorderWidth);
	TRect rect;
	//
	// draw the left margin of the screen (in 3 rects to avoid the label)
	// top rect
	rect.iTl.iX = 0;
	rect.iTl.iY = 0;
	rect.iBr.iX = iActualGutter.iWidth/2+1; // rounds down
	rect.iBr.iY = iActualGutter.iHeight+iPanelSizeInPixels.iHeight+2*KPreviewPanelBorderWidth-iLabelSize.iHeight;
	aGc->DrawRect(rect);
	// bottom rect
	rect.iTl.iY = iActualGutter.iHeight+iPanelSizeInPixels.iHeight+2*KPreviewPanelBorderWidth;
	rect.iBr.iY = iWinSize.iHeight;
	aGc->DrawRect(rect);
	// middle rect (next to label)
	rect.iBr.iX = (*iPanelArray)[0].iPanelPos.iX-KPreviewPanelBorderWidth-iFormat.iLabelOffsetInPixels-iLabelSize.iWidth+1;
	rect.iBr.iY = iActualGutter.iHeight+iPanelSizeInPixels.iHeight+2*KPreviewPanelBorderWidth;
	rect.iTl.iY = iActualGutter.iHeight+iPanelSizeInPixels.iHeight+2*KPreviewPanelBorderWidth-iLabelSize.iHeight;
	aGc->DrawRect(rect);
	//
	// draw the right margin of the screen
	rect.iTl.iX = iWinSize.iWidth-(iActualGutter.iWidth)/2-iRndError.iWidth;
	rect.iTl.iY = 0;
	rect.iBr.iX = iWinSize.iWidth;
	rect.iBr.iY = iWinSize.iHeight;
	aGc->DrawRect(rect);
	}


void CPrintPreviewImage::DrawToGutter(CGraphicsContext* aGc,TInt aPageNo)
// draws to all the areas of screen that surround a particular panel
//
	{
	aGc->SetPenStyle(CGraphicsContext::ENullPen);
	aGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
	aGc->SetBrushColor(iFormat.iPreviewBackgroundColor);
	//
	TRect panelRect(iPanelSizeInPixels);
	panelRect.Grow(KPreviewPanelBorderWidth,KPreviewPanelBorderWidth);
	//
	TRect rect;
	// draw across the top
	rect.iTl.iX = panelRect.iTl.iX-(iActualGutter.iWidth+2)/2; // rounds up
	rect.iTl.iY = panelRect.iTl.iY-iActualGutter.iHeight;
	rect.iBr.iX = panelRect.iBr.iX+(iActualGutter.iWidth+2)/2; // rounds up
	rect.iBr.iY = panelRect.iTl.iY;
	aGc->DrawRect(rect);
	//
	// draw across the bottom
	rect.iTl.iY = panelRect.iBr.iY+iFormat.iPanelShadowWidthInPixels;
	rect.iBr.iY = panelRect.iBr.iY+iActualGutter.iHeight+iRndError.iHeight;
	aGc->DrawRect(rect);
	//
	// draw down the left (in three rects to avoid the label and fill in around the shadow)
	// top rect
	rect.iTl.iX = -(iActualGutter.iWidth+2)/2; // rounds up
	rect.iTl.iY = panelRect.iTl.iY;
	rect.iBr.iX = panelRect.iTl.iX;
	rect.iBr.iY = panelRect.iBr.iY-iLabelSize.iHeight;
	aGc->DrawRect(rect);
	// bottom rect (next to shadow)
	rect.iTl.iY = panelRect.iBr.iY;
	rect.iBr.iX	+= iFormat.iPanelShadowWidthInPixels;
	rect.iBr.iY = panelRect.iBr.iY+iFormat.iPanelShadowWidthInPixels;
	aGc->DrawRect(rect);
	// middle rect (between label and panel)
	rect.iTl.iX = -iFormat.iLabelOffsetInPixels;
	rect.iTl.iY = panelRect.iBr.iY-iLabelSize.iHeight-1;
	rect.iBr.iX = panelRect.iTl.iX;
	rect.iBr.iY = panelRect.iBr.iY+1;
	aGc->DrawRect(rect);
	//
	// draw down the right (in a few rects to fill in around the shadow and avoid a possible label)
	rect.iTl.iX = panelRect.iBr.iX;
	rect.iTl.iY = panelRect.iTl.iY;
	rect.iBr.iX = panelRect.iBr.iX+(iActualGutter.iWidth+2)/2;
	rect.iBr.iY = panelRect.iTl.iY+iFormat.iPanelShadowWidthInPixels;
	aGc->DrawRect(rect);
	if (IsLastPanel(aPageNo))
		{// no label - only one rect required
		rect.iTl.iX = panelRect.iBr.iX+iFormat.iPanelShadowWidthInPixels;
		rect.iTl.iY = panelRect.iTl.iY+iFormat.iPanelShadowWidthInPixels;
		rect.iBr.iX = panelRect.iBr.iX+(iActualGutter.iWidth+2)/2;
		rect.iBr.iY = panelRect.iBr.iY+iFormat.iPanelShadowWidthInPixels;
		aGc->DrawRect(rect);
		}
	else
		{// use three rects to avoid the label
		rect.iTl.iX = panelRect.iBr.iX+iFormat.iPanelShadowWidthInPixels;
		rect.iTl.iY = panelRect.iTl.iY+iFormat.iPanelShadowWidthInPixels;
		rect.iBr.iX = panelRect.iBr.iX+(iActualGutter.iWidth+2)/2;
		rect.iBr.iY = panelRect.iBr.iY-iLabelSize.iHeight;
		aGc->DrawRect(rect);
		rect.iTl.iY = panelRect.iBr.iY-1;
		rect.iBr.iY = panelRect.iBr.iY+iFormat.iPanelShadowWidthInPixels;
		aGc->DrawRect(rect);
		rect.iTl.iY = panelRect.iBr.iY-iLabelSize.iHeight-1;
		rect.iBr.iX = panelRect.iBr.iX+iActualGutter.iWidth-iLabelSize.iWidth-iFormat.iLabelOffsetInPixels+1;
		rect.iBr.iY = panelRect.iBr.iY;
		aGc->DrawRect(rect);
		}
	//
	aGc->SetPenStyle(CGraphicsContext::ESolidPen); // back to normal
	}


CPrintPreviewImage::TPreviewPanel::TPreviewPanel()
	:iPanelPos(0,0),
	iFullyDrawn(EFalse)
	{}
