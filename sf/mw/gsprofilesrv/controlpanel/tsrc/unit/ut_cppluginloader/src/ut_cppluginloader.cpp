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

#include "ut_cppluginloader.h"
#include "cppluginloader.h"

#include <QtTest/QtTest>


/*!
    \class TestCpPluginLoader
    \brief The TestCpPluginLoader class is designed for the unit testing for CpPluginLoader class.
    This unit test is supported by QTest.
    There are total 6 test cases in this unit.
 */

void TestCpPluginLoader::initTestCase()
    {
    }

void TestCpPluginLoader::cleanupTestCase()
    {
    QCoreApplication::processEvents();
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: loadCpPluginInterface() \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
        load a plugin file, which is derived from CpPluginInterface class.\n&nbsp;
   3. Input Parameters: cpcfg file name. \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpPluginLoader::testLoadCpPluginInterfaceWithCpcfgFilename()
    {
    CpPluginInterface * plugin = CpPluginLoader::loadCpPluginInterface("testplugin.cpcfg");
    QVERIFY(plugin != 0);
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: loadCpPluginInterface() \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
        load a plugin file, which is derived from CpPluginInterface class.\n&nbsp;
   3. Input Parameters: absolute path of cpcfg file name. \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpPluginLoader::testLoadCpPluginInterfaceWithAbsoluteQtpluginPath()
    {
    CpPluginInterface * plugin = CpPluginLoader::loadCpPluginInterface("/resource/qt/plugins/controlpanel/testplugin.qtplugin");
    QVERIFY(plugin != 0);
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: loadCpPluginInterface() \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
        load a plugin file, which is derived from CpPluginInterface class.\n&nbsp;
   3. Input Parameters: qtplugin file name. \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpPluginLoader::testLoadCpPluginInterfaceWithQtpluginFilename()
    {
    CpPluginInterface * plugin = CpPluginLoader::loadCpPluginInterface("testplugin.qtplugin");
    QVERIFY(plugin != 0);
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: loadCpLauncherInterface() \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
        load a plugin file, which is derived from CpLauncherInterface class.\n&nbsp;
   3. Input Parameters: defined cpcfg file name. \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpPluginLoader::testLoadCpLauncherInterfaceWithCpcfgFilename()
    {
    CpLauncherInterface * plugin = CpPluginLoader::loadCpLauncherInterface("testplugin.cpcfg");
    QVERIFY(plugin != 0);
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: loadCpLauncherInterface() \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
        load a plugin file, which is derived from CpLauncherInterface class.\n&nbsp;
   3. Input Parameters: absolute path of a user defined qtplugin file name. \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpPluginLoader::testLoadCpLauncherInterfaceWithAbsoluteQtpluginFilename()
    {
    CpLauncherInterface * plugin = CpPluginLoader::loadCpLauncherInterface("/resource/qt/plugins/controlpanel/testplugin.qtplugin");
    QVERIFY(plugin != 0);
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: loadCpLauncherInterface() \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
        load a plugin file, which is derived from CpLauncherInterface class.\n&nbsp;
   3. Input Parameters: qtplugin file name. \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpPluginLoader::testLoadCpLauncherInterfaceWithQtpluginFilename()
    {
    CpLauncherInterface * plugin = CpPluginLoader::loadCpLauncherInterface("testplugin.qtplugin");
    QVERIFY(plugin != 0);
    }

QTEST_APPLESS_MAIN(TestCpPluginLoader)
