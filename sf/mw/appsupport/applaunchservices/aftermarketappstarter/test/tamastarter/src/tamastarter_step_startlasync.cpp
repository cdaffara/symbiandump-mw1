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
// NOTE: These test cases are very similar to those in the standalone step, but with the difference that instead of launching
// Amastarter.exe, we use AmaStarter DLL, ie, the test now uses CAmaStart::StartL() to start the AMAs.
// Since this is intended as Unit testing for StartL() API only one instance of the test database is checked, unlike 
// standalone tests where different test cases are perfomed.
// 
//

/**
 @file
 @test
 @internalComponent - Internal Symbian test code 
*/

#include "appfwk_test_utils.h"
#include "tamastarter_constants.h"
#include "tamastarter_database_utils.h"
#include "tamastarter_step_startlasync.h"
#include "tamastarterappobserver.h"
#include "amastart.h"

const TInt KTimerDelay = 1000000;

void SignalAndCloseMutex(TAny* aMutex)
	{
	RMutex* mutex = static_cast<RMutex*>(aMutex);
	mutex->Signal();
	mutex->Close();
	}

void CAppfwkAmaStarterTestStepStartLAsync::CompareResultsL(const TDesC& aResultFileName, const TDesC& aAmcName, TInt aRetryCntr, TInt aLaunchCntr, TInt aSuccessCntr, TInt aPanicCntr, TBool aMonitored)
	{	
	INFO_PRINTF3(_L("Comparing results for filename '%S', section '%S'"), &aResultFileName, &aAmcName);

	TBuf<KMaxFileName>	fileName(KTestAppResultsPath);
	fileName.Append(aResultFileName);
	fileName.Append(KTestLogFileExtension);

	RMutex testAmaStarterLogMutex;
	TInt error;

	//Creating a mutex to allow one application to write to log at one time
	do
		{
		error=testAmaStarterLogMutex.CreateGlobal(KTestAmaStarterLogMutexName);

		if (error==KErrAlreadyExists)
				{
				error=testAmaStarterLogMutex.OpenGlobal(KTestAmaStarterLogMutexName);
				}
		}
	//Retry the creation of mutex if RMutex::OpenGlobal fails with KErrNotFound.
	//This may happen due to simultaneous execution of multiple threads on SMP environment.
	//Other threads might cause to delete the mutex before the current thread open handle to it.
	while(error==KErrNotFound); 
					
	//Leave if the creation of mutex fails.
	User::LeaveIfError(error);
	
	INFO_PRINTF1(_L("Check the actual counter values from the result log file"));

	CIniData* resultFile;	
	TInt actualLaunchCntr = 0;
	TInt retryCount=10; //retries to be done if the required section is not found in log file. 
	TBool result;
	
	do
		{
		testAmaStarterLogMutex.Wait();
		CleanupStack::PushL(TCleanupItem(SignalAndCloseMutex, &testAmaStarterLogMutex));
		resultFile = CIniData::NewL(fileName);	
		CleanupStack::PushL(resultFile);

		result=resultFile->FindVar(aAmcName, KLaunchCntr, actualLaunchCntr);
		
		//If the KLaunchCntr in aAmcName section is not found, retry again.		
		if(!result)
			{
				CleanupStack::PopAndDestroy(2); //Release mutex and file handle so that the tamastarter_app[1-3].exe can write data to log.		
				User::After(200000); //Try after 200ms
			}
		retryCount--;
		}
	while(!result && retryCount); 
	
	TEST(result);
	
	TInt actualSuccessCntr = 0;
	TEST(resultFile->FindVar(aAmcName, KSuccessCntr, actualSuccessCntr));

	TInt actualRetryCntr = 0;
	TEST(resultFile->FindVar(aAmcName, KRetryCntr, actualRetryCntr));

	TInt actualPanicCntr = 0;
	TEST(resultFile->FindVar(aAmcName, KPanicCntr, actualPanicCntr));

	// If the application was requested to be monitored by SysMon, the test application might be launched 
	//	more than the expected number of times, as the test application may die and restarted many times 
	//  during the entire test case execution.		
	if (!aMonitored)
		{
		TEST(actualLaunchCntr == aLaunchCntr);
		TEST(actualSuccessCntr == aSuccessCntr);
		TEST(actualRetryCntr == aRetryCntr);
		TEST(actualPanicCntr == aPanicCntr);
		}
	else
		{
		TEST(actualLaunchCntr >= aLaunchCntr);
		TEST(actualSuccessCntr >= aSuccessCntr);
		TEST(actualRetryCntr >= aRetryCntr);
		TEST(actualPanicCntr >= aPanicCntr);
		}
	
	if(result)
		CleanupStack::PopAndDestroy(2);//resultFile, testAmaStarterLogMutex
	}

