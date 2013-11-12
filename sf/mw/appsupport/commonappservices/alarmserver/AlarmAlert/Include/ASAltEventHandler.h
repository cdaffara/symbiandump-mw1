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

#ifndef __ASALTEVENTHANDLER_H__
#define __ASALTEVENTHANDLER_H__

// System includes
#include <e32base.h>

// User includes
#include <asshddefs.h>

// Type definitions

// Constants

// Enumerations

// Classes referenced
class RASAltClientSession;
class MASAltAlarmAlertObserver;
class CASAltRequestQuietPeriodEnd;

//
// ----> CASAltEventHandler (header)
//
/**
 * Interprets the events back from the alarm alert server thread.
 */
class CASAltEventHandler : public CActive
	{
//
public:										// STATIC CONSTRUCT / DESTRUCT
//
	static CASAltEventHandler*				NewL(RASAltClientSession& aSession, MASAltAlarmAlertObserver& aObserver);
	~CASAltEventHandler();

//
public:										// ACCESS
//

	void									RequestEvents();

//
private:									// INTERNAL CONSTRUCT
//
	CASAltEventHandler(RASAltClientSession& aSession, MASAltAlarmAlertObserver& aObserver);
	void									ConstructL();

//
private:									// FROM CActive
//

	void									RunL();

	void									DoCancel();

//
private:									// INTERNAL
//

	inline RASAltClientSession&				Session() { return iSession; }

	inline MASAltAlarmAlertObserver&		Observer() { return iObserver; }

//
private:									// MEMBER DATA
//

	RASAltClientSession&					iSession;

	MASAltAlarmAlertObserver&				iObserver;

	TTime									iTimeValueFromAlarmAlertServer;

	CASAltRequestQuietPeriodEnd*			iRequestQuietPeriodEnd;
	
	TAlarmId								iAlarmId;
	};

#endif
