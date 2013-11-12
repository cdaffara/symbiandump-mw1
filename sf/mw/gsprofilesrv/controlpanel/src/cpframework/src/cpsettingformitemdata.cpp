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
* Description:  This class is a control panel defined HbDataFormModelItem
*
*/
#include "cpsettingformitemdata.h"

/*!
    \class CpSettingFormItemData
    \brief The CpSettingFormItemData is a extension to HbSettingItemData.
    With this class, user can monitor the value change event by connecting a slot to valueChanged slot.
 */

/*!
    Construct a new CpSettingFormItemData with the given type,label and parent.
*/
CpSettingFormItemData::CpSettingFormItemData(
        HbDataFormModelItem::DataItemType type,
        const QString &label,
        const HbDataFormModelItem *parent /*= 0*/) :   
        QObject(),
        HbDataFormModelItem(type,label,parent)

{
}

/*!
    Construct a new CpSettingFormItemData with the given parent.
*/
CpSettingFormItemData::CpSettingFormItemData(const HbDataFormModelItem *parent /*= 0*/) : 
        QObject(),
        HbDataFormModelItem(parent)
{
}

/*!
     Destructor.
*/
CpSettingFormItemData::~CpSettingFormItemData()
{
}

