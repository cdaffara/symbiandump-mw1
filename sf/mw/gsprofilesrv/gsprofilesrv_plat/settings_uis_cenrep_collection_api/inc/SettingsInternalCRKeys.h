/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Settings UIs domain Central Repository keys.
*
*/



#ifndef SETTINGSINTERNALCRKEYS_H
#define SETTINGSINTERNALCRKEYS_H

// INCLUDES
#include <e32def.h>

// CONSTANTS

/**
* Network settings CR UID.
*/
const TUid KCRUidNetworkSettings = {0x101F8751};


/**
* Determines whether the user see visual notifications
* when entering or leaving different cellular systems.
* Possible interger values:
* 0 = Off
* 1 = On
*/
const TUint32 KSettingsServiceAlertNotes = 0x00000004;

/**
* Determines whether the user will hear a service tone
* when entering or leaving different cellular systems.
* Possible interger values:
* 0 = Off
* 1 = On
*/
const TUint32 KSettingsServiceAlertTones = 0x00000005;

/**
* Micro cellular network (Mcn) Display
*
* Possible values:
* 0 = Off
* 1 = On
*/
const TUint32 KSettingsMcnDisplay = 0x00000006;


/**
* Security settings CR UID
*/
const TUid KCRUidSecuritySettings = {0x101F877A};

/**
* Autolock time
* Possible values are:
* 0-999 minutes
*/
const TUint32 KSettingsAutoLockTime = 0x00000001;

/**
* Autolock status
* Possible values are:
* 0 = phone is not locked
* 1 = phone is locked
*/
const TUint32 KSettingsAutolockStatus = 0x00000002;

/**
* A CDMA Operator specific requirement, to be able to set the phone to
* "Lock On Power Up" in General Settings. When this key is set to 1
* the device security will lock the system (using Autolock) after each
* phone reboot.
*
* Possible values are:
* 0 = The phone doesn't get locked on Power Up
* 1 = The phone gets locked on Power Up
*/
const TUint32 KSettingsLockOnPowerUp = 0x00000003;

/**
* Transaction Tracking status
* Possible values are:
* 0 = disabled
* 1 = enabled
*/
const TUint32 KSettingsTransactionTracking = 0x00000004;

/**
* Automatic keyguard time in seconds
* Possible values are from 0 to 3600
*/
const TUint32 KSettingsAutomaticKeyguardTime = 0x00000005;

/**
* Automatic keyguard maximum time
* Value in seconds
*/
const TUint32 KSettingsMaxAutomaticKeyguardTime = 0x00000006;


/**
* Communication (aka networking) related settings CR UID
*/
const TUid KCRUidCommunicationSettings = {0x101F877B};

/**
* Name of polymorphic DLL to be loaded for VPN management UI.
*/
const TUint32 KSettingsVPNImplementation = 0x00000001;

/**
* VPN tab in Connection settings UI to be displayed or
* hidden from the user.
*
* Possible values are:
* 0 = Tab is hidden
* 1 = Tab is shown
*/
const TUint32 KSettingsVPNSupported = 0x00000002;

/**
* Access Points item in Connection settings folder to be displayed or
* hidden from the user.
*
* Possible values are:
* 0 = item is hidden
* 1 = item is shown
*/
const TUint32 KSettingsAPSettingsSupported = 0x00000003;


/**
*  Current airplane mode
*  
*  Possible values are:
*  0 = airplane mode (offline mode) Off
*  1 = airplane mode (offline mode) On
*/
const TUint32 KSettingsAirplaneMode = 0x00000004;

/**
* Terminal personalization settings CR UID
*/
const TUid KCRUidPersonalizationSettings = {0x101F877C};



/**
* Is operator logo is shown to the user.
* 0 = Logo not shown
* 1 = Logo shown
*/
const TUint32 KSettingsDisplayOperatorLogo = 0x00000003;

/**
* Defines if the "Operator Logo" list item is to be made visible or
* hidden in General Settings.
*/
const TUint32 KSettingsShowOperatorLogoSetting = 0x00000004;

/**
* Defines if Active Idle functionality is enabled.
* 0 = Disabled
* 1 = Enabled
*/
const TUint32 KSettingsActiveIdleState = 0x00000006;

/**
* Hide/Unhide Confirm Sat operations item in UI
* 0 = Hide
* 1 = Show
*/
const TUint32 KSettingsConfirmSatOperationsSupported = 0x00000007;

