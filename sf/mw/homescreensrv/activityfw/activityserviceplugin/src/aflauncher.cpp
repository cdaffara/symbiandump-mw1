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

#include "aflauncher.h"
#include "applicationlauncher_p.h"

AfLauncher::AfLauncher() : d_ptr(new ApplicationLauncherPrivate())
{
}

AfLauncher::~AfLauncher()
{
    delete d_ptr;
}

bool AfLauncher::isRunning(int applicationId)
{
    return d_ptr->isRunning(applicationId);
}

void AfLauncher::startApplication(int applicationId, const QUrl &uri)
{
    d_ptr->startApplication(applicationId, uri);
}

void AfLauncher::bringToForeground(int applicationId)
{
    d_ptr->bringToForeground(applicationId);
}
