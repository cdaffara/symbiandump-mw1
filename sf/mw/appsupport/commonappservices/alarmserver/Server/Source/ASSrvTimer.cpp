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

#include "ASSrvTimer.h"

// System includes
#include <e32base.h>

// User includes
#include "ASSrvStaticUtils.h"
#include "ASSrvAlarmQueue.h"
#include "ASSrvTimerObserver.h"
#include "ASSrvServerWideData.h"
#include "ASSrvAnyEventManager.h"

//
// ----> CASSrvAlarmTimer (source)
//

//*************************************************************************************
CASSrvAlarmTimer::CASSrvAlarmTimer(CASSrvServerWideData& aServerWideData)
:	CTimer(CActive::EPriorityStandard), iServerWideData(aServerWideData), iNextDueAlarmId(KNullAlarmId),
	iInvalidatedByInternalize(EFalse)
	{
	CActiveScheduler::Add(this);
	}


//*************************************************************************************
CASSrvAlarmTimer::~CASSrvAlarmTimer()
	{
	ServerData().Queue().NotificationPoolChangeCancel(*this);
	iNotificationList.Close();
	}


//*************************************************************************************
void CASSrvAlarmTimer::ConstructL()
	{
	CTimer::ConstructL();
	ServerData().Queue().NotificationPoolChangeL(*this);
	}


//*************************************************************************************
CASSrvAlarmTimer* CASSrvAlarmTimer::NewL(CASSrvServerWideData& aServerWideData)
	{
	CASSrvAlarmTimer* self = new(ELeave) CASSrvAlarmTimer(aServerWideData);
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
 * Request notifications when an alarm expires
 */
void CASSrvAlarmTimer::NotifyAlarmExpiredL(MASSrvAlarmTimerObserver& aObserver)
	{
	User::LeaveIfError(iNotificationList.InsertInAddressOrder(&aObserver));
	}


//*************************************************************************************
/**
 * Cancel a previous notification request
 */
void CASSrvAlarmTimer::NotifyAlarmExpiredCancel(MASSrvAlarmTimerObserver& aObserver)
	{
	TInt index = KErrNotFound;
	const TInt error = iNotificationList.FindInAddressOrder(&aObserver, index);
	if	(error != KErrNotFound)
		iNotificationList.Remove(index);
	}


//
//
//


//*************************************************************************************
/**
 * @see MASSrvAlarmQueueObserver
 */
void CASSrvAlarmTimer::MAlarmQueueObserverHandleEvent(TASSrvAlarmQueueEvent aEvent, TAlarmId aAlarmId)
	{
	const TAlarmId previousDueAlarmId = iNextDueAlarmId;

	/* There two interesting events that we should look for:
	 * Alarm Queue Start Internalize - everything we know is now invalid
	 * Alarm Queue Head Item Changed - stop what we are doing, and see if/when
	 *   the next alarm is due.
	 */
	switch (aEvent)
		{
	case EASSrvAlarmQueueEventAlarmStartInternalize:
		// stop processing alarm queue, (until a new head alarm appears)
		Cancel();
		// RunL could have started already (it waits for an Access Token)
		// and would be processing an invalid head alarm!
		iInvalidatedByInternalize = ETrue;
		break;

	case EASSrvAlarmQueueEventHeadItemChanged:
		{
	// The next alarm has changed.
	Cancel();

		// head alarm is valid
		iInvalidatedByInternalize = EFalse;

	// Set ourselves up for the next alarm, assuming there is one.
	CASSrvAlarmQueue& queue = ServerData().Queue();
	const TASSrvAlarm* nextDueAlarm = queue.HeadAlarm();
	if	(nextDueAlarm && aAlarmId != KNullAlarmId)
		{
		__ASSERT_DEBUG(nextDueAlarm->Id() != KNullAlarmId, ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicQueueAlarmIdNull));
		
		// Convert alarm time to local time
		TTime dueTime = nextDueAlarm->NextDueTime();
		
		// Get current time
		const TTime now(ASSrvStaticUtils::UtcTimeNow());
		
		// Expire now (if the alarm is in the past), or when the alarm
		// is really due (if it's in the future).
		TTime t = dueTime > now? dueTime : now;


		AtUTC(t);					

		// Update next due alarm id and original due time
		iOriginalExpiryTimeForAlarm = dueTime;
		iNextDueAlarmId = aAlarmId;
		}
		}
		break;

	default: // don't care about other events
		return;
		}

	// Notify change
	if	(iNextDueAlarmId != previousDueAlarmId)
		ServerData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventHeadQueueItemChanged, iNextDueAlarmId);
	}


