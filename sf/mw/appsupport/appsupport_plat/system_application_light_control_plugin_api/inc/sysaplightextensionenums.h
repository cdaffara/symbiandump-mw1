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
* Description:  Light control extension plugin enumerations
*
*/


#ifndef SYSAPLIGHTEXTENSIONENUMS_H
#define SYSAPLIGHTEXTENSIONENUMS_H

namespace SysApLightExtension
    {
    
    /**
    * Light event.
    *
    * Light event is a signal about a system state change
    * which is interesting in System Application's light control perspective.
    */
    enum TLightEvent
        {
        /** 
        * Reserved for internal purposes, never signaled to plug-in.
        */
        ELightEventNone = -1,
        
        /**
        * Lights are set on in device startup.
        *
        * When the device is started, lights are set on until normal operation
        * mode is reached (signaled with ELightsAllowed).
        * 
        * @see ELightsAllowed
        */
        EInitialLightsOn = 0,
                
        /**
        * An alarm has been activated.
        * 
        * @param TBool  ETrue alarm is ON
        *               EFalse alarm is OFF
        */
        EAlarmState,
        
        /**
        * An accessory device has been connected.
        *
        * @param TBool  ETrue accessory connected
        *               EFalse accessory disconnected
        */
        EAccessoryConnection,
        
        /**
        * Charger has been connected.
        *
        * @param TBool  ETrue charger connected
        *               EFalse charger disconnected
        */
        EChargerConnection,
        
        /**
        * Charging has been completed.
        */
        EChargingComplete,
        
        /**
        * Battery empty state has been notified.
        *
        * @param TBool  ETrue battery is empty
        *               EFalse battery not empty
        */
        EBatteryEmptyState,
        
        /**
        * Profile has been changed.
        */
        EProfileChanged,
        
        /**
        * Memory card has been inserted.
        */
        EMemoryCardInserted,
        
        /**
        * Call state changed to ringing.
        *
        * @param TBool  ETrue call state is ringing
        *               EFalse call state is not ringing
        */
        ECallStateRinging,
        
        /**
        * Power key has been pressed.
        */
        EPowerKeyPressed,
        
        /**
        * Grip has been opened.
        *
        * @param TBool  ETrue grip has been opened
        *               EFalse grip has been closed
        */ 
        EGripOpenState,
        
        /**
        * Flip has been opened.
        *
        * @param TBool  ETrue flip has been opened
        *               EFalse flip has been close
        */
        EFlipOpenState,
        
        /**
        * Silent mode has been set on.
        *
        * @param TBool  ETrue silent mode is ON.
        *               EFalse silent mode is OFF.
        */
        ESilentMode,
        
        /**
        * A generic lights on requirement.
        */
        ELightsRequire,
        
        /**
        * Key has been pressed.
        */
        ELightsRequireRawKeyEvent,
        
        /**
        * Device lock has been enabled.
        *
        * @param TBool  ETrue  device lock enabled
        *               EFalse device lock disabled
        */
        EDeviceLockState,
        
        /**
        * Keylock has been enabled.
        *
        * @param TBool  ETrue  keylock enabled
        *               EFalse keylock disabled
        */
        EKeyLockState,
        
        /**
        * Soft reject has been applied to a phone call.
        */
        ELightsOnSoftReject,
        
        /**
        * Lighs are switched on due to a security query, e.g. for opening the device lock.
        */
        ELightsOnSecQuery,
        
        /**
        * Lights are switched on for displaying emergency call query.
        */
        ELightsOnEcsQuery,
        
        /**
        * Lights are switched on for displaying note instruction user to unlock the keypad.
        */
        ELightsOnUnlockNote,
        
        /**
        * User inactivity timeout has been changed.
        *
        * @param TInt the new timeout value in milliseconds
        */
        EInactivityTimeoutChanged,
        
        /**
        * User activity has been detected.
        */
        EActivityDetected,
        
        /**
        * User inactivity has been detected.
        */
        EInactivityDetected,
        
        /**
        * Lights-off timer has been started.
        *
        * @param TInt timeout in milliseconds after
        *             which lights are set off.  
        */
        ELightsOffTimerStart,
        
        /**
        * Lights-off timeout has been expired.
        *
        * @param TBool  ETrue  timeout expired normally
        *               EFalse timer was canceled
        */
        ELightsOffTimerStop,
        
        /**
        * Lights on requirement from Video Telephony.
        *
        * @param TBool  ETrue lights ON
        *               EFalse lights OFF
        */
        ELightsForcedVideoTel,
        
        /**
        * Lights on requirement from Screen Saver.
        *
        * @param TBool  ETrue lights were set ON
        *               EFalse lights were set OFF  
        *
        */
        ELightsForcedScreenSaver
        };
   
    /**
    * Light commands.
    *
    * System Application notifies the plug-in about the forthcoming
    * light command with these enumerations.
    */
    enum TLightCommand
        {
        /** 
        * Reserved for internal purposes, never signaled to plug-in.
        */
        ELightCommandNone = -1,
        
        /**
        * No light commands will be taken in context with previous light event.
        */
        ELightCommandNoAction,
        
        /**
        * Lights off command.
        */
        ELightCommandOff,
        
        /**
        * Lights on command.
        */
        ELightCommandOn,
        
        /**
        * Lights blink command.
        */
        ELightCommandBlink,
        };
    
    /**
    * Light command response.
    *
    */    
    enum TLightCommandResponse
        {
        /**
        * Command was not handled by the plug-in.
        *
        * Plug-in must return this value whenever it receives a light command
        * and there is no need to alter System Application's default behavior.
        * Upon receiving this value, System Application will execute the light command
        * it notified the plug-in about.
        */
        ECommandNotHandled,
        
        /**
        * Command was handled by the plug-in.
        *
        * Plug-in must return this value whenever it receives a light command
        * and it wants to prevent System Application from executing the light command.
        */
        ECommandHandled
        };        
        
    }


#endif // SYSAPLIGHTEXTENSIONENUMS_H
