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


#include "eunittools.h"

#include <eunitmacros.h>
#include <ecom/ecom.h>
#include <badesca.h>

#include <ximpbase.h>
#include <ximpclient.h>
#include <ximpcontext.h>
#include <ximpcontextstateevent.h>
#include <ximpcontextstate.h>
#include <ximpobjectfactory.h>
#include <presencepublishing.h>
#include <presentitypresenceevent.h>
#include <presenceobjectfactory.h>
#include <presencefeatures.h>
#include <presenceinfofield.h>
#include <presenceinfofieldcollection.h>
#include <servicepresenceinfo.h>
#include <devicepresenceinfo.h>
#include <personpresenceinfo.h>
#include <presenceinfofieldvalueenum.h>
#include <presenceinfofieldvaluetext.h>
#include <presenceinfo.h>

#include "t_presencepublishing.h"

#include "prfwtestmessaging.h"
#include "prfwtestmessenger.h"
#include "prfwtestcontextwrappermgr.h"
#include "prfwtestcontextwrapper.h"

#include "prfwtestpresencedatautils.h"

#include "ximprequestcompleteeventimp.h"
#include "ximpcontextstateeventimp.h"
#include "ximpcontextstateimp.h"
#include "ximpstatusimp.h"
#include "ownpresenceeventimp.h"

// new test stuff
#include "prfwtestlistener.h"
#include "prfwtestfilesrvmsg.h"
#include "prfwtestfiletool.h"


#include "prfwtestrobustnesstools.h"

_LIT8( KPrefix1, "PreA_" );
_LIT8( KPrefix2, "PreB_" );
_LIT8( KPrefix3, "PreC_" );
_LIT8( KPrefix4, "PreD_" );

_LIT8( KServiceType, "test_service" );
_LIT(  KStatusMsg, "This is my status." );

const TInt KTestAvailability = 1;

// ============================ MEMBER FUNCTIONS ===============================

