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
// Description: Implementation of the class representing the Alarm Queue 
//

#include <e32property.h>
#include "ASSrvAlarmQueue.h"


// User Includes
#include "ASSrvTimer.h"
#include "ASSrvStaticUtils.h"
#include "ASSrvServerWideData.h"
#include "ASSrvAnyEventManager.h"
#include "ASSrvIteratorByState.h"
#include "ASSrvIteratorByStatus.h"
#ifdef SYMBIAN_SKIPPED_CALENDAR_ALARMS
#include "ASSrvIteratorByCategory.h"
#endif
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
#include "ASSrvIteratorByWakeup.h"
#endif
#include "ASSrvEnvironmentChangeManager.h"
#include "ASSrvSoundSettings.h"
#include "ASSrvDataPool.h"
#include "ASSrvDSTChange.h"


// Constants
const TInt KASSrvNumberOfDaysInOneWeek = 7;

// Macro for security policy objects
static _LIT_SECURITY_POLICY_PASS(KReadPolicy);
static _LIT_SECURITY_POLICY_S0(KWritePolicy, 0x101f5027);

// Enumerations
#define DEBUG_PRINT_QUEUE(a){   RDebug::Print(_L(a));                  \
                                 for(TInt i=0; i<iAlarms.Count(); i++)  \
                                     {                                  \
                                     const TASSrvAlarm& alarm = QueueAlarmAt(i); \
                                     RDebug::Print(_L("ALARMSERVER\t Alarm[%02d] \"%S\" (%d)"), i, &alarm.Message(), alarm.Id()); \
                                     }                              \
                                RDebug::Print(_L("\n"));            \
                            }

// Uncomment the following define macro for getting logs in verbose mode
// #define VERBOSE_DEBUG

#if defined(VERBOSE_DEBUG)
#define DEBUG_PRINT1(A)			RDebug::Print(A)
#define DEBUG_PRINT2(A,B)		RDebug::Print(A,B)
#else
#define DEBUG_PRINT1(A)
#define DEBUG_PRINT2(A,B)
#endif


CASSrvAlarmQueue::CASSrvAlarmQueue(CASSrvServerWideData& aServerWideData)
    :iServerWideData(aServerWideData)
	{
	}

CASSrvAlarmQueue::~CASSrvAlarmQueue()
	{
	ServerData().EnvironmentChangeManager().RequestEnvironmentChangesCancel(*this);
	iAlarmObservers.Close();
 	iAlarms.ResetAndDestroy();
 	iAlarms.Close();
	if (iInternalizeAlarmQueue.Count() != 0)
	    {
	    iInternalizeAlarmQueue.ResetAndDestroy();
	    }
	iInternalizeAlarmQueue.Close();
 	iNotificationList.Close();
 	delete iASSrvDSTChange;

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
 	iSsmRtcAdaptation.Close();
#endif
	}

void CASSrvAlarmQueue::ConstructL()
	{
	ServerData().EnvironmentChangeManager().RequestEnvironmentChangesL(*this);
	iPreviousUtcOffset = User::UTCOffset();
	iASSrvDSTChange = CASSrvDSTChange::NewL(*this);

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	User::LeaveIfError(iSsmRtcAdaptation.Connect());
	// Notify the listeners of 'KWakeupAlarmPubSubKey' key that the alarm queue is yet  to be internalized 
	// by Alarm Server on its start-up at the device boot time.
	RProperty::Define(KAlarmServerPubSubCategory, KWakeupAlarmPubSubKey, RProperty::EInt, KReadPolicy, KWritePolicy);
	RProperty::Set( KAlarmServerPubSubCategory, KWakeupAlarmPubSubKey, EActiveWakeupAlarmUninitialized );
	DEBUG_PRINT1(_L("Set the KWakeupAlarmPubSubKey to EActiveWakeupAlarmUninitialized (startup)"));
#endif
	}

CASSrvAlarmQueue* CASSrvAlarmQueue::NewL(CASSrvServerWideData& aServerWideData)
	{
	CASSrvAlarmQueue* self = new(ELeave) CASSrvAlarmQueue(aServerWideData);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
 * @see MASSrvEnvironmentChangeObserver
 */
void CASSrvAlarmQueue::MEnvChangeHandleEvent(TInt aChanges, TUint aWorkdays, TBool aWorkdaysChanged)
	{
	// Locale changes can be triggered by a lot of reasons, process locale
	// changes only due to utc offset change and workdays change.
	if ( (aChanges & EChangesSystemTime) || aWorkdaysChanged || (aChanges & EChangesLocale && (iPreviousUtcOffset != User::UTCOffset())) )
		{
#ifdef SYMBIAN_SKIPPED_CALENDAR_ALARMS
		TBool possibleHiddenFloatingAlarm(EFalse);
		// Store the time of the first calendar alarm, as it may have been skipped		
		TTime firstSkippedAlarmLocalTime(0);
		
		const TAlarmId headCalendarAlarmId = HeadCalendarAlarmId();
		if (headCalendarAlarmId != KNullAlarmId)
			{
			const TASSrvAlarm* headCalendarAlarm = QueueAlarmById(headCalendarAlarmId);
			firstSkippedAlarmLocalTime = headCalendarAlarm->NextDueTime();
		
			// The calendar server adds a single alarm at a time.  If this 
			// alarm is fixed, a skipped alarm instances notification must be 
			// sent, since the calendar server may have had other unqueued 
			// alarms that were skipped.
			possibleHiddenFloatingAlarm = !headCalendarAlarm->IsFloating() && (headCalendarAlarm->NextDueTime() + iPreviousUtcOffset < ASSrvStaticUtils::LocalTimeNow());
			}
		//Retrive the old local time before the time zone has been changed for the first skipped alarm.
		firstSkippedAlarmLocalTime = firstSkippedAlarmLocalTime + ServerData().CachedUtcOffset();
		TBool skippedAgendaAlarm(EFalse);
		
		// Tell every alarm that the date/time has changed
		const TInt count = QueueAlarmCount();
		
		TRAP_IGNORE(
                    for (TInt i = count - 1; i >= 0; i--)
                        {
                        TASSrvAlarm& alarm = QueueAlarmAt(i);
                        if (alarm.HandleDateTimeChangedL(aWorkdays, aWorkdaysChanged))
                            {
                            skippedAgendaAlarm = ETrue;	
                            }
                        }
                    )
		    		
		// If there was a skipped agenda alarm, publish the environment change and the details of the first
		// skipped alarm, otherwise, if there was a UTC offset change, only publish the environment change
		if (skippedAgendaAlarm)
			{
			PublishSkippedAlarm(ETrue);
			PublishAlarmedInstanceParams(firstSkippedAlarmLocalTime, aChanges & EChangesSystemTime);
			}
		else if (possibleHiddenFloatingAlarm)
			{
			PublishAlarmedInstanceParams(firstSkippedAlarmLocalTime, aChanges & EChangesSystemTime);			
			}
		else if (iPreviousUtcOffset != User::UTCOffset())
			{
			PublishSkippedAlarm(EFalse);
			}
#else
		// Tell every alarm that the date/time has changed
		const TInt count = QueueAlarmCount();
			
		TBool skippedAgendaAlarm = EFalse;
			TRAP_IGNORE(
	    		for (TInt i = count - 1; i >= 0; i--)
	    			{
	    			TASSrvAlarm& alarm = QueueAlarmAt(i);
	 				if (alarm.HandleDateTimeChangedL(aWorkdays, aWorkdaysChanged))
	    				{
	    				skippedAgendaAlarm = ETrue;	
	    				}
	   				}
			    )
		if (skippedAgendaAlarm || iPreviousUtcOffset != User::UTCOffset())
			{
			PublishSkippedAlarm(skippedAgendaAlarm);
			}
#endif		
	
		//Re sort the alarms after the above date/time change
  		TLinearOrder<TASSrvAlarm> order(ASSrvStaticUtils::CompareAlarms);
  		iAlarms.Sort(order);

		// Update floating alarms' due times.
		UpdateFloatingDueTimes();

		// When the date/time/locale changes we always simulate a change in the head item so that the timer will reset itself.
		const TAlarmId newHeadItemId = HeadAlarmId();
		NotifyEvent(MASSrvAlarmQueueObserver::EASSrvAlarmQueueEventHeadItemChanged, newHeadItemId);

		// Global any event notification
		ServerData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventSystemDateTimeChanged, KNullAlarmId);

		// Remove the dead alarms
		RemoveDeadAlarms();
		
		// And update the UTC offset			
		iPreviousUtcOffset = User::UTCOffset();
		}

	if	(aChanges & EChangesMidnightCrossover)
		{
		//Tidy up alarm queue every midnight
		RemoveDeadAlarms(); 
		}
	}

