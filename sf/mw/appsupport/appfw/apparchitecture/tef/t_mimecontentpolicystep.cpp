// Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
 @test
 @internalComponent - Internal Symbian test code  
*/

#include "t_mimecontentpolicystep.h"
#include <centralrepository.h>
#include <apmstd.h>

//Closed content and extension information repository UID
const TUid KClosedContentAndExtensionInfoRepositoryUID={0x10003A3F};

_LIT(KPathjpg1, "z:\\system\\data\\type-r.jpg");
_LIT(KPathjpg2, "z:\\system\\data\\propelli.jpg");
_LIT(KPathdcf,  "z:\\system\\data\\sd_goo.dcf");
_LIT(KPathdm1,  "z:\\system\\data\\fl_goo.dm");
_LIT(KPathgif,  "z:\\system\\data\\gif_wallpaper.gif");
_LIT(KPathdm2,  "z:\\system\\data\\jpeg_wes.dm");

/**
   Constructor
 */	
CT_MimeContentPolicyStep::CT_MimeContentPolicyStep()
	{
	// Call base class method to set up the human readable name for logging*/
	SetTestStepName(KT_MimeContentPolicyStep);
	}

/**
   Destructor
 */
CT_MimeContentPolicyStep::~CT_MimeContentPolicyStep()
	{
	delete iCcp;
	iApaLsSession.Close();
	iFs.Close();
	}

TVerdict CT_MimeContentPolicyStep::doTestStepL()
	{
	INFO_PRINTF1(_L("Testing Apparc...T_MimeContentPolicyStep Test Cases Running..."));
	TInt error = iApaLsSession.Connect();
	TEST(error == KErrNone);

	error = iFs.Connect();
	TEST(error == KErrNone);
	
	iCcp = CApfMimeContentPolicy::NewL();
	
	User::LeaveIfError(iFs.ShareProtected());

	TRAPD(ret,RunTestCasesL())
	TEST(ret==KErrNone);
	
	INFO_PRINTF1(_L("...T_MimeContentPolicyStep Test Cases Completed."));
	return TestStepResult();
	}

void CT_MimeContentPolicyStep::RunTestCasesL()
	{
	HEAP_TEST_LS_SESSION(iApaLsSession, 0, 0, CCPTestIsClosedType(), NO_CLEANUP);
	HEAP_TEST_LS_SESSION(iApaLsSession, 0, 0, CCPTestIsClosedExtension(), NO_CLEANUP);
	HEAP_TEST_LS_SESSION(iApaLsSession, 0, 0, CCPTestIsDRMEnvelopeFileNameL(), NO_CLEANUP);
	HEAP_TEST_LS_SESSION(iApaLsSession, 0, 0, CCPTestIsClosedFileFileNameL(), iApaLsSession.FlushRecognitionCache());
	HEAP_TEST_LS_SESSION(iApaLsSession, 0, 0, CCPTestIsDRMEnvelopeFileHandleL(), NO_CLEANUP);
	HEAP_TEST_LS_SESSION(iApaLsSession, 0, 0, CCPTestIsClosedFileFileHandleL(), iApaLsSession.FlushRecognitionCache());
	HEAP_TEST_LS_SESSION(iApaLsSession, 0, 0, CCPOOMTestL(), iApaLsSession.FlushRecognitionCache());
	HEAP_TEST_LS_SESSION(iApaLsSession, 0, 0, CCPTestIsClosedContentAndExtenstionInfoRepositoryReadOnlyL(), NO_CLEANUP);
	}

/**
   @SYMTestCaseID APPFWK-APPARC-0077

   @SYMREQ REQ7736
 
   @SYMTestCaseDesc Tests IsClosedType() method for different mime types
  
   @SYMTestPriority High 
 
   @SYMTestStatus Implemented
  
   @SYMTestActions Closed types are the mime types which are listed in the 10003a3f repository file.
   Calls CApfMimeContentPolicy::IsClosedType(const TDesC& aMimeType); for different closed and non-closed mime types.
   And also it tests whether invalid mime types are not added to the list.
  
   @SYMTestExpectedResults The test checks whether IsClosedType returns ETrue for the Closed Mime types and EFalse for non-closed Mime types
 */
