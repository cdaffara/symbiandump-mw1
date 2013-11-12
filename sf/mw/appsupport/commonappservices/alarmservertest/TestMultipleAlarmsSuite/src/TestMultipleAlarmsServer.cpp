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
// Contains implementation of CTestMultipleAlarmsServer class
// 
//

/**
 @file
 @internalTechnology 
*/

// User Includes
#include "AlarmControlsManager.h"

// Test Server
#include "TestMultipleAlarmsServer.h"

// Test Steps
#include "TestStartAlertServerStep.h"
#include "TestWaitStep.h"
#include "TestCreateCalEntriesStep.h"
#include "TestAlarmSnoozeStep.h"
#include "TestAlarmControlStateStep.h"
#include "TestAlarmSnoozeStep.h"
#include "TestAlarmClearStep.h"
#include "TestAssociatedDataStep.h"
#include "TestKillAlertServerStep.h"
#include "TestCleanupStep.h"
#include "TestPerformanceStep.h"
#include "TestCreateEntriesOOMStep.h"
#include "TestAlarmContentOOMStep.h"

// System Include
#include <e32base.h>

/*@{*/
// The system-wide unique name for the test-server
_LIT(KServerName, "TestMultipleAlarmsServer");
/*@}*/

/**
Static factory constructor. Creates and returns instance of the test server
@internalTechnology
@test
@return		A pointer to the newly created CTestMultipleAlarmsServer object
*/
CTestMultipleAlarmsServer*  CTestMultipleAlarmsServer::NewL()
	{
	// Construct the server
	CTestMultipleAlarmsServer* server = new(ELeave) CTestMultipleAlarmsServer();
	CleanupStack::PushL(server);

	server->ConstructL();
		
	CleanupStack::Pop(server);
	return server;
	}

/**
Second phase constructor. Calls base class onstructL.
And calls ShareAuto on the Logger session.
@internalTechnology
@test
*/
void CTestMultipleAlarmsServer::ConstructL()
	{
	TInt error = KErrNone;

	// CTestServer base class call
	// Names the server using the system-wide unique string
	// and also starts the server's logger. This logger is used
	// by the alert server thread
	CTestServer::ConstructL(KServerName);
	
	// The dummy alarm control running from the alert server thread needs to
	// share the test server's logger session to do some logging. As it is
	// non-shareable by default unless we uncomment those lines in 
	// testexecute.cfg, we explicitly ShareAuto() it here.
	error = Logger().ShareAuto();
	if(error != KErrNone)
		{
		INFO_PRINTF2(_L("Error occured while doing ShareAuto(): %D, The alert server thread will not perform any logging"), error);
		}
	else
		{
		iLogSessionShared = ETrue;	
		}
	}

/**
Destructor. 
@internalTechnology
@test
*/
CTestMultipleAlarmsServer::~CTestMultipleAlarmsServer()
	{
	}

/**
EKA2 version of MainL()
Uses the new Rendezvous call instead of the older semaphore.
*/
LOCAL_C void MainL()
	{
	// For platform security
#if (defined __DATA_CAGING__)
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().SecureApi(RProcess::ESecureApiOn);
#endif

	CActiveScheduler* sched = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CTestMultipleAlarmsServer* server = NULL;

	// Create the test-server
	TRAPD(err, server = CTestMultipleAlarmsServer::NewL());
	
	if(!err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	delete server;
	delete sched;
	}

/**
Exe entry point code
*/
GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup *cleanup = CTrapCleanup::New();
	if (cleanup == NULL)
		{
		return KErrNoMemory;
		}
	TInt err = KErrNone;
	TRAP(err, MainL());
	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
	}

