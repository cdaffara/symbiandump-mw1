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
* Description:  CSysApLightsController class definition. This class handles
*                keypad and display background lights.
*
*/


#ifndef SYSAPLIGHTSCONTROLLER_H
#define SYSAPLIGHTSCONTROLLER_H

// INCLUDES
#include <e32base.h>
#include <hwrmlight.h>
#include "SysAp.hrh"
#include "SysApTimer.h"

// FORWARD DECLARATIONS
class CUserActivityManager;
class CSysApAppUi;
#ifdef RD_LIGHT_CONTROL_CHANGE
class CSysApLightPluginHandler;
#endif // RD_LIGHT_CONTROL_CHANGE

class CSysApFeatureManager;

// CLASS DECLARATION
/**
*  CSysApLightsController class
*
*  @lib None.
*  @since 1.0 
*/

class CSysApLightsController : public CBase, public MSysApTimerResponder 
    {
    public: // Constructors and destructor
        /**
        * Symbian default constructor.
        */
        static CSysApLightsController* NewL( CSysApAppUi& aSysApAppUi, const TInt aTimeout, const TBool aCoverDisplay );
    
        /**
        * Destructor.
        */
        virtual ~CSysApLightsController();
    
    public:
        /**
        * Alarm time was due and thus there is need to switch lights on
        * @param aAlarmActive if ETrue, alarm is active
        */
        void AlarmOccuredL( const TBool aAlarmActive );
    
        /**
        * Accessory was connected and thus there is need to switch lights on
        *
        * @param aConnected ETrue accessory connected
        *                   EFalse accessory removed
        */
        void AccessoryConnectedL( TBool aConnected = ETrue );
        
        /**
        * Switch lights when connected to charger.
        *
        * @param aConnected ETrue charger is connected
        *                   EFalse charger is removed
        *
        * @since S60 3.2
        */
        void ChargerConnectedL( TBool aConnected );
    
        /**
        * Switch lights on when charging has been completed.
        *
        * @since S60 3.2
        */
        void ChargingCompleteL();

        /**
        * Switch lights on when profile is changed.
        *
        * @since S60 3.2
        */
        void ProfileChangedL();
    
        /**
        * Switch lights on when a memory card has been inserted.
        *
        * @since S60 3.2
        */
        void MemoryCardInsertedL();
    
        /**
        * Handles call state related lights switching on. If the phone is in silent mode
        * and call is ringing, lights are set blinking. 
        *
        * @param aCallRinging   ETrue, if ringing, otherwise EFalse.
        */
        void CallComingInL( const TBool aCallRinging );
    
        /**
        * Power key was pressed short and thus there is need to switch lights on. In case
        * the device or key lock is enabled, the lights will be set off with a timer.
        */
        void PowerKeyPressedL();
    
        /**
        * Grip state was changed and thus there is need to change lights behaviour accordingly
        *
        * @param aOpen ETrue if grip is open, otherwise EFalse.
        */
        void SetGripOpenL( const TBool aOpen );
    
        /**
        * Flip state was changed and thus there is need to change lights behaviour accordingly
        *
        * @param aOpen ETrue if flip is open, otherwise EFalse.
        */
        void SetFlipOpenL( const TBool aOpen );

        /**
        * Silent mode status was changed 
        *
        * @param aSilent ETrue if silent mode is set on.
        */
        void SetSilentModeOn( const TBool aSilent );

        /**
        * Handles special lights requirements. Lights are set on without further checks.
        */
        void HandleLightsRequireL();

        /**
        * Lights are set on if device is not locked.
        * This method gets called when NsPsWsPlugin reports raw key events. 
        *
        * @since S60 3.2
        */
        void HandleRawKeyEventLightsRequireL();
        
        /**
        * Gets called when Video Telephony requests to force lights on/off.
        *
        * @param aLightsParameter EForceLightsOn - lights on
        *                         EForcedLighsOff - lights off  
        * @see TLighs
        */      
        void HandleForcedLightsVTRequireL( const TInt aLightsParameter );

        /**
        * Gets called when Screen Saver requests to force lights on/off
        *
        * @param aLightsParameter EForceLightsOn - lights on
        *                         EForcedLighsOff - lights off  
        * @see TLighs
        */      
        void HandleForcedLightsSSRequireL( const TInt aLightsParameter );
        
        /**
        * Gets called when AT command requests to force lights on/off
        *
        * @param aLightsParameter EForceLightsOn - lights on
        *                         EForcedLighsOff - lights off  
        * @see TLights
        */      
        void HandleForcedLightsATCRequireL( const TInt aLightsParameter );
public:
        /**
        * Handles lights when device lock state changes.
        *
        * @since S60 3.2
        * @param aEnabled if ETrue, device lock is ON.
        */
        void DeviceLockStateChangedL( TBool aEnabled );
    
        /**
        * Handles lights when keylock state changes.
        *
        * @since S60 3.2
        * @param aEnabled if ETrue, keylock is ON.
        */
        void KeylockStateChangedL( TBool aEnabled );
        
        /**
        * Light handling for soft reject case: when the phone is in silent mode and ringing,
        * light blinking is disabled for better usability.
        *
        * @since S60 3.1
        */      
        void SwitchLightsOnSoftRejectL();
        
        /**
        * Gets called when the value of lights timeout changes.
        *
        * @param aTimeout   lights timeout in seconds
        */      
        void DoLightsTimeoutChangedL( const TInt aTimeout );

private:
        /**
        * Switches lights off with delay
        */
        void SetLightsExplicitlyOffWithDelayL();

        /**
        * Cancels lights off with delay request
        */
        void CancelLightsExplicitlyOffWithDelayL();

public:
        /**
        * Sets lights on unlock note for a short period.
        *
        * @since S60 3.2
        */
        void SetLightsOnUnlockNoteL();
        
        /**
        * Sets lights on incoming request from UI FW due to
        * displaying of emergency call query.
        * This method is called when keypad/device is locked, when
        * lights cannot be switched on due to user activity.
        *
        * @since S60 3.2
        */
        void SetLightsOnEcsQueryL();
        
        /**
        * Sets lights on incoming request from UI FW due to
        * displaying of security code query.
        * This method is called when keypad/device is locked, when
        * lights cannot be switched on due to user activity.
        *
        * @since S60 3.2
        */
        void SetLightsOnSecurityQueryL();

        /**
        * Called when "battery empty" or "charger connected" notification has been received.
        */
        void BatteryEmptyL( TBool aEmpty );
        
        /**
        * Sets lights on allowed flag.
        * Used to block unnecessary multiple lights on commands at startup
        */
        void AllowLightsOn();

        /**
        * Returns pointer to HWRM server light session 
        *
        * @since S60 3.1 
        * @return CHWRMLight* pointer to HWRM server light session
        */
        CHWRMLight* GetHwrmLight();

#ifdef RD_LIGHT_CONTROL_CHANGE        
        /**
        * Inform lights controller that key event forwarding can be activated.
        *
        * @since S60 5.0
        */
        void KeyEventForwardingReady();
#endif // RD_LIGHT_CONTROL_CHANGE
        
    private:
        /**
        * Symbian 2nd-phase constructor.
        */
        void ConstructL();
    
        /**
        * Constructor
        * @param aSysApAppUi reference to application class
        * @param aTimeout lights timeout in seconds
        * @param aCoverDisplay If ETrue, device has a cover display.
        */        
        CSysApLightsController( CSysApAppUi& aSysApAppUi, const TInt aTimeout, const TBool aCoverDisplay );
    
        /**
        * C++ default constructor.
        */
        CSysApLightsController();

    private:
        // By default, prohibit copy constructor
        CSysApLightsController( const CSysApLightsController& );
    
        // Prohibit assigment operator
        CSysApLightsController& operator= ( const CSysApLightsController& );
    
    private:         
        /**
        * Gets called by User Activity Manager when user activity is detected after period of inactivity
        * @param aPtr pointer to CSysApLightsController object
        * @see CUserActivityManager
        */
        static TInt HandleActiveEventL( TAny* aPtr );

        /**
        * Gets called by User Activity Manager when user inactivity is detected after period of activity
        * @param aPtr pointer to CSysApLightsController object
        * @see CUserActivityManager
        */
        static TInt HandleInactiveEventL( TAny* aPtr );
        
        /**
        * Disables Activity Manager.
        */
        void DisableActivityManager();
        
        /**
        * Enables Activity Manager.
        */
        void EnableActivityManagerL();
        
        /**
        * Enables Activity Manager with timeout depending on device lock or keylock state. 
        * @param aIsLocked
        */
        void EnableActivityManagerL(TBool aIsLocked);

        /**
        * Reports user acticity to kernel's inactivity counters.
        */
        void ReportUserActivity() const;

        /**
        * This method puts the lights off.
        */
        void SetLightsOffL();

    private:

        /**
        * This method puts DISPLAY lights on UNTIL ETERNITY without resetting inactivity timers. 
        */
        void SetDisplayLightsOnWithoutResettingInactivityTimersL();

        /**
        * This is the normal way of putting the lights on
        * @param TBool aBlinking: the lights will be set blinking
        */                
        void SetLightsOnL( TBool aBlinking = EFalse );
        
        /**
        * Reserves light unless already reserved.
        */
        void ReserveLightL();
        
        /*
        * Releases light if reservation no longer needed
        */
        void ReleaseLight();
        
        /**
        * Sets flip open status but does not invoke any light control
        *
        * @since S60 3.1
        * @param aOpen              flip open status, ignored if aFetchFlipStatus is ETrue
        * @param aFetchFlipStatus   if ETrue, flip status is read from P&S key
        */
        void SetFlipOpenNoLights( const TBool aOpen, const TBool aFetchFlipStatus );
        
        /**
        * Turns off inactive light targets, e.g. primary display when flip is closed.
        *
        * @since S60 3.2
        */
        void TurnOffInactiveTargetsL();
        
        /*
        * Used for setting lights on primary targets with custom intensity when
        * device is in locked state i.e. either keylock or device lock is on.
        *
        * @since S60 3.1
        */
        void SetLockedStatePrimaryLightsL();
        
        /*
        * Used for resolving the target(s) that should be lit when the device is
        * locked state.
        *
        * @since S60 3.1
        * @return TInt target mask, bit values are defined in CHWRMLight
        * @see SetLockedStatePrimaryLightsL
        */
        TInt ResolveLockedStateLightTarget() const;
        
        /**
        * Returns whether there's a call ringing or an alarm alerting.
        *
        * @since S60 3.2
        * @return TBool ETrue if a call is ringing or an alarm is alerting.
        */ 
        TBool Alerting() const;

    protected: // From MSysApTimerResponder

        /**
        * Handle events from CSysApTimer
        */
        void TimerExpiredL();

    private:
        CUserActivityManager*        iActivityManager;

        /*****************************************************
         *      Series 60 Customer / DOS
         *      Series 60 Domestic OS API
         *****************************************************/

        CHWRMLight*                  iLight;    
        
        // Light targets depend on supported targets, coverUI feature and flip status.
        TBool                        iReserved;        
        TBool                        iAllowLightsOn;     // To optimize startup lights on
        TTime                        iLastLightsOnTime;  // Last time lights were turned on 
        TBool                        iLightsCurrentlyOn; // If last call was regular system lights on, this is ETrue
        TBool                        iGripOpen;
        TBool                        iFlipOpen;
        CSysApAppUi&                 iSysApAppUi;
        TInt                         iLightsTimeout;
        CSysApTimer*                 iSysApTimer;
        TBool                        iCoverDisplaySupported;
        TBool                        iSilent;
        TBool                        iCallRinging;
        TBool                        iAlarmAlerting;
        TBool                        iBatteryEmpty;
        TBool                        iForcedLightsOn;
        
        CSysApFeatureManager*           iSysApFeatureManager;
        
        /**
        * Indicates whether user inactity has been detected.
        */
        TBool iInactivityDetected;
        
#ifdef RD_LIGHT_CONTROL_CHANGE
        CSysApLightPluginHandler* iLightPluginHandler;
        TBool iKeyEventsObserved;
#endif // RD_LIGHT_CONTROL_CHANGE        
    };

#endif      // SYSAPSLIGHTSCONTROLLER_H
            
// End of File
