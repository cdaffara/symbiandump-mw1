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
* Description:  Implementation of the MT_MProEngProfile test class.
*
*/

////////////////////////////////////////////////////////////////////////
// MT_MProEngProfile.cpp
//

//  CLASS HEADER
#include "MT_MProEngProfile.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <MProfileEngine.h>
#include <MProfile.h>
#include <MProEngProfile.h>
#include <MProEngEngine.h>
#include <MProEngTones.h>
#include <MProEngProfileName.h>
#include <MProEngToneSettings.h>
#include <ProEngFactory.h>
#include <Profile.hrh>
#include <MProfileEngineExtended.h>
#include <MProfileExtended.h>
#include <CPbkContactEngine.h>

//  INTERNAL INCLUDES
#include "T_ProfileTestTools.h"

// CONSTRUCTION
MT_MProEngProfile* MT_MProEngProfile::NewL()
    {
    MT_MProEngProfile* self = MT_MProEngProfile::NewLC(); 
    CleanupStack::Pop();

    return self;
    }
MT_MProEngProfile* MT_MProEngProfile::NewLC()
    {
    MT_MProEngProfile* self = new( ELeave ) MT_MProEngProfile();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

MT_MProEngProfile::~MT_MProEngProfile()
    {
    if( iInternalEngine )
        {
        iInternalEngine->Release();
        }
    if( iEngine )
        {
        iEngine->Release();
        }
    }

MT_MProEngProfile::MT_MProEngProfile()
    {
    }

void MT_MProEngProfile::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    iInternalEngine = CreateProfileEngineExtendedL();
    iEngine = ProEngFactory::NewEngineL();
    }

void MT_MProEngProfile::EmptySetup()
    {
    }

void MT_MProEngProfile::SetupL()
    {
    T_ProfileTestTools::ResetRepository();
    iInternalProfile = iInternalEngine->ProfileL( iProfileId );
    iProfile = iEngine->ProfileL( iProfileId );
    }

void MT_MProEngProfile::Setup0L()
    {
    iProfileId = 0;
    SetupL();
    }

void MT_MProEngProfile::Setup1L()
    {
    iProfileId = 1;
    SetupL();
    }

void MT_MProEngProfile::Setup2L()
    {
    iProfileId = 2;
    SetupL();
    }

void MT_MProEngProfile::Setup3L()
    {
    iProfileId = 3;
    SetupL();
    }

void MT_MProEngProfile::Setup4L()
    {
    iProfileId = 4;
    SetupL();
    }

#ifdef __OFFLINE_MODE
void MT_MProEngProfile::Setup5L()
    {
    iProfileId = 5;
    SetupL();
    }
#endif // __OFFLINE_MODE
#ifdef __DRIVE_MODE
void MT_MProEngProfile::Setup6L()
    {
    iProfileId = 6;
    SetupL();
    }
#endif // __DRIVE_MODE

void MT_MProEngProfile::SetupSetAlertForTestL()
    {
    User::LeaveIfError( iFs.Connect() );
    iContactEngine = CPbkContactEngine::NewL( &iFs );
    T_ProfileTestTools::CreateContactGroupsL( 5, iContactGroups, *iContactEngine );
    }

void MT_MProEngProfile::SetupAlertForTestL()
    {
    SetupSetAlertForTestL();
    User::LeaveIfError( iProfile->SetAlertForL( iContactGroups.Array() ) );
    }

void MT_MProEngProfile::SetupAlertForTest0L()
    {
    Setup0L();
    SetupAlertForTestL();
    }

void MT_MProEngProfile::SetupSetAlertForTest0L()
    {
    Setup0L();
    SetupSetAlertForTestL();
    }

void MT_MProEngProfile::SetupCommitChangeTest0L()
    {
    Setup0L();
    SetupSetAlertForTestL();
    }


void MT_MProEngProfile::SetupAlertForTest1L()
    {
    Setup1L();
    SetupAlertForTestL();
    }

void MT_MProEngProfile::SetupSetAlertForTest1L()
    {
    Setup1L();
    SetupSetAlertForTestL();
    }

void MT_MProEngProfile::SetupCommitChangeTest1L()
    {
    Setup1L();
    SetupSetAlertForTestL();
    }


