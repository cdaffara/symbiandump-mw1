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

#include "TestCITAlarmServerSuiteServer.h"
#include "TestUTCChangeFor.h"
#include "TestUTCChangeUntil.h"
#include "TestSystemChangeFor.h"
#include "TestSystemChangeUntil.h"
#include "TestCITAlarmServer.h"
#include "TestCITAlarmServerSuiteDefs.h"

_LIT(KServerName,"TestCITAlarmServerSuite");

CTestCITAlarmServerSuite* CTestCITAlarmServerSuite::NewL()
/**
 * @return - Instance of the test server
 * Same code for Secure and non-secure variants
 * Called inside the MainL() function to create and start the
 * CTestServer derived server.
 */
	{
	CTestCITAlarmServerSuite * server = new (ELeave) CTestCITAlarmServerSuite();
	CleanupStack::PushL(server);
	// CServer base class call which can be either StartL or ConstructL,
	// the later will permit Server Logging.

	server->StartL(KServerName);
	CleanupStack::Pop(server);
	return server;
	}

// Secure variants much simpler
// For EKA2, just an E32Main and a MainL()
LOCAL_C void MainL()
/**
 * Secure variant
 * Much simpler, uses the new Rendezvous() call to sync with the client
 */
	{
	CActiveScheduler* sched=NULL;
	sched=new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CTestCITAlarmServerSuite* server = NULL;
	// Create the CTestServer derived server
	TRAPD(err,server = CTestCITAlarmServerSuite::NewL());
	
	if(!err)
		{
		RProcess::Rendezvous(KErrNone);

#ifdef __WINS__ 
		// Automated hardware testing is run on a Techview image,
		// but automated emulator testing is run in TEXTSHELL
		// mode. Connecting to the alarm server without an alert server
		// running will fail, so it is necessary to start the console
		// alarm alert server in TEXTSHELL mode, as the 'real' alert server
		// is started by Techview.
		
		// Start console alarm alert server so it does not
		// acknowledge, alarms...
		RProcess consoleAlertServer;
		_LIT(KConsoleAlarmAlertServerImg,"ConsoleAlarmAlertServer");
		const TUid KServerUid2={0x1000008D};
		const TUid KServerUid3={0x101F502A};
		const TUidType KServerUid(KNullUid,KServerUid2,KServerUid3);
		consoleAlertServer.Create(KConsoleAlarmAlertServerImg, KNullDesC(), KServerUid);	
		TRequestStatus stat;
		consoleAlertServer.Rendezvous(stat);
		if (stat != KRequestPending)
			{
			consoleAlertServer.Kill(0);
			}
		else
			{
			consoleAlertServer.Resume();
			}
		User::WaitForRequest(stat);
		TInt result = stat.Int();
		if(result != KErrNone && result != KErrAlreadyExists)
			{
			User::LeaveIfError(result);	
			}
						
#endif //WINS

		sched->Start();		
		}
	delete server;
	delete sched;
	}

GLDEF_C TInt E32Main()
/**
 * @return - Standard Epoc error code on process exit
 * Secure variant only
 * Process entry point. Called by client using RProcess API
 */
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
	TRAP_IGNORE(MainL());
	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
    }

CTestStep* CTestCITAlarmServerSuite::CreateTestStep(const TDesC& aStepName)
/**
 * @return - A CTestStep derived instance
 * Secure and non-secure variants
 * Implementation of CTestServer pure virtual
 */
	{
	CTestStep* testStep = NULL;

    if(aStepName == KTestUTCChangeFor)
    	{
    	testStep = new CTestUTCChangeFor();
    	}
    else if (aStepName == KTestUTCChangeUntil)
    	{
    	testStep = new CTestUTCChangeUntil();
    	}
    else if (aStepName == KTestSystemChangeFor)
    	{
    	testStep = new CTestSystemChangeFor();
    	}
    else if (aStepName == KTestSystemChangeUntil)
    	{
    	testStep = new CTestSystemChangeUntil();
    	}
    else if (aStepName == KTestCITAlarmServer)
    	{
    	testStep = new CTestCITAlarmServer();
    	}
    	
	return testStep;
	}
