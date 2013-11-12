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

#ifndef __ASSRV_H__
#define __ASSRV_H__

#include <e32std.h>

class TASSrvAlarm;

/**
 * An abstract class representing an object which wishes to be informed
 * when the state or status of an alarm changes.
 */
class MASSrvAlarmObserver
	{
public:
	/**
	 * What type of event took place
	 */
	enum TObserverEvent
		{
		/**
		 * The alarm changed state
		 */
		EAlarmObserverStateChanged = 0,

		/**
		 * The alarm changed status
		 */
		EAlarmObserverStatusChanged
		};

public:
	/**
	 * Called when the state or status of an alarm changes.
	 */
	virtual void MASSrvAlarmObsHandleEvent(TObserverEvent aEvent, const TASSrvAlarm& aAlarm, TInt aEventSpecificData) = 0;
	};

#endif
