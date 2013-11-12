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
#include "applicationlauncher_p.h"

#include <apgcli.h>
#include <apacmdln.h>
#include <apgtask.h>
#include <eikenv.h>

#include <XQConversions>

bool ApplicationLauncherPrivate::isRunning(int applicationId)
{
    TApaTaskList taskList(CEikonEnv::Static()->WsSession());
    TApaTask task = taskList.FindApp(TUid::Uid(applicationId));
    return task.Exists();
}

void ApplicationLauncherPrivate::startApplication(int applicationId, const QUrl &uri)
{
    QString commandLine = QString("-activity %1").arg(QString(uri.toEncoded()));

    TRAP_IGNORE(
        HBufC *commandLineAsDescriptor = XQConversions::qStringToS60Desc(commandLine);
        CleanupStack::PushL(commandLineAsDescriptor);

        RApaLsSession apaLsSession;
        User::LeaveIfError(apaLsSession.Connect());
        CleanupClosePushL(apaLsSession);

        TApaAppInfo appInfo;
        User::LeaveIfError(apaLsSession.GetAppInfo(appInfo, TUid::Uid(applicationId)));

        RProcess application;
        User::LeaveIfError(application.Create(appInfo.iFullName, *commandLineAsDescriptor));
        application.Resume();

        CleanupStack::PopAndDestroy(&apaLsSession);
        CleanupStack::PopAndDestroy(commandLineAsDescriptor);
    );
}

void ApplicationLauncherPrivate::bringToForeground(int applicationId)
{
    TApaTaskList taskList(CEikonEnv::Static()->WsSession());
    TApaTask task = taskList.FindApp(TUid::Uid(applicationId));
    if (task.Exists()) {
        task.BringToForeground();
    } else {
        qCritical("Cannot bring to forward task %08x", applicationId);
    }
}
