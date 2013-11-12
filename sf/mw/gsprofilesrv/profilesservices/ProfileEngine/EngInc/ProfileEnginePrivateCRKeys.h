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
* Description:  Profiles engine private Central Repository keys
*
*/



#ifndef PROFILEENGINEPRIVATECRKEYS_H
#define PROFILEENGINEPRIVATECRKEYS_H

//  INCLUDES
#include <e32std.h>
#include <ProfileEngineInternalCRKeys.h>
#include <ProfileEng.hrh>

// CONSTANTS
/**
 * List of IDs used for user-created profiles. Binary.
 *
 * Default value is empty (0-length byte array).
 */
const TUint32 KProEngUsedIds               = 0x80000002;

/**
 * If the current active profile is timed, this contains the ID of the previous
 * active profile (in other words, the profile which should be activated after
 * timer goes off). Integer.
 * Default value: -1
 */
const TUint32 KProEngPreviousActiveId      = 0x80000003;

/**
 * If the current active profile is timed, this contains the scheduler handle
 * ID for the task for the profile deactivation. Integer.
 * Default value: -1
 */
const TUint32 KProEngSchedulerHandleId     = 0x80000004;

/**
 * If the current active profile is timed, this contains the scheduler task
 * ID for the profile deactivation. Integer.
 * Default value: -1
 */
const TUint32 KProEngSchedulerTaskId       = 0x80000005;

//
// The rest of the constants are used with profile-specific settings. The
// actual key for a setting of a profile must be constructed as follows:
//
//   (PROFILE_ID << 32) & SETTING_CONSTANT
//

/**
 * Bitmask used to indicate which of the settings in Profiles are allowed to
 * be modified by the user. Integer.
 *
 * Default value: 0x000037FF for General (id == 0) profile
 *                0x000000EC for Off-line profile
 *                0x00003FFF for other profiles
 */
const TUint32 KProEngModifiableFlags       = EProfileSettingIdModifiableFlags;

/**
 * Bitmask used to indicate which of the settings in Profiles should be
 * visible to the user. Integer.
 *
 * Default value: 0x000037FF for General (id == 0) profile
 *                0x000000EC for Off-line profile
 *                0x00003FFF for other profiles
 */
const TUint32 KProEngVisibleFlags          = EProfileSettingIdVisibleFlags;

/**
 * The user-given name of the profile. String.
 *
 * Default value must be empty string.
 */
const TUint32 KProEngProfileName           = EProfileSettingIdProfileName;

/**
 * List of Contact group IDs. Binary.
 *
 * Default value must be empty.
 */
const TUint32 KProEngAlertForGroups        = EProfileSettingIdAlertForGroups;

/**
 * The path to the tone file to be used as (line 1) ringing tone. String.
 *
 * Default values:
 *
 *   General (0):  "Z:\\Nokia\\Sounds\\Digital\\Nokia Tune.mid"
 *   Silent (1):   "Z:\\Nokia\\Sounds\\Digital\\Nokia Tune.mid"
 *   Meeting (2):  "Z:\\Nokia\\Sounds\\Digital\\Nokia Tune.mid"
 *   Outdoor (3):  "Z:\\Nokia\\Sounds\\Digital\\Nokia Tune.mid"
 *   Pager (4):    "Z:\\Nokia\\Sounds\\Digital\\Nokia Tune.mid"
 *   Off-line (5): "Z:\\resource\\No_Sound.wav
 */
const TUint32 KProEngRingingTone           = EProfileSettingIdRingingTone;

/**
 * The path to the tone file to be used as line 2 ringing tone. String.
 *
 * Default values:
 *
 *   General (0):  "Z:\\Nokia\\Sounds\\Digital\\Nokia Tune.mid"
 *   Silent (1):   "Z:\\Nokia\\Sounds\\Digital\\Nokia Tune.mid"
 *   Meeting (2):  "Z:\\Nokia\\Sounds\\Digital\\Nokia Tune.mid"
 *   Outdoor (3):  "Z:\\Nokia\\Sounds\\Digital\\Nokia Tune.mid"
 *   Pager (4):    "Z:\\Nokia\\Sounds\\Digital\\Nokia Tune.mid"
 *   Off-line (5): "Z:\\resource\\No_Sound.wav
 */
