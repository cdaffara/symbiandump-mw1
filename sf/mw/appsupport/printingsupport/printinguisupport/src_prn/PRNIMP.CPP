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
#include <e32hal.h>

#include <txtrich.h>
#include <frmtlay.h>
#include <frmprint.h>
#include "PRNIMP.H"
#include "PRNSTD.H"
#include <prnsetup.h>


//
// CHeaderFooterPrinter
//

CHeaderFooterPrinter* CHeaderFooterPrinter::NewL(CHeaderFooter& aHeaderFooter,TRect aHeaderFooterRect)
	{
	CHeaderFooterPrinter* self=new(ELeave) CHeaderFooterPrinter(aHeaderFooter,aHeaderFooterRect);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


CHeaderFooterPrinter::CHeaderFooterPrinter(CHeaderFooter& aHeaderFooter,TRect aHeaderFooterRect)
	: iHeaderFooter(&aHeaderFooter),
	iHdFtRect(aHeaderFooterRect)
	{}


void CHeaderFooterPrinter::ConstructL()
	{
	iTextLayout = CTextLayout::NewL(iHeaderFooter->Text(),iHdFtRect.Width());
	}


CHeaderFooterPrinter::~CHeaderFooterPrinter()
	{
	delete iTextLayout;
	}


void CHeaderFooterPrinter::PrintBandL(CGraphicsDevice* aDevice,TInt aPageNo,const TBandAttributes& aBandInPixels)
	{
	// Does the band intersect my bit?
	if (aBandInPixels.iRect.Intersects(iHdFtRect))
		{// If so print all the text...
		if (aPageNo>=iHeaderFooter->FirstPageToPrintTo())
			{// only print first page if we're meant to
			iHeaderFooter->Text()->UpdateAllFieldsL(); // make sure page nums etc. are correct
			DrawTextL(aDevice);
			}
		}
	}


void CHeaderFooterPrinter::DrawTextL(CGraphicsDevice* aDevice)
	{
	// set the device
	iTextLayout->SetImageDeviceMap(aDevice);
	// create a gc with which to draw
	CGraphicsContext* gc;
	User::LeaveIfError(aDevice->CreateContext(gc));
	CleanupStack::PushL(gc);
	// set clipping region in gc??
	// create a layout context
	TDrawTextLayoutContext context;
	context.SetGc(gc);
	context.iViewRect = iHdFtRect;
	context.iTextStartX=0;
	context.iGutterMarginWidth=0;
	// draw the text
	iTextLayout->FormatBandL();
	iTextLayout->DrawL(iHdFtRect,&context);
	CleanupStack::PopAndDestroy(); // gc
	}


//
// CPreviewProcess
//

CPreviewProcess* CPreviewProcess::NewL(TInt aPriority)
	{
	CPreviewProcess* self=new(ELeave) CPreviewProcess(aPriority);
	CActiveScheduler::Add(self); // Adds itself to the active scheduler
	return self;
	}


CPreviewProcess::CPreviewProcess(TInt aPriority)
	:CActive(aPriority),
	iBandAttribs(),
	iNumBands(1)
	{
	__DECLARE_NAME(_S("CPreviewProcess"));	
	}


CPreviewProcess::~CPreviewProcess()
	{
	Cancel();
	if (iOwnerReferencePtr)
		*iOwnerReferencePtr = NULL;
	delete iHeaderPrinter;
	delete iFooterPrinter;
	}


void CPreviewProcess::RunL()
	// Called by active scheduler.
	// Previews one page of a document at a time.
	//
	{
	// check RequestStatus for error codes... can't be KRequestPending
	if (iStatus != KErrNone)
		EndProcess(iStatus.Int());
	else if (IncrementPrintState())
		SetActive();
	}


void CPreviewProcess::SetObserver(MPrintProcessObserver* anObserver)
	// Sets the observer and gives it a handle to the process
	//
	{
	__ASSERT_DEBUG((anObserver != NULL),Panic(EPanicNoPrintObserver));
	iObserver = anObserver;
	}


void CPreviewProcess::SetHeaderFooter(CHeaderFooterPrinter* aHeaderPrinter,CHeaderFooterPrinter* aFooterPrinter)
	{
	iHeaderPrinter = aHeaderPrinter;
	iFooterPrinter = aFooterPrinter;
	}


void CPreviewProcess::StartPreview(const TPrintParameters& aPrintParams,CGraphicsDevice& aPrinterDev,  
								MPageRegionPrinter& aBodyPrinter,const TPageSpec& aPageSpec,TInt aNumBands,void** aRef)
	{
	iNumBands = aNumBands;
	iPageSpec = aPageSpec;
	iPreviewDev = &aPrinterDev;
	iCurrentBandNum = 1;
	Init(aPrintParams,aBodyPrinter,aRef);
	// Start the print
	SetActive();
	SetRequestStatus(); // restart self
	}


void CPreviewProcess::Init(const TPrintParameters& aPrintParams,MPageRegionPrinter& aBodyPrinter,void** aRef)
	// Called to initialize the process
	//
	{
	__ASSERT_ALWAYS((aPrintParams.iFirstPage >= 0),Panic(EPanicInvalidPageNumbers));
	__ASSERT_ALWAYS((aPrintParams.iLastPage >= aPrintParams.iFirstPage),Panic(EPanicInvalidPageNumbers));
	__ASSERT_ALWAYS((aPrintParams.iNumCopies > 0),Panic(EPanicInvalidNumCopies));
	__ASSERT_ALWAYS((aRef != NULL),Panic(EPanicProcessHandleNull));

	// do initialisation stuff	
	iPrintParams = aPrintParams;
	iBodyPrinter = &aBodyPrinter;
	iOwnerReferencePtr = aRef;
	iCurrentPageNum = iPrintParams.iFirstPage;
	// inform observer
	if (iObserver)
		iObserver->NotifyPrintStarted(iPrintParams);
	}


void CPreviewProcess::SetRequestStatus()
	// Called to set the process ready to receive more requests.
	//
	{
	iStatus = KRequestPending;
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status,KErrNone);
	}