/**
 * Request notification when the state or status of an alarm changes
 */
void CASSrvAlarmQueue::RequestAlarmObservationEventsL(MASSrvAlarmObserver& aObserver)
	{
	User::LeaveIfError(iAlarmObservers.Append(TASSrvAlarmObserverMapplet(aObserver)));
	}

/**
 * Cancel a previous notification request
 */
void CASSrvAlarmQueue::RequestAlarmObservationEventsCancel(MASSrvAlarmObserver& aObserver)
	{
	const TInt count = iAlarmObservers.Count();
	for(TInt i=0; i<count; i++)
		{
		TASSrvAlarmObserverMapplet& mapplet = iAlarmObservers[i];
		if	(&mapplet.Observer() == &aObserver)
			{
			iAlarmObservers.Remove(i);
			return;
			}
		}
	}

/**
 * Temporarily enabled or disable notifications
 */
void CASSrvAlarmQueue::RequestAlarmObservationEventsEnabled(MASSrvAlarmObserver& aObserver, TBool aEnabled)
	{
	const TInt count = iAlarmObservers.Count();
	for(TInt i=0; i<count; i++)
		{
		TASSrvAlarmObserverMapplet& mapplet = iAlarmObservers[i];
		if	(&mapplet.Observer() == &aObserver)
			{
			mapplet.SetEnabled(aEnabled);
			return;
			}
		}
	ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicNoMatchingObserver);
	}

/**
 * Adds the specified alarm to the alarm queue. Updates the alarm object
 * so that it contains a uniquely allocated id, suitable for clients to identify
 * this alarm amongst all others.
 */
void CASSrvAlarmQueue::QueueAlarmAndAllocateIdL(TASSrvAlarm& aAlarm, TAlarmId aSpecificAlarmId)
	{
	// Get the head id so that we know if the queue has changed
	const TAlarmId headItemId = HeadAlarmId();

	// Now allocate an id for the alarm. Doesn't matter if adding
	// the alarm fails (and therefore we orphan an Id - they can
	// be reused, as long as no current alarm shares the same id as 
	// another).
	const TAlarmId newId = aSpecificAlarmId == KNullAlarmId? NextFreeAlarmId() : aSpecificAlarmId;
	aAlarm.Id()=newId;
	
	// Add the alarm
	TASSrvAlarm* alarm = new(ELeave) TASSrvAlarm(ServerData());
	CleanupStack::PushL(alarm);
	*alarm = aAlarm;
	//
	TLinearOrder<TASSrvAlarm> order(ASSrvStaticUtils::CompareAlarms);
	User::LeaveIfError(iAlarms.InsertInOrderAllowRepeats(alarm, order));
	CleanupStack::Pop(alarm);

	// Change the state to queued
	TASSrvAlarm* newAlarm = QueueAlarmById(newId);
	newAlarm->SetState(EAlarmStateQueued);

	// Notify we've added an alarm
	NotifyEvent(MASSrvAlarmQueueObserver::EASSrvAlarmQueueEventAlarmAdded, newAlarm->Id());

	// Did the head item change?
	const TAlarmId newHeadItemId = HeadAlarmId();
	if	(newHeadItemId != headItemId)
		{
		// Head item has changed. The timer will pick up the change and requeue itself
		// for the new alarm.
		NotifyEvent(MASSrvAlarmQueueObserver::EASSrvAlarmQueueEventHeadItemChanged, newHeadItemId);
		}

	ServerData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventAlarmAddition, newId);
	}

/**
 * Releases a previously allocated alarm. Removes it from the queue, and if this
 * alarm was at the head of the queue, a new alarm is promoted.
 */
void CASSrvAlarmQueue::DeQueueAlarm(const TASSrvAlarm& aAlarm)
	{
	const TAlarmId id = aAlarm.Id();
	const TBool isHeadItem = (id == HeadAlarmId());

	// Find index from alarm
	TIdentityRelation<TASSrvAlarm> identityRelation(ASSrvStaticUtils::CompareAlarmsExact);
	const TInt errorOrIndex = iAlarms.Find(&aAlarm, identityRelation);
	__ASSERT_ALWAYS(errorOrIndex != KErrNotFound, ASSrvStaticUtils::Fault(ASSrvStaticUtils::EASSrvFaultAlarmNotFound));

	// Inform alarm it's being destroyed
	TASSrvAlarm* alarm = &QueueAlarmAt(errorOrIndex);
	__ASSERT_ALWAYS(alarm->Id() == aAlarm.Id(), ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicAttemptingToDequeWrongAlarm));
	alarm->HandleDeQueue();

	// DEBUG_PRINT_QUEUE("ALARMSERVER Before removing item")

	// Remove from queue
#ifdef _DEBUG
	const TInt count = QueueAlarmCount();
#endif
	iAlarms.Remove(errorOrIndex);
	delete alarm;
	__ASSERT_DEBUG(iAlarms.Count() == count-1, ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicAlarmNotDeQueued));

	// DEBUG_PRINT_QUEUE("ALARMSERVER After removing item")

	// Notify we've deleted an alarm
	NotifyEvent(MASSrvAlarmQueueObserver::EASSrvAlarmQueueEventAlarmDeleted, id);

	// DEBUG_PRINT_QUEUE("ALARMSERVER After notifying event")

	// Notify change
	ServerData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventAlarmDeletion, id);

	// DEBUG_PRINT_QUEUE("ALARMSERVER After notifying any event")

	if	(isHeadItem)
		{
		// Head item has changed. The timer will pick up the change and requeue itself
		// for the new alarm.
		const TAlarmId newHeadItem = HeadAlarmId();
		NotifyEvent(MASSrvAlarmQueueObserver::EASSrvAlarmQueueEventHeadItemChanged, newHeadItem);

		// DEBUG_PRINT_QUEUE("ALARMSERVER After head item changed")
		}
	}

/**
 * Returns the alarm at the head of the queue or NULL if there is none.
 */
