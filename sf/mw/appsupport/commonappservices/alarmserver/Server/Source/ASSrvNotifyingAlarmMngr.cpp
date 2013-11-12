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
// Implementation of CASSrvNotifyingAlarmMngr
// 
//

/**
 @file
 @internalTechnology
*/

#include "ASSrvNotifyingAlarmMngr.h"
#include "ASSrvServerWideData.h"
#include "ASSrvTimer.h"
#include "ASSrvAlarmQueue.h"
#include "ASSrvNotificationCoordinator.h"

/**
Standard 2 phase constructor
*/
CASSrvNotifyingAlarmMngr* CASSrvNotifyingAlarmMngr::NewL(CASSrvNotificationCoordinator& aParent, CASSrvServerWideData& aServerWideData)
	{
	CASSrvNotifyingAlarmMngr* self = new (ELeave) CASSrvNotifyingAlarmMngr(aParent, aServerWideData);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
Destructor
*/
CASSrvNotifyingAlarmMngr::~CASSrvNotifyingAlarmMngr()
	{
	ServerWideData().Timer().NotifyAlarmExpiredCancel(*this);
	iCurrentlyNotifyingAlarmIds.Close();
	}

/**
Standard 2nd phase constractor
*/
void CASSrvNotifyingAlarmMngr::ConstructL()
	{
	ServerWideData().Timer().NotifyAlarmExpiredL(*this);
	}

/**
Standard private constructor
*/
CASSrvNotifyingAlarmMngr::CASSrvNotifyingAlarmMngr(CASSrvNotificationCoordinator& aParent, CASSrvServerWideData& aServerWideData)
:iParent(aParent), iServerWideData(aServerWideData)
	{
	}

/**
Set the maximum number of alarms supported by Alert Server
*/
void CASSrvNotifyingAlarmMngr::SetMaxNumberOfAlarms(TInt aMaxAlarms)
	{
	iMaxNumberOfAlarms = aMaxAlarms;
	}

/**
@see MASSrvAlarmTimerObserver
*/
void CASSrvNotifyingAlarmMngr::MATimerHandleAlarmExpired(TAlarmTimerEvent aEvent, TAlarmId aAlarmId)
	{
	if (aEvent != EAlarmTimerEventAlarmExpired)
		{
		return;
		}

	CASSrvAlarmQueue& queue = ServerWideData().Queue();
	TASSrvAlarm* justExpiredAlarm = queue.QueueAlarmById(aAlarmId);

	if (SoleNotifyingAlarmHasSoundPaused())
	// If there's 1 alarm notifying and it has it's sound paused,
		{
		// then we'll snooze it and take it's place.
		TASSrvAlarm* pausedAlarm = queue.QueueAlarmById(iCurrentlyNotifyingAlarmIds[0]);
		pausedAlarm->SetState(EAlarmStateSnoozed);
		SetAsNextNotifyingAlarm(aAlarmId);
		}
	else if (AllowMoreNotifyingAlarms())
	// If the number of notifying alarm is less than maximum allowed
		{
		if (iParent.SetAlarmInProgress())
		// Alert server hasn't replied to the previous alarm...
			{
			// set the alarm state to waiting to notify
			justExpiredAlarm->SetState(EAlarmStateWaitingToNotify);
			}
		else 
			{
			SetAsNextNotifyingAlarm(aAlarmId);
			}
		}
	else
	// The max number of allowed alarm has reached, set the state of the just 
	// expired alarm depending on its exipry time
		{
		TAlarmId youngestAlarmId(YoungestNotifyingAlarmId());
		TASSrvAlarm* youngestAlarm = queue.QueueAlarmById(youngestAlarmId);
		if	(justExpiredAlarm->OriginalExpiryTime() < youngestAlarm->OriginalExpiryTime() && !justExpiredAlarm->HasSoundPaused())
			{
			// The alarm that just expired is actually older than an alarm
			// we're notifying about. This kind of thing can occur
			// when somebody adds an alarm to the alarm server which is 
			// in the past, e.g a day in the past, and the alarm server is 
			// already notifying about an alarm which is 1 minute old.
			//
			// In this case, an alarm which we were notifying about should no
			// longer in the notifying queue, therefore we set its state back
			// to "waiting to notify" and then proceed to notify about this
			// new alarm.
			youngestAlarm->SetState(EAlarmStateWaitingToNotify);
			SetAsNextNotifyingAlarm(aAlarmId);
			}
		else
			{
			justExpiredAlarm->SetState(EAlarmStateWaitingToNotify);
			}
		}
	}

/**
Return the server data object.
*/
CASSrvServerWideData& CASSrvNotifyingAlarmMngr::ServerWideData() const
	{
	return iServerWideData;
	}

/**
Set the alarm with the given alarm id to be the next notifying alarm
@param aAlarmId the id of the alarm to set to notifying state
*/
void CASSrvNotifyingAlarmMngr::SetAsNextNotifyingAlarm(TAlarmId aAlarmId)
	{
	// This function shouldn't be called if max # of alarm has reached
	__ASSERT_DEBUG(AllowMoreNotifyingAlarms(), 
		ASSrvStaticUtils::Fault(ASSrvStaticUtils::EASSrvFaultNotifyAlarmExceedMax));

	CASSrvAlarmQueue& queue = ServerWideData().Queue();
	TASSrvAlarm* alarm = queue.QueueAlarmById(aAlarmId);

	// Update the alarm we are notifying about
	if (iCurrentlyNotifyingAlarmIds.Append(aAlarmId) != KErrNone)
		{
		ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicOutOfMemory);
		}

	// Now we can start notifying this new alarm. Because this class observes all
	// state or status changes, we will be notified about the change and will
	// update the Alarm Alert Server accordingly.
	TAlarmState state(EAlarmStateNotifying);
#ifdef _DEBUG
	if (iParent.PreventUserNotification())
		{
		state = EAlarmStateNotified;
		// Remove the alarm from notifying list
		iCurrentlyNotifyingAlarmIds.Remove(iCurrentlyNotifyingAlarmIds.Count() - 1);
		}
#endif
	alarm->SetState(state);
	}

