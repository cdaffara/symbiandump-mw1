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
* Description:  Common path definitions for controlpanel
*
*/

#ifndef CPBASEPATH_H
#define CPBASEPATH_H

#include <QLatin1String>

/*
 CP_PLUGIN_PATH : path to place controlpanel plugin stub files (.qtplugin files)
 CP_PLUGIN_CONFIG_PATH: path to place controlpanel plugin configuration files (.cpcfg files)
*/
#ifdef Q_OS_SYMBIAN
        #define CP_PLUGIN_PATH QLatin1String("/resource/qt/plugins/controlpanel")
        #define CP_PLUGIN_CONFIG_PATH QLatin1String("/resource/qt/plugins/controlpanel/config")
#else 
    #ifdef _DEBUG
        #define CP_PLUGIN_PATH QLatin1String("/ControlPanel/debug/bin")
        #define CP_PLUGIN_CONFIG_PATH QLatin1String("/ControlPanel/debug/bin/config")
    #else
        #define CP_PLUGIN_PATH QLatin1String("/ControlPanel/release/bin")
        #define CP_PLUGIN_CONFIG_PATH QLatin1String("/ControlPanel/release/bin/config")
    #endif
#endif 


#endif  //CPBASEPATH_H

//End of File

