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
#include "tstasksettings_p.h"

#include <fbs.h>
#include <coecntrl.h>
#include <QScopedPointer>
#include <QApplication>
#include <QDesktopWidget>

#include "tsscreenshotclient.h"
#include "tstaskmonitorglobals.h"

TsTaskSettingsPrivate::TsTaskSettingsPrivate() : mClient(0)
{
    QT_TRAP_THROWING(mClient = CTsScreenshotClient::NewL());
}

TsTaskSettingsPrivate::~TsTaskSettingsPrivate()
{
    delete mClient;
}

bool TsTaskSettingsPrivate::registerScreenshot(const QPixmap &screenshot)
{    
    QScopedPointer<CFbsBitmap> bitmap(screenshot.toSymbianCFbsBitmap());
    if (bitmap.isNull()) 
        return false;

    return KErrNone == mClient->RegisterScreenshot(bitmap.data(), wgId());
}

bool TsTaskSettingsPrivate::unregisterScreenshot()
{
    return KErrNone == mClient->UnregisterScreenshot(wgId());
}

bool TsTaskSettingsPrivate::setVisibility(bool visibility)
{
    return (KErrNone == mClient->SetVisibility(wgId(), visibility ? Visible : Invisible));
}

int TsTaskSettingsPrivate::wgId()
{
    return QApplication::desktop()->winId()->ControlEnv()->RootWin().WindowGroupId();
}
