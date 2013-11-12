/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApStartupController implementation.
*
*/


// INCLUDE FILES

#include <startupdomainpskeys.h>
#include <startupdomaincrkeys.h>
#include "coreapplicationuisprivatecrkeys.h"
#include <hwrmpowerstatesdkpskeys.h>
#include "SysApStartupController.h"
#include "SysApAppUi.h"
#include "SysApSubscriber.h"
#include "SysAp.hrh"

//CONSTANTS

// ========================== MEMBER FUNCTIONS ================================

// ----------------------------------------------------------------------------
// CSysApStartupController* CSysApStartupController::NewL()
// ----------------------------------------------------------------------------

CSysApStartupController* CSysApStartupController::NewL( CSysApAppUi& aSysApAppUi, TBool aOfflineModeSupported )
    {       
    TRACES( RDebug::Print( _L("CSysApStartupController::NewL" ) ) );
    CSysApStartupController* self = new ( ELeave ) CSysApStartupController( aSysApAppUi, aOfflineModeSupported );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }

#pragma warning( disable : 4355 ) // 'this' used in base member initializer list
// ----------------------------------------------------------------------------
// CSysApStartupController::CSysApStartupController( CSysApAppUi& aSysApAppUi )
// ----------------------------------------------------------------------------

CSysApStartupController::CSysApStartupController( CSysApAppUi& aSysApAppUi, TBool aOfflineModeSupported  )
    : 
    iSysApAppUi( aSysApAppUi ),
    iOfflineModeSupported( aOfflineModeSupported ),
    iFirstNormalState( ETrue ),
    iSystemStateBeingNotified( EFalse ),
    iNotifiedGlobalState( 0 )
#ifdef SYSAP_USE_STARTUP_UI_PHASE
    , iStartupUiPhaseSubscriber( NULL ),
    iStartupUiAllDoneHandled( EFalse ),
    iStartupUiAllDone( EFalse )
#endif // SYSAP_USE_STARTUP_UI_PHASE    
    {
    TRACES( RDebug::Print( _L("CSysApStartupController::CSysApStartupController iOfflineModeSupported = %d" ), iOfflineModeSupported ) );
    }
#pragma warning( default : 4355 )

// ----------------------------------------------------------------------------
// CSysApStartupController::~CSysApStartupController()
// ----------------------------------------------------------------------------

CSysApStartupController::~CSysApStartupController()
    {
    TRACES( RDebug::Print( _L("~CSysApStartupController") ) );
    delete iGlobalSystemState;

#ifdef SYSAP_USE_STARTUP_UI_PHASE
    delete iStartupUiPhaseSubscriber;
#endif // SYSAP_USE_STARTUP_UI_PHASE    
    }

// ----------------------------------------------------------------------------
// CSysApStartupController::ConstructL()
// ----------------------------------------------------------------------------

void CSysApStartupController::ConstructL(  )
    {
    iGlobalSystemState = CSysApSubscriber::NewL( *this, KPSUidStartup, KPSGlobalSystemState ); // Old Shared Data name: KStateValueKey
    iGlobalSystemState->Subscribe();
    
#ifdef SYSAP_USE_STARTUP_UI_PHASE
    iStartupUiPhaseSubscriber = CSysApSubscriber::NewL( *this, KPSUidStartup, KPSStartupUiPhase );
    iStartupUiPhaseSubscriber->Subscribe();
#endif // SYSAP_USE_STARTUP_UI_PHASE    
    }

// ----------------------------------------------------------------------------
// CSysApStartupController::HandlePropertyChangedL
// ----------------------------------------------------------------------------

void CSysApStartupController::HandlePropertyChangedL( const TUid& aCategory, const TUint aKey )
    {
    TInt value( iSysApAppUi.StateOfProperty( aCategory, aKey ) );
    
    TRACES( RDebug::Print( _L("CSysApStartupController::HandlePropertyChangedL: aCategory=0x%x, aKey=%d, value=%d" ), 
                           aCategory, aKey, value ) );

    
    if ( iSysApAppUi.ResourcesFreed() )
        {
        TRACES( RDebug::Print( _L( "CSysApStartupController::HandlePropertyChangedL: discarded, already shutting down" ) ) );
        return;
        }
    
    if ( aKey == KPSGlobalSystemState )
        {
        iSystemStateBeingNotified = ETrue;
        iNotifiedGlobalState = value;
        TRAPD( err, HandleGlobalSystemStateChangeL( value ) );
        iSystemStateBeingNotified = EFalse;
        if ( err != KErrNone )
            {
            TRACES( RDebug::Print( _L("CSysApStartupController::HandlePropertyChangedL: err=%d"), err ) );
            }
        User::LeaveIfError( err );

        }
        
#ifdef SYSAP_USE_STARTUP_UI_PHASE
     if ( aKey == KPSStartupUiPhase && value == EStartupUiPhaseAllDone && !iStartupUiAllDoneHandled )
        {
        iStartupUiAllDoneHandled = ETrue;
        iStartupUiAllDone = ETrue;
        iSysApAppUi.HandleUiReadyAfterBootL();    
        }
#endif // SYSAP_USE_STARTUP_UI_PHASE        
    }

