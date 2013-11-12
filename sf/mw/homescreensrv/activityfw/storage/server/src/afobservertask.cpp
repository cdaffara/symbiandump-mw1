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
#include "afobservertask.h"
#include "afcmd.h"
#include "afentry.h"
// -----------------------------------------------------------------------------
/**
 * Constructor for performing 1st stage construction
 * @param globalStorage - global observers storage
 * @param localStorage - local observers storage
 * @param msg - request message
 */
CAfObserverTask::CAfObserverTask(MAfTaskStorage& globalStorage, 
                                 MAfTaskStorage& localStorage, 
                                 const RMessage2& msg)
:
    mGlobalStorage(globalStorage),
    mLocalStorage(localStorage),
    mMsg(msg)
{
    // No implementation required
}

// -----------------------------------------------------------------------------
/**
 * Destructor.
 */
CAfObserverTask::~CAfObserverTask()
{
    mData.Close();
}

// -----------------------------------------------------------------------------
/**
 * Handle observer request. 
 * @param globalStorage - global observers storage
 * @param localStorage - local observers storage
 * @param msg - request message
 */

void CAfObserverTask::ExecuteLD(MAfTaskStorage& globalStorage, 
                                MAfTaskStorage& localStorage, 
                                const RMessage2& msg)
{
    CAfObserverTask *self = new(ELeave)CAfObserverTask(globalStorage, 
                                                       localStorage, 
                                                       msg);
    CleanupStack::PushL(self);
    globalStorage.PushL(self);
    CleanupStack::Pop(self);
}

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see CActivityTask::Data()
 */
const TDesC8& CAfObserverTask::Data() const
{
    return mData;
}

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see CActivityTask::BroadcastReceivedL(const RMessage2&)
 */
void CAfObserverTask::BroadcastReceivedL(const RMessage2& msg, TBool applicationUninstalled)
{
    if (!applicationUninstalled && mMsg.IsNull()) {
        mGlobalStorage.Pop(this);//
        mLocalStorage.Pop(this);
        delete this;
    } else if(NotifyChange == mMsg.Function() &&
              (applicationUninstalled ||
               SaveActivity == msg.Function() ||
               RemoveActivity == msg.Function() ||
               RemoveApplicationActivities == msg.Function())){
        mMsg.Complete(KErrNone);
        mGlobalStorage.Pop(this);
        delete this;
    } else if (WaitActivity == mMsg.Function() &&
               LaunchActivity == msg.Function()) {
               
        CAfEntry* entry = CAfEntry::NewLC(msg);
        TPckgBuf<TInt> observerdId;
        mMsg.ReadL(KRequestAppIdOffset, observerdId);
        if (observerdId() == entry->ApplicationId()) {
            CAfEntry::ReallocL(mData, entry->ActivityId().Length());
            mData.Copy(entry->ActivityId());
            WriteResponseL();
            mLocalStorage.PushL(this);
            mGlobalStorage.Pop(this);
            mMsg.Complete(KErrNone);
        }
        CleanupStack::PopAndDestroy(entry);
        
    } else if (WaitActivity == mMsg.Function() &&
               CancelWait == msg.Function() &&
               mMsg.Session() == msg.Session()) {
        mGlobalStorage.Pop(this);
        mMsg.Complete(KErrCancel);
        delete this;
    } else if (NotifyChange == mMsg.Function() &&
               CancelNotify == msg.Function() &&
               mMsg.Session() == msg.Session()) {
        mGlobalStorage.Pop(this);
        mMsg.Complete(KErrCancel);
        delete this;
    }
}

// -----------------------------------------------------------------------------
/**
 * Write response data to requested message 
 */
void CAfObserverTask::WriteResponseL()
{
    mMsg.WriteL(KResponseDataSizeOffset, TPckgBuf<TInt>(mData.Length()));//write data size
    mMsg.WriteL(KResponseIdOffset, TPckgBuf<CBase*>(this));//task identyfier
}

// -----------------------------------------------------------------------------
/**
 * Returns ETrue if task is related with session argument
 */
TBool CAfObserverTask::IsSessionTask(const CSession2* session)
{
    return mMsg.Session() == session ? ETrue : EFalse;
}
