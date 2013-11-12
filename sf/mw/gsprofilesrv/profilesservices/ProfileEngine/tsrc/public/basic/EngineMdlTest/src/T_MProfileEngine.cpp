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
* Description:  Implementation of the T_MProfileEngine test class.
*
*/

////////////////////////////////////////////////////////////////////////
// T_MProfileEngine.cpp
//

// INCLUDES
#include <centralrepository.h>
#include <ProfileEngineSDKCRKeys.h>
#include <e32math.h>
#include <e32svr.h>
#include <bautils.h>

#include <EUnitMacros.h>
#include <TEUnitAssertionInfo.h>

#include <BAMDESCA.H>
#include <MProfileEngine.h>
#include <MProfile.h>
#include <MProfilesNamesArray.h>
#include <MProfileTones.h>
#include <MProfileExtraTones.h>
#include "MProfilesLocalFeatures.h"
#include "ProfilesVariant.hrh"
#include "MProfileExtended.h"
#include "MProfileSetName.h"
#include "MProfileEngineExtended.h"
#include <TProfileToneSettings.h>
//#include "T_ProfileTestTools.h"
//#include "T_ProfileTestHelper.h"

// the header for the tested class
#include "T_MProfileEngine.h"

//CONSTS
const TProfileRingingVolume
        KProfileTestExpectedVolume( EProfileRingingVolumeLevel3 );

// Redefined from RSSSettings.h (which includes shareddatakeys.h and thus cannot
// be included here):
namespace
    {
    enum TSSSettingsAlsValue
        {
        ESSSettingsAlsNotSupported = 0,
        ESSSettingsAlsPrimary = 1,
        ESSSettingsAlsAlternate = 2
        };
    }

// create the testing class
T_MProfileEngine* T_MProfileEngine::NewLC()
    {
    T_MProfileEngine* self = new(ELeave) T_MProfileEngine;

    CleanupStack::PushL( self );
    // need to generate the table, so call base classes
    // second phase constructor
    self->ConstructL();
    return self;
    }

T_MProfileEngine::~T_MProfileEngine()
    {
    iFs.Close();
    if( iProfileEngine )
        {
        iProfileEngine->Release();
        }
    iArray.Close();
    }

/**
 * Setup
 */
void T_MProfileEngine::EmptySetupL()
    {
    iFs.Connect();
    }



void T_MProfileEngine::SetupL()
    {
    iFs.Connect();
    iCenRep = CRepository::NewL( KCRUidProfileEngine );
    iCenRep->Reset();

    iProfileEngine = CreateProfileEngineL( &iFs );
    iArray.Reset();
    iProfileEngine->SetActiveProfileL( iActiveId );
    iTimed = EFalse;
    }

void T_MProfileEngine::Setup0L()
    {
    iActiveId = 0;
    SetupL();
    }

void T_MProfileEngine::SetupGetRingingVolumeL()
    {
    Setup0L();

    User::LeaveIfError( iCenRep->Get( KProEngActiveRingingVolume,
                                     iOrigRingingVolume ) );
    User::LeaveIfError( iCenRep->Set( KProEngActiveRingingVolume,
                                     KProfileTestExpectedVolume ) );
    }

void T_MProfileEngine::SetupGetMediaVolumeL()
    {
    Setup0L();
    }


/**
 * Teardown
 */
void T_MProfileEngine::EmptyTeardown()
    {
    iFs.Close();
    }

void T_MProfileEngine::Teardown()
    {
    iFs.Close();
    delete iCenRep;
    if( iProfileEngine )
        {
        iProfileEngine->Release();
        iProfileEngine = NULL;
        }
    iArray.Reset();
    }


/**
 * Tests
 */
void T_MProfileEngine::EmptyTestL()
    {
    //EUNIT_ASSERT( ETrue );
    }

void T_MProfileEngine::CreateTestL()
    {
    MProfileEngine* tmp = NULL;
    tmp = CreateProfileEngineL();
    EUNIT_ASSERT( tmp );
    if( tmp )
        {
        tmp->Release();
        }
    
    tmp = NULL;
    tmp = CreateProfileEngineL();
    CleanupReleasePushL( *tmp );
    EUNIT_ASSERT( tmp );
    CleanupStack::PopAndDestroy();
    
    tmp = NULL;
    tmp = CreateProfileEngineL( &iFs );
    EUNIT_ASSERT( tmp );
    if( tmp )
        {
        tmp->Release();
        }
    
    tmp = NULL;
    tmp = CreateProfileEngineL( &iFs );
    CleanupReleasePushL( *tmp );
    EUNIT_ASSERT( tmp );
    CleanupStack::PopAndDestroy();
    }

