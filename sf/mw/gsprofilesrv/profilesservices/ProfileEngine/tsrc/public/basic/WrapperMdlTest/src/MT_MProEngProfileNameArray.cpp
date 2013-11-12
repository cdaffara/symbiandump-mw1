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
* Description:  Implementation of the MT_MProEngProfileNameArray test class.
*
*/

////////////////////////////////////////////////////////////////////////
// MT_MProEngProfileNameArray.cpp
//

//  CLASS HEADER
#include "MT_MProEngProfileNameArray.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <MProEngProfileNameArray.h>
#include <MProEngProfile.h>
#include <MProEngEngine.h>
#include "MProfileEngineExtended.h"
#include "MProfile.h"
#include "MProfileName.h"
#include "MProfilesNamesArray.h"
#include "MProfileExtended.h"
#include <ProEngFactory.h>
#include <Profile.hrh>

//  INTERNAL INCLUDES
#include "T_ProfileTestTools.h"

MT_MProEngProfileNameArray* MT_MProEngProfileNameArray::NewL()
    {
    MT_MProEngProfileNameArray* self = MT_MProEngProfileNameArray::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

MT_MProEngProfileNameArray* MT_MProEngProfileNameArray::NewLC()
    {
    MT_MProEngProfileNameArray* self = new( ELeave ) MT_MProEngProfileNameArray();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }


MT_MProEngProfileNameArray::~MT_MProEngProfileNameArray()
    {
    if( iInternalEngine )
        {
        iInternalEngine->Release();
        iInternalEngine = NULL;
        }
    if( iEngine )
        {
        iEngine->Release();
        iEngine = NULL;
        }
    }


MT_MProEngProfileNameArray::MT_MProEngProfileNameArray()
    {
    }

void MT_MProEngProfileNameArray::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    iInternalEngine = CreateProfileEngineExtendedL();
    iEngine = ProEngFactory::NewEngineL();
    }


void MT_MProEngProfileNameArray::SetupL()
    {
    iArray = iEngine->ProfileNameArrayLC();
    CleanupStack::Pop(); // iArray
    iInternalArray = iInternalEngine->ProfilesNamesArrayLC();
    CleanupStack::Pop(); // iInternalArray
    }

void MT_MProEngProfileNameArray::SetupDynamicProfilesL()
    {
    MProfileExtended* internalProfile( iInternalEngine->CreateProfileL() );
    if( internalProfile )
        {
        iProfileId = ( internalProfile->ProfileName().Id() );
        internalProfile->Release();
        }
    SetupL();
    }

void MT_MProEngProfileNameArray::Teardown()
    {
    delete iArray;
    delete iInternalArray;
    }

void MT_MProEngProfileNameArray::TeardownDynamicProfiles()
    {
    Teardown();
    TRAP_IGNORE( iInternalEngine->DeleteProfileL( iProfileId ) );
    }


void MT_MProEngProfileNameArray::ProfileIdTestL()
    {
    TInt count( iArray->MdcaCount() );
    TInt internalCount( iInternalArray->MdcaCount() );
    EUNIT_ASSERT( count == internalCount );
    for( TInt i( 0 ); i<internalCount; ++i )
        {
        TInt id1( iArray->ProfileId(i) );
        TInt id2( iInternalArray->ProfileName(i)->Id() );
        EUNIT_ASSERT( id1 == id2 );
        }
    }


void MT_MProEngProfileNameArray::FindByIdTestL()
    {
    TInt lastId( EProfilePagerId + 1 );
#ifdef __OFFLINE_MODE
    lastId = EProfileOffLineId + 1;
#endif // __OFFLINE_MODE
#ifdef __DRIVE_MODE
    lastId = EProfileDriveId + 1;
#endif // __DRIVE_MODE
    // i<=lastId: tests also the first id which isn't found
    for( TInt i( 0 ); i<=lastId; ++i )
        {
        EUNIT_ASSERT( iArray->FindById(i) == iInternalArray->FindById(i) );
        }
    }


void MT_MProEngProfileNameArray::FindByNameTestL()
    {
    EUNIT_ASSERT( iArray->FindByName(KProfileGeneralName) ==
                  iInternalArray->FindByName(KProfileGeneralName) );
    EUNIT_ASSERT( iArray->FindByName(KProfileSilentName) ==
                  iInternalArray->FindByName(KProfileSilentName) );
    EUNIT_ASSERT( iArray->FindByName(KProfileMeetingName) ==
                  iInternalArray->FindByName(KProfileMeetingName) );
    EUNIT_ASSERT( iArray->FindByName(KProfileOutdoorName) ==
                  iInternalArray->FindByName(KProfileOutdoorName) );
    EUNIT_ASSERT( iArray->FindByName(KProfilePagerName) ==
                  iInternalArray->FindByName(KProfilePagerName) );
#ifdef __OFFLINE_MODE
    EUNIT_ASSERT( iArray->FindByName(KProfileOfflineName) ==
                  iInternalArray->FindByName(KProfileOfflineName) );
#endif
#ifdef __DRIVE_MODE
    EUNIT_ASSERT( iArray->FindByName(KProfileDriveName) ==
                  iInternalArray->FindByName(KProfileDriveName) );
#endif
    EUNIT_ASSERT( iArray->FindByName(KProfileInvalidName) ==
                  iInternalArray->FindByName(KProfileInvalidName) );
    }

void MT_MProEngProfileNameArray::FindByIdDynamicProfileTestL()
    {
    EUNIT_ASSERT( iArray->FindById( iProfileId ) ==
                  iInternalArray->FindById( iProfileId ) );
    }

void MT_MProEngProfileNameArray::FindByNameDynamicProfileTestL()
    {
    EUNIT_ASSERT( iArray->FindByName(KProfileDynamicName) ==
                  iInternalArray->FindByName(KProfileDynamicName) );
    }

EUNIT_BEGIN_TEST_TABLE( 
    MT_MProEngProfileNameArray,
    "Profile Name Array Wrapper Test cases.",
    "MODULE" )

EUNIT_TEST(
    "Get Profile Id by index test",
    "MProEngProfileNameArray",
    "ProfileId",
    "FUNCTIONALITY",
    SetupL,
    ProfileIdTestL,
    Teardown)

EUNIT_TEST(
    "Find index by Profile ID test",
    "MProEngProfileNameArray",
    "FindById",
    "FUNCTIONALITY",
    SetupL,
    FindByIdTestL,
    Teardown)

EUNIT_TEST(
    "Find index by Profile name test",
    "MProEngProfileNameArray",
    "FindByName",
    "FUNCTIONALITY",
    SetupL,
    FindByNameTestL,
    Teardown)

EUNIT_TEST(
    "Find index by Dynamic Profile ID test",
    "MProEngProfileNameArray",
    "FindById",
    "FUNCTIONALITY",
    SetupDynamicProfilesL,
    FindByIdDynamicProfileTestL,
    TeardownDynamicProfiles)

EUNIT_TEST(
    "Find index by Dynamic Profile name test",
    "MProEngProfileNameArray",
    "FindByName",
    "FUNCTIONALITY",
    SetupDynamicProfilesL,
    FindByNameDynamicProfileTestL,
    TeardownDynamicProfiles)

EUNIT_END_TEST_TABLE

