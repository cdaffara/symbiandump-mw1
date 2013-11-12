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
* Description:  Implementation of the T_MProfilesNamesArray test class.
*
*/


////////////////////////////////////////////////////////////////////////
// T_MProfilesNamesArray.cpp
//

// INCLUDES
#include <e32math.h>
#include <e32svr.h>

#include <EUnitMacros.h>
#include <TEUnitAssertionInfo.h>

#include <BAMDESCA.H>
#include <MProfileEngine.h>
#include <MProfileName.h>
#include <MProfilesNamesArray.h>
#include "T_ProfileTestTools.h"

// the header for the tested class
#include "T_MProfilesNamesArray.h"

//CONSTS

// create the testing class
T_MProfilesNamesArray* T_MProfilesNamesArray::NewLC()
    {
    T_MProfilesNamesArray* self = new(ELeave) T_MProfilesNamesArray;

    CleanupStack::PushL( self );
    // need to generate the table, so call base classes
    // second phase constructor
    self->ConstructL();
    return self;
    }

void T_MProfilesNamesArray::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

T_MProfilesNamesArray::~T_MProfilesNamesArray()
    {
    iFs.Close();
    if( iProfileEngine )
        {
        iProfileEngine->Release();
        }
    delete iProfilesNames;
    }

/**
 * Assistance methods
 */


/**
 * Setup
 */
void T_MProfilesNamesArray::EmptySetupL()
    {
    }



void T_MProfilesNamesArray::SetupL()
    {
    iFs.Connect();
    
    iProfileEngine = CreateProfileEngineL();
    iProfilesNames = iProfileEngine->ProfilesNamesArrayLC();
    CleanupStack::Pop();
    }
/*
void T_MProfilesNamesArray::Setup0L()
    {
    iActiveId = 0;
    SetupL();
    }

void T_MProfilesNamesArray::Setup1L()
    {
    iActiveId = 1;
    SetupL();
    }

void T_MProfilesNamesArray::Setup2L()
    {
    iActiveId = 2;
    SetupL();
    }

void T_MProfilesNamesArray::Setup3L()
    {
    iActiveId = 3;
    SetupL();
    }

void T_MProfilesNamesArray::Setup4L()
    {
    iActiveId = 4;
    SetupL();
    }
*/

/**
 * Teardown
 */
void T_MProfilesNamesArray::EmptyTeardown()
    {

    }

void T_MProfilesNamesArray::Teardown()
    {
    iFs.Close();
    if( iProfileEngine )
        {
        iProfileEngine->Release();
        iProfileEngine = NULL;
        }
    delete iProfilesNames;
    iProfilesNames = NULL;
    }

/**
 * Tests
 */
void T_MProfilesNamesArray::EmptyTestL()
    {
    //EUNIT_ASSERT( ETrue );
    }

void T_MProfilesNamesArray::CreateTestL()
    {
    MProfilesNamesArray* array = iProfileEngine->ProfilesNamesArrayLC();
    CleanupStack::PopAndDestroy();
    array = iProfileEngine->ProfilesNamesArrayLC();
    CleanupStack::Pop();
    delete array;
    }

void T_MProfilesNamesArray::ProfileNameTestL()
    {
    __UHEAP_MARK;
    const MProfileName* pn = NULL;
    pn = iProfilesNames->ProfileName( 0 );
    EUNIT_ASSERT( pn );
    EUNIT_ASSERT( pn->Name().Compare( KProfileGeneralName ) == 0 );
    EUNIT_ASSERT( pn->Id() == 0 );
    
    pn = iProfilesNames->ProfileName( 1 );
    EUNIT_ASSERT( pn );
    EUNIT_ASSERT( pn->Name().Compare( KProfileSilentName ) == 0 );
    EUNIT_ASSERT( pn->Id() == 1 );

    pn = iProfilesNames->ProfileName( 2 );
    EUNIT_ASSERT( pn );
    EUNIT_ASSERT( pn->Name().Compare( KProfileMeetingName ) == 0 );
    EUNIT_ASSERT( pn->Id() == 2 );

    pn = iProfilesNames->ProfileName( 3 );
    EUNIT_ASSERT( pn );
    EUNIT_ASSERT( pn->Name().Compare( KProfileOutdoorName ) == 0 );
    EUNIT_ASSERT( pn->Id() == 3 );

    pn = iProfilesNames->ProfileName( 4 );
    EUNIT_ASSERT( pn );
    EUNIT_ASSERT( pn->Name().Compare( KProfilePagerName ) == 0 );
    EUNIT_ASSERT( pn->Id() == 4 );

    pn = iProfilesNames->ProfileName( -1 );
    EUNIT_ASSERT( !pn );

    pn = iProfilesNames->ProfileName( 12 );
    EUNIT_ASSERT( !pn );

#ifdef __OFFLINE_MODE
    pn = iProfilesNames->ProfileName( 5 );
    EUNIT_ASSERT( pn );
    EUNIT_ASSERT( pn->Name().Compare( KProfileOfflineName ) == 0 );
    EUNIT_ASSERT( pn->Id() == 5 );
#endif

#ifdef __DRIVE_MODE
    pn = iProfilesNames->ProfileName( 6 );
    EUNIT_ASSERT( pn );
    EUNIT_ASSERT( pn->Name().Compare( KProfileDriveName ) == 0 );
    EUNIT_ASSERT( pn->Id() == 6 );
#endif
    pn = iProfilesNames->ProfileName( 7 );
    EUNIT_ASSERT( !pn );

    __UHEAP_MARKEND;
    }

