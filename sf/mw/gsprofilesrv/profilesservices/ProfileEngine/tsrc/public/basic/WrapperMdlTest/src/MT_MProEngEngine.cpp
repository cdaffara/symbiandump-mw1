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
* Description:  Implementation of the MT_MProEngEngine test class.
*
*/

////////////////////////////////////////////////////////////////////////
// MT_MProEngEngine.cpp
//

//  CLASS HEADER
#include "MT_MProEngEngine.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <MProEngEngine.h>
#include <MProEngProfile.h>
#include "MProfileEngineExtended.h"
#include "MProfile.h"
#include "MProfileExtended.h"
#include <ProEngFactory.h>
#include <Profile.hrh>
#include "ProfileEngineConstants.h"

//  INTERNAL INCLUDES
#include "T_ProfileTestTools.h"

MT_MProEngEngine* MT_MProEngEngine::NewL()
    {
    MT_MProEngEngine* self = MT_MProEngEngine::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

MT_MProEngEngine* MT_MProEngEngine::NewLC()
    {
    MT_MProEngEngine* self = new( ELeave ) MT_MProEngEngine();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }


MT_MProEngEngine::~MT_MProEngEngine()
    {
    if( iInternalEngine )
        {
        iInternalEngine->Release();
        iInternalEngine = NULL;
        }
    }


MT_MProEngEngine::MT_MProEngEngine()
    {
    }

void MT_MProEngEngine::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    iInternalEngine = CreateProfileEngineExtendedL();
    }


void MT_MProEngEngine::SetupL()
    {
    EUNIT_PRINT( _L( "MT_MProEngEngine::SetupL begin" ) );
    iEngine = ProEngFactory::NewEngineL();
    iOrigId = iInternalEngine->ActiveProfileId();
    iInternalEngine->SetActiveProfileL( iActiveId );
    EUNIT_PRINT( _L( "MT_MProEngEngine::SetupL end" ) );
    }

void MT_MProEngEngine::Setup0L()
    {
    iActiveId = 0;
    SetupL();
    }

void MT_MProEngEngine::Setup1L()
    {
    iActiveId = 1;
    SetupL();
    }

void MT_MProEngEngine::Setup2L()
    {
    iActiveId = 2;
    SetupL();
    }

void MT_MProEngEngine::Setup3L()
    {
    iActiveId = 3;
    SetupL();
    }

void MT_MProEngEngine::Setup4L()
    {
    iActiveId = 4;
    SetupL();
    }

#ifdef __OFFLINE_MODE
void MT_MProEngEngine::Setup5L()
    {
    iActiveId = 5;
    SetupL();
    }

#endif // __OFFLINE_MODE
#ifdef __DRIVE_MODE
void MT_MProEngEngine::Setup6L()
    {
    iActiveId = 6;
    SetupL();
    }

#endif // __DRIVE_MODE

#ifdef __DYNAMIC_PROFILES
void MT_MProEngEngine::SetupDynamicProfilesL()
    {
    EUNIT_PRINT( _L( "MT_MProEngEngine::SetupDynamicProfilesL begin" ) );
    const TInt start( KProfileBaseDynamicProfileId );
    const TInt end( start + KProfilesMaxNumberOfDynamicProfiles );
    for( TInt i(start); i<end; ++i )
        {
        MProfileExtended* p( iInternalEngine->CreateProfileL() );
        p->Release();
        }
    EUNIT_PRINT( _L( "MT_MProEngEngine::SetupDynamicProfilesL end" ) );
    }

void MT_MProEngEngine::Setup30L()
    {
    EUNIT_PRINT( _L( "MT_MProEngEngine::Setup30L begin" ) );
    SetupDynamicProfilesL();
    iActiveId = 30;
    SetupL();
    EUNIT_PRINT( _L( "MT_MProEngEngine::Setup30L end" ) );
    }

void MT_MProEngEngine::Setup31L()
    {
    SetupDynamicProfilesL();
    iActiveId = 31;
    SetupL();
    }

void MT_MProEngEngine::Setup32L()
    {
    SetupDynamicProfilesL();
    iActiveId = 32;
    SetupL();
    }

void MT_MProEngEngine::Setup33L()
    {
    SetupDynamicProfilesL();
    iActiveId = 33;
    SetupL();
    }

