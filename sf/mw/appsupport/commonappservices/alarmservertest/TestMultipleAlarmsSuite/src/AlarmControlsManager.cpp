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
// Contains implementation of CAlarmControlsManager class
// 
//

/**
 @file
 @internalTechnology 
*/

// User Includes
#include "DummyAlarmControl.h"
#include "AlarmActionPerformer.h"

/**
Constructor
@param aTestServer Reference to the Test Server object
@internalTechnology
@test
*/
CAlarmControlsManager::CAlarmControlsManager(CTestMultipleAlarmsServer& aTestServer) : iTestServer(aTestServer)
	{
	}
	
/**
First phase Constructor.
@param aTestServer Reference to the Test Server object
@return Pointer to self
@internalTechnology
@test
*/
CAlarmControlsManager* CAlarmControlsManager::NewL(CTestMultipleAlarmsServer& aTestServer)
	{
	CAlarmControlsManager *self = new (ELeave) CAlarmControlsManager(aTestServer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
/**
Second phase Constructor.
Initializes Alarm Action Performer.
@internalTechnology
@test
*/
void CAlarmControlsManager::ConstructL()	
	{
	iAlarmActionPerformer = new (ELeave) CAlarmActionPerformer();
	}
	
/**
Destructor. Resets and destroys the alarm control list
@internalTechnology
@test
*/
CAlarmControlsManager::~CAlarmControlsManager()
	{
	iAlarmControlList.ResetAndDestroy();
	iAlarmControlList.Close();
	delete iAlarmActionPerformer;
	}

/**
MEikServAlarmFactory implementation. Creates and returns an alarm control object
Called by the Alarm Alert Supervisor
@param aSupervisor Reference to the Alarm Alert supervisor object.
@return A pointer to the Alarm Control object
@internalTechnology
@test
*/
MEikServAlarm* CAlarmControlsManager::NewAlarmL(CEikAlmControlSupervisor& aSupervisor)
	{
	CDummyAlarmControl* alarmControl = new (ELeave) CDummyAlarmControl(&aSupervisor, this);
	CleanupStack::PushL(alarmControl);
	iAlarmControlList.AppendL(alarmControl);
	CleanupStack::Pop(alarmControl);
	return alarmControl;
	}

/**
Increments the Test Server's flag of currently notifying alarms
@internalTechnology
@test
*/
void CAlarmControlsManager::IncrementNoOfCurrentlyNotifyingAlarms()
	{
	iTestServer.NoOfCurrentlyNotifyingAlarms() += 1;
	}
	
/**
Decrements the Test Server's flag of currently notifying alarms
@internalTechnology
@test
*/
void CAlarmControlsManager::DecrementNoOfCurrentlyNotifyingAlarms()	
	{
	iTestServer.NoOfCurrentlyNotifyingAlarms() -= 1;	
	}

/**
Updates the Test Server's flag of the currently playing alarm's ID
@internalTechnology
@test
*/
void CAlarmControlsManager::UpdateCurrentlyPlayingAlarmId(const TAlarmId& aAlarmId)
	{
	iTestServer.CurrentlyPlayingAlarmId() = aAlarmId;
	}

/**
Returns the Test Server's flag of the currently supported no. of maxalarms
Ereturn The Test Server's flag of the currently supported no. of maxalarms
@internalTechnology
@test
*/
TInt CAlarmControlsManager::CurrentMaxAlarms()
	{
	return iTestServer.CurrentMaxAlarms();
	}

/**
Returns the string equivalent for the enumeration identifying the call-back
@param aCallBack The enumeration identifying the call-back
@return The string equivalent for the enumeration
@internalTechnology
@test
*/
const TDesC& CAlarmControlsManager::GetFunctionName(TInt aCallBack)
	{
	_LIT(KConstructed, "Constructed");
	_LIT(KRelease, "Release");
	_LIT(KShowAlarm, "ShowAlarm");
	_LIT(KHideAlarm, "HideAlarm");
	_LIT(KCurrentServerState, "CurrentServerState");
	_LIT(KUpdateSoundPauseTimeInterval, "UpdateSoundPauseTimeInterval");
	_LIT(KUpdateForAlarmServerState, "UpdateForAlarmServerState");
	_LIT(KUpdateAlarmInfo, "UpdateAlarmInfo");
	_LIT(KStartPlayAlarm, "StartPlayAlarm");
	_LIT(KStopPlayAlarm, "StopPlayAlarm");
	
	switch(aCallBack) 
		{
		case TestMultipleAlarms::EConstructed:
			return KConstructed();
		case TestMultipleAlarms::ERelease:
			return KRelease();
		case TestMultipleAlarms::EShowAlarm:
			return KShowAlarm();
		case TestMultipleAlarms::EHideAlarm:
			return KHideAlarm();
		case TestMultipleAlarms::ECurrentServerState:
			return KCurrentServerState();
		case TestMultipleAlarms::EUpdateSoundPauseTimeInterval:
			return KUpdateSoundPauseTimeInterval();
		case TestMultipleAlarms::EUpdateForAlarmServerState:
			return KUpdateForAlarmServerState();
		case TestMultipleAlarms::EUpdateAlarmInfo:
			return KUpdateAlarmInfo();
		case TestMultipleAlarms::EStartPlayAlarm:
			return KStartPlayAlarm();
		case TestMultipleAlarms::EStopPlayAlarm:
			return KStopPlayAlarm();
		default:
			return KNullDesC();	
		}
	}		
	
/**
Prints the message using the Test Server's Logger
@param aMessage The message to be printed
@internalTechnology
@test
*/
void CAlarmControlsManager::PrintLog(const TDesC& aMessage) const
	{
	if(iTestServer.LogSessionShared())
		{
		iTestServer.INFO_PRINTF1(aMessage);
		}
	}
	
/**
Searches for the alarm control object associated with the message passed as 
parameter and returns a pointer to it if found.
@param aAlarmMessage The alarm message to identify its alarm control object
@leave KErrNotFound If no alarm control object is found corresponding to the
message
@return A pointer to the alarm control object
the message that was passed
@internalTechnology
@test
*/
MEikServAlarm* CAlarmControlsManager::GetAlarmControlObjectL(const TDesC& aAlarmMessage)
	{
	CDummyAlarmControl* alarmControl = NULL;
	for(TInt ctr = 0; ctr < iAlarmControlList.Count(); ++ctr)
		{
		alarmControl = static_cast<CDummyAlarmControl*>(iAlarmControlList[ctr]);
		if(alarmControl->AlarmMessage().Compare(aAlarmMessage) == KErrNone)
			{
			return iAlarmControlList[ctr];
			}
		}
	User::Leave(KErrNotFound);
	// To satisfy compilers
	return NULL;
	}

/**
Returns the count of the alarm control list that the alarm controls manager 
maintains
@return The count of the alarm control list
@internalTechnology
@test
*/
TInt CAlarmControlsManager::GetAlarmControlListCount()
	{
	return iAlarmControlList.Count();
	}

/**
Returns the alarm control list that the alarm controls manager 
maintains
@return The alarm control list
@internalTechnology
@test
*/
RPointerArray<MEikServAlarm> CAlarmControlsManager::GetAlarmControlListL()
	{
	return iAlarmControlList;
	}

/**
Removes the alarm control pointer from the alarm control list that the alarm 
controls manager maintains
@param aDummyAlarmControl The pointer to be removed
@internalTechnology
@test
*/
void CAlarmControlsManager::RemoveFromList(CDummyAlarmControl* aDummyAlarmControl)
	{
	TInt index = iAlarmControlList.Find(aDummyAlarmControl);
	if(index >= 0)
		{
		iAlarmControlList.Remove(index);
		}
	}

/**
Sets up the Alarm Action Performer to perform an action
@param aAlarmMessage The alarm message to identify its alarm control object
@param aStatus Pointer to the request status which the Action Performer will 
complete
@param aAlarmAction The action to be performed, i.e, snooze/clear
@internalTechnology
@test
*/
void CAlarmControlsManager::SetAlarmActionCommonParamsL(const TDesC& aAlarmMessage, TRequestStatus* aStatus, const TestMultipleAlarms::TAlarmAction& aAlarmAction)	
	{
	CDummyAlarmControl* alarmControl = dynamic_cast<CDummyAlarmControl*>(GetAlarmControlObjectL(aAlarmMessage));
	RThread thisThread;
	thisThread.Duplicate(RThread());
	iAlarmActionPerformer->SetCommonParams(alarmControl, aStatus, thisThread, aAlarmAction);
	}
	
/**
Requests the Alarm Action Performer to perform a Snooze
@param aAlarmMessage The alarm message to identify its alarm control object
@param aStatus Pointer to the request status which the Action Performer will 
complete
@param aMinsToSnooze Number of minutes to snooze the alarm
@internalTechnology
@test
*/
void CAlarmControlsManager::SnoozeAlarmL(const TDesC& aAlarmMessage, TRequestStatus* aStatus, const TInt& aMinsToSnooze)
	{
	SetAlarmActionCommonParamsL(aAlarmMessage, aStatus, TestMultipleAlarms::ESnooze);
	iAlarmActionPerformer->SetMinsToSnooze(aMinsToSnooze);
	SignalAlertServerThread();
	}
	
/**
Requests the Alarm Action Performer to perform a Clear
@param aAlarmMessage The alarm message to identify its alarm control object
@param aStatus Pointer to the request status which the Action Performer will 
complete
@internalTechnology
@test
*/
void CAlarmControlsManager::ClearAlarmL(const TDesC& aAlarmMessage, TRequestStatus* aStatus)
	{
	SetAlarmActionCommonParamsL(aAlarmMessage, aStatus, TestMultipleAlarms::EClear);
	SignalAlertServerThread();
	}

/**
Requests the action performer to stop the active scheduler
@param aStatus Pointer to the request status which the Action Performer will 
complete
@internalTechnology
@test
*/
void CAlarmControlsManager::EndAlertServerThreadL(TRequestStatus* aStatus)
	{
	RThread thisThread;
	thisThread.Duplicate(RThread());
	iAlarmActionPerformer->SetCommonParams(NULL, aStatus, thisThread, TestMultipleAlarms::EStopActiveScheduler);
	SignalAlertServerThread();
	}

/**
Signals the Alert Server thread in whose scheduler Alarm Action Performer 
is added
@internalTechnology
@test
*/
void CAlarmControlsManager::SignalAlertServerThread()
	{
	TRequestStatus* status = (iAlarmActionPerformer->Status());
	iTestServer.AltSrvThread().RequestComplete(status, KErrNone);	
	}

