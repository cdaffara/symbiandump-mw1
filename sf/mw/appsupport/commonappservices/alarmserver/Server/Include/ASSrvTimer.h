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

#ifndef __ASSRVTIMER_H__
#define __ASSRVTIMER_H__

// System includes
#include <e32base.h>

// User includes
#include "ASSrvTimerObserver.h"
#include "ASSrvAlarmQueueObserver.h"

// Classes referenced
class CASSrvServerWideData;


//
// ----> CASSrvAlarmTimer (header)
//
/**
 * The timer for the next alarm in the queue
 */
class CASSrvAlarmTimer : public CTimer, public MASSrvAlarmQueueObserver
	{
//
public:										// STATIC CONSTRUCT / DESTRUCT
//
	static CASSrvAlarmTimer*				NewL(CASSrvServerWideData& aServerWideData);
	~CASSrvAlarmTimer();

//
private:									// INTERNAL CONSTRUCT
//
	CASSrvAlarmTimer(CASSrvServerWideData& aServerWideData);
	void									ConstructL();

//
public:										// ACCESS
//

	void									NotifyAlarmExpiredL(MASSrvAlarmTimerObserver& aObserver);

	void									NotifyAlarmExpiredCancel(MASSrvAlarmTimerObserver& aObserver);

	/**
	 *  Return the id of the next due alarm
	 */
	inline TAlarmId							NextDueAlarmId() const { return iNextDueAlarmId; }

	/**
	 * Return the time at which the next due alarm is expected to expire
	 */
	inline const TTime&						NextDueAlarmOriginalExpiryTime() const { return iOriginalExpiryTimeForAlarm; }

//
private:									// FROM MASSrvAlarmQueueObserver
//

	void									MAlarmQueueObserverHandleEvent(TASSrvAlarmQueueEvent aEvent, TAlarmId aAlarmId);

//
private:									// FROM CActive
//

	void									RunL();

	void									Cancel();

	TInt									RunError(TInt aError);

//
private:									// INTERNAL
//

	/**
	 * Access the server wide data
	 */
	inline CASSrvServerWideData&			ServerData() const { return iServerWideData; }

	void									NotifyEvent(MASSrvAlarmTimerObserver::TAlarmTimerEvent aEvent, TAlarmId aId);

	void									HandleTimerError(TAlarmId aIdOfJustExpiredAlarm, TInt aErrorCode);

//
private:									// MEMBER DATA
//

	/**
	 * The server wide data
	 */
	CASSrvServerWideData&					iServerWideData;

	/**
	 * The id of the next due alarm
	 */
	TAlarmId								iNextDueAlarmId;

	/**
	 * The time the alarm is due to expire. This allows the queue to
	 * compare the alarm at the head of the queue against this alarm
	 * to work out if the head item has changed.
	 */
	TTime									iOriginalExpiryTimeForAlarm;

	/**
	 * A list of observers
	 */
	RPointerArray<MASSrvAlarmTimerObserver>	iNotificationList;

	/**
	 * An Internalize has happened, invalidating the alarm queue and head alarm.
	 */
	TBool									iInvalidatedByInternalize;
	};

#endif
