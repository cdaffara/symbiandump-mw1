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

#include "ASSrvAlarm.h"

// User includes
#include "ASSrvTimer.h"
#include "ASSrvDataPool.h"
#include "ASSrvAlarmQueue.h"
#include "ASSrvStaticUtils.h"
#include "ASSrvSoundSettings.h"
#include "ASSrvServerWideData.h"
#include "ASSrvAlarmSoundDetails.h"
#include "ASSrvIteratorByState.h"
#include "ASSrvSoundController.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include "ASShdAlarm_internal.h"
#endif


// Definitions
//#define __DEBUGGING_TIMES


//
// ----> TASSrvAlarm (source)
//

//*************************************************************************************
/**
 * Constructor
 */
TASSrvAlarm::TASSrvAlarm(CASSrvServerWideData& aData)
:	iSoundPeriodCycleNumber(KUndefinedCycleIndex), iStartTimeForSoundCalculations(Time::NullTTime()),
	iSoundState(ESoundStatePlayingNothing), iInternalServerFlags(0),
	iOriginatingSessionId(KErrNotFound), iServerWideData(aData)
	{
	}


//
//
//


//*************************************************************************************
/**
 * Copy operator. Only copies certain data members. Possibly this
 * should be removed...
 */
TASSrvAlarm& TASSrvAlarm::operator=(const TASSrvAlarm& aAlarm)
	{
	TASShdAlarm::operator=(aAlarm);
	//
	iSoundPeriodCycleNumber = aAlarm.iSoundPeriodCycleNumber;
	iStartTimeForSoundCalculations = aAlarm.iStartTimeForSoundCalculations;
	iSoundState = aAlarm.iSoundState;
	iInternalServerFlags = aAlarm.iInternalServerFlags;
	iOriginatingSessionId = aAlarm.iOriginatingSessionId;
	iNotificationMessage = aAlarm.iNotificationMessage;
	//
	return *this;
	}


//
//
//


//*************************************************************************************
/**
 * Change the status of this alarm, i.e. enabled, disabled
 *
 * @return  TInt KErrNone on success, or one of the system wide standard error codes.
 *				 KErrLocked is returned if this alarm is a "workday" alarm but no valid
 *				 workdays have been defined.
 */
TInt TASSrvAlarm::SetStatus(TAlarmStatus aStatus)
	{
	return doSetStatus(aStatus, EFalse);
	}


/**
Change the state of this alarm.
*/
void TASSrvAlarm::SetState(TAlarmState aState)
	{
	const TAlarmState oldState = State();
	
	// Only process a change in state.
	if (oldState == aState)
		{
		return;
		}

	// Update current state.
	iState = aState;

	// If a paused alarm has re-notified (after the pause period is up) or has
	// been dismissed then clear the paused flag.
	if (HasSoundPaused() || iState == EAlarmStateNotifying || iState == EAlarmStateNotified)
		{
		ClearSoundPausedFlag();
		}

	// Handle new state.
	switch(aState)
		{
	case EAlarmStateQueued:
	case EAlarmStateSnoozed:
	case EAlarmStateWaitingToNotify:
	case EAlarmStateInPreparation:
		break;

	case EAlarmStateNotified:
		{		
		// If the alarm does not repeat or it is a 24 hour alarm then we do
		// nothing.  Notified alarms are cleaned up by the queue when they are
		// more than 7 days old.
		if (RepeatDefinition() != EAlarmRepeatDefintionRepeatOnce && RepeatDefinition() != EAlarmRepeatDefintionRepeatNext24Hours)
			{
			// Work out when the alarm should next repeat.
			PrepareForNextRepeat();
			}
		}
		break;

	case EAlarmStateNotifying:
		{
		TTime now(ASSrvStaticUtils::UtcTimeNow());
		ReinitializeSoundState(now);
		}
		break;

	default:
		{
		__ASSERT_DEBUG(EFalse, ASSrvStaticUtils::Fault(ASSrvStaticUtils::EASSrvFaultAlarmStateNotHandled));
		}
		break;
		}

	// Notify queue so that it can work out if it needs to do some reordering.
	// Only do this if this alarm has been queued (which we can infer if this
	// alarm has a non-null identifier).
	if (Id() != KNullAlarmId)
		{
		ServerData().Queue().HandleAlarmStateChanged(Id(), oldState);
		}

	// We call ourselves recursively if we are a repeating alarm.  We must do
	// this *after* the above HandleAlarmStateChanged() call because there must
	// be two state change notifictions otherwise observers will not behave
	// correctly...
	switch (RepeatDefinition())
		{
	case EAlarmRepeatDefintionRepeatOnce:
	case EAlarmRepeatDefintionRepeatNext24Hours:
		break;

	case EAlarmRepeatDefintionRepeatDaily:
	case EAlarmRepeatDefintionRepeatWorkday:
	case EAlarmRepeatDefintionRepeatWeekly:
#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
	case EAlarmRepeatDefinitionRepeatDailyOnGivenDays:
#endif
		if (oldState == EAlarmStateNotifying)
			{
			// Make sure we keep the snoozed state and not change to queued as
			// in the non-repeating alarms.
			if (aState == EAlarmStateSnoozed)
				{
				if (Id() != KNullAlarmId)
					{
					ServerData().Queue().HandleAlarmStateChanged(Id(), aState);						
					}
				}
			else
				{
				SetState(EAlarmStateQueued);					
				}
			}
		break;

	default:
		break;
		}
	}

