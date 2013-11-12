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
#include "psmindicator.h"

#include <QWidget>
#include <QtPlugin>
#include <QProcess>
#include <cppluginlauncher.h>
#include <hbview.h>
#include <hbapplication.h>
#include <hbmainwindow.h>
#include <hbaction.h>
#include <cpbasesettingview.h>
#include <e32debug.h>


Q_EXPORT_PLUGIN(PsmIndicatorPlugin)
const static char IndicatorType[] = "com.nokia.hb.powersavemodeplugin/1.0";
QString KPsm = "PSM";
QString KCharging = "Charging";

PsmIndicatorPlugin::PsmIndicatorPlugin():HbIndicatorInterface(IndicatorType,HbIndicatorInterface::NotificationCategory,InteractionActivated)
    {
    RDebug::Print( _L("PsmIndicatorPlugin constructor begin  ") );  
    iIndicatorTypes << "com.nokia.hb.powersavemodeplugin/1.0";
    RDebug::Print( _L("PsmIndicatorPlugin constructor end  ")  ); 
    }

PsmIndicatorPlugin::~PsmIndicatorPlugin()
    {
   RDebug::Print( _L("PsmIndicatorPlugin destructor begin  ")  ); 
    }

// ----------------------------------------------------------------------------
// psmIndicatorPlugin::indicatorTypes
// returns the indicator types handled by this plugin
// ----------------------------------------------------------------------------

QStringList PsmIndicatorPlugin::indicatorTypes() const
    {
    RDebug::Print( _L("PsmIndicatorPlugin indicatortypes  ")); 
    return iIndicatorTypes;
    }

// ----------------------------------------------------------------------------
// psmIndicatorPlugin::createIndicator
// creates an indicator.
// ----------------------------------------------------------------------------

HbIndicatorInterface* PsmIndicatorPlugin::createIndicator(
        const QString &indicatorType)
    {
    Q_UNUSED(indicatorType)
    RDebug::Print( _L("PsmIndicatorPlugin createindicator  ") ); 
    return this;
    }

// ----------------------------------------------------------------------------
// psmIndicatorPlugin::error
// returns the error code.
// ----------------------------------------------------------------------------

int PsmIndicatorPlugin::error() const
    {
    RDebug::Print( _L("PsmIndicatorPlugin error  ") ); 
    return mError;
    }

// ----------------------------------------------------------------------------
// psmIndicatorPlugin::accessAllowed
// check for the access rights of the client. As there are no restrictions for 
// this plugin it always returns true.
// ----------------------------------------------------------------------------

bool PsmIndicatorPlugin::accessAllowed(const QString &indicatorType,
        const QVariantMap &     securityInfo) const
    {
    Q_UNUSED(indicatorType)
    Q_UNUSED(securityInfo)
    
    RDebug::Print( _L("PsmIndicatorPlugin accessallowed  ") ); 
    return true;
    }

// ----------------------------------------------------------------------------
// psmIndicator::handleInteraction
// called when the user interacts with the indicator.Enable the interaction 
// for psm enabled and charger connected
// ----------------------------------------------------------------------------
bool PsmIndicatorPlugin::handleInteraction(InteractionType type)
    {
    bool handled = false;
    RDebug::Print( _L("PsmIndicatorPlugin handleinteraction begin  ")); 
    if (type == InteractionActivated) 
        {
         RDebug::Print( _L("PsmIndicatorPlugin handleinteraction inside interactionactivated  ") );	
         QObject::connect( &iProcess, SIGNAL(error(QProcess::ProcessError)),                       
                                          this, SLOT(processError(QProcess::ProcessError)));
                                          
         // Launch the process to show the view.
         iProcess.start("CpPsmPluginLauncher");
         handled = true;
                        
        }   
    RDebug::Print( _L("PsmIndicatorPlugin handleinteraction end  ") );         
    return handled;
    }

// ----------------------------------------------------------------------------
// psmIndicator::indicatorData
// returns the name and icon to be displayed in the universal indicator menu.
// ----------------------------------------------------------------------------
QVariant PsmIndicatorPlugin::indicatorData(int role) const
    {
    RDebug::Print( _L("PsmIndicatorPlugin indicatordata begin  ") );
    switch(role)
        {
        //for displaying the string in indicator.
        case PrimaryTextRole: 
            {
             return iDisplayName;
            }
        //for displaying the icon in indicator.
        case DecorationNameRole:
            {
             return iIcon;
            }
        default: 
            return QVariant();      
        }
    }

// ----------------------------------------------------------------------------
// psmIndicatorPlugin::handleClientRequest
// this function gets called when client activates plugin
// ----------------------------------------------------------------------------
bool PsmIndicatorPlugin::handleClientRequest( RequestType type, 
        const QVariant &parameter)
    {    
    RDebug::Print( _L("PsmIndicatorPlugin handleclientrequest begin  ") );
    bool handled(false);
    switch (type) 
        {
        case RequestActivate:
            {
            RDebug::Print( _L("PsmIndicatorPlugin handleclientrequest requestactivate  ") );
            // Get the param from the hbindicator.activate() which is called from sysap.
            HbTranslator trans("powermanagement");
            iDisplayName.clear();
            //if it's a PSM indicator then prepare the string and icon accordingly
            if((parameter.operator ==(QString("PSM"))))
               {
            RDebug::Print( _L("PsmIndicatorPlugin handleclientrequest requestactivate PSM  "));   	
            iDisplayName.append(hbTrId("txt_power_management_dpopinfo_psm_activated_automa"));
            iIcon.append(QString("qtg_large_psm"));
               }
            //else it's a charging indicator then prepare the string and icon accordingly
            else
              {
            RDebug::Print( _L("PsmIndicatorPlugin handleclientrequest requestactivate charging  ") );   	  	
            iDisplayName.append(hbTrId("txt_power_management_dblist_charging"));
            iIcon.append(QString("qtg_mono_flash_charging"));
               }

            
            emit dataChanged();
            handled =  true;
            }
            break;
        case RequestDeactivate:
            {
            RDebug::Print( _L("PsmIndicatorPlugin handleclientrequest requestdeactivate  ") );   	
            // reset data 
            iDisplayName = QString();
            iIcon=QString();
            
            emit deactivate();
            }
            break;
        default:
            break;
    }
    RDebug::Print( _L("PsmIndicatorPlugin handleclientrequest end  ") );
    return handled;
    }


// ----------------------------------------------------------------------------
// psmIndicator::processError
// handle the error conditions returned by the QProcess.
// ----------------------------------------------------------------------------

void PsmIndicatorPlugin::processError(QProcess::ProcessError err)
    {
    RDebug::Print( _L("PsmIndicatorPlugin processerror begin  "));   		
    switch (err)
        {   
        case QProcess::FailedToStart: 
        case QProcess::Crashed: 
        case QProcess::Timedout: 
        case QProcess::ReadError: 
        case QProcess::WriteError: 
        case QProcess::UnknownError:
             break;  
        default:
            break;
        }
    }


