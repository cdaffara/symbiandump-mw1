// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ASSrvNotificationCoordinator.h"

// System includes

// User includes
#include <asaltdefs.h>
#include "ASAltEventHandler.h"
#include "ASAltServerDeathObserver.h"
//
#include "ASSrvTimer.h"
#include "ASSrvAlarmQueue.h"
#include "ASSrvServerWideData.h"
#include "ASSrvSoundController.h"
#include "ASSrvAnyEventManager.h"
#include "asaltrequestsetalarm.h"

#include "ASSrvNotifyingAlarmMngr.h"

// Definitions
#ifdef _DEBUG
#	define __DbgIfAllowNotifications(x)		if (!iPreventUserNotification) { x; }
#else
#	define __DbgIfAllowNotifications(x)		x
#endif

// Constants

// Enumerations

// Classes referenced


//
// ----> CASSrvNotificationCoordinator (source)
//

//*************************************************************************************
CASSrvNotificationCoordinator::CASSrvNotificationCoordinator(CASSrvServerWideData& aServerWideData)
:	CActive(CActive::EPriorityHigh-1), iServerWideData(aServerWideData)
	{
	// NOTE: has a very high active object priority so that it runs as soon as the
	// server thread's active scheduler kicks into action, but *after* any
	// CServer2 related asynchronous operations.
	CActiveScheduler::Add(this);
	}


//*************************************************************************************
CASSrvNotificationCoordinator::~CASSrvNotificationCoordinator()
	{
	Cancel();
	//
	ServerWideData().Queue().RequestAlarmObservationEventsCancel(*this);
	ServerWideData().Queue().NotificationPoolChangeCancel(*this);
	//
	delete iAlarmAlertDeathObserver;
	delete iAlarmAlertEventHandler;
	delete iRequestSetAlarm;
	delete iNotifyingAlarmMngr;
	//
	iAlarmAlertSession.Close();
	}


//*************************************************************************************
void CASSrvNotificationCoordinator::ConstructL()
	{
	// We have to connect to the Alarm Alert Server in an idle
	// operation because of the cyclic dependency between EikSrv
	// and the Alarm Server (during device boot).
#ifdef __SYNCHRONOUS_CONNECTION_ATTEMPT_TO_ALARMALERTSERVER__
	RunL();
#else
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
	SetActive();
#endif
	}


//*************************************************************************************
CASSrvNotificationCoordinator* CASSrvNotificationCoordinator::NewL(CASSrvServerWideData& aServerWideData)
	{
	CASSrvNotificationCoordinator* self = new(ELeave) CASSrvNotificationCoordinator(aServerWideData);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}


//
//
//


//*************************************************************************************
/**
 * @see MASSrvAlarmTimerObserver
 */


//
//
//


//*************************************************************************************
/**
 * @see MASAltAlarmAlertObserver
 */
void CASSrvNotificationCoordinator::ASAltSilenceCurrentAlarm(TAlarmId aAlarmId)
	{
	ServerWideData().SoundController().CancelSound(aAlarmId);
	}

//*************************************************************************************
/**
 * @see MASAltAlarmAlertObserver
 */
