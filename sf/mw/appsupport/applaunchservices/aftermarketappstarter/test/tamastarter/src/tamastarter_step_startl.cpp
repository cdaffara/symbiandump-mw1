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


#include "amastart.h"

#include "appfwk_test_utils.h"
#include "tamastarter_constants.h"
#include "tamastarter_database_utils.h"
#include "tamastarter_step_startl.h"
#include "tamastarterappobserver.h"

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
#include <ssm/ssmloadamastart.h>

_LIT( KAmaStartDLL, "loadamastart.dll" );
typedef CSsmLoadAmaStart* (*TFuncCreateL)(void);
#endif // SYMBIAN_SYSTEM_STATE_MANAGEMENT

_LIT_SECURITY_POLICY_PASS(KReadPolicy);
_LIT_SECURITY_POLICY_PASS(KWritePolicy);

//----------------------
void CAppfwkAmaStarterTestStepStart::CompareResultsL(const TDesC& aResultFileName, const TDesC& aAmcName, TInt aRetryCntr, TInt aLaunchCntr, TInt aSuccessCntr, TInt aPanicCntr, TBool aMonitored)
	{	
	INFO_PRINTF3(_L("Comparing results for filename '%S', section '%S'"), &aResultFileName, &aAmcName);

	TBuf<KMaxFileName>	fileName(KTestAppResultsPath);
	fileName.Append(aResultFileName);
	fileName.Append(KTestLogFileExtension);

	CIniData* resultFile = CIniData::NewL(fileName);	
	CleanupStack::PushL(resultFile);

	INFO_PRINTF1(_L("Check the actual counter values from the result log file"));
	TInt actualLaunchCntr = 0;
	TEST(resultFile->FindVar(aAmcName, KLaunchCntr, actualLaunchCntr));

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
	CleanupStack::PopAndDestroy(resultFile);
	}


//---------