void CT_MimeContentPolicyStep::CCPTestIsClosedType()
	{
	_LIT(KMimeType0, "application/vnd.oma.drm.message");
	_LIT(KMimeType1, "application/vnd.oma.drm.rights+xml");
	_LIT(KMimeType2, "application/vnd.oma.drm.rights+wbxml");
	_LIT(KMimeType3, "application/vnd.nokia.ringing-tone");
	_LIT(KMimeType4, "audio/amr-wb");
	_LIT(KMimeType5, "audio/sp-midi");
	_LIT(KMimeType6, "image/vnd.nok.3Dscreensaver");
	_LIT(KMimeType7, "image/vnd.nok-wallpaper");
	_LIT(KMimeType8, "image/vnd.nok-oplogo");
	_LIT(KMimeType9, "image/vnd.nok-oplogo-color");
	_LIT(KMimeType10, "application/java");
	_LIT(KMimeType11, "application/java-archive");
	_LIT(KMimeType12, "application/x-java-archive");
	_LIT(KMimeType13, "text/vnd.sun.j2me.app-descriptor");
	_LIT(KMimeType14, "application/x-NokiaGameData");
	_LIT(KMimeType15, "application/vnd.symbian.install");
	_LIT(KMimeType16, "x-epoc/x-sisx-app"); 

	_LIT(KMimeType17, "image/tiff");
	_LIT(KMimeType18, "application/javascript");
	_LIT(KMimeType19, "video/3gpp");
	_LIT(KMimeType20, "video/mpeg");
	_LIT(KMimeType21, "video/quicktime");
	_LIT(KMimeType22, "video/mpeg4-generic");
		
    //Invalid mime types
    _LIT(KMimeType23, "/test");
    _LIT(KMimeType24, "test");
    _LIT(KMimeType25, "test/");
    _LIT(KMimeType26, "/test/");
    _LIT(KMimeType27, "test/testmime/");
    _LIT(KMimeType28, "/test/testmime");
    _LIT(KMimeType29, "test\\testmime");      

    
  
	INFO_PRINTF1(_L("Tests the MIME types found on closed content list"));
    
	TEST(iCcp->IsClosedType(KMimeType0));
	INFO_PRINTF2(_L("%S is Closed Type"), &KMimeType0);
    
	TEST(iCcp->IsClosedType(KMimeType1));
	INFO_PRINTF2(_L("%S is Closed Type"), &KMimeType1);		
	
	TEST(iCcp->IsClosedType(KMimeType2));
	INFO_PRINTF2(_L("%S is Closed Type"), &KMimeType2);
    
	TEST(iCcp->IsClosedType(KMimeType3));
	INFO_PRINTF2(_L("%S is Closed Type"), &KMimeType3);
   
	TEST(iCcp->IsClosedType(KMimeType4));
  	INFO_PRINTF2(_L("%S is Closed Type"), &KMimeType4);
   
	TEST(iCcp->IsClosedType(KMimeType5));
  	INFO_PRINTF2(_L("%S is Closed Type"), &KMimeType5);
    
	TEST(iCcp->IsClosedType(KMimeType6));
    INFO_PRINTF2(_L("%S is Closed Type"), &KMimeType6);
    
	TEST(iCcp->IsClosedType(KMimeType7));
   	INFO_PRINTF2(_L("%S is Closed Type"), &KMimeType7);
    
	TEST(iCcp->IsClosedType(KMimeType8));
   	INFO_PRINTF2(_L("%S is Closed Type"), &KMimeType8);

	TEST(iCcp->IsClosedType(KMimeType9));
  	INFO_PRINTF2(_L("%S is Closed Type"), &KMimeType9);

	TEST(iCcp->IsClosedType(KMimeType10));
  	INFO_PRINTF2(_L("%S is Closed Type"), &KMimeType10);
  	
	TEST(iCcp->IsClosedType(KMimeType11));
  	INFO_PRINTF2(_L("%S is Closed Type"), &KMimeType11);
  	
	TEST(iCcp->IsClosedType(KMimeType12));
  	INFO_PRINTF2(_L("%S is Closed Type"), &KMimeType12);

	TEST(iCcp->IsClosedType(KMimeType13));
  	INFO_PRINTF2(_L("%S is Closed Type"), &KMimeType13);

	TEST(iCcp->IsClosedType(KMimeType14));
  	INFO_PRINTF2(_L("%S is Closed Type"), &KMimeType14);

	TEST(iCcp->IsClosedType(KMimeType15));
  	INFO_PRINTF2(_L("%S is Closed Type"), &KMimeType15);

	TEST(iCcp->IsClosedType(KMimeType16));
  	INFO_PRINTF2(_L("%S is Closed Type"), &KMimeType16);
  	
  	TEST(!iCcp->IsClosedType(KMimeType17));
  	INFO_PRINTF2(_L("%S is not Closed Type"), &KMimeType17);
  	
  	TEST(!iCcp->IsClosedType(KMimeType18));
  	INFO_PRINTF2(_L("%S is not Closed Type"), &KMimeType18);
  	
  	TEST(!iCcp->IsClosedType(KMimeType19));
  	INFO_PRINTF2(_L("%S is not Closed Type"), &KMimeType19);
  	
  	TEST(!iCcp->IsClosedType(KMimeType20));
  	INFO_PRINTF2(_L("%S is not Closed Type"), &KMimeType20);
  	
  	TEST(!iCcp->IsClosedType(KMimeType21));
  	INFO_PRINTF2(_L("%S is not Closed Type"), &KMimeType21);
  	
  	TEST(!iCcp->IsClosedType(KMimeType22));
  	INFO_PRINTF2(_L("%S is not Closed Type"), &KMimeType22);
  	
    TEST(!iCcp->IsClosedType(KMimeType23));
    INFO_PRINTF2(_L("%S is not Closed Type"), &KMimeType23);
   
    TEST(!iCcp->IsClosedType(KMimeType24));
    INFO_PRINTF2(_L("%S is not Closed Type"), &KMimeType24);
  
    TEST(!iCcp->IsClosedType(KMimeType25));
    INFO_PRINTF2(_L("%S is not Closed Type"), &KMimeType25);

    TEST(!iCcp->IsClosedType(KMimeType26));
    INFO_PRINTF2(_L("%S is not Closed Type"), &KMimeType26);

    TEST(!iCcp->IsClosedType(KMimeType27));
    INFO_PRINTF2(_L("%S is not Closed Type"), &KMimeType27);
    
    TEST(!iCcp->IsClosedType(KMimeType28));
    INFO_PRINTF2(_L("%S is not Closed Type"), &KMimeType28); 
    
    TEST(!iCcp->IsClosedType(KMimeType29));
    INFO_PRINTF2(_L("%S is not Closed Type"), &KMimeType29);    
  	}

