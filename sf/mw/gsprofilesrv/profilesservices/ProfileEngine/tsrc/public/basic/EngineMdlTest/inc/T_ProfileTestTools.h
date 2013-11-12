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
* Description:  Tools class used in ProfileEngine tests.
*
*/

////////////////////////////////////////////////////////////////////////
// T_ProfileTestTools.h
//

// INCLUDES
#include <e32std.h>
#include <CNTDEF.H>

#include <MProfileChangeObserver.h>
//CONSTS

//#define PROFILE_TEST_RUN_ALLOC


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

_LIT( KProfileGeneralName2, "General2" );
_LIT( KProfileSilentName2,  "Silent2" );
_LIT( KProfileMeetingName2, "Meeting2" );
_LIT( KProfileOutdoorName2, "Outdoor2" );
_LIT( KProfilePagerName2,   "Pager2" );
#ifdef __OFFLINE_MODE
_LIT( KProfileOfflineName2,   "Off-line2" );
#endif
#ifdef __DRIVE_MODE
_LIT( KProfileDriveName2,   "Drive2" );
#endif

// default tone settings
_LIT( KProfileNokiaTune, "z:\\data\\sounds\\digital\\nokia tune.aac" );
_LIT( KProfileMessageTone, "z:\\data\\sounds\\digital\\message 1.aac" );
_LIT( KProfileIMMessageTone, "z:\\data\\sounds\\digital\\Chat alert.aac" );
_LIT( KProfileBeep, "z:\\data\\sounds\\simple\\beep.rng" );
_LIT( KProfileBeepTwice, "z:\\data\\sounds\\simple\\beep twice.rng" );
_LIT( KProfileNoSound, "z:\\resource\\no_sound.wav" );

// default presence settings
//_LIT( KProfileStatusMessageAvailable, "Available" );
//_LIT( KProfileStatusMessageDiscreet, "Discreet" );
//_LIT( KProfileStatusMessageNotAvailable, "Not available" );
_LIT( KProfileStatusMessageAvailable, "" );
_LIT( KProfileStatusMessageDiscreet, "" );
_LIT( KProfileStatusMessageNotAvailable, "" );

_LIT( KProfileEmpty, "" );
_LIT( KProfileMinName, "Z" );
_LIT( KProfileMaxName, "1234567890123456789012345678901234567890123456789012345678901234" ); // 64
//_LIT( KProfileTooLongProfileName, "1234567890123456789012345678901234567890123456789012345678901234567890" ); // 70
_LIT( KProfileMaxFileName, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456" ); // 256
_LIT( KProfileTooLongFileName, "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567" ); // 257
_LIT( KProfileMaxStatusMessage, "1234567890123456789012345678901234567890" );   // 40

const TInt KMaxNumberOfDynamicProfiles( 20 );


// FORWARD DECLARATION
class RFs;
class MProfilesNamesArray;
struct TProfileToneSettings;
class CPbkContactEngine;

struct TSharedDataSettings
    {
    TInt iRingType;
    TInt iRingVol;
    TInt iVibrAlert;
    TInt iKeyVolume;
    TInt iWarnTone;
    TInt iMsgAlert;
    TBuf<64> iRingTone1;
    TBuf<64> iRingTone2;
    TBuf<64> iMsgTone;
    TBuf<16> iName;
    TInt iActive;
    };

/**
* Test observer
*/
class CTestObserver: public CBase, public MProfileChangeObserver
    {
    public:
        static CTestObserver* NewL()
                    {CTestObserver* self = new (ELeave) CTestObserver; return self;}
        ~CTestObserver()
                    {};
        void HandleActiveProfileEventL( TProfileEvent aProfileEvent, TInt aProfileId )
                    {
                    iLastEvent = aProfileEvent;
                    iLastId = aProfileId;
                    CActiveScheduler::Stop();
                    }
        TProfileEvent iLastEvent;
        TInt iLastId;
    };

/**
 * Test Tools
 */
class T_ProfileTestTools
    {
    public:
        static void ResetRepository();
        static void RemoveFilesL( RFs& aFs );
        static TInt DefaultFiles( RFs& aFs );
        static TInt CorruptFiles( RFs& aFs );
        static TInt MakeBackupFiles( RFs& aFs );
        static TBool CompareArrays( const TArray<TContactItemId>& aArray1, const TArray<TContactItemId>& aArray2 );
        static TBool CheckArray( const MProfilesNamesArray& aArray );
        static TBool CheckModifiedArray( MProfilesNamesArray& aArray );
        static const TDesC& ProfileShortName( TInt aId );
        static const TDesC& ProfileName( TInt aId );
        static const TDesC& ProfileModifiedName( TInt aId );
        static const TDesC& ProfileRingingTone( TInt aId );
        static const TDesC& ProfileMessageAlertTone( TInt aId );
        static const TDesC& ProfileIMMessageAlertTone( TInt aId );
        static const TDesC& ProfileEmailAlertTone( TInt aId );
        static const TDesC& ProfileVideoCallRingingTone( TInt aId );
        static const TDesC& ProfileStatusMessage( TInt aId );
        static TBool CheckToneSettings( TInt aId, const TProfileToneSettings&  aSettings );
        static void CreateContactGroupsL( TInt aNumber, RArray<TContactItemId>& aArray, CPbkContactEngine& aContactEngine );
        static void DeleteContactGroups( RArray<TContactItemId>& aArray, CPbkContactEngine& aContactEngine );
        static void CreateContactsL( TInt aNumber, RArray<TContactItemId>& aArray, CPbkContactEngine& aContactEngine );
        static void DeleteContacts( RArray<TContactItemId>& aArray, CPbkContactEngine& aContactEngine );
        static void WriteAlertForIdsToCenrepL( TInt aProfileId, RArray<TContactItemId>& aArray );
        static void DeleteAlertForIdsFromCenrep( TInt aProfileId );
        static TBool CheckSharedDataSettingsL( TInt aId );
        static TBool CheckSharedDataSettingsL( const TSharedDataSettings& aSettings );
        static TBool CompareSettings( const TSharedDataSettings& aSettings, const TSharedDataSettings& aSettings1 );
        static TInt ProfilePttStatus( TInt aProfileId );
        static const TDesC& ProfilePttRingingTone( TInt aProfileId );
    };
