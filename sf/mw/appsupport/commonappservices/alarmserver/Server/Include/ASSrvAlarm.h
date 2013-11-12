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

#ifndef __ASSRVALARM_H__
#define __ASSRVALARM_H__

// System includes
#include <e32base.h>

// User includes
#include <asshdalarm.h>
#include <asshddefs.h>
#include "ASSrvDefs.h"
#include "ASSrvTimerObserver.h"

// Constants
const TASSrvAlarmSoundCycleNumber KUndefinedCycleIndex = -1;
const TInt KAlarmServerCTimerFudgeTimeInSeconds = 2;
const TInt KAlarmServerDefaultAlowableWindowInSeconds = 0;
const TInt KAlarmServerStartupExpireWindowInSeconds = 59;

// Enumerations

// Classes referenced
class CASSrvServerWideData;


//
// ----> TASSrvAlarm (header)
//
/**
 * The server-side representation of an alarm - now based on a UTC alarm.
 */
class TASSrvAlarm : public TASShdAlarm, public MASSrvAlarmTimerObserver
	{
//
public:										// ENUMERATIONS
//
	/**
	 * Used to keep track of whether the alarm has been snoozed
	 * and which time to next use as the basis for alarm expiry
	 */
	enum TTimeType
		{
		ETimeTypeNextDue					= 0,
		ETimeTypeOriginalExpiry
		};

	/**
	 * Internal flags used within the Alarm Server
	 */
	enum TInternalServerFlags
		{
		/**
		 * The alarm server has a handle to a client's
		 * TRequestStatus (via an RMessage2) which it
		 * will complete when this alarm expires.
		 */ 
		EInternalServerFlagsNotifyPending	= 0,
		/**
		 * This alarm sound playing has been paused
		 */ 
		EASShdAlarmFlagsSoundHasBeenPaused
		};

	/**
	 * The state of this alarm with respect to sound
	 */
	enum TSoundState
		{
		/**
		 * This alarm is not playing a sound
		 */
		ESoundStatePlayingNothing			= 0,

		/**
		 * This alarm is playing a sound
		 */
		ESoundStatePlayingSound
		};
	
//
public:										// STATIC CONSTRUCT / DESTRUCT
//

	TASSrvAlarm(CASSrvServerWideData& aData);

//
public:										// OPERATORS
//

	TASSrvAlarm&							operator=(const TASSrvAlarm& aAlarm);

//
public:										// ACCESS - SETTERS
//

	TInt									SetStatus(TAlarmStatus aStatus);

	void									SetState(TAlarmState aState);

	void									SetCharacteristicsL(TAlarmCharacteristicsFlags aFlags, TASSrvSessionId aSessionChangingFlags);

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	void									SetWakeupAndNotifyQueueL(TBool aEnable);
#endif
	
#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
	void									SetAlarmDaysL(TUint8 aAlarmDays);
#endif
	
	void									SetAlarmOrphaned();//set alarm orphaned when the owner session has been closed

	void									SetOriginatingSessionId(TASSrvSessionId aId);

//
public:										// ACCESS - GETTERS
//

	/**
	 * Access the originating session id for this alarm.
	 */
	TASSrvSessionId 						OriginatingSessionId() const;

	/**
	 * Access the sound state for this alarm
	 */
	TSoundState 							SoundState() const;

//
public:										// ACCESS - MISC
//

	void									RequestExpiryNotificationL(const RMessage2& aMessage);

	void									RequestExpiryNotificationComplete(TInt aErrorCode);

	/**
	 * Does this alarm have an owning session and 
	 */
	TBool									HasNotificationRequestPending() const;

	void									RoundDownTimeToMinute(TTimeType aType);

	void									DeQueue();

	void									Reset();
	
	void									ClearFlags();

	/**
	 * Update the sound state of this alarm
	 */
	void									SetSoundState(TSoundState aSoundState);

	void									ToggleSoundState();

	void									ReinitializeSoundState(const TTime& aBaselineForSoundTiming);

	TTime									CalculateAndPrepareNextSoundCycle();

	TInt									ValidateAndEnable(TTimeIntervalSeconds aAllowableWindow = KAlarmServerStartupExpireWindowInSeconds, TBool aAllowAnyOnceAlarmInThePast = EFalse, TBool aEnable = ETrue);

	void									Snooze(const TTime& aTimeToAwaken);

	/**
	 * Has the alarm sound playing been paused?
	 */
	TBool									HasSoundPaused() const;

	/**
	 * Set the EASShdAlarmFlagsSoundHasBeenPaused flag
	 */
	void									SetSoundPausedFlag();

	/**
	 * Clear the EASShdAlarmFlagsSoundHasBeenPaused flag
	 */
	void									ClearSoundPausedFlag();

//
public:										// DATA RELATED
//

	void									DataAttachL(HBufC8* aData);

	void									DataDetachL();

	TInt									DataSizeL() const;

	const TDesC8&							DataL() const;

	const TDesC8&							Data() const;

//
public:										// EVENT HANDLERS
//

	void									HandleTimerError(TInt aErrorCode);

	void									HandleDeQueue();

	TBool									HandleDateTimeChangedL(TUint aWorkdays, TBool aWorkdaysChanged);

//
private:									// FROM MASSrvAlarmTimerObserver
//

	void									MATimerHandleAlarmExpired(TAlarmTimerEvent aEvent, TAlarmId aAlarmId);

//
private:									// INTERNAL
//

	/**
	 * Return the server data object
	 */
	CASSrvServerWideData&					ServerData() const;

	void									NotificationMessageComplete(TInt aCompletionCode);

	TInt									PrepareForNextRepeat(TTimeIntervalSeconds aAllowableWindow = KAlarmServerDefaultAlowableWindowInSeconds);

//
private:									// INTERNAL - SOUND TIMING RELATED
//

	/**
	 * Set the sound cycle
	 */
	void									SetSoundTimingCycleIndex(TASSrvAlarmSoundCycleNumber aSoundNumber);

	/**
	 * Access the sound cycle
	 */
	TASSrvAlarmSoundCycleNumber			SoundTimingCycleIndex() const;

//
public:										// Helper for AlarmQueue Internalize
//

	/**
	 * Internalize of the Alarm Queue after a Restore (from a backup)
	 * deletes the old queue.  This method sends a Cancel notification for
	 * a 'Session Alarm' to its the TRequestStatus object.
	 */
	void									CancelSessionAlarm();

//
private:									// INTERNAL - Set Alarm Status
//

	/**
	 * Internally used function for set alarm status.
	 */
	TInt									doSetStatus(TAlarmStatus aStatus, TBool aAutoDisabled);

//
private:									// MEMBER DATA
//

	/**
	 * When an alarm plays, it has a controlling time period which specifies how
	 * long the sound should be played for (before it is automatically made silent again).
	 * Typically there are a series of cycles, each with a custom-defined duration
	 * for the sound. This attribute specifies which cycle is currently being timed.
	 */
	TASSrvAlarmSoundCycleNumber			iSoundPeriodCycleNumber;

	/**
	 * Ordinarily, an alarm is first "heard" when at its due time. However, if
	 * the alarm is snoozed, or there is a quiet period, the alarm's original due
	 * time cannot be used for sound-timing related calcualtions. Therefore, by
	 * default, this value is initialized to the due time when the alarm first
	 * expires, and is updated with a new time when the alarm is snoozed or
	 * the sound is paused.
	 */
	TTime									iStartTimeForSoundCalculations;

	/**
	 * When an alarm is notifying it either is playing a sound or silent (depending
	 * on the alarm sound play intervals). This enumeration represents that state.
	 */
	TSoundState								iSoundState;

	/**
	 * Internal flags used by the server side representation of an alarm object.
	 */
	TBitFlags								iInternalServerFlags;

	/**
	 * The session which originally created this alarm
	 */
	TASSrvSessionId							iOriginatingSessionId;

	/**
	 * The request status object that is completed when the alarm expires or is
	 * cancelled.
	 */
	RMessagePtr2							iNotificationMessage;

	/**
	 * A handle to the server wide data.
	 */
	CASSrvServerWideData&					iServerWideData;
	};


#endif