void CASSrvNotificationCoordinator::ASAltAcknowledgeAlarm(TAcknowledgeType aType, TAlarmId aAlarmId)
	{
	// Currently notifying alarm list can be reset by Restore/Internalize
	if (iNotifyingAlarmMngr->AmNotifyingAboutAlarm())
		{
		CASSrvAlarmQueue& queue = ServerWideData().Queue();
		if	(aType == EAcknowledgeAllAlarms)
			{
			// Cancel event change notifications whilst we update
			// the state of all the waiting alarms to "notified". If we don't
			// do this, then the alarm alert server will be updated for each
			// of the state changes where the old or new state is Notifying.
			queue.RequestAlarmObservationEventsEnabled(*this, EFalse);

			// Any alarms which are waiting for notification (but haven't
			// yet notified) should be cleared.
			TInt pendingAlarmCount(queue.NumberOfAlarmsPendingNotification());
			TInt i(0);
			for(; i < pendingAlarmCount; i++)
				{
				TASSrvAlarm& alarm = queue.NextAlarmWaitingForNotification();
				alarm.SetState(EAlarmStateNotified);
				}

			ServerWideData().SoundController().CancelAllSounds();

			__DbgIfAllowNotifications(AlarmAlert().SetVisibility(EFalse));
			__DbgIfAllowNotifications(AlarmAlert().DeleteAlarm());

			iNotifyingAlarmMngr->AcknowledgeAllNotifyingAlarms();

			queue.RequestAlarmObservationEventsEnabled(*this, ETrue);
			}
		else // aType != EAcknowledgeAllAlarms
			{
			if (iNotifyingAlarmMngr->AlarmIsNotifying(aAlarmId))
				{
				TASSrvAlarm* alarm = queue.QueueAlarmById(aAlarmId);
				alarm->SetState(EAlarmStateNotified);
				iNotifyingAlarmMngr->FindAndExecuteNextAlarmAwaitingNotification();
				}
			}
		}
	}


//*************************************************************************************
/**
 * @see MASAltAlarmAlertObserver
 */
TInt CASSrvNotificationCoordinator::ASAltSnoozeAlarmUntil(const TTime& aTimeWhenAlarmToReAwaken, TAlarmId aAlarmId)
	{
	// Currently notifying alarm list can be reset by Restore/Internalize
	if (iNotifyingAlarmMngr->AlarmIsNotifying(aAlarmId))
		{
		CASSrvAlarmQueue& queue = ServerWideData().Queue();
		
		TASSrvAlarm* alarm = queue.QueueAlarmById(aAlarmId);
		alarm->Snooze(aTimeWhenAlarmToReAwaken);
		
		iNotifyingAlarmMngr->FindAndExecuteNextAlarmAwaitingNotification();
		}

	return KErrNone;
	}

//*************************************************************************************
/**
 * @see MASAltAlarmAlertObserver
 */
void CASSrvNotificationCoordinator::ASAltQuietPeriodUntil(const TTime& aTimeWhenSoundsShouldResume)
	{
	// We don't adjust the alarm's state, we just turn the sound off for
	// a bit.
	CASSrvSoundController& soundController = ServerWideData().SoundController();
	soundController.MakeAllSoundsQuietUntil(aTimeWhenSoundsShouldResume);
	}


//*************************************************************************************
/**
 * @see MASAltAlarmAlertObserver
 */
void CASSrvNotificationCoordinator::ASAltPauseSoundUntil(const TTime& aTimeWhenSoundsShouldResume, TAlarmId aAlarmId)
	{
	if (!iNotifyingAlarmMngr->AlarmIsNotifying(aAlarmId))
		{
		return;
		}
	
	CASSrvAlarmQueue& queue = ServerWideData().Queue();
 	TASSrvAlarm* alarm = queue.QueueAlarmById(aAlarmId);
	alarm->SetSoundPausedFlag();
	alarm->NextDueTime() = aTimeWhenSoundsShouldResume;
 	if (queue.HaveAdditionalAlarmsToNotify() || iNotifyingAlarmMngr->NumberOfNotifyingAlarm() > 1)
		{
 		// If there are alarms waiting to notify or there are other notifying 
 		// alarms, we should snooze the notifying alarm
		alarm->SetState(EAlarmStateSnoozed);
		iNotifyingAlarmMngr->FindAndExecuteNextAlarmAwaitingNotification();
		}
	else
		{
		CASSrvSoundController& soundController = ServerWideData().SoundController();

		soundController.SetFixed();	
		soundController.CurrentAlarmSoundPausedUntil(aTimeWhenSoundsShouldResume);
		}
	}

//
//
//


//*************************************************************************************
/**
 * @see MASSrvSoundControllerObserver
 */
