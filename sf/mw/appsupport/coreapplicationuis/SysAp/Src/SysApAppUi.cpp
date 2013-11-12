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
* Description:  CSysApAppUi implementation.
*
*/
// INCLUDES

#include <startupdomainpskeys.h>
#include "sysapappui.h"
#include "coreapplicationuisprivatepskeys.h"
#include <HbDeviceMessageBoxSymbian.h>
//#include <hbdevicepowermenusymbian.h>
#include <sacls.h>
#include <featmgr.h>
#include <DevEncProtectedPSKey.h>
#include <ctsydomainpskeys.h>
#include <secuisystemlock.h>
#include <apgtask.h>
#include <hwrmpowerstatesdkpskeys.h>
#include <wlaninternalpskeys.h> 
#include <HbDeviceNotificationDialogSymbian.h>
#include <hbsymbianvariant.h>
#include <hbtextresolversymbian.h>
#include <UikonInternalPSKeys.h>

//#include "SysApWsClient.h"
#include "SysApFeatureManager.h"
#include "SysApNspsHandler.h"
#include "SysApPubSubObserver.h"
#include "sysapdefaultkeyhandler.h"
#include "SysApStartupController.h"
#include "MSysApOfflineModeController.h"
#include "SysApCenRepController.h"
#include "sysapcenreplightsettingsobserver.h"
#include "SysApCenRepLogsObserver.h"
#include "SysApCenRepBTObserver.h"
#include "SysApCenRepHacSettingObserver.h"
#include "sysapaudioroutingobserver.h"
#include "sysapbatteryinfocontroller.h"
#include "SysApSimChanged.h"
#include "MSysApBtSapController.h"
#include "MSysApBtController.h"
#include "MSysApUsbIndicator.h"
#include "sysapkeymanagement.h"
#include "SysApShutdownImage.h"
#include "SysApKeySndHandler.h"

#include "SysApShutdownAnimation.h"
#include "SysApEtelConnector.h"

#ifdef RD_MULTIPLE_DRIVE
#include "sysapdrivelist.h"
#include "sysapdriveunlockhandler.h"
#include "sysapdriveejecthandler.h"
#include "hbdeviceinputdialogsymbian.h"
#endif // RD_MULTIPLE_DRIVE

#include <settingsinternalcrkeys.h>
#include <keyguardaccessapi.h>
#include <eikdef.h>
#include <eikenv.h>
#include <UsbWatcherInternalPSKeys.h> // USB transfer modes
#include <usbpersonalityids.h>
#include "sysap.rsg"
#include <hbindicatorsymbian.h>

class CHbSymbianVariant;
const TInt KModifierMask( 0 );
_LIT_SECURITY_POLICY_PASS(KAlwaysPassPolicy);
_LIT_SECURITY_POLICY_C1(KWriteDeviceDataPolicy, ECapabilityWriteDeviceData);
const TInt KDelayBeforeNextScanningRound( 1000000 );

/* ENABLE ANIMATION: Add id of background image.
   Example: const TInt KBackgroundImageID = EMbmSysapQgn_graf_startup_bg;
   If there is no image defined, clear screen is used.*/
const TInt KBackgroundImageID = 0;


_LIT(KAccesoryPlugin,"com.nokia.accessory.indicatorplugin/1.0");
_LIT(KAccMode, "AccMode");
_LIT(KAccPhyConType, "AccType");

_LIT(KPsmPlugin,"com.nokia.hb.powersavemodeplugin/1.0");
_LIT(KPsm,"PSM");
_LIT(KCharging,"Charging");
_LIT(KPsmlocalisationfile, "powermanagement_");
_LIT(KtsfilePath, "z:/resource/qt/translations/");
_LIT(KlowbatteryIcon,"qtg_small_bt_low_battery.svg");
_LIT(KbatteryFullIcon,"qtg_status_battery.svg");



// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApAppUi::CSysApAppUi()
// ----------------------------------------------------------------------------

