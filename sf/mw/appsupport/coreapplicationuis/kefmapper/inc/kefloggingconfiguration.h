/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
*        Macro definition file for logging configuration.
*       (Note:This file has to be separate from the logging header file 
*             to be able to include it in mmp file. If not we will get warning about not used 
*             flogger.lib in other logging cases where flogger.lib is not used.)
*
*/



#ifndef KEFLOGGINGCONFIGURATION_H
#define KEFLOGGINGCONFIGURATION_H

#ifndef __WINS__
#ifdef _DEBUG
/**
* - Have this line active if you want to have logging for this component
* - Comment this line out if you don't want any logging for this component
*/
#define _KEF_LOGGING

/**
* - Have this line active if you want to have logging to file (defined in KefAnimLogger.h)
* - Comment this line out if you want to have logging via RDebug::Print 
*/
//#define _KEF_LOGGING_TO_FILE

#endif // _DEBUG
#endif // __WINS__

#endif // KEFLOGGINGCONFIGURATION_H

// End of File
