/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  AppUi class of the application.
*
*/


// SYSTEM INCLUDES
#include <StringLoader.h>

#include <featmgr.h>                // Feature Manager
#include <centralrepository.h>
#include <startup.rsg>
#include <starterdomaincrkeys.h>
#include "startupappprivatepskeys.h"
#include <startupdomainpskeys.h>
#include <startupdomaincrkeys.h>
#include <CoreApplicationUIsSDKCRKeys.h>
#include <starterclient.h>
#include <hbdevicemessageboxsymbian.h>
#include "sanimstartupctrl.h"
#include <apgtask.h>

// USER INCLUDES
#include "StartupAppUi.h"
#include "StartupApplication.h"

#include "StartupPubSubObserver.h"
#include "startupanimationwrapper.h"
#include "startupview.h"
  
#include <eikenv.h>


static const CSAnimStartupCtrl::TAnimationParams KStartupAnimationParams =
    {
    KCRUidStartupConf,
    KStartupAnimationPath,
    KStartupAnimationFrameDelay,
    KStartupAnimationScaling,
    KStartupTonePath,
    KStartupToneVolume
    };


_LIT_SECURITY_POLICY_C1(KReadDeviceDataPolicy, ECapabilityReadDeviceData);
_LIT_SECURITY_POLICY_C1(KWriteDeviceDataPolicy, ECapabilityWriteDeviceData);
_LIT_SECURITY_POLICY_PASS(KAlwaysPassPolicy);

// ======== LOCAL FUNCTIONS ==================================================

namespace
    {
    TInt AnimationFinishedFunc( TAny* aPtr )
        {
        static_cast<CStartupAppUi*>( aPtr )->AnimationFinished();
        return KErrNone;
        }
    }



