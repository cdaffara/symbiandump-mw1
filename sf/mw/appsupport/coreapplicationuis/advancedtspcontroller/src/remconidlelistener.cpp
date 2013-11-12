/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Idle state listener
*
*/


// INCLUDE FILES
#include "remconidlelistener.h"
#include "RemConDebug.h"
#include "remcontspcontroller.h"
#include <startupdomainpskeys.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS
const TInt KRetryInterval = 2000000; // 2 seconds

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CRemConIdleListener::~CRemConIdleListener
// -----------------------------------------------------------------------------
//
CRemConIdleListener::~CRemConIdleListener()
    {
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConIdleListener::~CRemConIdleListener()" );

    Cancel();
    iProperty.Close();
    delete iPeriodic;
    if ( iDeleteInd ) // Update delete indicator
        {
        *iDeleteInd = ETrue;
        }

    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConIdleListener::~CRemConIdleListener() - return" );
    }

// -----------------------------------------------------------------------------
// CRemConIdleListener::CRemConIdleListener
// -----------------------------------------------------------------------------
//
CRemConIdleListener::CRemConIdleListener( CRemConTspController& aController )
    : CActive ( EPriorityNormal ),
      iController( aController )
    {
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConIdleListener::CRemConIdleListener()" );
    }

// -----------------------------------------------------------------------------
// CRemConIdleListener::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CRemConIdleListener::ConstructL()
    {
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConIdleListener::ConstructL()" );

    CActiveScheduler::Add( this );
	User::LeaveIfError( iProperty.Attach( KPSUidStartup,
	                    KPSIdlePhase1Ok ) );                  
    RunL(); // Start listening

    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConIdleListener::ConstructL() - return void" );
    }

// -----------------------------------------------------------------------------
// CRemConIdleListener::NewL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
CRemConIdleListener* CRemConIdleListener::NewL( CRemConTspController& aController )
    {
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConIdleListener::NewL()" );

    CRemConIdleListener * self = new ( ELeave ) CRemConIdleListener( aController );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConIdleListener::NewL() - return void" );
    return self;
    }

// -----------------------------------------------------------------------------
// CRemConIdleListener::RetryCB()
// 
// -----------------------------------------------------------------------------
//
TInt CRemConIdleListener::RetryCB( TAny* aPtr )
    {   
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConIdleListener::RetryCB()" );

    static_cast<CRemConIdleListener*>( aPtr )->CreateController();

    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConIdleListener::RetryCB() - return" );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CRemConIdleListener::CreateController()
// 
// -----------------------------------------------------------------------------
//
void CRemConIdleListener::CreateController()
    {   
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConIdleListener::CreateController()" );

    // Setup delete indicator to detect listener deletion from controller and try to create
    TBool deleteInd( EFalse );
    iDeleteInd = &deleteInd;
    TRAPD( err, iController.CreateAfterIdleL() );
    COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConIdleListener::CreateController() - Create err=%d", err );

    if ( !deleteInd )
        {
        iDeleteInd = NULL; // Reset delete indicator
        if ( err == KErrNone )
            {
            // Stop retry callback on success
            delete iPeriodic;
            iPeriodic = NULL;
            }
        else if ( !iPeriodic )
            {
            // Start retry callback on error
            iPeriodic = CPeriodic::New( CActive::EPriorityStandard );
            if( iPeriodic )
                {
                iPeriodic->Start( KRetryInterval, KRetryInterval, TCallBack( RetryCB, this ) );
                }
            else
                {
                COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConIdleListener::CreateController() - Retry out of memory" );
                }
            }
        if ( iPeriodic )
            {
            COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConIdleListener::CreateController() - Retry after %d us",
                KRetryInterval );
            }
        }

    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConIdleListener::CreateController() - return" );
    }

// -----------------------------------------------------------------------------
// CRemConIdleListener::DoCancel()
// 
// -----------------------------------------------------------------------------
//
void CRemConIdleListener::DoCancel()
    {   
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConIdleListener::DoCancel()" );

    iProperty.Cancel();

    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConIdleListener::DoCancel() - return void" );
    }

// -----------------------------------------------------------------------------
// CRemConIdleListener::RunL()
// 
// -----------------------------------------------------------------------------
//
void CRemConIdleListener::RunL()
    {
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConIdleListener::RunL()" );

    iProperty.Subscribe( iStatus );
    TInt value( 0 );
    TInt err( iProperty.Get( value ) );
    COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConIdleListener::RunL() - Value err", err );
    SetActive();

    if ( err == KErrNone && value == EIdlePhase1Ok )
        {
        // Cancel listening and create controller
        COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConIdleListener::RunL() - Idle detected!" );
        Cancel();
        CreateController();
        }
    else
        {
        // Listening continues 
        COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConIdleListener::RunL() - No idle detected, subscribe and wait idle key" );
        }

    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConIdleListener::RunL() - return void" );    
    }

// -----------------------------------------------------------------------------
// CRemConIdleListener::RunError
// Handle error in RunL
// -----------------------------------------------------------------------------
//
#ifdef _DEBUG
TInt CRemConIdleListener::RunError(TInt aError)
#else
TInt CRemConIdleListener::RunError(TInt /*aError*/)
#endif
    {
    COM_TRACE_1( "[REMCONTSPCONTROLLER] CRemConIdleListener::RunError(%d)", aError );        
    // Error occurred while trying to communicate with context framework.    
    // Return KErrNone to avoid panic.
    return KErrNone;
    }   
// End of file