void CPreviewProcess::DoCancel()
	// Called if print job is canceled externally
	//
	{}


void CPreviewProcess::InformObserver()
	// Called to inform observer of current print status 
	// (including percentage of page completed)
	//
	{
	if (iObserver)
		iObserver->NotifyBandPrinted(100,iCurrentPageNum,1); // 100% complete, copy no. 1
	}


void CPreviewProcess::TerminatePrint(TInt aErrNum)
	{
	EndProcess(aErrNum);
	}


void CPreviewProcess::EndProcess(TInt aErrNum)
	// Called on completion of print job
	//
	{
	if (!(iFlags&EEndOfProcessAlreadyActivated))
		{
		iFlags |= EEndOfProcessAlreadyActivated;
		// If observer exists inform that print job has completed, (or that an error has occured)
		if (iObserver)
			iObserver->NotifyPrintEnded(aErrNum);
		// then (in d'tor) set CPrinterSetup's PrintProcess* handle to null and destroy PrintProcess 
		delete this;
		}
	}


TBool CPreviewProcess::IncrementPrintState()
	// Called to print a band
	// if more is returned true then IncrementPrintState() is called again
	//
	{
	SetBandRect();
	TBool more = ETrue; // assume there is more printing to be done
	TRAPD(ret,PrintBandL(iPreviewDev));
	if (ret!=KErrNone)
		{
		more = EFalse;
		EndProcess(ret);
		return more;
		}
	iCurrentBandNum++;
	if (iCurrentBandNum>iNumBands)
		{
		iCurrentBandNum = 1;
		iCurrentPageNum++;
		}
	if (iCurrentPageNum>iPrintParams.iLastPage)
		{
		more = EFalse;
		EndProcess(); // destroys PrintProcess
		}
	if (more)
		SetRequestStatus(); // restart self
	return more;
	}


