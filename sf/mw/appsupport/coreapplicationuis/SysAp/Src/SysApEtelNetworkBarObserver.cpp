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
* Description:  CSysApEtelNetworkBarObserver implementation
*
*/


// INCLUDES
#include "SysApEtelNetworkBarObserver.h"
#include "SysApAppUi.h"

// Constants 
const TInt8 KNetworkBarsUninitialized(-111);

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApEtelNetworkBarObserver::NewL() 
// ----------------------------------------------------------------------------
CSysApEtelNetworkBarObserver* CSysApEtelNetworkBarObserver::NewL( CSysApAppUi& aSysApAppUi, RMobilePhone& aPhone )
    {
    CSysApEtelNetworkBarObserver* self = new(ELeave) CSysApEtelNetworkBarObserver( aSysApAppUi, aPhone );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApEtelNetworkBarObserver::CSysApEtelNetworkBarObserver() 
// ----------------------------------------------------------------------------
CSysApEtelNetworkBarObserver::CSysApEtelNetworkBarObserver( CSysApAppUi& aSysApAppUi, RMobilePhone& aPhone ) 
    : CActive(EPriorityStandard), 
    iSysApAppUi( aSysApAppUi ),
    iPhone( aPhone ),
    iSignalBars( KNetworkBarsUninitialized ),
    iPreviousNetworkBarValue( KNetworkBarsUninitialized )
    {
    CActiveScheduler::Add(this);
    }

// ----------------------------------------------------------------------------
// CSysApEtelNetworkBarObserver::ConstructL() 
// ----------------------------------------------------------------------------
void CSysApEtelNetworkBarObserver::ConstructL()
    {    
    // Get initial value (will also reorder in RunL)
    TRACES( RDebug::Print(_L("CSysApEtelNetworkBarObserver::ConstructL - Ordering signal strength change." ) ) );
    OrderNotification();
    iNetworkBarGetter = CSysApEtelNetworkBarGetter::NewL( *this, iPhone );
    iNetworkBarGetter->GetBarCountL();
    }

// ----------------------------------------------------------------------------
// CSysApEtelNetworkBarObserver::~CSysApEtelNetworkBarObserver() 
// ----------------------------------------------------------------------------

CSysApEtelNetworkBarObserver::~CSysApEtelNetworkBarObserver()
    {
    TRACES( RDebug::Print( _L("CSysApEtelNetworkBarObserver::~CSysApEtelNetworkBarObserver" ) ) );
    delete iNetworkBarGetter;
    Cancel();
    }


// -----------------------------------------------------------------------------
// CSysApEtelNetworkBarObserver::RunL
// Handle notification from pubsub session.
// -----------------------------------------------------------------------------
//
void CSysApEtelNetworkBarObserver::RunL()
    {
    TRACES( RDebug::Print(_L("CSysApEtelNetworkBarObserver::RunL - Signal Strength change: %d, bars: %d" ), iSignalStrength, iSignalBars ) );
    
    // Reorder notification
    OrderNotification();

    // Notify SysApAppUi
    if ( iPreviousNetworkBarValue != iSignalBars )
        {
        iPreviousNetworkBarValue = iSignalBars;
        TRAPD(err, iSysApAppUi.UpdateSignalBarsL());
        if ( err != KErrNone )
            {
            TRACES( RDebug::Print(_L("CSysApEtelNetworkBarObserver::RunL - UpdateSignalBarsL error: %d" ), err ));                
            }
        }
    }

// -----------------------------------------------------------------------------
// CSysApEtelNetworkBarObserver::DoCancel
// Handle cancel order on this active object.
// -----------------------------------------------------------------------------
//
void CSysApEtelNetworkBarObserver::DoCancel()
    {
    TRACES( RDebug::Print( _L("CSysApEtelNetworkBarObserver::DoCancel" ) ) );
    if ( IsActive() )
        {
        iPhone.CancelAsyncRequest( EMobilePhoneNotifySignalStrengthChange );
        }
    }

// -----------------------------------------------------------------------------
// CSysApEtelNetworkBarObserver::OrderNotification
// Order new notification from CenRep.
// -----------------------------------------------------------------------------
//
void CSysApEtelNetworkBarObserver::OrderNotification()
    {
    TRACES( RDebug::Print( _L("CSysApEtelNetworkBarObserver::OrderNotification" ) ) );
    iPhone.NotifySignalStrengthChange(iStatus, iSignalStrength, iSignalBars);
    SetActive();
    }

// -----------------------------------------------------------------------------
// CSysApEtelNetworkBarObserver::GetBarCount
// -----------------------------------------------------------------------------
//
TInt CSysApEtelNetworkBarObserver::GetBarCount() const
    {
    TRACES( RDebug::Print( _L("CSysApEtelNetworkBarObserver::GetBarCount: iSignalBars=%d, iSignalBarsFromGetter=%d" ),
                           iSignalBars, iSignalBarsFromGetter ) );

    if ( iSignalBars == KNetworkBarsUninitialized ) // notification not yet completed
        {
        return iSignalBarsFromGetter; // from EMobilePhoneGetSignalStrength
        }
    else        
        {
        return iSignalBars; // from EMobilePhoneNotifySignalStrengthChange    
        }        
    }

// -----------------------------------------------------------------------------
// CSysApEtelNetworkBarObserver::GetBarCountCompleted
// -----------------------------------------------------------------------------
//
void CSysApEtelNetworkBarObserver::GetBarCountCompleted( TInt8 aSignalBars )
    {
    TRACES( RDebug::Print( _L("CSysApEtelNetworkBarObserver::GetBarCountCompleted:  aSignalBars=%d" ), aSignalBars ) );
    
    iSignalBarsFromGetter = aSignalBars;
    
    // Update indicator only if notification isn't completed meanwhile. Use iPreviousNetworkBarValue for
    // evaluation because iSignalBars may be updated even if RunL has not yet been executed
    if ( iPreviousNetworkBarValue == KNetworkBarsUninitialized )
        {
        TRAPD( err, iSysApAppUi.UpdateSignalBarsL() );
        
        if ( err != KErrNone )
            {
            TRACES( RDebug::Print(_L("CSysApEtelNetworkBarObserver::GetBarCountCompleted - UpdateSignalBarsL error: %d" ), err ));
            }
        }
        
    // getter object no longer needed as signal bar information is received via notification
    delete iNetworkBarGetter;
    iNetworkBarGetter = NULL;    
    }

// End of file
