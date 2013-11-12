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

#include "ut_cpbasesettingview.h"

#include "cpbasesettingview.h"
#include "mycpbasesettingview.h"

#include <QtTest/QtTest>
#include <hbdataform.h>
#include <hbpushbutton.h>

/*!
    \class TestCpBaseSettingView
    \brief \n
    The TestCpBaseSettingView is used for the unit testing for CpBaseSettingView class.
    This unit test is supported by QTest.
    There are 3 test cases for this unit.
 */

void TestCpBaseSettingView::initTestCase()
    {

    }

void TestCpBaseSettingView::cleanupTestCase()
    {
    QCoreApplication::processEvents();
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: CpBaseSettingView() \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
       2.1 Create CpBaseSettingView object with 0 pointer.\n &nbsp;&nbsp;
       2.2 call setWidget to set a widget into the view object.\n &nbsp;&nbsp;
       2.3 verify widget object is set correctly.\n &nbsp;&nbsp;
       2.4 delete the object.\n &nbsp;
   3. Input Parameters: \n&nbsp;
       @param: 0\n
   4. Expected result: \n&nbsp;&nbsp;
       CpBaseSettingView object created successfully.\n 
 */
void TestCpBaseSettingView::testCpBaseSettingViewWithNullPointer()
    {
    CpBaseSettingView * pView = new CpBaseSettingView(0);
    QVERIFY( pView != 0 );
    
    HbDataForm *pForm = new HbDataForm(0);
    pView->setWidget(pForm);
    HbDataForm *pForm1 = qobject_cast<HbDataForm *>(pView->widget());
    QVERIFY( pForm1 != 0);
    QVERIFY( pForm1 == pForm );
    
    delete pView;
    pView = 0;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: CpBaseSettingView() \n&nbsp;
   2. Function Descrition: \n&nbsp;&nbsp;
       2.1 Create CpBaseSettingView object with a widget pointer.\n &nbsp;&nbsp;
       2.2 verify widget object is set correctly.\n &nbsp;&nbsp;
       2.3 delete the object.\n &nbsp;
   3. Input Parameters: \n&nbsp;
       @param: widget pointer. \n
   4. Expected result: \n&nbsp;&nbsp;
       CpBaseSettingView object created successfully.\n 
 */
void TestCpBaseSettingView::testCpBaseSettingViewWithPushButtonWidget()
    {
    HbPushButton *widget = new HbPushButton();
    QVERIFY(widget != 0);
    
    CpBaseSettingView *pView1 = new CpBaseSettingView(widget);
    QVERIFY(pView1!=0);
    
    HbPushButton *button = qobject_cast<HbPushButton *>(pView1->widget());
    QVERIFY(button != 0);
    QVERIFY(button == widget);
    
    delete pView1;
    pView1 = 0;    
    }

/*!
 * Test protected close() function, must be called in derived class
 */
/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: close() \n&nbsp;
   2. Function Descrition: \n&nbsp;&nbsp;
       2.1 Declare a derived class, MyCpBaseSettingView, create a MyCpBaseSettingView object with default constructor.\n &nbsp;&nbsp;
       2.2 call close().\n &nbsp;&nbsp;
       2.3 delete the object.\n &nbsp;
   3. Input Parameters: \n&nbsp;
       @param: 0\n
   4. Expected result: \n&nbsp;&nbsp;
       view closed.\n 
 */
void TestCpBaseSettingView::testCloseWithoutParam()
    {
    MyCpBaseSettingView * pMyView = new MyCpBaseSettingView();
    QVERIFY( pMyView != 0 );
    HbDataForm *pForm = new HbDataForm(0);
    pMyView->setWidget(pForm);
    HbDataForm *pForm1 = qobject_cast<HbDataForm *>(pMyView->widget());
    QVERIFY( pForm1 != 0);
    QVERIFY( pForm1 == pForm );
    
    pMyView->close();
    
    delete pMyView;
    pMyView = 0;
    }

void TestCpBaseSettingView::testPrivateFunc()
    {
    MyCpBaseSettingView * pMyView = new MyCpBaseSettingView();
    QVERIFY( pMyView != 0 );
    pMyView->testprivate();
    
    delete pMyView;
    pMyView = 0;
    }

QTEST_MAIN(TestCpBaseSettingView)