void MT_MProEngEngine::Setup34L()
    {
    SetupDynamicProfilesL();
    iActiveId = 34;
    SetupL();
    }

void MT_MProEngEngine::Setup35L()
    {
    SetupDynamicProfilesL();
    iActiveId = 35;
    SetupL();
    }

void MT_MProEngEngine::Setup36L()
    {
    SetupDynamicProfilesL();
    iActiveId = 36;
    SetupL();
    }

void MT_MProEngEngine::Setup37L()
    {
    SetupDynamicProfilesL();
    iActiveId = 37;
    SetupL();
    }

void MT_MProEngEngine::Setup38L()
    {
    SetupDynamicProfilesL();
    iActiveId = 38;
    SetupL();
    }

void MT_MProEngEngine::Setup39L()
    {
    SetupDynamicProfilesL();
    iActiveId = 39;
    SetupL();
    }

void MT_MProEngEngine::Setup40L()
    {
    SetupDynamicProfilesL();
    iActiveId = 40;
    SetupL();
    }

void MT_MProEngEngine::Setup41L()
    {
    SetupDynamicProfilesL();
    iActiveId = 41;
    SetupL();
    }

void MT_MProEngEngine::Setup42L()
    {
    SetupDynamicProfilesL();
    iActiveId = 42;
    SetupL();
    }

void MT_MProEngEngine::Setup43L()
    {
    SetupDynamicProfilesL();
    iActiveId = 43;
    SetupL();
    }

void MT_MProEngEngine::Setup44L()
    {
    SetupDynamicProfilesL();
    iActiveId = 44;
    SetupL();
    }

void MT_MProEngEngine::Setup45L()
    {
    SetupDynamicProfilesL();
    iActiveId = 45;
    SetupL();
    }

void MT_MProEngEngine::Setup46L()
    {
    SetupDynamicProfilesL();
    iActiveId = 46;
    SetupL();
    }

void MT_MProEngEngine::Setup47L()
    {
    SetupDynamicProfilesL();
    iActiveId = 47;
    SetupL();
    }

void MT_MProEngEngine::Setup48L()
    {
    SetupDynamicProfilesL();
    iActiveId = 48;
    SetupL();
    }

void MT_MProEngEngine::Setup49L()
    {
    SetupDynamicProfilesL();
    iActiveId = 49;
    SetupL();
    }
#endif // __DYNAMIC_PROFILES

void MT_MProEngEngine::Teardown()
    {
    if( iEngine )
        {
        iEngine->Release();
        iEngine = NULL;
        }
    T_ProfileTestTools::ResetRepository();
    //iInternalEngine->SetActiveProfileL( iOrigId );
    }

#ifdef __DYNAMIC_PROFILES
void MT_MProEngEngine::TeardownDynamicProfiles()
    {
    Teardown();
    }
#endif // __DYNAMIC_PROFILES

void MT_MProEngEngine::ReleaseTestL()
    {
    TRAP_IGNORE( ForcedLeaveL() );
    }

void MT_MProEngEngine::ForcedLeaveL()
    {
    MProEngEngine* engine( ProEngFactory::NewEngineL() );
    CleanupReleasePushL( *engine );
    User::Leave( KErrGeneral );
    }

void MT_MProEngEngine::ActiveProfileLCTestL()
    {
    MProEngProfile* profile( iEngine->ActiveProfileLC() );
    MProfile* internalProfile( iInternalEngine->ActiveProfileLC() );
    TBool result(
        T_ProfileTestTools::CompareProfiles( *profile, *internalProfile ) );
    CleanupStack::PopAndDestroy( 2 ); // internalProfile, profile
    EUNIT_ASSERT( result );
    }

void MT_MProEngEngine::ActiveProfileTestL()
    {
    MProEngProfile* profile( iEngine->ActiveProfileL() );
    CleanupReleasePushL( *profile );
    MProfile* internalProfile( iInternalEngine->ActiveProfileL() );
    TBool result(
            T_ProfileTestTools::CompareProfiles( *profile, *internalProfile ) );
    internalProfile->Release();
    CleanupStack::PopAndDestroy(); // profile
    EUNIT_ASSERT( result );
    }

void MT_MProEngEngine::ActiveProfileIdTestL()
    {
    TInt id( iEngine->ActiveProfileId() );
    EUNIT_ASSERT( id == iActiveId );
    }