/**
   @SYMTestCaseID APPFWK-APPARC-0078

   @SYMREQ REQ7736
 
   @SYMTestCaseDesc Tests IsClosedExtension() method for various File Extensions
  
   @SYMTestPriority High 
 
   @SYMTestStatus Implemented
  
   @SYMTestActions Closed file extensions are the file extensions which are listed in the 10003a3f repository.
   Calls CApfMimeContentPolicy::IsClosedExtension(const TDesC& aFileExtension); for different closed and non-closed File Extensions.
   And also it tests whether invalid closed extensions are not added to the list.
  
   @SYMTestExpectedResults The test checks whether IsClosedExtension returns ETrue for the Closed File Extensions and EFalse for non-closed File Extensions.
 */
void CT_MimeContentPolicyStep::CCPTestIsClosedExtension()
	{
	_LIT(KExtension0,  ".dm");
	_LIT(KExtension1,  ".dr");
	_LIT(KExtension2,  ".drc");
	_LIT(KExtension3,  ".ott");
	_LIT(KExtension4,  ".awb");
	_LIT(KExtension5,  ".mid");
	_LIT(KExtension6,  ".c3d");
	_LIT(KExtension7,  ".jar");
	_LIT(KExtension8,  ".ngd");
	_LIT(KExtension9,  ".sis");
	_LIT(KExtension10,  ".sisx");
	_LIT(KExtension11, ".jpg");
	_LIT(KExtension12, "jar.mp3");
	_LIT(KExtension13, ".siss");
	_LIT(KExtension14, ".sis7");
	_LIT(KExtension15, ".0sis");
	_LIT(KExtension16, ".gif");
	
	//Invalid extension
    _LIT(KExtension17, "tst");	

    INFO_PRINTF1(_L("Tests the extensions found on closed content list"));
	
	TEST(iCcp->IsClosedExtension(KExtension0));
   	INFO_PRINTF2(_L("%S is Closed Extension"), &KExtension0);

	TEST(iCcp->IsClosedExtension(KExtension1));
   	INFO_PRINTF2(_L("%S is Closed Extension"), &KExtension1);
	
	TEST(iCcp->IsClosedExtension(KExtension2));
	INFO_PRINTF2(_L("%S is Closed Extension"), &KExtension2);

	TEST(iCcp->IsClosedExtension(KExtension3));
	INFO_PRINTF2(_L("%S is Closed Extension"), &KExtension3);
	
	TEST(iCcp->IsClosedExtension(KExtension4));
	INFO_PRINTF2(_L("%S is Closed Extension"), &KExtension4);
	
	TEST(iCcp->IsClosedExtension(KExtension5));
	INFO_PRINTF2(_L("%S is Closed Extension"), &KExtension5);
	
	TEST(iCcp->IsClosedExtension(KExtension6));
	INFO_PRINTF2(_L("%S is Closed Extension"), &KExtension6);					

	TEST(iCcp->IsClosedExtension(KExtension7));
	INFO_PRINTF2(_L("%S is Closed Extension"), &KExtension7);

	TEST(iCcp->IsClosedExtension(KExtension8));
	INFO_PRINTF2(_L("%S is Closed Extension"), &KExtension8);

	TEST(iCcp->IsClosedExtension(KExtension9));
	INFO_PRINTF2(_L("%S is Closed Extension"), &KExtension9);

	TEST(iCcp->IsClosedExtension(KExtension10));
	INFO_PRINTF2(_L("%S is Closed Extension"), &KExtension10);
	
	TEST(!iCcp->IsClosedExtension(KExtension11));
	INFO_PRINTF2(_L("%S is not Closed Extension"), &KExtension11);
	
	TEST(!iCcp->IsClosedExtension(KExtension12));
	INFO_PRINTF2(_L("%S is not Closed Extension"), &KExtension12);
	
	TEST(!iCcp->IsClosedExtension(KExtension13));
	INFO_PRINTF2(_L("%S is not Closed Extension"), &KExtension13);
	
	TEST(!iCcp->IsClosedExtension(KExtension14));
	INFO_PRINTF2(_L("%S is not Closed Extension"), &KExtension14);
	
	TEST(!iCcp->IsClosedExtension(KExtension15));
	INFO_PRINTF2(_L("%S is not Closed Extension"), &KExtension15);
	
	TEST(!iCcp->IsClosedExtension(KExtension16));
	INFO_PRINTF2(_L("%S is not Closed Extension"), &KExtension16);
	
    TEST(!iCcp->IsClosedExtension(KExtension17));
    INFO_PRINTF2(_L("%S is not Closed Extension"), &KExtension17);	
	}

