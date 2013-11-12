/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Profiles engine internal Central Repository keys.
*
*/



#ifndef PROFILEENGINEINTERNALCRKEYS_H
#define PROFILEENGINEINTERNALCRKEYS_H

//  INCLUDES
#include <e32std.h>
#include <ProfileEngineDomainCRKeys.h>

// CONSTANTS

/**
 * Information about whether self recorded tones can be used as ringing tones
 * or not. Integer value.
 *
 * Possible values:
 *
 * 0 (disabled)
 * 1 (enabled)
 */
const TUint32 KProEngRecordedRingingTones = 0x80000100;

/**
 * If this is true, DRM protected MP4 tones are not allowed as ringing tones.
 *
 * Possible values:
 *
 * 0 (allowed, default)
 * 1 (not allowed)
 */
const TUint32 KProEngNoDrmMP4RingingTones = 0x80000101;

/**
 * If this is true, WMA tones are not allowed as ringing tones.
 *
 * Possible values:
 *
 * 0 (allowed, default)
 * 1 (not allowed)
 */
const TUint32 KProEngNoWmaRingingTones = 0x80000102;

/**
 * Tactile feedback setting for the currently active profile.
 * THIS COULD BE MOVED TO THE SDK HEADER LATER?
 *
 * Possible values:
 *
 * 0 (Off)
 * 1 (Level 1)
 * 2 (Level 2)
 * 3 (Level 3)
 */
const TUint32 KProEngActiveTactileFeedback = 0x7E000019;

/**
 * Audio feedback setting for the currently active profile.
 * THIS COULD BE MOVED TO THE SDK HEADER LATER?
 *
 * Possible values:
 *
 * 0 (Off)
 * 1 (Level 1)
 * 2 (Level 2)
 * 3 (Level 3)
 */
const TUint32 KProEngActiveAudioFeedback = 0x7E00001A;

/**
* Absolute file path of ring tone of the currently active profile, string.
* Since 10.1
**/
const TUint32 KProEngActiveRingTone = 0x7E00001B;

/**
* Absolute file path of message tone of the currently active profile, string.
* Since 10.1
**/
const TUint32 KProEngActiveMessageTone = 0x7E00001C;

/**
* Absolute file path of reminder tone of the currently active profile, string.
* Since 10.1
**/
const TUint32 KProEngActiveReminderTone = 0x7E00001D;

/**
* Absolute file path of clock alarm tone of the currently active profile, string.
* Since 10.1
**/
const TUint32 KProEngActiveClockAlarmTone = 0x7E00001E;

/**
* Alert vibra setting of the currently active profile, string.
* 
* Since 10.1
* 
* Possible values (bit OR):
* 
* enum TProfileAlertVibra
    {
    EProfileNoAlertVibra       = 0x00000000,
    EProfileRingAlertVibra     = 0x00000001,
    EProfileMessageAlertVibra  = 0x00000002,
    EProfileEmailAlertVibra    = 0x00000004,
    EProfileReminderAlarmVibra = 0x00000008,
    EProfileClockAlarmVibra    = 0x00000010,
    EProfileInformationVibra   = 0x00000020
    };
* 
**/
const TUint32 KProEngActiveAlertVibra = 0x7E00001F;

/**
* Master volume for all profiles, int
* Since 10.1
**/
const TUint32 KProEngMasterVolume = 0x80000200;

/**
* Master vibra for all profiles, int
* Since 10.1
**/
const TUint32 KProEngMasterVibra  = 0x80000201;

/**
* Silence mode, bool 1: On 0: Off
* Since 10.1
**/
const TUint32 KProEngSilenceMode  = 0x80000202;

#endif      // PROFILEENGINEINTERNALCRKEYS_H

// End of File