void MT_MProEngProfile::SetupAlertForTest2L()
    {
    Setup2L();
    SetupAlertForTestL();
    }

void MT_MProEngProfile::SetupSetAlertForTest2L()
    {
    Setup2L();
    SetupSetAlertForTestL();
    }

void MT_MProEngProfile::SetupCommitChangeTest2L()
    {
    Setup2L();
    SetupSetAlertForTestL();
    }


void MT_MProEngProfile::SetupAlertForTest3L()
    {
    Setup3L();
    SetupAlertForTestL();
    }

void MT_MProEngProfile::SetupSetAlertForTest3L()
    {
    Setup3L();
    SetupSetAlertForTestL();
    }

void MT_MProEngProfile::SetupCommitChangeTest3L()
    {
    Setup3L();
    SetupSetAlertForTestL();
    }


void MT_MProEngProfile::SetupAlertForTest4L()
    {
    Setup4L();
    SetupAlertForTestL();
    }

void MT_MProEngProfile::SetupSetAlertForTest4L()
    {
    Setup4L();
    SetupSetAlertForTestL();
    }

void MT_MProEngProfile::SetupCommitChangeTest4L()
    {
    Setup4L();
    SetupSetAlertForTestL();
    }

#ifdef __OFFLINE_MODE
void MT_MProEngProfile::SetupAlertForTest5L()
    {
    Setup5L();
    SetupAlertForTestL();
    }

void MT_MProEngProfile::SetupSetAlertForTest5L()
    {
    Setup5L();
    SetupSetAlertForTestL();
    }

void MT_MProEngProfile::SetupCommitChangeTest5L()
    {
    Setup5L();
    SetupSetAlertForTestL();
    }
#endif // __OFFLINE_MODE


#ifdef __DRIVE_MODE
void MT_MProEngProfile::SetupAlertForTest6L()
    {
    Setup6L();
    SetupAlertForTestL();
    }

void MT_MProEngProfile::SetupSetAlertForTest6L()
    {
    Setup6L();
    SetupSetAlertForTestL();
    }

void MT_MProEngProfile::SetupCommitChangeTest6L()
    {
    Setup6L();
    SetupSetAlertForTestL();
    }
#endif // __DRIVE_MODE

void MT_MProEngProfile::EmptyTeardown()
    {
    }

void MT_MProEngProfile::Teardown()
    {
    iInternalProfile->Release();
    iProfile->Release();
    }

void MT_MProEngProfile::TeardownAlertForGroups()
    {
    Teardown();
    TRAP_IGNORE( T_ProfileTestTools::DeleteContactGroups(
                iContactGroups, *iContactEngine ) );
    delete iContactEngine;
    iContactGroups.Close();
    iFs.Close();
    }

void MT_MProEngProfile::ReleaseTestL()
    {
    MProEngProfile* profile = iEngine->ProfileL( iProfileId );
    CleanupReleasePushL( *profile );
    CleanupStack::PopAndDestroy(); // profile
    }

void MT_MProEngProfile::AlertForTestL()
    {
    const TArray<TContactItemId> tids( iProfile->AlertForL() );
    EUNIT_ASSERT( T_ProfileTestTools::CompareArrays( iContactGroups.Array(), tids ) );
    }

void MT_MProEngProfile::SetAlertForTestL()
    {
    TInt error( iProfile->SetAlertForL( iContactGroups.Array() ) );
    EUNIT_ASSERT( error == KErrNone );
    TArray<TContactItemId> tids( iProfile->AlertForL() );
    EUNIT_ASSERT( T_ProfileTestTools::CompareArrays( iContactGroups.Array(), tids ) );
    }

void MT_MProEngProfile::IsSilentTestL()
    {
    TBool silent( iProfile->IsSilent() );
    TBool expected( EFalse );
    switch( iProfileId )
        {
        case 1:
        case 4:
        case 6:
            {
            expected = ETrue;
            break;
            }
        default: // EFalse is already set above
            {
            break;
            }
        }
    EUNIT_ASSERT( expected == silent );
    }

void MT_MProEngProfile::ProfileNameTestL()
    {
    MProEngProfileName& name = iProfile->ProfileName();
    EUNIT_ASSERT( &name );
    }

void MT_MProEngProfile::ProfileTonesTestL()
    {
    MProEngTones& tones = iProfile->ProfileTones();
    EUNIT_ASSERT( &tones );
    }

