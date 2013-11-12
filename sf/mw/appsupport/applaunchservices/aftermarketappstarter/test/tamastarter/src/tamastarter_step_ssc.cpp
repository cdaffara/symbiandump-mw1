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
// NOTE: When testing using sysstart, a test SSC resource file will be required that will contain the necessary AMAs for the test. 
// In addition, the SSC resource fill will contain an entry for an extra application that will create the necessary 
// DSC database store (tamastarter_amaadder.exe).
// The tests will certify that the applications specified in the SSC file via START_AMA_STARTER are launched appropriately.
// As a pre-requisite, tamastarter_amaadder will create these DSC items in the database, so when the START_AMC_STARTER
// structure is found the appropriate action is taken. Please note that this means that tamastarter_amaadder.exe 
// is required to be in the SSC startup resource file itself.
// It will be the SSC file, together with the database created by tamastarter_amaadder that will execute all the actions
// under these test cases, ie, launch applications. These tests cases here will just verify that all applications were
// executed according to the specifications by checking the log files produced.
//

// #ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
// This test will be run twice, 
// a) using the fallback mechanism to boot techview using sysstart. Resource file follows SysStart format.
// b) using new SSM architecture to boot techview. Resource file follows SSM format.		
// #endif
//

/**
 @file
 @test
 @internalComponent - Internal Symbian test code 
*/

#include "appfwk_test_utils.h"
#include "appfwk_test_appui.h"

#include "tamastarter_constants.h"
#include "tamastarter_database_utils.h"
#include "tamastarter_step_ssc.h"



//----------------------


