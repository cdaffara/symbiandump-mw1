/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApEtelConnector implementation.
*
*/


// INCLUDE FILES
#include    <e32svr.h>
#include    <mmtsy_names.h>
#include    "SysApAppUi.h"
#include    "SysApEtelConnector.h"
#include    "SysApEtelNetworkStatusNspsObserver.h"
#include    "SysApEtelSmsStoreObserver.h"
#include    "SysApEtelNetworkBarObserver.h"
#include "sysapeteldatabearerobserver.h"
#include "sysapetelnetworkinfoobserver.h"
#include "SysApFeatureManager.h"

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------

CSysApEtelConnector::CSysApEtelConnector( CSysApAppUi& aSysApAppUi ) : 
                                          iSysApEtelNetworkStatusNspsObserver( NULL ),
                                          iSysApAppUi( aSysApAppUi ),
                                          iSysApEtelNetworkBarObserver( NULL ),
                                          iSysApEtelSmsStoreObserver( NULL ),
                                          iSysApEtelDataBearerObserver( NULL ),
                                          iSysApEtelNetworkInfoObserver( NULL )
    {
    }

// ----------------------------------------------------------------------------
// CSysApEtelConnector::ConstructL()
// ----------------------------------------------------------------------------

void CSysApEtelConnector::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApEtelConnector::ConstructL: START") ) );
    TInt err( KErrGeneral );
    TInt thisTry( 0 );
    /* Server connections are tried KTriesToConnectServer times because occasional
    fails on connections are possible at least on some servers */

    TRACES( RDebug::Print( _L("CSysApEtelConnector::ConstructL: trying RTelServer::Connect()") ) );
    while ( ( err = iEtelServer.Connect() ) != KErrNone && ( thisTry++ ) <= KTriesToConnectServer )
        {
        TRACES( RDebug::Print( _L("CSysApEtelConnector::ConstructL: RTelServer::Connect() failed. Error: %d"), err ) );
        User::After( KTimeBeforeRetryingServerConnection );
        }
    User::LeaveIfError( err );

    TRACES( RDebug::Print( _L("CSysApEtelConnector::ConstructL: trying RTelServer::LoadPhoneModule()") ) );
    err = iEtelServer.LoadPhoneModule( KMmTsyModuleName );
    while ( ( err != KErrNone ) && ( err != KErrAlreadyExists ) && ( thisTry++ ) <= KTriesToConnectServer )
        {
        TRACES( RDebug::Print( _L("CSysApEtelConnector::ConstructL: RTelServer::LoadPhoneModule() failed. Error: %d"), err ) );
        User::After( KTimeBeforeRetryingServerConnection );
        err = iEtelServer.LoadPhoneModule( KMmTsyModuleName );
        }
    User::LeaveIfError( err );

    thisTry = 0;
    TRACES( RDebug::Print( _L("CSysApEtelConnector::ConstructL: trying RMobilePhone::Open()") ) );
    while ( ( err = iPhone.Open( iEtelServer, KMmTsyPhoneName ) ) != KErrNone && ( thisTry++ ) <= KTriesToConnectServer )
        {
        TRACES( RDebug::Print( _L("CSysApEtelConnector::ConstructL: RMobilePhone::Open() failed. Error: %d"), err ) );
        User::After( KTimeBeforeRetryingServerConnection );
        }
    User::LeaveIfError( err );

    thisTry = 0;
    TRACES( RDebug::Print( _L("CSysApEtelConnector::ConstructL: trying RMmCustomAPI::Open()") ) );
    while ( ( err = iCustomAPI.Open( iPhone ) ) != KErrNone && ( thisTry++ ) <= KTriesToConnectServer )
        {
        TRACES( RDebug::Print( _L("CSysApEtelConnector::ConstructL: RMmCustomAPI::Open() failed. Error: %d"), err ) );
        User::After( KTimeBeforeRetryingServerConnection );
        }
    User::LeaveIfError( err );
    
    // Connect to SMS messaging
    thisTry = 0;
    TRACES( RDebug::Print( _L("CSysApEtelConnector::ConstructL: trying RMobileSmsMessaging::Open()") ) );
    while ( ( err = iSmsMessaging.Open( iPhone ) ) != KErrNone && ( thisTry++ ) <= KTriesToConnectServer )
        {
        TRACES( RDebug::Print( _L("CSysApEtelConnector::ConstructL: RMobileSmsMessaging::Open() failed. Error: %d"), err ) );
        User::After( KTimeBeforeRetryingServerConnection );
        }
    User::LeaveIfError( err );
    
    // Connect to SMS message store
    thisTry = 0;
    TRACES( RDebug::Print( _L("CSysApEtelConnector::ConstructL: trying RMobileSmsStore::Open()") ) );
    while ( ( err = iSmsStore.Open( iSmsMessaging, KETelIccSmsStore ) ) != KErrNone && ( thisTry++ ) <= KTriesToConnectServer )
        {
        TRACES( RDebug::Print( _L("CSysApEtelConnector::ConstructL: RMobileSmsStore::Open() failed. Error: %d"), err ) );
        User::After( KTimeBeforeRetryingServerConnection );
        }
    User::LeaveIfError( err );
    
    // Initialize SMS Message store observer
    iSysApEtelSmsStoreObserver = CSysApEtelSmsStoreObserver::NewL( iSysApAppUi, iSmsStore );
    
    iSysApEtelNetworkStatusNspsObserver = new( ELeave ) CSysApEtelNetworkStatusNspsObserver( iSysApAppUi, iCustomAPI );

    iSysApEtelNetworkBarObserver = CSysApEtelNetworkBarObserver::NewL(iSysApAppUi, iPhone);
    
    iSysApEtelDataBearerObserver = CSysApEtelDataBearerObserver::NewL( iSysApAppUi, iPhone );
    
    if ( iSysApAppUi.SysApFeatureManager().Supported( KSysApFeatureIdUseHsdpaAvailableIcon ) )
        {
        iSysApEtelNetworkInfoObserver = CSysApEtelNetworkInfoObserver::NewL( iSysApAppUi, iPhone, iCustomAPI );    
        }

    TRACES( RDebug::Print( _L("CSysApEtelConnector::ConstructL: END") ) );
}

