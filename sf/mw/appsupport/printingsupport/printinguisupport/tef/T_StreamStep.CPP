// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Test CPrintSetup Streaming conversions.\n
// Test Print APIs related to streaming of PrintSetup info,storing and restore of footer
// and headers and current print model, picture persistance.\n
// 
//

/**
 @file
 @internalComponent - Internal Symbian test code
*/

#include <s32mem.h>
#include <c32comm.h>
#include <txtfmlyr.h>
#include <prnsetup.h>
#include /**/ <fbs.h>
#include <pdrport.h>
#include <graphics/fbsdefs.h>
#include "T_StreamStep.h"


//const TInt KTestCleanupStack=0x20;
//const TInt KTestExpandSize=0x20;

//! Constant for Printer Uid
const TUid KUid={1};

/**
 Auxiliary Fn for Test Case ID T-StreamStep-TestPrintSetupL,
 T-StreamStep-TestModelRestoreL,
 T-StreamStep-TestHeaderFooterL.\n

 This function tests Picture persistance.\n
 Copies the aOriginal CPrintSetup object to the store and restores to the aCopy object\n
*/
void CT_StreamStep::TestStoreRestoreL(CPrintSetup* aCopy, CPrintSetup* aOriginal)
    {
	// set up the store
	RFile theFile;
	TInt ret=theFile.Replace(iFs,_L("c:\\tprint.tst"),EFileRead|EFileWrite);
	 	TEST(ret==KErrNone);
	CFileStore* theStore;
	theStore=CDirectFileStore::NewLC(theFile);  // Creates concrete store over the file.
	theStore->SetTypeL(TUidType(KDirectFileStoreLayoutUid,KNullUid,KNullUid));
	//
	// store the original
	TStreamId id=0;
	TRAP(ret,id=aOriginal->StoreL(*theStore));
		TEST(ret==KErrNone);
	//
	// restore into the copy
	TRAP(ret,aCopy->RestoreL(*theStore,id));
		TEST(ret==KErrNone);
	//
	// tidy up
	CleanupStack::PopAndDestroy();  // theStore
	iFs.Delete(_L("c:\\tprint.tst"));
    }

/**
 @return - TBool.\n

 Auxiliary Fn for Test Case ID T-StreamStep-TestPrintSetupL,
 T-StreamStep-TestModelRestoreL,
 T-StreamStep-TestHeaderFooterL.\n
 This is used as part of testing the validity of data in the store.\n
 This function is used to compare the number of the first page on the header and footer.\n
 This function compares two boolean values.\n
 Returns True if bools are equal, false otherwise\n
*/
TBool CT_StreamStep::EquateBools(TBool aFirstBool, TBool aSecondBool)
	{
	return ((aFirstBool&&aSecondBool)||(!aFirstBool&&!aSecondBool));
	}
/**
* @return - TInt.\n

 Auxiliary Fn for Test Case ID T-StreamStep-TestPrintSetupL,
 T-StreamStep-TestModelRestoreL,
 T-StreamStep-TestHeaderFooterL.\n

 This function compares two CPrintSetup objects,aCopy and anOriginal.\n
 Returns true if aCopy contents matches anOriginal contents.\n
 The parameters compared are 
 Uid, Orientation,PortraitPageSize,PageMargins etc. \n

*/
TInt CT_StreamStep::IsEqual(const CPrintSetup* aCopy,const CPrintSetup* anOriginal)
	{
	INFO_PRINTF1(_L("--  Comparing..."));
	TEST(anOriginal->PrinterDevice()->Model().iUid==aCopy->PrinterDevice()->Model().iUid);
	TEST(anOriginal->PrinterDevice()->CurrentPageSpecInTwips().iOrientation==aCopy->PrinterDevice()->CurrentPageSpecInTwips().iOrientation);
	TEST(anOriginal->PrinterDevice()->CurrentPageSpecInTwips().iPortraitPageSize.iWidth==aCopy->PrinterDevice()->CurrentPageSpecInTwips().iPortraitPageSize.iWidth);
	TEST(anOriginal->PrinterDevice()->CurrentPageSpecInTwips().iPortraitPageSize.iHeight==aCopy->PrinterDevice()->CurrentPageSpecInTwips().iPortraitPageSize.iHeight);
	TEST(anOriginal->iPageMarginsInTwips.iMargins.iLeft==aCopy->iPageMarginsInTwips.iMargins.iLeft);
	TEST(anOriginal->iPageMarginsInTwips.iMargins.iRight==aCopy->iPageMarginsInTwips.iMargins.iRight);
	TEST(anOriginal->iPageMarginsInTwips.iMargins.iTop==aCopy->iPageMarginsInTwips.iMargins.iTop);
	TEST(anOriginal->iPageMarginsInTwips.iMargins.iBottom==aCopy->iPageMarginsInTwips.iMargins.iBottom);
	TEST(anOriginal->iPageMarginsInTwips.iHeaderOffset==aCopy->iPageMarginsInTwips.iHeaderOffset);
	TEST(anOriginal->iPageMarginsInTwips.iFooterOffset==aCopy->iPageMarginsInTwips.iFooterOffset);
	TEST(EquateBools(anOriginal->Header()->FirstPageToPrintTo(),aCopy->Header()->FirstPageToPrintTo()));
	TEST(EquateBools(anOriginal->Footer()->FirstPageToPrintTo(),aCopy->Footer()->FirstPageToPrintTo()));
	TEST(anOriginal->iNumOfFirstPage==aCopy->iNumOfFirstPage);
	return 1;
	}

