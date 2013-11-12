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

#ifndef TEST_CP_ITEM_DATA_HELPER_H
#define TEST_CP_ITEM_DATA_HELPER_H

#include <QObject>

class TestCpItemDataHelper : public QObject
{
    Q_OBJECT
    
private slots:
    void initTestCase();
    void cleanupTestCase();

//public slots:
    void testCpItemDataHelperWithNullPointer();
    void testCpItemDataHelperWithDataForm();
    
    void testBindToFormWithDataForm();
    
    void testAddItemPrototypeWithNullPointer1();
    void testAddItemPrototypeWithViewItem1();
    void testAddItemPrototypeWithNullPointer2();
    void testAddItemPrototypeWithViewItem2();
    
    void testAddConnectionWithnoDataForm();
    void testAddConnectionWithDataForm();
    
    void testRemoveConnectionNoDataForm();
    void testRemoveConnectionWithDataForm();
    
    void testConnectToFormNoDataForm();
    void testConnectToFormWithDataForm();
    
    void testDisconnectToFormNoDataForm();
    void testDisconnectToFormWithDataForm();
    
    void testWidgetFromModelIndexWithDataForm();
    void testWidgetFromModelIndexNoDataForm();
    void testWidgetFromModelIndexWithDataFormBinded();
    
    void testModelItemFromModelIndexWithDataForm();
    void testModelItemFromModelIndexNoDataForm();
    void testModelItemFromModelIndexWithModel();
};

#endif // TEST_CP_ITEM_DATA_HELPER_H