// ----------------------------------------------------------------------------
// CSysApEtelConnector::CommandNetCsWakeupOnNsps()
// ----------------------------------------------------------------------------

void CSysApEtelConnector::CommandNetCsWakeupOnNsps()
    {
    iSysApEtelNetworkStatusNspsObserver->CommandNetCsWakeupOnNsps();
    }

// ----------------------------------------------------------------------------
// CSysApEtelConnector::GetSignalBars()
// ----------------------------------------------------------------------------

TInt CSysApEtelConnector::GetSignalBars() const
    {
    return iSysApEtelNetworkBarObserver->GetBarCount();
    }

// ----------------------------------------------------------------------------
// CSysApEtelConnector::GetTelServer()
// ----------------------------------------------------------------------------

RTelServer* CSysApEtelConnector::GetTelServer()
    {
    return &iEtelServer;
    }

// ----------------------------------------------------------------------------
// CSysApEtelConnector::NewL( CSysApAppUi& aSysApAppUi )
// ----------------------------------------------------------------------------

CSysApEtelConnector* CSysApEtelConnector::NewL( CSysApAppUi& aSysApAppUi )
    {
    TRACES( RDebug::Print( _L("CSysApEtelConnector::NewL") ) );
    CSysApEtelConnector* self = new (ELeave) CSysApEtelConnector( aSysApAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApEtelConnector::~CSysApEtelConnector()
// ----------------------------------------------------------------------------

CSysApEtelConnector::~CSysApEtelConnector()
    {
    TRACES( RDebug::Print( _L("CSysApEtelConnector::~CSysApEtelConnector") ) );
    delete iSysApEtelNetworkStatusNspsObserver;
    delete iSysApEtelNetworkBarObserver;

    delete iSysApEtelSmsStoreObserver;
    
    iSmsStore.Close();
    iSmsMessaging.Close();
    
    delete iSysApEtelDataBearerObserver;
    delete iSysApEtelNetworkInfoObserver;
    
    iCustomAPI.Close();
    iPhone.Close();
    iEtelServer.Close();
    }


// ----------------------------------------------------------------------------
// CSysApEtelConnector::AlsSupported()
// ----------------------------------------------------------------------------

TBool CSysApEtelConnector::AlsSupported() const
    {
    TBool alsSupported = AlsPpOnOff();
    // if ALS product profile is TRUE then ALS supported and there
    // is no reason to check AlsSupported info.
    
    if ( !alsSupported )
        {
        // ALS Product Profile is Off then we must check ALS support by
        // using AlsSimSupport since it check SIM ALS support information.
        alsSupported = AlsSimSupport();
        }
    
    TRACES( RDebug::Print( _L("CSysApEtelConnector::AlsSupported: alsSupported=%d"),
            alsSupported ) );
    
    return alsSupported;
    }
    
// ----------------------------------------------------------------------------
// CSysApEtelConnector::IsBearerHsdpa()
// ----------------------------------------------------------------------------

TBool CSysApEtelConnector::IsBearerHsdpa() const
    {
    return iSysApEtelDataBearerObserver->IsBearerHsdpa();
    }

// ----------------------------------------------------------------------------
// CSysApEtelConnector::HsdpaAvailable()
// ----------------------------------------------------------------------------

TBool CSysApEtelConnector::HsdpaAvailable() const
    {
    if ( iSysApEtelNetworkInfoObserver ) // observer created only if availability icon is supported
        {
        return iSysApEtelNetworkInfoObserver->HsdpaAvailable();    
        }
    else
        {
        return EFalse;
        }
    }

// ----------------------------------------------------------------------------
// CSysApEtelConnector::AlsPpOnOff()
// ----------------------------------------------------------------------------

TBool CSysApEtelConnector::AlsPpOnOff() const
    {
    RMmCustomAPI::TAlsSupport ppAlsSupport;
    TBool alsPpOnOff(EFalse);
    TInt err = iCustomAPI.CheckAlsPpSupport( ppAlsSupport ); 
    
    if ( ( err == KErrNone ) && ( ppAlsSupport == RMmCustomAPI::EAlsSupportOn ) )
        {
        alsPpOnOff = ETrue;
        }
        
    TRACES( RDebug::Print( _L("CSysApEtelConnector::AlsPpOnOff: alsPpOnOff=%d, err=%d"), alsPpOnOff, err ) );
            
    return alsPpOnOff;
    }

// ----------------------------------------------------------------------------
// CSysApEtelConnector::AlsCspOnOff()
// ----------------------------------------------------------------------------

TBool CSysApEtelConnector::AlsCspOnOff() const
    {
    TBool alsCspOnOff( EFalse );
    RMobilePhone::TMobilePhoneCspFileV1 cspFile;
    RMobilePhone::TMobilePhoneCspFileV1Pckg cspFilePckg( cspFile );
    TRequestStatus status;
    iPhone.GetCustomerServiceProfile( status, cspFilePckg );
    User::WaitForRequest( status );
    TInt err( status.Int() );
    if ( err == KErrNone && ( cspFile.iCphsTeleservices & RMobilePhone::KCspALS ) )
        {
        alsCspOnOff = ETrue;
        }

    TRACES( RDebug::Print( _L("CSysApEtelConnector::AlsCspOnOff: alsCspOnOff=%d, err=%d, cphs=%d"),
        alsCspOnOff, err, cspFile.iCphsTeleservices ) );

    return alsCspOnOff;
    }
    
// ----------------------------------------------------------------------------
// CSysApEtelConnector::AlsSimSupport()
// ----------------------------------------------------------------------------

TBool CSysApEtelConnector::AlsSimSupport() const
    {
    RMobilePhone::TMobilePhoneALSLine alsLine( RMobilePhone::EAlternateLineNotAvailable );
    TBool alsSupported( EFalse );
    
    if ( AlsCspOnOff() )
        {
        if ( iPhone.GetALSLine( alsLine ) == KErrNone )
            {
            switch ( alsLine )
                {
                case RMobilePhone::EAlternateLinePrimary:
                case RMobilePhone::EAlternateLineAuxiliary:
                case RMobilePhone::EAlternateLineUnknown:
                    alsSupported = ETrue;
                    break;
                
                default:
                case RMobilePhone::EAlternateLineNotAvailable:
                    alsSupported = EFalse;
                    break;
                }
            }
        }

    TRACES( RDebug::Print( _L("CSysApEtelConnector::AlsSimSupport: alsLine=%d, alsSupported=%d"),
                           alsLine, alsSupported ) );
    
    return alsSupported;    
    }    

// ----------------------------------------------------------------------------
// CSysApEtelConnector::IsSimSmsStoreFull()
// ----------------------------------------------------------------------------

TBool CSysApEtelConnector::IsSimSmsStoreFull() const
    {
    return iSysApEtelSmsStoreObserver->IsFull();
    }

// ----------------------------------------------------------------------------
// CSysApEtelConnector::ReadSimSmsStoreStatusInStartup()
// ----------------------------------------------------------------------------

void CSysApEtelConnector::ReadSimSmsStoreStatusInStartup()
    {
    iSysApEtelSmsStoreObserver->ReadStatusInStartup();
    }

// ----------------------------------------------------------------------------
// CSysApEtelConnector::SimCallDivertEfSupported()
// ----------------------------------------------------------------------------

TBool CSysApEtelConnector::SimCallDivertEfSupported() const
    {
#ifndef __WINS__ // ETel panics due to TSY stub returning KErrNotSupported in emulator
    TRequestStatus status;
    RMmCustomAPI::TCFIndicators cfIndicators;
    RMmCustomAPI::TCFIndicatorsPckg cfIndicatorPckg( cfIndicators );
    
    iCustomAPI.GetIccCallForwardingIndicatorStatus( status, cfIndicatorPckg );
    User::WaitForRequest( status );
    
    TInt err( status.Int() );
    
    TRACES( RDebug::Print( _L("CSysApEtelConnector::SimCallDivertEfSupported: err=%d"), err ) );
    
    // KErrNotFound explicitly states that CF-EF is not present in the SIM, but it is probably
    // safer to assume no support for CF-EF also with other error values
    return ( err == KErrNone );
#else // __WINS__
    return EFalse;
#endif // __WINS__   
    }

//  End of File  


