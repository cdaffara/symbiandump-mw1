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
#ifndef AFSTORAGEASYNCTASK_H
#define AFSTORAGEASYNCTASK_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include "aftask.h"
#include "aftaskstorage.h"
#include "afstorage.h"

class CAfStorageAsyncTask : public CAfTask
{
public:
    ~CAfStorageAsyncTask();

    static void ExecuteLD(MAfTaskStorage& taskStorage, 
                          CAfStorage& dataStorage, 
                          const RMessage2& msg);

    const TDesC8& Data() const;

    void BroadcastReceivedL(const RMessage2 &, TBool applicationUninstalled = EFalse);

private:
    CAfStorageAsyncTask();

    void ExecuteL(CAfStorage& dataStorage, const RMessage2& msg);

    void AllActivitiesL(CAfStorage& dataStorage, const RMessage2& msg);

    void ApplicationActivitiesL(CAfStorage& dataStorage, const RMessage2& msg);

    void ApplicationActivityL(CAfStorage& dataStorage, const RMessage2& msg);

    void WriteResponseL(const RMessage2& msg);

    TBool IsSessionTask(const CSession2* session);

    void ExternalizeL();

private:
    RBuf8 mExternalizedData;
    RPointerArray<CAfEntry> mInternalizedData;
};

#endif // AFSTORAGEASYNCTASK_H
