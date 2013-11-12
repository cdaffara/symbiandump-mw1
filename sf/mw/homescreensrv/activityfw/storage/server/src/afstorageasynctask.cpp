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
#include <s32mem.h>

#include "afstorageasynctask.h"
#include "afcmd.h"

// -----------------------------------------------------------------------------
/**
 * Constructor for performing 1st stage construction
 */
CAfStorageAsyncTask::CAfStorageAsyncTask()
{
    // No implementation required
}

// -----------------------------------------------------------------------------
/**
 * Destructor.
 */
CAfStorageAsyncTask::~CAfStorageAsyncTask()
{
    mExternalizedData.Close();
    mInternalizedData.ResetAndDestroy();
}

// -----------------------------------------------------------------------------
/**
 * Handle asynchronous data storage requests
 * @param taskStorage - data tasks storage
 * @param dataStorage - data storage
 * @param msg - request message
 */
void CAfStorageAsyncTask::ExecuteLD(MAfTaskStorage& taskStorage, 
                                    CAfStorage& dataStorage, 
                                    const RMessage2& msg)
{
    CAfStorageAsyncTask* self = new (ELeave) CAfStorageAsyncTask();
    CleanupStack::PushL(self);
    self->ExecuteL(dataStorage, msg);
    taskStorage.PushL(self);
    CleanupStack::Pop(self);
    msg.Complete(KErrNone);
}

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see CActivityTask::Data()
 */
const TDesC8& CAfStorageAsyncTask::CAfStorageAsyncTask::Data() const
{
    return mExternalizedData;
}

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see CActivityTask::BroadcastReceivedL(const RMessage2 &)
 */
void CAfStorageAsyncTask::BroadcastReceivedL(const RMessage2&, TBool)
{
    // No implementation required
}

// -----------------------------------------------------------------------------
/**
 * Handle asynchronous data storage requests
 * @param dataStorage - data storage
 * @param msg - request message
 */
void CAfStorageAsyncTask::ExecuteL(CAfStorage& dataStorage, 
                                   const RMessage2& msg)
{
    switch (msg.Function()) {
    case Activities:
        AllActivitiesL(dataStorage, msg);
        break;
    case ApplicationActivities:
        ApplicationActivitiesL(dataStorage, msg);
        break;
    case ApplicationActivity:
        ApplicationActivityL(dataStorage, msg);
        break;
    };
}

// -----------------------------------------------------------------------------
/**
 * Handle getting all activities request
 * @param dataStorage - data storage
 * @param msg - requested message
 */
void CAfStorageAsyncTask::AllActivitiesL(CAfStorage& dataStorage, 
                                         const RMessage2& msg)
{
    dataStorage.AllActivitiesL(mInternalizedData, msg.Int3());
    ExternalizeL();
    WriteResponseL(msg);
}

// -----------------------------------------------------------------------------
/**
 * Handle getting application activities request
 * @param dataStorage - data storage
 * @param msg - requested message
 */
void CAfStorageAsyncTask::ApplicationActivitiesL(CAfStorage& dataStorage, 
                                                 const RMessage2& msg)
{
    CAfEntry *entry = CAfEntry::NewLC(msg);
    dataStorage.ActivitiesL(mInternalizedData, entry->ApplicationId());
    CleanupStack::PopAndDestroy(entry);
    ExternalizeL();
    WriteResponseL(msg);
}

// -----------------------------------------------------------------------------
/**
 * Handle getting application activity request
 * @param dataStorage - data storage
 * @param msg - requested message
 */
void CAfStorageAsyncTask::ApplicationActivityL(CAfStorage& dataStorage, 
                                               const RMessage2& msg)
{
    CAfEntry *src(CAfEntry::NewLC(msg));
    dataStorage.ActivityL(mInternalizedData, *src);
    CleanupStack::PopAndDestroy(src);
    ExternalizeL();
    WriteResponseL(msg);
}

// -----------------------------------------------------------------------------
void CAfStorageAsyncTask::ExternalizeL()
{
    mExternalizedData << mInternalizedData;
    mInternalizedData.ResetAndDestroy();
}

// -----------------------------------------------------------------------------
/**
 * Write response data into request message
 * @param msg - destination message
 */
void CAfStorageAsyncTask::WriteResponseL(const RMessage2& msg)
{
    msg.WriteL(1, 
               TPckgBuf<TInt>(mExternalizedData.Length()));//write data size
    msg.WriteL(2, 
               TPckgBuf<CBase*>(this));//task identyfier
}

// -----------------------------------------------------------------------------
/**
 * Returns ETrue if task is related with session argument
 */

TBool CAfStorageAsyncTask::IsSessionTask(const CSession2* /*session*/)
{
	return EFalse;
}
