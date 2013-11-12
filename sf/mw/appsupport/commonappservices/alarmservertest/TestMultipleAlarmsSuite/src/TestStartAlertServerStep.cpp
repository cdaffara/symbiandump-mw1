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
// Contains implementation of CTestStartAlertServerStep class
// 
//

/**
 @file
 @internalTechnology 
*/

// User Include
#include "TestStartAlertServerStep.h"
#include "AlarmControlsManager.h"

// System Include
#include <uikon/eikalsrv.h>

/**
Constructor. Sets the test step name. Testserver reference passed to make use 
of TEF's method of sharing data between test steps
Max alarms defaults to 1 if not mentioned in ini
@internalTechnology
@test
*/
CTestStartAlertServerStep::CTestStartAlertServerStep(CTestMultipleAlarmsServer& aTestServer) 
: CTestBaseStep(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestStartAlertServerStep);
	}

/**
Base class pure virtual.
@return		EPass or EFail indicating the result of the test step.
@internalTechnology
@test
*/
TVerdict CTestStartAlertServerStep::doTestStepL()
	{
	TInt maxAlarms;
	if(!ReadIntsFromConfig(ConfigSection(), &KIniMaxAlarms(), &maxAlarms, NULL))
		{
		INFO_PRINTF1(_L("maxalarms value not found in ini. maxalarms will hence default to 1"));
		maxAlarms = 1;
		}
	
	// Let the test server record the current maxalarms value, to share it 
	// with other test steps
	TestServer()->CurrentMaxAlarms() = maxAlarms;
	
	PrintIfError(CreateThreadAndStartAltSrv());
	return TestStepResult();	
	}
	
/**
Starts the alarm alert server
@param aTestStep Pointer to the CTestStartAlertServerStep.
@internalTechnology
@test
*/
LOCAL_C void StartAlarmAlertServer(CTestMultipleAlarmsServer* aTestServer)
	{
	const TInt KMaxAttempts = 5;
	TInt error = KErrNone;
	CEikServAlarmAlertServer* alertServer = NULL;
	CAlarmControlsManager *manager = NULL;
	
	TRAP(error, manager = CAlarmControlsManager::NewL(*aTestServer));
	
	if(error == KErrNone)
		{
		aTestServer->AlarmControlsManager() = manager;
		for(TInt attempt = 0; attempt < KMaxAttempts; ++attempt)
			{
			if(aTestServer->CurrentMaxAlarms() == 1)
				{// Calling the single alarm notification version. Can still call the multiple alarm 
				 // version with 1 as maxalarms value, but still just to cover the existing API too.
				TRAP(error, alertServer = CEikServAlarmAlertServer::NewL(manager));
				}
			else
				{// Multiple alarm version
				TRAP(error, alertServer = CEikServAlarmAlertServer::NewL(manager, aTestServer->CurrentMaxAlarms()));
				}
				
			if(error != KErrAlreadyExists)
				{// Although the thread is shown as dead, EPOC seems to do some
				 // background closing operations, and does not allow us to create a 
				 // thread with the same name till then. So we try a few more times
				 // only if it is KErrAlreadyExists
				break;
				}
			}
		}
	// Tell the test step thread about the status	
	RThread::Rendezvous(error);
	
	if(error == KErrNone)
		{
		aTestServer->AlertServer() = alertServer;
	
		// enter the active scheduler if everything is ok
		CActiveScheduler::Start();
			
		// When thread exits, clean up
		delete (aTestServer->AlertServer());
		delete (aTestServer->AlarmControlsManager());
		}	
	}

/**
Thread entry point
@return Error code of the operation performed
@internalTechnology
@test
*/
static TInt ThreadStartFn(TAny* aPtr)
	{
	__UHEAP_MARK;
	CActiveScheduler* sched = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		delete sched;
		return KErrNoMemory;
		}
	CTestMultipleAlarmsServer* testServer = (static_cast<CTestStartAlertServerStep*>(aPtr))->TestServer();	
	StartAlarmAlertServer(testServer);
	delete sched;
	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
	}


/**
Starts the alarm alert server
@param aMaxAlarms Maximum number of alarms for which we want the alert server 
to notify at a time	
@return The error code
@internalTechnology
@test
*/
TInt CTestStartAlertServerStep::CreateThreadAndStartAltSrv()
	{
	_LIT(KChildThreadName, "ChildThreadThatStartsAlarmAlertServerInAlarmAlertServerTests");
	TBuf<65> childThreadName(KChildThreadName);
	RThread childThread;
	
	const TInt KMaxTestThreadHeapSize = 0x10000;
	
	TInt err =	childThread.Create(
				childThreadName,
				ThreadStartFn,
				KDefaultStackSize,
				KMinHeapSize,
				KMaxTestThreadHeapSize,
				this,			
				EOwnerProcess);
				
	if(err == KErrNone)
		{
		TRequestStatus status;
		childThread.Rendezvous(status);
		if(status != KRequestPending)
			{
			childThread.Kill(0);
			}
		else	
			{
			childThread.Resume();
			
			// Cache the thread handle, so as to examine its death later
			TestServer()->AltSrvThread() = childThread;
			
			INFO_PRINTF2(_L("<B>Alert Server thread has started, Max alarms = %D</B>"), TestServer()->CurrentMaxAlarms());
			}
		User::WaitForRequest(status);
		
		// Examine the child thread's exit type and reason.
		// If it is a panic, it can be KErrNone too, and if so, make it KErrGeneral,
		// so that the Test Step does not wrongly EPass seeing KErrNone in err.
		// The Kern-Exec Panic 0 information will not be lost either, as each test
		// step checks and displays this in its Postamble.
		err = (childThread.ExitType()==EExitPanic && childThread.ExitReason() == KErrNone
			  ) ? KErrGeneral : status.Int();
		}
	return err;	
	}
	