/**
  @SYMTestCaseID UIF-StreamStep-TestPrintSetupL
 
  @SYMPREQ
 
  @SYMTestCaseDesc Tests streaming of PrintSetup info.
                Stream from one copy to another, then compare\n
  
  @SYMTestPriority High 
 
  @SYMTestStatus Implemented
  
  @SYMTestActions \n
  The test creates two CPrintSetup objects,namely original and copy.\n
  Adds "system/printers" directory to printer directory of both objects.\n
  Creates a printer device for the first printer Uid ONE.\n
  Configures orientation, portraitpage size, pagemargins etc for the original CPrintSetup object.\n
  Performs copying of original and copy CPrintSetup objects using CFileStore.\n
  <b> API Calls: </b>\n	
  CPrintSetup::AddPrinterDriverDirL(const TDesC& aDriverDir)\n
  CPrintSetup::PrinterDevice()->SelectPageSpecInTwips()\n
  CPrintSetup::Header()->SetFirstPageToPrintTo()\n
  CPrintSetup::Footer()->SetFirstPageToPrintTo()\n
  CPrintSetup::CreatePrinterDeviceL(TUid aModelUid,RFs& aFs)\n
  
  @SYMTestExpectedResults The two CPrintSetup objects,original and copy should be similar.\n
   
 */
void CT_StreamStep::TestPrintSetupL()
	{
	// create the PrintSetups
	INFO_PRINTF1(_L("Streaming CPrintSetup"));
	CPrintSetup* original = CPrintSetup::NewL();
	CPrintSetup* copy = CPrintSetup::NewL();
	TRAPD(ret,original->AddPrinterDriverDirL( KDefaultPrinterDriverPath ));
		TEST(ret==KErrNone);
	TRAP(ret,copy->AddPrinterDriverDirL( KDefaultPrinterDriverPath ));
		TEST(ret==KErrNone);

	// change data in original and test stream
	TRAP(ret,original->CreatePrinterDeviceL(KUid,iFs));
		TEST(ret==KErrNone);
	TPageSpec spec;
	spec.iOrientation = TPageSpec::ELandscape;
	spec.iPortraitPageSize = TSize(100,100);
	original->PrinterDevice()->SelectPageSpecInTwips(spec);
	original->iPageMarginsInTwips.iMargins.iLeft = 32;
	original->iPageMarginsInTwips.iHeaderOffset = 250;
	original->Header()->SetFirstPageToPrintTo(0);
	original->Footer()->SetFirstPageToPrintTo(2); 
	original->iNumOfFirstPage = 3;

	TestStoreRestoreL(copy,original);
	TEST(IsEqual(copy,original));

	// end
	delete original;
	delete copy;
	}

/**
  @SYMTestCaseID UIF-StreamStep-TestModelRestoreL
 
  @SYMPREQ 
 
  @SYMTestCaseDesc Tests storing and restoring of the current printer model in CPrintSetup objects  correctly.
 
  @SYMTestPriority High 
 
  @SYMTestStatus Implemented
  
  @SYMTestActions \n
  The test creates two CPrintSetup objects,original and copy.\n
  Adds "system/printers" directory to printer directory of both objects/\n
  Configures Modellist for original using API ModelNameListL and creates a printer
  device for the first printer available in the  modellist.\n
  Performs copying of original and copy CPrintSetup objects using CFileStore.\n
  <b> API Calls: </b>\n	
  CPrintSetup::AddPrinterDriverDirL(const TDesC& aDriverDir)\n
  CPrintSetup::ModelNameListL(RFs& aFs)\n
  CPrintSetup::CreatePrinterDeviceL(TUid aModelUid,RFs& aFs)\n
  CPrintSetup::FreeModelList()\n
  
  @SYMTestExpectedResults The two CPrintSetup objects should be similar.\n
 */
