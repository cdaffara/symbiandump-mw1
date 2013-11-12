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

#include "ut_cpitemdatahelper.h"

#include <cpitemdatahelper.h>
#include <QtTest/QtTest>
#include <hbdataformmodel.h>
#include <hbdataform.h>
#include <hbdataformviewitem.h>
#include <QModelIndex>

/*!
    \class TestCpItemDataHelper
    \brief The TestCpItemDataHelper class is designed for the unit testing for CpItemDataHelper class.
    This unit test is supported by QTest.
    There are total 21 test cases in this unit.
 */

void TestCpItemDataHelper::initTestCase()
    {
    }

void TestCpItemDataHelper::cleanupTestCase()
    {
    QCoreApplication::processEvents();
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: CpItemDataHelper() \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
       2.1 Create CpItemDataHelper object with a 0 pointer.\n &nbsp;&nbsp;
       2.2 verify object created successfully.\n &nbsp;&nbsp;
       2.3 delete the object.\n &nbsp;
   3. Input Parameters: \n&nbsp;
       @param: none.\n
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpItemDataHelper::testCpItemDataHelperWithNullPointer()
    {
    CpItemDataHelper *pHelper = new CpItemDataHelper();
    QVERIFY(pHelper != 0);
    
    delete pHelper;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: CpItemDataHelper() \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
       2.1 Create CpItemDataHelper object with a HbDataForm pointer.\n &nbsp;&nbsp;
       2.2 verify object created successfully.\n &nbsp;&nbsp;
       2.3 delete the object.\n &nbsp;
   3. Input Parameters: valid HbDataForm object pointer. \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpItemDataHelper::testCpItemDataHelperWithDataForm()
    {
    HbDataForm* form = new HbDataForm();
    QVERIFY(form != 0);
    
    HbDataFormModel *model = new HbDataFormModel();
    QVERIFY(model != 0);
    HbDataFormModelItem *general = model->appendDataFormPage(QString("General"));
    QVERIFY(general != 0);
    
    form->setModel(model);
    
    CpItemDataHelper *pHelper = new CpItemDataHelper(form);
    QVERIFY(pHelper != 0);
    
    delete pHelper;
    delete form;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: bindToForm() \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
       2.1 Create CpItemDataHelper object with a 0 pointer.\n &nbsp;&nbsp;
       2.2 call bindToForm later.\n &nbsp;&nbsp;
       2.3 delete the object.\n &nbsp;
   3. Input Parameters: 0/HbDataForm object pointer. \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpItemDataHelper::testBindToFormWithDataForm()
    {
    HbDataForm* form = new HbDataForm();
    QVERIFY(form != 0);
    HbDataFormModel *model = new HbDataFormModel();
    QVERIFY(model != 0);
    HbDataFormModelItem *general = model->appendDataFormPage(QString("General"));
    QVERIFY(general != 0);
    
    form->setModel(model);
    
    CpItemDataHelper *pHelper = new CpItemDataHelper();
    QVERIFY(pHelper != 0);

    //to increase the branch coverage
    pHelper->bindToForm(form);
    pHelper->bindToForm(form);
    pHelper->bindToForm(0);
    pHelper->bindToForm(form);
    
    delete pHelper;
    delete form;
    }
    
/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: addItemPrototype() \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
       2.1 Create CpItemDataHelper object with a 0 pointer.\n &nbsp;&nbsp;
       2.2 call addItemPrototype, provide 0 pointer.\n &nbsp;&nbsp;
       2.3 delete the object.\n &nbsp;
   3. Input Parameters: 0. \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpItemDataHelper::testAddItemPrototypeWithNullPointer1()
    {
    HbDataForm* form = new HbDataForm();
    QVERIFY(form != 0);
    HbDataFormModel *model = new HbDataFormModel();
    QVERIFY(model != 0);
    HbDataFormModelItem *general = model->appendDataFormPage(QString("General"));
    QVERIFY(general != 0);
    
    form->setModel(model);
    
    CpItemDataHelper *pHelper = new CpItemDataHelper();
    QVERIFY(pHelper != 0);
    
    int count1 = form->itemPrototypes().count();
    HbDataFormViewItem * pitem1 = 0;
    //pass 0 proto type
    pHelper->addItemPrototype(pitem1);
    int count2 = form->itemPrototypes().count();
    QVERIFY(count1 == count2);
    
    delete pHelper;
    delete form;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: addItemPrototype() \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
   3. Input Parameters: HbDataFormViewItem object pointer. \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpItemDataHelper::testAddItemPrototypeWithViewItem1()
    {
    HbDataForm* form = new HbDataForm();
    QVERIFY(form != 0);
    HbDataFormModel *model = new HbDataFormModel();
    QVERIFY(model != 0);
    HbDataFormModelItem *general = model->appendDataFormPage(QString("General"));
    QVERIFY(general != 0);
    
    form->setModel(model);
    
    CpItemDataHelper *pHelper = new CpItemDataHelper();
    QVERIFY(pHelper != 0);
    
    int count1 = form->itemPrototypes().count();
    HbDataFormViewItem * pitem1 = new HbDataFormViewItem();
    //pass 0 proto type
    pHelper->addItemPrototype(pitem1);
    int count2 = form->itemPrototypes().count();
    QVERIFY(count1 == count2);
    
    pHelper->bindToForm(form);
    int count3 = form->itemPrototypes().count();
    QVERIFY(count1 + 1 == count3);
    
    delete pHelper;
    delete form;
    }

/*!
 * Create CpItemDataHelper instance, call addItemPrototype with 0
 */
/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: addItemPrototype() \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
   3. Input Parameters: 0 pointer. \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpItemDataHelper::testAddItemPrototypeWithNullPointer2()
    {
    HbDataForm* form = new HbDataForm();
    QVERIFY(form != 0);
    HbDataFormModel *model = new HbDataFormModel();
    QVERIFY(model != 0);
    HbDataFormModelItem *general = model->appendDataFormPage(QString("General"));
    QVERIFY(general != 0);
    
    form->setModel(model);
    
    CpItemDataHelper *pHelper = new CpItemDataHelper(form);
    QVERIFY(pHelper != 0);
    
    int count1 = form->itemPrototypes().count();
    HbDataFormViewItem * pitem1 = 0;
    //pass 0 proto type
    pHelper->addItemPrototype(pitem1);
    int count2 = form->itemPrototypes().count();
    QVERIFY(count1 == count2);
    
    delete pHelper;
    delete form;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: addItemPrototype() \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
   3. Input Parameters: HbDataFormViewItem object pointer. \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpItemDataHelper::testAddItemPrototypeWithViewItem2()
    {
    HbDataForm* form = new HbDataForm();
    QVERIFY(form != 0);
    HbDataFormModel *model = new HbDataFormModel();
    QVERIFY(model != 0);
    HbDataFormModelItem *general = model->appendDataFormPage(QString("General"));
    QVERIFY(general != 0);
    
    form->setModel(model);
    
    CpItemDataHelper *pHelper = new CpItemDataHelper();
    QVERIFY(pHelper != 0);
    
    pHelper->bindToForm(form);
    
    int count1 = form->itemPrototypes().count();
    HbDataFormViewItem * pitem1 = new HbDataFormViewItem();
    //pass 0 proto type
    pHelper->addItemPrototype(pitem1);
    int count2 = form->itemPrototypes().count();
    QVERIFY(count1 + 1 == count2);
    
    delete pHelper;
    delete form;
    }
    
/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: addConnection(HbDataFormModelItem *item,
        const char *signal,
        QObject *receiver,
        const char *method) \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
   3. Input Parameters: \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpItemDataHelper::testAddConnectionWithnoDataForm()
    {
    HbDataForm* form = new HbDataForm();
    QVERIFY(form != 0);
    HbDataFormModel *model = new HbDataFormModel();
    QVERIFY(model != 0);
    HbDataFormModelItem *general = model->appendDataFormPage(QString("General"));
    QVERIFY(general != 0);
    
    form->setModel(model);
    
    CpItemDataHelper *pHelper = new CpItemDataHelper();
    QVERIFY(pHelper != 0);
    
    pHelper->addConnection( general, "hello", form, "hello");
    
    delete pHelper;
    delete form;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: addConnection(HbDataFormModelItem *item,
        const char *signal,
        QObject *receiver,
        const char *method) \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
   3. Input Parameters: \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpItemDataHelper::testAddConnectionWithDataForm()
    {
    HbDataForm* form = new HbDataForm();
    QVERIFY(form != 0);
    HbDataFormModel *model = new HbDataFormModel();
    QVERIFY(model != 0);
    HbDataFormModelItem *general = model->appendDataFormPage(QString("General"));
    QVERIFY(general != 0);
    
    form->setModel(model);
    
    CpItemDataHelper *pHelper = new CpItemDataHelper(form);
    QVERIFY(pHelper != 0);
    
    pHelper->addConnection( general, "hello", form, "hello");
    
    delete pHelper;
    delete form;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: removeConnection(HbDataFormModelItem *item,
        const char *signal,
        QObject *receiver,
        const char *method) \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
   3. Input Parameters: \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpItemDataHelper::testRemoveConnectionNoDataForm()
    {
    HbDataForm* form = new HbDataForm();
    QVERIFY(form != 0);
    HbDataFormModel *model = new HbDataFormModel();
    QVERIFY(model != 0);
    HbDataFormModelItem *general = model->appendDataFormPage(QString("General"));
    QVERIFY(general != 0);
    
    form->setModel(model);
    
    CpItemDataHelper *pHelper = new CpItemDataHelper();
    QVERIFY(pHelper != 0);
    
    pHelper->addConnection( general, "hello", form, "hello");
    pHelper->removeConnection(general,"hello", form, "hello");
    
    delete pHelper;
    delete form;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: removeConnection(HbDataFormModelItem *item,
        const char *signal,
        QObject *receiver,
        const char *method) \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
   3. Input Parameters: \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpItemDataHelper::testRemoveConnectionWithDataForm()
    {
    HbDataForm* form = new HbDataForm();
    QVERIFY(form != 0);
    HbDataFormModel *model = new HbDataFormModel();
    QVERIFY(model != 0);
    HbDataFormModelItem *general = model->appendDataFormPage(QString("General"));
    QVERIFY(general != 0);
    
    form->setModel(model);
    
    CpItemDataHelper *pHelper = new CpItemDataHelper(form);
    QVERIFY(pHelper != 0);
    
    pHelper->addConnection( general, "hello", form, "hello");
    pHelper->removeConnection(general,"hello", form, "hello");
    
    delete pHelper;
    delete form;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: connectToForm(const char *signal,
        QObject *receiver,
        const char *method) \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
   3. Input Parameters: \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpItemDataHelper::testConnectToFormNoDataForm()
    {
    HbDataForm* form = new HbDataForm();
    QVERIFY(form != 0);
    HbDataFormModel *model = new HbDataFormModel();
    QVERIFY(model != 0);
    HbDataFormModelItem *general = model->appendDataFormPage(QString("General"));
    QVERIFY(general != 0);
    
    form->setModel(model);
    
    CpItemDataHelper *pHelper = new CpItemDataHelper();
    QVERIFY(pHelper != 0);
    
    pHelper->connectToForm("hello", form, "hello");
    
    delete pHelper;
    delete form;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: connectToForm(const char *signal,
        QObject *receiver,
        const char *method) \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
   3. Input Parameters: \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpItemDataHelper::testConnectToFormWithDataForm()
    {
    HbDataForm* form = new HbDataForm();
    QVERIFY(form != 0);
    HbDataFormModel *model = new HbDataFormModel();
    QVERIFY(model != 0);
    HbDataFormModelItem *general = model->appendDataFormPage(QString("General"));
    QVERIFY(general != 0);
    
    form->setModel(model);
    
    CpItemDataHelper *pHelper = new CpItemDataHelper(form);
    QVERIFY(pHelper != 0);
    
    pHelper->connectToForm("hello", form, "hello");
    
    delete pHelper;
    delete form;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: disconnectFromForm(const char *signal,
        QObject *receiver,
        const char *method) \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
   3. Input Parameters: \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpItemDataHelper::testDisconnectToFormNoDataForm()
    {
    HbDataForm* form = new HbDataForm();
    QVERIFY(form != 0);
    HbDataFormModel *model = new HbDataFormModel();
    QVERIFY(model != 0);
    HbDataFormModelItem *general = model->appendDataFormPage(QString("General"));
    QVERIFY(general != 0);
    
    form->setModel(model);
    
    CpItemDataHelper *pHelper = new CpItemDataHelper();
    QVERIFY(pHelper != 0);
    
    pHelper->connectToForm("hello", form, "hello");
    pHelper->disconnectFromForm("hello", form, "hello");
    
    delete pHelper;
    delete form;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: disconnectFromForm(const char *signal,
        QObject *receiver,
        const char *method) \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
   3. Input Parameters: \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpItemDataHelper::testDisconnectToFormWithDataForm()
    {
    HbDataForm* form = new HbDataForm();
    QVERIFY(form != 0);
    HbDataFormModel *model = new HbDataFormModel();
    QVERIFY(model != 0);
    HbDataFormModelItem *general = model->appendDataFormPage(QString("General"));
    QVERIFY(general != 0);
    
    form->setModel(model);
    
    CpItemDataHelper *pHelper = new CpItemDataHelper(form);
    QVERIFY(pHelper != 0);
    
    pHelper->connectToForm("hello", form, "hello");
    pHelper->disconnectFromForm("hello", form, "hello");
    
    delete pHelper;
    delete form;
    delete model;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: HbWidget *widgetFromModelIndex(const QModelIndex &index); \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
   3. Input Parameters: QModelIndex object. \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpItemDataHelper::testWidgetFromModelIndexWithDataForm()
    {
    HbDataForm* form = new HbDataForm();
    QVERIFY( form != 0 );
    
    HbDataFormModel *model = new HbDataFormModel();
    QVERIFY( model != 0 );
    
    HbDataFormModelItem *general = model->appendDataFormItem(HbDataFormModelItem::TextItem, QString("General"));
    QVERIFY( general != 0 );
    
    HbDataFormModelItem *special = model->appendDataFormItem(HbDataFormModelItem::SliderItem, QString("Special"));
    QVERIFY( special != 0 );
    
    CpItemDataHelper *phelper = new CpItemDataHelper(form);
    QVERIFY( phelper != 0 );
    
    form->setModel(model);
    
    QModelIndex index = model->indexFromItem(special);
    
    HbWidget * widget = phelper->widgetFromModelIndex(index);
    QVERIFY( widget != 0 );
    QVERIFY( widget->type() == Hb::ItemType_Slider );
    
    delete phelper;
    delete form;
    delete model;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: HbWidget *widgetFromModelIndex(const QModelIndex &index); \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
   3. Input Parameters: QModelIndex object. \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpItemDataHelper::testWidgetFromModelIndexNoDataForm()
    {
    HbDataForm* form = new HbDataForm();
    QVERIFY( form != 0 );
    
    HbDataFormModel *model = new HbDataFormModel();
    QVERIFY( model != 0 );
    
    HbDataFormModelItem *general = model->appendDataFormItem(HbDataFormModelItem::SliderItem, QString("General"));
    QVERIFY( general != 0 );
    
    form->setModel(model);
    
    CpItemDataHelper *phelper = new CpItemDataHelper();
    QVERIFY( phelper != 0 );
    
    QModelIndex index = model->indexFromItem(general);
    HbWidget * widget = phelper->widgetFromModelIndex(index);
    QVERIFY( widget == 0 );
    
    phelper->bindToForm(form);
    HbWidget * widget1 = phelper->widgetFromModelIndex(index);
    QVERIFY( widget1 != 0 );
    QVERIFY( widget1->type() == Hb::ItemType_Slider );
    
    delete phelper;
    delete form;
    delete model;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: HbWidget *widgetFromModelIndex(const QModelIndex &index); \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
   3. Input Parameters: QModelIndex object. \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpItemDataHelper::testWidgetFromModelIndexWithDataFormBinded()
    {
    HbDataForm* form = new HbDataForm();
    QVERIFY( form != 0 );
    
    HbDataFormModel *model = new HbDataFormModel();
    QVERIFY( model != 0 );
    
    HbDataFormModelItem *general = model->appendDataFormItem(HbDataFormModelItem::SliderItem, QString("General"));
    QVERIFY( general != 0 );
    
    HbDataFormModelItem * general1 = new HbDataFormModelItem();
    QVERIFY( general1 != 0 );
    
    form->setModel(model);
    
    CpItemDataHelper *phelper = new CpItemDataHelper(form);
    QVERIFY( phelper != 0 );
    
    QModelIndex index = model->indexFromItem(general1);
    HbWidget * widget = phelper->widgetFromModelIndex(index);
    QVERIFY( widget == 0 );
    
    phelper->bindToForm(form);
    HbWidget * widget1 = phelper->widgetFromModelIndex(index);
    QVERIFY( widget1 == 0 );
    
    delete phelper;
    delete form;
    delete model;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: HbDataFormModelItem *modelItemFromModelIndex(const QModelIndex &index); \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
   3. Input Parameters: QModelIndex object. \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpItemDataHelper::testModelItemFromModelIndexWithDataForm()
    {
    HbDataForm* form = new HbDataForm();
    QVERIFY( form != 0 );
    
    HbDataFormModel *model = new HbDataFormModel();
    QVERIFY( model != 0 );
    
    HbDataFormModelItem *general = model->appendDataFormItem(HbDataFormModelItem::TextItem, QString("General"));
    QVERIFY( general != 0 );
    
    HbDataFormModelItem *special = model->appendDataFormItem(HbDataFormModelItem::SliderItem, QString("Special"));
    QVERIFY( special != 0 );
    
    CpItemDataHelper *phelper = new CpItemDataHelper(form);
    QVERIFY( phelper != 0 );
    
    form->setModel(model);
    
    QModelIndex index = model->indexFromItem(special);
    
    HbDataFormModelItem * item = phelper->modelItemFromModelIndex(index);
    QVERIFY( item != 0 );
    QVERIFY( item->type() == HbDataFormModelItem::SliderItem );
    
    delete phelper;
    delete form;
    delete model;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: HbDataFormModelItem *modelItemFromModelIndex(const QModelIndex &index); \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
   3. Input Parameters: QModelIndex object. \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpItemDataHelper::testModelItemFromModelIndexNoDataForm()
    {
    HbDataForm* form = new HbDataForm();
    QVERIFY( form != 0 );
    
    HbDataFormModel *model = new HbDataFormModel();
    QVERIFY( model != 0 );
    
    HbDataFormModelItem *general = model->appendDataFormItem(HbDataFormModelItem::SliderItem, QString("General"));
    QVERIFY( general != 0 );
    
    form->setModel(model);
    
    CpItemDataHelper *phelper = new CpItemDataHelper();
    QVERIFY( phelper != 0 );
    
    QModelIndex index = model->indexFromItem(general);
    HbDataFormModelItem * item = phelper->modelItemFromModelIndex(index);
    QVERIFY( item == 0 );
    
    phelper->bindToForm(form);
    
    HbDataFormModelItem * item1 = phelper->modelItemFromModelIndex(index);
    QVERIFY( item1 != 0 );
    QVERIFY( item1->type() == HbDataFormModelItem::SliderItem );
    
    delete phelper;
    delete form;
    delete model;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: HbDataFormModelItem *modelItemFromModelIndex(const QModelIndex &index); \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
   3. Input Parameters: QModelIndex object. \n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpItemDataHelper::testModelItemFromModelIndexWithModel()
    {
    HbDataForm* form = new HbDataForm();
    QVERIFY( form != 0 );
    
    HbDataFormModel *model = new HbDataFormModel();
    QVERIFY( model != 0 );
    
    HbDataFormModelItem *general = model->appendDataFormItem(HbDataFormModelItem::SliderItem, QString("General"));
    QVERIFY( general != 0 );
    
    HbDataFormModelItem * general1 = new HbDataFormModelItem();
    QVERIFY( general1 != 0 );
    
    CpItemDataHelper *phelper = new CpItemDataHelper(form);
    QVERIFY( phelper != 0 );
    
    QModelIndex index = model->indexFromItem(general1);
    HbDataFormModelItem * item = phelper->modelItemFromModelIndex(index);
    QVERIFY( item == 0 );

    form->setModel(model);
    
    HbDataFormModelItem * item1 = phelper->modelItemFromModelIndex(index);
    QVERIFY( item1 == 0 );
    
    delete phelper;
    delete form;
    delete model;
    }

QTEST_MAIN(TestCpItemDataHelper)
