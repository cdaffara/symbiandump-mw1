/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
#include "MT_CFActionPlugIn.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <EUnitDecorators.h>
#include <ECom/ECom.h>

//  INTERNAL INCLUDES
#include <CFActionPlugin.h>
#include <cfactionindication.h>

// CONSTANTS
const TUid KTestActionPluginImplementationUid = {0x10002003};
const TInt KSecDelay = 1000000; // 1 seconds

_LIT( KTestActionIdTestAc3, "AC3" );
_LIT( KTestActionIdLeave, "AC_Leave" );

/**
* Wait info.
*/
NONSHARABLE_CLASS( TWaitInfo )
    {
    public:
    
        CPeriodic* iPeriodic;
        CActiveSchedulerWait* iWait;
    };
    
/**
* Call back method when we need to stop active scheduler wait.
*/
LOCAL_C TInt WaitCallBack( TAny* aSelf )
    {
    if( aSelf )
        {
        TWaitInfo* info = static_cast<TWaitInfo*>( aSelf );
        if( info->iPeriodic )
            {
            info->iPeriodic->Cancel();
            }
        if( info->iWait )
            {
            if( info->iWait->IsStarted() )
                {
                info->iWait->AsyncStop();
                }
            }
        }
    
    return KErrNone;
    }

/**
* Helper method to wait current scheduler before teardown is completed.
*/
LOCAL_C void Wait( TInt aIntervalInMicorseconds )
    {
    TWaitInfo info;
    
    // Construct periodic
    CPeriodic* periodic = CPeriodic::NewL( CActive::EPriorityStandard );
    CleanupStack::PushL( periodic );
    info.iPeriodic = periodic;
    
    // Construct active scheduler wait
    CActiveSchedulerWait* wait = new( ELeave ) CActiveSchedulerWait;
    CleanupStack::PushL( wait );
    info.iWait = wait;
    
    // Start timer and wait
    TCallBack cb( WaitCallBack, &info );
    periodic->Start( aIntervalInMicorseconds, aIntervalInMicorseconds, cb );
    wait->Start();
    
    // Cleanup
    CleanupStack::PopAndDestroy( wait );
    CleanupStack::PopAndDestroy( periodic );
    }


// CONSTRUCTION
MT_CCFActionPlugIn* MT_CCFActionPlugIn::NewL()
    {
    MT_CCFActionPlugIn* self = MT_CCFActionPlugIn::NewLC();
    CleanupStack::Pop();

    return self;
    }

MT_CCFActionPlugIn* MT_CCFActionPlugIn::NewLC()
    {
    MT_CCFActionPlugIn* self = new( ELeave ) MT_CCFActionPlugIn();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
MT_CCFActionPlugIn::~MT_CCFActionPlugIn()
    {
    }

// Default constructor
MT_CCFActionPlugIn::MT_CCFActionPlugIn()
    {
    }

// Second phase construct
void MT_CCFActionPlugIn::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void MT_CCFActionPlugIn::SetupL(  )
    {
    iCCFActionPlugIn = CCFActionPlugIn::NewL( KTestActionPluginImplementationUid );
    iCCFActionPlugIn->InitializeL();
    }   

void MT_CCFActionPlugIn::SetupEmptyL(  )
    {
    }   

void MT_CCFActionPlugIn::Teardown(  )
    {
    delete iCCFActionPlugIn;
    iCCFActionPlugIn = NULL;
     
    Wait( KSecDelay * 1 );

#ifdef __WINS__
    REComSession::FinalClose();
#endif
    }

void MT_CCFActionPlugIn::TeardownEmpty(  )
    {
#ifdef __WINS__
    REComSession::FinalClose();
#endif
    }

void MT_CCFActionPlugIn::MT_CCFActionPlugIn_NewL(  )
    {
    CCFActionPlugIn* obj = CCFActionPlugIn::NewL( KTestActionPluginImplementationUid );
    CleanupStack::PushL( obj );
    
    EUNIT_ASSERT_DESC( obj, "Instance not created");
    
    CleanupStack::PopAndDestroy( obj );
    
    Wait( KSecDelay * 1 );
    }
    
void MT_CCFActionPlugIn::MT_CCFActionPlugIn_NewLC(  )
    {
    CCFActionPlugIn* obj = CCFActionPlugIn::NewLC( KTestActionPluginImplementationUid );
    
    EUNIT_ASSERT_DESC( obj, "Instance not created");
    
    CleanupStack::PopAndDestroy( obj );

    Wait( KSecDelay * 1 );
    }
    
void MT_CCFActionPlugIn::MT_CCFActionPlugIn_AsyncExecutionCompletedL(  )
    {
    CCFActionIndication* actionIndication = CCFActionIndication::NewLC();
    actionIndication->SetIdentifierL( KTestActionIdTestAc3 );

    // Execute test
    EUNIT_ASSERT_NO_LEAVE( iCCFActionPlugIn->PrepareExecutionL() );
    EUNIT_ASSERT_NO_LEAVE( iCCFActionPlugIn->ExecuteL( actionIndication ) );
    Wait( KSecDelay * 5 );
    iCCFActionPlugIn->FinishedExecution();
    
    // Clean up
    CleanupStack::PopAndDestroy( actionIndication );
    }
    
void MT_CCFActionPlugIn::MT_CCFActionPlugIn_PrepareExecutionLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iCCFActionPlugIn->PrepareExecutionL( ) );
    }
    
