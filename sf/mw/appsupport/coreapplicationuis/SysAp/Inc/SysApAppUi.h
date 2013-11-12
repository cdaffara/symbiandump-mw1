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
 *
 */

#ifndef SYSAPAPPUI_H
#define SYSAPAPPUI_H

#include <aknappui.h>
#include "SysAp.hrh"
#include <f32file.h> 
#include <W32STD.H>
#include <coedef.h>
#include <e32property.h>
#include <sysapcallback.h>
#include <starterclient.h>
#include <rmmcustomapi.h>


#include "SysApTimer.h"
#include "SysApLightsController.h"
#include "coreapplicationuisprivatecrkeys.h"
#include "sysapusbchargerdetector.h"
#include "SysApAccessoryObserver.h"
#include <hbsymbianvariant.h>
#include <hbindicatorsymbian.h>
//For Powermenu--CHbDevicePowerMenuSymbian
#include "hbdevicepowermenusymbian.h"

#include "sysappsmcontroller.h"
#include "SysApMMCObserver.h"

#ifdef RD_MULTIPLE_DRIVE
#include "sysapdrivelist.h"
#endif // RD_MULTIPLE_DRIVE

#include <eikappui.h>


class CSysApFeatureManager;
class CSysApNspsHandler;
class CSysApPubSubObserver;
class CSysApStartupController;
class CSysApDefaultKeyHandler;
class CSysApKeyManagement; 
class MSysApOfflineModeController;
class CHbDevicePowerMenuSymbian;
class CSysApShutdownImage;
class CSysApKeySndHandler;
class CSysApCenRepController;
class CSysApCenRepLightSettingsObserver;
class CSysApCenRepBtObserver;
class CSysApCenRepHacSettingObserver;
class CSysApAudioRoutingObserver;
class CSysApSsSettingsObserver;
class CSystemLock;
class CSysApCenRepCallForwardingObserver;
class CSysApBatteryInfoController;
class MSysApBtSapController;
class MSysApBtController;
class CSysApCenRepLogsObserver;
class MSysApUsbIndicator;
class CKeyguardAccessApi;
class CHbIndicatorSymbian;	
class CSysApShutdownAnimation;

class CSysApDefaultKeyHandler;
class CSysApDriveUnlockHandler;
class CSysApDriveEjectHandler;
class CHbDeviceInputDialogSymbian;
class CSysApEtelConnector;

const TInt KBluetoothModulePowerModeOn ( 1 );
const TInt KDummyReason( -1 );


//CLASS DECLARATION
/**
*  CSysApAppUi class. This class serves as a link between the other
*  SysAp classes and performs the most UI specific operations on SysAp's
*  responsibility.
*
*  @lib   sysap
*  @since 1.0
*/

