/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Call state source context definition.
*
*/



#ifndef CALLSTATESOURCECONTEXTDEF_H
#define CALLSTATESOURCECONTEXTDEF_H

#include <cfdefinitions.h>

// Context source: 'Call'
_LIT( KCallStateSource, "Call" );

//-----------------------------------------------------------------------------
// Call: State
//
// Listens P&S key related to call states.
//-----------------------------------------------------------------------------
//
// Type definition
_LIT( KCallStateType, "State" );

// Value definitions
const TInt KCallStateTypeValueCount = 9;
_LIT_CONTEXT_VALUES( KCallStateTypeValues ) =
    {
    _S( "Uninitialized" ),
    _S( "None" ),
    _S( "Alerting" ),
    _S( "Ringing" ),
    _S( "Dialling" ),
    _S( "Answering" ),
    _S( "Disconnecting" ),
    _S( "Connected" ),
    _S( "Hold" ),
    };
    
// Security definition
_LIT_SECURITY_POLICY_C1( KCallStateTypeSec, ECapabilityLocalServices );

#endif
