// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This file contains the main TestServer suite functions including E32Main.
// For (WINS && !EKA2) versions will be xxxServer.Dll and require a thread to be started
// in the process of the client. The client initialises the server by calling the
// one and only ordinal.
// 
//

// EPOC includes
#include <e32base.h>
#include <e32cmn.h> 

// Type definitions
#define UNUSED_VAR(a) a = a

#if (!defined TS_Alarm_SERVER_H_)
	#include "CSuite.h"
#endif

// Add Test Step headers here
#include "EASShdOpCodeNotifyChange1_CStep.h"
#include "EASShdOpCodeGetAlarmCategory1_CStep.h"
#include "EASShdOpCodeGetAlarmOwner1_CStep.h"
#include "EASShdOpCodeGetAlarmStatus1_CStep.h"
#include "EASShdOpCodeGetAlarmDayOrTimed1_CStep.h"
#include "EASShdOpCodeGetAlarmCharacteristics1_CStep.h"
#include "EASShdOpCodeAlarmDataSize1_CStep.h"
#include "EASShdOpCodeGetAlarmCountForCategory1_CStep.h"
#include "EASShdOpCodeGetAvailableCategoryList1_CStep.h"
#include "EASShdOpCodeGetAlarmIdListForCategory1_CStep.h"
#include "EASShdOpCodeAlarmCountByState1_CStep.h"
#include "EASShdOpCodeGetAlarmIdListByState1_CStep.h"
#include "EASShdOpCodeGetAlarmIdList1_CStep.h"
#include "EASShdOpCodeGetNextDueAlarmId1_CStep.h"
#include "EASShdOpCodeNumberOfAlarmsActiveInQueue1_CStep.h"
#include "EASShdOpCodeGetAlarmSoundState1_CStep.h"
#include "EASShdOpCodeGetAlarmSoundsSilentUntil1_CStep.h"
#include "SShdOpCodeAlarmSoundsTemporarilySilenced1_CStep.h"
#include "EASShdOpCodeGetAlarmPlayIntervals1_CStep.h"
#include "EASShdOpCodeNotifyChangeCancel1_CStep.h"
#include "EASShdOpCodeFetchTransferBuffer1_CStep.h"
#include "EASShdOpCodeAlarmCount1_CStep.h"
#include "EASShdOpCodeDbgFailAlloc1_CStep.h"
#include "EASShdOpCodeDbgPreventUserNotify1_CStep.h"
#include "EASShdOpCodeDbgSnoozeAlarm1_CStep.h"
#include "EASShdOpCodeFlushServer1_CStep.h"

#include "EASShdOpCodeGetAlarmDetails1_CStep.h"
#include "EASShdOpCodeGetAlarmDetails_DifferentSID1_CStep.h"

#include "EASShdOpCodeGetAlarmData1_CStep.h"
#include "EASShdOpCodeGetAlarmData_DifferentSID1_CStep.h"

#include "EASShdOpCodeAlarmAtIndex1_CStep.h"
#include "EASShdOpCodeAlarmAtIndex_DifferentSID1_CStep.h"

#include "SShdpCdAlarmNotificationCancelAndDeQueue1_CStep.h"
#include "SShdpCdAlarmNotificationCancelAndDeQueue_DifferentSID1_CStep.h"

#include "EASShdOpCodeAlarmDelete1_CStep.h"
#include "EASShdOpCodeAlarmDelete_DifferentSID1_CStep.h"

#include "EASShdOpCodeSetAlarmStatus1_CStep.h"
#include "EASShdOpCodeSetAlarmStatus_DifferentSID1_CStep.h"

#include "EASShdOpCodeSetAlarmDayOrTimed1_CStep.h"
#include "EASShdOpCodeSetAlarmDayOrTimed_DifferentSID1_CStep.h"

#include "EASShdOpCodeSetAlarmCharacteristics1_CStep.h"
#include "EASShdOpCodeSetAlarmCharacteristics_DifferentSID1_CStep.h"

#include "EASShdOpCodeSetClientData1_CStep.h"
#include "EASShdOpCodeSetClientData_DifferentSID1_CStep.h"

