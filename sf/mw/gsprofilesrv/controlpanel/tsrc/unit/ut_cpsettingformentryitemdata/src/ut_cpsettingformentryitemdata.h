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

#ifndef UT_CP_SETTING_FORM_ENTRY_ITEM_DATA_H
#define UT_CP_SETTING_FORM_ENTRY_ITEM_DATA_H

#include <QObject>

class TestCpSettingFormEntryItemData : public QObject
{
    Q_OBJECT
    
private slots:
    void initTestCase();
    void cleanupTestCase();

//public slots:
    void testCpSettingFormEntryItemDataWithHelperNoForm();
    void testCpSettingFormEntryItemDataWithHelperAndForm();
    void testCpSettingFormEntryItemDataWithForm();
    void testCpSettingFormEntryItemDataWithListTypeAndForm();
    void testCpSettingFormEntryItemDataWithButtonTypeAndForm();
    void testCpSettingFormEntryItemData005();
    void testCpSettingFormEntryItemData005a();
    void testCpSettingFormEntryItemDataWithListTypeAndHelper();
    void testCpSettingFormEntryItemDataWithButtonTypeAndHelper();
    void testCpSettingFormEntryItemDataWithListTypeAndHelperForm();
    void testCpSettingFormEntryItemDataWithButtonTypeAndHelperForm();
    void testCpSettingFormEntryItemDataWithFormParam();
    void testCpSettingFormEntryItemDataWithHelperParam();
    void testCpSettingFormEntryItemDataWithHelperForm();
    void testCpSettingFormEntryItemDataWithListTypeNullPointer();
    void testCpSettingFormEntryItemDataWithButtonTypeNullPointer();
    void testCpSettingFormEntryItemDataWithInvalidTypeNullPointer();
    void testCpSettingFormEntryItemDataWithParentParam();
    
    void testOnLaunchViewNoParam();
};

#endif // UT_CP_SETTING_FORM_ENTRY_ITEM_DATA_H
