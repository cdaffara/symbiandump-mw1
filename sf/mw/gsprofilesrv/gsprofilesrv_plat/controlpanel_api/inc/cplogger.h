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
* Description:  Log utility for controlpanel framework.
*
*/
#ifndef CPLOGGER_H
#define CPLOGGER_H


/* 
    config file path :/logconf/controlpanellog.conf

    Format:

    [CpFramework]
    logdatetime = 1
    logloggername = 1
    datetimeformat = hh:mm:ss:zzz
    output = debugoutput fileoutput
    fileoutput/logfile = C:/data/logs/cpframework.log
    fileoutput/truncate = 1
    
    [CpPerformance]
    logdatetime = 1
    datetimeformat = hh:mm:ss:zzz
    output = fileoutput
    fileoutput/logfile = C:/data/logs/cpperformance.log
    fileoutput/truncate = 1
*/

#include <QLatin1String>
#include <logger.h>

/*
 define ENABLE_CPFW_LOG to  make CPFW_LOG work
*/

//#define ENABLE_CPFW_LOG

/*
 define ENABLE_CPPERF_LOG to make CPPERF_LOG work
*/

//#define ENABLE_CPPERF_LOG

#define CPFW_LOGGER_NAME       QLatin1String("CpFramework")
#define CPPERF_LOGGER_NAME     QLatin1String("CpPerformance")

#define CP_LOGGER_CONFIG_PATH QLatin1String(":/logconf/controlpanellog.conf")

#ifdef ENABLE_CPFW_LOG
    #define CPFW_LOG(str)             Logger::instance(CPFW_LOGGER_NAME)->log(str);
    #define CPSP_LOG_FUNC_ENTRY(func) LogFunctionEntryHelper ___cpfw_log_func_entry_helper(CPFW_LOGGER_NAME,func);
#else
    #define CPFW_LOG(str)
    #define CPSP_LOG_FUNC_ENTRY(func)
#endif

#ifdef ENABLE_CPPERF_LOG
    #define CPPERF_LOG(str) Logger::instance(CPPERF_LOGGER_NAME)->log(str);
#else
    #define CPPERF_LOG(str)
#endif

#endif /* CPLOGGER_H */
