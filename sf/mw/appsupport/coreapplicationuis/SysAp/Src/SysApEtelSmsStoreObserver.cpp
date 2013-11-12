/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApEtelSmsStoreObserver implementation
*
*/


// INCLUDES
#include "SysApEtelSmsStoreObserver.h"
#include "SysApAppUi.h"


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApEtelSmsStoreObserver::NewL() 
// ----------------------------------------------------------------------------
CSysApEtelSmsStoreObserver* CSysApEtelSmsStoreObserver::NewL( CSysApAppUi& aSysApAppUi, RMobileSmsStore& aStore )
    {
    CSysApEtelSmsStoreObserver* self = new(ELeave) CSysApEtelSmsStoreObserver( aSysApAppUi, aStore );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApEtelSmsStoreObserver::CSysApEtelSmsStoreObserver() 
// ----------------------------------------------------------------------------
CSysApEtelSmsStoreObserver::CSysApEtelSmsStoreObserver( CSysApAppUi& aSysApAppUi, RMobileSmsStore& aStore ) 
    : CActive(EPriorityStandard), 
    iSysApAppUi( aSysApAppUi ),
    iStore( aStore ),
    iStoreFull( EFalse )
    {
    CActiveScheduler::Add(this);
    }

// ----------------------------------------------------------------------------
// CSysApEtelSmsStoreObserver::ConstructL() 
// ----------------------------------------------------------------------------
void CSysApEtelSmsStoreObserver::ConstructL()
    {    
    // Get initial value (will also reorder in RunL)
    TRACES( RDebug::Print(_L("CSysApEtelSmsStoreObserver::ConstructL - Ordering sms store change notifications." ) ) );
    OrderNotification();
    }

// ----------------------------------------------------------------------------
// CSysApEtelSmsStoreObserver::~CSysApEtelSmsStoreObserver() 
// ----------------------------------------------------------------------------

CSysApEtelSmsStoreObserver::~CSysApEtelSmsStoreObserver()
    {
    TRACES( RDebug::Print( _L("CSysApEtelSmsStoreObserver::~CSysApEtelSmsStoreObserver") ) );
    Cancel();
    delete iStatusReader;
    }


// -----------------------------------------------------------------------------
// CSysApEtelSmsStoreObserver::RunL
// Handle notification from pubsub session.
// -----------------------------------------------------------------------------
//
void CSysApEtelSmsStoreObserver::RunL()
    {
    // Check status before reordering as that resets it
    if ( iStatus == KErrNone )
        {
        // Reorder notification
        OrderNotification();
        if ( iStatusReader )
            {
            iStatusReader->Cancel(); // Status read is not required anymore
            }
        TBool previous = iStoreFull;

        TRACES( RDebug::Print(_L("CSysApEtelSmsStoreObserver::RunL - EMobilePhoneStoreNotifyStoreEvent, iNewEvent: 0x%x" ), iNewEvent) );
        if ( iNewEvent & RMobilePhoneStore::KStoreFull )
            {
            iStoreFull = ETrue;
            }
        else if ( iNewEvent & RMobilePhoneStore::KStoreHasSpace || iNewEvent & RMobilePhoneStore::KStoreEmpty )
            {
            iStoreFull = EFalse;
            }

        TRACES(RDebug::Print(_L("CSysApEtelSmsStoreObserver::RunL - previous:%d iStoreFull:%d"),previous,iStoreFull));

        // Notify SysApAppUi
        if ( previous != iStoreFull )
            {
            TRAPD(err, iSysApAppUi.HandleSmsStorageNotificationL(iStoreFull));
            if ( err != KErrNone )
                {
                TRACES( RDebug::Print(_L("CSysApEtelSmsStoreObserver::RunL - HandleSmsStorageNotificationL error: %d" ), err ));                
                }
            }
        }
    else
        {
        TRACES( RDebug::Print(_L("CSysApEtelSmsStoreObserver::RunL - error: %d" ), iStatus.Int()) );        

        // Reorder notification
        OrderNotification();
        }
    
    }

// -----------------------------------------------------------------------------
// CSysApEtelSmsStoreObserver::DoCancel
// Handle cancel order on this active object.
// -----------------------------------------------------------------------------
//
void CSysApEtelSmsStoreObserver::DoCancel()
    {
    if ( IsActive() )
        {
        iStore.CancelAsyncRequest(EMobilePhoneStoreNotifyStoreEvent);
        }
    }
// -----------------------------------------------------------------------------
// CSysApEtelSmsStoreObserver::OrderNotification
// Order new notification from CenRep.
// -----------------------------------------------------------------------------
//
void CSysApEtelSmsStoreObserver::OrderNotification()
    {   
    iStore.NotifyStoreEvent(iStatus, iNewEvent, iChangedIndex);
    SetActive();
    }

// -----------------------------------------------------------------------------
// CSysApEtelSmsStoreObserver::IsFull
// -----------------------------------------------------------------------------
//
TInt CSysApEtelSmsStoreObserver::IsFull() const
    {
    return iStoreFull;
    }

// -----------------------------------------------------------------------------
// CSysApEtelSmsStoreObserver::ReadStatusInStartup
// -----------------------------------------------------------------------------
//
void CSysApEtelSmsStoreObserver::ReadStatusInStartup()
    {
    if ( !iStatusReader )
        {        
        iStatusReader = new CStoreStatusReader( *this );
        if ( iStatusReader )
            {
            iStatusReader->Start();
            }
        else
            {
            TRACES( RDebug::Print( _L( "CSysApEtelSmsStoreObserver::ReadStatusInStartup - no memory" ) ) );
            }            
        }
    }

// -----------------------------------------------------------------------------
// CSysApEtelSmsStoreObserver::CStoreStatusReader::CStoreStatusReader
// -----------------------------------------------------------------------------
//
CSysApEtelSmsStoreObserver::CStoreStatusReader::CStoreStatusReader(
        CSysApEtelSmsStoreObserver& aObserver ) :
    CActive( EPriorityStandard ),
    iObserver( aObserver ),
    iInfoPckg( aObserver.iStoreInfo )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CSysApEtelSmsStoreObserver::CStoreStatusReader::~CStoreStatusReader
// -----------------------------------------------------------------------------
//
CSysApEtelSmsStoreObserver::CStoreStatusReader::~CStoreStatusReader()
    {
    Cancel();
    }

// -----------------------------------------------------------------------------
// CSysApEtelSmsStoreObserver::CStoreStatusReader::RunL
// -----------------------------------------------------------------------------
//
void CSysApEtelSmsStoreObserver::CStoreStatusReader::RunL()
    {
    TInt err( iStatus.Int() );
    if ( err == KErrNone && !iObserver.iStoreFull )
        {
        RMobilePhoneStore::TMobilePhoneStoreInfoV1& info( iObserver.iStoreInfo );

        TRACES( RDebug::Print(
            _L( "CSysApEtelSmsStoreObserver::CStoreStatusReader::RunL - used:%d total:%d" ),
            info.iUsedEntries, info.iTotalEntries ) );

        if ( info.iTotalEntries > 0 && info.iUsedEntries >= info.iTotalEntries )
            {
            // Blink the Envelope indicator
/*            iObserver.iSysApAppUi.SetIndicatorStateL(
                EAknIndicatorEnvelope, EAknIndicatorStateAnimate );
            iObserver.iStoreFull = ETrue;
*/            }
        }

    TRACES( RDebug::Print(
        _L( "CSysApEtelSmsStoreObserver::CStoreStatusReader::RunL - err:%d full:%d" ),
        err, iObserver.iStoreFull ) );
    }

// -----------------------------------------------------------------------------
// CSysApEtelSmsStoreObserver::CStoreStatusReader::DoCancel
// -----------------------------------------------------------------------------
//
void CSysApEtelSmsStoreObserver::CStoreStatusReader::DoCancel()
    {
    iObserver.iStore.CancelAsyncRequest( EMobilePhoneStoreGetInfo );
    }

// -----------------------------------------------------------------------------
// CSysApEtelSmsStoreObserver::CStoreStatusReader::Start
// -----------------------------------------------------------------------------
//
void CSysApEtelSmsStoreObserver::CStoreStatusReader::Start()
    {
    if ( !IsActive() )
        {
        iObserver.iStore.GetInfo( iStatus, iInfoPckg );
        SetActive();
        }
    }

// End of file
