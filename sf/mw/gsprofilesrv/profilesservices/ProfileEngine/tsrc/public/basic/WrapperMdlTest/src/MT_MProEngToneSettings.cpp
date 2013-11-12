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
* Description:  Implementation of the MT_MProEngToneSettings test class.
*
*/

////////////////////////////////////////////////////////////////////////
// MT_MProEngToneSettings.cpp
//

//  CLASS HEADER
#include "MT_MProEngToneSettings.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <MProEngToneSettings.h>
#include <MProEngProfile.h>
#include <MProEngEngine.h>
#include "MProfileEngineExtended.h"
#include "MProfile.h"
#include "MProfileTones.h"
#include "TProfileToneSettings.h"
#include "MProfileExtraTones.h"
#include "MProfileExtended.h"
#include <ProEngFactory.h>
#include <Profile.hrh>

//  INTERNAL INCLUDES
#include "T_ProfileTestTools.h"

MT_MProEngToneSettings* MT_MProEngToneSettings::NewL()
    {
    MT_MProEngToneSettings* self = MT_MProEngToneSettings::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

MT_MProEngToneSettings* MT_MProEngToneSettings::NewLC()
    {
    MT_MProEngToneSettings* self = new( ELeave ) MT_MProEngToneSettings();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }


MT_MProEngToneSettings::~MT_MProEngToneSettings()
    {
    if( iInternalEngine )
        {
        iInternalEngine->Release();
        iInternalEngine = NULL;
        }
    if( iEngine )
        {
        iEngine->Release();
        iEngine = NULL;
        }
    }


MT_MProEngToneSettings::MT_MProEngToneSettings()
    {
    }

void MT_MProEngToneSettings::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    iInternalEngine = CreateProfileEngineExtendedL();
    iEngine = ProEngFactory::NewEngineL();
    }


void MT_MProEngToneSettings::SetupL()
    {
    iInternalProfile = iInternalEngine->ProfileL( iProfileId );
    iProfile = iEngine->ProfileL( iProfileId );
    }

void MT_MProEngToneSettings::Setup0L()
    {
    iProfileId = 0;
    SetupL();
    }

void MT_MProEngToneSettings::Setup1L()
    {
    iProfileId = 1;
    SetupL();
    }

void MT_MProEngToneSettings::Setup2L()
    {
    iProfileId = 2;
    SetupL();
    }

void MT_MProEngToneSettings::Setup3L()
    {
    iProfileId = 3;
    SetupL();
    }

void MT_MProEngToneSettings::Setup4L()
    {
    iProfileId = 4;
    SetupL();
    }

#ifdef __OFFLINE_MODE
void MT_MProEngToneSettings::Setup5L()
    {
    iProfileId = 5;
    SetupL();
    }

#endif // __OFFLINE_MODE
#ifdef __DRIVE_MODE
void MT_MProEngToneSettings::Setup6L()
    {
    iProfileId = 6;
    SetupL();
    }

#endif // __DRIVE_MODE


void MT_MProEngToneSettings::Teardown()
    {
    if( iInternalProfile )
        {
        iInternalProfile->Release();
        iInternalProfile = NULL;
        }
    if( iProfile )
        {
        iProfile->Release();
        iProfile = NULL;
        }
    }

void MT_MProEngToneSettings::GetRingingTypeTestL()
    {
    MProEngToneSettings& tones( iProfile->ToneSettings() );
    const TProfileToneSettings& tones2( iInternalProfile->ProfileTones().ToneSettings() );
    EUNIT_ASSERT( tones.RingingType() == tones2.iRingingType );
    }

void MT_MProEngToneSettings::GetKeypadVolumeTestL()
    {
    MProEngToneSettings& tones( iProfile->ToneSettings() );
    const TProfileToneSettings& tones2( iInternalProfile->ProfileTones().ToneSettings() );
    EUNIT_ASSERT( tones.KeypadVolume() == tones2.iKeypadVolume );
    }