/**
   @SYMTestCaseID APPFWK-APPARC-0079

   @SYMREQ REQ7736
 
   @SYMTestCaseDesc Tests IsDRMEnvelopeL() method for various files.
  
   @SYMTestPriority High 
 
   @SYMTestStatus Implemented
  
   @SYMTestActions Calls CApfMimeContentPolicy::IsDRMEnvelopeL(const TDesC& aFileName); for different not DRM Envelopes files.
     
   @SYMTestExpectedResults The test checks whether IsDRMEnvelopeL() returns EFalse for non-DRM protected Files.
 */
void CT_MimeContentPolicyStep::CCPTestIsDRMEnvelopeFileNameL()
	{
    INFO_PRINTF1(_L("Tests given files are DRM envelopes given the file name"));
      	
	TEST(!iCcp->IsDRMEnvelopeL(KPathjpg1));
	TEST(!iCcp->IsDRMEnvelopeL(KPathjpg2));
	TEST(!iCcp->IsDRMEnvelopeL(KPathdcf));
	TEST(!iCcp->IsDRMEnvelopeL(KPathdm1));
    TEST(!iCcp->IsDRMEnvelopeL(KPathgif));
	TEST(!iCcp->IsDRMEnvelopeL(KPathdm2));
	}

/**
   @SYMTestCaseID APPFWK-APPARC-0080

   @SYMREQ REQ7736
 
   @SYMTestCaseDesc Tests IsClosedFileL() method for various files.
  
   @SYMTestPriority High 
 
   @SYMTestStatus Implemented
  
   @SYMTestActions Closed files are files whose file extensions are listed in the 10003a3f repository.
   Calls CApfMimeContentPolicy::IsClosedFileL(const TDesC& aFileName); for different Closed and non-closed files.
   Calls CApfMimeContentPolicy::IsClosedFileL(const TDesC& aFileName); with file which is already open and checks whether \n
   call succeeds.
   Calls CApfMimeContentPolicy::IsClosedFileL(const TDesC& aFileName); with Null file handle\n
     
   @SYMTestExpectedResults The test checks whether 
   1. IsClosedFileL() returns EFalse for Files which are not closed and ETrue for files which are closed.  
   2. IsClosedFileL() call succeeds even if we pass a open file handle.
   3. IsClosedFileL() leaves with KErrBadHandle if a null file handle is passed.
 */
