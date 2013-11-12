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
#include "afsession.h"
#include "aftask.h"
#include "afcmd.h"

#include "afstorageasynctask.h"
#include "afstoragesynctask.h"
#include "afobservertask.h"
#include "afbroadcasttask.h"
#include "afdataprovidertask.h"
#include "afthumbnailtask.h"

_LIT(KTaskAlreadyExists, "Activity task exists");

// -----------------------------------------------------------------------------
/**
 * Constructor for performing 1st stage construction
 * @param fileSession - initialized file system session
 * @param taskStorage - global observers storage
 * @param storage - data storage
 */

CAfSession::CAfSession(RFs& fileSession, 
                       MAfTaskStorage& taskStorage, 
                       CAfStorage& storage)
:
mFileSession(fileSession),
mTasksStorage(taskStorage),
mStorage(storage)
{
    // No implementation required
}

// -----------------------------------------------------------------------------
/**
 * Destructor.
 */
CAfSession::~CAfSession()
{
    RemoveNotValidTasks(this);
    mTasksStorage.RemoveNotValidTasks(this);
}

// -----------------------------------------------------------------------------
/**
 * Two-phased constructor.
 * @param fileSession - initialized file system session
 * @param taskStorage - global observers storage
 * @param storage - data storage
 */
CAfSession* CAfSession::NewL(RFs& fileSession,
                             MAfTaskStorage& taskStorage,
                             CAfStorage& storage)
{
    CAfSession* self = new (ELeave) CAfSession(fileSession, 
                                               taskStorage, 
                                               storage);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

// -----------------------------------------------------------------------------
/**
 * EPOC default constructor for performing 2nd stage construction
 */
void CAfSession::ConstructL()
{
}

// -----------------------------------------------------------------------------
/**
 * Implements interface
 * @see void CSession2::ServiceL(const RMessage2&)
 */
void CAfSession::ServiceL(const RMessage2& message)
{
    switch (message.Function()) {
    case SaveActivity:
    case RemoveActivity:
    case RemoveApplicationActivities:
        {
        mStorage.InterruptCleanup();
        AfStorageSyncTask::ExecuteL(mTasksStorage, mStorage, message);
        mStorage.RequestCleanup();
        break;
        }
    
    case ApplicationActivity:
    case Activities:
    case ApplicationActivities:
        {
        TBool cleanupInterrupted = mStorage.InterruptCleanup();
        CAfStorageAsyncTask::ExecuteLD(*this, mStorage, message);
        if (cleanupInterrupted) mStorage.RequestCleanup();
        break;
        }
    
    case WaitActivity:
    case NotifyChange:
        CAfObserverTask::ExecuteLD(mTasksStorage, *this, message);
        break;
    
    case GetThumbnail:
        CAfThumbnailTask::ExecuteLD(*this, message);
        break;
    
    case LaunchActivity:
    case CancelWait:
    case CancelNotify:
        AfBroadcastTask::ExecuteL(mTasksStorage, message);
        break;
    
    case GetData:
        AfDataProviderTask::ExecuteL(*this,message);
        break;
    }
}

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see MAfTaskStorage::PushL(CAfTask *)
 */
void CAfSession::PushL(CAfTask * task)
{
    (KErrNotFound == mRunningTasks.Find(task)) ?
     mRunningTasks.AppendL(task) :
     User::Panic(KTaskAlreadyExists, KErrAlreadyExists);
}

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see MAfTaskStorage::Pop(CAfTask *)
 */
void CAfSession::Pop(CAfTask *task)
{
    const TInt offset(mRunningTasks.Find(task));
    if (KErrNotFound != offset) {
        mRunningTasks.Remove(offset);
    }
}

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see MAfTaskStorage::StorageData()
 */
const RPointerArray<CAfTask>& CAfSession::StorageData() const
{
    return mRunningTasks;
}

// -----------------------------------------------------------------------------
/**
 * Removes not valid task
 */
void CAfSession::RemoveNotValidTasks(const CSession2* session)
{
    if (session == this) {
        mRunningTasks.ResetAndDestroy();
    }
}
