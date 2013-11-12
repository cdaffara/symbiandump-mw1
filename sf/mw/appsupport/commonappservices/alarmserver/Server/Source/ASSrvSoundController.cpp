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

#include "ASSrvSoundController.h"

// System includes

// User includes
#include "ASSrvAlarm.h"
#include "ASSrvAlarmQueue.h"
#include "ASSrvStaticUtils.h"
#include "ASSrvSoundSettings.h"
#include "ASSrvServerWideData.h"
#include "ASSrvAnyEventManager.h"
#include "ASSrvSoundControllerObserver.h"
#include "ASSrvEnvironmentChangeManager.h"

#include "ASSrvIteratorByState.h"
#include "ASSrvNotificationCoordinator.h"


//
// ----> CASSrvSoundController (source)
//

	/**
	 * Indicates that the Sound Controller is disabled (ie. no sounds
	 * intervals configured in the rsc file)
	 */
TBool CASSrvSoundController::IsSoundControllerDisabled() const
	{
	return 0==ServerData().SoundSettings().SoundIntervalCount();
	}

//*************************************************************************************
CASSrvSoundController::CASSrvSoundController(CASSrvServerWideData& aServerWideData, MASSrvSoundControllerObserver& aObserver)
:	CTimer(CActive::EPriorityUserInput), iServerWideData(aServerWideData), iObserver(aObserver)
	{
	iPreviousSoundEvent = MASSrvSoundControllerObserver::ESoundControllerEventPlayNothing;
	CActiveScheduler::Add(this);
	}


//*************************************************************************************
CASSrvSoundController::~CASSrvSoundController()
	{
	Cancel();

	// It's ok to Cancel these even if we didn't register for the them:
	ServerData().EnvironmentChangeManager().RequestEnvironmentChangesCancel(*this);
	ServerData().SoundSettings().NotifySoundSettingsChangeCancel(*this);
	ServerData().Queue().RequestAlarmObservationEventsCancel(*this);
	ServerData().Queue().NotificationPoolChangeCancel(*this);
	}


//*************************************************************************************
void CASSrvSoundController::ConstructL()
	{
	CTimer::ConstructL();

	if( !IsSoundControllerDisabled() )
		{
		ServerData().Queue().NotificationPoolChangeL(*this);
		ServerData().Queue().RequestAlarmObservationEventsL(*this);
		ServerData().SoundSettings().NotifySoundSettingsChangeL(*this);
		ServerData().EnvironmentChangeManager().RequestEnvironmentChangesL(*this);
		iPreviousUTCOffset = User::UTCOffset();
		iSoundControllerFlags.Set(ESoundControllerFlagsIsFixed);
		}
	}


//*************************************************************************************
CASSrvSoundController* CASSrvSoundController::NewL(CASSrvServerWideData& aServerWideData, MASSrvSoundControllerObserver& aObserver)
	{
	CASSrvSoundController* self = new(ELeave) CASSrvSoundController(aServerWideData, aObserver);
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
 * @see MASSrvAlarmObserver
 *
 * When an alarm expires, we should start timing the alarm sound intervals. Note that
 * no sound is played directly by the alarm server - it's all handled by proxy in the
 * EikSrv thread (Alarm Alert Server).
 * 
 * We only ever receive one notification that an alarm has expired (unless its snoozed
 * and then the snooze expires). Therefore, the sound controller has to perform cycle
 * management for the alerting (notifying) head alarm in the queue).
 */
