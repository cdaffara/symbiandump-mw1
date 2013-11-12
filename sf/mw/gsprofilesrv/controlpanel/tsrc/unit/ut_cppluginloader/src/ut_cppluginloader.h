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
* Description:  
*       test application for qt control panel public apis.
*/

#ifndef UT_CP_PLUGIN_LOADER_H
#define UT_CP_PLUGIN_LOADER_H

#include <QObject>

class TestCpPluginLoader : public QObject
{
    Q_OBJECT
    
private slots:
    void initTestCase();
    void cleanupTestCase();
    
//public slots:
    void testLoadCpPluginInterfaceWithCpcfgFilename();
    void testLoadCpPluginInterfaceWithAbsoluteQtpluginPath();
    void testLoadCpPluginInterfaceWithQtpluginFilename();
    
    void testLoadCpLauncherInterfaceWithCpcfgFilename();
    void testLoadCpLauncherInterfaceWithAbsoluteQtpluginFilename();
    void testLoadCpLauncherInterfaceWithQtpluginFilename();
    
};

#endif // UT_CP_PLUGIN_LOADER_H