/**
	@SYMTestCaseID 		APPFWK-AMASTARTER-0008
	@SYMTestCaseDesc	Check if AMA can be started using AmaStarter DLL.
	@SYMPREQ 			PREQ871
	@SYMREQ				REQ9141
	@SYMTestType		UT
	@SYMTestPriority	Critical
	@SYMTestStatus 		Implemented
	@SYMTestCaseDependencies	The appropriate database has been created.
	@SYMTestActions		The test uses a known DSC store database with valid items in it.
						Then the test uses CAmaStart::StartL() with the appropriate item id to start these AMAs
						from an existing DSC item in the created test database.
	@SYMTestExpectedResults 	The different AMAs are started as expected from the test database.
								The application logs are read to check the applications were launched as expected.
*/
void CAppfwkAmaStarterTestStepStart::TestCase1L()
	{
	// Launch the appropriate AMAStarter(s) via CAmaStart::StartL()
	CTestAmastartAppObserver* testPropertyAmastart = CTestAmastartAppObserver::NewL(iActiveSchedulerWait);
	CleanupStack::PushL(testPropertyAmastart);
	testPropertyAmastart->StartL();

	CAmaStart* amaStart = CAmaStart::NewL();
	CleanupStack::PushL(amaStart);

	TRAPD(ret, amaStart->StartL(TUid::Uid(0x10000011)));
	TEST(ret == KErrNone);
	INFO_PRINTF2(_L("CAmaStart->StartL(TUid::Uid(0x10000011)) finished with return code '%d'"), ret);

	iActiveSchedulerWait->Start();

	// Check and compare results as they have been stored in the result log file
	CompareResultsL(_L("tamastarter_result_1"), _L("tamastarter_app1"), 0, 1, 1, 0, EFalse);
	CompareResultsL(_L("tamastarter_result_1"), _L("tamastarter_app2"), 1, 2, 1, 0, EFalse);
	CompareResultsL(_L("tamastarter_result_1"), _L("tamastarter_app3"), 0, 1, 1, 0, EFalse);

	// clean-up
	CleanupStack::PopAndDestroy(2, testPropertyAmastart); //testPropertyAmastart, amaStart
	}

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
/**
	@SYMTestCaseID 		APPFWK-AMA-0006
	@SYMTestCaseDesc	Check if AMA can be started using LoadAmaStart DLL.
	@SYMPREQ 			PREQ1871
	@SYMREQ				REQ9141
	@SYMTestType		UT
	@SYMTestPriority	Critical
	@SYMTestStatus 		Implemented
	@SYMTestCaseDependencies	The appropriate database has been created.
	@SYMTestActions		The test uses a known DSC store database with valid items in it.
						Then the test uses CLoadAmaStart::StartL() with the appropriate item id to start these AMAs
						from an existing DSC item in the created test database.
	@SYMTestExpectedResults 	The different AMAs are started as expected from the test database.
								The application logs are read to check the applications were launched as expected.
*/
void CAppfwkAmaStarterTestStepStart::TestCase2L()
	{
	CTestAmastartAppObserver* testPropertyAmastart = CTestAmastartAppObserver::NewL(iActiveSchedulerWait);
	CleanupStack::PushL(testPropertyAmastart);
	testPropertyAmastart->StartL();

	// Launch the appropriate AMAStarter(s) via CAmaStart::StartL()
	RLibrary myAmaStartLib;
	CSsmLoadAmaStart* myAmaStart;
	User::LeaveIfError(myAmaStartLib.Load(KAmaStartDLL));
	CleanupClosePushL(myAmaStartLib);
	TFuncCreateL amaStart = reinterpret_cast<TFuncCreateL>(myAmaStartLib.Lookup(1));
	TInt err = NULL==amaStart?KErrGeneral:KErrNone;
	User::LeaveIfError(err);
	myAmaStart = amaStart();
	CleanupStack::PushL(myAmaStart);

	TRAPD(ret, myAmaStart->StartL(TUid::Uid(0x10000011)));
	TEST(ret == KErrNone);
	INFO_PRINTF2(_L("CLoadAmaStart->StartL(TUid::Uid(0x10000011)) finished with return code '%d'"), ret);

	iActiveSchedulerWait->Start();

	// Check and compare results as they have been stored in the result log file
	CompareResultsL(_L("tamastarter_result_1"), _L("tamastarter_app1"), 0, 1, 1, 0, EFalse);
	CompareResultsL(_L("tamastarter_result_1"), _L("tamastarter_app2"), 1, 2, 1, 0, EFalse);
	CompareResultsL(_L("tamastarter_result_1"), _L("tamastarter_app3"), 0, 1, 1, 0, EFalse);

	// clean-up
	CleanupStack::PopAndDestroy(3, testPropertyAmastart); //myAmaStart, myAmaStartLib, testPropertyAmastart
	}
#endif // SYMBIAN_SYSTEM_STATE_MANAGEMENT

