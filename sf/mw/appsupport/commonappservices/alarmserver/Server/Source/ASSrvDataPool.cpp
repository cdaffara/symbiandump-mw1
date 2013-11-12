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

#include "ASSrvDataPool.h"

// System includes

// User includes
#include "ASSrvAlarmQueue.h"
#include "ASSrvStaticUtils.h"
#include "ASSrvServerWideData.h"

// Type definitions

// Constants

// Enumerations

// Classes referenced






//
// ----> CASSrvAlarmDataMapplet (header)
//

//*************************************************************************************
CASSrvDataPool::CASSrvAlarmDataMapplet::CASSrvAlarmDataMapplet(TAlarmId aAlarmId, HBufC8* aAlarmData)
:	iAlarmId(aAlarmId), iAlarmData(aAlarmData)
	{
	}


//*************************************************************************************
CASSrvDataPool::CASSrvAlarmDataMapplet::~CASSrvAlarmDataMapplet()
	{
	delete iAlarmData;
	}


//*************************************************************************************
CASSrvDataPool::CASSrvAlarmDataMapplet* CASSrvDataPool::CASSrvAlarmDataMapplet::NewLC(TAlarmId aAlarmId, HBufC8* aAlarmData)
	{
	__ASSERT_ALWAYS(aAlarmData, ASSrvStaticUtils::Fault(ASSrvStaticUtils::EASSrvFaultNoData));
	//
	CleanupStack::PushL(aAlarmData);
	CASSrvAlarmDataMapplet* self = new(ELeave) CASSrvAlarmDataMapplet(aAlarmId, aAlarmData);
	CleanupStack::Pop(aAlarmData);
	CleanupStack::PushL(self);
	return self;
	}











//
// ----> CASSrvDataPool (source)
//

//*************************************************************************************
CASSrvDataPool::CASSrvDataPool(CASSrvServerWideData& aServerWideData)
:	iServerWideData(aServerWideData)
	{
	}


//*************************************************************************************
CASSrvDataPool::~CASSrvDataPool()
	{
	iDataEntries.ResetAndDestroy();
	iDataEntries.Close();
	if(iInternalizeDataEntries.Count() != 0)
		iInternalizeDataEntries.ResetAndDestroy();
	iInternalizeDataEntries.Close();
	ServerData().Queue().NotificationPoolChangeCancel(*this);
	}


//*************************************************************************************
void CASSrvDataPool::ConstructL()
	{
	ServerData().Queue().NotificationPoolChangeL(*this);
	}


//*************************************************************************************
CASSrvDataPool* CASSrvDataPool::NewL(CASSrvServerWideData& aServerWideData)
	{
	CASSrvDataPool* self = new(ELeave) CASSrvDataPool(aServerWideData);
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
 * Does the specified alarm have any associated arbitrary data?
 */
TBool CASSrvDataPool::DataPoolContainsEntry(TAlarmId aAlarmId) const
	{
	return (FindEntry(aAlarmId) != KErrNotFound);
	}


//*************************************************************************************
/**
 * Return the associated arbitrary data for the specified alarm.
 */
const TDesC8& CASSrvDataPool::DataPoolEntry(TAlarmId aAlarmId) const
	{
	const TInt index = FindEntry(aAlarmId);
	__ASSERT_ALWAYS(index >= 0, ASSrvStaticUtils::Fault(ASSrvStaticUtils::EASSrvFaultAlarmDataEntryNotFound));
	return iDataEntries[index]->Data();
	}


//*************************************************************************************
/**
 * Add a new data mapping to the pool. 
 */
void CASSrvDataPool::DataPoolAddDataL(TAlarmId aAlarmId, HBufC8* aData)
	{
	CleanupStack::PushL(aData);
	const TInt index = FindEntry(aAlarmId);
	if	(index < KErrNone)
		{
		CleanupStack::Pop(aData);
		CASSrvAlarmDataMapplet* mapplet = CASSrvAlarmDataMapplet::NewLC(aAlarmId, aData);
		User::LeaveIfError(iDataEntries.Append(mapplet));
		CleanupStack::Pop(mapplet);
		ServerData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventAlarmData, aAlarmId);
		}
	else
		User::Leave(KErrInUse);
	}


