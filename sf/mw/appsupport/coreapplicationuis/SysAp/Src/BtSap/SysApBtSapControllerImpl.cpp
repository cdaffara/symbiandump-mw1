/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApBtSapController implementation
 *
*/


// INCLUDES

#include "SysApBtSapControllerImpl.h"
#include "SysApSubscriber.h"
#include "SysApAppUi.h"
#include <BTSapDomainPSKeys.h>
#include <btengdevman.h>
#include <btengconnman.h>
#include <StringLoader.h>
#include <coemain.h>
//#include <SysAp.rsg>


MSysApBtSapController* CreateSysApBtSapControllerL( CSysApAppUi& aSysApAppUi )
    {
    return CSysApBtSapController::NewL( aSysApAppUi );
    }

// ============================ LOCAL FUNCTIONS ==============================
// -----------------------------------------------------------------------------
// SysApCBTDeviceArrayCleanup
// Cleanup operation for CBTDeviceArray
// -----------------------------------------------------------------------------
static void SysApCBTDeviceArrayCleanup( TAny* aItem )
    {
    CBTDeviceArray* devices = static_cast<CBTDeviceArray*>( aItem );
    devices->ResetAndDestroy();
    delete devices;
    }

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApBtSapController::NewL() 
// ----------------------------------------------------------------------------
CSysApBtSapController* CSysApBtSapController::NewL( CSysApAppUi& aSysApAppUi )
    {
    TRACES( RDebug::Print( _L("CSysApBtSapController::NewL") ) );
    CSysApBtSapController* self = new(ELeave) CSysApBtSapController( aSysApAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApBtSapController::CSysApBtSapController() 
// ----------------------------------------------------------------------------
CSysApBtSapController::CSysApBtSapController( CSysApAppUi& aSysApAppUi ) :
    iSysApAppUi( aSysApAppUi ),
    iBtSapEnabled( EFalse ),
    iSwitchingToOffline( EFalse )
    {
    }

// ----------------------------------------------------------------------------
// CSysApBtSapController::ConstructL() 
// ----------------------------------------------------------------------------
void CSysApBtSapController::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApBtSapController::ConstructL") ) );
    iBtSapSubscriber = CSysApSubscriber::NewL( *this, KPSUidBluetoothSapConnectionState, KBTSapConnectionState );
    iBtSapSubscriber->Subscribe();
    }

// ----------------------------------------------------------------------------
// CSysApBtSapController::~CSysApBtSapController() 
// ----------------------------------------------------------------------------

CSysApBtSapController::~CSysApBtSapController()
    {
    TRACES( RDebug::Print( _L("CSysApBtSapController::~CSysApBtSapController") ) );
    delete iBtSapSubscriber;
    }

// ----------------------------------------------------------------------------
// CSysApBtSapController::BtSapEnabled()
// ----------------------------------------------------------------------------
TBool CSysApBtSapController::BtSapEnabled()
    {
    return iBtSapEnabled;
    }

// ----------------------------------------------------------------------------
// CSysApBtSapController::Disconnect()
// ----------------------------------------------------------------------------
TInt CSysApBtSapController::Disconnect()
    {
    TRACES( RDebug::Print( _L("CSysApBtSapController::Disconnect: iBtSapEnabled=%d"), iBtSapEnabled ) );

    RBTDevAddrArray addrArray;
    CBTEngConnMan* btConnMan = NULL;
    TRAPD( err, btConnMan = CBTEngConnMan::NewL() );
    if ( err == KErrNone )
        {
        err = btConnMan->GetConnectedAddresses( addrArray, EBTProfileSAP );
        if ( (err == KErrNone) && addrArray.Count() )
            {
            err = btConnMan->Disconnect( addrArray[0], EBTDiscGraceful );
            }
        else
            {
            TRACES( RDebug::Print( _L("CSysApBtSapController::Disconnect CBTEngConnMan::GetConnectedAddresses failed, err=%d"), err ) );
            }            
        }
    else
        {
        TRACES( RDebug::Print( _L("CSysApBtSapController::Disconnect CBTEngConnMan::NewL failed, err=%d"), err ) );
        }        
    delete btConnMan;
    addrArray.Close();
    
    return err;
    }

// ----------------------------------------------------------------------------
// CSysApBtSapController::SwitchingToOffline()
// ----------------------------------------------------------------------------
void CSysApBtSapController::SwitchingToOffline()
    {
    TRACES( RDebug::Print( _L("CSysApBtSapController:SwitchingToOffline") ) );
    iSwitchingToOffline = ETrue;
    }
    
// ----------------------------------------------------------------------------
// CSysApBtSapController::SimApplicationsClosed()
// ----------------------------------------------------------------------------
void CSysApBtSapController::SimApplicationsClosed()
    {
    TRACES( RDebug::Print( _L("CSysApBtSapController::SimApplicationsClosed()" ) ) );
    TInt err ( 0 );

    err = iSysApAppUi.SetStarterState( RStarterSession::EBTSap );

    if ( err )
        {
        TRACES( RDebug::Print( _L("CSysApBtSapController::SimApplicationsClosed SetMtcState( ESysApMtcBtSapState ) returns %d"), err ) );
        // We should end up in this branch only in the early integration phase of a product, and the
        // device is most likely dysfunctional after this error.
        }
    else
        {
        iFinalizeDisconnectNeeded = ETrue;
        
        // No RF available so signal bars should be same as in offline mode.
        iSysApAppUi.IgnoreZeroNetworkBarNotifications(ETrue);
//        TRAP( err, iSysApAppUi.UpdateSignalBarsL( KAknSignalOffLineMode ) );

        iSysApAppUi.SetNetworkConnectionAllowed( ECoreAppUIsNetworkConnectionNotAllowed );

        // The SAP connection is accepted via P&S key
        RProperty::Set( KPSUidBluetoothSapConnectionState, KBTSapConnectionState, EBTSapAccepted );
        }
        
    TRACES( RDebug::Print( _L("CSysApBtSapController::SimApplicationsClosed() - END" ) ) );
    }

// ----------------------------------------------------------------------------
// CSysApBtSapController::FinalizeDisconnect()
// ----------------------------------------------------------------------------
void CSysApBtSapController::FinalizeDisconnect()
    {
    TRACES( RDebug::Print( _L("CSysApBtSapController::FinalizeDisconnect(): iSwitchingToOffline=%d"), iSwitchingToOffline ) );
    TInt err ( 0 );
    iFinalizeDisconnectNeeded = EFalse;
    iSysApAppUi.IgnoreZeroNetworkBarNotifications(EFalse);

//    if ( iSysApAppUi.ActiveProfileId() == KOfflineModeProfileId )
      if(0)
        {
        if ( iSwitchingToOffline )
            {
            TRAP( err, iSysApAppUi.SwitchFromOnlineToOfflineModeL() );
            TRACES( RDebug::Print( _L("SwitchFromOnlineToOfflineModeL() leaves with err=%d"), err ) );
            iSwitchingToOffline = EFalse;
            }
        else
            {
            err = iSysApAppUi.SetStarterState( RStarterSession::EOffline );
            TRACES( RDebug::Print( _L("CSysApBtSapController::FinalizeDisconnect SetMtcState( ESysApNormalState ) returns %d"), err ) );
            }
        }
    else
        {
        err = iSysApAppUi.SetStarterState( RStarterSession::ENormal );
        
        if ( err == KErrNone )
            {
            iSysApAppUi.SetNetworkConnectionAllowed(ECoreAppUIsNetworkConnectionAllowed);
            }
        
        TRACES( RDebug::Print( _L("CSysApBtSapController::FinalizeDisconnect SetMtcState( ESysApNormalState ) returns %d"), err ) );
        }
    }

// ----------------------------------------------------------------------------
// CSysApBtSapController::HandlePropertyChangedL()
// ----------------------------------------------------------------------------
void CSysApBtSapController::HandlePropertyChangedL( const TUid& aCategory, const TUint aKey )
    {
    if ( aCategory == KPSUidBluetoothSapConnectionState && aKey == KBTSapConnectionState )
        {
        TInt value( 0 );
        RProperty::Get( aCategory, aKey, value );
        switch ( value )
            {
            case EBTSapNotConnected:
                TRACES( RDebug::Print( _L("CSysApBtSapController::HandlePropertyChangedL: EBTSapNotConnected, iFinalizeDisconnectNeeded=%d"), iFinalizeDisconnectNeeded ) );
                if ( iFinalizeDisconnectNeeded )
                    {
                    FinalizeDisconnect();
                    }
                iBtSapEnabled = EFalse;
                break;
            case EBTSapConnecting:
                TRACES( RDebug::Print( _L("CSysApBtSapController::HandlePropertyChangedL: EBTSapConnecting, iBtSapEnabled=%d"), iBtSapEnabled ) );
                if ( !iBtSapEnabled )
                    {
                    iSysApAppUi.InitCloseSimApplicationsL();
                    }
                break;
            case EBTSapConnected:
                TRACES( RDebug::Print( _L("CSysApBtSapController::HandlePropertyChangedL: EBTSapConnected") ) );
                iBtSapEnabled = ETrue;
                break;
            default:
                TRACES( RDebug::Print( _L("CSysApBtSapController::HandlePropertyChangedL: unhandled value: %d"), value ) );
                break;
            }
        }
    }


// ----------------------------------------------------------------------------
// CSysApBtSapController::GetBtSapDeviceNameL()
// ----------------------------------------------------------------------------
HBufC* CSysApBtSapController::GetBtSapDeviceNameL()
    {
    TRACES( RDebug::Print( _L("CSysApBtSapController::GetBtSapDeviceNameL()") ) );
    
    TBTDevAddr address;
    RBTDevAddrArray addrArray;
    CBTEngConnMan* btConnMan = NULL;
    TRAPD( err, btConnMan = CBTEngConnMan::NewL() );
    CleanupStack::PushL( btConnMan );
    if ( err == KErrNone )
        {
        err = btConnMan->GetConnectedAddresses( addrArray, EBTProfileSAP );
        if ( ( err == KErrNone ) && addrArray.Count() )
            {
            address = addrArray[0];
            }
        else if ( ( err == KErrNone) && !addrArray.Count() ) // not known whether this will ever occur
            {
            TRACES( RDebug::Print( _L("CSysApBtSapController::GetBtSapDeviceNameL:  CBTEngConnMan::GetConnectedAddresses() devices not found") ) );
            err = KErrNotFound; // discontinue name fetching
            }
        else
            {
            TRACES( RDebug::Print( _L("CSysApBtSapController::GetBtSapDeviceNameL:  CBTEngConnMan::GetConnectedAddresses() failed, err=%d"), err ) );
            }            
        }
    else
        {
        TRACES( RDebug::Print( _L("CSysApBtSapController::GetBtSapDeviceNameL:  CBTEngConnMan::NewL() failed, err=%d"), err ) );
        }        
    CleanupStack::PopAndDestroy( btConnMan );
    addrArray.Close();

    HBufC* prompt = NULL;

    if ( err == KErrNone )            
        {
        CBTEngDevMan* devMan = CBTEngDevMan::NewLC( NULL );
            
        TBTRegistrySearch criteria;
        criteria.FindAddress( address );
            
        CBTDeviceArray* devices = new (ELeave) CBTDeviceArray( 1 );
        CleanupStack::PushL( TCleanupItem( SysApCBTDeviceArrayCleanup, devices ) );
            
        err = devMan->GetDevices( criteria, devices );
            
        if ( err != KErrNone )
            {
            TRACES( RDebug::Print( _L("CSysApBtSapController::GetBtSapDeviceNameL CBTEngDevMan::GetDevices() returns %d"), err ) );
            }
        else
            {
            if ( devices->At( 0 )->IsValidFriendlyName() )
                {
                prompt = devices->At( 0 )->FriendlyName().AllocL();
                }
            else if ( devices->At( 0 )->IsValidDeviceName() )
                {
                prompt = (BTDeviceNameConverter::ToUnicodeL( devices->At( 0 )->DeviceName() ) ).AllocL();
                }    
            }        
            
        CleanupStack::PopAndDestroy( devices );
        CleanupStack::PopAndDestroy( devMan );
        }
    return prompt;        
    }

// End of File