void MT_MProEngEngine::ProfileNameArrayLCTestL()
    {
    __UHEAP_MARK;
    MProEngProfileNameArray* array = iEngine->ProfileNameArrayLC();
    MProfilesNamesArray* array2 = iInternalEngine->ProfilesNamesArrayLC();
    TBool result( T_ProfileTestTools::CompareNameArrays( *array, *array2 ) );
    CleanupStack::PopAndDestroy( 2 ); // array2, array
    __UHEAP_MARKEND;
    EUNIT_ASSERT( result );
    }

void MT_MProEngEngine::SetActiveProfileTestL()
    {
    EUNIT_PRINT( _L( "MT_MProEngEngine::SetActiveProfileTestL begin" ) );
    TInt lastId( EProfilePagerId + 1 );
#ifdef __OFFLINE_MODE
    lastId = EProfileOffLineId + 1;
#endif // __OFFLINE_MODE
#ifdef __DRIVE_MODE
    lastId = EProfileDriveId + 1;
#endif // __DRIVE_MODE
    TInt i( 0 );
    EUNIT_PRINT( _L( "MT_MProEngEngine::SetActiveProfileTestL before for-loop" ) );
    for( ; i<lastId; ++i )
        {
        EUNIT_PRINT( _L( "MT_MProEngEngine::SetActiveProfileTestL before iEngine->SetActiveProfileL(%d)" ), i );
        iEngine->SetActiveProfileL( i );
        EUNIT_PRINT( _L( "MT_MProEngEngine::SetActiveProfileTestL after iEngine->SetActiveProfileL(%d)" ), i );
        EUNIT_ASSERT( i == iInternalEngine->ActiveProfileId() );
        EUNIT_PRINT( _L( "MT_MProEngEngine::SetActiveProfileTestL after ASSERT" ) );
        }
    EUNIT_PRINT( _L( "MT_MProEngEngine::SetActiveProfileTestL after for before iEngine->SetActiveProfileL(%d)" ), i );
    TRAPD( err, iEngine->SetActiveProfileL( i ) );
    EUNIT_PRINT( _L( "MT_MProEngEngine::SetActiveProfileTestL after for after iEngine->SetActiveProfileL(%d)" ), i );
    EUNIT_ASSERT( err == KErrNotFound );
    }

void MT_MProEngEngine::ProfileLCTestL()
    {
    TInt lastId( EProfilePagerId + 1 );
#ifdef __OFFLINE_MODE
    lastId = EProfileOffLineId + 1;
#endif // __OFFLINE_MODE
#ifdef __DRIVE_MODE
    lastId = EProfileDriveId + 1;
#endif // __DRIVE_MODE
    TInt i( 0 );
    EUNIT_PRINT( _L( "MT_MProEngEngine::ProfileLCTestL lastId=%d" ), lastId );
    for( ; i<lastId; ++i )
        {
        EUNIT_PRINT( _L( "MT_MProEngEngine::ProfileLCTestL in for i=%d" ), i );
        MProEngProfile* p( iEngine->ProfileLC( i ) );
        MProfileExtended* p2( iInternalEngine->ProfileLC( i ) );
        TBool result( T_ProfileTestTools::CompareProfiles( *p, *p2 ) );
        CleanupStack::PopAndDestroy( 2 ); // p2, p
        EUNIT_ASSERT( result );
        }
    EUNIT_PRINT( _L( "MT_MProEngEngine::ProfileLCTestL after for i=%d" ), i );
    TRAPD( err, MProEngProfile* p = iEngine->ProfileLC( i ) );
    EUNIT_PRINT( _L( "MT_MProEngEngine::ProfileLCTestL before last assert" ) );
    EUNIT_ASSERT( err == KErrNotFound );
    }