//*************************************************************************************
/**
 * Change the characteristics of this alarm
 */
void TASSrvAlarm::SetCharacteristicsL(TAlarmCharacteristicsFlags aFlags, TASSrvSessionId aSessionChangingFlags)
	{
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	// if it is a wakeup alarm and they are trying to set session specific then leave
	if (aFlags.IsSet(EAlarmCharacteristicsSessionSpecific) && IsWakeup())
		{
		User::Leave(KErrArgument);		
		}
#endif
	
	const TAlarmCharacteristicsFlags oldCharacteristics = Characteristics();
	if	(oldCharacteristics.Value() == aFlags.Value())
		return;

	// If it was session-specific, but it isn't anymore, then we need
	// to complete the pending notification with KErrCancel
	if	(oldCharacteristics.IsSet(EAlarmCharacteristicsSessionSpecific) &&
		 !aFlags.IsSet(EAlarmCharacteristicsSessionSpecific))
		{
		// If there was a notification request then we complete it
		NotificationMessageComplete(KErrCancel);

		// Indicate that this alarm is no longer owned by a session
		iFlags.Set(EASShdAlarmFlagsHasBecomeOrphaned);
		}
	else if (!oldCharacteristics.IsSet(EAlarmCharacteristicsSessionSpecific) &&
		aFlags.IsSet(EAlarmCharacteristicsSessionSpecific))
		{
		// The session indicated wishes to take ownership of this alarm (but
		// can't request a expiry notification because these can only be setup
		// when an alarm is first created).
		SetOriginatingSessionId(aSessionChangingFlags);

		// Indicate that this alarm is now owned by a session
		iFlags.Clear(EASShdAlarmFlagsHasBecomeOrphaned);
		}

	// Update state now
	iCharacteristics = aFlags;

	// Notify queue so that it can work out if it needs to do
	// some re-ordering (only do this if we've been queued, and therefore
	// have an alarm id).
	if	(Id() != KNullAlarmId)
		ServerData().Queue().HandleAlarmCharacteristicsChanged(Id(), oldCharacteristics);
	}

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
void TASSrvAlarm::SetWakeupAndNotifyQueueL(TBool aEnable)
	{
	if (IsWakeup() == aEnable)
		{
		return;
		}

	if (aEnable && iCharacteristics.IsSet(EAlarmCharacteristicsSessionSpecific))
		{
		// you cannot set a session alarm as wakeup
		User::Leave(KErrArgument);		
		}
	
	SetWakeup(aEnable);
	if (Id() != KNullAlarmId)
		{
		ServerData().Queue().HandleWakeupChanged(Id());
		}
	}
#endif

#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
void TASSrvAlarm::SetAlarmDaysL(TUint8 aAlarmDays)
	{
	// Check if any modification to current alarms days is required.
	if (aAlarmDays == AlarmDays())
		{
		return;
		}

	User::LeaveIfError(SetAlarmDays(aAlarmDays));
	User::LeaveIfError(ASSrvStaticUtils::ValidateAlarm(*this));
	
	// Notify queue so that it can work out if it needs to do some reordering.
	ServerData().Queue().HandleAlarmDaysChanged(Id());
	}
#endif

void TASSrvAlarm::SetAlarmOrphaned()
	{
	iFlags.Set(EASShdAlarmFlagsHasBecomeOrphaned);
	}
	
//*************************************************************************************
/**
 * Set the originating session id for this alarm. The originating session Id is the
 * session Id (a unique id allocated to each session) which is used to track
 * all "session alarms" within the server.
 */
void TASSrvAlarm::SetOriginatingSessionId(TASSrvSessionId aId)
	{
	iOriginatingSessionId = aId;
	if	(aId >= 0)
		{
		iFlags.Set(EASShdAlarmFlagsHasOwningSession);
		}
	else
		{
		iFlags.Clear(EASShdAlarmFlagsHasOwningSession);
		}
	}


//
//
//


/**
Return the originating session id.

@return The id of the session that created the TASSrvAlarm.
*/
TASSrvSessionId TASSrvAlarm::OriginatingSessionId() const
	{
	return iOriginatingSessionId;
	}


/**
Return the alarm's sound state.

@return The state of the alarm's sound.
*/
TASSrvAlarm::TSoundState TASSrvAlarm::SoundState() const
	{
	return iSoundState;
	}


//
//
//


//*************************************************************************************
/**
 * Issue a request for notifications when this alarm expires
 */
void TASSrvAlarm::RequestExpiryNotificationL(const RMessage2& aMessage)
	{
	// Queue for notifications. This allows us to observe when the alarm expires
	// and notify the client
	ServerData().Timer().NotifyAlarmExpiredL(*this);

	// Save a pointer
	iNotificationMessage = aMessage;

	// Update flags to indicate we have a pending notification message pointer
	iInternalServerFlags.Set(EInternalServerFlagsNotifyPending);
	}


//*************************************************************************************
/**
 * Cancel a previous expiry notification request
 */
