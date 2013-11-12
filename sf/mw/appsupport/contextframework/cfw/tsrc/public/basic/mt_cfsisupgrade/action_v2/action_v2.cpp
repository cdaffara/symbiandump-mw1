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
* Description:  ?Description
*
*/



// INCLUDE FILES
#include <e32property.h>
#include <e32debug.h>

#include "action_v2.h"
#include "actiondef_v2.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAction_V2::CAction_V2
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CAction_V2::CAction_V2(): CCFActionPlugIn()
    {
    
    }

// -----------------------------------------------------------------------------
// CAction_V2::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAction_V2::ConstructL()
    {
    _LIT_SECURITY_POLICY_PASS( passSec );
    iSecurityPolicy = passSec;
    }

// -----------------------------------------------------------------------------
// CAction_V2::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAction_V2* CAction_V2::NewL()
    {
    CAction_V2* self = CAction_V2::NewLC();
    CleanupStack::Pop();
    return self;
    }
  
// -----------------------------------------------------------------------------
// CAction_V2::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAction_V2* CAction_V2::NewLC()
    {
    CAction_V2* self = new( ELeave ) CAction_V2();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
  
// Destructor
CAction_V2::~CAction_V2()
    {
    TInt err = RProperty::Set( KAction_V2PSUid, KAction_V2PSCategory,
        EAction_V2NotTriggered );
    RDebug::Print( _L("[Action_V2]: Set property: err = %d"), err );
    }

// METHODS

//-----------------------------------------------------------------------------
// CAction_V2::InitializeL
//-----------------------------------------------------------------------------
//
void CAction_V2::InitializeL()
    {
    RDebug::Print( _L("[Action_V2]: Action V2 initialized") );
    
    _LIT_SECURITY_POLICY_PASS( passSec );
    TInt err = RProperty::Define( KAction_V2PSUid, KAction_V2PSCategory,
        RProperty::EInt, passSec, passSec );
    RDebug::Print( _L("[Action_V2]: Define property: err = %d"), err );
    
    err = RProperty::Set( KAction_V2PSUid, KAction_V2PSCategory,
        EAction_V2NotTriggered );
    RDebug::Print( _L("[Action_V2]: Set property: err = %d"), err );
    }

//-----------------------------------------------------------------------------
// CAction_V2::PrepareExecutionL
//-----------------------------------------------------------------------------
//
void CAction_V2::PrepareExecutionL()
    {
    
    }

//-----------------------------------------------------------------------------
// CAction_V2::ExecuteL
// Triggers an action.
//-----------------------------------------------------------------------------
//
CCFActionPlugIn::TExecutionTime CAction_V2::ExecuteL(
    CCFActionIndication* /*aActionIndication */)
    {
    RDebug::Print( _L("[Action_V2]: Action V2 executed") );
    
    TInt err = RProperty::Set( KAction_V2PSUid, KAction_V2PSCategory,
        EAction_V2Triggered );
    RDebug::Print( _L("[Action_V2]: Set property: err = %d"), err );
    
    return CCFActionPlugIn::ENone;
    }

//-----------------------------------------------------------------------------
// CAction_V2::FinishedExecution
//-----------------------------------------------------------------------------
//
void CAction_V2::FinishedExecution()
    {
    
    }

//-----------------------------------------------------------------------------
// CAction_V2::GetActionsL
// Gets all the actions the plugin can perform.
//-----------------------------------------------------------------------------
//
void CAction_V2::GetActionsL( CDesCArray& aActionList ) const
    {
    RDebug::Print( _L("[Action_V2]: Action V2 get actions") );

    aActionList.AppendL( KAction_V2Id );
    }

//-----------------------------------------------------------------------------
// CAction_V2::SecurityPolicy
// Returns the capabilities required by the plugin.
//-----------------------------------------------------------------------------
//
const TSecurityPolicy& CAction_V2::SecurityPolicy() const
    {
    return iSecurityPolicy;
    }

// End of file
