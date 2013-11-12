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
#ifndef AFOBSERVERTASK_H
#define AFOBSERVERTASK_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include "aftask.h"
#include "aftaskstorage.h"

class CAfObserverTask : public CAfTask
{
public:
    ~CAfObserverTask();

    static void ExecuteLD(MAfTaskStorage& globalStorage, 
                          MAfTaskStorage& localStorage,
                          const RMessage2& msg);

    const TDesC8& Data() const;

    void BroadcastReceivedL(const RMessage2&, TBool applicationUninstalled = EFalse);

private:
    CAfObserverTask(MAfTaskStorage& globalStorage, 
                    MAfTaskStorage& localStorage,
                    const RMessage2& msg);

    void WriteResponseL();

    TBool IsSessionTask(const CSession2* session);

private:
    MAfTaskStorage& mGlobalStorage;
    MAfTaskStorage& mLocalStorage;
    const RMessage2 mMsg;
    RBuf8 mData;
};

#endif // AFOBSERVERTASK_H
