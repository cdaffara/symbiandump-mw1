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

#ifndef __ASSRVTIMEROBSERVER_H__
#define __ASSRVTIMEROBSERVER_H__

// System includes
#include <e32base.h>

// User includes
#include <asshddefs.h>

// Classes referenced
class CASSrvServerWideData;


//
// ----> MASSrvAlarmTimerObserver (header)
//
/**
 * An abstract class which all observers of the alarm timer must implement
 * in order to receive events
 */
class MASSrvAlarmTimerObserver
	{
//
public:										// FROM MASSrvAlarmTimerObserver
//

	/**
	 * The alarm timer event
	 */
	enum TAlarmTimerEvent
		{
		/**
		 * An alarm has expired
		 */
		EAlarmTimerEventAlarmExpired = 0,

		/**
		 * The date/time has changed
		 */
		EAlarmTimerEventTimeOrDateChanged,

		/**
		 * There was an error returned by CTimer::iStatus
		 */
		EAlarmTimerEventTimingError
		};

	
//
public:										// FROM MASSrvAlarmTimerObserver
//
	
	/**
	 * Handle the event
	 */
	virtual void							MATimerHandleAlarmExpired(TAlarmTimerEvent aEvent, TAlarmId aAlarmId) = 0;
	};

#endif
