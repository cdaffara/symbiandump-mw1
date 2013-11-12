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

#include "ASSrvIteratorByState.h"

// System includes

// User includes

// Type definitions

// Constants

// Enumerations

// Classes referenced


//
// ----> RASSrvIteratorByState (source)
//

//*************************************************************************************
RASSrvIteratorByState::RASSrvIteratorByState(CASSrvAlarmQueue& aQueue, TAlarmState aState, TStateMatchType aMatchType)
:	RASSrvIteratorBase(aQueue), iMatchType(aMatchType)
	{
	SetState(aState);
	}

//*************************************************************************************
RASSrvIteratorByState::	RASSrvIteratorByState(CASSrvAlarmQueue& aQueue, TAlarmState aEither, TAlarmState aOr)
:	RASSrvIteratorBase(aQueue), iMatchType(EStateEitherOr)
	{
	SetStates(aEither, aOr);
	}


//
//
//


//*************************************************************************************
/**
 * Change the state for this iterator. Resets the iterator back to the start
 * of the queue
 */
void RASSrvIteratorByState::SetState(TAlarmState aState, TStateMatchType aMatchType)
	{
	iState = aState;
	iMatchType = aMatchType;
	}


//*************************************************************************************
/**
 * Change the state for this iterator. Resets the iterator back to the start
 * of the queue
 */
void RASSrvIteratorByState::SetStates(TAlarmState aEither, TAlarmState aOr)
	{
	iState = aEither;
	iOtherState = aOr;
	iMatchType = EStateEitherOr;
	}


//
//
//


//*************************************************************************************
TBool RASSrvIteratorByState::Matches(const TASSrvAlarm& aAlarm) const
	{
	// Check with other iterator - returns ETrue if there is no attached
	// iterator to check with.
	TBool matches = RASSrvIteratorBase::Matches(aAlarm);

	// Now check this iterator
	if	(matches)
		{
		switch(iMatchType)
			{
		default:
		case EStateEitherOr:
			return (aAlarm.State() == iState || aAlarm.State() == iOtherState);
		case EStateMustMatch:
			return aAlarm.State() == iState;
		case EStateMustNotMatch:
			return aAlarm.State() != iState;
			}
		}
	//
	return matches;
	}
