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
#include "afapplicationsmonitor.h"

//------------------------------------------------------------------------------
CAfApplicationsMonitor* CAfApplicationsMonitor::NewL(RApaLsSession& serviceProvider,
                                                   MAfApplicationsObserver& observer)
{
    CAfApplicationsMonitor* self = new (ELeave) CAfApplicationsMonitor(serviceProvider, observer);
    self->Subscribe();
    return self;
}

//------------------------------------------------------------------------------
CAfApplicationsMonitor::CAfApplicationsMonitor(RApaLsSession& serviceProvider, 
                                             MAfApplicationsObserver& observer)
:
    CActive(EPriorityLow),
    mServiceProvider(serviceProvider),
    mObserver(observer)
{
    CActiveScheduler::Add(this);
}

//------------------------------------------------------------------------------
CAfApplicationsMonitor::~CAfApplicationsMonitor()
{
    Cancel();
}

//------------------------------------------------------------------------------
void CAfApplicationsMonitor::RunL()
{
    User::LeaveIfError(iStatus.Int());
    mObserver.applicationsChanged();
    Subscribe();
}

//------------------------------------------------------------------------------
void CAfApplicationsMonitor::Subscribe()
{
    mServiceProvider.SetNotify(EFalse, iStatus);
    SetActive();
}

//------------------------------------------------------------------------------
TInt CAfApplicationsMonitor::RunError(TInt /*error*/)
{
    Subscribe();
    return KErrNone;
}

//------------------------------------------------------------------------------
void CAfApplicationsMonitor::DoCancel()
{
    mServiceProvider.CancelNotify();
}
