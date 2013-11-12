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
* Description:  Domain Central Repository definitions of Startup Services subsystem.
*
*/


#ifndef STARTUPDOMAINCRKEYS_H
#define STARTUPDOMAINCRKEYS_H

#include <centralrepository.h>

// =============================================================================
// Startup Configuration API
// =============================================================================
static const TUid KCRUidStartupConf = { 0x101F8763 };

// Key values starting from 0x00000101 are reserved - do not use for this API.

/**
* Welcome note image (user selection in GS)
* Old Shared Data constant name: KGSWelcomeNoteImage
*/
const TUint32 KStartupWelcomeNoteImage  = 0x00000004;

/**
* Welcome note text (user selection in GS)
* Old Shared Data constant name: KGSWelcomeNoteText
*/
const TUint32 KStartupWelcomeNoteText = 0x00000005;

/**
* Welcome note type (user selection in GS).
* Old Shared Data constant name: KGSWelcomeNoteType
*/
const TUint32 KStartupWelcomeNoteType  = 0x00000006;
enum TStartupWelcomeNoteType
    {
    EDefaultWelcomeNote = 0,
    ETextWelcomeNote,
    EImageWelcomeNote
    };

/**
* Operator note image. Will overrule the user selection, if defined.
*/
const TUint32 KStartupOperatorNoteImage = 0x00000007;

/**
* Operator note text. Will overrule the user selection, if defined.
*/
const TUint32 KStartupOperatorNoteText = 0x00000008;

/**
* Enable / disable startup queries (city, date, time).
* Can be used also with Predictive Time and Country Selection
* to disable queries that are shown when network time is unavailable.
*/
const TUint32 KStartupQueries = 0x0000000A;
enum TStartupQueries
    {
    EStartupQueriesDisabled = 0,
    EStartupQueriesEnabled
    };

/**
* Predictive Time and Country Selection.
* When Predictive Time and Country Selection is enabled, time and country
* information is queried from the network instead of from user.
* If network does not support this feature, user is queried to insert
* time, date and country manually.
*/
const TUint32 KPredictiveTimeAndCountrySelection = 0x0000000B;
enum TPredictiveTime
    {
    EPredictiveTimeDisabled = 0,
    EPredictiveTimeEnabled
    };

/**
* A string that defines the path and file name of the startup animation file.
* This is the first animation shown during the system start-up.
* The value is mandatory, and it has to be a file name and a path of a valid
* animation file.
* The animation file should be stored in a location where system applications
* have read access.
* Supported formats are SVG-T (.svg, .svgb), MIF (.mif) and GIF (.gif).
* Text or embedded images in SVG-T files are not supported.
* The length of the startup animation should be 3 seconds.
* Example: "Z:\\resource\\apps\\startup.svg"
*/
const TUint32 KStartupAnimationPath = 0x0000000D;

/**
* A frame delay value in microseconds for fine-tuning the startup animation.
* The value is only used if:
* a) The animation file contains an animation format, which does not contain
*    frame delay information in it (MIF), or
* b) The animation file contains a still image, and no startup tone
*    has been specified, in which case this value defines how long the image
*    is shown.
*/
const TUint32 KStartupAnimationFrameDelay = 0x0000000E;

/**
* Enable/disable scaling of the animation frames.
* The value is only used if the animation format supports enabling/disabling
* scaling.
* One of the values of TStartupAnimationScalingValue enumeration.
*/
const TUint32 KStartupAnimationScaling = 0x00000017;
enum TStartupAnimationScalingValue
    {
    EScalingDisabled = 0,
    EScalingEnabled
    };

/**
* A string that defines the path to the startup tone MIDI file.
* The tone is played at the same time as the startup animation is being shown.
* The value is optional - if no tone file is defined, the default beep tone is
* played.
* The tone file should be stored in a location where system applications have
* read access.
* The startup tone should be at most 3 seconds long. The tone will be played to
* the end before continuing with the next animation.
* Example: "Z:\\resource\\apps\\startup.wav"
* Old Shared Data constant name: KStartupAppTonePath
*/
const TUint32 KStartupTonePath = 0x00000003;

/**
* The volume level used when playing the startup tone.
* Possible values range from 0 to 10.
* If the ringing tone is silent, the tone is not played.
* Old Shared Data constant name: KStartupAppToneVolume
*/
const TUint32 KStartupToneVolume = 0x00000002;