void CASSrvSoundController::MASSrvAlarmObsHandleEvent(TObserverEvent aEvent, const TASSrvAlarm& aAlarm, TInt aEventSpecificData)
	{
	// Get a modifiable version of the alarm
	TASSrvAlarm* alarm = ServerData().Queue().QueueAlarmById(aAlarm.Id());

	if	(aEvent == EAlarmObserverStateChanged)
		{
		// Notifying / snoozed / notified / etc
		const TAlarmState oldState = static_cast<TAlarmState>(aEventSpecificData);
		if	(oldState == EAlarmStateNotifying && IdOfNotifyingAlarm() == aAlarm.Id())
			{
			// If the alarm was notifying, but now it's back in the queue
			// then we need to cancel any sound playing in preparation for
			// the new notifying alarm
			if	(!InQuietPeriod())
				{
				Cancel();
				}
				
			if (SoundIsPausedForAlarm())
				{
				iSoundControllerFlags.Clear(ESoundControllerFlagsPausing);
				}					

			// Reset this now
			IdOfNotifyingAlarm() = KNullAlarmId;
			}
		//
		switch(aAlarm.State())
			{
		case EAlarmStateNotifying:
			{
			// The only thing we really care about is that the new notifying alarm is not null.
			__ASSERT_DEBUG(aAlarm.Id() != KNullAlarmId, 
				ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicQueueAlarmIdNull));

			// Ensure Alarm Disabled status change didn't slip in ahead in server queue
			if (alarm->Status() == EAlarmStatusEnabled)
				{
				if	(InQuietPeriod())
					{
					// delay any sound playing until the quiet period ends
					alarm->SetSoundState(TASSrvAlarm::ESoundStatePlayingNothing);
					IdOfNotifyingAlarm() = aAlarm.Id();
					}
				else
					{
					Cancel();

					// Work out when we should start playing the sound
					IdOfNotifyingAlarm() = aAlarm.Id();
					WorkOutAndScheduleForNextSoundCycle();
					}
				}
 
			break;
			}

		case EAlarmStateWaitingToNotify:
		case EAlarmStateSnoozed:
		case EAlarmStateNotified:
			// An alarm is no longer notifying, this is handle above already
			break;
		
		// We're not interested in these kind of events
		case EAlarmStateQueued:
		case EAlarmStateInPreparation:
			return;
			}
		}
	else if (aEvent == EAlarmObserverStatusChanged)
		{
		// Enabled / disabled
		if	(aAlarm.Id() == IdOfNotifyingAlarm() && alarm->Status() == EAlarmStatusDisabled)
			{
			// Cancel sound & timer
			if	(!InQuietPeriod())
				{
				Cancel();
				}

			// Reset this now
			IdOfNotifyingAlarm() = KNullAlarmId;
			}
		}
		
	// If there is no alarm playing (b/c the previous playing alarm is snoozed,
	// notified, disabled etc), but there are multiple notifying alarms, 
	// restart sound playing sequence for another notifying alarm
	if (IdOfNotifyingAlarm() == KNullAlarmId && !InQuietPeriod())
		{
		PlayPreviousNotifyingAlarm();
		}
	}


//
//
//


//*************************************************************************************
/**
 * @see MASSrvSoundSettingsObserver
 */
