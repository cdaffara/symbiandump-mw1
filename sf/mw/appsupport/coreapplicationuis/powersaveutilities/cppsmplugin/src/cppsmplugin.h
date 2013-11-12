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
#ifndef	CPPSMPLUGIN_H
#define	CPPSMPLUGIN_H

#include <qobject.h>
#include <cpplugininterface.h>
#include <cplauncherinterface.h>
#include <hbtranslator.h>

class CpBaseSettingView;
class QVariant;

class CpPsmPlugin : public QObject, public CpPluginInterface,public CpLauncherInterface
{
    Q_OBJECT
    Q_INTERFACES(CpPluginInterface)
    Q_INTERFACES(CpLauncherInterface)
public:
    CpPsmPlugin();
    virtual ~CpPsmPlugin();
    //from CpPluginInterface
    virtual QList<CpSettingFormItemData*> createSettingFormItemData(CpItemDataHelper &itemDataHelper) const;
    //from CpLauncherInterface
    virtual CpBaseSettingView *createSettingView(const QVariant &hint) const;
private:
    HbTranslator* iTrans;
};

#endif	//CPPSMPLUGIN_H
