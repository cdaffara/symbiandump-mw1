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
* Description:  Implementation of the T_MProfileName test class.
*
*/


////////////////////////////////////////////////////////////////////////
// T_MProfileName.cpp
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
//#include "T_ProfileTestTools.h"

// the header for the tested class
#include "T_MProfileName.h"

//CONSTS

// create the testing class
T_MProfileName* T_MProfileName::NewLC()
    {
    T_MProfileName* self = new(ELeave) T_MProfileName;

    CleanupStack::PushL( self );
    // need to generate the table, so call base classes
    // second phase constructor
    self->ConstructL();
    return self;
    }

void T_MProfileName::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

T_MProfileName::~T_MProfileName()
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
void T_MProfileName::EmptySetupL()
    {
    }



void T_MProfileName::SetupL()
    {
    iFs.Connect();

    iProfileEngine = CreateProfileEngineL();

    iProfileEngine->SetActiveProfileL( iActiveId );
    iProfile = iProfileEngine->ActiveProfileL();

    iProfileName = &( iProfile->ProfileName() );
    }


/**
 * Teardown
 */
void T_MProfileName::EmptyTeardown()
    {

    }

void T_MProfileName::Teardown()
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

    }

/**
 * Tests
 */
void T_MProfileName::EmptyTestL()
    {
    //EUNIT_ASSERT( ETrue );
    }

void T_MProfileName::GetIdTestL()
    {
    __UHEAP_MARK;
    TInt id = iProfileName->Id();
    __UHEAP_MARKEND;
    EUNIT_ASSERT( id == iActiveId );
    }

void T_MProfileName::GetNameTestL()
    {
    __UHEAP_MARK;
    const TDesC& des = iProfileName->Name();
    __UHEAP_MARKEND;
    //EUNIT_ASSERT( des.Compare( T_ProfileTestTools::ProfileName( iActiveId ) ) == 0 );
    }

void T_MProfileName::GetShortNameTestL()
    {
    __UHEAP_MARK;
    const TDesC& des = iProfileName->ShortName();
    __UHEAP_MARKEND;
    //EUNIT_ASSERT( des.Compare( T_ProfileTestTools::ProfileName( iActiveId ) ) == 0 );
    }
void T_MProfileName::GetNaviNameTestL()
    {
    __UHEAP_MARK;
    const TDesC& des = iProfileName->NaviName();
    __UHEAP_MARKEND;
    //EUNIT_ASSERT( des.Compare( T_ProfileTestTools::ProfileName( iActiveId ) ) == 0 );
    }
void T_MProfileName::GetTitleNameTestL()
    {
    __UHEAP_MARK;
    const TDesC& des = iProfileName->TitleName();
    __UHEAP_MARKEND;
    //EUNIT_ASSERT( des.Compare( T_ProfileTestTools::ProfileName( iActiveId ) ) == 0 );
    }

// Test case table for this test suite class
EUNIT_BEGIN_TEST_TABLE( T_MProfileName, "T_MProfileName test suite", "MODULE" )

    EUNIT_TESTCASE("Get ID test ", SetupL, GetIdTestL, Teardown)
    EUNIT_TESTCASE("Get Name test ", SetupL, GetNameTestL, Teardown)
    EUNIT_TESTCASE("Get ShortName test ", SetupL, GetShortNameTestL, Teardown)
    EUNIT_TESTCASE("Get NaviName test ", SetupL, GetNaviNameTestL, Teardown)
    EUNIT_TESTCASE("Get TitleName test ", SetupL, GetTitleNameTestL, Teardown)

EUNIT_END_TESTCASE_TABLE

//  End of File
