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
* Description:  Implementation of the MT_ProEngFactory test class.
*
*/
////////////////////////////////////////////////////////////////////////
// MT_ProEngFactory.cpp
//

//  CLASS HEADER
#include "MT_ProEngFactory.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <ProEngFactory.h>
#include <MProEngEngine.h>
#include <MProEngAlertToneSeeker.h>
#include <MProEngNotifyHandler.h>

//  INTERNAL INCLUDES


// CONSTRUCTION
MT_ProEngFactory* MT_ProEngFactory::NewL()
    {
    MT_ProEngFactory* self = MT_ProEngFactory::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

MT_ProEngFactory* MT_ProEngFactory::NewLC()
    {
    MT_ProEngFactory* self = new( ELeave ) MT_ProEngFactory();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
MT_ProEngFactory::~MT_ProEngFactory()
    {
    iFs.Close();
    }

// Default constructor
MT_ProEngFactory::MT_ProEngFactory()
    {
    }

// Second phase construct
void MT_ProEngFactory::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    User::LeaveIfError( iFs.Connect() );
    }

//  METHODS



void MT_ProEngFactory::SetupL(  )
    {

    } 

void MT_ProEngFactory::Teardown(  )
    {

    }

void MT_ProEngFactory::MT_ProEngFactory_NewEngineLL(  )
    {
    MProEngEngine* engine = ProEngFactory::NewEngineL();
    EUNIT_ASSERT( engine );
    engine->Release();
    engine = NULL;

    // Test deletion through destructor:
    engine = ProEngFactory::NewEngineL();
    EUNIT_ASSERT( engine );
    delete engine;
    }

void MT_ProEngFactory::MT_ProEngFactory_NewEngineLCL(  )
    {
    MProEngEngine* engine = ProEngFactory::NewEngineLC();
    CleanupStack::Pop(); // engine
    EUNIT_ASSERT( engine );
    engine->Release();
    }

void MT_ProEngFactory::MT_ProEngFactory_NewEngineL_1L(  )
    {
    MProEngEngine* engine = ProEngFactory::NewEngineL( iFs );
    EUNIT_ASSERT( engine );
    engine->Release();
    }

void MT_ProEngFactory::MT_ProEngFactory_NewEngineLC_1L(  )
    {
    MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
    CleanupStack::Pop(); // engine
    EUNIT_ASSERT( engine );
    engine->Release();
    }

void MT_ProEngFactory::MT_ProEngFactory_NewNotifyHandlerLL(  )
    {
    MProEngNotifyHandler* handler = ProEngFactory::NewNotifyHandlerL();
    EUNIT_ASSERT( handler );
    delete handler;
    }

void MT_ProEngFactory::MT_ProEngFactory_NewAlertToneSeekerLL(  )
    {
    MProEngAlertToneSeeker* seeker = ProEngFactory::NewAlertToneSeekerL();
    EUNIT_ASSERT( seeker );
    delete seeker;
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    MT_ProEngFactory,
    "Profiles Wrapper Factory test cases.",
    "MODULE" )

EUNIT_TEST(
    "NewEngineL - test ",
    "ProEngFactory",
    "NewEngineL",
    "FUNCTIONALITY",
    SetupL, MT_ProEngFactory_NewEngineLL, Teardown)

EUNIT_TEST(
    "NewEngineLC - test ",
    "ProEngFactory",
    "NewEngineLC",
    "FUNCTIONALITY",
    SetupL, MT_ProEngFactory_NewEngineLCL, Teardown)

EUNIT_TEST(
    "NewEngineL - test ",
    "ProEngFactory",
    "NewEngineL",
    "FUNCTIONALITY",
    SetupL, MT_ProEngFactory_NewEngineL_1L, Teardown)

EUNIT_TEST(
    "NewEngineLC - test ",
    "ProEngFactory",
    "NewEngineLC",
    "FUNCTIONALITY",
    SetupL, MT_ProEngFactory_NewEngineLC_1L, Teardown)

EUNIT_TEST(
    "NewNotifyHandlerL - test ",
    "ProEngFactory",
    "NewNotifyHandlerL",
    "FUNCTIONALITY",
    SetupL, MT_ProEngFactory_NewNotifyHandlerLL, Teardown)

EUNIT_TEST(
    "NewAlertToneSeekerL - test ",
    "ProEngFactory",
    "NewAlertToneSeekerL",
    "FUNCTIONALITY",
    SetupL, MT_ProEngFactory_NewAlertToneSeekerLL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
