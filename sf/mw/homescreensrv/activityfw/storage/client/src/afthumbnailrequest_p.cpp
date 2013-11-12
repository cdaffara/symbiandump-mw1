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

#include <fbs.h>

#include "afthumbnailrequest_p.h"
#include "afstorageclient_p.h"
#include "afstorageclient.h"
#include "afentry.h"
#include "afcmd.h"

// -----------------------------------------------------------------------------
/**
 * Create and initialize handler for activity thumbnail request
 * @param observer - request completion observer
 * @param session - activity client implementation
 * @param source - thumbnail source location
 * @param userData - rsponse user data
 */
void CAfThumbnailRequestPrivate::NewLD(MAfAsyncRequestObserver &observer, 
                                       MAfAsyncSession & session,
                                       const TDesC& source,
                                       TAny* userData)
{
    CAfThumbnailRequestPrivate* self = new(ELeave)CAfThumbnailRequestPrivate(observer, session, userData);
    CleanupStack::PushL(self);
    self->ConstructL(source);
    session.PushL(self);
    CleanupStack::Pop(self);
}

// -----------------------------------------------------------------------------
/**
 * Constructor
 * @param observer - request completion observer
 * @param session - activity client implementation
 * @param userData - rsponse user data
 */
CAfThumbnailRequestPrivate::CAfThumbnailRequestPrivate(MAfAsyncRequestObserver & observer, 
                                                       MAfAsyncSession & session,
                                                       TAny* userData)
:
CActive(EPriorityStandard),
mObserver(observer),
mSession(session),
mUserData(userData)
{
    CActiveScheduler::Add(this);
}

// -----------------------------------------------------------------------------
/**
 * Second phase constructor
 * @param source - thumbnail source location
 */
void CAfThumbnailRequestPrivate::ConstructL(const TDesC& source)
{
    mIds[0] = 0;
    mIds[1] = 0;
    CAfEntry::CopyL(mImgSrc, source);
    mSession.sendAsync(GetThumbnail, 
                       TIpcArgs(&mIds[0],&mIds[1], &mImgSrc),
                       iStatus);
    SetActive();
}

// -----------------------------------------------------------------------------
/**
 * Destuctor
 */
CAfThumbnailRequestPrivate::~CAfThumbnailRequestPrivate()
{
    Cancel();
    mImgSrc.Close();
}

// -----------------------------------------------------------------------------
/**
 * Cancel pending requst
 */
void CAfThumbnailRequestPrivate::DoCancel()
{
}

// -----------------------------------------------------------------------------
/**
 * Function handle request completion, copy data, forward information and destroy handler 
 */
void CAfThumbnailRequestPrivate::RunL()
{
    User::LeaveIfError(iStatus.Int());
    mObserver.getThumbnailRequestCompleted(iStatus.Int(), 
                                           mIds[0](), 
                                           mUserData);
    RBuf8 data;                                        
    CleanupClosePushL(data);
    mSession.getDataL((mIds[1])(), data);
    CleanupStack::PopAndDestroy(&data);
    mSession.Pop(this);
    delete this;
}

// -----------------------------------------------------------------------------
/**
 * Function handle request processing errors
 * @param error - error code 
 */
TInt CAfThumbnailRequestPrivate::RunError(TInt error)
{
    mObserver.getThumbnailRequestCompleted(error, -1, mUserData);
    mSession.Pop(this);
    delete this;
    return KErrNone;
}

