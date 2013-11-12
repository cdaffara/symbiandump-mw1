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

#ifndef __ASSRVITERATORBYCATEGORY_H__
#define __ASSRVITERATORBYCATEGORY_H__

// System includes
#include <e32base.h>

// User includes
#include "ASSrvIteratorBase.h"
#include "ASSrvDefs.h"

//
// ----> RASSrvIteratorByCategory (header)
//
/**
 * Locate alarms by their category
 */
class RASSrvIteratorByCategory : public RASSrvIteratorBase
	{
//
public:										// CONSTRUCT
//

	/**
	 * Constructor
	 *
	 * @param aQueue The alarm queue containing the alarms
	 * @param aCategory The category of alarms to locate
	 */
	RASSrvIteratorByCategory(CASSrvAlarmQueue& aQueue, TAlarmCategory aCategory);

//
public:										// ACCESS
//
	
	/**
	 * Change the category for this iterator. Resets the iterator back to the start
	 * of the queue
	 */
	void									SetCategory(TAlarmCategory aCategory);

//
private:									// FROM RASSrvIteratorBase
//

	/**
	 * @see RASSrvIteratorBase
	 */
	TBool									Matches(const TASSrvAlarm& aAlarm) const;

//
private:									// MEMBER DATA
//

	/**
	 * The category being located
	 */
	TAlarmCategory							iCategory;

	};

#endif
