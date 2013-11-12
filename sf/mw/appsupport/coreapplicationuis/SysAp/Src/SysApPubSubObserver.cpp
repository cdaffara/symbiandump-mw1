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
* Description:  CSysApPubSubObserver implementation
*
*/


// INCLUDES
#include <e32property.h>
#include <sacls.h>
#include <startupdomainpskeys.h>
#include "coreapplicationuisprivatepskeys.h"
#include <ctsydomainpskeys.h>
#include <wlaninternalpskeys.h>
#include <connect/sbdefs.h>
#include <hwrmpowerstatesdkpskeys.h>
#include "SysApPubSubObserver.h"
#include "SysApSubscriber.h"
#include "SysApAppUi.h"
#include "SysApFeatureManager.h"
#include <ir_sock.h>
#include <networkhandlingdomainpskeys.h>
#include <hwrmdomainpskeys.h>
#include <DataSyncInternalPSKeys.h>
//#include <hwrmfmtxdomainpskeys.h>
//#include <hwrmfmtx.h>
#include "sysapremconobserver.h"
#include <lbs/locationfwdomainpskeys.h>
#include <smsuaddr.h>
#include <avkondomainpskeys.h>

#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS
  #include <ASShdAlarm.h>
#else
  #include <asshddefs.h>
  #include <ASShdAlarmCal.h>
  #include <asshdalarm.h>
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS

// CONSTANTS
const TUint KCoreAppUIsMmcRemovedWithoutEjectCmdMask = 0xFFFF;
const TUint KCoreAppUIsMmcRemovedWithoutEjectValShift = 16;