void TASSrvAlarm::RequestExpiryNotificationComplete(TInt aErrorCode)
 	{
	NotificationMessageComplete(aErrorCode);	
 	}

/**
Return whether or not the alarm has a notification request pending.

@return A TBool that is ETrue if the alarm has a notification request pending.
*/
TBool TASSrvAlarm::HasNotificationRequestPending() const
	{
	return iInternalServerFlags.IsSet(EInternalServerFlagsNotifyPending) && HasOwningSession();
	}


//*************************************************************************************
/**
 * Map the specified time object (data member) to the nearest minute (rounding down)
 */
void TASSrvAlarm::RoundDownTimeToMinute(TTimeType aType)
	{
	switch(aType)
		{
	case ETimeTypeNextDue:
		ASSrvStaticUtils::RoundTimeDownToTheMinute(iNextDueTime);
		break;
	case ETimeTypeOriginalExpiry:
		ASSrvStaticUtils::RoundTimeDownToTheMinute(iOriginalExpiryTime);
		break;
		}
	}


//*************************************************************************************
/**
 * Destroy this alarm. Removes it from the queue and marks it as inactive.
 */
void TASSrvAlarm::DeQueue()
	{
	ServerData().Queue().DeQueueAlarm(*this);
	}


//*************************************************************************************
/**
 * Reset this alarm back to a completely uninitialized state.
 */
void TASSrvAlarm::Reset()
	{
	TASShdAlarm::Reset();
	//
	iInternalServerFlags = 0;
	iOriginatingSessionId = 0;
	iSoundPeriodCycleNumber = 0;
	}


/**
Clears the alarm's flags.
*/
void TASSrvAlarm::ClearFlags()
	{
	iFlags = 0;
	}


/**
Set the alarm's sound state.

@param aSoundState The new TSoundState of this alarm.
*/
void TASSrvAlarm::SetSoundState(TSoundState aSoundState)
	{
	iSoundState = aSoundState;
	}


//*************************************************************************************
/**
 * Toggle the sound state of this alarm
 */
void TASSrvAlarm::ToggleSoundState()
	{
	// Invert the existing sound state
	if	(iSoundState == ESoundStatePlayingNothing)
		iSoundState = ESoundStatePlayingSound;
	else
		iSoundState = ESoundStatePlayingNothing;
	}


//*************************************************************************************
/**
 * If sound is temporarily silenced, then this method is used to re-initialize
 * the alarm's starting time for any future sound calculations.
 */
void TASSrvAlarm::ReinitializeSoundState(const TTime& aBaselineForSoundTiming)
	{
	// Reset the cycle and sound state ready to start playing
	ASSrvStaticUtils::TodayAtTheSpecifiedTime(aBaselineForSoundTiming, iStartTimeForSoundCalculations);
	SetSoundState(TASSrvAlarm::ESoundStatePlayingNothing);
	SetSoundTimingCycleIndex(0);
	}


//*************************************************************************************
/**
 * Work out when the alarm sound should next start or stop playing.
 */
TTime TASSrvAlarm::CalculateAndPrepareNextSoundCycle()
	{
	// If the new state is not to play any sound, and we've already progressed
	// through all the Sound Intervals, the SoundTimingCycleIndex will have
	// been set to KErrNotFound (see below)
	if ((SoundState() == ESoundStatePlayingNothing) &&
		(SoundTimingCycleIndex() == KErrNotFound)
#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
		|| Continuous()
#endif
		)
		{
		SetSoundTimingCycleIndex(0); // just to be safe, we'll reset this.
		return Time::NullTTime();
		}

	const CASSrvSoundSettings& soundSettings = ServerData().SoundSettings();

	// If the new state is not to play any sound, then we should work out when 
	// we should start playing sound again. If we're now about to start playing sound
	// we should work out when we're going to stop playing it again.
	CASSrvSoundSettings::TSoundCyclePosition position = CASSrvSoundSettings::ESoundCyclePositionAtStart;
	if (SoundState() == ESoundStatePlayingSound)
		{
		position = CASSrvSoundSettings::ESoundCyclePositionAtEnd;
		}

	//
#ifdef __DEBUGGING_TIMES
	TDateTime startTimeForSoundCalculations = iStartTimeForSoundCalculations.DateTime();
#endif

	ASSrvStaticUtils::TodayAtTheSpecifiedTime(iStartTimeForSoundCalculations, iStartTimeForSoundCalculations);
#ifdef __DEBUGGING_TIMES
	startTimeForSoundCalculations = iStartTimeForSoundCalculations.DateTime();
#endif

	TTime returnTime(soundSettings.SoundIntervalTime(iStartTimeForSoundCalculations, SoundTimingCycleIndex(), position));
#ifdef __DEBUGGING_TIMES
	TDateTime returnTimeDT = returnTime.DateTime();
#endif

	// Increment position at the end of the cycle
	if (SoundState() == ESoundStatePlayingSound)
		{
		// There will always be at least one sound play interval, so we can 
		// assume that here. If there hadn't been any intervals, the CASSrvSoundController
		// would be considered disabled, and wouldn't have invoked the current method.
		TInt tempSS = SoundTimingCycleIndex();
		if (tempSS >= (soundSettings.SoundIntervalCount()-1)) // Index E (0..SoundIntervalCount()-1)
			{
			switch ( soundSettings.RepeatSetting() )
				{
			case EAlarmSoundRepeatSettingLoop:
				// Go back to the first cycle, resetting the start time
				// to avoid setting an alarm time in the past.
				tempSS = 0;
				iStartTimeForSoundCalculations = returnTime;
				break;
			case EAlarmSoundRepeatSettingRepeatLast:
				{
				// Repeat the last interval; Don't change the Sound Interval
				// Index (currently tempSS). The last offset will be added to
				// iStartTimeForSoundCalculations in the call to SoundIntervalTime,
				// so we need to update it so that the timing is correct.

				// Determine the duration of the last interval:
				TTime lastStartTime(soundSettings.SoundIntervalTime(iStartTimeForSoundCalculations,
																	soundSettings.SoundIntervalCount()-1,
																	CASSrvSoundSettings::ESoundCyclePositionAtStart ));
				TTime nextToLastStartTime(soundSettings.SoundIntervalTime(iStartTimeForSoundCalculations,
																	soundSettings.SoundIntervalCount()-2,
																	CASSrvSoundSettings::ESoundCyclePositionAtStart ));
				TTimeIntervalMinutes diff;
				lastStartTime.MinutesFrom(nextToLastStartTime, diff);
				iStartTimeForSoundCalculations += diff;
				}
				break;
			case EAlarmSoundRepeatSettingStop:
				// This will cause TTime::NullTTime() to be returned to the
				// Sound Controller when the next time interval expires.
				tempSS = KErrNotFound;
				break;
				}
			}
		else
			{
			tempSS++;
			}
		SetSoundTimingCycleIndex(tempSS);
		}
	return returnTime;
	}


