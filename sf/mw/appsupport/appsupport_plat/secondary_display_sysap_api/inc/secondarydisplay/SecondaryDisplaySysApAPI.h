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
* Description:  The set of messages provided to Secondary Display software by
*                SysAp application.
*
*/


#ifndef SECONDARY_DISPLAY_SYSAP_API_H
#define SECONDARY_DISPLAY_SYSAP_API_H

// INCLUDES
#include <e32base.h>

/*
* ==============================================================================
* This file contains the following sections:
*   - Dialog definitions
*   - Command definitions
*   - Event definitions
*   - Parameter definitions
* ==============================================================================
*/

namespace SecondaryDisplay
{
	
// The category UID for the messages in this header file.
//
const TUid KCatSysAp = {0x100058F3};

/*
* ==============================================================================
* Dialogs shown by SysAp subsystem. These messages are handled using the
* Secondary Display support in Avkon.
* ==============================================================================
*/
enum TSecondaryDisplaySysApDialogs
    {
    /**
    * No note. Error condition if this comes to CoverUI
    */
    ECmdNoNote = 0,
    
    /**
    * A command for showing the "Accessory not supported" note on secondary display.
    */
    ECmdShowAccessoryNotSupportedNote,
    
    /**
    * A command for showing the "Insert Sim" note on secondary display.
    */
    ECmdShowInsertSimNote,

    /**
    * A command for showing the "Memory card removed without eject" note on secondary display.
    */
    ECmdShowMemoryCardRemovedWithoutEjectNote,

    /**
    * A command for showing the "Gprs suspended" note on secondary display.
    */
    ECmdShowGprsSuspendedNote,

    /**
    * A command for showing the "Gprs resumed" note on secondary display.
    */
    ECmdShowGprsResumedNote,

    /**
    * A command for showing the shutdown note on secondary display.
    */
    ECmdShowShutdownNote,

    /**
    * A command for showing the "Cannot activate offline mode" note on secondary display.
    */
    ECmdShowCannotActivateOfflineModeNote,

    /**
    * A command for showing the "Cannot deactivate offline mode" note on secondary display.
    */
    ECmdShowCannotDeactivateOfflineModeNote,

    /**
    * A command for showing the "Keypad active" note on secondary display.
    */
    ECmdShowKeypadActiveNote,

    /**
    * A command for showing the "Closing applications" note on secondary display.
    */
    ECmdShowClosingApplicationsNote,

    /**
    * A command for showing the "Restart Phone" query on secondary display.
    */
    ECmdShowRestartPhoneQuery,

    /**
    * A command for showing the "Leave offline mode" query on secondary display.
    */
    ECmdShowLeaveOfflineModeQuery,

    /**
    * A command for showing the "Remove memory card" query on secondary display.
    */
    ECmdShowRemoveMmcQuery,

    /**
    * A command for showing the "Eject memory card" query on secondary display.
    */
    ECmdShowEjectMmcQuery,

    /**
    * A command for showing the "Locked memory cards not supported" query on secondary display.
    */
    ECmdShowMemoryCardLockedQuery,

    /**
    * A command for showing the VoIP "Do not show this emergency call warning in the future" 
    * query on secondary display. 
    * Only needed if __VOIP feature flag is defined.
    */
    ECmdShowDoNotShowVoipEmergencyCallWarningQuery,

    /**
    * A command for showing the query about VoIP call readiness being reached
    * first time on secondary display.
    * Only needed if __VOIP feature flag is defined.
    */
    ECmdShowVoipEmergencyCallReadinessQuery,

    /**
    * A command for showing the query about only having VoIP call capability
    * on secondary display.
    * Only needed if __VOIP feature flag is defined.
    */
    ECmdShowVoipEmergencyCallErrorNoteNoCsQuery,

    /**
    * A command for showing the "SIM memory low" query on secondary display.
    */
    ECmdShowSimMemoryLowQuery,

    /**
    * A command for showing the "Device memory low" query on secondary display.
    */
    ECmdShowDeviceMemoryLowQuery,

    /**
    * A command for showing a profile change note on secondary display
    *
    * @input    TProfileNotePckg          Profile name.
    */    
    ECmdShowProfileNote,
    
    /**
    * A command for showing the startup failure note on secondary display.
    *
    * @input    TStartupFailureNotePckg   Failed application name.
    *
    * @deprecated Note removed in S60 3.2.
    */
    ECmdShowStartupFailureNote,
       
    /**
    * A command for showing BT SAP disconnection query on secondary display.
    *
    * @input    TBtSapDisconnectQueryPckg Disconnected device name.
    */
    ECmdShowBtSapDisconnectQuery,
    
