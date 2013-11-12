/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Macro definition file for logging configuration.
*                 Note: This file has to be separate from the logging header
*                       file to be able to include it in mmp file. If not, we
*                       will get warning about not used flogger.lib in other
                        logging cases where flogger.lib is not used.
*
*/



#ifndef GSCONFIGURATION_H
#define GSCONFIGURATION_H

#ifdef _DEBUG // logging to file in debug only

/**
* Have this line active if you want to have logging for this component.
* Comment this line out if you don't want any logging for this component.
*/
//#define _GSLOGGING

/**
* Have this line active if you want to have logging to file.
* Comment this line out if you want to have logging via RDebug::Print.
*/
//#define _GS_LOGGING_TO_FILE

/**
* Have this line active in case you want to have plugin loading performance
* measurements activated.
*/
//#define _GS_PERFORMANCE_TRACES

/**
* Have this line active in case you want to have verbose messages for plugin
* loading. This is helpful if loading of plugin(s) is failing.
*/
//#define _GS_PLUGINLOADER_VERBOSE_TRACES

/**
* Enable to get traces about plugin sorting. Usable to get each sorted view
* items.
*/
//#define _GS_PLUGINLOADER_FINAL_SORTING_TRACES

/**
* Enable to get traces about plugin sorting for each iteration. Usable if
* sorting logic fails.
*/
//#define _GS_PLUGINLOADER_ITERATION_SORTING_TRACES

/**
* Enable to active verbose messages for CGSWatchDog. This is helpful if plugins
* are missing or disabled from GS incorrectly.
*/
//#define _GS_WATCHDOG_VERBOSE_TRACES

/**
* Activate to get traces of lbx format strings. Good for finding how and which
* icons and texts are used for lbx items.
*/
//#define _GS_PARENTPLUGIN_LBX_FORMAT_TRACES



#endif // _DEBUG

#endif // GSCONFIGURATION_H
