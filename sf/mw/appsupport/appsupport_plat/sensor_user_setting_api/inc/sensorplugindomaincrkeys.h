/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Definitions for domain cenrep keys for sensor settings
*
*/



#ifndef SENSORPLUGINDOMAINCRKEYS_H
#define SENSORPLUGINDOMAINCRKEYS_H

#include <e32cmn.h>

/**
 * Sensors Settings API
 *
 * Contains sensors interaction settings
 */

const TUid KCRUidSensorSettings = {0x10282DF0};

/**
 * Specifies user seleted value for sensors activation status
 *
 * Possible values: 0: OFF
 *                  1: ON
 * 
 * Value is an integer.
 *
 * Default value: 0
 */
const TUint32 KSenSettingsSensorsOn    = 0x00000001;

/**
 * Bitmask of user selected use cases for Tapping interaction.
 *
 * Defined values:
 *  0000 0000: No selected use cases
 *  0000 0001: Only Silence calls use case selected
 *  0000 0010: Only Silence alarm use case selected
 *  0000 0100: Only Message use case selected
 *  0000 1000: Only Music Player use case selected
 *
 * Bitmask may contain none to all above selections
 * 
 * Value is an integer.
 *
 * Default value: 0
 */
const TUint32 KSenSettingsTapCtrl      = 0x00000002;

/**
 * Bitmask of user selected use cases for Turning interaction.
 *
 * Defined values:
 *  0000 0000: No selected use cases
 *  0000 0001: Only Silence calls use case selected
 *  0000 0010: Only Silence alarm use case selected
 *
 * Bitmask may contain none to all above selections
 * 
 * Value is an integer.
 *
 * Default value: 0
 */
const TUint32 KSenSettingsTurnCtrl     = 0x00000003;

/**
 * Bitmask of user selected use cases for Orientation interaction.
 *
 * Defined values:
 *  0000 0000: No selected use cases
 *  0000 0001: Only Camera use case selected
 *  0000 0010: Only Phone display use case selected
 *
 * Bitmask may contain none to all above selections
 * 
 * Value is an integer.
 *
 * Default value: 0
 */
const TUint32 KSenSettingsOrientation  = 0x00000004;

/// Variation keys

/**
 * Bitmask of variated use cases for Tapping interaction.
 *
 * Possible values:
 *  0000 0000: No selected use cases, Tapping interactions disabled
 *  0000 0001: Only Silence calls use case enabled
 *  0000 0010: Only Snooze alarm use case enabled
 *
 * Bitmask may contain different variations of above selections
 * 
 * Value is an integer.
 *
 * Default value: 3
 */
const TUint32 KSenSetVariationTapCtrl      = 0x00100002;

/**
 * Bitmask of variated use cases for Turning interaction.
 *
 * Possible values:
 *  0000 0000: No selected use cases, Turning interactions disabled
 *  0000 0001: Only Silence calls use case enabled
 *  0000 0010: Only Snooze alarm use case enabled
 *  0000 0100: Only Display orientation use case enabled
 *
 * Bitmask may contain different variations of above selections
 * 
 * Value is an integer.
 *
 * Default value: 7
 */
const TUint32 KSenSetVariationTurnCtrl     = 0x00100003;

#endif //SENSORPLUGINDOMAINCRKEYS_H

// End of File
