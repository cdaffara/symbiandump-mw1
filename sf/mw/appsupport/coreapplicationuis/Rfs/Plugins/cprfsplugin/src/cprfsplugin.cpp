/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  Creates the plug-in to the control panel.Creates a setting form 
 * item data, by which the control panel framework can build a setting form model 
 *
 */
#include "cprfsplugin.h"
#include "cprfsview.h"
#include <cpsettingformentryitemdataimpl.h>
#include <cpitemdatahelper.h>
#include <hbtranslator.h>

CpRfsPlugin::CpRfsPlugin()
    {
    }

CpRfsPlugin::~CpRfsPlugin()
    {
    }
QList<CpSettingFormItemData*> CpRfsPlugin::createSettingFormItemData(CpItemDataHelper &itemDataHelper) const
    {
    HbTranslator trans("control_panel");
    return QList<CpSettingFormItemData*>() 
                << new CpSettingFormEntryItemDataImpl<CpRfsView>(itemDataHelper,hbTrId("txt_cp_dblist_reset"),hbTrId("txt_cp_dblist_reset_val_reset_your_device"),QString("qtg_large_reset"));
    }

Q_EXPORT_PLUGIN2(cprfsplugin, CpRfsPlugin);
