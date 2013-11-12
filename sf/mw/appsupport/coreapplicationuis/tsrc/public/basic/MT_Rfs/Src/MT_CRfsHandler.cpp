/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
#include "MT_CRfsHandler.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <EUnitDecorators.h>
#include <rfsHandler.h>

//  INTERNAL INCLUDES


// CONSTRUCTION
MT_CRfsHandler* MT_CRfsHandler::NewL()
    {
    MT_CRfsHandler* self = MT_CRfsHandler::NewLC();
    CleanupStack::Pop();
    return self;
    }

MT_CRfsHandler* MT_CRfsHandler::NewLC()
    {
    MT_CRfsHandler* self = new( ELeave ) MT_CRfsHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// Destructor (virtual by CBase)
MT_CRfsHandler::~MT_CRfsHandler()
    {
    delete iCRfsHandler;
    }

// Default constructor
MT_CRfsHandler::MT_CRfsHandler()
    {
    }

// Second phase construct
void MT_CRfsHandler::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
void MT_CRfsHandler::SetupEmptyL()
    {
    // Nothing to do
    }
    
void MT_CRfsHandler::TeardownEmpty()
    {
    // Nothing to do
    }

void MT_CRfsHandler::SetupL()
    {
    iCRfsHandler = new( ELeave ) CRfsHandler;
    }
    
void MT_CRfsHandler::Teardown()
    {
    delete iCRfsHandler; 
    iCRfsHandler = NULL; 
    }

void MT_CRfsHandler::SpecialSetupL(  )
    {
    SetupL();
    
    iPeriodic = CPeriodic::NewL( CActive::EPriorityStandard );
    }
    

void MT_CRfsHandler::SpecialTeardown(  )
    {
    Teardown();
    
    if ( iPeriodic )
        {
        iPeriodic->Cancel();
        }
    delete iPeriodic;    
    }    

void MT_CRfsHandler::TestCRfsHandlerL(  )
    {
    CRfsHandler* handler = new( ELeave ) CRfsHandler;
    CleanupStack::PushL( handler );
    EUNIT_ASSERT( handler != NULL );
    CleanupStack::PopAndDestroy( handler );
    }
    
void MT_CRfsHandler::TestActivateRfsLL(  )
    {
    if ( !iPeriodic->IsActive() )
    {
    iPeriodic->Start( 4000000, 1000000, TCallBack( SimulateRightSoftkey, static_cast<TAny*>( this ) ) );
    }
    
    iCRfsHandler->ActivateRfsL( ERfsNormal );
    }
    
void MT_CRfsHandler::TestCancelL(  )
    {
    iCRfsHandler->Cancel( );
     //EUNIT_ASSERT_DESC( EFalse, "Generated assert, replace with real");
    }

TInt MT_CRfsHandler::SimulateRightSoftkey( TAny* aPtr )
    {
    MT_CRfsHandler* instL = static_cast<MT_CRfsHandler*>( aPtr );
    instL->DoSimulateRightL();
    
	return KErrNone;
    }
    
void MT_CRfsHandler::DoSimulateRightL()
	{
    TKeyEvent event;
    event.iCode = EKeyCBA2;
    event.iScanCode = 0;
    event.iRepeats = 0;
    event.iModifiers = 0;

	TWsEvent wsEvent;
	*( wsEvent.Key() ) = event;
	wsEvent.SetType( EEventKey );
	wsEvent.SetTimeNow();

	RWsSession*	ws = &CCoeEnv::Static()->WsSession();      
	
	TInt wgId = ws->GetFocusWindowGroup();
	ws->SendEventToWindowGroup( wgId, wsEvent );
	
	iPeriodic->Cancel();
	}

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    MT_CRfsHandler,
    "CRfsHandler test suite",
    "MODULE" )

EUNIT_TEST(
    "Instantiate and destroy CRfsHandler",
    "CRfsHandler",
    "~CRfsHandler - test0",
    "FUNCTIONALITY",
    SetupEmptyL, TestCRfsHandlerL, TeardownEmpty )
    
EUNIT_TEST(
    "ActivateRfsL - test1",
    "CRfsHandler",
    "ActivateRfsL - test1",
    "FUNCTIONALITY",
    SpecialSetupL, TestActivateRfsLL, SpecialTeardown )
    
EUNIT_TEST(
    "Cancel - test2",
    "CRfsHandler",
    "Cancel - test2",
    "FUNCTIONALITY",
    SetupL, TestCancelL, Teardown )
    

EUNIT_END_TEST_TABLE

//  END OF FILE
