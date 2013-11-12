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
#include "MT_CPhoneHandler.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <EUnitDecorators.h>
#include <PhoneHandler.h>
#include <RemConInterfaceSelector.h>

//  INTERNAL INCLUDES


// CONSTRUCTION
MT_CPhoneHandler* MT_CPhoneHandler::NewL()
    {
    MT_CPhoneHandler* self = MT_CPhoneHandler::NewLC();
    CleanupStack::Pop();

    return self;
    }

MT_CPhoneHandler* MT_CPhoneHandler::NewLC()
    {
    MT_CPhoneHandler* self = new( ELeave ) MT_CPhoneHandler();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
MT_CPhoneHandler::~MT_CPhoneHandler()
    {
    }

// Default constructor
MT_CPhoneHandler::MT_CPhoneHandler()
    {
    }

// Second phase construct
void MT_CPhoneHandler::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void MT_CPhoneHandler::EmptySetupL(  )
    {
    }
    

void MT_CPhoneHandler::EmptyTeardown(  )
    {
    }
    

void MT_CPhoneHandler::T_CPhoneHandler_NewL1L(  )
    {
	CPhoneHandler* pHandler = CPhoneHandler::NewL();
	CleanupStack::PushL( pHandler );
	EUNIT_ASSERT_DESC( pHandler != NULL, "An instance of CPhoneHandler should be created");
	CleanupStack::PopAndDestroy( pHandler );
    }
    
void MT_CPhoneHandler::T_CPhoneHandler_NewL2L(  )
    {
    CRemConInterfaceSelector* selecter = CRemConInterfaceSelector::NewL();
    CleanupStack::PushL( selecter );
    
	CPhoneHandler* pHandler = CPhoneHandler::NewL( *selecter );
	CleanupStack::PushL( pHandler );
	
	EUNIT_ASSERT_DESC( pHandler != NULL, "An instance of CPhoneHandler should be created");
	
	CleanupStack::PopAndDestroy( pHandler );
	CleanupStack::PopAndDestroy( selecter );
    }    

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    MT_CPhoneHandler,
    "CPhoneHandler test suite",
    "MODULE" )

EUNIT_TEST(
    "NewL case1 and Destructor test",
    "CPhoneHandler",
    "NewL and Destructor",
    "FUNCTIONALITY",
    EmptySetupL, T_CPhoneHandler_NewL1L, EmptyTeardown)

EUNIT_TEST(
    "NewL case2 test",
    "CPhoneHandler",
    "NewL",
    "FUNCTIONALITY",
    EmptySetupL, T_CPhoneHandler_NewL2L, EmptyTeardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE
