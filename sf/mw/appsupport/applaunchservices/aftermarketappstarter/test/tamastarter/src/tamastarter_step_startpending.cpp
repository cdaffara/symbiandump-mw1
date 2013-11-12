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
#include "tamastarter_step_startpending.h"

_LIT(KLogFileName, "C:\\testprocslow.log");

static TInt CallBackL(TAny* aAppfwkAmaStarterTestStepStartPending)
	{
	//Call back function to stop active scheduler
	CAppfwkAmaStarterTestStepStartPending* test = reinterpret_cast<CAppfwkAmaStarterTestStepStartPending*>(aAppfwkAmaStarterTestStepStartPending);
	test->CallBackRunL();
	return KErrNone;
	}

void CAppfwkAmaStarterTestStepStartPending::CallBackRunL()
	{
	//Stop the active scheduler if the request is completed else
	//callback once again
	if(iRequest != KRequestPending)
		{
		iActiveSchedulerWait->AsyncStop();
		}
	else
		{
		iAsyncStopScheduler->CallBack();
		}
	}

//----------------------

/**
	@SYMTestCaseID 				APPFWK-AMA-0003
	@SYMTestCaseDesc			Call StartL() when an asynchronous StartL() is pending.
	@SYMPREQ 					PREQ1871
	@SYMREQ						REQ9141
	@SYMTestType				UT
	@SYMTestPriority			Critical
	@SYMTestStatus 				Implemented
	@SYMTestCaseDependencies	The appropriate database has been created.
	@SYMTestActions				1.	Call asynchronous StartL() with a DSC that takes a long time to complete execution.
								2.	Call synchronous StartL() (with a valid DSC). Check Leave value.
								3.	Call asynchronous StartL() (with a valid DSC). Check Leave value.
								4.	Call User::WaitForRequest() to wait for the asynchronous StartL() to complete.
									Check that the last item, in the list has been started.

	@SYMTestExpectedResults 	1.	The associated TRequestStatus object has the value KRequestPending.
								2.	Leaves with KErrNotReady.
								3.	Leaves with KErrNotReady.
								4.	The last item has been run.
*/
void CAppfwkAmaStarterTestStepStartPending::TestCase1L()
	{
	// Launch the appropriate AMAStarter(s) via CAmaStart::StartL()
	CAmaStart* amaStart = CAmaStart::NewL();
	CleanupStack::PushL(amaStart);

	amaStart->Start(TUid::Uid(0x10000017), iRequest);
	TEST(iRequest == KRequestPending);
	INFO_PRINTF3(_L("CAmaStart->Start(TUid::Uid(0x10000017), iRequest) request status '%d' (expected '%d')"), iRequest.Int(), KRequestPending);
	


	TRAPD(ret, amaStart->StartL(TUid::Uid(0x10000016)));
	TEST(ret == KErrNotReady);
	INFO_PRINTF3(_L("CAmaStart->StartL(TUid::Uid(0x10000016)) Leave code '%d' (expected '%d')"), ret, KErrNotReady);
	
	TRequestStatus req;
	amaStart->Start(TUid::Uid(0x10000016), req);
	User::WaitForRequest(req);
	TEST(req == KErrNotReady);
	INFO_PRINTF3(_L("CAmaStart->Start(TUid::Uid(0x10000016), req) request status '%d' (expected '%d')"), req.Int(), KErrNotReady);

	TCallBack stop(CallBackL, this);
	iAsyncStopScheduler->Set(stop);
	iAsyncStopScheduler->CallBack();
	iActiveSchedulerWait->Start();

	User::WaitForRequest(iRequest);
	TEST(iRequest == KErrNone);
	
	INFO_PRINTF3(_L("CAmaStart->Start(TUid::Uid(0x10000017), iRequest) request status '%d' (expected '%d')"), iRequest.Int(), KErrNone);

	//Check that the last item in the DSC did run
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	RFileReadStream rs;
	ret = rs.Open(fs, KLogFileName, EFileShareExclusive|EFileRead);
	if (KErrNone != ret)
	{
		ERR_PRINTF2(_L("Could not open log file. Leave code: '%d'"), ret);
		User::LeaveIfError(ret);
		}
	rs.PushL();
	const TInt KMaxCommandLength = 256;
	TBuf<KMaxCommandLength> commandLine;
	rs.ReadL(commandLine,1);
	TEST(commandLine.Compare(_L("9")) == 0);
	INFO_PRINTF2(_L("Checking log result: Expected '9', Actual '%S'"), &commandLine);
	CleanupStack::PopAndDestroy(); //rs
	
	ret = fs.Delete(KLogFileName);
	if (KErrNone != ret)
		{
		ERR_PRINTF2(_L("Could not delete log file. Leave code: '%d'"), ret);
		User::LeaveIfError(ret);
		}

	CleanupStack::PopAndDestroy(2, amaStart); // fs, amastart
	}



//---------

CAppfwkAmaStarterTestStepStartPending::CAppfwkAmaStarterTestStepStartPending(const TDesC& aStepName)
	{	
	SetTestStepName(aStepName);
	}


CAppfwkAmaStarterTestStepStartPending::~CAppfwkAmaStarterTestStepStartPending()
	{
	delete iAsyncStopScheduler;
	delete iActiveSchedulerWait;
	delete iActiveScheduler;
	}


TVerdict CAppfwkAmaStarterTestStepStartPending::doTestStepL()
	{	
	INFO_PRINTF1(_L("AMAStarter start while start pending DSC tests started...."));

 	__UHEAP_MARK;
 	//Create and install active scheduler
	iActiveScheduler = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install (iActiveScheduler);
	iActiveSchedulerWait = new(ELeave) CActiveSchedulerWait;
	//Needed for calling calback for stopping active scheduler
	iAsyncStopScheduler = new(ELeave) CAsyncCallBack(CActive::EPriorityIdle);
	
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

	INFO_PRINTF1(_L("...AMAStarter start while start pending DSC tests finished!!"));
	return TestStepResult();
	}

