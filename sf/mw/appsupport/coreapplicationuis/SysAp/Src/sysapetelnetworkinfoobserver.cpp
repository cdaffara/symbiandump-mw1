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
* Description:  CSysApEtelNetworkInfoObserver implementation
*
*/


#include <rmmcustomapi.h>
#include "sysapetelnetworkinfoobserver.h"
#include "SysApAppUi.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSysApEtelNetworkInfoObserver::ConstructL
// ---------------------------------------------------------------------------
//
void CSysApEtelNetworkInfoObserver::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApEtelNetworkInfoObserver::ConstructL") ) );
    // get current network information
    TRequestStatus status( KErrNone );
    iPhone.GetCurrentNetwork( status, iNetInfoPckg );
    User::WaitForRequest( status );
    
    if ( status.Int() == KErrNone )
        {
        iHsdpaAvailable = iNetInfoPckg().iHsdpaAvailableIndicator;
        TRACES( RDebug::Print( _L("CSysApEtelNetworkInfoObserver::ConstructL: iHsdpaAvailable=%d"), iHsdpaAvailable ) );
        }
    else
        {
        TRACES( RDebug::Print( _L("CSysApEtelNetworkInfoObserver::ConstructL: error, status=%d") ) );
        }        
    
    // order notification for network information change
    OrderNotification();
    }

// ---------------------------------------------------------------------------
// CSysApEtelNetworkInfoObserver::NewL
// ---------------------------------------------------------------------------
//
CSysApEtelNetworkInfoObserver* CSysApEtelNetworkInfoObserver::NewL( CSysApAppUi& aSysApAppUi, 
                                                                    RMobilePhone& aPhone,
                                                                    RMmCustomAPI& aCustomApi )
    {
    TRACES( RDebug::Print( _L("CSysApEtelNetworkInfoObserver::NewL") ) );
    CSysApEtelNetworkInfoObserver* self = new (ELeave ) CSysApEtelNetworkInfoObserver( aSysApAppUi, aPhone, aCustomApi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CSysApEtelNetworkInfoObserver::CSysApEtelNetworkInfoObserver
// ---------------------------------------------------------------------------
//
CSysApEtelNetworkInfoObserver::CSysApEtelNetworkInfoObserver( CSysApAppUi& aSysApAppUi, 
                                                              RMobilePhone& aPhone,
                                                              RMmCustomAPI& aCustomApi ) :
                                                              CActive( EPriorityStandard ),
                                                              iSysApAppUi( aSysApAppUi ),
                                                              iPhone( aPhone ),
                                                              iCustomApi( aCustomApi ),
                                                              iNetInfoPckg( iNetInfo ),
                                                              iHsdpaAvailable( EFalse )
    {
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// CSysApEtelNetworkInfoObserver::CSysApEtelNetworkInfoObserver
// ---------------------------------------------------------------------------
//
CSysApEtelNetworkInfoObserver::~CSysApEtelNetworkInfoObserver()
    {
    TRACES( RDebug::Print( _L("CSysApEtelNetworkInfoObserver::~CSysApEtelNetworkInfoObserver()") ) );
    Cancel();
    }

// ---------------------------------------------------------------------------
// From class CActive.
// CSysApEtelNetworkInfoObserver::RunL
// ---------------------------------------------------------------------------
//
void CSysApEtelNetworkInfoObserver::RunL()
    {
    TRACES( RDebug::Print( _L("CSysApEtelNetworkInfoObserver::RunL: iStatus=%d"), iStatus.Int() ) );
    
    if ( iStatus.Int() == KErrNone )
        {
        TBool hsdpaAvailable = iNetInfoPckg().iHsdpaAvailableIndicator;
        
        TRACES( RDebug::Print( _L("CSysApEtelNetworkInfoObserver::RunL: hsdpaAvailable=%d"), hsdpaAvailable ) );
        
        // update indicators only if HSDPA availability has been changed
        if ( hsdpaAvailable != iHsdpaAvailable )
            {
            iHsdpaAvailable = hsdpaAvailable;
/*            TRAPD( err, iSysApAppUi.SetSignalIndicatorL() );
            
            if ( err != KErrNone )
                {
                TRACES( RDebug::Print( _L("CSysApEtelNetworkInfoObserver::RunL: err=%d"), err ) );
                }
*/            }
        }
    
    OrderNotification();    
    }
    
// ---------------------------------------------------------------------------
// From class CActive.
// CSysApEtelNetworkInfoObserver::DoCancel
// ---------------------------------------------------------------------------
//
void CSysApEtelNetworkInfoObserver::DoCancel()
    {
    TRACES( RDebug::Print( _L("CSysApEtelNetworkInfoObserver::DoCancel") ) );
    
    if ( IsActive() )
        {
        iPhone.CancelAsyncRequest( EMobilePhoneNotifyCurrentNetworkNoLocationChange );
        }
    }

// ---------------------------------------------------------------------------
// CSysApEtelNetworkInfoObserver::OrderNotification
// ---------------------------------------------------------------------------
//    
void CSysApEtelNetworkInfoObserver::OrderNotification()
    {
    TRACES( RDebug::Print( _L("CSysApEtelNetworkInfoObserver::OrderNotification") ) );
    
    iPhone.NotifyCurrentNetworkChange( iStatus, iNetInfoPckg );
    SetActive();
    }
    
// ---------------------------------------------------------------------------
// CSysApEtelNetworkInfoObserver::HsdpaAvailable
// ---------------------------------------------------------------------------
//        
TBool CSysApEtelNetworkInfoObserver::HsdpaAvailable() const
    {
    TRequestStatus status;
    RMmCustomAPI::THSxPAStatus hsxpaStatus;
    iCustomApi.ReadHSxPAStatus( status, hsxpaStatus );
    User::WaitForRequest( status );
    TInt err = status.Int();
    
    if ( err != KErrNone || hsxpaStatus == RMmCustomAPI::EHSxPADisabled )
        {
        // user has disabled HSDPA, HSDPA Availability must not be indicated even
        // if supported by current cell
        return EFalse;
        }
    
    TRACES( RDebug::Print( _L("CSysApEtelNetworkInfoObserver::HsdpaAvailable: err=%d, iHsdpaAvailable=%d, hsxpaStatus=%d "),
            err, iHsdpaAvailable, hsxpaStatus ) );
    
    return iHsdpaAvailable;
    }    