/**
* Screen saver period.
* The value is an integer between 5 and 90.
* Default value is 15
**/
const TUint32 KSettingsScreenSaverPeriod =  0x00000008;

/**
* Confirm Sat operations setting
* 0 = No
* 1 = Yes
*/
const TUint32 KSettingsConfirmSatOperations = 0x00000009;

/**
* Custom banner text for idle screen
* String value.
*/
const TUint32 KSettingsCustomBannerText = 0x0000000A;

/**
* Power Save Led setting value
* Integer value.
*/
const TUint32 KSettingsDisplayTurnoffTimeout = 0x0000000B;

/**
* Plugin UID of the operator specific full screen ActiveIdle.
* Integer value.
* Default value: 0
*/
const TUint32 KSettingsOperatorActiveIdle = 0x0000000C;

/**
* Pen writing speed for HWR
* Possible values are: 1-5 (very fast - very slow)
* Default value: 3
*/
const TUint32 KSettingsWritingSpeed = 0x0000000D;

/**
* Pen trail width for HWR.
* Possible values are: 1-5 (very thin - very thick)
* Default value: 3
*/
const TUint32 KSettingsPenWidth = 0x0000000E;

/**
* Pen trail color for HWR.
* 24-bit RGB colour value with 8 bits each for red, green and blue.
* The integer is of form 0xaabbggrr
* Default value: 0
*/
const TUint32 KSettingsPenColor = 0x0000000F;

/**
* Default input method of the Chinese PRC VKB.
* Possible values are: 1 - PinYin; 4 - Stroke
* Default value: 1
*/
const TUint32 KSettingsDefaultVkbPrc = 0x00000010;

/**
* Default input method of the Chinese TaiWan VKB.
* Possible values are: 2 - ZhuYin; 4 - Stroke
* Default value: 2
*/
const TUint32 KSettingsDefaultVkbTw = 0x00000011;

/**
* Default input method of the Chinese HongKong VKB.
* Possible values are: 4 - Stroke; 8 - CangJie
* Default value: 4
*/
const TUint32 KSettingsDefaultVkbHk = 0x00000012;

/**
* Pen Input Language.
* Possible values defined in enum TLanguage
* Default value: 0
*/
const TUint32 KSettingsPenInputLang = 0x00000013;

/**
* Indicates whether T-coil hearing aid mode is enabled or not.
* Integer value.
* Default value: 0
* 0 = disabled
* 1 = enabled
*/
const TUint32 KSettingsHearingAidMode = 0x00000014;

/**
* Guide line used in hand writing recognition box.
* If enabled, horizontal line will appear in the box.
* Default value: 1
* 0 = Guide Line Off
* 1 = Guide Line On
*/
const TUint32 KSettingsPenInputGuideLine = 0x00000015;

/**
* To Variate Operator Specific ActiveIdle Text.
* String value.
*/
const TUint32 KSettingsOperatorVariantActiveIdleText = 0x00000016;

/**
* To variate screensaver timeout settings item visibility
* 0 = item is hidden (default)
* 1 = item is shown
*/
const TUint32 KSettingsScreensaverTimeoutItemVisibility = 0x00000017;

/**
* Indicates whether the function (recognition with dictionary) is enabled or not.
* Boolean value.
* Default value: 0
* 0 = disabled
* 1 = enabled
*/
const TUint32 KSettingRecognitionwithdictionary = 0x00000018;

/**
* To save the setting for enable / disable optical joystick
* Integer value.
* Default value: 1
* 0 = Guide Line Off
* 1 = Guide Line On
*/
const TUint32 KSettingsOpticalJoystickStatus = 0x00000019;

/**
* Telephony settings CR UID.
*/
const TUid KCRUidTelephonySettings = {0x101F877E};

/**
* Any key answer
*
* Possible values:
* 0 = Off
* 1 = On
*/
const TUint32 KSettingsAnyKeyAnswer = 0x00000001;

/**
* Automatic redial
*
* 0 = Off
* 1 = On
*/
const TUint32 KSettingsAutomaticRedial = 0x00000002;

/**
* One key dialling
*
* 0 = Off
* 1 = On
*/
const TUint32 KSettingsOneKeyDialling = 0x00000003;

/**
* This key will include information whether the prefix change
* is active.
*
* 0 = Off
* 1 = On
*/
const TUint32 KSettingsDialPrefixChangeMode = 0x00000004;

