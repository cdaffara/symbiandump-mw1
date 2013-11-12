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

#include "ut_cppluginutility.h"

#include <QtTest/QtTest>
#include <hbdataform.h>
#include <cppluginutility.h>

/*!
    \class TestCpPluginUtility
    \brief This class is used for the unit test for class CpPluginUtility.\n
      CpPluginUtility class is a utility class used to provide method to add 2 two new customed prototype into the HbDataForm object. \n
      This unit test is supported by QTest.
      There are total 2 test cases in this unit.
 */

void TestCpPluginUtility::initTestCase()
    {
    }

void TestCpPluginUtility::cleanupTestCase()
    {
    QCoreApplication::processEvents();
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: addCpItemPrototype() \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
        add 2 new prototype to a HbDataForm object.\n&nbsp;
   3. Input Parameters: \n&nbsp;
       @param: HbDataForm *settingForm\n
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpPluginUtility::testAddCpItemPrototypeWithDataForm()
    {
    HbDataForm * settingForm = new HbDataForm();
    QVERIFY(settingForm != 0);
    
    int count0 = settingForm->itemPrototypes().count();
    CpPluginUtility::addCpItemPrototype(settingForm);
    int count1 = settingForm->itemPrototypes().count();
    QVERIFY(count0 + 2 == count1);
    
    delete settingForm;
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: addCpItemPrototype() \n&nbsp;
   2. Function Descrition: \n &nbsp;&nbsp;
        add 2 new prototype to a HbDataForm object.\n&nbsp;
   3. Input Parameters: \n&nbsp;
       @param: 0.\n
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpPluginUtility::testAddCpItemPrototypeWithNullPointer()
    {
    CpPluginUtility::addCpItemPrototype(0);
    }

QTEST_MAIN(TestCpPluginUtility)
