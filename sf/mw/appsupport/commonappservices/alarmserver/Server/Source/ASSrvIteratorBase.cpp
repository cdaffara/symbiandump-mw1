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

#include "ASSrvIteratorBase.h"

// System includes

// User includes
#include "ASSrvAlarmQueue.h"

// Type definitions

// Constants
const TInt KInitialIndexValue = -1;

// Enumerations

// Classes referenced


//
// ----> RASSrvIteratorBase (source)
//

//*************************************************************************************
RASSrvIteratorBase::RASSrvIteratorBase(CASSrvAlarmQueue& aQueue)
:	iQueue(aQueue), iCurrentIndex(KInitialIndexValue), iType(EASSrvIteratorTypeSecondary), iAttachedIterator(NULL)
	{
	}


//*************************************************************************************
/**
 * Only the primary iterator should ever be opened.
 */
void RASSrvIteratorBase::Open()
	{
	iType = EASSrvIteratorTypePrimary;

	// Restore back to default state
	Reset();
	}


//
//
//


//*************************************************************************************
/**
 * Is another alarm available?
 */
TBool RASSrvIteratorBase::NextAlarmAvailable() const
	{
	RASSrvIteratorBase& self = const_cast<RASSrvIteratorBase&>(*this);
	const TInt count = AlarmCount();
	TInt index = iCurrentIndex;
	//
	while(++index < count)
		{
		const TASSrvAlarm& alarm = self.AlarmAt(index);
		if	(Matches(alarm))
			return ETrue;
		}
	return EFalse;
	}


//*************************************************************************************
/**
 * Return the next available alarm
 */
TASSrvAlarm& RASSrvIteratorBase::NextAlarm()
	{
	const TInt count = AlarmCount();
	while (++iCurrentIndex < count)
	{
		TASSrvAlarm& alarm = AlarmAt(iCurrentIndex);
		if	(Matches(alarm))
			{
			IteratorSynchronize(*this);
			return alarm;
			}
		}
	
	// Panic because there isn't another available alarm.
	Panic(EASSrvIteratorPanicNoNextAlarmForIterator);
	TASSrvAlarm* dummy = NULL;
	return *dummy;
	}


//*************************************************************************************
/**
 * Resets the iterator back to the start of the queue
 */
void RASSrvIteratorBase::Reset()
	{
	iCurrentIndex = KInitialIndexValue;
	IteratorSynchronize(*this);
	}


//*************************************************************************************
/**
 * Attaches another iterator to this one. Allows iterators to be chained
 * together so that different conditions can be satisfied without complex
 * (custom-written) individual iterator combinations.
 */
void RASSrvIteratorBase::IteratorAttach(RASSrvIteratorBase& aIterator)
	{
	__ASSERT_ALWAYS(&aIterator != this, Panic(EASSrvIteratorPanicCyclicAttach));
	iAttachedIterator = &aIterator;
	}


//*************************************************************************************
/**
 * Release a previously attached iterator
 */
void RASSrvIteratorBase::IteratorRelease()
	{
	iAttachedIterator = NULL;
	}


//
//
//


//*************************************************************************************
/**
 * Is an iterator attached?
 */
TBool RASSrvIteratorBase::IteratorAttached() const
	{
	return (iAttachedIterator != NULL);
	}


//*************************************************************************************
/**
 * Access the attached iterator
 */
RASSrvIteratorBase& RASSrvIteratorBase::Iterator()
	{
	__ASSERT_ALWAYS(IteratorAttached(), Panic(EASSrvIteratorPanicNoneAttached));
	return *iAttachedIterator;
	}


//*************************************************************************************
/**
 * Access a constant handle to the attached iterator
 */
const RASSrvIteratorBase& RASSrvIteratorBase::Iterator() const
	{
	__ASSERT_ALWAYS(IteratorAttached(), Panic(EASSrvIteratorPanicNoneAttached));
	return *iAttachedIterator;
	}


//*************************************************************************************
/**
 * Synchronise this iterator with another
 */
void RASSrvIteratorBase::IteratorSynchronize(RASSrvIteratorBase& aInterator)
	{
	iCurrentIndex = aInterator.iCurrentIndex;
	if	(IteratorAttached())
		Iterator().IteratorSynchronize(*this);
	}


//*************************************************************************************
/**
 * Does the specified alarm satisfy the criteria associated with
 * this iterator?
 */
TBool RASSrvIteratorBase::Matches(const TASSrvAlarm& aAlarm) const
	{
	// If we have an attached iterator, then we should
	// check with that too.
	TBool matches = ETrue;
	//
	if	(IteratorAttached())
		matches = Iterator().Matches(aAlarm);
	//
	return matches;
	}


//*************************************************************************************
/**
 * Return a reference to the alarm at the specified index
 */
TASSrvAlarm& RASSrvIteratorBase::AlarmAt(TInt aIndex)
	{
	__ASSERT_ALWAYS(iType == EASSrvIteratorTypePrimary, Panic(EASSrvIteratorPanicTypeMismatch));
	return iQueue.QueueAlarmAt(aIndex);
	}


//*************************************************************************************
/**
 * Return the number of alarms
 */
TInt RASSrvIteratorBase::AlarmCount() const
	{
	__ASSERT_ALWAYS(iType == EASSrvIteratorTypePrimary, Panic(EASSrvIteratorPanicTypeMismatch));
	return iQueue.QueueAlarmCount();
	}


//
//
//


//*************************************************************************************
/**
 * Panic the Alarm Server
 */
void RASSrvIteratorBase::Panic(TASSrvIteratorPanic aPanic)
	{
	_LIT(KPanicCategory, "ASSrvIterator");
	User::Panic(KPanicCategory, aPanic);
	}