// CONSTRUCTION
T_PresencePublishing* T_PresencePublishing::NewL()
    {
    T_PresencePublishing* self = new( ELeave ) T_PresencePublishing;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


T_PresencePublishing::~T_PresencePublishing()
    {
    }


void T_PresencePublishing::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


T_PresencePublishing::T_PresencePublishing()
    {
    }



// ===========================================================================
// TEST CASE SETUP & TEARDOWN METHODS
// ===========================================================================
//

void T_PresencePublishing::Setup_L()
    {
    PrfwTestRobustness::DoPreCleaning();
  
    iWrapperMgr = CXIMPTestContextWrapperMgr::NewL();
    iWrapperMgr->CreateWrapperL();
    }

void T_PresencePublishing::SetupMultiple_L()
    {
    PrfwTestRobustness::DoPreCleaning();

    iWrapperMgr = CXIMPTestContextWrapperMgr::NewL();
    iWrapperMgr->CreateWrapperL();
    iWrapperMgr->CreateWrapperL();
    iWrapperMgr->CreateWrapperL();
    }


void T_PresencePublishing::BindL()
    {
    CXIMPTestContextWrapper* wrapper0 = iWrapperMgr->GetWrapperL( 0 );

    wrapper0->BindL( 0 );
    }

void T_PresencePublishing::Bind2L()
    {
    CXIMPTestContextWrapper* wrapper0 = iWrapperMgr->GetWrapperL( 0 );
    wrapper0->BindWithSettingsL( 0 );
    }
    
void T_PresencePublishing::BindAllL()
    {
    for( TInt a = 0; a < iWrapperMgr->WrapperCount(); ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        wrapper->BindL( 0 );
        }
    }


void T_PresencePublishing::Teardown()
    {
    delete iWrapperMgr;
    iWrapperMgr = NULL;

    REComSession::FinalClose();

    PrfwTestRobustness::DoPreCleaning();
    }

void T_PresencePublishing::UnbindL()
    {
    // Do unbind
    CXIMPTestContextWrapper* wrapper0 = iWrapperMgr->GetWrapperL( 0 );
    wrapper0->UnbindL();
    }

void T_PresencePublishing::UnbindAllL()
    {
    for( TInt a = 0; a < iWrapperMgr->WrapperCount(); ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        wrapper->UnbindL();
        }
    }


// ===========================================================================
// TEST CASES for presence publish
// ===========================================================================
void T_PresencePublishing::T_Publish_Single_L()
    {
    EUNIT_PRINT( _L("Single Presence Publish test") );

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresencePublishing& presPub = presFeat->PresencePublishing();

    // Generate a doc
    CPresenceInfoImp* presInfo =
            XIMPTestPresenceDataUtils::GenerateInfoLC( &KPrefix1 );

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Publish the presence
    TXIMPRequestId reqId = presPub.PublishOwnPresenceL( *presInfo );

    // Setup status event listener
    CXIMPRequestCompleteEventImp* evReqComplete =
                            CXIMPRequestCompleteEventImp::NewLC( reqId );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // Wait for events on the request
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Publishing presence failed" );
    

    COMMONASSERT( messenger, EXIMPPlgTestPublishPresenceCalled, "PublishPresenceL was not called", ETrue, NULL );

    CleanupStack::PopAndDestroy( presInfo );
    CleanupStack::PopAndDestroy( listener2 );
    UnbindL();
    }

void T_PresencePublishing::T_NetworkUnplugError_L()
	{
    EUNIT_PRINT( _L("Error from plug-in in publish own presence") );
    
    // Creating the error which comes from KErrTimedOut and protocol sends Connection terminated.

// BIND AND SETUP

    Bind2L();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();


    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresencePublishing& presPub = presFeat->PresencePublishing();


// PUBLISH OWN

    //messenger->SetLeave( KErrAbort );
	messenger->SetError(KErrTimedOut);
	
    // publish own presence in here
    MPresenceInfo* info = CreateInfoLC();

    TXIMPRequestId reqId = presPub.PublishOwnPresenceL( *info );
    
   
    // Setup status event listener for RequestComplete
    listener2->Reset();
    // request complete with KErrAbort
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqId );
    evReqComplete->CompletionResultImp().SetResultCode( KErrTimedOut );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );
    
    CXIMPContextStateEventImp* evContextEvent = CXIMPContextStateEventImp::NewLC();
    evContextEvent->ContextStateImp().SetStateValue( MXIMPContextState::EUnbinding);
    listener2->ExpectL( evContextEvent );
    CleanupStack::Pop( evContextEvent );
    
    CXIMPContextStateEventImp* evContextEvent2 = CXIMPContextStateEventImp::NewLC();
    evContextEvent2->ContextStateImp().SetStateValue( MXIMPContextState::EInactive);
    listener2->ExpectL( evContextEvent2 );
    CleanupStack::Pop( evContextEvent2 );
    
    listener2->WaitAndAssertL();

// UNBIND

    CleanupStack::PopAndDestroy(1);//info
    CleanupStack::PopAndDestroy( listener2 );

    messenger->SetError( KErrNone );

    UnbindL();
	}    


