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

#ifndef __ASSRVITERATORBYSESSIONID_H__
#define __ASSRVITERATORBYSESSIONID_H__

// System includes
#include <e32base.h>

// User includes
#include <asshddefs.h>
#include "ASSrvDefs.h"
#include "ASSrvIteratorBase.h"

// Classes referenced
class CASSrvAlarmQueue;


//
// ----> RASSrvIteratorBySessionId (header)
//
/**
 * Locate alarms by their session identifier.
 */
class RASSrvIteratorBySessionId : public RASSrvIteratorBase
	{
//
public:										// CONSTRUCT
//

	/**
	 * Constructor
	 * 
	 * @param aQueue The alarm queue
	 * @param aId The session id of alarms to locate
	 */
	RASSrvIteratorBySessionId(CASSrvAlarmQueue& aQueue, TASSrvSessionId aId);

//
public:										// ACCESS
//
	
	/**
	 * Change the category for this iterator. Resets the iterator back to the start
	 * of the queue.
	 */
	void									SetSessionId(TASSrvSessionId aId);

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
	 * The session id we're matching against
	 */
	TASSrvSessionId							iSessionId;
	};

#endif