TBool CASSrvNotifyingAlarmMngr::AmNotifyingAboutAlarm() const
	{
	return (iCurrentlyNotifyingAlarmIds.Count() > 0);
	}

/**
Locate and initiate notifications for the next alarm in the queue that is waiting
@param aUpdate if true, update alert server if next alarm is not available
*/
void CASSrvNotifyingAlarmMngr::FindAndExecuteNextAlarmAwaitingNotification(TBool aUpdate)
	{
	if(AllowMoreNotifyingAlarms() && ServerWideData().Queue().HaveAdditionalAlarmsToNotify())
		{
		TASSrvAlarm& alarm = ServerWideData().Queue().NextAlarmWaitingForNotification();
		SetAsNextNotifyingAlarm(alarm.Id());
		}
	else if (aUpdate && !AmNotifyingAboutAlarm())
		{
		// Alarm server has no more alarm to notify, so update Alert Server
		// visibility and flags
		iParent.UpdateAlarmAlertServer(KNullAlarmId);
		}
	}

/**
Check if the given alarm is in the alarm notifying list
*/
TBool CASSrvNotifyingAlarmMngr::AlarmIsNotifying(TAlarmId aAlarmId) const
	{
	if (iCurrentlyNotifyingAlarmIds.Find(aAlarmId) != KErrNotFound)
		{
		return ETrue;
		}
	return EFalse;	
	}

/**
Check if alert server can accept more notifying alarm
*/
TBool CASSrvNotifyingAlarmMngr::AllowMoreNotifyingAlarms() const
	{
	if (iCurrentlyNotifyingAlarmIds.Count() < iMaxNumberOfAlarms)
		{
		return ETrue;
		}
	return EFalse;	
	}