void T_MProfileEngine::GetActiveProfileTestL()
    {
    MProfile* profile = NULL;
    profile = iProfileEngine->ActiveProfileL();
    EUNIT_ASSERT( profile );
    profile->Release();

    profile = NULL;
    profile = iProfileEngine->ActiveProfileLC();
    CleanupStack::Pop(); // profile
    EUNIT_ASSERT( profile );
    profile->Release();
    }

void T_MProfileEngine::GetActiveProfileIdTestL()
    {
    TInt id = User::LeaveIfError( iProfileEngine->ActiveProfileId() );
    EUNIT_ASSERT( iActiveId == id );
    }

void T_MProfileEngine::SetActiveProfileTestL()
    {
    TInt id(0);
    iProfileEngine->SetActiveProfileL( 0 );
    id = iProfileEngine->ActiveProfileId();
    User::LeaveIfError( id );
    EUNIT_ASSERT( id == 0 );
    //EUNIT_ASSERT( T_ProfileTestTools::CheckSharedDataSettingsL( 0 ) );

    iProfileEngine->SetActiveProfileL( 1 );
    id = iProfileEngine->ActiveProfileId();
    User::LeaveIfError( id );
    EUNIT_ASSERT( id == 1 );
    //EUNIT_ASSERT( T_ProfileTestTools::CheckSharedDataSettingsL( 1 ) );

    iProfileEngine->SetActiveProfileL( 2 );
    id = iProfileEngine->ActiveProfileId();
    User::LeaveIfError( id );
    EUNIT_ASSERT( id == 2 );
    //EUNIT_ASSERT( T_ProfileTestTools::CheckSharedDataSettingsL( 2 ) );

    iProfileEngine->SetActiveProfileL( 3 );
    id = iProfileEngine->ActiveProfileId();
    User::LeaveIfError( id );
    EUNIT_ASSERT( id == 3 );
    //EUNIT_ASSERT( T_ProfileTestTools::CheckSharedDataSettingsL( 3 ) );

    iProfileEngine->SetActiveProfileL( 4 );
    id = iProfileEngine->ActiveProfileId();
    User::LeaveIfError( id );
    EUNIT_ASSERT( id == 4 );
    //EUNIT_ASSERT( T_ProfileTestTools::CheckSharedDataSettingsL( 4 ) );

    iProfileEngine->SetActiveProfileL( 0 );
    id = User::LeaveIfError( iProfileEngine->ActiveProfileId() );
    EUNIT_ASSERT( id == 0 );
    
    TRAPD( err, iProfileEngine->SetActiveProfileL( -1 ) );
    EUNIT_ASSERT( err != KErrNone );
    if( ( err != KErrNotFound ) && ( err != KErrPathNotFound ) )
        {
        User::LeaveIfError( err );
        }
    EUNIT_ASSERT( ( err == KErrNotFound ) || ( err == KErrPathNotFound ) );
    id = User::LeaveIfError( iProfileEngine->ActiveProfileId() );
    EUNIT_ASSERT( id == 0 );
    //EUNIT_ASSERT( T_ProfileTestTools::CheckSharedDataSettingsL( 0 ) );

#ifdef __OFFLINE_MODE
    iProfileEngine->SetActiveProfileL( 5 );
    id = iProfileEngine->ActiveProfileId();
    User::LeaveIfError( id );
    EUNIT_ASSERT( id == 5 );
    //EUNIT_ASSERT( T_ProfileTestTools::CheckSharedDataSettingsL( 5 ) );

    iProfileEngine->SetActiveProfileL( 0 );
    id = User::LeaveIfError( iProfileEngine->ActiveProfileId() );
    EUNIT_ASSERT( id == 0 );
#endif
    
#ifdef __DRIVE_MODE
    iProfileEngine->SetActiveProfileL( 6 );
    id = iProfileEngine->ActiveProfileId();
    User::LeaveIfError( id );
    EUNIT_ASSERT( id == 6 );
    //EUNIT_ASSERT( T_ProfileTestTools::CheckSharedDataSettingsL( 6 ) );

    iProfileEngine->SetActiveProfileL( 0 );
    id = User::LeaveIfError( iProfileEngine->ActiveProfileId() );
    EUNIT_ASSERT( id == 0 );
#endif
    TRAP( err, iProfileEngine->SetActiveProfileL( 7 ) );
    EUNIT_ASSERT( err != KErrNone );
    if( ( err != KErrNotFound ) && ( err != KErrPathNotFound ) )
        {
        User::LeaveIfError( err );
        }
    EUNIT_ASSERT( ( err == KErrNotFound ) || ( err == KErrPathNotFound ) );
    id = User::LeaveIfError( iProfileEngine->ActiveProfileId() );
    EUNIT_ASSERT( id == 0 );
    //EUNIT_ASSERT( T_ProfileTestTools::CheckSharedDataSettingsL( 0 ) );

    TRAP( err, iProfileEngine->SetActiveProfileL( 1232131 ) );
    EUNIT_ASSERT( err != KErrNone );
    if( ( err != KErrNotFound ) && ( err != KErrPathNotFound ) )
        {
        User::LeaveIfError( err );
        }
    EUNIT_ASSERT( ( err == KErrNotFound ) || ( err == KErrPathNotFound ) );
    id = User::LeaveIfError( iProfileEngine->ActiveProfileId() );
    EUNIT_ASSERT( id == 0 );
    //EUNIT_ASSERT( T_ProfileTestTools::CheckSharedDataSettingsL( 0 ) );
    }