void CASSrvNotificationCoordinator::MASHandleSoundControllerEvent(TSoundControllerEvent aEvent, TAlarmId aAlarmId)
	{
	// If we're still constructing ourself then return straight away
	if (IsActive())
		{
		return;
		}

	if (aEvent == ESoundControllerEventPlaySound)
		{
		// Get the alarm from the pool
		const TASSrvAlarm* alarm = ServerWideData().Queue().QueueAlarmById(aAlarmId);
	
		if (!alarm->Characteristics().IsSet(EAlarmCharacteristicsDoNotNotifyAlarmAlertServer))
			{
			__ASSERT_DEBUG(iNotifyingAlarmMngr->AlarmIsNotifying(aAlarmId), ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicNotifyingAboutWrongAlarm));
	
			// Show the alarms and start playing the specified sound file
			__DbgIfAllowNotifications(AlarmAlert().SetVisibility(ETrue)); 
			__DbgIfAllowNotifications(AlarmAlert().StartPlayingSound(alarm->SoundName(), aAlarmId));
			}
		
		ServerWideData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventSoundPlaying, aAlarmId);
		}
	else if (aEvent == ESoundControllerEventPlayNothing)
		{
		// Stop playing the current sound
		if (aAlarmId == KNullAlarmId)
			{
			__DbgIfAllowNotifications(AlarmAlert().StopPlayingSound());
			}
		else
			{
			__DbgIfAllowNotifications(AlarmAlert().StopPlayingSound(aAlarmId));
			}

		// Notify change
		ServerWideData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventSoundStopped, aAlarmId);
		}
	}

//
//
//


//*************************************************************************************
/**
 * @see MASSrvSoundControllerObserver
 */
void CASSrvNotificationCoordinator::MASSrvAlarmObsHandleEvent(TObserverEvent aEvent, const TASSrvAlarm& aAlarm, TInt aEventSpecificData)
	{
	// If we're still constructing ourself then return straight away
	if	(IsActive())
		return;

	switch(aEvent)
		{
	case EAlarmObserverStateChanged:
		HandleAlarmStateChange(aAlarm, static_cast<TAlarmState>(aEventSpecificData));
		break;
	case EAlarmObserverStatusChanged:
		HandleAlarmStatusChange(aAlarm, static_cast<TAlarmStatus>(aEventSpecificData));
		break;
	default:
		__ASSERT_DEBUG(EFalse, ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicInvalidTObserverEvent));
		break;
		}
	}


//
//
//


//*************************************************************************************
/**
 * @see MASSrvAlarmQueueObserver
 */
void CASSrvNotificationCoordinator::MAlarmQueueObserverHandleEvent(TASSrvAlarmQueueEvent aEvent, TAlarmId aAlarmId)
	{
	if (aEvent == EASSrvAlarmQueueEventAlarmDeleted)
		{
		// alarting alarm may have been deleted
		if (iNotifyingAlarmMngr->AlarmIsNotifying(aAlarmId))
			{
			iRequestSetAlarm->RequestSetAlarmCancel(aAlarmId);
			
			ServerWideData().SoundController().CancelSound(aAlarmId);
			
			// We're no longer interested in notifying about this dead alarm, so locate
			// the next available one in the queue to work with.
			DeleteNotifyingAlarm(aAlarmId);
			iNotifyingAlarmMngr->FindAndExecuteNextAlarmAwaitingNotification();
			}
		}
	else if (aEvent == EASSrvAlarmQueueEventAlarmStartInternalize)
		{
		// ensure there is no alerting alarm
		if (iNotifyingAlarmMngr->AmNotifyingAboutAlarm())
			{
			ServerWideData().SoundController().CancelAllSounds();
			DeleteAllNotifyingAlarms();
			CompleteUpdateAlarmAlertServer(NULL, ETrue);
			}
		}
	}

//
//
//


