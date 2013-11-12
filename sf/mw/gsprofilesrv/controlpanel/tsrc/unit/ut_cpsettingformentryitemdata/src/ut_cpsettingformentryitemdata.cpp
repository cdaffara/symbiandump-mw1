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

#include "ut_cpsettingformentryitemdata.h"

#include "mycpsettingformentryitemdata.h"

#include <cpsettingformentryitemdata.h>
#include <cpsettingformentryitemdataimpl.h>
#include <cpitemdatahelper.h>
#include <cpbasesettingview.h>
#include <QtTest/QtTest>
#include <hbdataform.h>
#include <hbpushbutton.h>
#include <hbicon.h>

/*!
    \class TestCpSettingFormEntryItemData
    \brief This class is used for the unit test for class CpSettingFormEntryItemData.\n
      CpSettingFormEntryItemData class is used for access to a control panel setting form entry item data, \n
      this class has a pure virtual member function, so it is not allowed to create instance directly, instead of that, \n
      developer should a template class CpSettingFormEntryItemDataImpl to create new instance of CpSettingFormEntryItemData.\n
      This unit test is supported by QTest.\n
      There are total 18 test cases in this unit.\n 
 */

void TestCpSettingFormEntryItemData::initTestCase()
    {

    }

void TestCpSettingFormEntryItemData::cleanupTestCase()
    {
    QCoreApplication::processEvents();
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
         CpSettingFormEntryItemDataImpl(CpItemDataHelper &itemDataHelper,
            const QString &text = QString(),
            const QString &description = QString(),
            const HbIcon &icon = HbIcon(),
            const HbDataFormModelItem *parent = 0) : 
            CpSettingFormEntryItemData(itemDataHelper,text,description,icon,parent);\n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
        \n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
       CpItemDataHelper &itemDataHelper\n&nbsp;&nbsp;
       QString &text\n&nbsp;&nbsp;
       QString &description\n&nbsp;&nbsp;
       HbIcon &icon\n&nbsp;&nbsp;
       HbDataFormModelItem *parent\n&nbsp;
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormEntryItemData::testCpSettingFormEntryItemDataWithHelperNoForm()
    {
    CpItemDataHelper *pHelper1 = new CpItemDataHelper();
    QVERIFY( pHelper1 != 0 );
    
    CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp1 = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>( *pHelper1, "text", "description"  );
    QVERIFY( pdataimp1 !=0 );
    QString strtxt1 = "input texts";
    pdataimp1->setText(strtxt1);
    QVERIFY( pdataimp1->text() == strtxt1 );
    
    QString strDes1 = "input descriptions";
    pdataimp1->setDescription(strDes1);
    QVERIFY( pdataimp1->description() == strDes1);
    
    QString strIconName1 = "input iconname";
    pdataimp1->setIconName(strIconName1);
    QVERIFY( pdataimp1->iconName() == strIconName1);
    
    HbIcon itemIcon1(QString(""));
    pdataimp1->setEntryItemIcon(itemIcon1);
    QVERIFY( pdataimp1->entryItemIcon() == itemIcon1);
    
    delete pdataimp1;
    delete pHelper1;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
         CpSettingFormEntryItemDataImpl(CpItemDataHelper &itemDataHelper,
            const QString &text = QString(),
            const QString &description = QString(),
            const HbIcon &icon = HbIcon(),
            const HbDataFormModelItem *parent = 0) : 
            CpSettingFormEntryItemData(itemDataHelper,text,description,icon,parent);\n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
        \n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
       CpItemDataHelper &itemDataHelper\n&nbsp;&nbsp;
       QString &text\n&nbsp;&nbsp;
       QString &description\n&nbsp;&nbsp;
       HbIcon &icon\n&nbsp;&nbsp;
       HbDataFormModelItem *parent\n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormEntryItemData::testCpSettingFormEntryItemDataWithHelperAndForm()
    {
    HbDataForm *pForm2 = new HbDataForm(0);
    QVERIFY(pForm2 != 0);
    
    CpItemDataHelper *pHelper2 = new CpItemDataHelper(pForm2);
    QVERIFY( pHelper2 != 0 );
    
    CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp2 = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>( *pHelper2, "text", "description"  );
    QVERIFY( pdataimp2 !=0 );
    QString strtxt2 = "input texts";
    pdataimp2->setText(strtxt2);
    QVERIFY( pdataimp2->text() == strtxt2 );
    
    QString strDes2 = "input descriptions";
    pdataimp2->setDescription(strDes2);
    QVERIFY( pdataimp2->description() == strDes2);
    
    delete pdataimp2;
    delete pHelper2;
    delete pForm2;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
         CpSettingFormEntryItemDataImpl(HbDataForm *dataForm,
            const QString &text = QString(),
            const QString &description = QString(),
            const HbIcon &icon = HbIcon(),
            const HbDataFormModelItem *parent = 0) : 
            CpSettingFormEntryItemData(dataForm,text,description,icon,parent);\n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
        \n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
       HbDataForm *dataForm\n&nbsp;&nbsp;
       QString &text\n&nbsp;&nbsp;
       QString &description\n&nbsp;&nbsp;
       HbIcon &icon\n&nbsp;&nbsp;
       HbDataFormModelItem *parent\n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormEntryItemData::testCpSettingFormEntryItemDataWithForm()
    {
    HbDataForm * pForm1 = new HbDataForm();
    QVERIFY(pForm1 != 0);
    
    CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp1 = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>( pForm1, "text", "description" );
    QVERIFY( pdataimp1 !=0 );
    QString strtxt1 = "input texts";
    pdataimp1->setText(strtxt1);
    QVERIFY( pdataimp1->text() == strtxt1 );
    
    QString strDes1 = "input descriptions";
    pdataimp1->setDescription(strDes1);
    QVERIFY( pdataimp1->description() == strDes1);
    
    QString strIconName1 = "input iconname";
    pdataimp1->setIconName(strIconName1);
    QVERIFY( pdataimp1->iconName() == strIconName1);
    
    HbIcon itemIcon1(QString(""));
    pdataimp1->setEntryItemIcon(itemIcon1);
    QVERIFY( pdataimp1->entryItemIcon() == itemIcon1);
    
    delete pdataimp1;
    delete pForm1;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
         CpSettingFormEntryItemDataImpl(
            EntryItemType type,
            CpItemDataHelper &itemDataHelper,
            const QString &text = QString(),
            const QString &description = QString(),
            const QString &icon = QString(),
            const HbDataFormModelItem *parent = 0) : 
            CpSettingFormEntryItemData(type,itemDataHelper,text,description,icon,parent);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        \n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
       EntryItemType type\n&nbsp;&nbsp;
       CpItemDataHelper &itemDataHelper\n&nbsp;&nbsp;
       QString &text\n&nbsp;&nbsp;
       QString &description\n&nbsp;&nbsp;
       HbIcon &icon\n&nbsp;&nbsp;
       HbDataFormModelItem *parent\n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormEntryItemData::testCpSettingFormEntryItemDataWithListTypeAndForm()
    {
    HbDataForm * pForm1 = new HbDataForm();
    QVERIFY(pForm1 != 0);
    
    CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp1 = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>( CpSettingFormEntryItemData::ListEntryItem, pForm1, "text", "description" );
    QVERIFY( pdataimp1 !=0 );
    QString strtxt1 = "input texts";
    pdataimp1->setText(strtxt1);
    QVERIFY( pdataimp1->text() == strtxt1 );
    
    QString strDes1 = "input descriptions";
    pdataimp1->setDescription(strDes1);
    QVERIFY( pdataimp1->description() == strDes1);
    
    QString strIconName1 = "input iconname";
    pdataimp1->setIconName(strIconName1);
    QVERIFY( pdataimp1->iconName() == strIconName1);
    
    HbIcon itemIcon1(QString(""));
    pdataimp1->setEntryItemIcon(itemIcon1);
    QVERIFY( pdataimp1->entryItemIcon() == itemIcon1);
    
    delete pdataimp1;
    delete pForm1;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
         CpSettingFormEntryItemDataImpl(
            EntryItemType type,
            HbDataForm *dataForm,
            const QString &text = QString(),
            const QString &description = QString(),
            const QString &icon = QString(),
            const HbDataFormModelItem *parent = 0) : 
            CpSettingFormEntryItemData(type,dataForm,text,description,icon,parent);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        \n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
       EntryItemType type\n&nbsp;&nbsp;
       HbDataForm *dataForm\n&nbsp;&nbsp;
       QString &text\n&nbsp;&nbsp;
       QString &description\n&nbsp;&nbsp;
       HbIcon &icon\n&nbsp;&nbsp;
       HbDataFormModelItem *parent\n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormEntryItemData::testCpSettingFormEntryItemDataWithButtonTypeAndForm()
    {
    HbDataForm * pForm1 = new HbDataForm();
    QVERIFY(pForm1 != 0);
    
    CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp1 = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>( CpSettingFormEntryItemData::ButtonEntryItem, pForm1, "text", "description" );
    QVERIFY( pdataimp1 !=0 );
    QString strtxt1 = "input texts";
    pdataimp1->setText(strtxt1);
    QVERIFY( pdataimp1->text() == strtxt1 );
    
    QString strDes1 = "input descriptions";
    pdataimp1->setDescription(strDes1);
    QVERIFY( pdataimp1->description() == strDes1);
    
    QString strIconName1 = "input iconname";
    pdataimp1->setIconName(strIconName1);
    QVERIFY( pdataimp1->iconName() == strIconName1);
    
    HbIcon itemIcon1(QString(""));
    pdataimp1->setEntryItemIcon(itemIcon1);
    QVERIFY( pdataimp1->entryItemIcon() == itemIcon1);
    
    delete pdataimp1;
    delete pForm1;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
         CpSettingFormEntryItemDataImpl(
            EntryItemType type,
            HbDataForm *dataForm,
            const QString &text = QString(),
            const QString &description = QString(),
            const QString &icon = QString(),
            const HbDataFormModelItem *parent = 0) : 
            CpSettingFormEntryItemData(type,dataForm,text,description,icon,parent);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        \n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
       EntryItemType type\n&nbsp;&nbsp;
       HbDataForm *dataForm\n&nbsp;&nbsp;
       QString &text\n&nbsp;&nbsp;
       QString &description\n&nbsp;&nbsp;
       HbIcon &icon\n&nbsp;&nbsp;
       HbDataFormModelItem *parent\n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormEntryItemData::testCpSettingFormEntryItemData005()
    {
    HbDataForm * pForm1 = new HbDataForm();
    QVERIFY(pForm1 != 0);
    
    CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp1 = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>( CpSettingFormEntryItemData::ListEntryItem, pForm1, "text", "description" );
    QVERIFY( pdataimp1 !=0 );
    QString strtxt1 = "input texts";
    pdataimp1->setText(strtxt1);
    QVERIFY( pdataimp1->text() == strtxt1 );
    
    QString strDes1 = "input descriptions";
    pdataimp1->setDescription(strDes1);
    QVERIFY( pdataimp1->description() == strDes1);

    QString strIconName1 = "input iconname";
    pdataimp1->setIconName(strIconName1);
    QVERIFY( pdataimp1->iconName() == strIconName1);
    
    HbIcon itemIcon1(QString(""));
    pdataimp1->setEntryItemIcon(itemIcon1);
    QVERIFY( pdataimp1->entryItemIcon() == itemIcon1);
    
    delete pdataimp1;
    delete pForm1;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
         CpSettingFormEntryItemDataImpl(
            EntryItemType type,
            HbDataForm *dataForm,
            const QString &text = QString(),
            const QString &description = QString(),
            const QString &icon = QString(),
            const HbDataFormModelItem *parent = 0) : 
            CpSettingFormEntryItemData(type,dataForm,text,description,icon,parent);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        \n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
       EntryItemType type\n&nbsp;&nbsp;
       HbDataForm *dataForm\n&nbsp;&nbsp;
       QString &text\n&nbsp;&nbsp;
       QString &description\n&nbsp;&nbsp;
       HbIcon &icon\n&nbsp;&nbsp;
       HbDataFormModelItem *parent\n&nbsp;
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormEntryItemData::testCpSettingFormEntryItemData005a()
    {
    HbDataForm * pForm1 = new HbDataForm();
    QVERIFY(pForm1 != 0);
    
    CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp1 = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>( CpSettingFormEntryItemData::ButtonEntryItem, pForm1, "text", "description" );
    QVERIFY( pdataimp1 !=0 );
    QString strtxt1 = "input texts";
    pdataimp1->setText(strtxt1);
    QVERIFY( pdataimp1->text() == strtxt1 );
    
    QString strDes1 = "input descriptions";
    pdataimp1->setDescription(strDes1);
    QVERIFY( pdataimp1->description() == strDes1);

    QString strIconName1 = "input iconname";
    pdataimp1->setIconName(strIconName1);
    QVERIFY( pdataimp1->iconName() == strIconName1);
    
    HbIcon itemIcon1(QString(""));
    pdataimp1->setEntryItemIcon(itemIcon1);
    QVERIFY( pdataimp1->entryItemIcon() == itemIcon1);
    
    delete pdataimp1;
    delete pForm1;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
         CpSettingFormEntryItemDataImpl(
            EntryItemType type,
            CpItemDataHelper &itemDataHelper,
            const QString &text = QString(),
            const QString &description = QString(),
            const QString &icon = QString(),
            const HbDataFormModelItem *parent = 0) : 
            CpSettingFormEntryItemData(type,itemDataHelper,text,description,icon,parent);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        \n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
       EntryItemType type\n&nbsp;&nbsp;
       CpItemDataHelper &itemDataHelper\n&nbsp;&nbsp;
       QString &text\n&nbsp;&nbsp;
       QString &description\n&nbsp;&nbsp;
       HbIcon &icon\n&nbsp;&nbsp;
       HbDataFormModelItem *parent\n&nbsp;
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormEntryItemData::testCpSettingFormEntryItemDataWithListTypeAndHelper()
    {
    CpItemDataHelper *pHelper1 = new CpItemDataHelper();
    QVERIFY(pHelper1 != 0);
    
    CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp1 = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>( CpSettingFormEntryItemData::ListEntryItem, *pHelper1, "text", "description" );
    QVERIFY( pdataimp1 !=0 );
    QString strtxt1 = "input texts";
    pdataimp1->setText(strtxt1);
    QVERIFY( pdataimp1->text() == strtxt1 );
    
    QString strDes1 = "input descriptions";
    pdataimp1->setDescription(strDes1);
    QVERIFY( pdataimp1->description() == strDes1);
    
    QString strIconName1 = "input iconname";
    pdataimp1->setIconName(strIconName1);
    QVERIFY( pdataimp1->iconName() == strIconName1);
    
    HbIcon itemIcon1(QString(""));
    pdataimp1->setEntryItemIcon(itemIcon1);
    QVERIFY( pdataimp1->entryItemIcon() == itemIcon1);
    
    delete pdataimp1;
    delete pHelper1;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
         CpSettingFormEntryItemDataImpl(
            EntryItemType type,
            CpItemDataHelper &itemDataHelper,
            const QString &text = QString(),
            const QString &description = QString(),
            const QString &icon = QString(),
            const HbDataFormModelItem *parent = 0) : 
            CpSettingFormEntryItemData(type,itemDataHelper,text,description,icon,parent);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        \n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
       EntryItemType type\n&nbsp;&nbsp;
       CpItemDataHelper &itemDataHelper\n&nbsp;&nbsp;
       QString &text\n&nbsp;&nbsp;
       QString &description\n&nbsp;&nbsp;
       HbIcon &icon\n&nbsp;&nbsp;
       HbDataFormModelItem *parent\n&nbsp;
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormEntryItemData::testCpSettingFormEntryItemDataWithButtonTypeAndHelper()
    {
    CpItemDataHelper *pHelper1 = new CpItemDataHelper();
    QVERIFY(pHelper1 != 0);
    
    CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp1 = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>( CpSettingFormEntryItemData::ButtonEntryItem, *pHelper1, "text", "description" );
    QVERIFY( pdataimp1 !=0 );
    QString strtxt1 = "input texts";
    pdataimp1->setText(strtxt1);
    QVERIFY( pdataimp1->text() == strtxt1 );
    
    QString strDes1 = "input descriptions";
    pdataimp1->setDescription(strDes1);
    QVERIFY( pdataimp1->description() == strDes1);
    
    QString strIconName1 = "input iconname";
    pdataimp1->setIconName(strIconName1);
    QVERIFY( pdataimp1->iconName() == strIconName1);
    
    HbIcon itemIcon1(QString(""));
    pdataimp1->setEntryItemIcon(itemIcon1);
    QVERIFY( pdataimp1->entryItemIcon() == itemIcon1);
    
    delete pdataimp1;
    delete pHelper1;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
         CpSettingFormEntryItemDataImpl(
            EntryItemType type,
            CpItemDataHelper &itemDataHelper,
            const QString &text = QString(),
            const QString &description = QString(),
            const QString &icon = QString(),
            const HbDataFormModelItem *parent = 0) : 
            CpSettingFormEntryItemData(type,itemDataHelper,text,description,icon,parent);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        \n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
       EntryItemType type\n&nbsp;&nbsp;
       CpItemDataHelper &itemDataHelper\n&nbsp;&nbsp;
       QString &text\n&nbsp;&nbsp;
       QString &description\n&nbsp;&nbsp;
       HbIcon &icon\n&nbsp;&nbsp;
       HbDataFormModelItem *parent\n&nbsp;
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormEntryItemData::testCpSettingFormEntryItemDataWithListTypeAndHelperForm()
    {
    HbDataForm * pForm1 = new HbDataForm();
    QVERIFY(pForm1 != 0);
    
    CpItemDataHelper *pHelper1 = new CpItemDataHelper(pForm1);
    QVERIFY(pHelper1 != 0);
    
    CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp1 = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>( CpSettingFormEntryItemData::ListEntryItem, *pHelper1, "text", "description" );
    QVERIFY( pdataimp1 !=0 );
    QString strtxt1 = "input texts";
    pdataimp1->setText(strtxt1);
    QVERIFY( pdataimp1->text() == strtxt1 );
    
    QString strDes1 = "input descriptions";
    pdataimp1->setDescription(strDes1);
    QVERIFY( pdataimp1->description() == strDes1);
    
    QString strIconName1 = "input iconname";
    pdataimp1->setIconName(strIconName1);
    QVERIFY( pdataimp1->iconName() == strIconName1);
    
    HbIcon itemIcon1(QString(""));
    pdataimp1->setEntryItemIcon(itemIcon1);
    QVERIFY( pdataimp1->entryItemIcon() == itemIcon1);
    
    delete pdataimp1;
    delete pHelper1;
    delete pForm1;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
         CpSettingFormEntryItemDataImpl(
            EntryItemType type,
            CpItemDataHelper &itemDataHelper,
            const QString &text = QString(),
            const QString &description = QString(),
            const QString &icon = QString(),
            const HbDataFormModelItem *parent = 0) : 
            CpSettingFormEntryItemData(type,itemDataHelper,text,description,icon,parent);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        \n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
       EntryItemType type\n&nbsp;&nbsp;
       CpItemDataHelper &itemDataHelper\n&nbsp;&nbsp;
       QString &text\n&nbsp;&nbsp;
       QString &description\n&nbsp;&nbsp;
       HbIcon &icon\n&nbsp;&nbsp;
       HbDataFormModelItem *parent\n&nbsp;
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormEntryItemData::testCpSettingFormEntryItemDataWithButtonTypeAndHelperForm()
    {
    HbDataForm * pForm1 = new HbDataForm();
    QVERIFY(pForm1 != 0);
    
    CpItemDataHelper *pHelper1 = new CpItemDataHelper(pForm1);
    QVERIFY(pHelper1 != 0);
    
    CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp1 = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>( CpSettingFormEntryItemData::ButtonEntryItem, *pHelper1, "text", "description" );
    QVERIFY( pdataimp1 !=0 );
    QString strtxt1 = "input texts";
    pdataimp1->setText(strtxt1);
    QVERIFY( pdataimp1->text() == strtxt1 );
    
    QString strDes1 = "input descriptions";
    pdataimp1->setDescription(strDes1);
    QVERIFY( pdataimp1->description() == strDes1);
    
    QString strIconName1 = "input iconname";
    pdataimp1->setIconName(strIconName1);
    QVERIFY( pdataimp1->iconName() == strIconName1);
    
    HbIcon itemIcon1(QString(""));
    pdataimp1->setEntryItemIcon(itemIcon1);
    QVERIFY( pdataimp1->entryItemIcon() == itemIcon1);
    
    delete pdataimp1;
    delete pHelper1;
    delete pForm1;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
         CpSettingFormEntryItemDataImpl(HbDataForm *dataForm,
            const QString &text = QString(),
            const QString &description = QString(),
            const HbIcon &icon = HbIcon(),
            const HbDataFormModelItem *parent = 0) : 
            CpSettingFormEntryItemData(dataForm,text,description,icon,parent);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        \n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
       HbDataForm *dataForm\n&nbsp;&nbsp;
       QString &text\n&nbsp;&nbsp;
       QString &description\n&nbsp;&nbsp;
       HbIcon &icon\n&nbsp;&nbsp;
       HbDataFormModelItem *parent\n&nbsp;
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormEntryItemData::testCpSettingFormEntryItemDataWithFormParam()
    {
    HbDataForm * form = new HbDataForm();
    QVERIFY(form != 0);
    
    CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp1 = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>(form);
    QVERIFY( pdataimp1 !=0 );
    QString strtxt1 = "input texts";
    pdataimp1->setText(strtxt1);
    QVERIFY( pdataimp1->text() == strtxt1 );
    
    QString strDes1 = "input descriptions";
    pdataimp1->setDescription(strDes1);
    QVERIFY( pdataimp1->description() == strDes1);
    delete pdataimp1;
    delete form;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
         CpSettingFormEntryItemDataImpl(CpItemDataHelper &itemDataHelper,
            const QString &text = QString(),
            const QString &description = QString(),
            const HbIcon &icon = HbIcon(),
            const HbDataFormModelItem *parent = 0) : 
            CpSettingFormEntryItemData(itemDataHelper,text,description,icon,parent);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        \n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
       CpItemDataHelper &itemDataHelper\n&nbsp;&nbsp;
       QString &text\n&nbsp;&nbsp;
       QString &description\n&nbsp;&nbsp;
       HbIcon &icon\n&nbsp;&nbsp;
       HbDataFormModelItem *parent\n&nbsp;
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormEntryItemData::testCpSettingFormEntryItemDataWithHelperParam()
    {
    CpItemDataHelper *pHelper1 = new CpItemDataHelper();
    QVERIFY(pHelper1 != 0);
    
    CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp1 = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>(*pHelper1);
    QVERIFY( pdataimp1 !=0 );
    QString strtxt1 = "input texts";
    pdataimp1->setText(strtxt1);
    QVERIFY( pdataimp1->text() == strtxt1 );
    
    QString strDes1 = "input descriptions";
    pdataimp1->setDescription(strDes1);
    QVERIFY( pdataimp1->description() == strDes1);
    delete pdataimp1;
    delete pHelper1;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
         CpSettingFormEntryItemDataImpl(CpItemDataHelper &itemDataHelper,
            const QString &text = QString(),
            const QString &description = QString(),
            const HbIcon &icon = HbIcon(),
            const HbDataFormModelItem *parent = 0) : 
            CpSettingFormEntryItemData(itemDataHelper,text,description,icon,parent);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        \n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
       CpItemDataHelper &itemDataHelper\n&nbsp;&nbsp;
       QString &text\n&nbsp;&nbsp;
       QString &description\n&nbsp;&nbsp;
       HbIcon &icon\n&nbsp;&nbsp;
       HbDataFormModelItem *parent\n&nbsp;
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormEntryItemData::testCpSettingFormEntryItemDataWithHelperForm()
    {
    HbDataForm * pForm1 = new HbDataForm();
    QVERIFY(pForm1 != 0);
    
    CpItemDataHelper *pHelper1 = new CpItemDataHelper(pForm1);
    QVERIFY(pHelper1 != 0);
    
    CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp1 = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>(*pHelper1);
    QVERIFY( pdataimp1 !=0 );
    QString strtxt1 = "input texts";
    pdataimp1->setText(strtxt1);
    QVERIFY( pdataimp1->text() == strtxt1 );
    
    QString strDes1 = "input descriptions";
    pdataimp1->setDescription(strDes1);
    QVERIFY( pdataimp1->description() == strDes1);
    delete pdataimp1;
    delete pHelper1;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
         CpSettingFormEntryItemDataImpl(
            EntryItemType type,
            CpItemDataHelper &itemDataHelper,
            const QString &text = QString(),
            const QString &description = QString(),
            const QString &icon = QString(),
            const HbDataFormModelItem *parent = 0) : 
            CpSettingFormEntryItemData(type,itemDataHelper,text,description,icon,parent);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        \n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
       EntryItemType type\n&nbsp;&nbsp;
       CpItemDataHelper &itemDataHelper\n&nbsp;&nbsp;
       QString &text\n&nbsp;&nbsp;
       QString &description\n&nbsp;&nbsp;
       HbIcon &icon\n&nbsp;&nbsp;
       HbDataFormModelItem *parent\n&nbsp;
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormEntryItemData::testCpSettingFormEntryItemDataWithListTypeNullPointer()
    {
    HbDataForm * pForm1 = new HbDataForm();
    QVERIFY(pForm1 != 0);
    
    CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp1 = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>( CpSettingFormEntryItemData::ListEntryItem, 0, "text", "description" );
    QVERIFY( pdataimp1 !=0 );
    QString strtxt1 = "input texts";
    pdataimp1->setText(strtxt1);
    QVERIFY( pdataimp1->text() == strtxt1 );
    
    QString strDes1 = "input descriptions";
    pdataimp1->setDescription(strDes1);
    QVERIFY( pdataimp1->description() == strDes1);
    delete pdataimp1;
    delete pForm1;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
         CpSettingFormEntryItemDataImpl(
            EntryItemType type,
            CpItemDataHelper &itemDataHelper,
            const QString &text = QString(),
            const QString &description = QString(),
            const QString &icon = QString(),
            const HbDataFormModelItem *parent = 0) : 
            CpSettingFormEntryItemData(type,itemDataHelper,text,description,icon,parent);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        \n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
       EntryItemType type\n&nbsp;&nbsp;
       CpItemDataHelper &itemDataHelper\n&nbsp;&nbsp;
       QString &text\n&nbsp;&nbsp;
       QString &description\n&nbsp;&nbsp;
       HbIcon &icon\n&nbsp;&nbsp;
       HbDataFormModelItem *parent\n&nbsp;
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormEntryItemData::testCpSettingFormEntryItemDataWithButtonTypeNullPointer()
    {
    HbDataForm * pForm1 = new HbDataForm();
    QVERIFY(pForm1 != 0);
    
    CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp1 = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>( CpSettingFormEntryItemData::ButtonEntryItem, 0, "text", "description" );
    QVERIFY( pdataimp1 !=0 );
    QString strtxt1 = "input texts";
    pdataimp1->setText(strtxt1);
    QVERIFY( pdataimp1->text() == strtxt1 );
    
    QString strDes1 = "input descriptions";
    pdataimp1->setDescription(strDes1);
    QVERIFY( pdataimp1->description() == strDes1);
    delete pdataimp1;
    delete pForm1;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
         CpSettingFormEntryItemDataImpl(
            EntryItemType type,
            CpItemDataHelper &itemDataHelper,
            const QString &text = QString(),
            const QString &description = QString(),
            const QString &icon = QString(),
            const HbDataFormModelItem *parent = 0) : 
            CpSettingFormEntryItemData(type,itemDataHelper,text,description,icon,parent);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        \n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
       EntryItemType type\n&nbsp;&nbsp;
       CpItemDataHelper &itemDataHelper\n&nbsp;&nbsp;
       QString &text\n&nbsp;&nbsp;
       QString &description\n&nbsp;&nbsp;
       HbIcon &icon\n&nbsp;&nbsp;
       HbDataFormModelItem *parent\n&nbsp;
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormEntryItemData::testCpSettingFormEntryItemDataWithInvalidTypeNullPointer()
    {
    HbDataForm * pForm1 = new HbDataForm();
    QVERIFY(pForm1 != 0);
    
    CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp1 = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>( CpSettingFormEntryItemData::ButtonEntryItem, 0, "text", "description" );
    QVERIFY( pdataimp1 !=0 );
    QString strtxt1 = "input texts";
    pdataimp1->setText(strtxt1);
    QVERIFY( pdataimp1->text() == strtxt1 );
    
    QString strDes1 = "input descriptions";
    pdataimp1->setDescription(strDes1);
    QVERIFY( pdataimp1->description() == strDes1);
    delete pdataimp1;
    delete pForm1;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
         CpSettingFormEntryItemData(const HbDataFormModelItem *parent);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        \n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
       HbDataFormModelItem *parent\n&nbsp;
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormEntryItemData::testCpSettingFormEntryItemDataWithParentParam()
    {
    MyCpSettingFormEntryItemData * item = new MyCpSettingFormEntryItemData(0);
    QVERIFY(item != 0);
    
    delete item;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
         onLaunchView();\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        \n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
       QString &text\n&nbsp;&nbsp;
       QString &description\n&nbsp;&nbsp;
       HbIcon &icon\n&nbsp;&nbsp;
       HbDataFormModelItem *parent\n&nbsp;
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormEntryItemData::testOnLaunchViewNoParam()
    {
    HbDataForm *pForm2 = new HbDataForm(0);
    QVERIFY(pForm2 != 0);
    
    CpItemDataHelper *pHelper2 = new CpItemDataHelper(pForm2);
    QVERIFY( pHelper2 != 0 );
    
    CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp2 = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>( *pHelper2, "text", "description"  );
    QVERIFY( pdataimp2 !=0 );
    
    pdataimp2->onLaunchView();
    pdataimp2->onLaunchView();
    
    QString strtxt2 = "input texts";
    pdataimp2->setText(strtxt2);
    QVERIFY( pdataimp2->text() == strtxt2 );
    
    QString strDes2 = "input descriptions";
    pdataimp2->setDescription(strDes2);
    QVERIFY( pdataimp2->description() == strDes2);
    
    delete pdataimp2;
    delete pHelper2;
    delete pForm2;
    }

QTEST_MAIN(TestCpSettingFormEntryItemData)