/**
* A string that defines the path and file name of the operator-specific startup
* animation file.
* The operator-specific startup animation is shown after the startup animation.
* The value is optional. If defined, it has to be a file name and a path of a
* valid animation file.
* The animation file should be stored in a location where system applications
* have read access.
* Supported formats are SVG-T (.svg, .svgb), MIF (.mif) and GIF (.gif).
* Text or embedded images in SVG-T files are not supported.
* The length of the operator-specific startup animation should be 3-5 seconds.
* Animations longer than that may cause performance problems.
* Example: "Z:\\resource\\apps\\operatorstartup.svg"
*/
const TUint32 KStartupOperatorAnimationPath = 0x0000000F;

/**
* A frame delay value in microseconds for fine-tuning the operator-specific
* startup animation.
* The value is only used if:
* a) The operator-specific startup animation file contains an animation format,
*    which does not contain frame delay information in it (MIF), or
* b) The operator-specific startup animation file contains a still image, and no
*    operator-specific startup tone has been specified, in which case this value
*    defines how long the image is shown.
*/
const TUint32 KStartupOperatorAnimationFrameDelay = 0x00000010;

/**
* Enable/disable scaling of the animation frames.
* The value is only used if the animation format supports enabling/disabling
* scaling.
* One of the values of TStartupAnimationScalingValue enumeration.
*/
const TUint32 KStartupOperatorAnimationScaling = 0x00000018;

/**
* A string that defines the path to the operator-specific startup tone MIDI file.
* The tone is played at the same time as the operator-specific startup animation
* is shown.
* The value is optional.
* The tone file should be stored in a location where system applications have
* read access.
* The operator-specific startup tone should be at most 5 seconds long. The tone
* will be played to the end before continuing the startup.
* Example: "Z:\\resource\\apps\\operatorstartup.wav"
*/
const TUint32 KStartupOperatorTonePath = 0x00000009;

/**
* The volume level used when playing the operator-specific startup tone.
* Possible values range from 0 to 10.
* If the ringing tone is silent, the tone is not played.
*/
const TUint32 KStartupOperatorToneVolume = 0x00000011;

/**
* A string that defines the path and file name of the shutdown animation file.
* The shutdown animation is shown during the system shutdown.
* The value is optional. If defined, it has to be a file name and a path of a
* valid animation file.
* The animation file should be stored in a location where system applications
* have read access.
* Supported formats are SVG-T (.svg, .svgb), MIF (.mif) and GIF (.gif).
* Text or embedded images in SVG-T files are not supported.
* The length of the shutdown animation should be at most 3 seconds.
* Example: "Z:\\resource\\apps\\shutdown.svg"
*/
const TUint32 KShutdownAnimationPath = 0x00000012;

/**
* A frame delay value in microseconds for fine-tuning the shutdown animation.
* The value is only used if:
* a) The shutdown animation file contains an animation format, which does not
*    contain frame delay information in it (MIF), or
* b) The shutdown animation file contains a still image, and no shutdown tone
*    has been specified, in which case this value defines how long the image
*    is shown.
*/
const TUint32 KShutdownAnimationFrameDelay = 0x00000013;

/**
* Enable/disable scaling of the animation frames.
* The value is only used if the animation format supports enabling/disabling
* scaling.
* One of the values of TStartupAnimationScalingValue enumeration.
*/
const TUint32 KShutdownAnimationScaling = 0x00000019;

/**
* A string that defines the path to the shutdown tone MIDI file.
* The tone is played at the same time as the shutdown animation is shown.
* The value is optional.
* The tone file should be stored in a location where system applications have
* read access.
* The shutdown tone should be at most 3 seconds long. The tone will be played to
* the end before continuing the shutdown.
* Example: "Z:\\resource\\apps\\shutdown.wav"
*/
const TUint32 KShutdownTonePath = 0x00000014;

/**
* The volume level used when playing the shutdown tone.
* Possible values range from 0 to 10.
* If the ringing tone is silent, the tone is not played.
*/
const TUint32 KShutdownToneVolume = 0x00000015;

/**
* Default date & time value to propose to user during first boot.
* Format is "YYYYMMDD:HHMMSS". The month and day values are offset from zero.
* Example: "20070000:090000" - 01.01.2007 09:00 AM.
*/
const TUint32 KStartupDefaultTime = 0x00000016;

/**
 * Enables or disables the FTU StartupSequence. This key is modified by ftu when user accepts the eterms
 * Possible values true or false
 */
const TUint32 KFtuStartupEnabled = 0x0000001A;


#endif // STARTUPDOMAINCRKEYS_H

// End of File

