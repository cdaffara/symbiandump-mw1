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
* Description:  Implementation of CStartupPubSubObserver class.
*
*/


// SYSTEM INCLUDES
#include "startupappinternalpskeys.h"
#include <featmgr.h>

// USER INCLUDES
#include "StartupPubSubObserver.h"
#include "StartupSubscriber.h"
#include "startupappprivatepskeys.h"

// ================= MEMBER FUNCTIONS =======================

// ----------------------------------------------------
// StartupPubSubObserver::StartupPubSubObserver( CStartupAppUi* aStartupAppUi )
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------
CStartupPubSubObserver::CStartupPubSubObserver( CStartupAppUi* aStartupAppUi ) :
    iStartupAppUi( aStartupAppUi )
#if defined (RD_SCALABLE_UI_V2) && !defined(RD_STARTUP_ANIMATION_CUSTOMIZATION)
    , iTouchScreenCalibSupport( EFalse )
#endif // RD_SCALABLE_UI_V2 && !RD_STARTUP_ANIMATION_CUSTOMIZATION
    {
    }

// ----------------------------------------------------
// CStartupPubSubObserver::ConstructL()
// ----------------------------------------------------
void CStartupPubSubObserver::ConstructL()
    {
    TRACES("CStartupPubSubObserver::ConstructL()");

    iGlobalSystemStateSubscriber = CStartupSubscriber::NewL( *this,
                                                             KPSUidStartup,
                                                             KPSGlobalSystemState );
    iGlobalSystemStateSubscriber->Subscribe();

    TInt globalSystemState;
    User::LeaveIfError( RProperty::Get (
        KPSUidStartup, KPSGlobalSystemState, globalSystemState ) );

    TRACES1("CStartupPubSubObserver::ConstructL(): globalSystemState: %d",globalSystemState );

    // If critical phase in Starter has ended
    if ( globalSystemState == ESwStateNormalRfOn ||
         globalSystemState == ESwStateNormalRfOff ||
         globalSystemState == ESwStateNormalBTSap ||
         globalSystemState == ESwStateCriticalPhaseOK ||
         globalSystemState == ESwStateEmergencyCallsOnly )
        {
        iStartupAppUi->SetCriticalBlockEndedL();
        }

    else if ( globalSystemState == ESwStateFatalStartupError )
        {
        iStartupAppUi->SwStateFatalStartupErrorL( EFalse ); // property changed = EFalse
        }

    iStartupPhaseSubscriber = CStartupSubscriber::NewL( *this,
                                                        KPSUidStartupApp,
                                                        KPSStartupAppState );
    iStartupPhaseSubscriber->Subscribe();

    TInt startupPhase;
    User::LeaveIfError( RProperty::Get (
        KPSUidStartupApp, KPSStartupAppState, startupPhase ) );

    TRACES1("CStartupPubSubObserver::ConstructL(): startupPhase: %d", startupPhase );

    if ( startupPhase == EStartupAppStateStartAnimations )
        {
        iStartupAppUi->SetWaitingStartupAnimationStartL();
        }

#if defined (RD_SCALABLE_UI_V2) && !defined(RD_STARTUP_ANIMATION_CUSTOMIZATION)
    FeatureManager::InitializeLibL();
    if ( FeatureManager::FeatureSupported(KFeatureIdPenSupport) &&
         FeatureManager::FeatureSupported(KFeatureIdPenSupportCalibration) )
        {
        iTouchScreenCalibSupport = ETrue;

        iTouchScreenCalibSyncSubscriber = CStartupSubscriber::NewL( *this,
                                                         KPSUidStartup,
                                                         KPSTouchScreenCalibration );
        iTouchScreenCalibSyncSubscriber->Subscribe();

        TInt touchScreenCalibrationState;
        RProperty::Get( KPSUidStartup, KPSTouchScreenCalibration, touchScreenCalibrationState );
        if ( touchScreenCalibrationState == ETouchScreenCalibrationOk )
            {
            iStartupAppUi->TouchScreenCalibrationDoneL();
            }
        }
    FeatureManager::UnInitializeLib();
#endif // RD_SCALABLE_UI_V2 && !RD_STARTUP_ANIMATION_CUSTOMIZATION

    TRACES("CStartupPubSubObserver::ConstructL(): End");
    }

