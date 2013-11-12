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
* Description:  CCFContextSourceCmd implementation
*
*/


// SYSTEM INCLUDES
#include <cfcontextsourcecommand.h>

// USER INCLUDES
#include "cfcontextsourcecmd.h"
#include "cfcontextsourcecommandinterface.h"
#include "cftrace.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCFContextSourceCmd::CCFContextSourceCmd(
    MCFContextSourceCommand& aSourceCommandHandler )
    :   iContextSourceCommandHandler( aSourceCommandHandler )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CCFContextSourceCmd::ConstructL( const CCFContextSourceCommand& aCommand )
    {
    FUNC_LOG;

    iCommand = aCommand.CloneL(); // Make a copy of the command.
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CCFContextSourceCmd* CCFContextSourceCmd::NewL(
    MCFContextSourceCommand& aSourceCommandHandler,
    const CCFContextSourceCommand& aCommand )
    {
    FUNC_LOG;

    CCFContextSourceCmd* self = CCFContextSourceCmd::NewLC(
            aSourceCommandHandler,
            aCommand );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CCFContextSourceCmd* CCFContextSourceCmd::NewLC(
    MCFContextSourceCommand& aSourceCommandHandler,
    const CCFContextSourceCommand& aCommand )
    {
    FUNC_LOG;

    CCFContextSourceCmd* self
        = new( ELeave ) CCFContextSourceCmd( aSourceCommandHandler );
    CleanupStack::PushL( self );
    self->ConstructL( aCommand );
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CCFContextSourceCmd::~CCFContextSourceCmd()
    {
    FUNC_LOG;

    delete iCommand;
    }


// ---------------------------------------------------------------------------
// CCFContextSourceCmd::ExecuteL
// ---------------------------------------------------------------------------
//
void CCFContextSourceCmd::ExecuteL()
    {
    FUNC_LOG;

    iContextSourceCommandHandler.HandleCommandL( *iCommand );
    }

// ---------------------------------------------------------------------------
// CCFContextSourceCmd::LogError
// ---------------------------------------------------------------------------
//
void CCFContextSourceCmd::LogError( TInt aError )
    {
    FUNC_LOG;

    if( aError != KErrNone )
        {
        ERROR( aError, "CCFContextSourceCmd::LogError - MCFContextSourceCommand::HandleCommandL() execution failed." );
        }
    }

// End of file
