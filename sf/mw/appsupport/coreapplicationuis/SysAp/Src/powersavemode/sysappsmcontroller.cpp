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
* Description:  SysAp power save mode controller implementation
*
*/


#include <e32debug.h>
#include <psmclient.h>
#include <psmsettings.h>
#include <centralrepository.h>
#include <settingsinternalcrkeys.h>
#include "sysappsmcontroller.h"
#include "SysAp.hrh"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSysApPsmController::CSysApPsmController
// ---------------------------------------------------------------------------
//
CSysApPsmController::CSysApPsmController( MSysApPsmControllerNotifyCallback& aCallback ) : 
                                             iCallback( aCallback ),
                                             iPsmClient( NULL ),
                                             iActivePsm( EPsmsrvModeNormal ),
                                             iForcedPartialPsm( EFalse ),
                                             iUiItemId( 0 ),
                                             iNextUiOperation( EFalse ),
                                             iDiscardUiNotes( EFalse ),
                                             iBatteryLowCounter( 0 ),
                                             iChargerConnected( EFalse )
                                             
    {
    }

// ---------------------------------------------------------------------------
// CSysApPsmController::ConstructL
// ---------------------------------------------------------------------------
//
void CSysApPsmController::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApPsmController::ConstructL") ) );
    
    iPsmClient = CPsmClient::NewL( *this );

    // request explicitly to be notified if some other client changes power save mode
    iPsmClient->RequestPowerSaveModeNotification(); 
    
    TInt err = iPsmClient->PsmSettings().GetCurrentMode( iActivePsm );
    
    if ( err != KErrNone )
        {
        iActivePsm = EPsmsrvModeNormal;    
        }
    
    if ( iActivePsm == EPsmsrvPartialMode )
        {
        iForcedPartialPsm = ETrue;
        }
    

    TRACES( RDebug::Print( _L("CSysApPsmController::ConstructL: err=%d, iActivePsm=%d"), err, iActivePsm ) );
    }