void CPreviewProcess::SetBandRect()
	{
	// is it the 1st band
	if (iCurrentBandNum==1)
		iBandAttribs.iFirstBandOnPage = ETrue;
	else
		iBandAttribs.iFirstBandOnPage = EFalse;
	// is it the last band?
	if (iCurrentBandNum==iNumBands)
		iBandAttribs.iTextIsIgnored = ETrue; // pass the fact that it's the last band on the sly...
	else 
		iBandAttribs.iTextIsIgnored = EFalse;
	// set the band to the correct size
	iBandAttribs.iRect = TRect(iPageSpec.OrientedPageSize());
	iBandAttribs.iRect.iTl.iY += (iPageSpec.OrientedPageSize().iHeight*(iCurrentBandNum-1))/iNumBands;
	iBandAttribs.iRect.iBr.iY -= (iPageSpec.OrientedPageSize().iHeight*(iNumBands-iCurrentBandNum))/iNumBands;
	//
	// convert the band to pixels
	iBandAttribs.iRect.iTl.iX = iPreviewDev->HorizontalTwipsToPixels(iBandAttribs.iRect.iTl.iX);
	iBandAttribs.iRect.iBr.iX = iPreviewDev->HorizontalTwipsToPixels(iBandAttribs.iRect.iBr.iX);
	iBandAttribs.iRect.iTl.iY = iPreviewDev->VerticalTwipsToPixels(iBandAttribs.iRect.iTl.iY);
	iBandAttribs.iRect.iBr.iY = iPreviewDev->VerticalTwipsToPixels(iBandAttribs.iRect.iBr.iY);
	}


void CPreviewProcess::PrintBandL(CGraphicsDevice* aDev)
	// Carries out the actual printing of the band
	//
	{
	iBodyPrinter->PrintBandL(aDev,iCurrentPageNum,iBandAttribs);

	if (iHeaderPrinter)
		iHeaderPrinter->PrintBandL(aDev,iCurrentPageNum,iBandAttribs);	
	
	if (iFooterPrinter)
		iFooterPrinter->PrintBandL(aDev,iCurrentPageNum,iBandAttribs);	
	} 


TInt CPreviewProcess::CurrentPageNum()const
	{
	return iCurrentPageNum;
	}


//
// CPrintProcess 
//

CPrintProcess* CPrintProcess::NewL(TInt aPriority)
	{
	CPrintProcess* self=new(ELeave) CPrintProcess(aPriority);
	CActiveScheduler::Add(self); // Adds itself to the active scheduler
	return self;
	}


CPrintProcess::CPrintProcess(TInt aPriority)
	:CPreviewProcess(aPriority)
	{}


CPrintProcess::~CPrintProcess()
	{
	Cancel();
	delete iPrinterUi;
	if (iPrinterDev)
		iPrinterDev->DeleteControl();
	}


void CPrintProcess::StartPrintL(const TPrintParameters& aPrintParams,CPrinterDevice& aPrinterDev,
										 CPrinterDriverUI* aPrinterUi,MPageRegionPrinter& aBodyPrinter,
										 CPrinterPort* aPort,void** aRef)
	// Called to initiate printing of a document
	//
	{
	__ASSERT_ALWAYS(iPrinterDev==NULL,Panic(EPanicStartCalledTwice));

	iPrinterDev = &aPrinterDev;
	iPrinterUi = aPrinterUi;
	iCurrentCopyNum = 1;
	iAreaPrinted = 0;
	iPrintStatus = EStartingBand;
	Init(aPrintParams,aBodyPrinter,aRef);
	iPrinterDev->CreateControlL(aPort); // leaves if no printer drivers exist
	//
	// set up the UI if there is one
	TBool cont=ETrue;
	if (iPrinterUi)
		cont = iPrinterUi->BeforePrintL();
	if (!cont)
		User::Leave(KErrCancel);
	//
	// Start the print
	SetActive();
	SetRequestStatus(); // restart self
	}