void T_PresencePublishing::T_NetworkUnplugErrorMultiple_L()
	{
    EUNIT_PRINT( _L("Network disconnect from plug-in in publish own presence, multiple contexts") );

	// Bind 2 contexts
	BindAllL();

	/* 
	 * Make the 1st client publish someting and then the disconnect would happen 
	 */
	CXIMPTestContextWrapper* wrapper0 = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context0 = wrapper0->GetContext();
    CXIMPTestMessenger* messenger0 = wrapper0->GetMessenger();


    CXIMPTestListener* listener0 = CXIMPTestListener::NewL( context0);
    CleanupStack::PushL( listener0);

    MPresenceFeatures* presFeat0 = wrapper0->GetPresenceFeatures();
    MPresencePublishing& presPub0 = presFeat0->PresencePublishing();

    //messenger->SetLeave( KErrAbort );
	messenger0->SetError(KErrTimedOut);
	

    // publish own presence in here
    MPresenceInfo* info = CreateInfoLC();
    TXIMPRequestId reqId = presPub0.PublishOwnPresenceL( *info );

    /* Setup status event listener for RequestComplete for client 1 */
    listener0->Reset();
    // request complete with KErrTimedOut
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqId );
    evReqComplete->CompletionResultImp().SetResultCode( KErrTimedOut );
    listener0->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );
    
    // Create the context state events for unbinding
    CXIMPContextStateEventImp* evContextEvent = CXIMPContextStateEventImp::NewLC();
    evContextEvent->ContextStateImp().SetStateValue( MXIMPContextState::EUnbinding);
    listener0->ExpectL( evContextEvent );
    CleanupStack::Pop( evContextEvent );
    
    CXIMPContextStateEventImp* evContextEvent2 = CXIMPContextStateEventImp::NewLC();
    evContextEvent2->ContextStateImp().SetStateValue( MXIMPContextState::EInactive);
    listener0->ExpectL( evContextEvent2 );
    CleanupStack::Pop( evContextEvent2 );

  
   
   	/* 
   	 * Create a request for client 2, this will not be processed by the protocol plug-in 
   	 */
	CXIMPTestContextWrapper* wrapper1 = iWrapperMgr->GetWrapperL( 1 );
    MXIMPContext* context1 = wrapper1->GetContext();

    MPresenceFeatures* presFeat1 = wrapper1->GetPresenceFeatures();
    MPresencePublishing& presPub1 = presFeat1->PresencePublishing();


    // Create the empty presence info filter
    CPresenceInfoFilterImp* pif = CPresenceInfoFilterImp::NewLC();
    TXIMPRequestId reqId1 = presPub1.SubscribeOwnPresenceL( *pif );
	CleanupStack::PopAndDestroy(pif);

    // Setup status event listener for RequestComplete
    CXIMPTestListener* listener1 = CXIMPTestListener::NewL( context1);
    CleanupStack::PushL( listener1);
    listener1->Reset();
    // normal request complete
    CXIMPRequestCompleteEventImp* evReqComplete1 = CXIMPRequestCompleteEventImp::NewLC( reqId1 );
    evReqComplete1->CompletionResultImp().SetResultCode( KErrTimedOut );
    listener1->ExpectL( evReqComplete1 );
    CleanupStack::Pop( evReqComplete1 );

    // Create the context state events for unbinding
    evContextEvent = CXIMPContextStateEventImp::NewLC();
    evContextEvent->ContextStateImp().SetStateValue( MXIMPContextState::EUnbinding);
    listener1->ExpectL( evContextEvent );
    CleanupStack::Pop( evContextEvent );
    
    evContextEvent2 = CXIMPContextStateEventImp::NewLC();
    evContextEvent2->ContextStateImp().SetStateValue( MXIMPContextState::EInactive);
    listener1->ExpectL( evContextEvent2 );
    CleanupStack::Pop( evContextEvent2 );


    // Wait for events on the request
    
    EUNIT_ASSERT_DESC( KErrNone == listener0->WaitAndAssertL(), "Publish own presence failed in client 1");

    EUNIT_ASSERT_DESC( KErrNone == listener1->WaitAndAssertL(), "SubscribeOwnPresence failed in client 2" );

    CleanupStack::PopAndDestroy( listener1 );
    CleanupStack::PopAndDestroy(1);//info
    CleanupStack::PopAndDestroy( listener0 );
		
	// Finish the test case	
	UnbindAllL();	
	}

void T_PresencePublishing::T_Publish_ErrFromPlg_Leave_L()
    {
    EUNIT_PRINT( _L("Single Presence Publish test") );
    EUNIT_PRINT( _L("Plugin leaves from PublishOwnPresenceL") );

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresencePublishing& presPub = presFeat->PresencePublishing();

    // Generate a doc
    CPresenceInfoImp* presDoc =
                XIMPTestPresenceDataUtils::GenerateInfoLC( &KPrefix1 );

    // Setup status event listener
    SetupListenerReqCompleteL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // Tell the test protocol to leave on next operation
    messenger->SetLeave( KErrAbort );

    // Publish the presence
    TXIMPRequestId reqId = presPub.PublishOwnPresenceL( *presDoc );
    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Publishing single presence, leave with error: ") );

    // Verify that plugin left and didn't go through the PublishOwnPresenceL.
    COMMONASSERT( messenger, EXIMPPlgTestPublishPresenceCalled, "PublishPresenceL was not called",
                                                EFalse, "PublishPresenceL was called." );


    // TODO wait and verify the events
    EUNIT_FAIL_TEST( "Under construction." );

    CleanupStack::PopAndDestroy( presDoc );
    CleanupStack::PopAndDestroy( listener2 );
    UnbindL();
    }