const TUint32 KProEngRingingTone2          = EProfileSettingIdRingingTone2;

/**
 * The path to the tone file to be used as SMS alert tone. String.
 *
 * Default values:
 *
 *   General (0):  "Z:\\Nokia\\Sounds\\Digital\\Message 1.mid"
 *   Silent (1):   "Z:\\resource\\No_Sound.wav
 *   Meeting (2):  "Z:\\Nokia\\Sounds\\Simple\\Beep twice.rng"
 *   Outdoor (3):  "Z:\\Nokia\\Sounds\\Digital\\Message 1.mid"
 *   Pager (4):    "Z:\\Nokia\\Sounds\\Digital\\Message 1.mid"
 *   Off-line (5): "Z:\\Nokia\\Sounds\\Digital\\Message 1.mid"
 */
const TUint32 KProEngMsgTone               = EProfileSettingIdMsgTone;

/**
 * Ringing type of the profile. Integer.
 *
 * Default values:
 *
 *   General (0):  0
 *   Silent (1):   4
 *   Meeting (2):  3
 *   Outdoor (3):  0
 *   Pager (4):    4
 *   Off-line (5): 0
 */
const TUint32 KProEngRingingType           = EProfileSettingIdRingingType;

/**
 * Volume of the keypad tones of the profile. Integer.
 *
 * Default values:
 *
 *   General (0):  2
 *   Silent (1):   0
 *   Meeting (2):  0
 *   Outdoor (3):  3
 *   Pager (4):    2
 *   Off-line (5): 2
 */
const TUint32 KProEngKeypadVolume          = EProfileSettingIdKeypadVolume;

/**
 * Volume used when playing an alert tone. Integer (1-10).
 *
 * Default values:
 *
 *   General (0):  7
 *   Silent (1):   1
 *   Meeting (2):  1
 *   Outdoor (3):  10
 *   Pager (4):    7
 *   Off-line (5): 7
 */
const TUint32 KProEngRingingVolume         = EProfileSettingIdRingingVolume;

/**
 * This value indicates whether vibra should be on or off. Integer (0 or 1).
 *
 * Default values:
 *
 *   General (0):  0
 *   Silent (1):   0
 *   Meeting (2):  0
 *   Outdoor (3):  1
 *   Pager (4):    1
 *   Off-line (5): 0
 */
const TUint32 KProEngVibra                 = EProfileSettingIdVibra;

/**
 * This value indicates whether warning and game tones should be on or off.
 * Integer (0 or 1).
 *
 * Default values:
 *
 *   General (0):  1
 *   Silent (1):   0
 *   Meeting (2):  0
 *   Outdoor (3):  1
 *   Pager (4):    1
 *   Off-line (5): 1
 */
const TUint32 KProEngWarnAndGameTones      = EProfileSettingIdWarnAndGameTones;

/**
 * NOTE! This key is not used anymore.
 * The path to the tone file to be used as IM alert tone. String.
 *
 * Default values:
 *
 *   General (0):  "Z:\\Nokia\\Sounds\\Digital\\Chat alert.mid"
 *   Silent (1):   "Z:\\resource\\No_Sound.wav
 *   Meeting (2):  "Z:\\Nokia\\Sounds\\Simple\\Beep twice.rng"
 *   Outdoor (3):  "Z:\\Nokia\\Sounds\\Digital\\Chat alert.mid"
 *   Pager (4):    "Z:\\Nokia\\Sounds\\Digital\\Chat alert.mid"
 *   Off-line (5): "Z:\\resource\\No_Sound.wav
 */
