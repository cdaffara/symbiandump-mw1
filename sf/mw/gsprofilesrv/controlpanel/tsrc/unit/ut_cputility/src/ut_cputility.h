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

#ifndef TEST_CP_UTILITY_H
#define TEST_CP_UTILITY_H

#include <QObject>

class TestCpUtility : public QObject
{
    Q_OBJECT
    
private slots:
    void initTestCase();
    void cleanupTestCase();
    
//public slots:
    void testDrivesOnce();
    void testDrivesRepeatTwice();
    
    void testPluginDirectoriesOnce();
    void testPluginDirectoriesRepeatTwice();
    
    void testConfigFileDirectoriesOnce();
    void testConfigFileDirectoriesRepeatTwice();
    
private:
    void verifyDrivesList(const QStringList &strList);
    void verifyDirectoriesList(const QStringList &strList);
    void verifyConfigFileDirectories(const QStringList &strList);
};

#endif // TEST_CP_PLUGIN_UTILITY_H
