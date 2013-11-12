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

#ifndef __ASSRVSOUNDCONTROLLER_H__
#define __ASSRVSOUNDCONTROLLER_H__

// System includes
#include <e32base.h>
#include <babitflags.h>

// User includes
#include <asshddefs.h>
#include "ASSrvAlarmObserver.h"
#include "ASSrvAlarmQueueObserver.h"
#include "ASSrvSoundSettingsObserver.h"
#include "ASSrvSoundControllerObserver.h"
#include "ASSrvEnvironmentChangeObserver.h"

// Classes referenced
class TASSrvAlarm;
class CASSrvServerWideData;
class MASSrvSoundControllerObserver;


//
// ----> CASSrvSoundController (header)
//
/**
 * The sound controller performs all the timing related functionality associated
 * with the sound play intervals (these essentially are "auto-snoozed" alarm sounds,
 * whereby after a given length of time, the alarm sound is silenced for a small
 * amount of time). For example, the sound plays for 30 seconds, then is silent
 * for 30, then it plays again for 30 seconds, and then is slient for 1 minute, etc.
 *
 * The sound controller notifies an observer which actually implements the sound
 * playing functionality (in fact, the observer is just a proxy for the Alarm
 * Alert Server, where the sound is really played).
 */
class CASSrvSoundController : public CTimer, 
	public MASSrvAlarmObserver, 
	public MASSrvSoundSettingsObserver, 
	public MASSrvAlarmQueueObserver, 
	public MASSrvEnvironmentChangeObserver
	{
//
public:										// STATIC CONSTRUCT / DESTRUCT
//
	static CASSrvSoundController*			NewL(CASSrvServerWideData& aServerWideData, MASSrvSoundControllerObserver& aObserver);
	~CASSrvSoundController();

//
private:									// INTERNAL CONSTRUCT
//
	CASSrvSoundController(CASSrvServerWideData& aServerWideData, MASSrvSoundControllerObserver& aObserver);
	void									ConstructL();

//
private:									// FROM MASSrvAlarmObserver
//

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
	void									MASSrvAlarmObsHandleEvent(TObserverEvent aEvent, const TASSrvAlarm& aAlarm, TInt aEventSpecificData);

//
private:									// FROM MASSrvSoundSettingsObserver
//

	/**
	 * @see MASSrvSoundSettingsObserver
	 */
	void									MASSoundSettingsHandleChangeEvent(TSoundSettingsEvent aEvent);

//
private:									// FROM MASSrvAlarmQueueObserver
//

	/**
	 * @see MASSrvAlarmQueueObserver
	 */
	void									MAlarmQueueObserverHandleEvent(TASSrvAlarmQueueEvent aEvent, TAlarmId aAlarmId);

//
private:									// FROM MASSrvEnvironmentChangeObserver
//

	/**
	* @see MASSrvEnvironmentChangeObserver
	*
	* Since PREQ234 all alarm silent periods are stored in UTC time but specified in 
	* local time hence the need to track changes to system time.
	*/

	void MEnvChangeHandleEvent(TInt aChanges, TUint aWorkdays, TBool aWorkdaysChanged);

//
public:										// ACCESS
//

	/**
	 * Stops any sound from playing and resets the sound timer to an idle state.
	 */
	void									CancelAllSounds();

	/**
	 * Stop playing any sounds until the specified time updating with UTC offset changes.
	 */
	void									MakeAllSoundsQuietUntil(const TTime& aTime);
	
	/**
	 * Returns the
	 */
	inline const TTime&						SoundsQuietUntil() const { return iTimeToReturnToNormalSoundTimingBehaviour; }

	/**
	 * Stop playing the sound (for just this alarm) until the specified time.
	 */
	void									CurrentAlarmSoundPausedUntil(const TTime& aTime);

	/**
	 * Returns a boolean indicating whether or not the alarm server is currently
	 * in a quiet period.
	 */
	TBool									InQuietPeriod() const;

	/**
	 * Return a boolean indicating whether sound is currently
	 * paused within the alarm server.
	 */
	TBool									SoundIsPausedForAlarm() const;

	/**
	 * Cancel a previously enabled silent period
	 */
	void									CancelSilence();
	
	/**
	 * Do not update silent period with UTC offset changes.
	 */
	inline void								SetFixed() { Flags().Set(ESoundControllerFlagsIsFixed); }

	/**
	 * Update silent period with UTC offset changes.
	 */	
	inline void								SetFloating() { Flags().Clear(ESoundControllerFlagsIsFixed); }

	void									CancelSound(TAlarmId aAlarmId);
	
//
private:									// FROM CActive
//

	/**
	 * @see CActive
	 */
	void									RunL();

	/**
	 * Replacement of CActive::Cancel function
	 */
	void									Cancel();

//
private:									// INTERNAL
//

	/**
	 * Access the server wide data
	 */
	inline CASSrvServerWideData&			ServerData() const { return iServerWideData; }

	/**
	 * Writable access to this class' interal flags
	 */
	inline TBitFlags&						Flags() { return iSoundControllerFlags; }

	/**
	 * Access to this class' interal flags
	 */
	inline const TBitFlags&					Flags() const { return iSoundControllerFlags; }

	/**
	 * Writable access to the id of the currently notifying alarm
	 */
	inline TAlarmId&						IdOfNotifyingAlarm() { return iIdOfNotifyingAlarm; }

	/**
	 * The id of the currently notifying alarm
	 */
	inline TAlarmId							IdOfNotifyingAlarm() const { return iIdOfNotifyingAlarm; }

	/**
	 * Handle a timer event - called when the sound timer expires
	 */
	void									ReactToSoundTimerExpiry();

	/**
	 * Notify observers about the specified event
	 */
	void									NotifySoundEvent(MASSrvSoundControllerObserver::TSoundControllerEvent aEvent, TAlarmId aAlarmId);

	/**
	 * Manage sound timing
	 */
	void									StartSoundTimingForAnyWaitingAlarmAfterPausingOrQuietPeriod();

	/**
	 * Calculate when the next start/stop timer period should be for
	 * alarm sounds
	 */
	void									WorkOutAndScheduleForNextSoundCycle();
	
	/**
	 * Indicates that the Sound Controller is disabled (ie. no sounds
	 * intervals configured in the rsc file)
	 */
	TBool									IsSoundControllerDisabled() const;
	

	void									PlayPreviousNotifyingAlarm();

//
private:									// INTERNAL ENUMERATIONS
//
	/**
	 * Internal sound flags
	 */
	enum TSoundControllerFlags
		{
		/**
		 * Pausing
		 */
		ESoundControllerFlagsPausing		= 0,

		/**
		 * Quiet period
		 */
		ESoundControllerFlagsInQuietPeriod	= 1,
		
		/**
		 * Until (floating) or For (fixed) silent period
		 */
		 
		ESoundControllerFlagsIsFixed = 2,
		};

//
private:									// MEMBER DATA
//

	/**
	 * The alarm id of the alarm which is currently being timed. KNullAlarmID if 
	 * no current alarm to notify.
	 */
	TAlarmId								iIdOfNotifyingAlarm;

	/**
	 * Record the previous sound event we sent to the SoundControlllerObserver
	 * (and AlertServer) so that we can suppress duplicate events.
	 */
	MASSrvSoundControllerObserver::TSoundControllerEvent	iPreviousSoundEvent;

	/**
	 * The time when normal sound behaviour should be resumed.
	 */
	TTime									iTimeToReturnToNormalSoundTimingBehaviour;

	/**
	 * Internal flags
	 */
	TBitFlags								iSoundControllerFlags;

	/**
	 * The server wide data handle
	 */
	CASSrvServerWideData&					iServerWideData;

	/**
	 * The observer of this class, in this case, the notification
	 * co-ordinator
	 */
	MASSrvSoundControllerObserver&			iObserver;
	
	/**
	 * UTC offset when the last request involving time was made
	 */
	 
	 TTimeIntervalSeconds iPreviousUTCOffset;
	};

#endif