CSysApAppUi::CSysApAppUi()
    :iSysApShutdownImage(NULL),
     iPowerMenuDialog(NULL),
     iDeviceLockEnabled(EFalse),
     iKeyLockEnabled(EFalse),
     iPowerKeyPopupMenuActive(EFalse),
     iResourcesFreed (EFalse),
     iShutdownStarted (EFalse),
     iSysApAudioRoutingObserver (NULL),
     iSysApBatteryInfoController (NULL),
     iSysApPsmController(NULL),
     iSapTimer (NULL),
     iSysApCenRepLogsObserver (NULL),
     iSysApUsbIndicatorController(NULL),
     iKeyguardController (NULL),
     iKeyLockOnBeforeCall (EFalse),
     iCheckLongPowerKeyEvent (EFalse),
     iMMCEjectUsed( EFalse ),               
     iMemCardPwdDialog(NULL)
	{
	TRACES( RDebug::Print( _L("CSysApAppUi::CSysApAppUi()") ) );
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::ConstructL()
// ----------------------------------------------------------------------------

void CSysApAppUi::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: START") ) );
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: trying BaseConstructL()") ) );
    BaseConstructL();
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: BaseConstructL() OK") ) );

    iEikonEnv->SetSystem( ETrue );

    iEikonEnv->WsSession().ComputeMode( RWsSession::EPriorityControlDisabled );
    
    RThread().SetProcessPriority( EPriorityForeground );
    
    TInt mode;
    User::LeaveIfError( RProperty::Get( KPSUidStartup, KPSGlobalStartupMode, mode ) );
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: KPSGlobalStartupMode = %d"), mode ) );
    
    //SysAp needs to capture PowerKey events because it handles all functionality related to that
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: trying CEikApplication::RootWin()") ) );
    RWindowGroup groupWin = iCoeEnv->RootWin();
    User::LeaveIfError ( iCapturedEKeyPowerOff = groupWin.CaptureKey( EKeyPowerOff, KModifierMask, KModifierMask ) );
    User::LeaveIfError ( iCapturedEKeyPowerOffUpAndDowns = groupWin.CaptureKeyUpAndDowns( EStdKeyDevice2, KModifierMask, KModifierMask ) );
     
    TRACES ( RDebug::Print( _L("CSysApAppUi::ConstructL: trying CSysApDefaultKeyHandler::NewL()") ) );
    iSysApDefaultKeyHandler = CSysApDefaultKeyHandler::NewL(*this);
    
    // Define P&S keys "owned" by SysAp
    RProperty::Define( KPSUidUikon, KUikMMCInserted, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );
    //initially assuming that the memory card is not inserted
    RProperty::Set( KPSUidUikon, KUikMMCInserted, 0 );
    TDriveInfo driveInfo;
    TInt driveNumber; 
    TInt err;    
    RFs& fileServer = iEikonEnv->FsSession();  
    for ( driveNumber = EDriveD; driveNumber < EDriveZ; driveNumber++ )
         {
      err = fileServer.Drive(driveInfo,driveNumber);
          if(driveNumber == EDriveF && err == KErrNone && driveInfo.iType == EMediaHardDisk &&  driveInfo.iDriveAtt & KDriveAttRemovable)     
            {     
            TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: err = %d, driveInfo.iType = %d, driveInfo.iDriveAtt %d, KDriveAttRemovable = %d "),err,driveInfo.iType,driveInfo.iDriveAtt,KDriveAttRemovable) );     
            RProperty::Set( KPSUidUikon, KUikMMCInserted, 1 );
                break;  // Memory card drive found...     
            }
         }
           
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: trying CSysApFeatureManager::NewL()") ) );
    iSysApFeatureManager = CSysApFeatureManager::NewL();
    
    // Setup USB charging detector
     iSysApUsbChargerDetector.EnableUsbCharging(
         iSysApFeatureManager->Supported( KSysApFeatureIdChargerReminderNotes ) &&
         iSysApFeatureManager->Supported( KSysApFeatureIdUsbChargingWithoutReminderNotes ) );
    
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: trying CSysApPubSubObserver::NewL()") ) );
    iSysApPubSubObserver = CSysApPubSubObserver::NewL( *this );

    RProperty::Define( KPSUidCoreApplicationUIs, KCoreAppUIsPoCIndicator, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );
    RProperty::Set( KPSUidCoreApplicationUIs, KCoreAppUIsPoCIndicator, ECoreAppUIsPocIndicatorUninitialized );
    RProperty::Define( KPSUidCoreApplicationUIs, KCoreAppUIsPoCMissedIndicator, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );
    RProperty::Set( KPSUidCoreApplicationUIs, KCoreAppUIsPoCMissedIndicator, ECoreAppUIsPocMissedIndicatorUninitialized );
    
    // initial value won't be set to KCoreAppUIsUSBFileTransfer, because UsbMscPersonality-plugin may also define it if USB cable is connected during boot
    // remove this definition after all clients have taken the USB personality ids in use
    RProperty::Define( KPSUidCoreApplicationUIs, KCoreAppUIsUSBFileTransfer, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );
    
    RProperty::Define( KPSUidCoreApplicationUIs, KCoreAppUIsVideoSharingIndicator, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );
    RProperty::Set( KPSUidCoreApplicationUIs, KCoreAppUIsVideoSharingIndicator, ECoreAppUIsVideoSharingIndicatorUninitialized );

    RProperty::Define( KPSUidCoreApplicationUIs, KCoreAppUIsHideAlarm, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );
    RProperty::Define( KPSUidCoreApplicationUIs, KCoreAppUIsDisableKeyguard, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );
    RProperty::Define( KPSUidCoreApplicationUIs, KCoreAppUIsSoftReject, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );
    RProperty::Define( KPSUidCoreApplicationUIs, KCoreAppUIsUipInd, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );
    RProperty::Define( KPSUidCoreApplicationUIs, KCoreAppUIsMessageToneQuit, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );
    RProperty::Define( KPSUidCoreApplicationUIs, KCoreAppUIsNspsRawKeyEvent, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );
    RProperty::Define( KPSUidCoreApplicationUIs, KCoreAppUIsLightsRawKeyEvent, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );
    RProperty::Define( KPSUidCoreApplicationUIs, KCoreAppUIsMmcRemovedWithoutEject, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );
    RProperty::Define( KPSUidCoreApplicationUIs, KLightsAlarmLightActive, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );
    RProperty::Define( KPSUidCoreApplicationUIs, KLightsVTForcedLightsOn, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );
    RProperty::Define( KPSUidCoreApplicationUIs, KLightsSSForcedLightsOn, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );
    RProperty::Define( KPSUidCoreApplicationUIs, KLightsATCForcedLightsOn, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );
    RProperty::Define( KPSUidCoreApplicationUIs, KLightsControl, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );
    RProperty::Define( KPSUidCoreApplicationUIs, KCoreAppUIsTarmIndicator, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );
    RProperty::Define( KPSUidCoreApplicationUIs, KCoreAppUIsMtvRecStatus, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );
    RProperty::Define( KPSUidCoreApplicationUIs, KCoreAppUIsMtvDvbhStatus, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );
    RProperty::Define( KPSUidCoreApplicationUIs, KCoreAppUIsNewEmailStatus, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );

    // Initialize P&S keys "owned" by SysAp
    RProperty::Set( KPSUidCoreApplicationUIs, KCoreAppUIsHideAlarm, ECoreAppUIsHideAlarmUninitialized );
    
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: trying CSysApAccessoryObserver::NewL()") ) );
    iSysApAccessoryObserver = CSysApAccessoryObserver::NewL( *this );
    
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: trying CSysApCenRepController::NewL()") ) );
    iSysApCenRepController = CSysApCenRepController::NewL();
    
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: trying CSysApCenRepLightSettingsObserver::NewL") ) );
    iSysApCenRepLightSettingsObserver = CSysApCenRepLightSettingsObserver::NewL( *this );
    
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: trying CSysApCenRepBtObserver::NewL") ) );
    iSysApCenRepBtObserver = CSysApCenRepBtObserver::NewL( *this );
    
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: trying CSysApStartupController::NewL()") ) );
    iSysApStartupController = CSysApStartupController::NewL( *this, iSysApFeatureManager->OfflineModeSupported() );

    iActiveProfileBeforeOfflineMode = iSysApCenRepController->GetInt( KCRUidCoreApplicationUIsSysAp, KSysApProfileBeforeOfflineMode );    
           
    iSysApFeatureManager->FeatureVariationCheckDone();        
    
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: trying CSysApLightsController::NewL()") ) );
    iSysApLightsController = CSysApLightsController::NewL( *this,
                                                           iSysApCenRepLightSettingsObserver->GetLightsTimeout(),
                                                           iSysApFeatureManager->CoverDisplaySupported() );
            
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL : trying CreateSysApOfflineModeControllerL()") ) );
    iSysApOfflineModeController = CreateSysApOfflineModeControllerL( *this );
    
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL : trying DoInitialSwStateCheckL()") ) );
    iSysApStartupController->DoInitialSwStateCheckL();
    
    TRACES( RDebug::Print( _L("CCSysApAppUi::ConstructL  trying CSysApCenRepHacSettingObserver::NewL") ) );
    iSysApCenRepHacSettingObserver = CSysApCenRepHacSettingObserver::NewL( *this ); 
    
#ifndef RD_MULTIPLE_DRIVE
    if ( iSysApFeatureManager->MmcSupported() )
        {
        iSysApMMCObserver = CSysApMMCObserver::NewL( this, &iEikonEnv->FsSession(), iSysApFeatureManager->MmcHotSwapSupported() );
        }
#else // RD_MULTIPLE_DRIVE
    iSysApDriveList = CSysApDriveList::NewL( iEikonEnv->FsSession() );
    if ( iSysApFeatureManager->MmcSupported() )
        {
        iSysApMMCObserver = CSysApMMCObserver::NewL(
            iEikonEnv->FsSession(), *iSysApDriveList, *this, iSysApFeatureManager->MmcHotSwapSupported() );
        iSysApDriveUnlockHandler = CSysApDriveUnlockHandler::NewL(
            *iSysApDriveList, *this, iSysApFeatureManager->MemoryCardLockSupported() );
        iSysApDriveEjectHandler = CSysApDriveEjectHandler::NewL(
            *iSysApDriveList, *this, iEikonEnv->FsSession() );
        }
#endif // RD_MULTIPLE_DRIVE
    
    //Instantiate the KEF plugin manager
    //Trap constuction, since Sysap may live without iSysApKeyManagement
    TRAPD(keyManagementErr, iSysApKeyManagement=CSysApKeyManagement::NewL(CCoeEnv::Static()->RootWin(), *this));

    if (keyManagementErr)
        {
        TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL : CSysApKeyManagement::NewL returns error=%d"), keyManagementErr ) );
        }
    
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: trying CSysApEtelConnector::NewL()") ) );
    iSysApEtelConnector = CSysApEtelConnector::NewL( *this );
    
    // Initialize animdll for handling side volume keys
    // (needed before normal mode in case emergency number is dialed from PIN query)
    iSysApKeySndHandler = CSysApKeySndHandler::NewL(iEikonEnv->WsSession());
    iKeyguardController = CKeyguardAccessApi::NewL();
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: trying CSysApShutdownImage::NewL()") ) );
    iSysApShutdownImage = CSysApShutdownImage::NewL();//NULL; //
    
    // Initialize nsps handler. Needed for proper lights control during PIN query.
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: trying CSysApNspsHandler::NewL") ) );
    iSysApNspsHandler = CSysApNspsHandler::NewL( iEikonEnv->WsSession(), iSysApShutdownImage->ShutdownCoeControlWindow() );

    RProperty::Define( KPSUidCoreApplicationUIs,KCoreAppUIsPowerMenuCustomDialogStatus, RProperty::EInt, KAlwaysPassPolicy, KWriteDeviceDataPolicy );
    RProperty::Set( KPSUidCoreApplicationUIs, KCoreAppUIsPowerMenuCustomDialogStatus, ECoreAppUIsPowerMenuCustomDialogUninitialized );
    
	TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: trying CHbIndicatorSymbian::NewL()") ) );
    iHbIndicatorSymbian = CHbIndicatorSymbian::NewL();
	
	  TBool result = HbTextResolverSymbian::Init(KPsmlocalisationfile, KtsfilePath);
	
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: END") ) );    
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::FreeResources()
// ----------------------------------------------------------------------------

void CSysApAppUi::FreeResources()
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::FreeResources") ) );
    delete iSysApBatteryInfoController;
    delete iSysApPsmController;
    delete iVariantAccState; 

    delete iSysApAudioRoutingObserver;
    
    if (iMemCardPwdDialog!=NULL)
        {
        //PowerMenu already exist
        delete iMemCardPwdDialog;
        iMemCardPwdDialog = NULL;
        }
    
    if ( iSapTimer )
        {
        iSapTimer->Cancel();
        delete iSapTimer;
        }
    //Powermenu
    if (iPowerMenuDialog!=NULL)
        {
        //PowerMenu already exist
        delete iPowerMenuDialog;
        iPowerMenuDialog = NULL;
        }
    
    RWindowGroup groupWin = iCoeEnv->RootWin();
    groupWin.CancelCaptureKey( iCapturedEKeyPowerOff );
    groupWin.CancelCaptureKeyUpAndDowns( iCapturedEKeyPowerOffUpAndDowns );
    
    delete iSysApDefaultKeyHandler;
    delete iSysApCenRepLightSettingsObserver;
    delete iSysApCenRepBtObserver;
    delete iSysApCenRepHacSettingObserver;
    delete iSysApCenRepController;

    delete iSysApPubSubObserver;
    
    delete iSysApLightsController;
    delete iSysApFeatureManager;
    
    delete iSysApCenRepLogsObserver;
    delete iSysApOfflineModeController;
    
    delete iSysApUsbIndicatorController;
    delete iKeyguardController;
    delete iHbIndicatorSymbian; 
    delete iSysApKeyManagement;
    iSysApKeyManagement = NULL;
    
    REComSession::FinalClose();
    iResourcesFreed = ETrue;
    TRACES( RDebug::Print( _L("CSysApAppUi::FreeResources:END") ) );
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::PrepareToExit()
// ---------------------------------------------------------------------------
void CSysApAppUi::PrepareToExit()
    {
    TRACES(RDebug::Print( _L("CSysApAppUi::PrepareToExit()")));
    CEikAppUi::PrepareToExit();
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::StateOfProperty()
// ----------------------------------------------------------------------------
TInt CSysApAppUi::StateOfProperty( const TUid& aCategory, const TUint aKey ) const
    {
    TInt err( KErrNone );
    TInt value( 0 );
    err = RProperty::Get( aCategory, aKey, value );
    if ( err )
        {
        TRACES( RDebug::Print( _L("CSysApAppUi::StateOfProperty. RProperty::Get: err=%d"), err ) );
        return err;
        }
    return value;
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::OfflineModeActive()
// ----------------------------------------------------------------------------
TBool CSysApAppUi::OfflineModeActive()
    {
    if(iSysApOfflineModeController)
        {
        return iSysApOfflineModeController->OfflineModeActive();
        }
    else
        {
        return EFalse;
        }
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::GoOnlineL()
// ----------------------------------------------------------------------------

void CSysApAppUi::GoOnlineL( TBool /* aDoProfileChange */ )
    {
    if ( iSysApFeatureManager->OfflineModeSupported() )
        {
//        TRACES( RDebug::Print( _L("CSysApAppUi::GoOnlineL: going from off-line into on-line: aDoProfileChange=%d" ), aDoProfileChange ) );
        iActivateBt = iSysApOfflineModeController->MustBtBeActivated();
        iSysApOfflineModeController->SwitchFromOfflineToOnlineModeL();
        TRACES( RDebug::Print( _L("CSysApAppUi::GoOnlineL: iActivateBt = %d" ), iActivateBt ) );
        }
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::GoOfflineL()
// ----------------------------------------------------------------------------

void CSysApAppUi::GoOfflineL()
    {
    if ( iSysApFeatureManager->OfflineModeSupported() )
        {
        TRACES( RDebug::Print( _L("CSysApAppUi::GoOfflineL" ) ) );
        iDeactivateBt = ETrue;
        iSysApOfflineModeController->SwitchFromOnlineToOfflineModeL();
        }
    }

#ifndef SYSAP_USE_STARTUP_UI_PHASE
// need to configure the above macro.
#else // SYSAP_USE_STARTUP_UI_PHASE

// ----------------------------------------------------------------------------
// CSysApAppUi::HandleUiReadyAfterBootL()
// Called when startup UI activities has been finished
// ----------------------------------------------------------------------------

void CSysApAppUi::HandleUiReadyAfterBootL()
    {
    TRACES( RDebug::Print(_L("CSysApAppUi::HandleUiReadyAfterBootL" ) ) );    
    TInt state( StateOfProperty( KPSUidHWRMPowerState, KHWRMBatteryLevel ) );

    UpdateBatteryBarsL( state );   
    DoSwStateNormalConstructionL();
    HandleAccessoryProfileInStartupL();
          
    if ( iSysApFeatureManager->MmcSupported() )
        {
#ifndef RD_MULTIPLE_DRIVE
        MountMMC();
        MMCStatusChangedL();
        iHideFirstBeep = EFalse;
#else // RD_MULTIPLE_DRIVE
		if(iSysApDriveList == NULL)
			{
				iSysApDriveList = CSysApDriveList::NewL( iEikonEnv->FsSession() );
			}
        iSysApDriveList->MountDrive( iSysApDriveList->DefaultMemoryCard() );
        UpdateInsertedMemoryCardsL();
#endif // RD_MULTIPLE_DRIVE
        }
		
	if ( iSysApFeatureManager->MmcHotSwapSupported() )
        {
        if(iSysApMMCObserver == NULL)
        	{
        	iSysApMMCObserver = CSysApMMCObserver::NewL(
            iEikonEnv->FsSession(), *iSysApDriveList, *this, iSysApFeatureManager->MmcHotSwapSupported() );
        	}
        iSysApMMCObserver->StartMountObserver();
        }    
   
    if ( !iSysApPsmController ) // created here if first state change has not occurred yet
       {
       iSysApPsmController = CSysApPsmController::NewL( *this );        
       }

    if ( iSysApPsmController )
       {
	   if ( iCharging ) // if charger is connected on boot PSM queries may need to be shown
        {
         HandleChargingStatusL( StateOfProperty( KPSUidHWRMPowerState, KHWRMChargingStatus ) );
        }
		
       if ( iSysApPsmController->FullPsmEnabled() )
           {
           // activate psm indicator 
            iVariantAccState = CHbSymbianVariant::NewL(&KPsm, CHbSymbianVariant::EDes);
            if (!iHbIndicatorSymbian->Activate(KPsmPlugin,iVariantAccState)) 
               {
               int error = iHbIndicatorSymbian->Error();
               //use the errorcode...
               }
           }
     
       }
    
    TInt batteryStatus = StateOfProperty( KPSUidHWRMPowerState, KHWRMBatteryStatus );
    TRACES( RDebug::Print(_L("CSysApAppUi::HandleUiReadyAfterBootL: batteryStatus %d" ), batteryStatus ) );
    if(  batteryStatus == EBatteryStatusLow || batteryStatus == EBatteryStatusEmpty )
        {
        // low and empty battery states are informed to the user in device startup
        HandleBatteryStatusL( batteryStatus );
        }
    else if ( iSysApPsmController && !iCharging )
        {
            TRACES( RDebug::Print(_L("CSysApAppUi::HandleUiReadyAfterBootL: batteryStatus %d, iCharging %d -> disable partial psm" ), batteryStatus, iCharging ) );

            iSysApPsmController->BatteryLow( EFalse );
            iSysApPsmController->DoEnablePartialPsm( EFalse );
        }
    
    if ( ! iSysApUsbIndicatorController )
        {
        TRAPD ( usbErr, iSysApUsbIndicatorController = CreateSysApUsbIndicatorL( *this ) );
        if ( usbErr )
            {
            TRACES( RDebug::Print(_L("CSysApAppUi::HandleUiReadyAfterBootL: error in constructing USB ind. controller %d" ), usbErr ) );
            }
        }
   }

// ----------------------------------------------------------------------------
// CSysApAppUi::DoStateChangedL(const RStarterSession::TGlobalState aSwState)
// This method is not called after boot has finished.
// ----------------------------------------------------------------------------

void CSysApAppUi::DoStateChangedL(const RStarterSession::TGlobalState aSwState)
    {
    TRACES( RDebug::Print(_L("CSysApAppUi::DoStateChangedL: %d" ), aSwState ) );

    TRAPD( simChangedErr, DoSimChangedFromPreviousBootL() );
    TRACES( RDebug::Print( _L("CSysApAppUi::DoStateChangedL: simChangedErr = %d" ), simChangedErr ) );
    simChangedErr = simChangedErr; // suppress 'variable not used' warning
    LogsObserverL().HandleSimChangedCheckDoneL();

    if ( iSysApFeatureManager->PowerSaveSupported() )
        {
        // create controller before checking battery state, so that power saving can be enabled during boot if needed
        if ( !iSysApPsmController ) // created only in first state change
            {
            iSysApPsmController = CSysApPsmController::NewL( *this );        
            }

        // in charger boot explicitly disable  power save mode
        if ( aSwState == RStarterSession::ECharging )
            {
            iSysApPsmController->ChargerConnected();
            iSysApPsmController->DoEnableFullPsm(EFalse); // disable  power save now
            }
        }

    if ( aSwState == RStarterSession::ECharging || aSwState == RStarterSession::EAlarm )
        {
        TInt state( StateOfProperty( KPSUidHWRMPowerState, KHWRMBatteryLevel ) );
        //Also Charging status will be updated with the following function.
        UpdateBatteryBarsL( state );    
        }
        
    if( IsStateNormal() )
        {
        TRACES( RDebug::Print(_L("CSysApAppUi::DoStateChangedL to normal state.") ) );
               
        iSysApBtController = CreateSysApBtControllerL( *this );
        iSysApBtSapController = CreateSysApBtSapControllerL( *this );

        if ( iActivateBt )
            {
            TRACES( RDebug::Print(_L("CSysApAppUi::DoStateChangedL Activating BT" ) ) );
            SetBtPowerState( ETrue );
            }

        if ( iDeactivateBt )
            {
            TRACES( RDebug::Print(_L("CSysApAppUi::DoStateChangedL Deactivating BT" ) ) );
            SetBtPowerState( EFalse );
            }

        if ( !StarterSession().Handle() )
            {
            User::Leave( KErrBadHandle );
            }

/*        if ( !KeyLock().Handle() )
            {
            User::Leave( KErrBadHandle );
            }
*/
#ifdef __SYSAP_MODULE_TEST
        ModuleTestShowUiNoteL( _L("SysAp: SW state normal!") );
#endif
        }

    // Allow lights
    iSysApLightsController->AllowLightsOn();
    }

#endif // SYSAP_USE_STARTUP_UI_PHASE

// ----------------------------------------------------------------------------
// CSysApAppUi::~CSysApAppUi()
// ----------------------------------------------------------------------------

CSysApAppUi::~CSysApAppUi()
    {
    TRACES( RDebug::Print( _L("~CSysApAppUi() started") ) );
    if( !iResourcesFreed )
      {
        FreeResources();

        iStarterSession.Close();

        }

    delete iSysApShutdownImage;

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
    if (iSysApShutdownAnimation)
        {
        RemoveFromStack( iSysApShutdownAnimation );
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
        delete iSysApShutdownAnimation;
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

//    delete iProfileNote;

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
    if( iAnimTimer )
        {
        iAnimTimer->Cancel();
        }
    delete iAnimTimer;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    delete iSysApStartupController;

#ifdef RD_MULTIPLE_DRIVE
    iInsertedMemoryCards.Close();
    delete  iSysApDriveEjectHandler;
    delete iSysApDriveUnlockHandler;
    delete iSysApDriveList;
#endif // RD_MULTIPLE_DRIVE

    TRACES( RDebug::Print( _L("~CSysApAppUi() completed") ) );
    }
        
TBool CSysApAppUi::ResourcesFreed() const
    {
    return iResourcesFreed;
    }

void CSysApAppUi::ShowExampleUiNoteL( const TDesC& noteText )const
    {          
 	TRACES( RDebug::Print( _L("CSysApAppUi::ShowExampleUiNoteL:: constructing CHbDeviceMessageBoxSymbian:BeGIN") ) );    
    CHbDeviceMessageBoxSymbian *note = CHbDeviceMessageBoxSymbian::NewL(CHbDeviceMessageBoxSymbian::EInformation);
 	CleanupStack::PushL(note);
    TRACES( RDebug::Print( _L("CSysApAppUi::ShowExampleUiNoteL:: construction of CHbDeviceMessageBoxSymbian:END") ) ); 
    note->SetTextL(noteText);
	note->SetTimeout(300);
 	TRACES( RDebug::Print( _L("CSysApAppUi:: Display of  CHbDeviceMessageBoxSymbian::Begin") ) );    
    note->ShowL();
	TRACES( RDebug::Print( _L("CSysApAppUi:: Display of  CHbDeviceMessageBoxSymbian::End") ) );
	CleanupStack::PopAndDestroy(note);
    }


void CSysApAppUi::ExecQueryL( TSysapQuery aQuery, TDes8& aReturn )
    {
    ExecQueryL( aQuery, aReturn, KNullDesC8 );
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::ExecQueryL
// ----------------------------------------------------------------------------
//
void CSysApAppUi::ExecQueryL( TSysapQuery aQuery, TDes8& aReturn, const TDesC8& /*aParam*/ )
    {
    TRACES( RDebug::Print(_L("CSysApAppUi::ExecQueryL: aQuery=%d"), aQuery ) );

    switch ( aQuery )
        {
        case EGetHwrmLight: // get handle to HWRM client session. Returns CHWRMLight*.
            {
            THwrmLightBuf retBuf( iSysApLightsController->GetHwrmLight() );
            aReturn.Copy( retBuf );
            }
            break;

        case EGetKeylock: // get handle to keylock client session. Returns RAknKeylock2*.
            {
            // do nothing
            }
            break;


        default:
            __ASSERT_DEBUG( EFalse, User::Panic( _L("CSysApAppUi::ExecQueryL: Invalid query"), KErrArgument ) );
            User::Leave( KErrArgument );
            break;
        }
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::ExecCommandL
// ----------------------------------------------------------------------------
//
void CSysApAppUi::ExecCommandL( TSysapCommand aCommand )
    {
    ExecCommandL( aCommand, KNullDesC8 );
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::ExecCommandL
// ----------------------------------------------------------------------------
//
void CSysApAppUi::ExecCommandL( TSysapCommand aCommand, const TDesC8&  /* aParam */ )
    {
    TRACES( RDebug::Print(_L("CSysApAppUi::ExecCommandL: aCommand=%d"), aCommand ) );

    switch ( aCommand )
        {
        case EResetKeyguardState: // reset the internal keyguard flags of the SysAp, except for iKeyLockOnBeforeCradle
            iKeyLockOnBeforeCall = EFalse;
            iKeyLockOnBeforeAlarm = EFalse;
            break;

        default:
            // do nothing in release builds since no harm is done
            __ASSERT_DEBUG( EFalse, User::Panic( _L("CSysApAppUi::ExecCommandL: Invalid command"), KErrArgument ) );
            break;
        }
    }
    
// ----------------------------------------------------------------------------
// CSysApAppUi::TimerExpiredL()
// ----------------------------------------------------------------------------
void CSysApAppUi::TimerExpiredL()
    {
/*
 * Not suppported , will take care in wk25
 * #ifndef RD_MULTIPLE_DRIVE 
 * CloseUIAppsInHotSwapL();
 * #endif // RD_MULTIPLE_DRIVE
*/   
   }


  // ----------------------------------------------------------------------------
// CSysApAppUi::KeyLockState() const
// ----------------------------------------------------------------------------

TBool CSysApAppUi::KeyLockState() const
    {
    return iKeyLockEnabled;
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::DeviceLockState() const
// ----------------------------------------------------------------------------

TBool CSysApAppUi::DeviceLockState() const
    {
    return iDeviceLockEnabled;
    }
    
// ----------------------------------------------------------------------------
// CSysApAppUi::SysApFeatureManager
// ----------------------------------------------------------------------------

CSysApFeatureManager& CSysApAppUi::SysApFeatureManager()
    {
    __ASSERT_DEBUG( iSysApFeatureManager, User::Panic( _L("iSysApFeatureManager == NULL"), KErrBadHandle ) );

    return *iSysApFeatureManager;
    }
    
// ----------------------------------------------------------------------------
// CSysApAppUi::ActivateKeyeventForwardingForLights()
// ----------------------------------------------------------------------------

void CSysApAppUi::ActivateKeyeventForwardingForLights(TBool aActivate)
    {
    if( iSysApNspsHandler )
        {
        if ( aActivate )
            {
            iSysApNspsHandler->ActivateKeyeventForwardingForLights();
            }
        else
            {
            iSysApNspsHandler->DeActivateKeyeventForwardingForLights();
            }
        }
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::StarterSession()
// ----------------------------------------------------------------------------

RStarterSession& CSysApAppUi::StarterSession()
    {
    if ( !iStarterSession.Handle() )
        {
        // All server connections are tried to be made KTriesToConnectServer times because occasional
        // fails on connections are possible at least on some servers
        TRACES( RDebug::Print( _L("CSysApAppUi::StarterSession: trying RStarterSession::Connect()") ) );
        TInt thisTry = 0;
        TInt err;
        while ( ( err = iStarterSession.Connect() ) != KErrNone && ( thisTry++ ) <= KTriesToConnectServer )
            {
            User::After( KTimeBeforeRetryingServerConnection );
            }

        if ( err != KErrNone )
            {
            // What do in error case?
            TRACES( RDebug::Print( _L("CSysApAppUi::StarterSession: RStarterSession::Connect() failed with %d"), err ) );
            }
        }

    return iStarterSession;
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::SetBtPowerState()
// ----------------------------------------------------------------------------
TInt CSysApAppUi::SetBtPowerState( TBool aBtState )
    {
    if ( iSysApBtController )
        {
        return iSysApBtController->SetPowerState( aBtState );
        }
    else
        {
        return KErrNotReady;
        }
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::SetStarterState()
// ----------------------------------------------------------------------------

TInt CSysApAppUi::SetStarterState( const RStarterSession::TGlobalState aState )
    {
    if ( iSysApStartupController->GlobalStateChangeAllowed( aState ) )
        {
        TRACES( RDebug::Print( _L("CSysApAppUi::SetStarterState: aState=%d" ), aState ) );
        return StarterSession().SetState( aState );
        }
    else
        {
        TRACES( RDebug::Print( _L("CSysApAppUi::SetStarterState: aState=%d - not allowed" ), aState ) );
        return KErrNone;
        }
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::SetNetworkConnectionAllowed()
// ----------------------------------------------------------------------------

void CSysApAppUi::SetNetworkConnectionAllowed( TCoreAppUIsNetworkConnectionAllowed aNetworkConnectionAllowed )
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::SetNetworkConnectionAllowed: status: %d" ), aNetworkConnectionAllowed ) );

    iSysApCenRepController->SetInt( KCRUidCoreApplicationUIs, KCoreAppUIsNetworkConnectionAllowed, (TInt) aNetworkConnectionAllowed );

    // Also set the Symbian PS key used for the same purpose:
    RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, aNetworkConnectionAllowed ? ESAPhoneOn : ESAPhoneOff);
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::BluetoothPowerMode()
// ----------------------------------------------------------------------------
TInt CSysApAppUi::BluetoothPowerMode() const
    {
    return iSysApCenRepBtObserver->BluetoothPowerMode();
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::SimSupported()
// ----------------------------------------------------------------------------
TBool CSysApAppUi::SimSupported()
    {
    return iSysApFeatureManager->SimSupported();
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::BtSapEnabled()
// ----------------------------------------------------------------------------

TBool CSysApAppUi::BtSapEnabled()
    {
    if ( iSysApBtSapController )
        {
        return iSysApBtSapController->BtSapEnabled();    
        }  
    return EFalse;
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::OfflineModeChangedL()
// ----------------------------------------------------------------------------
void CSysApAppUi::OfflineModeChangedL()
    {
#ifdef SYSAP_USE_STARTUP_UI_PHASE    
    // if UI is not ready, don't update indicators
    if ( !UiReady() )
        {
        return;
        }
#endif // SYSAP_USE_STARTUP_UI_PHASE

#if 0 // Not used as of now
    SetHacIndicatorL();
#endif //0	
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::UiReady()
// 
// ----------------------------------------------------------------------------

TBool CSysApAppUi::UiReady() const
    {
#ifdef SYSAP_USE_STARTUP_UI_PHASE    
    return iSysApStartupController->UiReady(); 
#else // SYSAP_USE_STARTUP_UI_PHASE
    // if startup UI phase information is not used, global system state normal is handled as UI idle state
    return IsStateNormal();
#endif // SYSAP_USE_STARTUP_UI_PHASE        
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::CompleteShutdown()
// ----------------------------------------------------------------------------

void CSysApAppUi::CompleteShutdown( const TBool aReset, const TInt aResetReason )
    {
    TRACES( RDebug::Print(_L("CSysApAppUi::CompleteShutdown(): START" ) ) );
    
    iCheckLongPowerKeyEvent = ETrue;

    PrepareForShutdownImage();//SysAp's internal preparation for ShutDown with image

    FreeResources();

    if ( aReset )
        {
        __ASSERT_DEBUG( aResetReason >= RStarterSession::ELanguageSwitchReset &&
                        aResetReason <= RStarterSession::EDataRestoreReset,
                        User::Invariant() );
        StarterSession().Reset( static_cast<RStarterSession::TResetReason>( aResetReason ) );
        }
    else
        {
        StarterSession().Shutdown();
        }

    StarterSession().Close();

    TRACES( RDebug::Print(_L("CSysApAppUi::CompleteShutdown(): END" ) ) );
    }



// ----------------------------------------------------------------------------
// CSysApAppUi::DoShutdownL( const TBool aReset, const TSWStartupReason aSWStartupReason )
// ----------------------------------------------------------------------------

void CSysApAppUi::DoShutdownL( const TBool aReset, const TInt aResetReason )
    {
    TRACES( RDebug::Print(_L("CSysApAppUi::DoShutdownL: aResetReason:%d, aReset:%d" ),
        aResetReason, aReset ) );
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
    TBool animationenabled( EFalse );
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    if( OkToInitiateShutdown() )
        {
        TRACES( RDebug::Print(_L("CSysApAppUi::DoShutdownL: Was OkToToInitiateShutdown" ) ) );


        if ( !aReset && iSysApFeatureManager->Supported(KSysApFeatureIdGoodbyeNote) )
            {
            // show shutdown note
            }

        if( !aReset )
            {
    #ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
            TRAPD( err, ShowAnimationL() );
            if ( err )
                {
                TRACES( RDebug::Print(_L("CSysApAppUi::DoShutdownL ShowAnimationL() leaved: %d" ), err ) );
                CompleteShutdown(aReset, aResetReason);
                }
            }
        else // aReset
           {
            CompleteShutdown(aReset, aResetReason);
            }
    #else // RD_STARTUP_ANIMATION_CUSTOMIZATION
            TRAPD( err, animationenabled = ShowAnimationL() );
            if ( err )
                {
                TRACES( RDebug::Print(_L("CSysApAppUi::DoShutdownL ShowAnimationL() leaved: %d" ), err ) );
                }
            }

        if ( !animationenabled )
            {
            CompleteShutdown(aReset, aResetReason);
            }
    #endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
        }
    else
        {
        TRACES( RDebug::Print(_L("CSysApAppUi::DoShutdownL: Was not OkToToInitiateShutdown" ) ) );
        }
    TRACES( RDebug::Print(_L("CSysApAppUi::DoShutdownL: END" ) ) );
    }



/* 
----------------------------------------------------------------------------
CSysApAppUi::IsEncryptionOperationOngoingL()
Introduced the code as a part of Sub : 405-3362 - Power Management SUB for ES 
recapture functionalities in platform which support multidrive as well. 
-------------------------------------------------------------------------------
*/
TBool CSysApAppUi::IsEncryptionOperationOngoingL() const
    {
        TBool deFeatureSupported(EFalse);
        FeatureManager::InitializeLibL();
        deFeatureSupported = FeatureManager::FeatureSupported( KFeatureIdFfDeviceEncryptionFeature);
        FeatureManager::UnInitializeLib();

        if(deFeatureSupported)
            {         
            /**
             * Store the last memory status changed
             * 0: Idle. It can be Encrypted or Decrypted
             * 1: Encrypting
             * 2: Decrypting
             **/
             
            RProperty deProperty;
            User::LeaveIfError(deProperty.Attach(KDevEncProtectedUid, KDevEncOperationKey,EOwnerThread));
            TInt deValue = 0;
            if((deProperty.Get(deValue)== KErrNone)&&( deValue == EOpEncrypting || deValue == EOpDecrypting))
                {
                deProperty.Close();
                return ETrue;
                }
            else
                {
                deProperty.Close();
                return EFalse;
                }
            }
        else
            {
            return EFalse;
            }
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::HandleLongPowerKeyPressedL()
// ----------------------------------------------------------------------------

void CSysApAppUi::HandleLongPowerKeyPressedL()
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::HandleLongPowerKeyPressedL()") ) );

    TInt swState;
    RProperty::Get( KPSUidStartup, KPSGlobalSystemState, swState );
    
    iSysApLightsController->PowerKeyPressedL();  //need to handle later.

    if( swState == ESwStateStartingCriticalApps ||
        swState == ESwStateSelfTestOK ||
        swState == ESwStateSecurityCheck ||
        swState == ESwStateCriticalPhaseOK ||
        swState == ESwStateEmergencyCallsOnly ||
        swState == ESwStateNormalRfOn ||
        swState == ESwStateNormalRfOff ||
        swState == ESwStateNormalBTSap ||
        swState == ESwStateFatalStartupError )
        {
        if( !iDeviceLockEnabled )
            {
            if( iPowerKeyPopupMenuActive )
                {
                // do nothing
                }
            else if( !iIgnoreNextPowerKeyRepeats )
                {
                if( !iKeyLockEnabled && !IsEncryptionOperationOngoingL() )
                    {
                    DoShutdownL( EFalse, KDummyReason );
                    }
                }
            iIgnoreNextPowerKeyRepeats = ETrue;
            }
        }
    else if( swState == ESwStateAlarm )
        {
        //Just comment here to highlight that this is not possible since
        //from alarm state already a short press always shuts the device down.
        }
    else if( swState == ESwStateCharging )
        //Startup to Normal state
        {
        if( !iIgnoreNextPowerKeyRepeats )
            {
            iIgnoreNextPowerKeyRepeats = ETrue;

            SetStarterState( RStarterSession::ENormal );
            }
        }
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::IsDeviceModeKey
// ----------------------------------------------------------------------------

TBool CSysApAppUi::IsDeviceModeKey( const TKeyEvent& aKeyEvent ) const
    {
    return CSysApDefaultKeyHandler::IsDeviceModeKey( aKeyEvent );
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::ShowShutdownImage
// ----------------------------------------------------------------------------
//
TInt CSysApAppUi::ShowShutdownImage( TInt aBitmapId )
    {
    TRACES( RDebug::Print(_L("CSysApAppUi::ShowShutdownImage") ) );

    TRAPD( err, ShowShutdownImageL( aBitmapId ) );

    if ( err != KErrNone )
        {
        TRACES( RDebug::Print(_L("CSysApAppUi::ShowShutdownImageL failed, err=%d"), err ) );
        }

    return err;
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::ShowShutdownImageL
// ----------------------------------------------------------------------------
//
void CSysApAppUi::ShowShutdownImageL( TInt aBitmapId )
    {
    TRACES( RDebug::Print(_L("CSysApAppUi::ShowShutdownImageL") ) );
    // do nothing
    if ( !iSysApShutdownImage ) // Shutdown image may be shown without entering normal state (e.g. charging), so construct here
        {
        TRACES( RDebug::Print( _L("CSysApAppUi::ShowShutdownImageL: trying CSysApShutdownImage::NewL()") ) );
        iSysApShutdownImage = CSysApShutdownImage::NewL();
        }
    TRACES( RDebug::Print( _L("CSysApAppUi::ShowShutdownImageL: trying CSysApShutdownImage::ShowShutdownImageL()") ) );
    iSysApShutdownImage->ShowShutdownImageL( aBitmapId );
   }
   
// ----------------------------------------------------------------------------
// CSysApAppUi::PrepareForShutdownImage()
// ----------------------------------------------------------------------------

void CSysApAppUi::PrepareForShutdownImage()
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::PrepareForShutdownImage() START" ) ) );
 
    // If RD_STARTUP_ANIMATION_CUSTOMIZATION is enabled, the following actions are always taken in PrepareForShutdownAnimation
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
    if ( !iAnimationShowingTime ) // actions already taken in PrepareForShutdownAnimation
        {
        static_cast<CAknAppUi*>(iEikonEnv->EikAppUi())->KeySounds()->PlaySound( EAvkonSIDPowerOffTone );
        CancelGlobalListQuery();
        
        if ( iSysApPowerKeyMenuObserver )
            {
            iSysApPowerKeyMenuObserver->Cancel();
            }
        
        // deactivate notifiers before showing the shutdown image
        AknNotifierController::CancelAllNotifications();
        AknNotifierController::HideAllNotifications(ETrue);
        RWindowGroup groupWin = iCoeEnv->RootWin();
        iCapturedAppskey = groupWin.CaptureKey( EKeyApplication, KModifierMask, KModifierMask );
        iCapturedAppskeyUpAndDowns = groupWin.CaptureKeyUpAndDowns( EStdKeyApplication0, KModifierMask, KModifierMask );
        }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

#ifndef __SYSAP_MODULE_TEST //to make errors during shutdown sequence being seen more easily
    iEikonEnv->RootWin().SetOrdinalPosition(0, ECoeWinPriorityAlwaysAtFront );
    TRACES( RDebug::Print( _L("CSysApAppUi::PrepareForShutdownImage() Show Shutdown image" ) ) );
    ShowShutdownImage( 1  );//EMbmSysapQgn_startup_screen
#endif
    TRACES( RDebug::Print( _L("CSysApAppUi::PrepareForShutdownImage() END" ) ) );
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::OkToInitiateShutdown()
// ----------------------------------------------------------------------------

TBool CSysApAppUi::OkToInitiateShutdown()
    {
    TInt swState;
    RProperty::Get( KPSUidStartup, KPSGlobalSystemState, swState );

    if( !iShutdownStarted &&
        swState == ESwStateStartingCriticalApps ||
        swState == ESwStateSelfTestOK ||
        swState == ESwStateSecurityCheck ||
        swState == ESwStateCriticalPhaseOK ||
        swState == ESwStateEmergencyCallsOnly ||
        swState == ESwStateCharging ||
        swState == ESwStateAlarm ||
        swState == ESwStateNormalRfOn ||
        swState == ESwStateNormalRfOff ||
        swState == ESwStateNormalBTSap ||
        swState == ESwStateFatalStartupError )
        {
        iShutdownStarted = ETrue;
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }


void CSysApAppUi::PopupNote()
    {
    iIgnoreNextPowerKeyUpEvent = ETrue;
    // TRACES( RDebug::Print( _L("CSysApAppUi::Key was consumed") ) );
    _LIT(KPowerPressKey,"Short power key");
    HBufC* aString = HBufC16::NewLC(50);
    TPtrC aStringPointer = aString->Des();
    aStringPointer.Set(KPowerPressKey);
    TRACES( RDebug::Print( _L("CSysApWsClient::RunL(): Key EEventKeyUp 01") ) );   
    ShowExampleUiNoteL( aStringPointer );  
    CleanupStack::PopAndDestroy(); // aString
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::SetUsbAttachStatus
// ----------------------------------------------------------------------------
#ifndef RD_MULTIPLE_DRIVE
void CSysApAppUi::SetUsbAttachStatus( const TBool aUsbAttached )
    {
    // Prevent double beep when USB file transfer gets activated but
    // allow MMC inserted beep when:
    // a) USB file transfer is active
    // b) MMC is not inserted when USB cable is attached
    iHideNextBeep = ( iMMCInserted && aUsbAttached );
    }
#else // RD_MULTIPLE_DRIVE
void CSysApAppUi::SetUsbAttachStatus( const TBool aUsbAttached )
    {
    TRACES( RDebug::Print(
        _L( "CSysApAppUi::SetUsbAttachStatus: aUsbAttached: %d" ),
        aUsbAttached ) );

    if ( aUsbAttached )
        {
          // For ignoring extra beeps caused by USB file transfer
        iSysApDriveList->ResetDrivesInsertBeepIgnored();
        iSysApDriveList->MarkDrivesInsertBeepIgnored( iInsertedMemoryCards );
        }
    }
#endif // RD_MULTIPLE_DRIVE


// ----------------------------------------------------------------------------
// CSysApAppUi::UsbChargerDetector
// ----------------------------------------------------------------------------
//
TSysApUsbChargerDetector& CSysApAppUi::UsbChargerDetector()
    {
    return iSysApUsbChargerDetector;
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::HandleAccessoryDisconnectedL()
// ----------------------------------------------------------------------------

void CSysApAppUi::HandleAccessoryDisconnectedL()
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::HandleAccessoryDisconnectedL()" ) ) );


    TAccMode accessoryState(EAccModeHandPortable);
    if ( iSysApAccessoryObserver )
        {
        accessoryState = iSysApAccessoryObserver->GetAccessoryMode();
        }

    if ( accessoryState == EAccModeHandPortable )
        {
        TRACES( RDebug::Print( _L("CSysApAppUi::HandleAccessoryDisconnectedL:Before Deactivating accessory Plugin")));
        iHbIndicatorSymbian->Deactivate(KAccesoryPlugin);
        TRACES( RDebug::Print( _L("CSysApAppUi::HandleAccessoryDisconnectedL:After  Deactivating accessory Plugin")));
        iSysApLightsController->AccessoryConnectedL( EFalse );
        iSysApCenRepController->SetInt( KCRUidCoreApplicationUIsSysAp, KSysApAccessoryConnected, 0 );
        }

    SetIhfIndicatorL();
    SetHacIndicatorL();

    // If lock disabled because of accessory, reactivate it, unless phone call or alarm is ongoing
    if ( iKeyLockOnBeforeCradle )
        {
        iKeyLockOnBeforeCradle = EFalse;
        if ( !iKeyLockOnBeforeCall && !iKeyLockOnBeforeAlarm )
            {
            if ( !iDeviceLockEnabled )
                {
                iKeyguardController->EnableKeyguard(ETrue);
                }
            else
                {
                iKeyguardController->EnableKeyguard(EFalse);
                }
            }
        }
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::HandleAccessoryConnectedL()
// ----------------------------------------------------------------------------

void CSysApAppUi::HandleAccessoryConnectedL( TAccMode aAccessoryState, TInt aPhysicalConnectionType )
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::HandleAccessoryConnectedL( aAccessoryState: %d )(aPhysicalConnectionType: %d "), aAccessoryState, aPhysicalConnectionType ) );

    CHbSymbianVariantMap* iAccVariantMap = CHbSymbianVariantMap::NewL();
    CleanupStack::PushL(iAccVariantMap);
    CHbSymbianVariant* variantAccState = CHbSymbianVariant::NewL(&aAccessoryState, CHbSymbianVariant::EInt);
    iAccVariantMap->Add(KAccMode,variantAccState);
    CHbSymbianVariant* variantAccType = CHbSymbianVariant::NewL(&aPhysicalConnectionType, CHbSymbianVariant::EInt);
    iAccVariantMap->Add(KAccPhyConType,variantAccType);
    
    
    CHbSymbianVariant* iAccVariant = CHbSymbianVariant::NewL(iAccVariantMap, CHbSymbianVariant::EVariantMap ); 
    CleanupStack::PushL(iAccVariant);
    iHbIndicatorSymbian->Activate(KAccesoryPlugin, iAccVariant);


    TInt swState( StateOfProperty( KPSUidStartup, KPSGlobalSystemState ) );
    TRACES( RDebug::Print( _L("CSysApAppUi::HandleAccessoryConnectedL: swState: %d"), swState ) );

    if( UiReady() || swState == ESwStateSecurityCheck )
        {
        iSysApLightsController->AccessoryConnectedL( ETrue );
        }

    if ( ( ! iSysApOfflineModeController->OfflineModeActive() ))
        {
        if ( iSysApCenRepController->GetInt( KCRUidCoreApplicationUIsSysAp, KSysApAccessoryConnected ) < 1 )
            // accessory not connected already
             {
            iSysApCenRepController->SetInt( KCRUidCoreApplicationUIsSysAp, KSysApAccessoryConnected, 1 );
           }
        }
    
    // Carkit and music stand have also light on permanently option that needs to be checked
    if( aAccessoryState == EAccModeWiredCarKit || aAccessoryState == EAccModeMusicStand )
        {
        // When device is placed on a handsfree cradle, keyguard must be disabled (unless device is locked)
        // Note: It is assumed that if carkit/music stand is connected, the phone is in the cradle.
        if ( iKeyLockEnabled || iDeviceLockEnabled || iKeyLockOnBeforeCall || iKeyLockOnBeforeAlarm )
            {
            iKeyLockOnBeforeCradle = ETrue;

            if ( iKeyLockEnabled && !iDeviceLockEnabled )
                {
                iKeyguardController->DisableKeyguard(EFalse);
                }
            }
        }
    SetIhfIndicatorL();
    SetHacIndicatorL();
    CleanupStack::PopAndDestroy(2);
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::SetHacIndicatorL()
// ----------------------------------------------------------------------------
void CSysApAppUi::SetHacIndicatorL()
    {

    if ( !iSysApCenRepHacSettingObserver )
        {
        // Do nothing if HAC setting observer has not been created yet
        // This occurs in case some party is trying to set HAC indicator during
        // ConstructL(), e.g. when switching from offline to online during startup
        TRACES( RDebug::Print( _L("CSysApAppUi::SetHacIndicatorL: do nothing, HAC observer does not exist yet") ) );
        return;
        }

    if ( iSysApCenRepHacSettingObserver->HacMode() )
        {
        TBool accessoryConnected ( EFalse );

        TAccMode accMode(EAccModeHandPortable);

        if ( iSysApAccessoryObserver )
            {
            accMode = iSysApAccessoryObserver->GetAccessoryMode();

            if ( accMode != EAccModeHandPortable )
                {
                accessoryConnected = ETrue;
                }
            }

//        TInt indicatorState = EAknIndicatorStateOn;

        if ( !accessoryConnected ) // When accessory is not connected, check offline and call state
            {
            if ( OfflineModeActive() ) // HAC indicator is not shown in offline mode
                {
 //               indicatorState = EAknIndicatorStateOff;
                }
            else
                {
                // during active call with IHF activated HAC indicator is not shown
                TInt callState( StateOfProperty( KPSUidCtsyCallInformation, KCTsyCallState ) );
                if ( callState < KErrNone )
                    {
                    callState = EPSCTsyCallStateNone;
                    }

                if ( callState != EPSCTsyCallStateNone && IhfEnabledL() )
                    {
 //                   indicatorState = EAknIndicatorStateOff;
                    }
                }

            }
        else
            {
            // If an accessory is connected, HAC indicator should not be shown
//            indicatorState = EAknIndicatorStateOff;
            }

 //       SetIndicatorStateL( EAknIndicatorHAC, indicatorState );

        }
    else
        {
        // If HAC is not active, always disable indicator regardless of accessory state
 //       SetIndicatorStateL( EAknIndicatorHAC, EAknIndicatorStateOff );
        }


    }


// ----------------------------------------------------------------------------
// CSysApAppUi::DoNotActivateBt()
// ----------------------------------------------------------------------------
void CSysApAppUi::DoNotActivateBt()
    {
    iSysApOfflineModeController->DoNotActivateBt();
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::IhfEnabledL
// ----------------------------------------------------------------------------

TBool CSysApAppUi::IhfEnabledL()
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::IhfEnabledL(): iSysApAudioRoutingObserver=0x%x"), iSysApAudioRoutingObserver ) );

    if ( !iSysApAudioRoutingObserver ) // create observer if needed
        {
        iSysApAudioRoutingObserver = CSysApAudioRoutingObserver::NewL( *this );
        }

    return iSysApAudioRoutingObserver->IhfEnabled();
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::SetIhfIndicatorL()
// ----------------------------------------------------------------------------
void CSysApAppUi::SetIhfIndicatorL()
    {
    TBool accessoryConnected ( EFalse );

    TInt callState( StateOfProperty( KPSUidCtsyCallInformation, KCTsyCallState ) );

    TAccMode accMode(EAccModeHandPortable);

    if ( iSysApAccessoryObserver )
        {
        accMode = iSysApAccessoryObserver->GetAccessoryMode();

        if ( accMode != EAccModeHandPortable )
            {
            accessoryConnected = ETrue;
            }
        }

    if( (callState != EPSCTsyCallStateNone && IhfEnabledL() && !accessoryConnected)
        || accMode == EAccModeMusicStand )  // Music stand always shows IHF indicator
        {
        if ( accMode == EAccModeMusicStand )
            {
            // do nothing
            }
        }
    else
        {
        // do nothing
        }
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::EjectStarted()
// ----------------------------------------------------------------------------

void CSysApAppUi::EjectStarted( TBool ejectStarted )
    {
    TRACES( RDebug::Print( _L( "CSysApAppUi::EjectStarted" ) ) );
    iMMCEjectUsed = ejectStarted;
    
#ifndef RD_MULTIPLE_DRIVE
    iFileManagerCloseDisabled = ejectStarted;
#endif // RD_MULTIPLE_DRIVE
}

// ----------------------------------------------------------------------------
// CSysApAppUi::DoLightsTimeoutChangedL
// ----------------------------------------------------------------------------
void CSysApAppUi::DoLightsTimeoutChangedL( const TInt aTimeout )
    {
    iSysApLightsController->DoLightsTimeoutChangedL( aTimeout );
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::DoSwStateNormalConstructionL()
// ----------------------------------------------------------------------------

void CSysApAppUi::DoSwStateNormalConstructionL()
    {
    // Let's ensure that the lights will remain on for 15 secs after sw state normal
    User::ResetInactivityTime();

    TRACES( RDebug::Print( _L("CSysApAppUi::DoSwStateNormalConstructionL : START" ) ) );

    // In case of unexpected reset (e.g. hidden boot) the keylock must be enabled silently. Locking is done
    // prior to time-consuming initializations, because otherwise UI is in unlocked state for a few seconds.
    if ( iSysApCenRepController->GetInt( KCRUidCoreApplicationUIsSysAp, KSysApKeyguardActive ) == 1  )
        {
        TRACES( RDebug::Print( _L("CSysApAppUi::DoSwStateNormalConstructionL: enabling keylock") ) );
        iKeyguardController->EnableKeyguard(ETrue);
        }

    // Initiate bearer change observation
//    iSysApConnectionMonitorObserver = CSysApConnectionMonitorObserver::NewL( *this );

    TRACES( RDebug::Print( _L("SysAp: trying CSystemLock::NewL" ) ) );
    iSysApSystemLock = CSystemLock::NewL();

    
    if ( iSysApFeatureManager->Supported( KSysApFeatureIdBatteryInfoPopup ) )
        {
        iSysApBatteryInfoController = CSysApBatteryInfoController::NewL( iSysApCenRepController->GetInt( KCRUidCoreApplicationUIsConf, 
                                                                                                     KCoreAppUIsBatteryInformationLowThreshold ) );    
        }
    TRACES( RDebug::Print( _L("CSysApAppUi::DoSwStateNormalConstructionL : END" ) ) );
    }



// ----------------------------------------------------------------------------
// CSysApAppUi::DoSimChangedFromPreviousBootL()
// ----------------------------------------------------------------------------

void CSysApAppUi::DoSimChangedFromPreviousBootL()
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::DoSimChangedFromPreviousBootL: iSimChangedDone=%d" ), iSimChangedDone ) );

    if ( !iSimChangedDone  )
        {
        iSimChangedDone = ETrue;
        CSysApSimChanged* simChanged = CSysApSimChanged::NewL( *this, iEikonEnv->FsSession() );
        CleanupStack::PushL( simChanged );
        simChanged->HandleSimChangedL();
        CleanupStack::PopAndDestroy( simChanged );
        }
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::IgnoreZeroNetworkBarNotifications
// ----------------------------------------------------------------------------
//
void CSysApAppUi::IgnoreZeroNetworkBarNotifications(TBool aIgnore)
    {
    iIgnoreZeroNetworkBarNotifications = aIgnore;
    }



// ----------------------------------------------------------------------------
// CSysApAppUi::SwitchFromOnlineToOfflineModeL()
// ----------------------------------------------------------------------------
void CSysApAppUi::SwitchFromOnlineToOfflineModeL()
    {
   iSysApOfflineModeController->SwitchFromOnlineToOfflineModeL();
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::InitCloseSimApplicationsL()
// ----------------------------------------------------------------------------
void CSysApAppUi::InitCloseSimApplicationsL()
    {
    iApplicationScanningRoundNumberInBtSap = 0;
    iTimeToKillInBtSap = EFalse;
    CloseSimApplicationsL();
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::CloseSimApplicationsL()
// ----------------------------------------------------------------------------
void CSysApAppUi::CloseSimApplicationsL()
    {
    iApplicationScanningRoundNumberInBtSap++;
    TRACES( RDebug::Print( _L("CSysApAppUi::CloseSimApplicationsL: scanning round = %d"),
                           iApplicationScanningRoundNumberInBtSap ) );

    if ( iApplicationScanningRoundNumberInBtSap == KMaxExitTimeInBtSap )
        {
        iTimeToKillInBtSap = ETrue;
        }

    const TInt KNumberOfSimDependantApps( 7 );
    TUid simDependantAppUid[ KNumberOfSimDependantApps ];
    simDependantAppUid[ 0 ] = KSysApGSUid;
    simDependantAppUid[ 1 ] = KSysApSATUid;
    simDependantAppUid[ 2 ] = KSysApMailboxUid;
    simDependantAppUid[ 3 ] = KSysApSpeedDialingUid;
    simDependantAppUid[ 4 ] = KSysApSimDirectoryUid;
    simDependantAppUid[ 5 ] = KSysApMCEUid;
    simDependantAppUid[ 6 ] = KSysApCellBroadcastMessagesUid;

    TBool appsExiting( EFalse );
    TApaTaskList apaTaskList( CCoeEnv::Static()->WsSession() );

    for ( TInt i = 0; i < KNumberOfSimDependantApps; i++ )
        {
        TApaTask apaTask = apaTaskList.FindApp( simDependantAppUid[ i ] );
        if ( apaTask.Exists() )
            {
            appsExiting = ETrue;
            if ( iApplicationScanningRoundNumberInBtSap == 1 )
                {
                apaTask.EndTask();  // applications are kindly requested to close themselves on the first round
                }
            else if ( iTimeToKillInBtSap )
                {
                apaTask.KillTask(); // used after timeout on the last round
                }
            }
        }

    if ( !appsExiting || iTimeToKillInBtSap )
        {
        iSysApBtSapController->SimApplicationsClosed();
        }
    else
        {
        if ( !iSapTimer )
            {
            iSapTimer = CPeriodic::NewL( EPriorityNormal );
            }
        iSapTimer->Cancel();
        iSapTimer->Start( KDelayBeforeNextScanningRound,
                          KDelayBeforeNextScanningRound,
                          TCallBack( DoCloseSimApplicationsAfterCallBack, this ) );
        }
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::DoCloseSimApplicationsAfterCallBack
// ----------------------------------------------------------------------------
TInt CSysApAppUi::DoCloseSimApplicationsAfterCallBack( TAny* aObject )
    {
    CSysApAppUi* appUi = STATIC_CAST( CSysApAppUi*, aObject );

    if ( appUi->iSapTimer )
        {
        appUi->iSapTimer->Cancel();
        }

    TRAP_IGNORE( appUi->CloseSimApplicationsL() );

    return KErrNone;
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::NotifyPowerSaveModeL
// ----------------------------------------------------------------------------
//
void CSysApAppUi::NotifyPowerSaveModeL( TSysApPsmStatus aStatus )
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::NotifyPowerSaveModeL aStatus: %d"), aStatus ) );

    // cancel any active power saving query because user has changed the state manually
    // CancelQuery( ESysApBattChargingPowerSavingQuery );
    // CancelQuery( ESysApBattLowPowerSavingQuery );
    // will take care in next sub
    
    switch ( aStatus )
        {
        case MSysApPsmControllerNotifyCallback::EPsmActivationComplete:
             UpdateBatteryBarsL( StateOfProperty( KPSUidHWRMPowerState, KHWRMBatteryLevel ) );
             iVariantAccState = CHbSymbianVariant::NewL(&KPsm, CHbSymbianVariant::EDes);
             if (!iHbIndicatorSymbian->Activate(KPsmPlugin,iVariantAccState)) 
                {
                int error = iHbIndicatorSymbian->Error();
                //use the errorcode...
                }
             break;
        
        case MSysApPsmControllerNotifyCallback::EPsmDeactivationComplete:
             UpdateBatteryBarsL( StateOfProperty( KPSUidHWRMPowerState, KHWRMBatteryLevel ) );
             if (!iHbIndicatorSymbian->Deactivate(KPsmPlugin)) 
                {
                int error = iHbIndicatorSymbian->Error();
                 //use the errorcode...
                }
            break;
            
        case MSysApPsmControllerNotifyCallback::EPsmActivationFailed:
            ShowUiNoteL( ECannotActivatePowerSaveMode );
            break;
            
        case MSysApPsmControllerNotifyCallback::EPsmDeactivationFailed:
            ShowUiNoteL( ECannotDeactivatePowerSaveMode );
            break;            
        
        default:
            break;   
        }
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::ShowAlarmIndicatorL()
// ----------------------------------------------------------------------------

void CSysApAppUi::ShowAlarmIndicatorL( TBool aShowIndicator )
    {
    if ( aShowIndicator)
        {
        // do nothing
        }
    else
        {
        // do nothing
        }
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::AlertUiAlertingL( const TBool aAlerting )
// ----------------------------------------------------------------------------
void CSysApAppUi::AlertUiAlertingL( const TBool aAlerting )
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::AlertUiAlerting aAlerting:%d"), aAlerting ) );

    if ( aAlerting )
        {
        TInt swState;
        RProperty::Get( KPSUidStartup, KPSGlobalSystemState, swState );

        if( swState == ESwStateCharging )
            {
            SetStarterState( RStarterSession::EAlarm );
            }
        }

    iSysApLightsController->AlarmOccuredL( aAlerting );
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::HandleForcedLightsVTRequireL( const TInt aLightParameter )
// ----------------------------------------------------------------------------

void CSysApAppUi::HandleForcedLightsVTRequireL( const TInt aLightsParameter ) const
    {
    iSysApLightsController->HandleForcedLightsVTRequireL( aLightsParameter );
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::HandleForcedLightsSSRequireL( const TInt aLightParameter )
// ----------------------------------------------------------------------------

void CSysApAppUi::HandleForcedLightsSSRequireL( const TInt aLightsParameter ) const
    {
    iSysApLightsController->HandleForcedLightsSSRequireL( aLightsParameter );
    }

// ---------------------------------------------------------------------------- 
// CSysApAppUi::HandleForcedLightsATCRequireL( const TInt aLightParameter ) 
// ---------------------------------------------------------------------------- 
void CSysApAppUi::HandleForcedLightsATCRequireL( const TInt aLightsParameter ) const 
    { 
    iSysApLightsController->HandleForcedLightsATCRequireL( aLightsParameter ); 
    } 


// ----------------------------------------------------------------------------
// CSysApAppUi::HandleLightsRequireL()
// ----------------------------------------------------------------------------

void CSysApAppUi::HandleLightsRequireL() const
    {
    iSysApLightsController->HandleLightsRequireL();
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::HandleRawKeyEventLightsRequireL()
// ----------------------------------------------------------------------------

void CSysApAppUi::HandleRawKeyEventLightsRequireL() const
    {
    iSysApLightsController->HandleRawKeyEventLightsRequireL();
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::AlertUiKeyLockOff( const TBool aKeyLockOff )
// ----------------------------------------------------------------------------

void CSysApAppUi::AlertUiKeyLockOff( const TBool aKeyLockOff )
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::AlertUiKeyLockOff aKeyLockOff:%d, devicelock:%d"), 
        aKeyLockOff, iDeviceLockEnabled ) );
    
    if( aKeyLockOff )
        {
        // Notification to release keypad might come several times if e.g. calls are received
        if ( !iKeyLockOnBeforeAlarm )
            {
            if ( iKeyLockEnabled || iKeyLockOnBeforeCradle || iKeyLockOnBeforeCall )
                {
                iKeyLockOnBeforeAlarm = ETrue;
                }
            }
        iKeyguardController->DisableKeyguard(EFalse);
        }
    else
        {
        if (iDeviceLockEnabled)
            {
            // Re-enable keylock in devicelock case only if phone call is not ongoing or is 
            // connected. Otherwise e.g. messages can be read using softkeys during ongoing call.
            TInt callState( StateOfProperty( KPSUidCtsyCallInformation, KCTsyCallState ) );
            if ( callState == EPSCTsyCallStateNone || callState == EPSCTsyCallStateConnected )
                {
                iKeyguardController->EnableKeyguard(EFalse);
                }
            }
        else if ( iKeyLockOnBeforeAlarm && !iKeyLockOnBeforeCradle && !iKeyLockOnBeforeCall )
            {
            iKeyguardController->EnableKeyguard(ETrue);
            }
        iKeyLockOnBeforeAlarm = EFalse;
        }
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::DoNotEnableKeylock()
// ----------------------------------------------------------------------------

void CSysApAppUi::DoNotEnableKeylock()
    {
    if ( !iKeyLockOnBeforeCradle )
        {
        iShowkeypadActivatedNoteAfterSoftReject = iKeyLockOnBeforeCall;
        }

    iKeyLockOnBeforeCall = EFalse;
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::SwitchLightsOnSoftRejectL()
// ----------------------------------------------------------------------------
void CSysApAppUi::SwitchLightsOnSoftRejectL()
    {
    iSysApLightsController->SwitchLightsOnSoftRejectL();
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::SetDeviceLockEnabledL( const TBool aLockEnabled )
// ----------------------------------------------------------------------------

void CSysApAppUi::SetDeviceLockEnabledL( const TBool aLockEnabled )
    {
    TRACES( RDebug::Print(_L("CSysApAppUi::SetDeviceLockEnabledL aLockEnabled:%d" ), aLockEnabled ) );
    TBool currentState( iDeviceLockEnabled );
    
    iDeviceLockEnabled = aLockEnabled;
    
    if ( currentState != iDeviceLockEnabled ) // filter out unnecessary light status modifications
        {
        iSysApLightsController->DeviceLockStateChangedL( iDeviceLockEnabled );
        }
    
    if( iDeviceLockEnabled )
        {
        if( iKeyLockEnabled )
            {
            iKeyguardController->DisableKeyguard(EFalse);
            }
        }
    else
        {
        // let's not activate keylock in case device lock was disabled during call or in cradle
        iKeyLockOnBeforeCall = EFalse;
        iKeyLockOnBeforeCradle = EFalse;

        // Memory card needs to be unlocked when device is unlocked
        if ( iSysApFeatureManager->MmcHotSwapSupported() )
            {
             RunUnlockNotifierL();
            // Need to handle feature for unlocking the MMC card
            }
        }
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::HandleBatteryStatusL
// ----------------------------------------------------------------------------
//
void CSysApAppUi::HandleBatteryStatusL( const TInt aValue )
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::HandleBatteryStatusL aValue: %d"), aValue ) );
    
    TBool enableAutoPsm(EFalse);
    CRepository* repository( NULL );
    TRAPD( err, repository = CRepository::NewL( KCRUidDeviceManagementSettings ) );
        
    if ( err == KErrNone )
       {
       TInt value( 0 );
       err = repository->Get( KSettingsPowerSavingQuery, value );
            
       if ( err == KErrNone )
          {
          enableAutoPsm = value ? EFalse: ETrue;
          }
       }
      
     _LIT(Klowbattery, "txt_power_management_dpopinfo_low_battery");
     // returns the string "low battery"
     HBufC* lowBattery = HbTextResolverSymbian::LoadL(Klowbattery);    
    
    
    if ( aValue == EBatteryStatusEmpty )
        {
        //Display Recharge Battery note
        ShowUiNoteL( ERechargeBatteryNote );
        BatteryEmptyL();
        }
    else if ( aValue == EBatteryStatusLow )
        {
        if ( iSysApPsmController && UiReady() )
            {
            iSysApPsmController->BatteryLow( ETrue );
            
                       
            if ( enableAutoPsm)
                {
                iSysApPsmController->DoEnableFullPsm( ETrue );
                          
                _LIT(KPsmOn, "txt_power_management_dpopinfo_psm_activated_automa");
                // returns the string "power saving mode  on"
                HBufC* psmOn = HbTextResolverSymbian::LoadL(KPsmOn);
                             
               _LIT(Kicon, "qgn_indi_battery_ps_activate");
               // returns the power save mode icon
               HBufC* psmIcon = HbTextResolverSymbian::LoadL(Kicon);                  
            
               CHbDeviceNotificationDialogSymbian::NotificationL(*psmIcon,*psmOn,*lowBattery);       
                }
            else // default low warning note must be shown
                {
                //Display Battery Low note.
                CHbDeviceNotificationDialogSymbian::NotificationL(KlowbatteryIcon,KNullDesC,*lowBattery);   
                }                
            }
        else
            {
            //Display Battery Low note.
            CHbDeviceNotificationDialogSymbian::NotificationL(KlowbatteryIcon,KNullDesC,*lowBattery);     
            }            
        }
        
    if ( iSysApBatteryInfoController )
    {
     iSysApBatteryInfoController->BatteryStatusUpdated( aValue );
    }
      
 delete repository;  
   }

// ----------------------------------------------------------------------------
// CSysApAppUi::ShowUiNoteL( const TSysApNoteIds aNote ) const
// ----------------------------------------------------------------------------

void CSysApAppUi::ShowUiNoteL( const TSysApNoteIds aNote ) const
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::ShowUiNoteL aNote: %d"), aNote ) );
    
    TInt swState( StateOfProperty( KPSUidStartup, KPSGlobalSystemState ) );

    if( UiReady() || swState == ESwStateSecurityCheck)
      {
          switch ( aNote )
            {
            case EBatteryLowNote:
                {
                 _LIT(KPowerPressKey,"Battery low");
                 HBufC* aString = HBufC16::NewLC(100);
                 TPtrC aStringPointer = aString->Des();
                 aStringPointer.Set(KPowerPressKey);
                 TRACES( RDebug::Print( _L("CSysApWsClient::RunL(): Key EEventKeyUp 01") ) );   
                 ShowExampleUiNoteL( aStringPointer );
                 CleanupStack::PopAndDestroy(); // aString
                }
                break;
            case EBatteryFullNote:
                {
                _LIT(KPowerPressKey,"Battery full");
                HBufC* aString = HBufC16::NewLC(100);
                TPtrC aStringPointer = aString->Des();
                aStringPointer.Set(KPowerPressKey);
                TRACES( RDebug::Print( _L("CSysApWsClient::RunL(): Key EEventKeyUp 01") ) );   
                ShowExampleUiNoteL( aStringPointer );
                CleanupStack::PopAndDestroy(); // aString
                }
                break;
            case ERechargeBatteryNote:
                {
                iSysApLightsController->BatteryEmptyL( ETrue );
                _LIT(KPowerPressKey,"Battery empty. Recharge");
                HBufC* aString = HBufC16::NewLC(100);
                TPtrC aStringPointer = aString->Des();
                aStringPointer.Set(KPowerPressKey);
                TRACES( RDebug::Print( _L("CSysApWsClient::RunL(): Key EEventKeyUp 01") ) );   
                ShowExampleUiNoteL( aStringPointer );
                CleanupStack::PopAndDestroy(); // aString
                }
                break;
            case ENotChargingNote:
                {
                _LIT(KPowerPressKey,"Not charging");
                HBufC* aString = HBufC16::NewLC(100);
                TPtrC aStringPointer = aString->Des();
                aStringPointer.Set(KPowerPressKey);
                TRACES( RDebug::Print( _L("CSysApWsClient::RunL(): Key EEventKeyUp 01") ) );   
                ShowExampleUiNoteL( aStringPointer );
                CleanupStack::PopAndDestroy(); // aString
                }
                break;
            case EBatteryFullUnplugChargerNote:
                {
                TRACES( RDebug::Print( _L("CSysApWsClient::RunL(): Key EEventKeyUp 01") ) ); 
                iSysApLightsController->BatteryEmptyL( ETrue );
                 _LIT(KunplugCharger,"txt_power_dpopinfo_unplug_charger_to_save_energy");                 
                 HBufC* unplugCharger = HbTextResolverSymbian::LoadL(KunplugCharger);
                 _LIT(KbatteryFull,"txt_power_management_dpophead_100_full");
                 HBufC* batteryFull = HbTextResolverSymbian::LoadL(KbatteryFull);
                 CHbDeviceNotificationDialogSymbian::NotificationL(KbatteryFullIcon,*unplugCharger,*batteryFull);       
                 }
                break;
            case EUnplugChargerNote:
                {
                _LIT(KPowerPressKey,"Unplug charger from power supply to save energy");
                HBufC* aString = HBufC16::NewLC(250);
                TPtrC aStringPointer = aString->Des();
                aStringPointer.Set(KPowerPressKey);
                TRACES( RDebug::Print( _L("CSysApWsClient::RunL(): Key EEventKeyUp 01") ) );   
                ShowExampleUiNoteL( aStringPointer );
                CleanupStack::PopAndDestroy(); // aString
                }
                break;
            default:
                break;
            }
        }
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::BatteryEmptyL()
// ----------------------------------------------------------------------------

void CSysApAppUi::BatteryEmptyL()
    {
    iSysApLightsController->BatteryEmptyL( ETrue );
    // Do not reactivate keyguard in the next startup
    iSysApCenRepController->SetInt( KCRUidCoreApplicationUIsSysAp, KSysApKeyguardActive, 0 );
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::ShowQueryL()
// ----------------------------------------------------------------------------

void CSysApAppUi::ShowQueryL( const TSysApConfirmationQueryIds /* aQueryId */, const TDesC& /* aValue */)
    {
    // do nothing
    }



// ----------------------------------------------------------------------------
// CSysApAppUi::HandleChargingStatusL
// ----------------------------------------------------------------------------
//
void CSysApAppUi::HandleChargingStatusL( const TInt aValue )
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::HandleChargingStatusL aValue: %d"), aValue ) );
    
    TBool showNote( ETrue );
    
    UpdateBatteryBarsL( StateOfProperty( KPSUidHWRMPowerState, KHWRMBatteryLevel ) );

    if ( iSysApPsmController && UiReady() ) 
        {             
        if ( iCharging && !iSysApPsmController->ChargerConnected() ) // first time after charger connection
            {
            iSysApPsmController->ConnectCharger( ETrue );
            iSysApPsmController->DoEnableFullPsm(EFalse);             
            iVariantAccState = CHbSymbianVariant::NewL(&KCharging, CHbSymbianVariant::EDes);
               
            if (!iHbIndicatorSymbian->Activate(KPsmPlugin,iVariantAccState)) 
               {
               int error = iHbIndicatorSymbian->Error();
               //use the errorcode...
               }
              
                           
            }
        else if ( aValue == EChargingStatusNotConnected )
            {
            iSysApPsmController->ConnectCharger( EFalse );
            if (!iHbIndicatorSymbian->Deactivate(KPsmPlugin)) 
               {
               int error = iHbIndicatorSymbian->Error();
               //use the errorcode...
               }
            }            
        }
    if( showNote )
        {
        HandleChargerNotesL( aValue );    
        }
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::HandleChargerNotesL()
// ----------------------------------------------------------------------------
void CSysApAppUi::HandleChargerNotesL( const TInt aValue )
    {
    TBool showNote(StateOfProperty( KPSUidCtsyCallInformation, KCTsyCallState ) !=  EPSCTsyCallStateRinging);
    if ( aValue == EChargingStatusCharging )
        {
        iSysApLightsController->BatteryEmptyL( EFalse );
        TRACES( RDebug::Print( _L("SysAp: charger connected") ) );
        iSysApLightsController->ChargerConnectedL( ETrue );
        ShowChargingNoteL();
        }
    //Display Not Charging note
    else if ( aValue == EChargingStatusError )
        {
        if(showNote)
            {
            ShowUiNoteL( ENotChargingNote );
            }
        }
    else if ( aValue == EChargingStatusChargingComplete )
        {
        iSysApLightsController->ChargingCompleteL();
        
        TSysApNoteIds note( EBatteryFullNote );
        
        if ( !iSysApUsbChargerDetector.HostOnlyUsbChargingUsed() &&
             iSysApFeatureManager->Supported( KSysApFeatureIdChargerReminderNotes ) )
            {
            note = EBatteryFullUnplugChargerNote;
            }
        iSysApUsbChargerDetector.Reset();
        if(showNote)
            {
            ShowUiNoteL( note );
            }
        }
    else if ( aValue == EChargingStatusNotConnected )
        {
        TRACES( RDebug::Print( _L("SysAp: charger removed") ) );
        iSysApLightsController->ChargerConnectedL( EFalse );
        
        if ( !iSysApUsbChargerDetector.HostOnlyUsbChargingUsed() &&
             iSysApFeatureManager->Supported( KSysApFeatureIdChargerReminderNotes ) )
            {
        if(showNote)
                {
                ShowUiNoteL( EUnplugChargerNote );
                }
            }
        iSysApUsbChargerDetector.Reset();
        }
    else if ( aValue == EChargingStatusNotCharging )
        {
        TRACES( RDebug::Print( _L("SysAp: Not charging") ) );
        }
    else if ( aValue == EChargingStatusAlmostComplete )
        {
        TRACES( RDebug::Print( _L("SysAp: Charging almost complete") ) );
        }
    else if ( aValue == EChargingStatusChargingContinued )
        {
        //in this case we simply want to scroll the battery bars and not turn light on or show ui note
        // set lights controller iBatteryEmpty to EFalse
        iSysApLightsController->BatteryEmptyL( EFalse );
        TRACES( RDebug::Print( _L("SysAp: charging continues") ) );
        }
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::UpdateBatteryBarsL( const TInt aState )
// ----------------------------------------------------------------------------

void CSysApAppUi::UpdateBatteryBarsL( const TInt  /* aState */)
    {
    TInt state( StateOfProperty( KPSUidHWRMPowerState, KHWRMChargingStatus ) );
//    TRACES( RDebug::Print( _L("CSysApAppUi::UpdateBatteryBarsL aState: %d, chargerState:%d"), aState, state ) );
    HandleUsbCharger( state );
    if( state == EChargingStatusCharging ||
        state == EChargingStatusChargingContinued ||
        state == EChargingStatusAlmostComplete )
        {
        if ( !iCharging )
            {
            TRACES( RDebug::Print( _L("CSysApAppUi::UpdateBatteryBarsL: Starting Charging") ) );
            iCharging = ETrue;
            }
        }
    else
        {
        if ( iCharging )
            {
            TRACES( RDebug::Print( _L("CSysApAppUi::UpdateBatteryBarsL: Stopping Charging") ) );
            iCharging = EFalse;
            }
        }
    
    if ( !iSysApPsmController ) // created here if first state change has not occurred yet
        {
        iSysApPsmController = CSysApPsmController::NewL( *this );        
        }

    if ( iSysApPsmController )
        {
        if ( iSysApPsmController->FullPsmEnabled() )
            {
             // activate psm indicator 
            }
        else
            {
            // deactivate psm indicator
            }            
        }
        
       if ( iSysApBatteryInfoController )        
        {
         iSysApBatteryInfoController->BatteryLevelUpdatedL();
        }
        
     }



// ----------------------------------------------------------------------------
// CSysApAppUi::ShowChargingNoteL()
// ----------------------------------------------------------------------------

void CSysApAppUi::ShowChargingNoteL()
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::ShowChargingNoteL") ) );
    TBool showNote( ETrue );

    TInt swState( StateOfProperty( KPSUidStartup, KPSGlobalSystemState ) );
    TRACES( RDebug::Print( _L("CSysApAppUi::ShowChargingNoteL: swState: %d"), swState ) );

    if( UiReady() || swState == ESwStateSecurityCheck )
        {
        if ( StateOfProperty( KPSUidCtsyCallInformation, KCTsyCallState ) ==  EPSCTsyCallStateRinging )
            {
            showNote = EFalse;
            }
        TRACES( RDebug::Print( _L("CSysApAppUi::ShowChargingNoteL KCTsyCallState=%d"), StateOfProperty( KPSUidCtsyCallInformation, KCTsyCallState ) ) );
        if ( showNote ) // Power Mgmt UI spec defines that no Charging note is shown while the phone is ringing/alerting
            {
            TRACES( RDebug::Print( _L("CSysApWsClient::RunL(): Key EEventKeyUp 01") ) );   
            _LIT(KChargingNote,"txt_power_management_dblist_charging");
            HBufC* chargingNote = HbTextResolverSymbian::LoadL(KChargingNote);
            CHbDeviceNotificationDialogSymbian::NotificationL(KNullDesC,*chargingNote);     
                    
            }
        }
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::HandleUsbCharger
// ----------------------------------------------------------------------------
//
void CSysApAppUi::HandleUsbCharger( const TInt aValue )
    {        
    if ( aValue == EChargingStatusCharging ||
         aValue == EChargingStatusChargingContinued ||
         aValue == EChargingStatusAlmostComplete )
        {
        iSysApUsbChargerDetector.SetChargingUsed( ETrue );
        if ( !iSysApUsbIndicatorController &&
             iSysApFeatureManager->Supported( KSysApFeatureIdChargerReminderNotes ) &&
             iSysApFeatureManager->Supported( KSysApFeatureIdUsbChargingWithoutReminderNotes ) )
            {
            // Start observing USB state for the reminder note
            TRAPD ( usbErr, iSysApUsbIndicatorController = CreateSysApUsbIndicatorL( *this ) );
            if ( usbErr )
                {
                TRACES( RDebug::Print(
                    _L("CSysApAppUi::HandleUsbCharger: error in constructing USB ind. controller %d" ),
                    usbErr ) );
                }
            }
        }
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::HandleCurrentCallStateChangeL()
// ----------------------------------------------------------------------------

void CSysApAppUi::HandleCurrentCallStateChangeL( TInt aCurrentCallState )
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::HandleCurrentCallStateChangeL: KUidCurrentCall: %d" ), aCurrentCallState ) );

    switch ( aCurrentCallState )
        {
        case EPSCTsyCallStateRinging:
            {
            iSysApLightsController->CallComingInL( ETrue );
            // Disable keylock when a call is coming in
            if ( iKeyLockEnabled || iDeviceLockEnabled || iKeyLockOnBeforeCradle || iKeyLockOnBeforeAlarm )
                {
                TRACES( RDebug::Print( _L("CSysApAppUi::HandleCurrentCallStateChangeL: EPSCTsyCallStateRinging: disable keylock") ) );
                iKeyLockOnBeforeCall = ETrue;

                if ( iKeyLockEnabled || iDeviceLockEnabled )
                    {
                    if ( !iSysApFeatureManager->TouchUnlockStrokeSupported() )
                        {
                        iKeyguardController->DisableKeyguard(EFalse);
                        }
                    }
                }
            break;
            }
            
        case EPSCTsyCallStateDialling:
            {
            // Disable keypad lock during an emergency call
            // no need to disable the key lock when a call is made using the wireless car-kit
            // but if the call is an emergency one then we will disable the keypad lock
            if ( iKeyLockEnabled || iDeviceLockEnabled || iKeyLockOnBeforeCradle )
                {
                TRACES( RDebug::Print( _L("CSysApAppUi::HandleCurrentCallStateChangeL: EPSCTsyCallStateDialling: disable keylock") ) );
                iKeyLockOnBeforeCall = ETrue;

                if ( IsEmergencyCall() && (iKeyLockEnabled || iDeviceLockEnabled ))
                    {
                    iKeyguardController->DisableKeyguard(EFalse);
                    }
                }

            // Enable signal & network indicators when an emergency call is made in Offline Mode
            if( iSysApOfflineModeController->OfflineModeActive() )
                    {
                    // Signal indicators not updated with VoIP call
                    if ( StateOfProperty(KPSUidCtsyCallInformation, KCTsyCallType) != EPSCTsyCallTypeVoIP) 
                        {
                        iEmergencyCallActive = ETrue;
                        // not supporting this
                        // UpdateSignalBarsL();
                        // SetSignalIndicatorL();
                        }
                    }
            break;
            }
            
        case EPSCTsyCallStateConnected:
            {
            if (StateOfProperty(KPSUidCtsyCallInformation, KCTsyCallType) == EPSCTsyCallTypeCSVoice)
                {
                // Check if GPRS suspended note is required
                iCallActivated = ETrue;
                // not supporting this
                // HandleGprsNotesL();
                }
            if ( iDeviceLockEnabled )
                {
                // Enable keylock via autolock emulation if device lock enabled. 
                // Otherwise e.g. messages can be read using softkeys during ongoing call.
                iKeyguardController->EnableKeyguard(EFalse);
                }
            break;
            }

        case EPSCTsyCallStateNone:
            {
            // Reset timers in ScreenSaver and Autolock
            User::ResetInactivityTime();

            if ( iEmergencyCallActive )
                {
                iEmergencyCallActive = EFalse;
                // not supporting this
                // UpdateSignalBarsL( 0 );
                // SetSignalIndicatorL();
                }
            if ( iKeyLockOnBeforeCall )
                {
                TRACES( RDebug::Print( _L("CSysApAppUi::HandleCurrentCallStateChangeL: EPSCTsyCallStateNone: enable keylock") ) );
                iKeyLockOnBeforeCall = EFalse;
                if ( !iDeviceLockEnabled )
                    {
                    if ( !iKeyLockOnBeforeCradle && !iKeyLockOnBeforeAlarm &&
                          !iSysApFeatureManager->TouchUnlockStrokeSupported() )
                        {
                        if ( iSysApCenRepController->GetInt( KCRUidCommonTelephonySettings, KSettingsSummaryAfterCall ) == 1 )
                            {
                            // Let's not hide "Summary After Call" dialog
                            iKeyguardController->EnableKeyguard(EFalse);
                            }
                        else
                            {
                            iKeyguardController->EnableKeyguard(ETrue);
                            }
                        }
                    }
                else
                    {
                    iKeyguardController->EnableKeyguard(EFalse);
                    }
                }
            if ( iShowkeypadActivatedNoteAfterSoftReject )
                {
                ShowUiNoteL( EKeypadActiveNote );
                iShowkeypadActivatedNoteAfterSoftReject = EFalse;
                }
            iCallActivated = EFalse;
            break;
            }
            
        default:
            break;
        }

    if ( aCurrentCallState != EPSCTsyCallStateRinging )
        {
        iSysApLightsController->CallComingInL( EFalse );
        }

    SetIhfIndicatorL();
    SetHacIndicatorL();
    }



/**
 * To check the for an emergency call. 
 * 
 * @return ETrue if there is an emergency call active otherwise, EFalse.
 */
TBool CSysApAppUi::IsEmergencyCall()
        {
        TBool retVal( EFalse );
        TInt err( KErrNone );
        TInt state( 0 );
     
        err = RProperty::Get(KPSUidCtsyEmergencyCallInfo, KCTSYEmergencyCallInfo, state );
        if ( err == KErrNone && state )
            {
            retVal = ETrue;            
            }
        return retVal;
        }



// ----------------------------------------------------------------------------
// CSysApAppUi::IsStateNormal()
//
// ----------------------------------------------------------------------------
TBool CSysApAppUi::IsStateNormal() const
    {
    TInt state;
    TInt errorCode =
        RProperty::Get( KPSUidStartup, KPSGlobalSystemState, state );
    if ( errorCode == KErrNone )
        {
        return state == ESwStateNormalRfOn ||
               state == ESwStateNormalRfOff ||
               state == ESwStateNormalBTSap;
        }
    else
        {
        TRACES( RDebug::Print( _L( "CSysApAppUi::IsStateNormal: Failed to read global system state, error code %d." ), errorCode ) );
        return EFalse;
        }
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::HandleAccessoryProfileInStartupL()
// ----------------------------------------------------------------------------

void CSysApAppUi::HandleAccessoryProfileInStartupL()
    {
    TRACES( RDebug::Print( _L( "CSysApAppUi::HandleAccessoryProfileInStartupL" ) ) );

    if ( !iSysApOfflineModeController->OfflineModeActive() )
        {
        iIgnoreAccessorySpecificProfileChanges = EFalse;
        TBool accessoryConnectedInShutdown( EFalse );
        TInt accessoryTemp( iSysApCenRepController->GetInt( KCRUidCoreApplicationUIsSysAp, KSysApAccessoryConnected ) );
        if ( accessoryTemp == 1 )
            {
            accessoryConnectedInShutdown = ETrue;
            }

        TBool accessoryConnectedNow ( EFalse );

        TAccMode accessoryState(EAccModeHandPortable);
        TInt physicalConnectionType = 0;
        if ( iSysApAccessoryObserver )
            {
            accessoryState = iSysApAccessoryObserver->GetAccessoryMode();
            physicalConnectionType = iSysApAccessoryObserver->GetAccessoryConnectionType();
            }

        if ( accessoryState != EAccModeHandPortable )
            {
            accessoryConnectedNow = ETrue;
            }
        TRACES( RDebug::Print( _L( "CSysApAppUi::HandleAccessoryProfileInStartupL: accessoryConnectedInShutdown: %d, accessoryConnectedNow: %d" ),
                              accessoryConnectedInShutdown, accessoryConnectedNow ) );

        if ( accessoryConnectedInShutdown && !accessoryConnectedNow )
            {
            HandleAccessoryDisconnectedL();
            }
        else if ( !accessoryConnectedInShutdown && accessoryConnectedNow )
            {
            HandleAccessoryConnectedL( accessoryState, physicalConnectionType );
            }
        else if ( !accessoryConnectedNow )
            {
            // not supporting this 
            // TInt activeProfile ( ActiveProfileId() );
            // SysApCenRepController->SetInt( KCRUidCoreApplicationUIsSysAp, KSysApActiveProfileBeforeAccessoryConnected, activeProfile );
            }
        }
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::CenRepController()
// ----------------------------------------------------------------------------

CSysApCenRepController& CSysApAppUi::CenRepController()
    {
    __ASSERT_ALWAYS( iSysApCenRepController, User::Panic( _L("CSysApAppUi::CenRepController"), KErrBadHandle ) );

    return *iSysApCenRepController;
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::LogsObserverL
// ----------------------------------------------------------------------------
//
CSysApCenRepLogsObserver& CSysApAppUi::LogsObserverL()
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::LogsObserverL()") ) );
    
    // Create Logs observer when it is needed for the first time
    if ( !iSysApCenRepLogsObserver )
        {
        iSysApCenRepLogsObserver = CSysApCenRepLogsObserver::NewL( *this );
        }
    
    return *iSysApCenRepLogsObserver;
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::HandleWlanIndicator()
// ----------------------------------------------------------------------------
void CSysApAppUi::HandleWlanIndicatorL( TInt aValue )
    {
    if ( aValue == EPSWlanIndicatorAvailable )
        {
        // do nothing
        }
    else if ( aValue == EPSWlanIndicatorActive )
        {
        // do nothing
        }
    else if ( aValue == EPSWlanIndicatorActiveSecure )
        {
        // do nothing
        }
    else // No indication required
        {
        // do nothing
        }
    }

void CSysApAppUi::ShowNotificationDialog(const TDesC& noteText)const
{
    //Todo:4.Notification--Just Popup message
    //Todo: #include <HbDeviceNotificationDialogSymbian.h>
    CHbDeviceNotificationDialogSymbian *notificationDialog =  CHbDeviceNotificationDialogSymbian::NewL();
    CleanupStack::PushL(notificationDialog);
    notificationDialog->SetTextL(noteText);//noteText with Qm file input
    notificationDialog->SetTimeout(100);
    notificationDialog->ShowL();
    CleanupStack::PopAndDestroy(notificationDialog);
}

void CSysApAppUi::SetKeyLockEnabledL()
    {
    iKeyLockEnabled = ETrue;
    iSysApCenRepController->SetInt( KCRUidCoreApplicationUIsSysAp, KSysApKeyguardActive, 1 );
    // not supporting indicator 
//    SetIndicatorStateL( EAknIndicatorKeyguard, EAknIndicatorStateOn );
    iSysApLightsController->KeylockStateChangedL( ETrue );
    }

void CSysApAppUi::SetKeyLockDisabledL()
    {
    iKeyLockEnabled = EFalse;
    iSysApCenRepController->SetInt( KCRUidCoreApplicationUIsSysAp, KSysApKeyguardActive, 0 );
    // SetIndicatorStateL( EAknIndicatorKeyguard, EAknIndicatorStateOff );
    if (! iDeviceLockEnabled )
        {
        iSysApLightsController->KeylockStateChangedL( EFalse );
        if ( iSysApFeatureManager->MmcHotSwapSupported() )
            {
            if ( StateOfProperty( KPSUidCtsyCallInformation, KCTsyCallState ) !=  EPSCTsyCallStateRinging && StateOfProperty( KPSUidCtsyCallInformation, KCTsyCallState ) !=  EPSCTsyCallStateAlerting )
                {
                 RunUnlockNotifierL();
                // need to handle MMC unlock query in next sub
                }
            }
        }
    }

void CSysApAppUi::SetLightsOnUnlockNoteL()
    {
    iSysApLightsController->SetLightsOnUnlockNoteL();
    }

void CSysApAppUi::SetLightsOnEcsQueryL()
    {
    iSysApLightsController->SetLightsOnEcsQueryL();
    }

void CSysApAppUi::SetLightsOnSecurityQueryL()
    {
    iSysApLightsController->SetLightsOnSecurityQueryL();
    }

TBool CSysApAppUi::CheckLongPowerKeyPressed()
    {
    return iCheckLongPowerKeyEvent;
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::HandleKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType )
// ----------------------------------------------------------------------------

TKeyResponse CSysApAppUi::HandleKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    if ( ! iShutdownStarted )
        {
        TRACES( RDebug::Print( _L("CSysApAppUi::HandleKeyEventL: aKeyEvent.iCode:%d, aKeyEvent.iScanCode:%d, aType:%d, iIgnoreNextPowerKeyUpEvent:%d, iPowerKeyPopupMenuActive:%d, iLastPowerKeyWasShort:%d, iPowerKeyPopupMenuDismissed:%d"),
                               aKeyEvent.iCode, aKeyEvent.iScanCode, aType, iIgnoreNextPowerKeyUpEvent, iPowerKeyPopupMenuActive, iLastPowerKeyWasShort, iPowerKeyPopupMenuDismissed ) );

#ifdef _DEBUG
        // camery latency measurement environment instrumentation, don't remove
        if ( aType == EEventKey && aKeyEvent.iCode == EKeyCamera )
            {
            TRACES( RDebug::Print( _L("e_KEY_EVENT_SENDING 0") ) );
            }
#endif // _DEBUG

        TKeyResponse response(EKeyWasNotConsumed);
        if (response)
            {
            //Do nothing:: To supress warning
            }
        
        if( aType == EEventKey )
            {
            switch ( aKeyEvent.iCode )
                {
#ifdef _DEBUG
                case 'E': //For testing
                    Exit();
                    break;
#endif
                case EKeyPowerOff:
                    //Short power key press
                    iKeyBoardRepeatCount++;
                    if( aKeyEvent.iRepeats == 0 )
                        {
                        TRACES( RDebug::Print(_L("CSysApAppUi::HandleKeyEventL, Short powerkey") ) );
                        iLastPowerKeyWasShort = ETrue;
                        TRACES( RDebug::Print( _L("CSysApAppUi::Key was consumed by pressing short power") ) );          
                        //Powermenu
                        if (iPowerMenuDialog!=NULL)
                            {
                            //PowerMenu already exist
                            delete iPowerMenuDialog;
                            iPowerMenuDialog = NULL;
                            }                                               
                        iPowerMenuDialog = CHbDevicePowerMenuSymbian::NewL(*this);  
                        iPowerMenuDialog->ShowL();           
                                         
                        iIgnoreNextPowerKeyRepeats = EFalse;
                        }
                    //Long power key press
                    else if( aKeyEvent.iRepeats > 0 && iKeyBoardRepeatCount >= iSysApCenRepController->GetInt( KCRUidCoreApplicationUIsSysAp, KSysApPowerkeyRepeatcount ))
                        {
                        iKeyBoardRepeatCount = -1;
                        TRACES( RDebug::Print(_L("CSysApAppUi::HandleKeyEventL, Long powerkey") ) );
                        iLastPowerKeyWasShort = EFalse;
                        HandleLongPowerKeyPressedL();
                        }
                    break;

                default:
                    {
                    TKeyResponse response = EKeyWasNotConsumed;
                    if (iSysApKeyManagement)
                        {
                        response = iSysApKeyManagement->HandleKeyEventL(aKeyEvent, aType );
                        }

                    if ( response == EKeyWasNotConsumed ) // none of the plugins consumed the key, offer it to default handling
                        {
                        iSysApDefaultKeyHandler->HandleKeyEventL( aKeyEvent, aType );
                        }
                    }
                    break;
                }
            }
        else if( aType == EEventKeyUp )
            {
            if( aKeyEvent.iScanCode == EStdKeyDevice2 )
                {
                if ( iIgnoreNextPowerKeyUpEvent )
                    {
                    if ( !iPowerKeyPopupMenuDismissed ) // If the popup menu has been dismissed, do nothing
                        {
                        iPowerKeyPopupMenuActive = ETrue;
                        iIgnoreNextPowerKeyUpEvent = EFalse;
                        }
                    }
                else if( iLastPowerKeyWasShort )
                    {
                    if ( iPowerKeyPopupMenuActive )
                        {
                        }
                    }

                }
            }

        TRACES( RDebug::Print( _L("CSysApAppUi::HandleKeyEventL:ended, aKeyEvent.iCode:%d, aKeyEvent.iScanCode:%d, aType:%d, iIgnoreNextPowerKeyUpEvent:%d, iPowerKeyPopupMenuActive:%d, iLastPowerKeyWasShort:%d, iPowerKeyPopupMenuDismissed:%d"),
                               aKeyEvent.iCode, aKeyEvent.iScanCode, aType, iIgnoreNextPowerKeyUpEvent, iPowerKeyPopupMenuActive, iLastPowerKeyWasShort, iPowerKeyPopupMenuDismissed ) );
        }
    else // shutdown started
        {
        TRACES( RDebug::Print( _L("CSysApAppUi::HandleKeyEventL: aKeyEvent.iCode:%d, aKeyEvent.iScanCode:%d, aType:%d"),
                               aKeyEvent.iCode, aKeyEvent.iScanCode, aType ) );

        // When shutdown has been started, only device mode key events are passed to plug-in framework
        // This for enabling plug-in activities during shutdown animation
        if ( IsDeviceModeKey( aKeyEvent ) ) // Other key events are not passed, because they must cancel the shutdown animation as stated in UI spefication
            {
            if ( aType == EEventKey && iSysApKeyManagement )
                {
                iSysApKeyManagement->HandleKeyEventL( aKeyEvent, aType );
                }
            }
        TRACES( RDebug::Print( _L("CSysApAppUi::HandleKeyEventL: ended") ) );
        }
    return EKeyWasConsumed;
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::HandleCommandL( TInt aCommand)
// ----------------------------------------------------------------------------

void CSysApAppUi::HandleCommandL( TInt aCommand )
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::HandleCommandL aCommend: %d"), aCommand ) );
    switch ( aCommand )
        {
        case EEikCmdExit:   // EAknCmdExit
            Exit();
            break;
        default:
            break;
        }
    }

TBool CSysApAppUi::ReleasePowerMenuCustomDialogMemory()
    {
    if (iPowerMenuDialog!=NULL)
        {
        //PowerMenu already exist
        delete iPowerMenuDialog;
        iPowerMenuDialog = NULL;
        TRACES( RDebug::Print(_L("CSysApAppUi::ReleasePowerMenuCustomDialogMemory True") ) );                            
        return ETrue;
        }
    TRACES( RDebug::Print(_L("CSysApAppUi::ReleasePowerMenuCustomDialogMemory false") ) );                            
    return EFalse;
    }

TBool CSysApAppUi::NotifiedDialogIfRequiredAndReleaseMemory()
    {
    TRACES( RDebug::Print(_L("CSysApAppUi::NotifiedDialogIfRequiredAndReleaseMemory(): Begin") ) );
    TRACES( RDebug::Print(_L("CSysApAppUi::NotifiedDialogIfRequiredAndReleaseMemory(): Enter for popping another dialog") ) );
    TInt popUpError = iSysApDriveUnlockHandler->CheckMemoryDialogIfNeeded();
    
    //Deside if dialog required again !!    
    if(popUpError)
        {
        iSysApDriveUnlockHandler->ReleaseMemoryForInputCardDialog();  // check memory has released.
        iSysApDriveUnlockHandler->UnlockComplete(KErrNone);
        }
    else
        {
        iSysApDriveUnlockHandler->StartUnlock();// pop up the dialog again !!        
        }
    
    TRACES( RDebug::Print(_L("CSysApAppUi::NotifiedDialogIfRequiredAndReleaseMemory(): End")) );    
    return popUpError;
    }

void CSysApAppUi::ReleaseMemoryForMemoryCardDialog()
    {
    TRACES( RDebug::Print(_L("CSysApAppUi::CSysApAppUi::ReleaseMemoryForMemoryCardDialog()")) );                                    
    iSysApDriveUnlockHandler->ReleaseMemoryForInputCardDialog();
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::ShowAnimationL()
// ----------------------------------------------------------------------------

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
void
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
TBool
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
CSysApAppUi::ShowAnimationL()
    {
    TRACES( RDebug::Print(_L("CSysApAppUi::ShowAnimationL(): START" ) ) );

    TRACES( RDebug::Print( _L("CSysApAppUi::ShowAnimationL: Initialise shutdown animation") ) );

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION

    PrepareForShutdownAnimation();

    iSysApShutdownAnimation = CSysApShutdownAnimation::NewL( *iSysApShutdownImage );
    iSysApShutdownAnimation->Play( TCallBack( DoStopAnimTiming, this ) );

    TRACES( RDebug::Print(_L("CSysApAppUi::ShowAnimationL(): End" ) ) );

#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
    iSysApShutdownAnimation = CSysApShutdownAnimation::NewL( this );
    AddToStackL( iSysApShutdownAnimation );
    iAnimationShowingTime = iSysApShutdownAnimation->ShowingTime();
    TRACES( RDebug::Print( _L("CSysApAppUi::ShowAnimationL: Shutdown animation initialised. Animation time = %d") ,iAnimationShowingTime) );

    TBool ret_val( EFalse );

    if ( iAnimationShowingTime )
        {
        if ( iSysApFeatureManager->CoverDisplaySupported() )
            {
            // Construct mediator observer
            iSysApMediatorObserver = CSysApMediatorObserver::NewL( this );

            // Sync animation
            TInt err = iSysApMediatorObserver->SyncShutdownAnimation();

            if ( err != KErrNone )
                {
                // Pretend coverUI synced instantly if error in issuing command.
                ShutdownAnimationSyncOK();
                }
            }
        else
            {
            // Pretend coverUI synced instantly when it is not supported.
            ShutdownAnimationSyncOK();
            }

        ret_val = ETrue;
        }

    TRACES( RDebug::Print(_L("CSysApAppUi::ShowAnimationL(): returns: %d" ),ret_val ) );
    return ret_val;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
    }



// ----------------------------------------------------------------------------
// CSysApAppUi::PrepareForShutdownAnimation()
// ----------------------------------------------------------------------------
void CSysApAppUi::PrepareForShutdownAnimation()
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::PrepareForShutdownAnimation() begin") ) );

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
    TRACES( RDebug::Print( _L("CSysApAppUi::PrepareForShutdownAnimation() showtime = %d"), iAnimationShowingTime ) );
    if ( iAnimationShowingTime )
        {
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
        
        if (iPowerMenuDialog!=NULL)
            {
            //PowerMenu already exist
            delete iPowerMenuDialog;
            iPowerMenuDialog = NULL;
            } 

        RWindowGroup groupWin = iCoeEnv->RootWin();
        iCapturedAppskey = groupWin.CaptureKey( EKeyApplication, KModifierMask, KModifierMask );
        iCapturedAppskeyUpAndDowns = groupWin.CaptureKeyUpAndDowns( EStdKeyApplication0, KModifierMask, KModifierMask );
        iEikonEnv->RootWin().SetOrdinalPosition(0, ECoeWinPriorityAlwaysAtFront );

        TRACES( RDebug::Print( _L("CSysApAppUi::PrepareForShutdownAnimation() Draw background image" ) ) );

        ShowShutdownImage( KBackgroundImageID );

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        TRACES( RDebug::Print( _L("CSysApAppUi::PrepareForShutdownAnimation() Show Shutdown animation" ) ) );
        TInt err(0);
        TRAP( err, iSysApShutdownAnimation->StartL( iLastPowerKeyWasShort ) );
        if ( err )
            {
            TRACES( RDebug::Print( _L("SysAp: Shutdown animation fails. Error code: %d" ), err ) );
            // Start animation timing immediatily if animation starting fails.
            // Otherwise animation will call StartAnimTiming when it is ready.
            StartAnimTiming();
            }
        }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    TRACES( RDebug::Print( _L("CSysApAppUi::PrepareForShutdownAnimation() end") ) );
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::DoStopAnimTiming( TAny* aObject )
// ----------------------------------------------------------------------------

TInt CSysApAppUi::DoStopAnimTiming( TAny* aObject )
    {
    TInt err(KErrNone);
    CSysApAppUi* appUi = STATIC_CAST( CSysApAppUi*, aObject );

    // This method could theoretically be called by two timers (iAnimTimer and one in CSysApShutdownAnimation),
    // so a check is needed to prevent multiple executions.
    if ( !(appUi->iShutdownContinued) )
        {
        appUi->iShutdownContinued = ETrue;

        TRACES( RDebug::Print( _L("CSysApAppUi::DoStopAnimTiming() Animation timer completed or animation skipped" ) ) );

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        if ( appUi->iAnimTimer )
            {
            appUi->iAnimTimer->Cancel();
            }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

        TRACES( RDebug::Print( _L("CSysApAppUi::DoStopAnimTiming() Call ContinueShutdown(...)" ) ) );
        appUi->ContinueShutdown();
        TRACES( RDebug::Print( _L("CSysApAppUi::DoStopAnimTiming() end") ) );
        }

    return err;
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::ContinueShutdown()
// ----------------------------------------------------------------------------

void CSysApAppUi::ContinueShutdown()
    {
    TRACES( RDebug::Print(_L("CSysApAppUi::ContinueShutdown() started" ) ) );
    CompleteShutdown();
    TRACES( RDebug::Print(_L("CSysApAppUi::ContinueShutdown() completed" ) ) );
    }

CEikStatusPane* CSysApAppUi::StatusPane()
{
return iEikonEnv->AppUiFactory()->StatusPane();
}
 

#ifndef RD_MULTIPLE_DRIVE

// ----------------------------------------------------------------------------
// CSysApAppUi::EjectUsed
// ----------------------------------------------------------------------------

void CSysApAppUi::EjectUsed( TInt /*aDrive*/ )
    {
    }

#else // RD_MULTIPLE_DRIVE

// ----------------------------------------------------------------------------
// CSysApAppUi::ShowEjectWaitNoteL
// ----------------------------------------------------------------------------

void CSysApAppUi::ShowEjectWaitNoteL( TInt /* aDriveToEject */ )
    {
 /*   if ( iSysApWaitNote )
        {
        return;
        }
    HBufC* text = iSysApDriveList->GetFormattedDriveNameLC(
        aDriveToEject,
        0, // Not used
        R_QTN_EJECTING_MEMORY_NAME_WAIT );
    iSysApWaitNote = CSysApWaitNote::NewL(
        iSysApFeatureManager->CoverDisplaySupported() );
    iSysApWaitNote->ShowNoteL( EClosingApplicationsNote, text );
    CleanupStack::PopAndDestroy( text );
 */   }

// ----------------------------------------------------------------------------
// CSysApAppUi::IsEjectQueryVisible
// ----------------------------------------------------------------------------

TBool CSysApAppUi::IsEjectQueryVisible()
    {
//    if ( !iSysApConfirmationQuery )
        {
        return EFalse;
        }
//    TInt queryId( iSysApConfirmationQuery->CurrentQuery() );
//    return ( queryId == ESysApEjectMmcQuery || queryId == ESysApRemoveMmcNote );
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::UpdateInsertedMemoryCardsL
// ----------------------------------------------------------------------------

void CSysApAppUi::UpdateInsertedMemoryCardsL()
    {
    // Update inserted memory cards
    iSysApDriveList->GetMemoryCardsL(
        iInsertedMemoryCards, CSysApDriveList::EIncludeInserted );

    // Update memory card indicator status
//    SetMemoryCardIndicatorL();

    // Handle unlock
    RunUnlockNotifierL();
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::EjectUsed
// ----------------------------------------------------------------------------

void CSysApAppUi::EjectUsed( TInt aDrive )
    {
    // Check drive inserted before starting eject confirm query
    TInt insertedIndex( CSysApDriveList::Find( iInsertedMemoryCards, aDrive ) );

    TRACES( RDebug::Print(
        _L( "CSysApAppUi::EjectUsed: drive: %d, index: %d" ),
        aDrive, insertedIndex ) );

    if ( insertedIndex == KErrNotFound )
        {
        return;
        }

    iMMCEjectUsed = ETrue;
    iDriveToEject = aDrive;
    iSysApDriveList->ResetDrivesToEject();
    TRAPD( err, EjectMMCL() );
    if ( err != KErrNone )
        {
        TRACES( RDebug::Print(
            _L( "CSysApAppUi::EjectUsed: err: %d" ), err ) );
        iMMCEjectUsed = EFalse;
        }
    }
#endif // RD_MULTIPLE_DRIVE


// ----------------------------------------------------------------------------
// CSysApAppUi::EjectMMCCanceled
// ----------------------------------------------------------------------------

void CSysApAppUi::EjectMMCCanceled()
    {
    // Called from eject confirm query, reset eject status
#ifdef RD_MULTIPLE_DRIVE
    iMMCEjectUsed = EFalse;
#endif // RD_MULTIPLE_DRIVE
    }



#ifndef RD_MULTIPLE_DRIVE
// ----------------------------------------------------------------------------
// CSysApAppUi::MountMMC()
// ----------------------------------------------------------------------------

TInt CSysApAppUi::MountMMC()
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::MountMMC") ) );
    TInt err ( KErrNotSupported );
    if ( iSysApFeatureManager->MmcSupported() )
        {
#ifdef __WINS__ // Let's sleep a second in WINS
        User::After( 1000000 );
#endif
        err = iEikonEnv->FsSession().MountFileSystem( KFSName, KMMCDrive );
        TRACES( RDebug::Print( _L("CSysApAppUi::MountMMC: RFs::MountFileSystem() returned: %d"), err ) );
        if ( err == KErrInUse )
            {
            User::After( 1000000 );
            err = iEikonEnv->FsSession().MountFileSystem( KFSName, KMMCDrive );
            TRACES( RDebug::Print( _L("CSysApAppUi::MountMMC: RFs::MountFileSystem() returned: %d"), err ) );
            }
        }
    return err;
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::DismountMMC()
// ----------------------------------------------------------------------------

TInt CSysApAppUi::DismountMMC()
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::DismountMMC") ) );
#ifdef __WINS__ // Let's sleep a second in WINS
        User::After( 1000000 );
#endif

    TInt err ( KErrNotSupported );
    if ( iSysApFeatureManager->MmcSupported() )
        {
        err = iEikonEnv->FsSession().DismountFileSystem( KFSName, KMMCDrive );
        TRACES( RDebug::Print( _L("CSysApAppUi::DismountMMC: RFs::DismountFileSystem() returned: %d"), err ) );
        }
    return err;
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::EjectMMCL()
// ----------------------------------------------------------------------------

void CSysApAppUi::EjectMMCL()
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::EjectMMCL") ) );
    iMMCEjectUsed = ETrue;
    iMMCPowerMenuEjectUsed = ETrue;
    iTimeToKill = EFalse;
    iApplicationScanningRoundNumber = 0;
    CloseUIAppsInHotSwapL();
    }

#else // RD_MULTIPLE_DRIVE

// ----------------------------------------------------------------------------
// CSysApAppUi::EjectMMCL()
// ----------------------------------------------------------------------------

void CSysApAppUi::EjectMMCL()
    {
    // Called from eject confirm query, check drive inserted before start
    TInt insertedIndex( CSysApDriveList::Find(
        iInsertedMemoryCards, iDriveToEject ) );

    TRACES( RDebug::Print(
        _L( "CSysApAppUi::EjectMMCL: iMMCEjectUsed: %d, drive: %d, index: %d "),
        iMMCEjectUsed, iDriveToEject, insertedIndex ) );

    if ( insertedIndex != KErrNotFound )
        {
        iSysApDriveList->MarkDriveToEject(
            iDriveToEject, CSysApDriveList::EEjectFromMenu );
        iSysApDriveEjectHandler->StartEject();
        }
    iMMCEjectUsed = EFalse;
    }

#endif // RD_MULTIPLE_DRIVE

#ifndef RD_MULTIPLE_DRIVE
// ----------------------------------------------------------------------------
// CSysApAppUi::RunUnlockNotifierL()
// ----------------------------------------------------------------------------

void CSysApAppUi::RunUnlockNotifierL( TSysApMemoryCardStatus aMemoryCardStatus )
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::RunUnlockNotifierL: START") ) );
    if ( iSysApFeatureManager->MmcSupported() )
        {
        if ( UiReady() )
            {
            TSysApMemoryCardStatus memoryCardStatus;
            if ( aMemoryCardStatus == ESysApMemoryCardStatusNotKnown )
                {
                memoryCardStatus = iSysApMMCObserver->MemoryCardStatus();
                }
            else
                {
                memoryCardStatus = aMemoryCardStatus;
                }

            TRACES( RDebug::Print( _L("CSysApAppUi::RunUnlockNotifierL: memoryCardStatus=%d"), memoryCardStatus ) );

            switch ( memoryCardStatus )
                {
                case ESysApMemoryCardInserted:
                    TRACES( RDebug::Print( _L("CSysApAppUi::RunUnlockNotifierL MMC inserted") ) );
                    if ( aMemoryCardStatus == ESysApMemoryCardInserted )
                        {
                        RProperty::Set( KPSUidUikon, KUikMMCInserted, 1 );
                        }
                    break;
                case ESysApMemoryCardLocked:
                    {
                    TRACES( RDebug::Print( _L("CSysApAppUi::RunUnlockNotifierL MMC locked") ) );
                    RProperty::Set( KPSUidUikon, KUikMMCInserted, 0 );
                    TInt callState = StateOfProperty( KPSUidCtsyCallInformation, KCTsyCallState );

                    if (  (! ( iDeviceLockEnabled || iKeyLockEnabled ) ) &&
                          callState != EPSCTsyCallStateRinging && // We don't want to see the MMC passwd query
                          callState != EPSCTsyCallStateAlerting ) // when the user is e.g. making an emergency call
                        {
                        if ( iSysApFeatureManager->MemoryCardLockSupported() )
                            {
                            if ( ! iMemoryCardDialog )
                                {
                                TRACES( RDebug::Print( _L("CSysApAppUi::RunUnlockNotifierL: ACTIVATE MMC passwd query") ) );
                                CAknMemoryCardDialog* mmcDialog = CAknMemoryCardDialog::NewLC( this );
                                iMemoryCardDialog = mmcDialog; // temporary variable used for hiding codescanner error 
                                iMemoryCardDialog->SetSelfPointer( &iMemoryCardDialog );
                                iMemoryCardDialog->UnlockCardLD(); // when UnlockCardLD completes it calls UnlockComplete()
                                }
                            }
                        else
                            {
                            // Since locked cards are not supported, notify user that card is locked.
                            ShowQueryL( ESysApMemoryCardLockedNote );
                            }
                        }
                    }
                    break;

                case ESysApMemoryCardNotInserted:
                default:
                    TRACES( RDebug::Print( _L("CSysApAppUi::RunUnlockNotifierL MMC not inserted") ) );
                    if ( aMemoryCardStatus == ESysApMemoryCardNotInserted )
                        {
                        RProperty::Set( KPSUidUikon, KUikMMCInserted, 0 );
                        }
                }
            }
        }
    TRACES( RDebug::Print( _L("CSysApAppUi::RunUnlockNotifierL: END") ) );
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::UnlockComplete() from MAknMemoryCardDialogObserver
// ----------------------------------------------------------------------------

void CSysApAppUi::UnlockComplete( TInt aResult )
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::UnlockComplete result: %d"), aResult ) );
    if ( aResult == KErrNone )
        {
        RProperty::Set( KPSUidUikon, KUikMMCInserted, 1 );
        }
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::MMCStatusChangedL() from MSysApMemoryCardObserver
// ----------------------------------------------------------------------------

void CSysApAppUi::MMCStatusChangedL()
    {
    TRACES( RDebug::Print( _L( "CSysApAppUi::MMCStatusChangedL START: iMMCInserted: %d, iMMCEjectUsed: %d, iHideFirstBeep: %d, iHideNextBeep: %d" ), iMMCInserted, iMMCEjectUsed, iHideFirstBeep, iHideNextBeep ) );

    if ( iSysApFeatureManager->MmcSupported() && !iShutdownStarted )
        {
        TSysApMemoryCardStatus memoryCardStatus = iSysApMMCObserver->MemoryCardStatus();
        TRACES( RDebug::Print( _L( "CSysApAppUi::MMCStatusChangedL memoryCardStatus=%d" ), memoryCardStatus ) );

        switch ( memoryCardStatus )
            {
            case ESysApMemoryCardInserted:
            case ESysApMemoryCardLocked:
                TRACES( RDebug::Print( _L( "CSysApAppUi::MMCStatusChangedL: MMC drive mounted" ) ) );
                if ( ! iMMCInserted ) // MMC was not inserted before
                    {
                    TRACES( RDebug::Print( _L( "CSysApAppUi::MMCStatusChangedL: MMC newly inserted" ) ) );
                    CancelWaitNote(); // just to be sure, the user might keep on closing and opening the MMC latch
                    // An MMC has been newly inserted, so play a sound and check if its password protected

                    if ( UiReady() )
                        {
                        if ( !iHideFirstBeep && !iHideNextBeep ) // starting up
                            {
                            Beep();
                            // Switch lights on
                            iSysApLightsController->MemoryCardInsertedL();
                            }
                        iHideNextBeep = EFalse;
                        }

                    iMMCPowerMenuEjectUsed = EFalse;
                    iMMCEjectUsed = EFalse;
                    iMMCInserted = ETrue;

                    // No need to show ESysApRemoveMmcNote after MMC already mounted
                    if ( iSysApConfirmationQuery )
                        {
                        if ( iSysApConfirmationQuery->CurrentQuery() == ESysApRemoveMmcNote )
                            {
                            iSysApConfirmationQuery->Cancel();
                            }
                        }

                    // Check if the MMC is locked and unlock it if necessary
                    RunUnlockNotifierL( memoryCardStatus );
                    }
                break;
            default:
                TRACES( RDebug::Print( _L( "CSysApAppUi::MMCStatusChangedL: MMC drive not mounted" ) ) );
                delete iMemoryCardDialog; // sets itself to null
                RProperty::Set( KPSUidUikon, KUikMMCInserted, 0 );
                if ( iMMCInserted )
                    {
                    // No need to show ESysApEjectMmcQuery after MMC already removed
                    if ( iSysApConfirmationQuery )
                        {
                        if ( iSysApConfirmationQuery->CurrentQuery() == ESysApEjectMmcQuery )
                            {
                            iSysApConfirmationQuery->Cancel();
                            }
                        }

                    iMMCInserted = EFalse;

                    TInt propertyValue( StateOfProperty( KPSUidUsbWatcher, KUsbWatcherSelectedPersonality ) );
                    if ( !iMMCEjectUsed && propertyValue != KUsbPersonalityIdMS )
                        {
                        // if USB file transfer active, do not close applications
                        // if eject selected in MMC App, MMC App takes care of the following and
                        // if eject selected from powerkeymenu, applications have already been shutdown
                        iTimeToKill = EFalse;
                        iApplicationScanningRoundNumber = 0;
                        CloseUIAppsInHotSwapL();
                        }
                    }
                else
                    {
                    // If MMC was not previously inserted and eject was chosed from power key menu, attempt to remount.
                    if ( iMMCPowerMenuEjectUsed )
                        {
                        TRACES( RDebug::Print( _L( "CSysApAppUi::MMCStatusChangedL: Attempt to remount" ) ) );
                        MountMMC();
                        // If mount was successful, unnecessary note will be canceled in insert notification handling.
                        }
                    }
                break;
            }

        // Update memory card indicator status
        SetMemoryCardIndicatorL();
        }
    TRACES( RDebug::Print( _L( "CSysApAppUi::MMCStatusChangedL   END: iMMCInserted: %d, iMMCEjectUsed: %d, iHideFirstBeep: %d, iHideNextBeep: %d" ), iMMCInserted, iMMCEjectUsed, iHideFirstBeep, iHideNextBeep ) );
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::ShowMMCDismountedDialogL()
// ----------------------------------------------------------------------------

void CSysApAppUi::ShowMMCDismountedDialogL()
    {
    TInt propertyValue( StateOfProperty( KPSUidUsbWatcher, KUsbWatcherSelectedPersonality ) );

    // Do not show any note if USB file transfer is active.
    if ( propertyValue != KUsbPersonalityIdMS )
        {
        if ( iSysApFeatureManager->MemoryCardHatchSupported() )
            { // MMC hatch has been opened and MMC has been dismounted
            TRACES( RDebug::Print( _L( "CSysApAppUi::ShowMMCDismountedDialogL: Show note: Remove MMC and press OK." ) ) );
            ShowQueryL( ESysApRemoveMmcNote );
            }
        else
            {
            // MMC has been removed and dismounted
            TRACES( RDebug::Print( _L( "CSysApAppUi::ShowMMCDismountedDialogL: Show note: You might have lost some data." ) ) );
            // No confirmation but let's set this true to enable MMC passwd query
            ShowUiNoteL( EMemoryCardRemovedWithoutEjectNote );
            }
        }
    else
        {
        TRACES( RDebug::Print( _L( "CSysApAppUi::ShowMMCDismountedDialogL: No note shown, USB file transfer caused dismount." ) ) );
        }
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::MMCDismountedDialogConfirmed
// ----------------------------------------------------------------------------

void CSysApAppUi::MMCDismountedDialogConfirmed()
    {
    TRACES( RDebug::Print( _L( "CSysApAppUi::MMCDismountedDialogConfirmed") ) );

    // Try to remount just in case when eject was chosen from power key menu
    if ( iMMCPowerMenuEjectUsed )
        {
        iMMCPowerMenuEjectUsed = EFalse;

        if ( MountMMC() == KErrLocked ) // In case a locked card was not removed after all
            {
            TRAPD( err, MMCStatusChangedL() ); // This will update power menu and memory card icon. Also memory card password is requeried.
            if ( err != KErrNone )
                {
                TRACES( RDebug::Print( _L( "CSysApAppUi::MMCDismountedDialogConfirmed: MMCStatusChangedL failed, err=%d"), err ) );
                }
            }
        }

    }

#else // RD_MULTIPLE_DRIVE

// ----------------------------------------------------------------------------
// CSysApAppUi::MMCInsertedL
// ----------------------------------------------------------------------------

void CSysApAppUi::MMCInsertedL()
    {
    TRACES( RDebug::Print( _L( "CSysApAppUi::MMCInsertedL") ) );

    if ( iSysApFeatureManager->MmcSupported() && !iShutdownStarted )
        {
        TBool normalState( UiReady() );

        if ( normalState )
            {
            TInt defaultMemoryCard( iSysApDriveList->DefaultMemoryCard() );
            TSysApMemoryCardStatus memoryCardStatus( iSysApDriveList->MemoryCardStatus( defaultMemoryCard ) );

            TRACES( RDebug::Print(
                _L( "CSysApAppUi::MMCInsertedL: drive: %d, memoryCardStatus: %d" ), defaultMemoryCard, memoryCardStatus ) );

            switch ( memoryCardStatus )
                {
                case ESysApMemoryCardInserted: // Fall through
                case ESysApMemoryCardLocked:
                    {
                    // Reset eject and unlock of inserted memory card
                    iSysApDriveList->ResetDriveToEject( defaultMemoryCard );
                    iSysApDriveList->ResetDriveUnlockQueryShown( defaultMemoryCard );

//                    CancelWaitNote();

                    if ( memoryCardStatus == ESysApMemoryCardInserted )
                        {
                        TRACES( RDebug::Print(
                            _L( "CSysApAppUi::MMCInsertedL: memoryCardStatus ESysApMemoryCardInserted: %d" ), memoryCardStatus ) );
                        RProperty::Set( KPSUidUikon, KUikMMCInserted, 1 );
                        }
                    else
                        {
                        TRACES( RDebug::Print(
                            _L( "CSysApAppUi::MMCInsertedL: memoryCardStatus ESysApMemoryCardLocked: %d" ), memoryCardStatus ) );
                        RProperty::Set( KPSUidUikon, KUikMMCInserted, 0 );
                        }
                    break;
                    }
                case ESysApMemoryCardNotInserted:
                    {
                    // Always reset eject and unlock of removed memory card
                    TBool isEject( iSysApDriveList->IsDriveToEject( defaultMemoryCard ) );

                    TRACES( RDebug::Print(
                        _L( "CSysApAppUi::MMCInsertedL: isEject: %d, drive: %d" ),
                        isEject, defaultMemoryCard ) );

                    iSysApDriveList->ResetDriveToEject( defaultMemoryCard );
                    iSysApDriveList->ResetDriveUnlockQueryShown( defaultMemoryCard );

                    TRACES( RDebug::Print(
                        _L( "SysApAppUi::MMCInsertedL: memoryCardStatus ESysApMemoryCardNotInserted: %d" ), memoryCardStatus ) );

                    RProperty::Set( KPSUidUikon, KUikMMCInserted, 0 );
                    break;
                    }
                case ESysApMemoryCardStatusNotKnown: // Fall through
                default:
                    {
                    break;
                    }
                }
                RunUnlockNotifierL();
            }
        }
    }


// ----------------------------------------------------------------------------
// CSysApAppUi::RunUnlockNotifierL()
// ----------------------------------------------------------------------------

void CSysApAppUi::RunUnlockNotifierL()
    {
    TRACES( RDebug::Print(
        _L("CSysApAppUi::RunUnlockNotifierL: START iDeviceLockEnabled: %d, iKeyLockEnabled: %d" ),
         iDeviceLockEnabled, iKeyLockEnabled ) );

    if ( !iSysApFeatureManager->MmcSupported() ||
         iDeviceLockEnabled ||
         iKeyLockEnabled ||
         !UiReady() )
        {
        TRACES( RDebug::Print( _L("CSysApAppUi::RunUnlockNotifierL: END") ) );
        return;
        }

    // We don't want to see the MMC passwd query
    // when the user is e.g. making an emergency call
    TInt callState( StateOfProperty( KPSUidCtsyCallInformation, KCTsyCallState ) );
    if ( callState != EPSCTsyCallStateRinging &&
        callState != EPSCTsyCallStateAlerting )
        {
        TInt defaultMemoryCard( iSysApDriveList->DefaultMemoryCard() );
        TSysApMemoryCardStatus memoryCardStatus( iSysApDriveList->MemoryCardStatus( defaultMemoryCard ) );
        if ( memoryCardStatus == ESysApMemoryCardLocked)
            {
            iSysApDriveUnlockHandler->StartUnlock();
            }
        }

    TRACES( RDebug::Print(
        _L("CSysApAppUi::RunUnlockNotifierL: END callState: %d"), callState ) );
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::MMCStatusChangedL() from MSysApMemoryCardObserver
// ----------------------------------------------------------------------------

void CSysApAppUi::MMCStatusChangedL( TInt aDrive )
    {
    TRACES( RDebug::Print(
        _L( "CSysApAppUi::MMCStatusChangedL START: MMCCount: %d, iMMCEjectUsed: %d" ),
        iInsertedMemoryCards.Count(), iMMCEjectUsed ) );

    if ( iShutdownStarted || !iSysApFeatureManager->MmcSupported() )
        {
        TRACES( RDebug::Print(
            _L( "CSysApAppUi::MMCStatusChangedL iShutdownStarted: %d END" ),
            iShutdownStarted ) );
        return;
        }

    TBool normalState( UiReady() );

    TInt defaultMemoryCard( iSysApDriveList->DefaultMemoryCard() );
    TSysApMemoryCardStatus memoryCardStatus( iSysApDriveList->MemoryCardStatus( aDrive ) );
    TInt insertedIndex( CSysApDriveList::Find( iInsertedMemoryCards, aDrive ) );

    TRACES( RDebug::Print(
        _L( "CSysApAppUi::MMCStatusChangedL: normalState: %d, index: %d, drive: %d, memoryCardStatus: %d" ),
        normalState, insertedIndex, aDrive, memoryCardStatus ) );

    switch ( memoryCardStatus )
        {
        case ESysApMemoryCardInserted: // Fall through
        case ESysApMemoryCardLocked:
            {
            if ( insertedIndex == KErrNotFound ) // Not inserted before
                {
                // Reset eject and unlock of inserted memory card
                iSysApDriveList->ResetDriveToEject( aDrive );
                iSysApDriveList->ResetDriveUnlockQueryShown( aDrive );

//                CancelWaitNote();

                if ( aDrive == defaultMemoryCard )
                    {
                    if ( memoryCardStatus == ESysApMemoryCardInserted )
                        {
                         RProperty::Set( KPSUidUikon, KUikMMCInserted, 1 );
                        }
                    else
                        {
                        RProperty::Set( KPSUidUikon, KUikMMCInserted, 0 );
                        }
                    }

                if ( normalState )
                    {
                    // Ignore extra beep from USB file transfer
                    TBool ignoreBeep(
                        iSysApDriveList->IsDriveInsertBeepIgnored( aDrive ) );

                    TRACES( RDebug::Print(
                        _L( "CSysApAppUi::MMCStatusChangedL: ignoreBeep: %d, drive: %d" ),
                        ignoreBeep, aDrive ) );

                    if ( !ignoreBeep )
                        {
//                        Beep();
                        iSysApLightsController->MemoryCardInsertedL();
                        }

                    iSysApDriveList->ResetDriveInsertBeepIgnored( aDrive );

                    // Keep ignoring extra beep if USB file transfer is active
                    TInt propertyValue( StateOfProperty( KPSUidUsbWatcher, KUsbWatcherSelectedPersonality ) );

                    TRACES( RDebug::Print(
                        _L( "CSysApAppUi::MMCStatusChangedL: usbState: %d" ), propertyValue ) );

                    if ( propertyValue == KUsbPersonalityIdMS )
                        {
                        iSysApDriveList->MarkDriveInsertBeepIgnored( aDrive );
                        }
                    }

//                if ( iDriveToDismount == aDrive && iSysApConfirmationQuery )
                    {
//                    if ( iSysApConfirmationQuery->CurrentQuery() == ESysApRemoveMmcNote )
                        {
                        // User put back ejected memory card or did not remove it
//                        iSysApConfirmationQuery->Cancel();
                        }
                    }
                RunUnlockNotifierL();
                }
            break;
            }
        case ESysApMemoryCardNotInserted:
            {
            // Always reset eject and unlock of removed memory card
            TBool isEject( iSysApDriveList->IsDriveToEject( aDrive ) );

            TRACES( RDebug::Print(
                _L( "CSysApAppUi::MMCStatusChangedL: isEject: %d, drive: %d" ),
                isEject, aDrive ) );

            iSysApDriveList->ResetDriveToEject( aDrive );
            iSysApDriveList->ResetDriveUnlockQueryShown( aDrive );

            if ( insertedIndex != KErrNotFound ) // Inserted before
                {
                if ( memoryCardStatus == ESysApMemoryCardNotInserted )
                    {
                    // Reset extra beep ignore if memory card was removed without eject
                    iSysApDriveList->ResetDriveInsertBeepIgnored( aDrive );
                    }

//                CancelGlobalListQuery(); // Cancel power menu

                if ( aDrive == defaultMemoryCard )
                    {
                    RProperty::Set( KPSUidUikon, KUikMMCInserted, 0 );
                    }

//                if ( iSysApConfirmationQuery )
                    {
//                    if ( iSysApConfirmationQuery->CurrentQuery() == ESysApEjectMmcQuery )
                        {
                        // User removed memory card too early
//                        iSysApConfirmationQuery->Cancel();
                        // Allow application closing to execute.
                        // Also FileManager is closed in this case, regardless where the eject was initiated from.
                        iMMCEjectUsed = EFalse;
                        }
                    }

                // Stop unlock of removed memory card
                iSysApDriveUnlockHandler->StopUnlock( aDrive );

                if ( !iMMCEjectUsed && !isEject )
                    {
                    TInt propertyValue( StateOfProperty( KPSUidUsbWatcher, KUsbWatcherSelectedPersonality ) );

                    TRACES( RDebug::Print(
                        _L( "CSysApAppUi::MMCStatusChangedL: usbState: %d" ), propertyValue ) );

                    if ( propertyValue != KUsbPersonalityIdMS )
                        {
                        if ( iSysApFeatureManager->MemoryCardHatchSupported() )
                            {
                            // Store drive removed without eject and start eject handling
                            iSysApDriveList->MarkDriveToEject(
                                aDrive, CSysApDriveList::EEjectRemovedWithoutEject );
                            iSysApDriveEjectHandler->StartEject();
                            }
                        else
                            {
                            // Just show the note
                            ShowUiNoteL( EMemoryCardRemovedWithoutEjectNote );
                            }
                        }
                    }
                iMMCEjectUsed = EFalse;
                }
            break;
            }
        case ESysApMemoryCardStatusNotKnown: // Fall through
        default:
            {
            break;
            }
        }

    // Update inserted memory cards
    iSysApDriveList->GetMemoryCardsL(
        iInsertedMemoryCards, CSysApDriveList::EIncludeInserted );

    // Update memory card indicator status
//    SetMemoryCardIndicatorL();

    TRACES( RDebug::Print(
        _L( "CSysApAppUi::MMCStatusChangedL END: MMCCount: %d, iMMCEjectUsed: %d" ),
        iInsertedMemoryCards.Count(), iMMCEjectUsed ) );
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::ShowMMCDismountedDialogL()
// ----------------------------------------------------------------------------

TBool CSysApAppUi::ShowMMCDismountedDialogL(
        TInt aDrive, CSysApDriveList::TDriveEjectType aEjectType )
    {
    TRACES( RDebug::Print(
        _L( "CSysApAppUi::ShowMMCDismountedDialogL: aDrive: %d, aEjectType: %d" ),
        aDrive, aEjectType ) );

    TBool ret( EFalse );
    iDriveToDismount = aDrive;
    HBufC* driveName = iSysApDriveList->GetFormattedDriveNameLC(
        aDrive,
        R_QTN_EJECT_REMOVE_MEMORY_INFO );

    if ( aEjectType == CSysApDriveList::EEjectFromMenu )
        {
        TRACES( RDebug::Print(
            _L( "CSysApAppUi::ShowMMCDismountedDialogL: Show note: Remove MMC and press OK...." ) ) );
        ShowQueryL( ESysApRemoveMmcNote, *driveName );
        ret = ETrue;
        }
    else
        {
        // Memory card was removed without eject
        TInt propertyValue( StateOfProperty( KPSUidUsbWatcher, KUsbWatcherSelectedPersonality ) );

        // Do not show any note if USB file transfer is active.
        if ( propertyValue != KUsbPersonalityIdMS )
            {
            if ( iSysApFeatureManager->MemoryCardHatchSupported() )
                {
                // MMC hatch has been opened and MMC has been dismounted
                TRACES( RDebug::Print(
                    _L( "CSysApAppUi::ShowMMCDismountedDialogL: Show note: Remove MMC and press OK." ) ) );
                ShowQueryL( ESysApRemoveMmcNote, *driveName );
                ret = ETrue;
                }
            else
                {
                iSysApDriveList->ResetDrivesToEject();

                // MMC has been removed and dismounted
                TRACES( RDebug::Print( _L(
                    "CSysApAppUi::ShowMMCDismountedDialogL: Show note: You might have lost some data." ) ) );

                // No confirmation but let's set this true to enable MMC passwd query
                ShowUiNoteL( EMemoryCardRemovedWithoutEjectNote );
                }
            }
        else
            {
            iSysApDriveList->ResetDrivesToEject();
            TRACES( RDebug::Print(
                _L( "CSysApAppUi::ShowMMCDismountedDialogL: No note shown, USB file transfer caused dismount." ) ) );
            }
        }

    CleanupStack::PopAndDestroy( driveName );

    TRACES( RDebug::Print(
        _L( "CSysApAppUi::ShowMMCDismountedDialogL: ret: %d" ), ret ) );

    return ret;
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::MMCDismountedDialogConfirmed
// ----------------------------------------------------------------------------

void CSysApAppUi::MMCDismountedDialogConfirmed()
    {
    TRACES( RDebug::Print(
        _L( "CSysApAppUi::MMCDismountedDialogConfirmed: iDriveToDismount: %d" ),
        iDriveToDismount ) );

    // Try remount to check if drive was put back
    iSysApDriveList->MountDrive( iDriveToDismount );
    if ( !iSysApDriveEjectHandler->CompleteDismount( iDriveToDismount ) )
        {
        // When ready, check for locked memory cards and update indicators
 //       TRAP_IGNORE( SetMemoryCardIndicatorL() );
        TRAP_IGNORE( RunUnlockNotifierL() );
        }
    }

#endif // RD_MULTIPLE_DRIVE


// ----------------------------------------------------------------------------
// CSysApAppUi::HandleApplicationSpecificEventL(TInt aType,const TWsEvent& aEvent)
// ----------------------------------------------------------------------------

void CSysApAppUi::HandleApplicationSpecificEventL(TInt aType,const TWsEvent& aEvent)
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::HandleApplicationSpecificEventL: aType:%d"), aType ) );

    CEikAppUi::HandleApplicationSpecificEventL(aType, aEvent);
    
    if ( ResourcesFreed() )
        {
        TRACES( RDebug::Print( _L("CSysApAppUi::HandleApplicationSpecificEventL: discarded, shutting down") ) );
        return;
        }
    
    switch( aType )
        {
        case EEikKeyLockEnabled:
            iKeyLockEnabled = ETrue;
            iSysApCenRepController->SetInt( KCRUidCoreApplicationUIsSysAp, KSysApKeyguardActive, 1 );
//            SetIndicatorStateL( EAknIndicatorKeyguard, EAknIndicatorStateOn );
            iSysApLightsController->KeylockStateChangedL( ETrue );
            break;
        case EEikKeyLockDisabled:
            iKeyLockEnabled = EFalse;
            iSysApCenRepController->SetInt( KCRUidCoreApplicationUIsSysAp, KSysApKeyguardActive, 0 );
//            SetIndicatorStateL( EAknIndicatorKeyguard, EAknIndicatorStateOff );
            if (! iDeviceLockEnabled )
                {
                iSysApLightsController->KeylockStateChangedL( EFalse );
                if ( iSysApFeatureManager->MmcHotSwapSupported() )
                    {
                    if ( StateOfProperty( KPSUidCtsyCallInformation, KCTsyCallState ) !=  EPSCTsyCallStateRinging && StateOfProperty( KPSUidCtsyCallInformation, KCTsyCallState ) !=  EPSCTsyCallStateAlerting )
                        {
                        RunUnlockNotifierL();
                        }
                    }
                }
            break;
        case EEikKeyLockPowerKeyPressed: //sent when power key is captured by keylockserver
//            HandleShortPowerKeyPressedL();
            break;

        case EEikKeyLockLightsOnRequest:
            iSysApLightsController->SetLightsOnUnlockNoteL();
            break;

        case EEikEcsQueryLights: // emergency note is shown
            iSysApLightsController->SetLightsOnEcsQueryL();
            break;

        case EEikSecurityQueryLights: // for device lock security query
            iSysApLightsController->SetLightsOnSecurityQueryL();
            break;

        default:
            break;
            }
    }



// ----------------------------------------------------------------------------
// CSysApAppUi::HandleNspsRawKeyEventL()
// ----------------------------------------------------------------------------

void CSysApAppUi::HandleNspsRawKeyEventL()
    {
#ifdef __SYSAP_MODULE_TEST
    ModuleTestShowUiNoteL( _L("Network wakeup from NSPS") );
#endif

    if ( iSysApEtelConnector )
        {
        iSysApEtelConnector->CommandNetCsWakeupOnNsps();
        }
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::UpdateSignalBarsL()
// ----------------------------------------------------------------------------

void CSysApAppUi::UpdateSignalBarsL()
    {
//    UpdateSignalBarsL(iSysApEtelConnector->GetSignalBars());
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::HandleSmsStorageNotificationL( TBool aSimStoreFull )
// ----------------------------------------------------------------------------

void CSysApAppUi::HandleSmsStorageNotificationL( TBool aSimStoreFull )
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::HandleSmsStorageNotificationL: aSimStoreFull: %d "), aSimStoreFull ) );

    if ( aSimStoreFull )
        {
/*        HBufC* noteStringBuf;
        noteStringBuf = StringLoader::LoadLC( R_QTN_MEMLO_MEMORY_LOW_SIM_MES, iEikonEnv );
        TPtr textBuffer = noteStringBuf->Des();
        iSysApMsgSimMemLowQuery->StartL( textBuffer );
        CleanupStack::PopAndDestroy();
*/        }

//    SetEnvelopeIndicatorL();
    }

// ----------------------------------------------------------------------------
// CSysApAppUi::HandleNetworkNspsNotification( RMmCustomAPI::TNspsStatus aNspsStatus )
// ----------------------------------------------------------------------------

void CSysApAppUi::HandleNetworkNspsNotification( RMmCustomAPI::TNspsStatus aNspsStatus )
    {
    TRACES( RDebug::Print( _L("CSysApAppUi::HandleNetworkNspsNotification aNspsStatus:%d, iNsps:%d )" ), aNspsStatus, iNsps ) );
    if( iSysApNspsHandler )
        {
        if( aNspsStatus == RMmCustomAPI::ENspsOn )
            {
            if( !iNsps )
                {
#ifdef __SYSAP_MODULE_TEST
                TRAPD( err, ModuleTestShowUiNoteL( _L("Setting NSPS on") ) );
#endif
                iSysApNspsHandler->SetNspsOn();
                iNsps = ETrue;
                }
            }
        else if( aNspsStatus == RMmCustomAPI::ENspsOff )
            {
            if( iNsps )
                {
#ifdef __SYSAP_MODULE_TEST
                TRAPD( err, ModuleTestShowUiNoteL( _L("Setting NSPS off") ) );
#endif
                iSysApNspsHandler->SetNspsOff();
                iNsps = EFalse;
                }
            }
        }
    }



//end of file
