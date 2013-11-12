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
* Description:  Implementation of the T_MProfile3DToneSettings test class.
*
*/


////////////////////////////////////////////////////////////////////////
// T_MProfile3DToneSettings.cpp
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
#include <MProfile3DToneSettings.h>
// the header for the tester class
#include "T_MProfile3DToneSettings.h"

//CONSTS

// create the testing class
T_MProfile3DToneSettings* T_MProfile3DToneSettings::NewLC()
    {
    T_MProfile3DToneSettings* self = new(ELeave) T_MProfile3DToneSettings;

    CleanupStack::PushL( self );
    // need to generate the table, so call base classes
    // second phase constructor
    self->ConstructL();
    return self;
    }

void T_MProfile3DToneSettings::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

T_MProfile3DToneSettings::~T_MProfile3DToneSettings()
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
void T_MProfile3DToneSettings::EmptySetupL()
    {
    }



void T_MProfile3DToneSettings::SetupL()
    {
    iFs.Connect();
    iCenRep = CRepository::NewL( KCRUidProfileEngine );
    iCenRep->Reset();

    iProfileEngine = CreateProfileEngineL();
    iProfileEngine->SetActiveProfileL( iActiveId );
    iProfile = iProfileEngine->ActiveProfileL();

    iProfileExtraSettings = &( iProfile->ProfileExtraSettings() );
    iProfile3DToneSettings = &( iProfileExtraSettings->Profile3DToneSettings() );
    }


/**
 * Teardown
 */
void T_MProfile3DToneSettings::EmptyTeardown()
    {

    }

void T_MProfile3DToneSettings::Teardown()
    {
    iFs.Close();
    if( iProfile )
        {
        iProfile->Release();
        iProfile = NULL;
        iProfile3DToneSettings = NULL;
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
void T_MProfile3DToneSettings::EmptyTestL()
    {
    //EUNIT_ASSERT( ETrue );
    }

void T_MProfile3DToneSettings::GetEffectTestL()
    {
    __UHEAP_MARK;
    TProfile3DToneEffect effect( iProfile3DToneSettings->Effect() );
    __UHEAP_MARKEND;
    }

void T_MProfile3DToneSettings::GetEchoTestL()
    {
    __UHEAP_MARK;
    TProfile3DToneEcho echo( iProfile3DToneSettings->Echo() );
    __UHEAP_MARKEND;
    }

// Test case table for this test suite class
EUNIT_BEGIN_TEST_TABLE( T_MProfile3DToneSettings, "T_MProfile3DToneSettings test suite", "MODULE" )

    EUNIT_TESTCASE("Get Effect test ", SetupL, GetEffectTestL, Teardown)

    EUNIT_TESTCASE("Get Echo test ", SetupL, GetEchoTestL, Teardown)

EUNIT_END_TESTCASE_TABLE

//  End of File