//*************************************************************************************
/**
 * Release the data associated with the specified entry
 */
void CASSrvDataPool::DataPoolRemoveDataL(TAlarmId aAlarmId)
	{
	const TInt index = FindEntry(aAlarmId);
	if	(index < KErrNone)
		User::Leave(index);
	//
	delete iDataEntries[index];
	iDataEntries.Remove(index);
	//
	ServerData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventAlarmData, aAlarmId);
	}


//
//
//


//*************************************************************************************
/**
 * Internalize the contents of the datapool from a stream
 */
void CASSrvDataPool::InternalizeL(RReadStream& aStream)
	{
	const TInt count = aStream.ReadInt32L();
	if(iInternalizeDataEntries.Count() != 0)
		iInternalizeDataEntries.ResetAndDestroy();
	//
	for(TInt i=0; i<count; i++)
		{
		const TAlarmId id = aStream.ReadInt32L();
		HBufC8* data = HBufC8::NewLC(aStream, KMaxTInt);
		//
		CASSrvAlarmDataMapplet* mapplet = CASSrvAlarmDataMapplet::NewLC(id, data);
		CleanupStack::Pop(2, data);
		CleanupStack::PushL(mapplet);
		//
		User::LeaveIfError(iInternalizeDataEntries.Append(mapplet));
		CleanupStack::Pop(mapplet);
		}
	}


//*************************************************************************************
/**
 * Externalize the contents of the datapool to a stream
 */
void CASSrvDataPool::ExternalizeL(RWriteStream& aStream) const
	{
	const TInt count = iDataEntries.Count();
	aStream.WriteInt32L(count);
	//
	for(TInt i=0; i<count; i++)
		{
		const CASSrvAlarmDataMapplet& mapplet = *iDataEntries[i];
		aStream.WriteInt32L(mapplet.Id());
		aStream << mapplet.Data();
		}
	}


//*************************************************************************************
/**
 * Whether to use the Internalize buffer, or throw it away
 */
void CASSrvDataPool::ApplyInternalizedData(TBool aUseNewData)
	{
	if (aUseNewData)
		{
		// Internalize Success

		// free old entries
		iDataEntries.ResetAndDestroy();

		// new data pool entries
		iDataEntries = iInternalizeDataEntries;

		// re-initialise source pointer array, by re-running its constructor
		new(&iInternalizeDataEntries) RPointerArray<CASSrvAlarmDataMapplet>;
		}
	else 
		{
		// Internalize Failure
		iInternalizeDataEntries.ResetAndDestroy();
		}
	}


//
//
//


//*************************************************************************************
/**
 * @see MASSrvAlarmQueueObserver
 */
void CASSrvDataPool::MAlarmQueueObserverHandleEvent(TASSrvAlarmQueueEvent aEvent, TAlarmId aAlarmId)
	{
	// If an alarm is deleted, then we must also remove any associated 
	// arbitrary data.
	if	(aEvent == EASSrvAlarmQueueEventAlarmDeleted && DataPoolContainsEntry(aAlarmId))
		{
		TRAP_IGNORE(DataPoolRemoveDataL(aAlarmId));
		}
	}


//
//
//


//*************************************************************************************
/**
 * Find a specific data pool entry based upon an alarm id
 */
TInt CASSrvDataPool::FindEntry(TAlarmId aAlarmId) const
	{
	const TInt count = iDataEntries.Count();
	for(TInt i=0; i<count; i++)
		{
		if	(iDataEntries[i]->Id() == aAlarmId)
			return i;
		}
	return KErrNotFound;
	}

