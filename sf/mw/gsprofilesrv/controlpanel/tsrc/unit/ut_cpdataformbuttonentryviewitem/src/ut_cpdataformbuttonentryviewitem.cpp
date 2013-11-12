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
#include "ut_cpdataformbuttonentryviewitem.h"

#include "cpdataformbuttonentryviewitem.h"
#include "mycpdataformbuttonentryviewitem.h"
#include "cpbasesettingview.h"

#include <QtTest/QtTest>
#include <HbMainWindow>
#include <hbdataform.h>
#include <cppluginutility.h>
#include <hbdataformmodel.h>
#include <cpsettingformentryitemdata.h>
#include <cpsettingformentryitemdataimpl.h>

/*!
    \class TestCpDataFormButtonEntryViewItem
    \brief The TestCpDataFormButtonEntryViewItem class is designed for the unit testing for CpDataFormButtonEntryViewItem class.
    This unit test is supported by QTest.
    There are total 8 test cases in this unit.
 */

void TestCpDataFormButtonEntryViewItem::initTestCase()
    {
    mainWindow = 0;
    }

void TestCpDataFormButtonEntryViewItem::cleanupTestCase()
    {
    delete mainWindow;
    
    //if delete mainwindow, test app will be freezing
    QCoreApplication::processEvents();
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: CpDataFormButtonEntryViewItem() \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
       2.1 Create CpDataFormButtonEntryViewItem object with a HbDataForm pointer.\n &nbsp;&nbsp;
       2.2 verify object created successfully.\n &nbsp;&nbsp;
       2.3 delete the object.\n &nbsp;
   3. Input Parameters: \n&nbsp;
       @param: HbDataForm pointer.\n
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpDataFormButtonEntryViewItem::testCpDataFormButtonEntryViewItemWithDataFormParent()
    {
    HbDataForm * form = new HbDataForm();
    QVERIFY( form != 0 );
    
    int count0 = form->itemPrototypes().count();
    CpPluginUtility::addCpItemPrototype(form);
    int count1 = form->itemPrototypes().count();
    QVERIFY( count0 + 2 == count1 );
    
    CpDataFormButtonEntryViewItem * item = new CpDataFormButtonEntryViewItem(form);
    QVERIFY(item != 0);
        
    delete item;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: CpDataFormButtonEntryViewItem() \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
       2.1 Create CpDataFormButtonEntryViewItem object with a 0 pointer.\n &nbsp;&nbsp;
       2.2 verify object created successfully.\n &nbsp;&nbsp;
       2.3 delete the object.\n &nbsp;
   3. Input Parameters:  \n&nbsp;
       @param: 0 pointer.\n
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpDataFormButtonEntryViewItem::testCpDataFormButtonEntryViewItemWithNullParent()
    {
    CpDataFormButtonEntryViewItem * item = new CpDataFormButtonEntryViewItem();
    QVERIFY(item != 0);
    
    delete item;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: CpDataFormButtonEntryViewItem(const CpDataFormButtonEntryViewItem &ohter) \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
       2.1 Create CpDataFormButtonEntryViewItem object with another CpDataFormButtonEntryViewItem object.\n &nbsp;&nbsp;
       2.2 verify object created successfully.\n &nbsp;&nbsp;
       2.3 delete the object.\n &nbsp;
   3. Input Parameters:  \n&nbsp;
       @param: CpDataFormButtonEntryViewItem object.\n
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpDataFormButtonEntryViewItem::testCpDataFormButtonEntryViewItemWithCopyConstructor()
    {
    MyCpDataFormButtonEntryViewItem * item1 = new MyCpDataFormButtonEntryViewItem();
    QVERIFY( item1 != 0 );
    
    MyCpDataFormButtonEntryViewItem * item2 = new MyCpDataFormButtonEntryViewItem(*item1);
    QVERIFY( item2 != 0 );
    
    delete item1;
    delete item2;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name:\n&nbsp;&nbsp;
       HbAbstractViewItem *createItem();\n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
       2.1 Create CpDataFormButtonEntryViewItem object.\n &nbsp;&nbsp;
       2.2 verify object created successfully.\n &nbsp;&nbsp;
       2.3 call createItem() to create a HbAbstractViewItem object.\n&nbsp;&nbsp;
       2.4 verify the object is not 0.\n&nbsp;&nbsp;
       2.5 delete both objects.\n &nbsp;
   3. Input Parameters: \n&nbsp;
       @param: none.\n
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpDataFormButtonEntryViewItem::testCreateItemWithDataForm()
    {
    HbDataForm * form = new HbDataForm();
    QVERIFY( form != 0 );
    
    int count0 = form->itemPrototypes().count();
    CpPluginUtility::addCpItemPrototype(form);
    int count1 = form->itemPrototypes().count();
    QVERIFY( count0 + 2 == count1 );
    
    CpDataFormButtonEntryViewItem * item = new CpDataFormButtonEntryViewItem(form);
    QVERIFY(item != 0);
    
    HbAbstractViewItem * pvItem1 = item->createItem();
    QVERIFY(pvItem1 != 0);
    
    delete pvItem1;
    delete item;
    delete form;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name:\n&nbsp;&nbsp;
       HbAbstractViewItem *createItem();\n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
       2.1 Create CpDataFormButtonEntryViewItem object.\n &nbsp;&nbsp;
       2.2 verify object created successfully.\n &nbsp;&nbsp;
       2.3 call createItem() to create a HbAbstractViewItem object.\n&nbsp;&nbsp;
       2.4 verify the object is not 0.\n&nbsp;&nbsp;
       2.5 delete both objects.\n &nbsp;
   3. Input Parameters: \n&nbsp;
       @param: none.\n
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpDataFormButtonEntryViewItem::testCreateItemNoDataForm()
    {
    CpDataFormButtonEntryViewItem * item = new CpDataFormButtonEntryViewItem();
    QVERIFY(item != 0);
    
    HbAbstractViewItem * pvItem1 = item->createItem();
    QVERIFY(pvItem1 != 0);
    
    delete pvItem1;
    delete item;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name:\n&nbsp;&nbsp;
       CpDataFormButtonEntryViewItem &operator = (const CpDataFormButtonEntryViewItem &ohter);\n\n&nbsp;
       
   2. Function Descrition: \n &nbsp;&nbsp;
       2.1 Create CpDataFormButtonEntryViewItem object.\n &nbsp;&nbsp;
       2.2 verify object created successfully.\n &nbsp;&nbsp;
       2.3 assign this object to another object.\n&nbsp;&nbsp;
       2.4 delete object.\n\n &nbsp;
       
   3. Input Parameters: \n\n&nbsp;
       @param: none.\n
   
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpDataFormButtonEntryViewItem::testAssignmentOperatorWithOtherObject()
    {
    MyCpDataFormButtonEntryViewItem * item1 = new MyCpDataFormButtonEntryViewItem();
    QVERIFY(item1 != 0);
    
    MyCpDataFormButtonEntryViewItem item2;
    
    item2 = *item1;
    
    delete item1;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name:\n&nbsp;&nbsp;
       CpDataFormButtonEntryViewItem &operator = (const CpDataFormButtonEntryViewItem &ohter);\n\n&nbsp;
       
   2. Function Descrition: \n &nbsp;&nbsp;
       2.1 Create CpDataFormButtonEntryViewItem object.\n &nbsp;&nbsp;
       2.2 verify object created successfully.\n &nbsp;&nbsp;
       2.3 assign this object to itself.\n&nbsp;&nbsp;
       2.4 delete object.\n\n &nbsp;
       
   3. Input Parameters: \n\n&nbsp;
       @param: none.\n
   
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpDataFormButtonEntryViewItem::testAssignmentOperatorBySelf()
    {
    MyCpDataFormButtonEntryViewItem * item1 = new MyCpDataFormButtonEntryViewItem();
    QVERIFY(item1 != 0);
    
    *item1 = *item1;
    
    delete item1;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name:\n&nbsp;&nbsp;
       HbWidget *createCustomWidget();
       
   2. Function Descrition: \n &nbsp;&nbsp;
       
   3. Input Parameters: \n\n&nbsp;
       @param: none.\n
   
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpDataFormButtonEntryViewItem::testCreateCustomedWidgetWithDataForm()
    {
    HbDataForm * form = new HbDataForm();
    QVERIFY( form != 0 );
    
    int count0 = form->itemPrototypes().count();
    CpPluginUtility::addCpItemPrototype(form);
    int count1 = form->itemPrototypes().count();
    QVERIFY( count0 + 2 == count1 );
        
    CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp1 = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>( CpSettingFormEntryItemData::ButtonEntryItem, form, "text", "description" );
    QVERIFY( pdataimp1 !=0 );
    
    HbDataFormModel * model = new HbDataFormModel;
    QVERIFY( model != 0 );
    
    model->appendDataFormItem(pdataimp1);
    form->setModel(model);
    
    mainWindow = new HbMainWindow;
    QVERIFY( mainWindow != 0 );
    
    CpBaseSettingView *mainView = new CpBaseSettingView(form);
    QVERIFY( mainView != 0 );
    
    QObject::connect(mainView,SIGNAL(aboutToClose()),qApp,SLOT(quit()));
    HbView * view = mainWindow->addView(mainView);
    QVERIFY( view != 0 );
    
    mainWindow->setCurrentView(mainView);
    HbView * curView = mainWindow->currentView();
    QVERIFY( curView == mainView );
    
    mainWindow->show();
    }

QTEST_MAIN(TestCpDataFormButtonEntryViewItem)