const TASSrvAlarm* CASSrvAlarmQueue::HeadAlarm() const
	{
	// There is a head alarm if there is at least one alarm in the queue
	// and that alarm is not disabled.
	CASSrvAlarmQueue& self = const_cast<CASSrvAlarmQueue&>(*this);

	// Create & open primary iterator
	RASSrvIteratorByStatus primaryIterator(self, EAlarmStatusEnabled);
	primaryIterator.Open();

	// Create and attach secondary iterator
	RASSrvIteratorByState secondaryIterator(self, EAlarmStateQueued, EAlarmStateSnoozed);
	primaryIterator.IteratorAttach(secondaryIterator);

	if (primaryIterator.NextAlarmAvailable())
		{
		return QueueAlarmById(primaryIterator.NextAlarm().Id());
		}
	else
		{
		return NULL;
		}
	}

#ifdef SYMBIAN_SKIPPED_CALENDAR_ALARMS	
/**
 * Returns the calendar alarm at the head of the queue
 */
TAlarmId CASSrvAlarmQueue::HeadCalendarAlarmId() const
	{
	CASSrvAlarmQueue& self = const_cast<CASSrvAlarmQueue&>(*this);

	// Create & open primary iterator
	RASSrvIteratorByStatus primaryIterator(self, EAlarmStatusEnabled);
	primaryIterator.Open();

	// Create and attach secondary iterator
	RASSrvIteratorByState secondaryIterator(self, EAlarmStateQueued, EAlarmStateSnoozed);
	primaryIterator.IteratorAttach(secondaryIterator);
	
	// The category for calendar alarms is defined in caalarm.h and is
	// reproduced here to avoid a dependency on the calendar server
	const TUid KUidAgendaModelAlarmCategory = { 0x101F4A70 };
	
	// Create and attach tertiary iterator
	RASSrvIteratorByCategory tertiaryIterator(self, KUidAgendaModelAlarmCategory);
	primaryIterator.IteratorAttach(tertiaryIterator);

	TAlarmId returnAlarmId(KNullAlarmId);
	
	if (primaryIterator.NextAlarmAvailable())
		{
		returnAlarmId = primaryIterator.NextAlarm().Id();
		}
	
	return returnAlarmId;
	}
#endif

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
/**
 * Returns the alarm id of the head wakeup alarm of the queue
 */
TAlarmId CASSrvAlarmQueue::HeadWakeupAlarmId() const
	{
	// There is a head alarm if there is at least one alarm in the queue
	// and that alarm is not disabled.
	CASSrvAlarmQueue& self = const_cast<CASSrvAlarmQueue&>(*this);

	// Create & open primary iterator
	RASSrvIteratorByStatus primaryIterator(self, EAlarmStatusEnabled);
	primaryIterator.Open();

	// Create and attach secondary iterator
	RASSrvIteratorByState secondaryIterator(self, EAlarmStateQueued, EAlarmStateSnoozed);
	primaryIterator.IteratorAttach(secondaryIterator);
	
	// Create and attach tertiary iterator	
	RASSrvIteratorByWakeup tertiaryIterator(self);
	secondaryIterator.IteratorAttach(tertiaryIterator);
	
	TAlarmId returnAlarmId(KNullAlarmId);
	
	if (primaryIterator.NextAlarmAvailable())
		{
		returnAlarmId = primaryIterator.NextAlarm().Id();
		}
	
	return returnAlarmId;
	}
#endif

/**
 * Returns the id of the alarm at the head of the queue, or if there isn't a head alarm returns KNullAlarmId
 */
TAlarmId CASSrvAlarmQueue::HeadAlarmId() const
	{
	TAlarmId headItemId = KNullAlarmId;
	const TASSrvAlarm* headAlarm = HeadAlarm();
	if	(headAlarm)
		{
		headItemId = headAlarm->Id();
		}
	return headItemId;
	}

/**
 * Called whenever a session logs off. All session-specific alarm should be removed in this instance.
 */
void CASSrvAlarmQueue::RemoveAllSessionAlarmsBySessionId(TASSrvSessionId aSessionId)
	{
	const TInt count = QueueAlarmCount();
	for(TInt i=count-1; i>=0; i--)
		{
		const TASSrvAlarm& alarm = QueueAlarmAt(i);
		if	(alarm.OriginatingSessionId() == aSessionId && alarm.Characteristics().IsSet(EAlarmCharacteristicsSessionSpecific))
			DeQueueAlarm(alarm);
		}
	}

/**
 * Generates and returns the next valid alarm id.
 */
TAlarmId CASSrvAlarmQueue::NextFreeAlarmId()
	{
	if	(iNextFreeAlarmId == KMaxTInt)
	    {
	    iNextFreeAlarmId = 1; // Zero is special - "No alarm id"
	    }
	else
	    {
	    ++iNextFreeAlarmId;
	    }

	// Generate a unique alarm Id.
	while(AlarmIdIsInUse(iNextFreeAlarmId))
		{
		if	(iNextFreeAlarmId == KMaxTInt)
		    {
		    iNextFreeAlarmId = 1;
		    }
		else
		    {
		    ++iNextFreeAlarmId;
		    }
		}

	return iNextFreeAlarmId;
	}

/**
 * Specify type of Store (or Internalize) operation wanted:
 * 1. Externalize
 *		- synchronous write of alarm queue and related information to file.
 * 2. Internalize after Startup or Restore 
 *		- synchronous read of alarm queue and related information from file.
 * 3. Backup
 */
TInt CASSrvAlarmQueue::StartAlarmStoreOperation(TStoreOperation aStoreOperation)
	{
	TInt error = KErrNone;

	switch (aStoreOperation)
		{
	    case EStoreInternalizeStartup:
	        {
            DEBUG_PRINT1(_L("> StartAlarmStoreOperation (InternalizeStartup)"));
            // Can't have two concurrent Store operations
            if (iStoreOperation != EStoreIdle)
                error = KErrLocked;
            break;
	        }
	    case EStoreInternalizeRestore:
	        {
            DEBUG_PRINT1(_L("> StartAlarmStoreOperation (InternalizeRestore)"));
            // Check this is progression to Restore from Internalize
            if (iStoreOperation != EStoreRestore)
                error = KErrLocked;
            break;
	        }
	    case EStoreExternalize:
	        {
            DEBUG_PRINT1(_L("> StartAlarmStoreOperation (Externalize)"));
            // Can't have two concurrent Store operations
            if (iStoreOperation != EStoreIdle)
                error = KErrLocked;
            break;
	        }
	    case EStoreBackup:
	        {
	        DEBUG_PRINT1(_L("> StartAlarmStoreOperation (Backup)"));
	        // Can't have two concurrent Store operations
	        if (iStoreOperation != EStoreIdle)
	            error = KErrLocked;
	        break;
	        }
	    case EStoreRestore:
	        {
	        DEBUG_PRINT1(_L("> StartAlarmStoreOperation (Restore)"));
	        // Can't have two concurrent Store operations
	        if (iStoreOperation != EStoreIdle)
	            {
	            error = KErrLocked;
	            }
	        else
	            {
	            // notify clients that Restore has started
	            ServerData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventRestoreStarted, KNullAlarmId);
	            }
	        break;
	        }
	    case EStoreIdle:
        default:
            {
            DEBUG_PRINT1(_L("> StartAlarmStoreOperation (Idle or other invalid)"));
            error = KErrInUse;
            __ASSERT_DEBUG(EFalse, ASSrvStaticUtils::Fault(ASSrvStaticUtils::EASSrvFaultStartInvalidAlarmStoreOperation));
            break;
	        }
		}
	if (!error)
		{
		iStoreOperation = aStoreOperation;
		}
	
	DEBUG_PRINT2(_L("< StartAlarmStoreOperation result = %i"), error);
	return error;
	}

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
/** 
 * Handle a change to the next DST event
 */
void CASSrvAlarmQueue::HandleNextDSTChangeEventL()
	{
	UpdateRTC();
	}