class CSysApAppUi : public CAknAppUi,
                    public MSysApMemoryCardObserver,
                    public MSysApTimerResponder,
                    public MSysapCallback,
                    public MSysApPsmControllerNotifyCallback
	{
		public:
        /**
        * C++ default constructor.
        * @param None
        * @return void
        */
        CSysApAppUi();

        /**
        * Destructor
        * @param None
        * @return void
        */
        virtual ~CSysApAppUi();
        
        // from CCoeAppUiBase
        void PrepareToExit();
        
    protected: // From MSysApTimerResponder

        /**
        * Handle events from CSysApTimer
        * @param None
        * @return void
        */
        void TimerExpiredL();
        
    private: // from MSysapCallback
        /**
        * Executes a command in SysAp.
        *
        * @since S60 3.1
        * @param[in] aCommand   command identifier
        * @return None
        * @leave an error code, system wide/command specific
        *
        * @see MSysapCallback::TSysapCommand
        */
        void ExecCommandL( TSysapCommand aCommand );

        /**
        * Executes a command in SysAp.
        *
        * @since S60 3.1
        * @param aCommand   command identifier
        * @param aParam     command specific parameter data
        * @return None
        * @leave an error code, system wide/command specific
        *
        * @see MSysapCallback::TSysapCommand
        */
        void ExecCommandL( TSysapCommand aCommand, const TDesC8& aParam );

        /**
        * Queries information from SysAp.
        *
        * @since S60 3.1
        * @param[in]  aQuery    query identifier
        * @param[out] aReturn   query specific return data
        * @return None
        * @leave an error code, system wide/command specific
        *
        * @see MSysapCallback::TSysapQuery
        */
        void ExecQueryL( TSysapQuery aQuery, TDes8& aReturn );

        /**
        * Queries information from SysAp.
        *
        * @since S60 3.1
        * @param[in]  aQuery    query identifier
        * @param[out] aReturn   query specific return data
        * @param[in]  aParam    query specific parameter data
        * @return None
        * @leave an error code, system wide/command specific
        *
        * @see MSysapCallback::TSysapQuery
        */
        void ExecQueryL( TSysapQuery aQuery, TDes8& aReturn, const TDesC8& aParam );
        
//    private:
        /**
        * EPOC default constructor.
        * @param None
        * @return void
        */
        
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
    void
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
    TBool
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
    ShowAnimationL();
    
    void PrepareForShutdownAnimation();
        
    public: 
        void ConstructL();
        
    private: // from CEikAppUi
        /**
        * Handles a change to the application's resources which are shared across the environment.
        *
        * @since S60 3.1
        * @param aType The type of resources that have changed.
        * @return None.
        */
//        void HandleResourceChangeL(TInt aType);
        
    private:
        /**
        * Takes care of command handling.
        * @param aCommand command to be handled
        * @return void
        */
        void HandleCommandL( TInt aCommand);

        /**
        * handles key events
        * @param TKeyEvent& aKeyEvent
        * @param TEventCode aType
        * @return TKeyResponse
        */
        TKeyResponse HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );
        void HandleApplicationSpecificEventL(TInt aType,const TWsEvent& aEvent);
      
     public:
     	//	void CallFromMain();
     		
     		 /**
        * Returns the state of keylock
        * @param none
        * @return TBool the state of keylock
        */
        TBool KeyLockState() const;

        /**
        * Returns the state of devicelock
        * @param none
        * @return TBool the state of devicelock
        */
        TBool DeviceLockState() const;
        
        /**
        * Returns reference to SysAp's internal feature manager.
        *
        * @since S60 3.1
        * @return CSysApFeatureManager&
        */
        CSysApFeatureManager& SysApFeatureManager();
        
                /*
        * Called by SysApLightsController when the lights are switced on/off
        * @param aActivate If ETrue forwarding is activated, otherwise deactivated
        * @return void
        */
        void ActivateKeyeventForwardingForLights(TBool aActivate);
        
        TBool IsEncryptionOperationOngoingL() const;
//        void ConnectToFileServerL();
//        void ConnectToWindowServerL();
        void ShowExampleUiNoteL ( const TDesC& noteText )const;
        void ShowNotificationDialog(const TDesC& noteText)const;
        void PopupNote();