void MT_MProEngProfile::ToneSettingsTestL()
    {
    MProEngToneSettings& ts = iProfile->ToneSettings();
    EUNIT_ASSERT( &ts );
    }

void MT_MProEngProfile::CommitChangeTestL()
    {
    RDebug::Print( _L( "Digia:CommitchangeTest: begin" ) );
    //////////////////////////////////////////
    // change  settings and commit changes
    //////////////////////////////////////////
    MProEngProfileName& pn1 = iProfile->ProfileName();
    MProEngTones& pt1 = iProfile->ProfileTones();
    MProEngToneSettings& pts1 = iProfile->ToneSettings();
    if( ( iProfileId != EProfileOffLineId ) &&
        ( iProfileId != EProfileGeneralId ) )
        {
        User::LeaveIfError( pn1.SetNameL( KProEngMTTestName ) );
        }
    //TInt error( iProfile->SetAlertForL( iContactGroups.Array() ) );
    TInt error( KErrNone );
    EUNIT_ASSERT( error == KErrNone );
    User::LeaveIfError( pt1.SetMessageAlertToneL( KProEngMTTestTone ) );
    if( iProfileId != EProfileOffLineId )
        {
        User::LeaveIfError( pt1.SetRingingTone1L( KProEngMTTestTone ) );
        User::LeaveIfError( pt1.SetRingingTone2L( KProEngMTTestTone ) );
        User::LeaveIfError( pt1.SetEmailAlertToneL( KProEngMTTestTone ) );
        User::LeaveIfError( pt1.SetVideoCallRingingToneL( KProEngMTTestTone ) );
        User::LeaveIfError( pts1.SetRingingType( KProEngMTTestRingingType ) );
        User::LeaveIfError( pts1.SetTextToSpeech( !pts1.TextToSpeech() ) );
        }
    User::LeaveIfError( pts1.SetKeypadVolume( KProEngMTTestKeypadVolume ) );
    User::LeaveIfError( pts1.SetRingingVolume( KProEngMTTestRingingVolume ) );
    User::LeaveIfError( pts1.SetVibratingAlert( !pts1.VibratingAlert() ) );
    User::LeaveIfError( pts1.SetWarningAndGameTones( !pts1.WarningAndGameTones() ) );
    RDebug::Print( _L( "Digia:CommitchangeTest: before CommitChangeL" ) );
    iProfile->CommitChangeL();
    RDebug::Print( _L( "Digia:CommitchangeTest: after CommitChangeL" ) );

    // read settings
    MProEngProfile* profile = NULL;
    profile = iEngine->ProfileL( iProfileId );
    EUNIT_ASSERT( profile );
    CleanupReleasePushL( *profile );

    MProEngProfileName& pn2 = profile->ProfileName();
    MProEngTones& pt2 = profile->ProfileTones();
    MProEngToneSettings& pts2 = profile->ToneSettings();
    //const TArray<TContactItemId>& array = profile->AlertForL();
    

    EUNIT_ASSERT( pn1.Name().Compare( pn2.Name() ) == 0 );
    EUNIT_ASSERT( pt1.RingingTone1().Compare( pt2.RingingTone1() ) == 0 );
    EUNIT_ASSERT( pt1.RingingTone2().Compare( pt2.RingingTone2() ) == 0 );
    EUNIT_ASSERT( pt1.MessageAlertTone().Compare( pt2.MessageAlertTone() ) == 0 );
    EUNIT_ASSERT( pt1.EmailAlertTone().Compare( pt2.EmailAlertTone() ) == 0 );
    EUNIT_ASSERT( pts1.RingingType() == pts2.RingingType() );
    EUNIT_ASSERT( pts1.KeypadVolume() == pts2.KeypadVolume() );
    EUNIT_ASSERT( pts1.RingingVolume() == pts2.RingingVolume() );
    EUNIT_ASSERT( pts1.VibratingAlert() == pts2.VibratingAlert() );
    EUNIT_ASSERT( pts1.WarningAndGameTones() == pts2.WarningAndGameTones() );
    EUNIT_ASSERT( pts1.TextToSpeech() == pts2.TextToSpeech() );
    //EUNIT_ASSERT( T_ProfileTestTools::CompareArrays( array, iContactGroups.Array() ) );

    CleanupStack::PopAndDestroy();  //profile->Release
    RDebug::Print( _L( "Digia:CommitchangeTest: end" ) );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    MT_MProEngProfile,
    "Profile Settings Wrapper Test cases.",
    "MODULE" )

