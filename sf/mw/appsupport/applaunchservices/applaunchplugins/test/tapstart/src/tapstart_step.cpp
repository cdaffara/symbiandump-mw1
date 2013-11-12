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

#include <apgtask.h>

#include <apastarter.h>
#include "tapstart_step.h"
#include "testapps.h"

_LIT(KApStartDLL, "apstart.dll");
typedef CApaStarter* (*TFuncNewL)();

CApStartTestStep::CApStartTestStep()
	{
	SetTestStepName(KCTestCaseApStart);
	iAppStart = NULL;	
	}

CApStartTestStep::~CApStartTestStep()
	{
	if(iAppStart != NULL)
		{
		delete iAppStart;
		iAppStart = NULL;
		}
	iLibrary.Close();
	}

TVerdict CApStartTestStep::doTestStepPreambleL()
	{
	// Load the CApaStarter implementation
	User::LeaveIfError(iLibrary.Load(KApStartDLL)); // load the library
	TLibraryFunction ord1 = iLibrary.Lookup(1); // This is the NewL function
	TFuncNewL funcL = reinterpret_cast<TFuncNewL>(ord1);
	iAppStart = static_cast<CApaStarter*> (funcL());
	
	return CTestStep::doTestStepPreambleL();	
	}

TVerdict CApStartTestStep::doTestStepPostambleL()
	{
 	return CTestStep::doTestStepPostambleL();
	}

TVerdict CApStartTestStep::doTestStepL()
	{
	INFO_PRINTF1(_L("CApStartTestStep started....\n"));
	__UHEAP_MARK;

	RWsSession wsSession;
	User::LeaveIfError(wsSession.Connect());
	CleanupClosePushL(wsSession);	
	
	TRAPD(err, TestStartAndForgetL(wsSession));
	TESTE(KErrNone == err, err);

	TRAP(err, TestStartAndFollowUpL(wsSession));
	TESTE(KErrNone == err, err);
	
	CleanupStack::PopAndDestroy();	//wsSession.Close();
	
	TRAP(err, TestInitApparcL());
	TESTE(KErrNone == err, err);

	__UHEAP_MARKEND; 
	INFO_PRINTF1(_L(".... CApStartTestStep finished!!"));
	
	return TestStepResult();
	}

/** 
 * @SYMTestCaseID	APPFWK-APSTART-0001
 * @SYMPREQ			PREQ1089 PREQ1871
 * @SYMTestType		UT
 * @SYMTestPriority	Critical
 * @SYMTestStatus	Implemented
 * @SYMTestCaseDesc	Check if an application can be loaded with the StartApp method 
 * 					which do not take a TRequestStatus.
 * @SYMTestActions	Load apstart.dll dynamically and create an object for CAppStart. 
 * 					Use the CApStarter::StartL() function which do not take a TRequestStatus 
 * 					to load TestApp. Verify that the application has been loaded.
 * @SYMTestExpectedResults 	Application should be started with no errors.
 */
void CApStartTestStep::TestStartAndForgetL(RWsSession& aWsSession)
	{
	INFO_PRINTF1(_L("Entering test case: APPFWK-APSTART-0001"));
	
	TApaTask task = TestStartL(NULL, aWsSession);
	
	if(task.Exists())
		{
		task.KillTask();
		INFO_PRINTF1(_L("Leaving test case: APPFWK-APSTART-0001\n"));
		}
	else
		{
		ERR_PRINTF1(_L("APPFWK-APSTART-0001 failed\n"));  
		}
	}

/**
 * @SYMTestCaseID	APPFWK-APSTART-0002
 * @SYMPREQ			PREQ1089 PREQ1871
 * @SYMTestType		UT
 * @SYMTestPriority	Critical
 * @SYMTestStatus	Implemented
 * @SYMTestCaseDesc	Check if an application can be loaded with the apstart API call that 
 * 					takes a TRequestStatus parameter.
 * @SYMTestActions	Load apstart.dll dynamically and create an object for CApStart. 
 * 					Use CApStart::StartL(...TRequestStatus) function to load TestApp. 
 * 					Call User::WaitForRequest to check the loading of the application.
 * @SYMTestExpectedResults 	Application should be loaded with no errors.
 */