void T_MProfilesNamesArray::FindIdTestL()
    {
    __UHEAP_MARK;
    EUNIT_ASSERT( iProfilesNames->FindById( 0 ) == 0 );
    EUNIT_ASSERT( iProfilesNames->FindById( 1 ) == 1 );
    EUNIT_ASSERT( iProfilesNames->FindById( 2 ) == 2 );
    EUNIT_ASSERT( iProfilesNames->FindById( 3 ) == 3 );
    EUNIT_ASSERT( iProfilesNames->FindById( 4 ) == 4 );
#ifdef __OFFLINE_MODE
    EUNIT_ASSERT( iProfilesNames->FindById( 5 ) == 5 );
#endif
#ifdef __DRIVE_MODE
    EUNIT_ASSERT( iProfilesNames->FindById( 6 ) == 6 );
#endif
    EUNIT_ASSERT( iProfilesNames->FindById( 7 ) == KErrNotFound );
    EUNIT_ASSERT( iProfilesNames->FindById( -1 ) == KErrNotFound );
    EUNIT_ASSERT( iProfilesNames->FindById( 12312312 ) == KErrNotFound );
    __UHEAP_MARKEND;
    }

void T_MProfilesNamesArray::FindNameTestL()
    {
    __UHEAP_MARK;
    EUNIT_ASSERT( iProfilesNames->FindByName( KProfileGeneralName ) == 0 );
    EUNIT_ASSERT( iProfilesNames->FindByName( KProfileSilentName ) == 1 );
    EUNIT_ASSERT( iProfilesNames->FindByName( KProfileMeetingName ) == 2 );
    EUNIT_ASSERT( iProfilesNames->FindByName( KProfileOutdoorName ) == 3 );
    EUNIT_ASSERT( iProfilesNames->FindByName( KProfilePagerName ) == 4 );
    EUNIT_ASSERT( iProfilesNames->FindByName( _L( "general" ) ) == KErrNotFound );
    EUNIT_ASSERT( iProfilesNames->FindByName( _L( "" ) ) == KErrNotFound );
    EUNIT_ASSERT( iProfilesNames->FindByName( _L( "dasdasdsadsadasdsadasddddddddddddddddddddddddddddddddddddd" ) ) == KErrNotFound );
    __UHEAP_MARKEND;
    }

void T_MProfilesNamesArray::MdcaCountTestL()
    {
    __UHEAP_MARK;
    TInt expectedCount( 5 );
#ifdef __OFFLINE_MODE
    ++expectedCount;
#endif
#ifdef __DRIVE_MODE
    ++expectedCount;
#endif
    EUNIT_ASSERT( iProfilesNames->MdcaCount() == expectedCount );
    __UHEAP_MARKEND;
    
    }

void T_MProfilesNamesArray::MdcaPointTestL()
    {
    __UHEAP_MARK;
    EUNIT_ASSERT( iProfilesNames->MdcaPoint( 0 ).Compare( KProfileGeneralName ) == 0 );
    EUNIT_ASSERT( iProfilesNames->MdcaPoint( 1 ).Compare( KProfileSilentName ) == 0 );
    EUNIT_ASSERT( iProfilesNames->MdcaPoint( 2 ).Compare( KProfileMeetingName ) == 0 );
    EUNIT_ASSERT( iProfilesNames->MdcaPoint( 3 ).Compare( KProfileOutdoorName ) == 0 );
    EUNIT_ASSERT( iProfilesNames->MdcaPoint( 4 ).Compare( KProfilePagerName ) == 0 );
#ifdef __OFFLINE_MODE
    EUNIT_ASSERT( iProfilesNames->MdcaPoint( 5 ).Compare( KProfileOfflineName ) == 0 );
#endif
#ifdef __DRIVE_MODE
    EUNIT_ASSERT( iProfilesNames->MdcaPoint( 6 ).Compare( KProfileDriveName ) == 0 );
#endif
    EUNIT_ASSERT( iProfilesNames->MdcaPoint( 7 ).Compare( _L( "" ) ) == 0 );
    EUNIT_ASSERT( iProfilesNames->MdcaPoint( -1 ).Compare( _L( "" ) ) == 0 );
    EUNIT_ASSERT( iProfilesNames->MdcaPoint( 12312312 ).Compare( _L( "" ) ) == 0 );
    __UHEAP_MARKEND;
    }


// Test case table for this test suite class
EUNIT_BEGIN_TEST_TABLE( T_MProfilesNamesArray, "T_MProfilesNamesArray test suite", "MODULE" )

    EUNIT_TESTCASE("Create test ",			SetupL, CreateTestL, Teardown)
    EUNIT_TESTCASE("Profile name test ",	SetupL, ProfileNameTestL, Teardown)
    EUNIT_TESTCASE("Find id test ",			SetupL, FindIdTestL, Teardown)
    EUNIT_TESTCASE("Find name test ",		SetupL, FindNameTestL, Teardown)
    EUNIT_TESTCASE("MdcaCount test ",		SetupL, MdcaCountTestL, Teardown)
    EUNIT_TESTCASE("MdcaPoint test ",		SetupL, MdcaPointTestL, Teardown)


EUNIT_END_TESTCASE_TABLE

//  End of File
