// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Tests Print APIs related to modellist interfaces,
// creation of modellists,printer devices and printing to file .\n
// 
//

/**
 @file
 @test
 @internalComponent - Internal Symbian test code
*/
#include /**/ <e32base.h> // Active object bits
#include <c32comm.h>
#include <gdi.h>
#include <prnsetup.h>
#include <fbs.h>
#include <txtetext.h>
#include <txtrich.h>
#include <flddef.h>
#include <fldbltin.h>
#include <pdrport.h>
#include <graphics/fbsdefs.h>
#include "T_FprStep.h"


#if defined(__GCC32__)
template class TBuf<40>;
template class TRefByValue<TDesC8 const>;
#endif


//! Default Page spec in Twips
#define KDefaultPageSpecInTwips TPageSpec(TPageSpec::EPortrait,TSize(11906,16838))

//! Literal for non default directory for printers
_LIT( KOtherPrinterDir, "c:\\system\\" );


// test classes

//!  A test Active Scheduler test class derived from CActiveScheduler. 
class CMyScheduler : public CActiveScheduler
	{
public:
//! Function for Error Handling
	void Error(TInt) const { return; }
	};


//!  A TMyPrintApp test class derived from MPageRegionPrinter. 
/*!
  Class is an Interface for printing in bands.\n
  Provides a single PrintBandL() function that prints the body of each page band by band.\n
*/
class TMyPrintApp : public MPageRegionPrinter
	{
public:
	TMyPrintApp(CPrintSetup* aPrintSetup);
	virtual ~TMyPrintApp();
	// from MPageRegionPrinter
	virtual void PrintBandL(CGraphicsDevice* aLayoutDevice,TInt aPageNo,const TBandAttributes& aBand);
private:
	//! Pointer to CPrintSetup class 
	CPrintSetup* iPrintAppSetup;
	};


//!  A TMyObserver test class derived from MPrintProcessObserver. 
/*!
  Print progress and status notification interface class.\n
  The notification functions are called before, during and after a 
  print or print preview operation,to give notification of its progress.\n
  The print process observer will often be the GUI's standard print progress
  or print preview dialog.\n
*/
class TMyObserver : public MPrintProcessObserver
	{
public:
	virtual void NotifyPrintStarted(TPrintParameters /*aPrintParams*/);
	virtual void NotifyBandPrinted(TInt /*aPercent*/, TInt aCurrentPageNum, TInt /*aCurrentCopyNum*/);
	virtual void NotifyPrintEnded(TInt aErrNum);
private:
	//! Member Variable for page counter
	TInt iPageCounter;
	};


//!  A TNumPagesThing test class derived from MFieldNumPagesInfo. 
/*!
  Specifies the mixin protocol for evaluating a total number of pages field.\n
  Implement the UpdateFieldNumPages() function in a concrete derived class.\n
 
*/
class TNumPagesThing : public MFieldNumPagesInfo
	{
public:
	TNumPagesThing(TInt aNum);
	TInt UpdateFieldNumPages()const;
private:
	//! Member variable for Number of Pages
	TInt iNum;
	};
	

TNumPagesThing::TNumPagesThing(TInt aNum)
	:iNum(aNum)
/**
  Constructor
 */	
	{}


TInt TNumPagesThing::UpdateFieldNumPages()const
/**
  @return - TInt (Number of pages).\n
 
  Override of base class virtual.\n
  Returns the number of pages in the current document.\n
 */
	{
	return iNum;
	}

TMyPrintApp::TMyPrintApp(CPrintSetup* aPrintSetup)
/**
  Constructor
 */
	{ 
	iPrintAppSetup=aPrintSetup;
	}

TMyPrintApp::~TMyPrintApp()
/**
  Destructor
 */
	{
		delete iPrintAppSetup;
	}

