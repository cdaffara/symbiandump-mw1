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
* Description:  Profiles engine domain Central Repository keys.
*
*/



#ifndef PROFILEENGINEDOMAINCRKEYS_H
#define PROFILEENGINEDOMAINCRKEYS_H

//  INCLUDES
#include <e32std.h>
#include <ProfileEngineSDKCRKeys.h>

// CONSTANTS

/**
 * Default ringing tone for all profiles. String value. Read-only.
 * This ringing tone is used if some error condition prevents using the user-set
 * ringing tone for alerting of incoming call.
 *
 * Central Repository file of the key: KCRUidProfileEngine.
 *
 */
const TUint32 KProEngDefaultRingingTone = 0x7F000004;

/**
 * Default message alert tone for all profiles. String value. Read-only.
 * This alert tone is used if some error condition prevents using the user-set
 * alert tone for alerting of incoming message.
 *
 * Central Repository file of the key: KCRUidProfileEngine.
 *
 */
const TUint32 KProEngDefaultMessageTone = 0x7F000006;

/**
 * Default Instant Message alert tone for all profiles. String value. Read-only.
 * @deprecated IM does not use Profiles-dependent alert tone anymore.
 *
 * Central Repository file of the key: KCRUidProfileEngine.
 *
 */
const TUint32 KProEngDefaultImTone = 0x7F00000C;

/**
 * Default reminder tone for all profiles. String value. Read-only.
 * 
 * Central Repository file of the key: KCRUidProfileEngine.
 *
 */
const TUint32 KProEngDefaultReminderTone = 0x7F00001B;

/**
 * Default clock alarm tone for all profiles. String value. Read-only.
 * 
 * Central Repository file of the key: KCRUidProfileEngine.
 *
 */
const TUint32 KProEngDefaultClockAlarmTone = 0x7F00001C;

/**
 * This integer indicates the maximum file size (kilo bytes) of ring tone
 * files that can be selected as voice call ringing tone.
 *
 * Central Repository file of the key: KCRUidProfileEngine.
 *
 * Possible values:
 *
 * 0   switches off file size checking. I.e. then all files can be selected.
 * >0  max size. For example 100 would set the maximum file size as 100 kB.
 *
 */
const TUint32 KProEngRingingToneMaxSize = 0x80000103;



/**
 * Profiles local variation CR file.
 */
const TUid KCRUidProfilesLV = {0x101F8799};

/**
 * Standard bitmask type of key used for local variation of Profiles.
 * One bit is used to control one feature. If the bit is on (= 1) the feature
 * is enabled and if the bit is off the feature is off.
 * The meaning of each bit are defined in ProfilesVariant.hrh.
 *
 * Central Repository file of the key: KCRUidProfilesLV.
 *
 * Integer value.
 *
 * @since 3.0
 *
 * Default values:
 * S60 3.1: 1 (KProEngFeatureIdVTRingingTone enabled)
 * S60 3.2: 1
 * S60 4.0: 1
 */
const TUint32 KProfilesLVFlags             = 0x00000000;

/**
 * Standard bitmask type of key used for local variation of Profiles Tones View
 * (in General Settings). One bit is used to control one feature. If the bit is
 * on (= 1) the feature is enabled and if the bit is off the feature is off.
 * The meanings for each bit used are defined in the end of this file.
 *
 * Central Repository file of the key: KCRUidProfilesLV.
 *
 * Integer value.
 *
 * @since 3.1
 *
 * Default values:
 * S60 3.1: 0 (KTonesViewFeatureIdHac disabled)
 * S60 3.2: 1 (KTonesViewFeatureIdHac enabled)
 */
const TUint32 KProEngTonesViewLVFlags = 0x00000001;

/**
 * This key defines the set of MIME-types that are not allowed as alert tones.
 * When the user is trying to set an alert tone, this list must be checked
 * first before the following 2 lists and act accordingly. Trying to set a tone
 * of a type given in this list as alert tone should result in an error note: 
 * "Not allowed" or similar.
 *
 * Central Repository file of the key: KCRUidProfilesLV.
 *
 * String value.
 *
 * Possible values: space separated list of MIME-types to be blocked.
 *
 * Default value:""
 */
const TUint32 KProEngBlockedTones = 0x00000002;

/**
 * This key defines the set of MIME-types that are not allowed as alert tones
 * if the tone file in question is NOT DRM protected. Trying to set a tone of
 * a type given in this list as alert tone should result in an error note:
 * "Unprotected file. Cannot be selected as ringing tone." or similar.
 * Note, if a MIME-type is already included in KProEngBlockedTones, it should
 * not have to be included in this list.
 *
 * Central Repository file of the key: KCRUidProfilesLV.
 *
 * String value.
 *
 * Possible values: space separated list of MIME-types to be blocked.
 *
 * Default value:""
 */
const TUint32 KProEngBlockedUnprotectedTones = 0x00000003;

/**
 * This key defines the set of MIME-types that are not allowed as alert tones
 * if the tone file in question IS DRM protected. Trying to set a tone of a
 * type given in this list as alert tone should result in an error note:
 * "Protected file. Cannot be selected as ringing tone." or similar.
 * Note, if a MIME-type is already included in KProEngBlockedTones, it should
 * not have to be included in this list.
 *
 * Central Repository file of the key: KCRUidProfilesLV.
 *
 * String value.
 *
 * Possible values: space separated list of MIME-types to be blocked.
 *
 * Default value:""
 */
const TUint32 KProEngBlockedProtectedTones = 0x00000004;

/**
 * This key defines the set of MIME-types that are not allowed to be demo-played
 * in Tone Selection dialog.
 *
 * Central Repository file of the key: KCRUidProfilesLV.
 *
 * String value.
 *
 * Possible values: space separated list of MIME-types to be blocked. If the
 * list is empty, the demo-play of tones are not blocked at all (if the MIME
 * type is otherwise supported by MMF).
 */
const TUint32 KProEngBlockedDemoPlayTones = 0x00000005;

/**
 * This key defines the set of MIME-types that are to be excluded from the list
 * of tones in Tone Selection dialog. If the attempt to set the tone is carried
 * out by other means than using Tone Selection dialog, the behaviour will be
 * the same as with the tones in KProEngBlockedTones list.
 *
 * Central Repository file of the key: KCRUidProfilesLV.
 *
 * String value.
 *
 * Possible values: space separated list of MIME-types to be excluded/blocked.
 * If the list is empty, the demo-play of tones are not excluded at all (if the
 * MIME type is otherwise supported by MMF).
 */
const TUint32 KProEngExcludedTones = 0x00000006;


//
// Bit definitions for KProEngTonesViewLVFlags
//

/**
 * This bit indicates whether HAC hearing aid compatibility support for 
 * size architecture feature is enabled.
 * @since 3.1
 */
const TUint32 KTonesViewFeatureIdHac = 0x00000001;

#endif      // PROFILEENGINEDOMAINCRKEYS_H

// End of File
