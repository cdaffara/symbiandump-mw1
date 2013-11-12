/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Test Tools.
*
*/
////////////////////////////////////////////////////////////////////////
// T_ProfileTestTools.h
//

// INCLUDES
#include <e32std.h>
#include <CNTDEF.H>
#include <Profile.hrh>

//CONSTS

// default profile names
_LIT( KProfileGeneralName, "General" );
_LIT( KProfileSilentName,  "Silent" );
_LIT( KProfileMeetingName, "Meeting" );
_LIT( KProfileOutdoorName, "Outdoor" );
_LIT( KProfilePagerName,   "Pager" );
#ifdef __OFFLINE_MODE
_LIT( KProfileOfflineName,   "Off-line" );
#endif
#ifdef __DRIVE_MODE
_LIT( KProfileDriveName,   "Drive" );
#endif
_LIT( KProfileInvalidName,   "Invalid" );
_LIT( KProfileDynamicName,   "New profile" );

// default tone settings
_LIT( KProfileNokiaTune, "z:\\data\\sounds\\digital\\nokia tune.aac" );
_LIT( KProfileMessageTone, "z:\\data\\sounds\\digital\\message 1.aac" );
_LIT( KProfileIMMessageTone, "z:\\data\\sounds\\digital\\Chat alert.aac" );
_LIT( KProfileBeep, "z:\\data\\sounds\\simple\\beep.rng" );
_LIT( KProfileBeepTwice, "z:\\data\\sounds\\simple\\beep twice.rng" );
_LIT( KProfileNoSound, "z:\\resource\\no_sound.wav" );

// Tone size limit testing uses two different sized files.
// They should be selected according to currently used limit. Initially 600kB.
_LIT( KProEngMTTestToneSizeOk,     "C:\\data\\sounds\\digital\\mt_below_limit.mp3" );
_LIT( KProEngMTTestToneSizeTooBig, "C:\\data\\sounds\\digital\\mt_over_limit.mp3" );

#ifdef __WINS__
_LIT( KProEngMTTestTone, "C:\\Data\\Sounds\\Digital\\flow.aac" );
#else
_LIT( KProEngMTTestTone, "Z:\\Data\\Sounds\\Digital\\flow.aac" );
#endif

#ifdef __WINS__
// combined delivery
_LIT( KProEngMTTestDRM_CD_StartEndTone, "C:\\Data\\encrypted\\bee-boogie_start_end.dm" );
_LIT( KProEngMTTestDRM_CD_5TimesTone, "C:\\Data\\encrypted\\bee-boogie_5times.dm" );
_LIT( KProEngMTTestDRM_CD_FutureTone, "C:\\Data\\encrypted\\bee-boogie_future.dm" );
_LIT( KProEngMTTestDRM_CD_1YearTone, "C:\\Data\\encrypted\\bee-boogie_1year.dm" );
// separate delivery
_LIT( KProEngMTTestDRM_SD_StartEndTone, "C:\\Data\\encrypted\\bee-boogie_sd_start_end.dcf" );
_LIT( KProEngMTTestDRM_SD_5TimesTone, "C:\\Data\\encrypted\\bee-boogie_sd_5times.dcf" );
_LIT( KProEngMTTestDRM_SD_1YearTone, "C:\\Data\\encrypted\\bee-boogie_sd_1year.dcf" );
_LIT( KProEngMTTestDRM_SD_DisplayOnly, "C:\\Data\\encrypted\\bee-boogie_sd_display_only.dcf" );
#else
// Note, that these files must be always downloaded first from
// http://trsrv42.wirelessfuture.com/content/s60/index.wml 
// combined delivery
_LIT( KProEngMTTestDRM_CD_StartEndTone, "C:\\Data\\sounds\\digital\\DDCD_TestTrack2MT.dcf" );
_LIT( KProEngMTTestDRM_CD_5TimesTone, "C:\\Data\\sounds\\digital\\DDCD_11KHz_55b_MP4_LC.dcf" );
_LIT( KProEngMTTestDRM_CD_FutureTone, "C:\\Data\\sounds\\digital\\DDCD_mp3FutureRo.dcf" );
_LIT( KProEngMTTestDRM_CD_1YearTone, "C:\\Data\\sounds\\digital\\DDCD_TestTrack_3_MT.dcf" );
// separate delivery
_LIT( KProEngMTTestDRM_SD_StartEndTone, "C:\\Data\\sounds\\digital\\DDSD_TestTrack2MT.dcf" );
_LIT( KProEngMTTestDRM_SD_5TimesTone, "C:\\Data\\sounds\\digital\\DDSD_11KHz_55b_MP4_LC.dcf" );
_LIT( KProEngMTTestDRM_SD_1YearTone, "C:\\Data\\sounds\\digital\\DDSD_TestTrack_3_MT.dcf" );
_LIT( KProEngMTTestDRM_SD_DisplayOnly, "C:\\Data\\sounds\\digital\\SD_unusable_RO.dcf" );
#endif

