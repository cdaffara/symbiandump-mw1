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
//

#include "CITAlarmTestServer.h"
#include "TestMultipleAlarmsStep.h"
#include "TestAlarmPlayStep.h"

_LIT(KServerName,"T_PREQ1118_TestSuite");

CCITAlarmTestServer* CCITAlarmTestServer::NewL()
/**
 * @return - Instance of the test server
 * Called inside the MainL() function to create and start the
 * CTestServer derived server.
 */
	{
	CCITAlarmTestServer* server = new (ELeave) CCITAlarmTestServer();
	CleanupStack::PushL(server);
	
	// Either use a StartL or ConstructL, the latter will permit
	// Server Logging.

	server-> ConstructL(KServerName);
	CleanupStack::Pop(server);
	return server;
	}


LOCAL_C void MainL()
/**
 * Uses the new Rendezvous() call to sync with the client
 */
	{
	// Leave the hooks in for platform security
#if (defined __DATA_CAGING__)
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().SecureApi(RProcess::ESecureApiOn);
#endif
	CActiveScheduler* sched = NULL;
	sched = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CCITAlarmTestServer* server = NULL;
	// Create the CTestServer derived server
	TRAPD(err,server = CCITAlarmTestServer::NewL());
	if(KErrNone == err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	delete server;
	delete sched;
	}


GLDEF_C TInt E32Main()
/**
 * @return - Standard Epoc error code on exit
 */
	{
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
	TRAPD(err,MainL());
	
	if (err != KErrNone)
	    {
	    // Error	
	    }	
	    
	delete cleanup;
	return KErrNone;
    }
    

CTestStep* CCITAlarmTestServer::CreateTestStep(const TDesC& aStepName)
/**
 * @return - A CTestStep derived instance
 * Implementation of CTestServer pure virtual
 */
	{
	CTestStep* testStep = NULL;

    if (aStepName == KTestMultipleAlarms)
        {
	    testStep = new CTestMultipleAlarmsStep();
        }  
    else if (aStepName == KTesSameTimeMultiAlarms)
        {
    	testStep = new CTestSameTimeMultiAlarmsStep();
        }
    else if (aStepName == KTestMultiWithSnooze)
        {
    	testStep = new CTestMultipleAlarmsSnoozeStep();
        }
    else if (aStepName == KTestMultiWithInvalidSnooze)
        {
    	testStep = new CTestMultiAlarmsInvalidSnoozeStep();
        }
    else if (aStepName == KTestPauseSound)
        {
    	testStep = new CTestMultiAlarmsPauseSoundStep();
        }
    else if (aStepName == KTestClear)
        {
    	testStep = new CTestMultipleAlarmsClearStep();
        }
    else if (aStepName == KTestInvalidClear)
        {
    	testStep = new CTestMultiAlarmsInvalidClearStep();
        }
    else if (aStepName == KTestQueue)
        {
    	testStep = new CTestMultipleAlarmsQueueStep();
        }
    else if (aStepName == KTestSilenceStep)
        {
    	testStep = new CTestSilenceAlarmStep();
        }
    else if (aStepName == KBackwardsComp)
        {
    	testStep = new CTestBackwardsCompatibilityStep();
        }
    else if (aStepName == KTestDefPlay)
        {
    	testStep = new CTestDefaultAlarmPlayStep();
        }
    else if (aStepName == KTestPlayChange)
        {
    	testStep = new CTestChangePlayIntervalStep();
        }
    else if (aStepName == KTestPlayInvalid)
        {
    	testStep = new CTestInvalidIntervalStep();
        }
    else if (aStepName == KTestLastPlay)
        {
    	testStep = new CTestRepeatLastIntervalStep();
        }
    else if (aStepName == KTestStopPlay)
        {
    	testStep = new CTestStopPlayStep();
        }
    else if (aStepName == KTestDisable)
        {
    	testStep = new CTestDisablePlayStep();
        }
    else if (aStepName == KTestOverride)
        {
    	testStep = new CTestOverrideResourceStep();
        }
        
	return testStep;
	}
