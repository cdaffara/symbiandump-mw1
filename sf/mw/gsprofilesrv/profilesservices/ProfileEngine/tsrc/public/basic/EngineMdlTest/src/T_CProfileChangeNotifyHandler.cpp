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
* Description:  Implementation of the T_CProfileChangeNotifyHandler test class.
*
*/

////////////////////////////////////////////////////////////////////////
// T_CProfileChangeNotifyHandler.cpp
//

// INCLUDES
#include <e32math.h>
#include <e32svr.h>

#include <EUnitMacros.h>
#include <TEUnitAssertionInfo.h>

#include <CProfileChangeNotifyHandler.h>

#include "T_ProfileTestTools.h"
#include <centralrepository.h>
#include <e32property.h>
#include <ProfileEngineSDKCRKeys.h>
#include <MProfileEngineExtended.h>
#include <MProfileExtended.h>
#include <TProfileToneSettings.h>
#include <MProfileTones.h>
#include "MProfileSetTones.h"
#include "ProfileEnginePrivatePSKeys.h"

// the header for the tested class
#include "T_CProfileChangeNotifyHandler.h"

//CONSTS
//#define PROFILE_TEST_RUN_ALLOC


// create the testing class
T_CProfileChangeNotifyHandler* T_CProfileChangeNotifyHandler::NewLC()
    {
    T_CProfileChangeNotifyHandler* self = new(ELeave) T_CProfileChangeNotifyHandler;

    CleanupStack::PushL( self );
    // need to generate the table, so call base classes
    // second phase constructor
    self->ConstructL();
    return self;
    }

void T_CProfileChangeNotifyHandler::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

T_CProfileChangeNotifyHandler::~T_CProfileChangeNotifyHandler()
    {
    delete iObserver;
    }

/**
 * Assistance methods
 */


/**
 * Setup
 */
void T_CProfileChangeNotifyHandler::EmptySetupL()
    {
    }



void T_CProfileChangeNotifyHandler::SetupL()
    {
    iObserver = CTestObserver::NewL();
    }



/**
 * Teardown
 */
void T_CProfileChangeNotifyHandler::EmptyTeardown()
    {

    }

void T_CProfileChangeNotifyHandler::Teardown()
    {
    delete iObserver;
    iObserver = NULL;
    }

/**
 * Tests
 */

void T_CProfileChangeNotifyHandler::CreateTestL()
    {
    CProfileChangeNotifyHandler* handler = CProfileChangeNotifyHandler::NewL( iObserver );
    EUNIT_ASSERT( handler );
    delete handler;
    }


// Test case table for this test suite class
EUNIT_BEGIN_TEST_TABLE( T_CProfileChangeNotifyHandler, "T_CProfileChangeNotifyHandler test suite", "MODULE" )

    EUNIT_TESTCASE("Create test ", SetupL, CreateTestL, Teardown)

EUNIT_END_TESTCASE_TABLE

//  End of File