/**
	@SYMTestCaseID 		APPFWK-AMASTARTER-0013
	@SYMTestCaseDesc	Check for Handle leaks in CAmaStart::StartL().
	@SYMDEF 			DEF119802
	@SYMTestType		UT
	@SYMTestPriority	Critical
	@SYMTestStatus 		Implemented
	@SYMTestCaseDependencies	The appropriate database has been created.
	@SYMTestActions		The test uses a known DSC store database with valid items in it.
						Then the test uses CAmaStart::StartL() with the appropriate item id to start these AMAs
						from an existing DSC item in the created test database. The test case checks
						for the no. of open handles in the process owning this thread and the
						no. of open handles in this thread.
	@SYMTestExpectedResults 	The no. of open handles in process owning this thread and no. of open handles in this thread should
							be same before calling CAmaStart::StartL() and after destroying CAmaStart object.
*/
void CAppfwkAmaStarterTestStepStart::TestCase3L()
	{
	CTestAmastartAppObserver* testPropertyAmastart = CTestAmastartAppObserver::NewL(iActiveSchedulerWait);
	CleanupStack::PushL(testPropertyAmastart);
	testPropertyAmastart->StartL();

	// Launch the appropriate AMAStarter(s) via CAmaStart::StartL()
  	TInt threadHandles_BeforeAmaStart;
	TInt processHandles_BeforeAmaStart;
	RThread().HandleCount(processHandles_BeforeAmaStart, threadHandles_BeforeAmaStart);
	INFO_PRINTF2(_L("No. of open handles in process which owns this process %d before calling CAmaStart::StartL"),processHandles_BeforeAmaStart);
	INFO_PRINTF2(_L("No. of open handles in this thread %d before calling CAmaStart::StartL"),threadHandles_BeforeAmaStart);

	CAmaStart* amaStart = CAmaStart::NewL();
	CleanupStack::PushL(amaStart);

	TRAPD(ret, amaStart->StartL(TUid::Uid(0x10000018)));
	TEST(ret == KErrNone);
	INFO_PRINTF2(_L("CAmaStart->StartL(TUid::Uid(0x10000018)) finished with return code '%d'"), ret);

	iActiveSchedulerWait->Start();

	// Check and compare results as they have been stored in the result log file
	CompareResultsL(_L("tamastarter_result_7"), _L("tamastarter_app1"), 0, 1, 1, 0, EFalse);
	CompareResultsL(_L("tamastarter_result_7"), _L("tamastarter_app3"), 0, 1, 1, 0, EFalse);

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

	CleanupStack::PopAndDestroy(testPropertyAmastart);
	}

//---------

CAppfwkAmaStarterTestStepStart::CAppfwkAmaStarterTestStepStart(const TDesC& aStepName)
	{	
	SetTestStepName(aStepName);
	}


CAppfwkAmaStarterTestStepStart::~CAppfwkAmaStarterTestStepStart()
	{
	delete iActiveSchedulerWait;
	delete iActiveScheduler;
	}


TVerdict CAppfwkAmaStarterTestStepStart::doTestStepL()
	{	
	INFO_PRINTF1(_L("AMAStarter StartL tests started...."));

	//Create and install active scheduler
	iActiveScheduler = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install (iActiveScheduler);
	iActiveSchedulerWait = new(ELeave) CActiveSchedulerWait;

	TInt err = RProperty::Define(KTestAmastartCategory, KTestAmastartKey, RProperty::EInt , KReadPolicy, KWritePolicy);
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

	RSmlTestUtils fs;
	CleanupClosePushL(fs);
	User::LeaveIfError(fs.Connect());

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	INFO_PRINTF1(_L("Delete log files created to store temporary results"));
	User::LeaveIfError(fs.DeleteFileUsingWildcardL(KTestLogFileWild));

	TRAP(ret, TestCase2L());
	TESTE(ret == KErrNone, ret);
	INFO_PRINTF2(_L("TestCase2L() finished with code '%d'\n"), ret);
#endif // SYMBIAN_SYSTEM_STATE_MANAGEMENT

	TRAP(ret, TestCase3L());
	TESTE(ret == KErrNone, ret);
	INFO_PRINTF2(_L("TestCase3L() finished with code '%d'\n"), ret);

	INFO_PRINTF1(_L("Restore DBMS / SQL data store backups available prior to running the test"));
	TRAP(ret, DscDatabaseRestoreL());
	TESTE(ret == KErrNone, ret);

	INFO_PRINTF1(_L("Delete log files created to store temporary results"));
	User::LeaveIfError(fs.DeleteFileUsingWildcardL(KTestLogFileWild));
	CleanupStack::PopAndDestroy(&fs);

	__UHEAP_MARKEND;
	INFO_PRINTF1(_L("...AMAStarter StartL tests finished!!"));

	return TestStepResult();
	}