_LIT( KProEngMTTestName, "Test Profile" );
const TProfileRingingType KProEngMTTestRingingType( EProfileRingingTypeAscending );
const TProfileKeypadVolume KProEngMTTestKeypadVolume( EProfileKeypadVolumeLevel1 );
const TProfileRingingVolume KProEngMTTestRingingVolume( EProfileRingingVolumeLevel5 );


_LIT( KProfileEmpty, "" );
_LIT( KProfileMinName, "Z" );
_LIT( KProfileMaxName, "1234567890123456789012345678901234567890123456789012345678901234" ); // 64
//_LIT( KProfileTooLongProfileName, "1234567890123456789012345678901234567890123456789012345678901234567890" ); // 70
_LIT( KProfileMaxFileName, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456" ); // 256
_LIT( KProfileTooLongFileName, "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567" ); // 257
_LIT( KProfileMaxStatusMessage, "1234567890123456789012345678901234567890" );   // 40
_LIT( KProfileMaxCallObjectText, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" ); // 1000
_LIT( KProfileTooLongCallObjectText, "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901" ); // 1001

const TInt KMaxNumberOfDynamicProfiles( 20 );


// FORWARD DECLARATION
class RFs;
class MProfile;
class MProEngProfile;
class MProEngProfileNameArray;
class MProfilesNamesArray;
struct TProfileToneSettings;
class CPbkContactEngine;

/**
 * Test Tools
 */
class T_ProfileTestTools
    {
    public:
        static void ResetRepository();
        static TBool CompareArrays( const TArray<TContactItemId>& aArray1, const TArray<TContactItemId>& aArray2 );
        static TBool CompareNameArrays( const MProEngProfileNameArray& aArray1,
                                        const MProfilesNamesArray& aArray2 );
        static TBool CompareProfiles( MProEngProfile& ap1, MProfile& ap2 );
        static TBool CheckArray( const MProfilesNamesArray& aArray );
        static const TDesC& ProfileName( TInt aId );
        static const TDesC& ProfileRingingTone( TInt aId );
        static const TDesC& ProfileMessageAlertTone( TInt aId );
        static const TDesC& ProfileIMMessageAlertTone( TInt aId );
        static const TDesC& ProfileEmailAlertTone( TInt aId );
        static const TDesC& ProfileVideoCallRingingTone( TInt aId );
        static TBool CheckToneSettings( TInt aId, const TProfileToneSettings&  aSettings );
        static void CreateContactGroupsL( TInt aNumber, RArray<TContactItemId>& aArray, CPbkContactEngine& aContactEngine );
        static void DeleteContactGroups( RArray<TContactItemId>& aArray, CPbkContactEngine& aContactEngine );
        static void CreateContactsL( TInt aNumber, RArray<TContactItemId>& aArray, CPbkContactEngine& aContactEngine );
        static void DeleteContacts( RArray<TContactItemId>& aArray, CPbkContactEngine& aContactEngine );
        static void WriteAlertForIdsToCenrepL( TInt aProfileId, RArray<TContactItemId>& aArray );
        static void DeleteAlertForIdsFromCenrep( TInt aProfileId );
        static TBool IsCenrepFeatureEnabledL( TUid aUid, TUint32 aKey );
    };
