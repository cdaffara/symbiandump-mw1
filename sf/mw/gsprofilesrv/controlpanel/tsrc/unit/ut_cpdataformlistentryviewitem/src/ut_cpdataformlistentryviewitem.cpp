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

#include "ut_cpdataformlistentryviewitem.h"

#include "cpdataformlistentryviewitem.h"
#include "CpSettingFormEntryItemDataImpl.h"
#include "mycpdataformlistentryviewitem.h"

#include <QtTest/QtTest>
#include <HbMainWindow>
#include <hbdataform.h>
#include <hbdataformmodel.h>
#include <hbpushbutton.h>
#include <cppluginutility.h>
#include <cpbasesettingview.h>

/*!
    \class TestCpDataFormListEntryViewItem
    \brief The TestCpDataFormListEntryViewItem class is designed for the unit testing for CpDataFormListEntryViewItem class.
    This unit test is supported by QTest.
    There are total 6 test cases in this unit.
 */

void TestCpDataFormListEntryViewItem::initTestCase()
    {
    mainWindow = 0;
    }

void TestCpDataFormListEntryViewItem::cleanupTestCase()
    {
    delete mainWindow;
    
    //if delete mainwindow, test app will be freezing
    QCoreApplication::processEvents();
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: CpDataFormListEntryViewItem() \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
       2.1 Create CpDataFormListEntryViewItem object with a HbDataForm pointer.\n &nbsp;&nbsp;
       2.2 verify object created successfully.\n &nbsp;&nbsp;
       2.3 delete the object.\n &nbsp;
   3. Input Parameters: \n&nbsp;
       @param: HbDataForm pointer.\n
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpDataFormListEntryViewItem::testCpDataFormListEntryViewItemWithDataForm()
    {
    HbDataForm * form = new HbDataForm();
    QVERIFY( form != 0 );
    
    int count0 = form->itemPrototypes().count();
    CpPluginUtility::addCpItemPrototype(form);
    int count1 = form->itemPrototypes().count();
    QVERIFY( count0 + 2 == count1 );
    
    CpDataFormListEntryViewItem * item = new CpDataFormListEntryViewItem(form);
    QVERIFY(item != 0);
        
    delete form;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: CpDataFormListEntryViewItem() \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
       2.1 Create CpDataFormListEntryViewItem object with a 0 pointer.\n &nbsp;&nbsp;
       2.2 verify object created successfully.\n &nbsp;&nbsp;
       2.3 delete the object.\n &nbsp;
   3. Input Parameters: \n&nbsp;
       @param: HbDataForm pointer.\n
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpDataFormListEntryViewItem::testCpDataFormListEntryViewItemWithNullPointer()
    {
    CpDataFormListEntryViewItem * item = new CpDataFormListEntryViewItem();
    QVERIFY(item != 0);
    
    delete item;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name:\n&nbsp;&nbsp;
       HbAbstractViewItem *createItem();\n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
       2.1 Create CpDataFormListEntryViewItem object.\n &nbsp;&nbsp;
       2.2 verify object created successfully.\n &nbsp;&nbsp;
       2.3 call createItem() to create a HbAbstractViewItem object.\n&nbsp;&nbsp;
       2.4 verify the object is not 0.\n&nbsp;&nbsp;
       2.5 delete both objects.\n &nbsp;
   3. Input Parameters: \n&nbsp;
       @param: none.\n
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpDataFormListEntryViewItem::testCreateItemNoDataForm()
    {
    CpDataFormListEntryViewItem * item = new CpDataFormListEntryViewItem();
    QVERIFY(item != 0);
    
    HbAbstractViewItem * item1 = item->createItem();
    QVERIFY(item1 != 0);
    
    delete item1;
    delete item;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name:\n&nbsp;&nbsp;
       HbAbstractViewItem *createItem();\n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
       2.1 Create CpDataFormListEntryViewItem object.\n &nbsp;&nbsp;
       2.2 verify object created successfully.\n &nbsp;&nbsp;
       2.3 call createItem() to create a HbAbstractViewItem object.\n&nbsp;&nbsp;
       2.4 verify the object is not 0.\n&nbsp;&nbsp;
       2.5 delete both objects.\n &nbsp;
   3. Input Parameters: \n&nbsp;
       @param: none.\n
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpDataFormListEntryViewItem::testCreateItemWithDataForm()
    {
    HbDataForm * form = new HbDataForm();
    QVERIFY( form != 0 );
    
    CpDataFormListEntryViewItem * item = new CpDataFormListEntryViewItem(form);
    QVERIFY(item != 0);
    
    HbAbstractViewItem * item1 = item->createItem();
    QVERIFY(item1 != 0);
    
    delete item;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: CanSetModelIndex();\n&nbsp;&nbsp;
       
   2. Function Descrition: \n &nbsp;&nbsp;

   3. Input Parameters: \n&nbsp;
       @param: none.\n
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpDataFormListEntryViewItem::testCanSetModelIndexWithDataForm()
    {
    HbDataForm * form = new HbDataForm();
    QVERIFY( form != 0 );
    
    int count0 = form->itemPrototypes().count();
    CpPluginUtility::addCpItemPrototype(form);
    int count1 = form->itemPrototypes().count();
    QVERIFY( count0 + 2 == count1 );
        
    CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp1 = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>( CpSettingFormEntryItemData::ListEntryItem, form, "text", "description" );
    QVERIFY( pdataimp1 !=0 );
    
    HbDataFormModel * model = new HbDataFormModel;
    QVERIFY( model != 0 );
    
    model->appendDataFormItem(pdataimp1);
    form->setModel(model);
    form->show();
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
      HbWidget* createCustomWidget();
       
   2. Function Descrition: \n &nbsp;&nbsp;

   3. Input Parameters: \n&nbsp;
       @param: none.\n
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpDataFormListEntryViewItem::testCreateCustomedWidgetNoParam()
    {
    HbDataForm * form = new HbDataForm();
    QVERIFY( form != 0 );
    
    int count0 = form->itemPrototypes().count();
    CpPluginUtility::addCpItemPrototype(form);
    int count1 = form->itemPrototypes().count();
    QVERIFY( count0 + 2 == count1 );
        
    CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp1 = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>( CpSettingFormEntryItemData::ListEntryItem, form, "text", "description" );
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

QTEST_MAIN(TestCpDataFormListEntryViewItem)
