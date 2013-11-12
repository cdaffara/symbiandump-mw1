/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CFStarter class implementation.
*
*/


#include "cfstarter.h"
#include "cfstarterobserver.h"
#include "cfphasedevicestarting.h"
#include "cfphasedevicestarted.h"
#include "cfphaseloadingplugins.h"
#include "cfphaseloadingrules.h"
#include "cfphasecfwready.h"
#include "cfwcontextdef.h"
#include "cfcontextinterface.h"
#include "cftrace.h"

// CONSTANTS

#ifdef _DEBUG

// Panic category
_LIT( KPanicCat, "CFStarter" );

// Panic codes
enum TPanicReason
    {
    EInvalidObserver,
    EAlreadyStarted,
    EInvalidState
    };

// Local panic function
LOCAL_C void Panic( TInt aCode )
    {
    User::Panic( KPanicCat, aCode );
    }

#endif

// MEMBER FUNCTIONS

CCFStarter* CCFStarter::NewL( MCFContextInterface& aCF )
    {
    FUNC_LOG;
    
    CCFStarter* self = CCFStarter::NewLC( aCF );
    CleanupStack::Pop( self );

    return self;
    }

CCFStarter* CCFStarter::NewLC( MCFContextInterface& aCF )
    {
    FUNC_LOG;
    
    CCFStarter* self = new ( ELeave ) CCFStarter( aCF );
    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }

CCFStarter::~CCFStarter()
    {
    FUNC_LOG;
    
    Cancel();
    iPhases.ResetAndDestroy();
    iObservers.Close();
    }

CCFStarter::CCFStarter( MCFContextInterface& aCF ):
    CActive( EPriorityStandard ),
    iCF( aCF )
    {
    FUNC_LOG;
    
    CActiveScheduler::Add( this );
    }

void CCFStarter::ConstructL()
    {
    FUNC_LOG;
    
    // Construct starter phases
    iPhases.InsertL(
        CCFPhaseDeviceStarting::NewL( iCF ),
        CCFPhaseBase::ECFDeviceStarting );
    iPhases.InsertL(
        CCFPhaseDeviceStarted::NewL( iCF ),
        CCFPhaseBase::ECFDeviceStarted );
    iPhases.InsertL(
        CCFPhaseLoadingPlugins::NewL( iCF ),
        CCFPhaseBase::ECFLoadingPlugins );
    iPhases.InsertL(
        CCFPhaseLoadingRules::NewL( iCF ),
        CCFPhaseBase::ECFLoadingRules );
    iPhases.InsertL(
        CCFPhaseCfwReady::NewL( iCF ),
        CCFPhaseBase::ECFCfwReady );
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFStarter::AddObserverL
//-----------------------------------------------------------------------------
//
void CCFStarter::AddObserverL( MCFStarterObserver* aObserver )
    {
    FUNC_LOG;
    
    __ASSERT_DEBUG( aObserver, Panic( EInvalidObserver ) );
    
    iObservers.AppendL( aObserver );

    aObserver->SetEventHandler( *this );
    }

//-----------------------------------------------------------------------------
// CCFStarter::Start
//-----------------------------------------------------------------------------
//
void CCFStarter::StartL()
    {
    FUNC_LOG;
    
    HEAP( "Starting CF initialization" );
    TIMESTAMP( "Starting CF initialization" );
    
    // Define context: [ContextFramework: Status]
    iCF.DefineContext( KCfwSource, KCfwSourceStatus, KCfwSourceStatusSec );
    
    // Set in first state and run it
    iCurrentPhase = CCFPhaseBase::ECFDeviceStarting;
    RunNextL();
    }

//-----------------------------------------------------------------------------
// CCFStarter::RunL
//-----------------------------------------------------------------------------
//
void CCFStarter::RunL()
    {
    FUNC_LOG;
    
    // First switch to next phase if error occurs we can continue
    CCFPhaseBase::TCFPhaseId phaseId = SwitchToNextPhase();

    INFO_1( "Phase %d execution start", phaseId );
    HEAP_1( "Phase %d execution start", phaseId );
    TIMESTAMP( "Phase execution start" );

    // Handle current phase
    if( iStatus == KErrNone )
        {
        TInt count = iObservers.Count();
        for( TInt i = 0; i < count; i++ )
            {
            TInt err = KErrNone;
            TRAP( err, iObservers[i]->InitializePhaseL( phaseId ) );
            ERROR_3( err, "Phase observer with index: [%d] failed to init in phase: [%d] with error code: %d",
                i,
                phaseId,
                err );
            }
        }
        
    INFO_1( "Phase %d execution end", phaseId );
    HEAP_1( "Phase %d execution end", phaseId );
    TIMESTAMP( "Phase execution end" );

    // Run next phase
    RunNextL();
    }

//-----------------------------------------------------------------------------
// CCFStarter::DoCancel
//-----------------------------------------------------------------------------
//
void CCFStarter::DoCancel()
    {
    FUNC_LOG;
    
    if( iCurrentPhase < iPhases.Count() )
        {
        iPhases[iCurrentPhase]->Cancel();
        }
    }

//-----------------------------------------------------------------------------
// CCFStarter::RunError
//-----------------------------------------------------------------------------
//
TInt CCFStarter::RunError( TInt /*aError*/ )
    {
    FUNC_LOG;

    // Current phase failed to run in RunL
    // Switch to next phase
    SwitchToNextPhase();
    TInt count = iPhases.Count();
    for( TInt i = iCurrentPhase; i < count; i++ )
        {
        TRAPD( err, RunNextL() );
        if( err == KErrNone )
            {
            break;
            }
        else
            {
            // Failed to run next phase again
            // Switch to next phase
            SwitchToNextPhase();
            }
        }
    return KErrNone;
    }

//-----------------------------------------------------------------------------
// CCFStarter::HandleEvent
//-----------------------------------------------------------------------------
//
void CCFStarter::HandleEvent( TCFStarterEvents aEvent )
    {
    FUNC_LOG;
    
    if( iCurrentPhase >= 0 && iCurrentPhase < iPhases.Count() )
        {
        iPhases[iCurrentPhase]->HandleEvent( aEvent );	
        }    
    }

//-----------------------------------------------------------------------------
// CCFStarter::SwitchToNextPhase
//-----------------------------------------------------------------------------
//
CCFPhaseBase::TCFPhaseId CCFStarter::SwitchToNextPhase()
    {
    FUNC_LOG;
    
    __ASSERT_DEBUG( iCurrentPhase >= 0 && iCurrentPhase < iPhases.Count(),
        Panic( EInvalidState ) );

    CCFPhaseBase::TCFPhaseId previous = iCurrentPhase;
    iCurrentPhase = iPhases[iCurrentPhase]->NextPhase();
    
    return previous;
    }

//-----------------------------------------------------------------------------
// CCFStarter::RunNextL
//-----------------------------------------------------------------------------
//
void CCFStarter::RunNextL()
    {
    FUNC_LOG;

    __ASSERT_DEBUG( !IsActive(), Panic( EAlreadyStarted ) );

    if( iCurrentPhase < iPhases.Count() )
        {
        iStatus = KRequestPending;
        iPhases[iCurrentPhase]->ExecuteL( &iStatus );
        SetActive();
        }
    else
        {
        HEAP( "[END] CF Starter phases executed" );
        }
    }
