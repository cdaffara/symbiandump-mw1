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
* Description:  Panic codes for OOM monitor.
*
*/


#ifndef OOMPANIC_
#define OOMPANIC_

#include <e32def.h>

enum TOomMonitorPanic
    {
    KRuleConfiguredBeforeApplication,
    KCloseAppActionIsResued,
    KAppConfigAddedTwice,
    KSameAppClosureConfiguredTwice,
    KOomDefaultAppCloseNotConfigured,
    KPluginConfigAddedTwice,
    KRuleConfiguredBeforePlugin,
    KOomDefaultAppNotConfigured,
    KOomDefaultPluginNotConfigured,
    KNoCoeEnvFound,
    KInvalidWgName,
    KOomInvalidPriority,
    KAppCloseActionEqualPriorities,
    KClientQueueNotEmpty,
    KClientRequestTimerActive,
    KInvalidClientRequestType,
    KPluginArrayIndexOutOfBounds,
    KWindowGroupArrayIndexOutOfBounds,
    KStartingActiveCloseAppTimer,
    KStartingActiveAppCloseWatcher,
    KStartingActivePluginWaiter
    };

void OomMonitorPanic(TOomMonitorPanic aReason);
void OomConfigParserPanic(TInt aReason);

#endif /*OOMPANIC_*/
