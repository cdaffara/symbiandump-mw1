/*
 * Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * CSysApCenRepOfflineModeObserver is a an observer for the Offline Mode central Repository key.
 * This class handles the changes to the offline mode changes done from Power Menu or Control Panel 
 *
 */

#include "sysapcenrepofflinemodeobserver.h"
#include "CoreApplicationUIsSDKCRKeys.h"
#include "SysAp.hrh"

#ifdef _DEBUG
#include <e32debug.h>
#endif

// ----------------------------------------------------------------------------
// CSysApCenRepOfflineModeObserver::NewL( MSysApOfflineModeController& )
// Creates an instance of CSysApCenRepOfflineModeObserver and returns
// ----------------------------------------------------------------------------

CSysApCenRepOfflineModeObserver* CSysApCenRepOfflineModeObserver::NewL( 
                    MSysApOfflineModeController& aSysApOfflineModeController )
    {
    TRACES( RDebug::Printf("CSysApCenRepOfflineModeObserver::NewL") );
    CSysApCenRepOfflineModeObserver* self = 
    new (ELeave) CSysApCenRepOfflineModeObserver(aSysApOfflineModeController);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }


// ----------------------------------------------------------------------------
// CSysApCenRepOfflineModeObserver::CSysApCenRepOfflineModeObserver( MSysApOfflineModeController& )
// ----------------------------------------------------------------------------

CSysApCenRepOfflineModeObserver::CSysApCenRepOfflineModeObserver( 
                MSysApOfflineModeController& aSysApOfflineModeController  )
            : iSysApOfflineModeController( aSysApOfflineModeController )
    {
    TRACES( RDebug::Printf("CSysApCenRepOfflineModeObserver::CSysApCenRepOfflineModeObserver" ) );
    }

// ----------------------------------------------------------------------------
// CSysApCenRepOfflineModeObserver::~CSysApCenRepOfflineModeObserver()
// ----------------------------------------------------------------------------

CSysApCenRepOfflineModeObserver::~CSysApCenRepOfflineModeObserver()
    {
    TRACES( RDebug::Printf("~CSysApCenRepOfflineModeObserver" ) );
    if( iOfflineModeHandler )
    {
    iOfflineModeHandler->StopListening();
    }
    delete iOfflineModeHandler;
    delete iOfflineCenRep;
    }

// ----------------------------------------------------------------------------
// CSysApCenRepOfflineModeObserver::ConstructL()
// ----------------------------------------------------------------------------

void CSysApCenRepOfflineModeObserver::ConstructL()
    {
    TRACES(RDebug::Printf("CSysApCenRepOfflineModeObserver::ConstructL: trying CRepository::NewL(KCRUidCoreApplicationUIs)"));
    
    iOfflineCenRep = CRepository::NewL( KCRUidCoreApplicationUIs );
    
    iOfflineModeHandler =
    CCenRepNotifyHandler::NewL( *this,
    *iOfflineCenRep,
    CCenRepNotifyHandler::EIntKey,
    KCoreAppUIsNetworkConnectionAllowed );
    iOfflineModeHandler->StartListeningL();    
    }

// ----------------------------------------------------------------------------
// CSysApCenRepOfflineModeObserver::HandleNotifyInt()
// Handles the changes to offline mode state
// ----------------------------------------------------------------------------

void CSysApCenRepOfflineModeObserver::HandleNotifyInt( TUint32 aId, TInt aNewValue  )
    {
    TRACES(RDebug::Printf("CSysApCenRepOfflineModeObserver::HandleNotifyInt(), aId=0x%x, aNewValue=%d", aId, aNewValue));    
    if ( aId == KCoreAppUIsNetworkConnectionAllowed )
    {
        if( aNewValue == ECoreAppUIsNetworkConnectionNotAllowed )
        {
        // Offline mode enabled. Go Offline
        TRACES( RDebug::Printf("CSysApCenRepOfflineModeObserver::HandleNotifyInt() Offline mode Enabled"));
        // Ignore any leave because it will have already been handled by showing an 
        // error notification to the user
        TRAP_IGNORE(iSysApOfflineModeController.SwitchFromOnlineToOfflineModeL());
        }
    else if( aNewValue == ECoreAppUIsNetworkConnectionAllowed )
    {
        // Offline mode disabled. Go Online
        TRACES( RDebug::Printf("CSysApCenRepOfflineModeObserver::HandleNotifyInt() Offline mode Disabled"));
        // Ignore any leave because it will have already been handled by showing an 
        // error notification to the user
        TRAP_IGNORE(iSysApOfflineModeController.GoOnlineIfOkL());
    }
    else
    {		
        // Any other value is invalid	
        TRACES(RDebug::Printf("CSysApCenRepOfflineModeObserver::HandleNotifyInt() Invalid Value %d", aNewValue));
    }
    }
    }

// ----------------------------------------------------------------------------
// CSysApCenRepOfflineModeObserver::HandleNotifyError()
// ----------------------------------------------------------------------------

void CSysApCenRepOfflineModeObserver::HandleNotifyError( TUint32, TInt, CCenRepNotifyHandler* )
    {
    TRACES(RDebug::Printf("CSysApCenRepOfflineModeObserver::HandleNotifyError()"));
    // Nothing to do if start listening leaves
    TRAP_IGNORE(iOfflineModeHandler->StartListeningL());    
    }



// ----------------------------------------------------------------------------
// CSysApCenRepOfflineModeObserver::IsOffline()
// Fetches the Cen Rep for Offline mode and 
// Return true if Offline mode is enabled, false otherwise
// ----------------------------------------------------------------------------
TBool CSysApCenRepOfflineModeObserver::IsOffline() const
    {
    TRACES(RDebug::Printf("CSysApCenRepOfflineModeObserver::IsOffline"));
    TBool nwConnAllowed = ETrue;
    iOfflineCenRep->Get(KCoreAppUIsNetworkConnectionAllowed, nwConnAllowed);
    TRACES(RDebug::Printf("CSysApCenRepOfflineModeObserver::IsOffline returns %d",!nwConnAllowed));
    return !nwConnAllowed;
    }
