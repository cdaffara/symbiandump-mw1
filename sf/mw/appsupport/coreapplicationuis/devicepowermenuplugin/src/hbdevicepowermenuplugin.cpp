/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of "Eclipse Public License v1.0"
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

//System includes
#include <QtPlugin>
#include <hbdevicedialog.h>

//user includes
#include "hbdevicepowermenuplugin_p.h"
#include "hbdevicepowermenuwidegt_p.h"
#include "hbdevicepowermenupluginerrors_p.h"

//SysAp's Secure UID
const quint32 KSecureUid = 0x100058F3;

Q_EXPORT_PLUGIN2(devicepowermenuplugin, HbDevicePowerMenuPlugin)

// This plugin implements a device dialog type
static const struct 
	{
    const char *mTypeString;
	} dialogInfos[] = {{"com.nokia.hb.devicepowermenu/1.0"}};

class HbDevicePowerMenuPluginPrivate
	{
public:
    HbDevicePowerMenuPluginPrivate() {mError = NoError;}
    int mError;
	};

/**
 * Constructor
 */
HbDevicePowerMenuPlugin::HbDevicePowerMenuPlugin()
	{
    TRACES( RDebug::Print( _L("HbDevicePowerMenuPlugin::Constructor:Begin") ) );
    d = new HbDevicePowerMenuPluginPrivate;
    TRACES( RDebug::Print( _L("HbDevicePowerMenuPlugin::Constructor:End") ) );
	}

/**
 * Destructor
 */
HbDevicePowerMenuPlugin::~HbDevicePowerMenuPlugin()
	{
	TRACES( RDebug::Print( _L("HbDevicePowerMenuPlugin::Destructor:Begin") ) );
    delete d;
    TRACES( RDebug::Print( _L("HbDevicePowerMenuPlugin::Destructor:End") ) );
	}

/**
 * This method is Pure Virtual in HbDeviceDialogPlugin 
 * Checks if client is allowed to use device dialog widget
 * Its a customized widget for Sysap, and allows only Sysap to use this plugin
 */
bool HbDevicePowerMenuPlugin::accessAllowed(const QString &deviceDialogType, const QVariantMap &parameters, const QVariantMap &securityInfo) const
    {
	TRACES( RDebug::Print( _L("HbDevicePowerMenuPlugin::accessAllowed:Begin") ) );
    Q_UNUSED(deviceDialogType)
    Q_UNUSED(parameters)
    TBool secured(false);
    quint32 secUid = static_cast<quint32>(securityInfo.value("sym-secureId").toInt());
    if (secUid == KSecureUid )
	    {
        secured = true;
	    }
    TRACES( RDebug::Print( _L("HbDevicePowerMenuPlugin::accessAllowed:End") ) );
    return secured;
    }

/**
 * Creates device dialog widget
 */
HbDeviceDialogInterface *HbDevicePowerMenuPlugin::createDeviceDialog(const QString &deviceDialogType, const QVariantMap &parameters)
	{
	TRACES( RDebug::Print( _L("HbDevicePowerMenuPlugin::createDeviceDialog:Begin") ) );
    d->mError = NoError;

    HbDeviceDialogInterface *ret(NULL);
    if (dialogInfos[0].mTypeString == deviceDialogType) 
		{
		 QVariantMap params = parameters;
		 HbDevicePowerMenuWidget *powMenu = new HbDevicePowerMenuWidget(params);
		 d->mError = powMenu->deviceDialogError();
		 if (d->mError != NoError) 
			 {
			 delete powMenu;
			 powMenu = NULL;
			 }
		 ret = powMenu;
		} 
    else 
		{
		 d->mError = UnknownDeviceDialogError;
		 ret = NULL;
		}
    TRACES( RDebug::Print( _L("HbDevicePowerMenuPlugin::createDeviceDialog:End") ) );
    return ret;
	}

/**
 * Return information of device dialog the plugin creates
 */
bool HbDevicePowerMenuPlugin::deviceDialogInfo(const QString &deviceDialogType, const QVariantMap &parameters, DeviceDialogInfo *info) const
	{
	TRACES( RDebug::Print( _L("HbDevicePowerMenuPlugin::deviceDialogInfo:Begin") ) );
    Q_UNUSED(parameters)
    Q_UNUSED(deviceDialogType)

    info->group = GenericDeviceDialogGroup;
    info->flags = SecurityCheck;
    info->priority = DefaultPriority;
    TRACES( RDebug::Print( _L("HbDevicePowerMenuPlugin::deviceDialogInfo:End") ) );
    return true;
	}

/**
 * Return device dialog types this plugin implements
 */
QStringList HbDevicePowerMenuPlugin::deviceDialogTypes() const
	{
	TRACES( RDebug::Print( _L("HbDevicePowerMenuPlugin::deviceDialogTypes:Begin") ) );
	QStringList types;
	const int numTypes = sizeof(dialogInfos) / sizeof(dialogInfos[0]);
	for(int i = 0; i < numTypes; i++) 
		{
		types.append(dialogInfos[i].mTypeString);
		}
	TRACES( RDebug::Print( _L("HbDevicePowerMenuPlugin::deviceDialogTypes:End") ) );
	return types;
	}

/**
 * Return plugin flags
 */
HbDeviceDialogPlugin::PluginFlags HbDevicePowerMenuPlugin::pluginFlags() const
	{
	TRACES( RDebug::Print( _L("HbDevicePowerMenuPlugin::pluginFlags:Begin") ) );
	TRACES( RDebug::Print( _L("HbDevicePowerMenuPlugin::pluginFlags:End") ) );
	return NoPluginFlags;
	}

/**
 * Return last error
 */
int HbDevicePowerMenuPlugin::error() const
	{
	TRACES( RDebug::Print( _L("HbDevicePowerMenuPlugin::error:Begin") ) );
	TRACES( RDebug::Print( _L("HbDevicePowerMenuPlugin::error:End") ) );
	return d->mError;
	}