void MT_MProEngEngine::ProfileTestL()
    {
    TInt lastId( EProfilePagerId + 1 );
#ifdef __OFFLINE_MODE
    lastId = EProfileOffLineId + 1;
#endif // __OFFLINE_MODE
#ifdef __DRIVE_MODE
    lastId = EProfileDriveId + 1;
#endif // __DRIVE_MODE
    TInt i( 0 );
    for( ; i<lastId; ++i )
        {
        MProEngProfile* p( iEngine->ProfileL( i ) );
        CleanupReleasePushL( *p );
        MProfileExtended* p2( iInternalEngine->ProfileL( i ) );
        TBool result( T_ProfileTestTools::CompareProfiles( *p, *p2 ) );
        p2->Release();
        CleanupStack::PopAndDestroy(); // p
        EUNIT_ASSERT( result );
        }
    TRAPD( err, MProEngProfile* p = iEngine->ProfileL( i ) );
    EUNIT_ASSERT( err == KErrNotFound );
    }

#ifdef __DYNAMIC_PROFILES
void MT_MProEngEngine::SetActiveProfileDynamicProfilesTestL()
    {
    const TInt start( KProfileBaseDynamicProfileId );
    const TInt end( start + KProfilesMaxNumberOfDynamicProfiles );
    TInt i( start );
    for( ; i<end; ++i )
        {
        iEngine->SetActiveProfileL( i );
        EUNIT_ASSERT( i == iInternalEngine->ActiveProfileId() );
        }
    TRAPD( err, iEngine->SetActiveProfileL( i ) );
    EUNIT_ASSERT( err == KErrNotFound );
    }

void MT_MProEngEngine::ProfileDynamicProfilesTestL()
    {
    const TInt start( KProfileBaseDynamicProfileId );
    const TInt end( start + KProfilesMaxNumberOfDynamicProfiles );
    TInt i( start );
    for( ; i<end; ++i )
        {
        MProfileExtended* p2( iInternalEngine->ProfileLC( i ) );
        MProEngProfile* p( iEngine->ProfileL( i ) );
        TBool result( T_ProfileTestTools::CompareProfiles( *p, *p2 ) );
        p->Release();
        CleanupStack::PopAndDestroy(); // p2
        EUNIT_ASSERT( result );
        }
    TRAPD( err, MProEngProfile* p = iEngine->ProfileLC( i ) );
    EUNIT_ASSERT( err == KErrNotFound );
    }
#endif // __DYNAMIC_PROFILES

EUNIT_BEGIN_TEST_TABLE( 
    MT_MProEngEngine,
    "Profile Engine Wrapper Test cases.",
    "MODULE" )

EUNIT_TEST(
    "Release - test",
    "MProEngEngine",
    "Release",
    "FUNCTIONALITY",
    Setup0L,
    ReleaseTestL,
    Teardown)

EUNIT_TEST(
    "Get Active profile settings (object pushed to stack) 0 - test",
    "MProEngEngine",
    "ActiveProfileLC",
    "FUNCTIONALITY",
    Setup0L,
    ActiveProfileLCTestL,
    Teardown)

EUNIT_TEST(
    "Get Active profile settings 0 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup0L,
    ActiveProfileTestL,
    Teardown)

EUNIT_TEST(
    "Get Active profile id 0 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup0L,
    ActiveProfileIdTestL,
    Teardown)

EUNIT_TEST(
    "Get profile name array 0 - test",
    "MProEngEngine",
    "ProfileNameArrayLC",
    "FUNCTIONALITY",
    Setup0L,
    ProfileNameArrayLCTestL,
    Teardown)

EUNIT_TEST(
    "Set active profile 0 - test",
    "MProEngEngine",
    "SetActiveProfileL",
    "FUNCTIONALITY",
    Setup0L,
    SetActiveProfileTestL,
    Teardown)

EUNIT_TEST(
    "Get profile settings (object pushed to stack) 0 - test",
    "MProEngEngine",
    "ProfileLC",
    "FUNCTIONALITY",
    Setup0L,
    ProfileLCTestL,
    Teardown)

EUNIT_TEST(
    "Get profile settings 0 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup0L,
    ProfileTestL,
    Teardown)

EUNIT_TEST(
    "Get Active profile settings (object pushed to stack) 1 - test",
    "MProEngEngine",
    "ActiveProfileLC",
    "FUNCTIONALITY",
    Setup1L,
    ActiveProfileLCTestL,
    Teardown)

EUNIT_TEST(
    "Get Active profile settings 1 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup1L,
    ActiveProfileTestL,
    Teardown)

EUNIT_TEST(
    "Get Active profile id 1 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup1L,
    ActiveProfileIdTestL,
    Teardown)