void TMyPrintApp::PrintBandL(CGraphicsDevice* aLayoutDevice,TInt aPageNo,const TBandAttributes& /*aBand*/)
/**
  Override of base class virtual\n
  Prints a band and sets up up a graphics context to which all 
  drawing should take place using CGraphicsDevice::CreateContext()\n
 */
	{
	CGraphicsContext* gc;
	aLayoutDevice->CreateContext(gc);
	CleanupStack::PushL(gc);

//	INFO_PRINTF1(_L("\n   Print started \n"));
	TInt widthInPixels = aLayoutDevice->HorizontalTwipsToPixels(iPrintAppSetup->PrinterDevice()->CurrentPageSpecInTwips().iPortraitPageSize.iWidth);
	TInt heightInPixels = aLayoutDevice->HorizontalTwipsToPixels(iPrintAppSetup->PrinterDevice()->CurrentPageSpecInTwips().iPortraitPageSize.iHeight);
    TRect rect(TSize(widthInPixels,heightInPixels)); // should be aBand...!!!
    //rect.Shrink(10,10);

	TFontSpec fontSpec(_L("Arial"),36);
	fontSpec.iFontStyle.SetPosture(EPostureItalic);
    CFont* font;
	aLayoutDevice->GetNearestFontToDesignHeightInTwips(font,fontSpec);
	gc->UseFont(font);
	TBuf<40> outputBuf;
	outputBuf.Format(_L("This is page %d (actually page %d)"),aPageNo+iPrintAppSetup->iNumOfFirstPage,aPageNo);
    TInt ascent=(rect.iBr.iY-rect.iTl.iY-font->HeightInPixels())/2 + font->AscentInPixels();
   	gc->SetUnderlineStyle(EUnderlineOn);
	gc->SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc->DrawText(outputBuf,rect,ascent,CGraphicsContext::ECenter,0);
   	gc->SetUnderlineStyle(EUnderlineOff);
//	INFO_PRINTF1(_L("\n   Print finished\n"));
	CleanupStack::PopAndDestroy(); // gc
	aLayoutDevice->ReleaseFont(font);
	}

void TMyObserver::NotifyPrintStarted(TPrintParameters /*aPrintParams*/)
/**
  Override of base class virtual.\n
  Notifies that a print or print preview operation is about to begin.\n
 */
	{
	iPageCounter = 0;
	}

void TMyObserver::NotifyBandPrinted(TInt /*aPercent*/, TInt aCurrentPageNum, TInt /*aCurrentCopyNum*/)
/**
  Override of base class virtual.\n
  Notifies that a band is about to be printed used to display print progress information,
  including the current page number.\n
  It is called immediately before each band is printed.\n
 */
	{
	if (iPageCounter==0)
		{
//		INFO_PRINTF1(_L("\n   Pages Printed: "));
		}
	if (iPageCounter<aCurrentPageNum)
		{
		iPageCounter = aCurrentPageNum;
//		INFO_PRINTF1(_L("%d.."),iPageCounter);
		}
	}

void TMyObserver::NotifyPrintEnded(TInt aErrNum)
/**
  Override of base class virtual.\n
  Notifies that the print or print preview operation has completed.\n
  used to display information about how the operation completed,
  for example any errors that occurred.\n
 */
	{
	if (aErrNum!=KErrNone)
		{
//		INFO_PRINTF2(_L("Error: %n\n"),aErrNum);
		}	
	CActiveScheduler::Stop();
	}


//
// Start of test code
//

/**
 Auxiliary Fn for Test Case ID T-FprStep-testPrintFileL

 This function is used to insert and update the number of pages field and the current page field
 in the header.\n
 Also creates the footer with current page number field and updates the same.\n
 Sets the first page, last page and number of copies parameters and starts printing to a file
 using StartPrintL API. \n

*/
void CT_FprStep::Test1L(CPrinterPort* aPort)
	{
	// create a header
	iPrintSetup->Header()->CreateTextL();
	TBuf<40> buf(_L("This is the header on page  out of "));
	buf.Append(CEditableText::EParagraphDelimiter);
	iPrintSetup->Header()->Text()->InsertL(0,buf);
	CTextField* pNumberfield=iPrintSetup->Header()->Text()->NewTextFieldL(KPageNumberFieldUid);
	iPrintSetup->Header()->Text()->InsertFieldL(27,pNumberfield,KPageNumberFieldUid);  
	CNumPagesField* numPagesField=(CNumPagesField*)iPrintSetup->Header()->Text()->NewTextFieldL(KNumPagesFieldUid);
	iPrintSetup->Header()->Text()->InsertFieldL(35,numPagesField,KNumPagesFieldUid);  
	iPrintSetup->Header()->Text()->UpdateFieldL(27);
	iPrintSetup->Header()->Text()->UpdateFieldL(35);
	iPrintSetup->Header()->SetFirstPageToPrintTo(3); // no header on first 3 pages

	iPrintSetup->Footer()->CreateTextL();
	TBuf<40> buf2(_L("This is the footer on page "));
	buf2.Append(CEditableText::EParagraphDelimiter);
	iPrintSetup->Footer()->Text()->InsertL(0,buf2);
	CTextField* pNumberfield2=iPrintSetup->Footer()->Text()->NewTextFieldL(KPageNumberFieldUid);
	iPrintSetup->Footer()->Text()->InsertFieldL(27,pNumberfield2,KPageNumberFieldUid);  
	iPrintSetup->Footer()->Text()->UpdateFieldL(27);

	// start print to file
	INFO_PRINTF1(_L("- Initiate printing to file"));
	iPrintSetup->iNumOfFirstPage = 2;
	TPrintParameters params;
	params.iFirstPage = 1; // don't print the first page at all, ie print page no's 3-6
	params.iLastPage = 4;
	params.iNumCopies = 1;
	TPageSpec spec;
	spec.iPortraitPageSize = KA4PaperSizeInTwips;
	spec.iOrientation = TPageSpec::EPortrait;
	iPrintSetup->PrinterDevice()->SelectPageSpecInTwips(spec);
	//
	//
	// start printing
	iPrintSetup->StartPrintL(params,*iRegionPrinter,aPort,iObserver);

	CActiveScheduler::Start(); // stopped by killing it - subtle...

	INFO_PRINTF1(_L("- File saved"));
	// tidy up
	}
