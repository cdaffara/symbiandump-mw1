/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApConfirmationQuery implementation.
*
*/


// INCLUDES

#include <avkon.hrh>
#include <avkon.rsg>
#include "SysAp.hrh"
#include "SysApConfirmationQuery.h"
#include "SysApAppUi.h"
#include <StringLoader.h>
#include <avkon.hrh>
#include <avkon.rsg>
#include <SysAp.rsg>
#include <SecondaryDisplay/SecondaryDisplaySysApAPI.h>
#include <aknSDData.h>

// Constants
_LIT( KEmptyString, "" );



// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApConfirmationQuery::NewL() 
// ----------------------------------------------------------------------------
CSysApConfirmationQuery* CSysApConfirmationQuery::NewL( CSysApAppUi& aSysApAppUi )
    {
    CSysApConfirmationQuery* self = new(ELeave) CSysApConfirmationQuery( aSysApAppUi );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApConfirmationQuery::ConstructL() 
// ----------------------------------------------------------------------------
void CSysApConfirmationQuery::ConstructL()
    {
    CActiveScheduler::Add(this);
    iQuery = CAknGlobalConfirmationQuery::NewL();           
    iQueryId = ESysApNoQuery;
    }

// ----------------------------------------------------------------------------
// CSysApConfirmationQuery::CSysApConfirmationQuery( CSysApAppUi& aSysApAppUi )
// ----------------------------------------------------------------------------

CSysApConfirmationQuery::CSysApConfirmationQuery( CSysApAppUi& aSysApAppUi ) 
  : CActive( EPriorityStandard ), iSysApAppUi( aSysApAppUi )
    {
    }

// ----------------------------------------------------------------------------
// CSysApConfirmationQuery::~CSysApConfirmationQuery() 
// ----------------------------------------------------------------------------

CSysApConfirmationQuery::~CSysApConfirmationQuery() 
    {
    Cancel();
    delete iQuery;
    iQuery = NULL;
    }


// ----------------------------------------------------------------------------
// CSysApConfirmationQuery::DoCancel() 
// ----------------------------------------------------------------------------

void CSysApConfirmationQuery::DoCancel() 
    {
    TRACES( RDebug::Print( _L("CSysApConfirmationQuery::DoCancel()" ) ) );
    iQuery->CancelConfirmationQuery();
    }

// ----------------------------------------------------------------------------
// CSysApConfirmationQuery::ShowQueryL()
// ----------------------------------------------------------------------------

void CSysApConfirmationQuery::ShowQueryL( const TSysApConfirmationQueryIds aQueryId, CCoeEnv* aLoaderEnv )
    {
    ShowQueryL( aQueryId, KNullDesC, aLoaderEnv );
    }

// ----------------------------------------------------------------------------
// CSysApConfirmationQuery::ShowQueryL()
// ----------------------------------------------------------------------------

#ifdef RD_MULTIPLE_DRIVE
void CSysApConfirmationQuery::ShowQueryL( const TSysApConfirmationQueryIds aQueryId, const TDesC& aValue, CCoeEnv* aLoaderEnv )
#else // RD_MULTIPLE_DRIVE
void CSysApConfirmationQuery::ShowQueryL( const TSysApConfirmationQueryIds aQueryId,  const TDesC& /*aValue*/, CCoeEnv* aLoaderEnv )
#endif // RD_MULTIPLE_DRIVE
    {
    TRACES( RDebug::Print( _L("CSysApConfirmationQuery::ShowQueryL( %d )"), aQueryId ) );
    HBufC* queryStringBuf = NULL;
    HBufC* deviceNameBuf = NULL;
    CAknQueryDialog::TTone tone = CAknQueryDialog::ENoTone;    
    
    if( !IsActive() )
        {
        iQueryId = aQueryId;
        TInt secondaryDisplayCmdId(SecondaryDisplay::ECmdNoNote);
        TInt keys(0);
        TInt anim(0);        

        switch ( iQueryId )
            {
            case ESysApRestartPhoneQuery:
                queryStringBuf = StringLoader::LoadLC( R_QTN_RESTART_QUERY, aLoaderEnv );
                keys = R_AVKON_SOFTKEYS_OK_EMPTY;
                anim = R_QGN_NOTE_WARNING_ANIM;
                secondaryDisplayCmdId = SecondaryDisplay::ECmdShowRestartPhoneQuery;
                break;
            case ESysApLeaveOfflineModeQuery:
                queryStringBuf = StringLoader::LoadLC( R_QTN_LEAVE_OFFLINE_MODE_QUERY, aLoaderEnv );
                keys = R_AVKON_SOFTKEYS_YES_NO;
                secondaryDisplayCmdId = SecondaryDisplay::ECmdShowLeaveOfflineModeQuery;
                break;
#ifndef RD_MULTIPLE_DRIVE
            case ESysApRemoveMmcNote:
                queryStringBuf = StringLoader::LoadLC( R_QTN_MEMC_INFO_EJECT, aLoaderEnv );
                keys = R_AVKON_SOFTKEYS_OK_EMPTY;
                anim = R_QGN_NOTE_INFO_ANIM;
                secondaryDisplayCmdId = SecondaryDisplay::ECmdShowRemoveMmcQuery;
                break;
            case ESysApEjectMmcQuery:
                queryStringBuf = StringLoader::LoadLC( R_QTN_MEMC_CONF_EJECT, aLoaderEnv );
                keys = R_AVKON_SOFTKEYS_YES_NO;
                secondaryDisplayCmdId = SecondaryDisplay::ECmdShowEjectMmcQuery;
                break;
#else // RD_MULTIPLE_DRIVE
            case ESysApRemoveMmcNote:
                queryStringBuf = aValue.AllocLC();
                keys = R_AVKON_SOFTKEYS_OK_EMPTY;
                anim = R_QGN_NOTE_INFO_ANIM;
                secondaryDisplayCmdId = SecondaryDisplay::ECmdShowRemoveMmcQuery;
                break;
            case ESysApEjectMmcQuery:
                queryStringBuf = aValue.AllocLC();
                keys = R_AVKON_SOFTKEYS_YES_NO;
                secondaryDisplayCmdId = SecondaryDisplay::ECmdShowEjectMmcQuery;
                break;
#endif // RD_MULTIPLE_DRIVE
            case ESysApBtSapDisconnectQuery:
                deviceNameBuf = iSysApAppUi.GetBtSapDeviceNameL();
                if ( deviceNameBuf )
                    {
                    CleanupStack::PushL(deviceNameBuf);
                    queryStringBuf = StringLoader::LoadL( R_QTN_BT_DISCONN_FROM, *deviceNameBuf, aLoaderEnv );
                    }
                else
                    {
                    queryStringBuf = StringLoader::LoadL( R_QTN_BT_DISCONN_FROM, KEmptyString, aLoaderEnv ); 
                    }
                CleanupStack::PushL(queryStringBuf);
                keys = R_AVKON_SOFTKEYS_YES_NO;
                secondaryDisplayCmdId = SecondaryDisplay::ECmdShowBtSapDisconnectQuery;
                break;
            case ESysApMemoryCardLockedNote:
                iPendingQuery = ESysApMemoryCardLockedNote;
                queryStringBuf = StringLoader::LoadLC( R_QTN_MEMC_LOCKED_NOT_SUPPORTED, aLoaderEnv );
                keys = R_AVKON_SOFTKEYS_OK_EMPTY;
                anim = R_QGN_NOTE_INFO_ANIM;
                secondaryDisplayCmdId = SecondaryDisplay::ECmdShowMemoryCardLockedQuery;
                break;
/*            case ESysApUseFmTxInOfflineQuery:
                 queryStringBuf 
                     = StringLoader::LoadLC( R_QTN_FMTX_SYSAP_NOTE_ACTIVATE_IN_OFFLINE,
                                             aLoaderEnv );
                 keys = R_AVKON_SOFTKEYS_YES_NO;
                 secondaryDisplayCmdId = SecondaryDisplay::ECmdShowFmTxKeepOnInOfflineQuery;
                 break;
*/            case ESysApBattChargingPowerSavingQuery:
                queryStringBuf = StringLoader::LoadLC( R_QTN_BATTERY_CHARGING_POWER_SAVING_QUERY, aLoaderEnv );
                keys = R_AVKON_SOFTKEYS_YES_NO;
                secondaryDisplayCmdId = SecondaryDisplay::ECmdShowChargingDeactivatePowerSavingQuery;
                tone = static_cast<CAknQueryDialog::TTone>( EAvkonSIDChargingBatteryTone );                
                break;
            
            case ESysApBattLowPowerSavingQuery:
                queryStringBuf = StringLoader::LoadLC( R_QTN_BATTERY_CONF_QUERY, aLoaderEnv );
                keys = R_AVKON_SOFTKEYS_YES_NO;
                secondaryDisplayCmdId = SecondaryDisplay::ECmdShowBattLowActivatePowerSavingQuery;
                break;
            
            default:
                break;
            }

        if ( iSysApAppUi.CoverDisplaySupported() && secondaryDisplayCmdId != SecondaryDisplay::ECmdNoNote )
            {
            TRACES( RDebug::Print( _L("CSysApConfirmationQuery::ShowQueryL - Notifying secondary display") ) );
            CAknSDData* sd = NULL;
            if ( secondaryDisplayCmdId == SecondaryDisplay::ECmdShowBtSapDisconnectQuery )
                {
                SecondaryDisplay::TDeviceName sdDeviceName;
                sdDeviceName.Append(deviceNameBuf->Left(SecondaryDisplay::KMaxDeviceNameLen));
                SecondaryDisplay::TBtSapDisconnectQueryPckg pckg(sdDeviceName);
                sd = CAknSDData::NewL(SecondaryDisplay::KCatSysAp, SecondaryDisplay::ECmdShowBtSapDisconnectQuery, pckg);
                }
            else
                {
                sd = CAknSDData::NewL(SecondaryDisplay::KCatSysAp, secondaryDisplayCmdId, KNullDesC8);
                }
            iQuery->SetSecondaryDisplayData(sd); // ownership to notifier client
            }

        if ( keys && queryStringBuf )
            {
            if ( anim )
                {
                iQuery->ShowConfirmationQueryL( iStatus, 
                                                queryStringBuf->Des(), 
                                                keys,
                                                anim,
                                                KNullDesC,
                                                0,
                                                0,
                                                tone );
                }
            else
                {
                iQuery->ShowConfirmationQueryL( iStatus, 
                                                queryStringBuf->Des(), 
                                                keys,
                                                0,
                                                KNullDesC,
                                                0,
                                                0,
                                                tone );
                }
            }

        SetActive();
        }    
     else
        {
        // If another query is wanted when ESysApMemoryCardLockedNote is active, override it (Continues in RunL which handles cancel).
        if ( iQueryId == ESysApMemoryCardLockedNote && aQueryId != ESysApMemoryCardLockedNote )
            {
            iPendingQuery = aQueryId;
            iQuery->CancelConfirmationQuery();
            }
        }

    if ( queryStringBuf )
        {
        CleanupStack::PopAndDestroy( queryStringBuf ); // queryStringbuf
        }

    if ( deviceNameBuf )
        {
        CleanupStack::PopAndDestroy( deviceNameBuf ); // deviceNameBuf
        }

    }       

// ----------------------------------------------------------------------------
// CSysApConfirmationQuery::RunL() 
// ----------------------------------------------------------------------------

void CSysApConfirmationQuery::RunL() 
    {
    TRACES( RDebug::Print( _L( "CSysApConfirmationQuery::RunL: iQueryId = %d, iStatus = %d" ), iQueryId, iStatus.Int() ) );
    switch ( iQueryId ) 
        {
        case ESysApRestartPhoneQuery: 
            {    
            TRACES( RDebug::Print( _L( "CSysApConfirmationQuery::RunL: calling CSysApAppUi::DoShutdownL( ETrue, EDataRestoreReset )" ) ) );
            iSysApAppUi.DoShutdownL( ETrue, RStarterSession::EDataRestoreReset );
            }
            break;
        case ESysApLeaveOfflineModeQuery:                  
            if ( iStatus.Int() == EAknSoftkeyYes )
                {
                TRACES( RDebug::Print( _L( "CSysApConfirmationQuery::RunL: calling CSysApAppUi::ActivateOnlineProfileL()" ) ) );
                iSysApAppUi.ActivateOnlineProfileL();
                }
            break;
        case ESysApRemoveMmcNote:
            TRACES( RDebug::Print( _L( "CSysApConfirmationQuery::RunL: calling CSysApAppUi::MMCDismountedDialogConfirmed()" ) ) );
            iSysApAppUi.MMCDismountedDialogConfirmed();
            break;
        case ESysApEjectMmcQuery:
            if ( iStatus.Int() == EAknSoftkeyYes || iStatus.Int() == EEikBidOk )
                {
                TRACES( RDebug::Print( _L( "CSysApConfirmationQuery::RunL: calling CSysApAppUi::EjectMMCL()" ) ) );
                iSysApAppUi.EjectMMCL();
                }
            else
                {
                iSysApAppUi.EjectMMCCanceled();
                }
            break;
        case ESysApBtSapDisconnectQuery:
            if ( iStatus.Int() == EAknSoftkeyYes )
                {
                TRACES( RDebug::Print( _L( "CSysApConfirmationQuery::RunL: calling CSysApAppUi::DisconnectBtSap()" ) ) );
                iSysApAppUi.DisconnectBtSap();
                }
            break;
        case ESysApMemoryCardLockedNote:
            // In case of MMC locked note we are only interested about error, as it means there is overriding query
            if ( iStatus.Int() < 0 && iPendingQuery != ESysApMemoryCardLockedNote )
                {
                TRACES( RDebug::Print( _L( "CSysApConfirmationQuery::RunL: Canceled ESysApMemoryCardLockedNote because more important query" ) ) );
                ShowQueryL( iPendingQuery, CCoeEnv::Static() );
                }
            break;
/*         case ESysApUseFmTxInOfflineQuery:
             if ( iStatus.Int() == EAknSoftkeyNo )
                 {
                 TRACES( RDebug::Print( _L( "CSysApConfirmationQuery::RunL: calling CSysApAppUi::ChangeFmTxStateL( EFalse )" ) ) );
                 iSysApAppUi.ChangeFmTxStateL( EFalse ); // disable FM TX
                 }
             break;
*/         case ESysApBattChargingPowerSavingQuery:
            iSysApAppUi.HandleDeactivatePsmQueryResponse( iStatus.Int() == EAknSoftkeyYes );
            break;
            
         case ESysApBattLowPowerSavingQuery:
            iSysApAppUi.HandleActivatePsmQueryResponse( iStatus.Int() == EAknSoftkeyYes );
            break;           
         
         default:
            TRACES( RDebug::Print( _L( "CSysApConfirmationQuery::RunL: default" ) ) );
            break;
        }
    }

// ----------------------------------------------------------------------------
// CSysApConfirmationQuery::RunError( TInt aError )
// ----------------------------------------------------------------------------

#ifdef _DEBUG
TInt CSysApConfirmationQuery::RunError( TInt aError )
#else
TInt CSysApConfirmationQuery::RunError( TInt /* aError */ )
#endif
    {
    TRACES( RDebug::Print( _L( "CSysApConfirmationQuery::RunError: error code: %d " ), aError ) );
    return KErrNone;            
    }
    
// ----------------------------------------------------------------------------
// CSysApConfirmationQuery::CurrentQuery()
// ----------------------------------------------------------------------------

TInt CSysApConfirmationQuery::CurrentQuery()
    {
    TInt retval(ESysApNoQuery);
    
    if( IsActive() )
        {
        retval = iQueryId;
        }
        
    return retval;
    }

// End of File