/**
 * Find the head wakeup alarm and queue set it with the real-time-clock.
 */
void CASSrvAlarmQueue::UpdateRTC()
	{
	// We always call UnsetWakeupAlarm even if we are going to call SetWakeupAlarm later because the RTC
	// adaptation plug-in requires UnsetWakeupAlarm to be called before any call to SetWakeupAlarm.
	TRequestStatus status;
	iSsmRtcAdaptation.UnsetWakeupAlarm(status);
	User::WaitForRequest(status);
		
	// loop through the alarm queue until the first wakeup alarm is found
	TAlarmId headWakeupAlarmId = HeadWakeupAlarmId();
	
	DEBUG_PRINT2(_L("CASSrvAlarmQueue::UpdateRTC alarm count = %d"),QueueAlarmCount());
	
	if (headWakeupAlarmId != KNullAlarmId)
		{
		// There is a wakeup alarm
		const TASSrvAlarm* headWakeupAlarm = QueueAlarmById(headWakeupAlarmId);
		TTime wakeupAlarmTime(headWakeupAlarm->NextDueTime());
		
		if (headWakeupAlarm->IsFloating())
			{
			TTime nextDSTChangeUTC = iASSrvDSTChange->NextDSTChangeUTC();
			
			if (nextDSTChangeUTC != Time::NullTTime() && nextDSTChangeUTC < wakeupAlarmTime)
				{
				// The alarm is set to expire after a DST event so adjust the UTC time
				// of the alarm to what it will be after the DST rollover
				wakeupAlarmTime += iPreviousUtcOffset;
				wakeupAlarmTime -= iASSrvDSTChange->NextUTCOffset();
				}
			}
		
		// Set the key value to 'EWakeupAlarmSet' so that listeners are notified of the presence of an active wakeup alarm.  
		RProperty::Set(KAlarmServerPubSubCategory, KWakeupAlarmPubSubKey, EActiveWakeupAlarmSet );
		DEBUG_PRINT1(_L("Set the KWakeupAlarmPubSubKey to EActiveWakeupAlarmSet"));
	  		
		// Set the Real Time Clock in UTC
		TRequestStatus status;
		TPckgC<TTime> wakeupAlarmTimePckg(wakeupAlarmTime);
		iSsmRtcAdaptation.SetWakeupAlarm(wakeupAlarmTimePckg, status);
		User::WaitForRequest(status);
		}
	else
	    {
	    // Notify the listeners that there is no head wake alarm present in the alarm queue 
	    RProperty::Set(KAlarmServerPubSubCategory, KWakeupAlarmPubSubKey, EActiveNoWakeupAlarmsSet );
	    DEBUG_PRINT1(_L("Set the KWakeupAlarmPubSubKey to EActiveNoWakeupAlarmsSet"));
	    }
	
	}
#endif

/**
 * End an Alarm Store Operation (Internalize, Externalize, Backup or Restore).
 * Various tidying up including:
 *   Finalise an Internalise, or throw it away. (depending on aError)
 *   Process queued change events.
 * Release the Access Token.
 */
void CASSrvAlarmQueue::EndAlarmStoreOperation(TInt aError)
	{
	switch (iStoreOperation)
		{
	    case EStoreIdle:
	    default:
	        {
            DEBUG_PRINT1(_L("> EndAlarmStoreOperation (EStoreIdle or invalid)"));
            __ASSERT_DEBUG(EFalse, ASSrvStaticUtils::Fault(ASSrvStaticUtils::EASSrvFaultEndInvalidAlarmStoreOperation));
            break;
	        }
	    case EStoreInternalizeStartup:
	    case EStoreInternalizeRestore:
	        {
	        DEBUG_PRINT2(_L("> EndAlarmStoreOperation (InternalizeXXX, aError = %i)"), aError);

	        if (!aError)
	            {
	            // Alarm Queue is about to change
	            // (stops Alarm Timer, currently notifying alarm, etc...)
	            NotifyEvent(MASSrvAlarmQueueObserver::EASSrvAlarmQueueEventAlarmStartInternalize, KNullAlarmId);

	            // Apply Internalize buffers for Alarm Queue, Alarm Data & Sound Intervals
	            // Alarm Queue
	            ApplyInternalizedData(ETrue);
	            // Alarm Data Pool
	            ServerData().DataPool().ApplyInternalizedData(ETrue);
	            // Sound Settings
	            ServerData().SoundSettings().ApplyInternalizedData(ETrue);

	            // Notify observers about new queue
	            const TAlarmId newHeadItemId = HeadAlarmId();
	            if  (newHeadItemId != KNullAlarmId)
	                {
	                // Head item has changed. The timer will pick up the change and requeue itself
	                // for the new alarm.
	                NotifyEvent(MASSrvAlarmQueueObserver::EASSrvAlarmQueueEventHeadItemChanged, newHeadItemId);
	                }

	            // notify clients that Restore has completed
	            if (iStoreOperation == EStoreInternalizeRestore)
	                { 
	                ServerData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventRestoreCompleted, KNullAlarmId);
	                }

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT              
            
	            else if (iStoreOperation == EStoreInternalizeStartup)
	                {
	                // When internalize happens on alarm server start-up publish the information 
	                // whether we have an active alarm set or not
	                DEBUG_PRINT1(_L("CASSrvAlarmQueue::EndAlarmStoreOperation Calls UpdateRTC after internalizing the queue on alarm server startup"));
	                UpdateRTC();
	                }
            
#endif            
	            } // end of if block
	        else
	            {
	            // Discard Internalize buffers for Alarm Queue, Alarm Data & Sound Intervals
	            // Alarm Queue
	            ApplyInternalizedData(EFalse);
	            // Alarm Data Pool
	            ServerData().DataPool().ApplyInternalizedData(EFalse);
	            // Sound Settings
	            ServerData().SoundSettings().ApplyInternalizedData(EFalse);

	            // notify clients that Restore has failed
	            if (iStoreOperation == EStoreInternalizeRestore)
	                {
	                ServerData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventRestoreFailed, KNullAlarmId);
	                }
	            }
	        break;
	        }
	    case EStoreExternalize:
	        {
	        DEBUG_PRINT1(_L("> EndAlarmStoreOperation (Externalize)"));

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	        TRAP_IGNORE(UpdateRTC());
#endif
	        break;
	        }
	    case EStoreBackup:
	        {
	        DEBUG_PRINT1(_L("> EndAlarmStoreOperation (Backup)"));
	        break;
	        }
	    case EStoreRestore:
	        {
	        DEBUG_PRINT1(_L("> EndAlarmStoreOperation (Restore)"));
	        // notify clients that Restore has failed
	        ServerData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventRestoreFailed, KNullAlarmId);
	        break;
	        }
		} // end of switch-case block 
	
#ifdef SYMBIAN_SKIPPED_CALENDAR_ALARMS	
	
	// Republish the last alarmed instance parameters after internalizing on startup
	if (iStoreOperation == EStoreInternalizeStartup)
		{
		PublishAlarmedInstanceParams(iLastAlarmedInstanceParams);
		}
	
#endif		

	iStoreOperation = EStoreIdle;
	DEBUG_PRINT1(_L("< EndAlarmStoreOperation completed"));
	return;
	}

/**
 * Check that Alarm and Alarm Data is writable.
 * Policy is to leave with KErrLocked if Restore is in progress.
 */
void CASSrvAlarmQueue::CheckAlarmQueueWritableL()
	{
	if (iStoreOperation == EStoreRestore 
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
|| iServerWideData.ServerIsReadOnly()
#endif
      )
		{
		User::Leave(KErrLocked);
		}
	}

