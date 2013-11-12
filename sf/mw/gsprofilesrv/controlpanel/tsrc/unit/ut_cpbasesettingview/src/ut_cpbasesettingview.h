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

#ifndef UT_CP_BASE_SETTING_VIEW_H
#define UT_CP_BASE_SETTING_VIEW_H

#include <QObject>

class TestCpBaseSettingView : public QObject
{
    Q_OBJECT
    
private slots:
    void initTestCase();
    void cleanupTestCase();
    
//public slots:
    /*!
     * Test case 1 for constructor of class CpBaseSettingView
     */
    void testCpBaseSettingViewWithNullPointer();
    /*!
     * Test case 2 for constructor of class CpBaseSettingView
     */
    void testCpBaseSettingViewWithPushButtonWidget();
    /*!
     * Test case for protected close() function
     */
    void testCloseWithoutParam();
    
    void testPrivateFunc();
};

#endif // UT_CP_BASE_SETTING_VIEW_H
