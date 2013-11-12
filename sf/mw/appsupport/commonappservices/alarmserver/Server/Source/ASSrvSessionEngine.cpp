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

#include "ASSrvSessionEngine.h"

// System includes

// User includes
#include "ASSrvAlarmStore.h"
#include "ASSrvAlarmQueue.h"
#include "ASSrvStaticUtils.h"
#include "ASSrvServerWideData.h"
#include "ASSrvAnyEventManager.h"
#include "ASSrvIteratorByState.h"
#include "ASSrvIteratorByStatus.h"
#include "ASSrvSessionCollection.h"
#include "ASSrvIteratorByCategory.h"
#include "ASSrvIteratorBySessionId.h"

// Type definitions

// Constants
const TAlarmCategory KCalendarCategeory = {0x101F4A70};

// Enumerations

// Classes referenced


//
// ----> CASSrvSessionEngine (source)
//

//*************************************************************************************
CASSrvSessionEngine::CASSrvSessionEngine(CASSrvServerWideData& aServerWideData, MASSrvAnyEventObserver& aChangeObserver, MASSrvSession& aSession)
:	iServerWideData(aServerWideData), iChangeObserver(aChangeObserver), iSession(aSession)
	{
	}


//*************************************************************************************
CASSrvSessionEngine::~CASSrvSessionEngine()
	{
	ServerData().SessionCollection().MASSessionCollectionDetach(*this);
	ServerData().AnyEventManager().MASAnyEventManagerObserverRemove(*this);
	}


//*************************************************************************************
void CASSrvSessionEngine::ConstructL()
	{
	ServerData().AnyEventManager().MASAnyEventManagerObserverAddL(*this);
	iSessionId = ServerData().SessionCollection().MASSessionCollectionAttachL(*this);
	}


