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
#include <QCoreApplication>

ApplicationLauncherPrivate *ApplicationLauncherPrivate::instance = NULL;
bool ApplicationLauncherPrivate::isRunningReturnValue = false;

ApplicationLauncherPrivate::ApplicationLauncherPrivate()
{
    if (instance) {
        delete instance;
    }
    instance = this;
}

ApplicationLauncherPrivate::~ApplicationLauncherPrivate()
{
    // create copy so we can check output even when original object is deleted
    instance = new ApplicationLauncherPrivate(*this);
}

bool ApplicationLauncherPrivate::isRunning(int applicationId)
{
    QVariantHash call;
    call.insert("applicationId", applicationId);
    isRunningCalls.append(call);

    return isRunningReturnValue;
}

void ApplicationLauncherPrivate::startApplication(int applicationId, const QUrl &uri)
{
    QVariantHash call;
    call.insert("applicationId", applicationId);
    call.insert("uri", uri);
    startApplicationCalls.append(call);
}

void ApplicationLauncherPrivate::bringToForeground(int applicationId)
{
    QVariantHash call;
    call.insert("applicationId", applicationId);
    bringToForegroundCalls.append(call);
}