_LIT(KGpsIndicatorPlugin, "com.nokia.positioning.indicatorplugin/1.0");
const TUid KPSUidSecurityUIs = { 0x100059b5 };
const TUint32 KSecurityUIsLights  = 0x00000308;

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApPubSubObserver::NewL() 
// ----------------------------------------------------------------------------
CSysApPubSubObserver* CSysApPubSubObserver::NewL( CSysApAppUi& aSysApAppUi )
    {
    CSysApPubSubObserver* self = new(ELeave) CSysApPubSubObserver( aSysApAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApPubSubObserver::CSysApPubSubObserver() 
// ----------------------------------------------------------------------------
CSysApPubSubObserver::CSysApPubSubObserver( CSysApAppUi& aSysApAppUi ) :
  iSysApAppUi( aSysApAppUi ),
  iRestoreOngoing( EFalse )
    {
    }

// ----------------------------------------------------------------------------
// CSysApPubSubObserver::ConstructL() 
// ----------------------------------------------------------------------------
void CSysApPubSubObserver::ConstructL()
    {    
    // Category KUidSystemCategory
    iBatteryBarsSubscriber = CSysApSubscriber::NewL( *this, KPSUidHWRMPowerState, KHWRMBatteryLevel );
    iBatteryBarsSubscriber->Subscribe();
    iBatteryCStatusSubscriber = CSysApSubscriber::NewL( *this, KPSUidHWRMPowerState, KHWRMBatteryStatus ); 
    iBatteryCStatusSubscriber->Subscribe();
    iChargerCStatusSubscriber = CSysApSubscriber::NewL( *this, KPSUidHWRMPowerState, KHWRMChargingStatus ); 
    iChargerCStatusSubscriber->Subscribe();
    iInboxStatusSubscriber = CSysApSubscriber::NewL( *this, KUidSystemCategory, KUidInboxStatusValue ); 
    iInboxStatusSubscriber->Subscribe();
    iOutboxStatusSubscriber = CSysApSubscriber::NewL( *this, KUidSystemCategory, KUidOutboxStatusValue ); 
    iOutboxStatusSubscriber->Subscribe();
    iIrdaStatusSubscriber = CSysApSubscriber::NewL( *this, KIrdaPropertyCategory, KIrdaStatus );
    iIrdaStatusSubscriber->Subscribe();
//    iGprsStatusSubscriber = CSysApSubscriber::NewL( *this, KUidSystemCategory, KPSUidGprsStatusValue );
//    iGprsStatusSubscriber->Subscribe();
//    iWcdmaStatusSubscriber = CSysApSubscriber::NewL( *this, KUidSystemCategory, KPSUidWcdmaStatusValue ); 
//    iWcdmaStatusSubscriber->Subscribe();
    iLocationGlobalPrivacySubscriber = CSysApSubscriber::NewL( *this, KUidSystemCategory, KPSUidLocationGlobalPrivacyValue );
    iLocationGlobalPrivacySubscriber->Subscribe();
    iRestoreBackupSubscriber = CSysApSubscriber::NewL( *this, KUidSystemCategory, conn::KUidBackupRestoreKey );
    iRestoreBackupSubscriber->Subscribe();

    // Category KPSUidStartup
    iSimChangedSubscriber = CSysApSubscriber::NewL( *this, KPSUidStartup, KPSSimOwned );
    iSimChangedSubscriber->Subscribe();
    iWakeupAlarmSubscriber = CSysApSubscriber::NewL( *this, KAlarmServerPubSubCategory, KWakeupAlarmPubSubKey );
    iWakeupAlarmSubscriber->Subscribe();

    // Category KPSUidCoreApplicationUIs
    iAlarmLightSubscriber = CSysApSubscriber::NewL( *this, KPSUidCoreApplicationUIs, KLightsAlarmLightActive );
    iAlarmLightSubscriber->Subscribe();
    iVTLightsSubscriber = CSysApSubscriber::NewL( *this, KPSUidCoreApplicationUIs, KLightsVTForcedLightsOn ); 
    iVTLightsSubscriber->Subscribe();
    iSSLightsSubscriber = CSysApSubscriber::NewL( *this, KPSUidCoreApplicationUIs, KLightsSSForcedLightsOn );
    iSSLightsSubscriber->Subscribe();
    iATCLightsSubscriber = CSysApSubscriber::NewL( *this, KPSUidCoreApplicationUIs, KLightsATCForcedLightsOn );
    iATCLightsSubscriber->Subscribe();
    iLightsControlSubscriber = CSysApSubscriber::NewL( *this, KPSUidCoreApplicationUIs, KLightsControl );
    iLightsControlSubscriber->Subscribe();
    iDisableKeyguardSubscriber = CSysApSubscriber::NewL( *this, KPSUidCoreApplicationUIs, KCoreAppUIsDisableKeyguard );
    iDisableKeyguardSubscriber->Subscribe();
    iSoftRejectSubscriber = CSysApSubscriber::NewL( *this, KPSUidCoreApplicationUIs, KCoreAppUIsSoftReject );
    iSoftRejectSubscriber->Subscribe();
    iUipIndSubscriber = CSysApSubscriber::NewL( *this, KPSUidCoreApplicationUIs, KCoreAppUIsUipInd );
    iUipIndSubscriber->Subscribe();
    iMessageToneQuitSubscriber = CSysApSubscriber::NewL( *this, KPSUidCoreApplicationUIs, KCoreAppUIsMessageToneQuit );
    iMessageToneQuitSubscriber->Subscribe();
    iNspsRawKeyEventSubscriber = CSysApSubscriber::NewL( *this, KPSUidCoreApplicationUIs, KCoreAppUIsNspsRawKeyEvent );
    iNspsRawKeyEventSubscriber->Subscribe();
    iLightsRawKeyEventSubscriber = CSysApSubscriber::NewL( *this, KPSUidCoreApplicationUIs, KCoreAppUIsLightsRawKeyEvent );
    iLightsRawKeyEventSubscriber->Subscribe();
    iMmcRemovedWithoutEjectSubscriber = CSysApSubscriber::NewL( *this, KPSUidCoreApplicationUIs, KCoreAppUIsMmcRemovedWithoutEject );
    iMmcRemovedWithoutEjectSubscriber->Subscribe();
    iNewEmailStatusSubscriber = CSysApSubscriber::NewL( *this, KPSUidCoreApplicationUIs, KCoreAppUIsNewEmailStatus );
    iNewEmailStatusSubscriber->Subscribe();
    iAutolockStatusSubscriber = CSysApSubscriber::NewL( *this, KPSUidCoreApplicationUIs, KCoreAppUIsAutolockStatus );
    iAutolockStatusSubscriber->Subscribe();
    iPoCIndicatorSubscriber = CSysApSubscriber::NewL( *this, KPSUidCoreApplicationUIs, KCoreAppUIsPoCIndicator );
    iPoCIndicatorSubscriber->Subscribe();
    iPoCMissedIndicatorSubscriber = CSysApSubscriber::NewL( *this, KPSUidCoreApplicationUIs, KCoreAppUIsPoCMissedIndicator );
    iPoCMissedIndicatorSubscriber->Subscribe();
    iTarmStatusSubscriber = CSysApSubscriber::NewL( *this, KPSUidCoreApplicationUIs, KCoreAppUIsTarmIndicator );
    iTarmStatusSubscriber->Subscribe();
    iMtvRecStatusSubscriber = CSysApSubscriber::NewL( *this, KPSUidCoreApplicationUIs, KCoreAppUIsMtvRecStatus );
    iMtvRecStatusSubscriber->Subscribe();
    iMtvDvbhStatusSubscriber = CSysApSubscriber::NewL( *this, KPSUidCoreApplicationUIs, KCoreAppUIsMtvDvbhStatus );
    iMtvDvbhStatusSubscriber->Subscribe();
    
    // Category KPSUidWlan
    iWlanIndicatorSubscriber = CSysApSubscriber::NewL( *this, KPSUidWlan, KPSWlanIndicator );
    iWlanIndicatorSubscriber->Subscribe();

    // KPSUidCtsyCallInformation
    iCallInfoSubscriber = CSysApSubscriber::NewL( *this, KPSUidCtsyCallInformation, KCTsyCallState );
    iCallInfoSubscriber->Subscribe();

    // Category KPSUidNetworkInfo
    iHomeZoneStatusSubscriber = CSysApSubscriber::NewL( *this, KPSUidNetworkInfo, KNWHomeZoneStatus ); 
    iHomeZoneStatusSubscriber->Subscribe();
    iNetworkModeSubscriber = CSysApSubscriber::NewL( *this, KPSUidNetworkInfo, KNWTelephonyNetworkMode );
    iNetworkModeSubscriber->Subscribe();

    // Category KPSUidHWRM
    iFlipStatusSubscriber = CSysApSubscriber::NewL( *this, KPSUidHWRM, KHWRMFlipStatus );
    iFlipStatusSubscriber->Subscribe();
    
/*    if ( iSysApAppUi.SysApFeatureManager().FmTxSupported() )
        {
        // Category KHWRMFmTxStatus
        iFmTxStatusSubscriber = CSysApSubscriber::NewL( *this, KPSUidHWRMFmTx, KHWRMFmTxStatus );
        iFmTxStatusSubscriber->Subscribe();
        iPreviousFmTxPSValue = EFmTxStateUnknown;    
        }
*/
    iSyncStatusSubscriber = CSysApSubscriber::NewL( *this, KPSUidDataSynchronizationInternalKeys, KDataSyncStatus );
    iSyncStatusSubscriber->Subscribe();
    
    iVideoSharingIndicatorSubscriber = CSysApSubscriber::NewL( *this, KPSUidCoreApplicationUIs, KCoreAppUIsVideoSharingIndicator );
    iVideoSharingIndicatorSubscriber->Subscribe();
    
    iGpsIndicatorSubscriber = CSysApSubscriber::NewL( *this, KPosIndicatorCategoryUid, KPosIntGpsHwStatus );
    iGpsIndicatorSubscriber->Subscribe();

    iSmsPhoneStoreSubscriber = CSysApSubscriber::NewL( *this, KUidPSSMSStackCategory, KUidPSSMSStackDiskSpaceMonitorKey );
    iSmsPhoneStoreSubscriber->Subscribe();
    
    iKeylockEventSubscriber = CSysApSubscriber::NewL( *this, KPSUidAvkonDomain, KAknKeyguardStatus );
    iKeylockEventSubscriber->Subscribe();
    
    iLightsOnRequestSubscriber = CSysApSubscriber::NewL( *this, KPSUidSecurityUIs, KSecurityUIsLights );
    iLightsOnRequestSubscriber->Subscribe();
    
    iPowerMenuCustomDialogSubscriber = CSysApSubscriber::NewL( *this, KPSUidCoreApplicationUIs, KCoreAppUIsPowerMenuCustomDialogStatus );
    iPowerMenuCustomDialogSubscriber->Subscribe();
    }

// ----------------------------------------------------------------------------
// CSysApPubSubObserver::~CSysApPubSubObserver() 
// ----------------------------------------------------------------------------

CSysApPubSubObserver::~CSysApPubSubObserver()
    {
    delete iAutolockStatusSubscriber;
    delete iBatteryBarsSubscriber;
    delete iBatteryLowIndicationSubscriber;
    delete iBatteryCStatusSubscriber;
    delete iChargerCStatusSubscriber; 
    delete iCallInfoSubscriber;
    delete iInboxStatusSubscriber;
    delete iOutboxStatusSubscriber; 
    delete iIrdaStatusSubscriber;
//    delete iGprsStatusSubscriber; 
//    delete iWcdmaStatusSubscriber;
    delete iHomeZoneStatusSubscriber;
    delete iNewEmailStatusSubscriber; 
    delete iLocationGlobalPrivacySubscriber;
    delete iRestoreBackupSubscriber;
    delete iPoCIndicatorSubscriber;
    delete iPoCMissedIndicatorSubscriber;
    delete iTarmStatusSubscriber;
    delete iMtvRecStatusSubscriber;
    delete iMtvDvbhStatusSubscriber;
    delete iSimChangedSubscriber;
    delete iWakeupAlarmSubscriber;
    delete iAlarmLightSubscriber;
    delete iVTLightsSubscriber; 
    delete iSSLightsSubscriber;
    delete iATCLightsSubscriber;
    delete iLightsControlSubscriber;
    delete iDisableKeyguardSubscriber;
    delete iSoftRejectSubscriber;
    delete iUipIndSubscriber;
    delete iMessageToneQuitSubscriber;
    delete iNspsRawKeyEventSubscriber;
    delete iLightsRawKeyEventSubscriber;
    delete iMmcRemovedWithoutEjectSubscriber;
    delete iNetworkModeSubscriber;
    delete iWlanIndicatorSubscriber;
    delete iFlipStatusSubscriber;
//    delete iFmTxStatusSubscriber;
//    delete iFmTxRemConObserver;
    delete iSyncStatusSubscriber;
    delete iVideoSharingIndicatorSubscriber;
    delete iGpsIndicatorSubscriber;
    delete iSmsPhoneStoreSubscriber;
    delete iKeylockEventSubscriber;
    delete iLightsOnRequestSubscriber;
    delete iPowerMenuCustomDialogSubscriber;
    }


// ----------------------------------------------------------------------------
// CSysApPubSubObserver::HandlePropertyChangedL()
// ----------------------------------------------------------------------------
void CSysApPubSubObserver::HandlePropertyChangedL( const TUid& aCategory, const TUint aKey )
    {
    TInt value( 0 );
    RProperty::Get( aCategory, aKey, value );

#ifdef _DEBUG

    HBufC* hbuf = HBufC::New( 120 );

    if ( hbuf )
        {
        TPtr buf = hbuf->Des();

        buf = _L("CSysApPubSubObserver::HandlePropertyChangedL: category=0x");
        buf.AppendNumFixedWidth( aCategory.iUid, EHex, 8 );
        buf.Append( _L(", key=0x") );
        buf.AppendNumFixedWidth( aKey, EHex, 8 );
        buf.Append( _L(", value=") );
        buf.AppendNum( value );

        RDebug::Print( buf );
        delete hbuf;
        }

#endif

    // KIrdaPropertyCategory is actually defined as KUidSystemCategory, but just in case this ever changes,
    // check them separately here. The keys should have no overlap.
    if ( aCategory == KUidSystemCategory || aCategory == KIrdaPropertyCategory )
        {
        HandleSystemCategoryL( aKey, value );
        }
    else if ( aCategory == KPSUidStartup || aCategory == KAlarmServerPubSubCategory )
        {
        HandleStartupCategoryL( aKey, value );
        }
    else if ( aCategory == KPSUidCoreApplicationUIs )
        {
        HandleCoreAppUIsCategoryL( aKey, value );
        }
    else if ( aCategory == KPSUidHWRMPowerState )
        {
        HandleHwrmPowerStateCategoryL( aKey, value );
        }
    else if ( aCategory == KPSUidWlan )
        {
        HandleWlanCategoryL( aKey, value );
        }
    else if ( aCategory == KPSUidCtsyCallInformation )
        {
        HandleTelephonyCallInformationCategoryL( aKey, value );
        }
    else if ( aCategory == KPSUidNetworkInfo )
        {
        HandleNetworkInfoCategoryL( aKey, value );
        }
    else if ( aCategory == KPSUidHWRM )
        {
        HandleHwrmCategoryL( aKey, value );
        }
/*    else if ( aCategory == KPSUidHWRMFmTx )
        {
        HandleHwrmFmTxCategoryL( aKey, value );
        }
*/
    else if ( aCategory == KPSUidDataSynchronizationInternalKeys )
        {
        HandleDataSyncCategoryL( aKey, value );
        }
    else if ( aCategory == KPosIndicatorCategoryUid )
        {
        HandlePosIndicatorCategoryL( aKey, value );
        }
    else if ( aCategory == KUidPSSMSStackCategory )
        {
        HandleSmsStackCategoryL( aKey, value );
        }
    else if ( aCategory == KPSUidAvkonDomain)
        {
        HandleLKeylockEventL( aKey, value );
        }
    else if ( aCategory == KPSUidSecurityUIs)
        {
        HandleLightsOnRequestL( aKey, value );
        }
    }
// ----------------------------------------------------------------------------
// CSysApPubSubObserver::HandleCoreAppUIsCategoryL()
// ----------------------------------------------------------------------------
void CSysApPubSubObserver::HandleCoreAppUIsCategoryL( const TUint aKey, const TInt aValue )
    {
    switch ( aKey )
        {
        case KLightsAlarmLightActive:
            if ( aValue == ELightsBlinking )
                {
                iSysApAppUi.AlertUiAlertingL( ETrue );
                }
            else if ( aValue == ELightsNotBlinking )
                {
                iSysApAppUi.AlertUiAlertingL( EFalse );
                }
            break;
        case KLightsVTForcedLightsOn:
            iSysApAppUi.HandleForcedLightsVTRequireL( aValue );
            break;
        case KLightsSSForcedLightsOn:
            iSysApAppUi.HandleForcedLightsSSRequireL( aValue );
            break;
        case KLightsATCForcedLightsOn: 
              iSysApAppUi.HandleForcedLightsATCRequireL( aValue ); 
              break; 
        case KLightsControl:
            iSysApAppUi.HandleLightsRequireL();
            break;
        case KCoreAppUIsDisableKeyguard:
            iSysApAppUi.AlertUiKeyLockOff( aValue == ECoreAppUIsDisableKeyguard );
            break;
        case KCoreAppUIsSoftReject:
            if ( aValue == ECoreAppUIsSoftReject )
                {
                iSysApAppUi.DoNotEnableKeylock();
                iSysApAppUi.SwitchLightsOnSoftRejectL();
                }
            break;
        case KCoreAppUIsUipInd:
            if ( aValue == ECoreAppUIsDoNotShow )
                {
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorInstantMessage, EAknIndicatorStateOff ); 
                }
            else if ( aValue == ECoreAppUIsShow )
                {
//                iSysApAppUi.SetIndicatorStateL( EAknIndicatorInstantMessage, EAknIndicatorStateOn ); 
                }
            break;
        case KCoreAppUIsMessageToneQuit:
 //           iSysApAppUi.HandleMessageTonePlayingQuitability( aValue );
            break;
        case KCoreAppUIsNspsRawKeyEvent:
            iSysApAppUi.HandleNspsRawKeyEventL();
            break;
        case KCoreAppUIsLightsRawKeyEvent:
            if ( aValue == ECoreAppUIsKeyEvent )
                {
                iSysApAppUi.HandleRawKeyEventLightsRequireL();
                }
            break;
        case KCoreAppUIsMmcRemovedWithoutEject:
            {
            TUint cmd( aValue & KCoreAppUIsMmcRemovedWithoutEjectCmdMask );
            if ( cmd == ECoreAppUIsEjectCommandUsed )
                {
                iSysApAppUi.EjectStarted( ETrue );
                }
            else if ( cmd == ECoreAppUIsEjectCommandNotUsed )
                {
                iSysApAppUi.EjectStarted( EFalse );
                }
            else if ( cmd == ECoreAppUIsEjectCommandUsedToDrive )
                {
                TInt drive( aValue >> KCoreAppUIsMmcRemovedWithoutEjectValShift );
                iSysApAppUi.EjectUsed( drive );
                }
            break;
            }
        case KCoreAppUIsNewEmailStatus:
            if ( aValue == ECoreAppUIsNewEmail )
                {
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorEmailMessage, EAknIndicatorStateOn );
                }
            else if ( aValue == ECoreAppUIsNoNewEmail )
                {
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorEmailMessage, EAknIndicatorStateOff );
                }
            break;

        case KCoreAppUIsAutolockStatus:
            if ( aValue == EAutolockOff )
                {
                iSysApAppUi.SetDeviceLockEnabledL( EFalse );
                }
            else if ( aValue > EAutolockOff ) // we are not interested in why autolock is enabled
                {
                iSysApAppUi.SetDeviceLockEnabledL( ETrue );
                }
            break;

        case KCoreAppUIsPoCIndicator:
            if ( aValue == ECoreAppUIsPocIndicatorOff || aValue == ECoreAppUIsPocIndicatorUninitialized )
                {
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorPoC, EAknIndicatorStateOff );
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorPoCDnD, EAknIndicatorStateOff );
                }
            else if ( aValue == ECoreAppUIsPocIndicatorDoNotDisturb )
                {
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorPoC, EAknIndicatorStateOff );
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorPoCDnD, EAknIndicatorStateOn );
                }
            else if ( aValue == ECoreAppUIsPocIndicatorConnectionOn )
                {
//                iSysApAppUi.SetIndicatorStateL( EAknIndicatorPoC, EAknIndicatorStateOn );
//                iSysApAppUi.SetIndicatorStateL( EAknIndicatorPoCDnD, EAknIndicatorStateOff );
                }
            break;
        case KCoreAppUIsPoCMissedIndicator:
            if ( aValue == ECoreAppUIsPocMissedIndicatorOff || aValue == ECoreAppUIsPocMissedIndicatorUninitialized )
                {
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorPoCMissed, EAknIndicatorStateOff );
                }
            else if ( aValue == ECoreAppUIsPocMissedIndicatorOn )
                {
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorPoCMissed, EAknIndicatorStateOn );
                }
            break;
            
        case KCoreAppUIsTarmIndicator:
 //           iSysApAppUi.HandleTarmIndicatorL( aValue );
            break;

        case KCoreAppUIsMtvRecStatus:
            if ( aValue == ECoreAppUIsMtvRecStatusOn  )
                {
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorMobileTVRec, EAknIndicatorStateOn );
                }
            else if ( aValue == ECoreAppUIsMtvRecStatusOff || aValue == ECoreAppUIsMtvRecStatusUninitialized )
                {
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorMobileTVRec, EAknIndicatorStateOff );
                }
            break;

        case KCoreAppUIsMtvDvbhStatus:
            if ( aValue == ECoreAppUIsMtvDvbhStatusOn )
                {
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorMobileTV, EAknIndicatorStateOn );
                }
            else if ( aValue == ECoreAppUIsMtvDvbhStatusOff || aValue == ECoreAppUIsMtvDvbhStatusUninitialized )
                {
//                iSysApAppUi.SetIndicatorStateL( EAknIndicatorMobileTV, EAknIndicatorStateOff );
                }
            break;
        
        case KCoreAppUIsVideoSharingIndicator:
            if ( aValue == ECoreAppUIsVideoSharingIndicatorOn )
                {
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorVideoSharing, EAknIndicatorStateOn );
                }
            else if ( aValue == ECoreAppUIsVideoSharingIndicatorOff || aValue == ECoreAppUIsVideoSharingIndicatorUninitialized )
                {
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorVideoSharing, EAknIndicatorStateOff );
                }
            break;
            
        case KCoreAppUIsPowerMenuCustomDialogStatus:
            if ( aValue == ECoreAppUIsPowerMenuCustomDialogOn )
                { 
                TBool powerMenuMemoryStatus = EFalse;
                powerMenuMemoryStatus = iSysApAppUi.ReleasePowerMenuCustomDialogMemory();
                if(powerMenuMemoryStatus)
                    {
                    RProperty::Set( KPSUidCoreApplicationUIs, KCoreAppUIsPowerMenuCustomDialogStatus, ECoreAppUIsPowerMenuCustomDialogOff );
                    }
                }
            else if(aValue == ECoreAppUIsMemoryCardCustomDialogOk)
                {
                TRACES( RDebug::Print(_L("CSysApAppUi::ECoreAppUIsMemoryCardCustomDialogOk") ) );
                TBool memoryCardStatus = EFalse;
                memoryCardStatus = iSysApAppUi.NotifiedDialogIfRequiredAndReleaseMemory();
                if(memoryCardStatus)
                    {
                    TRACES( RDebug::Print(_L("CSysApAppUi::ECoreAppUIsMemoryCardCustomDialogOn: memoryCardStatus Begin") ) );                                       
                    RProperty::Set( KPSUidCoreApplicationUIs, KCoreAppUIsPowerMenuCustomDialogStatus, ECoreAppUIsPowerMenuCustomDialogOff );
                    TRACES( RDebug::Print(_L("CSysApAppUi::ECoreAppUIsMemoryCardCustomDialogOn: memoryCardStatus end") ) );
                    }
                TRACES( RDebug::Print(_L("CSysApAppUi::ECoreAppUIsMemoryCardCustomDialogOn: memoryCardStatus final end") ) );
                                    
                }
            else if(aValue == ECoreAppUIsMemoryCardCustomDialogCancel)
                {
                TRACES( RDebug::Print(_L("CSysApAppUi::ECoreAppUIsMemoryCardCustomDialogCancel") ) );
                iSysApAppUi.ReleaseMemoryForMemoryCardDialog();
                RProperty::Set( KPSUidCoreApplicationUIs, KCoreAppUIsPowerMenuCustomDialogStatus, ECoreAppUIsPowerMenuCustomDialogOff );
                }
            else if ( aValue == ECoreAppUIsPowerMenuCustomDialogOff || aValue == ECoreAppUIsPowerMenuCustomDialogUninitialized )
                {
                   // do nothing
                }
            break;   
        
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CSysApPubSubObserver::HandleStartupCategoryL()
// ----------------------------------------------------------------------------
void CSysApPubSubObserver::HandleStartupCategoryL( const TUint aKey, const TInt aValue )
    {
    switch ( aKey )
        {
        case KPSSimOwned:
            if ( aValue == ESimNotOwned )
                { 
                TRAPD( simChangedErr, iSysApAppUi.DoSimChangedFromPreviousBootL() );
                TRACES( RDebug::Print( _L("CSysApPubSubObserver::HandleStartupCategoryL: simChangedErr = %d" ), simChangedErr ) ); 
                simChangedErr = simChangedErr; // suppress 'variable not used' warning 
                }
            break;
            
        case KWakeupAlarmPubSubKey:
            {
            TRACES( RDebug::Print(_L("CSysApPubSubObserver::HandleStartupCategoryL called CSysApAppUi::ShowAlarmIndicatorL with aShowIndicator = %d "), (aValue == EActiveWakeupAlarmSet)));
            iSysApAppUi.ShowAlarmIndicatorL( aValue == EActiveWakeupAlarmSet );
            break;
            }
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CSysApPubSubObserver::HandleSystemCategoryL()
// ----------------------------------------------------------------------------
void CSysApPubSubObserver::HandleSystemCategoryL( const TUint aKey, const TInt aValue )
    {
    switch ( aKey )
        {
        case KUidInboxStatusValue:
//            iSysApAppUi.SetEnvelopeIndicatorL();
            break;

        case KUidOutboxStatusValue:
            if ( aValue == ESADocumentsInOutbox )
                {
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorOutbox, EAknIndicatorStateOn );
                }
            //Hide the Outbox indicator
            else if ( aValue == ESAOutboxEmpty )
                {
//                iSysApAppUi.SetIndicatorStateL( EAknIndicatorOutbox, EAknIndicatorStateOff );
                }
            break;

        case KIrdaStatus:
            if ( aValue == TIrdaStatusCodes::EIrLoaded || aValue == TIrdaStatusCodes::EIrBlocked || aValue == TIrdaStatusCodes::EIrDisconnected )
                {
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorIrActive, EAknIndicatorStateAnimate );
                }
            else if ( aValue == TIrdaStatusCodes::EIrConnected )
                {
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorIrActive, EAknIndicatorStateOn );
                }
            else if ( aValue == TIrdaStatusCodes::EIrUnloaded )
                {
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorIrActive, EAknIndicatorStateOff );
                }
            break;