    /**
    * A command for showing Power Key menu on secondary display.
    */
    ECmdShowPowerKeyListQuery,
    
    /**
    * A command for showing "Power saving activated" confirmation note on secondary display.
    */
    ECmdShowPowerSavingActivatedNote,
    
    /**
    * A command for showing "Could not activate power saving" warning note on secondary display.
    */
    ECmdShowCannotActivatePowerSavingNote,
    
    /**
    * A command for showing "Power saving deactivated" onfirmation note on secondary display.
    */
    ECmdShowPowerSavingDeactivatedNote,
    
    /**
    * A command for showing "Could not deactivate power saving" warning note on secondary display.
    */
    ECmdShowCannotDeactivatePowerSavingNote,
    
    /**
    * A command for showing the "Battery low. Activate power saving?" query on secondary display.
    */
    ECmdShowBattLowActivatePowerSavingQuery,
    
    /**
    * A command for showing the "Charging. Deactivate power saving?" query on secondary display.
    */
    ECmdShowChargingDeactivatePowerSavingQuery,
    
    /**
    * A command for showing "FM transmission disabled!" information note on secondary display.
    */
    ECmdShowFmTxDisabledNote,

    /**
    * A command for showing "Tune radio to %U MHz" information note on secondary display.
    *
    * @input TFmTxFrequencyInKhzPckg  tuned frequency in KHz. It is up to client to convert this value to MHz.
    */
    ECmdShowFmTxTuneRadioToFrequencyNote,

    /**
    * A command for showing "FM transmission goes standy while an accessory is connected" information note on secondary display.
    */
    ECmdShowFmTxStandbyInAccessoryConnectionNote,

    /**
    * A command for showing "Cannot enable FM transmission when an accessory is connected" information note on secondary display.
    */
    ECmdShowFmTxCannotEnableWhileAccessoryConnectedNote,

    /**
    * A command for showing "Cannot change volume when FM transmission is enabled" information note on secondary display.
    */
    ECmdShowFmTxVolumeDisabledNote,

    /**
    * A command for showing "Keep FM tranmission on in Offline mode?" query on secondary display.
    */
    ECmdShowFmTxKeepOnInOfflineQuery    
    };

/*
* ==============================================================================
* Identifiers for items shown in power key menu
* ==============================================================================
*/
enum TPowerKeyMenuIds
    {
    /**
    * Switch off
    */
    EPwrMenuItemSwitchOff = 0,
    
    /**
    * Lock keypad
    */
    EPwrMenuItemLockKeypad,
    
    /**
    * Exit Sim Access Profile
    */
    EPwrMenuItemExitBtSap,
    
    /**
    * Lock device
    */
    EPwrMenuItemLockDevice,
    
    /**
    * Eject MMC. This is not used with multiple drives!
    */
    EPwrMenuItemEjectMMC,
      
    /**
    * Profile item base. Profile ID of corresponding profile 
    * is added to this value.
    */
    EPwrMenuItemProfileItemBase = 10000,

    /**
    * Eject item base used with multiple drives.
    */
    EPwrMenuItemEjectItemBase = 12000,

    /**
    * Activate power saving
    */
    EPwrMenuItemActivatePowerSaving = 14000,
    
    /**
    * Deactivate power saving
    */
    EPwrMenuItemDeactivatePowerSaving
    
    };

/*
* ==============================================================================
* Parameter definitions for the messages in this file.
* ==============================================================================
*/

// Parameter definitions for ECmdShowProfileNote
//
const TInt KProfileNameMaxLen = 64;
typedef TBuf<KProfileNameMaxLen> TProfileName;
typedef TPckgBuf<TProfileName> TProfileNotePckg;

// Parameter definitions for ECmdShowStartupFailureNote
//
const TInt KAppNameMaxLen = 100; 
typedef TBuf<KAppNameMaxLen> TAppName;
typedef TPckgBuf<TAppName> TStartupFailureNotePckg;

// Parameter definitions for ECmdShowBtSapDisconnectQuery
//
const TInt KMaxDeviceNameLen = 256; 
typedef TBuf<KMaxDeviceNameLen> TDeviceName;
typedef TPckgBuf<TDeviceName> TBtSapDisconnectQueryPckg;

// Parameter definitions for ECmdShowFmTxTuneRadioToFrequencyNote 
//
typedef TPckgBuf<TInt> TFmTxFrequencyInKhzPckg;

} // namespace SecondaryDisplay

#endif      // SECONDARY_DISPLAY_SYSAP_API_H
            
// End of File