void MT_MProEngToneSettings::GetRingingVolumeTestL()
    {
    MProEngToneSettings& tones( iProfile->ToneSettings() );
    const TProfileToneSettings& tones2( iInternalProfile->ProfileTones().ToneSettings() );
    EUNIT_ASSERT( tones.RingingVolume() == tones2.iRingingVolume );
    }

void MT_MProEngToneSettings::GetVibraTestL()
    {
    MProEngToneSettings& tones( iProfile->ToneSettings() );
    const TProfileToneSettings& tones2( iInternalProfile->ProfileTones().ToneSettings() );
    EUNIT_ASSERT( tones.VibratingAlert() == tones2.iVibratingAlert );
    }

void MT_MProEngToneSettings::GetWarningAndGameTonesTestL()
    {
    MProEngToneSettings& tones( iProfile->ToneSettings() );
    const TProfileToneSettings& tones2( iInternalProfile->ProfileTones().ToneSettings() );
    EUNIT_ASSERT( tones.WarningAndGameTones() == tones2.iWarningAndGameTones );
    }

void MT_MProEngToneSettings::GetTextToSpeechTestL()
    {
    MProEngToneSettings& tones( iProfile->ToneSettings() );
    const TProfileToneSettings& tones2( iInternalProfile->ProfileTones().ToneSettings() );
    EUNIT_ASSERT( tones.TextToSpeech() == tones2.iTextToSpeech );
    }

void MT_MProEngToneSettings::SetRingingTypeTestL()
    {
    MProEngToneSettings& tones( iProfile->ToneSettings() );
    TProfileRingingType origValue( tones.RingingType() );
    TInt err( tones.SetRingingType( KProEngMTTestRingingType ) );
    if( ( iProfileId != EProfileOffLineId ) &&
        ( iProfileId != EProfileDriveId ) )
        {
        EUNIT_ASSERT( err == KErrNone );
        EUNIT_ASSERT( tones.RingingType() == KProEngMTTestRingingType );
        }
    else
        {
        EUNIT_ASSERT( err == KErrAccessDenied );
        EUNIT_ASSERT( tones.RingingType() == origValue );
        }
    }

void MT_MProEngToneSettings::SetKeypadVolumeTestL()
    {
    MProEngToneSettings& tones( iProfile->ToneSettings() );
    TProfileKeypadVolume origValue( tones.KeypadVolume() );
    TInt err( tones.SetKeypadVolume( KProEngMTTestKeypadVolume ) );
    if( iProfileId != EProfileDriveId )
        {
        EUNIT_ASSERT( err == KErrNone );
        EUNIT_ASSERT( tones.KeypadVolume() == KProEngMTTestKeypadVolume );
        }
    else
        {
        EUNIT_ASSERT( err == KErrAccessDenied );
        EUNIT_ASSERT( tones.KeypadVolume() == origValue );
        }
    }

void MT_MProEngToneSettings::SetRingingVolumeTestL()
    {
    MProEngToneSettings& tones( iProfile->ToneSettings() );
    TProfileRingingVolume origValue( tones.RingingVolume() );
    TInt err( tones.SetRingingVolume( KProEngMTTestRingingVolume ) );
    if( iProfileId != EProfileDriveId )
        {
        EUNIT_ASSERT( err == KErrNone );
        EUNIT_ASSERT( tones.RingingVolume() == KProEngMTTestRingingVolume );
        }
    else
        {
        EUNIT_ASSERT( err == KErrAccessDenied );
        EUNIT_ASSERT( tones.RingingVolume() == origValue );
        }
    }

void MT_MProEngToneSettings::SetVibraTestL()
    {
    MProEngToneSettings& tones( iProfile->ToneSettings() );
    TBool origValue( tones.VibratingAlert() );
    TInt err( tones.SetVibratingAlert( !origValue ) );
    if( iProfileId != EProfileDriveId )
        {
        EUNIT_ASSERT( err == KErrNone );
        EUNIT_ASSERT( tones.VibratingAlert() != origValue );
        }
    else
        {
        EUNIT_ASSERT( err == KErrAccessDenied );
        EUNIT_ASSERT( tones.VibratingAlert() == origValue );
        }
    }