void T_PresencePublishing::T_Publish_ErrFromPlg_CompleteWithError_L()
    {
    EUNIT_PRINT( _L("Single Presence Publish test") );
    EUNIT_PRINT( _L("Plugin completes PublishOwnPresenceL with error") );

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresencePublishing& presPub = presFeat->PresencePublishing();

    // Generate a doc
    CPresenceInfoImp* presDoc =
                XIMPTestPresenceDataUtils::GenerateInfoLC( &KPrefix1 );

    // Setup status event listener:
    //TODO  Setup presence event listener
    SetupListenerReqCompleteL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // Tell the test protocol to return with error on next operation
    messenger->SetError( KErrAbort );

    // Publish the presence
    TXIMPRequestId reqId = presPub.PublishOwnPresenceL( *presDoc );
    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Publishing single presence, complete with error: ") );

    // Verify that we did go through PublishOwnPresenceL.
    COMMONASSERT( messenger, EXIMPPlgTestPublishPresenceCalled, "PublishPresenceL was not called", ETrue, NULL );

    // TODO wait and verify presence events
    EUNIT_FAIL_TEST( "Under construction." );

    CleanupStack::PopAndDestroy( presDoc );
    CleanupStack::PopAndDestroy( listener2 );

    UnbindL();
    }


// ===========================================================================
// TEST CASES for presence interest registration
// ===========================================================================

// Single client, empty filter
void T_PresencePublishing::T_SubscribeOwnPresence_Single_Empty_L()
    {
    EUNIT_PRINT( _L("Single interest registration") );

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();


    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresencePublishing& presPub = presFeat->PresencePublishing();

    // Create the empty presence info filter
    CPresenceInfoFilterImp* pif = CPresenceInfoFilterImp::NewLC();

    // Setup status event listener for RequestComplete
    listener2->Reset();
    // normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // subscribe own presence
    TXIMPRequestId reqId = presPub.SubscribeOwnPresenceL( *pif );

    // Wait for events on the request
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "SubscribeOwnPresence failed" );

    // verify that SubscribeOwnPresenceL was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribeOwnPresenceCalled, "SubscribeOwnPresenceL was not called", ETrue, NULL );

    // Setup status event listener for RequestComplete
    listener2->Reset();
    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );
  
    // unsubscribe own presence
    reqId = presPub.UnsubscribeOwnPresenceL();

    // Wait for events on the request
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "UnsubscribeOwnPresence failed" );
    
    // verify that UnsubscribeOwnPresenceL was called
    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribeOwnPresenceCalled, "UnsubscribeOwnPresenceL was not called", ETrue, NULL );

    CleanupStack::PopAndDestroy( pif );
    CleanupStack::PopAndDestroy( listener2 );
    UnbindL();
    }

// Single client, empty filter
void T_PresencePublishing::T_SubscribeOwnPresence_Multiple_Empty_L()
    {
    EUNIT_PRINT( _L("Multiple client filter registration") );

    BindAllL();

    // Create the empty presence info filter
    CPresenceInfoFilterImp* pif = CPresenceInfoFilterImp::NewLC();

    TInt countOfWrappers = iWrapperMgr->WrapperCount();

    CXIMPTestListener* listeners[ 50 ];
    TXIMPRequestId reqIdDummy;  // not checked



// Setup listeners
    for( TInt a = 0; a < countOfWrappers; a++ )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        listeners[ a ] = CXIMPTestListener::NewL( context );
        CleanupStack::PushL( listeners[ a ] );
        }


// SUBSCRIBE
    for( TInt a = countOfWrappers - 1; a > 0 ; --a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Tell the protocol it should act normally
        messenger->SetNoError();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresencePublishing& presPub = presFeat->PresencePublishing();

        // Setup status event listener for RequestComplete
        listeners[ a ]->Reset();
        CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
        listeners[ a ]->ExpectL( evReqComplete );
        CleanupStack::Pop( evReqComplete );

        messenger->SetValueFor( EXIMPPlgTestSubscribeOwnPresenceCalled, 0 );
        // subscribe own presence
        TXIMPRequestId reqId = presPub.SubscribeOwnPresenceL( *pif );

        // Wait for events on the request
        EUNIT_ASSERT_DESC( KErrNone == listeners[ a ]->WaitAndAssertL(), "SubscribeOwnPresence failed" );

        // verify that SubscribeOwnPresenceL was called
        COMMONASSERT( messenger, EXIMPPlgTestSubscribeOwnPresenceCalled, "SubscribeOwnPresenceL was not called", 
        ( a > 1 ) ? ETrue : EFalse , "SubscribeOwnPresenceL was called." );
        }
        
