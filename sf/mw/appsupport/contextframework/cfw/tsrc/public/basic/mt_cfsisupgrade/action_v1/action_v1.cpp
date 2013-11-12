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

#include "action_v1.h"
#include "actiondef_v1.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestActionPlugIn::CTestActionPlugIn
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CAction_V1::CAction_V1(): CCFActionPlugIn()
    {
    
    }

// -----------------------------------------------------------------------------
// CAction_V1::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAction_V1::ConstructL()
    {
    _LIT_SECURITY_POLICY_PASS( passSec );
    iSecurityPolicy = passSec;
    }

// -----------------------------------------------------------------------------
// CAction_V1::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAction_V1* CAction_V1::NewL()
    {
    CAction_V1* self = CAction_V1::NewLC();
    CleanupStack::Pop();
    return self;
    }
  
// -----------------------------------------------------------------------------
// CAction_V1::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAction_V1* CAction_V1::NewLC()
    {
    CAction_V1* self = new( ELeave ) CAction_V1();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
  
// Destructor
CAction_V1::~CAction_V1()
    {
    TInt err = RProperty::Set( KAction_V1PSUid, KAction_V1PSCategory,
        EAction_V1NotTriggered );
    RDebug::Print( _L("[Action_V1]: Set property: err = %d"), err );
    }

// METHODS

//-----------------------------------------------------------------------------
// CAction_V1::InitializeL
//-----------------------------------------------------------------------------
//
void CAction_V1::InitializeL()
    {
    RDebug::Print( _L("[Action_V1]: Action v1 initialized") );
    
    _LIT_SECURITY_POLICY_PASS( passSec );
    TInt err = RProperty::Define( KAction_V1PSUid, KAction_V1PSCategory,
        RProperty::EInt, passSec, passSec );
    RDebug::Print( _L("[Action_V1]: Define property: err = %d"), err );
    
    err = RProperty::Set( KAction_V1PSUid, KAction_V1PSCategory,
        EAction_V1NotTriggered );
    RDebug::Print( _L("[Action_V1]: Set property: err = %d"), err );
    }

//-----------------------------------------------------------------------------
// CAction_V1::PrepareExecutionL
//-----------------------------------------------------------------------------
//
void CAction_V1::PrepareExecutionL()
    {
    
    }

//-----------------------------------------------------------------------------
// CAction_V1::ExecuteL
// Triggers an action.
//-----------------------------------------------------------------------------
//
CCFActionPlugIn::TExecutionTime CAction_V1::ExecuteL(
    CCFActionIndication* /*aActionIndication */)
    {
    RDebug::Print( _L("[Action_V1]: Action v1 executed") );
    
    TInt err = RProperty::Set( KAction_V1PSUid, KAction_V1PSCategory,
        EAction_V1Triggered );
    RDebug::Print( _L("[Action_V1]: Set property: err = %d"), err );
    
    return CCFActionPlugIn::ENone;
    }

//-----------------------------------------------------------------------------
// CAction_V1::FinishedExecution
//-----------------------------------------------------------------------------
//
void CAction_V1::FinishedExecution()
    {
    
    }

//-----------------------------------------------------------------------------
// CAction_V1::GetActionsL
// Gets all the actions the plugin can perform.
//-----------------------------------------------------------------------------
//
void CAction_V1::GetActionsL( CDesCArray& aActionList ) const
    {
    RDebug::Print( _L("[Action_V1]: Action v1 get actions") );

    aActionList.AppendL( KAction_V1Id );
    }

//-----------------------------------------------------------------------------
// CAction_V1::SecurityPolicy
// Returns the capabilities required by the plugin.
//-----------------------------------------------------------------------------
//
const TSecurityPolicy& CAction_V1::SecurityPolicy() const
    {
    return iSecurityPolicy;
    }

// End of file
