/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  NspsWsPlugin type definitions
*
*/


#ifndef NSPSWSPLUGINDLLSTD_H
#define NSPSWSPLUGINDLLSTD_H

// DATA TYPES

/** Type of animation */
enum TNspsWsPluginType
    {
    EForwardRawKeyeventsForNsps
    };

/** Operation */
enum TNspsWsPluginOpcode
    {
    EActivateNspsKeyeventForwarding,
    EDeActivateNspsKeyeventForwarding,
    EActivateKeyeventForwardingForNcn,
    EDeActivateKeyeventForwardingForNcn,
    EActivateKeyeventForwardingForLights,
    EDeActivateKeyeventForwardingForLights
    };


#endif
// End Of File
