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

#include  <e32std.h>

#include "PRNDEV.H"
#include "PREVSTD.H"

//
// CPrintPreviewDevice
//

CPrintPreviewDevice* CPrintPreviewDevice::NewL(CWsScreenDevice* aWinDevice)
	{
	return new(ELeave) CPrintPreviewDevice(aWinDevice);
	}


CPrintPreviewDevice::CPrintPreviewDevice(CWsScreenDevice* aWinDevice)
	: iWinDevice(aWinDevice),
	iVZoom(1000),
	iHZoom(1000)
	{}


CPrintPreviewDevice::~CPrintPreviewDevice()
	{
	delete iWinDevice;
	}


void CPrintPreviewDevice::SetTwipToPixelMapping(const TSize& aImageSizeInPixels,const TSize& aPageSizeInTwips)
	{
	iPageSizeInTwips = aPageSizeInTwips;
	TSize pageInPixels;
	pageInPixels.iHeight = iWinDevice->VerticalTwipsToPixels(aPageSizeInTwips.iHeight);
	pageInPixels.iWidth = iWinDevice->HorizontalTwipsToPixels(aPageSizeInTwips.iWidth);
	iVZoom = (pageInPixels.iHeight*EAccFac)/aImageSizeInPixels.iHeight;
	iHZoom = (pageInPixels.iWidth*EAccFac)/aImageSizeInPixels.iWidth;
	}


TInt CPrintPreviewDevice::CreateContext(CGraphicsContext*& aGc)
	{
	__ASSERT_ALWAYS(iWin,Panic(EPanicNoWindow));

	aGc=NULL;
	TRAPD(err,DoCreateContextL(aGc));
	if (err!=KErrNone)
		{
		delete aGc;
		aGc=NULL;
		}
	else
		{
		// set it active over the correct window
		((CWindowGc*)aGc)->Activate(*iWin);
		// set the origin correctly
		((CPrintPreviewGc*)aGc)->SetOriginOffset(iPanelPos);
		((CPrintPreviewGc*)aGc)->SetBandRect(iBandRect);
		}
	return(err);
	}


void CPrintPreviewDevice::DoCreateContextL(CGraphicsContext*& aGc)
	{
	aGc=new(ELeave) CPrintPreviewGc(iWinDevice);
	User::LeaveIfError(((CPrintPreviewGc*)aGc)->Construct());
	}


TSize CPrintPreviewDevice::SizeInPixels() const
	{
	return TwipsToPixels(iPageSizeInTwips).Size();
	}

TSize CPrintPreviewDevice::SizeInTwips() const
	{
	return iPageSizeInTwips;
	}

TInt CPrintPreviewDevice::HorizontalTwipsToPixels(TInt aTwips) const
	{
	return (iWinDevice->HorizontalTwipsToPixels(aTwips)*EAccFac)/iHZoom;
	}

TInt CPrintPreviewDevice::VerticalTwipsToPixels(TInt aTwips) const
	{
	return (iWinDevice->VerticalTwipsToPixels(aTwips)*EAccFac)/iVZoom;
	}

TInt CPrintPreviewDevice::HorizontalPixelsToTwips(TInt aPixels) const
	{
	return iWinDevice->HorizontalPixelsToTwips((aPixels*iHZoom)/EAccFac);
	}

TInt CPrintPreviewDevice::VerticalPixelsToTwips(TInt aPixels) const
	{
	return iWinDevice->VerticalPixelsToTwips((aPixels*iVZoom)/EAccFac);
	}

#ifdef SYMBIAN_SUPPORT_FONT_METRICS_V1
TInt CPrintPreviewDevice::GetNearestFontInTwips(CFont*& aFont, const TFontSpec& aFontSpec)
	{
	return GetNearestFontToDesignHeightInTwips(aFont, aFontSpec);
	}
#endif

TInt CPrintPreviewDevice::GetNearestFontToDesignHeightInTwips(CFont*& aFont, const TFontSpec& aFontSpec)
	{
	TFontSpec fontSpec = aFontSpec;
	fontSpec.iHeight = (fontSpec.iHeight  * EAccFac) / iVZoom;
	return iWinDevice->GetNearestFontToDesignHeightInTwips(aFont, fontSpec);
	}

