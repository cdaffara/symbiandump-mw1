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
#include "accindicator.h"

#include <QtPlugin>
#include <QProcess>
#include <accpolgenericiddefinitions.h>

Q_EXPORT_PLUGIN(AccIndicatorPlugin)
const static char IndicatorType[] = "com.nokia.accessory.indicatorplugin/1.0";
QString KAccMode = "AccMode";
QString KAccType = "AccType";

AccIndicatorPlugin::AccIndicatorPlugin() :
HbIndicatorInterface(IndicatorType,
        HbIndicatorInterface::SettingCategory,
        InteractionActivated)
    {
    mIndicatorTypes << "com.nokia.accessory.indicatorplugin/1.0";
    }

AccIndicatorPlugin::~AccIndicatorPlugin()
    {
    }

// ----------------------------------------------------------------------------
// AccIndicatorPlugin::indicatorTypes
// returns the indicator types handled by this plugin
// ----------------------------------------------------------------------------

QStringList AccIndicatorPlugin::indicatorTypes() const
    {
    return mIndicatorTypes;
    }

// ----------------------------------------------------------------------------
// AccIndicatorPlugin::createIndicator
// creates an indicator.
// ----------------------------------------------------------------------------

HbIndicatorInterface* AccIndicatorPlugin::createIndicator(
        const QString &indicatorType)
    {
    Q_UNUSED(indicatorType)
    return this;
    }

// ----------------------------------------------------------------------------
// AccIndicatorPlugin::error
// returns the error code.
// ----------------------------------------------------------------------------

int AccIndicatorPlugin::error() const
    {
    return mError;
    }

// ----------------------------------------------------------------------------
// AccIndicatorPlugin::accessAllowed
// check for the access rights of the client. As there are no restrictions for 
// this plugin it always returns true.
// ----------------------------------------------------------------------------

bool AccIndicatorPlugin::accessAllowed(const QString &indicatorType,const QVariantMap &securityInfo) const
    {
    Q_UNUSED(indicatorType)
    Q_UNUSED(securityInfo)

    return true;
    }

// ----------------------------------------------------------------------------
// AccIndicator::handleInteraction
// called when the user interacts with the indicator.Enable the interaction only
// for headset,tty and tv-out for user to change the settings.
// ----------------------------------------------------------------------------
bool AccIndicatorPlugin::handleInteraction(InteractionType type)
    {
    bool handled = false;
    if (type == InteractionActivated) 
        {
        // If it is 3-pole ( i.e., HeadSet or TTY ) and TV-Out enable the handleInteraction() to change the settings.
        if(mAccMode == EAccModeWiredHeadset || mAccMode == EAccModeWirelessHeadset || mAccMode == EAccModeTextDevice || mAccMode == EAccModeTVOut )
            {

            QVariant mode,type;
            mode.setValue((int)mAccMode); 
            type.setValue((int)mAccType);
            mArgs.append(mode.toString());
            mArgs.append(type.toString());
            
            // Launch the process to show the view.
            QString program("z://sys//bin//accindicatorsettings.exe");
		        QProcess::startDetached(program,mArgs);
            handled = true;
            }
        }
    return handled;
    }

// ----------------------------------------------------------------------------
// AccIndicator::indicatorData
// returns the name and icon to be displayed in the universal indicator menu.
// ----------------------------------------------------------------------------
QVariant AccIndicatorPlugin::indicatorData(int role) const
    {
    switch(role)
        {
        //for displaying the string in indicator.
        case PrimaryTextRole: 
            {
            QString type(mDisplayName);
            return type;
            }
        //for displaying the icon in indicator.
        case DecorationNameRole:
            {
            QString iconName;
            if(mAccType == KPCWired || mAccType == KPCUSB)
                {
                iconName = QString("z:/resource/accindicator/wired_accessory.svg");
                }
            else if (mAccType == KPCBluetooth || mAccType == KPCInfraRed)
                {
                iconName = QString("z:/resource/accindicator/wireless_accessory.svg");
                }
            return iconName;
            }
        default: 
            return QVariant();      
        }
    }

// ----------------------------------------------------------------------------
// AccIndicatorPlugin::handleClientRequest
// this function gets called when client activates plugin
// ----------------------------------------------------------------------------
bool AccIndicatorPlugin::handleClientRequest( RequestType type, 
        const QVariant &parameter)
    {    
    bool handled(false);
    switch (type) {
        case RequestActivate:
            {
            // Get the params(acctype and mode) from the hbindicator.activate() which is called from sysap.
            
            QVariantMap mapValues = parameter.toMap();
            if(mapValues.contains(KAccMode))
                {
                mAccMode = static_cast<TAccMode>(mapValues.value(KAccMode).toInt());
                }
            if(mapValues.contains(KAccType))
                {
                mAccType = mapValues.value(KAccType).toInt();
                }

            // prepare the name to be displayed in the universal indicator menu.
            prepareDisplayName();
            emit dataChanged();
            handled =  true;
            }
            break;
        case RequestDeactivate:
            {
            // reset data 
            mDisplayName = QString();
            emit deactivate();
            }
            break;
        default:
            break;
    }
    return handled;
    }

// ----------------------------------------------------------------------------
// AccIndicator::prepareDisplayName
// prepare the name to be displayed in the indicator menu.
// ----------------------------------------------------------------------------
void AccIndicatorPlugin::prepareDisplayName()
    {
    mDisplayName.clear();
    switch(mAccMode)
        {
        case EAccModeWiredHeadset:
            mDisplayName.append(QString("Wired Headset"));
            break;
        case EAccModeWirelessHeadset:
            mDisplayName.append(QString("Wireless Headset"));
            break;
        case EAccModeWiredCarKit:
            mDisplayName.append(QString("Wired CarKit"));
            break;
        case EAccModeWirelessCarKit:
            mDisplayName.append(QString("Wireless Carkit"));
            break;
        case EAccModeTextDevice:
            mDisplayName.append(QString("TTY"));
            break;
        case EAccModeLoopset:
            mDisplayName.append(QString("LoopSet"));
            break;
        case EAccModeMusicStand:
            mDisplayName.append(QString("Music Stand"));
            break;
        case EAccModeTVOut:
            mDisplayName.append(QString("TV Out"));
            break;
        case EAccModeHeadphones:
            mDisplayName.append(QString("Head Phones"));
            break;
        default :
            mDisplayName.append(QString("Unknown"));
        }
    }