TBool CPrintProcess::IncrementPrintState()
	// Called to print a band
	// if more is returned true then IncrementPrintState() is called again
	//
	{
	TBool more = ETrue; // assume there is more printing to be done

	switch (iPrintStatus)
		{
		case EStartingBand:	 
			// get the band size and find out whether it's the last band on the page
			iPrintStatus = EPrintingBand;
			iMoreOnPage = iPrinterDev->iControl->QueueGetBand(iStatus,iBandAttribs);
			break;

		case EPrintingBand:	
			// print the band
			iPrintStatus = EStartingBand; // by default assume there's more 
			TRAPD(ret, PrintBandL(iPrinterDev));
			if (ret == KErrMorePages)
				{
				while(ret == KErrMorePages && ++iCurrentPageNum <= iPrintParams.iLastPage)
			        {
					TRAP(ret, PrintBandL(iPrinterDev));
					}
				}
			if (ret!=KErrNone)
				{
				//HandlePrinterError(CPrinterDevice::EErrPrintBandLeave); // change to KErrXxx
				more = EFalse; 
				EndProcess(ret);
				}
			else
				{// inform observer of what we've just printed
				InformObserver();
				iCurrentBandNum++;
				// if more bands to print do the next one
				if (iMoreOnPage!=CPrinterControl::EMoreOnPage)
					// End of page has been passed
					iPrintStatus = EEndPage;
				SetRequestStatus(); // restart self
				}
			break;

		case EEndPage:
			// 	increment page number
			iCurrentBandNum=1;
			iCurrentPageNum++;
			iAreaPrinted = 0; // reset
			// Have we completed a copy?
			if (iCurrentPageNum <= iPrintParams.iLastPage)
				{// Print the next page if there are more
				iPrintStatus = EStartingBand;
				SetRequestStatus();
				}
			else
				{// increment copy number and reset
				iCurrentCopyNum++;
				iCurrentPageNum = iPrintParams.iFirstPage;
				// Are there more copies to print?
				if (iCurrentCopyNum <= iPrintParams.iNumCopies)
					{
					iPrintStatus = EStartingBand;
					SetRequestStatus();
					}
				else
					{
					// Tell device we've finished printing
					iPrintStatus = EEndPrint;
					iPrinterDev->iControl->QueueEndPrint(iStatus);
					}
				}
			break;

		case EEndPrint:
			//	tell observer we've finished.
			more = EFalse;
			EndProcess(); // destroys PrintProcess
			break;

		}
	return more;
	}
	

void CPrintProcess::PrintBandL(CGraphicsDevice* aDev)
	// Carries out the actual printing of the band
	//
	{
	if (iHeaderPrinter)
		iHeaderPrinter->PrintBandL(aDev,iCurrentPageNum,iBandAttribs);	

	iBodyPrinter->PrintBandL(aDev,iCurrentPageNum,iBandAttribs);
	
	if (iFooterPrinter)
		iFooterPrinter->PrintBandL(aDev,iCurrentPageNum,iBandAttribs);	
	} 


void CPrintProcess::InformObserver()
	// Called to inform observer of current print status 
	// (including percentage of page completed)
	//
	{
	if (iObserver)
		{
		TInt percentCompleted = (100*iCurrentBandNum)/iPrinterDev->iControl->BandsPerPage();
		iObserver->NotifyBandPrinted(percentCompleted,iCurrentPageNum,iCurrentCopyNum);
		}
	}


void CPrintProcess::DoCancel()
	// Called if print job is canceled externally
	//
	{
	if (iPrintStatus==EPrintingBand || iPrintStatus==EEndPrint)
		// the control has my status word at the moment, so call cancel on him. He will complete my request
		iPrinterDev->iControl->AbortPrint();
	}


void CPrintProcess::EndProcess(TInt aErrNum)
	{
	if (iPrinterUi && aErrNum==KErrNone)
		{
		TRAP_IGNORE(iPrinterUi->AfterPrintL());
		}
	CPreviewProcess::EndProcess(aErrNum);
	}