static TInt CallBackL(TAny* aAppfwkAmaStarterTestStepStartLAsync)
	{
	//Call back function to stop active scheduler
	CAppfwkAmaStarterTestStepStartLAsync* test = reinterpret_cast<CAppfwkAmaStarterTestStepStartLAsync*>(aAppfwkAmaStarterTestStepStartLAsync);
	test->CallBackRunL();
	return KErrNone;
	}

void CAppfwkAmaStarterTestStepStartLAsync::CallBackRunL()
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

void CAppfwkAmaStarterTestStepStartLAsync::TestCase1L()
	{
	// Launch the appropriate DSC via CAmaStart::Start()
	CAmaStart* amaStart = CAmaStart::NewL();
	CleanupStack::PushL(amaStart);

	amaStart->Start(TUid::Uid(0x10000011), iRequest);
	TCallBack stop(CallBackL, this);
	iAsyncStopScheduler->Set(stop);
	iAsyncStopScheduler->CallBack();
	iActiveSchedulerWait->Start();

	User::WaitForRequest(iRequest);
	TEST(iRequest == KErrNone);
	INFO_PRINTF3(_L("CAmaStart->Start(TUid::Uid(0x10000011), &myRequestStatus) finished with request status '%d' (expected '%d')"), iRequest.Int(), KErrNone);

	// Check and compare results as they have been stored in the result log file
	CompareResultsL(_L("tamastarter_result_1"), _L("tamastarter_app1"), 0, 1, 1, 0, EFalse);
	CompareResultsL(_L("tamastarter_result_1"), _L("tamastarter_app2"), 1, 2, 1, 0, EFalse);
	CompareResultsL(_L("tamastarter_result_1"), _L("tamastarter_app3"), 0, 1, 1, 0, EFalse);

	// clean-up
	CleanupStack::PopAndDestroy(amaStart);
	}


/**
	@SYMTestCaseID 		APPFWK-AMASTARTER-0013
	@SYMTestCaseDesc	Attempt to start non-existent DSC with Start()
	@SYMPREQ 			PREQ1871
	@SYMREQ				REQ9141
	@SYMTestType		UT
	@SYMTestPriority	Critical
	@SYMTestStatus 		Implemented
	@SYMTestActions		The test uses the ID for a DSC that does not exist.
						Then the test calls (asynchronous) CAmaStart::Start() with the appropriate DSC ID.
						The test calls User::WaitForRequest() to wait for the Start() call to complete.
	@SYMTestExpectedResults 	The TRequestStatus object used for the Start() call has a value of KErrArgument

*/
void CAppfwkAmaStarterTestStepStartLAsync::TestCase2L()
	{
	CAmaStart* amaStart = CAmaStart::NewL();
	CleanupStack::PushL(amaStart);

	amaStart->Start(TUid::Uid(0x100000FF), iRequest);
	TCallBack stop(CallBackL, this);
	iAsyncStopScheduler->Set(stop);
	iAsyncStopScheduler->CallBack();
	iActiveSchedulerWait->Start();

	User::WaitForRequest(iRequest);
	TEST(iRequest == KErrArgument);
	INFO_PRINTF3(_L("CAmaStart->StartL(TUid::Uid(0x100000FF), &myRequestStatus) finished with request status '%d' (expected '%d')"), iRequest.Int(), KErrArgument);

	// clean-up
	CleanupStack::PopAndDestroy(amaStart);
	}