#include "EASShdOpCodeAlarmDataAttach1_CStep.h"
#include "EASShdOpCodeAlarmDataAttach_DifferentSID1_CStep.h"

#include "EASShdOpCodeAlarmDataDetach1_CStep.h"
#include "EASShdOpCodeAlarmDataDetach_DifferentSID1_CStep.h"

#include "EASShdOpCodeAlarmAddWithNotification1_CStep.h"
#include "EASShdOpCodeOrphanAlarm1_CStep.h"
#include "EASShdOpCodeAlarmAdd1_CStep.h"
#include "EASShdOpCodeSetAlarmSoundState1_CStep.h"
#include "EASShdOpCodeSetAlarmSoundsSilentUntil1_CStep.h"
#include "EASShdOpCodeSetAlarmSoundsSilentFor1_CStep.h"
#include "EASShdOpCodeCancelAlarmSilence1_CStep.h"
#include "EASShdOpCodeSetAlarmPlayIntervals1_CStep.h"
#include "EASShdOpCodeSetAlarmStatusByCategory1_CStep.h"
#include "EASShdOpCodeAlarmDeleteAllByCategory1_CStep.h"
#include "EASShdOpCodeAlarmDeleteByCategory1_CStep.h"

// __EDIT_ME__ - Substitute the name of your test server 
_LIT(KServerName,"Cap_Alarm_sc");
// __EDIT_ME__ - Use your own server class name


/**
Same code for Secure and non-secure variants
Called inside the MainL() function to create and start the
CTestServer derived server.
@return - Instance of the test server
*/
CTestAlarmServer* CTestAlarmServer::NewL()
	{
	// __EDIT_ME__ - Use your own server class name
	CTestAlarmServer* server = new (ELeave) CTestAlarmServer();
	CleanupStack::PushL(server);

	// CServer base class call
	server->StartL(KServerName);
	CleanupStack::Pop(server);
	return server;
	}

