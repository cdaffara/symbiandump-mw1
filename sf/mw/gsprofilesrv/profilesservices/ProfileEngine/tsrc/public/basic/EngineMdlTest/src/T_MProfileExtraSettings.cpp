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
* Description:  Implementation of the T_MProfileExtraSettings test class.
*
*/


////////////////////////////////////////////////////////////////////////
// T_MProfileExtraSettings.cpp
//

// INCLUDES
#include <e32math.h>
#include <e32svr.h>

#include <EUnitMacros.h>
#include <TEUnitAssertionInfo.h>

#include <BAMDESCA.H>
#include <MProfileEngine.h>
#include <MProfile.h>
#include <MProfileExtraSettings.h>
#include <MProfilePttSettings.h>
#include <MProfileFeedbackSettings.h>
#include <MProfile3DToneSettings.h>
//#include "T_ProfileTestTools.h"

// the header for the tested class
#include "T_MProfileExtraSettings.h"


// create the testing class
T_MProfileExtraSettings* T_MProfileExtraSettings::NewLC()
    {
    T_MProfileExtraSettings* self = new(ELeave) T_MProfileExtraSettings;

    CleanupStack::PushL( self );
    // need to generate the table, so call base classes
    // second phase constructor
    self->ConstructL();
    return self;
    }

void T_MProfileExtraSettings::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();

    }

T_MProfileExtraSettings::~T_MProfileExtraSettings()
    {
    if( iProfile )
        {
        iProfile->Release();
        }
    if( iProfileEngine )
        {
        iProfileEngine->Release();
        }
    }

/**
 * Assistance methods
 */


/**
 * Setup
 */
void T_MProfileExtraSettings::EmptySetupL()
    {
    }



void T_MProfileExtraSettings::SetupL()
    {
    iProfileEngine = CreateProfileEngineL();
    iProfile = iProfileEngine->ActiveProfileL();
    }


/**
 * Teardown
 */
void T_MProfileExtraSettings::EmptyTeardown()
    {

    }

void T_MProfileExtraSettings::Teardown()
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
void T_MProfileExtraSettings::EmptyTestL()
    {
    //EUNIT_ASSERT( ETrue );
    }

/////////////////////
// MProfile
void T_MProfileExtraSettings::ProfilePttSettingsTestL()
    {
    const MProfileExtraSettings& extraSettings = iProfile->ProfileExtraSettings();
    const MProfilePttSettings& pttSettings = extraSettings.ProfilePttSettings();
    }

void T_MProfileExtraSettings::ProfileFeedbackSettingsTestL()
    {
    const MProfileExtraSettings& extraSettings = iProfile->ProfileExtraSettings();
    const MProfileFeedbackSettings& feedbackSettings = extraSettings.ProfileFeedbackSettings();
    }

void T_MProfileExtraSettings::Profile3DToneSettingsTestL()
    {
    const MProfileExtraSettings& extraSettings = iProfile->ProfileExtraSettings();
    const MProfile3DToneSettings& threeDSettings = extraSettings.Profile3DToneSettings();
    }


// Test case table for this test suite class
EUNIT_BEGIN_TEST_TABLE( T_MProfileExtraSettings, "T_MProfileExtended test suite", "MODULE" )

    EUNIT_TESTCASE("Get PTT interface test ", SetupL, ProfilePttSettingsTestL, Teardown )
    EUNIT_TESTCASE("Get Feedback interface test ", SetupL, ProfileFeedbackSettingsTestL, Teardown )
    EUNIT_TESTCASE("Get 3D-Tone interface test ", SetupL, Profile3DToneSettingsTestL, Teardown )

EUNIT_END_TESTCASE_TABLE

//  End of File
