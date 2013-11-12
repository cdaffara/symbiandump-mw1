// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#include "amastart.h"
#include <s32file.h>

#include "appfwk_test_utils.h"
#include "tamastarter_constants.h"
#include "tamastarter_database_utils.h"
#include "tamastarter_step_cancel.h"

CAppfwkAmaStarterTestStepCancel::CAppfwkAmaStarterTestStepCancel(const TDesC& aStepName)
	{	
	SetTestStepName(aStepName);
	}
	
CAppfwkAmaStarterTestStepCancel::~CAppfwkAmaStarterTestStepCancel()
	{	
	delete iAsyncStopScheduler;
	delete iActiveSchedulerWait;
	delete iActiveScheduler;
	}

static TInt CallBackL(TAny* aAppfwkAmaStarterTestStepCancel)
	{
	//Call back function to stop active scheduler
	CAppfwkAmaStarterTestStepCancel* test = reinterpret_cast<CAppfwkAmaStarterTestStepCancel*>(aAppfwkAmaStarterTestStepCancel);
	test->CallBackRunL();
	return KErrNone;
	}

void CAppfwkAmaStarterTestStepCancel::CallBackRunL()
	{
	iActiveSchedulerWait->AsyncStop();
	}

/**
	@SYMTestCaseID 				APPFWK-AMA-0001
	@SYMTestCaseDesc			Cancel a pending asynchronous StartL() request.
	@SYMPREQ 					PREQ1871
	@SYMREQ						REQ9141
	@SYMTestType				UT
	@SYMTestPriority			Critical
	@SYMTestStatus 				Implemented
	@SYMTestCaseDependencies	The appropriate database has been created.
	@SYMTestActions				Call asynchronous StartL() with a DSC that takes a long time to complete execution.
								Call CancelStart().
	@SYMTestExpectedResults 	The request status for the StartL() call will have the value KErrCancel.
*/
void CAppfwkAmaStarterTestStepCancel::TestCase1L()
	{
	// Launch the appropriate AMAStarter(s) via CAmaStart::StartL()
	TRequestStatus myRequestStatus;
	CAmaStart* amaStart = CAmaStart::NewL();
	CleanupStack::PushL(amaStart);

	amaStart->Start(TUid::Uid(0x10000017), myRequestStatus);
	TEST(myRequestStatus == KRequestPending);

	TCallBack stop(CallBackL, this);
	iAsyncStopScheduler->Set(stop);
	iAsyncStopScheduler->CallBack();
	iActiveSchedulerWait->Start();

	amaStart->CancelStart();	
	User::WaitForRequest(myRequestStatus);
	TEST(myRequestStatus == KErrCancel);
	INFO_PRINTF3(_L("CAmaStart->Start(TUid::Uid(0x10000017), &myRequestStatus) finished with request status '%d' (expected '%d')"), myRequestStatus.Int(), KErrCancel);

	// clean-up
	CleanupStack::PopAndDestroy(amaStart);
	}

	
/**
	@SYMTestCaseID 				APPFWK-AMA-0002
	@SYMTestCaseDesc			Call CancelStart() without first calling asynchronous StartL()
	@SYMPREQ 					PREQ1871
	@SYMREQ						REQ9141
	@SYMTestType				UT
	@SYMTestPriority			Critical
	@SYMTestStatus 				Implemented
	@SYMTestActions				Call CancelStart().
	@SYMTestExpectedResults 	The call doesn’t panic or leave. (CancelStart() has no return value.)
*/
void CAppfwkAmaStarterTestStepCancel::TestCase2L()
	{	
	INFO_PRINTF1(_L("AMAStarter Cancel test started...."));

	CAmaStart* amaStart = CAmaStart::NewL();
	CleanupStack::PushL(amaStart);

	amaStart->CancelStart();

	// clean-up
	CleanupStack::PopAndDestroy(amaStart);

	INFO_PRINTF1(_L("...AMAStarter Cancel test finished!!"));
	}

TVerdict CAppfwkAmaStarterTestStepCancel::doTestStepL()
	{	
	INFO_PRINTF1(_L("AMAStarter Cancel starting DSC tests started...."));

 	__UHEAP_MARK;
 
	//Create and install active scheduler
	iActiveScheduler = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install (iActiveScheduler);
	iActiveSchedulerWait = new(ELeave) CActiveSchedulerWait;
	//Needed for calling calback for stopping active scheduler
	iAsyncStopScheduler = new(ELeave) CAsyncCallBack(CActive::EPriorityStandard);

	// Create DBMS / SQL data store backups and delete original data stores
	TRAPD(ret, DscDatabaseBackupL());
	TESTE(ret == KErrNone, ret);

	INFO_PRINTF1(_L("Create appropriate data store for the tests"));
	TRAP(ret, CreateTestDatabaseL());
	TESTE(ret == KErrNone, ret);

	INFO_PRINTF1(_L("Run the test cases"));
	TRAP(ret, TestCase1L());
	TESTE(ret == KErrNone, ret);
	INFO_PRINTF2(_L("TestCase1L() finished with code '%d'\n"), ret);

	TRAP(ret, TestCase2L());
	TESTE(ret == KErrNone, ret);
	INFO_PRINTF2(_L("TestCase1L() finished with code '%d'\n"), ret);

	INFO_PRINTF1(_L("Restore DBMS / SQL data store backups available prior to running the test"));
	TRAP(ret, DscDatabaseRestoreL());
	TESTE(ret == KErrNone, ret);

	delete iAsyncStopScheduler;
	iAsyncStopScheduler = NULL;
	delete iActiveSchedulerWait;
	iActiveSchedulerWait = NULL;
	delete iActiveScheduler;
	iActiveScheduler = NULL;
	
	__UHEAP_MARKEND;

	INFO_PRINTF1(_L("...AMAStarter Cancel starting DSC tests finished!!"));

	return TestStepResult();
	}