//        case KPSUidGprsStatusValue:
 //       case KPSUidWcdmaStatusValue:        
 //           iSysApAppUi.SetSignalIndicatorL();
//            break;

        case KPSUidLocationGlobalPrivacyValue:
//            iSysApAppUi.HandleLocationPrivacyIndicatorL( aValue );
            break;

        case conn::KUidBackupRestoreKey:
            {            
            conn::TBURPartType partType = static_cast< conn::TBURPartType >( aValue & conn::KBURPartTypeMask );

            if ( partType == conn::EBURRestoreFull || partType == conn::EBURRestorePartial )
                {
                iRestoreOngoing = ETrue;
                }
            else if ( iRestoreOngoing )
                {
                TRACES( RDebug::Print( _L("CSysApPubSubObserver::HandleSystemCategoryL: restore finished, make the device reset" ) ) ); 
                User::After( 1000000 );
                iSysApAppUi.ShowQueryL( ESysApRestartPhoneQuery );   
                }
            }
            break;
            
        default:
            break;
        } 
    }

// ----------------------------------------------------------------------------
// CSysApPubSubObserver::HandleHwrmPowerStateCategoryL()
// ----------------------------------------------------------------------------
void CSysApPubSubObserver::HandleHwrmPowerStateCategoryL( const TUint aKey, const TInt aValue )
    {
    switch ( aKey )
        {
        case KHWRMBatteryLevel:
 //           iSysApAppUi.UpdateBatteryBarsL( aValue );
            break;
        
        case KHWRMBatteryStatus:
            iSysApAppUi.HandleBatteryStatusL( aValue );            
            break;
    
        case KHWRMChargingStatus:
            iSysApAppUi.HandleChargingStatusL( aValue );           
            break;

        default:
            break;
        } 
    }