/**
 * Restore the queue from the specified stream.
 */
void CASSrvAlarmQueue::InternalizeL(RReadStream& aStream)
	{
	// First read the leading count which indicates how many alarms there
	// are.
	const TInt count = aStream.ReadInt32L();

	// Read in the stream
	TLinearOrder<TASSrvAlarm> order(ASSrvStaticUtils::CompareAlarms);

	if (iInternalizeAlarmQueue.Count() != 0)
	    {
	    iInternalizeAlarmQueue.ResetAndDestroy();
	    }

	TTimeIntervalSeconds alarmExpireWindow = 0;

	// what type of Internalize are we doing?
	switch (iStoreOperation)
		{
	    case EStoreInternalizeStartup: // system startup
	        {
	        // 59s window for alarms in the past
	        alarmExpireWindow = KAlarmServerStartupExpireWindowInSeconds;
	        break;
	        }
	    case EStoreInternalizeRestore: // after a backup restore
	        {
            // 0 window for alarms in the past
            // NB Notified Alarms stay Notified,
            // but Notifying Alarms become Notified
            // This is reasonable as a Restore of alarms is a dramatic thing to do.
            alarmExpireWindow = 0;
            break;
	        }
	    default:
	        {
	        __ASSERT_ALWAYS(EFalse, ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicInternalizeTypeInvalid));
	        break;
	        }
		}
	
	RPointerArray<TASSrvAlarm> alarmArray;
	CleanupResetAndDestroyPushL(alarmArray);
		
	for(TInt j=0; j<count; j++)
		{
		TASSrvAlarm* alarm = new(ELeave) TASSrvAlarm(ServerData());
		CleanupStack::PushL(alarm);
		aStream >> *alarm;
		alarmArray.AppendL(alarm);
		CleanupStack::Pop();
		}
		
	// Read in the next valid id
	iInternalizeNextFreeAlarmId = aStream.ReadInt32L();
	
	// Internalize the UTC offset the last time the queue was run.
	TInt32 tempInt;
	aStream >> tempInt;
	TTimeIntervalSeconds currentOffset = User::UTCOffset();
	TTimeIntervalSeconds cachedOffset(tempInt);
	
	TTime previousDSTChangeUTC = iASSrvDSTChange->PreviousDSTChangeUTC();
	TTimeIntervalMinutes previousUTCOffset = iASSrvDSTChange->PreviousUTCOffset();
	TTime localTimeBeforeDSTChange = previousDSTChangeUTC + previousUTCOffset;
	TTime localTimeAfterDSTChange = previousDSTChangeUTC + currentOffset;
	
	for(TInt i=0; i<count; i++)
		{
		// Internalize the alarm (replacing any previous details)
		TASSrvAlarm* alarm ;
		alarm = alarmArray[i];
		// Set id to KNullAlarmId to prevent notifications when
		// the alarm is enabled
		const TAlarmId originalId = alarm->Id();
		alarm->Id()=KNullAlarmId;

		// Validate the alarm, retaining the alarm status (enabled or disabled)
		// - we only add valid alarms to the queue.
		const TBool KAllowBlanketInThePastOnceOnlyAlarms = EFalse; // Default value
		const TBool KEnableAlarm = alarm->Status()==EAlarmStatusEnabled;
		//An alarm set to 'repeat next 24 hour' can be missed while the alarm server is inactive.
		//Therefore it should be changed to 'repeat once' before it is restored to the alarm queue
		//so that it is not incorrectly resheduled to notify in the next 24 hours
		if(alarm->RepeatDefinition() == EAlarmRepeatDefintionRepeatNext24Hours)
			{
			alarm->RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
			}
			
		TTime previousAlarmLocalTime = alarm->NextDueTime() + previousUTCOffset;
		TBool alarmInDSTChangeGap = (previousDSTChangeUTC != Time::NullTTime() // make sure DST change info has been published
										&& (previousAlarmLocalTime >= localTimeBeforeDSTChange && previousAlarmLocalTime < localTimeAfterDSTChange ) );
		
		TTimeIntervalSeconds missingTime;
		localTimeAfterDSTChange.SecondsFrom(localTimeBeforeDSTChange, missingTime);
		
		// Increase the allowable window if this alarm was due to go off in the DST change gap
		TTimeIntervalSeconds thisAlarmsExpireWindow = alarmInDSTChangeGap ? alarmExpireWindow.Int() + missingTime.Int() : alarmExpireWindow;
		
		// Update the due time of floating alarms during a DST event. This will ensure that alarms
		// are not invalidated by ValidateAndEnable().
		TTimeIntervalSeconds offsetDifference;
	
		if(alarm->IsFloating())
			{
			if (cachedOffset != currentOffset)
				{
				// Calculate the change in the offset.
				offsetDifference = (currentOffset.Int() - cachedOffset.Int());
				alarm->NextDueTime() -= offsetDifference;
				alarm->OriginalExpiryTime() -= offsetDifference;
				}	
			}
	
		const TInt error = alarm->ValidateAndEnable(thisAlarmsExpireWindow, KAllowBlanketInThePastOnceOnlyAlarms, KEnableAlarm);
		
		if (alarmInDSTChangeGap)
			{
			// The local alarm time was set during the missing DST changeover gap
			// this repeat should actually appear an hour later than 
			alarm->NextDueTime() += missingTime;
			}
		
		if	(error != KErrArgument)
			{
			// Alarm is okay, so insert it into the queue in the 
			// right position
			alarm->Id()=originalId;
			const TInt error = iInternalizeAlarmQueue.InsertInOrderAllowRepeats(alarm, order);
			User::LeaveIfError(error);
			alarmArray[i] = NULL;
			}
		else
			{
			delete alarmArray[i];
			alarmArray[i] = NULL;
			}
		}
	CleanupStack::PopAndDestroy(); //alarmArray
	ServerData().CachedUtcOffset() = currentOffset;
	}

/**
 * Whether to use the Internalize buffer, or throw it away
 */
void CASSrvAlarmQueue::ApplyInternalizedData(TBool aUseNewData)
	{
	DEBUG_PRINT2(_L("** ApplyInternalizedData(%u)"), aUseNewData);

	if (aUseNewData)
		{
		// Internalize Success

		// Replace all alarms with those from the stream
		// Assign new queue to replace old one
		ReplaceQueueWithInternalizedQueue();

		iNextFreeAlarmId = iInternalizeNextFreeAlarmId;
	
		// Remove any which are too old
		RemoveDeadAlarms();
		// Check whether the UTC offset has changed since the last time the Alarm Server
		// was running, and update floating alarm due times if it has.
		UpdateFloatingDueTimes();
		}
	else 
		{
		// Internalize Failure
		iInternalizeAlarmQueue.ResetAndDestroy();
		
		// Set the previous UTC offset to the current UTC offset, since there was no
		// previous offset internalised.
	    ServerData().CachedUtcOffset() = User::UTCOffset();
		}
	}

/**
 * Update the due times of floating alarms due to a locale or DST change.
 * 
 * @internalComponent
 */
