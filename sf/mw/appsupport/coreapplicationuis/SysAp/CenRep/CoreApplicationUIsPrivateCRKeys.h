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
* Description:  Private Central Repository definitions of the
*                Core Application UIs subsystem
*
*/


#ifndef COREAPPLICATIONUISPRIVATECRKEYS_H
#define COREAPPLICATIONUISPRIVATECRKEYS_H

// INCLUDES
#include <CoreApplicationUIsSDKCRKeys.h>

// =============================================================================
// CoreAppUIs Configuration API
// =============================================================================
const TUid KCRUidCoreApplicationUIsConf = { 0x101F8764 };

/**
* Bitmask used in configuring SysAp. See below for SysAp features.
*/
const TUint32 KCoreAppUIsSysAp = 0x00000001;

/**
* Location Privacy indicator id in avkon.hrh.
*/
const TUint32 KCoreAppUIsLocPrivAcceptAll = 0x00000002;

/**
* Location Privacy indicator id in avkon.hrh.
*/
const TUint32 KCoreAppUIsLocRejectAll = 0x00000003;

/**
* Location Privacy indicator id in avkon.hrh.
*/
const TUint32 KCoreAppUIsLocAlwaysAsk = 0x00000004;

/**
* Location Privacy indicator id in avkon.hrh.
*/
const TUint32 KCoreAppUIsLocIndividualPrivacy = 0x00000005;

/**
* KCoreAppUIsMmc REMOVED!
*/
const TUint32 KCoreAppCRKeyRemoved1 = 0x00000006;

/**
* KCoreAppUIsHotSwapEjectAvailable REMOVED! Use KFeatureIdMmcEject instead.
*/
const TUint32 KCoreAppCRKeyRemoved3 = 0x00000007;

/**
* Enable/disable support for Memory Card hatch in SysAp.
* Possible values: 0 (NO hatch), 1 (device has a hatch)
*/
const TUint32 KCoreAppUIsHotSwapMemoryCardHatch = 0x00000008;

/**
*
* KCoreAppUIsMemoryCardLockSupported REMOVED! Use KFeatureIdMmcLock instead.
*
*/
const TUint32 KCoreAppCRKeyRemoved2 = 0x00000009;

/**
* Enable/disable support for turning on lights when the keypad or device lock is enabled.
* Lights are lit with the intensity defined with key KCoreAppUIsLockedStateLightsIntensity.
* Ignored when the device is in an unlocked state.
* 
* Possible values: 0 - disabled
*                  1 - enabled: lights are lit with the defined intensity if the device is locked 
*
* @see KCoreAppUIsLockedStateLightsIntensity
*/
const TUint32 KCoreAppUIsLockedStateLights = 0x0000000A;

/**
* Configures the intensity used for light commands when KCoreAppUIsLockedStateLights is enabled.
* Supported intensity values are defined in Light API. If the configured value is not in supported range,
* it is converted to a valid value.
*
* @see KCoreAppUIsLockedStateLights
* @see CHWRMLight
*/
const TUint32 KCoreAppUIsLockedStateLightsIntensity = 0x0000000B;

/**
* Configures the remaining battery charge status as a percent value which is used for determining
* when to show the battery information preview popup. The popup is automatically displayed when the battery level
* decreases below the threshold.
*/
const TUint32 KCoreAppUIsBatteryInformationLowThreshold = 0x0000000C;

// =============================================================================
// System Application State API
// =============================================================================

const TUid KCRUidCoreApplicationUIsSysAp = { 0x101F8765 };

/**
* Stores keyguard status over reboot. Possible values: 0 (false), 1 (true)
*/
const TUint32 KSysApKeyguardActive = 0x00000001;

/**
* Stores accessory status over reboot. Possible values: 0 (false), 1 (true)
*/
const TUint32 KSysApAccessoryConnected = 0x00000002;

/**
* Profile id of the profile that was active before connecting an accessory.
*/
const TUint32 KSysApActiveProfileBeforeAccessoryConnected = 0x00000003;

/**
* Set true when the profile has been changed because of an accessory connected.
* Possible values: 0 (false), 1 (true)
*/
const TUint32 KSysApProfileUpdateRequired = 0x00000004;

/**
* Profile id of the profile that was active before Offline Mode.
*/
const TUint32 KSysApProfileBeforeOfflineMode = 0x00000005;