//        void CreateWindowGroup();
        TBool ResourcesFreed() const;
        void ShowUiNoteL( const TSysApNoteIds aNote ) const;        
        TInt StateOfProperty( const TUid& aCategory, const TUint aKey ) const;
        TBool OfflineModeActive();
        void GoOnlineL( TBool aDoProfileChange = ETrue );
        void GoOfflineL();
        void SwitchFromOnlineToOfflineModeL();
        
        void DoStateChangedL( const RStarterSession::TGlobalState aSwState  );
        TInt SetStarterState( const RStarterSession::TGlobalState aState );
        TInt BluetoothPowerMode() const;
        void SetNetworkConnectionAllowed( TCoreAppUIsNetworkConnectionAllowed aNetworkConnectionAllowed );
        RStarterSession& StarterSession();
        TInt SetBtPowerState( TBool aBtState );
        TBool SimSupported();
        TBool BtSapEnabled();
        void OfflineModeChangedL();
        TBool UiReady() const;
        void DoShutdownL( const TBool aReset, const TInt aResetReason );
        TBool IsDeviceModeKey( const TKeyEvent& aKeyEvent ) const;
        void SetUsbAttachStatus( const TBool aUsbAttached );
        TSysApUsbChargerDetector& UsbChargerDetector();
        
        void HandleAccessoryConnectedL( TAccMode aAccessoryState, TInt aPhysicalConnectionType );

        void HandleAccessoryDisconnectedL();
        void DoLightsTimeoutChangedL( const TInt aTimeout );
        void SetHacIndicatorL();
        void DoNotActivateBt();
        void SetIhfIndicatorL();
		void HandleLongPowerKeyPressedL();
        void IgnoreZeroNetworkBarNotifications(TBool aIgnore);
        void InitCloseSimApplicationsL();
        void CloseSimApplicationsL();
        void DoSimChangedFromPreviousBootL();
        void ShowAlarmIndicatorL( TBool aShowIndicator );
        void AlertUiAlertingL( const TBool aAlerting );
        void HandleForcedLightsVTRequireL( const TInt aLightsParameter ) const;
        void HandleForcedLightsSSRequireL( const TInt aLightsParameter ) const;
        void HandleForcedLightsATCRequireL( const TInt aLightsParameter ) const;
        void HandleLightsRequireL() const;
        void AlertUiKeyLockOff( const TBool aKeyLockOff );
        void DoNotEnableKeylock();
        void SwitchLightsOnSoftRejectL();
        void SetDeviceLockEnabledL( const TBool aLockEnabled );
        void HandleBatteryStatusL( const TInt aValue );
        void BatteryEmptyL();
        void ShowQueryL( const TSysApConfirmationQueryIds aQueryId,
            const TDesC& aValue = KNullDesC );
        void HandleChargingStatusL( const TInt aValue );
        void HandleChargerNotesL( const TInt aValue );
        void UpdateBatteryBarsL( const TInt aState );
        void ShowChargingNoteL();
        void HandleCurrentCallStateChangeL( TInt aCurrentCallState );
        TBool IsEmergencyCall();
        CSysApCenRepController& CenRepController();
        void HandleWlanIndicatorL( TInt aValue );

        void SetKeyLockEnabledL();
        void SetKeyLockDisabledL();
        void SetLightsOnUnlockNoteL();
        void SetLightsOnEcsQueryL();
        void SetLightsOnSecurityQueryL();  
        TBool CheckLongPowerKeyPressed();
        TBool ReleasePowerMenuCustomDialogMemory();
        TBool NotifiedDialogIfRequiredAndReleaseMemory();
        static TInt DoStopAnimTiming( TAny* aObject );
        CEikStatusPane* StatusPane();
        
        void EjectUsed( TInt aDrive );
        
        void EjectMMCL();
        
#ifndef RD_MULTIPLE_DRIVE
        /**
        * Mounts MMC drive
        * @param None
        * @return TInt : error value from RFs::MountFileSystem()
        */
        TInt MountMMC();

        /**
        * Dismounts MMC drive
        * @param None
        * @return TInt : error value from RFs::MountFileSystem()
        */
        TInt DismountMMC();
#endif // RD_MULTIPLE_DRIVE
        
#ifndef RD_MULTIPLE_DRIVE
        /**
        * Unlocks MMC
        * @param None
        * @return void
        */
        void RunUnlockNotifierL( TSysApMemoryCardStatus aMemoryCardStatus = ESysApMemoryCardStatusNotKnown );

        /**
        * From MSysApMemoryCardObserver.
        * Gets called by CSysApMMCObserver when an MMC is inserted/removed
        * @return void
        */
        void MMCStatusChangedL();

        /**
        * Shows "MMC removed..." dialogs
        * @param None
        * @return void
        */
        void ShowMMCDismountedDialogL();
#else // RD_MULTIPLE_DRIVE

        /**
        * Updates PS key during boot
        * @return void
        */
        void MMCInsertedL();

        /**
        * Unlocks MMC
        * @param None
        * @return void
        */
        void RunUnlockNotifierL();

        /**
        * From MSysApMemoryCardObserver.
        * Gets called by CSysApMMCObserver when an MMC is inserted/removed
        * @param aDrive Changed drive
        * @return void
        */
        void MMCStatusChangedL( TInt aDrive );

        /**
        * Shows "MMC removed..." dialogs
        * @param aDrive Drive to dismount
        * @param aEjectType Eject type used for the drive
        * @return void
        */
        TBool ShowMMCDismountedDialogL(
            TInt aDrive, CSysApDriveList::TDriveEjectType aEjectType );
#endif // RD_MULTIPLE_DRIVE
        
#ifdef RD_MULTIPLE_DRIVE
        /*
        * Starts wait note for eject
        */
        void ShowEjectWaitNoteL(  TInt aDriveToEject );

        /*
        * Checks is eject query visible
        */
        TBool IsEjectQueryVisible();