//*************************************************************************************
CASSrvSessionEngine* CASSrvSessionEngine::NewL(CASSrvServerWideData& aServerWideData, MASSrvAnyEventObserver& aChangeObserver, MASSrvSession& aSession)
	{
	CASSrvSessionEngine* self = new(ELeave) CASSrvSessionEngine(aServerWideData, aChangeObserver, aSession);
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
 * @see MASSrvAnyEventObserver
 *
 * Facade interface that notifies observer about event. No real processing done
 * by this object.
 */
void CASSrvSessionEngine::MASSrvAnyEventHandleChange(TAlarmChangeEvent aEvent, TAlarmId aAlarmId)
	{
	iChangeObserver.MASSrvAnyEventHandleChange(aEvent, aAlarmId);
	}


//
//
//


//*************************************************************************************
/**
 * @see MASSrvSession
 *
 * Facade to the real session
 */
TASSrvSessionId CASSrvSessionEngine::MASSrvSessionId() const
	{
	return iSessionId;
	}


//*************************************************************************************
/**
 * @see MASSrvSession
 *
 * Facade to the real session
 */
void CASSrvSessionEngine::MASSrvSessionFullName(TDes& aDes) const
	{
	iSession.MASSrvSessionFullName(aDes);
	}


//
//
//


//*************************************************************************************
/**
 * Add an alarm to the alarm server
 */
void CASSrvSessionEngine::AlarmAddL(TASSrvAlarm& aAlarm, TAlarmId aSpecificAlarmId)
	{
	// Must clear this - all alarms will have their Id's allocated later
	aAlarm.Id() = KNullAlarmId;	
	
	// clear associateddata, session and state flags
	aAlarm.ClearFlags();

	// Check the alarm
	const TInt error = ASSrvStaticUtils::ValidateAlarm(aAlarm);
	User::LeaveIfError(error);

	// Assign session id
	aAlarm.SetOriginatingSessionId(MASSrvSessionId());

	// Reset alarm state
	aAlarm.SetState(EAlarmStateInPreparation);

	// Add the alarm to the queue. This will update the timer if this
	// new alarm is now the next alarm.
	ServerData().Queue().QueueAlarmAndAllocateIdL(aAlarm, aSpecificAlarmId);
	}


//*************************************************************************************
/**
 * Return information about an alarm
 */
void CASSrvSessionEngine::AlarmDetailsL(TAlarmId aAlarmId, TASSrvAlarm& aAlarm) const
	{
	// Fetch the alarm details if the specified alarm exists.
	User::LeaveIfError(ServerData().Queue().QueueAlarmById(aAlarmId, aAlarm));
	}


//*************************************************************************************
/**
 * Delete an alarm from the alarm server
 */
void CASSrvSessionEngine::AlarmDeleteL(TAlarmId aAlarmId)
	{
	TASSrvAlarm alarm(ServerData());
	User::LeaveIfError(ServerData().Queue().QueueAlarmById(aAlarmId, alarm));

	// If the alarm has a notification request outstanding against
	// it, then we don't allow it to be deleted (the only way of
	// removing the alarm is by cancelling the session notification).
	if	(alarm.HasNotificationRequestPending())
		User::Leave(KErrAccessDenied);
	ServerData().Queue().DeQueueAlarm(alarm);
	}


//*************************************************************************************
/**
 * Return the category of a given alarm
 */
TAlarmCategory CASSrvSessionEngine::AlarmCategoryL(TAlarmId aAlarmId) const
	{
	TASSrvAlarm alarm(ServerData());
	User::LeaveIfError(ServerData().Queue().QueueAlarmById(aAlarmId, alarm));
	return alarm.Category();
	}


//*************************************************************************************
/**
 * Set the status of an alarm
 */
void CASSrvSessionEngine::SetAlarmStatusL(TAlarmId aAlarmId, TAlarmStatus aStatus)
	{
	CASSrvAlarmQueue& queue = ServerData().Queue();
	TASSrvAlarm& alarm = queue.QueueAlarmByIdL(aAlarmId);
	User::LeaveIfError(alarm.SetStatus(aStatus));
	}

//*************************************************************************************

void CASSrvSessionEngine::SetAlarmStatusForCalendarFileL(const TDesC& aCalendarFileName, TAlarmStatus aStatus)
    {
    CASSrvAlarmQueue& queue = ServerData().Queue();
    
    RArray<TAlarmId>* calendarAlarmIds = AlarmIdListByCategoryLC(KCalendarCategeory);
    const TInt KCalendarAlarmCount(calendarAlarmIds->Count());
    
    for (TInt i(0) ; i < KCalendarAlarmCount ; ++i)
        {
        TASSrvAlarm* alarm = queue.QueueAlarmById((*calendarAlarmIds)[i]);
                
        const TAgnAlarmInfo* alarmInfo = reinterpret_cast<const TAgnAlarmInfo*>(alarm->DataL().Ptr());
        TBool correctFile = (alarmInfo->iFileName.CompareF(aCalendarFileName) == 0);
        if(correctFile)
            {
            alarm->SetStatus(aStatus);
            }
        }
     CleanupStack::PopAndDestroy(calendarAlarmIds);
     // coverity[leaked_storage]
    }

//*************************************************************************************
/**
 * Return the status of a given alarm
 */
TAlarmStatus CASSrvSessionEngine::AlarmStatusL(TAlarmId aAlarmId) const
	{
	TASSrvAlarm alarm(ServerData());
	User::LeaveIfError(ServerData().Queue().QueueAlarmById(aAlarmId, alarm));
	return alarm.Status();
	}


//*************************************************************************************
/**
 * Set the day/timed status of an alarm
 */
void CASSrvSessionEngine::SetAlarmDayOrTimedL(TAlarmId aAlarmId, TAlarmDayOrTimed aDayOrTimed)
	{
	CASSrvAlarmQueue& queue = ServerData().Queue();
	TASSrvAlarm& alarm = queue.QueueAlarmByIdL(aAlarmId);
	alarm.DayOrTimed() = aDayOrTimed;
	}


//*************************************************************************************
/**
 * Return the day/timed status of a given alarm
 */
TAlarmDayOrTimed CASSrvSessionEngine::AlarmDayOrTimedL(TAlarmId aAlarmId) const
	{
	TASSrvAlarm alarm(ServerData());
	User::LeaveIfError(ServerData().Queue().QueueAlarmById(aAlarmId, alarm));
	return alarm.DayOrTimed();
	}


//*************************************************************************************
/**
 * Set the characteristics of an alarm
 */
void CASSrvSessionEngine::SetAlarmCharacteristicsL(TAlarmId aAlarmId, TAlarmCharacteristicsFlags aCharacteristics)
	{
	CASSrvAlarmQueue& queue = ServerData().Queue();
	TASSrvAlarm& alarm = queue.QueueAlarmByIdL(aAlarmId);
	alarm.SetCharacteristicsL(aCharacteristics, MASSrvSessionId());
	}

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
void CASSrvSessionEngine::SetWakeupL(TAlarmId aAlarmId, TBool aEnable)
	{
	CASSrvAlarmQueue& queue = ServerData().Queue();
	TASSrvAlarm& alarm = queue.QueueAlarmByIdL(aAlarmId);
	alarm.SetWakeupAndNotifyQueueL(aEnable);
	}
#endif

#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
void CASSrvSessionEngine::SetAlarmDaysL(TAlarmId aAlarmId, TUint8 aAlarmDays)
	{
	CASSrvAlarmQueue& queue = ServerData().Queue();
	TASSrvAlarm& alarm = queue.QueueAlarmByIdL(aAlarmId);
	alarm.SetAlarmDaysL(aAlarmDays);
	}

TUint8 CASSrvSessionEngine::AlarmDaysL(TAlarmId aAlarmId) const
	{
	TASSrvAlarm alarm(ServerData());
	User::LeaveIfError(ServerData().Queue().QueueAlarmById(aAlarmId, alarm));
	return alarm.AlarmDays();
	}

void CASSrvSessionEngine::SetContinuousL(TAlarmId aAlarmId, TBool aContinuous)
	{
	CASSrvAlarmQueue& queue = ServerData().Queue();
	TASSrvAlarm& alarm = queue.QueueAlarmByIdL(aAlarmId);
	alarm.SetContinuous(aContinuous);
	}

TBool CASSrvSessionEngine::ContinuousL(TAlarmId aAlarmId) const
	{
	TASSrvAlarm alarm(ServerData());
	User::LeaveIfError(ServerData().Queue().QueueAlarmById(aAlarmId, alarm));
	return alarm.Continuous();
	}

#endif

void CASSrvSessionEngine::SetAlarmOrphanedL(TAlarmId aAlarmId)
	{
	CASSrvAlarmQueue& queue = ServerData().Queue();
	TASSrvAlarm& alarm = queue.QueueAlarmByIdL(aAlarmId);
	alarm.SetAlarmOrphaned();
	}


//*************************************************************************************
/**
 * Return the characteristics of an alarm
 */
TAlarmCharacteristicsFlags CASSrvSessionEngine::AlarmCharacteristicsL(TAlarmId aAlarmId) const
	{
	TASSrvAlarm alarm(ServerData());
	User::LeaveIfError(ServerData().Queue().QueueAlarmById(aAlarmId, alarm));
	return alarm.Characteristics();
	}


//
//
//

//*************************************************************************************
void CASSrvSessionEngine::AlarmDataAttachL(TAlarmId aAlarmId, HBufC8* aData)
	{
	// Give ownership of the data to the alarm. This will leave with 
	// KErrInUse should the specified alarm already have attached data.
	TASSrvAlarm& alarm = ServerData().Queue().QueueAlarmByIdL(aAlarmId);
	alarm.DataAttachL(aData);
	}


//*************************************************************************************
void CASSrvSessionEngine::AlarmDataDetachL(TAlarmId aAlarmId)
	{
	// Remove the data from the pool - if the specified alarm doesn't exist
	// within the data pool, then this will leave with KErrNotFound.
	TASSrvAlarm& alarm = ServerData().Queue().QueueAlarmByIdL(aAlarmId);
	alarm.DataDetachL();	
	}



//*************************************************************************************
/**
 * Change all alarms of a given category to have the specified status
 */
void CASSrvSessionEngine::SetAlarmStatusByCategoryL(TAlarmCategory aCategory, TAlarmStatus aStatus)
	{
	//
	CASSrvAlarmQueue& queue = ServerData().Queue();
	RASSrvIteratorByCategory iterator(queue, aCategory);
	iterator.Open();
	//
	while(iterator.NextAlarmAvailable())
		{
		TASSrvAlarm* alarm = queue.QueueAlarmById(iterator.NextAlarm().Id());
		User::LeaveIfError(alarm->SetStatus(aStatus));
		}
	}


//*************************************************************************************
/**
 * Return the number of alarms within the alarm server queue that
 * have the specified category identifier.
 */
TInt CASSrvSessionEngine::AlarmCountByCategory(TAlarmCategory aCategory) const
	{
	TInt count = 0;
	//
	RASSrvIteratorByCategory iterator(ServerData().Queue(), aCategory);
	iterator.Open();
	//
	while(iterator.NextAlarmAvailable())
		{
		iterator.NextAlarm();
		++count;
		}
	return count;
	}


//*************************************************************************************
/**
 * Return the number of alarms within the alarm server that 
 * have the specified state.
 */
TInt CASSrvSessionEngine::AlarmCountByState(TAlarmState aState) const
	{
	TInt count = 0;
	//
	RASSrvIteratorByState iterator(ServerData().Queue(), aState);
	iterator.Open();
	//
	while(iterator.NextAlarmAvailable())
		{
		iterator.NextAlarm();
		++count;
		}
	return count;
	}


//*************************************************************************************
/**
 * Delete all of the alarms within the alarm server queue which
 * match that of the specified category and alarm's state
 */
void CASSrvSessionEngine::DeleteAllAlarmsByCategoryL(TAlarmCategory aCategory, TBool aRestrictToOrphanedAlarms, TDeleteType aWhatToDelete)
 	{
	CASSrvAlarmQueue& queue = ServerData().Queue();
	RArray<TAlarmId>* alarmToDeleteIds = AlarmIdListByCategoryLC(aCategory);
	TInt count = alarmToDeleteIds->Count();
	//
	for (TInt i=0; i<count; ++i)
		{
		const TASSrvAlarm* alarm = queue.QueueAlarmById((*alarmToDeleteIds)[i]);
		const TBool isOrphanedAndDeletable = aRestrictToOrphanedAlarms && alarm->HasBecomeOrphaned();
		TBool toDelete=EFalse;
		if (!aRestrictToOrphanedAlarms || isOrphanedAndDeletable)
			{
			if(!aWhatToDelete)
				toDelete=ETrue;
			else
				{
				switch (alarm->State())
					{
					case EAlarmStateInPreparation:
						{
						toDelete=ETrue;
						break;
						}

					case EAlarmStateQueued:
						{
						if (aWhatToDelete&EFuture)
							{
							toDelete=ETrue;
							}
						break;
						}
					case EAlarmStateNotified:
						{
						if (aWhatToDelete&EExpired)
							{
							toDelete=ETrue;
							}
						break;
						}

					case EAlarmStateSnoozed:
					case EAlarmStateWaitingToNotify:
					case EAlarmStateNotifying:
						{
						if (aWhatToDelete&EActive)
							{
							toDelete=ETrue;
							}
						break;
						}

					default:
						__ASSERT_DEBUG(EFalse, ASSrvStaticUtils::Fault(ASSrvStaticUtils::EASSrvFaultAlarmStateNotHandled));
					break;
					}
				}
			 if (toDelete)
				{
				queue.DeQueueAlarm(*alarm);
				}
			}
		}
	alarmToDeleteIds->Close();
	CleanupStack::PopAndDestroy(alarmToDeleteIds);
	// coverity [leaked_storage]
	}

//*************************************************************************************
/**
 * Delete all of the alarms within the alarm server queue which
 * match that of the specified calendar file name and alarm's state
 */
void CASSrvSessionEngine::DeleteAllAlarmsByCalendarFileL(const TDesC& aCalendarFileName, TDeleteType aWhatToDelete)
    {
    RDebug::Print(_L("CASSrvSessionEngine::DeleteAllAlarmsByCalendarFileL"));
    
    CASSrvAlarmQueue& queue = ServerData().Queue();
    RArray<TAlarmId>* calendarAlarmIds = AlarmIdListByCategoryLC(KCalendarCategeory);
    const TInt KCalendarAlarmCount(calendarAlarmIds->Count());
    
    for (TInt i(0) ; i < KCalendarAlarmCount ; ++i)
        {
        const TASSrvAlarm* alarm = queue.QueueAlarmById((*calendarAlarmIds)[i]);
        TBool correctDeleteType(ETrue);
        
        const TAgnAlarmInfo* alarmInfo = reinterpret_cast<const TAgnAlarmInfo*>(alarm->DataL().Ptr());
        TBool correctFile = (alarmInfo->iFileName.CompareF(aCalendarFileName) == 0);
                
        if (aWhatToDelete && correctFile)
            {
            switch (alarm->State())
                {
                case EAlarmStateInPreparation:
                    {
                    correctDeleteType = ETrue;
                    break;
                    }
                case EAlarmStateQueued:
                    {
                    correctDeleteType = aWhatToDelete & EFuture;
                    break;
                    }
                case EAlarmStateNotified:
                    {
                    correctDeleteType = aWhatToDelete & EExpired;
                    break;
                    }
                case EAlarmStateSnoozed:
                case EAlarmStateWaitingToNotify:
                case EAlarmStateNotifying:
                    {
                    correctDeleteType = aWhatToDelete & EActive;
                    break;
                    }
                default:
                    {
                    __ASSERT_DEBUG(EFalse, ASSrvStaticUtils::Fault(ASSrvStaticUtils::EASSrvFaultAlarmStateNotHandled));
                    }
                    break;
                }
            }
        
         if (correctDeleteType && correctFile)
            {
            queue.DeQueueAlarm(*alarm);
            }
        }
    
    calendarAlarmIds->Close();
    CleanupStack::PopAndDestroy(calendarAlarmIds);
    // coverity [leaked_storage]
    }



//*************************************************************************************
/**
 * Return a list of all alarm categories currently in use within
 * the alarm server queue.
 */
RArray<TAlarmCategory>* CASSrvSessionEngine::AlarmCategoryListLC() const
	{
	TLinearOrder<TAlarmCategory> order(ASSrvStaticUtils::CompareCategories);
	CASSrvAlarmQueue& queue = ServerData().Queue();
	//
	RArray<TAlarmCategory>* list = new(ELeave) RArray<TAlarmCategory>(4);
	CleanupStack::PushL(TCleanupItem(ASSrvStaticUtils::CleanupCloseDeleteCategoryArray, list));
	//
	const TInt count = queue.QueueAlarmCount();
	for(TInt i=0; i<count; i++)
		{
		// Is this category already present in the list?
		const TAlarmCategory category(queue.QueueAlarmAt(i).Category());
		if	(list->FindInOrder(category, order) == KErrNotFound)
			{
			// Add this category to the list
			User::LeaveIfError(list->InsertInOrder(category, order));
			}
		}
	//
	return list;
	}


//*************************************************************************************
/**
 * Return a list of alarm ids for a given category
 */
RArray<TAlarmId>* CASSrvSessionEngine::AlarmIdListByCategoryLC(TAlarmCategory aCategory) const
	{
	CASSrvAlarmQueue& queue = ServerData().Queue();
	RASSrvIteratorByCategory iterator(queue, aCategory);
	iterator.Open();
	//coverity[return_alloc_fn]
	return AlarmIdListFromIteratorLC(iterator);
	}


//*************************************************************************************
/**
 * Return a list of alarm ids for a given alarm state
 */
RArray<TAlarmId>* CASSrvSessionEngine::AlarmIdListByStateLC(TAlarmState aState) const
	{
	CASSrvAlarmQueue& queue = ServerData().Queue();
	RASSrvIteratorByState iterator(queue, aState);
	iterator.Open();
	//
	return AlarmIdListFromIteratorLC(iterator);
	}


//*************************************************************************************
/**
 * Return all alarm ids currently used within the alarm server
 * queue.
 */
RArray<TAlarmId>* CASSrvSessionEngine::AlarmIdListLC() const
	{
	CASSrvAlarmQueue& queue = ServerData().Queue();
	RASSrvIteratorBase iterator(queue);
	iterator.Open();
	//
	return AlarmIdListFromIteratorLC(iterator);
	}


//*************************************************************************************
/**
 * Return the number of active alarms, that is, enabled, queued or snoozed 
 * alarms
 */
TInt CASSrvSessionEngine::NumberOfActiveAlarmsInQueue() const
	{
	CASSrvAlarmQueue& queue = ServerData().Queue();
	//
	RASSrvIteratorByStatus status(queue, EAlarmStatusEnabled);
	status.Open();
	//
	RASSrvIteratorByState state(queue, EAlarmStateQueued, EAlarmStateSnoozed);
	status.IteratorAttach(state);
	//
	TInt count = 0;
	while(status.NextAlarmAvailable())
		{
		++count;
		status.NextAlarm();
		}
	return count;
	}


//
//
//


//*************************************************************************************
/**
 * Return a list of alarm ids which is generated by navigating the specified iterator
 */
RArray<TAlarmId>* CASSrvSessionEngine::AlarmIdListFromIteratorLC(RASSrvIteratorBase& aIterator) const
	{
	RArray<TAlarmId>* list = new(ELeave) RArray<TAlarmId>(4);
	CleanupStack::PushL(TCleanupItem(ASSrvStaticUtils::CleanupCloseDeleteAlarmIdArray, list));
	//
	while(aIterator.NextAlarmAvailable())
		{
		const TASShdAlarm& alarm = aIterator.NextAlarm();
		list->AppendL(alarm.Id());
		}
	//
	return list;
	}