/**
	@SYMTestCaseID 		APPFWK-AMASTARTER-0014
	@SYMTestCaseDesc	Check for Handle leaks in CAmaStart::StartL().
	@SYMDEF 			DEF119802
	@SYMTestType		UT
	@SYMTestPriority	Critical
	@SYMTestStatus 		Implemented
	@SYMTestCaseDependencies	The appropriate database has been created.
	@SYMTestActions		The test uses a known DSC store database with valid items in it.
						Then the test uses (asynchronous) CAmaStart::Start() with the appropriate item id to start these 
						AMAs from an existing DSC item in the created test database.
						The test calls User::WaitForRequest() to wait for the Start() call to complete.
						The test case checks for the no. of open handles in the process owning this thread and the
						no. of open handles in this thread.
	@SYMTestExpectedResults 	The no. of open handles in process owning this thread and no. of open handles in this thread 
								should be same before calling CAmaStart::Start() and after destroying CAmaStart object.
*/
void CAppfwkAmaStarterTestStepStartLAsync::TestCase3L()
	{
	// Launch the appropriate AMAStarter(s) via CAmaStart::StartL()
  	TInt threadHandles_BeforeAmaStart;
	TInt processHandles_BeforeAmaStart;
	RThread().HandleCount(processHandles_BeforeAmaStart, threadHandles_BeforeAmaStart);
	INFO_PRINTF2(_L("No. of open handles in process which owns this process %d before calling CAmaStart::StartL"),processHandles_BeforeAmaStart);
	INFO_PRINTF2(_L("No. of open handles in this thread %d before calling CAmaStart::StartL"),threadHandles_BeforeAmaStart);
	CAmaStart* amaStart = CAmaStart::NewL();
	CleanupStack::PushL(amaStart);

	amaStart->Start(TUid::Uid(0x10000018), iRequest);
	TCallBack stop(CallBackL, this);
	iAsyncStopScheduler->Set(stop);
	iAsyncStopScheduler->CallBack();
	iActiveSchedulerWait->Start();
	User::WaitForRequest(iRequest);
	TEST(iRequest == KErrNone);
	INFO_PRINTF3(_L("CAmaStart->Start(TUid::Uid(0x10000018), &myRequestStatus) finished with request status '%d' (expected '%d')"), iRequest.Int(), KErrNone);

	// Check and compare results as they have been stored in the result log file
	CompareResultsL(_L("tamastarter_result_7"), _L("tamastarter_app1"), 0, 1, 1, 0, EFalse);
	CompareResultsL(_L("tamastarter_result_7"), _L("tamastarter_app3"), 0, 1, 1, 0, EFalse);

	CTestAmastartTimer* testAmastartTimer1 = CTestAmastartTimer::NewL(iActiveSchedulerWait, KTimerDelay);
	testAmastartTimer1->StartL();
	iActiveSchedulerWait->Start();
	delete testAmastartTimer1;

	TInt threadHandles_AfterAmaStart;
	TInt processHandles_AfterAmaStart;
	RThread().HandleCount(processHandles_AfterAmaStart, threadHandles_AfterAmaStart);
	TEST(processHandles_AfterAmaStart == processHandles_BeforeAmaStart);
	INFO_PRINTF2(_L("No. of open handles in process which owns this process %d after calling CAmaStart::StartL"),processHandles_AfterAmaStart);

	TEST(threadHandles_AfterAmaStart > threadHandles_BeforeAmaStart);
	INFO_PRINTF2(_L("No. of open handles in this thread %d after calling CAmaStart::StartL"),threadHandles_AfterAmaStart);

	// clean-up
	CleanupStack::PopAndDestroy(amaStart);

	TInt threadHandles_AfterAmaStartDeleted;
	TInt processHandles_AfterAmaStartDeleted;
	RThread().HandleCount(processHandles_AfterAmaStartDeleted, threadHandles_AfterAmaStartDeleted);

	TEST(processHandles_AfterAmaStartDeleted == processHandles_BeforeAmaStart);
	INFO_PRINTF2(_L("No. of open handles in process which owns this process %d after deleting CAmaStart object"),processHandles_AfterAmaStartDeleted);

	TEST(threadHandles_AfterAmaStartDeleted == threadHandles_BeforeAmaStart);
	INFO_PRINTF2(_L("No. of open handles in this thread %d after deleting CAmaStart object"),threadHandles_AfterAmaStartDeleted);
	}


//---------

CAppfwkAmaStarterTestStepStartLAsync::CAppfwkAmaStarterTestStepStartLAsync(const TDesC& aStepName)
	{	
	SetTestStepName(aStepName);
	}


CAppfwkAmaStarterTestStepStartLAsync::~CAppfwkAmaStarterTestStepStartLAsync()
	{
	delete iAsyncStopScheduler;
	delete iActiveSchedulerWait;
	delete iActiveScheduler;
	}

TVerdict CAppfwkAmaStarterTestStepStartLAsync::doTestStepL()
	{	
	INFO_PRINTF1(_L("AMAStarter StartL asynchronous tests started...."));

	//Create and install active scheduler
	iActiveScheduler = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install (iActiveScheduler);
	iActiveSchedulerWait = new(ELeave) CActiveSchedulerWait;
	//Needed for calling calback for stopping active scheduler
	iAsyncStopScheduler = new(ELeave) CAsyncCallBack(CActive::EPriorityIdle);

	__UHEAP_MARK;
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
	INFO_PRINTF2(_L("TestCase2L() finished with code '%d'\n"), ret);

	TRAP(ret, TestCase3L());
	TESTE(ret == KErrNone, ret);
	INFO_PRINTF2(_L("TestCase3L() finished with code '%d'\n"), ret);

	INFO_PRINTF1(_L("Restore DBMS / SQL data store backups available prior to running the test"));
	TRAP(ret, DscDatabaseRestoreL());
	TESTE(ret == KErrNone, ret);

	INFO_PRINTF1(_L("Delete log files created to store temporary results"));
	RSmlTestUtils fs;
	CleanupClosePushL(fs);
	User::LeaveIfError(fs.Connect());
	User::LeaveIfError(fs.DeleteFileUsingWildcardL(KTestLogFileWild));
	CleanupStack::PopAndDestroy(&fs);
	__UHEAP_MARKEND;

	INFO_PRINTF1(_L("...AMAStarter StartL asynchronous tests finished!!"));
	return TestStepResult();
	}

