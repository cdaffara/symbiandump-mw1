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

#ifndef __ASSRVANYEVENTMANAGER_H__
#define __ASSRVANYEVENTMANAGER_H__

// System includes
#include <e32std.h>

// User includes
#include <asshddefs.h>

// Type definitions

// Constants

// Enumerations

// Classes referenced
class MASSrvAnyEventObserver;


//
// ----> MASSrvAnyEventManager (header)
//
/**
 * This class has two purposes:-
 *
 * 1) Defines the interface which allows other objects within the Alarm Server
 *    to receive arbitrary events ("any events")
 * 
 * 2) Defines the interface which allows other objects within the Alarm Server
 *    to notify a central controller (the real instance-object which implements
 *    this mixin) about arbitrary events.
 *
 */
class MASSrvAnyEventManager
	{
//
public:										// FROM MASSrvAnyEventManager
//

	/**
	 * Request events
	 */
	virtual void							MASAnyEventManagerObserverAddL(MASSrvAnyEventObserver& aObserver) = 0;

	/**
	 * Cancel event request
	 */
	virtual void							MASAnyEventManagerObserverRemove(const MASSrvAnyEventObserver& aObserver) = 0;

	/**
	 * Submit an event
	 */
	virtual void							MASAnyEventManagerObserverNotifyChanges(TAlarmChangeEvent aEvent, TAlarmId aAlarmId) = 0;
	};

#endif
