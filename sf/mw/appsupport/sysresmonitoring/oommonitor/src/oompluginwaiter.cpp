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
* Description:  Classes for executing OOM actions (e.g. closing applications and running plugins).
*
*/


#include "oompluginwaiter.h"
#include "oomrunplugin.h"
#include "OomTraces.h"
#include "oomconstants.hrh"

COomPluginWaiter* COomPluginWaiter::NewL(TInt aMillisecondsToWait, COomRunPlugin& aCallbackClass)
    {
    FUNC_LOG;

    COomPluginWaiter* self = new (ELeave) COomPluginWaiter(aMillisecondsToWait, aCallbackClass);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

COomPluginWaiter::COomPluginWaiter(TInt aMillisecondsToWait, COomRunPlugin& aCallbackClass) : CTimer(EPriorityStandard), iMillisecondsToWait(aMillisecondsToWait), iCallbackClass(aCallbackClass)
    {
    FUNC_LOG;

    CActiveScheduler::Add(this);
    }

// Start the timer, it will call the plugin back when it expires
void COomPluginWaiter::Start()
    {
    FUNC_LOG;

    HighRes(iMillisecondsToWait * KMicrosecondsInMillisecond);
    }
    
void COomPluginWaiter::RunL()
    {
    FUNC_LOG;

    iCallbackClass.WaitCompleted();
    }