/**
  @SYMTestCaseID UIF-FprStep-testPrintFileL
 
  @SYMPREQ 
 
  @SYMTestCaseDesc
  Tests printing to a File.
  
  @SYMTestPriority High
 
  @SYMTestStatus Implemented
 
  @SYMTestActions \n
  Adds both default and non default directories to printer driver directory using AddPrinterDriverDirL API.\n
 
  Creates a list of available printers using ModelNameListL API\n
  Creates a printer device of the first printer in the modelname list using CreatePrinterDeviceL API.\n
  Creates a printer port if necessary and sets the page margin parameters\n
  Prints to a file.\n
 
  <b> API Calls: </b>\n
  CPrintSetup::AddPrinterDriverDirL().\n
  CPrintSetup::ModelNameListL().\n
  CPrintSetup::CreatePrinterDeviceL().\n
  CPrintSetup::FreeModelList().\n
  CPrintSetup::StartPrintL().\n
 
  @SYMTestExpectedResults Print to the file should complete without any error.
  
 */

void CT_FprStep::RunTestsL()
    {
	iFs.Connect();
	INFO_PRINTF1(_L("Testing printing to file"));
	iPrintSetup = CPrintSetup::NewL();
	TNumPagesThing thing(6);
	iPrintSetup->Header()->SetNumPagesInfo(thing);
	TRAPD(ret,iPrintSetup->AddPrinterDriverDirL( KDefaultPrinterDriverPath ));
		TEST(ret==KErrNone);
	TRAP(ret,iPrintSetup->AddPrinterDriverDirL(KOtherPrinterDir));
		TEST(ret==KErrNone);
	//
	// create a model list
	CPrinterModelList* modelList=NULL;
	TRAP(ret, modelList = iPrintSetup->ModelNameListL(iFs) );
		TEST(modelList!=NULL);
	//
	//TUid KUidHP3={268435723};
	//TInt modelNum = modelList->UidToNum(KUidHP3);
	TInt modelNum = modelList->UidToNum((*modelList)[0].iUid);
		TEST(modelNum>=0);
	TRAP(ret, iPrintSetup->CreatePrinterDeviceL(modelNum) );
		TEST(ret==KErrNone);
	//
	// create a port if necessary
	CPrinterPort* port=NULL;
	if ((*modelList)[modelNum].iRequiresPrinterPort)
		port = CFilePrinterPort::NewL(_L("c:\\fpr.pcl"));
	iPrintSetup->FreeModelList();

/**	INFO_PRINTF1(_L("- Get the name of the current printer device"));
	TPrinterModelName name = iPrintSetup->PrinterDevice()->ModelName();
	INFO_PRINTF2(_L("   Name of current model: %S\n"), &name); */

	//
	iPrintSetup->PrinterDevice()->SelectPageSpecInTwips(KDefaultPageSpecInTwips);
	iPrintSetup->iPageMarginsInTwips.iHeaderOffset = 720;
	iPrintSetup->iPageMarginsInTwips.iFooterOffset = 0;
	iPrintSetup->iPageMarginsInTwips.iMargins.iLeft = 1440; // 1 inch
	iPrintSetup->iPageMarginsInTwips.iMargins.iRight = 1440;
	iPrintSetup->iPageMarginsInTwips.iMargins.iTop = 1440;
	iPrintSetup->iPageMarginsInTwips.iMargins.iBottom = 1440;
	//
	iRegionPrinter = new(ELeave) TMyPrintApp(iPrintSetup);
	iObserver = new(ELeave) TMyObserver();

	// print to file
	INFO_PRINTF1(_L("Printing to file"));
	TRAP(ret,Test1L(port));
	TEST(ret==KErrNone);
	
/*
	// print to fax (doesn't require port)
	test(_L("Printing to fax"));
	TUid KUidFax={268435731};
	TRAP(ret, iPrintSetup->CreatePrinterDeviceL(KUidFax,iFs) );
		test(ret==KErrNone);
	TRAP(ret,Test1L());
		test(ret==KErrNone);
*/
	// clean up
	INFO_PRINTF1(_L("- Destroy the CPrintSetup et al"));
	delete iObserver;
	delete iRegionPrinter;
	delete iPrintSetup;
	iFs.Close();
    }

