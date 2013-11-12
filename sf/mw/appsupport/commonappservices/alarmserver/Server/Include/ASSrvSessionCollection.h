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

#ifndef __ASSRVSESSIONCOLLECTION_H__
#define __ASSRVSESSIONCOLLECTION_H__

// System includes
#include <e32std.h>

// User includes
#include "ASSrvDefs.h"

// Classes referenced
class CASSrvSession;


//
// ----> MASSrvSession (header)
//
/**
 * Represents a session with the Alarm Server
 */
class MASSrvSession
	{
//
public:										// FROM MASSrvSession
//

	/**
	 * Returns the unique id assocaited with a session
	 */
	virtual TASSrvSessionId					MASSrvSessionId() const = 0;

	/**
	 * Returns the name of the thread associated with a session
	 */
	virtual void							MASSrvSessionFullName(TDes& aDes) const = 0;
	};


//
// ----> MASSrvSessionCollection (header)
//
/**
 * Each session has a unique identifier associated with it (see the CASSrvSessionEngine
 * object). This unique identifier is used for two purposes:
 *
 * 1). To ensure that when a client creates a "session alarm" (AddWithNotification)
 *     we are able to correctly DeQueue the right alarm when that session disconnects
 *	   unexpectedly.
 *
 * 2). So that it is possible to identify the owning thread for a given alarm. Each
 *     CSession2 object has an associated (and underlying RThread). In order to return
 *     the name of the thread through the client-API we need a method of associating
 *     sessions with alarms.
 */
class MASSrvSessionCollection
	{
//
public:										// FROM MASSrvSessionConnection
//

	/**
	 * Attach a particular session to the list of sessions
	 */
	virtual TASSrvSessionId					MASSessionCollectionAttachL(const MASSrvSession& aSession) = 0;

	/**
	 * Remove a session from the list of sessions
	 */
	virtual void							MASSessionCollectionDetach(const MASSrvSession& aSession) = 0;

	/**
	 * Get the owning thread name of a particular alarm
	 */
	virtual TInt							MASSessionCollectionAlarmOwner(TAlarmId aAlarmId, TDes& aFullName) const = 0;
	};

#endif
