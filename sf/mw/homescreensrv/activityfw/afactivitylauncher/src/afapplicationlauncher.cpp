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
#include "afapplicationlauncher.h"

#include <apacmdln.h>
#include <eikenv.h>
#include <apgtask.h>

// -----------------------------------------------------------------------------
//  Construction
// -----------------------------------------------------------------------------
//
TAfApplicationLauncher::TAfApplicationLauncher(RApaLsSession &apaLsSession, RWsSession &wsSession) : mApaLsSession(apaLsSession), mWsSession(wsSession)
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
//
TBool TAfApplicationLauncher::isRunning(TUid applicationId)
{
    TApaTaskList taskList(mWsSession);
    TApaTask task = taskList.FindApp(applicationId);
    return task.Exists();
}

void TAfApplicationLauncher::startApplicationL(TUid applicationId, const TDesC &uri)
{
    _LIT(KActivityMarker, "-activity ");
    RBuf commandLine;
    CleanupClosePushL(commandLine);    
    commandLine.ReAllocL(KActivityMarker().Length() + uri.Length());
    commandLine.Append(KActivityMarker());
    commandLine.Append(uri);
    
    TApaAppInfo appInfo;
    User::LeaveIfError(mApaLsSession.GetAppInfo(appInfo, applicationId));

    RProcess application;
    CleanupClosePushL(application);
    User::LeaveIfError(application.Create(appInfo.iFullName, commandLine));
    application.Resume();

    CleanupStack::PopAndDestroy(&application);
    CleanupStack::PopAndDestroy(&commandLine);
}
