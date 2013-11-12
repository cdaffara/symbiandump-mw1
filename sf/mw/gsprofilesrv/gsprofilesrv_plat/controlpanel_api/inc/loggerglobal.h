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
* Description:  Common definitions for log.
*
*/
#ifndef LOGGERGLOBAL_H
#define LOGGERGLOBAL_H

#include <QtGlobal>

#ifdef BUILD_LOGGER_DLL
    #define LOGGER_EXPORT Q_DECL_EXPORT
#else
    #define LOGGER_EXPORT Q_DECL_IMPORT
#endif

#define LINE_SEPERATOR              QLatin1String("\r\n");

#define SETTINGS_SECTION_SEPERATOR  QLatin1String("/")

//following strings can be used in configuration file to specify logger's properties
//refer to logger.h
#define LOGGER_LOGDATETIME          QLatin1String("logdatetime")
#define LOGGER_LOGLOGGERNAME        QLatin1String("logloggername")
#define LOGGER_DATETIMEFORMAT       QLatin1String("datetimeformat")
#define LOGGER_OUTPUT               QLatin1String("output")

#define DEBUGOUTPUT_NAME            QLatin1String("debugoutput")
#define CONSOLEOUTPUT_NAME          QLatin1String("consoleoutput")
#define FILEOUTPUT_NAME             QLatin1String("fileoutput")
    #define FILEOUTPUT_LOGFILE      QLatin1String("logfile")
    #define FILEOUTPUT_TRUNCATE     QLatin1String("truncate")

#endif //LOGGERGLOBAL_H
