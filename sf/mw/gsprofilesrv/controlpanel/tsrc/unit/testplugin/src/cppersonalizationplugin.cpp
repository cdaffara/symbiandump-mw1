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
 *
 */

#include "cppersonalizationplugin.h"
#include <cpitemdatahelper.h>
#include <cpsettingformitemdata.h>
#include <cpbasesettingview.h>

CpPersonalizationPlugin::CpPersonalizationPlugin()
{
}

CpPersonalizationPlugin::~CpPersonalizationPlugin()
{
}

QList<CpSettingFormItemData*> CpPersonalizationPlugin::createSettingFormItemData(CpItemDataHelper &itemDataHelper) const
{
    CpSettingFormItemData *itemData = new CpSettingFormItemData();

    return QList<CpSettingFormItemData*>() << itemData;
}

CpBaseSettingView *CpPersonalizationPlugin::createSettingView(const QVariant &hint) const
{
    if (hint.toString().compare("profile_view",Qt::CaseInsensitive) == 0) {
        return new CpBaseSettingView;
    }
    return 0;
}

Q_EXPORT_PLUGIN2(cppersonalizationplugin, CpPersonalizationPlugin);


