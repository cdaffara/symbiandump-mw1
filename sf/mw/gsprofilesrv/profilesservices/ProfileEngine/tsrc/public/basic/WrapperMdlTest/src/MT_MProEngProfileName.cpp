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
* Description:  Implementation of the MT_MProEngProfileName test class.
*
*/

////////////////////////////////////////////////////////////////////////
// MT_MProEngProfileName.cpp
//

//  CLASS HEADER
#include "MT_MProEngProfileName.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <MProEngProfileName.h>
#include <MProEngProfile.h>
#include <MProEngEngine.h>
#include "MProfileEngineExtended.h"
#include "MProfile.h"
#include "MProfileName.h"
#include "MProfileExtended.h"
#include <ProEngFactory.h>
#include <Profile.hrh>

//  INTERNAL INCLUDES
#include "T_ProfileTestTools.h"

MT_MProEngProfileName* MT_MProEngProfileName::NewL()
    {
    MT_MProEngProfileName* self = MT_MProEngProfileName::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

MT_MProEngProfileName* MT_MProEngProfileName::NewLC()
    {
    MT_MProEngProfileName* self = new( ELeave ) MT_MProEngProfileName();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }


MT_MProEngProfileName::~MT_MProEngProfileName()
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


MT_MProEngProfileName::MT_MProEngProfileName()
    {
    }

void MT_MProEngProfileName::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    iInternalEngine = CreateProfileEngineExtendedL();
    iEngine = ProEngFactory::NewEngineL();
    }


void MT_MProEngProfileName::SetupL()
    {
    iInternalProfile = iInternalEngine->ProfileL( iProfileId );
    iProfile = iEngine->ProfileL( iProfileId );
    }

void MT_MProEngProfileName::Setup0L()
    {
    iProfileId = 0;
    SetupL();
    }

void MT_MProEngProfileName::Setup1L()
    {
    iProfileId = 1;
    SetupL();
    }

void MT_MProEngProfileName::Setup2L()
    {
    iProfileId = 2;
    SetupL();
    }

void MT_MProEngProfileName::Setup3L()
    {
    iProfileId = 3;
    SetupL();
    }

void MT_MProEngProfileName::Setup4L()
    {
    iProfileId = 4;
    SetupL();
    }

#ifdef __OFFLINE_MODE
void MT_MProEngProfileName::Setup5L()
    {
    iProfileId = 5;
    SetupL();
    }

#endif // __OFFLINE_MODE
#ifdef __DRIVE_MODE
void MT_MProEngProfileName::Setup6L()
    {
    iProfileId = 6;
    SetupL();
    }

#endif // __DRIVE_MODE


void MT_MProEngProfileName::Teardown()
    {
    if( iInternalProfile )
        {
        iInternalProfile->Release();
        iInternalProfile = NULL;
        }
    if( iProfile )
        {
        iProfile->Release();
        iProfile = NULL;
        }
    }


void MT_MProEngProfileName::GetIdTestL()
    {
    MProEngProfileName& profileName( iProfile->ProfileName() );
    const MProfileName& name( iInternalProfile->ProfileName() );
    EUNIT_ASSERT( profileName.Id() == name.Id() );
    }


void MT_MProEngProfileName::GetNameTestL()
    {
    MProEngProfileName& profileName( iProfile->ProfileName() );
    const MProfileName& name( iInternalProfile->ProfileName() );
    EUNIT_ASSERT( profileName.Name().Compare( name.Name() ) == 0 );
    }


void MT_MProEngProfileName::SetNameTestL()
    {
    MProEngProfileName& profileName( iProfile->ProfileName() );
    const TDesC& origName( profileName.Name() );
    TInt err( profileName.SetNameL( KProEngMTTestName ) );
    if( ( iProfileId != EProfileOffLineId ) &&
        ( iProfileId != EProfileDriveId ) &&
        ( iProfileId != EProfileGeneralId ) )
        {
        EUNIT_ASSERT( err == KErrNone );
        EUNIT_ASSERT( profileName.Name().Compare( KProEngMTTestName ) == 0 );
        }
    else
        {
        EUNIT_ASSERT( err == KErrAccessDenied );
        EUNIT_ASSERT( profileName.Name().Compare( origName ) == 0 );
        }
    }

EUNIT_BEGIN_TEST_TABLE( 
    MT_MProEngProfileName,
    "Profile Name Wrapper Test cases.",
    "MODULE" )

EUNIT_TEST(
    "Get Profile ID 0 - test",
    "MProEngProfileName",
    "Id",
    "FUNCTIONALITY",
    Setup0L,
    GetIdTestL,
    Teardown)