void CASSrvSoundController::MASSoundSettingsHandleChangeEvent(TSoundSettingsEvent aEvent)
	{
	CASSrvSoundSettings& soundSettings = ServerData().SoundSettings();
	//
	switch(aEvent)
		{
	case ESoundSettingsEventGlobalSoundState:
		if	(soundSettings.GlobalSoundState() == EAlarmGlobalSoundStateOff)
			{
			// Stop playing anything right now
			CancelAllSounds();
			}
		else
			{
			// Resume sound timing from now
			iTimeToReturnToNormalSoundTimingBehaviour = ASSrvStaticUtils::UtcTimeNow();
			
			StartSoundTimingForAnyWaitingAlarmAfterPausingOrQuietPeriod();
			}

		break;

	case ESoundSettingsEventSoundIntervals:
		// Resume sound timing from now
		CancelAllSounds();
		iTimeToReturnToNormalSoundTimingBehaviour = ASSrvStaticUtils::UtcTimeNow();
		
		StartSoundTimingForAnyWaitingAlarmAfterPausingOrQuietPeriod();
		break;

	case ESoundSettingsClearPauseFlag:
		iSoundControllerFlags.Clear(ESoundControllerFlagsPausing);
		break;

	default:
		ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicInvalidTSoundSettingsEvent);
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
void CASSrvSoundController::MAlarmQueueObserverHandleEvent(TASSrvAlarmQueueEvent aEvent, TAlarmId aAlarmId)
	{
	/* If a notifying alarm is deleted stop playing alarm sound.
	 * Looking for either:
	 * 1. The currently Notifying alarm is deleted.
	 * 2. Alarm Queue Internalize starts whilst any alarm is notifying
	 */
	if ((aEvent == EASSrvAlarmQueueEventAlarmDeleted) && (aAlarmId == IdOfNotifyingAlarm()))
		{
		// Cancel the sound and timer, and clear the id of the alarm (we're
		// no longer notifying about anything).
		// Cancel sound & timer
		if (!InQuietPeriod())
			{
			Cancel();
			}
		
		IdOfNotifyingAlarm() = KNullAlarmId;
		
		// If a notifying alarm is deleted, look for the previous notifying alarm.
		// The following function is called even if we are in quiet period 
		// because we want the appropriate alarm to be played when quite period end.
		PlayPreviousNotifyingAlarm();
		}
	else if ((aEvent == EASSrvAlarmQueueEventAlarmStartInternalize) && (KNullAlarmId != IdOfNotifyingAlarm()))
		{
		// Cancel the sound and timer, and clear the id of the alarm (we're
		// no longer notifying about anything).
		// Cancel sound & timer
		if	(!InQuietPeriod())
			{
			Cancel();
			}

		IdOfNotifyingAlarm() = KNullAlarmId;
		}
	}

//*************************************************************************************
/**
 * @see MASSrvEnvironmentChangeObserver
 */

void CASSrvSoundController::MEnvChangeHandleEvent(TInt aChanges, TUint /*aWorkdays*/, TBool /*aWorkdaysChanged*/)
	{
	if (aChanges & EChangesSystemTime || (aChanges & EChangesLocale && (iPreviousUTCOffset != User::UTCOffset())) )
		{
		// UTC offset might have changed we need to redo the CTimer::AtUTC with a new UTC offset computed

		if (!iSoundControllerFlags.IsSet(ESoundControllerFlagsIsFixed)) //  if floating period
			{
			iTimeToReturnToNormalSoundTimingBehaviour -= TTimeIntervalSeconds(User::UTCOffset().Int() - iPreviousUTCOffset.Int());
			}
		else  //if fixed period
			{
			//cancel silent period because we can't recalculate it
			iTimeToReturnToNormalSoundTimingBehaviour = ASSrvStaticUtils::UtcTimeNow();
			}

		if (InQuietPeriod() || SoundIsPausedForAlarm())
			{
			Cancel();

			if(iTimeToReturnToNormalSoundTimingBehaviour <= ASSrvStaticUtils::UtcTimeNow()) 
				{
				ReactToSoundTimerExpiry(); // Silent period expired
				}
			else 
				{
				AtUTC(iTimeToReturnToNormalSoundTimingBehaviour);
				}
			}
		else if (IdOfNotifyingAlarm() != KNullAlarmId)
			{
			// resume sound timing from now because of changed system time
			StartSoundTimingForAnyWaitingAlarmAfterPausingOrQuietPeriod();
			}

		// And update the UTC offset			
		iPreviousUTCOffset = User::UTCOffset();
		}
	}


//
//
//


//*************************************************************************************
/**
 * Stops any sound from playing and resets the sound timer to an idle state.
 */
void CASSrvSoundController::CancelAllSounds()
	{
	// Tell observer to stop
	if( !IsSoundControllerDisabled() )
		{
		NotifySoundEvent(MASSrvSoundControllerObserver::ESoundControllerEventPlayNothing, IdOfNotifyingAlarm());
		}
	}


//*************************************************************************************
/**
 * Stop playing any sounds until the specified time.
 */
void CASSrvSoundController::MakeAllSoundsQuietUntil(const TTime& aTime)
	{
	if( IsSoundControllerDisabled() )
		{
		return;
		}

	Cancel();

	// Update the flags
	iSoundControllerFlags.Set(ESoundControllerFlagsInQuietPeriod);

	// set global sound state
	ServerData().SoundSettings().SetGlobalSoundState(EAlarmGlobalSoundStateOff);
	
	// Wake up when sounds are to start again
	iTimeToReturnToNormalSoundTimingBehaviour = aTime;
	AtUTC(iTimeToReturnToNormalSoundTimingBehaviour);
	
	// Notify change
	ServerData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventSoundSilence, KNullAlarmId);
	}
	