void CAppfwkAmaStarterTestStepSsc::CompareResultsL(const TDesC& aResultFileName, const TDesC& aAmcName, TInt aRetryCntr, TInt aLaunchCntr, TInt aSuccessCntr, TInt aPanicCntr, TBool aMonitored)
	{	
	INFO_PRINTF3(_L("Comparing results for filename '%S', section '%S'"), &aResultFileName, &aAmcName);
	
	TBuf<KMaxFileName>	fileName(KTestAppResultsPath);
	fileName.Append(aResultFileName);
	fileName.Append(KTestLogFileExtension);
	
	CIniData* resultFile = CIniData::NewL(fileName);	
	CleanupStack::PushL(resultFile);
	
	// Get the actual counter values from the result log file
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
	@SYMTestCaseID 		APPFWK-AMASTARTER-0001
	@SYMTestCaseDesc	To check correct launch of AMCs with and without any attempt to retry.
	@SYMPREQ 			PREQ871
	@SYMREQ				REQ9141
	@SYMTestType		CIT
	@SYMTestPriority	Critical
	@SYMTestStatus 		Implemented
	@SYMTestCaseDependencies	tamastarter_amaadder has been launched as part of a modified test SSC resource file.
	@SYMTestActions		Add the Dsc item(s) for the test AMC(s) to be launched at start up, with following attributes: 
						Start Method as EWaitForStart, a valid timeout interval, a valid launch retry count and 
						start up type as EStartupProcess and for EStartupApp.
						Make few test AMC(s) EXE(s) to wait for longer than timeout interval when AMAStarter tries to 
						launch it for the first time.
	@SYMTestExpectedResults 	The test AMC EXE should be launched successfully within the timeout interval without any retries.
								The test AMC EXE which is timed out should be launched successfully after first retry.
*/
void CAppfwkAmaStarterTestStepSsc::TestCase1L()
	{
	// Check and compare results as they have been stored in the result log file
	CompareResultsL(_L("tamastarter_result_1"), _L("tamastarter_app1"), 0, 1, 1, 0, EFalse);
	CompareResultsL(_L("tamastarter_result_1"), _L("tamastarter_app2"), 1, 2, 1, 0, EFalse);
	CompareResultsL(_L("tamastarter_result_1"), _L("tamastarter_app3"), 0, 1, 1, 0, EFalse);
	}


/**
	@SYMTestCaseID 		APPFWK-AMASTARTER-0002
	@SYMTestCaseDesc	To check failing to launch AMC in case of timeout, system moves on to launch next AMC in DSC.
	@SYMPREQ 			PREQ871
	@SYMREQ				REQ9141
	@SYMTestType		CIT
	@SYMTestPriority	Critical
	@SYMTestStatus 		Implemented
	@SYMTestCaseDependencies	tamastarter_amaadder has been launched as part of a modified test SSC resource file.
	@SYMTestActions		Add the Dsc item(s) for the test AMC(s) to be launched at start up, with following attributes: 
						Start Method as EWaitForStart, a timeout interval, a valid launch retry count and 
						start up type as EStartupProcess and EStartupApp.
						Add the Dsc item(s) for the test AMC(s) to be launched at start up, with following attributes: 
						Start Method as EFireAndForget, and start up type as EStartupProcess and for EStartupApp.
						Make the test AMC EXE launch timed out when AMAStarter tries to launch it.
	@SYMTestExpectedResults 	The AMC(s) which neither timed out nor panic should be launched successfully. 
								The AMC(s) which either timed out or panics should not affect launch of other AMCs in the DSC.
*/
void CAppfwkAmaStarterTestStepSsc::TestCase2L()
	{
	// Check and compare results as they have been stored in the result log file
	CompareResultsL(_L("tamastarter_result_2_1"), _L("tamastarter_app1"), 1, 1, 0, 0, EFalse);
	CompareResultsL(_L("tamastarter_result_2_2"), _L("tamastarter_app2"), 0, 1, 1, 0, EFalse);
	CompareResultsL(_L("tamastarter_result_2_3"), _L("tamastarter_app3"), 0, 1, 1, 0, EFalse);
	CompareResultsL(_L("tamastarter_result_2_4"), _L("tamastarter_app4"), 0, 1, 1, 0, EFalse);
	}


/**
	@SYMTestCaseID 		APPFWK-AMASTARTER-0003
	@SYMTestCaseDesc	To check if launching AMC panics, system moves on to launch next AMC in DSC.
	@SYMPREQ 			PREQ871
	@SYMREQ				REQ9141
	@SYMTestType		CIT
	@SYMTestPriority	Critical
	@SYMTestStatus 		Implemented
	@SYMTestCaseDependencies	tamastarter_amaadder has been launched as part of a modified test SSC resource file.
	@SYMTestActions		Repeat the test APPFWK-AMASTARTER-0002, instead of waiting longer than timeout interval, 
						to fail the launch, provide the invalid AMC path and make the AMC(s) panic during startup.
	@SYMTestExpectedResults 	All the other AMCs apart from the one which panics, in all the DSCs are launched properly. 
								Appropriate logging information is provided for the failed AMC launch.
*/
void CAppfwkAmaStarterTestStepSsc::TestCase3L()
	{
	// Check and compare results as they have been stored in the result log file
	CompareResultsL(_L("tamastarter_result_3_1"), _L("tamastarter_app1"), 0, 2, 0, 2, EFalse);
	CompareResultsL(_L("tamastarter_result_3_2"), _L("tamastarter_app2"), 0, 1, 1, 0, EFalse);
	CompareResultsL(_L("tamastarter_result_3_3"), _L("tamastarter_app3"), 0, 1, 1, 0, EFalse);
	CompareResultsL(_L("tamastarter_result_3_4"), _L("tamastarter_app4"), 0, 1, 1, 0, EFalse);
	}


/**
	@SYMTestCaseID 		APPFWK-AMASTARTER-0004
	@SYMTestCaseDesc	To check all AMCs in multiple DSCs are launched starting AMAStarter multiple times
	@SYMPREQ 			PREQ871
	@SYMREQ				REQ9141
	@SYMTestType		CIT
	@SYMTestPriority	Critical
	@SYMTestStatus 		Implemented
	@SYMTestCaseDependencies	tamastarter_amaadder has been launched as part of a modified test SSC resource file.
	@SYMTestActions		Create multiple DSCs having duplicate AMCs as well. Use KDefaultSymbianDsc for one of the DSC.
	@SYMTestExpectedResults 	All the AMCs in all the DSCs are launched. AMCs in the default DSC are launched in the last.
*/
void CAppfwkAmaStarterTestStepSsc::TestCase4L()
	{
	// Check and compare results as they have been stored in the result log file
	CompareResultsL(_L("tamastarter_result_4"), _L("tamastarter_app1"), 0, 3, 3, 0, EFalse);
	}
	

/**
	@SYMTestCaseID 		APPFWK-AMASTARTER-0005
	@SYMTestCaseDesc	To ensure that AMC can register for process failure monitoring.
	@SYMPREQ 			PREQ871
	@SYMREQ				REQ9141
	@SYMTestType		CIT
	@SYMTestPriority	Critical
	@SYMTestStatus 		Implemented
	@SYMTestCaseDependencies	tamastarter_amaadder has been launched as part of a modified test SSC resource file.
	@SYMTestActions		Add the Dsc item for the test AMC to be launched at start up and to be monitored, 
						with following attributes: 
						Start Method as EWaitForStart, and start up type as EStartupProcess.
						Check the recovery method for the AMC. Should be EIgnoreProcessFailure.
						Make the test AMC EXE to register for process failure monitoring.
	@SYMTestExpectedResults 	The test AMC EXE should be launched and registered for monitoring.
*/
void CAppfwkAmaStarterTestStepSsc::TestCase5L()
	{
	// Check and compare results as they have been stored in the result log file
	CompareResultsL(_L("tamastarter_result_5_1"), _L("tamastarter_app1"), 0, 1, 1, 0, ETrue);
	CompareResultsL(_L("tamastarter_result_5_2"), _L("tamastarter_app2"), 0, 1, 1, 0, ETrue);
	CompareResultsL(_L("tamastarter_result_5_3"), _L("tamastarter_app3"), 0, 1, 1, 0, ETrue);
	CompareResultsL(_L("tamastarter_result_5_4"), _L("tamastarter_app4"), 0, 1, 1, 0, ETrue);

	CompareDscItemsL(TUid::Uid(0x10000015), KAMCFileNameApp1, _L("-Log tamastarter_result_5_1 -Re 0"), EStartProcess, EWaitForStart, ERetry0, 5000, ETrue, EFalse, EFalse);
	CompareDscItemsL(TUid::Uid(0x10000015), KAMCFileNameApp2, _L("-Log tamastarter_result_5_2 -Re 0"), EStartApp, EWaitForStart, ERetry0, 5000, ETrue, ETrue, ETrue);
	CompareDscItemsL(TUid::Uid(0x10000015), KAMCFileNameApp3, _L("-Log tamastarter_result_5_3"), EStartProcess, EFireAndForget, ERetry0, 5000, ETrue, ETrue, EFalse);
	CompareDscItemsL(TUid::Uid(0x10000015), KAMCFileNameApp4, _L("-Log tamastarter_result_5_4"), EStartApp, EFireAndForget, ERetry0, 5000, ETrue, EFalse, EFalse);
	}


/*
	@SYMTestCaseID 		APPFWK-AMASTARTER-0006
	@SYMTestCaseDesc	To check if launching AMAStarter at critical-static, critical-dynamic, Non-critical stages is successful.
	@SYMPREQ 			PREQ871
	@SYMREQ				REQ9141
	@SYMTestType		CIT
	@SYMTestPriority	Critical
	@SYMTestStatus 		Implemented
	@SYMTestCaseDependencies	tamastarter_amaadder has been launched as part of a modified test SSC resource file.
	@SYMTestActions		Create the START_AMC_STARTER structure at critical-static, critical-dynamic, Non-critical-dynamic 
						stages with a valid DSC Id.
	@SYMTestExpectedResults 	The system should process all the DSCs mentioned at all start-up stages mentioned.
*/
void CAppfwkAmaStarterTestStepSsc::TestCase6L()
	{
	// Check and compare results as they have been stored in the result log file
	CompareResultsL(_L("tamastarter_result_6"), _L("tamastarter_app1"), 0, 3, 3, 0, ETrue);
	}



//---------

CAppfwkAmaStarterTestStepSsc::CAppfwkAmaStarterTestStepSsc(const TDesC& aStepName)
	{	
	SetTestStepName(aStepName);
	}


CAppfwkAmaStarterTestStepSsc::~CAppfwkAmaStarterTestStepSsc()
	{	
	}


TVerdict CAppfwkAmaStarterTestStepSsc::doTestStepL()
	{	
	INFO_PRINTF1(_L("AMAStarter SSC tests started...."));

 	__UHEAP_MARK;
 	
	// Run the test cases
	TRAPD(ret, TestCase1L());
	TESTE(ret == KErrNone, ret);
	INFO_PRINTF2(_L("TestCase1L() finished with code '%d'\n"), ret);

	TRAP(ret, TestCase2L());
	TESTE(ret == KErrNone, ret);
	INFO_PRINTF2(_L("TestCase2L() finished with code '%d'\n"), ret);

	TRAP(ret, TestCase3L());
	TESTE(ret == KErrNone, ret);
	INFO_PRINTF2(_L("TestCase3L() finished with code '%d'\n"), ret);

	TRAP(ret, TestCase4L());
	TESTE(ret == KErrNone, ret);
	INFO_PRINTF2(_L("TestCase4L() finished with code '%d'\n"), ret);

	TRAP(ret, TestCase5L());
	TESTE(ret == KErrNone, ret);
	INFO_PRINTF2(_L("TestCase5L() finished with code '%d'\n"), ret);

	TRAP(ret, TestCase6L());
	TESTE(ret == KErrNone, ret);
	INFO_PRINTF2(_L("TestCase6L() finished with code '%d'\n"), ret);

	// Restore DBMS / SQL data store backups available prior to running the test
	TRAP(ret, DscDatabaseRestoreL());
	TESTE(ret == KErrNone, ret);
	
	// Delete log files created to store temporary results
	RSmlTestUtils fs;
	CleanupClosePushL(fs);
	User::LeaveIfError(fs.Connect());
	User::LeaveIfError(fs.DeleteFileUsingWildcardL(KTestLogFileWild));
	CleanupStack::PopAndDestroy(&fs);

	// Close All panic windows that appear from negative testing conditions in this test
	CloseAllPanicWindowsL();

	__UHEAP_MARKEND;

	INFO_PRINTF1(_L("...AMAStarter SSC tests finished!!"));

	return TestStepResult();
	}

