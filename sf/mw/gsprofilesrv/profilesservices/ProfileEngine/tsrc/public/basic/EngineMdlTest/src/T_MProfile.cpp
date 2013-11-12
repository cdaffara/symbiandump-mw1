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
* Description:  Implementation of the T_MProfile test class.
*
*/

////////////////////////////////////////////////////////////////////////
// T_MProfile.cpp
//

// INCLUDES
#include <e32math.h>
#include <e32svr.h>

#include <EUnitMacros.h>
#include <TEUnitAssertionInfo.h>

#include <BAMDESCA.H>
#include <RSSSettings.h>
#include "MProfileEngine.h"
#include "MProfileEngineExtended.h"
#include "MProfile.h"
#include "MProfileName.h"
#include "MProfileExtraTones.h"
#include "MProfileSetExtraTones.h"
#include "MProfileExtraSettings.h"
#include "MProfilesLocalFeatures.h"
#include "MProfileTones.h"
#include <TProfileToneSettings.h>
#include "MProfileExtended.h"
#include "MProfileSetTones.h"
//#include "T_ProfileTestTools.h"
#include "ProfilesVariant.hrh"

// the header for the tested class
#include "T_MProfile.h"

//CONSTS

// create the testing class
T_MProfile* T_MProfile::NewLC()
    {
    T_MProfile* self = new(ELeave) T_MProfile;

    CleanupStack::PushL( self );
    // need to generate the table, so call base classes
    // second phase constructor
    self->ConstructL();
    return self;
    }

void T_MProfile::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

T_MProfile::~T_MProfile()
    {
    if( iProfileEngine )
        {
        iProfileEngine->Release();
        }
    if( iProfile )
        {
        iProfile->Release();
        }
    }

/**
 * Assistance methods
 */


void T_MProfile::SetupL()
    {
    iActiveId = 0;

    iProfileEngine = CreateProfileEngineL();

    iProfileEngine->SetActiveProfileL( iActiveId );
    iProfile = iProfileEngine->ActiveProfileL();
    }


/**
 * Teardown
 */

void T_MProfile::Teardown()
    {
    if( iProfile )
        {
        iProfile->Release();
        iProfile = NULL;
        }
    if( iProfileEngine )
        {
        iProfileEngine->Release();
        iProfileEngine = NULL;
        }
    }

/**
 * Tests
 */
void T_MProfile::EmptyTestL()
    {
    //EUNIT_ASSERT( ETrue );
    }

void T_MProfile::CreateTestL()
    {
    MProfile* profile = NULL;
    profile = iProfileEngine->ActiveProfileL();
    EUNIT_ASSERT( profile );
    profile->Release();
    profile = NULL;

    profile = iProfileEngine->ActiveProfileL();
    EUNIT_ASSERT( profile );
    CleanupReleasePushL( *profile );
    CleanupStack::PopAndDestroy();
    profile = NULL;

    profile = iProfileEngine->ActiveProfileLC();
    EUNIT_ASSERT( profile );
    CleanupStack::PopAndDestroy();
    profile = NULL;
    }

void T_MProfile::GetAlertForTestL()
    {
    __UHEAP_MARK;
    TArray<TContactItemId> array = iProfile->AlertForL();
    __UHEAP_MARKEND;

    EUNIT_ASSERT( array.Count() == 0 );
    }

void T_MProfile::IsSilentTestL()
    {
    __UHEAP_MARK;
    TBool isSilent( iProfile->IsSilent() );
    __UHEAP_MARKEND;
    if( iActiveId == 1 || iActiveId == 4 )
        {
        EUNIT_ASSERT( isSilent );
        }
    else
        {
        EUNIT_ASSERT( !isSilent );
        }
    }

void T_MProfile::GetProfileNameTestL()
    {
    const MProfileName* profileName = NULL;
    __UHEAP_MARK;
    profileName = &( iProfile->ProfileName() );
    __UHEAP_MARKEND;
    EUNIT_ASSERT( profileName );
    }

void T_MProfile::GetProfileTonesTestL()
    {
    const MProfileTones* profileTones = NULL;
    __UHEAP_MARK;
    profileTones = &( iProfile->ProfileTones() );
    __UHEAP_MARKEND;
    EUNIT_ASSERT( profileTones );
    }

void T_MProfile::GetProfileExtraTonesTestL()
    {
    const MProfileExtraTones* profileExtraTones = NULL;
    __UHEAP_MARK;
    profileExtraTones = &( iProfile->ProfileExtraTones() );
    __UHEAP_MARKEND;
    EUNIT_ASSERT( profileExtraTones );
    }

void T_MProfile::GetProfileExtraSettingsTestL()
    {
    const MProfileExtraSettings* profileExtraSettings = NULL;
    __UHEAP_MARK;
    profileExtraSettings = &( iProfile->ProfileExtraSettings() );
    __UHEAP_MARKEND;
    EUNIT_ASSERT( profileExtraSettings );
    }


// Test case table for this test suite class
EUNIT_BEGIN_TEST_TABLE( T_MProfile, "T_MProfile test suite", "MODULE" )

    EUNIT_TESTCASE("Create test ",              SetupL, CreateTestL, Teardown)
    EUNIT_TESTCASE("Get alert for test ",       SetupL, GetAlertForTestL, Teardown)
    EUNIT_TESTCASE("Is silent test ",           SetupL, IsSilentTestL, Teardown)
    EUNIT_TESTCASE("Get profile name test ",    SetupL, GetProfileNameTestL, Teardown)
    EUNIT_TESTCASE("Get profile tones test ",   SetupL, GetProfileTonesTestL, Teardown)
    EUNIT_TESTCASE("Get profile extra tones test ",   SetupL, GetProfileExtraTonesTestL, Teardown)
    EUNIT_TESTCASE("Get profile extra settings test ",   SetupL, GetProfileExtraSettingsTestL, Teardown)

EUNIT_END_TESTCASE_TABLE

//  End of File
