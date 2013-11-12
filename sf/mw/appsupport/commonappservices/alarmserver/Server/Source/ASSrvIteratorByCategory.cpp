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

#include "ASSrvIteratorByCategory.h"

// System includes

// User includes

// Type definitions

// Constants

// Enumerations

// Classes referenced


//
// ----> RASSrvIteratorByCategory (source)
//

//*************************************************************************************
RASSrvIteratorByCategory::RASSrvIteratorByCategory(CASSrvAlarmQueue& aQueue, TAlarmCategory aCategory)
:	RASSrvIteratorBase(aQueue)
	{
	SetCategory(aCategory);
	}


//
//
//


//*************************************************************************************
/**
 * Change the category for this iterator. Resets the iterator back to the start
 * of the queue
 */
void RASSrvIteratorByCategory::SetCategory(TAlarmCategory aCategory)
	{
	iCategory = aCategory;
	}


//
//
//


//*************************************************************************************
/**
 * @see RASSrvIteratorBase
 */
TBool RASSrvIteratorByCategory::Matches(const TASSrvAlarm& aAlarm) const
	{
	// Check with other iterator - returns ETrue if there is no attached
	// iterator to check with.
	TBool matches = RASSrvIteratorBase::Matches(aAlarm);

	// Now check this iterator
	if	(matches)
		matches = (aAlarm.Category() == iCategory);
	//
	return matches;
	}