void CApStartTestStep::TestStartAndFollowUpL(RWsSession& aWsSession)
	{
	INFO_PRINTF1(_L("Entering test case: APPFWK-APSTART-0002"));	
	
	TRequestStatus status;
	TApaTask task = TestStartL(&status, aWsSession);
	TESTE(KErrNone == status.Int(), status.Int());
	
	if(task.Exists())
		{
		task.KillTask();
		INFO_PRINTF1(_L("Leaving test case: APPFWK-APSTART-0002\n"));
		}
	else
		{
		ERR_PRINTF1(_L("APPFWK-APSTART-0002 failed\n"));  
		}
	}

/**
 * @SYMTestCaseID	APPFWK-APSTART-0003
 * @SYMPREQ			PREQ1089 PREQ1871
 * @SYMTestType		UT
 * @SYMTestPriority	Critical
 * @SYMTestStatus	Implemented
 * @SYMTestCaseDesc	Check for successful initialization of AppArc's CApaAppList.
 * @SYMTestActions	Load apstart.dll dynamically and create an object for CApStart. 
 * 					Call CApStart::WaitForApparcToInitializeL() and make sure it doesn't leave.
 * @SYMTestExpectedResults 	The function should not leave.
 */
void CApStartTestStep::TestInitApparcL()
	{ // Leavescan, this implementation of this method does not leave but further derived implementations may if they choose
	INFO_PRINTF1(_L("Entering test case: APPFWK-APSTART-0003"));
	INFO_PRINTF1(_L("Going to sleep until Apparc finished initialisation."));
	TRAPD(err, iAppStart->WaitForApparcToInitialiseL());
	TEST(err == KErrNone);
	if(err)
		ERR_PRINTF2(_L("APPFWK-APSTART-0003 finished with errorcode = '%d'\n"), err);  
	else
		INFO_PRINTF1(_L("Leaving test case: APPFWK-APSTART-0003\n"));
	}

/**
 * Common implementation for test case 0001 and 0002. See INFO_PRINTs for further information.
 */
TApaTask CApStartTestStep::TestStartL(TRequestStatus* aRequestStatus, RWsSession& aWsSession)
	{
	INFO_PRINTF1(_L("Requesting start of KApstartTestAppGood in foreground."));
	const TDesC& cmdLineParams = KNullDesC;
	const TBool viewLess = EFalse;
	const TBool startInBackground = EFalse;
	TThreadId tId;
	if(aRequestStatus)
		{
		iAppStart->StartAppL(KApstartTestAppGood, cmdLineParams, viewLess, startInBackground, tId, *aRequestStatus);
		}
	else
		{
		iAppStart->StartAppL(KApstartTestAppGood, cmdLineParams, viewLess, startInBackground, tId);
		}
		
	RThread thread;
	RProcess process;
	INFO_PRINTF1(_L("Verifying that the process was created."));
	TEST(KErrNone == thread.Open(tId));
	TEST(KErrNone == thread.Process(process)); 
	
	INFO_PRINTF1(_L("Waiting for KApstartTestAppGood to resume."));
	if(aRequestStatus)
		{
		User::WaitForRequest(*aRequestStatus);
		}
	else
		{
		TRequestStatus status;
		process.Rendezvous(status);
		User::WaitForRequest(status);
		}
	process.Close();
	thread.Close();
	
	INFO_PRINTF1(_L("Verifying that KApstartTestAppGood is now up and running."));
	TApaTaskList taskList(aWsSession); 
	const TUid appUid = {KApstartTestAppGoodUid};
	TApaTask task = taskList.FindApp(appUid);
	TEST(task.Exists());
	
	return task;
	}