/**
  @SYMTestCaseID UIF-FprStep-TestModelListL
 
  @SYMPREQ 
 
  @SYMTestCaseDesc
  Tests the model list interface
 
  @SYMTestPriority High
 
  @SYMTestStatus Implemented
 
  @SYMTestActions \n
  Constructs an object of CPrintSetup.\n
  Tests the interface to Pdrstore i.e; the driver already available in the default directory. \n
  Add "system/printers" directory to printer driver directory using  AddPrinterDriverDirL API\n
  Create a model list of available printers using ModelNameListL API of CPrintSetup class.\n
  Test the number of available printers to be greater than zero to check if there are more than
  one printers available in the default directory.\n
  Create a printer device of the first printer using CreatePrinterDeviceL API of CPrintSetup class\n
 
  <b> API Calls: </b> \n
 
  CPrintSetup::AddPrinterDriverDirL().\n
  CPrintSetup::ModelNameListL().\n
  CPrintSetup::CreatePrinterDeviceL().\n
  CPrintSetup::FreeModelList().\n
 
 @SYMTestExpectedResults The creation of printer device should complete with out any error.\n
 
 */
void CT_FprStep::TestModelListL()
// test the model list interface to PdrStore.
//
    {
	INFO_PRINTF1(_L("Testing the model list"));
	iFs.Connect();
	iPrintSetup = CPrintSetup::NewL();
	TRAPD(ret,iPrintSetup->AddPrinterDriverDirL( KDefaultPrinterDriverPath ));
		TEST(ret==KErrNone);
	//
	// create a model list
	CPrinterModelList* modelList=NULL;
	TRAP(ret, modelList = iPrintSetup->ModelNameListL(iFs) );
		TEST(modelList!=NULL);
		TEST(modelList->ModelCount()>0);
	//
	TEST(modelList->ModelCount()>0);
//	TUid KUidHP3={268435723};
	TInt modelNum = modelList->UidToNum((*modelList)[0].iUid);
INFO_PRINTF2 (_L("Model Number = %d"), modelNum);
		TEST(modelNum>=0);
	TRAP(ret, iPrintSetup->CreatePrinterDeviceL(modelNum) );
	if (ret!=KErrNone)
INFO_PRINTF2(_L("\nerror: %d\n"),ret);
		TEST(ret==KErrNone);
	//
	iPrintSetup->FreeModelList();
	delete iPrintSetup;
	iPrintSetup = NULL;
	}


CT_FprStep::~CT_FprStep()
/**
  Destructor
 */
	{
	}

CT_FprStep::CT_FprStep()
/**
  Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KT_FprStep);
	}

TVerdict CT_FprStep::doTestStepL()
/**
  @return - TVerdict code
  Override of base class virtual
 */
	{
	INFO_PRINTF1(_L("Test Started"));

	iScheduler = new CMyScheduler;
	TEST(iScheduler!=NULL);
//	__ASSERT_ALWAYS(iScheduler,User::Panic(_L("test"),KErrServerTerminated));
	CActiveScheduler::Install(iScheduler);
	TInt ret=KErrNone;
#if defined(__WINS__)
	ret=FbsStartup();
		TEST(ret==KErrNone);
#else
	FbsStartup();
	StartC32();
#endif
	SetTestStepID(_L("UIF-FprStep-TestModelListL"));
	__UHEAP_MARK;
	ret=RFbsSession::Connect();
		TEST(!ret); 
	TRAPD(r,CT_FprStep::TestModelListL());
	TEST(r==KErrNone);
//The memory that is allocated in AllocScanLineBuffer() is shared between all 
//bitmaps using the same session and is only released when RFbsSession::Disconnect(); is called.
	RFbsSession::Disconnect();
	__UHEAP_MARKEND;
	RecordTestResultL();

	SetTestStepID(_L("UIF-FprStep-testPrintFileL"));
	__UHEAP_MARK;
	ret=RFbsSession::Connect();
	TEST(!ret); 
	TRAP(r,CT_FprStep::RunTestsL());
	TEST(r==KErrNone);
	RFbsSession::Disconnect();
	__UHEAP_MARKEND;
	RecordTestResultL();
	CloseTMSGraphicsStep();

	delete iScheduler;
	INFO_PRINTF1(_L("Test Finished"));
	return TestStepResult();
	}

