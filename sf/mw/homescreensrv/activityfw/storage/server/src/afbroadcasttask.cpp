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
#include "afbroadcasttask.h"

// -----------------------------------------------------------------------------
/**
 * Handle broadccast message request
 * @param storage - observer tasks storage
 * @param msg - request message that will be provided to observers
 */
void AfBroadcastTask::ExecuteL(MAfTaskStorage& storage, 
                               const RMessage2& msg)
{
    const RPointerArray<CAfTask> &tasks(storage.StorageData());
    for (TInt iter(tasks.Count() - 1); iter >= 0 ; --iter ) {
        (tasks[iter])->BroadcastReceivedL(msg);
    }
    msg.Complete(KErrNone);
}
