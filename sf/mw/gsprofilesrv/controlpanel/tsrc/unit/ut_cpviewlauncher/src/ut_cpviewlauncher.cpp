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
#include "ut_cpviewlauncher.h"

#include "cpviewlauncher.h"
#include "cpbasesettingview.h"

#include <QtTest/QtTest>
#include <hbdataform.h>
#include <hbpushbutton.h>
#include <HbMainWindow>
#include <QSignalSpy>
#include <QList>

/*!
    \class TestCpViewLauncher
    \brief This class is used for the unit test for class CpViewLauncher.\n
      CpViewLauncher class is used for add a HbView into current HbMainWindow, \n
      and set the new added HbView instance as current view object, then completed \n
      related signal/slot setting.\n 
      This unit test is supported by QTest.\n
      There are total 3 test cases in this unit.\n
 */

/*!
 * init function called by QTest framework.
 */
void TestCpViewLauncher::initTestCase()
    {
    }

/*!
 * cleanup function used by QTest framework.
 */
void TestCpViewLauncher::cleanupTestCase()
    {
    QCoreApplication::processEvents();
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
      static void launchView(HbView *view);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        Launch an exist CpBaseSettingView object, add the CpBaseSettingView object into HbMainWindow.\n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
       @param: HbView *view\n
       
   4. Expected result: \n&nbsp;&nbsp;
       View can't be launched for there is no HbMainWindow created.\n 
 */
void TestCpViewLauncher::testLaunchViewNoMainWindow()
    {
    CpBaseSettingView * pView1 = new CpBaseSettingView(0);
    QVERIFY( pView1 != 0 );
    HbDataForm *pForm = new HbDataForm(0);
    QVERIFY( pForm != 0 );
    
    pView1->setWidget(pForm);
    HbDataForm *pForm1 = qobject_cast<HbDataForm *>(pView1->widget());
    QVERIFY( pForm1 != 0);
    QVERIFY( pForm1 == pForm );
    
    CpBaseSettingView * pView2 = new CpBaseSettingView(pForm);
    QVERIFY( pView2 != 0 );
    
    QObject::connect(this, SIGNAL(callAboutToClose()), pView1, SIGNAL(aboutToClose()));
    QObject::connect(this, SIGNAL(callAboutToClose()), pView2, SIGNAL(aboutToClose()));

    CpViewLauncher::launchView(pView1);
    CpViewLauncher::launchView(pView2);
    
    emit callAboutToClose();
    emit callAboutToClose();
    
    delete pView1;
    delete pView2;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
      static void launchView(HbView *view);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
       Launch an exist CpBaseSettingView object, add the CpBaseSettingView object into HbMainWindow.\n&nbsp;
   3. Pre-condition: \n &nbsp;&nbsp;
       HbMainWindow object created correctly.
   
   4. Input: a pointer to CpBaseSettingView object\n&nbsp;&nbsp;
       @param: HbView *view\n
       
   5. Expected result: \n&nbsp;&nbsp;
       View launched successfully.\n 
 */
void TestCpViewLauncher::testLaunchViewWithMainWindow()
    {
    mainWindow = new HbMainWindow;
    QVERIFY( mainWindow != 0 );
    
    mainWindow->show();
    
    CpBaseSettingView * pView1 = new CpBaseSettingView(0);
    QVERIFY( pView1 != 0 );
    HbDataForm *pForm = new HbDataForm(0);
    QVERIFY( pForm != 0 );
    
    pView1->setWidget(pForm);
    HbDataForm *pForm1 = qobject_cast<HbDataForm *>(pView1->widget());
    QVERIFY( pForm1 != 0);
    QVERIFY( pForm1 == pForm );
    
    CpBaseSettingView * pView2 = new CpBaseSettingView(pForm);
    QVERIFY( pView2 != 0 );
    
    QObject::connect(this, SIGNAL(callAboutToClose()), pView1, SIGNAL(aboutToClose()));
    QObject::connect(this, SIGNAL(callAboutToClose()), pView2, SIGNAL(aboutToClose()));
    
    QObject::connect(this, SIGNAL(callAboutToClose()), qApp, SLOT(quit()));
    
    CpViewLauncher::launchView(pView1);
    CpViewLauncher::launchView(pView2);
    
    emit callAboutToClose();
    emit callAboutToClose();
    
    delete mainWindow;
    mainWindow = 0;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
      static void launchView(HbView *view);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        Launch an exist CpBaseSettingView object, add the CpBaseSettingView object into HbMainWindow.\n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
       @param: 0\n
   4. Expected result: \n&nbsp;&nbsp;
       launchView do nothing for the 0 parameter.\n 
 */
void TestCpViewLauncher::testLaunchViewNoView()
    {
    HbPushButton *widget = new HbPushButton();
    QVERIFY(widget != 0);
    CpBaseSettingView *pView1 = new CpBaseSettingView(widget);
    QVERIFY(pView1!=0);
    HbPushButton *button = qobject_cast<HbPushButton *>(pView1->widget());
    QVERIFY(button != 0);
    QVERIFY(button == widget);
    
    CpViewLauncher::launchView(0);
    
    delete pView1;
    }

QTEST_MAIN(TestCpViewLauncher)
