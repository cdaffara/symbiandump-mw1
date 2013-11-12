/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFNotifyAction class implementation.
*
*/



// INCLUDES
#include "cfnotifyaction.h"
#include "cfscriptevent.h"
#include "cfbasicoptrace.h"

#include <cfoperationservices.h>

// MEMBER FUNCTIONS

// -----------------------------------------------------------------------------
// CCFNotifyAction::CCFNotifyAction
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CCFNotifyAction::CCFNotifyAction( MCFOperationServices& aServices ) 
    :   CCFScriptAction( aServices, CCFScriptAction::ENotify )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFNotifyAction::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCFNotifyAction::ConstructL( const TDesC& aActionId, 
    const RPointerArray< CCFKeyValuePair >& aParameters )
    {
    FUNC_LOG;

    iIdentifier = aActionId.AllocL();
    for ( TInt i = 0; i < aParameters.Count(); ++i )
        {
        iParameters.AppendL( CCFKeyValuePair::NewL( aParameters[ i ]->Key(), 
            aParameters[ i ]->Value() ) );
        }
    }

// -----------------------------------------------------------------------------
// CCFNotifyAction::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFNotifyAction* CCFNotifyAction::NewL( MCFOperationServices& aServices, 
    const TDesC& aActionId, 
    const RPointerArray< CCFKeyValuePair >& aParameters )
    {
    FUNC_LOG;

    CCFNotifyAction* self = CCFNotifyAction::NewLC( aServices, 
            aActionId,
            aParameters );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCFNotifyAction::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFNotifyAction* CCFNotifyAction::NewLC( MCFOperationServices& aServices,
    const TDesC& aActionId, 
    const RPointerArray< CCFKeyValuePair >& aParameters )
    {
    FUNC_LOG;

    CCFNotifyAction* self = new( ELeave ) CCFNotifyAction( aServices );
    CleanupStack::PushL( self );
    self->ConstructL( aActionId, aParameters );
    return self;
    }

// Destructor
CCFNotifyAction::~CCFNotifyAction()
    {
    FUNC_LOG;

    iParameters.ResetAndDestroy();
    delete iIdentifier;
    }


// -----------------------------------------------------------------------------
// CCFNotifyAction::ActL
// -----------------------------------------------------------------------------
//
void CCFNotifyAction::ActL()
    {
    FUNC_LOG;

    CCFScriptEvent* newEvent = CCFScriptEvent::NewL( iServices.ScriptId(),
            iParameters,
            *iIdentifier );
    CleanupStack::PushL( newEvent );                // CLEANUP<< newEvent
    iServices.FireActionL( newEvent );
    CleanupStack::Pop( newEvent );                  // CLEANUP>> newEvent

    ACTION_INFO_2( "CCFNotifyAction::ActL - Script ID=[%d] Fired action: %S",
            iServices.ScriptId(),
            &( *iIdentifier ) );
    }

// -----------------------------------------------------------------------------
// CCFNotifyAction::GetSecurityPolicy
// -----------------------------------------------------------------------------
//
TInt CCFNotifyAction::GetSecurityPolicy( TSecurityPolicy& aPolicy )
	{
    FUNC_LOG;

	return iServices.GetActionSecurityPolicy( *iIdentifier, aPolicy );
	}