EUNIT_TEST(
    "Get profile name array 1 - test",
    "MProEngEngine",
    "ProfileNameArrayLC",
    "FUNCTIONALITY",
    Setup1L,
    ProfileNameArrayLCTestL,
    Teardown)

EUNIT_TEST(
    "Set active profile 1 - test",
    "MProEngEngine",
    "SetActiveProfileL",
    "FUNCTIONALITY",
    Setup1L,
    SetActiveProfileTestL,
    Teardown)

EUNIT_TEST(
    "Get profile settings (object pushed to stack) 1 - test",
    "MProEngEngine",
    "ProfileLC",
    "FUNCTIONALITY",
    Setup1L,
    ProfileLCTestL,
    Teardown)

EUNIT_TEST(
    "Get profile settings 1 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup1L,
    ProfileTestL,
    Teardown)

EUNIT_TEST(
    "Get Active profile settings (object pushed to stack) 2 - test",
    "MProEngEngine",
    "ActiveProfileLC",
    "FUNCTIONALITY",
    Setup2L,
    ActiveProfileLCTestL,
    Teardown)

EUNIT_TEST(
    "Get Active profile settings 2 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup2L,
    ActiveProfileTestL,
    Teardown)

EUNIT_TEST(
    "Get Active profile id 2 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup2L,
    ActiveProfileIdTestL,
    Teardown)

EUNIT_TEST(
    "Get profile name array 2 - test",
    "MProEngEngine",
    "ProfileNameArrayLC",
    "FUNCTIONALITY",
    Setup2L,
    ProfileNameArrayLCTestL,
    Teardown)

EUNIT_TEST(
    "Set active profile 2 - test",
    "MProEngEngine",
    "SetActiveProfileL",
    "FUNCTIONALITY",
    Setup2L,
    SetActiveProfileTestL,
    Teardown)

EUNIT_TEST(
    "Get profile settings (object pushed to stack) 2 - test",
    "MProEngEngine",
    "ProfileLC",
    "FUNCTIONALITY",
    Setup2L,
    ProfileLCTestL,
    Teardown)

EUNIT_TEST(
    "Get profile settings 2 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup2L,
    ProfileTestL,
    Teardown)

EUNIT_TEST(
    "Get Active profile settings (object pushed to stack) 3 - test",
    "MProEngEngine",
    "ActiveProfileLC",
    "FUNCTIONALITY",
    Setup3L,
    ActiveProfileLCTestL,
    Teardown)

EUNIT_TEST(
    "Get Active profile settings 3 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup3L,
    ActiveProfileTestL,
    Teardown)

EUNIT_TEST(
    "Get Active profile id 3 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup3L,
    ActiveProfileIdTestL,
    Teardown)

EUNIT_TEST(
    "Get profile name array 3 - test",
    "MProEngEngine",
    "ProfileNameArrayLC",
    "FUNCTIONALITY",
    Setup3L,
    ProfileNameArrayLCTestL,
    Teardown)

EUNIT_TEST(
    "Set active profile 3 - test",
    "MProEngEngine",
    "SetActiveProfileL",
    "FUNCTIONALITY",
    Setup3L,
    SetActiveProfileTestL,
    Teardown)

EUNIT_TEST(
    "Get profile settings (object pushed to stack) 3 - test",
    "MProEngEngine",
    "ProfileLC",
    "FUNCTIONALITY",
    Setup3L,
    ProfileLCTestL,
    Teardown)

EUNIT_TEST(
    "Get profile settings 3 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup3L,
    ProfileTestL,
    Teardown)

EUNIT_TEST(
    "Get Active profile settings (object pushed to stack) 4 - test",
    "MProEngEngine",
    "ActiveProfileLC",
    "FUNCTIONALITY",
    Setup4L,
    ActiveProfileLCTestL,
    Teardown)

EUNIT_TEST(
    "Get Active profile settings 4 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup4L,
    ActiveProfileTestL,
    Teardown)

EUNIT_TEST(
    "Get Active profile id 4 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup4L,
    ActiveProfileIdTestL,
    Teardown)

EUNIT_TEST(
    "Get profile name array 4 - test",
    "MProEngEngine",
    "ProfileNameArrayLC",
    "FUNCTIONALITY",
    Setup4L,
    ProfileNameArrayLCTestL,
    Teardown)

