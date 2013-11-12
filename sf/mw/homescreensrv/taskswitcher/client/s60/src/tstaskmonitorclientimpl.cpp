/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/

#include <e32base.h>

#include "tstaskmonitorclientimpl.h"
#include "tstaskmonitorobserver.h"

CTsTaskMonitorClientImpl* CTsTaskMonitorClientImpl::NewL()
{
    CTsTaskMonitorClientImpl* self = NewLC();
    CleanupStack::Pop(self);
    return self;
}

CTsTaskMonitorClientImpl* CTsTaskMonitorClientImpl::NewLC()
{
    CTsTaskMonitorClientImpl* self = new (ELeave) CTsTaskMonitorClientImpl;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}

CTsTaskMonitorClientImpl::CTsTaskMonitorClientImpl() : CActive(CActive::EPriorityStandard)
{
    CActiveScheduler::Add(this);
}

CTsTaskMonitorClientImpl::~CTsTaskMonitorClientImpl()
{
    Cancel();
    iSession.Close();
}

void CTsTaskMonitorClientImpl::ConstructL()
{
    User::LeaveIfError(iSession.Connect());
}


HBufC8* CTsTaskMonitorClientImpl::TasksContentLC()
{
    return iSession.TasksContentLC();
}

void CTsTaskMonitorClientImpl::Subscribe(MTsTaskMonitorObserver& aObserver)
{
    Cancel();
    iObserver = &aObserver;
    iSession.Subscribe(iStatus);
    SetActive();
}

void CTsTaskMonitorClientImpl::CancelSubscribe()
{
    Cancel();
}

void CTsTaskMonitorClientImpl::OpenTask(const TDesC8 &key)
{
    iSession.OpenTask(key);
}

void CTsTaskMonitorClientImpl::CloseTask(const TDesC8 &key)
{
    iSession.CloseTask(key);
}

void CTsTaskMonitorClientImpl::RunL()
{
    if (iStatus == KErrNone && iObserver) {
        iSession.Subscribe(iStatus);
        SetActive();
        iObserver->HandleRunningAppChange();
    }
}

void CTsTaskMonitorClientImpl::DoCancel()
{
    iSession.CancelSubscribe();
}
