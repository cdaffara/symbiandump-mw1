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
* Description:  CCFScriptEvent class implementation.
*
*/



// INCLUDES
#include "cfscriptevent.h"
#include "cftrace.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCFScriptEvent::CCFScriptEvent
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CCFScriptEvent::CCFScriptEvent( const TInt& aScriptId ) 
    : iScriptId( aScriptId )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFScriptEvent::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCFScriptEvent::ConstructL( const TDesC& aIdentifier, 
    const RKeyValueArray& aParameters )
    {
    FUNC_LOG;

    iIdentifier = aIdentifier.AllocL();

    for ( TInt i = 0; i < aParameters.Count(); ++i )
        {
        iParameters.AppendL( aParameters[ i ] );
        }
    }

// -----------------------------------------------------------------------------
// CCFScriptEvent::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CCFScriptEvent* CCFScriptEvent::NewL( const TInt& aScriptId, 
    const RKeyValueArray& aParameters, 
    const TDesC& aIdentifier )
    {
    FUNC_LOG;

    CCFScriptEvent* event 
        = CCFScriptEvent::NewLC( aScriptId, aParameters, aIdentifier );
    CleanupStack::Pop( event );
    return event;
    }

// -----------------------------------------------------------------------------
// CCFScriptEvent::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFScriptEvent* CCFScriptEvent::NewLC( const TInt& aScriptId, 
    const RKeyValueArray& aParameters, 
    const TDesC& aIdentifier )
    {
    FUNC_LOG;

    CCFScriptEvent* event = new( ELeave ) CCFScriptEvent( aScriptId );
    CleanupStack::PushL( event );
    event->ConstructL( aIdentifier, aParameters );
    return event;
    }

// Destructor
CCFScriptEvent::~CCFScriptEvent()
    {
    FUNC_LOG;

    iParameters.Close(); // Content is not owned.
    delete iIdentifier;
    }

// -----------------------------------------------------------------------------
// CCFScriptEvent::ScriptId
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CCFScriptEvent::ScriptId() const
    {
    FUNC_LOG;

    return iScriptId;
    }

// -----------------------------------------------------------------------------
// CCFScriptEvent::Identifier
// -----------------------------------------------------------------------------
//
EXPORT_C TPtrC CCFScriptEvent::Identifier() const
    {
    FUNC_LOG;

    return TPtrC( *iIdentifier );
    }

// -----------------------------------------------------------------------------
// CCFScriptEvent::Parameters
// -----------------------------------------------------------------------------
//
EXPORT_C const RKeyValueArray& CCFScriptEvent::Parameters() const
    {
    FUNC_LOG;

    return iParameters;
    }