void MT_MProEngToneSettings::SetWarningAndGameTonesTestL()
    {
    MProEngToneSettings& tones( iProfile->ToneSettings() );
    TBool origValue( tones.WarningAndGameTones() );
    TInt err( tones.SetWarningAndGameTones( !origValue ) );
    if( iProfileId != EProfileDriveId )
        {
        EUNIT_ASSERT( err == KErrNone );
        EUNIT_ASSERT( tones.WarningAndGameTones() != origValue );
        }
    else
        {
        EUNIT_ASSERT( err == KErrAccessDenied );
        EUNIT_ASSERT( tones.WarningAndGameTones() == origValue );
        }
    }

void MT_MProEngToneSettings::SetTextToSpeechTestL()
    {
    MProEngToneSettings& tones( iProfile->ToneSettings() );
    TBool origValue( tones.TextToSpeech() );
    TInt err( tones.SetTextToSpeech( !origValue ) );
    if( ( iProfileId != EProfileOffLineId ) &&
        ( iProfileId != EProfileDriveId ) )
        {
        EUNIT_ASSERT( err == KErrNone );
        EUNIT_ASSERT( tones.TextToSpeech() != origValue );
        }
    else
        {
        EUNIT_ASSERT( err == KErrAccessDenied );
        EUNIT_ASSERT( tones.TextToSpeech() == origValue );
        }
    }



EUNIT_BEGIN_TEST_TABLE( 
    MT_MProEngToneSettings,
    "Profile Tone Settings Wrapper Test cases.",
    "MODULE" )

EUNIT_TEST(
    "Get Ringing type 0 - test",
    "MProEngToneSettings",
    "RingingType",
    "FUNCTIONALITY",
    Setup0L,
    GetRingingTypeTestL,
    Teardown)

EUNIT_TEST(
    "Get Keypad volume 0 - test",
    "MProEngToneSettings",
    "KeypadVolume",
    "FUNCTIONALITY",
    Setup0L,
    GetKeypadVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Get Ringing Volume 0 - test",
    "MProEngToneSettings",
    "RingingVolume",
    "FUNCTIONALITY",
    Setup0L,
    GetRingingVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Get Vibrating alert 0 - test",
    "MProEngToneSettings",
    "VibratingAlert",
    "FUNCTIONALITY",
    Setup0L,
    GetVibraTestL,
    Teardown)

EUNIT_TEST(
    "Get Warning And Game tones 0 - test",
    "MProEngToneSettings",
    "WarningAndGameTones",
    "FUNCTIONALITY",
    Setup0L,
    GetWarningAndGameTonesTestL,
    Teardown)

EUNIT_TEST(
    "Get Text-to-Speech tone 0 - test",
    "MProEngToneSettings",
    "TextToSpeech",
    "FUNCTIONALITY",
    Setup0L,
    GetTextToSpeechTestL,
    Teardown)

EUNIT_TEST(
    "Set Ringing type 0 - test",
    "MProEngToneSettings",
    "SetRingingType",
    "FUNCTIONALITY",
    Setup0L,
    SetRingingTypeTestL,
    Teardown)

EUNIT_TEST(
    "Set Keypad volume 0 - test",
    "MProEngToneSettings",
    "SetKeypadVolume",
    "FUNCTIONALITY",
    Setup0L,
    SetKeypadVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Set Ringing Volume 0 - test",
    "MProEngToneSettings",
    "SetRingingVolume",
    "FUNCTIONALITY",
    Setup0L,
    SetRingingVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Set Vibrating alert 0 - test",
    "MProEngToneSettings",
    "SetVibratingAlert",
    "FUNCTIONALITY",
    Setup0L,
    SetVibraTestL,
    Teardown)

EUNIT_TEST(
    "Set Warning And Game tones 0 - test",
    "MProEngToneSettings",
    "SetWarningAndGameTones",
    "FUNCTIONALITY",
    Setup0L,
    SetWarningAndGameTonesTestL,
    Teardown)

EUNIT_TEST(
    "Set Text-to-Speech tone 0 - test",
    "MProEngToneSettings",
    "SetTextToSpeech",
    "FUNCTIONALITY",
    Setup0L,
    SetTextToSpeechTestL,
    Teardown)

