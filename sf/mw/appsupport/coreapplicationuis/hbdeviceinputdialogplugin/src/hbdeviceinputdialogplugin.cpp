/****************************************************************************
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
 ***************************************************************************/

#include <QtPlugin>

#include <hbdevicedialog.h>
//#include <hbdevicedialogtrace_p.h>
#include "hbdeviceinputdialogplugin_p.h"
#include "hbdeviceinputdialogwidget_p.h"
#include "hbdeviceinputdialogpluginerrors_p.h"

Q_EXPORT_PLUGIN2(deviceinputdialogplugin, HbDeviceInputDialogPlugin)

// This plugin implements one device dialog type
static const struct {
    const char *mTypeString;
} dialogInfos[] = {
    {"com.nokia.hb.deviceinputdialog/1.0"}
};

class HbDeviceInputDialogPluginPrivate
{
public:
    HbDeviceInputDialogPluginPrivate() {mError = NoError;}

    int mError;
};

// Constructor
HbDeviceInputDialogPlugin::HbDeviceInputDialogPlugin()
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogPlugin::Constructor:Begin") ) );
    d = new HbDeviceInputDialogPluginPrivate;
    TRACES( RDebug::Print( _L("HbDeviceInputDialogPlugin::Constructor:End") ) );
}

// Destructor
HbDeviceInputDialogPlugin::~HbDeviceInputDialogPlugin()
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogPlugin::Destructor:Begin") ) );
    delete d;
    TRACES( RDebug::Print( _L("HbDeviceInputDialogPlugin::Destructor:End") ) );
}

// Check if client is allowed to use device dialog widget
bool HbDeviceInputDialogPlugin::accessAllowed(const QString &deviceDialogType,
    const QVariantMap &parameters, const QVariantMap &securityInfo) const
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogPlugin::AccessAllowed:Begin") ) );
    Q_UNUSED(deviceDialogType)
    Q_UNUSED(parameters)
    Q_UNUSED(securityInfo)

    // This plugin doesn't perform operations that may compromise security.
    // All clients are allowed to use.
    TRACES( RDebug::Print( _L("HbDeviceInputDialogPlugin::AccessAllowed:End") ) );    
    return true;

}

// Create device dialog widget
HbDeviceDialogInterface *HbDeviceInputDialogPlugin::createDeviceDialog(const QString &deviceDialogType, const QVariantMap &parameters)
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogPlugin::createDeviceDialog:Begin") ) );
    d->mError = NoError;

    HbDeviceDialogInterface *ret(0);
    int i(0);
    const int numTypes = sizeof(dialogInfos) / sizeof(dialogInfos[0]);
    for(i = 0; i < numTypes; i++) {
        if (dialogInfos[i].mTypeString == deviceDialogType) {
            break;
        }
    }
    if (i < numTypes) {
        QVariantMap params = parameters;
        HbDeviceInputDialogWidget *deviceDialog = new HbDeviceInputDialogWidget(params);
        d->mError = deviceDialog->deviceDialogError();
        if (d->mError != NoError) {
            delete deviceDialog;
            deviceDialog = 0;
        }
        ret = deviceDialog;
    } else {
        d->mError = UnknownDeviceDialogError;
        ret = 0;
    }
    TRACES( RDebug::Print( _L("HbDeviceInputDialogPlugin::createDeviceDialog:End") ) );
    return ret;
}

// Return information of device dialog the plugin creates
bool HbDeviceInputDialogPlugin::deviceDialogInfo(const QString &deviceDialogType, const QVariantMap &parameters, DeviceDialogInfo *info) const
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogPlugin::deviceDialogInfo:Begin") ) );
    Q_UNUSED(parameters)
    Q_UNUSED(deviceDialogType)

    info->group = GenericDeviceDialogGroup;
    info->flags = NoDeviceDialogFlags;
    info->priority = DefaultPriority;
    TRACES( RDebug::Print( _L("HbDeviceInputDialogPlugin::deviceDialogInfo:End") ) );
    return true;
    
}

// Return device dialog types this plugin implements
QStringList HbDeviceInputDialogPlugin::deviceDialogTypes() const
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogPlugin::deviceDialogTypes:Begin") ) );
    QStringList types;
    const int numTypes = sizeof(dialogInfos) / sizeof(dialogInfos[0]);
    for(int i = 0; i < numTypes; i++) {
        types.append(dialogInfos[i].mTypeString);
    }
    TRACES( RDebug::Print( _L("HbDeviceInputDialogPlugin::deviceDialogTypes:End") ) );
    return types;
}

// Return plugin flags
HbDeviceDialogPlugin::PluginFlags HbDeviceInputDialogPlugin::pluginFlags() const
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogPlugin::pluginFlags:Begin") ) );
    TRACES( RDebug::Print( _L("HbDeviceInputDialogPlugin::pluginFlags:End") ) );
    return NoPluginFlags;
}

// Return last error
int HbDeviceInputDialogPlugin::error() const
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogPlugin::error:Begin") ) );
    TRACES( RDebug::Print( _L("HbDeviceInputDialogPlugin::error:End") ) );
    return d->mError;
}