#endif // RD_MULTIPLE_DRIVE
        
        
#ifdef SYSAP_USE_STARTUP_UI_PHASE        
        /**
        * Called when UI is ready after boot.
        */
        void HandleUiReadyAfterBootL();

#endif // SYSAP_USE_STARTUP_UI_PHASE

       
        void MMCDismountedDialogConfirmed();
        
        void EjectStarted( TBool ejectStarted );
        
        void EjectMMCCanceled();
        
        void HandleNspsRawKeyEventL();

        void HandleNetworkNspsNotification( RMmCustomAPI::TNspsStatus aNspsStatus );
        
        void HandleRawKeyEventLightsRequireL() const;
        
        void HandleSmsStorageNotificationL( TBool aSimStoreFull );
        
        void UpdateSignalBarsL();
        
        void ReleaseMemoryForMemoryCardDialog();
        
     private:
         /**
         * Frees SysAp's reserved memory, closes connections to servers etc.
         * @param None
         * @return void
         */
         void FreeResources();
         TBool OkToInitiateShutdown();


         void CompleteShutdown( const TBool aReset = EFalse, const TInt aResetReason = KDummyReason );
         void PrepareForShutdownImage();
         TInt ShowShutdownImage( TInt aBitmapId );
         void ShowShutdownImageL( TInt aBitmapId );
         TBool IhfEnabledL();
         void DoSwStateNormalConstructionL();
         static TInt DoCloseSimApplicationsAfterCallBack( TAny* aObject );
         
         // from MSysApPsmControllerNotifyCallback
         void NotifyPowerSaveModeL( TSysApPsmStatus aStatus );
         
         void HandleUsbCharger( const TInt aValue );
         TBool IsStateNormal() const;
         void HandleAccessoryProfileInStartupL( );
         
         CSysApCenRepLogsObserver& CSysApAppUi::LogsObserverL();
         void ContinueShutdown();
         
#ifdef RD_MULTIPLE_DRIVE
        void UpdateInsertedMemoryCardsL();
#endif // RD_MULTIPLE_DRIVE
        
     private:
//          CSysApWsClient*                 iSysApWsClient;
          CSysApLightsController*         iSysApLightsController;
          CSysApFeatureManager*           iSysApFeatureManager;
          CSysApNspsHandler*              iSysApNspsHandler;
          CSysApPubSubObserver*           iSysApPubSubObserver;
    
          CSysApDefaultKeyHandler*          iSysApDefaultKeyHandler;