/*
// ----------------------------------------------------------------------------
// CSysApPubSubObserver::HandleHwrmFmTxCategoryL()
// ----------------------------------------------------------------------------
void CSysApPubSubObserver::HandleHwrmFmTxCategoryL( const TUint aKey, 
                                                    const TInt aValue )
    {
	TRACES(RDebug::Print(_L("CSysApPubSubObserver::HandleHwrmFmTxCategoryL aKey %d, aValue %d"), aKey, aValue));
    	
    if ( aKey == KHWRMFmTxStatus )
        {
        TBool isFmTxTurnedOn( EFalse );
        TBool isFmTxTurnedOff( EFalse );
        
        const TInt KFmTxNotSendingIndicator( EAknIndicatorFMTxEnabled );
        const TInt KFmTxActiveIndicator( EAknIndicatorFMTxSending );
            
		TRACES(RDebug::Print(_L("CSysApPubSubObserver::HandleHwrmFmTxCategoryL aKey = KHWRMFmTxStatus, iPreviousFmTxPSValue = %d"), iPreviousFmTxPSValue));            
            
        switch ( iPreviousFmTxPSValue )
            {
            // Previous "On" states
            case EFmTxStateActive:
            case EFmTxStateInactive:         
            case EFmTxStateScanning: // equivalent to EFmTxStateInactive for UI purposes                                                  
                            
                if ( aValue == EFmTxStateUnknown || aValue == EFmTxStateOff || 
                     aValue == EFmTxStatePowerSaveInactivity || aValue == EFmTxStatePowerSaveAccessory  )
                    {
                    // Active/Inactive -> Off state                    
                    TRACES(RDebug::Print(_L("CSysApPubSubObserver::HandleHwrmFmTxCategoryL Active/Inactive -> Off transition")));
                    
    	            // turn any FM TX indicators off
//	                iSysApAppUi.SetIndicatorStateL( KFmTxActiveIndicator, EAknIndicatorStateOff );
//	                iSysApAppUi.SetIndicatorStateL( KFmTxNotSendingIndicator, EAknIndicatorStateOff );
                    
                    // give notification
                    if ( aValue == EFmTxStatePowerSaveAccessory )
                    	{
//                    	iSysApAppUi.ShowUiNoteL( EFmTxAccessoryStandbyNote );
                    	}
                    else
                    	{
 //                   	iSysApAppUi.ShowUiNoteL( EFmTxOffNote );
                    	}
                    isFmTxTurnedOff = ETrue;                                                                           	
                    }
                else if ( aValue == EFmTxStateInactive || aValue == EFmTxStateScanning )
                    {
                    // Active -> Inactive
                    TRACES(RDebug::Print(_L("CSysApPubSubObserver::HandleHwrmFmTxCategoryL Active -> Inactive transition")));
                   
                    // enable not sending indicator
//	                iSysApAppUi.SetIndicatorStateL( KFmTxActiveIndicator, EAknIndicatorStateOff );                      
//                    iSysApAppUi.SetIndicatorStateL( KFmTxNotSendingIndicator, EAknIndicatorStateOn );                  
                    }
                else if ( aValue == EFmTxStateActive )
                    {
                    // Inactive -> Active
                    TRACES(RDebug::Print(_L("CSysApPubSubObserver::HandleHwrmFmTxCategoryL Inactive -> Active transition")));
                    
                    // enable active indicator
//                    iSysApAppUi.SetIndicatorStateL( KFmTxNotSendingIndicator, EAknIndicatorStateOff );    
//                    iSysApAppUi.SetIndicatorStateL( KFmTxActiveIndicator, EAknIndicatorStateOn );					                
                    }    
        
                break;    
 
			// Previous "Off" states       
            case EFmTxStateUnknown:			 
            case EFmTxStateOff:			 
            case EFmTxStatePowerSaveInactivity:
            case EFmTxStatePowerSaveAccessory:            
                                                                                       
                if ( aValue == EFmTxStateActive )
                    {
                    // Off -> Active
                    TRACES(RDebug::Print(_L("CSysApPubSubObserver::HandleHwrmFmTxCategoryL Off -> Active transition")));
                                        
                    // enable active indicator
 //                   iSysApAppUi.SetIndicatorStateL( KFmTxNotSendingIndicator, EAknIndicatorStateOff );   
 //                   iSysApAppUi.SetIndicatorStateL( KFmTxActiveIndicator, EAknIndicatorStateOn );					                  
                    
                    // give notification
 //                   iSysApAppUi.ShowUiNoteL( EFmTxOnNote );
                    isFmTxTurnedOn = ETrue;
                    }
                else if ( aValue == EFmTxStateInactive || aValue == EFmTxStateScanning )
                	{
                	// Off -> Inactive
                	TRACES(RDebug::Print(_L("CSysApPubSubObserver::HandleHwrmFmTxCategoryL Off -> Inactive transition")));
                	
                    // enable not sending indicator
//	                iSysApAppUi.SetIndicatorStateL( KFmTxActiveIndicator, EAknIndicatorStateOff );                      
 //                   iSysApAppUi.SetIndicatorStateL( KFmTxNotSendingIndicator, EAknIndicatorStateOn );                      	
                    
                    // give notification
 //                   iSysApAppUi.ShowUiNoteL( EFmTxOnNote );
                    isFmTxTurnedOn = ETrue;
                	}                
                break;                
                            
            }
            
        if ( isFmTxTurnedOn )
            {
            if ( !iFmTxRemConObserver )
                {
                // create observer which captures volume keys
                iFmTxRemConObserver = CSysApRemConObserver::NewL( iSysApAppUi );
                }


            if ( iSysApAppUi.SysApFeatureManager().FmTxRdsTextSupported() )
                {
                _LIT( KRdsObserverExe, "RDSEventObserver.exe" );
                
                RProcess process;
                TInt error = process.Create( KRdsObserverExe, KNullDesC );
                
                if ( error == KErrNone )
                    {
                    process.Resume();
                    }
                else
                    {
                    TRACES(RDebug::Print(_L("CSysApPubSubObserver::HandleHwrmFmTxCategoryL: error=%d"), error ) );
                    }
                
                process.Close();
                }
        
            }
        else if ( isFmTxTurnedOff )
            {
            delete iFmTxRemConObserver;
            iFmTxRemConObserver = NULL;
            }
        iPreviousFmTxPSValue = aValue;
        }
    }
*/