//*************************************************************************************
/**
 * @see CActive
 *
 * Completes the construction of this object. Panics if it isn't possible
 * to connect to the Alarm Alert Server. Has to be done as an idle
 * callback since this would cause a nasty circular deadlock dependency
 * if performed from the ConstructL function (since EikSrv is waiting on
 * the Alarm Server to start up, and the Alarm Server is waiting for EikSrv
 * to respond to it's "Create me an Alarm Alert session" request).
 */
void CASSrvNotificationCoordinator::RunL()
	{
	const TInt error = FinalizeConstruction();
	if (error != KErrNone)
		User::Exit(error); // don't panic - just exit in the same way that we would if the server's earlier initialization had failed

#ifndef __SYNCHRONOUS_CONNECTION_ATTEMPT_TO_ALARMALERTSERVER__
	UpdateAlarmAlertServer(KNullAlarmId);
#endif //__SYNCHRONOUS_CONNECTION_ATTEMPT_TO_ALARMALERTSERVER__
	}


//*************************************************************************************
/**
 * Cancel the idle construction
 */
void CASSrvNotificationCoordinator::DoCancel()
	{
	// Do nothing - there isn't anything to cancel since we completed
	// our own request.
	}


//
//
//


//*************************************************************************************
#ifdef _DEBUG
void CASSrvNotificationCoordinator::__DbgPreventUserNotify(TBool aAllowNotification)
	{
	iPreventUserNotification = aAllowNotification;
	}
#endif


//
//
//


//*************************************************************************************
/**
 * Update the status of the Alarm Alert Server, including visibility and
 * various flags.
 */
void CASSrvNotificationCoordinator::UpdateAlarmAlertServer(TAlarmId aAlarmId)
	{
	// If we're still trying to connect to the Alarm Alert Server
	// then there is nothing to be done here (yet)
	if (IsActive())
		{
		return;
		}

	CASSrvAlarmQueue& queue = ServerWideData().Queue();

	if (aAlarmId != KNullAlarmId)
		{
		if (!iNotifyingAlarmMngr->AlarmIsNotifying(aAlarmId))
			{
			return;
			}
		
		const TASSrvAlarm* alarm = queue.QueueAlarmById(aAlarmId);
		
		// If the alarm object specifies that we shouldn't tell the alarm
		// alert server, then we remove the alarm from alarm notifying list 
		// and return straight away here
		if (alarm->Characteristics().IsSet(EAlarmCharacteristicsDoNotNotifyAlarmAlertServer))
			{
			DeleteNotifyingAlarm(aAlarmId, EFalse);
			return;
			}

		// Notify the alarm alert server about the alarm and its associated
		__DbgIfAllowNotifications(iRequestSetAlarm->RequestSetAlarm(alarm->Id()));
		//CompleteUpdateAlarmAlertServer called by CASAltRequestSetAlarm on request completion
		}
	else
		{
		CompleteUpdateAlarmAlertServer(NULL);
		}
	}

