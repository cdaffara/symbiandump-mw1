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
// The following test case is used to test whether viewtestserver has the capability
// to access apis being policed.
// 
//

/**
 @file
 @test
 @internalComponent - Internal Symbian test code 
*/

#include <viewcli.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <viewclipartner.h>
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS
#include "appfwk_test.h"
#include "appfwk_viewsrv_test_step_Capability1.h"


#define KUidTViewAppValue 0x1								//! 32 bit Integer Uid value for first Application												//! 32 bit Integer Uid value for first View
#define KUidTViewValue 0x1

const TUid KUidTViewApp={KUidTViewAppValue};				//! Uid for Application One
const TUid KUidTView={KUidTViewValue};						//! Uid for first View Id 

#define APP_VIEW_ID TVwsViewId(KUidTViewApp,KUidTView)		//! Application View Identifier for first View

const TInt KDummyThreadId=1;


// The CAppStarterCap1 method definitions
//

/**
  Factory method that constructs the view server application object.\n
 */
CAppStarterCap1* CAppStarterCap1::NewL()
	{
	CAppStarterCap1* self=new(ELeave) CAppStarterCap1;
	return self;
	}

/**
  Destructor\n
 */
CAppStarterCap1::~CAppStarterCap1()
	{
	}

/**
  Starts the app with thread id KDummyThreadId.\n
 */
void CAppStarterCap1::StartAppL(TUid aAppUid,TThreadId& aThreadId)
	{
	RDebug::Print(_L("MVwsAppStarter AppUid: 0x%x"), aAppUid.iUid);
	aThreadId=TThreadId(KDummyThreadId);
	}


/**
  @SYMTestCaseID			APPFWK-VIEWSRV-0001
 
  @SYMPREQ					Unknown
  
  @SYMREQ					Unknown		
 
  @SYMTestCaseDesc 			Ensure ViewServer responds with permission denied for test server without
  							the correct capabilities.
 
  @SYMTestPriority 			High
 
  @SYMTestStatus 			Implemented
  
  @SYMTestActions  			Connect to ViewServer and check whether test server process
  							has the permission to access the mentioned apis
  							API Calls:\n		
  							CVwsSessionWrapper::NewL();
  							CVwsSessionWrapper::SetSystemDefaultView(const TVwsViewId &aViewId);
  							CVwsSessionWrapper::SetClientRequestTimeOut(TTimeIntervalMicroSeconds32 aDuration);
  							CVwsSessionWrapper::SetServerEventTimeOut(TTimeIntervalMicroSeconds32 aDuration);
  							CVwsSessionWrapper::EnableServerEventTimeOut(TBool aEnable);
  							CVwsSessionWrapper::ShutdownViewServer();
 
  @SYMTestExpectedResults 	Test should complete without any panic.The called API's should return
  							KErrPermissionDenied.
  
 **/