//*************************************************************************************
/**
 * Perform some sanity checking after internalizing an alarm or receiving
 * an alarm via the client API. Checks that the alarm isn't too old to 
 * still be considered valid. If the alarm is okay, its status is changed
 * to enabled. Note that this method checks the workdays, and hence it
 * might also disable an otherwise "valid" alarm.
 * 
 * @return  KErrNone if the alarm is valid (and therefore may be queued),
 *			KErrArgument if this alarm isn't valid, KErrLocked if a "Workdays" alarm
 *			has no valid workdays to test itself against (but is otherwise valid).
 */
TInt TASSrvAlarm::ValidateAndEnable(TTimeIntervalSeconds aAllowableWindow, TBool aAllowAnyOnceAlarmInThePast, TBool aEnable)
	{
	TInt alarmErrorCode = KErrArgument;
	//
	if (OriginalExpiryTime() == Time::NullTTime())
		{
		OriginalExpiryTime() = NextDueTime();
		}
	// Was the alarm notifying or snoozed when the server closed?
	// If so, we don't want to adjust the next due time.
	if (State() == EAlarmStateNotifying)
		{
		// Set the state back to EAlarmStateQueued so the alarm will notify again.
		iState = EAlarmStateQueued;
		alarmErrorCode = KErrNone;
		}
	else if (State() == EAlarmStateSnoozed)
		{
		alarmErrorCode = KErrNone;
		}
	else
		{
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
		if (!(IsWakeup() && iCharacteristics.IsSet(EAlarmCharacteristicsSessionSpecific)))
			{
#endif
			TTime oldestValidTimeForAlarms(ASSrvStaticUtils::UtcTimeNow());
			if	(NextDueTime() != Time::NullTTime())
				{
				if	(RepeatDefinition() == EAlarmRepeatDefintionRepeatOnce)
					{
					// Does the alarm fall within the window?
					oldestValidTimeForAlarms -= aAllowableWindow;
			
					// 1. Ignore alarms in the past (taking the window into account)
					// 2. If the "allow anything" flag is set (parameter) then we always
					//    allow the alarm.
					// 3. If the alarm is in the past, but it's state is "Notified" then we 
					//    allow it anyway.
					const TBool insideWindow = NextDueTime() >= oldestValidTimeForAlarms;
					if	(insideWindow || aAllowableWindow.Int() < 0 || aAllowAnyOnceAlarmInThePast || State() == EAlarmStateNotified)
						{
						// This alarm is okay
						alarmErrorCode = KErrNone;
						}
					}
				else
					{
					// The alarm must be one of the repeating types.
					__ASSERT_ALWAYS(RepeatDefinition() != EAlarmRepeatDefintionRepeatOnce, ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicInvalidAlarmRepeat));
	
					// If it's a repeat in the next 24 hours then we need to
					// work out the next time. Otherwise, we can use the standard
					// PrepareForNextRepeat method.
					if	(RepeatDefinition() == EAlarmRepeatDefintionRepeatNext24Hours)
						{
						ASSrvStaticUtils::CalculateNext24HoursRepeat(*this, aAllowableWindow);
						alarmErrorCode = KErrNone;
						}
					else
					    {
					    alarmErrorCode = PrepareForNextRepeat(aAllowableWindow);
					    }
					}
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
				}
#endif
			}
		}

	// If the alarm is valid then we enable it. Note that this will leave (otherwise) valid
	// "Workdays" alarms disabled (e.g. if there aren't any workdays defined by the user, then
	// the alarm is valid, but we can't enable it because we don't know when it will next be
	// due).
	if	(alarmErrorCode == KErrNone && aEnable)
		{
		// Enable the alarm if its not rejected
		const TInt error = doSetStatus(EAlarmStatusEnabled, EFalse);
		__ASSERT_ALWAYS(error == KErrNone, ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicCannotSetAlarmStatus)); // Something has really gone wrong...
		}

	return alarmErrorCode;
	}