EUNIT_TEST(
    "Set active profile 4 - test",
    "MProEngEngine",
    "SetActiveProfileL",
    "FUNCTIONALITY",
    Setup4L,
    SetActiveProfileTestL,
    Teardown)

EUNIT_TEST(
    "Get profile settings (object pushed to stack) 4 - test",
    "MProEngEngine",
    "ProfileLC",
    "FUNCTIONALITY",
    Setup4L,
    ProfileLCTestL,
    Teardown)

EUNIT_TEST(
    "Get profile settings 4 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup4L,
    ProfileTestL,
    Teardown)

#ifdef __OFFLINE_MODE
EUNIT_TEST(
    "Get Active profile settings (object pushed to stack) 5 - test",
    "MProEngEngine",
    "ActiveProfileLC",
    "FUNCTIONALITY",
    Setup5L,
    ActiveProfileLCTestL,
    Teardown)

EUNIT_TEST(
    "Get Active profile settings 5 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup5L,
    ActiveProfileTestL,
    Teardown)

EUNIT_TEST(
    "Get Active profile id 5 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup5L,
    ActiveProfileIdTestL,
    Teardown)

EUNIT_TEST(
    "Get profile name array 5 - test",
    "MProEngEngine",
    "ProfileNameArrayLC",
    "FUNCTIONALITY",
    Setup5L,
    ProfileNameArrayLCTestL,
    Teardown)

EUNIT_TEST(
    "Set active profile 5 - test",
    "MProEngEngine",
    "SetActiveProfileL",
    "FUNCTIONALITY",
    Setup5L,
    SetActiveProfileTestL,
    Teardown)

EUNIT_TEST(
    "Get profile settings (object pushed to stack) 5 - test",
    "MProEngEngine",
    "ProfileLC",
    "FUNCTIONALITY",
    Setup5L,
    ProfileLCTestL,
    Teardown)

EUNIT_TEST(
    "Get profile settings 5 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup5L,
    ProfileTestL,
    Teardown)

#endif // __OFFLINE_MODE

#ifdef __DRIVE_MODE
EUNIT_TEST(
    "Get Active profile settings (object pushed to stack) 6 - test",
    "MProEngEngine",
    "ActiveProfileLC",
    "FUNCTIONALITY",
    Setup6L,
    ActiveProfileLCTestL,
    Teardown)

EUNIT_TEST(
    "Get Active profile settings 6 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup6L,
    ActiveProfileTestL,
    Teardown)

EUNIT_TEST(
    "Get Active profile id 6 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup6L,
    ActiveProfileIdTestL,
    Teardown)

EUNIT_TEST(
    "Get profile name array 6 - test",
    "MProEngEngine",
    "ProfileNameArrayLC",
    "FUNCTIONALITY",
    Setup6L,
    ProfileNameArrayLCTestL,
    Teardown)

EUNIT_TEST(
    "Set active profile 6 - test",
    "MProEngEngine",
    "SetActiveProfileL",
    "FUNCTIONALITY",
    Setup6L,
    SetActiveProfileTestL,
    Teardown)

EUNIT_TEST(
    "Get profile settings (object pushed to stack) 6 - test",
    "MProEngEngine",
    "ProfileLC",
    "FUNCTIONALITY",
    Setup6L,
    ProfileLCTestL,
    Teardown)

EUNIT_TEST(
    "Get profile settings 6 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup6L,
    ProfileTestL,
    Teardown)

