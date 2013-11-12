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
#ifndef AFSERVER_H
#define AFSERVER_H

#define __E32SVR_H__

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <f32file.h>    // RFs
#include <apgcli.h>

#include "aftaskstorage.h"

class CAfStorage;

class CAfServer : public CPolicyServer,
                  public MAfTaskStorage
{
public:
    ~CAfServer();

    static CAfServer* NewLC();

public:
    void PushL(CAfTask *);
    
    void Pop(CAfTask *);
    
    const RPointerArray<CAfTask>& StorageData() const;
    

private:
    CAfServer();

    void ConstructL();

    CSession2* NewSessionL(const TVersion& version, const RMessage2& message) const;

    void RemoveNotValidTasks(const CSession2* session);

private:
    RApaLsSession mApaSession;
    RFs mFsSession;
    CAfStorage* mStorage;
    RPointerArray<CAfTask> mObservers;
    CBase* mAppEngine;
};

#endif // AFSERVER_H