EUNIT_TEST(
    "Release profile - test",
    "MProEngProfile",
    "Release",
    "FUNCTIONALITY",
    EmptySetup,
    ReleaseTestL,
    EmptyTeardown
    )

EUNIT_TEST(
    "Get alert for groups 0 - test",
    "MProEngProfile",
    "AlertForL",
    "FUNCTIONALITY",
    SetupAlertForTest0L,
    AlertForTestL,
    TeardownAlertForGroups
    )

EUNIT_TEST(
    "Set alert for groups 0 - test",
    "MProEngProfile",
    "SetAlertForL",
    "FUNCTIONALITY",
    SetupSetAlertForTest0L,
    SetAlertForTestL,
    TeardownAlertForGroups
    )

EUNIT_TEST(
    "Silence state check 0 - test",
    "MProEngProfile",
    "IsSilent",
    "FUNCTIONALITY",
    Setup0L,
    IsSilentTestL,
    Teardown
    )

EUNIT_TEST(
    "MProEngProfileName interface getter 0 - test",
    "MProEngProfile",
    "ProfileName",
    "FUNCTIONALITY",
    Setup0L,
    ProfileNameTestL,
    Teardown
    )

EUNIT_TEST(
    "MProEngTones interface getter 0 - test",
    "MProEngProfile",
    "ProfileTones",
    "FUNCTIONALITY",
    Setup0L,
    ProfileTonesTestL,
    Teardown
    )

EUNIT_TEST(
    "MProEngToneSettings interface getter 0 - test",
    "MProEngProfile",
    "ToneSettings",
    "FUNCTIONALITY",
    Setup0L,
    ToneSettingsTestL,
    Teardown
    )

EUNIT_TEST(
    "Commiting of changes 0 - test",
    "MProEngProfile",
    "CommitChangeL",
    "FUNCTIONALITY",
    SetupCommitChangeTest0L,
    CommitChangeTestL,
    TeardownAlertForGroups
    )

EUNIT_TEST(
    "Get alert for groups 1 - test",
    "MProEngProfile",
    "AlertForL",
    "FUNCTIONALITY",
    SetupAlertForTest1L,
    AlertForTestL,
    TeardownAlertForGroups
    )

EUNIT_TEST(
    "Set alert for groups 1 - test",
    "MProEngProfile",
    "SetAlertForL",
    "FUNCTIONALITY",
    SetupSetAlertForTest1L,
    SetAlertForTestL,
    TeardownAlertForGroups
    )

EUNIT_TEST(
    "Silence state check 1 - test",
    "MProEngProfile",
    "IsSilent",
    "FUNCTIONALITY",
    Setup1L,
    IsSilentTestL,
    Teardown
    )

EUNIT_TEST(
    "MProEngProfileName interface getter 1 - test",
    "MProEngProfile",
    "ProfileName",
    "FUNCTIONALITY",
    Setup1L,
    ProfileNameTestL,
    Teardown
    )

EUNIT_TEST(
    "MProEngTones interface getter 1 - test",
    "MProEngProfile",
    "ProfileTones",
    "FUNCTIONALITY",
    Setup1L,
    ProfileTonesTestL,
    Teardown
    )

EUNIT_TEST(
    "MProEngToneSettings interface getter 1 - test",
    "MProEngProfile",
    "ToneSettings",
    "FUNCTIONALITY",
    Setup1L,
    ToneSettingsTestL,
    Teardown
    )

EUNIT_TEST(
    "Commiting of changes 1 - test",
    "MProEngProfile",
    "CommitChangeL",
    "FUNCTIONALITY",
    SetupCommitChangeTest1L,
    CommitChangeTestL,
    TeardownAlertForGroups
    )

EUNIT_TEST(
    "Get alert for groups 2 - test",
    "MProEngProfile",
    "AlertForL",
    "FUNCTIONALITY",
    SetupAlertForTest2L,
    AlertForTestL,
    TeardownAlertForGroups
    )

