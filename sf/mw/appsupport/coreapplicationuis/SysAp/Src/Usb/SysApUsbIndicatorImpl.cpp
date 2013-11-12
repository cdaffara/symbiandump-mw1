/*
* Copyright (c) 2003-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApUsbIndicator implementation.
 *
*/


// INCLUDES

#include "SysApUsbIndicatorImpl.h"
#include "SysApAppUi.h"

// CONSTANTS

const TUint KSysApUsbAllStates = 0xFFFFFFFF;

MSysApUsbIndicator* CreateSysApUsbIndicatorL( CSysApAppUi& aSysApAppUi )
    {
    return CSysApUsbIndicator::NewL( aSysApAppUi );
    }

// ============================= MEMBER FUNCTIONS =============================

// ----------------------------------------------------------------------------
// CSysApUsbIndicator::RunL()
// ----------------------------------------------------------------------------
void CSysApUsbIndicator::RunL()
    {
    TRACES( RDebug::Print( _L("CSysApUsbIndicator::RunL: iDeviceState = %d, iStatus = %d"), iDeviceState, iStatus.Int() ) ); 

    iSysApAppUi.SetUsbAttachStatus( iDeviceState == EUsbDeviceStateAttached );
    TSysApUsbChargerDetector& sysApUsbChargingDetector = iSysApAppUi.UsbChargerDetector();
    if ( iDeviceState == EUsbDeviceStateConfigured )
        {
        if ( !iIndicatorOn )
            {
            sysApUsbChargingDetector.SetUsbDeviceUsed( ETrue );
            iIndicatorOn = ETrue;
            }
        }
    else if ( iDeviceState != EUsbDeviceStateSuspended )
        {
        if ( iIndicatorOn )
            {
            if ( !sysApUsbChargingDetector.HostOnlyUsbChargingUsed() )
                {
                // Reset the detector here if no charging activity appeared.
                // Otherwise it is used and reset in charging handling.
                sysApUsbChargingDetector.Reset();
                }
            iIndicatorOn = EFalse;
            }
        }

    iUsbMan.DeviceStateNotification( KSysApUsbAllStates, iDeviceState, iStatus );
    SetActive();
    }

// ----------------------------------------------------------------------------
// CSysApUsbIndicator::RunError( TInt aError )
// ----------------------------------------------------------------------------

TInt CSysApUsbIndicator::RunError( TInt /* aError */ )
    {
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// CSysApUsbIndicator::DoCancel()
// ----------------------------------------------------------------------------
void CSysApUsbIndicator::DoCancel()
    {
    iUsbMan.DeviceStateNotificationCancel();
    }

// ----------------------------------------------------------------------------
// CSysApUsbIndicator::NewL() 
// ----------------------------------------------------------------------------
CSysApUsbIndicator* CSysApUsbIndicator::NewL( CSysApAppUi& aSysApAppUi )
    {
    CSysApUsbIndicator* self = new (ELeave) CSysApUsbIndicator( aSysApAppUi );
    TRAPD( err, self->ConstructL() );
    if ( err )
        {
        TRACES( RDebug::Print ( _L("CSysApUsbIndicator::ConstructL: ERROR: %d"), err ) );
        }
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApUsbIndicator::CSysApUsbIndicator() 
// ----------------------------------------------------------------------------
CSysApUsbIndicator::CSysApUsbIndicator( CSysApAppUi& aSysApAppUi ) :
  CActive( EPriorityStandard ),
  iSysApAppUi( aSysApAppUi )
    {
    CActiveScheduler::Add( this );
    }

// ----------------------------------------------------------------------------
// CSysApUsbIndicator::ConstructL() 
// ----------------------------------------------------------------------------
void CSysApUsbIndicator::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApUsbIndicator::ConstructL() starts") ) ); 
    TInt err ( 0 );
    err = iUsbMan.Connect();
    TRACES( RDebug::Print ( _L("CSysApUsbIndicator::ConstructL: RUsb::Connect() returned %d "), err ) );
    if ( !err )
        {
        iUsbMan.GetDeviceState( iDeviceState );
        TRACES( RDebug::Print( _L("CSysApUsbIndicator::ConstructL: iDeviceState = %d"), iDeviceState ) );
        
        if ( iDeviceState == EUsbDeviceStateConfigured )
            {
            iSysApAppUi.UsbChargerDetector().SetUsbDeviceUsed( ETrue );
            iIndicatorOn = ETrue;
            }
        else
            {
            iIndicatorOn = EFalse;
            }
        
        iUsbMan.DeviceStateNotification( KSysApUsbAllStates, iDeviceState, iStatus );
        SetActive();
        }
    TRACES( RDebug::Print( _L("CSysApUsbIndicator::ConstructL() ends") ) ); 
    }

// ----------------------------------------------------------------------------
// CSysApUsbIndicator::~CSysApUsbIndicator()
// ----------------------------------------------------------------------------

CSysApUsbIndicator::~CSysApUsbIndicator()
    {
    Cancel();
    iUsbMan.Close();
    }

// End of File