// REFRESH

    for( TInt a = countOfWrappers - 1; a > 0 ; --a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Tell the protocol it should act normally
        messenger->SetNoError();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresencePublishing& presPub = presFeat->PresencePublishing();

        // Setup status event listener for RequestComplete
        listeners[ a ]->Reset();
        CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
        listeners[ a ]->ExpectL( evReqComplete );
        CleanupStack::Pop( evReqComplete );
        
        CXIMPDataSubscriptionStateImp* stateImp = CXIMPDataSubscriptionStateImp::NewLC();
        stateImp->SetDataStateL( MXIMPDataSubscriptionState::EDataUnavailable );
        stateImp->SetSubscriptionStateL( MXIMPDataSubscriptionState::ESubscriptionActive );
        COwnPresenceEventImp* evOwnPresence = COwnPresenceEventImp::NewLC( stateImp );
        CleanupStack::Pop( 2, stateImp );
        CleanupStack::PushL( evOwnPresence );
        listeners[ a ]->ExpectL( evOwnPresence );
        CleanupStack::Pop( evOwnPresence );
        
        // TODO check if this call is still needed
        messenger->SetValueFor( EXIMPPlgTestSubscribeOwnPresenceCalled, 0 );

        // subscribe own presence
        TXIMPRequestId reqId = presPub.SubscribeOwnPresenceL( *pif );

        // Wait for events on the request
        EUNIT_ASSERT_DESC( KErrNone == listeners[ a ]->WaitAndAssertL(), "SubscribeOwnPresence failed" );
        
        // verify that SubscribeOwnPresenceL was called
        COMMONASSERT( messenger, EXIMPPlgTestSubscribeOwnPresenceCalled,
                        "SubscribeOwnPresenceL was not called", 
                        EFalse , "SubscribeOwnPresenceL was called." );
        }

        
// UNSUBSCRIBE
        
    for( TInt a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresencePublishing& presPub = presFeat->PresencePublishing();

        // Setup status event listener for RequestComplete
        listeners[ a ]->Reset();
        CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
        listeners[ a ]->ExpectL( evReqComplete );
        CleanupStack::Pop( evReqComplete );
        

        // register interest
        TXIMPRequestId reqId = presPub.UnsubscribeOwnPresenceL();

        // Wait for events on the request
        EUNIT_ASSERT_DESC( KErrNone == listeners[ a ]->WaitAndAssertL(), "UnsubscribeOwnPresence failed" );

 
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestUnsubscribeOwnPresenceCalled, 
            "UnsubscribeOwnPresenceL was not called",
            ( a < 2 ) ? EFalse : ETrue,
            "UnsubscribeOwnPresenceL was called" );
        }


    // delete the listeners
    CleanupStack::PopAndDestroy( countOfWrappers ); // the contents of listeners[]

    CleanupStack::PopAndDestroy( pif );
    
    UnbindAllL();
    }


// Single client, filled filter
void T_PresencePublishing::T_SubscribeOwnPresence_Single_L()
    {
    EUNIT_PRINT( _L("Single interest registration") );

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresencePublishing& presPub = presFeat->PresencePublishing();

    // Create the filled interest document
    CPresenceInfoFilterImp* pif =
                XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

    // Tell the protocol it should act normally
    messenger->SetNoError();


// SUBSCRIBE

    // subscribe own presence
    TXIMPRequestId reqId = presPub.SubscribeOwnPresenceL( *pif );

    // Setup status event listener for RequestComplete
    listener2->Reset();
    SetupListenerReqCompleteL( wrapper, EXIMPTestStatusEvents_RequestOk );
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqId );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // Wait for events on the request
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "SubscribeOwnPresence failed" );

    // verify that SubscribeOwnPresenceL was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribeOwnPresenceCalled, "SubscribeOwnPresenceL was not called", ETrue, NULL );    

