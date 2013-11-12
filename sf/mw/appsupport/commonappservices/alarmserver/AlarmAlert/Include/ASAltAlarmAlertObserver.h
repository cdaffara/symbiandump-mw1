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

#ifndef __ASALTALARMALERTOBSERVERT_H__
#define __ASALTALARMALERTOBSERVERT_H__

// System includes
#include <e32std.h>
#include <asshddefs.h>

//
// ----> MASAltAlarmAlertObserver (header)
//
/**
 * Mixin class which represents an observer of command instructions
 * from the Alarm Alert Server (which runs in the EikSrv thread).
 */
class MASAltAlarmAlertObserver
	{
//
public:										// FROM MASAltAlarmAlertObserver
//
	enum TAcknowledgeType
		{
		/**
		 * Dismiss the currently notifying alarm
		 */
		EAcknowledgeCurrentAlarm = 0,

		/**
		 * Dismiss the currently notifying alarm, and any others that
		 * are awaiting notification.
		 */
		EAcknowledgeAllAlarms
		};

//
public:										// FROM MASAltAlarmAlertObserver
//

	/**
	 * Stop any sound calculations for the current alarm. This effectively initates
	 * a callback to the Alarm Alert Server informing it to stop playing sound 
	 * for the current alarm.
	 * If the given alarm is not playing sound, the silence operation is ignored.
	 *
	 * @param aAlarmId Alarm Id of the alarm to silence.
	 */
	virtual void							ASAltSilenceCurrentAlarm(TAlarmId aAlarmId) = 0;

	/**
	 * Dismiss the current alarm
	 *
	 * @param aType The type of alarm to dismiss (just the currently playing alarm with the given
	 *              alarm id, or all alarms that may be alerting).
	 * @param aAlarmId If aType is EAcknowledgeCurrentAlarm, Alarm Id of the alarm to acknowledge.
	 */
	virtual void							ASAltAcknowledgeAlarm(TAcknowledgeType aType, TAlarmId aAlarmId) = 0;

	/**
	 * Snooze the current alarm, preventing any sound from taking place. The alarm
	 * notification furniture will also be removed from display.
	 *
	 * @param aTimeWhenAlarmToReAwaken Time that the alarm snooze will reawaken.
	 * @param aAlarmId Alarm Id of the alarm to snooze.
	 * @return KErrNone if no error
	 */
	virtual TInt							ASAltSnoozeAlarmUntil(const TTime& aTimeWhenAlarmToReAwaken, TAlarmId aAlarmId) = 0;

	/**
	 * Silence all alarm sounds until the specified time. Note that no sound will
	 * occur for any alarms until the resumption time is reached.
	 * 
	 * @param aTimeWhenSoundsShouldResume The time when the sound should start playing again
	 */
	virtual void							ASAltQuietPeriodUntil(const TTime& aTimeWhenSoundsShouldResume) = 0;

	/**
	 * Pause the alarm sound until the specified time. The alarm state does not change.
	 * 
	 * @param aTimeWhenSoundSoundResume The time when the sound should start playing again
	 * @param aAlarmId Alarm Id of the alarm that will have sound paused.
	 */
	virtual void							ASAltPauseSoundUntil(const TTime& aTimeWhenSoundSoundResume, TAlarmId aAlarmId) = 0;

	};

#endif
