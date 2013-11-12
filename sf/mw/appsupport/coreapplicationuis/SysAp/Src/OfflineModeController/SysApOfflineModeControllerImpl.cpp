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
 * CSysApOfflineModeController controls the offline mode.
 * It turns OFF RF, BT and WLAN when offline mode is enabled and turns them ON when
 * Offline mode is disabled
 *
 */


// INCLUDES

#include "SysApOfflineModeControllerImpl.h"
#include "SysApAppUi.h"
#include "SysAp.hrh"
#include "DosSvrServices.h"
#include "SysApCenRepController.h"
#include "sysapcenrepofflinemodeobserver.h"
#include <startupdomainpskeys.h>
#include <PSVariables.h>
#include <wlandevicesettingsinternalcrkeys.h> // For Turning ON/OFF WLAN


MSysApOfflineModeController* CreateSysApOfflineModeControllerL( CSysApAppUi& aSysApAppUi )
    {
    return CSysApOfflineModeController::NewL( aSysApAppUi );
    }

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::NewL() 
// ----------------------------------------------------------------------------
CSysApOfflineModeController* CSysApOfflineModeController::NewL( CSysApAppUi& aSysApAppUi )
    {
    TRACES( RDebug::Print( _L("CSysApOfflineModeController::NewL") ) ); 
    CSysApOfflineModeController* self = new(ELeave) CSysApOfflineModeController( aSysApAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::ConstructL() 
// ----------------------------------------------------------------------------
void CSysApOfflineModeController::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApOfflineModeController::ConstructL") ) ); 
    
    // create an observer for Offline Mode
    TRACES( RDebug::Print( _L("CSysApOfflineModeController::ConstructL: trying CSysApCenRepOfflineModeObserver::NewL") ) );
    iSysApCenRepOfflineModeObserver = CSysApCenRepOfflineModeObserver::NewL(*this );         
    iOfflineModeActive = iSysApCenRepOfflineModeObserver->IsOffline();
    }

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::CSysApOfflineModeController() 
// ----------------------------------------------------------------------------
CSysApOfflineModeController::CSysApOfflineModeController( CSysApAppUi& aSysApAppUi ) :
  iSysApAppUi( aSysApAppUi )
    {

    }

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::~CSysApOfflineModeController()
// ----------------------------------------------------------------------------

CSysApOfflineModeController::~CSysApOfflineModeController()
    {
    delete iSysApCenRepOfflineModeObserver;
    }

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::OfflineModeActive()
// ----------------------------------------------------------------------------

TBool CSysApOfflineModeController::OfflineModeActive()
    {
    TRACES( RDebug::Print( _L("CSysApOfflineModeController::OfflineModeActive returns %d"), iOfflineModeActive ) );
    return iOfflineModeActive;
    }

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::SwitchFromOnlineToOfflineModeL()
// ----------------------------------------------------------------------------

void CSysApOfflineModeController::SwitchFromOnlineToOfflineModeL()
    {
    TRACES( RDebug::Print( _L("CSysApOfflineModeController::SwitchFromOnlineToOfflineModeL") ) ); 
        
    TInt err ( 0 );

    // Set state to starter. Note that if state is same as the current state, then nothing happens.
    err = iSysApAppUi.SetStarterState(RStarterSession::EOffline);

    if ( err )
        {
        iSysApAppUi.ShowUiNoteL( ECannotActivateOfflineModeNote );
        }
    else
        {
        iOfflineModeActive = ETrue;
        iSysApAppUi.SetNetworkConnectionAllowed( ECoreAppUIsNetworkConnectionNotAllowed );

        // Switch BT off
        if ( iSysApAppUi.BluetoothPowerMode() == KBluetoothModulePowerModeOn )
            {
            err = iSysApAppUi.SetBtPowerState( EFalse ); // Switch BT off
            SetBtActiveBeforeOfflineMode( ETrue );       // Update "BT active" setting
            TRACES( RDebug::Print( _L("CSysApOfflineModeController SetBtPowerStateL returned %d"), err ) );
            }
        /* Turn OFF WLAN */        
        TInt err( KErrNone );
        CRepository* repository = CRepository::NewL( KCRUidWlanDeviceSettingsRegistryId );
        err = repository->Set(KWlanOnOff, 0);
        delete repository;
        if(err) 
		{
           TRACES( RDebug::Printf("CSysApOfflineModeController::SwitchFromOnlineToOfflineModeL Could not disable WLAN") );
		    User::Leave(err);
        }
        else 
		{
            TRACES( RDebug::Printf("CSysApOfflineModeController::SwitchFromOnlineToOfflineModeL WLAN Disabled" ) );
        }
		
		iSysApAppUi.OfflineModeChangedL();
        }
    }

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::SwitchFromOfflineToOnlineModeL()
// ----------------------------------------------------------------------------

void CSysApOfflineModeController::SwitchFromOfflineToOnlineModeL()
    {
    TRACES( RDebug::Print( _L("CSysApOfflineModeController::SwitchFromOfflineToOnlineModeL") ) );

    TInt err ( 0 );

    // Set state to starter. Note that if state is same as the current state, then nothing happens.
    // In some cases (like BtSap) it might be necessary to delay setting starter state to normal.
    // In these cases state is set to normal by some other handler later when appropriate.
    if ( !iDoNotActivateRF )
        {
        err = iSysApAppUi.SetStarterState(RStarterSession::ENormal);
        }
        
    if ( err )
        {
        iSysApAppUi.ShowUiNoteL( ECannotDeactivateOfflineModeNote );
        }
    else
        {
        iOfflineModeActive = EFalse;

        // Switch BT on
        if ( BtActiveBeforeOfflineMode() && iSysApAppUi.BluetoothPowerMode() != KBluetoothModulePowerModeOn )
            {
            err = iSysApAppUi.SetBtPowerState( ETrue );  // Switch BT on
            SetBtActiveBeforeOfflineMode( EFalse );      // Update "BT active" setting
            TRACES( RDebug::Print( _L("CSysApOfflineModeController SetBtPowerStateL returned %d"), err ) );
            }
        
		        /* Turn ON WLAN */    
           TInt err( KErrNone );
           CRepository* repository = CRepository::NewL( KCRUidWlanDeviceSettingsRegistryId );
           err = repository->Set(KWlanOnOff, 1);
           delete repository;
           if(err)
		   {
               TRACES( RDebug::Printf("CSysApOfflineModeController::SwitchFromOfflineToOnlineModeL Could not enable WLAN") );
		       User::Leave(err);
           }
           else
		   {
               TRACES( RDebug::Printf("CSysApOfflineModeController::SwitchFromOfflineToOnlineModeL WLAN Enabled") );
           }

        iSysApAppUi.OfflineModeChangedL();
		iSysApAppUi.SetNetworkConnectionAllowed( ECoreAppUIsNetworkConnectionAllowed );
		}
	}

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::CheckOfflineModeInitialStatusesL()
// ----------------------------------------------------------------------------

void CSysApOfflineModeController::CheckOfflineModeInitialStatusesL()
    {
    TRACES( RDebug::Print( _L("CSysApOfflineModeController::CheckOfflineModeInitialStatusesL") ) ); 
    }

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::DoNotActivateBt()
// ----------------------------------------------------------------------------
void CSysApOfflineModeController::DoNotActivateBt()
    {
    TRACES( RDebug::Print( _L("CSysApOfflineModeController::DoNotActivateBt") ) ); 
    SetBtActiveBeforeOfflineMode( EFalse );
    }

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::DoNotActivateRF()
// ----------------------------------------------------------------------------
void CSysApOfflineModeController::DoNotActivateRF()
    {
    TRACES( RDebug::Print( _L("CSysApOfflineModeController::DoNotActivateRF") ) ); 
    iDoNotActivateRF = ETrue;
    }

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::GoOnlineIfOkL()
// ----------------------------------------------------------------------------
void CSysApOfflineModeController::GoOnlineIfOkL()
    {
    TRACES( RDebug::Print( _L("CSysApOfflineModeController::GoOnlineIfOkL") ) ); 

    TInt simStatus = iSysApAppUi.StateOfProperty( KPSUidStartup, KPSSimStatus );
    if ( ( !iSysApAppUi.SimSupported() ) ||
         ( iSysApAppUi.BtSapEnabled() && simStatus != ESimNotPresent ) ||     
         simStatus == ESimUsable )
        {
        TRACES( RDebug::Printf("CSysApOfflineModeController::GoOnlineIfOkL SIM check PASSED") );
        SwitchFromOfflineToOnlineModeL();
        }
    else    
        {
        TRACES( RDebug::Printf("CSysApOfflineModeController::GoOnlineIfOkL SIM check FALIED" ) );
        iSysApAppUi.ShowUiNoteL( EInsertSimNote );
        }
    }

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::SetBtActiveBeforeOfflineMode()
// ----------------------------------------------------------------------------

void CSysApOfflineModeController::SetBtActiveBeforeOfflineMode( TBool aBtActive )
    {
    TRACES( RDebug::Print( _L("CSysApOfflineModeController::SetBtActiveBeforeOfflineMode: status: %d" ), aBtActive ) );
    iSysApAppUi.CenRepController().SetInt( KCRUidCoreApplicationUIsSysAp, KSysApBtStatusBeforeOfflineMode, (TInt) aBtActive );
    }

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::BtActiveBeforeOfflineMode()
// ----------------------------------------------------------------------------
TBool CSysApOfflineModeController::BtActiveBeforeOfflineMode()
    {

    TBool err = EFalse;
    
    TInt btToBeActivated = iSysApAppUi.CenRepController().GetInt( KCRUidCoreApplicationUIsSysAp,
                                                                  KSysApBtStatusBeforeOfflineMode,
                                                                  &err );

    if ( err )
        {
        TRACES( RDebug::Print( _L("CSysApOfflineModeController::BtActiveBeforeOfflineMode: ERROR: %d" ), err ) );
        return EFalse;
        }
    else
        {
        return ( TBool ) btToBeActivated;
        //return ETrue;
        }

    }

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::MustBtBeActivated()
// ----------------------------------------------------------------------------
TBool CSysApOfflineModeController::MustBtBeActivated()
    {
    return BtActiveBeforeOfflineMode();
    }


// End of File