//*************************************************************************************
/**
 * Snooze this alarm until the specified time
 */
void TASSrvAlarm::Snooze(const TTime& aTimeToAwaken)
	{
	TTime tempTTime = aTimeToAwaken;
	ASSrvStaticUtils::RoundTimeDownToTheMinute(tempTTime);
	NextDueTime() = tempTTime;

	// Changing the state to snoozed will notify the alarm queue of a state change.
	// At this point, the queue is resorted to ensure that the snoozed alarm is
	// present in the queue at the right point (since a snoozed alarm may end up
	// moving to the head of the queue).
	SetState(EAlarmStateSnoozed);
	}


/**
Returns ETrue if the alarm sound playing has been paused.

@return ETrue if the alarm sound playing has been paused, EFalse otherwise.
*/
TBool TASSrvAlarm::HasSoundPaused() const
	{
	return iInternalServerFlags.IsSet(EASShdAlarmFlagsSoundHasBeenPaused);
	}


/**
Sets an internal flag.

@internalComponent
*/
void TASSrvAlarm::SetSoundPausedFlag()
	{
	iInternalServerFlags.Set(EASShdAlarmFlagsSoundHasBeenPaused);
	}


/**
Clears an internal flag.

@internalComponent
*/
void TASSrvAlarm::ClearSoundPausedFlag()
	{
	iInternalServerFlags.Clear(EASShdAlarmFlagsSoundHasBeenPaused);
	}


//
//
//


//*************************************************************************************
/**
 * Set the data for this alarm. Leaves with KErrInUse if this alarm
 * already has associated data.
 */
void TASSrvAlarm::DataAttachL(HBufC8* aData)
	{
	CASSrvDataPool& dataPool = ServerData().DataPool();
	dataPool.DataPoolAddDataL(Id(), aData);

	// Set the base-class flags
	iFlags.Set(EASShdAlarmFlagsHasAssociatedData);

	// make sure all data gets saved
	ServerData().Queue().HandleAlarmDataChanged(Id());
	}


//*************************************************************************************
/**
 * Remove the data from the pool. Leaves if the alarm doesn't have
 * any associated data.
 */
void TASSrvAlarm::DataDetachL()
	{
	CASSrvDataPool& dataPool = ServerData().DataPool();
	dataPool.DataPoolRemoveDataL(Id());

	// Clear the base-class flags
	iFlags.Clear(EASShdAlarmFlagsHasAssociatedData);
	}


//*************************************************************************************
/**
 * Returns the size in bytes of the data associated with this alarm.
 * Leaves with KErrNotFound if there is no data for this alarm.
 */
TInt TASSrvAlarm::DataSizeL() const
	{
	TInt size = 0;
	CASSrvDataPool& dataPool = ServerData().DataPool();
	//
	if	(HasAssociatedData() && dataPool.DataPoolContainsEntry(Id()))
		size = dataPool.DataPoolEntry(Id()).Size();
	else
		User::Leave(KErrNotFound);
	//
	return size;
	}


//*************************************************************************************
/**
 * Access the data associated with this alarm. Will leave with 
 * KErrNotFound if this alarm doesn't have any data.
 */
const TDesC8& TASSrvAlarm::DataL() const
	{
	CASSrvDataPool& dataPool = ServerData().DataPool();
	//
	if	(!HasAssociatedData() || !dataPool.DataPoolContainsEntry(Id()))
		User::Leave(KErrNotFound);
	//
	const TDesC8& data = dataPool.DataPoolEntry(Id());
	return data;
	}


//*************************************************************************************
/**
 * Same as DataL() except this panics if the alarm doesn't have any
 * data.
 */
const TDesC8& TASSrvAlarm::Data() const
	{
	return ServerData().DataPool().DataPoolEntry(Id());
	}


//
//
//


//*************************************************************************************
/**
 * When attempting to action this alarm, a timer error occurred. Depending
 * on the alarm type, a different action is performed.
 */
void TASSrvAlarm::HandleTimerError(TInt aErrorCode)
	{
	__ASSERT_ALWAYS(aErrorCode != KErrNone, ASSrvStaticUtils::Fault(ASSrvStaticUtils::EASSrvFaultPhantomErrorReported));

	// Need to notify session if there was a pending notification request
	NotificationMessageComplete(aErrorCode);

	// Destroy the alarm
	DeQueue();
	}


//*************************************************************************************
/**
 * Called when an alarm is removed from the queue. Notifies any observers
 * that this alarm has died.
 */
void TASSrvAlarm::HandleDeQueue()
	{
	NotificationMessageComplete(KErrDied);
	}