/**
* This key will include the prefix set by user.
* Text string maximum 10 characters.
* Only dial related characters allowed.
*/
const TUint32 KSettingsDialPrefixText = 0x00000005;

/**
* Soft Reject default text used.
*
* 0 = Default in use
* 1 = User defined text in use
*/
const TUint32 KSettingsSoftRejectDefaultInUse = 0x00000006;

/**
* Soft Reject On/Off. Has the user enabled Soft Reject.
* 0 = Off
* 1 = On
*/
const TUint32 KSettingsSoftReject = 0x00000007;

/**
* User defined Soft Reject text. Any text up to 160 characters.
* No default value in CentRep (there is a default value, but it is
* localized).
*/
const TUint32 KSettingsSoftRejectText = 0x00000008;

/**
* Defines if still image for video calls (when camera is disabled) is in use.
* 0 = Off
* 1 = On
*/
const TUint32 KSettingsVTStillImage = 0x00000009;

/**
* This keys is required for the Video Telephony still image
* feature in General Settings to store the still image path
*/
const TUint32 KSettingsVTStillImagePath = 0x0000000A;

/**
* This key is required for VoIP Last Call Timer
*/
const TUint32 KLogsLastCallTimerLineVoIP = 0x0000000B;

/**
* This key is required for VoIP Received Calls Timer
*/
const TUint32 KLogsReceivedCallsTimerLineVoIP = 0x0000000C;

/**
* This key is required for VoIP Dialled Calls Timer
*/
const TUint32 KLogsDialledCallsTimerLineVoIP = 0x0000000D;

/**
* This key is required for SCCP stack installation indication
*/
const TUint32 KSCCPinstallUpgrade = 0x0000000E;

/**
* This key is required for dynamic VoIP
*/
const TUint32 KDynamicVoIP = 0x0000000F;

/**
* This key is required for Long Press of Call Key setting item
* Values: 0,1 --> Inactive
*		  2 --> Video call
* (Default value is 1 and when Restore Fact. Settings is done value is 0)
*/
const TUint32 KSettingsUiLongCallKeyPress = 0x00000010;

/**
* This key is required for VT call answer mute status
* 0 = Ask first (default value)
* 1 = No
* 2 = Yes
*/
const TUint32 KSettingsVTVideoSending = 0x00000011;


/**
* key to define what to do when opening slide
* 0: Not answer call when opening slide
* 1: Answer call when opening slide (default)
*/
const TUint32 KSettingsOpeningSlideAnswer = 0x00000012;

/**
* key to define what to do when closing slide
* 0: Not end call when closing slide
* 1: End call when closing slide (default)
*/
const TUint32 KSettingsClosingSlideEnd = 0x00000013;


/**
* key to define whether to show or not show side settings 
* 0: Not show slide settings (default)
* 1: Show slide settings
*/

const TUint32 KSettingsShowSlideSettings = 0x00000014;

/**
* CDMA Telephony settings CR UID.
*/
const TUid KCRUidCdmaTelephonySettings = {0x101F877F};

/**
* Locale settings CR UID.
*/
const TUid KCRUidLocaleSettings = {0x101F8780};

/**
* Arabic & Hebrew default number mode - integer value.
*/
const TUint32 KSettingsDefaultNumberMode = 0x00000001;

/**
* Default Indic Number Mode for Hindi Digits - integer value.
*/
const TUint32 KSettingsIndicDefaultNumberMode = 0x00000002;

/**
* Peripheral settings CR UID.
*/
const TUid KCRUidPeripheralSettings = {0x101F8781};

/**
* Timeout for setting off the display lights.
* Value is in seconds.
*/
const TUint32 KSettingsDisplayLightsTimeout = 0x00000001;

/**
* Call forwarding settings CR UID.
*/
const TUid KCRUidCallForwarding = {0x101F8782};

/**
* call forwarding "forwarded-to" number 1.
* String value
*/
const TUint32 KSettingsCFDefaultNumber1 = 0x00000001;

/**
* call forwarding "forwarded-to" number 2.
* String value
*/
const TUint32 KSettingsCFDefaultNumber2 = 0x00000002;

/**
* call forwarding "forwarded-to" number 3.
* String value
*/
const TUint32 KSettingsCFDefaultNumber3 = 0x00000003;

