// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __UTILITIES_H__
#define __UTILITIES_H__


#include <e32base.h>
#include <e32test.h>
#include <tzupdate.h>
#include <e32property.h>
#include <asclisession.h>


class CNotificationTimer: public CBase
	{
public:
	IMPORT_C static CNotificationTimer* NewL();
	IMPORT_C void StartTimer(TTimeIntervalMicroSeconds aTimeOutMicroSeconds, CActive::TPriority aTimerPriority=CActive::EPriorityStandard);
	IMPORT_C void StopTimer();
	IMPORT_C TBool HasTimedOut()const;
	IMPORT_C ~CNotificationTimer();
	static TInt TimeOutCallBack(TAny* aPtr);
	void DoTimeOutCallBack();
private:
	CNotificationTimer();
	void ConstructL();
private:
	CPeriodic* iTimer;
	TBool iTimedOut;
	};

/*
This class can be used for observing changes to any RProperty.
Tests can wait for a certain event to happen instead of using User::After.
*/

class CPropertyWatcher: public CActive
	{
	public:
		IMPORT_C static CPropertyWatcher* NewL(TUid aPropertyCategory, TUint aPropertyKey, TOwnerType aType=EOwnerProcess);
		IMPORT_C void StartWatchingL(TTimeIntervalMicroSeconds aTimeOutMicroSeconds, CActive::TPriority aTimerPriority=CActive::EPriorityStandard);
		IMPORT_C RProperty& GetProperty();
		IMPORT_C ~CPropertyWatcher();
		//from CActive
		void RunL();
		void DoCancel();		
	private:
		CPropertyWatcher();
		void ConstructL(TUid aPropertyCategory, TUint aPropertyKey, TOwnerType aType);
		void Subscribe();
	private:
		CNotificationTimer* iWatcherTimeOutTimer;
		TUint iPropertyKey;
		TUid iPropertyCategory;
		RProperty iProperty;	
	};


class CAlarmEventWatcher : public CActive
	{
public:
	IMPORT_C static CAlarmEventWatcher* NewL(RASCliSession& aAlarmSession);
	IMPORT_C void StartWaitingForEventL(TAlarmChangeEvent aAlarmChangeEvent, TTimeIntervalMicroSeconds aTimeOutMicroSeconds, CActive::TPriority aTimerPriority=CActive::EPriorityStandard);
	IMPORT_C ~CAlarmEventWatcher();
	//from CActive
	void RunL();
	void DoCancel();
private:
	CAlarmEventWatcher(RASCliSession& aAlarmSession);
	void ConstructL();
	void Subscribe();
private:
	TAlarmChangeEvent iAlarmChangeEvent;
	RASCliSession& iAlarmSession;
	CNotificationTimer* iEventNotificationTimer;
	TAlarmId iAlarmId;
	TInt iEventCount;
	};

#endif

