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
 * Description :
 *
 */
#include "tsresourcemanagerimp.h"
#include "tswindowgroupsmonitorimp.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CTsResourceManager* CTsResourceManager::NewL()
{
    CTsResourceManager* self =new(ELeave) CTsResourceManager();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CTsResourceManager::ConstructL()
{
    User::LeaveIfError(mWsSession.Connect());
    User::LeaveIfError(mApaSeesion.Connect());
    iMonitor = CTsWindowGroupsMonitor::NewL(*this);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CTsResourceManager::~CTsResourceManager()
{
    delete iMonitor;
    mApaSeesion.Close();
    mWsSession.Close();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
RWsSession& CTsResourceManager::WsSession()
{
    return mWsSession;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
RApaLsSession& CTsResourceManager::ApaSession()
{
    return mApaSeesion;
}

// -----------------------------------------------------------------------------
MTsWindowGroupsMonitor& CTsResourceManager::WsMonitor()
    {
    return *iMonitor;
    }