// ----------------------------------------------------------------------------
// CStartupPubSubObserver::HandlePropertyChangedL()
// ----------------------------------------------------------------------------
void CStartupPubSubObserver::HandlePropertyChangedL( const TUid& /*aCategory*/, TUint aKey )
    {
    TRACES("CStartupPubSubObserver::HandlePropertyChangedL()");
    TRACES1("CStartupPubSubObserver::HandlePropertyChangedL(): aKey: %d",aKey );

    TInt eventState( 0 );

    switch ( aKey )
        {
        case KPSGlobalSystemState:
            {
            TRACES("CStartupPubSubObserver::HandlePropertyChangedL(): key == KPSGlobalSystemState");
            User::LeaveIfError( RProperty::Get ( KPSUidStartup,
                                                 KPSGlobalSystemState,
                                                 eventState ) );
            if( eventState == ESwStateCriticalPhaseOK ||
            		eventState == ESwStateNormalRfOn ||
            		eventState == ESwStateNormalRfOff ||
            		eventState == ESwStateNormalBTSap )
                {
                TRACES("CStartupPubSubObserver::HandlePropertyChangedL(): eventState == ESwStateCriticalPhaseOK");
                iStartupAppUi->SetCriticalBlockEndedL();
                }
            else if( eventState == ESwStateEmergencyCallsOnly )
                {
                TRACES("CStartupPubSubObserver::HandlePropertyChangedL(): eventState == ESwStateEmergencyCallsOnly");
                iStartupAppUi->SetEmergencyCallsOnlyL();
                }
            else if (eventState == ESwStateFatalStartupError )
                {
                TRACES("CStartupPubSubObserver::HandlePropertyChangedL(): eventState == ESwStateFatalStartupError");
                iStartupAppUi->SwStateFatalStartupErrorL( ETrue ); // property changed = ETrue
                }
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
            else if ( eventState == ESwStateChargingToNormal ||
                      eventState == ESwStateAlarmToNormal )
                {
                TRACES("CStartupPubSubObserver::HandlePropertyChangedL(): eventState == ESwStateAlarm/ChargingToNormal");
                iStartupAppUi->TryPreLoadAnimation();
                }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
            }
            break;
        case KPSStartupAppState:
            {
            TRACES("CStartupPubSubObserver::HandlePropertyChangedL(): key == KPSStartupAppState");
            User::LeaveIfError( RProperty::Get ( KPSUidStartupApp,
                                                 KPSStartupAppState,
                                                 eventState ) );
            
            TRACES1("CStartupPubSubObserver::HandlePropertyChangedL(): eventState = %d", eventState);
            
            if ( eventState == EStartupAppStateStartAnimations )
                {                
                iStartupAppUi->SetWaitingStartupAnimationStartL();
                }            
            }
            break;
#if defined (RD_SCALABLE_UI_V2) && !defined(RD_STARTUP_ANIMATION_CUSTOMIZATION)
        case KPSTouchScreenCalibration:
            {
            TInt state;

            RProperty::Get( KPSUidStartup, KPSTouchScreenCalibration, state );
            if ( state == ETouchScreenCalibrationOk )
                {
                iStartupAppUi->TouchScreenCalibrationDoneL();
                }
            }
            break;
#endif // RD_SCALABLE_UI_V2 && !RD_STARTUP_ANIMATION_CUSTOMIZATION
        default:
            TRACES("CStartupPubSubObserver::HandlePropertyChangedL(): key is unsupported");
            break;
        }//End of switch-clause

    TRACES("CStartupPubSubObserver::HandlePropertyChangedL(): End");
    }

// ----------------------------------------------------
// CStartupPubSubObserver::NewL( CStartupAppUi* aStartupAppUi )
// ----------------------------------------------------
CStartupPubSubObserver* CStartupPubSubObserver::NewL( CStartupAppUi* aStartupAppUi )
    {
    TRACES("CStartupPubSubObserver::NewL()");
    CStartupPubSubObserver* self = new (ELeave) CStartupPubSubObserver( aStartupAppUi );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self

    TRACES("CStartupPubSubObserver::NewL(): End");
    return self;
    }

// ----------------------------------------------------
// CStartupPubSubObserver::~CStartupPubSubObserver()
// ----------------------------------------------------
CStartupPubSubObserver::~CStartupPubSubObserver()
    {
    TRACES("CStartupPubSubObserver::~CStartupPubSubObserver()");

    delete iGlobalSystemStateSubscriber;
    delete iStartupPhaseSubscriber;
#if defined (RD_SCALABLE_UI_V2) && !defined(RD_STARTUP_ANIMATION_CUSTOMIZATION)
    if (iTouchScreenCalibSupport)
        {
        delete iTouchScreenCalibSyncSubscriber;
        }
#endif // RD_SCALABLE_UI_V2 && !RD_STARTUP_ANIMATION_CUSTOMIZATION

    TRACES("CStartupPubSubObserver::~CStartupPubSubObserver(): End");
    }
//  End of File
