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
// Contains implementation of CDummyAlarmControl class 
// 
//

/**
 @file
 @internalTechnology 
*/

// User Includes
#include "DummyAlarmControl.h"

/*@{*/
// < and > escape encoded as &lt; and &gt; as TestExecuteLogEngine
// does not do it for us.
_LIT(KNotKnown, "&lt;message not known yet&gt;");
/*@}*/
	
/**
Constructor
@param aSupervisor Pointer to the supervisor corresponding to this alarm 
control
@param aAlarmControlsManager Pointer to the alarm controls manager
@internalTechnology
@test
*/
CDummyAlarmControl::CDummyAlarmControl(CEikAlmControlSupervisor* aSupervisor, CAlarmControlsManager* aAlarmControlsManager) 
: iSupervisor(aSupervisor), iMyManager(aAlarmControlsManager), iAlarmMessage(KNotKnown)
	{
	}
	
/**
Destructor
@internalTechnology
@test
*/
CDummyAlarmControl::~CDummyAlarmControl()	
	{
	delete iAlarmSoundName;
	}
	
//
// MEikServAlarm callbacks Impelementations start
//

/**
MEikServAlarm implementation. Prints that it was called.
Asks its manager to remove it from his list. Deletes itself
@internalTechnology
@test
*/
void CDummyAlarmControl::Release()
	{
	TRAP_IGNORE(PrintWhoWasCalledL(TestMultipleAlarms::ERelease));
	
	iMyManager->RemoveFromList(this);
	delete this;
	}

/**
MEikServAlarm implementation. Prints that it was called.
Asks its manager to increment the number of currently notifying alarms if
it had not already been showing
@internalTechnology
@test
*/
void CDummyAlarmControl::ShowAlarm()
	{
	TRAP_IGNORE(PrintWhoWasCalledL(TestMultipleAlarms::EShowAlarm));
	
	// KNullAlarmId check is done to be safe in case of Single Alarm mode,
	// where some call backs occur before we have the alarm info. In such a 
	// case, no alarm is considered to be notifying
	if(iAlarm.Id() != KNullAlarmId && !iIsShowing)
		{
		iMyManager->IncrementNoOfCurrentlyNotifyingAlarms();
		}
	
	iIsShowing = ETrue;
	}

/**
MEikServAlarm implementation. Prints that it was called.
Asks its manager to decrement the number of currently notifying alarms if
it has been showing. Marks itself as not showing
@internalTechnology
@test
*/
void CDummyAlarmControl::HideAlarm()
	{
	TRAP_IGNORE(PrintWhoWasCalledL(TestMultipleAlarms::EHideAlarm));
	if(iAlarm.Id() != KNullAlarmId && iIsShowing)
		{
		iMyManager->DecrementNoOfCurrentlyNotifyingAlarms();
		}
	iIsShowing = EFalse;
	
	// If the alarm has been StopPlayAlarm-ed and HideAlarm is now being
	// called, it means, this alarm is being hidden as a result of a 
	// snooze or a clear. Hence it must not be consiedered the 
	// CurrentlyPlayingAlarm. We check for iIsPlaying just to make sure
	// that if HideAlarm has been erroneously called before StopPlayAlarm
	// it will get trapped by the TestAlarmControlState step somewhere
	
	// We dont do this in StopPlayAlarm itself, because, if the alarm is
	// left unattended it will stop and start playing periodically, and 
	// we must not consider that this alarm is not the currently notifying one
	// in that case
	if(!iIsPlaying)
		{
		iMyManager->UpdateCurrentlyPlayingAlarmId(KNullAlarmId);
		}
	
	// If in single alarm mode, then we remove the record of the alarm message
	// when the alarm is snoozed or cleared	
	if(iMyManager->CurrentMaxAlarms() == 1)
		{
		iAlarmMessage.Set(KNotKnown);
		}
	}
	
/**
MEikServAlarm implementation. Prints that it was called.
Returns the current state of the alarm control object.
@return The current state of the alarm control object.
@internalTechnology
@test
*/
TInt CDummyAlarmControl::CurrentServerState() const
	{
	TRAP_IGNORE(PrintWhoWasCalledL(TestMultipleAlarms::ECurrentServerState));
	return iCurrentAlarmServerState;
	}
	
/**
MEikServAlarm implementation. Prints that it was called.
Marks itself as Snoozing
@param aMinutes Is not used
@internalTechnology
@test
*/
void CDummyAlarmControl::UpdateSoundPauseTimeInterval(TInt aMinutes)
	{
	_LIT(KPrintMinsToSnooze, "Mins to snooze = ");
	TRAP_IGNORE(PrintWhoWasCalledL(TestMultipleAlarms::EUpdateSoundPauseTimeInterval));
	TBuf<20> message(KPrintMinsToSnooze);
	message.AppendNum(aMinutes);
	iMyManager->PrintLog(message);
	iMinsToSnooze = aMinutes;
	iIsSnoozed = ETrue; 
	}
		
/**
MEikServAlarm implementation. Prints that it was called.
Updates its current alarm server state flag
@param aNewAlarmServerState The new alarm server state.
@internalTechnology
@test
*/
void CDummyAlarmControl::UpdateForAlarmServerState(TInt aNewAlarmServerState)
	{
	TRAP_IGNORE(PrintWhoWasCalledL(TestMultipleAlarms::EUpdateForAlarmServerState));
	iCurrentAlarmServerState = aNewAlarmServerState;
	}