#endif // __DRIVE_MODE
#ifdef __DYNAMIC_PROFILES
EUNIT_TEST(
    "Get Profiles name array with user created profiles - test",
    "MProEngEngine",
    "ProfileNameArrayLC",
    "FUNCTIONALITY",
    Setup30L,
    ProfileNameArrayLCTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Set Active Profile (user created profile) - test",
    "MProEngEngine",
    "SetActiveProfileL",
    "FUNCTIONALITY",
    Setup30L,
    SetActiveProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile ID (user created profile) 30 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup30L,
    ActiveProfileIdTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile settings (user created profile) 30 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup30L,
    ActiveProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Profile settings (user created profile) 30 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup30L,
    ProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile ID (user created profile) 31 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup31L,
    ActiveProfileIdTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile settings (user created profile) 31 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup31L,
    ActiveProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Profile settings (user created profile) 31 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup31L,
    ProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile ID (user created profile) 32 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup32L,
    ActiveProfileIdTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile settings (user created profile) 32 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup32L,
    ActiveProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Profile settings (user created profile) 32 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup32L,
    ProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile ID (user created profile) 33 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup33L,
    ActiveProfileIdTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile settings (user created profile) 33 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup33L,
    ActiveProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Profile settings (user created profile) 33 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup33L,
    ProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile ID (user created profile) 34 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup34L,
    ActiveProfileIdTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile settings (user created profile) 34 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup34L,
    ActiveProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Profile settings (user created profile) 34 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup34L,
    ProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile ID (user created profile) 35 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup35L,
    ActiveProfileIdTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile settings (user created profile) 35 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup35L,
    ActiveProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Profile settings (user created profile) 35 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup35L,
    ProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile ID (user created profile) 36 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup36L,
    ActiveProfileIdTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile settings (user created profile) 36 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup36L,
    ActiveProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Profile settings (user created profile) 36 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup36L,
    ProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile ID (user created profile) 37 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup37L,
    ActiveProfileIdTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile settings (user created profile) 37 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup37L,
    ActiveProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Profile settings (user created profile) 37 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup37L,
    ProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile ID (user created profile) 38 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup38L,
    ActiveProfileIdTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile settings (user created profile) 38 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup38L,
    ActiveProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Profile settings (user created profile) 38 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup38L,
    ProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile ID (user created profile) 39 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup39L,
    ActiveProfileIdTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile settings (user created profile) 39 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup39L,
    ActiveProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Profile settings (user created profile) 39 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup39L,
    ProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile ID (user created profile) 40 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup40L,
    ActiveProfileIdTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile settings (user created profile) 40 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup40L,
    ActiveProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Profile settings (user created profile) 40 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup40L,
    ProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile ID (user created profile) 41 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup41L,
    ActiveProfileIdTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile settings (user created profile) 41 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup41L,
    ActiveProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Profile settings (user created profile) 41 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup41L,
    ProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile ID (user created profile) 42 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup42L,
    ActiveProfileIdTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile settings (user created profile) 42 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup42L,
    ActiveProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Profile settings (user created profile) 42 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup42L,
    ProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile ID (user created profile) 43 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup43L,
    ActiveProfileIdTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile settings (user created profile) 43 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup43L,
    ActiveProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Profile settings (user created profile) 43 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup43L,
    ProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile ID (user created profile) 44 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup44L,
    ActiveProfileIdTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile settings (user created profile) 44 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup44L,
    ActiveProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Profile settings (user created profile) 44 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup44L,
    ProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile ID (user created profile) 45 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup45L,
    ActiveProfileIdTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile settings (user created profile) 45 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup45L,
    ActiveProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Profile settings (user created profile) 45 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup45L,
    ProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile ID (user created profile) 46 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup46L,
    ActiveProfileIdTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile settings (user created profile) 46 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup46L,
    ActiveProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Profile settings (user created profile) 46 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup46L,
    ProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile ID (user created profile) 47 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup47L,
    ActiveProfileIdTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile settings (user created profile) 47 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup47L,
    ActiveProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Profile settings (user created profile) 47 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup47L,
    ProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile ID (user created profile) 48 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup48L,
    ActiveProfileIdTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile settings (user created profile) 48 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup48L,
    ActiveProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Profile settings (user created profile) 48 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup48L,
    ProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile ID (user created profile) 49 - test",
    "MProEngEngine",
    "ActiveProfileId",
    "FUNCTIONALITY",
    Setup49L,
    ActiveProfileIdTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Active Profile settings (user created profile) 49 - test",
    "MProEngEngine",
    "ActiveProfileL",
    "FUNCTIONALITY",
    Setup49L,
    ActiveProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Get Profile settings (user created profile) 49 - test",
    "MProEngEngine",
    "ProfileL",
    "FUNCTIONALITY",
    Setup49L,
    ProfileTestL,
    TeardownDynamicProfiles)

#endif // __DYNAMIC_PROFILES

EUNIT_END_TEST_TABLE

// END OF FILE

