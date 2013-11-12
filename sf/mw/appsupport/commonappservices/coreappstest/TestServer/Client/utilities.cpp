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

#include "utilities.h"


CPropertyWatcher::CPropertyWatcher(): CActive(EPriorityLow)
	{
	iStatus = KErrNone;
	CActiveScheduler::Add(this);
	}

void CPropertyWatcher::ConstructL(TUid aPropertyCategory, TUint aPropertyKey, TOwnerType aType)
	{
	iWatcherTimeOutTimer = CNotificationTimer::NewL();
	iPropertyKey = aPropertyKey;
	iPropertyCategory = aPropertyCategory;
	User::LeaveIfError(iProperty.Attach(iPropertyCategory, iPropertyKey, aType));
	Subscribe();
	}

EXPORT_C CPropertyWatcher* CPropertyWatcher::NewL(TUid aPropertyCategory, TUint aPropertyKey, TOwnerType aType)
	{
	CPropertyWatcher* self = new(ELeave)CPropertyWatcher();
	CleanupStack::PushL(self);
	self->ConstructL(aPropertyCategory, aPropertyKey, aType);
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CPropertyWatcher::~CPropertyWatcher()
	{
	Cancel();
	iProperty.Close();
	delete iWatcherTimeOutTimer;
	}

EXPORT_C RProperty& CPropertyWatcher::GetProperty()
	{
	return iProperty;
	}

EXPORT_C void CPropertyWatcher::StartWatchingL(TTimeIntervalMicroSeconds aTimeOutMicroSeconds, CActive::TPriority aTimerPriority)
	{
	iWatcherTimeOutTimer->StartTimer(aTimeOutMicroSeconds, aTimerPriority);
	
	CActiveScheduler::Start();
	
	if (iWatcherTimeOutTimer->HasTimedOut())
		{
		DoCancel();
		RDebug::Print(_L("Timer timed out!"));
		User::Leave(KErrNotFound);	
		}
	else
		{
		iWatcherTimeOutTimer->StopTimer();
		}
	}
void CPropertyWatcher::DoCancel()
	{
	iProperty.Cancel();
	}


void CPropertyWatcher::Subscribe()
	{
	if (!IsActive())
		{
		iStatus = KRequestPending;
		iProperty.Subscribe(iStatus);
		SetActive();		
		}
	}

void CPropertyWatcher::RunL()
	{
	CActiveScheduler::Stop();
	}


/*Notification Timer starts*/

CNotificationTimer::CNotificationTimer()
	{}

EXPORT_C CNotificationTimer::~CNotificationTimer()
	{
	delete iTimer;
	}

EXPORT_C CNotificationTimer* CNotificationTimer::NewL()
	{
	CNotificationTimer* self = new(ELeave)CNotificationTimer();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CNotificationTimer::ConstructL()
	{
	iTimer = CPeriodic::NewL(EPriorityNormal);
	iTimedOut = EFalse;
	}

EXPORT_C void CNotificationTimer::StartTimer(TTimeIntervalMicroSeconds aTimeOutMicroSeconds, CActive::TPriority aTimerPriority)
	{
	const TInt KPanicCode = -1000;
	if (iTimer->IsActive())
		{
		User::Panic(_L("Timer is active!!"), KPanicCode);
		}
	TInt64 timeOut = aTimeOutMicroSeconds.Int64();
	iTimedOut = EFalse;
	iTimer->SetPriority(aTimerPriority);
	iTimer->Start(timeOut, timeOut, TCallBack(TimeOutCallBack, this));
	}

EXPORT_C void CNotificationTimer::StopTimer()
	{
	if (iTimer->IsActive())
		{
		iTimer->Cancel();
		}
	}

EXPORT_C TBool CNotificationTimer::HasTimedOut()const
	{
	return iTimedOut;
	}

TInt CNotificationTimer::TimeOutCallBack(TAny* aPtr)
	{
	CNotificationTimer* self = static_cast<CNotificationTimer*>(aPtr);
	if (self)
		{
		self->DoTimeOutCallBack();
		}
	return KErrNone;
	}

void CNotificationTimer::DoTimeOutCallBack()
	{
	RDebug::Print(_L("Timer timed out!"));
	iTimedOut = ETrue;
	CActiveScheduler::Stop();
	}

/*Notification Timer ends*/

/*Alarm Event Watcher starts*/

CAlarmEventWatcher::CAlarmEventWatcher(RASCliSession& aAlarmSession):CActive(EPriorityLow),iAlarmSession(aAlarmSession)
	{
	iStatus = KErrNone;
	CActiveScheduler::Add(this);
	}

void CAlarmEventWatcher::ConstructL()
	{
	iEventNotificationTimer = CNotificationTimer::NewL();
	iEventCount = 0;
	iAlarmId = 0;
	Subscribe();
	}

EXPORT_C CAlarmEventWatcher* CAlarmEventWatcher::NewL(RASCliSession& aAlarmSession)
	{		
	CAlarmEventWatcher* self = new(ELeave)CAlarmEventWatcher(aAlarmSession);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}
		
EXPORT_C CAlarmEventWatcher::~CAlarmEventWatcher()
	{
	Cancel();
	delete iEventNotificationTimer;
	}

EXPORT_C void CAlarmEventWatcher::StartWaitingForEventL(TAlarmChangeEvent aAlarmChangeEvent, TTimeIntervalMicroSeconds aTimeOutMicroSeconds, CActive::TPriority aTimerPriority)
	{
	iEventNotificationTimer->StartTimer(aTimeOutMicroSeconds, aTimerPriority);
	iAlarmChangeEvent = aAlarmChangeEvent;
	CActiveScheduler::Start();
	
	if (iEventNotificationTimer->HasTimedOut())
		{
		DoCancel();
		RDebug::Print(_L("Timer timed out!"));
		User::Leave(KErrNotFound);	
		}
	else
		{
		iEventNotificationTimer->StopTimer();
		}
	}

void CAlarmEventWatcher::DoCancel()
	{
	iAlarmSession.NotifyChangeCancel();
	}

void CAlarmEventWatcher::Subscribe()
	{
	if (!IsActive())
		{
		RDebug::Print(_L("Waiting for Alarm Event: %d"), iAlarmChangeEvent);
		iStatus = KRequestPending;
		iAlarmSession.NotifyChange(iStatus, iAlarmId);
		SetActive();		
		}
	}

void CAlarmEventWatcher::RunL()
	{
	_LIT(KTimeFmt, "%F%Y%M%D-%H:%T:%S");
	const TInt KBufSize = 255;
	TBuf<KBufSize> buf;
	
	if(iAlarmChangeEvent == iStatus.Int())
		{
		CActiveScheduler::Stop();
		}
	TTime now;
	now.HomeTime();
	now.FormatL(buf, KTimeFmt);	
	RDebug::Print(_L("Local time now: %s"), buf.PtrZ());
	now.UniversalTime();
	now.FormatL(buf, KTimeFmt);	
	RDebug::Print(_L("UTC time now: %s"), buf.PtrZ());
	RDebug::Print(_L("Received Alarm Event: %d,  Alarm Id: %d"), iStatus.Int(), iAlarmId);
	
	Subscribe();
	}

/*Alarm Event Watcher ends*/
