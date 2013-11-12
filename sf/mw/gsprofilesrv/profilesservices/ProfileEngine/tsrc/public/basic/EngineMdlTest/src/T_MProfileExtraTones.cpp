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
* Description:  Implementation of the T_MProfileExtraTones test class.
*
*/


////////////////////////////////////////////////////////////////////////
// T_MProfileExtraTonesExtraTones.cpp
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
#include "T_MProfileExtraTones.h"

//CONSTS

// create the testing class
T_MProfileExtraTones* T_MProfileExtraTones::NewLC()
    {
    T_MProfileExtraTones* self = new(ELeave) T_MProfileExtraTones;

    CleanupStack::PushL( self );
    // need to generate the table, so call base classes
    // second phase constructor
    self->ConstructL();
    return self;
    }

void T_MProfileExtraTones::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

T_MProfileExtraTones::~T_MProfileExtraTones()
    {
    iFs.Close();
    if( iProfileEngine )
        {
        iProfileEngine->Release();
        }
    iArray.Close();
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
void T_MProfileExtraTones::EmptySetupL()
    {
    }



void T_MProfileExtraTones::SetupL()
    {
    iFs.Connect();
    iCenRep = CRepository::NewL( KCRUidProfileEngine );
    iCenRep->Reset();

    iArray.Reset();
    iProfileEngine = CreateProfileEngineL();

    iProfileEngine->SetActiveProfileL( iActiveId );
    iProfile = iProfileEngine->ActiveProfileL();

    iProfileExtraTones = &( iProfile->ProfileExtraTones() );
    }

void T_MProfileExtraTones::Setup0L()
    {
    iActiveId = 0;
    SetupL();
    }

void T_MProfileExtraTones::Setup1L()
    {
    iActiveId = 1;
    SetupL();
    }

void T_MProfileExtraTones::Setup2L()
    {
    iActiveId = 2;
    SetupL();
    }

void T_MProfileExtraTones::Setup3L()
    {
    iActiveId = 3;
    SetupL();
    }

void T_MProfileExtraTones::Setup4L()
    {
    iActiveId = 4;
    SetupL();
    }


/**
 * Teardown
 */
void T_MProfileExtraTones::EmptyTeardown()
    {

    }

void T_MProfileExtraTones::Teardown()
    {
    iFs.Close();
    iArray.Reset();
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
void T_MProfileExtraTones::EmptyTestL()
    {
    //EUNIT_ASSERT( ETrue );
    }

void T_MProfileExtraTones::GetIMMessageAlertToneTestL()
    {
    __UHEAP_MARK;
    const TDesC& des = iProfileExtraTones->IMMessageAlertTone();
    __UHEAP_MARKEND;

    //EUNIT_ASSERT( des.CompareF( T_ProfileTestTools::ProfileIMMessageAlertTone( iActiveId ) ) == 0 );
    }

void T_MProfileExtraTones::GetEmailAlertToneTestL()
    {
    __UHEAP_MARK;
    const TDesC& des = iProfileExtraTones->EmailAlertTone();
    __UHEAP_MARKEND;

    //EUNIT_ASSERT( des.CompareF( T_ProfileTestTools::ProfileEmailAlertTone( iActiveId ) ) == 0 );
    }

void T_MProfileExtraTones::GetVideoCallRingingToneTestL()
    {
    __UHEAP_MARK;
    const TDesC& des = iProfileExtraTones->VideoCallRingingTone();
    __UHEAP_MARKEND;

    //EUNIT_ASSERT( des.CompareF( T_ProfileTestTools::ProfileVideoCallRingingTone( iActiveId ) ) == 0 );
    }

// Test case table for this test suite class
EUNIT_BEGIN_TEST_TABLE( T_MProfileExtraTones, "T_MProfileExtraTones test suite", "MODULE" )

    EUNIT_TESTCASE("0: Get IM msg alert tone test ", Setup0L, GetIMMessageAlertToneTestL, Teardown)

    EUNIT_TESTCASE("1: Get IM msg alert tone test ", Setup1L, GetIMMessageAlertToneTestL, Teardown)

    EUNIT_TESTCASE("2: Get IM msg alert tone test ", Setup2L, GetIMMessageAlertToneTestL, Teardown)

    EUNIT_TESTCASE("3: Get IM msg alert tone test ", Setup3L, GetIMMessageAlertToneTestL, Teardown)

    EUNIT_TESTCASE("4: Get IM msg alert tone test ", Setup4L, GetIMMessageAlertToneTestL, Teardown)

    EUNIT_TESTCASE("0: Get Email alert tone test ", Setup0L, GetEmailAlertToneTestL, Teardown)

    EUNIT_TESTCASE("1: Get Email alert tone test ", Setup1L, GetEmailAlertToneTestL, Teardown)

    EUNIT_TESTCASE("2: Get Email alert tone test ", Setup2L, GetEmailAlertToneTestL, Teardown)

    EUNIT_TESTCASE("3: Get Email alert tone test ", Setup3L, GetEmailAlertToneTestL, Teardown)

    EUNIT_TESTCASE("4: Get Email alert tone test ", Setup4L, GetEmailAlertToneTestL, Teardown)

    EUNIT_TESTCASE("0: Get Video call ringing tone test ", Setup0L, GetVideoCallRingingToneTestL, Teardown)

    EUNIT_TESTCASE("1: Get Video call ringing tone test ", Setup1L, GetVideoCallRingingToneTestL, Teardown)

    EUNIT_TESTCASE("2: Get Video call ringing tone test ", Setup2L, GetVideoCallRingingToneTestL, Teardown)

    EUNIT_TESTCASE("3: Get Video call ringing tone test ", Setup3L, GetVideoCallRingingToneTestL, Teardown)

    EUNIT_TESTCASE("4: Get Video call ringing tone test ", Setup4L, GetVideoCallRingingToneTestL, Teardown)

EUNIT_END_TESTCASE_TABLE

//  End of File
