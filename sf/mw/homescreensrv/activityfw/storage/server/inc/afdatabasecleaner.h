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
#ifndef AFDATABASECLEANER_H
#define AFDATABASECLEANER_H

#include <e32base.h>
#include <d32dbms.h>

class CAfDatabaseCleaner : public CActive
{

private:
    enum CleanupStep {
        Initial,
        Continuation,
    };

public:
    CAfDatabaseCleaner(RDbStoreDatabase &database);
    ~CAfDatabaseCleaner();

    void StartCleanup();

private: // from CActive
    void RunL();
    void DoCancel();
    TInt RunError(TInt aError);
    
private:
    void CompleteSelf(TInt completionCode);

private:
    RDbStoreDatabase &mDatabase;
    RDbIncremental mIncrementalInterface;
    TInt mStep;
    CleanupStep mCleanupStep;
    
};

#endif // AFDATABASECLEANER_H
