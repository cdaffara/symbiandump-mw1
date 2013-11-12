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
 * Description:  
 *
 */
#include "cppsmplugin.h"
#include "cppsmview.h"
#include <cpsettingformentryitemdataimpl.h>
#include <e32debug.h>


CpPsmPlugin::CpPsmPlugin()
{
	RDebug::Print( _L("CpPsmPlugin::constructor:Begin") );
    iTrans=new  HbTranslator("powermanagement");
    RDebug::Print( _L("CpPsmPlugin::constructor:end") );
}

CpPsmPlugin::~CpPsmPlugin()
{
	RDebug::Print( _L("CpPsmPlugin::destructor:Begin") );
    delete iTrans;
    RDebug::Print( _L("CpPsmPlugin::destructor:end"));  
}
QList<CpSettingFormItemData*> CpPsmPlugin::createSettingFormItemData(CpItemDataHelper &itemDataHelper) const
{
   //QList<T> is represented as an array of pointers to items of type T,here T is CpSettingFormItemData 
   //and here the << function is used which appends object to the list
   RDebug::Print( _L("CpPsmPlugin::createSettingFormItemData:Begin") );
   return QList<CpSettingFormItemData*>() 
            << new CpSettingFormEntryItemDataImpl<CpPsmView>(
			   itemDataHelper,
               hbTrId("txt_power_management_subhead_power_management"), 
			   hbTrId("txt_power_list_power_save_mode"),
			   QString("qtg_large_power_management"));
    
}

CpBaseSettingView *CpPsmPlugin::createSettingView(const QVariant &hint) const
{
    RDebug::Print( _L("CpPsmPlugin::createSettingView:Begin") );
    if (hint.toString().compare("psm_view",Qt::CaseInsensitive) == 0) {
        return new CpPsmView;
    }
    RDebug::Print( _L("CpPsmPlugin::createSettingView:end") );
    return 0;
}

Q_EXPORT_PLUGIN2(cppsmplugin, CpPsmPlugin);
