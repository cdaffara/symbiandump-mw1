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

/**
 @file
 @test
 @internalComponent - Internal Symbian test code 
*/
#include "tamastarterappobserver.h"
#include "tamastarter_constants.h"

CTestAmastartAppObserver* CTestAmastartAppObserver::NewL(CActiveSchedulerWait* aActiveScheduler)
	{
	CTestAmastartAppObserver* self = new(ELeave)CTestAmastartAppObserver();
	CleanupStack::PushL(self);
	self->ConstructL(aActiveScheduler);
	CleanupStack::Pop(self);
	return self;
	}

CTestAmastartAppObserver::CTestAmastartAppObserver() : CActive(CActive::EPriorityIdle)
    {
    CActiveScheduler::Add(this);
    }

void CTestAmastartAppObserver:: ConstructL(CActiveSchedulerWait* aActiveScheduler)
    {
    iActiveScheduler = aActiveScheduler;
    User::LeaveIfError(iProperty.Attach(KTestAmastartCategory, KTestAmastartKey));
    }

void CTestAmastartAppObserver::StartL()
    {
    iProperty.Subscribe(iStatus);
    SetActive();
    }

CTestAmastartAppObserver::~CTestAmastartAppObserver()
	 {
	 Cancel();
	 iProperty.Close();
	 }

void CTestAmastartAppObserver::RunL()
	{
	TInt amaStarterAppVal;
	User::LeaveIfError(RProperty::Get(KTestAmastartCategory, KTestAmastartKey, amaStarterAppVal));

	if(amaStarterAppVal == 3)
		{
		User::LeaveIfError(RProperty::Set(KTestAmastartCategory, KTestAmastartKey, 0));
		iActiveScheduler->AsyncStop();
		}
	else
		{
		iStatus = KRequestPending;
	    TRequestStatus* status = &iStatus;
	    SetActive();
	    User::RequestComplete(status, KErrNone);
		}
	}

void CTestAmastartAppObserver::DoCancel()
	{
	iProperty.Cancel();
	}

CTestAmastartTimer* CTestAmastartTimer::NewL(CActiveSchedulerWait* aActiveScheduler, TInt aDelay)
	{
	CTestAmastartTimer* self = new(ELeave)CTestAmastartTimer();
	CleanupStack::PushL(self);
	self->ConstructL(aActiveScheduler, aDelay);
	CleanupStack::Pop(self);
	return self;
	}

CTestAmastartTimer::CTestAmastartTimer() : CTimer(CActive::EPriorityIdle)
    {
    CActiveScheduler::Add(this);
    }

void CTestAmastartTimer:: ConstructL(CActiveSchedulerWait* aActiveScheduler, TInt aDelay)
    {
    iActiveScheduler = aActiveScheduler;
    iDelay = aDelay;
    CTimer::ConstructL();
    }

void CTestAmastartTimer::StartL()
    {
    After(iDelay);
    }

CTestAmastartTimer::~CTestAmastartTimer()
	{
	 Cancel();
	}

void CTestAmastartTimer::RunL()
	{
	iActiveScheduler->AsyncStop();
	}

void CTestAmastartTimer::DoCancel()
	{
	//Nothing to cancel
	}