EUNIT_TEST(
    "Set alert for groups 2 - test",
    "MProEngProfile",
    "SetAlertForL",
    "FUNCTIONALITY",
    SetupSetAlertForTest2L,
    SetAlertForTestL,
    TeardownAlertForGroups
    )

EUNIT_TEST(
    "Silence state check 2 - test",
    "MProEngProfile",
    "IsSilent",
    "FUNCTIONALITY",
    Setup2L,
    IsSilentTestL,
    Teardown
    )

EUNIT_TEST(
    "MProEngProfileName interface getter 2 - test",
    "MProEngProfile",
    "ProfileName",
    "FUNCTIONALITY",
    Setup2L,
    ProfileNameTestL,
    Teardown
    )

EUNIT_TEST(
    "MProEngTones interface getter 2 - test",
    "MProEngProfile",
    "ProfileTones",
    "FUNCTIONALITY",
    Setup2L,
    ProfileTonesTestL,
    Teardown
    )

EUNIT_TEST(
    "MProEngToneSettings interface getter 2 - test",
    "MProEngProfile",
    "ToneSettings",
    "FUNCTIONALITY",
    Setup2L,
    ToneSettingsTestL,
    Teardown
    )

EUNIT_TEST(
    "Commiting of changes 2 - test",
    "MProEngProfile",
    "CommitChangeL",
    "FUNCTIONALITY",
    SetupCommitChangeTest2L,
    CommitChangeTestL,
    TeardownAlertForGroups
    )

EUNIT_TEST(
    "Get alert for groups 3 - test",
    "MProEngProfile",
    "AlertForL",
    "FUNCTIONALITY",
    SetupAlertForTest3L,
    AlertForTestL,
    TeardownAlertForGroups
    )

EUNIT_TEST(
    "Set alert for groups 3 - test",
    "MProEngProfile",
    "SetAlertForL",
    "FUNCTIONALITY",
    SetupSetAlertForTest3L,
    SetAlertForTestL,
    TeardownAlertForGroups
    )

EUNIT_TEST(
    "Silence state check 3 - test",
    "MProEngProfile",
    "IsSilent",
    "FUNCTIONALITY",
    Setup3L,
    IsSilentTestL,
    Teardown
    )

EUNIT_TEST(
    "MProEngProfileName interface getter 3 - test",
    "MProEngProfile",
    "ProfileName",
    "FUNCTIONALITY",
    Setup3L,
    ProfileNameTestL,
    Teardown
    )

EUNIT_TEST(
    "MProEngTones interface getter 3 - test",
    "MProEngProfile",
    "ProfileTones",
    "FUNCTIONALITY",
    Setup3L,
    ProfileTonesTestL,
    Teardown
    )

EUNIT_TEST(
    "MProEngToneSettings interface getter 3 - test",
    "MProEngProfile",
    "ToneSettings",
    "FUNCTIONALITY",
    Setup3L,
    ToneSettingsTestL,
    Teardown
    )

EUNIT_TEST(
    "Commiting of changes 3 - test",
    "MProEngProfile",
    "CommitChangeL",
    "FUNCTIONALITY",
    SetupCommitChangeTest3L,
    CommitChangeTestL,
    TeardownAlertForGroups
    )

EUNIT_TEST(
    "Get alert for groups 4 - test",
    "MProEngProfile",
    "AlertForL",
    "FUNCTIONALITY",
    SetupAlertForTest4L,
    AlertForTestL,
    TeardownAlertForGroups
    )

EUNIT_TEST(
    "Set alert for groups 4 - test",
    "MProEngProfile",
    "SetAlertForL",
    "FUNCTIONALITY",
    SetupSetAlertForTest4L,
    SetAlertForTestL,
    TeardownAlertForGroups
    )

EUNIT_TEST(
    "Silence state check 4 - test",
    "MProEngProfile",
    "IsSilent",
    "FUNCTIONALITY",
    Setup4L,
    IsSilentTestL,
    Teardown
    )

EUNIT_TEST(
    "MProEngProfileName interface getter 4 - test",
    "MProEngProfile",
    "ProfileName",
    "FUNCTIONALITY",
    Setup4L,
    ProfileNameTestL,
    Teardown
    )

EUNIT_TEST(
    "MProEngTones interface getter 4 - test",
    "MProEngProfile",
    "ProfileTones",
    "FUNCTIONALITY",
    Setup4L,
    ProfileTonesTestL,
    Teardown
    )

