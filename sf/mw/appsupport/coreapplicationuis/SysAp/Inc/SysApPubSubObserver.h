/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApPubSubObserver handles SysAp PubSub API
*              : related functionality except for BT.
*
*/


#ifndef SYSAPPUBSUBOBSERVER_H
#define SYSAPPUBSUBOBSERVER_H

// INCLUDES
#include <e32base.h>
#include "MSysApPropertyResponder.h"

// FORWARD DECLARATIONS

class CSysApAppUi;
class CSysApSubscriber;
class CSysApRemConObserver;

// CLASS DECLARATION
/**
*  CSysApPubSubObserver
*  
*  @lib   sysap
*  @since 2.X 
*/

enum TSecurityUIsLights
    {
    ESecurityUIsLightsUninitialized = 0,
    ESecurityUIsLightsLockOnRequest,
    ESecurityUIsLightsQueryOnRequest,
    ESecurityUIsLightsLockOffRequest,
    ESecurityUIsLightsLastValue
    };

class CSysApPubSubObserver : 
              public CBase, 
              public MSysApPropertyResponder
{
public:

    /**
    * Two-phased constructor.
    */         
    static CSysApPubSubObserver* NewL( CSysApAppUi& aSysApAppUi );

    /**
    * Destructor.
    */
    ~CSysApPubSubObserver();

private:

    /**
    * Handle keys under category KUidSystemCategory
    */         
    void HandleSystemCategoryL( const TUint aKey, const TInt aValue );

    /**
    * Handle keys under category KPSUidStartup
    */         
    void HandleStartupCategoryL( const TUint aKey, const TInt aValue );

    /**
    * Handle keys under category KPSUidCoreApplicationUIs
    */         
    void HandleCoreAppUIsCategoryL( const TUint aKey, const TInt aValue );

    /**
    * Handle keys under category KPSUidHWRMPowerState
    */         
    void HandleHwrmPowerStateCategoryL( const TUint aKey, const TInt aValue );

    /**
    * Handle keys under category KPSUidWlan
    */
    void HandleWlanCategoryL( const TUint aKey, const TInt aValue );

    /**
    * Handle keys under category KPSUidNetworkInfo
    */
    void HandleNetworkInfoCategoryL( const TUint aKey, const TInt aValue );

    /**
    * Handle keys under category KPSUidCtsyCallInformation.
    */
    void HandleTelephonyCallInformationCategoryL( const TUint aKey, const TInt aValue );
    
    /**
    * Handle keys under category KPSUidHWRM
    */
    void HandleHwrmCategoryL( const TUint aKey, const TInt aValue );

    /**
    * Handle keys under category KPSUidHWRMFmTx
    */
    // void HandleHwrmFmTxCategoryL( const TUint aKey, const TInt aValue );
    
    /**
    * Handle keys under category KPSUidDataSynchronizationInternalKeys
    * 
    * @since S60 3.2
    * @param aKey   key identifier whose change triggered notification
    * @param aValue key value
    * @return None.
    */
    void HandleDataSyncCategoryL( const TUint aKey, const TInt aValue );

    /**
    * Handle keys under category KPosIndicatorCategory
    * 
    * @since S60 3.2
    * @param aKey   key identifier whose change triggered notification
    * @param aValue key value
    */
    void HandlePosIndicatorCategoryL( const TUint aKey, const TInt aValue );

    /**
    * Handle keys under category KUidPSSMSStackCategory
    * 
    * @since S60 3.2
    * @param aKey   key identifier whose change triggered notification
    * @param aValue key value
    */
    void HandleSmsStackCategoryL( const TUint aKey, const TInt aValue );

    /**
    * Constructors.
    */         
    CSysApPubSubObserver( CSysApAppUi& aSysApAppUi );
    void ConstructL();
    
    void HandleLKeylockEventL( const TUint aKey, const TInt aValue  );
    void HandleLightsOnRequestL( const TUint aKey, const TInt aValue  );

protected: // From MSysApPropertyResponder

    void HandlePropertyChangedL( const TUid& aCategory, const TUint aKey );
 
private:
    
    CSysApAppUi&        iSysApAppUi;

    // Category KUidSystemCategory
    CSysApSubscriber*   iBatteryBarsSubscriber;              // KPSUidBatteryBars
    CSysApSubscriber*   iBatteryLowIndicationSubscriber;     // KPSUidBatteryLowIndication
    CSysApSubscriber*   iBatteryCStatusSubscriber;           // KPSUidBatteryCStatus
    CSysApSubscriber*   iChargerCStatusSubscriber;           // KPSUidChargerCStatus
    CSysApSubscriber*   iInboxStatusSubscriber;              // KUidInboxStatusValue
    CSysApSubscriber*   iOutboxStatusSubscriber;             // KUidOutboxStatus
    CSysApSubscriber*   iIrdaStatusSubscriber;               // KPSUidIrdaStatus
//    CSysApSubscriber*   iGprsStatusSubscriber;               // KPSUidGprsStatus
//    CSysApSubscriber*   iWcdmaStatusSubscriber;              // KPSUidWcdmaStatus
    CSysApSubscriber*   iLocationGlobalPrivacySubscriber;    // KPSUidLocationGlobalPrivacy
    CSysApSubscriber*   iRestoreBackupSubscriber;            // KUidBackupRestoreKey

    // Category KPSUidStartup
    CSysApSubscriber*   iSimChangedSubscriber;               // KPSSimChanged

    // Category KHWRMFmTxStatus
    // CSysApSubscriber*   iFmTxStatusSubscriber;   // KPSUidHWRMFmTx
    
    // TInt iPreviousFmTxPSValue;
    
    // CSysApRemConObserver* iFmTxRemConObserver;
    
    // Category KPSUidCoreApplicationUIs

    // Light Control API
    CSysApSubscriber*   iAlarmLightSubscriber;               // KLightsAlarmLightActive (KSysApAlarmLightActive)
    CSysApSubscriber*   iVTLightsSubscriber;                 // KLightsVTForcedLightsOn (KSysApForcedLightsOn)
    CSysApSubscriber*   iSSLightsSubscriber;                 // KLightsSSForcedLightsOn (KSysApForcedSSLightsOn)
    CSysApSubscriber*   iATCLightsSubscriber;                // KLightsATCForcedLightsOn
    CSysApSubscriber*   iLightsControlSubscriber;            // KLightsControl (KSysApLightsControl)

    // System Application Notification API
    CSysApSubscriber*   iDisableKeyguardSubscriber;          // KCoreAppUIsDisableKeyguard (KSysApKeyGuardInactive)
    CSysApSubscriber*   iSoftRejectSubscriber;               // KCoreAppUIsSoftReject (KSysApSoftReject)
    CSysApSubscriber*   iUipIndSubscriber;                   // KCoreAppUIsUipInd (KSysApUipInd)
    CSysApSubscriber*   iMessageToneQuitSubscriber;          // KCoreAppUIsMessageToneQuit (KSysApMessageToneQuit)
    CSysApSubscriber*   iNspsRawKeyEventSubscriber;          // KCoreAppUIsNspsRawKeyEvent (KSysApNspsRawKeyEvent)
    CSysApSubscriber*   iLightsRawKeyEventSubscriber;        // KCoreAppUIsLightsRawKeyEvent (KSysApLightsControl)
    CSysApSubscriber*   iMmcRemovedWithoutEjectSubscriber;   // KCoreAppUIsMmcRemovedWithoutEject (KSysApMmcRemovedWithoutEject)
    CSysApSubscriber*   iNewEmailStatusSubscriber;           // KCoreAppUIsNewEmailStatus
    CSysApSubscriber*   iPoCIndicatorSubscriber;             // KCoreAppUIsPoCIndicator
    CSysApSubscriber*   iPoCMissedIndicatorSubscriber;       // KCoreAppUIsPoCMissedIndicator
    CSysApSubscriber*   iTarmStatusSubscriber;               // KCoreAppUIsTarmIndicator
    CSysApSubscriber*   iMtvRecStatusSubscriber;             // KCoreAppUIsMtvRecStatus
    CSysApSubscriber*   iMtvDvbhStatusSubscriber;            // KCoreAppUIsMtvDvbhStatus
    /**
    * Subscriber for KCoreAppUIsVideoSharingIndicator.
    * Own.
    */
    CSysApSubscriber* iVideoSharingIndicatorSubscriber;
    
    // Category KPSUidWlan
    CSysApSubscriber*   iWlanIndicatorSubscriber;            // KPSWlanIndicator WlanInternalPSKeys.h

    TBool               iRestoreOngoing;

    // Category KPSUidCtsyCallInformation
    CSysApSubscriber* iCallInfoSubscriber; // KCTsyCallState ctsydomainpskeys.h
    
    // Autolock Status API
    CSysApSubscriber*   iAutolockStatusSubscriber;           // KCoreAppUIsAutolockStatus

    // Category KPSUidNetworkInfo - NetworkHandlingDomainPSKeys.h
    CSysApSubscriber*   iHomeZoneStatusSubscriber;           // KNWHomeZoneStatus
    CSysApSubscriber*   iNetworkModeSubscriber;              // KNWTelephonyNetworkMode
    
    // Category KPSUidUikon
    CSysApSubscriber*   iFlipStatusSubscriber;               // KPSUidHWRM
    
    // Category KPSUidDataSynchronizationInternalKeys
    /**
    * Subscriber for key KDataSyncStatus.
    * Own.
    */
    CSysApSubscriber* iSyncStatusSubscriber;
    
    // Category KPosIndicatorCategory
    /**
    * Subscriber for key KPosIntGpsHwStatus.
    * Own.
    */
    CSysApSubscriber* iGpsIndicatorSubscriber;
    
    /**
    * Subscriber for the key KPSWakeupAlarmStatus.
    * Own.
    */
    CSysApSubscriber* iWakeupAlarmSubscriber;

    /**
    * Subscriber for the key KUidPSSMSStackDiskSpaceMonitorKey.
    * Own.
    */
    CSysApSubscriber* iSmsPhoneStoreSubscriber;
    
    CSysApSubscriber* iKeylockEventSubscriber;
    
    CSysApSubscriber* iLightsOnRequestSubscriber;
    
    CSysApSubscriber* iPowerMenuCustomDialogSubscriber;
    
};

#endif // SYSAPPUBSUBOBSERVER_H

// End of File