/**
Called when the date/time or work days changes.
*/
TBool TASSrvAlarm::HandleDateTimeChangedL(TUint aWorkdays, TBool aWorkdaysChanged)
	{
	// If this alarm has an outstanding expiry notification request it will be
	// completed with KErrCancel.
	NotificationMessageComplete(KErrCancel);
	
	TBool agendaAlarmInPast = EFalse;
	TTime alarmDue;

	// If the work days have changed (e.g. by a change in locale) and this alarm
	// is a "work day" alarm then the alarm state is updated based upon the new
	// work days.
	if (aWorkdaysChanged && RepeatDefinition() == EAlarmRepeatDefintionRepeatWorkday)
		{
		if (!aWorkdays)
			{
			User::LeaveIfError(doSetStatus(EAlarmStatusDisabled, ETrue));
			return agendaAlarmInPast;
			}
		// Enable the alarm now we have some work days.
		const TInt error = doSetStatus(EAlarmStatusEnabled, ETrue);
		__ASSERT_ALWAYS(error == KErrNone, ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicCannotSetAlarmStatus));
		PrepareForNextRepeat();
		return agendaAlarmInPast;
		}
	
	TTime timeNow = ASSrvStaticUtils::UtcTimeNow();
	// tempTime is used for rounding purposes.  In most cases it is equal to
	// NextDueTime().	
	TTime tempTime(NextDueTime());
	if (timeNow.DaysFrom(NextDueTime()).Int() == 0)
		{
		TTimeIntervalMinutes MinuteInterval;
		timeNow.MinutesFrom(NextDueTime(), MinuteInterval);
		tempTime = NextDueTime();
		tempTime -= MinuteInterval; 
		}

	const TInt days(timeNow.DaysFrom(tempTime).Int() > 0 ?
		timeNow.DaysFrom(tempTime).Int() :
		timeNow.DaysFrom(NextDueTime()).Int());

	// If the time has changed to the past we do not re-expire if the time
	// crossed a previous boundary for this alarm.  If the time has changed
	// to the future and crossed a repeat boundary then we expire
	// immediately.
	
	switch(RepeatDefinition())
		{
	case EAlarmRepeatDefintionRepeatNext24Hours:
		// Is next due time more than 1 day in the future?
		if (days <= -1)
			{
			// Alarm changes to "repeat once" and at a fixed time.
			TAlarmCharacteristicsFlags tempAFlags = Characteristics();
			tempAFlags.Clear(EAlarmRepeatDefintionRepeatNext24Hours);
			tempAFlags.Set(EAlarmRepeatDefintionRepeatOnce);
			SetCharacteristicsL(tempAFlags, OriginatingSessionId());
			RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
			}
		// Fall through intentionally so that the following case is applied if
		// the alarm repeat definition has been changed above from "next 24
		// hours" to "repeat once". 

	case EAlarmRepeatDefintionRepeatOnce:
		timeNow.DateTime().SetSecond(0);
		alarmDue = NextDueTime();
		if (!iCharacteristics.IsSet(EAlarmCharacteristicsIsFixed))
	        {
			// We need to compare alarm due time and current time in local.
			// Since it is a floating alarm the offset for the alarm due time is
	        // taken from previous offset stored with server. 
		    alarmDue += ServerData().CachedUtcOffset();
		    timeNow += User::UTCOffset();	        
	        }
		if (days >= 1 || (iCharacteristics.IsSet(EAlarmCharacteristicsDeQueueIfDueTimeInPast) && alarmDue < timeNow))
			{
			// The new time is at least one day after the original expiry time.
			// Alternatively it is an agenda alarm which does not want to be
			// notified if it is in the past.  We delete the alarm silently.
			if (iCharacteristics.IsSet(EAlarmCharacteristicsDeQueueIfDueTimeInPast) && State() != EAlarmStateNotified)
				{
				agendaAlarmInPast = ETrue;
				}
			DeQueue();
			}
		break;

	case EAlarmRepeatDefintionRepeatDaily:
	case EAlarmRepeatDefintionRepeatWorkday:
	case EAlarmRepeatDefintionRepeatWeekly:
#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
	case EAlarmRepeatDefinitionRepeatDailyOnGivenDays:
#endif
		// When days is 0 the alarm would be within 24 hours either in the
		// future or in the past in which case there is no need to recalculate
		// the next due time.
		if (days != 0)
			{
			PrepareForNextRepeat();
			}
		break;

	default:
		__ASSERT_DEBUG(EFalse, ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicInvalidAlarmRepeat));
		break;
		}
		
	return agendaAlarmInPast;
	}


//
//
//


//*************************************************************************************
/**
 * @see MASSrvAlarmTimerObserver
 */
void TASSrvAlarm::MATimerHandleAlarmExpired(TAlarmTimerEvent aEvent, TAlarmId aAlarmId)
	{
	// We're only interested in alarm expiry events relating to this alarm.
	if	(aAlarmId != Id())
		return;

	if	(iInternalServerFlags.IsSet(EInternalServerFlagsNotifyPending))
		{
		// Map the event onto an error value
		TInt completionCode = KErrNone;
		switch(aEvent)
			{
		case EAlarmTimerEventAlarmExpired:
			completionCode = KErrNone;
			break;
		case EAlarmTimerEventTimeOrDateChanged:
			completionCode = KErrAbort;
			break;
		case EAlarmTimerEventTimingError:
			completionCode = KErrGeneral;
			break;
		default:
			__ASSERT_DEBUG(EFalse, ASSrvStaticUtils::Fault(ASSrvStaticUtils::EASSrvFaultTimerEventNotHandled));
			break;
			}

		// Now complete the message
		NotificationMessageComplete(completionCode);
		}
	}

