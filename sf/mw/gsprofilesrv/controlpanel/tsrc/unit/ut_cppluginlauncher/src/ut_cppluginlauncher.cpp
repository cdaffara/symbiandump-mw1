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

#include "ut_cppluginlauncher.h"

#include "cppluginlauncher.h"
#include "cpbasesettingview.h"

#include <QtTest/QtTest>
#include <hbdataform.h>
#include <hbpushbutton.h>
#include <HbMainWindow>

/*!
    \class TestCpPluginLauncher
    \brief The TestCpPluginLauncher class is designed for the unit testing for CpPluginLauncher class.
    This unit test is supported by QTest.
    There are total 3 test cases in this unit.
 */

void TestCpPluginLauncher::initTestCase()
    {
    mainWindow = new HbMainWindow;
    mainWindow->show();
    }

void TestCpPluginLauncher::cleanupTestCase()
    {
    delete mainWindow;
    
    //if delete mainwindow, test app will be freezing
    QCoreApplication::processEvents();
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;
        static CpBaseSettingView* launchSettingView(const QString &pluginFile,const QVariant &hint = QVariant());&nbsp;

   2. Function Descrition: \n &nbsp;&nbsp;
       2.1 Create view object by call launchSettingView().\n &nbsp;&nbsp;
       2.2 emit signal to CpBaseSettingView object.\n &nbsp;&nbsp;
       
   3. Input Parameters: \n&nbsp;
       QString &pluginFile: plugin file path, either relative or absolute.\n&nbsp;&nbsp;
       QVariant &hint: hint string, for example, "profile_view".\n&nbsp;&nbsp;
   
   4. Expected result: \n&nbsp;&nbsp;
       CpBaseSettingView object created successfully, signal emit out to view object processed correctly.\n 
 */
void TestCpPluginLauncher::testLaunchSettingViewWithCorrectProfileName()
    {
    CpBaseSettingView * pView1 = CpPluginLauncher::launchSettingView("testplugin.cpcfg", "profile_view");
    QVERIFY(pView1 != 0);
    
    CpBaseSettingView * pView2 = CpPluginLauncher::launchSettingView("testplugin.cpcfg", "profile_view");
    QVERIFY(pView2 != 0);
    
    QObject::connect(this, SIGNAL(connectAboutToClose()), pView1, SIGNAL(aboutToClose()));
    emit connectAboutToClose();
    
    QObject::connect(this, SIGNAL(connectAboutToClose()), pView2, SIGNAL(aboutToClose()));
    emit connectAboutToClose();
    emit connectAboutToClose();
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;
        static CpBaseSettingView* launchSettingView(const QString &pluginFile,const QVariant &hint = QVariant());&nbsp;

   2. Function Descrition: \n &nbsp;&nbsp;
       2.1 Create view object by call launchSettingView().\n &nbsp;&nbsp;
       2.2 emit signal to CpBaseSettingView object.\n &nbsp;&nbsp;
       
   3. Input Parameters: \n&nbsp;
       QString &pluginFile: plugin file path, either relative or absolute.\n&nbsp;&nbsp;
       QVariant &hint: hint string, for example, "profile_view".\n&nbsp;&nbsp;
   
   4. Expected result: \n&nbsp;&nbsp;
       CpBaseSettingView object created successfully, signal emit out to view object processed correctly.\n 
 */
void TestCpPluginLauncher::testLaunchSettingViewWithWrongProfileName()
    {
    CpBaseSettingView * pView1 = CpPluginLauncher::launchSettingView("testplugin.cpcfg", "myprofile_view");
    QVERIFY(pView1 == 0);
    
    CpBaseSettingView * pView2 = CpPluginLauncher::launchSettingView("testplugin.cpcfg", "myprofile_view");
    QVERIFY(pView2 == 0);
    
    QObject::connect(this, SIGNAL(connectAboutToClose()), pView1, SIGNAL(aboutToClose()));
    emit connectAboutToClose();
    
    QObject::connect(this, SIGNAL(connectAboutToClose()), pView2, SIGNAL(aboutToClose()));
    emit connectAboutToClose();
    emit connectAboutToClose();
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;
        static CpBaseSettingView* launchSettingView(const QString &pluginFile,const QVariant &hint = QVariant());&nbsp;

   2. Function Descrition: \n &nbsp;&nbsp;
       2.1 Create view object by call launchSettingView().\n &nbsp;&nbsp;
       2.2 emit signal to CpBaseSettingView object.\n &nbsp;&nbsp;
       
   3. Input Parameters: \n&nbsp;
       QString &pluginFile: plugin file path, either relative or absolute.\n&nbsp;&nbsp;
       QVariant &hint: hint string, for example, "profile_view".\n&nbsp;&nbsp;
   
   4. Expected result: \n&nbsp;&nbsp;
       CpBaseSettingView object created successfully, signal emit out to view object processed correctly.\n 
 */
void TestCpPluginLauncher::testLaunchSettingViewWithEmptyParam()
    {
    CpBaseSettingView * pView1 = CpPluginLauncher::launchSettingView("", "");
    QVERIFY(pView1 == 0);
    }

QTEST_MAIN(TestCpPluginLauncher)
