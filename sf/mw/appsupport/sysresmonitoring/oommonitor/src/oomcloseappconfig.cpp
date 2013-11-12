/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Configuration representation classes for Out of Memory Monitor.
*
*/



#include "oomcloseappconfig.h"
#include "OomTraces.h"

COomCloseAppConfig* COomCloseAppConfig::NewL(TInt32 aId)
    {
    FUNC_LOG;

    COomCloseAppConfig* self = new (ELeave) COomCloseAppConfig(aId);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

COomCloseAppConfig::~COomCloseAppConfig()
    {
    FUNC_LOG;
    }

COomCloseAppConfig::COomCloseAppConfig(TInt32 aId) : COomActionConfig(aId)
    {
    FUNC_LOG;
    }