// REFRESH

    // subscribe own presence (refresh)
    reqId = presPub.SubscribeOwnPresenceL( *pif );

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteL( wrapper, EXIMPTestStatusEvents_RequestOk );
    messenger->SetValueFor( EXIMPPlgTestSubscribeOwnPresenceCalled, 0 );
    listener2->Reset();
    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqId );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );
    // also the event
    
    CXIMPDataSubscriptionStateImp* stateImp = CXIMPDataSubscriptionStateImp::NewLC();
    stateImp->SetDataStateL( MXIMPDataSubscriptionState::EDataUnavailable );
    stateImp->SetSubscriptionStateL( MXIMPDataSubscriptionState::ESubscriptionActive );    
    COwnPresenceEventImp* event = COwnPresenceEventImp::NewLC( stateImp );
    CleanupStack::Pop( 2, stateImp );
    CleanupStack::PushL( event );    
    listener2->ExpectL( event );
    CleanupStack::Pop( event );

    // Wait for events on the request
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Refreshing own presence subscription failed" );

    // verify that SubscribeOwnPresenceL was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribeOwnPresenceCalled, "SubscribeOwnPresenceL was not called", EFalse, "SubscribeOwnPresenceL was called" );

    CleanupStack::PopAndDestroy( pif );
    CleanupStack::PopAndDestroy( listener2 );
    UnbindL();
    }

void T_PresencePublishing::T_ErrFromPlg_LeaveOnSubscribeOwnPresence_L()
    {
    EUNIT_PRINT( _L("Error from plug-in in presentity presence subscribe") );
    EUNIT_PRINT( _L("Plug-in leaves from SubscribePresence().") );

// BIND AND SETUP

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();


    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresencePublishing& presPub = presFeat->PresencePublishing();

    // Create the filled interest document
    CPresenceInfoFilterImp* pif =
                XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

// SUBSCRIBE

    messenger->SetLeave( KErrAbort );

    // subscribe own presence
    TXIMPRequestId reqId = presPub.SubscribeOwnPresenceL( *pif );

    // Setup status event listener for RequestComplete
    listener2->Reset();
    // request complete with KErrAbort
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqId );
    evReqComplete->CompletionResultImp().SetResultCode( KErrAbort );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // Wait for events on the request
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribing presentity presence, didn't complete with error" );
    
    // verify that SubscribeOwnPresenceL was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribeOwnPresenceCalled, "SubscribeOwnPresenceL was not called", EFalse, "SubscribeOwnPresenceL was called" );
    
        
// SUBSCRIBE

    messenger->SetLeave( KErrNone );

    // subscribe own presence
    reqId = presPub.SubscribeOwnPresenceL( *pif );

    // Setup status event listener for RequestComplete
    listener2->Reset();
    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqId );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // Wait for events on the request
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribing presentity presence failed" );

    // verify that SubscribeOwnPresenceL was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribeOwnPresenceCalled, "SubscribeOwnPresenceL was not called", ETrue, "SubscribeOwnPresenceL was called" );
    

// UNSUBSCRIBE

    messenger->SetLeave( KErrAbort );

    // register interest
    reqId = presPub.UnsubscribeOwnPresenceL();

    // Setup status event listener for RequestComplete
    listener2->Reset();
    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqId );
    evReqComplete->CompletionResultImp().SetResultCode( KErrAbort );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // Wait for events on the request
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsubscribing presentity presence with error failed" );

    // verify that UnsubscribeOwnPresenceL was called
    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribeOwnPresenceCalled, "UnsubscribeOwnPresenceL was not called", EFalse, 
                                                                       "UnsubscribeOwnPresenceL was called." );
    
// UNBIND

    CleanupStack::PopAndDestroy( pif );
    CleanupStack::PopAndDestroy( listener2 );

    messenger->SetLeave( KErrNone );

    UnbindL();
    }

// ===========================================================================
// Single client, filled filter, publish.
// ===========================================================================
//