void CAppfwkViewsrvTestStepCapability1::TestCapability1L()
	{
	//Start the server. It already exists.
	CAppStarterCap1* appStarter=CAppStarterCap1::NewL();
	TInt err = CVwsSessionWrapper::StartViewServer(*appStarter);
	INFO_PRINTF1(_L("Starting ViewSrv...it should already be running..."));
	INFO_PRINTF3(_L("Expected return code: %d, Actual return code: %d"), KErrAlreadyExists, err);
	TEST(err==KErrAlreadyExists);
	delete appStarter;
	
	//Create a session
	CVwsSessionWrapper *viewServerSession = NULL;
	TRAP(err,viewServerSession=CVwsSessionWrapper::NewL());
	CleanupStack::PushL(viewServerSession);
	
	INFO_PRINTF1(_L("Connecting to View Server"));
	INFO_PRINTF3(_L("Expected return code: %d, Actual return code: %d"), KErrNone, err);
	TEST(err==KErrNone);
	
	if(err==KErrNone)
		{	
		//Capability check for SetSystemDefaultView API
		APPFWK_NEGATIVE_PLATSEC_START;
		TInt ret=viewServerSession->SetSystemDefaultView(APP_VIEW_ID);
		APPFWK_NEGATIVE_PLATSEC_FINISH;
		INFO_PRINTF1(_L("Capability check of SetSystemDefaultView"));
		INFO_PRINTF3(_L("Expected return code: %d, Actual return code: %d"), KErrPermissionDenied, ret);
		TEST(ret==KErrPermissionDenied);
		
		//Capability check for SetClientRequestTimeOut API
		APPFWK_NEGATIVE_PLATSEC_START;
		ret=viewServerSession->SetClientRequestTimeOut(TTimeIntervalMicroSeconds32(1000000));
		APPFWK_NEGATIVE_PLATSEC_FINISH;
		INFO_PRINTF1(_L("Capability check of SetClientRequestTimeOut"));
		INFO_PRINTF3(_L("Expected return code: %d, Actual return code: %d"), KErrPermissionDenied, ret);
		TEST(ret==KErrPermissionDenied);

		//Capability check for SetServerEventTimeOut API
		APPFWK_NEGATIVE_PLATSEC_START;
		ret=viewServerSession->SetServerEventTimeOut(TTimeIntervalMicroSeconds32(5000000));
		APPFWK_NEGATIVE_PLATSEC_FINISH;
		INFO_PRINTF1(_L("Capability check of SetServerEventTimeOut"));
		INFO_PRINTF3(_L("Expected return code: %d, Actual return code: %d"), KErrPermissionDenied, ret);
		TEST(ret==KErrPermissionDenied);
		
		//Capability check for EnableServerEventTimeOut API
		APPFWK_NEGATIVE_PLATSEC_START;
		ret=viewServerSession->EnableServerEventTimeOut(EFalse);
		APPFWK_NEGATIVE_PLATSEC_FINISH;
		INFO_PRINTF1(_L("Capability check of EnableServerEventTimeOut"));
		INFO_PRINTF3(_L("Expected return code: %d, Actual return code: %d"), KErrPermissionDenied, ret);
		TEST(ret==KErrPermissionDenied);
		
		//Capability check for EnableServerBlankScreen API
		APPFWK_NEGATIVE_PLATSEC_START;
		ret=viewServerSession->EnableServerBlankScreen(EFalse);
		APPFWK_NEGATIVE_PLATSEC_FINISH;
		INFO_PRINTF1(_L("Capability check of EnableServerBlankScreen"));
		INFO_PRINTF3(_L("Expected return code: %d, Actual return code: %d"), KErrPermissionDenied, ret);
		TEST(ret==KErrPermissionDenied);
		
		//Capability check for ShutdownViewServer API.
		APPFWK_NEGATIVE_PLATSEC_START;	
		ret=viewServerSession->ShutdownViewServer();
		APPFWK_NEGATIVE_PLATSEC_FINISH;
		INFO_PRINTF1(_L("Capability check of ShutdownViewServer"));
		INFO_PRINTF3(_L("Expected return code: %d, Actual return code: %d"), KErrPermissionDenied, ret);
		TEST(ret==KErrPermissionDenied);
		}
		
	//Free memory here as heapcheck is done in doTestStepL().	
	CleanupStack::PopAndDestroy(viewServerSession);
	}
	
/**
  Constructor
 */
CAppfwkViewsrvTestStepCapability1::CAppfwkViewsrvTestStepCapability1()
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KCAppfwkViewsrvTestStepCapability1);
	}

/**
  Destructor
 */
CAppfwkViewsrvTestStepCapability1::~CAppfwkViewsrvTestStepCapability1()
	{ 
	}

TVerdict CAppfwkViewsrvTestStepCapability1::doTestStepL()
    {
	INFO_PRINTF1(_L("APPFWK-VIEWSRV-0001: Capability1 - Started"));
	
	__UHEAP_MARK;
	
	TestCapability1L();
	
	__UHEAP_MARKEND;

	INFO_PRINTF1(_L("APPFWK-VIEWSRV-0001: Capability1 - Finished"));
	return TestStepResult();
	}
