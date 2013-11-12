/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFCmd class implementation.
*
*/


// SYSTEM INCLUDES

// USER INCLUDES
#include "cfcmd.h"
#include "cftrace.h"

#include <cfcontextsubscriptionlistener.h>

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCFCmd::CCFCmd()
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CCFCmd::~CCFCmd( )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CCFCmd::LogError
// ---------------------------------------------------------------------------
//
void CCFCmd::LogError( TInt aError )
    {
    FUNC_LOG;

    if( aError != KErrNone )
        {
        ERROR( aError, "CCFCmd::LogError - Command execution failed." );
        }
    }

// ---------------------------------------------------------------------------
// CCFCmd::SglQueLinkOffset
// ---------------------------------------------------------------------------
//
TInt CCFCmd::SglQueLinkOffset()
    {
    FUNC_LOG;

    return _FOFF( CCFCmd, iSlink );
    }

// End of file
