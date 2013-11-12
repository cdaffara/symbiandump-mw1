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
* Description:  
*
*/
#ifndef LOGLOGGER_H
#define LOGLOGGER_H

#include "logger.h"

#define LOGLOGGER_NAME QLatin1String("##loglogger##")

#ifdef _DEBUG
    #define LOGGER_LOG(str) Logger::instance(LOGLOGGER_NAME)->log(str);
#else
    #define LOGGER_LOG(str) 
#endif

#endif