void CT_MimeContentPolicyStep::CCPTestIsClosedFileFileNameL()
	{
	INFO_PRINTF1(_L("Tests given files are Closed Files using the FileName"));
    	
	TEST(!iCcp->IsClosedFileL(KPathjpg1));
    
    // File passed to IsClosedFileL is already open.
    RFile fileHandle;
	TEST((fileHandle.Open(iFs, KPathjpg2, EFileShareAny)) == KErrNone);
	CleanupClosePushL(fileHandle);
	TEST(!iCcp->IsClosedFileL(KPathjpg2));
	CleanupStack::PopAndDestroy(&fileHandle);
    
	// Bad File Handle is passed to IsClosedFileL API
    RFile nullFileHandle;
    INFO_PRINTF1(_L("Tests IsClosedFileL method passing null file handle"));
	TRAPD(err, iCcp->IsClosedFileL(nullFileHandle));
	TEST(err == KErrBadHandle);

	// Check file extension.	
	TParse parser;
	parser.Set(KPathdm2, NULL, NULL);
	TEST(iCcp->IsClosedExtension(parser.Ext()));
	INFO_PRINTF2(_L("%S is Closed File"), &KPathdm2);

	CApfMimeContentPolicy* mimeContentPolicy = CApfMimeContentPolicy::NewL(iFs);
	CleanupStack::PushL(mimeContentPolicy);
  	TEST(!mimeContentPolicy->IsClosedFileL(KPathdcf));
  	INFO_PRINTF2(_L("%S is not Closed File"), &KPathdcf);
  	CleanupStack::PopAndDestroy(mimeContentPolicy);

	mimeContentPolicy = CApfMimeContentPolicy::NewLC(iFs);
	TEST(!mimeContentPolicy->IsClosedFileL(KPathgif));
	INFO_PRINTF2(_L("%S is not Closed File"), &KPathgif);
	CleanupStack::PopAndDestroy(mimeContentPolicy);
	}

TBool CT_MimeContentPolicyStep::DoCCPTestUsingFileHandleL(const TDesC &aName, TBool aIsDRMEnvelope)
	{
	TBool ret = EFalse;
    RFile fileHandle;

	TInt err;
	err = fileHandle.Open(iFs, aName, EFileShareReadersOrWriters);
    TEST(err == KErrNone);
	CleanupClosePushL(fileHandle);

	if (aIsDRMEnvelope)
		ret = iCcp->IsDRMEnvelopeL(fileHandle);
	else
		ret = iCcp->IsClosedFileL(fileHandle);

    CleanupStack::PopAndDestroy(&fileHandle);
	return ret;
	}

