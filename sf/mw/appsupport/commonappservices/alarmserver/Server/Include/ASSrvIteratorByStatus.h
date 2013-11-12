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

#ifndef __ASSRVITERATORBYSTATUS_H__
#define __ASSRVITERATORBYSTATUS_H__

// System includes
#include <e32base.h>

// User includes
#include "ASSrvIteratorBase.h"
#include "ASSrvDefs.h"

// Type definitions

// Constants

// Enumerations

// Classes referenced


//
// ----> RASSrvIteratorByStatus (header)
//
/**
 * Locate alarms by their status
 */
class RASSrvIteratorByStatus : public RASSrvIteratorBase
	{
//
public:										// ENUMERATIONS
//
	/**
	 *  The match type
	 */
	enum TStatusMatchType
		{
		/**
		 * The status must match
		 */
		EStatusMustMatch = 0,

		/**
		 * The status must not match
		 */
		EStatusMustNotMatch = 1
		};

//
public:										// CONSTRUCT
//

	/**
	 * Constructor
	 *
	 * @param aQueue The alarm queue
	 * @param aStatus The alarm status to use as search criteria
	 * @param aMatchType The match type
	 */
	RASSrvIteratorByStatus(CASSrvAlarmQueue& aQueue, TAlarmStatus aStatus, TStatusMatchType aMatchType = EStatusMustMatch);

//
public:										// ACCESS
//
	
	/**
	 * Change the category for this iterator. Resets the iterator back to the start
	 * of the queue
	 */
	void									SetStatus(TAlarmStatus aStatus, TStatusMatchType aMatchType = EStatusMustMatch);

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
	 * The matching status
	 */
	TAlarmStatus							iStatus;

	/**
	 * The match type
	 */
	TStatusMatchType						iMatchType;

	};

#endif
