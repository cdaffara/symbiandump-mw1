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

#include "ut_cpsettingformitemdata.h"

#include <cpsettingformitemdata.h>
#include <QtTest/QtTest>

/*!
    \class TestCpSettingFormItemData
    \brief This class is used for the unit test for class CpSettingFormItemData.\n
      CpSettingFormItemData class is derived from HbDataFormModelItem, customed model item for control panel.\n
      This unit test is supported by QTest.\n
      There are total 11 test cases in this unit.\n 
 */

void TestCpSettingFormItemData::initTestCase()
    {
    }

void TestCpSettingFormItemData::cleanupTestCase()
    {
    QCoreApplication::processEvents();
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
      CpSettingFormItemData(HbDataFormModelItem::DataItemType type,
        const QString &label,
        const HbDataFormModelItem *parent = 0);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        Construct a new CpSettingFormItemData with the given type,label and parent.\n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
           HbDataFormModelItem::DataItemType type\n&nbsp;&nbsp;
           QString &label\n&nbsp;&nbsp;
           HbDataFormModelItem *parent\n&nbsp;
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormItemData::testCpSettingFormItemDataWithTypeTextItem()
    {
    CpSettingFormItemData *pdata = new CpSettingFormItemData( HbDataFormModelItem::TextItem, "label" );
    QVERIFY( pdata !=0 );
    QVERIFY( pdata->type() == HbDataFormModelItem::TextItem );
    QVERIFY( pdata->label() == "label");
    
    delete pdata;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
      CpSettingFormItemData(HbDataFormModelItem::DataItemType type,
        const QString &label,
        const HbDataFormModelItem *parent = 0);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        Construct a new CpSettingFormItemData with the given type,label and parent.\n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
           HbDataFormModelItem::DataItemType type\n&nbsp;&nbsp;
           QString &label\n&nbsp;&nbsp;
           HbDataFormModelItem *parent\n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormItemData::testCpSettingFormItemDataWithTypeSliderItem()
    {
    CpSettingFormItemData *pdata = new CpSettingFormItemData( HbDataFormModelItem::SliderItem, "label" );
    QVERIFY( pdata !=0 );
    QVERIFY( pdata->type() == HbDataFormModelItem::SliderItem );
    QVERIFY( pdata->label() == "label");
    
    delete pdata;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
      CpSettingFormItemData(HbDataFormModelItem::DataItemType type,
        const QString &label,
        const HbDataFormModelItem *parent = 0);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        Construct a new CpSettingFormItemData with the given type,label and parent.\n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
           HbDataFormModelItem::DataItemType type\n&nbsp;&nbsp;
           QString &label\n&nbsp;&nbsp;
           HbDataFormModelItem *parent\n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormItemData::testCpSettingFormItemDataWithTypeVolumeSliderItem()
    {
    CpSettingFormItemData * pdata = new CpSettingFormItemData( HbDataFormModelItem::VolumeSliderItem, "label" );
    QVERIFY( pdata !=0 );
    QVERIFY( pdata->type() == HbDataFormModelItem::VolumeSliderItem );
    QVERIFY( pdata->label() == "label");
    
    delete pdata;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
      CpSettingFormItemData(HbDataFormModelItem::DataItemType type,
        const QString &label,
        const HbDataFormModelItem *parent = 0);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        Construct a new CpSettingFormItemData with the given type,label and parent.\n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
           HbDataFormModelItem::DataItemType type\n&nbsp;&nbsp;
           QString &label\n&nbsp;&nbsp;
           HbDataFormModelItem *parent\n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormItemData::testCpSettingFormItemDataWithTypeCheckBoxItem()
    {
    CpSettingFormItemData * pdata = new CpSettingFormItemData( HbDataFormModelItem::CheckBoxItem, "label" );
    QVERIFY( pdata !=0 );
    QVERIFY( pdata->type() == HbDataFormModelItem::CheckBoxItem );
    QVERIFY( pdata->label() == "label");
    
    delete pdata;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
      CpSettingFormItemData(HbDataFormModelItem::DataItemType type,
        const QString &label,
        const HbDataFormModelItem *parent = 0);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        Construct a new CpSettingFormItemData with the given type,label and parent.\n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
           HbDataFormModelItem::DataItemType type\n&nbsp;&nbsp;
           QString &label\n&nbsp;&nbsp;
           HbDataFormModelItem *parent\n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormItemData::testCpSettingFormItemDataWithTypeToggleValueItem()
    {
    CpSettingFormItemData * pdata = new CpSettingFormItemData( HbDataFormModelItem::ToggleValueItem, "label" );
    QVERIFY( pdata !=0 );
    QVERIFY( pdata->type() == HbDataFormModelItem::ToggleValueItem );
    QVERIFY( pdata->label() == "label");
    
    delete pdata;
    }
    
/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
      CpSettingFormItemData(HbDataFormModelItem::DataItemType type,
        const QString &label,
        const HbDataFormModelItem *parent = 0);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        Construct a new CpSettingFormItemData with the given type,label and parent.\n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
           HbDataFormModelItem::DataItemType type\n&nbsp;&nbsp;
           QString &label\n&nbsp;&nbsp;
           HbDataFormModelItem *parent\n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormItemData::testCpSettingFormItemDataWithTypeRadioButtonListItem()
    {
    CpSettingFormItemData * pdata = new CpSettingFormItemData( HbDataFormModelItem::RadioButtonListItem, "label" );
    QVERIFY( pdata !=0 );
    QVERIFY( pdata->type() == HbDataFormModelItem::RadioButtonListItem );
    QVERIFY( pdata->label() == "label");
    
    delete pdata;
    }
    
/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
      CpSettingFormItemData(HbDataFormModelItem::DataItemType type,
        const QString &label,
        const HbDataFormModelItem *parent = 0);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        Construct a new CpSettingFormItemData with the given type,label and parent.\n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
           HbDataFormModelItem::DataItemType type\n&nbsp;&nbsp;
           QString &label\n&nbsp;&nbsp;
           HbDataFormModelItem *parent\n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormItemData::testCpSettingFormItemDataWithTypeMultiselectionItem()
    {
    CpSettingFormItemData * pdata = new CpSettingFormItemData( HbDataFormModelItem::MultiselectionItem, "label" );
    QVERIFY( pdata !=0 );
    QVERIFY( pdata->type() == HbDataFormModelItem::MultiselectionItem );
    QVERIFY( pdata->label() == "label");
    
    delete pdata;
    }
    
/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
      CpSettingFormItemData(HbDataFormModelItem::DataItemType type,
        const QString &label,
        const HbDataFormModelItem *parent = 0);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        Construct a new CpSettingFormItemData with the given type,label and parent.\n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
           HbDataFormModelItem::DataItemType type\n&nbsp;&nbsp;
           QString &label\n&nbsp;&nbsp;
           HbDataFormModelItem *parent\n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormItemData::testCpSettingFormItemDataWithTypeComboBoxItem()
    {
    CpSettingFormItemData * pdata = new CpSettingFormItemData( HbDataFormModelItem::ComboBoxItem, "label" );
    QVERIFY( pdata !=0 );
    QVERIFY( pdata->type() == HbDataFormModelItem::ComboBoxItem );
    QVERIFY( pdata->label() == "label");
    
    delete pdata;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
      CpSettingFormItemData(HbDataFormModelItem::DataItemType type,
        const QString &label,
        const HbDataFormModelItem *parent = 0);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        Construct a new CpSettingFormItemData with the given type,label and parent.\n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
           HbDataFormModelItem::DataItemType type\n&nbsp;&nbsp;
           QString &label\n&nbsp;&nbsp;
           HbDataFormModelItem *parent\n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormItemData::testCpSettingFormItemDataWithParent()
    {
    CpSettingFormItemData * pdata = new CpSettingFormItemData( HbDataFormModelItem::SliderItem, "parent", 0 );
    QVERIFY( pdata !=0 );
    QVERIFY( pdata->type() == HbDataFormModelItem::SliderItem );

    CpSettingFormItemData * cdata = new CpSettingFormItemData( HbDataFormModelItem::TextItem, "label", pdata);
    QVERIFY( cdata != 0 );
    QVERIFY( cdata->type() == HbDataFormModelItem::TextItem );
    
    delete pdata;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
      CpSettingFormItemData(HbDataFormModelItem::DataItemType type,
        const QString &label,
        const HbDataFormModelItem *parent = 0);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        Construct a new CpSettingFormItemData with the given type,label and parent.\n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
           HbDataFormModelItem::DataItemType type\n&nbsp;&nbsp;
           QString &label\n&nbsp;&nbsp;
           HbDataFormModelItem *parent\n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormItemData::testCpSettingFormItemDataNoParameter()
    {
    CpSettingFormItemData * pdata = new CpSettingFormItemData();
    QVERIFY( pdata !=0 );
    pdata->setType(HbDataFormModelItem::SliderItem);
    pdata->setLabel("label");
    
    QVERIFY( pdata->type() == HbDataFormModelItem::SliderItem );
    delete pdata;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
      CpSettingFormItemData(HbDataFormModelItem::DataItemType type,
        const QString &label,
        const HbDataFormModelItem *parent = 0);\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        Construct a new CpSettingFormItemData with the given type,label and parent.\n&nbsp;
   3. Input Parameters: \n&nbsp;&nbsp;
           HbDataFormModelItem::DataItemType type\n&nbsp;&nbsp;
           QString &label\n&nbsp;&nbsp;
           HbDataFormModelItem *parent\n&nbsp;
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpSettingFormItemData::testCpSettingFormItemDataWithParentNoType()
    {
    CpSettingFormItemData * pdata = new CpSettingFormItemData( HbDataFormModelItem::SliderItem, "parent", 0 );
    QVERIFY( pdata !=0 );
    QVERIFY( pdata->type() == HbDataFormModelItem::SliderItem );

    CpSettingFormItemData * cdata = new CpSettingFormItemData(pdata);
    QVERIFY( cdata != 0 );
    cdata->setType(HbDataFormModelItem::TextItem);
    QVERIFY( cdata->type() == HbDataFormModelItem::TextItem );
    
    delete pdata;
    }

QTEST_APPLESS_MAIN(TestCpSettingFormItemData)