EUNIT_TEST(
    "Get Ringing type 1 - test",
    "MProEngToneSettings",
    "RingingType",
    "FUNCTIONALITY",
    Setup1L,
    GetRingingTypeTestL,
    Teardown)

EUNIT_TEST(
    "Get Keypad volume 1 - test",
    "MProEngToneSettings",
    "KeypadVolume",
    "FUNCTIONALITY",
    Setup1L,
    GetKeypadVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Get Ringing Volume 1 - test",
    "MProEngToneSettings",
    "RingingVolume",
    "FUNCTIONALITY",
    Setup1L,
    GetRingingVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Get Vibrating alert 1 - test",
    "MProEngToneSettings",
    "VibratingAlert",
    "FUNCTIONALITY",
    Setup1L,
    GetVibraTestL,
    Teardown)

EUNIT_TEST(
    "Get Warning And Game tones 1 - test",
    "MProEngToneSettings",
    "WarningAndGameTones",
    "FUNCTIONALITY",
    Setup1L,
    GetWarningAndGameTonesTestL,
    Teardown)

EUNIT_TEST(
    "Get Text-to-Speech tone 1 - test",
    "MProEngToneSettings",
    "TextToSpeech",
    "FUNCTIONALITY",
    Setup1L,
    GetTextToSpeechTestL,
    Teardown)

EUNIT_TEST(
    "Set Ringing type 1 - test",
    "MProEngToneSettings",
    "SetRingingType",
    "FUNCTIONALITY",
    Setup1L,
    SetRingingTypeTestL,
    Teardown)

EUNIT_TEST(
    "Set Keypad volume 1 - test",
    "MProEngToneSettings",
    "SetKeypadVolume",
    "FUNCTIONALITY",
    Setup1L,
    SetKeypadVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Set Ringing Volume 1 - test",
    "MProEngToneSettings",
    "SetRingingVolume",
    "FUNCTIONALITY",
    Setup1L,
    SetRingingVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Set Vibrating alert 1 - test",
    "MProEngToneSettings",
    "SetVibratingAlert",
    "FUNCTIONALITY",
    Setup1L,
    SetVibraTestL,
    Teardown)

EUNIT_TEST(
    "Set Warning And Game tones 1 - test",
    "MProEngToneSettings",
    "SetWarningAndGameTones",
    "FUNCTIONALITY",
    Setup1L,
    SetWarningAndGameTonesTestL,
    Teardown)

EUNIT_TEST(
    "Set Text-to-Speech tone 1 - test",
    "MProEngToneSettings",
    "SetTextToSpeech",
    "FUNCTIONALITY",
    Setup1L,
    SetTextToSpeechTestL,
    Teardown)

EUNIT_TEST(
    "Get Ringing type 2 - test",
    "MProEngToneSettings",
    "RingingType",
    "FUNCTIONALITY",
    Setup2L,
    GetRingingTypeTestL,
    Teardown)

EUNIT_TEST(
    "Get Keypad volume 2 - test",
    "MProEngToneSettings",
    "KeypadVolume",
    "FUNCTIONALITY",
    Setup2L,
    GetKeypadVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Get Ringing Volume 2 - test",
    "MProEngToneSettings",
    "RingingVolume",
    "FUNCTIONALITY",
    Setup2L,
    GetRingingVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Get Vibrating alert 2 - test",
    "MProEngToneSettings",
    "VibratingAlert",
    "FUNCTIONALITY",
    Setup2L,
    GetVibraTestL,
    Teardown)

EUNIT_TEST(
    "Get Warning And Game tones 2 - test",
    "MProEngToneSettings",
    "WarningAndGameTones",
    "FUNCTIONALITY",
    Setup2L,
    GetWarningAndGameTonesTestL,
    Teardown)

EUNIT_TEST(
    "Get Text-to-Speech tone 2 - test",
    "MProEngToneSettings",
    "TextToSpeech",
    "FUNCTIONALITY",
    Setup2L,
    GetTextToSpeechTestL,
    Teardown)