/*
Returns the Alarm Id for the "youngest" notifying alarm, ie. with the largest (latest) OriginalExpiryTime.
*/
TAlarmId CASSrvNotifyingAlarmMngr::YoungestNotifyingAlarmId() const
	{
	// This function shouldn't be called if there are no notifying alarms
	__ASSERT_DEBUG(iCurrentlyNotifyingAlarmIds.Count(), 
		ASSrvStaticUtils::Fault(ASSrvStaticUtils::EASSrvFaultNoNotifyingAlarms));

	CASSrvAlarmQueue& queue = ServerWideData().Queue();

	TAlarmId youngestAlarm(iCurrentlyNotifyingAlarmIds[0]);
	TTime latestTime(queue.QueueAlarmById(youngestAlarm)->OriginalExpiryTime());

	for(TInt i = iCurrentlyNotifyingAlarmIds.Count()-1; i >= 1 ; i--)
		{
		TASSrvAlarm* currentAlarm = queue.QueueAlarmById(iCurrentlyNotifyingAlarmIds[i]);

		if	(latestTime < currentAlarm->OriginalExpiryTime())
			{
			youngestAlarm = currentAlarm->Id();
			latestTime = queue.QueueAlarmById(youngestAlarm)->OriginalExpiryTime();
			}
		}

	return youngestAlarm;
	}


/*
Determine if there is only 1 Notifying Alarm and if has it's Sound Paused.
@return ETrue if this is true. Otherwise EFalse
*/
TBool CASSrvNotifyingAlarmMngr::SoleNotifyingAlarmHasSoundPaused() const
	{
	if( iCurrentlyNotifyingAlarmIds.Count() != 1 )
		{
		return EFalse;
		}

	TASSrvAlarm* notifyingAlarm = ServerWideData().Queue().QueueAlarmById(iCurrentlyNotifyingAlarmIds[0]);

	if	(notifyingAlarm->HasSoundPaused())
		{
		return ETrue;
		}

	return EFalse;
	}

/**
Remove an alarm from the notifying alarm list
@param aAlarmId the id of the alarm to be removed
@return ETrue if the alarm was in the notifying list before removal. Otherwise EFalse
*/
TBool CASSrvNotifyingAlarmMngr::RemoveNotifyingAlarm(TAlarmId aAlarmId)
	{
	TInt alarmIndex(iCurrentlyNotifyingAlarmIds.Find(aAlarmId));
	if (alarmIndex == KErrNotFound)
		{
		return EFalse;
		}
	
	iCurrentlyNotifyingAlarmIds.Remove(alarmIndex);
	return ETrue;
	}

/**
Remove all alarms from the notifying alarm list
*/
void CASSrvNotifyingAlarmMngr::RemoveAllNotifyingAlarms()
	{
	iCurrentlyNotifyingAlarmIds.Reset();
	}

/**
Return the number of the notifying alarm
*/
TInt CASSrvNotifyingAlarmMngr::NumberOfNotifyingAlarm()
	{
	return iCurrentlyNotifyingAlarmIds.Count();
	}

/**
Set all notifying alarm to Notified state
*/
void CASSrvNotifyingAlarmMngr::AcknowledgeAllNotifyingAlarms()
	{
	CASSrvAlarmQueue& queue = ServerWideData().Queue();

	TInt count(iCurrentlyNotifyingAlarmIds.Count());
	for (TInt i = 0; i < count; i++)
		{
		TASSrvAlarm* alarm = queue.QueueAlarmById(iCurrentlyNotifyingAlarmIds[i]);
		alarm->SetState(EAlarmStateNotified);
		}
		
	// Because we disabled the AlarmObservers, we have to
	// manually remove the alarms
	iCurrentlyNotifyingAlarmIds.Reset();
	}