void CASSrvAlarmQueue::UpdateFloatingDueTimes()
	{
	// Has the UTC offset changed due to a Locale change?
	TTimeIntervalSeconds offset = User::UTCOffset();
	
	if (ServerData().CachedUtcOffset() != offset)
		{
		// Update the locally set alarm due times because the UTC offset has changed.
		
		// Calculate the change in the offset.
		TTimeIntervalSeconds offsetDifference(offset.Int() - ServerData().CachedUtcOffset().Int());

		// Get the head id so that we know if the queue has changed
		const TAlarmId headItemId = HeadAlarmId();
		
		// Cycle through all alarms.
		TInt count = QueueAlarmCount();
		TBool orderChanged = EFalse;
		
		for (TInt i = 0; i < count; i++)
			{
			TASSrvAlarm& alarm = QueueAlarmAt(i);
			
			// Check whether the alarm is floating.
			if (alarm.IsFloating())
				{
				// Update the due time of the alarm, since it is floating.
				TTime oldDueTime = alarm.NextDueTime();
				
				// Subtract the change in offset from the due time to get the new due time.
				TTime newDueTime = oldDueTime - offsetDifference;
				
				// Set the new due time of the alarm.
				alarm.NextDueTime()=newDueTime;
				
				// Also, shift the original expiry time, to ensure that repeating
				// alarms repeat correctly.
				newDueTime = alarm.OriginalExpiryTime() - offsetDifference;
				
				// Shift the original ExpiryTime.
				alarm.OriginalExpiryTime()=newDueTime;
				orderChanged = ETrue;
				}
			}
			
		// Check whether the order of the alarm queue may have changed.
		if (orderChanged)
			{
			// Re-order the alarm queue.
			TLinearOrder<TASSrvAlarm> order(ASSrvStaticUtils::CompareAlarms);
			iAlarms.Sort(order);
			
			// Did the head item change?
			const TAlarmId newHeadItemId = HeadAlarmId();
			
			if	(newHeadItemId != headItemId)
				{
				// Head item has changed. The timer will pick up the change and requeue itself
				// for the new alarm.
				NotifyEvent(MASSrvAlarmQueueObserver::EASSrvAlarmQueueEventHeadItemChanged, newHeadItemId);
				}
			}

		// Set the previous UTC offset to the current UTC offset.
		ServerData().CachedUtcOffset() = offset;
		}
	}

/** 
 * Publish skipped alarm data.
 * 
 * @internalComponent
 */
void CASSrvAlarmQueue::PublishSkippedAlarm(TBool aSkippedCalAlarm)
	{
	// Define the property before use
	TInt err = RProperty::Define(KAlarmServerPubSubCategory, KMissingAlarmPubSubKey, RProperty::EByteArray, KReadPolicy, KWritePolicy);
	
	// If the define completed successfully, create the data and publish
	if (err == KErrNone || err == KErrAlreadyExists)
		{
		TMissedAlarmPubSubData pubSubData;
		pubSubData.iTimeOfChangeUtc.UniversalTime(); 
		if(aSkippedCalAlarm)
			{
			// There are skipped alarms after a system time or time zone change
			pubSubData.iValue = 2;
			}
		else
			{
			// There was a time zone change, but no alarms were skipped
			pubSubData.iValue = 1;	
			}
		
		TPckgBuf<TMissedAlarmPubSubData> pubSubBuf(pubSubData);
		RProperty::Set(KAlarmServerPubSubCategory, KMissingAlarmPubSubKey, pubSubBuf);	
		}
	}

#ifdef SYMBIAN_SKIPPED_CALENDAR_ALARMS

/**
 * Publish data about the skipped alarm.
 * 
 * @internalComponent
 */
void CASSrvAlarmQueue::PublishAlarmedInstanceParams(const TTime& aFirstSkippedAlarmLocalTime, TBool aSystemTimeChange)
	{
	// Create the data structure before publishing
	TASShdAlarmedInstanceParams alarmedInstanceParams;
	alarmedInstanceParams.iLocalStartTime = aFirstSkippedAlarmLocalTime;
	alarmedInstanceParams.iLocalEndTime = ASSrvStaticUtils::LocalTimeNow();
	alarmedInstanceParams.iTimeType = aSystemTimeChange ? EFloatingOrFixed : EFloating;
	
	PublishAlarmedInstanceParams(alarmedInstanceParams);
	}

/**
 * Publish data about the skipped alarm.
 * 
 * @internalComponent
 */
void CASSrvAlarmQueue::PublishAlarmedInstanceParams(const TASShdAlarmedInstanceParams& aAlarmedInstanceParams)
	{
	// Define the property before use
	TInt err = RProperty::Define(KAlarmServerPubSubCategory, KSkippedAlarmInstancesPubSubKey, RProperty::EByteArray, KReadPolicy, KWritePolicy);
	
	// If the define completed successfully, publish
	if (err == KErrNone || err == KErrAlreadyExists)
		{		
		TPckgBuf<TASShdAlarmedInstanceParams> pubSubBuf(aAlarmedInstanceParams);
		RProperty::Set(KAlarmServerPubSubCategory, KSkippedAlarmInstancesPubSubKey, pubSubBuf);
		iLastAlarmedInstanceParams = aAlarmedInstanceParams;
		}	
	}
#endif

/**
 * Store the queue to the specified stream.
 */
void CASSrvAlarmQueue::ExternalizeL(RWriteStream& aStream) const
	{
	const TInt count = QueueAlarmCount();
	aStream.WriteInt32L(count);
	//
	for(TInt i=0; i<count; i++)
		{
		const TASSrvAlarm& alarm = QueueAlarmAt(i);
		aStream << alarm;
		}
	aStream.WriteInt32L(iNextFreeAlarmId);
	
	// Externalize the UTC offset.
	TInt32 tempInt = ServerData().CachedUtcOffset().Int();
	aStream << tempInt;
	}

#ifdef SYMBIAN_SKIPPED_CALENDAR_ALARMS
/**
 * Externalize the last skipped agenda alarm data
 */
void CASSrvAlarmQueue::ExternalizeLastAlarmedInstanceParamsL(RWriteStream& aStream) const
	{
	TPckgBuf<TASShdAlarmedInstanceParams> paramsBuf(iLastAlarmedInstanceParams);
	aStream << paramsBuf;
	
	// Write two words reserved for future use
	aStream.WriteInt32L(0);
	aStream.WriteInt32L(0);
	}

/**
 * Internalize the last published skipped agenda alarm data
 */
void CASSrvAlarmQueue::InternalizeLastAlarmedInstanceParamsL(RReadStream& aStream)
	{
	TPckgBuf<TASShdAlarmedInstanceParams> paramsBuf;
	aStream >> paramsBuf;
	iLastAlarmedInstanceParams = paramsBuf();
	
	// Read two words reserved for future use
	aStream.ReadInt32L();
	aStream.ReadInt32L();	
	}
#endif

/**
 * Called when the status of one or more alarms changes
 */
void CASSrvAlarmQueue::HandleAlarmStatusChanged(TAlarmId aAlarmThatChangedStatus, TAlarmStatus aOldStatus)
	{
	// Resort the queue. We need to do this for alarms which may, for example,
	// have a repeat definition and was disabled and has been re-enabled but the
	// original expiry time has since passed.  The alarm's next due time has
	// been updated but the queue does not yet reflect this.
	TLinearOrder<TASSrvAlarm> order(ASSrvStaticUtils::CompareAlarms);
	iAlarms.Sort(order);

	// Called whenever an alarm changes state. We need to update the alarm timer.
	const TASSrvAlarm* alarm = QueueAlarmById(aAlarmThatChangedStatus);
	//
	const TAlarmId newHeadAlarmId = HeadAlarmId();
	const TAlarmId timerAlarmId = ServerData().Timer().NextDueAlarmId();
	//
	if	(timerAlarmId != newHeadAlarmId || aAlarmThatChangedStatus == timerAlarmId)
		{
		// The head item has changed.
		NotifyEvent(MASSrvAlarmQueueObserver::EASSrvAlarmQueueEventHeadItemChanged, newHeadAlarmId);
		}

	// Notify observers
	NotifyEvent(MASSrvAlarmQueueObserver::EASSrvAlarmQueueEventAlarmChanged, aAlarmThatChangedStatus);
	NotifyAlarmObserverEvent(MASSrvAlarmObserver::EAlarmObserverStatusChanged, *alarm, static_cast<TInt>(aOldStatus));

	// Notify change
	ServerData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventStatus, aAlarmThatChangedStatus);
	}