EUNIT_TEST(
    "Set Ringing type 2 - test",
    "MProEngToneSettings",
    "SetRingingType",
    "FUNCTIONALITY",
    Setup2L,
    SetRingingTypeTestL,
    Teardown)

EUNIT_TEST(
    "Set Keypad volume 2 - test",
    "MProEngToneSettings",
    "SetKeypadVolume",
    "FUNCTIONALITY",
    Setup2L,
    SetKeypadVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Set Ringing Volume 2 - test",
    "MProEngToneSettings",
    "SetRingingVolume",
    "FUNCTIONALITY",
    Setup2L,
    SetRingingVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Set Vibrating alert 2 - test",
    "MProEngToneSettings",
    "SetVibratingAlert",
    "FUNCTIONALITY",
    Setup2L,
    SetVibraTestL,
    Teardown)

EUNIT_TEST(
    "Set Warning And Game tones 2 - test",
    "MProEngToneSettings",
    "SetWarningAndGameTones",
    "FUNCTIONALITY",
    Setup2L,
    SetWarningAndGameTonesTestL,
    Teardown)

EUNIT_TEST(
    "Set Text-to-Speech tone 2 - test",
    "MProEngToneSettings",
    "SetTextToSpeech",
    "FUNCTIONALITY",
    Setup2L,
    SetTextToSpeechTestL,
    Teardown)

EUNIT_TEST(
    "Get Ringing type 3 - test",
    "MProEngToneSettings",
    "RingingType",
    "FUNCTIONALITY",
    Setup3L,
    GetRingingTypeTestL,
    Teardown)

EUNIT_TEST(
    "Get Keypad volume 3 - test",
    "MProEngToneSettings",
    "KeypadVolume",
    "FUNCTIONALITY",
    Setup3L,
    GetKeypadVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Get Ringing Volume 3 - test",
    "MProEngToneSettings",
    "RingingVolume",
    "FUNCTIONALITY",
    Setup3L,
    GetRingingVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Get Vibrating alert 3 - test",
    "MProEngToneSettings",
    "VibratingAlert",
    "FUNCTIONALITY",
    Setup3L,
    GetVibraTestL,
    Teardown)

EUNIT_TEST(
    "Get Warning And Game tones 3 - test",
    "MProEngToneSettings",
    "WarningAndGameTones",
    "FUNCTIONALITY",
    Setup3L,
    GetWarningAndGameTonesTestL,
    Teardown)

EUNIT_TEST(
    "Get Text-to-Speech tone 3 - test",
    "MProEngToneSettings",
    "TextToSpeech",
    "FUNCTIONALITY",
    Setup3L,
    GetTextToSpeechTestL,
    Teardown)

EUNIT_TEST(
    "Set Ringing type 3 - test",
    "MProEngToneSettings",
    "SetRingingType",
    "FUNCTIONALITY",
    Setup3L,
    SetRingingTypeTestL,
    Teardown)

EUNIT_TEST(
    "Set Keypad volume 3 - test",
    "MProEngToneSettings",
    "SetKeypadVolume",
    "FUNCTIONALITY",
    Setup3L,
    SetKeypadVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Set Ringing Volume 3 - test",
    "MProEngToneSettings",
    "SetRingingVolume",
    "FUNCTIONALITY",
    Setup3L,
    SetRingingVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Set Vibrating alert 3 - test",
    "MProEngToneSettings",
    "SetVibratingAlert",
    "FUNCTIONALITY",
    Setup3L,
    SetVibraTestL,
    Teardown)

EUNIT_TEST(
    "Set Warning And Game tones 3 - test",
    "MProEngToneSettings",
    "SetWarningAndGameTones",
    "FUNCTIONALITY",
    Setup3L,
    SetWarningAndGameTonesTestL,
    Teardown)

EUNIT_TEST(
    "Set Text-to-Speech tone 3 - test",
    "MProEngToneSettings",
    "SetTextToSpeech",
    "FUNCTIONALITY",
    Setup3L,
    SetTextToSpeechTestL,
    Teardown)