void CASSrvNotificationCoordinator::CompleteUpdateAlarmAlertServer(const TASShdAlarm* aAlarm, TBool aInternalizing)
	{
	CASSrvSoundController& soundController = ServerWideData().SoundController();
	TBitFlags stateFlags(0);
	TBool notifyingAboutAlarm(EFalse);
	TAlarmId alarmId(KNullAlarmId);
	if (aAlarm != NULL)
		{
		notifyingAboutAlarm = ETrue;
		alarmId = aAlarm->Id();
		}

	if (notifyingAboutAlarm)
		{
		stateFlags.Assign(EASAltStateFlagsAlarmHasNoSoundFileName, aAlarm->SoundName().Length() == 0);
		}

	// if showing an alarm this fn is called by a CASAltRequestSetAlarm active object
	// (added previously to relieve a deadlock problem with the alert server)
	// However this introduces a small delay, so now we need to check the alarm 
	// is still alerting.
	// (DEF030901 - Alarm could be deleted in the meantime, and the Alarm Sound player may
	// have already forced the Alarm Alert Server/Client to make the Alarm popup window
	// visible so the alarm may have beeb cleared or snoozed by the user.)
	if ((aAlarm == NULL) || (iNotifyingAlarmMngr->AlarmIsNotifying(aAlarm->Id())))
		{
		// Now show or hide the alarm alert server furniture as appropriate
		if (notifyingAboutAlarm)
			{
			__DbgIfAllowNotifications(AlarmAlert().SetVisibility(ETrue, alarmId));
			ServerWideData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventAlarmUIVisible, KNullAlarmId);
			}
		else
			{
			__DbgIfAllowNotifications(AlarmAlert().SetVisibility(EFalse));
			ServerWideData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventAlarmUIInvisible, KNullAlarmId);
			}

		// Send other details
		stateFlags.Assign(EASAltStateFlagsSoundIsPaused, soundController.SoundIsPausedForAlarm());
		stateFlags.Assign(EASAltStateFlagsInQuietPeriod, soundController.InQuietPeriod());
		stateFlags.Assign(EASAltStateFlagsMoreThanOneDueAlarm, ServerWideData().Queue().HaveAdditionalAlarmsToNotify());

		// There is always memory to snooze an alarm in the new Alarm Server. This enum should
		// maybe be removed.
		stateFlags.Assign(EASAltStateFlagsNoMemoryForSnoozeAlarm, EFalse);
		//
		if (notifyingAboutAlarm)
			{
			__DbgIfAllowNotifications(AlarmAlert().SetAlertServerState(stateFlags, alarmId));
			}
		else
			{
			__DbgIfAllowNotifications(AlarmAlert().SetAlertServerState(stateFlags));
			}
		}
	
	if (!aInternalizing) // If internalize is pending, alarm queue contains alarms will be replaced
		{
		// Try to find the next alarm to notify, if a next alarm is not found, don't update
		// alert server because alert server already get the latest status from above
		iNotifyingAlarmMngr->FindAndExecuteNextAlarmAwaitingNotification(EFalse);
		}
	}

//*************************************************************************************
/**
 * Update this class' and the specified alarm's state to indicate that it
 * is currently alerting.
 */

//*************************************************************************************
/**
 * Called from the idle-construction callback, and connects to the
 * Alarm Alert Server. Has to be completed as an idle callback, otherwise
 * deadlock during device boot will occur.
 */
TInt CASSrvNotificationCoordinator::FinalizeConstruction()
	{
	// Complete the construction of this class.
	TInt error = iAlarmAlertSession.Connect();
	if	(error != KErrNone)
		return error;
	
	TInt maxAlarm(0);
	__DbgIfAllowNotifications(AlarmAlert().GetMaxNumberOfAlarms(maxAlarm));
	if (maxAlarm <= 0)
		{
		return KErrArgument;
		}

	CASSrvAlarmQueue& queue = ServerWideData().Queue();
	MASSrvSessionCollection& sessions = ServerWideData().SessionCollection();

	TRAP(error,
		iAlarmAlertEventHandler = CASAltEventHandler::NewL(iAlarmAlertSession, *this);
		iAlarmAlertDeathObserver = CASAltServerDeathObserver::NewL(iAlarmAlertSession);
		iRequestSetAlarm = CASAltRequestSetAlarm::NewL(iAlarmAlertSession, *this, queue, sessions);
		iNotifyingAlarmMngr = CASSrvNotifyingAlarmMngr::NewL(*this, ServerWideData());
		// Setup for notifications
		ServerWideData().Queue().NotificationPoolChangeL(*this);
		ServerWideData().Queue().RequestAlarmObservationEventsL(*this);
		);

	iNotifyingAlarmMngr->SetMaxNumberOfAlarms(maxAlarm);
	return error;
	}


//*************************************************************************************
/**
 * Locate and initiate notifications for the next alarm in the queue that is waiting
 */

//
//
//


//*************************************************************************************
/**
 * Handle a change in state by an alarm
 */
