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
* Description:  CCFActionCmd implementation
*
*/


// SYSTEM INCLUDES

// USER INCLUDES
#include "cfactioncmd.h"
#include "cfactionhandler.h"
#include "cftrace.h"

#include <cfscriptevent.h>

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCFActionCmd::CCFActionCmd( MCFActionHandler& aActionHandler,
    CCFScriptEvent* aEvent )
    :   iActionHandler( aActionHandler ),
        iEvent( aEvent )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CCFActionCmd* CCFActionCmd::NewL( MCFActionHandler& aActionHandler,
    CCFScriptEvent* aEvent )
    {
    FUNC_LOG;

    CCFActionCmd* self = CCFActionCmd::NewLC( aActionHandler, aEvent );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CCFActionCmd* CCFActionCmd::NewLC( MCFActionHandler& aActionHandler,
    CCFScriptEvent* aEvent )
    {
    FUNC_LOG;

    CCFActionCmd* self = new( ELeave ) CCFActionCmd( aActionHandler, aEvent );
    CleanupStack::PushL( self );
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CCFActionCmd::~CCFActionCmd()
    {
    FUNC_LOG;

    delete iEvent;
    }


// ---------------------------------------------------------------------------
// CCFActionCmd::ExecuteL
// ---------------------------------------------------------------------------
//
void CCFActionCmd::ExecuteL()
    {
    FUNC_LOG;

    iActionHandler.FireActionL( iEvent );
    iEvent = NULL; // Ownership was transferred.
    }

// ---------------------------------------------------------------------------
// CCFActionCmd::LogError
// ---------------------------------------------------------------------------
//
void CCFActionCmd::LogError( TInt aError )
    {
    FUNC_LOG;

    if( aError != KErrNone )
        {
        ERROR( aError, "CCFActionCmd::LogError - MCFActionHandler::FireActionL() execution failed." );
        }
    }

// End of file
