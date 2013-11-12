/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*           This class handles the received indications from the PubSub.
*
*/


// SYSTEM INCLUDES

// USER INCLUDES
#include "outofdiskmonitor.h"
#include "outofdisksubscriber.h"
#include "outofdiskpsobserver.h"
#include "OodTraces.h"

// ================= MEMBER FUNCTIONS =======================

// ----------------------------------------------------
// COutOfDiskPSObserver::COutOfDiskPSObserver( COutOfDiskMonitor* aOutOfDiskMonitor )
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------
COutOfDiskPSObserver::COutOfDiskPSObserver( COutOfDiskMonitor* aOutOfDiskMonitor ) :
    iOutOfDiskMonitor( aOutOfDiskMonitor )
    {
    }

// ----------------------------------------------------
// COutOfDiskPSObserver::ConstructL()
// ----------------------------------------------------
void COutOfDiskPSObserver::ConstructL()
    {
    TRACES("COutOfDiskPSObserver::ConstructL()");

    iUikGlobalNotesAllowedSubscriber = COutOfDiskSubscriber::NewL( *this, 
                                                                   KPSUidUikon, 
                                                                   KUikGlobalNotesAllowed );
    iUikGlobalNotesAllowedSubscriber->Subscribe();

    TInt state(0);
    RProperty::Get( KPSUidUikon, KUikGlobalNotesAllowed, state );
    iOutOfDiskMonitor->SetGlobalNotesAllowed(state);

    iAutolockStatusSubscriber = COutOfDiskSubscriber::NewL( *this, 
                                                            KPSUidCoreApplicationUIs, 
                                                            KCoreAppUIsAutolockStatus );
    iAutolockStatusSubscriber->Subscribe();
    RProperty::Get( KPSUidCoreApplicationUIs, KCoreAppUIsAutolockStatus, iAutolockStatus );
    
    TRACES("COutOfDiskPSObserver::ConstructL(): End");
    }

// ----------------------------------------------------------------------------
// COutOfDiskPSObserver::HandlePropertyChangedL()
// ----------------------------------------------------------------------------
void COutOfDiskPSObserver::HandlePropertyChangedL( const TUid& aCategory, TUint aKey )
    {
    TRACES("COutOfDiskPSObserver::HandlePropertyChangedL()+++++++++++++++++++++++++++++");
    TRACES1("COutOfDiskPSObserver::HandlePropertyChangedL(): aKey: %d",aKey );
    
    if ( aCategory == KPSUidUikon && aKey == KUikGlobalNotesAllowed )
        {
        TRACES("COutOfDiskPSObserver::HandlePropertyChangedL(): Key: KUikGlobalNotesAllowed");
        TInt state(0);
        RProperty::Get( KPSUidUikon, KUikGlobalNotesAllowed, state );
        iOutOfDiskMonitor->SetGlobalNotesAllowed(state);
        if (state)
            {
            TRACES("COutOfDiskPSObserver::HandlePropertyChangedL: Reset all displayed fields for all drives");
			iOutOfDiskMonitor->ResetDisplayedStatus(NULL, ETrue);
            iOutOfDiskMonitor->ShowGlobalQueryIfNeededL();
            }
        }
    else if ( aCategory == KPSUidCoreApplicationUIs && aKey == KCoreAppUIsAutolockStatus )
        {
        TRACES("COutOfDiskPSObserver::HandlePropertyChangedL(): Key: KCoreAppUIsAutolockStatus");
        TInt status(0);
        RProperty::Get( KPSUidCoreApplicationUIs, KCoreAppUIsAutolockStatus, status );
        if (status == EAutolockOff && iAutolockStatus > EAutolockOff)
            {
            TRACES("COutOfDiskPSObserver::HandlePropertyChangedL: Reset all displayed fields for all drives");
            iOutOfDiskMonitor->ResetDisplayedStatus(NULL, ETrue);
            iOutOfDiskMonitor->ShowGlobalQueryIfNeededL();
            }
        iAutolockStatus = status;
        }
    TRACES("COutOfDiskPSObserver::HandlePropertyChangedL(): End ------------------------------ ");
    }

// ----------------------------------------------------
// COutOfDiskPSObserver::NewL( COutOfDiskMonitor** aOutOfDiskMonitor )
// ----------------------------------------------------
COutOfDiskPSObserver* COutOfDiskPSObserver::NewL( COutOfDiskMonitor* aOutOfDiskMonitor )
    {
    TRACES("COutOfDiskPSObserver::NewL()");
    COutOfDiskPSObserver* self = new (ELeave) COutOfDiskPSObserver( aOutOfDiskMonitor );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    TRACES("COutOfDiskPSObserver::NewL(): End");
    return self;
    }

// ----------------------------------------------------
// COutOfDiskPSObserver::~COutOfDiskPSObserver()
// ----------------------------------------------------
COutOfDiskPSObserver::~COutOfDiskPSObserver()
    {
    TRACES("COutOfDiskPSObserver::~COutOfDiskPSObserver()");
    delete iAutolockStatusSubscriber;
    delete iUikGlobalNotesAllowedSubscriber;
    TRACES("COutOfDiskPSObserver::~COutOfDiskPSObserver(): End");
    }

//  End of File