void CASSrvNotificationCoordinator::HandleAlarmStateChange(const TASSrvAlarm& aAlarm, TAlarmState aOldState)
	{
	TAlarmState newState(aAlarm.State());

	// Depending on the type of state change, we need to perform
	// various actions...
	switch(newState)
		{
	case EAlarmStateWaitingToNotify:
	case EAlarmStateNotified:
	case EAlarmStateSnoozed:
	case EAlarmStateQueued:
		if (aOldState == EAlarmStateNotifying)
			{
			DeleteNotifyingAlarm(aAlarm.Id());
			}
		break;

	case EAlarmStateNotifying:
		// This alarm is now ready to alert. The sound controller also
		// observes these state changes, so it will handle the sound aspect.
		__ASSERT_DEBUG(iNotifyingAlarmMngr->AlarmIsNotifying(aAlarm.Id()), 
			ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicNotifyingAlarmIsNotAlertingAlarm));

		// This object, however, has to update the Alarm Alert Server so
		// that it knows about the right alarm.
		UpdateAlarmAlertServer(aAlarm.Id());

		// We are already dealing with the head item, so we don't want
		// to handle any of the "waiting" alarms yet.
		break;

	case EAlarmStateInPreparation:
	// We're not interested in these kind of events
		break;

	default:
		ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicInvalidAlarmState);
		break;
		}
	}


//*************************************************************************************
/**
 * Handle a change in status by an alarm
 */
void CASSrvNotificationCoordinator::HandleAlarmStatusChange(const TASSrvAlarm& aAlarm, TAlarmStatus /*aOldStatus*/)
	{
	// We're only interested in certain alarms here. Those that used to
	// be in the alerting queue, and those that already are alerting.
	// Note that there should never be any alarms that are in the alerting
	// state and are disabled!
	if	(aAlarm.Status() == EAlarmStatusDisabled)
		{
		// If the alarm's state was "Notifying" then we need to put it back
		// to "In the queue". No alarm can be in the disabled and alerting
		// state.
		TASSrvAlarm& alarm = const_cast<TASSrvAlarm&>(aAlarm);
		if	(aAlarm.State() == EAlarmStateNotifying)
			{
			alarm.SetState(EAlarmStateWaitingToNotify);
			iNotifyingAlarmMngr->FindAndExecuteNextAlarmAwaitingNotification();
			}
		}
	}

//*************************************************************************************
/**
 * Update notifying alarm manager that an alarm is no longer notifying
 * and Update alert server if aUpdateAlertServer is ETrue
 */
void CASSrvNotificationCoordinator::DeleteNotifyingAlarm(TAlarmId aAlarmId, TBool aUpdateAlertServer)
	{
	TBool ret(iNotifyingAlarmMngr->RemoveNotifyingAlarm(aAlarmId));

	if (ret && aUpdateAlertServer)
		{
		__DbgIfAllowNotifications(AlarmAlert().SetVisibility(EFalse, aAlarmId));
		__DbgIfAllowNotifications(AlarmAlert().DeleteAlarm(aAlarmId));
		}
	}

//*************************************************************************************
/**
 * Remove all alarms from notifying alarm list
 */
void CASSrvNotificationCoordinator::DeleteAllNotifyingAlarms()
	{
	__DbgIfAllowNotifications(AlarmAlert().SetVisibility(EFalse));
	__DbgIfAllowNotifications(AlarmAlert().DeleteAlarm());
	iNotifyingAlarmMngr->RemoveAllNotifyingAlarms();
	}

//*************************************************************************************
/**
 * Pending Alert server response for the set alarm request
 */
TBool CASSrvNotificationCoordinator::SetAlarmInProgress()
	{
	return iRequestSetAlarm->IsActive();
	}

#ifdef _DEBUG
//*************************************************************************************
/**
 * Return to status of iPreventUserNotify
 */
TBool CASSrvNotificationCoordinator::PreventUserNotification()
	{
	return iPreventUserNotification;
	}
#endif // _DEBUG