void T_PresencePublishing::T_SubscribeOwnPresence_Publish_Single_L()
    {
    EUNIT_PRINT( _L("Single interest registration with publish") );

    /*
     * TODO: 
     *
     * case 1
     *
     * - subscribetä oma presence
     * - luo filtteri
     * - luo info
     * - publishaa oma presence + info
     * - tsekkaa että alakerta on saanut infon
     * - subscribetä uudelleen (=refresh case)
     * - tsekkaa että kaikki ok
     *
     * case 2
     *
     * - sama filtteri kuin edellä
     * - feikkaa verkkoserveriltä tuleva viesti, jossa sama info kuin edellä
     * - tsekkaa, että frameworkin läpi tuleva info sisältää vain ne kamat
     *   jotka on sallittu ja että muut on blokattu. ts. "test_service":n
     *   "availability" pitäisi tulla läpi. person-presenceen tungettu kama jää
     *   pois
     *
     */

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresencePublishing& presPub = presFeat->PresencePublishing();

    // Create the filter 
    MPresenceInfoFilter* pif = presFeat->PresenceObjectFactory().NewPresenceInfoFilterLC();
    pif->AcceptServiceFilterL( KServiceType, NPresenceInfo::NFieldType::KAvailability ); // "test_service", "availability"
    // note, the person info will be filtered out now since we don't accept it.

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // SUBSCRIBE

    // subscribe own presence
    TXIMPRequestId reqId = presPub.SubscribeOwnPresenceL( *pif );

    // Setup status event listener for RequestComplete
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqId );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // Wait for events on the request
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "SubscribeOwnPresence failed" );

    // verify that SubscribeOwnPresenceL was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribeOwnPresenceCalled, "SubscribeOwnPresenceL was not called", ETrue, NULL );    
    
    // publish own presence in here
    MPresenceInfo* info = CreateInfoLC();

    reqId = presPub.PublishOwnPresenceL( *info );

    // Setup status event listener
    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqId );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );
    
    CXIMPDataSubscriptionStateImp* stateImp = CXIMPDataSubscriptionStateImp::NewLC();
    stateImp->SetDataStateL( MXIMPDataSubscriptionState::EDataAvailable );
    stateImp->SetSubscriptionStateL( MXIMPDataSubscriptionState::ESubscriptionActive );    
    COwnPresenceEventImp* event = COwnPresenceEventImp::NewLC( stateImp );
    CleanupStack::Pop( 2, stateImp );
    CleanupStack::PushL( event );    
    listener2->ExpectL( event );
    CleanupStack::Pop( event );

    // Wait for events on the request
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Publishing presence failed" );
    COMMONASSERT( messenger, EXIMPPlgTestPublishPresenceCalled, "PublishPresenceL was not called", ETrue, NULL );

    // REFRESH

    // subscribe own presence (refresh)
    reqId = presPub.SubscribeOwnPresenceL( *pif );

    // Setup status event listener for RequestComplete
    messenger->SetValueFor( EXIMPPlgTestSubscribeOwnPresenceCalled, 0 );
    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqId );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );
    // also the event
    
    stateImp = CXIMPDataSubscriptionStateImp::NewLC();
    stateImp->SetDataStateL( MXIMPDataSubscriptionState::EDataAvailable );
    stateImp->SetSubscriptionStateL( MXIMPDataSubscriptionState::ESubscriptionActive );    
    event = COwnPresenceEventImp::NewLC( stateImp );
    CleanupStack::Pop( 2, stateImp );
    CleanupStack::PushL( event );    
    listener2->ExpectL( event );
    CleanupStack::Pop( event );

    // Wait for events on the request
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Refreshing own presence subscription failed" );

    // verify that SubscribeOwnPresenceL was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribeOwnPresenceCalled, "SubscribeOwnPresenceL was not called", EFalse, "SubscribeOwnPresenceL was called" );

    CleanupStack::PopAndDestroy( 3, listener2 ); // info, pif, listener2
    UnbindL();
    }


// ===========================================================================
// SETUP HELPERS FOR THE LISTENER
// ===========================================================================
//
void T_PresencePublishing::SetupListenerReqCompleteL( CXIMPTestContextWrapper* aWrapper, TXIMPTestStatusEventTemplate aEventType )
    {
    RArray< TInt32 > array;
    CleanupClosePushL( array );
    array.Append( MXIMPContextStateEvent::KInterfaceId );
    array.Append( MOwnPresenceEvent::KInterfaceId );
    aWrapper->SetupListenerReqCompleteL( aEventType, &array );
    CleanupStack::PopAndDestroy(); // array
    }

void T_PresencePublishing::SetupListenerReqCompleteAllowEventsL(  CXIMPTestContextWrapper* aWrapper, TXIMPTestStatusEventTemplate aEventType )
    {
    RArray< TInt32 > array;
    CleanupClosePushL( array );
    array.AppendL( MXIMPRequestCompleteEvent::KInterfaceId );
    array.AppendL( MXIMPContextStateEvent::KInterfaceId );
    array.AppendL( MOwnPresenceEvent::KInterfaceId );
    aWrapper->SetupListenerReqCompleteL( aEventType, &array );
    CleanupStack::PopAndDestroy(); // array
    }