const TUint32 KProEngImTone                = EProfileSettingIdImTone;

/**
 * The path to the tone file to be used as E-mail alert tone. String.
 *
 * Default values:
 *
 *   General (0):  "Z:\\Nokia\\Sounds\\Digital\\Message 1.mid"
 *   Silent (1):   "Z:\\resource\\No_Sound.wav
 *   Meeting (2):  "Z:\\Nokia\\Sounds\\Simple\\Beep twice.rng"
 *   Outdoor (3):  "Z:\\Nokia\\Sounds\\Digital\\Message 1.mid"
 *   Pager (4):    "Z:\\Nokia\\Sounds\\Digital\\Message 1.mid"
 *   Off-line (5): "Z:\\resource\\No_Sound.wav"
 */
const TUint32 KProEngEmailTone             = EProfileSettingIdEmailTone;

/**
 * The path to the tone file to be used as video call ringing tone. String.
 *
 * Default values:
 *
 *   General (0):  "Z:\\Nokia\\Sounds\\Digital\\Nokia Tune.mid"
 *   Silent (1):   "Z:\\Nokia\\Sounds\\Digital\\Nokia Tune.mid"
 *   Meeting (2):  "Z:\\Nokia\\Sounds\\Digital\\Nokia Tune.mid"
 *   Outdoor (3):  "Z:\\Nokia\\Sounds\\Digital\\Nokia Tune.mid"
 *   Pager (4):    "Z:\\Nokia\\Sounds\\Digital\\Nokia Tune.mid"
 *   Off-line (5): "Z:\\resource\\No_Sound.wav
 */
const TUint32 KProEngVideoCallTone         = EProfileSettingIdVideoCallTone;

/**
 * NOTE! This key is not currently used as there is no Presence Application.
 * If the status message of Presence is set to depend on Profiles, this value
 * contains the status message used with the profile. String.
 *
 * Default value must be empty.
 */
const TUint32 KProEngStatusMsg             = EProfileSettingIdStatusMsg;

/**
 * NOTE! This key is not currently used as there is no Presence Application.
 * If the availability attribute of Presence is set to depend on Profiles, this
 * contains the value of the attribute. Integer.
 *
 * Default values:
 *
 *   General (0):  2
 *   Silent (1):   1
 *   Meeting (2):  1
 *   Outdoor (3):  2
 *   Pager (4):    1
 *   Off-line (5): 1
 */
const TUint32 KProEngAvailAttribute        = EProfileSettingIdAvailAttribute;

/**
 * NOTE! This key is not used anymore.
 * Volume setting used by different media playing applications. Integer.
 *
 * Default values:
 *
 *   General (0):  7
 *   Silent (1):   1
 *   Meeting (2):  1
 *   Outdoor (3):  10
 *   Pager (4):    7
 *   Off-line (5): 7
 */
const TUint32 KProEngMediaVolume           = EProfileSettingIdMediaVolume;

/**
 * This integer key indicates whether text-to-speech ringing tone is on (1)
 * or off (0).
 *
 * Default value: 0 for all profiles
 */
const TUint32 KProEngTextToSpeechTone      = EProfileSettingIdTextToSpeechTone;

/**
 * This integer key indicates if the user has changed the name of the profile.
 * This information is needed by Profiles Engine to decide whether the name
 * should be localised or not.
 *
 * Not changed:    0
 * Changed:        1
 *
 * Default value: 0
 */
const TUint32 KProEngNameChanged           = EProfileSettingIdNameChanged;

/**
 * NOTE! This key is not used anymore.
 * This String key contains either the text of the call object if the selected
 * type is Text (= 2) or the path to the image file if the type is Image (= 3).
 *
 * Default value: empty string
 */
const TUint32 KProEngCallObjectContent     = EProfileSettingIdCallObjectContent;

