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

#ifndef TEST_CP_SETTING_FORM_ITEM_DATA_H
#define TEST_CP_SETTING_FORM_ITEM_DATA_H

#include <QObject>

class TestCpSettingFormItemData : public QObject
{
    Q_OBJECT
    
private slots:
    void initTestCase();
    void cleanupTestCase();
    
//public slots:
    void testCpSettingFormItemDataWithTypeTextItem();
    void testCpSettingFormItemDataWithTypeSliderItem();
    void testCpSettingFormItemDataWithTypeVolumeSliderItem();
    void testCpSettingFormItemDataWithTypeCheckBoxItem();
    void testCpSettingFormItemDataWithTypeToggleValueItem();
    void testCpSettingFormItemDataWithTypeRadioButtonListItem();
    void testCpSettingFormItemDataWithTypeMultiselectionItem();
    void testCpSettingFormItemDataWithTypeComboBoxItem();
    void testCpSettingFormItemDataWithParent();
    void testCpSettingFormItemDataNoParameter();
    void testCpSettingFormItemDataWithParentNoType();
};

#endif // TEST_CP_SETTING_FORM_ITEM_DATA_H