EUNIT_TEST(
    "Get Ringing type 4 - test",
    "MProEngToneSettings",
    "RingingType",
    "FUNCTIONALITY",
    Setup4L,
    GetRingingTypeTestL,
    Teardown)

EUNIT_TEST(
    "Get Keypad volume 4 - test",
    "MProEngToneSettings",
    "KeypadVolume",
    "FUNCTIONALITY",
    Setup4L,
    GetKeypadVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Get Ringing Volume 4 - test",
    "MProEngToneSettings",
    "RingingVolume",
    "FUNCTIONALITY",
    Setup4L,
    GetRingingVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Get Vibrating alert 4 - test",
    "MProEngToneSettings",
    "VibratingAlert",
    "FUNCTIONALITY",
    Setup4L,
    GetVibraTestL,
    Teardown)

EUNIT_TEST(
    "Get Warning And Game tones 4 - test",
    "MProEngToneSettings",
    "WarningAndGameTones",
    "FUNCTIONALITY",
    Setup4L,
    GetWarningAndGameTonesTestL,
    Teardown)

EUNIT_TEST(
    "Get Text-to-Speech tone 4 - test",
    "MProEngToneSettings",
    "TextToSpeech",
    "FUNCTIONALITY",
    Setup4L,
    GetTextToSpeechTestL,
    Teardown)

EUNIT_TEST(
    "Set Ringing type 4 - test",
    "MProEngToneSettings",
    "SetRingingType",
    "FUNCTIONALITY",
    Setup4L,
    SetRingingTypeTestL,
    Teardown)

EUNIT_TEST(
    "Set Keypad volume 4 - test",
    "MProEngToneSettings",
    "SetKeypadVolume",
    "FUNCTIONALITY",
    Setup4L,
    SetKeypadVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Set Ringing Volume 4 - test",
    "MProEngToneSettings",
    "SetRingingVolume",
    "FUNCTIONALITY",
    Setup4L,
    SetRingingVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Set Vibrating alert 4 - test",
    "MProEngToneSettings",
    "SetVibratingAlert",
    "FUNCTIONALITY",
    Setup4L,
    SetVibraTestL,
    Teardown)

EUNIT_TEST(
    "Set Warning And Game tones 4 - test",
    "MProEngToneSettings",
    "SetWarningAndGameTones",
    "FUNCTIONALITY",
    Setup4L,
    SetWarningAndGameTonesTestL,
    Teardown)

EUNIT_TEST(
    "Set Text-to-Speech tone 4 - test",
    "MProEngToneSettings",
    "SetTextToSpeech",
    "FUNCTIONALITY",
    Setup4L,
    SetTextToSpeechTestL,
    Teardown)

#ifdef __OFFLINE_MODE
EUNIT_TEST(
    "Get Ringing type 5 - test",
    "MProEngToneSettings",
    "RingingType",
    "FUNCTIONALITY",
    Setup5L,
    GetRingingTypeTestL,
    Teardown)

EUNIT_TEST(
    "Get Keypad volume 5 - test",
    "MProEngToneSettings",
    "KeypadVolume",
    "FUNCTIONALITY",
    Setup5L,
    GetKeypadVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Get Ringing Volume 5 - test",
    "MProEngToneSettings",
    "RingingVolume",
    "FUNCTIONALITY",
    Setup5L,
    GetRingingVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Get Vibrating alert 5 - test",
    "MProEngToneSettings",
    "VibratingAlert",
    "FUNCTIONALITY",
    Setup5L,
    GetVibraTestL,
    Teardown)

EUNIT_TEST(
    "Get Warning And Game tones 5 - test",
    "MProEngToneSettings",
    "WarningAndGameTones",
    "FUNCTIONALITY",
    Setup5L,
    GetWarningAndGameTonesTestL,
    Teardown)

EUNIT_TEST(
    "Get Text-to-Speech tone 5 - test",
    "MProEngToneSettings",
    "TextToSpeech",
    "FUNCTIONALITY",
    Setup5L,
    GetTextToSpeechTestL,
    Teardown)

EUNIT_TEST(
    "Set Ringing type 5 - test",
    "MProEngToneSettings",
    "SetRingingType",
    "FUNCTIONALITY",
    Setup5L,
    SetRingingTypeTestL,
    Teardown)