// ----------------------------------------------------------------------------
// CSysApPubSubObserver::HandleWlanCategoryL()
// ----------------------------------------------------------------------------
void CSysApPubSubObserver::HandleWlanCategoryL( const TUint aKey, const TInt aValue )
    {
    switch ( aKey )
        {
        case KPSWlanIndicator:
            iSysApAppUi.HandleWlanIndicatorL(aValue);
            break;        

        default:
            break;
        } 
    }

// ----------------------------------------------------------------------------
// CSysApPubSubObserver::HandleTelephonyCallInformationCategoryL()
// ----------------------------------------------------------------------------
void CSysApPubSubObserver::HandleTelephonyCallInformationCategoryL( const TUint aKey, const TInt aValue )
    {
    switch ( aKey )
        {
        case KCTsyCallState:
            iSysApAppUi.HandleCurrentCallStateChangeL( aValue );
            break;
        
        default:
            break;            
        }
    }

// ----------------------------------------------------------------------------
// CSysApPubSubObserver::HandleNetworkInfoCategoryL()
// ----------------------------------------------------------------------------
void CSysApPubSubObserver::HandleNetworkInfoCategoryL( const TUint aKey, const TInt aValue )
    {
    switch ( aKey )
        {
        case KNWHomeZoneStatus:
            if ( aValue == ENWHomeZone )
                {
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorHomeZone, EAknIndicatorStateOn );
                }
            else if ( aValue == ENWNone || aValue == ENWCityZone )
                {
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorHomeZone, EAknIndicatorStateOff );
                }
            break; 

        case KNWTelephonyNetworkMode:
 //           iSysApAppUi.SetSignalIndicatorL();
            break;

        default:
            break;
        } 
    }