/**
 * Contains the Push to talk ringing tone used with the profile. String.
 *
 * Default values:
 *
 *   General (0):  "Z:\\Nokia\\Sounds\\Digital\\Nokia Tune.mid"
 *   Silent (1):   "Z:\\Nokia\\Sounds\\Digital\\Nokia Tune.mid"
 *   Meeting (2):  "Z:\\Nokia\\Sounds\\Digital\\Nokia Tune.mid"
 *   Outdoor (3):  "Z:\\Nokia\\Sounds\\Digital\\Nokia Tune.mid"
 *   Pager (4):    "Z:\\Nokia\\Sounds\\Digital\\Nokia Tune.mid"
 *   Off-line (5): "Z:\\resource\\No_Sound.wav
 */
const TUint32 KProEngPttTone             = EProfileSettingIdPttTone;

/**
 * Contains the push to talk status value of this profile. Integer.
 *
 * Possible values:
 *
 *  0 (EProfilePttActive)
 *  1 (EProfilePttSilent)
 *  2 (EProfilePttDoNotDisturb)
 *
 * Default value: 1 for all profiles
 *
 */
const TUint32 KProEngPttStatus           = EProfileSettingIdPttStatus;

/**
 * Contains the 3D effect value of this profile. Integer.
 *
 * Possible values:
 *
 *  0 (EProfile3DEffectOff)
 *  1 (EProfile3DEffectCircle)
 *  2 (EProfile3DEffectFlyby)
 *  3 (EProfile3DEffectZigZag)
 *  4 (EProfile3DEffectRandomWalk)
 *  5 (EProfile3DEffectChaos)
 *  6 (EProfile3DEffectStereoWidening)
 *
 * Default value: 0 for all profiles
 *
 */
const TUint32 KProEng3DToneEffect           = EProfileSettingId3DEffect;

/**
 * Contains the 3D echo value of this profile. Integer.
 *
 * Possible values:
 *
 *  0 (EProfile3DEchoOff)
 *  1 (EProfile3DEchoLivingRoom)
 *  2 (EProfile3DEchoCave)
 *  3 (EProfile3DEchoStoneCorridor)
 *  4 (EProfile3DEchoForest)
 *  5 (EProfile3DEchoSewerPipe)
 *
 * Default value: 0 for all profiles
 *
 */
const TUint32 KProEng3DToneEcho           = EProfileSettingId3DEcho;

/**
 * Contains the tactile feedback value of this profile. Integer.
 *
 * Possible values:
 *
 *  0 (EProfileTactileFeedbackOff)
 *  1 (EProfileTactileFeedbackLevel1)
 *  2 (EProfileTactileFeedbackLevel2)
 *  3 (EProfileTactileFeedbackLevel3)
 *
 * Default value: 0 for Silent and Public/Drive, 1 for all others 
 *
 */
const TUint32 KProEngTactileFeedback           = EProfileSettingIdTactileFeedback;

/**
 * Contains the audio feedback value of this profile. Integer.
 *
 * Possible values:
 *
 *  0 (EProfileAudioFeedbackOff)
 *  1 (EProfileAudioFeedbackLevel1)
 *  2 (EProfileAudioFeedbackLevel2)
 *  3 (EProfileAudioFeedbackLevel3)
 *
 * Default value: 0 for all profiles
 *
 */
const TUint32 KProEngAudioFeedback           = EProfileSettingIdAudioFeedback;

/**
 * 
 */
const TUint32 KProEngReminderTone            = EProfileSettingIdReminderTone;

/**
 * 
 */
const TUint32 KProEngClockAlarmTone          = EProfileSettingIdClockAlarmTone;

/**
 * 
 */
const TUint32 KProEngAlertVibra              = EProfileSettingIdAlertVibra;

/**
 * The id used for storing the default settings for user created profiles.
 */
const TInt KProEngDefaultProfileId = 0x007F;

#endif      // PROFILEENGINEPRIVATECRKEYS_H

// End of File
