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

#ifndef __TSERVERSESSIONENGINE_H__
#define __TSERVERSESSIONENGINE_H__

// System includes
#include <e32base.h>

// User includes
#include <asshddefs.h>
#include "ASSrvSessionEngine.h"

// Type definitions

// Constants

// Enumerations

// Classes referenced
class CASSrvServerWideData;
//class MASSrvSession;


//
// ----> TASSrvAnyEventObserver (header)
//
class TASSrvAnyEventObserver : public MASSrvAnyEventObserver
	{
//
public:									// FROM MASSRVANYEVENTOBSERVER
//
	void									MASSrvAnyEventHandleChange(TAlarmChangeEvent aEvent, TAlarmId aAlarmId);
	};


//
// ----> TASSrvSession (header)
//
class TASSrvSession : public MASSrvSession
	{
//
public:									// FROM MASSRVSESSION
//
	TASSrvSessionId							MASSrvSessionId() const;
	void									MASSrvSessionFullName(TDes& aDes) const;
	};


//
// ----> TASSrvAlarmTimerObserver (header)
//
class TASSrvAlarmTimerObserver : public MASSrvAlarmTimerObserver
	{
//
public:									// FROM MASSRVALARMTIMEROBSERVER
//
	void									MATimerHandleAlarmExpired(TAlarmTimerEvent aEvent, TAlarmId aAlarmId);
	};


//
// ----> CTestServerSessionEngine (header)
//
class CTestServerSessionEngine : public CBase
	{
//
public:										// STATIC CONSTRUCT / DESTRUCT
//
	static CTestServerSessionEngine*		NewLC();
	~CTestServerSessionEngine();

//
private:									// INTERNAL CONSTRUCT
//
	CTestServerSessionEngine();
	void									ConstructL();

//
public:										// ACCESS
//
	inline CASSrvServerWideData&			ServerWideData() const { return *iServerWideData; }
	inline CASSrvSessionEngine&				Engine() const { return *iSessionEngine; }

	TAlarmId								AddAlarmWithSpecificTimeL(TInt aSecondsFromNow);
	TAlarmId								AddAlarmWithSpecificCategoryL(TAlarmCategory aCategory);
	void									AwaitNotificationL();
	void									ClearAlarmStoreL();


//
private:									// INTERNAL
//
	void									StartAlarmServerL();
	void									StartAlarmAlertServerL();

//
private:									// MEMBER DATA
//
	TASSrvAnyEventObserver*					iChangeObserver;
	TASSrvSession*							iSession;
	CASSrvServerWideData*					iServerWideData;
	CASSrvSessionEngine*					iSessionEngine;
	TASSrvAlarmTimerObserver*				iTimerObserver;
	CActiveScheduler*						iScheduler;
	TTime									iHomeTime;
	};


#endif
