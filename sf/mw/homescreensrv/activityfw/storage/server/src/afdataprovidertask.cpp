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
#include "afdataprovidertask.h"
#include "afcmd.h"

// -----------------------------------------------------------------------------
/**
 * Handle data request. Deliver data stored in other task.
 * @param storage - data tasks storage
 * @param msg - request message
 */
void AfDataProviderTask::ExecuteL(MAfTaskStorage& storage, 
                                  const RMessage2& msg)
{
    TPckgBuf<void*> requestId;
    CAfTask* taskPtr(0);
    msg.Read(KRequestIdOffset, requestId);
    
    for (TInt iter(0);iter <storage.StorageData().Count();++iter) {
        taskPtr = storage.StorageData()[iter];
        if (taskPtr == requestId()) {//compare requested task address with storage objects
            ProvideDataL(msg, *taskPtr);
            storage.Pop(taskPtr);
            delete taskPtr;
            return;
        }
    }
    User::Leave(KErrNotFound);
}

// -----------------------------------------------------------------------------
/**
 * Write response data to request message
 * @param msg - destination message
 * @param src - data source
 */
void AfDataProviderTask::ProvideDataL(const RMessage2& msg, 
                                      const CAfTask& src)
{
    msg.WriteL(KResponseDataOffset, src.Data());
    msg.Complete(KErrNone);
}