EUNIT_TEST(
    "Get Profile Name 0 - test",
    "MProEngProfileName",
    "Name",
    "FUNCTIONALITY",
    Setup0L,
    GetNameTestL,
    Teardown)

EUNIT_TEST(
    "Set Profile Name 0 - test",
    "MProEngProfileName",
    "SetNameL",
    "FUNCTIONALITY",
    Setup0L,
    SetNameTestL,
    Teardown)

EUNIT_TEST(
    "Get Profile ID 1 - test",
    "MProEngProfileName",
    "Id",
    "FUNCTIONALITY",
    Setup1L,
    GetIdTestL,
    Teardown)

EUNIT_TEST(
    "Get Profile Name 1 - test",
    "MProEngProfileName",
    "Name",
    "FUNCTIONALITY",
    Setup1L,
    GetNameTestL,
    Teardown)

EUNIT_TEST(
    "Set Profile Name 1 - test",
    "MProEngProfileName",
    "SetNameL",
    "FUNCTIONALITY",
    Setup1L,
    SetNameTestL,
    Teardown)

EUNIT_TEST(
    "Get Profile ID 2 - test",
    "MProEngProfileName",
    "Id",
    "FUNCTIONALITY",
    Setup2L,
    GetIdTestL,
    Teardown)

EUNIT_TEST(
    "Get Profile Name 2 - test",
    "MProEngProfileName",
    "Name",
    "FUNCTIONALITY",
    Setup2L,
    GetNameTestL,
    Teardown)

EUNIT_TEST(
    "Set Profile Name 2 - test",
    "MProEngProfileName",
    "SetNameL",
    "FUNCTIONALITY",
    Setup2L,
    SetNameTestL,
    Teardown)

EUNIT_TEST(
    "Get Profile ID 3 - test",
    "MProEngProfileName",
    "Id",
    "FUNCTIONALITY",
    Setup3L,
    GetIdTestL,
    Teardown)

EUNIT_TEST(
    "Get Profile Name 3 - test",
    "MProEngProfileName",
    "Name",
    "FUNCTIONALITY",
    Setup3L,
    GetNameTestL,
    Teardown)

EUNIT_TEST(
    "Set Profile Name 3 - test",
    "MProEngProfileName",
    "SetNameL",
    "FUNCTIONALITY",
    Setup3L,
    SetNameTestL,
    Teardown)

EUNIT_TEST(
    "Get Profile ID 4 - test",
    "MProEngProfileName",
    "Id",
    "FUNCTIONALITY",
    Setup4L,
    GetIdTestL,
    Teardown)

EUNIT_TEST(
    "Get Profile Name 4 - test",
    "MProEngProfileName",
    "Name",
    "FUNCTIONALITY",
    Setup4L,
    GetNameTestL,
    Teardown)

EUNIT_TEST(
    "Set Profile Name 4 - test",
    "MProEngProfileName",
    "SetNameL",
    "FUNCTIONALITY",
    Setup4L,
    SetNameTestL,
    Teardown)

#ifdef __OFFLINE_MODE
EUNIT_TEST(
    "Get Profile ID 5 - test",
    "MProEngProfileName",
    "Id",
    "FUNCTIONALITY",
    Setup5L,
    GetIdTestL,
    Teardown)

EUNIT_TEST(
    "Get Profile Name 5 - test",
    "MProEngProfileName",
    "Name",
    "FUNCTIONALITY",
    Setup5L,
    GetNameTestL,
    Teardown)

EUNIT_TEST(
    "Set Profile Name 5 - test",
    "MProEngProfileName",
    "SetNameL",
    "FUNCTIONALITY",
    Setup5L,
    SetNameTestL,
    Teardown)
#endif // __OFFLINE_MODE

#ifdef __DRIVE_MODE
EUNIT_TEST(
    "Get Profile ID 6 - test",
    "MProEngProfileName",
    "Id",
    "FUNCTIONALITY",
    Setup6L,
    GetIdTestL,
    Teardown)

EUNIT_TEST(
    "Get Profile Name 6 - test",
    "MProEngProfileName",
    "Name",
    "FUNCTIONALITY",
    Setup6L,
    GetNameTestL,
    Teardown)

EUNIT_TEST(
    "Set Profile Name 6 - test",
    "MProEngProfileName",
    "SetNameL",
    "FUNCTIONALITY",
    Setup6L,
    SetNameTestL,
    Teardown)
#endif // __DRIVE_MODE

EUNIT_END_TEST_TABLE