// ---------------------------------------------------------------------------
// CSysApPsmController::NewL
// ---------------------------------------------------------------------------
//
CSysApPsmController* CSysApPsmController::NewL( MSysApPsmControllerNotifyCallback& aCallback )
    {
    CSysApPsmController* self = new( ELeave ) CSysApPsmController( aCallback );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CSysApPsmController::~CSysApPsmController
// ---------------------------------------------------------------------------
//
CSysApPsmController::~CSysApPsmController()
    {
    TRACES( RDebug::Print( _L("CSysApPsmController::~CSysApPsmController") ) );
    delete iPsmClient;
    }

// ---------------------------------------------------------------------------
// CSysApPsmController::SetPowerSaveMode
// ---------------------------------------------------------------------------
//
void CSysApPsmController::SetPowerSaveMode( TInt aNewMode )
    {
    TRACES( RDebug::Print( _L("CSysApPsmController::SetPowerSaveMode: aNewMode=%d"), aNewMode ) );

    iPsmClient->CancelPowerSaveModeNotificationRequest();
    iPsmClient->ChangePowerSaveMode( aNewMode );
    }

// ---------------------------------------------------------------------------
// CSysApPsmController::FullPsmEnabled
// ---------------------------------------------------------------------------
//
TBool CSysApPsmController::FullPsmEnabled() const
    {
    return iActivePsm == EPsmsrvModePowerSave;
    }
    
// ---------------------------------------------------------------------------
// CSysApPsmController::PartialPsmEnabled
// ---------------------------------------------------------------------------
//
TBool CSysApPsmController::PartialPsmEnabled() const
    {
    return iActivePsm == EPsmsrvPartialMode;
    }

// ---------------------------------------------------------------------------
// CSysApPsmController::UiItemId
// ---------------------------------------------------------------------------
//
TInt CSysApPsmController::UiItemId() const
    {
    return iUiItemId;    
    }

// ---------------------------------------------------------------------------
// CSysApPsmController::SetUiItemId
// ---------------------------------------------------------------------------
//
void CSysApPsmController::SetUiItemId( TInt aUiItemId )
    {
    iUiItemId = aUiItemId;
    }
    
// ---------------------------------------------------------------------------
// CSysApPsmController::DoEnablePartialPsm
// ---------------------------------------------------------------------------
//
void CSysApPsmController::DoEnablePartialPsm( TBool aEnable )
    {
    TRACES( RDebug::Print( _L("CSysApPsmController::DoEnablePartialPsm: aEnable=%d"), aEnable ) );

    // full PSM is only controlled by the user so toggling partial PSM is only possible between itself and normal mode
    if ( aEnable )
        {
        // activate only from normal mode
        if( iActivePsm == EPsmsrvModeNormal )
            {
            SetPowerSaveMode( EPsmsrvPartialMode );
            iDiscardUiNotes = ETrue; // partial mode errors not reflected to UI    
            }    
        }
    else
        {
        // deactivate only from partial mode
        if ( iActivePsm == EPsmsrvPartialMode ) 
            {
            SetPowerSaveMode( EPsmsrvModeNormal );
            iDiscardUiNotes = ETrue; // partial mode errors not reflected to UI    
            }   
        }
    }


// ---------------------------------------------------------------------------
// CSysApPsmController::BatteryLow
// ---------------------------------------------------------------------------
//
void CSysApPsmController::BatteryLow( TBool aBatteryIsLow )
    {
    TRACES( RDebug::Print( _L("CSysApPsmController::BatteryLow: aBatteryIsLow=%d"), aBatteryIsLow ) );

    if ( aBatteryIsLow )
    	{
    	iForcedPartialPsm = ETrue; 	// when battery is low, at least partial PSM must be active
		}
	else
		{
	    iForcedPartialPsm = EFalse; // releasing partial PSM when battery is not low

	    iBatteryLowCounter = 0; 	// show activation query on next battery low event
		}
    }

// ---------------------------------------------------------------------------
// CSysApPsmController::ConnectCharger
// ---------------------------------------------------------------------------
//
void CSysApPsmController::ConnectCharger( TBool aConnect )
    {
    TRACES( RDebug::Print( _L("CSysApPsmController::ConnectCharger: aConnect=%d"), aConnect ) );
    
    BatteryLow( EFalse );
    
    iChargerConnected = aConnect;
    }

// ---------------------------------------------------------------------------
// CSysApPsmController::ChargerConnected
// ---------------------------------------------------------------------------
//
TBool CSysApPsmController::ChargerConnected() const
    {
    return iChargerConnected;
    }
    
// ---------------------------------------------------------------------------
// CSysApPsmController::DoEnableFullPsm
// ---------------------------------------------------------------------------
//
void CSysApPsmController::DoEnableFullPsm( TBool aEnable )
    {
    TRACES( RDebug::Print( _L("CSysApPsmController::DoEnableFullPsm: aEnable=%d, iActivePsm=%d, iForcedPartialPsm=%d"), 
            aEnable, iActivePsm, iForcedPartialPsm ) );
    
    if ( aEnable && iActivePsm != EPsmsrvModePowerSave )
        {
        SetPowerSaveMode( EPsmsrvModePowerSave );
        }
    else if ( !aEnable && iActivePsm != EPsmsrvModeNormal )
        {
        SetPowerSaveMode( EPsmsrvModeNormal );    
        }
    }
    
// ---------------------------------------------------------------------------
// CCSysApPsmController::SetNextUiOperation
// ---------------------------------------------------------------------------
//
void CSysApPsmController::SetNextUiOperation( TBool aIsActivate )
    {
    TRACES( RDebug::Print( _L("CSysApPsmController::SetNextUiOperation: aIsActivate=%d"), aIsActivate ) );
    iNextUiOperation = aIsActivate;    
    }
    
// ---------------------------------------------------------------------------
// CCSysApPsmController::NextUiOperation
// ---------------------------------------------------------------------------
//
TBool CSysApPsmController::NextUiOperation() const
    {
    return iNextUiOperation;
    }


// ---------------------------------------------------------------------------
// CSysApPsmController::PowerSaveModeChanged
// from MPsmClientObserver
// ---------------------------------------------------------------------------
//
void CSysApPsmController::PowerSaveModeChanged( const TPsmsrvMode aMode )
    {
    TRACES( RDebug::Print( _L("CSysApPsmController::PowerSaveModeChanged: iActivePsm=%d, aMode=%d, iForcedPartialPsm"),
            iActivePsm, aMode, iForcedPartialPsm ) );
    
    iPsmClient->RequestPowerSaveModeNotification(); // must be requested after every completion
    
    iActivePsm = aMode;
    TBool showUiNotes = !iDiscardUiNotes;
    iDiscardUiNotes = EFalse; // set now because if partial PSM is activated it sets this back to ETrue
    
    switch ( iActivePsm )
        {
        case EPsmsrvModeNormal:
            if ( showUiNotes )
                {
                TRAP_IGNORE( iCallback.NotifyPowerSaveModeL( MSysApPsmControllerNotifyCallback::EPsmDeactivationComplete ) );    
                }
            
            if ( iForcedPartialPsm )
                {
                DoEnablePartialPsm( ETrue );
                }
            break;
        
        case EPsmsrvModePowerSave:
            if ( showUiNotes )
                {
                TRAP_IGNORE( iCallback.NotifyPowerSaveModeL( MSysApPsmControllerNotifyCallback::EPsmActivationComplete ) );    
                }
            break;

        case EPsmsrvPartialMode: // partial mode not shown in UI level        
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CSysApPsmController::PowerSaveModeChangeError
// from MPsmClientObserver
// ---------------------------------------------------------------------------
//
void CSysApPsmController::PowerSaveModeChangeError( const TInt aError )
    {
    TRACES( RDebug::Print( _L("CSysApPsmController::PowerSaveModeChangeError: aError=%d, iDiscardUiNotes=%d"),
            aError, iDiscardUiNotes ) );
    
    if ( aError != KErrCancel ) // ignore error if it is due cancellation of notification request
        {
        if ( aError != KErrServerTerminated ) // new request would cause another completion with this error code (infinitely)
            {
            iPsmClient->RequestPowerSaveModeNotification(); // must be requested after every completion
            }        

        if ( !iDiscardUiNotes ) // The user is not notified if activating partial mode fails
            {
            if ( iActivePsm == EPsmsrvModePowerSave )
                {
                TRAP_IGNORE( iCallback.NotifyPowerSaveModeL( MSysApPsmControllerNotifyCallback::EPsmDeactivationFailed ) );
                }
            else
                {
                TRAP_IGNORE( iCallback.NotifyPowerSaveModeL( MSysApPsmControllerNotifyCallback::EPsmActivationFailed ) );
                }    
            }
        iDiscardUiNotes = EFalse;
        }
    }


// ---------------------------------------------------------------------------
// CSysApPsmController::ShowActivateQuery
// ---------------------------------------------------------------------------
//
TBool CSysApPsmController::ShowActivateQuery()
    {
    TBool retVal( EFalse );
    
    // conditions
    // power saving is not yet activated
    // this is the first battery low warning
    if ( iBatteryLowCounter == 0 && 
         iActivePsm == EPsmsrvModeNormal )
        {
        //  query must be activated in CenRep setting
        retVal = QueriesEnabled();
        }
    
    iBatteryLowCounter++; // query must not be shown on subsequent low events
    return retVal;    
    }

// ---------------------------------------------------------------------------
// CSysApPsmController::ShowDeactivateQuery
// ---------------------------------------------------------------------------
//
TBool CSysApPsmController::ShowDeactivateQuery() const
    {
    TBool retVal( EFalse );
    
    // conditions
    // power saving has been activated
    // query has been activated in CenRep setting
    if ( iActivePsm == EPsmsrvModePowerSave )
        {
        //  query must be activated in CenRep setting
        retVal = QueriesEnabled();
        }
    
    return retVal;
    }

// ---------------------------------------------------------------------------
// CSysApPsmController::QueriesEnabled
// ---------------------------------------------------------------------------
//    
TBool CSysApPsmController::QueriesEnabled() const
    {
    TBool enabled( ETrue ); // enabled by default
    
    CRepository* repository( NULL );
    
    TRAPD( err, repository = CRepository::NewL( KCRUidDeviceManagementSettings ) );
    
    if ( err == KErrNone )
        {
        TInt value( 0 );
        err = repository->Get( KSettingsPowerSavingQuery, value );
        
        if ( err == KErrNone )
            {
            enabled = value ? ETrue : EFalse;
            }
        }
    
    delete repository;                        
    
    TRACES( RDebug::Print( _L("CSysApPsmController::QueriesEnabled: enabled=%d, err=%d"), enabled, err ) );
    
    return enabled;
    }