// ----------------------------------------------------------------------------
// CSysApPubSubObserver::HandleHwrmCategoryL()
// ----------------------------------------------------------------------------
void CSysApPubSubObserver::HandleHwrmCategoryL( const TUint aKey, const TInt aValue )
    {
    switch ( aKey )
        {
        case KHWRMFlipStatus:
            if ( aValue == EPSHWRMFlipClosed )
                {
 //               iSysApAppUi.SimulateFlipKeyEventL( EFalse );
                }
            else if ( aValue == EPSHWRMFlipOpen )
                {
 //               iSysApAppUi.SimulateFlipKeyEventL( ETrue );
                }
            break; 

        default:
            break;
        } 
    }

// ----------------------------------------------------------------------------
// CSysApPubSubObserver::HandleDataSyncCategoryL()
// ----------------------------------------------------------------------------
void CSysApPubSubObserver::HandleDataSyncCategoryL( const TUint aKey, const TInt aValue )
    {
    switch ( aKey )
        {
        case KDataSyncStatus:
            if ( aValue > EDataSyncNotRunning )
                {
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorSynchronization , EAknIndicatorStateOn );
                }
            else
                {
  //              iSysApAppUi.SetIndicatorStateL( EAknIndicatorSynchronization, EAknIndicatorStateOff );
                }
            break;
        
        default:
            break;        
        }
        
    }