MPresenceInfo* T_PresencePublishing::CreateInfoLC()
    {
    // fill up the info structure with:
    // service info imp: 
    //   - info name:    "test_service"
    //   - field name:   availability
    //   - field value:  enum type, value 1
    // person info imp:
    //   - field name:   "status-message"
    //   - field value:  "This is my status message."
    //
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();

    // Get the interface
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceInfo* info = presFeat->PresenceObjectFactory().NewPresenceInfoLC();

    // fill service info
    MServicePresenceInfo* srvInfo = presFeat->PresenceObjectFactory().NewServicePresenceInfoLC();
    srvInfo->SetServiceTypeL( KServiceType );  // test

    MPresenceInfoField* infoField = presFeat->PresenceObjectFactory().NewInfoFieldLC();
    
    MPresenceInfoFieldValueEnum* enumField = presFeat->PresenceObjectFactory().NewEnumInfoFieldLC();
    enumField->SetValueL( KTestAvailability ); // 1
    infoField->SetFieldTypeL( NPresenceInfo::NFieldType::KAvailability ); // "availability"
    infoField->SetFieldValue( enumField );
    CleanupStack::Pop(); // enumField

    srvInfo->Fields().AddOrReplaceFieldL( infoField );
    CleanupStack::Pop(); // infoField 

    info->AddServicePresenceL( srvInfo );
    CleanupStack::Pop(); // srvInfo 

    // fill person info
    MPersonPresenceInfo* persInfo = presFeat->PresenceObjectFactory().NewPersonPresenceInfoLC();
    MPresenceInfoField* infoField2 = presFeat->PresenceObjectFactory().NewInfoFieldLC();
    
    MPresenceInfoFieldValueText* textField = presFeat->PresenceObjectFactory().NewTextInfoFieldLC();
    textField->SetTextValueL( KStatusMsg );

    infoField2->SetFieldTypeL( NPresenceInfo::NFieldType::KStatusMessage );
    infoField2->SetFieldValue( textField );
    CleanupStack::Pop(); // textField

    persInfo->Fields().AddOrReplaceFieldL( infoField2 );
    CleanupStack::Pop(); // infoField2

    info->SetPersonPresenceL( persInfo );
    CleanupStack::Pop(); // persInfo
    // that's it

    return info;
    }

// ===========================================================================
// TEST CASE TABLE
// ===========================================================================
//
EUNIT_BEGIN_TEST_TABLE(
        T_PresencePublishing,
        "XIMP presence management tests",
        "MODULE" )


PRFW_DECORATED_TEST(
        "Publish single presence",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_Publish_Single_L,
        Teardown )

/*
PRFW_NOT_DECORATED_TEST(
        "Publish, error leave",
        "",
        "",
        "ERRORHANDLING",
        Setup_Bind_L,
        T_Publish_ErrFromPlg_Leave_L,
        Teardown_Unbind )

PRFW_NOT_DECORATED_TEST(
        "Publish, error complete",
        "",
        "",
        "ERRORHANDLING",
        Setup_Bind_L,
        T_Publish_ErrFromPlg_CompleteWithError_L,
        Teardown_Unbind )
*/

PRFW_DECORATED_TEST(
        "Empty own presence subscribtion",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_SubscribeOwnPresence_Single_Empty_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Empty own presence subscribtion for multiple clients",
        "",
        "",
        "FUNCTIONALITY",
        SetupMultiple_L,
        T_SubscribeOwnPresence_Multiple_Empty_L,
        Teardown )


PRFW_DECORATED_TEST(
        "Single own presence subscribtion",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_SubscribeOwnPresence_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Single own presence subscribtion with publish",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_SubscribeOwnPresence_Publish_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Error - Fail subscription",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_ErrFromPlg_LeaveOnSubscribeOwnPresence_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Error - Network UnplugError",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_NetworkUnplugError_L,
        Teardown )

/*
PRFW_DECORATED_TEST(
        "Error - Network UnplugError",
        "",
        "",
        "FUNCTIONALITY",
        SetupMultiple_L,
        T_NetworkUnplugErrorMultiple_L,
        Teardown )
*/

EUNIT_END_TEST_TABLE


// ===========================================================================
// TEST SUITE CONSTRUCTION
// ===========================================================================
//
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    return T_PresencePublishing::NewL();
    }


// end of file






