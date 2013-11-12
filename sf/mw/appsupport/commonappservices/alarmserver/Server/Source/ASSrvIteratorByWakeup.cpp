// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ASSrvIteratorByWakeup.h"

RASSrvIteratorByWakeup::RASSrvIteratorByWakeup(CASSrvAlarmQueue& aQueue)
	:RASSrvIteratorBase(aQueue)
	{
	}

/**
 * @see RASSrvIteratorBase
 */
TBool RASSrvIteratorByWakeup::Matches(const TASSrvAlarm& aAlarm) const
	{
	// Check with other iterator - returns ETrue if there is no attached
	// iterator to check with.
	TBool matches = RASSrvIteratorBase::Matches(aAlarm);

	// Now check this iterator
	if (matches)
		{
		matches = aAlarm.IsWakeup();
		}
	
	return matches;
	}