//
//
//


//*************************************************************************************
/**
 * @see CActive
 */
void CASSrvAlarmTimer::RunL()
	{
	// Cache this, so that notifications can queue the next alarm with the
	// timer.
	const TAlarmId justExpiredAlarmId = iNextDueAlarmId;

	// Set this back to KNullAlarmId indicating that we don't (yet) have another
	// alarm to notify about. We must do this just in case there aren't any more
	// suitable alarms in the queue, otherwise we will always indicate that we
	// are notifying about an (already) notified alarm.
	iNextDueAlarmId = KNullAlarmId;

	// check that Internalize hasn't invalidated the head alarm
	if (!iInvalidatedByInternalize && (iNextDueAlarmId == KNullAlarmId))
		{
	const TInt errorStatus = iStatus.Int();
	switch(errorStatus)
		{
	// Time changed - RChangeNotifier for time changes only used if no alarm is set.
	case KErrAbort:
		NotifyEvent(MASSrvAlarmTimerObserver::EAlarmTimerEventTimeOrDateChanged, KNullAlarmId);
		break;
	
	// Ignore times set in the past
	case KErrUnderflow:
	case KErrNone:
			if (!iInvalidatedByInternalize)
		NotifyEvent(MASSrvAlarmTimerObserver::EAlarmTimerEventAlarmExpired, justExpiredAlarmId);
		break;

	// No timer reset if the date is close to the overflow range. 
	// This means that agenda will not get notified of time changes
	case KErrOverflow:

	// Fall through to error handler
	default:
		HandleTimerError(justExpiredAlarmId, errorStatus);
		NotifyEvent(MASSrvAlarmTimerObserver::EAlarmTimerEventTimingError, justExpiredAlarmId);
		break;
		}

	// Notify change
	ServerData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventTimerExpired, justExpiredAlarmId);
		}
	}


//*************************************************************************************
/**
 * @see CActive
 */
void CASSrvAlarmTimer::Cancel()
	{
	CTimer::Cancel();
	//
	iNextDueAlarmId = KNullAlarmId;
	iOriginalExpiryTimeForAlarm = Time::NullTTime();
	}


//*************************************************************************************
/**
 * @see CActive
 */
TInt CASSrvAlarmTimer::RunError(TInt /*aError*/)
	{
	// Ignore erorrs
	return KErrNone;
	}


//
//
//


//*************************************************************************************
/**
 * Notify observers about the specified event
 */
void CASSrvAlarmTimer::NotifyEvent(MASSrvAlarmTimerObserver::TAlarmTimerEvent aEvent, TAlarmId aId)
	{
  	for(TInt i=iNotificationList.Count(); i>0; --i)
   		// Notify observers
		// TASSrvAlarm::MATimerHandleAlarmExpired() removes itself from the list
   		iNotificationList[i-1]->MATimerHandleAlarmExpired(aEvent, aId);
	}


//*************************************************************************************
/**
 * Handle an error returned in CTimer::iStatus 
 */
void CASSrvAlarmTimer::HandleTimerError(TAlarmId aIdOfJustExpiredAlarm, TInt aErrorCode)
	{
	CASSrvAlarmQueue& queue = ServerData().Queue();
	TASSrvAlarm* expiredAlarm = queue.QueueAlarmById(aIdOfJustExpiredAlarm);
	expiredAlarm->HandleTimerError(aErrorCode);
	}
