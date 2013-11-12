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
* Description:  Implementation of the T_MProfilePttSettings test class.
*
*/


////////////////////////////////////////////////////////////////////////
// T_MProfilePttSettings.cpp
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
#include <MProfilePttSettings.h>
// the header for the tester class
#include "T_MProfilePttSettings.h"

//CONSTS

// create the testing class
T_MProfilePttSettings* T_MProfilePttSettings::NewLC()
    {
    T_MProfilePttSettings* self = new(ELeave) T_MProfilePttSettings;

    CleanupStack::PushL( self );
    // need to generate the table, so call base classes
    // second phase constructor
    self->ConstructL();
    return self;
    }

void T_MProfilePttSettings::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

T_MProfilePttSettings::~T_MProfilePttSettings()
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
void T_MProfilePttSettings::EmptySetupL()
    {
    }



void T_MProfilePttSettings::SetupL()
    {
    RDebug::Print( _L( "Digia:PttSettings test 1" ) );
    iFs.Connect();
    RDebug::Print( _L( "Digia:PttSettings test 2" ) );
    iCenRep = CRepository::NewL( KCRUidProfileEngine );
    RDebug::Print( _L( "Digia:PttSettings test 3" ) );
    iCenRep->Reset();
    RDebug::Print( _L( "Digia:PttSettings test 4" ) );

    iProfileEngine = CreateProfileEngineL();

    RDebug::Print( _L( "Digia:PttSettings test 5" ) );
    iProfileEngine->SetActiveProfileL( iActiveId );
    RDebug::Print( _L( "Digia:PttSettings test 6" ) );
    iProfile = iProfileEngine->ActiveProfileL();
    RDebug::Print( _L( "Digia:PttSettings test 7" ) );

    iProfileExtraSettings = &( iProfile->ProfileExtraSettings() );
    RDebug::Print( _L( "Digia:PttSettings test 8" ) );
    iProfilePttSettings = &( iProfileExtraSettings->ProfilePttSettings() );
    RDebug::Print( _L( "Digia:PttSettings test 9" ) );
    }

void T_MProfilePttSettings::Setup0L()
    {
    iActiveId = 0;
    SetupL();
    }

void T_MProfilePttSettings::Setup1L()
    {
    iActiveId = 1;
    SetupL();
    }

void T_MProfilePttSettings::Setup2L()
    {
    iActiveId = 2;
    SetupL();
    }

void T_MProfilePttSettings::Setup3L()
    {
    iActiveId = 3;
    SetupL();
    }

void T_MProfilePttSettings::Setup4L()
    {
    iActiveId = 4;
    SetupL();
    }


/**
 * Teardown
 */
void T_MProfilePttSettings::EmptyTeardown()
    {

    }

void T_MProfilePttSettings::Teardown()
    {
    iFs.Close();
    if( iProfile )
        {
        iProfile->Release();
        iProfile = NULL;
        iProfilePttSettings = NULL;
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
void T_MProfilePttSettings::EmptyTestL()
    {
    //EUNIT_ASSERT( ETrue );
    }

void T_MProfilePttSettings::GetPttStatusTestL()
    {
    __UHEAP_MARK;
    TProfilePttStatus status( iProfilePttSettings->PttStatus() );
    __UHEAP_MARKEND;

    RDebug::Print( _L( "GetPttStatusTestL: status=%d" ), status );
    //EUNIT_ASSERT( status == T_ProfileTestTools::ProfilePttStatus( iActiveId ) );
    }

void T_MProfilePttSettings::GetPttRingingToneTestL()
    {
    __UHEAP_MARK;
    const TDesC& desc = iProfilePttSettings->PttRingingTone();
    __UHEAP_MARKEND;

    //EUNIT_ASSERT( desc.CompareF( T_ProfileTestTools::ProfilePttRingingTone( iActiveId ) ) == 0 );
    }

// Test case table for this test suite class
EUNIT_BEGIN_TEST_TABLE( T_MProfilePttSettings, "T_MProfilePttSettings test suite", "MODULE" )

    EUNIT_TESTCASE("0: Get ptt status test ", Setup0L, GetPttStatusTestL, Teardown)

    EUNIT_TESTCASE("1: Get ptt status test ", Setup1L, GetPttStatusTestL, Teardown)

    EUNIT_TESTCASE("2: Get ptt status test ", Setup2L, GetPttStatusTestL, Teardown)

    EUNIT_TESTCASE("3: Get ptt status test ", Setup3L, GetPttStatusTestL, Teardown)

    EUNIT_TESTCASE("4: Get ptt status test ", Setup4L, GetPttStatusTestL, Teardown)

    EUNIT_TESTCASE("0: Get ptt ringing tone test ", Setup0L, GetPttRingingToneTestL, Teardown)

    EUNIT_TESTCASE("1: Get ptt ringing tone test ", Setup1L, GetPttRingingToneTestL, Teardown)

    EUNIT_TESTCASE("2: Get ptt ringing tone test ", Setup2L, GetPttRingingToneTestL, Teardown)

    EUNIT_TESTCASE("3: Get ptt ringing tone test ", Setup3L, GetPttRingingToneTestL, Teardown)

    EUNIT_TESTCASE("4: Get ptt ringing tone test ", Setup4L, GetPttRingingToneTestL, Teardown)

EUNIT_END_TESTCASE_TABLE

//  End of File

