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
* Description:  Data bearer change observer.
*
*/


#include <etelpckt.h>
#include <pcktcs.h>
#include "sysapeteldatabearerobserver.h"
#include "SysApAppUi.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSysApEtelDataBearerObserver::CSysApEtelDataBearerObserver
// ---------------------------------------------------------------------------
//
CSysApEtelDataBearerObserver::CSysApEtelDataBearerObserver( CSysApAppUi& aSysApAppUi,
                                                            RMobilePhone& aPhone ) : CActive( EPriorityStandard ),
                                                                                     iSysApAppUi( aSysApAppUi ),
                                                                                     iPhone( aPhone ),
                                                                                     iDynCaps( 0 ),
                                                                                     iHsdpaActive( EFalse )
    {
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// CSysApEtelDataBearerObserver::ConstructL
// ---------------------------------------------------------------------------
//
void CSysApEtelDataBearerObserver::ConstructL()
    {
    TInt err = iPacketService.Open( iPhone );
    
    TRACES( RDebug::Print( _L("CSysApEtelDataBearerObserver::ConstructL: err=%d"), err ) );
    
    User::LeaveIfError( err );
        
    OrderNotification();
    }

// ---------------------------------------------------------------------------
// CSysApEtelDataBearerObserver::NewL
// ---------------------------------------------------------------------------
//
CSysApEtelDataBearerObserver* CSysApEtelDataBearerObserver::NewL( CSysApAppUi& aSysApAppUi,
                                                                  RMobilePhone& aPhone )
    {
    TRACES( RDebug::Print( _L("CSysApEtelDataBearerObserver::NewL") ) );
    
    CSysApEtelDataBearerObserver* self = new( ELeave ) CSysApEtelDataBearerObserver( aSysApAppUi,
                                                                                     aPhone );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CSysApEtelDataBearerObserver::~CSysApEtelDataBearerObserver
// ---------------------------------------------------------------------------
//
CSysApEtelDataBearerObserver::~CSysApEtelDataBearerObserver()
    {
    TRACES( RDebug::Print( _L("CSysApEtelDataBearerObserver::~CSysApEtelDataBearerObserver") ) );
    Cancel();
    iPacketService.Close();
    }


// ---------------------------------------------------------------------------
// CSysApEtelDataBearerObserver::IsBearerHsdpa
// ---------------------------------------------------------------------------
//
TBool CSysApEtelDataBearerObserver::IsBearerHsdpa() const
    {
    return iHsdpaActive;
    }


// ---------------------------------------------------------------------------
// From class CActive
// CSysApEtelDataBearerObserver::RunL
// ---------------------------------------------------------------------------
//
void CSysApEtelDataBearerObserver::RunL()
    {
    TRACES( RDebug::Print( _L("CSysApEtelDataBearerObserver::RunL: iStatus=%d, iDynCaps=0x%x, iHsdpaActive=%d"),
                           iStatus.Int(), iDynCaps, iHsdpaActive ) );
    
    OrderNotification();
    
    TBool newHsdpaState = (iDynCaps & RPacketService::KCapsHSDPA) ? ETrue : EFalse;
    
    // update signal indicators only when HSDPA activation state changes
    if ( newHsdpaState != iHsdpaActive )
        {
        iHsdpaActive = newHsdpaState;
//        TRAPD( err, iSysApAppUi.SetSignalIndicatorL() );
        
//        if ( err != KErrNone )
            {
//            TRACES( RDebug::Print( _L("CSysApEtelDataBearerObserver::RunL: err=%d"), err ) );
            }
        }
    }

// ---------------------------------------------------------------------------
// From class CActive
// CSysApEtelDataBearerObserver::DoCancel
// ---------------------------------------------------------------------------
//
void CSysApEtelDataBearerObserver::DoCancel()
    {
    TRACES( RDebug::Print( _L("CSysApEtelDataBearerObserver::DoCancel") ) );
    
    if ( IsActive() )
        {
        iPacketService.CancelAsyncRequest( EPacketNotifyDynamicCapsChange );
        }
    }


// ---------------------------------------------------------------------------
// CSysApEtelDataBearerObserver::OrderNotification
// ---------------------------------------------------------------------------
//
void CSysApEtelDataBearerObserver::OrderNotification()
    {
    iPacketService.NotifyDynamicCapsChange( iStatus, iDynCaps );
    SetActive();
    }
