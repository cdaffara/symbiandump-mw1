/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implements initializer
*
*/


// INCLUDE FILES
#include "sensordatacompensatorinitializer.h"
#include "sensordatacompensatorplg.h"
#include "trace.h"

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
// CSensorDataCompensatorInitializer::~CSensorDataCompensatorInitializer
// -----------------------------------------------------------------------------
//
CSensorDataCompensatorInitializer::~CSensorDataCompensatorInitializer()
    {
    FUNC_LOG;

    Cancel();
    iProperty.Close();
    delete iPeriodic;
    if ( iDeleteInd ) // Update delete indicator
        {
        *iDeleteInd = ETrue;
        }
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorInitializer::CSensorDataCompensatorInitializer
// -----------------------------------------------------------------------------
//
CSensorDataCompensatorInitializer::CSensorDataCompensatorInitializer( CSensorDataCompensatorPlg& aPlg )
    : CActive ( EPriorityNormal ),
      iPlg( aPlg )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorInitializer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSensorDataCompensatorInitializer::ConstructL()
    {
    FUNC_LOG;

    CActiveScheduler::Add( this );
	User::LeaveIfError( iProperty.Attach( KPSUidStartup,
	                    KPSIdlePhase1Ok ) );
    RunL(); // Start listening
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorInitializer::NewL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
CSensorDataCompensatorInitializer* CSensorDataCompensatorInitializer::NewL( CSensorDataCompensatorPlg& aPlg )
    {
    FUNC_LOG;

    CSensorDataCompensatorInitializer * self =
                    new ( ELeave ) CSensorDataCompensatorInitializer(aPlg);

    CleanupStack::PushL( self );
    self->ConstructL();
	CleanupStack::Pop( self );
	return self;
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorInitializer::RetryCB()
//
// -----------------------------------------------------------------------------
//
TInt CSensorDataCompensatorInitializer::RetryCB( TAny* aPtr )
    {
    FUNC_LOG;

    static_cast<CSensorDataCompensatorInitializer*>( aPtr )->InitCfw();
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorInitializer::InitCfw()
//
// -----------------------------------------------------------------------------
//
void CSensorDataCompensatorInitializer::InitCfw()
    {
    FUNC_LOG;

    // Setup delete indicator to detect listener deletion from controller and try to create
    TBool deleteInd( EFalse );
    iDeleteInd = &deleteInd;
    TRAPD( err, iPlg.InitCfwL() );
    ERROR( err, "CSensorDataCompensatorInitializer::InitCfw() - InitCfwL fail" );
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
                INFO( "CSensorDataCompensatorInitializer::InitCfw() - Retry out of memory" );
                }
            }
        if ( iPeriodic )
            {
            INFO_1( "CSensorDataCompensatorInitializer::InitCfw() - Retry after %d us",
                KRetryInterval );
            }
        }
    }


// -----------------------------------------------------------------------------
// CSensorDataCompensatorInitializer::DoCancel()
//
// -----------------------------------------------------------------------------
//
void CSensorDataCompensatorInitializer::DoCancel()
    {
    FUNC_LOG;

    iProperty.Cancel();
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorInitializer::RunL()
//
// -----------------------------------------------------------------------------
//
void CSensorDataCompensatorInitializer::RunL()
    {
    FUNC_LOG;

    iProperty.Subscribe( iStatus );
    TInt value( 0 );
    TInt err( iProperty.Get( value ) );
    ERROR( err, "CSensorDataCompensatorInitializer::RunL() - Get property fail" );
    SetActive();

    if ( err == KErrNone && value == EIdlePhase1Ok )
        {
        // Cancel listening and create controller
        Cancel();
        InitCfw();
        }
    else
        {
        // Listening continues
        INFO( "CSensorDataCompensatorInitializer::RunL() - No idle detected, subscribe and wait idle key" );
        }
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorInitializer::RunError
// Handle error in RunL
// -----------------------------------------------------------------------------
//
#ifdef _DEBUG
TInt CSensorDataCompensatorInitializer::RunError(TInt aError)
#else
TInt CSensorDataCompensatorInitializer::RunError(TInt /*aError*/)
#endif
    {
    FUNC_LOG;
    ERROR( aError, "CSensorDataCompensatorInitializer::RunError" );
    // Error occurred while trying to communicate with context framework.
    // Return KErrNone to avoid panic.
    return KErrNone;
    }
// End of file