TInt CPrintPreviewDevice::GetNearestFontToMaxHeightInTwips(CFont*& aFont, const TFontSpec& aFontSpec, TInt aMaxHeight)
	{
	TFontSpec fontSpec = aFontSpec;
	fontSpec.iHeight = (fontSpec.iHeight  * EAccFac) / iVZoom;
	return iWinDevice->GetNearestFontToMaxHeightInTwips(aFont, aFontSpec, (aMaxHeight * EAccFac) / iVZoom);
	}

#ifdef SYMBIAN_SUPPORT_FONT_METRICS_V1
TInt CPrintPreviewDevice::GetNearestFontInPixels(CFont*& aFont, const TFontSpec& aFontSpec)
	{
	return GetNearestFontToDesignHeightInPixels(aFont, aFontSpec);
	}
#endif

TInt CPrintPreviewDevice::GetNearestFontToDesignHeightInPixels(CFont*& aFont, const TFontSpec& aFontSpec)
	{
	// doesn't carry out any zooming
	// TFontSpec fontSpec = aFontSpec;
	// fontSpec.iHeight = ( fontSpec.iHeight * iVZoom )  /EAccFac;
	return iWinDevice->GetNearestFontToDesignHeightInPixels(aFont, aFontSpec);
	}

TInt CPrintPreviewDevice::GetNearestFontToMaxHeightInPixels(CFont*& aFont, const TFontSpec& aFontSpec, TInt aMaxHeight)
	{
	// doesn't carry out any zooming
	// TFontSpec fontSpec = aFontSpec;
	// fontSpec.iHeight = ( fontSpec.iHeight * iVZoom )  /EAccFac;
	return iWinDevice->GetNearestFontToMaxHeightInPixels(aFont, aFontSpec, aMaxHeight);
	}

void CPrintPreviewDevice::ReleaseFont(CFont* aFont)
	{
	iWinDevice->ReleaseFont(aFont);
	}

TDisplayMode CPrintPreviewDevice::DisplayMode() const
	{
	return iWinDevice->DisplayMode();
	}

TInt CPrintPreviewDevice::NumTypefaces() const
	{
	return iWinDevice->NumTypefaces();
	}

void CPrintPreviewDevice::TypefaceSupport(TTypefaceSupport& aTypefaceSupport,TInt aTypefaceIndex) const
	{
	iWinDevice->TypefaceSupport(aTypefaceSupport,aTypefaceIndex);
	}

TInt CPrintPreviewDevice::FontHeightInTwips(TInt aTypefaceIndex,TInt aHeightIndex) const
	{
	return iWinDevice->FontHeightInTwips(aTypefaceIndex,aHeightIndex);
	}

void CPrintPreviewDevice::PaletteAttributes(TBool& aModifiable,TInt& aNumEntries) const
	{
	iWinDevice->PaletteAttributes(aModifiable,aNumEntries);
	}

void CPrintPreviewDevice::SetPalette(CPalette* aPalette)
	{
	iWinDevice->SetPalette(aPalette);
	}

TInt CPrintPreviewDevice::GetPalette(CPalette*& aPalette) const
	{
	return iWinDevice->GetPalette(aPalette);
	}

//
// CPrintPreviewGc
//


CPrintPreviewGc::CPrintPreviewGc(CWsScreenDevice* aDevice) 
	: CWindowGc(aDevice),
	iOriginOffset(TPoint(0,0))
	{}


CPrintPreviewGc::~CPrintPreviewGc()
	{}


void CPrintPreviewGc::SetOriginOffset(const TPoint& aOffset)
	{
	iOriginOffset = aOffset;
	SetOrigin(TPoint(0,0));
	}


void CPrintPreviewGc::SetOrigin(const TPoint& aPos)
	{
	CWindowGc::SetOrigin(aPos+iOriginOffset);
	}


void CPrintPreviewGc::SetBandRect(const TRect& aBandRect)
	{
	iBandRect = aBandRect;
	CWindowGc::SetClippingRect(iBandRect);
	}


void CPrintPreviewGc::SetClippingRect(const TRect& aRect)
	{
	TRect rect(aRect);
	rect.Intersection(iBandRect);
	CWindowGc::SetClippingRect(rect);
	}


void CPrintPreviewGc::CancelClippingRect()
	{
	CWindowGc::SetClippingRect(iBandRect);
	}

