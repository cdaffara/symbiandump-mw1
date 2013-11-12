/** @file
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies  this distribution, and is available 
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Upnp Media Server event
*
*/


// INCLUDE FILES
#include "upnpmediaserverevent.h"

// -----------------------------------------------------------------------------
// CUpnpMediaServerEvent::CUpnpMediaServerEvent
// Constructor
// -----------------------------------------------------------------------------
//
CUpnpMediaServerEvent::CUpnpMediaServerEvent(TInt aType) : iEventType( aType )
    {    
    }

// -----------------------------------------------------------------------------
// CUpnpMediaServerEvent::~CUpnpMediaServerEvent
// Connect destructor
// -----------------------------------------------------------------------------
//
CUpnpMediaServerEvent::~CUpnpMediaServerEvent()
    {
    }

// -----------------------------------------------------------------------------
// CUpnpMediaServerEvent::GetEventType
// Type getter
// -----------------------------------------------------------------------------
//
TInt CUpnpMediaServerEvent::GetEventType()
    {
    return iEventType;
    }