/**
 * Called when the state of one or more alarms changes
 */
void CASSrvAlarmQueue::HandleAlarmStateChanged(TAlarmId aAlarmThatChangedState, TAlarmState aOldState)
	{
	// Resort the queue. We need to do this for alarms which have now become snoozed or
	// alarms which were notified but are now re-queued because of their repeat definitions.
	TLinearOrder<TASSrvAlarm> order(ASSrvStaticUtils::CompareAlarms);
	iAlarms.Sort(order);

	// We need to update the alarm timer if the head alarm is different to
	// the old one.
	CASSrvAlarmTimer& timer = ServerData().Timer();
	//
	const TAlarmId newHeadAlarmId = HeadAlarmId();
	const TAlarmId timerAlarmId = timer.NextDueAlarmId();
	//
	if	(timerAlarmId != newHeadAlarmId)
		{
		// The head item has changed.
		NotifyEvent(MASSrvAlarmQueueObserver::EASSrvAlarmQueueEventHeadItemChanged, newHeadAlarmId);
		}
	else if (newHeadAlarmId != KNullAlarmId)
		{
		// It is possible for the head alarm to change it's due time.
		// In this case, the head alarm id is the same, but the times 
		// are different.
		const TASSrvAlarm* newHeadAlarm = QueueAlarmById(newHeadAlarmId);
		const TTime& timerDueTime = timer.NextDueAlarmOriginalExpiryTime();
		if	(timerAlarmId == newHeadAlarmId && timerDueTime != newHeadAlarm->NextDueTime())
			{
			// Alarm id hasn't changed, but the due time has
			NotifyEvent(MASSrvAlarmQueueObserver::EASSrvAlarmQueueEventHeadItemChanged, newHeadAlarmId);
			}
		}

	// Notify observers
	NotifyEvent(MASSrvAlarmQueueObserver::EASSrvAlarmQueueEventAlarmChanged, aAlarmThatChangedState);
	NotifyAlarmObserverEvent(MASSrvAlarmObserver::EAlarmObserverStateChanged, *QueueAlarmById(aAlarmThatChangedState), static_cast<TInt>(aOldState));

	// Notify change
	ServerData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventState, aAlarmThatChangedState);
	}

/**
 * Called when the state of one or more alarms changes
 */
void CASSrvAlarmQueue::HandleAlarmCharacteristicsChanged(TAlarmId aAlarmThatChangedCharacteristics, TAlarmCharacteristicsFlags /*aOldCharacteristics*/)
	{
	// Notify observers
	NotifyEvent(MASSrvAlarmQueueObserver::EASSrvAlarmQueueEventAlarmChanged, aAlarmThatChangedCharacteristics);
	/** 
	 * Alarm Observers don't currently need to know about this change.
	 * If this changes this is the place to do something like:
	 * NotifyAlarmObserverEvent(MASSrvAlarmObserver::EAlarmObserverCharacteristicsChanged, 
	 *     QueueAlarmById(aAlarmThatChangedCharacteristics),
	 *     static_cast<TInt>(aOldCharacteristics.Value()));
	 */

	// Notify change
	ServerData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventCharacteristics, aAlarmThatChangedCharacteristics);
	}

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
void CASSrvAlarmQueue::HandleWakeupChanged(TAlarmId aAlarmThatChangedWakeup)
	{
	// Notify observers
	NotifyEvent(MASSrvAlarmQueueObserver::EASSrvAlarmQueueEventAlarmChanged, aAlarmThatChangedWakeup);
	}
#endif

#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
void CASSrvAlarmQueue::HandleAlarmDaysChanged(TAlarmId aAlarmThatChangedDays)
	{
	// Check that the alarm has been queued (which we infer by the alarm having an id).
	if (aAlarmThatChangedDays == KNullAlarmId)
		{
		return;
		}
	
	// Resort the queue.
	TLinearOrder<TASSrvAlarm> order(ASSrvStaticUtils::CompareAlarms);
	iAlarms.Sort(order);

	// Get the id of the head alarm.
	const TAlarmId headAlarmId = HeadAlarmId();

	// Get the id of the alarm associated with the alarm timer.
	CASSrvAlarmTimer& timer = ServerData().Timer();
	const TAlarmId timerAlarmId = timer.NextDueAlarmId();

	// Assume the head item will not change.
	TBool headItemChanged = EFalse;
	
	if (headAlarmId != timerAlarmId)
		{
		headItemChanged = ETrue;
		}
	else
		{
		if (headAlarmId != KNullAlarmId)
			{
			const TASSrvAlarm* headAlarm = QueueAlarmById(headAlarmId);
			const TTime& timerDueTime = timer.NextDueAlarmOriginalExpiryTime();

			// It is possible for the head alarm to change its due time.  In
			// this case the alarm timer will no longer be correct.
			if (headAlarm->NextDueTime() != timerDueTime)
				{
				headItemChanged = ETrue;
				}
			}
		}
	
	if (headItemChanged)
		{
		NotifyEvent(MASSrvAlarmQueueObserver::EASSrvAlarmQueueEventHeadItemChanged, headAlarmId);
		}
	}
#endif

void CASSrvAlarmQueue::HandleAlarmDataChanged(TAlarmId aAlarmThatChangedData)
	{
	// Notify observers
	NotifyEvent(MASSrvAlarmQueueObserver::EASSrvAlarmQueueEventAlarmChanged, aAlarmThatChangedData);
	}

/**
 * Returns a boolean indicating whether or not the specified alarm id
 * is currently being used by another alarm.
 */
TBool CASSrvAlarmQueue::AlarmIdIsInUse(TAlarmId aAlarmId) const
	{
	const TInt count = QueueAlarmCount();
	for(TInt i=0; i<count; i++)
		{
		if	(QueueAlarmAt(i).Id() == aAlarmId)
			return ETrue;
		}
	return EFalse;
	}

/**
 * Remove any alarms from the queue that have notified already. Anything over 7 days old should be removed.
 */
void CASSrvAlarmQueue::RemoveDeadAlarms()
	{
	const TInt count = QueueAlarmCount();
	const TTime timeNow(ASSrvStaticUtils::UtcTimeNow());
	for(TInt i=count-1; i>=0; i--)
		{
		TASSrvAlarm& alarm = QueueAlarmAt(i);
		const TTimeIntervalDays daysSinceLastExpired(alarm.NextDueTime().DaysFrom(timeNow));
		if	(alarm.State() == EAlarmStateNotified && Abs(daysSinceLastExpired.Int()) >= KASSrvNumberOfDaysInOneWeek)
			alarm.DeQueue();
		}
	}

/**
 * Notify all alarm observers about the specified event.
 */
void CASSrvAlarmQueue::NotifyAlarmObserverEvent(MASSrvAlarmObserver::TObserverEvent aEvent, const TASSrvAlarm& aAlarm, TInt aEventSpecificData)
	{
	const TInt count = iAlarmObservers.Count();
	for(TInt i=0; i<count; i++)
		{
		TASSrvAlarmObserverMapplet& mapplet = iAlarmObservers[i];
		if	(mapplet.IsEnabled())
			mapplet.Observer().MASSrvAlarmObsHandleEvent(aEvent, aAlarm, aEventSpecificData);
		}
	}