/**
Base class pure virtual override
Secure and non-secure variants
Implementation of CTestServer pure virtual
@return - A CTestStep derived instance
*/
CTestStep* CTestAlarmServer::CreateTestStep(const TDesC& aStepName)
	{
	 CTestStep* testStep = NULL;
	// add test steps
		if (aStepName == _L("CEASShdOpCodeNotifyChange1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeNotifyChange1Step; // Method can leave during construction
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeGetAlarmCategory1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeGetAlarmCategory1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeGetAlarmOwner1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeGetAlarmOwner1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeGetAlarmStatus1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeGetAlarmStatus1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeGetAlarmDayOrTimed1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeGetAlarmDayOrTimed1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeGetAlarmCharacteristics1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeGetAlarmCharacteristics1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeAlarmDataSize1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeAlarmDataSize1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeGetAlarmCountForCategory1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeGetAlarmCountForCategory1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeGetAvailableCategoryList1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeGetAvailableCategoryList1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeGetAlarmIdListForCategory1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeGetAlarmIdListForCategory1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeAlarmCountByState1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeAlarmCountByState1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeGetAlarmIdListByState1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeGetAlarmIdListByState1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeGetAlarmIdList1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeGetAlarmIdList1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeGetNextDueAlarmId1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeGetNextDueAlarmId1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeNumberOfAlarmsActiveInQueue1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeNumberOfAlarmsActiveInQueue1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeGetAlarmSoundState1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeGetAlarmSoundState1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeGetAlarmSoundsSilentUntil1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeGetAlarmSoundsSilentUntil1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CSShdOpCodeAlarmSoundsTemporarilySilenced1Step"))
			{
			testStep =  new(ELeave) CSShdOpCodeAlarmSoundsTemporarilySilenced1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeGetAlarmPlayIntervals1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeGetAlarmPlayIntervals1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeNotifyChangeCancel1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeNotifyChangeCancel1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeFetchTransferBuffer1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeFetchTransferBuffer1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeAlarmCount1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeAlarmCount1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeDbgFailAlloc1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeDbgFailAlloc1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeDbgPreventUserNotify1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeDbgPreventUserNotify1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeDbgSnoozeAlarm1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeDbgSnoozeAlarm1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeFlushServer1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeFlushServer1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeGetAlarmDetails1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeGetAlarmDetails1Step;
			return testStep;
			}
		if (aStepName == _L("CEASShdOpCodeGetAlarmDetails_DifferentSID1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeGetAlarmDetails_DifferentSID1Step;
			return testStep;
			}	
			 
		if (aStepName == _L("CEASShdOpCodeGetAlarmData1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeGetAlarmData1Step;
			return testStep;
			}
		if (aStepName == _L("CEASShdOpCodeGetAlarmData_DifferentSID1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeGetAlarmData_DifferentSID1Step;
			return testStep;
			}	
			
			 
		if (aStepName == _L("CEASShdOpCodeAlarmAtIndex1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeAlarmAtIndex1Step;
			return testStep;
			}
			if (aStepName == _L("CEASShdOpCodeAlarmAtIndex_DifferentSID1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeAlarmAtIndex_DifferentSID1Step;
			return testStep;
			}
			 
			 
		if (aStepName == _L("CSShdpCdAlarmNotificationCancelAndDeQueue1Step"))
			{
			testStep =  new(ELeave) CSShdpCdAlarmNotificationCancelAndDeQueue1Step;
			return testStep;
			}
		if (aStepName == _L("CSShdpCdAlarmNotificationCancelAndDeQueue_DifferentSID1Step"))
			{
			testStep =  new(ELeave) CSShdpCdAlarmNotificationCancelAndDeQueue_DifferentSID1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeAlarmDelete1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeAlarmDelete1Step;
			return testStep;
			}
		if (aStepName == _L("CEASShdOpCodeAlarmDelete_DifferentSID1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeAlarmDelete_DifferentSID1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeSetAlarmStatus1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeSetAlarmStatus1Step;
			return testStep;
			}
			if (aStepName == _L("CEASShdOpCodeSetAlarmStatus_DifferentSID1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeSetAlarmStatus_DifferentSID1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeSetAlarmDayOrTimed1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeSetAlarmDayOrTimed1Step;
			return testStep;
			}
		if (aStepName == _L("CEASShdOpCodeSetAlarmDayOrTimed_DifferentSID1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeSetAlarmDayOrTimed_DifferentSID1Step;
			return testStep;
			}	
			 
		if (aStepName == _L("CEASShdOpCodeSetAlarmCharacteristics1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeSetAlarmCharacteristics1Step;
			return testStep;
			}
		if (aStepName == _L("CEASShdOpCodeSetAlarmCharacteristics_DifferentSID1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeSetAlarmCharacteristics_DifferentSID1Step;
			return testStep;
			}	
			 
		if (aStepName == _L("CEASShdOpCodeSetClientData1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeSetClientData1Step;
			return testStep;
			}
		if (aStepName == _L("CEASShdOpCodeSetClientData_DifferentSID1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeSetClientData_DifferentSID1Step;
			return testStep;
			}	
			 
		if (aStepName == _L("CEASShdOpCodeAlarmDataAttach1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeAlarmDataAttach1Step;
			return testStep;
			}
		if (aStepName == _L("CEASShdOpCodeAlarmDataAttach_DifferentSID1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeAlarmDataAttach_DifferentSID1Step;
			return testStep;
			}	
			 
		if (aStepName == _L("CEASShdOpCodeAlarmDataDetach1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeAlarmDataDetach1Step;
			return testStep;
			}
		if (aStepName == _L("CEASShdOpCodeAlarmDataDetach_DifferentSID1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeAlarmDataDetach_DifferentSID1Step;
			return testStep;
			}	
			 
		if (aStepName == _L("CEASShdOpCodeAlarmAddWithNotification1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeAlarmAddWithNotification1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeOrphanAlarm1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeOrphanAlarm1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeAlarmAdd1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeAlarmAdd1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeSetAlarmSoundState1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeSetAlarmSoundState1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeSetAlarmSoundsSilentUntil1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeSetAlarmSoundsSilentUntil1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeSetAlarmSoundsSilentFor1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeSetAlarmSoundsSilentFor1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeCancelAlarmSilence1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeCancelAlarmSilence1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeSetAlarmPlayIntervals1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeSetAlarmPlayIntervals1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeSetAlarmStatusByCategory1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeSetAlarmStatusByCategory1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeAlarmDeleteAllByCategory1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeAlarmDeleteAllByCategory1Step;
			return testStep;
			}
			 
		if (aStepName == _L("CEASShdOpCodeAlarmDeleteByCategory1Step"))
			{
			testStep =  new(ELeave) CEASShdOpCodeAlarmDeleteByCategory1Step;
			return testStep;
			}
			 
		return testStep;
	 }
	
/**
Secure variant. Just an E32Main and a MainL()
Much simpler, uses the new Rendezvous() call to sync with the client
*/
LOCAL_C void MainL()
	{
#if (defined __DATA_CAGING__)
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().DataCaging(RProcess::ESecureApiOn);
#endif
	CActiveScheduler* sched = NULL;
	sched=new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	
	// __EDIT_ME__ - Use your own server class name
	CTestAlarmServer* server = NULL;
	
	// Create the CTestServer derived server
	TRAPD(err,server = CTestAlarmServer::NewL());
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
Secure variant only
Process entry point. Called by client using RProcess API
@return - Standard Epoc error code on process exit
*/
GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
	TRAPD(err,MainL());
	UNUSED_VAR(err); //Used to supress build warnings
	
	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
    }
/**
Gets the id of the alarm in the Alarm Server
*/    
TAlarmId CCapabilityTestStep::GetAlarmId()
	{
	// Connect to Alarm Server
	RASCliSession alarmServerSession = RASCliSession();
	TInt con = alarmServerSession.Connect();
	if(con != KErrNone)
		{
		return con;
		}
	
	// Get list of alarm id. There should be only one alarm
	RArray<TAlarmId> alarmIds;
	TInt err = KErrNone;	
	TRAP(err, alarmServerSession.GetAlarmIdListL(alarmIds));
			
	TAlarmId alarmId;
	
	if(err != KErrNone)
		{
		alarmId = err;
		}
	else if(alarmIds.Count() == 0)
		{
		// after all alarms have been deleted , the count is 0.
		// needed as this will help testing with correct capability
		// where there is no SID checking.
		alarmId = 0;
		}
	else
		{
		// return the first element in array.
		alarmId = alarmIds[0];
		}
	alarmIds.Reset();
	return alarmId;		
	}

TVerdict CCapabilityTestStep::doTestStepPreambleL( void )
	{
	// If Preamble is not required just pass a success value
	TVerdict testResult = CTestStep::doTestStepPreambleL();
	return TestStepResult();
	}

TVerdict CCapabilityTestStep::doTestStepPostambleL( void )
	{
	// If Postamble is not required just pass a success value
	TVerdict testResult = CTestStep::doTestStepPostambleL();
	return TestStepResult();
	}

enum TVerdict CCapabilityTestStep::doTestStepL()
	{
	// DEF! INFO_PRINTF2(_L("%S - Starting ..."), &iTestStepName);
	// The MainThread()creates a separate thread that executes SendReceive
	TVerdict vResult = MainThread();
	SetTestStepResult(vResult);
	return TestStepResult();
	}

/**
ThreadStartFn:
Called by:	The Child thread
Function:	Calls the Exec_SendReceiveL
*/
static TInt ThreadStartFn(TAny* aPtr)
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	TInt retVal;
	TRAP(retVal, ((CCapabilityTestStep *)aPtr)->Exec_SendReceiveL());
	delete cleanup;
	__UHEAP_MARKEND;
	return retVal;
	}

/**
TVerdict GetVerdict(TInt aApiRetValue)
Called by: "MainThread" for returning verdict
Parameters(TInt aRetValue) :	0 if API call gets thru without any rejection
								1 if API call is rejected for capability error	
*/
enum TVerdict CCapabilityTestStep::GetVerdict(TInt aApiRetValue)
	{
	TVerdict vVerdict[] = {EPass, EFail};

	// please leave the following if/else block as the information printed by INFO_PRINTF1
	// is used by CapTestSumm
	if(iExpectRejection) //[Inverse Test] EPass for 1 while EFail for 0
		{
		INFO_PRINTF1(_L("Test Expected to Fail due to lack of capabilities"));
		return vVerdict[(aApiRetValue)?0:1];
		
		}
	else //[Direct Test] EPass for 0 while EFail for 1
		{
		INFO_PRINTF1(_L("Test Expected to Pass with correct capabilities"));
		return vVerdict[(aApiRetValue)?1:0];
		}
	}


/**
TVerdict MainThread()

Called by: "doTestStepL"

Purpose:	Creates the child thread(which calls the respective function with regard
			to the server and also implements the Message Call). Then this fn.waits for the
			completion of the childthread( doesnt matter how the thread did die!)

Return Value(Verdict of the TestStep):

			A.Reporting PASS/FAIL
				Direct Test:
						When a message call gets thru. Please note that in such cases
						actually the implementation of the message has started. As we
						are passing "0" Parameters, server may panic, though our botheration
						stops once the call gets thru.
						NOTE:	The style is the same when CONNECTION capabilities
								are tested, the only diff is you dont have to expect a 
								panic from server
				Inverse Test:
						The call should be either failed or panicked with
						"KErrPermissionDenied" flag. 

				General Case:
						If a thread creation failed or if the server couldnt be connected
						apart from the above two cases, then a FAIL is reported
			
			B.Reporting INCONCLUSIVE
						Any panic say from unexpected source (eg:KERN-EXEC) will be
						reported INCONCLUSIVE														
*/
TVerdict CCapabilityTestStep::MainThread()
	{
	TBuf<100>	tExitCategory;
	TInt		tExitReason = 0;		
	TBuf<100>	tTestStyle;

	iExpectRejection?tTestStyle = _L("Inverse"):tTestStyle = _L("Direct");
	TCapabilitySet tCaps =  TSecurityInfo(RProcess()).iCaps ;
	const TInt KMaxTestThreadHeapSize = 0x10000;

	// Initialize return values
	iResultSr		= KErrNone;
	iResultServer	= KErrNone;	
		

	// Create a child thread, with a new heap
	TInt resThread =	iChildThread.Create(
						iSrChildThread,
						ThreadStartFn,
						KDefaultStackSize,
						KMinHeapSize,
						KMaxTestThreadHeapSize,
						this,			
						EOwnerProcess);


	if(resThread == KErrNone) //Thread Created
		{
		// To when the thread is dead
		TRequestStatus tThreadStatus;
		iChildThread.Logon(tThreadStatus);
		iChildThread.Resume(); 
		
		// Is the thread dead?
		User::WaitForRequest(tThreadStatus);
		
		// Yes, The thread is dead. Now the Killer's profile
		tExitCategory	=	iChildThread.ExitCategory();
		tExitReason		=	iChildThread.ExitReason();
		
		// Somebody Please say what are we testing!!		
		if(iSessionCreated && (iSrMessageId >=0)) //Flag set by Child thread when connected to Server
			{
			// DEF INFO_PRINTF5(_L("Connected to Server(%S) for %S Test [MessageID: %d,Req.Cap: 0x%x,Present.Cap: 0x%x]"),&iSrServerName,&tTestStyle,iSrMessageId,iStepCap,TSecurityInfo(RProcess()));			
			}
		else if(iSrMessageId < 0)
			{
			// DEF INFO_PRINTF5(_L("Testing Connection capabilities[%S Test] for Server(%S) [Req.Cap: 0x%x,Present.Cap: 0x%x]"),&tTestStyle,
			// &iSrServerName,TSecurityInfo(RProcess()));			
			}
		else if(!iSessionCreated)// NO Connection
			{
			INFO_PRINTF4(	_L("Couldnt connect to the Server(%S) ErrorCode - ServerRet: %d C32ret: %d"),
							&iSrServerName,iResultServer,iResultC32
						);
			// INFO_PRINTF3(_L("Child Thread: ExitCategory : %S ExitReason : %d"),&tExitCategory,tExitReason);
  			return EFail;
 			}

		switch(iChildThread.ExitType())
			{			
			case EExitPanic:
				// 1.A Panic from the connected Server 
				// 2.A CServer Panic normally for capability rejection
				// 3.A kernel Panic (consider yourself doomed!)
				if((tExitReason == KErrPermissionDenied) ||
					// DEF ? it's old version (tExitReason == CServer::EClientDoesntHaveRequiredCaps))
					// Rejected for Insufficient Cap.
					// is it correct ?
					(tExitReason == CServer2::EClientDoesntHaveRequiredCaps))// Rejected for Insufficient Cap.
					{
					INFO_PRINTF2(_L("Rejected for insufficient capabilities [Return Value : %d] "),tExitReason);
					return(GetVerdict(API_RETVALUE_PERMISSIONDENIED));
					}
				else if(tExitCategory == iServerPanic) //Panic from Server
					{
					INFO_PRINTF2(_L("Server(%S) Panic to child thread"),&tExitCategory);	
					INFO_PRINTF3(_L("Child Thread: ExitCategory : %S ExitReason : %d"),&tExitCategory,tExitReason);			
					return(GetVerdict(API_RETVALUE_SERVERPANIC));
					}
				else//A kernel Panic possibly
					{
					INFO_PRINTF3(	_L("Child Thread: Panic from unexpected source (ExitCategory: %S ExitReason : %d)!"),
									&tExitCategory,tExitReason
								);
					return EInconclusive;
					}
			case EExitKill:
				if(iResultSr != KErrPermissionDenied)
					{
					INFO_PRINTF2(
									_L("A Successfull call (Return Value : %d)"),
									((iSrMessageId >=0)?iResultSr:iResultServer)
								);
					return(GetVerdict(API_RETVALUE_NOCAPERROR));
					}
				else 
					{
					INFO_PRINTF2(
									_L("Rejected for insufficient capabilities [Return Value : %d] "),
									((iSrMessageId >=0)?iResultSr:iResultServer)
								);
					return(GetVerdict(API_RETVALUE_PERMISSIONDENIED));			
					}
			default:					
					break;
			}
		}			
	else //Our thread couldnt start
		{
		INFO_PRINTF2(_L("ERROR: Failed to create Child thread,  ErrorCode:(%d)"),resThread);
		return EFail;
		}
	return EInconclusive;
	}

/** 
Starts the alarm server.
@return KErrNone if successful, KErrAlreadyExists if the server is already 
running, otherwise a system-wide error.
*/
TInt CCapabilityTestStep::StartServer()
	{
	return AlarmClientUtils::StartAlarmServer();
	}

	
TInt CCapabilityTestStep::TestDebugHeap(TInt* aDbgIPCNo)
 	{
  	// TDbgFns  {MarkHeapStart, MarkHeapEnd, CheckHeap, FailNext, ResetFailNext};
 	TInt aFnToTest= aDbgIPCNo[5];
 	
 	TInt resultSr [6] ={0};
 	TInt index = 1;
 	TInt testedFn = 0;
 
 	TInt dbgTestSequence[5][6]	=	{	{MarkHeapStart	,2,0,1,-1,-1}, 
 										{MarkHeapEnd	,2,0,1,-1,-1},
 										{CheckHeap		,3,0,2, 1,-1},
 										{FailNext		,4,0,3, 4, 1},
 										{ResetFailNext	,4,0,3, 4, 1}
 										
 									};
 
 	
 	TInt aCount = dbgTestSequence[aFnToTest][index];
 
 	while(aCount--  )
 		{
 		testedFn =  dbgTestSequence[aFnToTest][(++index)];
		resultSr[testedFn ]= SendReceive( aDbgIPCNo[testedFn],TIpcArgs(((aDbgIPCNo[testedFn]==3 )?4:0),0,0,0));
 		
 		if( ((testedFn !=aFnToTest)?resultSr[testedFn]:KErrNone) == KErrPermissionDenied) 			
			{
			User::Panic(_L("Failed at Initialization"),resultSr[testedFn]);		
			}
		}
	return resultSr[aFnToTest];
	}
	
 
