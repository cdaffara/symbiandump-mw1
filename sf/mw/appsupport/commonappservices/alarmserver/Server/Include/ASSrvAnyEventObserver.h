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

#ifndef __ASSRVANYEVENTOBSERVER_H__
#define __ASSRVANYEVENTOBSERVER_H__

// System includes
#include <e32std.h>

// User includes
#include <asshddefs.h>


//
// ----> MASSrvAnyEventObserver (header)
//
/**
 * Abstract interface for those objects that wish to receive global
 * server-wide events.
 */
class MASSrvAnyEventObserver
	{
//
public:										// FROM MASSrvAnyEventObserver
//

	/**
	 * Called when anything happens which should be cascaded to sessions with an
	 * outstanding "notify on change" event request.
	 *
	 * @param aChangeCode This is an error condition if less than KErrNone, or otherwise
	 * an event enumeration from T
	 */
	virtual void							MASSrvAnyEventHandleChange(TAlarmChangeEvent aEvent, TAlarmId aAlarmId) = 0;

	};

#endif