/**
Return the server data object.

@return The server data object created by the server that owns all the data.
*/
CASSrvServerWideData& TASSrvAlarm::ServerData() const
	{
	return iServerWideData;
	}


//
//
//


//*************************************************************************************
/**
 * Complete an outstanding notification request
 */
void TASSrvAlarm::NotificationMessageComplete(TInt aCompletionCode)
	{
	if	(iInternalServerFlags.IsSet(EInternalServerFlagsNotifyPending))
		{
		// Complete the outstanding request
		iNotificationMessage.Complete(aCompletionCode);

		// Update flags to indicate we no longer have a pending 
		// notification message pointer
		iInternalServerFlags.Clear(EInternalServerFlagsNotifyPending);

		// Reset session identified
		SetOriginatingSessionId(KErrNotFound);

		// Done with this now
		ServerData().Timer().NotifyAlarmExpiredCancel(*this);
		}
	}


/**
Calculate when this alarm is next due to expire.

@param aAllowableWindow
	This is a delta to apply to the curren time when calculating the next expiry
	time.  The delta is usually 0 seconds, however, when alarms are internalized
	from the backup store, we apply a 59 second delta which allows alarms which
	are less than one minute old to be treated as "not yet expired".  This
	allows devices with unpredictable start up times to show alarm expiry
	dialogs rather than alams silently going missing.

@return
	KErrNone if the alarm state is consistent, KErrArgument if this method is
	being called for the wrong type of alarm or KErrLocked if a "work day"
	alarm has no valid work days to test itself against.
*/
TInt TASSrvAlarm::PrepareForNextRepeat(TTimeIntervalSeconds aAllowableWindow)
	{
	// This should never be called for "repeat once" or "repeat next 24 hour"
	// alarms.
	__ASSERT_DEBUG
		(
		RepeatDefinition() != EAlarmRepeatDefintionRepeatOnce &&
		RepeatDefinition() != EAlarmRepeatDefintionRepeatNext24Hours,
		ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicInvalidAlarmRepeat)
		);
	if (RepeatDefinition() == EAlarmRepeatDefintionRepeatOnce ||
		 RepeatDefinition() == EAlarmRepeatDefintionRepeatNext24Hours
		)
		{
		return KErrArgument;
		}

	// This alarm has just been acknowledged. If it repeats we need to
	// reschedule it.
	TTime timeNow(ASSrvStaticUtils::UtcTimeNow());

	// CTimer::At() can complete >1 second early.	
	timeNow += TTimeIntervalSeconds(KAlarmServerCTimerFudgeTimeInSeconds);

	TTimeIntervalDays daysToAddFromNow = 0;
	TTimeIntervalDays rollOverDaysToAddFromNow = 7;

	// Update the nextRepeat object to contain the right hour and minutes
	// component from the last repeat time.
	TTime nextRepeat;
	
	TDateTime nextRepeatDateTime = timeNow.DateTime();
	const TDateTime oldTime = OriginalExpiryTime().DateTime();
	nextRepeatDateTime.SetHour(oldTime.Hour());
	nextRepeatDateTime.SetMinute(oldTime.Minute());
	nextRepeat = nextRepeatDateTime;
	
	// Use local time to find the current day number in the week. 
	TTime nextRepeatLocal = nextRepeat + User::UTCOffset();

	// Remove seconds and microseconds part.
	ASSrvStaticUtils::RoundTimeDownToTheMinute(nextRepeat);
	
	switch (RepeatDefinition())
		{
	case EAlarmRepeatDefintionRepeatDaily:
		{
		// The time is fixed, but the day changes.
		rollOverDaysToAddFromNow = 1;
		}
		break;

	case EAlarmRepeatDefintionRepeatWeekly:
		{
		// Same day, next week. 
		daysToAddFromNow = OriginalExpiryTime().DayNoInWeek() - timeNow.DayNoInWeek();
		}
		break;
	
	case EAlarmRepeatDefintionRepeatWorkday:
		{
		// Same time at next work day.
		const TUint KWorkDays = TLocale().WorkDays();
		if (!KWorkDays)
			{
			// If there are no work days defined then we disable the alarm.
			// When a change in work days is detected (caused by a change in
			// locale) the HandleDateTimeChanged() method will be called and the
			// alarm will be enabled again.
#ifdef _DEBUG
			const TInt ret = 
#endif
			doSetStatus(EAlarmStatusDisabled, ETrue);
			__ASSERT_DEBUG(ret == KErrNone, ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicCannotSetAlarmStatus));
			return KErrLocked;
			}
        ASSrvStaticUtils::DaysUntilNextActiveAlarmDay(daysToAddFromNow,
        	rollOverDaysToAddFromNow, nextRepeatLocal.DayNoInWeek(), KWorkDays);
		}
		break;
	
