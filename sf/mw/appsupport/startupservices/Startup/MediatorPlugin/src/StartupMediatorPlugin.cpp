/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Source file for Mediator Test plugin
*
*/


// SYSTEM INCLUDES
#include <e32def.h>
#include <w32std.h>
#include <MediatorDomainUIDs.h>
#include <featmgr.h>
#include <centralrepository.h>
#include <CoreApplicationUIsSDKCRKeys.h>
#include <avkondomainpskeys.h>
#include <coreapplicationuisdomainpskeys.h>


// USER INCLUDES
#include "StartupMediatorPlugin.h"
#include "StartupMediatorPluginTrace.h"
#include "StartupMediatorPluginPSObserver.h"
#include "startupdomainpskeys.h"

// CONSTANTS
const TInt KMediatorTimeout = 1000000;

// -----------------------------------------------------------------------------
// CUPnPAVControlImplementation::NewL
// Creates an instance of the implementation.
// -----------------------------------------------------------------------------
CStartupMediatorPlugin* CStartupMediatorPlugin::NewL()
    {
    TRACES("CStartupMediatorPlugin::NewL()");
    CStartupMediatorPlugin* self = new (ELeave) CStartupMediatorPlugin;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CStartupMediatorPlugin::CStartupMediatorPlugin
// First phase construction.
// -----------------------------------------------------------------------------
CStartupMediatorPlugin::CStartupMediatorPlugin()
    {
    TRACES("CStartupMediatorPlugin::CStartupMediatorPlugin()");
    // Nothing to do here
    }

// -----------------------------------------------------------------------------
// CStartupMediatorPlugin::ConstructL
// Second phase construction.
// -----------------------------------------------------------------------------
void CStartupMediatorPlugin::ConstructL()
    {
    TRACES("CStartupMediatorPlugin::ConstructL()");

    TRACES("CStartupMediatorPlugin::ConstructL(): End");
    }

// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
CStartupMediatorPlugin::~CStartupMediatorPlugin()
    {
    TRACES("CStartupMediatorPlugin::~CStartupMediatorPlugin()");
    delete iStartupMediatorPluginPSObserver;
    delete iCommandResponder;
    delete iEventProvider;
    TRACES("CStartupMediatorPlugin::~CStartupMediatorPlugin(): End");
    }

// -----------------------------------------------------------------------------
// CStartupMediatorPlugin::StartL()
// -----------------------------------------------------------------------------
void CStartupMediatorPlugin::StartL()
    {
    TRACES("CStartupMediatorPlugin::StartL()");

    iStartupMediatorPluginPSObserver = CStartupMediatorPluginPSObserver::NewL( this );

    TRACES("CStartupMediatorPlugin::StartL(): iCommandResponder");
    iCommandResponder = CMediatorCommandResponder::NewL(this);

    TRACES("CStartupMediatorPlugin::StartL(): iEventProvider");
    iEventProvider = CMediatorEventProvider::NewL();

    TRACES("CStartupMediatorPlugin::StartL(): Register event: EMsgAutolockEvent ");
    iEventProvider->RegisterEvent( KMediatorSecondaryDisplayDomain,
                                   SecondaryDisplay::KCatSystemState,
                                   SecondaryDisplay::EMsgAutolockEvent,
                                   TVersion(0,0,0),
                                   ECapabilitySwEvent);

    TRACES("CStartupMediatorPlugin::StartL(): Register event: EMsgSystemStateChange ");
    iEventProvider->RegisterEvent( KMediatorSecondaryDisplayDomain,
                                   SecondaryDisplay::KCatSystemState,
                                   SecondaryDisplay::EMsgSystemStateChange,
                                   TVersion(0,0,0),
                                   ECapabilitySwEvent);
                                   
    TRACES("CStartupMediatorPlugin::StartL(): Register event: EMsgKeyGuardEvent ");
    iEventProvider->RegisterEvent( KMediatorSecondaryDisplayDomain,
                                   SecondaryDisplay::KCatSystemState,
                                   SecondaryDisplay::EMsgKeyGuardEvent,
                                   TVersion(0,0,0),
                                   ECapabilitySwEvent);                                   

    TRACES("CStartupMediatorPlugin::StartL(): Register command: ECmdGetSystemState ");
    iCommandResponder->RegisterCommand(KMediatorSecondaryDisplayDomain, 
                                       SecondaryDisplay::KCatSystemState, 
                                       SecondaryDisplay::ECmdGetSystemState,
                                       TVersion(0,0,0),
                                       ECapabilitySwEvent,
                                       KMediatorTimeout);

    TRACES("CStartupMediatorPlugin::StartL(): Register command: ECmdGetAutolockStatus ");
    iCommandResponder->RegisterCommand(KMediatorSecondaryDisplayDomain, 
                                       SecondaryDisplay::KCatSystemState, 
                                       SecondaryDisplay::ECmdGetAutolockStatus,
                                       TVersion(0,0,0),
                                       ECapabilitySwEvent,
                                       KMediatorTimeout);

    TRACES("CStartupMediatorPlugin::StartL(): Register command: ECmdGetKeyGuardStatus ");
    iCommandResponder->RegisterCommand(KMediatorSecondaryDisplayDomain, 
                                       SecondaryDisplay::KCatSystemState, 
                                       SecondaryDisplay::ECmdGetKeyGuardStatus,
                                       TVersion(0,0,0),
                                       ECapabilitySwEvent,
                                       KMediatorTimeout);

    TRACES("CStartupMediatorPlugin::StartL(): End");
    }

// -----------------------------------------------------------------------------
// CStartupMediatorPlugin::RaiseEvent()
// -----------------------------------------------------------------------------
void CStartupMediatorPlugin::RaiseEvent( TUid aDomain,
                                         TUid aCategory,
                                         TInt aEventId,
                                         TVersion aVersion,
                                         const TDesC8& aData )
    {
    TRACES("CStartupMediatorPlugin::RaiseEvent()");

    iEventProvider->RaiseEvent( aDomain, 
                                aCategory,
                                aEventId,
                                aVersion,
                                aData );

    TRACES("CStartupMediatorPlugin::RaiseEvent(): End");
    }

// ---------------------------------------------------------
// CStartupMediatorPlugin::MediatorCommandL()
// ---------------------------------------------------------
void CStartupMediatorPlugin::MediatorCommandL( TUid aDomain,
                                               TUid aCategory,
                                               TInt aCommandId,
                                               TVersion /*aVersion*/,
                                               const TDesC8& /*aData*/ )
    {
    TRACES("CStartupMediatorPlugin::MediatorCommandL()");

    if ( aCategory == SecondaryDisplay::KCatSystemState )
        {
        switch( aCommandId )
            {
            case SecondaryDisplay::ECmdGetSystemState:
                {
                TRACES("CStartupMediatorPlugin::MediatorCommandL(): ECmdGetSystemState");

                TPckgBuf<TInt> data( GetSystemStateL() );
                iCommandResponder->IssueResponse( aDomain,
                                                  aCategory, 
                                                  aCommandId,
                                                  KErrNone,
                                                  data );
                break;
                }
            case SecondaryDisplay::ECmdGetAutolockStatus:
                {
                TRACES("CStartupMediatorPlugin::MediatorCommandL(): ECmdGetAutolockStatus");

                TPckgBuf<TInt> data( GetAutolockStateL() );
                iCommandResponder->IssueResponse( aDomain,
                                                  aCategory, 
                                                  aCommandId,
                                                  KErrNone,
                                                  data );
                break;
                }
            case SecondaryDisplay::ECmdGetKeyGuardStatus:
                {
                TRACES("CStartupMediatorPlugin::MediatorCommandL(): ECmdGetKeyGuardStatus");

                TPckgBuf<TInt> data( GetKeyGuardStateL() );
                iCommandResponder->IssueResponse( aDomain,
                                                  aCategory, 
                                                  aCommandId,
                                                  KErrNone,
                                                  data );
                break;
                }
            default:
                TRACES("CStartupMediatorPlugin::MediatorCommandL(): Command not supported");
                break;
            }
        }

    TRACES("CStartupMediatorPlugin::MediatorCommandL(): End");
    }

// ---------------------------------------------------------
// CStartupMediatorPlugin::CancelMediatorCommand()
// ---------------------------------------------------------
void CStartupMediatorPlugin::CancelMediatorCommand( TUid /*aDomain*/,
                                                    TUid /*aCategory*/,
                                                    TInt /*aCommandId*/ )
    {
    TRACES("CStartupMediatorPlugin::CancelMediatorCommand()");
    }

// ---------------------------------------------------------
// CStartupMediatorPlugin::GetSystemStateL()
// ---------------------------------------------------------
TInt CStartupMediatorPlugin::GetSystemStateL()
    {
    TRACES("CStartupMediatorPlugin::GetSystemStateL()");
    TInt state(SecondaryDisplay::ESystemStateNormal);

    TInt startupMode;

    RProperty::Get ( KPSUidStartup, KPSGlobalStartupMode, startupMode );

    switch (startupMode)
        {
        case EStartupModeCharging:
            TRACES("CStartupMediatorPlugin::GetSystemStateL(): EStartupModeCharging");
            state = SecondaryDisplay::ESystemStateCharging;
            break;
        case EStartupModeAlarm:
            TRACES("CStartupMediatorPlugin::GetSystemStateL(): EStartupModeAlarm");
            state = SecondaryDisplay::ESystemStateAlarm;
            break;
        case EStartupModeNormal:
            TRACES("CStartupMediatorPlugin::GetSystemStateL(): EStartupModeNormal");
            if (DosInOfflineModeL())
                {
                state = SecondaryDisplay::ESystemStateOffline;
                }
            else
                {
                state = SecondaryDisplay::ESystemStateNormal;
                }
            break;
        case EStartupModeTest:
            TRACES("CStartupMediatorPlugin::GetSystemStateL(): EStartupModeTest");
            state = SecondaryDisplay::ESystemStateTest;
            break;
        default:
            TRACES("CStartupMediatorPlugin::GetSystemStateL(): default");
            state = SecondaryDisplay::ESystemStateNormal;
            break;
        }


    TRACES1("CStartupMediatorPlugin::GetSystemStateL(): returns %d", state);
    return state;
    }

// ----------------------------------------------------------------------------
// CStartupMediatorPlugin::DosInOfflineModeL()
// ----------------------------------------------------------------------------
TBool CStartupMediatorPlugin::DosInOfflineModeL()
    {
    TRACES("CStartupMediatorPlugin::DosInOfflineModeL()");

    FeatureManager::InitializeLibL();
    if ( FeatureManager::FeatureSupported( KFeatureIdOfflineMode ) )
        {
        TRACES("CStartupMediatorPlugin::DosInOfflineModeL(): Offline mode supported");
        FeatureManager::UnInitializeLib();
        return UiInOfflineMode();
        }
    else
        {
        TRACES("CStartupMediatorPlugin::DosInOfflineModeL(): return EFalse");
        FeatureManager::UnInitializeLib();
        return EFalse; // device that does not support Off-line Mode can never be in Off-line Mode.
        }
    }


// ----------------------------------------------------------------------------
// CStartupMediatorPlugin::UiInOfflineMode()
// ----------------------------------------------------------------------------
TBool CStartupMediatorPlugin::UiInOfflineMode()
    {
    TRACES("CStartupMediatorPlugin::UiInOfflineMode()");

    TInt networkConnectionAllowed ( 0 );

    TRACES("CStartupMediatorPlugin::UiInOfflineMode(): Check Offline status from CenRep");
    CRepository* repository(NULL);

    TRAPD( err, repository = CRepository::NewL( KCRUidCoreApplicationUIs ) );
    if ( err == KErrNone )
        {
        err = repository->Get( KCoreAppUIsNetworkConnectionAllowed, networkConnectionAllowed );
        }

    #ifdef _DEBUG
        if ( err != KErrNone )
            TRACES1("CStartupMediatorPlugin::UiInOfflineMode(): Central repository access failed, error code %d.", err );
        else
            TRACES1("CStartupMediatorPlugin::UiInOfflineMode(): Offline: network connection allowed: %d", networkConnectionAllowed);
    #endif
    delete repository;

    TRACES1("CStartupMediatorPlugin::UiInOfflineMode():  returns %d", !( TBool ) networkConnectionAllowed);

    return ( ! ( TBool ) networkConnectionAllowed );
    }

// ---------------------------------------------------------
// CStartupMediatorPlugin::GetAutolockStateL()
// ---------------------------------------------------------
TInt CStartupMediatorPlugin::GetAutolockStateL()
    {
    TRACES("CStartupMediatorPlugin::GetAutolockStateL()");
    TInt state(0);

    RProperty::Get(KPSUidCoreApplicationUIs, KCoreAppUIsAutolockStatus, state);

    if(state > EAutolockOff)
        state = SecondaryDisplay::EAutolockOn;
    else
        state = SecondaryDisplay::EAutolockOff;

    TRACES1("CStartupMediatorPlugin::GetAutolockStateL(): returns %d", state);
    return state;
    }

// ---------------------------------------------------------
// CStartupMediatorPlugin::GetKeyGuardStateL()
// ---------------------------------------------------------
TInt CStartupMediatorPlugin::GetKeyGuardStateL()
    {
    TRACES("CStartupMediatorPlugin::GetKeyGuardStateL()");
    TInt state(0);

    RProperty::Get(KPSUidAvkonDomain, KAknKeyguardStatus, state);
    
    if(state == EKeyguardLocked)
        state = SecondaryDisplay::EKeyGuardOn;
    else
        state = SecondaryDisplay::EKeyGuardOff;

    TRACES1("CStartupMediatorPlugin::GetKeyGuardStateL() returns %d", state);
    return state;
    }

// End of File