void CT_StreamStep::TestModelRestoreL()
 	{
	// Create a printer in the original
	INFO_PRINTF1(_L("test streaming of the current model"));
	CPrintSetup* original = CPrintSetup::NewL();
	CPrintSetup* copy = CPrintSetup::NewL();
	TRAPD(ret,original->AddPrinterDriverDirL( KDefaultPrinterDriverPath ));
		TEST(ret==KErrNone);
	TRAP(ret,copy->AddPrinterDriverDirL( KDefaultPrinterDriverPath ));
		TEST(ret==KErrNone);
	//
	CPrinterModelList* modelList=NULL;
	TRAP(ret,modelList = original->ModelNameListL(iFs)); // not trapped because it calls NewLC - cleanup stack Panic!!
	TEST(modelList!=NULL);
		TEST(modelList->ModelCount()>0);
	TRAP(ret,original->CreatePrinterDeviceL((*modelList)[0].iUid,iFs));
		TEST(ret==KErrNone);
	original->FreeModelList();
	// stream
	TestStoreRestoreL(copy,original);
	TEST(IsEqual(copy,original));
	// tidy up
	delete original;
	delete copy;
	}

/**
 
  @SYMTestCaseID UIF-StreamStep-TestHeaderFooterL
 
  @SYMPREQ 
 
  @SYMTestCaseDesc Tests storing and restoring of CPrintSetup objects when header and footer text exists.
  
  @SYMTestPriority High 
 
  @SYMTestStatus Implemented
 
  @SYMTestActions\n
  The test creates two CPrintSetup objects,original and copy. \n
  Adds only the default printer directory "system/printers" to printer directory of both objects.\n
  Configures Header and Footer fields for original CPrintSetup object 
  and creates a printer device with Uid ONE.\n
  Writes the original CPrintSetup object to the Store .\n
  Constructs a second CPrintSetup object "copy" from the Store.\n
  Compares the original and the copy CPrintSetup objects.\n
  <b> API Calls: </b>\n	
  CPrintSetup::AddPrinterDriverDirL(const TDesC& aDriverDir)\n
  CPrintSetup::CreatePrinterDeviceL(TUid aModelUid,RFs& aFs)\n
  CPrintSetup::Header()->CreateTextL()\n
  CPrintSetup::Footer()->CreateTextL()\n
  CPrintSetup::FreeModelList()\n
  
  @SYMTestExpectedResults The two CPrintSetup objects should be similar.\n
   
 */
void CT_StreamStep::TestHeaderFooterL()
	{
	// Create the original
	INFO_PRINTF1(_L("test streaming with headers and footers"));
	CPrintSetup* original = CPrintSetup::NewL();
	CPrintSetup* copy = CPrintSetup::NewL();
	TRAPD(ret,original->AddPrinterDriverDirL( KDefaultPrinterDriverPath ));
		TEST(ret==KErrNone);
	TRAP(ret,copy->AddPrinterDriverDirL( KDefaultPrinterDriverPath ));
		TEST(ret==KErrNone);
	//
	TRAP(ret,original->CreatePrinterDeviceL(KUid,iFs));
		TEST(ret==KErrNone);
	// create a header and a footer
	original->Header()->CreateTextL();
	original->Footer()->CreateTextL();
	// stream
	TestStoreRestoreL(copy,original);
	TEST(IsEqual(copy,original));
	// tidy up
	delete original;
	delete copy;
	}

CT_StreamStep::~CT_StreamStep()
/**
  Destructor
 */
	{
	}

CT_StreamStep::CT_StreamStep()
/**
  Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KT_StreamStep);
	}

TVerdict CT_StreamStep::doTestStepL()
/**
  @return - TVerdict code.\n
 
  Override of base class virtual.\n
  Tests streaming conversions.\n
 */
    {
	INFO_PRINTF1(_L("Test Started"));

	iFs.Connect();
	iScheduler = new CActiveScheduler;
	__ASSERT_ALWAYS(iScheduler,User::Panic(_L("test"),KErrServerTerminated));
	CActiveScheduler::Install(iScheduler);
	TInt ret=0;
#if defined(__WINS__)
	ret=FbsStartup();
		TEST(ret==KErrNone);
#else
	FbsStartup();
	StartC32();
#endif
	ret=RFbsSession::Connect();
	TEST(!ret);

	__UHEAP_MARK;

	SetTestStepID(_L("UIF-StreamStep-TestPrintSetupL"));
	TRAPD(r,TestPrintSetupL());
	TEST(r==KErrNone);
	RecordTestResultL();

	SetTestStepID(_L("UIF-StreamStep-TestModelRestoreL"));
	TRAP(r,TestModelRestoreL());
	TEST(r==KErrNone);
	RecordTestResultL();

	SetTestStepID(_L("UIF-StreamStep-TestHeaderFooterL"));
	TRAP(r,TestHeaderFooterL());
	TEST(r==KErrNone);
	RecordTestResultL();

//	Do more tests here...

	CloseTMSGraphicsStep();

	__UHEAP_MARKEND;

	delete iScheduler;
	INFO_PRINTF1(_L("Test Finished"));
	return TestStepResult();
	}