/**
Base class pure virtual, called by TEF
@param aStepName Descriptor containing the test-step name
@return Instance of the test step
@internalTechnology
@test
*/
CTestStep* CTestMultipleAlarmsServer::CreateTestStep(const TDesC& aStepName)
	{
	CTestStep *testStep = NULL;
	
	if (aStepName == KTestStartAlertServerStep)
		{
		testStep = new (ELeave) CTestStartAlertServerStep(*this); // Method could leave while construction
		}
	else if (aStepName == KTestWaitStep)
		{
		testStep = new (ELeave) CTestWaitStep(*this);
		}
	else if (aStepName == KTestCreateCalEntriesStep)
		{
		testStep = new (ELeave) CTestCreateCalEntriesStep(*this);
		}
	else if (aStepName == KTestAlarmControlStateStep)
		{
		testStep = new (ELeave) CTestAlarmControlStateStep(*this);
		}
	else if (aStepName == KTestAlarmSnoozeStep)
		{
		testStep = new (ELeave) CTestAlarmSnoozeStep(*this);
		}
	else if (aStepName == KTestAlarmClearStep)
		{
		testStep = new (ELeave) CTestAlarmClearStep(*this);
		}
	else if (aStepName == KTestAssociatedDataStep)
		{
		testStep = new (ELeave) CTestAssociatedDataStep(*this);
		}
	else if (aStepName == KTestKillAlertServerStep)
		{
		testStep = new (ELeave) CTestKillAlertServerStep(*this);
		}
	else if (aStepName == KTestCleanupStep)
		{
		testStep = new (ELeave) CTestCleanupStep(*this);
		}
	else if (aStepName == KTestPerformanceStep)
		{
		testStep = new (ELeave) CTestPerformanceStep(*this);
		}
	else if (aStepName == KTestCreateEntriesOOMStep)
		{
		testStep = new (ELeave) CTestCreateEntriesOOMStep(*this);
		}				
	else if (aStepName == KTestAlarmContentOOMStep)
		{
		testStep = new (ELeave) CTestAlarmContentOOMStep(*this);
		}	
	return testStep;
	}

/**
Returns a refernce to the current max alarms value. Set by the 
TestStartAlertServerStep
@return A reference to the current max alarms value
@internalTechnology
@test
*/
TInt& CTestMultipleAlarmsServer::CurrentMaxAlarms()
	{
	return iCurrentMaxAlarms;
	}

/**
Returns the current max alarms value. Set by the TestStartAlertServerStep
@return The current max alarms value
@internalTechnology
@test
*/
TInt CTestMultipleAlarmsServer::CurrentMaxAlarms() const
	{
	return iCurrentMaxAlarms;
	}	
	
/**
Returns a reference to the pointer to CAlarmControlsManager
@return Reference to the pointer to CAlarmControlsManager
@internalTechnology
@test
*/
CAlarmControlsManager*& CTestMultipleAlarmsServer::AlarmControlsManager()
	{
	return iAlmCtrlsMgr;
	}
	
/**
Returns a reference to the pointer to CEikServAlarmAlertServer
@return Reference to the pointer to CEikServAlarmAlertServer
@internalTechnology
@test
*/
CEikServAlarmAlertServer*& CTestMultipleAlarmsServer::AlertServer()
	{
	return iAlertServer;
	}
	
/**
Returns the number of currently notifying alarms
This flag is incremented / decremented when CDummyAlarmControls are 
notified of alarms that expire
@return The number of currently notifying alarms
@internalTechnology
@test
*/
TInt& CTestMultipleAlarmsServer::NoOfCurrentlyNotifyingAlarms()
	{
	return iNoOfCurrentlyNotifyingAlarms;
	}
/**
Returns the Id of the alarm which was the last to start playing
@return The Id of the alarm which was the last to start playing
@internalTechnology
@test
*/
TAlarmId& CTestMultipleAlarmsServer::CurrentlyPlayingAlarmId()
	{
	return iCurrentlyPlayingAlarmId;
	}
	
/**
Returns a refernce to the thread that starts Alert Server
@return Refernce to the thread that starts Alert Server
@internalTechnology
@test
*/
RThread& CTestMultipleAlarmsServer::AltSrvThread()
	{
	return iAltSrvThread;
	}
	
/**
Returns whether we were successful in making the Logger shareable between threads
@return Whether we were successful in making the Logger shareable between threads
@internalTechnology
@test
*/
TBool CTestMultipleAlarmsServer::LogSessionShared()
	{
	return iLogSessionShared;
	}