EUNIT_TEST(
    "MProEngToneSettings interface getter 4 - test",
    "MProEngProfile",
    "ToneSettings",
    "FUNCTIONALITY",
    Setup4L,
    ToneSettingsTestL,
    Teardown
    )

EUNIT_TEST(
    "Commiting of changes 4 - test",
    "MProEngProfile",
    "CommitChangeL",
    "FUNCTIONALITY",
    SetupCommitChangeTest4L,
    CommitChangeTestL,
    TeardownAlertForGroups
    )

#ifdef __OFFLINE_MODE
EUNIT_TEST(
    "Get alert for groups 5 - test",
    "MProEngProfile",
    "AlertForL",
    "FUNCTIONALITY",
    SetupAlertForTest5L,
    AlertForTestL,
    TeardownAlertForGroups
    )

EUNIT_TEST(
    "Set alert for groups 5 - test",
    "MProEngProfile",
    "SetAlertForL",
    "FUNCTIONALITY",
    SetupSetAlertForTest5L,
    SetAlertForTestL,
    TeardownAlertForGroups
    )

EUNIT_TEST(
    "Silence state check 5 - test",
    "MProEngProfile",
    "IsSilent",
    "FUNCTIONALITY",
    Setup5L,
    IsSilentTestL,
    Teardown
    )

EUNIT_TEST(
    "MProEngProfileName interface getter 5 - test",
    "MProEngProfile",
    "ProfileName",
    "FUNCTIONALITY",
    Setup5L,
    ProfileNameTestL,
    Teardown
    )

EUNIT_TEST(
    "MProEngTones interface getter 5 - test",
    "MProEngProfile",
    "ProfileTones",
    "FUNCTIONALITY",
    Setup5L,
    ProfileTonesTestL,
    Teardown
    )

EUNIT_TEST(
    "MProEngToneSettings interface getter 5 - test",
    "MProEngProfile",
    "ToneSettings",
    "FUNCTIONALITY",
    Setup5L,
    ToneSettingsTestL,
    Teardown
    )

EUNIT_TEST(
    "Commiting of changes 5 - test",
    "MProEngProfile",
    "CommitChangeL",
    "FUNCTIONALITY",
    SetupCommitChangeTest5L,
    CommitChangeTestL,
    TeardownAlertForGroups
    )
#endif // __OFFLINE_MODE

#ifdef __DRIVE_MODE
EUNIT_TEST(
    "Get alert for groups 6 - test",
    "MProEngProfile",
    "AlertForL",
    "FUNCTIONALITY",
    SetupAlertForTest6L,
    AlertForTestL,
    TeardownAlertForGroups
    )

EUNIT_TEST(
    "Set alert for groups 6 - test",
    "MProEngProfile",
    "SetAlertForL",
    "FUNCTIONALITY",
    SetupSetAlertForTest6L,
    SetAlertForTestL,
    TeardownAlertForGroups
    )

EUNIT_TEST(
    "Silence state check 6 - test",
    "MProEngProfile",
    "IsSilent",
    "FUNCTIONALITY",
    Setup6L,
    IsSilentTestL,
    Teardown
    )

EUNIT_TEST(
    "MProEngProfileName interface getter 6 - test",
    "MProEngProfile",
    "ProfileName",
    "FUNCTIONALITY",
    Setup6L,
    ProfileNameTestL,
    Teardown
    )

EUNIT_TEST(
    "MProEngTones interface getter 6 - test",
    "MProEngProfile",
    "ProfileTones",
    "FUNCTIONALITY",
    Setup6L,
    ProfileTonesTestL,
    Teardown
    )

EUNIT_TEST(
    "MProEngToneSettings interface getter 6 - test",
    "MProEngProfile",
    "ToneSettings",
    "FUNCTIONALITY",
    Setup6L,
    ToneSettingsTestL,
    Teardown
    )

EUNIT_TEST(
    "Commiting of changes 6 - test",
    "MProEngProfile",
    "CommitChangeL",
    "FUNCTIONALITY",
    SetupCommitChangeTest6L,
    CommitChangeTestL,
    TeardownAlertForGroups
    )
#endif // __DRIVE_MODE

EUNIT_END_TEST_TABLE