//*************************************************************************************
/**
 * Stop playing the sound (for just this alarm) until the specified time.
 */
void CASSrvSoundController::CurrentAlarmSoundPausedUntil(const TTime& aTime)
	{
	if( IsSoundControllerDisabled() )
		{
		return;
		}

	Cancel();

	// Save the time we're supposed to wake up at and reset
	// the alarm's sound state so that it can start over
	// again when we're due to run.
	TASSrvAlarm* alarm = ServerData().Queue().QueueAlarmById(IdOfNotifyingAlarm());

	// We have a new baseline for all sound timing calculations. The next set of
	// cycle calcs must be based upon the time at which the quiet period ends
	// rather than the original due time.
	alarm->ReinitializeSoundState(aTime);

	// Update the flags
	iSoundControllerFlags.Set(ESoundControllerFlagsPausing);
	
	// Wake up when sounds are to start again
	iTimeToReturnToNormalSoundTimingBehaviour = aTime;
	
	AtUTC(aTime);

	// Notify change
	ServerData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventSoundSilence, KNullAlarmId);
	}


//*************************************************************************************
/**
 * Returns a boolean indicating whether or not the alarm server is currently
 * in a quiet period.
 */
TBool CASSrvSoundController::InQuietPeriod() const
	{
	if( IsSoundControllerDisabled() )
		{
		return ETrue;
		}
	return Flags().IsSet(ESoundControllerFlagsInQuietPeriod);
	}


//*************************************************************************************
/**
 * Return a boolean indicating whether sound is currently
 * paused within the alarm server.
 */
TBool CASSrvSoundController::SoundIsPausedForAlarm() const
	{
	if( IsSoundControllerDisabled() )
		{
		return EFalse;
		}
	return Flags().IsSet(ESoundControllerFlagsPausing);
	}


//*************************************************************************************
/**
 * Cancel a previously enabled silent period
 */
void CASSrvSoundController::CancelSilence()
	{
	if( IsSoundControllerDisabled() )
		{
		return;
		}

	if	(iSoundControllerFlags.IsSet(ESoundControllerFlagsPausing) || iSoundControllerFlags.IsSet(ESoundControllerFlagsInQuietPeriod))
		{
		Cancel();

		// Update the time to use as a baseline for the next bout of sounds
		iTimeToReturnToNormalSoundTimingBehaviour = ASSrvStaticUtils::UtcTimeNow();
		
		// Update the flags
		iSoundControllerFlags.Clear(ESoundControllerFlagsPausing);
		iSoundControllerFlags.Clear(ESoundControllerFlagsInQuietPeriod);

		// set global sound state
		ServerData().SoundSettings().SetGlobalSoundState(EAlarmGlobalSoundStateOn);

		// Notify change
		ServerData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventSoundSilence, KNullAlarmId);
		}
	}


//
//
//


//*************************************************************************************
/**
 * @see CActive
 */
void CASSrvSoundController::RunL()
	{
	const TInt statusValue = iStatus.Int();

	// When the user changes the system time, then the timer's request status
	// is completed with KErrAbort. If we don't gracefully trap this here, then
	// lots of strange thing start to happen (dialogs don't get dismissed in EikSrv etc).
	if	(statusValue == KErrAbort || statusValue == KErrCancel || statusValue == KErrOverflow)
		return;

	// Handle the timer expiry
	ReactToSoundTimerExpiry();
	}


//*************************************************************************************
/**
 * Replacement of CActive::Cancel function
 */