// ----------------------------------------------------------------------------
// CSysApStartupController::DoInitialSwStateCheckL()
// ----------------------------------------------------------------------------

void CSysApStartupController::DoInitialSwStateCheckL()
    {
    TRACES( RDebug::Print( _L( "CSysApStartupController::DoInitialSwStateCheckL : START" ) ) );

    HandlePropertyChangedL( KPSUidStartup, KPSGlobalSystemState );

#ifdef SYSAP_USE_STARTUP_UI_PHASE
    HandlePropertyChangedL( KPSUidStartup, KPSStartupUiPhase );
#endif // SYSAP_USE_STARTUP_UI_PHASE

    TRACES( RDebug::Print( _L( "CSysApStartupController::DoInitialSwStateCheckL : END" ) ) );
    }
    
// ----------------------------------------------------------------------------
// CSysApStartupController::HandleGlobalSystemStateChangeL
// ----------------------------------------------------------------------------

void CSysApStartupController::HandleGlobalSystemStateChangeL( TInt aValue )
    {
    TBool normalState(EFalse);

    switch ( aValue )
        {
        case ESwStateNormalRfOn:
            normalState = ETrue;
            if ( iOfflineModeSupported && (iSysApAppUi.OfflineModeActive() || iFirstNormalState) )
                {
                TBool changeProfile = ETrue;
                
                // during boot, allow profile change only if switching from offline to online state
                if ( iFirstNormalState && !iSysApAppUi.OfflineModeActive() )
                    {
                    changeProfile = EFalse;
                    }
                    
                // Offline/Online mode changed by some other application (or boot)
                iSysApAppUi.GoOnlineL( changeProfile );
                }

            break;
        case ESwStateNormalRfOff:
            normalState = ETrue;
            if ( iOfflineModeSupported && (!iSysApAppUi.OfflineModeActive() || iFirstNormalState) )
                {
                // Offline/Online mode changed by some other application (or boot)
                iSysApAppUi.GoOfflineL();
                }
            break;
        case ESwStateNormalBTSap:
            // This mode can conceivably only be set by SysAp, so ignore.
            break;
        case ESwStateCharging:
            iSysApAppUi.DoStateChangedL(RStarterSession::ECharging);     
            break;
        case ESwStateAlarm:
            iSysApAppUi.DoStateChangedL(RStarterSession::EAlarm);     
            break;
#ifdef SYSAP_USE_STARTUP_UI_PHASE
        case ESwStateEmergencyCallsOnly:
            // idle state UI initializations are not allowed
            iStartupUiAllDoneHandled = ETrue;
            iStartupUiAllDone = EFalse;
            break;
#endif // SYSAP_USE_STARTUP_UI_PHASE
        default:
            break;
        }

    if ( iFirstNormalState && normalState )
        {      
        iSysApAppUi.DoStateChangedL(RStarterSession::ENormal);
        User::ResetInactivityTime();
        iFirstNormalState = EFalse;              
        }
    }

// ----------------------------------------------------------------------------
// CSysApStartupController::GlobalStateChangeAllowed()
// ----------------------------------------------------------------------------

TBool CSysApStartupController::GlobalStateChangeAllowed( const RStarterSession::TGlobalState aState ) const
    {
    TRACES( RDebug::Print( _L( "CSysApStartupController::GlobalStateChangeAllowed : aState=%d, iSystemStateBeingNotified=%d, iNotifiedGlobalState=%d" ),
                           aState, iSystemStateBeingNotified, iNotifiedGlobalState ) );
    
    // Redundant state changes are filtered out when a global state change is ongoing
    if ( iSystemStateBeingNotified ) // KPSGlobalSystemState notification currently handled 
        {
        if ( aState == RStarterSession::ENormal && iNotifiedGlobalState == ESwStateNormalRfOn 
            || aState == RStarterSession::EOffline && iNotifiedGlobalState == ESwStateNormalRfOff )
            {
            return EFalse;
            }
        }
    
    return ETrue;
    }

#ifdef SYSAP_USE_STARTUP_UI_PHASE
// ----------------------------------------------------------------------------
// CSysApStartupController::UiReady()
// ----------------------------------------------------------------------------

TBool CSysApStartupController::UiReady() const
    {
    return iStartupUiAllDone;    
    }
#endif // SYSAP_USE_STARTUP_UI_PHASE

// End of File

