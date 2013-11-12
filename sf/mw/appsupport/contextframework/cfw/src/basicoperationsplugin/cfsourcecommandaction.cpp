/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFSourceCommandAction class implementation.
 *
*/


// SYSTEM INCLUDES
#include <cfoperationservices.h>
#include <cfcontextsourcecommand.h>

// USER INCLUDES
#include "cfsourcecommandaction.h"
#include "cfbasicoptrace.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCFSourceCommandAction::CCFSourceCommandAction(
    MCFOperationServices& aServices )
    :   CCFScriptAction( aServices, CCFScriptAction::ESourceCommand )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CCFSourceCommandAction::ConstructL( CCFContextSourceCommand* aCommand )
    {
    FUNC_LOG;

    // Take ownership as the last step of construction to ensure that leaving
    // construction does not take the ownership.
    iCommand = aCommand;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CCFSourceCommandAction* CCFSourceCommandAction::NewL(
    MCFOperationServices& aServices,
    CCFContextSourceCommand* aCommand )
    {
    FUNC_LOG;

    CCFSourceCommandAction* self = CCFSourceCommandAction::NewLC( aServices,
            aCommand );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CCFSourceCommandAction* CCFSourceCommandAction::NewLC(
    MCFOperationServices& aServices,
    CCFContextSourceCommand* aCommand )
    {
    FUNC_LOG;

    CCFSourceCommandAction* self
            = new( ELeave ) CCFSourceCommandAction( aServices );
    CleanupStack::PushL( self );
    self->ConstructL( aCommand );
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CCFSourceCommandAction::~CCFSourceCommandAction()
    {
    FUNC_LOG;

    delete iCommand;
    }


// -----------------------------------------------------------------------------
// CCFSourceCommandAction::ActL
// -----------------------------------------------------------------------------
//
void CCFSourceCommandAction::ActL()
    {
    FUNC_LOG;

    if ( iCommand )
        {
        TPtrC commandName( iCommand->Name() );
        ACTION_INFO_3( "CCFSourceCommandAction::ActL - Script ID=[%d] Firing action: %S for source [%x]",
                iServices.ScriptId(),
                &commandName,
                iCommand->SourceUid() );

        iServices.FireActionL( *iCommand );
        }
    else
        {
        ERROR_GEN_1( "CCFSourceCommandAction::ActL - Command is NULL for script ID=[%d]",
                iServices.ScriptId() );
        }
    }

// -----------------------------------------------------------------------------
// CCFSourceCommandAction::GetSecurityPolicy
// -----------------------------------------------------------------------------
//
TInt CCFSourceCommandAction::GetSecurityPolicy( TSecurityPolicy& aPolicy )
    {
    FUNC_LOG;

    TSecurityPolicy policy( TSecurityPolicy::EAlwaysPass );
    aPolicy = policy;
    return KErrNone; // Actual check not needed at the moment.
    }

// End of file