// ================= MEMBER FUNCTIONS ========================================
//
// ---------------------------------------------------------------------------
// CStartupAppUi::CStartupAppUi()
// ---------------------------------------------------------------------------
CStartupAppUi::CStartupAppUi() :
   iInternalState( EStartupStartingUp ),

    iStartupFirstBootAndRTCCheckAlreadyCalled( EFalse ),
    iChargingOrAlarmBoot( EFalse ),
    iFirstBoot( ETrue ),
    iCleanBoot( EStartupNormalBoot ),
    iOfflineModeSupported( EFalse ),
    iOfflineModeQueryShown( EFalse ),
    iCriticalBlockEnded( EFalse ),
    iSwStateFatalStartupError( EFalse ),
    iStartupWaitingShowStartupAnimation( EFalse ),
    iSimSupported( ETrue )

    {
    TRACES("CStartupAppUi::CStartupAppUi");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::ConstructL()
// ---------------------------------------------------------------------------
void CStartupAppUi::ConstructL()
    {
    RThread thread; //increase the thread priority to smooth the animation
    thread.SetPriority(EPriorityAbsoluteForeground);
    thread.Close();
    TRACES("CStartupAppUi::ConstructL()");
    TInt flags = EStandardApp;
   BaseConstructL( flags );
        
    iMainView = CStartupView::NewL( ApplicationRect() );
    
   
    iAnimation = CStartupAnimationWrapper::NewL( *iMainView );


    TInt err = RProperty::Define( KPSUidStartupApp,
                                  KPSStartupAppState,
                                  RProperty::EInt,
                                  KReadDeviceDataPolicy,
                                  KWriteDeviceDataPolicy );

    if( KErrNone != err &&
        KErrAlreadyExists != err )
        {
        TRACES1("CStartupAppUi::ConstructL(): KPSStartupAppState define err %d", err);
        }

    err = RProperty::Set( KPSUidStartupApp,
                          KPSStartupAppState,
                          EStartupAppStateWait );

    if( KErrNone != err )
        {
        TRACES1("CStartupAppUi::ConstructL(): KPSStartupAppState set err %d", err);
        }

    iStartupPubSubObserver = CStartupPubSubObserver::NewL( this );




    err = RProperty::Define( KPSUidStartup,
                             KPSStartupUiPhase,
                             RProperty::EInt, 
                             KAlwaysPassPolicy,
                             KWriteDeviceDataPolicy );
                             
    if( KErrNone != err &&
        KErrAlreadyExists != err )
        {
        TRACES1("CStartupAppUi::ConstructL(): KPSStartupUiPhase define err %d", err);       
        }
    
    UpdateStartupUiPhase( EStartupUiPhaseUninitialized );


    FeatureManager::InitializeLibL();
    if ( FeatureManager::FeatureSupported( KFeatureIdOfflineMode ) )
        {
        iOfflineModeSupported = ETrue;
        }
    if ( !FeatureManager::FeatureSupported( KFeatureIdSimCard ) )
        {
        iSimSupported = EFalse;
        }
    

    TRACES1("CStartupAppUi::ConstructL(): Offline mode supported: %d", iOfflineModeSupported );
    TRACES1("CStartupAppUi::ConstructL(): SIM card supported:     %d", iSimSupported );
    

    iFirstBoot = FirstBoot();
    TRACES1("CStartupAppUi::ConstructL(): First boot:             %d", iFirstBoot );


    CEikonEnv* eikEnv = CEikonEnv::Static();

    // Set Startup application to be system application
    eikEnv->SetSystem( ETrue );

    eikEnv->RootWin().SetOrdinalPosition(0,0);

    // Disable priority changes of window server
    eikEnv->WsSession().ComputeMode(
        RWsSession::EPriorityControlDisabled );
   iNoteTimer = CPeriodic::NewL( EPriorityNormal );

    iExitTimer = CPeriodic::NewL( EPriorityNormal );


    if ( iSwStateFatalStartupError )
        {
        TRACES("CStartupAppUi::ConstructL(): ESWStateFatalStartupError");
        DoNextStartupPhaseL( EStartupSystemFatalError );
        TRACES("CStartupAppUi::ConstructL(): End, Boot failed");
        return;//Execution stops here.
        }

    DoStartupStartPartL();
    TRACES("CStartupAppUi::ConstructL(): End");
    }


// ---------------------------------------------------------------------------
// CStartupAppUi::~CStartupAppUi()
// ---------------------------------------------------------------------------
CStartupAppUi::~CStartupAppUi()
    {
    TRACES("CStartupAppUi::~CStartupAppUi()");


    if( iExitTimer )
        {
        iExitTimer->Cancel();
        delete iExitTimer;
        }



    if( iNoteTimer )
        {
        iNoteTimer->Cancel();
        delete iNoteTimer;
        }


 
    delete iAnimation;
    delete iStartupPubSubObserver;
   
    delete iMainView;


    FeatureManager::UnInitializeLib();

    TRACES("CStartupAppUi::~CStartupAppUi(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::PrepareToExit()
// ---------------------------------------------------------------------------
void CStartupAppUi::PrepareToExit()
    {
    TRACES("CStartupAppUi::PrepareToExit()");



 
	CEikAppUi::PrepareToExit();

#ifndef RD_BOOT_CUSTOMIZABLE_AI
    if( !iChargingOrAlarmBoot )
        {
        TRACES("CStartupAppUi::PrepareToExit(): NOT Charger/alarm boot");

        TRACES("CStartupAppUi::PrepareToExit(): Activate Idle app");
        TVwsViewId viewId = TVwsViewId(TUid::Uid(0x101FD64C), TUid::Uid(0x101FD64C));

        TUid customMessageUid = KNullUid;

        TRAPD(ignore, CreateActivateViewEventL(viewId, customMessageUid, KNullDesC8()));
        if (ignore != KErrNone)
            {
            TRACES1("CStartupAppUi::PrepareToExit(): CreateActivateViewEventL() leaves, err = %d", ignore );
            }
        }
#endif //RD_BOOT_CUSTOMIZABLE_AI
    TRACES("CStartupAppUi::PrepareToExit(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::DoStopTimingL(TAny* aObject)
// ---------------------------------------------------------------------------
TInt CStartupAppUi::DoStopTimingL(TAny* aObject)
    {
    TRACES("CStartupAppUi::DoStopTimingL() +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ ");
    TRACES1("CStartupAppUi::DoStopTimingL(): Object: %d", aObject);
    STATIC_CAST( CStartupAppUi*, aObject )->StopTimingL(); // cast, and call non-static function
    TRACES("CStartupAppUi::DoStopTimingL(): End ------------------------------------------------------ ");
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::DoExitApplication(TAny* aObject)
// ---------------------------------------------------------------------------
TInt CStartupAppUi::DoExitApplication(TAny* aObject)
    {
    TRACES("CStartupAppUi::DoExitApplication()");
    STATIC_CAST( CStartupAppUi*, aObject )->ExitApplication(); // cast, and call non-static function
    TRACES("CStartupAppUi::DoExitApplication(): End");
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CStartupAppUi::HandleKeyEventL
// ---------------------------------------------------------------------------
TKeyResponse CStartupAppUi::HandleKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode aType )
    {
    TRACES3("CStartupAppUi::HandleKeyEventL(): code = %d, type = %d, repeats = %d",
        aKeyEvent.iCode, aType, aKeyEvent.iRepeats);

    TKeyResponse response( EKeyWasNotConsumed );
    if ( aType != EEventNull &&
         aKeyEvent.iCode == EKeyApplication0 && // Apps key
         ( iInternalState == EStartupShowingWelcomeAnimation ||
           iInternalState == EStartupShowingOperatorAnimation ||
           iInternalState == EStartupShowingUserWelcomeNote ) )
        {
        if ( aKeyEvent.iRepeats && iCleanBoot != EStartupCleanBoot)
            {
            // Long apps key press detected. Set clean boot.
            SetCleanBoot();
            }

        response = EKeyWasConsumed;
        }
    else if ( aType != EEventNull &&
              aKeyEvent.iCode != EStdKeyNull &&
              aType == EEventKeyDown )
        {
        TRACES("CStartupAppUi::HandleKeyEventL(): Event code is EEventKeyDown");

        if ( iOfflineModeQueryShown )
            {
            TRACES("CStartupAppUi::HandleKeyEventL(): First key event comes from Offline Mode Query");
            // first key event comes from Offline Mode Query
            iOfflineModeQueryShown = EFalse;
            response = EKeyWasConsumed;
            }
		
        else if ( ( iInternalState == EStartupShowingWelcomeAnimation ||
                    iInternalState == EStartupShowingOperatorAnimation ) &&
                   !( iAnimation->WasCancelled() ) )
            {
            TRACES("CStartupAppUi::HandleKeyEventL(): This key event is used to cancel animations");

            CancelAnimation();
            response = EKeyWasConsumed;
            }
        }

    TRACES1("CStartupAppUi::HandleKeyEventL(): End, return %d", response);
    return response;
    }

/*
 * Qt Support Not available..
// ---------------------------------------------------------------------------
// CStartupAppUi::HandleResourceChangeL
//
// ---------------------------------------------------------------------------
//
void CStartupAppUi::HandleResourceChangeL( TInt aType )
    {
    TRACES("CStartupAppUi::HandleResourceChangeL()");
    TRACES1("CStartupAppUi::HandleResourceChangeL Type: %d", aType);
  
    
     * No equivalent in Qt. 
    CAknAppUi::HandleResourceChangeL( aType );

    if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        iMainView->SetRect( ApplicationRect() );
        }
     
    TRACES("CStartupAppUi::HandleResourceChangeL(): End");
    }

*/


// ---------------------------------------------------------------------------
// CStartupAppUi::HandleCommandL(TInt aCommand)
// ---------------------------------------------------------------------------
void CStartupAppUi::HandleCommandL(TInt aCommand)
    {
    TRACES("CStartupAppUi::HandleCommandL()");
    switch (aCommand)
        {
        case EEikCmdExit:
            {
            TRACES("CStartupAppUi::HandleCommandL(): EEikCmdExit");

			Exit();
            }
            break;
        default:
            break;
        }//End of switch-clause
    TRACES("CStartupAppUi::HandleCommandL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::DoStartupStartPartL()
// ---------------------------------------------------------------------------
void CStartupAppUi::DoStartupStartPartL()
    {
    TRACES("CStartupAppUi::DoStartupStartPartL()");

    TryPreLoadAnimation();

    DoNextStartupPhaseL( EStartupWaitingCriticalBlock );
    TRACES("CStartupAppUi::DoStartupStartPartL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::TryPreLoadAnimation()
// ---------------------------------------------------------------------------
void CStartupAppUi::TryPreLoadAnimation()
    {
    TRACES("CStartupAppUi::TryPreLoadAnimation()");

    TInt state( ESwStateStartingCriticalApps );
    TInt err;
    err = RProperty::Get( KPSUidStartup, KPSGlobalSystemState, state ); // Use default if this fails
    if( KErrNone != err )
        {
        TRACES1("CStartupAppUi::TryPreLoadAnimation(): KPSGlobalSystemState get err %d", err);
        }
    TInt mode( EStartupModeNormal );
    err = RProperty::Get( KPSUidStartup, KPSGlobalStartupMode, mode ); // Use default if this fails
    if( KErrNone != err )
        {
        TRACES1("CStartupAppUi::TryPreLoadAnimation(): KPSGlobalStartupMode get err %d", err);
        }

    TRACES2("CStartupAppUi::TryPreLoadAnimation(): State: %d, mode %d", state, mode);

    if ( ( !HiddenReset() || iFirstBoot ) && // First boot or not-a-reset
         ( ( ( state == ESwStateStartingCriticalApps || state == ESwStateSelfTestOK ) &&
           mode == EStartupModeNormal ) || // Not going to charging or alarm mode
         state == ESwStateSecurityCheck ) )
        {
        iAnimation->PreLoad(
            ClientRect(),
			*iMainView,
            KStartupAnimationParams,
            ETrue,
            SecondaryDisplay::EStartWelcomeAnimation );
        }

    TRACES("CStartupAppUi::TryPreLoadAnimation(): End");
    }



// ---------------------------------------------------------------------------
// CStartupAppUi::DoStartupShowWelcomeAnimationL()
// ---------------------------------------------------------------------------
void CStartupAppUi::DoStartupShowWelcomeAnimationL()
    {
    TRACES("CStartupAppUi::DoStartupShowWelcomeAnimationL()");
    if ( HiddenReset() )
        {
        //skip the welcome animation and user welcome note and continue with
        //the same way like in the end of ShowUserWelcomeNoteL()
        TRACES("CStartupAppUi::DoStartupShowWelcomeAnimationL(): Skip the animation and UWN because it's hidden reset");
        TRACES("CStartupAppUi::DoStartupShowWelcomeAnimationL(): and change internal state directly to EStartupFirstBootAndRTCCheck");

        DoNextStartupPhaseL( EStartupFirstBootAndRTCCheck );

        }

    else
        {
        TRACES("CStartupAppUi::DoStartupShowWelcomeAnimationL(): Show animation");

        iAnimation->Play(
            ClientRect(),
			*iMainView,
            KStartupAnimationParams,
            ETrue,
            SecondaryDisplay::EStartWelcomeAnimation,
            TCallBack( AnimationFinishedFunc, this ) );
        }


    TRACES("CStartupAppUi::DoStartupShowWelcomeAnimationL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::DoStartupShowOperatorAnimationL()
// ---------------------------------------------------------------------------
void CStartupAppUi::DoStartupShowOperatorAnimationL()
    {
    TRACES("CStartupAppUi::DoStartupShowOperatorAnimationL()");
    if ( HiddenReset() )
        {
        //skip the operator animation and user welcome note and continue with
        //the same way like in the end of ShowUserWelcomeNoteL()
        TRACES("CStartupAppUi::DoStartupShowOperatorAnimationL(): Skip the animation and UWN because it's hidden reset");
        TRACES("CStartupAppUi::DoStartupShowOperatorAnimationL(): and change internal state directly to EStartupFirstBootAndRTCCheck");
        DoNextStartupPhaseL( EStartupFirstBootAndRTCCheck );

        }

    else
        {
        TRACES("CStartupAppUi::DoStartupShowOperatorAnimationL(): Show animation");

        CSAnimStartupCtrl::TAnimationParams params;
        params.iRepositoryUid = KCRUidStartupConf;
        params.iAnimationPathKey = KStartupOperatorAnimationPath;
        params.iFrameDelayKey = KStartupOperatorAnimationFrameDelay;
        params.iEnableScalingKey = KStartupOperatorAnimationScaling;
        params.iTonePathKey = KStartupOperatorTonePath;
        params.iVolumeKey = KStartupOperatorToneVolume;
        iAnimation->Play(
            ClientRect(),
	
            *iMainView,
            params,
            EFalse,
            SecondaryDisplay::EStartOperatorAnimation,
            TCallBack( AnimationFinishedFunc, this ) );
        }


    TRACES("CStartupAppUi::DoStartupShowOperatorAnimationL(): End");
    }


// ---------------------------------------------------------------------------
// CStartupAppUi::PredictiveTimeEnabled()
// ---------------------------------------------------------------------------
TBool CStartupAppUi::PredictiveTimeEnabled()
    {
    TRACES("CStartupAppUi::PredictiveTimeEnabled()");

    TInt value( EPredictiveTimeEnabled );
    CRepository* repository(NULL);

    TRAPD( err, repository = CRepository::NewL( KCRUidStartupConf ) );
    if ( err == KErrNone )
        {
        err = repository->Get( KPredictiveTimeAndCountrySelection, value );
        }
    delete repository;

    TRACES1("CStartupAppUi::PredictiveTimeEnabled(): returns %d", value);
    return value;
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::DoStartupFirstBootAndRTCCheckL()
// ---------------------------------------------------------------------------
void CStartupAppUi::DoStartupFirstBootAndRTCCheckL()
    {
    TRACES("CStartupAppUi::DoStartupFirstBootAndRTCCheckL()");


    RProperty::Set( KPSUidStartup, KStartupCleanBoot, iCleanBoot );
    RProperty::Set( KPSUidStartup, KPSSplashShutdown, ESplashShutdown );

    iMainView->DrawDeferred();


    // When Predictive Time and Country Selection is enabled, no queries are
    // shown to user during first boot. Instead, Clock application gets the
    // time and location from the network and marks the first boot as done.
    if( !PredictiveTimeEnabled() )
        {
        if( iFirstBoot && !HiddenReset() ) //&& StartupQueriesEnabled() )
            {
            TRACES("CStartupAppUi::DoStartupFirstBootAndRTCCheckL(): First boot. Show city, time and date queries.");
		    TRACES("CStartupAppUi::DoStartupFirstBootAndRTCCheckL(): Mark first boot");
            MarkFirstBoot();    
                
            }
		else if( !RTCStatus() && !HiddenReset() ) // && StartupQueriesEnabled())
            {
            TRACES("CStartupAppUi::DoStartupFirstBootAndRTCCheckL(): No first boot but RTCStatus is corrupted. Ask time and date");
		     }
		if( iFirstBoot )  // && !StartupQueriesEnabled() )
            {
            TRACES("CStartupAppUi::DoStartupFirstBootAndRTCCheckL(): First boot ongoing and queries are disabled.");
            MarkFirstBoot();
            }
        }
	else
		{
		// Temporary fix for the defect VEZI-7YDEAR , as clock is not yet supported.
		TRACES("CStartupAppUi::DoStartupFirstBootAndRTCCheckL(): if PredictiveTimeEnabled is set");
		if( iFirstBoot && !HiddenReset() )
		    {
            TRACES("CStartupAppUi::DoStartupFirstBootAndRTCCheckL(): Mark first boot");
            MarkFirstBoot();
			}
		// End of temporary fix.
		}

    TRACES("CStartupAppUi::DoStartupFirstBootAndRTCCheckL(): Setting KPSStartupAppState = EStartupAppStateFinished");
    TInt err = RProperty::Set( KPSUidStartupApp, KPSStartupAppState, EStartupAppStateFinished );
    if( KErrNone != err )
        {
        TRACES1("CStartupAppUi::DoStartupFirstBootAndRTCCheckL(): KPSStartupAppState set err %d"
                , err);
        }

    DoNextStartupPhaseL( EStartupStartupOK );
    }




// ---------------------------------------------------------------------------
// CStartupAppUi::DoStartupEndPart()
// ---------------------------------------------------------------------------
void CStartupAppUi::DoStartupEndPart()
    {
    TRACES("CStartupAppUi::DoStartupEndPart()");
    TRACES("CStartupAppUi::DoStartupEndPart(): STARTUP OK");
    
    TBool ftuenabled=EFalse;
    TInt err=KErrNone;
#ifndef __WINSCW__
    TRAP(err,
            TInt enabled=0;
            //open cenrep;
            CRepository *cenrep=CRepository::NewLC(KCRUidStartupConf);
            //check if ftu is enabled or not
            User::LeaveIfError(cenrep->Get(KFtuStartupEnabled,enabled));
            ftuenabled=!!enabled; //make sure we only have a boolean value here
            CleanupStack::PopAndDestroy(cenrep);
            );
#endif     
    if(!ftuenabled)
        {
        UpdateStartupUiPhase( EStartupUiPhaseAllDone );
        }

    TRACES("CStartupAppUi::DoStartupEndPart(): Exit application.");
    iExitTimer->Start( 100000, 100000, TCallBack( DoExitApplication, this ) );
    TRACES("CStartupAppUi::DoStartupEndPart(): DoExitApplication-timer called.");
    TRACES("CStartupAppUi::DoStartupEndPart(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::StopTimingL()
// ---------------------------------------------------------------------------
void CStartupAppUi::StopTimingL()
    {
    TRACES("CStartupAppUi::StopTimingL()");
    if( !iStartupFirstBootAndRTCCheckAlreadyCalled )
        {
        if (iInternalState == EStartupShowingUserWelcomeNote)
            {
            //telling that timer has expired in appui so that
            //key pressed are not handled any more...
            TRACES("CStartupAppUi::StopTimingL(): Stopping UWN");
            iStartupFirstBootAndRTCCheckAlreadyCalled = ETrue;
            iNoteTimer->Cancel();

            UpdateStartupUiPhase( EStartupUiPhaseUserWelcomeDone );
            DoNextStartupPhaseL( EStartupFirstBootAndRTCCheck );

            }
        }

    TRACES("CStartupAppUi::StopTimingL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::ExitApplication()
// ---------------------------------------------------------------------------
void CStartupAppUi::ExitApplication()
    {
    TRACES("CStartupAppUi::ExitApplication()");
    iExitTimer->Cancel();
    Exit();
    TRACES("CStartupAppUi::ExitApplication(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::SetCriticalBlockEndedL()
// ---------------------------------------------------------------------------
void CStartupAppUi::SetCriticalBlockEndedL()
    {
    TRACES("CStartupAppUi::SetCriticalBlockEndedL()");
    iCriticalBlockEnded = ETrue;
    if ( iInternalState == EStartupWaitingCriticalBlock )
        {
        WaitingCriticalBlockEndingL();
        }
    TRACES("CStartupAppUi::SetCriticalBlockEndedL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::WaitingCriticalBlockEndingL()
// ---------------------------------------------------------------------------
void CStartupAppUi::WaitingCriticalBlockEndingL()
    {
    TRACES("CStartupAppUi::WaitingCriticalBlockEndingL()");

    if( iCriticalBlockEnded )
        {
        TRACES("CStartupAppUi::WaitingCriticalBlockEndingL(): CriticalBlock has ended. Continue.");

        DoNextStartupPhaseL( EStartupOfflineModeQuery );
        }
    TRACES("CStartupAppUi::WaitingCriticalBlockEndingL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::SetWaitingStartupAnimationStartL()
// ---------------------------------------------------------------------------
void CStartupAppUi::SetWaitingStartupAnimationStartL()
    {
    TRACES("CStartupAppUi::SetWaitingStartupAnimationStartL()");
    iStartupWaitingShowStartupAnimation = ETrue;
    if ( iInternalState == EStartupWaitingShowStartupAnimation )
        {
        WaitingStartupAnimationStartL();
        }
    TRACES("CStartupAppUi::SetWaitingStartupAnimationStartL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::WaitingStartupAnimationStartL()
// ---------------------------------------------------------------------------
void CStartupAppUi::WaitingStartupAnimationStartL()
    {
    TRACES("CStartupAppUi::WaitingStartupAnimationStartL()");

    if( iStartupWaitingShowStartupAnimation )
        {
        TRACES("CStartupAppUi::WaitingStartupAnimationStartL(): Continue.");
        DoNextStartupPhaseL( EStartupShowingWelcomeAnimation );
        }
    TRACES("CStartupAppUi::WaitingStartupAnimationStartL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::SetEmergencyCallsOnlyL()
// ---------------------------------------------------------------------------
void CStartupAppUi::SetEmergencyCallsOnlyL()
    {
    TRACES("CStartupAppUi::SetEmergencyCallsOnlyL()");
    iCriticalBlockEnded = ETrue;
    if ( iInternalState == EStartupWaitingCriticalBlock )
        {
        TRACES("CStartupAppUi::SetEmergencyCallsOnlyL(): Entered emergency calls only state.");

        DoNextStartupPhaseL( EStartupStartupOK );
        }
    TRACES("CStartupAppUi::SetEmergencyCallsOnlyL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::SwStateFatalStartupErrorL()
// ---------------------------------------------------------------------------
void CStartupAppUi::SwStateFatalStartupErrorL(TBool aPropertyChanged)
    {
    TRACES("CStartupAppUi::SwStateFatalStartupErrorL()");

    iSwStateFatalStartupError = ETrue;

    if ( aPropertyChanged )
        {
        TRACES("CStartupAppUi::SwStateFatalStartupErrorL(): property changed");
        DoNextStartupPhaseL( EStartupSystemFatalError );
        }

    TRACES("CStartupAppUi::SwStateFatalStartupErrorL(): End");
    }
// ----------------------------------------------------------------------------
// CStartAppUi::DosInOfflineModeL()
// ----------------------------------------------------------------------------

TBool CStartupAppUi::DosInOfflineModeL()
    {
    TRACES("CStartupAppUi::DosInOfflineModeL()");
    if ( iOfflineModeSupported )
        {
        TRACES1("CStartupAppUi::DosInOfflineModeL(): iOfflineModeSupported == %d", iOfflineModeSupported );
        return UiInOfflineMode();
        }
    else
        {
        TRACES("CStartupAppUi::DosInOfflineModeL(): return EFalse");
        return EFalse; // device that does not support Off-line Mode can never be in Off-line Mode.
        }
    }


// ----------------------------------------------------------------------------
// CStartupAppUi::UiInOfflineMode()
// ----------------------------------------------------------------------------

TBool CStartupAppUi::UiInOfflineMode()
    {
    TRACES("CStartupAppUi::UiInOfflineMode()");
    if ( iOfflineModeSupported )
        {
        TInt networkConnectionAllowed ( 0 );

        TRACES("CStartupAppUi::UiInOfflineMode(): Check Offline status from CenRep");
        CRepository* repository(NULL);

        TRAPD( err, repository = CRepository::NewL( KCRUidCoreApplicationUIs ) );
        if ( err == KErrNone )
            {
            err = repository->Get( KCoreAppUIsNetworkConnectionAllowed, networkConnectionAllowed );
            }

        #ifdef _DEBUG
            if ( err != KErrNone )
                TRACES1("CStartupAppUi::UiInOfflineMode(): Central repository access failed, error code %d.", err );
            else
                TRACES1("CStartupAppUi::UiInOfflineMode(): Offline: network connection allowed: %d", networkConnectionAllowed);
        #endif
        delete repository;

        TRACES1("CStartupAppUi::UiInOfflineMode():  returns %d", !( TBool ) networkConnectionAllowed);

        return ( ! ( TBool ) networkConnectionAllowed );
        }
    else
        {
        TRACES("CStartupAppUi::UiInOfflineMode(): DosInOfflineMode returns EFalse");
        return EFalse; // device that does not support Off-line Mode can never be in Off-line Mode.
        }
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::ShowOfflineModeQueryL()
// ---------------------------------------------------------------------------
void CStartupAppUi::ShowOfflineModeQueryL()
    {
    TRACES("CStartupAppUi::ShowOfflineModeQueryL()");

    TInt status( EStartupSimInitWait );
    RProperty::Get(KPSUidStartup, KPSSimStatus, status);

    TInt reply ( 0 );
    if (status == ESimUsable || status == ESimReadable || status == ESimNotReady || !iSimSupported)
        {
        TRACES("CStartupAppUi::ShowOfflineModeQueryL(): Show offline mode query if needed");

        if ( HiddenReset() && DosInOfflineModeL() && !SimStatusChangedReset() )
            {
            TRACES("CStartupAppUi::ShowOfflineModeQueryL(): Offline Mode: Hidden reset and DOS in offline mode -> Boot to Offline");
            reply = 1;
            }
        else if ( iOfflineModeSupported && DosInOfflineModeL() )
            {
            TRACES("CStartupAppUi::ShowOfflineModeQueryL(): Offline mode query needed");
            RProperty::Set( KPSUidStartup, KPSSplashShutdown, ESplashShutdown );
            iAnimation->BringToForeground();
            iMainView->DrawDeferred();
            iOfflineModeQueryShown = ETrue;
            CHbDeviceMessageBoxSymbian *aMessageBox = NULL;
        	aMessageBox = CHbDeviceMessageBoxSymbian::NewL(CHbDeviceMessageBoxSymbian::EQuestion);
       	 	_LIT(KText, "Continue using phone in Offline mode?");
        	aMessageBox->SetTextL(KText);
        	_LIT(KAcceptText, "Yes");
        	aMessageBox->SetButtonTextL(CHbDeviceMessageBoxSymbian::EAcceptButton, KAcceptText);
        	_LIT(KRejectText, "No");
        	aMessageBox->SetButtonTextL(CHbDeviceMessageBoxSymbian::ERejectButton, KRejectText);
        	//aMessageBox->SetDismissPolicy(HbPopup::NoDismiss);
        	//define the selection button to hold user's option choice
        	CHbDeviceMessageBoxSymbian::TButtonId selection;
        	selection = aMessageBox->ExecL();
            if ( selection == CHbDeviceMessageBoxSymbian::EAcceptButton )
                {
                TRACES("CStartupAppUi::ShowOfflineModeQueryL(): Offline Mode query: YES -> Boot to Offline");
                reply = 1;
                }
            else
                {
                TRACES("CStartupAppUi::ShowOfflineModeQueryL(): Offline Mode query: NO -> Boot to Online");
                reply = 0;
                }
            }
        else
            {
            TRACES("CStartupAppUi::ShowOfflineModeQueryL(): Offline Mode: DOS in online mode -> Boot to Online");
            reply = 0;
            }

        }
#ifdef _DEBUG
    else
        {
        TRACES("CStartupAppUi::ShowOfflineModeQueryL(): SIM card removed. No offline mode query.");
        }
#endif

    TRACES1("CStartupAppUi::ShowOfflineModeQueryL(): BootIntoOffline=%d.", reply);
    TInt err = RProperty::Set( KPSUidStartup,
                               KStartupBootIntoOffline,
                               reply ? EBootIntoOfflineMode : EBootIntoOnlineMode );

    if( KErrNone != err )
        {
        TRACES1("CStartupAppUi::ShowOfflineModeQueryL(): KStartupBootIntoOffline set err %d", err);
        }
   
    TRACES("CStartupAppUi::ShowOfflineModeQueryL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::CancelAnimation(...)
// ---------------------------------------------------------------------------
void CStartupAppUi::CancelAnimation()
    {
    TRACES("CStartupAppUi::CancelAnimation()");

    iAnimation->Cancel();

    TRACES("CStartupAppUi::CancelAnimation(): End");
    }



// ---------------------------------------------------------------------------
// CStartupAppUi::FirstBoot()
// ---------------------------------------------------------------------------
TBool CStartupAppUi::FirstBoot()
    {
    TRACES("CStartupAppUi::FirstBoot()");
    TInt value( 0 );

    TRACES("CStartupAppUi::FirstBoot(): Read first boot info from Central Repository ");
    CRepository* repository(NULL);

    TRAPD( err, repository = CRepository::NewL( KCRUidStartup ) );
    if ( err == KErrNone )
        {
        err = repository->Get( KStartupFirstBoot, value );
        }

    delete repository;

    if (value)
        {
        TRACES("CStartupAppUi::FirstBoot(): End, return EFalse");
        return EFalse;
        }

    else
        {
        TRACES("CStartupAppUi::FirstBoot(): End, return ETrue");
        return ETrue;
        }
    }
// ---------------------------------------------------------------------------
// CStartupAppUi::HiddenReset()
// ---------------------------------------------------------------------------
TBool CStartupAppUi::HiddenReset()
    {
    TRACES("CStartupAppUi::HiddenReset()");

    TBool ret_val ( EFalse );
    if( !iFirstBoot )
        {
        TInt startupReason( ENormalStartup );
        TInt err = RProperty::Get( KPSUidStartup,
                                   KPSStartupReason,
                                   startupReason ); // Use default if this fails
        if( KErrNone != err )
            {
            TRACES1("CStartupAppUi::HiddenReset(): ENormalStartup get err %d", err);
            }
        ret_val = ( startupReason != ENormalStartup );
        }
    TRACES1("CStartupAppUi::HiddenReset(): End, return %d ",ret_val );
    return ret_val;
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::RTCStatus()
// ---------------------------------------------------------------------------
TBool CStartupAppUi::RTCStatus()
    {
    TRACES("CStartupAppUi::RTCStatus()");
    TBool ret_val ( EFalse );

    RStarterSession startersession;
    if( startersession.Connect() == KErrNone )
        {
        TRACES("CStartupAppUi::RTCStatus(): Connected to Starter ");
        ret_val = startersession.IsRTCTimeValid();
        startersession.Close();
        }
    else
        {
        TRACES("CStartupAppUi::RTCStatus(): Unable to connect to Starter ");
        }

    TRACES1("CStartupAppUi::RTCStatus(): End, return %d ",ret_val );
    return ret_val;
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::MarkFirstBoot()
// ---------------------------------------------------------------------------
void CStartupAppUi::MarkFirstBoot()
    {
    TRACES("CStartupAppUi::MarkFirstBoot()");
    TRACES("CStartupAppUi::MarkFirstBoot(): Write first boot info to Central Repository ");

    CRepository* repository(NULL);

    TRAPD( err, repository = CRepository::NewL( KCRUidStartup ) );
    if ( err == KErrNone )
        {
        err = repository->Set( KStartupFirstBoot, 1 );
        }

    delete repository;
    TRACES("CStartupAppUi::MarkFirstBoot(): End");
    }



// ---------------------------------------------------------------------------
// CStartupAppUi::AnimationFinished()
// ---------------------------------------------------------------------------
void CStartupAppUi::AnimationFinished()
    {
    TRACES1("CStartupAppUi::AnimationFinished(): iInternalState=%d", iInternalState );

    // first notify animation completion, it doesn't matter if it was canceled
    if ( iAnimation->HasContent() )
        {
        switch ( iInternalState )
            {
            case EStartupShowingWelcomeAnimation:
                UpdateStartupUiPhase( EStartupUiPhaseSystemWelcomeDone );
                break;
        
            case EStartupShowingOperatorAnimation: 
                UpdateStartupUiPhase( EStartupUiPhaseOperatorWelcomeDone );
                break;
        
            default:
                TRACES("CStartupAppUi::AnimationFinished(): unknown state");
                break;                    
            }    
        }
    
    
    TInt err = KErrNone;

    if ( iAnimation->WasCancelled() )
        {
        TRACES("Animations cancelled");

        TRAP(err, DoNextStartupPhaseL( EStartupFirstBootAndRTCCheck ));
        }
    else if ( iInternalState == EStartupShowingWelcomeAnimation )
        {
        TRAP(err, DoNextStartupPhaseL( EStartupShowingOperatorAnimation ));
        }
    else if ( iInternalState == EStartupShowingOperatorAnimation )
        {
        TRAP(err, DoNextStartupPhaseL( EStartupFirstBootAndRTCCheck));
        }

    if ( err != KErrNone )
        {
        TRAP_IGNORE( DoNextStartupPhaseL(EStartupSystemFatalError) );
        }

    TRACES("CStartupAppUi::AnimationFinished(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::SetCleanBoot()
// ---------------------------------------------------------------------------
void CStartupAppUi::SetCleanBoot()
    {
    TRACES("CStartupAppUi::SetCleanBoot()");
    iCleanBoot = EStartupCleanBoot;
    TRACES("CStartupAppUi::SetCleanBoot(): End");
    }

// ----------------------------------------------------------------------------
// CStartupAppUi::SimSupported()
// ----------------------------------------------------------------------------
TBool CStartupAppUi::SimSupported()
    {
    TRACES("CStartupAppUi::SimSupported()");
    return iSimSupported;
    }



// ---------------------------------------------------------------------------
// CStartupAppUi::DoNextStartupPhaseL( TStartupInternalState toState )
// State#                                Possible next states


// 0    EStartupStartingUp               1, 18
// 1    EStartupWaitingCriticalBlock     5, 18
// 5    EStartupWaitingTouchScreenCalib
// 6    EStartupWaitingPhoneLightIdle    8, 18
// 8    EStartupOfflineModeQuery         9, 18
// 9    EStartupWaitingCUIWelcomeAnim    10, 18 Removed
// 10   EStartupWaitingStartupTone       11, 18
// 11   EStartupShowingWelcomeAnimation  12, 14, 18
// 12   EStartupWaitingCUIOperatorAnim   13, 18
// 13   EStartupShowingOperatorAnimation 14, 14, 18
// 14   EStartupShowingUserWelcomeNote   15, 18 Removed
// 15   EStartupFirstBootAndRTCCheck     16, 18
// 16   EStartupWaitingCUIStartupReady   17, 18 Removed
// 17   EStartupStartupOK                -
// 18   EStartupSystemFatalError         -

// ---------------------------------------------------------------------------

void CStartupAppUi::DoNextStartupPhaseL( TStartupInternalState toState )
    {
    TRACES("CStartupAppUi::DoNextStartupPhaseL()");
    TRACES2("CStartupAppUi::DoNextStartupPhaseL(): Change internal state from %d to %d", iInternalState, toState );

    switch( iInternalState )
        {
        case EStartupStartingUp:
            {
            switch( toState )
                {
                case EStartupWaitingCriticalBlock:
                    iInternalState = EStartupWaitingCriticalBlock;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState : EStartupWaitingCriticalBlock");
                    WaitingCriticalBlockEndingL();
                    break;
                case EStartupSystemFatalError:
                    SystemFatalErrorL();
                    break;
                default:
                    __ASSERT_DEBUG( EFalse,
                        PANIC( EStartupInvalidInternalStateChange ) );
                    break;
                }
            }
            break;
        case EStartupWaitingCriticalBlock:
            {
            switch( toState )
                {
                case EStartupStartupOK:
                    iInternalState = EStartupStartupOK;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState : EStartupStartupOK");
                    DoStartupEndPart();
                    break;

                case EStartupOfflineModeQuery:
                    iInternalState = EStartupOfflineModeQuery;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState : EStartupOfflineModeQuery");
                    ShowOfflineModeQueryL();

                    DoNextStartupPhaseL( EStartupWaitingShowStartupAnimation );
                    break;
                case EStartupSystemFatalError:
                    SystemFatalErrorL();
                    break;
                default:
                    __ASSERT_DEBUG(
                        EFalse,
                        PANIC( EStartupInvalidInternalStateChange ) );
                    break;
                }
            }
            break;
        case EStartupOfflineModeQuery:
            {
            switch( toState )
                {

                case EStartupWaitingShowStartupAnimation:
                    iInternalState = EStartupWaitingShowStartupAnimation;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState : EStartupWaitingShowStartupAnimation");
                    WaitingStartupAnimationStartL();
                    break;
              case EStartupSystemFatalError:
                    SystemFatalErrorL();
                    break;
                default:
                    __ASSERT_DEBUG(
                        EFalse,
                        PANIC( EStartupInvalidInternalStateChange ) );
                    break;
                }
            }
            break;
        case EStartupWaitingShowStartupAnimation:
            {
            switch( toState )
                {
                case EStartupShowingWelcomeAnimation:
                    iInternalState = EStartupShowingWelcomeAnimation;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState : EStartupShowingWelcomeAnimation");
                    DoStartupShowWelcomeAnimationL();
                    break;
                case EStartupSystemFatalError:
                    SystemFatalErrorL();
                    break;
                default:
                    __ASSERT_DEBUG(
                        EFalse,
                        PANIC( EStartupInvalidInternalStateChange ) );
                    break;
                }
            }
            break;
        case EStartupShowingWelcomeAnimation:
            {
            switch( toState )
                {

                case EStartupShowingOperatorAnimation:
                    iInternalState = EStartupShowingOperatorAnimation;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState: EStartupShowingOperatorAnimation");
                    DoStartupShowOperatorAnimationL();
                    break;
                case EStartupFirstBootAndRTCCheck:
                    iInternalState = EStartupFirstBootAndRTCCheck;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState : EStartupFirstBootAndRTCCheck");
                    DoStartupFirstBootAndRTCCheckL();
                    break;
                case EStartupSystemFatalError:
                    SystemFatalErrorL();
                    break;
                default:
                    __ASSERT_DEBUG( EFalse,
                        PANIC( EStartupInvalidInternalStateChange ) );
                    break;
                }
            }
            break;
        case EStartupShowingOperatorAnimation:
            {
            switch( toState )
                {
                
                 
                case EStartupFirstBootAndRTCCheck:
                    iInternalState = EStartupFirstBootAndRTCCheck;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState : EStartupFirstBootAndRTCCheck");
                    DoStartupFirstBootAndRTCCheckL();
                    break;
                case EStartupSystemFatalError:
                    SystemFatalErrorL();
                    break;
                default:
                    __ASSERT_DEBUG( EFalse,
                        PANIC( EStartupInvalidInternalStateChange ) );
                    break;
                }
            }
            break;
      
        case EStartupFirstBootAndRTCCheck:
            {
            switch( toState )
                {
                case EStartupStartupOK:
                        iInternalState = EStartupStartupOK;
                        TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState : EStartupInvokeAndWaitForFtu");
                        DoStartupEndPart();
                    break;
                case EStartupSystemFatalError:
                    SystemFatalErrorL();
                    break;
                default:
                    __ASSERT_DEBUG( EFalse,
                        PANIC( EStartupInvalidInternalStateChange ) );
                    break;
                }
            }
            break;
        case EStartupSystemFatalError:
            {
            switch( toState )
                {
                case EStartupStartingUp:
                case EStartupOfflineModeQuery:
                case EStartupShowingWelcomeAnimation:
                case EStartupShowingOperatorAnimation:
                case EStartupFirstBootAndRTCCheck:
                case EStartupWaitingCUIStartupReady:
                case EStartupStartupOK:
                case EStartupSystemFatalError:
                    //no need to continue, because state is
                    //already in EStartupSystemFatalError
                    //so stop the execution of the other branch of
                    //the startup application. The other
                    //is displayin the "Selftest failed. Contact service"-dialog.
                    TRACES1("CStartupAppUi::DoNextStartupPhaseL(): InternalState : from EStartupSystemFatalError to %d -> so stop here.", toState );
                    break;
                default:
                    __ASSERT_DEBUG( EFalse,
                        PANIC( EStartupInvalidInternalStateChange ) );
                    break;
                }
            }
            break;
        default:
            __ASSERT_DEBUG( EFalse, PANIC( EStartupInvalidInternalStateChange ) );
            break;
        }
    }


// ---------------------------------------------------------------------------
// CStartupAppUi::SystemFatalErrorL()
// Call this only from DoNextStartupPhaseL()
// ---------------------------------------------------------------------------
void CStartupAppUi::SystemFatalErrorL()
    {
    TRACES("CStartupAppUi::SystemFatalErrorL()");

    iInternalState = EStartupSystemFatalError;

    TRACES("CStartupAppUi::SystemFatalErrorL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::SimStatusChangedReset()
// ---------------------------------------------------------------------------
TBool CStartupAppUi::SimStatusChangedReset()
    {
    TRACES( "CStartupAppUi::SimStatusChangedReset()" );
    TBool ret_val( EFalse );
    if( !iFirstBoot )
        {
        TInt startupReason( 0 );
        RProperty::Get( KPSUidStartup, KPSStartupReason, startupReason );
        ret_val = ( startupReason == ESIMStatusChangeReset );
        }
    TRACES1( "CStartupAppUi::SimStatusChangedReset(): End, return %d", ret_val );
    return ret_val;
    }


// ---------------------------------------------------------------------------
// CStartupAppUi::UpdateStartupUiPhase()
// ---------------------------------------------------------------------------
void CStartupAppUi::UpdateStartupUiPhase( TInt aValue )
    {
    TRACES1("CStartupAppUi::UpdateStartupUiPhase(): aValue=%d", aValue);
    
    TInt err = RProperty::Set( KPSUidStartup, KPSStartupUiPhase, aValue );
        
    if( KErrNone != err )
        {
        TRACES1("CStartupAppUi::UpdateStartupUiPhase(): KPSStartupUiPhase set err %d", err);
        }                          
    }

// End of file