void CASSrvSoundController::Cancel()
	{
	// If we're currently timing an event, Cancel the timer and send
	// a StopSound command. Otherwise, don't pollute the Alert Server
	// with an extra message.
	if( IsActive() )
		{
		CTimer::Cancel();
		CancelAllSounds();
		}
	}


//
//
//


//*************************************************************************************
void CASSrvSoundController::ReactToSoundTimerExpiry()
	{
	__ASSERT_DEBUG(!IsSoundControllerDisabled(), ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicUnexpectedEventSoundControllerDisabled));

	if	(InQuietPeriod())
		{
		// If we're in a quiet period, and the sound timer has expired,
		// then we should now start to play sound for any currently notifying
		// alarm.
		iSoundControllerFlags.Clear(ESoundControllerFlagsInQuietPeriod);
		ServerData().SoundSettings().SetGlobalSoundState(EAlarmGlobalSoundStateOn);
		}
	else if (SoundIsPausedForAlarm())
		{
		iSoundControllerFlags.Clear(ESoundControllerFlagsPausing);
		StartSoundTimingForAnyWaitingAlarmAfterPausingOrQuietPeriod();
		}
	else
		{
		// ensure notifying Alarm hasn't been disabled, etc... between Timer expiry and this function running
		if(IdOfNotifyingAlarm() != KNullAlarmId)
			{
			WorkOutAndScheduleForNextSoundCycle();
			}
		else
			{
			// stop any sound currently playing 
			CancelAllSounds();
			}
		}
	}


//*************************************************************************************
void CASSrvSoundController::NotifySoundEvent(MASSrvSoundControllerObserver::TSoundControllerEvent aEvent, TAlarmId aAlarmId)
	{
	__ASSERT_DEBUG(!IsSoundControllerDisabled(), ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicUnexpectedEventSoundControllerDisabled));

	if	(aEvent == MASSrvSoundControllerObserver::ESoundControllerEventPlaySound && 
		(InQuietPeriod() || SoundIsPausedForAlarm()))
		{
		// If we're in a quiet period, then we should ignore play sound requests...
		return;
		}
	else if (ServerData().SoundSettings().GlobalSoundState() == EAlarmGlobalSoundStateOff)
		{
		// Also ignore sounds requests if all sounds are disabled
		return;
		}

	// In an effort to minimise the polution passed to the Alert Server, let's
	// keep track of what message was last sent, except the SoundStopAll events.
	if ( MASSrvSoundControllerObserver::ESoundControllerEventPlayNothing == aEvent && aAlarmId == KNullAlarmId )
		{
		iObserver.MASHandleSoundControllerEvent(aEvent, aAlarmId);
		}
	else if ( iPreviousSoundEvent != aEvent )
		{
		iPreviousSoundEvent = aEvent;
		iObserver.MASHandleSoundControllerEvent(aEvent, aAlarmId);
		}
	}


//*************************************************************************************
void CASSrvSoundController::StartSoundTimingForAnyWaitingAlarmAfterPausingOrQuietPeriod()
	{
	__ASSERT_DEBUG(!IsSoundControllerDisabled(), ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicUnexpectedEventSoundControllerDisabled));

	const TAlarmId id = IdOfNotifyingAlarm();
	if	(id != KNullAlarmId)
		{
		// Cancel function of this object is overridden and sends 
		// ESoundControllerEventPlayNothing message as well as Cancels the timer.
		// The situation that the timer is pending is very unlikely, so we check 
		// if it is Active to prevent unnecessary message pollution.
		if (IsActive())
			{
			Cancel();
			}

		// Save the time we're supposed to wake up at and reset
		// the alarm's sound state so that it can start over
		// again when we're due to run.
		TASSrvAlarm* alarm = ServerData().Queue().QueueAlarmById(id);

		// Clear the sound paused flag for the notifying alarm
		alarm->ClearSoundPausedFlag();

		// We have a new baseline for all sound timing calculations. The next set of
		// cycle calcs must be based upon the time at which the quiet period ends
		// rather than the original due time.
		alarm->ReinitializeSoundState(iTimeToReturnToNormalSoundTimingBehaviour);

		// Is a fixed value, should not be recomputed if offset changes.
		iSoundControllerFlags.Set(ESoundControllerFlagsIsFixed);

		// Run again straight away - this will start the sound again immediately.
		AtUTC(iTimeToReturnToNormalSoundTimingBehaviour);
		}
	}