#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
	case EAlarmRepeatDefinitionRepeatDailyOnGivenDays:
		{
		// Same time at next alarm day.
        ASSrvStaticUtils::DaysUntilNextActiveAlarmDay(daysToAddFromNow,
        	rollOverDaysToAddFromNow, nextRepeatLocal.DayNoInWeek(),
        	AlarmDays());
		}
		break;
#endif

	default:
		{
		__ASSERT_DEBUG(EFalse, ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicInvalidAlarmRepeat));
		}
		break;
		}

	// Add the number of days on so that we repeat on the right day in the
	// future.
	nextRepeat += daysToAddFromNow;

    // If the NextDueTime() is after the nextRepeat time that has been
	// calculated so far then the alarm has already notified and been
	// rescheduled for its next repeat.  We don't enable the allowable window in
	// this case because it is possible that the alarm could be rescheduled for
	// the last repeat (that it has already notified about) if it is within the
	// allowable window.
	if (NextDueTime() <= nextRepeat)
		{
		timeNow -= aAllowableWindow;
		}

	// If the calculated time is still before the current time then add the
	// roll-over period too.  This mirrors the EALWL repeat code.  We also allow
	// a window (specified as a parameter to this function in seconds) so that
	// during startup we don't expire alarms which are less than a minute old.
	if (nextRepeat < timeNow)
	    {
	    nextRepeat += rollOverDaysToAddFromNow;
        // If calculated time after adding one rollOverDay is still before the
	    // current time then move it one day forward.
        if (RepeatDefinition() == EAlarmRepeatDefintionRepeatWorkday && 
        	nextRepeat - TTimeIntervalDays(rollOverDaysToAddFromNow.Int() - 1) < timeNow)
            {
            nextRepeat += TTimeIntervalDays(1);
            }
	    }

	// Update next due time and original expiry time.
	NextDueTime() = nextRepeat;
	OriginalExpiryTime() = nextRepeat;

	return KErrNone;
	}


/**
Sets the alarm's sound timing cycle index number.

@param aSoundNumber The alarm's sound timing cycle index number.
*/
void TASSrvAlarm::SetSoundTimingCycleIndex(TASSrvAlarmSoundCycleNumber aSoundNumber)
	{
	iSoundPeriodCycleNumber = aSoundNumber;
	}


/**
Gets the alarm's sound timing cycle index number.

@return The alarm's sound timing cycle index number.
*/
TASSrvAlarmSoundCycleNumber TASSrvAlarm::SoundTimingCycleIndex() const
	{
	return iSoundPeriodCycleNumber;
	}


/**
 * Internalize of the Alarm Queue after a Restore (from a backup)
 * deletes the old queue.  This method sends a Cancel notification for
 * a 'Session Alarm' to its the TRequestStatus object.
 */
void TASSrvAlarm::CancelSessionAlarm()
	{
	// If there was a notification request then we complete it
	if(iCharacteristics.IsSet(EAlarmCharacteristicsSessionSpecific))
		NotificationMessageComplete(KErrCancel);
	}
	
/**
Set alarm status and update disable flag.

@param aStatus
	Alarm status to set.
	
@param aAutoDisabled
	If it's called by locale change handler.
*/
TInt TASSrvAlarm::doSetStatus(TAlarmStatus aStatus, TBool aAutoDisabled)
	{
	const TAlarmStatus oldStatus = Status();
	
	if (oldStatus == aStatus)
		{
		if (aStatus == EAlarmStatusDisabled && !aAutoDisabled) 
			{
			//Change from auto-disabled to manual-disabled
			iFlags.Set(EASShdAlarmFlagsPermanentDisabled);	
			}
		return KErrNone;	
		}
	
	switch (aStatus)
		{
	case EAlarmStatusEnabled:
		if (RepeatDefinition() == EAlarmRepeatDefintionRepeatWorkday)
			{
			if (aAutoDisabled && iFlags.IsSet(EASShdAlarmFlagsPermanentDisabled))
				{
				// Called by locale change handler and we will not enable the
				// alarm as it has been disabled manually.
				return KErrNone;
				}
				
			// Calculate its next valid expiry date.  Will return KErrLocked if
			// there are no work days defined.
			const TInt error = PrepareForNextRepeat();
			if (error != KErrNone)
				{
				return error;
				}
			}
		iFlags.Clear(EASShdAlarmFlagsPermanentDisabled);	
		break;

	case EAlarmStatusDisabled:
		if(aAutoDisabled)
			{
			iFlags.Clear(EASShdAlarmFlagsPermanentDisabled);	
			}
		else
			{
			iFlags.Set(EASShdAlarmFlagsPermanentDisabled);	
			}	
		break;

	default:
		__ASSERT_DEBUG(EFalse, ASSrvStaticUtils::Fault(ASSrvStaticUtils::EASSrvFaultAlarmStatusNotHandled));
		break;
		}

	// Update status now.
	iStatus = aStatus;

	// Notify queue so that it can work out if it needs to do
	// some re-ordering (only do this if we've been queued, and therefore
	// have an alarm id).
	if (Id() != KNullAlarmId)
		{
		ServerData().Queue().HandleAlarmStatusChanged(Id(), oldStatus);
		}

	return KErrNone;
	}
	