/**
 * @see MASSrvAlarmInfoProvider
 */
TInt CASSrvAlarmQueue::ASSrvAlarmInfoCount() const
	{
	return QueueAlarmCount();
	}

/**
 * @see MASSrvAlarmInfoProvider
 */
void CASSrvAlarmQueue::ASSrvAlarmInfoAt(TInt aIndex, TASSrvAlarm& aAlarm) const
	{
	aAlarm = QueueAlarmAt(aIndex);
	}

/**
 * Register for notifications when the alarm queue order changes
 */
void CASSrvAlarmQueue::NotificationPoolChangeL(MASSrvAlarmQueueObserver& aObserver)
	{
	User::LeaveIfError(iNotificationList.InsertInAddressOrder(&aObserver));
	}

/**
 * Deregister for notifications when the alarm queue order changes
 */
void CASSrvAlarmQueue::NotificationPoolChangeCancel(MASSrvAlarmQueueObserver& aObserver)
	{
	TInt index = KErrNotFound;
	const TInt error = iNotificationList.FindInAddressOrder(&aObserver, index);
	if	(error != KErrNotFound)
		iNotificationList.Remove(index);
	}

/**
 * Return the number of alarms currently maintained within the alarm pool
 */
TInt CASSrvAlarmQueue::QueueAlarmCount() const
	{
	return iAlarms.Count();
	}

/**
 * Return a reference to the alarm at the specified reference
 */
TASSrvAlarm& CASSrvAlarmQueue::QueueAlarmAt(TInt aIndex)
	{
	return *iAlarms[aIndex];
	}

/**
 * Return a constant reference to the alarm at the specified index
 */
const TASSrvAlarm& CASSrvAlarmQueue::QueueAlarmAt(TInt aIndex) const
	{
	return *iAlarms[aIndex];
	}

/**
 * Return a copy of an alarm.
 */
TInt CASSrvAlarmQueue::QueueAlarmById(TAlarmId aId, TASSrvAlarm& aAlarm) const
	{
	const TInt count = iAlarms.Count();
	for(TInt i=0; i<count; i++)
		{
		const TASSrvAlarm& alarm = QueueAlarmAt(i);
		if	(alarm.Id() == aId)
			{
			aAlarm = alarm;
			return KErrNone;
			}
		}

	// No alarm available
	return KErrNotFound;
	}

/**
 * Returns a reference to a real alarm with the specified id.
 */
TASSrvAlarm* CASSrvAlarmQueue::QueueAlarmById(TAlarmId aId)
	{
	const TInt count = iAlarms.Count();
	for(TInt i=0; i<count; i++)
		{
		if	(iAlarms[i]->Id() == aId)
			return iAlarms[i];
		}
	
	return NULL;
	}

/**
 * Returns a constant reference to a real alarm with the specified id.
 */
const TASSrvAlarm* CASSrvAlarmQueue::QueueAlarmById(TAlarmId aId) const
	{
	const TInt count = iAlarms.Count();
	for(TInt i=0; i<count; i++)
		{
		if	(iAlarms[i]->Id() == aId)
			return iAlarms[i];
		}
		
	return NULL;
	}

/**
 * Returns a reference to a real alarm with the specified id.
 */
TASSrvAlarm& CASSrvAlarmQueue::QueueAlarmByIdL(TAlarmId aId)
	{
	TASSrvAlarm* alarm = QueueAlarmById(aId);
	if (!alarm)
		{
		User::Leave(KErrNotFound);
		}
	return *alarm;
	}

/**
 * Returns a constant reference to a real alarm with the specified id.
 */
const TASSrvAlarm& CASSrvAlarmQueue::QueueAlarmByIdL(TAlarmId aId) const
	{
	const TASSrvAlarm* alarm = QueueAlarmById(aId);
	if (!alarm)
		{
		User::Leave(KErrNotFound);
		}
	return *alarm;
	}

/**
 * Notify observers about an event
 */
void CASSrvAlarmQueue::NotifyEvent(MASSrvAlarmQueueObserver::TASSrvAlarmQueueEvent aEvent, TAlarmId aAlarmId)
	{
	const TInt count = iNotificationList.Count();
	for(TInt i=0; i<count; i++)
		{
		iNotificationList[i]->MAlarmQueueObserverHandleEvent(aEvent, aAlarmId);
		}
	}

/**
 * Replace one queue with another
 */
void CASSrvAlarmQueue::ReplaceQueueWithInternalizedQueue()
	{
	const TInt count = iAlarms.Count();
	for(TInt i=0; i<count; i++)
		{
		// If there was a notification request then we complete it
		iAlarms[i]->CancelSessionAlarm();
		}

	// free memory used by old queue
	iAlarms.ResetAndDestroy();

	// assign from new buffer
	iAlarms = iInternalizeAlarmQueue;

	// re-initialise source pointer array, by re-running its constructor
	new(&iInternalizeAlarmQueue) RPointerArray<TASSrvAlarm>;
	}

/**
 * Returns ETrue if there is at least one alarm in the "waiting to notify
 * state"
 */
TBool CASSrvAlarmQueue::HaveAdditionalAlarmsToNotify()
	{
	// Create & open primary iterator
	RASSrvIteratorByState primaryIterator(*this, EAlarmStateWaitingToNotify);
	primaryIterator.Open();

	// Create and attach secondary iterator
	RASSrvIteratorByStatus secondaryIterator(*this, EAlarmStatusEnabled);
	primaryIterator.IteratorAttach(secondaryIterator);

	return primaryIterator.NextAlarmAvailable();
	}

/**
 * Returns the number of alarms which are enabled and in the "waiting to
 * notify state"
 */
TInt CASSrvAlarmQueue::NumberOfAlarmsPendingNotification()
	{
	// Create & open primary iterator
	RASSrvIteratorByState primaryIterator(*this, EAlarmStateWaitingToNotify);
	primaryIterator.Open();

	// Create and attach secondary iterator
	RASSrvIteratorByStatus secondaryIterator(*this, EAlarmStatusEnabled);
	primaryIterator.IteratorAttach(secondaryIterator);

	TInt count = 0;
	while(primaryIterator.NextAlarmAvailable())
		{
		primaryIterator.NextAlarm();
		++count;
		}

	return count;
	}

/**
 * Return a reference to the alarm which is next awaiting notification
 */	
TASSrvAlarm& CASSrvAlarmQueue::NextAlarmWaitingForNotification()
	{
	// Create & open primary iterator
	RASSrvIteratorByState primaryIterator(*this, EAlarmStateWaitingToNotify);
	primaryIterator.Open();

	// Create and attach secondary iterator
	RASSrvIteratorByStatus secondaryIterator(*this, EAlarmStatusEnabled);
	primaryIterator.IteratorAttach(secondaryIterator);

	// The most recent alarm is at the head of the queue.
	// This will be the next one awaiting notification.
	TAlarmId idOfFirstAlarm = KNullAlarmId;
	if(primaryIterator.NextAlarmAvailable())
		{
		idOfFirstAlarm = primaryIterator.NextAlarm().Id();
		}
	
	__ASSERT_ALWAYS(idOfFirstAlarm != KNullAlarmId, ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicIteratorAlarmIdNull));
	return *QueueAlarmById(idOfFirstAlarm);
	}