/**
MEikServAlarm implementation. 
Updates its alarm message flag, which will later identify this alarm control 
object. Prints that it was called in the end, as then there is an advantage of 
knowing the alarm message
@param aAlarm The Alarm object that carries the alarm's details
@param aOwner The owner of the alarm. Not used
@internalTechnology
@test
*/
void CDummyAlarmControl::UpdateAlarmInfo(const TASShdAlarm& aAlarm,const TFullName& /* aOwner */)
	{
	// I got notified now
	iMyNotificationTime.UniversalTime();
	
	iAlarm = aAlarm;
	
	// I should have been notified at:
	iOrigExpTime = iAlarm.OriginalExpiryTime();
	
	iAlarmMessage.Set(iAlarm.Message());
	// In this function we print in the end, as we know iAlarmMessage henceforth
	TRAP_IGNORE(PrintWhoWasCalledL(TestMultipleAlarms::EUpdateAlarmInfo));
	}

/**
MEikServAlarm implementation. Prints that it was called.
Asks its manager to upadte the flag identifying the id of the currently playing 
alarm and marks itself as Playing.
@param aAlarmSoundName The alarm sound name to play
@internalTechnology
@test
*/
void CDummyAlarmControl::StartPlayAlarmL(const TDesC& aAlarmSoundName)
	{
	TRAP_IGNORE(PrintWhoWasCalledL(TestMultipleAlarms::EStartPlayAlarm));
	iMyManager->UpdateCurrentlyPlayingAlarmId(iAlarm.Id());
	iIsSnoozed = EFalse;
	iIsPlaying = ETrue;
	
	if(iAlarmSoundName)
		{
		delete iAlarmSoundName;
		iAlarmSoundName = NULL;
		}
	iAlarmSoundName = aAlarmSoundName.AllocL();
	}
	
/**
MEikServAlarm implementation. Prints that it was called.
Marks itself as Not-Playing.
@internalTechnology
@test
*/
void CDummyAlarmControl::StopPlayAlarm()
	{
	TRAP_IGNORE(PrintWhoWasCalledL(TestMultipleAlarms::EStopPlayAlarm));
	iIsPlaying = EFalse;
	}
	
//
// MEikServAlarm callbacks Impelementations end
//
	
	
/**
Returns the alarm-message of the alarm associated with this object
@return The alarm-message of the alarm associated with this object
@internalTechnology
@test
*/
const TDesC& CDummyAlarmControl::AlarmMessage()
	{
	return iAlarmMessage;
	}
	
/**
Returns the alarm-sound name of the alarm associated with this object
@return The alarm-sound name of the alarm associated with this object
@internalTechnology
@test
*/
const TDesC& CDummyAlarmControl::AlarmSoundName()
	{
	return *iAlarmSoundName;
	}

/**
Returns a pointer to the supervisor associated with this alarm control object
@return A pointer to the supervisor associated with this alarm control object
@internalTechnology
@test
*/
CEikAlmControlSupervisor* CDummyAlarmControl::Supervisor()	
	{
	return iSupervisor;
	}
	
/**
Returns the alarm object associated with this alarm control object
@return The alarm object associated with this alarm control object
@internalTechnology
@test
*/
const TASShdAlarm& CDummyAlarmControl::AlarmObject()
	{
	return iAlarm;
	}

/**
Returns the IsSnoozed flag of this object
@return The IsSnoozed flag of this object
@internalTechnology
@test
*/
TBool CDummyAlarmControl::IsSnoozed()
	{
	return iIsSnoozed;
	}

/**
Returns the IsShowing flag of this object
@return The IsShowing flag of this object
@internalTechnology
@test
*/
TBool CDummyAlarmControl::IsShowing()
	{
	return iIsShowing;
	}
	
/**
Returns the actual time the alarm corresponding to this object got notified
@return The actual time the alarm corresponding to this object got notified
@internalTechnology
@test
*/
TTime CDummyAlarmControl::ActualNotificationTime()	
	{
	return iMyNotificationTime;
	}

/**
Returns the original expiry time of the alarm corresponding to this object
@return The original expiry time of the alarm corresponding to this object
@internalTechnology
@test
*/
TTime CDummyAlarmControl::OriginalExpiryTime()	
	{
	return iOrigExpTime;
	}
	
/**
Returns the minutes to snooze value
@return The minutes to snooze value
@internalTechnology
@test
*/
TInt CDummyAlarmControl::MinsToSnooze()	
	{
	return iMinsToSnooze;
	}

	
/**
Prints which call-back was called by the alert server
@param aAlarmControlState The enumeration identifying the call-back
@internalTechnology
@test
*/
void CDummyAlarmControl::PrintWhoWasCalledL(TestMultipleAlarms::TAlarmControlState aAlarmControlState) const
	{
	_LIT(KLineFeed, "\n");
	_LIT(KWasCalledOn, "*** DUMMY ALARM CONTROL *** :- %S was called on %S");
	TPtrC callBackName = iMyManager->GetFunctionName(aAlarmControlState);
	HBufC* message = HBufC::NewL(KWasCalledOn().Length() + callBackName.Length() + iAlarmMessage.Length());
	message->Des().Format(KWasCalledOn(), &callBackName, &iAlarmMessage);
	message->Des().Insert(0, KLineFeed);
	message->Des().Append(KLineFeed);
	iMyManager->PrintLog(message->Des());
	delete message;
	}

