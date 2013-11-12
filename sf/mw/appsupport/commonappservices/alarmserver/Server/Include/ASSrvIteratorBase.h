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

#ifndef __ASSRVITERATORBASE_H__
#define __ASSRVITERATORBASE_H__

// System includes
#include <e32base.h>

// User includes
#include "ASSrvAlarm.h"

// Classes referenced
class CASSrvAlarmQueue;


//
// ----> RASSrvIteratorBase (header)
//
/**
 * The base class for server iterators
 */
class RASSrvIteratorBase
	{
//
public:										// CONSTRUCT
//

	/**
	 * Constructor
	 */
	RASSrvIteratorBase(CASSrvAlarmQueue& aQueue);

	/**
	 * Only the primary iterator should ever be opened.
	 */
	void									Open();

//
public:										// ACCESS
//

	/**
	 * Is another alarm available?
	 */
	virtual TBool							NextAlarmAvailable() const;

	/**
	 * Return the next available alarm
	 */
	virtual TASSrvAlarm&					NextAlarm();

	/**
	 * Resets the iterator back to the start of the queue
	 */
	virtual void							Reset();

	/**
	 * Attaches another iterator to this one. Allows iterators to be chained
	 * together so that different conditions can be satisfied without complex
	 * (custom-written) individual iterator combinations.
	 */
	virtual void							IteratorAttach(RASSrvIteratorBase& aIterator);

	/**
	 * Release a previously attached iterator
	 */
	virtual void							IteratorRelease();

//
protected:									// INTERNAL
//

	/**
	 * Is an iterator attached?
	 */
	TBool									IteratorAttached() const;

	/**
	 * Access the attached iterator
	 */
	RASSrvIteratorBase&						Iterator();

	/**
	 * Access a constant handle to the attached iterator
	 */
	const RASSrvIteratorBase&				Iterator() const;

	/**
	 * Synchronise this iterator with another
	 */
	virtual void							IteratorSynchronize(RASSrvIteratorBase& aInterator);

	/**
	 * Does the specified alarm satisfy the criteria associated with
	 * this iterator?
	 */
	virtual TBool							Matches(const TASSrvAlarm& aAlarm) const;

//
private:									// INTERNAL
//

	/**
	 * Return a reference to the alarm at the specified index
	 */
	TASSrvAlarm&							AlarmAt(TInt aIndex);

	/**
	 * Return the number of alarms
	 */
	TInt									AlarmCount() const;

//
private:									// INTERNAL ENUMERATIONS
//

	/**
	 * The type of iterator
	 */
	enum TASSrvIteratorType
		{
		/**
		 * The secondary iterator which is only called (to check for matches)
		 * when the primary iterator identifies a matching alarm.
		 */
		EASSrvIteratorTypeSecondary			= 0,

		/**
		 * The main iterator which is first checked, before
		 * passing onto the child.
		 */
		EASSrvIteratorTypePrimary
		};

	/**
	 * Iterator panic
	 */
	enum TASSrvIteratorPanic
		{
		/**
		 * The iterator has reached the end of the alarm
		 * queue unexpectedly.
		 */
		EASSrvIteratorPanicNoNextAlarmForIterator = 0,

		/**
		 * An iterator is attempting to add itself as
		 * a secondary iterator.
		 */
		EASSrvIteratorPanicCyclicAttach,

		/**
		 * No secondary iterator is attached, but it 
		 * is expected to be so.
		 */
		EASSrvIteratorPanicNoneAttached,

		/**
		 * An iterator isn't correctly initialized.
		 */
		EASSrvIteratorPanicNotInitialized,

		/**
		 * The iterator type isn't correct
		 */
		EASSrvIteratorPanicTypeMismatch

		};

	/**
	 * Panic the Alarm Server
	 */
	static void								Panic(TASSrvIteratorPanic aPanic);

//
protected:									// MEMBER DATA
//

	/**
	 * The queue
	 */
	CASSrvAlarmQueue&						iQueue;

	/**
	 * The index of the current alarm which the iterator is
	 * pointing to.
	 */
	TInt									iCurrentIndex;

	/**
	 * The type of iterator
	 */
	TASSrvIteratorType						iType;

	/**
	 * A pointer to a secondary iterator. Not always initialised.
	 */
	RASSrvIteratorBase*						iAttachedIterator;
	};

#endif
