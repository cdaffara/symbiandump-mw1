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

#include "afasyncrequest_p.h"
#include "afstorageclient_p.h"
#include "afstorageclient.h"
#include "afentry.h"

#include <fbs.h>

// -----------------------------------------------------------------------------
/**
 * Create and initialize handler for anyc. activity requests
 * @param observer - request completion observer
 * @param session - activity client implementation
 * @param cmd - requested functionality
 */
void CAfAsyncRequestPrivate::NewLD(MAfAsyncRequestObserver &observer, 
                                   MAfAsyncSession & session,
                                   ActivityCmd cmd)
{
    CAfAsyncRequestPrivate* self = new(ELeave)CAfAsyncRequestPrivate(observer, session, cmd);
    CleanupStack::PushL(self);
    self->ConstructL();
    session.PushL(self);
    CleanupStack::Pop(self);
}

// -----------------------------------------------------------------------------
/**
 * First phase construction
 * @param observer - request completion observer
 * @param session - activity client implementation
 * @param cmd - requested functionality
 */
CAfAsyncRequestPrivate::CAfAsyncRequestPrivate(MAfAsyncRequestObserver & observer, 
                                               MAfAsyncSession & session,
                                               ActivityCmd cmd)
:
CActive(EPriorityStandard),
mObserver(observer),
mSession(session),
mCmd(cmd)
{
    CActiveScheduler::Add(this);
    RProcess process;
    mIds[0] = static_cast<TInt>(process.SecureId().iId);
}

// -----------------------------------------------------------------------------
/**
 * Second phase construction
 */
void CAfAsyncRequestPrivate::ConstructL()
{
    mSession.sendAsync(mCmd, 
                       TIpcArgs(&mIds[0],&mIds[1], &mIds[2], &mIds[3]),
                       iStatus);
    SetActive();
}

// -----------------------------------------------------------------------------
/**
 * Destructor
 */
CAfAsyncRequestPrivate::~CAfAsyncRequestPrivate()
{
    Cancel();
}

// -----------------------------------------------------------------------------
/**
 * Cancel pending requst
 */
void CAfAsyncRequestPrivate::DoCancel()
{
    TRAP_IGNORE(
    switch (mCmd) {
    case WaitActivity: mSession.executeL(CancelWait); break;
    case NotifyChange: mSession.executeL(CancelNotify); break;
    }
    )
}

// -----------------------------------------------------------------------------
/**
 * Function handle request completion, copy data, forward information and destroy handler 
 */
void CAfAsyncRequestPrivate::RunL()
{
    User::LeaveIfError(iStatus.Int());
    switch (mCmd) {
    case WaitActivity:
        {
        RBuf8 data;
        CleanupClosePushL(data);
        CAfEntry::ReallocL(data, (mIds[0])());
        mSession.getDataL((mIds[3])(), data);
        mObserver.waitActivityRequestCompleted(iStatus.Int(), data);
        CleanupStack::PopAndDestroy(&data);
        break;
        }
    case NotifyChange:
        mObserver.dataChangeNotificationCompleted(iStatus.Int());
        break;
    }
    mSession.Pop(this);
    delete this;
}

// -----------------------------------------------------------------------------
/**
 * Function handle request processing errors
 * @param error - error code 
 */
TInt CAfAsyncRequestPrivate::RunError(TInt error)
{
    (WaitActivity == mCmd) ? mObserver.waitActivityRequestCompleted(error, KNullDesC8()) :
                             mObserver.dataChangeNotificationCompleted(error);
    mSession.Pop(this);
    delete this;
    return KErrNone;
}


