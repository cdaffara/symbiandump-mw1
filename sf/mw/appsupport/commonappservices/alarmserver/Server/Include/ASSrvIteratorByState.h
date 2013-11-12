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

#ifndef __ASSRVITERATORBYSTATE_H__
#define __ASSRVITERATORBYSTATE_H__

// System includes
#include <e32base.h>

// User includes
#include "ASSrvIteratorBase.h"
#include "ASSrvDefs.h"

//
// ----> RASSrvIteratorByState (header)
//
/**
 * Locate alarms that have certain states
 */
class RASSrvIteratorByState : public RASSrvIteratorBase
	{
//
public:										// ENUMERATIONS
//
	/**
	 * The state criteria to use for selection
	 */
	enum TStateMatchType
		{
		/**
		 * Matching alarms must have the specified state
		 */
		EStateMustMatch = 0,

		/**
		 * Matching alarms must not have the specified state
		 */
		EStateMustNotMatch = 1,

		/**
		 * Match either the first specifier or the second (see
		 * constructor).
		 */
		EStateEitherOr = 2
		};

//
public:										// CONSTRUCT
//

	/**
	 * Constructor
	 *
	 * @param aQueue The alarm queue
	 * @param aState The state of matching alarms
	 * @param aMatchType The match criteria
	 */
	RASSrvIteratorByState(CASSrvAlarmQueue& aQueue, TAlarmState aState, TStateMatchType aMatchType = EStateMustMatch);

	/**
	 * Constructor
	 *
	 * @param aQueue The alarm queue
	 * @param aEither The first state of matching alarms
	 * @param aOr The alternative state for matching alarms
	 */
	RASSrvIteratorByState(CASSrvAlarmQueue& aQueue, TAlarmState aEither, TAlarmState aOr);

//
public:										// ACCESS
//

	/**
	 * Change the state for this iterator. Resets the iterator back to the start
	 * of the queue
	 */
	void									SetState(TAlarmState aState, TStateMatchType aMatchType = EStateMustMatch);

	/**
	 * Change the state for this iterator. Resets the iterator back to the start
	 * of the queue
	 */
	void									SetStates(TAlarmState aEither, TAlarmState aOr);

//
private:									// FROM RASSrvIteratorBase
//

	/**
	 * Does the specified alarm meet the search criteria?
	 */
	TBool									Matches(const TASSrvAlarm& aAlarm) const;

//
private:									// MEMBER DATA
//

	/**
	 * The primary match state
	 */
	TAlarmState								iState;

	/**
	 * The secondary (alternative) match state
	 */
	TAlarmState								iOtherState;

	/**
	 * The type of matching to perform
	 */
	TStateMatchType							iMatchType;

	};

#endif
