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
* Description:  Timer class which implements a timeout for an application close action.
*
*/


#include "oomappclosetimer.h"
#include "oomcloseapp.h"
#include "OomTraces.h"

COomAppCloseTimer* COomAppCloseTimer::NewL(COomCloseApp& aMonitor)
    {
    FUNC_LOG;

    COomAppCloseTimer* self = new (ELeave) COomAppCloseTimer(aMonitor);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

COomAppCloseTimer::COomAppCloseTimer(COomCloseApp& aMonitor)
: CTimer(CActive::EPriorityStandard), iMonitor(aMonitor)
    {
    FUNC_LOG;

    CActiveScheduler::Add(this);
    }

void COomAppCloseTimer::RunL()
    {
    FUNC_LOG;

    iMonitor.CloseAppEvent();
    }