/**
* Possible values: 0 (BT not active), 1 (BT active)
*/
const TUint32 KSysApBtStatusBeforeOfflineMode = 0x00000006;

/**
* KSysApVoIPEmergencyWarningShown REMOVED!
*/
const TUint32 KSysApCRKeyRemoved1 = 0x00000007;

/**
* KSysApVoIPShowEmergencyWarningOnOff REMOVED! 
*/
const TUint32 KSysApCRKeyRemoved2 = 0x00000008;

/**
* LongPress Power Key Count
*/
const TUint32 KSysApPowerkeyRepeatcount = 0x00000009;

// =============================================================================
// System Application Local Variation constants
// =============================================================================

/** 
* KSysApFeatureIdGPRSNotes is an on/off setting for
* enabling GPRS suspend/resume notes when a phone call is activated/
* deactivated. By default on in S60 releases 2.6 and onwards.
*/
const TInt KSysApFeatureIdGPRSNotes = 0x01;  // 2^0

/** 
* KSysApFeatureIdGoodbyeNote is an on/off setting for enabling
* Goodbye Note for shutdown in CDMA products.
*/
const TInt KSysApFeatureIdGoodbyeNote = 0x02; // 2^1

/** 
* KSysApFeatureIdRoamingStatus is an on/off setting for enabling
* Roaming Status indicator in CDMA products.
*/
const TInt KSysApFeatureIdRoamingStatus = 0x04; // 2^2

/** 
* 
*/
const TInt KSysApFeatureIdUnused = 0x08; // 2^3

/** 
* KSysApFeatureIdCDMALocationIcon is an on/off setting for enabling
* CDMA Location indicator.
*/
const TInt KSysApFeatureIdCDMALocationIndicator = 0x10; // 2^4

/** 
* KSysApFeatureIdCDMA3GIcon is an on/off setting for enabling
* CDMA 3G indicator.
*/
const TInt KSysApFeatureIdCDMA3GIcon = 0x20; // 2^5

/** 
* KSysApFeatureIdGprs_G_Icon is an on/off setting for enabling
* the "old" G icon for GPRS status indicator.
*/
const TInt KSysApFeatureIdGprs_G_Icon = 0x40; // 2^6

/** 
* KSysApFeatureIdUseEdgeGPRSIcon is an on/off setting for enabling
* use of EdgeGPRS icon.
*/
const TInt KSysApFeatureIdUseEdgeGPRSIcon = 0x80; // 2^7

/**
* KSysApFeatureIdMemoryCardIcon is an on/off setting for enabling
* memory card icon.
*/
const TInt KSysApFeatureIdMemoryCardIcon = 0x100; // 2^8

/**
* KSysApFeatureIdUseHsdpaAvailableIcon is an on/off setting for enabling
* HSDPA Resource Availability icon.
*/
const TInt KSysApFeatureIdUseHsdpaAvailableIcon = 0x200; // 2^9

/**
* KSysApFeatureIdChargerReminderNotes is an on/off setting for enabling
* notes remainding user to unplug the charger when battery is full or 
* charger is disconnected.
*/
const TInt KSysApFeatureIdChargerReminderNotes = 0x400; // 2^10

/**
* KSysApFeatureIdUsbChargingWithoutReminderNotes is an on/off setting for disabling 
* unplug charger reminder notes when USB is used in normal mode e.g. for 
* file transfer. Define this setting only when USB charger is the only supported 
* charging method. DO NOT define this when phone supports non-USB chargers.
*/

const TInt KSysApFeatureIdUsbChargingWithoutReminderNotes = 0x800; // 2^11

/**
* If true phone display is not flashed when a call is received
*/
const TInt KSysApFeatureIdNoFlasUiInSilentMode = 0x1000; // 2^12

/**
* KSysApFeatureIdBatteryInfoPopup is an on/off setting for enabling
* a preview popup with battery level information in percents. If enabled, this popup
* is shown when the power key menu is activated or when the battery capacity
* decreases below the threshold value defined with the key KCoreAppUIsBatteryInformationLowThreshold.
* 
* @see KCoreAppUIsBatteryInformationLowThreshold
*/
const TInt KSysApFeatureIdBatteryInfoPopup = 0x2000; // 2^13

#endif      // COREAPPLICATIONUISPRIVATECRKEYS_H

// End of File

