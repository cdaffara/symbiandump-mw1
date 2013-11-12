/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Sensor source context definition.
*
*/



#ifndef CFWCONTEXTDEF_H
#define CFWCONTEXTDEF_H

#include <cfdefinitions.h>

//-----------------------------------------------------------------------------
// Source:  'ContextFramework'
//-----------------------------------------------------------------------------
//
_LIT( KCfwSource, "ContextFramework" );

//-----------------------------------------------------------------------------
// Source:  'ContextFramework'
// Type:    'Status'
//
// Describes Context Framework current status.
//-----------------------------------------------------------------------------
//
_LIT( KCfwSourceStatus, "Status" );

// Context values
_LIT_CONTEXT_VALUES( KCfwSourceStatusValues ) =
    {
    // Context Framework initializing
    _S( "Initializing" ),
    
    // Context Framework ready
    _S( "Ready" )
    };
    
// Enumerations for accessing values array
enum TSensorSourceEventValues
    {
    EStatusInitializing,
    EStatusReady,
    EStatusCount
    };
    
// Capabilities
_LIT_SECURITY_POLICY_PASS( KCfwSourceStatusSec );

#endif
