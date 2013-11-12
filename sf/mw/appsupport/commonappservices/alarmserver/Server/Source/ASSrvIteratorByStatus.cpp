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

#include "ASSrvIteratorByStatus.h"

// System includes

// User includes

// Type definitions

// Constants

// Enumerations

// Classes referenced


//
// ----> RASSrvIteratorByStatus (source)
//

//*************************************************************************************
RASSrvIteratorByStatus::RASSrvIteratorByStatus(CASSrvAlarmQueue& aQueue, TAlarmStatus aStatus, TStatusMatchType aMatchType)
:	RASSrvIteratorBase(aQueue), iMatchType(aMatchType)
	{
	SetStatus(aStatus);
	}


//
//
//


//*************************************************************************************
/**
 * Change the category for this iterator. Resets the iterator back to the start
 * of the queue
 */
void RASSrvIteratorByStatus::SetStatus(TAlarmStatus aStatus, TStatusMatchType aMatchType)
	{
	iStatus = aStatus;
	iMatchType = aMatchType;
	}


//
//
//


//*************************************************************************************
/**
 * @see RASSrvIteratorBase
 */
TBool RASSrvIteratorByStatus::Matches(const TASSrvAlarm& aAlarm) const
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
		case EStatusMustMatch:
			matches = aAlarm.Status() == iStatus;
			break;
		case EStatusMustNotMatch:
			matches = aAlarm.Status() != iStatus;
			break;
			}
		}
	//
	return matches;
	}
