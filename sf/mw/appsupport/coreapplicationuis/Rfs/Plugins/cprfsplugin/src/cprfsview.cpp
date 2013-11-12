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
 * Description:  Creates the settingForm for RFS plug-in where we can add setting item to the form later.
 *
 */

#include "cprfsview.h"
#include "cprfssettingsform.h"

#include <cpbasesettingview.h>
#include "cprfssettingsdataformcustomitem.h"

CpRfsView::CpRfsView(QGraphicsItem *parent):
CpBaseSettingView(0,parent)
	{
    CpRfsSettingsForm* settingForm = new CpRfsSettingsForm(this);
        
    //custom prototype
    CpRfsSettingsDataFormCustomItem* customPrototype = new CpRfsSettingsDataFormCustomItem(settingForm);
    
    QList <HbAbstractViewItem*> protos = settingForm->itemPrototypes();
    protos.append(customPrototype);
    settingForm->setItemPrototypes(protos);    
    //function from hbview
    setWidget(settingForm);
    
	}
CpRfsView::~CpRfsView()
    {
    }