//*************************************************************************************
void CASSrvSoundController::WorkOutAndScheduleForNextSoundCycle()
	{
	__ASSERT_DEBUG(!IsSoundControllerDisabled(), ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicUnexpectedEventSoundControllerDisabled));

	__ASSERT_ALWAYS(IdOfNotifyingAlarm() != KNullAlarmId, ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicNotifyingAboutWrongAlarm));

	const TAlarmId id = IdOfNotifyingAlarm();


	// Get the alarm which we are monitoring...
	TASSrvAlarm* alarm = ServerData().Queue().QueueAlarmById(id);
 

	// Tell observer what to do
	MASSrvSoundControllerObserver::TSoundControllerEvent event = MASSrvSoundControllerObserver::ESoundControllerEventPlaySound;
	if	(alarm->SoundState() == TASSrvAlarm::ESoundStatePlayingNothing)
		event = MASSrvSoundControllerObserver::ESoundControllerEventPlayNothing;
	NotifySoundEvent(event, id);

	// Work out when we're next due to play [sound/silence]
	const TTime nextTimePeriod = alarm->CalculateAndPrepareNextSoundCycle();
	
	if (nextTimePeriod == Time::NullTTime())
		{
		// We've reached the end of the Sound Interval sequence, with the 
		// Sound Setting Repeat option set to EAlarmSoundRepeatSettingStop.
		// Reset the Alarm Id and *don't* start the timer.
		IdOfNotifyingAlarm() = KNullAlarmId;
		}
	else
		{
		// Toggle the alarm's sound state so that when we wake up, we do the right thing
		alarm->ToggleSoundState();

		// Next time to react is a fixed offset to UTC
		iSoundControllerFlags.Set(ESoundControllerFlagsIsFixed);

		// Wait for the next time to react
		AtUTC(nextTimePeriod);
		}
	}


/**
Silent sound if the given alarm id is the currently playing alarm.
Handles EASAltAlertServerResponseSilence.
*/
void CASSrvSoundController::CancelSound(TAlarmId aAlarmId)
	{
	// Don't notify anyone if we're disabled.
	if( IsSoundControllerDisabled() )
		{
		return;
		}
	
	if ( aAlarmId == KNullAlarmId )
		{
		NotifySoundEvent(MASSrvSoundControllerObserver::ESoundControllerEventPlayNothing, aAlarmId);
		}
	else if ( aAlarmId == IdOfNotifyingAlarm() )
		{
		NotifySoundEvent(MASSrvSoundControllerObserver::ESoundControllerEventPlayNothing, aAlarmId);
		}
	}

/**
If there are other alarm in the notifying alarm list, start sound play
sequence for the previous notifying alarm. If we are in quite period, this previous
alarm will be played after quite period ends
*/
void CASSrvSoundController::PlayPreviousNotifyingAlarm()
	{
	CASSrvAlarmQueue& queue = ServerData().Queue();
	
	TASSrvAlarm* lastAlarm = NULL;
	RASSrvIteratorByState iterator(queue, EAlarmStateNotifying);
	iterator.Open();
	if (!iterator.NextAlarmAvailable())
		{
		return;
		}

	// look for a the latest alarm
	while (iterator.NextAlarmAvailable())
		{
		lastAlarm = &iterator.NextAlarm();
		}
	
	IdOfNotifyingAlarm() = lastAlarm->Id();
	
	if (!InQuietPeriod())
		{
		// reset the baseline for all sound timing calculations.
		TTime now(ASSrvStaticUtils::UtcTimeNow());			
		lastAlarm->ReinitializeSoundState(now);
		WorkOutAndScheduleForNextSoundCycle();
		}
	}