//          RWindowGroup                      groupWin;
          CSysApStartupController*        iSysApStartupController;
          MSysApOfflineModeController*    iSysApOfflineModeController;
          RStarterSession                 iStarterSession;
          TSysApUsbChargerDetector        iSysApUsbChargerDetector;
          CSysApAccessoryObserver*        iSysApAccessoryObserver;
          CSysApShutdownImage*            iSysApShutdownImage;
          CSysApShutdownAnimation*        iSysApShutdownAnimation;
          CSysApKeySndHandler*            iSysApKeySndHandler;
          CSysApCenRepController*                  iSysApCenRepController;
          CSysApCenRepLightSettingsObserver*  iSysApCenRepLightSettingsObserver;
          CSysApCenRepBtObserver*                  iSysApCenRepBtObserver;
          CSysApCenRepHacSettingObserver* iSysApCenRepHacSettingObserver;
          
          CSysApAudioRoutingObserver* iSysApAudioRoutingObserver;
          CSysApPsmController* iSysApPsmController;
		  CHbSymbianVariant* iVariantAccState ;
          CSysApSsSettingsObserver*       iSysApSsSettingsObserver;
          CSystemLock*                    iSysApSystemLock;
          CSysApCenRepCallForwardingObserver* iSysApCenRepCallForwardingObserver;
          CSysApBatteryInfoController* iSysApBatteryInfoController;
          MSysApBtSapController*          iSysApBtSapController;
          CPeriodic*                      iSapTimer;
          MSysApBtController*             iSysApBtController;
          CSysApCenRepLogsObserver*                iSysApCenRepLogsObserver;
		  MSysApUsbIndicator*             iSysApUsbIndicatorController;
		  CKeyguardAccessApi*             iKeyguardController;
		  CHbDevicePowerMenuSymbian*            iPowerMenuDialog;
	      CSysApKeyManagement*            iSysApKeyManagement;
	      CSysApMMCObserver*              iSysApMMCObserver;
	      CSysApEtelConnector*            iSysApEtelConnector;
		  
	public:		  
		  CHbIndicatorSymbian* 			  iHbIndicatorSymbian;
		  

     private:         
        TBool                           iPowerKeyPopupMenuActive;   
        TBool                           iDeviceLockEnabled;
        TBool                           iKeyLockEnabled;
        TBool                           iKeyLockOnBeforeCall;
        TBool                           iKeyLockOnBeforeAlarm;
        TBool                           iKeyLockOnBeforeCradle;
        TBool                           iResourcesFreed;        
        TInt                            iCapturedEKeyPowerOff;
        TInt                            iCapturedEKeyPowerOffUpAndDowns;
        TBool                           iOfflineModeActive;
        TBool                           iShutdownStarted;
        
        TBool                           iIgnoreNextPowerKeyUpEvent;  // need to be added this flag in maliks client side apis.
        TBool                           iLastPowerKeyWasShort;
        TBool                           iPowerKeyPopupMenuDismissed; // need to be added this flag in malik implementation.
        TInt                            iKeyBoardRepeatCount;
        TBool                           iIgnoreNextPowerKeyRepeats;
        TInt                            iActiveProfileBeforeOfflineMode;
        
        TBool                           iSimChangedDone;
        TBool                           iIgnoreZeroNetworkBarNotifications;
        TInt                            iApplicationScanningRoundNumberInBtSap;
        TBool                           iTimeToKillInBtSap;
        TBool                           iShowkeypadActivatedNoteAfterSoftReject;
        TBool                           iCharging;  // Indicates if charging is currently on
        TBool                           iEmergencyCallActive;
        TBool                           iCallActivated; // used    in defining whether to show GPRS suspended note

        TBool                           iActivateBt;
        TBool                           iDeactivateBt;
        
        TBool                           iIgnoreAccessorySpecificProfileChanges;
        TBool                           iCheckLongPowerKeyEvent;
        TInt                            iCapturedAppskey;
        TInt                            iCapturedAppskeyUpAndDowns;
        TBool                           iShutdownContinued;
        TBool                           iNsps;
        
        
        //CPeriodic*                      iTimer;
#ifndef RD_MULTIPLE_DRIVE
        TInt                            iPowerkeyMenuEjectSelection;
#endif // RD_MULTIPLE_DRIVE
        
#ifndef RD_MULTIPLE_DRIVE
        TBool                           iHideFirstBeep;
#endif // RD_MULTIPLE_DRIVE
        
#ifndef RD_MULTIPLE_DRIVE
        TBool                           iTimeToKill;                     // MMC Hot Swap
        TInt                            iApplicationScanningRoundNumber; // MMC Hot Swap
#endif // RD_MULTIPLE_DRIVE
        
#ifndef RD_MULTIPLE_DRIVE
        TBool                           iMMCPowerMenuEjectUsed;
        TBool                           iMMCInserted;
#endif // RD_MULTIPLE_DRIVE
        
#ifndef RD_MULTIPLE_DRIVE
        TBool iHideNextBeep; // Prevents double beep when USB file transfer gets activated
        TBool iFileManagerCloseDisabled; // Prevents File Manager to close on hotswap
#endif // RD_MULTIPLE_DRIVE
        
#ifdef RD_MULTIPLE_DRIVE
        // First power menu index used for eject
        TInt iPowerkeyMenuEjectSelectionBase;

        /**
        * Drive list container
        * Own.
        */
        CSysApDriveList* iSysApDriveList;

        /**
        * Drive unlock handler.
        * Own.
        */
        CSysApDriveUnlockHandler* iSysApDriveUnlockHandler;

        /**
        * Drive eject handler.
        * Own.
        */
        CSysApDriveEjectHandler* iSysApDriveEjectHandler;

        /**
        * Array for storing inserted memory cards.
        * Own.
        */
        RArray< CSysApDriveList::TMemoryCardEntry > iInsertedMemoryCards;

        // Stores drive for dismount query
        TInt iDriveToDismount;

        // Stores drive for eject query
        TInt iDriveToEject;
#endif // RD_MULTIPLE_DRIVE
        
        TBool                           iMMCEjectUsed;
        CHbDeviceInputDialogSymbian* iMemCardPwdDialog;
        
	};
	
#endif