/**
   @SYMTestCaseID APPFWK-APPARC-0081

   @SYMREQ REQ7736
 
   @SYMTestCaseDesc Tests IsDRMEnvelopeL() method for various files.
  
   @SYMTestPriority High 
 
   @SYMTestStatus Implemented
  
   @SYMTestActions Calls CApfMimeContentPolicy::IsDRMEnvelopeL(RFile& aFileHandle); for different non-DRM Envelopes files.
     
   @SYMTestExpectedResults The test checks whether IsDRMEnvelopeL() returns EFalse for Files which are not DRM protected.
 */
void CT_MimeContentPolicyStep::CCPTestIsDRMEnvelopeFileHandleL()
	{
    INFO_PRINTF1(_L("Tests given files are DRM envelopes given the file handle"));     

	TEST(!DoCCPTestUsingFileHandleL(KPathjpg1, ETrue));
	TEST(!DoCCPTestUsingFileHandleL(KPathjpg2, ETrue));
	TEST(!DoCCPTestUsingFileHandleL(KPathdcf, ETrue));
	TEST(!DoCCPTestUsingFileHandleL(KPathdm1, ETrue));
	TEST(!DoCCPTestUsingFileHandleL(KPathgif, ETrue));
	TEST(!DoCCPTestUsingFileHandleL(KPathdm2, ETrue));
	}

/**
   @SYMTestCaseID APPFWK-APPARC-0082

   @SYMREQ REQ7736
 
   @SYMTestCaseDesc Tests IsClosedFileL() method for various files.
  
   @SYMTestPriority High 
 
   @SYMTestStatus Implemented
  
   @SYMTestActions Closed files are files whose file extensions are listed in the 10003a3f repository.
   Calls CApfMimeContentPolicy::IsClosedFileL(RFile& aFileHandle); for different Closed and non-closed files.
     
   @SYMTestExpectedResults The test checks whether IsClosedFileL() returns EFalse for Files which are not closed and\n
   ETrue for files which are closed.
 */
void CT_MimeContentPolicyStep::CCPTestIsClosedFileFileHandleL()
	{
   	INFO_PRINTF1(_L("Tests given files are Closed Files using File Handle"));

	TEST(!DoCCPTestUsingFileHandleL(KPathjpg1, EFalse));
	TEST(!DoCCPTestUsingFileHandleL(KPathjpg2, EFalse));
	TEST(!DoCCPTestUsingFileHandleL(KPathdcf, EFalse));
	TEST(DoCCPTestUsingFileHandleL(KPathdm1, EFalse));
	TEST(!DoCCPTestUsingFileHandleL(KPathgif, EFalse));
	TEST(DoCCPTestUsingFileHandleL(KPathdm2, EFalse));
	}
	
/**
   @SYMTestCaseID APPFWK-APPARC-0084

   @SYMREQ REQ7736
 
   @SYMTestCaseDesc OOM test for CApfMimeContentPolicy's NewL() and IsClosedFileL() methods.
  
   @SYMTestPriority High 
 
   @SYMTestStatus Implemented
  
   @SYMTestActions Calls CApfMimeContentPolicy's NewL() and IsClosedFileL() methods under OOM conditions.
     
   @SYMTestExpectedResults Tests should complete without any memory leaks.
 */
