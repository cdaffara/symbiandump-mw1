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

#include "afthumbnailtask.h"
#include "afcmd.h"

#include <fbs.h>

// -----------------------------------------------------------------------------
/**
 * Constructor for performing 1st stage construction
 */
CAfThumbnailTask::CAfThumbnailTask(MAfTaskStorage& storage, 
                                   const RMessage2 msg)
:
    mStorage(storage), 
    mMsg(msg)
{
    // No implementation required
}

// -----------------------------------------------------------------------------
/**
 * Destructor.
 */
CAfThumbnailTask::~CAfThumbnailTask()
{
    delete mBitmap;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CAfThumbnailTask::ExecuteLD(MAfTaskStorage& taskStorage,
                                 const RMessage2& message)
{
    CAfThumbnailTask *self = new (ELeave)CAfThumbnailTask(taskStorage, message);
    CleanupStack::PushL(self);
    self->ConstructL();
    taskStorage.PushL(self);
    CleanupStack::Pop(self);
    message.Complete(KErrNone);
}

// -----------------------------------------------------------------------------
/**
 * EPOC default constructor for performing 2nd stage construction
 */
void CAfThumbnailTask::ConstructL()
{
    RBuf path;
    CleanupClosePushL(path);
    path.CreateL(mMsg.GetDesLengthL(2));
    mMsg.ReadL(2, path);
    
    mBitmap = new (ELeave) CFbsBitmap;
    User::LeaveIfError(mBitmap->Load(path));

    CleanupStack::PopAndDestroy(&path);


    mMsg.Write(0, TPckgBuf<int>(mBitmap->Handle()));
    mMsg.Write(1, TPckgBuf<void *>(this));    

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const TDesC8& CAfThumbnailTask::Data() const
{
    return KNullDesC8();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CAfThumbnailTask::BroadcastReceivedL(const RMessage2&, TBool)
{
}

// -----------------------------------------------------------------------------
/**
 * Returns ETrue if task is related with session argument
 */
TBool CAfThumbnailTask::IsSessionTask(const CSession2* session)
{
    return mMsg.Session() == session ? ETrue : EFalse;
}