void MT_CCFActionPlugIn::MT_CCFActionPlugIn_FinishedExecutionL(  )
    {
    iCCFActionPlugIn->FinishedExecution( );
    }
    
void MT_CCFActionPlugIn::MT_CCFActionPlugIn_ExtensionL(  )
    {
    const TUid KSomeUid = {0x12345678};
    TAny* interface = iCCFActionPlugIn->Extension( KSomeUid );
    EUNIT_ASSERT_DESC( !interface, "Invalid interface returned");
    }
    
void MT_CCFActionPlugIn::MT_CCFActionPlugIn_ExecuteLL(  )
    {
    CCFActionIndication* actionIndication = NULL;
    
    actionIndication = CCFActionIndication::NewLC();
    actionIndication->SetIdentifierL( KTestActionIdLeave );
    EUNIT_ASSERT_NO_LEAVE( iCCFActionPlugIn->PrepareExecutionL() );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iCCFActionPlugIn->ExecuteL( actionIndication ), KErrArgument );
    CleanupStack::PopAndDestroy( actionIndication );

    actionIndication = CCFActionIndication::NewLC();
    actionIndication->SetIdentifierL( KTestActionIdTestAc3 );
    EUNIT_ASSERT_NO_LEAVE( iCCFActionPlugIn->PrepareExecutionL() );
    EUNIT_ASSERT_NO_LEAVE( iCCFActionPlugIn->ExecuteL( actionIndication ) );
    Wait( KSecDelay * 5 );
    iCCFActionPlugIn->FinishedExecution();
    CleanupStack::PopAndDestroy( actionIndication );
    }
    
void MT_CCFActionPlugIn::MT_CCFActionPlugIn_GetActionsLL(  )
    {
    CDesCArrayFlat* actions = new( ELeave ) CDesCArrayFlat( 10 );
    CleanupStack::PushL( actions );
    EUNIT_ASSERT_NO_LEAVE( iCCFActionPlugIn->GetActionsL( *actions ) );
    TInt count = actions->Count();
    EUNIT_ASSERT_DESC( count == 2, "Invalid action count" );
    for( TInt i = 0; i < count; i++ )
        {
        TPtrC action( (*actions)[i] );
        EUNIT_ASSERT_DESC( action == KTestActionIdLeave || action == KTestActionIdTestAc3, "Invalid action" );
        }
    CleanupStack::PopAndDestroy( actions );
    }
    
void MT_CCFActionPlugIn::MT_CCFActionPlugIn_SecurityPolicyL(  )
    {
    iCCFActionPlugIn->SecurityPolicy();
    }
    

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    MT_CCFActionPlugIn,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL",
    "CCFActionPlugIn",
    "NewL",
    "FUNCTIONALITY",
    SetupEmptyL, MT_CCFActionPlugIn_NewL, TeardownEmpty)
    
EUNIT_TEST(
    "NewLC",
    "CCFActionPlugIn",
    "NewLC",
    "FUNCTIONALITY",
    SetupEmptyL, MT_CCFActionPlugIn_NewLC, TeardownEmpty)
    
EUNIT_TEST(
    "AsyncExecutionCompleted",
    "CCFActionPlugIn",
    "AsyncExecutionCompleted",
    "FUNCTIONALITY",
    SetupL, MT_CCFActionPlugIn_AsyncExecutionCompletedL, Teardown)
    
EUNIT_TEST(
    "PrepareExecutionL",
    "CCFActionPlugIn",
    "PrepareExecutionL",
    "FUNCTIONALITY",
    SetupL, MT_CCFActionPlugIn_PrepareExecutionLL, Teardown)
    
EUNIT_TEST(
    "FinishedExecution",
    "CCFActionPlugIn",
    "FinishedExecution",
    "FUNCTIONALITY",
    SetupL, MT_CCFActionPlugIn_FinishedExecutionL, Teardown)
    
EUNIT_TEST(
    "Extension",
    "CCFActionPlugIn",
    "Extension",
    "FUNCTIONALITY",
    SetupL, MT_CCFActionPlugIn_ExtensionL, Teardown)
    
EUNIT_TEST(
    "ExecuteL",
    "CCFActionPlugIn",
    "ExecuteL",
    "FUNCTIONALITY",
    SetupL, MT_CCFActionPlugIn_ExecuteLL, Teardown)
    
EUNIT_TEST(
    "GetActionsL",
    "CCFActionPlugIn",
    "GetActionsL",
    "FUNCTIONALITY",
    SetupL, MT_CCFActionPlugIn_GetActionsLL, Teardown)
    
EUNIT_TEST(
    "SecurityPolicy",
    "CCFActionPlugIn",
    "SecurityPolicy",
    "FUNCTIONALITY",
    SetupL, MT_CCFActionPlugIn_SecurityPolicyL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE
