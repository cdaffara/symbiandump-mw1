/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:
*
*/


//  CLASS HEADER
#include "MT_KefMapFactory.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <EUnitDecorators.h>
#include <keffactory.h>

//  INTERNAL INCLUDES


// CONSTRUCTION
MT_KefMapFactory* MT_KefMapFactory::NewL()
    {
    MT_KefMapFactory* self = MT_KefMapFactory::NewLC();
    CleanupStack::Pop();

    return self;
    }

MT_KefMapFactory* MT_KefMapFactory::NewLC()
    {
    MT_KefMapFactory* self = new( ELeave ) MT_KefMapFactory();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
MT_KefMapFactory::~MT_KefMapFactory()
    {
    }

// Default constructor
MT_KefMapFactory::MT_KefMapFactory()
    {
    }

// Second phase construct
void MT_KefMapFactory::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void MT_KefMapFactory::SetupL(  )
    {
    }
    

void MT_KefMapFactory::Teardown(  )
    {
    }
    

void MT_KefMapFactory::T_KefMapFactory_CreateKefMapLL(  )
    {
    CKefMap* kefMap = KefMapFactory::CreateKefMapL();
    CleanupStack::PushL( kefMap );
    EUNIT_ASSERT_DESC( kefMap != NULL, "CKefMap instanc should be created");
    CleanupStack::PopAndDestroy( kefMap );
    }
    

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    MT_KefMapFactory,
    "MT_KefMapFactory test suite",
    "MODULE" )

EUNIT_TEST(
    "Test Creating CKefMap instance",
    "KefMapFactory",
    "CreateKefMapL",
    "FUNCTIONALITY",
    SetupL, T_KefMapFactory_CreateKefMapLL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE
