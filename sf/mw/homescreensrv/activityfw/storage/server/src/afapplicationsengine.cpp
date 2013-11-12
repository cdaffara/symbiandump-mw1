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
#include "afapplicationsengine.h"
#include "afapplicationsstorage.h"
#include "afapplicationscollection.h"
#include "afapplicationsmonitor.h"
#include "aftaskstorage.h"
#include "aftask.h"

//------------------------------------------------------------------------------
CAfApplicationsEngine* CAfApplicationsEngine::NewL(RApaLsSession& serviceProvider, 
                                                   CAfStorage& storage, 
                                                   MAfTaskStorage& taskStorage)
{
    CAfApplicationsEngine *self = new(ELeave)CAfApplicationsEngine(taskStorage);
    CleanupStack::PushL(self);
    self->ConstructL(serviceProvider, storage);
    CleanupStack::Pop(self);
    return self;
}

//------------------------------------------------------------------------------
CAfApplicationsEngine::CAfApplicationsEngine(MAfTaskStorage& taskStorage) :
    mTaskStorage(taskStorage)
{
}

//------------------------------------------------------------------------------
void CAfApplicationsEngine::ConstructL(RApaLsSession& serviceProvider,
                                       CAfStorage& storage)
{
    mCollection = CAfApplicationsCollection::NewL(serviceProvider);
    mMonitor = CAfApplicationsMonitor::NewL(serviceProvider, *mCollection);
    mStorage = CAfApplicationsStorage::NewL(storage, *mCollection, *this);
    
    mCollection->setObserver(*mStorage);
}

//------------------------------------------------------------------------------
CAfApplicationsEngine::~CAfApplicationsEngine()
{
    delete mMonitor;
    delete mStorage;
    delete mCollection;
}

//------------------------------------------------------------------------------
void CAfApplicationsEngine::applicationsChanged()
{
    const RPointerArray<CAfTask> &table(mTaskStorage.StorageData());
    for (TInt iter(table.Count() - 1); 0 <= iter; --iter) {
        table[iter]->BroadcastReceivedL(RMessage2(), ETrue);
    }
}
