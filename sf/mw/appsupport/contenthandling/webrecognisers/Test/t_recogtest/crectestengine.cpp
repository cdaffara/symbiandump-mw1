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

#include "crectestengine.h"
#include "crectestlogger.h"

_LIT(KTxtRecogTestTitle, "Application-Protocols Recognisers Test Harness");
_LIT8(KTxtSeparator, "********************************************");

CRecTestEngine* CRecTestEngine::NewL()
	{
	CRecTestEngine* self = new(ELeave) CRecTestEngine();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
CRecTestEngine::~CRecTestEngine()
	{
	delete iLogger;
	iEcomSession.Close();
	iPluginArray.ResetAndDestroy();
	iTestArray.ResetAndDestroy();
	iFailedTests.ResetAndDestroy();
	iTest.Close();
	iAppArcSession.Close();
	}
	
CRecTestEngine::CRecTestEngine()
: iTest(KTxtRecogTestTitle())
	{
	iTest.Title();
	}
	
void CRecTestEngine::ConstructL()
	{
	// Create logger and connection to apparc
	iLogger = CRecTestLogger::NewL();
	User::LeaveIfError(iAppArcSession.Connect());
	
	// Open an ECom session and retrieve a list of all the test plugins bases
	// on the interface UID
	iEcomSession = REComSession::OpenL();
	iEcomSession.ListImplementationsL(KUidAppProtRecognisers, iPluginArray);
	
	TRecogTestConstructionParams params(&iAppArcSession, &iLogger->Logger());
	
	for(TInt ii=0; ii<iPluginArray.Count(); ++ii)
		{
		// Go through the plugin array and create test implementations
		// and store in test object array
		TUid pluginUid = iPluginArray[ii]->ImplementationUid();
		CEComRecogTest* testPlugin = CEComRecogTest::NewL(pluginUid, params);
		User::LeaveIfError(iTestArray.Append(testPlugin));	
		}
	}
	
void CRecTestEngine::StartTestsL()
	{	

	_LIT(KTxtSYMTestCaseID, "@SYMTestCaseID IWS-APPPROTOCOLS-RECOGNISERS-T_RECOGTEST-0001 ");
	iLogger->Logger().Write(KTxtSYMTestCaseID());

	// Loop through test plugins and execute each test case
	for(TInt ii=0; ii<iTestArray.Count(); ++ii)
		{
		_LIT(KTxtTestPluginTitle, "*** Running Test Cases from... %S ***\n");
		iTest.Printf(KTxtTestPluginTitle(), &iTestArray[ii]->TestPluginTitle());
		iLogger->Logger().WriteFormat(KTxtTestPluginTitle(), &iTestArray[ii]->TestPluginTitle());
		RunTestCasesFromPluginL(ii);	
		}
	
	// Log test summary
	iLogger->Logger().Write(KTxtSeparator());
	_LIT8(KTxtTestSummary, "*** TEST SUMMARY: Total Tests: %d, Passed: %d, Failed: %d ***");
	iLogger->Logger().WriteFormat(KTxtTestSummary(), iTestPassCount+iTestFailCount, iTestPassCount, iTestFailCount);
	
	// Log failed tests
	const TInt failedTestCount = iFailedTests.Count();
	if(failedTestCount>0)
		{
		_LIT(KTxtFailedTests, "Failed tests...");
		_LIT(KTxtFailedTestLists, "   Test %d - %S, failed with error %d");
		iLogger->Logger().Write(KTxtFailedTests());
		for(TInt ii=0; ii<failedTestCount; ++ii)
			{
			TFailedRecTest failedTest = *iFailedTests[ii];
			iLogger->Logger().WriteFormat(KTxtFailedTestLists(), failedTest.iTestNumber, &failedTest.iTestTitle, failedTest.iError);
			}
		}
	}

void CRecTestEngine::RunTestCasesFromPluginL(TInt aPluginIndex)
	{
	// Execute each test case in the specified plugin (aPluginIndex)
	const TInt testCaseCount = iTestArray[aPluginIndex]->TestCaseCount();
	for(TInt ii=0; ii<testCaseCount; ++ii)
		{
		++iCurrentTest;
		iTest.Start(iTestArray[aPluginIndex]->TestCaseTitle(ii));
		iLogger->Logger().Write(KTxtSeparator());
		_LIT(KTxtTestCaseTitle, "Running Test %d... %S");
		iCurrentTestTitle = &iTestArray[aPluginIndex]->TestCaseTitle(ii);
		iLogger->Logger().WriteFormat(KTxtTestCaseTitle(), iCurrentTest, iCurrentTestTitle);
		TRAPD(err, iTestArray[aPluginIndex]->RunTestCaseL(ii));
		ProcessTestResultL(err);
		iTest.End();
		}
	}
	
void CRecTestEngine::ProcessTestResultL(TInt aError)
	{
	if(aError==KErrNone)
		{
		_LIT(KTxtTestPassed, "TEST PASSED");
		iLogger->Logger().Write(KTxtTestPassed());
		++iTestPassCount;
		}
	else
		{
		// Add test to failed tests list
		TFailedRecTest* failedTest = new(ELeave) TFailedRecTest(iCurrentTest, iCurrentTestTitle, aError);
		CleanupStack::PushL(failedTest);
		User::LeaveIfError(iFailedTests.Append(failedTest));
		CleanupStack::Pop(failedTest);
		
		// Log and display failed test
		_LIT(KTxtTestFailed, "TEST FAILED with Error %d");
		iLogger->Logger().WriteFormat(KTxtTestFailed(), aError);
		_LIT(KTxtRTestFailed, "RTEST: FAIL : Failed with error %d\n");
		iTest.Printf(KTxtRTestFailed(), aError);
		++iTestFailCount;
		}
	}