/**
* call forwarding "forwarded-to" number 1. Alternate line
* String value
*/
const TUint32 KSettingsCFDefaultNumberAls1 = 0x00000004;

/**
* call forwarding "forwarded-to" number 2. Alternate line
* String value
*/
const TUint32 KSettingsCFDefaultNumberAls2 = 0x00000005;

/**
* call forwarding "forwarded-to" number 3. Alternate line
* String value
*/
const TUint32 KSettingsCFDefaultNumberAls3 = 0x00000006;

/**
* "call forwarding when no reply"-timer value.
* Integer value.
*/
const TUint32 KSettingsCFTimerValue = 0x00000007;

/**
* "call forwarding when no reply"-timer value for alternate line.
* Integer value.
*/
const TUint32 KSettingsCFTimerValueAls = 0x00000008;

/**
* Last used number diverted data calls
* String value
*/
const TUint32 KSettingsCFUsedDataNumber = 0x00000009;

/**
* Last used number diverted fax calls
* String value
*/
const TUint32 KSettingsCFUsedFaxNumber = 0x0000000A;

/**
* Common Telephony Settings CR UID.
*/
const TUid KCRUidCommonTelephonySettings = {0x101F8831};

/**
* Summary after call
*
* 0 = Off
* 1 = On
*/
const TUint32 KSettingsSummaryAfterCall = 0x00000001;

/**
* Settings for Rich Call features
*/
const TUid KCRUidRichCallSettings = {0x101F883B};

/**
* Calling Line Identification Restriction for PS calls
*
* 0 = KRCSEClirOff
* 1 = KRCSEClirOn
* 2 = KRCSEClirDefault
*/
const TUint32 KRCSEClir = 0x00000000;

/**
* Determines whether warning is given to user, when WLAN signal
* strength drops too low.
*
* 0 = KRCSEEarlyWarningOff
* 1 = KRCSEEarlyWarningOn
*/
const TUint32 KRCSEWlanEarlyWarning = 0x00000001;

/**
* Circuit Switched call waiting.
*
* 0 = KRCSECSCallWaitingOff
* 1 = KRCSECSCallWaitingOn
*/
const TUint32 KRCSECSCallWaiting = 0x00000002;

/**
* Determines whether the registeration of VoIP is automatic or manual.
*
* 0 = KRCSEAutomatic
* 1 = KRCSEManual
*/
const TUint32 KRCSEPSLogin = 0x00000003;

/**
* Determines whether the user wants to receive anonymous calls.
* Possible values are:
* 0 = KRCSEAnonymousCallsAllowed
* 1 = KRCSEAnonymousCallsBlocked
*/
const TUint32 KRCSEAnonymousCallBlockRule = 0x00000004;

/**
* Determines call type preference.
* Possible values are:
* 0 = KRCSECSCallPreferred
* 1 = KRCSEInternetCallPreferred
*/
const TUint32 KRCSEPreferredTelephony = 0x00000005;

/**
* Determines status of Do Not Disturb setting.
* Possible values are:
* 0 = KRCSEDoNoDisturbOff
* 1 = KRCSEDoNoDisturbOn
*/
const TUint32 KRCSEDoNotDisturb = 0x00000006;

/**
* Determines status of PS (VoIP) call waiting setting.
* Possible values are:
* 0 = KRCSEPSCallWaitingOff
* 1 = KRCSEPSCallWaitingOn
*/
const TUint32 KRCSPSCallWaiting = 0x00000007;

/**
* Determines id of preferred VOIP service.
* 0 = Not set
* 1 = CS / Video service
* 2 = voip stub
* 3 = voip service 1
* 4 = voip service 2
* n = voip service n
*/
const TUint32 KRCSPSPreferredService = 0x00000008;


/**
* Miscelaneous Settings
*/
const TUid KCRUidMiscSettings = {0x1000A82B};

/**
Phone settings CR UID
*/
const TUid KCRUidPhoneSettings = {0x1028239D};

/**
Call waiting feature
0 = disabled
1 = Enabled
*/
const TUint32 KPSetCallWaiting = 0x00000001;

/**
*  Device Management Settings CR UID.
*/
const TUid KCRUidDeviceManagementSettings = {0x2002120B};

/**
Determines whether to show power saving query before entering power saving mode. 
0: Off
1: On (Default)
*/
const TUint32 KSettingsPowerSavingQuery = 0x00000001;

#endif      // SETTINGSINTERNALCRKEYS_H

// End of file