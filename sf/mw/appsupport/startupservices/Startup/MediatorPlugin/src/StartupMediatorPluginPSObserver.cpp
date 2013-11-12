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
* Description: 
*           This class the handles the received indications from the System
*           Agent. Indications are SimStatus, SecurityCodeStatus and
*           MasterCodeStatus.
*
*/


// SYSTEM INCLUDES
#include <MediatorDomainUIDs.h>
#include <startupdomainpskeys.h>
#include <SecondaryDisplay/SecondaryDisplaySystemStateAPI.h>
#include <avkondomainpskeys.h>
#include <coreapplicationuisdomainpskeys.h>


// USER INCLUDES
#include "StartupMediatorPlugin.h"
#include "StartupMediatorPluginSubscriber.h"
#include "StartupMediatorPluginTrace.h"
#include "StartupMediatorPluginPSObserver.h"


// ================= MEMBER FUNCTIONS =======================

// ----------------------------------------------------
// CStartupMediatorPluginPSObserver::CStartupMediatorPluginPSObserver( CStartupMediatorPlugin* aStartupMediatorPlugin )
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------
CStartupMediatorPluginPSObserver::CStartupMediatorPluginPSObserver( CStartupMediatorPlugin* aStartupMediatorPlugin ) :
    iStartupMediatorPlugin( aStartupMediatorPlugin )
    {
    }

// ----------------------------------------------------
// CStartupMediatorPluginPSObserver::ConstructL()
// ----------------------------------------------------
void CStartupMediatorPluginPSObserver::ConstructL()
    {
    TRACES("CStartupMediatorPluginPSObserver::ConstructL()");

    // -------- Autolock status -------------
    iAutoLockStatusSubscriber = CStartupMediatorPluginSubscriber::NewL( *this, 
                                                                        KPSUidCoreApplicationUIs, 
                                                                        KCoreAppUIsAutolockStatus );
    iAutoLockStatusSubscriber->Subscribe();


    // -------- Keyquard status -------------
    iKeyGuardStatusSubscriber = CStartupMediatorPluginSubscriber::NewL( *this, 
                                                                        KPSUidAvkonDomain, 
                                                                        KAknKeyguardStatus );
    iKeyGuardStatusSubscriber->Subscribe();


    // -------- Global system state ---------
    iGlobalSystemStateSubscriber = CStartupMediatorPluginSubscriber::NewL( *this, 
                                                                           KPSUidStartup, 
                                                                           KPSGlobalSystemState );
    iGlobalSystemStateSubscriber->Subscribe();



    TRACES("CStartupMediatorPluginPSObserver::ConstructL(): End");
    }

// ----------------------------------------------------------------------------
// CStartupMediatorPluginPSObserver::HandlePropertyChangedL()
// ----------------------------------------------------------------------------
void CStartupMediatorPluginPSObserver::HandlePropertyChangedL( const TUid& aCategory, TUint aKey )
    {
    TRACES("CStartupMediatorPluginPSObserver::HandlePropertyChangedL()");
    TRACES1("CStartupMediatorPluginPSObserver::HandlePropertyChangedL(): aKey: %d",aKey );

    // -------- Global system state ---------
    if ( aCategory == KPSUidStartup && aKey == KPSGlobalSystemState )
        {
        TRACES("CStartupMediatorPluginPSObserver::HandlePropertyChangedL(): KPSGlobalSystemState");
        TInt value;
        value = iStartupMediatorPlugin->GetSystemStateL();
        if ( value == KErrNone )
            {
            TPckgBuf<TInt> data( value );
            iStartupMediatorPlugin->RaiseEvent( KMediatorSecondaryDisplayDomain,
                                                SecondaryDisplay::KCatSystemState, 
                                                SecondaryDisplay::EMsgSystemStateChange,
                                                TVersion(0,0,0),
                                                data );    
            }
        }

    // -------- Autolock status -------------
    else if ( aCategory == KPSUidCoreApplicationUIs && aKey == KCoreAppUIsAutolockStatus )
        {
        TRACES("CStartupMediatorPluginPSObserver::HandlePropertyChangedL(): KPSUidAutolockStatusValue");
        TInt value;
        value = iStartupMediatorPlugin->GetAutolockStateL();
        TPckgBuf<TInt> data( value );
             
        iStartupMediatorPlugin->RaiseEvent( KMediatorSecondaryDisplayDomain,
                                            SecondaryDisplay::KCatSystemState, 
                                            SecondaryDisplay::EMsgAutolockEvent,
                                            TVersion(0,0,0),
                                            data );
        }
    
    // -------- Keyquard status -------------
    else if ( aCategory == KPSUidAvkonDomain && aKey == KAknKeyguardStatus )
        {
        TRACES("CStartupMediatorPluginPSObserver::HandlePropertyChangedL(): KAknKeyguardStatus");
        TInt value;
        value = iStartupMediatorPlugin->GetKeyGuardStateL();
        TPckgBuf<TInt> data( value );
             
        iStartupMediatorPlugin->RaiseEvent( KMediatorSecondaryDisplayDomain,
                                            SecondaryDisplay::KCatSystemState, 
                                            SecondaryDisplay::EMsgKeyGuardEvent,
                                            TVersion(0,0,0),
                                            data );
        }
    
    else
        {   
        TRACES("CStartupMediatorPluginPSObserver::HandlePropertyChangedL(): Key not supported");        
        }
    TRACES("CStartupMediatorPluginPSObserver::HandlePropertyChangedL(): End");
    }

// ----------------------------------------------------
// CStartupMediatorPluginPSObserver::NewL( CStartupAppUi* aStartupAppUi )
// ----------------------------------------------------
CStartupMediatorPluginPSObserver* CStartupMediatorPluginPSObserver::NewL( CStartupMediatorPlugin* aStartupMediatorPlugin )
    {
    TRACES("CStartupMediatorPluginPSObserver::NewL()");
    CStartupMediatorPluginPSObserver* self = new (ELeave) CStartupMediatorPluginPSObserver( aStartupMediatorPlugin );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self

    TRACES("CStartupMediatorPluginPSObserver::NewL(): End");
    return self;
    }

// ----------------------------------------------------
// CStartupMediatorPluginPSObserver::~CStartupMediatorPluginPSObserver()
// ----------------------------------------------------
CStartupMediatorPluginPSObserver::~CStartupMediatorPluginPSObserver()
    {
    TRACES("CStartupMediatorPluginPSObserver::~CStartupMediatorPluginPSObserver()");

    delete iAutoLockStatusSubscriber;
    delete iKeyGuardStatusSubscriber;
    delete iGlobalSystemStateSubscriber;
    iProperty.Close();

    TRACES("CStartupMediatorPluginPSObserver::~CStartupMediatorPluginPSObserver(): End");
    }

//  End of File