void CT_MimeContentPolicyStep::CCPOOMTestL()
	{
	INFO_PRINTF1(_L("OOM test for CApfMimeContentPolicy"));
	CApfMimeContentPolicy* mimeContentPolicy = NULL;
	
	TInt fail = 0;
	for (fail = 1; ; fail++)
		{
		__UHEAP_SETFAIL(RHeap::EDeterministic, fail);
		__UHEAP_MARK;
		TRAPD(err, mimeContentPolicy = CApfMimeContentPolicy::NewLC(); CleanupStack::Pop(mimeContentPolicy));
		TEST(err == KErrNone || err == KErrNoMemory);
		if (err == KErrNone)
			{
			delete mimeContentPolicy;
			__UHEAP_MARKEND;
			break;
			}
		__UHEAP_MARKEND;
		}
	
	__UHEAP_RESET;	
	TRAPD(err, mimeContentPolicy = CApfMimeContentPolicy::NewL());
	CleanupStack::PushL(mimeContentPolicy);
	TEST(err == KErrNone);
	
	TInt ret = KErrNone;
	for (fail = 1; ; fail++)
		{
		__UHEAP_SETFAIL(RHeap::EDeterministic, fail);
		__UHEAP_MARK;
		TRAP(err, ret = mimeContentPolicy->IsClosedFileL(KPathdm1));
		TEST(err == KErrNone || err == KErrNoMemory);
		if (err == KErrNone)
			{
			INFO_PRINTF1(_L("OOM Test for IsClosedFileL Method"));	
			TEST(ret);
			INFO_PRINTF2(_L("%S is Closed File"), &KPathdm1);
			CleanupStack::PopAndDestroy(mimeContentPolicy);
			__UHEAP_MARKEND;
			break;
			}
		__UHEAP_MARKEND;
		}

	__UHEAP_RESET;	
	INFO_PRINTF1(_L("OOM test Completed"));	
	}


/**
   @SYMTestCaseID APPFWK-APPARC-0108

   @SYMREQ REQ410-2692
 
   @SYMTestCaseDesc Tests Closed content and extension information repository is not writable.
  
   @SYMTestPriority High 
 
   @SYMTestStatus Implemented
  
   @SYMTestActions Calls create, get, set, reset, delete functions on the repository. Checks only read operations are allowed.
     
   @SYMTestExpectedResults Tests should complete without any failure.
 */

void CT_MimeContentPolicyStep::CCPTestIsClosedContentAndExtenstionInfoRepositoryReadOnlyL()
    {
    INFO_PRINTF1(_L("Testcase CCPTestIsClosedContentAndExtenstionInfoRepositoryReadOnly...."));   
    CRepository *cenrep=CRepository::NewL(KClosedContentAndExtensionInfoRepositoryUID);  
    CleanupStack::PushL(cenrep);
    TInt newKeyValue=0x00010000;
    //This key already exists in the default Closed content and extension information repository
    TInt existingKey=0x1;
    TBuf<KMaxDataTypeLength> keyData;
    TInt err=KErrNone;
    
    INFO_PRINTF1(_L("Testing creation of key in the repository"));
    err=cenrep->Create(newKeyValue, 0);
    TEST(err==KErrPermissionDenied);
    INFO_PRINTF2(_L("Error code while creating a key: %d"), err);
    
    INFO_PRINTF1(_L("Testing setting value of an existing key in the repository"));    
    err=cenrep->Set(existingKey, 0);
    TEST(err==KErrPermissionDenied);
    INFO_PRINTF2(_L("Error code while setting a value of an existing key: %d"), err);
    
    INFO_PRINTF1(_L("Testing getting value of an existing key in the repository"));
    err=cenrep->Get(existingKey, keyData);
    TEST(err==KErrNone);
    INFO_PRINTF2(_L("Error code while getting a value of an existing key: %d"), err);
    
    INFO_PRINTF1(_L("Testing resetting value of an existing key in the repository"));
    err=cenrep->Reset(existingKey);
    TEST(err==KErrPermissionDenied);
    INFO_PRINTF2(_L("Error code while reseting a value of existing key: %d"), err);

    INFO_PRINTF1(_L("Testing deleting an existing key in the repository"));
    err=cenrep->Delete(existingKey);
    TEST(err==KErrPermissionDenied);
    INFO_PRINTF2(_L("Error code while deleting an existing key: %d"), err);
    CleanupStack::PopAndDestroy(cenrep);
    INFO_PRINTF1(_L("Testcase CCPTestIsClosedContentAndExtenstionInfoRepositoryReadOnly completed...."));    
    }