EUNIT_TEST(
    "Set Keypad volume 5 - test",
    "MProEngToneSettings",
    "SetKeypadVolume",
    "FUNCTIONALITY",
    Setup5L,
    SetKeypadVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Set Ringing Volume 5 - test",
    "MProEngToneSettings",
    "SetRingingVolume",
    "FUNCTIONALITY",
    Setup5L,
    SetRingingVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Set Vibrating alert 5 - test",
    "MProEngToneSettings",
    "SetVibratingAlert",
    "FUNCTIONALITY",
    Setup5L,
    SetVibraTestL,
    Teardown)

EUNIT_TEST(
    "Set Warning And Game tones 5 - test",
    "MProEngToneSettings",
    "SetWarningAndGameTones",
    "FUNCTIONALITY",
    Setup5L,
    SetWarningAndGameTonesTestL,
    Teardown)

EUNIT_TEST(
    "Set Text-to-Speech tone 5 - test",
    "MProEngToneSettings",
    "SetTextToSpeech",
    "FUNCTIONALITY",
    Setup5L,
    SetTextToSpeechTestL,
    Teardown)

#endif // __OFFLINE_MODE

#ifdef __DRIVE_MODE
EUNIT_TEST(
    "Get Ringing type 6 - test",
    "MProEngToneSettings",
    "RingingType",
    "FUNCTIONALITY",
    Setup6L,
    GetRingingTypeTestL,
    Teardown)

EUNIT_TEST(
    "Get Keypad volume 6 - test",
    "MProEngToneSettings",
    "KeypadVolume",
    "FUNCTIONALITY",
    Setup6L,
    GetKeypadVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Get Ringing Volume 6 - test",
    "MProEngToneSettings",
    "RingingVolume",
    "FUNCTIONALITY",
    Setup6L,
    GetRingingVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Get Vibrating alert 6 - test",
    "MProEngToneSettings",
    "VibratingAlert",
    "FUNCTIONALITY",
    Setup6L,
    GetVibraTestL,
    Teardown)

EUNIT_TEST(
    "Get Warning And Game tones 6 - test",
    "MProEngToneSettings",
    "WarningAndGameTones",
    "FUNCTIONALITY",
    Setup6L,
    GetWarningAndGameTonesTestL,
    Teardown)

EUNIT_TEST(
    "Get Text-to-Speech tone 6 - test",
    "MProEngToneSettings",
    "TextToSpeech",
    "FUNCTIONALITY",
    Setup6L,
    GetTextToSpeechTestL,
    Teardown)

EUNIT_TEST(
    "Set Ringing type 6 - test",
    "MProEngToneSettings",
    "SetRingingType",
    "FUNCTIONALITY",
    Setup6L,
    SetRingingTypeTestL,
    Teardown)

EUNIT_TEST(
    "Set Keypad volume 6 - test",
    "MProEngToneSettings",
    "SetKeypadVolume",
    "FUNCTIONALITY",
    Setup6L,
    SetKeypadVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Set Ringing Volume 6 - test",
    "MProEngToneSettings",
    "SetRingingVolume",
    "FUNCTIONALITY",
    Setup6L,
    SetRingingVolumeTestL,
    Teardown)

EUNIT_TEST(
    "Set Vibrating alert 6 - test",
    "MProEngToneSettings",
    "SetVibratingAlert",
    "FUNCTIONALITY",
    Setup6L,
    SetVibraTestL,
    Teardown)

EUNIT_TEST(
    "Set Warning And Game tones 6 - test",
    "MProEngToneSettings",
    "SetWarningAndGameTones",
    "FUNCTIONALITY",
    Setup6L,
    SetWarningAndGameTonesTestL,
    Teardown)

EUNIT_TEST(
    "Set Text-to-Speech tone 6 - test",
    "MProEngToneSettings",
    "SetTextToSpeech",
    "FUNCTIONALITY",
    Setup6L,
    SetTextToSpeechTestL,
    Teardown)

#endif // __DRIVE_MODE

EUNIT_END_TEST_TABLE



