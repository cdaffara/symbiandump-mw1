/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Launch controlpanel plugin setting view in client process.
*
*/

#include <cppluginlauncher.h>
#include <cpplugininterface.h>
#include <cplauncherinterface.h>
#include <QSharedPointer>
#include <cppluginloader.h>
#include <cpbasesettingview.h>
#include <cpitemdatahelper.h>
#include "cpviewlauncher.h"

/*!
    \class CpPluginLauncher
    \brief The CpPluginLauncher class loads a controlpanel plugin at run-time.And display the specify plugin view in client process.
*/

/*!
    Load and display a plugin view in client process. The client must be a orbit based application.
    The pluginFile can either absoulte file path or only file name. 
    Acceptable format:
        sampleplugin
        sampleplugin.qtplugin
        sampleplugin.dll
        C:/resource/qt/plugins/controlpanel/sampleplugin.qtplugin
        C:/resource/qt/plugins/controlpanel/sampleplugin.dll
*/
CpBaseSettingView* CpPluginLauncher::launchSettingView(const QString &pluginFile,const QVariant &hint /*= QVariant()*/)
{
    CpLauncherInterface *plugin = CpPluginLoader::loadCpLauncherInterface(pluginFile);
    if (plugin) {
        CpBaseSettingView *view = plugin->createSettingView(hint);
        if (view) {
            CpViewLauncher::launchView(view);
            return view;
        }
    }
    
    return 0;
}

//End of File
