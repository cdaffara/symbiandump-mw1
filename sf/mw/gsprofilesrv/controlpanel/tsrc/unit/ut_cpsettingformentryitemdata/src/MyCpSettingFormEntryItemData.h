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

#include "cpsettingformentryitemdata.h"
#include <cpbasesettingview.h>

class MyCpSettingFormEntryItemData : public CpSettingFormEntryItemData
    {
public:
    explicit MyCpSettingFormEntryItemData(const HbDataFormModelItem *parent = 0);
    
    virtual CpBaseSettingView *createSettingView() const
    { return new CpBaseSettingView(); }
    };