void T_MProfileEngine::GetProfilesNamesArrayTestL()
    {
    MProfilesNamesArray* array = iProfileEngine->ProfilesNamesArrayLC();
    EUNIT_ASSERT( array );
    //EUNIT_ASSERT( T_ProfileTestTools::CheckArray( *array ) );
    CleanupStack::PopAndDestroy();
    array = NULL;

    array = iProfileEngine->ProfilesNamesArrayLC();
    EUNIT_ASSERT( array );
    //EUNIT_ASSERT( T_ProfileTestTools::CheckArray( *array ) );
    CleanupStack::Pop();
    delete array;
    }

void T_MProfileEngine::SetTempRingingVolumeTestL()
    {
    iProfileEngine->SetTempRingingVolumeL( EProfileRingingVolumeLevel5 );
    TInt volume( 0 );
    User::LeaveIfError( iCenRep->Get( KProEngActiveRingingVolume, volume ) );
    EUNIT_ASSERT( volume == 5 );
    TRAPD( err, 
           iProfileEngine->SetTempRingingVolumeL( 
           EProfileRingingVolumeLevel1 ) );
    if( err )
        {
        }
#ifdef __PROTOCOL_CDMA
    //EUNIT_ASSERT( err == KErrNone );
#else
    //EUNIT_ASSERT( err == KErrNotSupported );
#endif

    iProfileEngine->SetTempRingingVolumeL( EProfileRingingVolumeLevel7 );
    }

void T_MProfileEngine::SetTempMediaVolumeTestL()
    {
    iProfileEngine->SetTempMediaVolumeL( EProfileRingingVolumeLevel2 );
    }

void T_MProfileEngine::GetTempRingingVolumeTestL()
    {
    TProfileRingingVolume vol( iProfileEngine->TempRingingVolumeL() );
    EUNIT_ASSERT( vol == KProfileTestExpectedVolume );
    }

void T_MProfileEngine::GetTempMediaVolumeTestL()
    {
    TProfileRingingVolume vol( iProfileEngine->TempMediaVolumeL() );
    }

void T_MProfileEngine::IsActiveProfileTimedLTestL()
    {
    MProfileEngineExtended* engineExt = CreateProfileEngineExtendedL( &iFs );
    CleanupReleasePushL( *engineExt );
    MProfilesLocalFeatures& features = engineExt->LocalFeatures();
    if( features.IsFeatureSupported( KProEngFeatureIdTimedProfiles ) )
        {
        TBool result( iProfileEngine->IsActiveProfileTimedL() );
        EUNIT_ASSERT( result == iTimed );
        }
    CleanupStack::PopAndDestroy(); //engineExt
    }

// Test case table for this test suite class
EUNIT_BEGIN_TEST_TABLE( T_MProfileEngine, "T_MProfileEngine test suite", "MODULE" )

    EUNIT_TESTCASE("Create test ", EmptySetupL, CreateTestL, EmptyTeardown)
//     
    EUNIT_TESTCASE("Get active profile test",      Setup0L, GetActiveProfileTestL, Teardown )
    EUNIT_TESTCASE("Get active profile id test ",   Setup0L, GetActiveProfileIdTestL, Teardown )
    EUNIT_TESTCASE("Set active profile test ",      Setup0L, SetActiveProfileTestL, Teardown )
    EUNIT_TESTCASE("Get profiles names array test ", Setup0L, GetProfilesNamesArrayTestL, Teardown )

    EUNIT_TESTCASE("Set temporary ringing volume ", Setup0L, SetTempRingingVolumeTestL, Teardown )
    EUNIT_TESTCASE("Set temporary media volume ", Setup0L, SetTempMediaVolumeTestL, Teardown )
    EUNIT_TESTCASE("Get temporary ringing volume ", SetupGetRingingVolumeL, GetTempRingingVolumeTestL, Teardown )
    EUNIT_TESTCASE("Get temporary media volume ", SetupGetMediaVolumeL, GetTempMediaVolumeTestL, Teardown )
    EUNIT_TESTCASE("Is Active Profile Timed test", Setup0L, IsActiveProfileTimedLTestL, Teardown )

EUNIT_END_TESTCASE_TABLE

//  End of File