// ----------------------------------------------------------------------------
// CSysApPubSubObserver::HandlePosIndicatorCategoryL()
// ----------------------------------------------------------------------------
void CSysApPubSubObserver::HandlePosIndicatorCategoryL( const TUint aKey, const TInt aValue )
    {
    switch ( aKey )
        {
        case KPosIntGpsHwStatus:
            if ( aValue == EPosIntGpsHwOff )
                {
                iSysApAppUi.iHbIndicatorSymbian->Deactivate(KGpsIndicatorPlugin);
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorGPS, EAknIndicatorStateOff );
                }
            else if ( aValue == EPosIntGpsHwOn )    
                {
                iSysApAppUi.iHbIndicatorSymbian->Activate(KGpsIndicatorPlugin);
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorGPS, EAknIndicatorStateOn );
                }
            else if ( aValue == EPosIntGpsHwIntermediate )
                {
 //               iSysApAppUi.SetIndicatorStateL( EAknIndicatorGPS, EAknIndicatorStateAnimate );
                }
            break;
        
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CSysApPubSubObserver::HandleSmsStackCategoryL()
// ----------------------------------------------------------------------------

void CSysApPubSubObserver::HandleSmsStackCategoryL( const TUint aKey, const TInt /*aValue*/ )
    {
    switch ( aKey )
        {
        case KAknKeyguardStatus:
 //           iSysApAppUi.SetEnvelopeIndicatorL();
            break;

        default:
            break;
        }
    }

void CSysApPubSubObserver::HandleLKeylockEventL( const TUint /* aKey */, const TInt aValue )
    {
    switch( aValue )
        {
        case EEikKeyLockEnabled:
            iSysApAppUi.SetKeyLockEnabledL();
            break;
        case EEikKeyLockDisabled:
            iSysApAppUi.SetKeyLockDisabledL();    
            break;
        default:
            break;
       }
    }

void CSysApPubSubObserver::HandleLightsOnRequestL( const TUint /* aKey */, const TInt aValue )
    {
    switch ( aValue )
        {
        case EEikKeyLockLightsOnRequest:
             iSysApAppUi.SetLightsOnUnlockNoteL();
             break;
        case EEikEcsQueryLights: // emergency note is shown
             iSysApAppUi.SetLightsOnEcsQueryL();
             break;

        case EEikSecurityQueryLights: // for device lock security query
        case ESecurityUIsLightsQueryOnRequest: 
		     iSysApAppUi.SetLightsOnSecurityQueryL();
             break;

        default:
            break;
        }
    }

// End of File
