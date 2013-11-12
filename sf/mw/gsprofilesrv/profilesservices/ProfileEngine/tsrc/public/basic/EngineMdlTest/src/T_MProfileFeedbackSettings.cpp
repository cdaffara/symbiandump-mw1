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
* Description:  Implementation of the T_MProfileFeedbackSettings test class.
*
*/


////////////////////////////////////////////////////////////////////////
// T_MProfileFeedbackSettings.cpp
//

// INCLUDES
#include <e32math.h>
#include <e32svr.h>

#include <EUnitMacros.h>
#include <TEUnitAssertionInfo.h>

#include <BAMDESCA.H>
#include <MProfileEngine.h>
#include <MProfile.h>
#include <MProfileName.h>
#include <MProfileExtraSettings.h>
//#include "T_ProfileTestTools.h"
#include <centralrepository.h>
#include <ProfileEngineSDKCRKeys.h>

// the header for the tested class
#include <MProfileFeedbackSettings.h>
// the header for the tester class
#include "T_MProfileFeedbackSettings.h"

//CONSTS

// create the testing class
T_MProfileFeedbackSettings* T_MProfileFeedbackSettings::NewLC()
    {
    T_MProfileFeedbackSettings* self = new(ELeave) T_MProfileFeedbackSettings;

    CleanupStack::PushL( self );
    // need to generate the table, so call base classes
    // second phase constructor
    self->ConstructL();
    return self;
    }

void T_MProfileFeedbackSettings::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

T_MProfileFeedbackSettings::~T_MProfileFeedbackSettings()
    {
    iFs.Close();
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


/**
 * Setup
 */
void T_MProfileFeedbackSettings::EmptySetupL()
    {
    }



void T_MProfileFeedbackSettings::SetupL()
    {
    iFs.Connect();
    iCenRep = CRepository::NewL( KCRUidProfileEngine );
    iCenRep->Reset();

    iProfileEngine = CreateProfileEngineL();
    iProfileEngine->SetActiveProfileL( iActiveId );
    iProfile = iProfileEngine->ActiveProfileL();

    iProfileExtraSettings = &( iProfile->ProfileExtraSettings() );
    iProfileFeedbackSettings = &( iProfileExtraSettings->ProfileFeedbackSettings() );
    }


/**
 * Teardown
 */
void T_MProfileFeedbackSettings::EmptyTeardown()
    {

    }

void T_MProfileFeedbackSettings::Teardown()
    {
    iFs.Close();
    if( iProfile )
        {
        iProfile->Release();
        iProfile = NULL;
        iProfileFeedbackSettings = NULL;
        iProfileExtraSettings = NULL;
        }
    if( iProfileEngine )
        {
        iProfileEngine->Release();
        iProfileEngine = NULL;
        }
    delete iCenRep;

    }

/**
 * Tests
 */
void T_MProfileFeedbackSettings::EmptyTestL()
    {
    //EUNIT_ASSERT( ETrue );
    }

void T_MProfileFeedbackSettings::GetTactileFeedbackTestL()
    {
    __UHEAP_MARK;
    TProfileTactileFeedback tac( iProfileFeedbackSettings->TactileFeedback() );
    __UHEAP_MARKEND;
    }

void T_MProfileFeedbackSettings::GetAudioFeedbackTestL()
    {
    __UHEAP_MARK;
    TProfileAudioFeedback aud( iProfileFeedbackSettings->AudioFeedback() );
    __UHEAP_MARKEND;
    }

// Test case table for this test suite class
EUNIT_BEGIN_TEST_TABLE( T_MProfileFeedbackSettings, "T_MProfileFeedbackSettings test suite", "MODULE" )

    EUNIT_TESTCASE("Get Tactile feedback test ", SetupL, GetTactileFeedbackTestL, Teardown)

    EUNIT_TESTCASE("Get Audio feedback test ", SetupL, GetAudioFeedbackTestL, Teardown)

EUNIT_END_TESTCASE_TABLE

//  End of File

