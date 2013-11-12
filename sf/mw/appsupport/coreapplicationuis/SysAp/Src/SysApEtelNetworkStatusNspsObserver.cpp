/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApEtelNetworkStatusNspsObserver implementation.
*
*/


// INCLUDE FILES
#include <avkon.hrh>
#include "SysApEtelNetworkStatusNspsObserver.h"
#include "SysApAppUi.h"

//CONSTANTS

// ========================= MEMBER FUNCTIONS ================================

// ----------------------------------------------------------------------------
// CSysApEtelNetworkStatusNspsObserver::CSysApEtelNetworkStatusNspsObserver
//            ( CSysApAppUi& aSysApAppUi, RMmCustomAPI& aCustomAPI )(
// ----------------------------------------------------------------------------

CSysApEtelNetworkStatusNspsObserver::CSysApEtelNetworkStatusNspsObserver
    ( CSysApAppUi& aSysApAppUi, RMmCustomAPI& aCustomAPI ) :
    CActive( EPriorityStandard ), iCustomAPI( aCustomAPI ), iSysApAppUi( aSysApAppUi )
    {
    TRACES( RDebug::Print( _L("CSysApEtelNetworkStatusNspsObserver::CSysApEtelNetworkStatusNspsObserver") ) );
    CActiveScheduler::Add( this );
    IssueNetworkStatusNspsNotification();
    }

// ----------------------------------------------------------------------------
// CSysApEtelNetworkStatusNspsObserver::~CSysApEtelNetworkStatusNspsObserver()
// ----------------------------------------------------------------------------

CSysApEtelNetworkStatusNspsObserver::~CSysApEtelNetworkStatusNspsObserver()
    {
    TRACES( RDebug::Print( _L("CSysApEtelNetworkStatusNspsObserver::~CSysApEtelNetworkStatusNspsObserver") ) );
    Cancel();
    }

// ----------------------------------------------------------------------------
// CSysApEtelNetworkStatusNspsObserver::CommandNetCsWakeupOnNsps()
// ----------------------------------------------------------------------------

void CSysApEtelNetworkStatusNspsObserver::CommandNetCsWakeupOnNsps()
    {
    TRACES( RDebug::Print( _L("CSysApEtelNetworkStatusNspsObserver::CommandNetCsWakeupOnNsps()") ) );
    if( IsActive() )
        {
        Cancel();
        }
    iCustomAPI.NetWakeup( iStatus );
    iNetWakeUpSent = ETrue;
    SetActive();
    }

// ----------------------------------------------------------------------------
// CSysApEtelNetworkStatusNspsObserver::DoCancel()
// ----------------------------------------------------------------------------

void CSysApEtelNetworkStatusNspsObserver::DoCancel()
    {
    // cancel NotifyNSPSStatus()
    iCustomAPI.CancelAsyncRequest( ECustomNotifyNSPSStatusIPC );
    // cancel NetWakeup()
    iCustomAPI.CancelAsyncRequest( ECustomNetWakeupIPC );
    iNetWakeUpSent = EFalse;
    }

// ----------------------------------------------------------------------------
// CSysApEtelNetworkStatusNspsObserver::RunL()
// ----------------------------------------------------------------------------

void CSysApEtelNetworkStatusNspsObserver::RunL()
    {
    TRACES( RDebug::Print( _L("CSysApEtelNetworkStatusNspsObserver::RunL(): iStatus: %d, iNspsStatus: %d"), iStatus.Int(), iNspsStatus ) );

    if ( iStatus == KErrNone )
        {
        if( !iNetWakeUpSent )
            {
            iSysApAppUi.HandleNetworkNspsNotification( iNspsStatus );
            }
        }
    
    if ( iStatus == KErrServerTerminated )
        {
        TRACES( RDebug::Print( _L("CSysApEtelNetworkStatusNspsObserver::RunL(): ETEL SERVER SEEMS TO HAVE CRASHED !!!!!") ) );
        }
    else if ( iStatus != KErrNotSupported )
        {
        IssueNetworkStatusNspsNotification();
        }
    }

// ----------------------------------------------------------------------------
// CSysApEtelNetworkStatusNspsObserver::RunError( TInt aError )
// ----------------------------------------------------------------------------

#ifdef _DEBUG
TInt CSysApEtelNetworkStatusNspsObserver::RunError( TInt aError )
#else
TInt CSysApEtelNetworkStatusNspsObserver::RunError( TInt /* aError */ )
#endif
    {
    TRACES( RDebug::Print( _L("CSysApEtelNetworkStatusNspsObserver::RunError( aError:%d )" ), aError ) );
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// CSysApEtelNetworkStatusNspsObserver::IssueNetworkStatusNspsNotification()
// ----------------------------------------------------------------------------

void CSysApEtelNetworkStatusNspsObserver::IssueNetworkStatusNspsNotification()
    {
    TRACES( RDebug::Print( _L("CSysApEtelNetworkStatusNspsObserver::IssueNetworkStatusNspsNotification()") ) );
    if( !IsActive() ) 
        {
        iCustomAPI.NotifyNSPSStatus( iStatus, iNspsStatus );
        iNetWakeUpSent = EFalse;
        SetActive() ;       
        }
    }

// End of File


