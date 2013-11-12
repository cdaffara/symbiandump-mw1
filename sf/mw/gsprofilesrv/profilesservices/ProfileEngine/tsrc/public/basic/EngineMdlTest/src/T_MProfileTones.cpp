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
* Description:  Implementation of the T_MProfileTones test class.
*
*/


////////////////////////////////////////////////////////////////////////
// T_MProfileTones.cpp
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
#include <MProfileExtraTones.h>
#include <MProfileTones.h>
#include <TProfileToneSettings.h>
//#include "T_ProfileTestTools.h"
#include <centralrepository.h>
#include <ProfileEngineSDKCRKeys.h>

// the header for the tested class
#include "T_MProfileTones.h"

//CONSTS

// create the testing class
T_MProfileTones* T_MProfileTones::NewLC()
    {
    T_MProfileTones* self = new(ELeave) T_MProfileTones;

    CleanupStack::PushL( self );
    // need to generate the table, so call base classes
    // second phase constructor
    self->ConstructL();
    return self;
    }

void T_MProfileTones::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

T_MProfileTones::~T_MProfileTones()
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
void T_MProfileTones::EmptySetupL()
    {
    }



void T_MProfileTones::SetupL()
    {
    iFs.Connect();
    iCenRep = CRepository::NewL( KCRUidProfileEngine );
    iCenRep->Reset();

    iProfileEngine = CreateProfileEngineL();

    iProfileEngine->SetActiveProfileL( iActiveId );
    iProfile = iProfileEngine->ActiveProfileL();

    iProfileTones = &( iProfile->ProfileTones() );
    }

void T_MProfileTones::Setup0L()
    {
    iActiveId = 0;
    SetupL();
    }

void T_MProfileTones::Setup1L()
    {
    iActiveId = 1;
    SetupL();
    }

void T_MProfileTones::Setup2L()
    {
    iActiveId = 2;
    SetupL();
    }

void T_MProfileTones::Setup3L()
    {
    iActiveId = 3;
    SetupL();
    }

void T_MProfileTones::Setup4L()
    {
    iActiveId = 4;
    SetupL();
    }


/**
 * Teardown
 */
void T_MProfileTones::EmptyTeardown()
    {

    }

void T_MProfileTones::Teardown()
    {
    iFs.Close();
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
    delete iCenRep;

    }

/**
 * Tests
 */
void T_MProfileTones::EmptyTestL()
    {
    //EUNIT_ASSERT( ETrue );
    }

void T_MProfileTones::GetRingingTone1TestL()
    {
    __UHEAP_MARK;
    const TDesC& des = iProfileTones->RingingTone1();
    __UHEAP_MARKEND;
    TFileName rt( des );
    //EUNIT_ASSERT( des.CompareF( T_ProfileTestTools::ProfileRingingTone( iActiveId ) ) == 0 );
    }

void T_MProfileTones::GetRingingTone2TestL()
    {
    __UHEAP_MARK;
    const TDesC& des = iProfileTones->RingingTone2();
    __UHEAP_MARKEND;
    //EUNIT_ASSERT( des.CompareF( T_ProfileTestTools::ProfileRingingTone( iActiveId ) ) == 0 );
    }

void T_MProfileTones::GetMessageAlertToneTestL()
    {
    __UHEAP_MARK;
    const TDesC& des = iProfileTones->MessageAlertTone();
    __UHEAP_MARKEND;
    //EUNIT_ASSERT( des.CompareF( T_ProfileTestTools::ProfileMessageAlertTone( iActiveId ) ) == 0 );
    }

void T_MProfileTones::GetProfileToneSettingsTestL()
    {
    __UHEAP_MARK;
    const TProfileToneSettings& settings = iProfileTones->ToneSettings();
    __UHEAP_MARKEND;
    //EUNIT_ASSERT( T_ProfileTestTools::CheckToneSettings( iActiveId, settings ) );
    }

// Test case table for this test suite class
EUNIT_BEGIN_TEST_TABLE( T_MProfileTones, "T_MProfileTones test suite", "MODULE" )

    EUNIT_TESTCASE("0: Get ringing tone 1 test ",  Setup0L, GetRingingTone1TestL, Teardown)
    EUNIT_TESTCASE("0: Get ringing tone 2 test ",  Setup0L, GetRingingTone2TestL, Teardown)
    EUNIT_TESTCASE("0: Get msg alert tone test ",  Setup0L, GetMessageAlertToneTestL, Teardown)
    EUNIT_TESTCASE("0: Get tone settings test ",   Setup0L, GetProfileToneSettingsTestL, Teardown)

    EUNIT_TESTCASE("1: Get ringing tone 1 test ",  Setup1L, GetRingingTone1TestL, Teardown)
    EUNIT_TESTCASE("1: Get ringing tone 2 test ",  Setup1L, GetRingingTone2TestL, Teardown)
    EUNIT_TESTCASE("1: Get msg alert tone test ",  Setup1L, GetMessageAlertToneTestL, Teardown)
    EUNIT_TESTCASE("1: Get tone settings test ",   Setup1L, GetProfileToneSettingsTestL, Teardown)

    EUNIT_TESTCASE("2: Get ringing tone 1 test ",  Setup2L, GetRingingTone1TestL, Teardown)
    EUNIT_TESTCASE("2: Get ringing tone 2 test ",  Setup2L, GetRingingTone2TestL, Teardown)
    EUNIT_TESTCASE("2: Get msg alert tone test ",  Setup2L, GetMessageAlertToneTestL, Teardown)
    EUNIT_TESTCASE("2: Get tone settings test ",   Setup2L, GetProfileToneSettingsTestL, Teardown)

    EUNIT_TESTCASE("3: Get ringing tone 1 test ",  Setup3L, GetRingingTone1TestL, Teardown)
    EUNIT_TESTCASE("3: Get ringing tone 2 test ",  Setup3L, GetRingingTone2TestL, Teardown)
    EUNIT_TESTCASE("3: Get msg alert tone test ",  Setup3L, GetMessageAlertToneTestL, Teardown)
    EUNIT_TESTCASE("3: Get tone settings test ",   Setup3L, GetProfileToneSettingsTestL, Teardown)

    EUNIT_TESTCASE("4: Get ringing tone 1 test ",  Setup4L, GetRingingTone1TestL, Teardown)
    EUNIT_TESTCASE("4: Get ringing tone 2 test ",  Setup4L, GetRingingTone2TestL, Teardown)
    EUNIT_TESTCASE("4: Get msg alert tone test ",  Setup4L, GetMessageAlertToneTestL, Teardown)
    EUNIT_TESTCASE("4: Get tone settings test ",   Setup4L, GetProfileToneSettingsTestL, Teardown)

EUNIT_END_TESTCASE_TABLE

//  End of File
