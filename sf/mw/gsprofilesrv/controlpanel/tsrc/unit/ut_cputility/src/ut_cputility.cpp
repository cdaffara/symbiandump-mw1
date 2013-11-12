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
#include "ut_cputility.h"
#include "cputility.h"

#include <QtTest/QtTest>

/*!
    \class TestCpUtility
    \brief This class is used for the unit test for class CpUtility.\n
      CpUtility class is an utility class to provide functionalities about drive and dir related processing.\n
      This unit test is supported by QTest.\n
      There are total 6 test cases in this unit.\n 
 */

void TestCpUtility::initTestCase()
    {
    }

void TestCpUtility::cleanupTestCase()
    {
    QCoreApplication::processEvents();
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
      static CpUtility::drives();\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        Get and store driver letter into QStringList object, using QDir::drives().\n&nbsp;
   3. Input Parameters: none.\n&nbsp;&nbsp;
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpUtility::testDrivesOnce()
    {
    QStringList strListDrives = CpUtility::drives();
    QVERIFY(!strListDrives.isEmpty());
    
    verifyDrivesList(strListDrives);
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
      static CpUtility::drives();\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        Get and store driver letter into QStringList object, using QDir::drives().\n&nbsp;
   3. Input Parameters: none.\n&nbsp;&nbsp;
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpUtility::testDrivesRepeatTwice()
    {
    QStringList strListDrives1 = CpUtility::drives();
    QVERIFY(!strListDrives1.isEmpty());
    
    verifyDrivesList(strListDrives1);
    
    QStringList strListDrives2 = CpUtility::drives();
    QVERIFY(!strListDrives2.isEmpty());
    
    verifyDrivesList(strListDrives2);
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
      static CpUtility::pluginDirectories();\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        Get and store plugin directories into QStringList object.\n&nbsp;
   3. Input Parameters: none.\n&nbsp;&nbsp;
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpUtility::testPluginDirectoriesOnce()
    {
    QStringList strListDirs = CpUtility::pluginDirectories();
    QVERIFY(!strListDirs.isEmpty());
    
    verifyDirectoriesList(strListDirs);
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
      static CpUtility::pluginDirectories();\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        Get and store plugin directories into QStringList object.\n&nbsp;
   3. Input Parameters: none.\n&nbsp;&nbsp;
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpUtility::testPluginDirectoriesRepeatTwice()
    {
    QStringList strListDirs1 = CpUtility::pluginDirectories();
    QVERIFY(!strListDirs1.isEmpty());
    
    verifyDirectoriesList(strListDirs1);
    
    QStringList strListDirs2 = CpUtility::pluginDirectories();
    QVERIFY(!strListDirs2.isEmpty());
    
    verifyDirectoriesList(strListDirs2);
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
      static CpUtility::configFileDirectories();\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        Get and store cpcfg file directories into QStringList object.\n&nbsp;
   3. Input Parameters: none.\n&nbsp;&nbsp;
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpUtility::testConfigFileDirectoriesOnce()
    {
    QStringList strListConfigDirs = CpUtility::configFileDirectories();
    QVERIFY(!strListConfigDirs.isEmpty());
    
    verifyConfigFileDirectories(strListConfigDirs);
    }

/*!
 * Test Case Description:\n &nbsp;
 * 1. Fucntion Name: \n&nbsp;&nbsp;
      static CpUtility::configFileDirectories();\n&nbsp;
            
   2. Function Descrition: \n &nbsp;&nbsp;
        Get and store cpcfg file directories into QStringList object.\n&nbsp;
   3. Input Parameters: none.\n&nbsp;&nbsp;
       
   4. Expected result: \n&nbsp;&nbsp;
       case run without verify failed.\n 
 */
void TestCpUtility::testConfigFileDirectoriesRepeatTwice()
    {
    QStringList strListConfigDirs1 = CpUtility::configFileDirectories();
    QVERIFY(!strListConfigDirs1.isEmpty());
    
    verifyConfigFileDirectories(strListConfigDirs1);
    
    QStringList strListConfigDirs2 = CpUtility::configFileDirectories();
    QVERIFY(!strListConfigDirs2.isEmpty());
    
    verifyConfigFileDirectories(strListConfigDirs2);
    }

void TestCpUtility::verifyDrivesList(const QStringList &strList)
    {
    QVERIFY( strList.contains("C:", Qt::CaseInsensitive) );
    QVERIFY( strList.contains("Z:", Qt::CaseInsensitive) );
    }

void TestCpUtility::verifyDirectoriesList(const QStringList &strList)
    {
    QVERIFY( strList.contains(QString("C:/resource/qt/plugins/controlpanel") + QDir::separator(), Qt::CaseInsensitive));
    }

void TestCpUtility::verifyConfigFileDirectories(const QStringList &strList)
    {
    QVERIFY( strList.contains(QString("C:/resource/qt/plugins/controlpanel/config") + QDir::separator(), Qt::CaseInsensitive));
    }

QTEST_APPLESS_MAIN(TestCpUtility)
