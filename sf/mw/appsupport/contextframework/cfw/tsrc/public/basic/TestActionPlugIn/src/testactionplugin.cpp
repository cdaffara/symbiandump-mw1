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
#include "TestActionPlugIn.h"
#include "CFActionIndication.h"

#include <badesca.h>
#include <e32property.h>

// LOCAL CONSTANTS AND MACROS
static const TInt KTimerValue           = 200000; // 2sec

/**
* Supported action identifiers.
*/
_LIT( KTestActionIdTestAc3, "AC3" );
_LIT( KTestActionIdLeave, "AC_Leave" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestActionPlugIn::CTestActionPlugIn
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CTestActionPlugIn::CTestActionPlugIn() : 
    CCFActionPlugIn()
    {
    }

// -----------------------------------------------------------------------------
// CTestActionPlugIn::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CTestActionPlugIn::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CTestActionPlugIn::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTestActionPlugIn* CTestActionPlugIn::NewL()
    {
    CTestActionPlugIn* self = CTestActionPlugIn::NewLC();
    CleanupStack::Pop();
    return self;
    }
  
// -----------------------------------------------------------------------------
// CTestActionPlugIn::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTestActionPlugIn* CTestActionPlugIn::NewLC()
    {
    CTestActionPlugIn* self = new( ELeave ) CTestActionPlugIn();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
  
// Destructor
CTestActionPlugIn::~CTestActionPlugIn()
    {
    delete iPeriodic;
    }

// METHODS

//-----------------------------------------------------------------------------
// CTestActionPlugIn::InitializeL
//-----------------------------------------------------------------------------
//
void CTestActionPlugIn::InitializeL()
    {
    TInt check( 0 );
    TInt err = RProperty::Get( KActionPluginTestCategory, KLeaveInitializationFunction, check );
    if( err != KErrNone )
        {
        check = 0;
        }
    if( check )
        {
        User::Leave( KErrAbort );
        }
    else
        {
        iPeriodic = CPeriodic::NewL( CActive::EPriorityStandard );
        }
    }

//-----------------------------------------------------------------------------
// CTestActionPlugIn::ExecuteL
// Triggers an action.
//-----------------------------------------------------------------------------
//
CCFActionPlugIn::TExecutionTime CTestActionPlugIn::ExecuteL(
    CCFActionIndication* aActionIndication )
    {
    TPtrC actionId( aActionIndication->Identifier() );

    // Check command type and handle the command if it is supported.
    if( actionId == KTestActionIdTestAc3 )
        {
        StartTimer();
        return CCFActionPlugIn::ESmall;
        }
    else if( actionId == KTestActionIdLeave )
        {
        User::Leave( KErrArgument );
        }
        
    return CCFActionPlugIn::ENone;
    }

//-----------------------------------------------------------------------------
// CTestActionPlugIn::GetActionsL
// Gets all the actions the plugin can perform.
//-----------------------------------------------------------------------------
//
void CTestActionPlugIn::GetActionsL( CDesCArray& aActionList ) const
    {
    aActionList.AppendL( KTestActionIdTestAc3 );
    aActionList.AppendL( KTestActionIdLeave );
    }

//-----------------------------------------------------------------------------
// CTestActionPlugIn::SecurityPolicy
// Returns the capabilities required by the plugin.
//-----------------------------------------------------------------------------
//
const TSecurityPolicy& CTestActionPlugIn::SecurityPolicy() const
    {
    return iSecurityPolicy;
    }

// -----------------------------------------------------------------------------
// CTestActionPlugIn::StartTimer
// -----------------------------------------------------------------------------
//
void CTestActionPlugIn::StartTimer()
    {
    TCallBack cb( CallBack, this );
    iPeriodic->Start( KTimerValue, KTimerValue, cb );
    }

// -----------------------------------------------------------------------------
// CTestActionPlugIn::CallBack
// -----------------------------------------------------------------------------
//
TInt CTestActionPlugIn::CallBack( TAny* aSelf )
    {
    CTestActionPlugIn* self = static_cast< CTestActionPlugIn* >( aSelf );
    self->iPeriodic->Cancel();
    self->AsyncExecutionCompleted();

    return KErrNone;
    }
