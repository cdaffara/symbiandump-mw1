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
#ifndef AFSESSION_H
#define AFSESSION_H

#ifndef __E32SVR_H__
#define __E32SVR_H__
#endif 
// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <f32file.h>

#include "aftaskstorage.h"

class CAfStorage;

class CAfSession : public CSession2,
                   public MAfTaskStorage
{
public:
    ~CAfSession();

    static CAfSession* NewL(RFs& fileSession,
                            MAfTaskStorage& taskStorage, 
                            CAfStorage&storage);

public:
    void PushL(CAfTask *);

    void Pop(CAfTask *);

    const RPointerArray<CAfTask>& StorageData() const;

    void RemoveNotValidTasks(const CSession2* session);

private:
    CAfSession(RFs& fileSession,
               MAfTaskStorage& taskStorage, 
               CAfStorage& storage);

    void ConstructL();

private:
    void ServiceL(const RMessage2& message);

private:
    RFs& mFileSession; 
    MAfTaskStorage& mTasksStorage;
    CAfStorage& mStorage;
    RPointerArray<CAfTask> mRunningTasks;
};

#endif // AFSESSION_H
