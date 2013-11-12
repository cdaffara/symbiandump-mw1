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

#ifndef __ASSRVALARMQUEUEOBSERVER_H__
#define __ASSRVALARMQUEUEOBSERVER_H__

// System includes
#include <e32base.h>

// User includes
#include <asshddefs.h>

// Type definitions

// Constants

// Enumerations

// Classes referenced


//
// ----> MASSrvAlarmQueueObserver (header)
//
/**
 * A mixin class to be implemented by any object wishing to be informed
 * when the status of the alarm queue changes
 */
class MASSrvAlarmQueueObserver
	{
//
public:										// FROM MASSrvAlarmQueueObserver
//

	/**
	 * The type of event that took place
	 */
	enum TASSrvAlarmQueueEvent
		{
		/**
		 * The head item in the alarm queue changed
		 */
		EASSrvAlarmQueueEventHeadItemChanged = 0,

		/**
		 * An alarm was added to the queue
		 */
		EASSrvAlarmQueueEventAlarmAdded,

		/**
		 * The properties of an alarm changed
		 */
		EASSrvAlarmQueueEventAlarmChanged,

		/**
		 * An alarm was removed from the queue
		 */
		EASSrvAlarmQueueEventAlarmDeleted,

		/**
		 * Alarm server is starting to Internalize
		 * (e.g. on startup, after backup restore) 
		 */
		EASSrvAlarmQueueEventAlarmStartInternalize
		};

//
public:										// FROM MASSrvAlarmQueueObserver
//

	/**
	 * Abstract event handler method
	 */
	virtual void							MAlarmQueueObserverHandleEvent(TASSrvAlarmQueueEvent aEvent, TAlarmId aAlarmId) = 0;
	};


#endif
