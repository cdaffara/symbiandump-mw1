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


#include <presencefeatures.h>
#include <presenceauthorization.h>
#include <presenceblocklistevent.h>
#include <presenceblockinfo.h>


#include <ximpclient.h>
#include <ximpcontext.h>
#include <presencefeatures.h>

#include "t_presenceblocking.h"

// blocking includes
#include "presenceblockinfoimp.h"
#include "presenceblocklisteventimp.h"

// other datamodel and utils includes
#include "ximpidentityimp.h"
#include "ximpobjecthelpers.h"


// testing tool includes
#include "prfwtestpresencedatautils.h"

#include "prfwtestmessaging.h"
#include "prfwtestmessenger.h"
#include "prfwtestcontextwrappermgr.h"
#include "prfwtestcontextwrapper.h"


#include "prfwtestfilesrvmsg.h"
#include "prfwtestfiletool.h"
#include "prfwtestlistener.h"
#include "ximprequestcompleteeventimp.h"

#include "prfwtestrobustnesstools.h"


// ============================ MEMBER FUNCTIONS ===============================

// CONSTRUCTION
T_PresenceBlocking* T_PresenceBlocking::NewL()
    {
    T_PresenceBlocking* self = new( ELeave ) T_PresenceBlocking;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


T_PresenceBlocking::~T_PresenceBlocking()
    {
    PrfwTestRobustness::DoPreCleaning();
    }


void T_PresenceBlocking::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


T_PresenceBlocking::T_PresenceBlocking()
    {
    }



// ===========================================================================
// TEST CASE SETUP & TEARDOWN METHODS
// ===========================================================================
//

void T_PresenceBlocking::Setup_L()
    {
    PrfwTestRobustness::DoPreCleaning();

    iWrapperMgr = CXIMPTestContextWrapperMgr::NewL();
    iWrapperMgr->CreateWrapperL();
    }

void T_PresenceBlocking::SetupMultiple_L()
    {
    PrfwTestRobustness::DoPreCleaning();

    iWrapperMgr = CXIMPTestContextWrapperMgr::NewL();
    iWrapperMgr->CreateWrapperL();
    iWrapperMgr->CreateWrapperL();
    iWrapperMgr->CreateWrapperL();
    }


void T_PresenceBlocking::BindL()
    {
    CXIMPTestContextWrapper* wrapper0 = iWrapperMgr->GetWrapperL( 0 );

    wrapper0->BindL( 0 );
    }

void T_PresenceBlocking::BindAllL()
    {
    for( TInt a = 0; a < iWrapperMgr->WrapperCount(); ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        wrapper->BindL( 0 );
        }
    }


void T_PresenceBlocking::Teardown()
    {
    delete iWrapperMgr;
    iWrapperMgr = NULL;
    
    REComSession::FinalClose();

    PrfwTestRobustness::DoPreCleaning();
    }

void T_PresenceBlocking::UnbindL()
    {
    // Do unbind
    CXIMPTestContextWrapper* wrapper0 = iWrapperMgr->GetWrapperL( 0 );
    wrapper0->UnbindL();
    }

void T_PresenceBlocking::UnbindAllL()
    {
    for( TInt a = 0; a < iWrapperMgr->WrapperCount(); ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        wrapper->UnbindL();
        }
    }


// ===========================================================================
// TEST CASES for presentity presence subscription
// ===========================================================================


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// Single client, empty filter
void T_PresenceBlocking::T_SubscribeBlockList_Single_L()
    {
    EUNIT_PRINT( _L("Single presence block list subscription") );

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
    
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

    // Setup status event listener for RequestComplete
    // ---------------------------------------------
    listener2->Reset();

    // normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );
    // ---------------------------------------------

    // subscribe
    // ---------------------------------------------
    TXIMPRequestId reqId = presAuth.SubscribePresenceBlockListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe failed" );
    // ---------------------------------------------

    // verify that SubscribeBlockListL was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresenceBlockListCalled, "SubscribeBlockListL was not called", ETrue, NULL );

    // Setup status event listener for RequestComplete
    // ---------------------------------------------
    listener2->Reset();

    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );
    // ---------------------------------------------

    // unsubscribe
    // ---------------------------------------------
    reqId = presAuth.UnsubscribePresenceBlockListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsubscribe failed" );
    // ---------------------------------------------

    // verify that UnsubscribeBlockListL was called
    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresenceBlockListCalled, "UnsubscribeBlockListL was not called", ETrue, NULL );

    // Deinitiliaze event source from listener.
    listener2->SetEventSourceL( NULL );

    UnbindL();

    //CleanupStack::Pop(presFeat);
    //delete presFeat;
    //CleanupStack::PopAndDestroy();
    CleanupStack::PopAndDestroy( listener2 );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// Single client, empty filter
void T_PresenceBlocking::T_SubscribeBlockListRefresh_Single_L()
    {
    EUNIT_PRINT( _L("Single presence block list subscription refresh") );

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
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

    // Setup status event listener for RequestComplete
    // ---------------------------------------------
    listener2->Reset();

    // normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );
    // ---------------------------------------------

    // subscribe
    // ---------------------------------------------
    TXIMPRequestId reqId = presAuth.SubscribePresenceBlockListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe failed" );
    // ---------------------------------------------

    // verify that SubscribeBlockListL was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresenceBlockListCalled, "SubscribeBlockListL was not called", ETrue, NULL );

    // Setup status event listener for RequestComplete
    // ---------------------------------------------
    listener2->Reset();

    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    RPrBlockInfoImpArray* currentList(NULL);
    CPresenceBlockListEventImp* event = CreateBlockListEventLCX(
        KNullDesC, KNullDesC,
        MXIMPDataSubscriptionState::ESubscriptionActive,
        MXIMPDataSubscriptionState::EDataUnavailable,
        ETestPBlCurrent, currentList );
    listener2->ExpectL( event );
    CleanupStack::Pop(); //event 4 to go.
    // ---------------------------------------------

    // subscribe again
    // ---------------------------------------------
    reqId = presAuth.SubscribePresenceBlockListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Refresh failed" );
    CleanupStack::PopAndDestroy( 4 ); // rest of the items.
    // ---------------------------------------------

    // Setup status event listener for RequestComplete
    // ---------------------------------------------
    listener2->Reset();

    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );
    // ---------------------------------------------

    // unsubscribe
    // ---------------------------------------------
    reqId = presAuth.UnsubscribePresenceBlockListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsubscribe failed" );
    // ---------------------------------------------

    // verify that UnsubscribeBlockListL was called
    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresenceBlockListCalled, "UnsubscribeBlockListL was not called", ETrue, NULL );

    listener2->SetEventSourceL( NULL );

    UnbindL();

    CleanupStack::PopAndDestroy( listener2 );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// Single client, empty filter
void T_PresenceBlocking::T_SubscribeBlockList_Multiple_L()
    {
    EUNIT_PRINT( _L("Multiple presence block list subscription") );

    BindAllL();

    TInt countOfWrappers = iWrapperMgr->WrapperCount();
    TInt a(0);
    for( a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

        CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
        CleanupStack::PushL( listener2 );

        // Tell the protocol it should act normally
        messenger->SetNoError();

        // expect normal request complete
        TXIMPRequestId reqIdDummy;  // not checked
        CXIMPRequestCompleteEventImp* evReqComplete =
                                CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
        listener2->ExpectL( evReqComplete );
        CleanupStack::Pop( evReqComplete );

        // subscribe
        TXIMPRequestId reqId = presAuth.SubscribePresenceBlockListL();

        EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe failed" );

        // verify that SubscribePresenceBlockListL was called
        COMMONASSERT( messenger, EXIMPPlgTestSubscribePresenceBlockListCalled, "SubscribePresenceBlockListL was not called", ETrue, NULL );

        CleanupStack::PopAndDestroy( listener2 );
        }

    for( a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

        CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
        CleanupStack::PushL( listener2 );

        // Tell the protocol it should act normally
        messenger->SetNoError();

        // expect normal request complete
        TXIMPRequestId reqIdDummy;  // not checked
        CXIMPRequestCompleteEventImp* evReqComplete =
                            CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
        listener2->ExpectL( evReqComplete );
        CleanupStack::Pop( evReqComplete );

        // unsubscribe
        TXIMPRequestId reqId = presAuth.UnsubscribePresenceBlockListL();

        EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsubscribe failed" );

        // verify that EXIMPPlgTestUnsubscribeBlockListCalled was called
        COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresenceBlockListCalled,
                "UnsubscribePresenceBlockListL was not called",
                ( a < 2 ) ? EFalse : ETrue,
                "UnsubscribePresenceBlockListL was called" );

        CleanupStack::PopAndDestroy( listener2 );
        }

    UnbindAllL();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void T_PresenceBlocking::SubscribeL()
    {
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    // ------------------------------------------------------
    // expect normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );
    // ------------------------------------------------------

    // subscribe
    TXIMPRequestId reqId = presAuth.SubscribePresenceBlockListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe failed" );

    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresenceBlockListCalled, "SubscribeBlockListL was not called", ETrue, NULL );

    CleanupStack::PopAndDestroy( listener2 );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void T_PresenceBlocking::UnsubscribeL()
    {
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    // ------------------------------------------------------
    // expect normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );
    // ------------------------------------------------------

    // unsubscribe
    TXIMPRequestId reqId = presAuth.UnsubscribePresenceBlockListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsubscribe failed" );

    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresenceBlockListCalled, "UnsubscribeBlockListL was not called", ETrue, NULL );

    CleanupStack::PopAndDestroy( listener2 );
    }


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void T_PresenceBlocking::T_HandlePresenceBlockList_L()
    {
    EUNIT_PRINT( _L("Handle presence block list.") );

    // log in and subscribe a block list
    BindL();
    SubscribeL();

    // ------------------------------------------------------
    // some startup stuff
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();

    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );
    // ------------------------------------------------------

    // Tell the protocol it should act normally
    wrapper->GetMessenger()->SetNoError();

    // ------------------------------------------------------
    // 1. Create faked server message about block list
    SendSrvMsgL( CXIMPTestFileSrvMsg::ETestSrvMsgPresenceBlockList );

    // ------------------------------------------------------
    // expect current(empty) MPresenceBlockList event.
    RPrBlockInfoImpArray* currentList(NULL);
    CPresenceBlockListEventImp* event = CreateBlockListEventLCX(
            KNullDesC, KNullDesC,
            MXIMPDataSubscriptionState::ESubscriptionActive,
            MXIMPDataSubscriptionState::EDataAvailable,
            ETestPBlCurrent, currentList);

    listener2->ExpectL( event );
    CleanupStack::Pop( event ); // 4 more items in cleanupstack

    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "HandlePresenceBlockListL failed" );
    CleanupStack::PopAndDestroy( 4 ); // lists
    // ------------------------------------------------------

    CleanupStack::PopAndDestroy( listener2 );

    // clean it up
    UnsubscribeL();
    UnbindL();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// Single client, empty filter
void T_PresenceBlocking::T_BlockPresence_L()
    {
    EUNIT_PRINT( _L("Block presence") );

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
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

    // Setup status event listener for RequestComplete
    // ---------------------------------------------
    listener2->Reset();

    // normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // subscribe
    TXIMPRequestId reqId = presAuth.SubscribePresenceBlockListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe failed" );

    // verify that SubscribePresenceBlockListL was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresenceBlockListCalled, "SubscribePresenceBlockListL was not called", ETrue, NULL );


    // ---------------------------------------------
    //  1. Block presence

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    _LIT16( KDisplayName1, "DispnameA, LoremI" );


    // Setup status event listener for RequestComplete
    // ---------------------------------------------
    listener2->Reset();

    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    RPrBlockInfoImpArray* currentList(NULL);
    CPresenceBlockListEventImp* event =
            CreateBlockListEventLCX( identity->Identity(),
                                     KNullDesC,
                                     MXIMPDataSubscriptionState::ESubscriptionActive,
                                     MXIMPDataSubscriptionState::EDataAvailable,
                                     ETestPBlAdded, currentList );

    listener2->ExpectL( event );
    CleanupStack::Pop( event );

    // block
    reqId = presAuth.BlockPresenceForPresentityL( *identity );
    
    //Waqas: fixed current list in event
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Block failed" );

    // verify that BlockPresenceForPresentityL was called
    COMMONASSERT( messenger, EXIMPPlgTestBlockPresenceForPresentityCalled, "BlockPresenceForPresentityL was not called", ETrue, NULL );

    CleanupStack::PopAndDestroy( 4 ); // lists
    // ---------------------------------------------


    // ---------------------------------------------
    //  2. Re-block

    // Setup status event listener for RequestComplete
    listener2->Reset();

    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // we need to create current list to give to the event
    currentList = new ( ELeave ) RPrBlockInfoImpArray; // previous is destroyed
    CleanupDeletePushL( currentList );
            // create the group info
    CPresenceBlockInfoImp* blockInfoForEvent = 
                            CPresenceBlockInfoImp::NewLC( *identity, KNullDesC );
    currentList->AppendL( blockInfoForEvent );
    CleanupStack::Pop( blockInfoForEvent );
    CleanupStack::Pop( currentList );
    
    // now make the event
    event = CreateBlockListEventLCX( identity->Identity(),
                                     KNullDesC,
                                     MXIMPDataSubscriptionState::ESubscriptionActive,
                                     MXIMPDataSubscriptionState::EDataAvailable,
                                     ETestPBlCurrent, currentList );

    listener2->ExpectL( event );
    CleanupStack::Pop( event );

    // re-block
    reqId = presAuth.BlockPresenceForPresentityL( *identity );

    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Block failed" );

    // verify that BlockPresenceForPresentityL was called
    COMMONASSERT( messenger, EXIMPPlgTestBlockPresenceForPresentityCalled, "BlockPresenceForPresentityL was not called", ETrue, NULL );


    CleanupStack::PopAndDestroy( 4 ); // lists


    // Deinitiliaze event source from listener.
    listener2->SetEventSourceL( NULL );

    UnbindL();
    
    CleanupStack::PopAndDestroy( identity );

    CleanupStack::PopAndDestroy( listener2 );
    
    }
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// Single client, empty filter
void T_PresenceBlocking::T_CancelBlockedPresence_L()
    {
    EUNIT_PRINT( _L("Cancel presence block") );

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
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

    // Setup status event listener for RequestComplete
    // ---------------------------------------------
    listener2->Reset();

    // normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );
    // ---------------------------------------------

    // subscribe
    // ---------------------------------------------
    TXIMPRequestId reqId = presAuth.SubscribePresenceBlockListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe failed" );
    // ---------------------------------------------

    // verify that SubscribePresenceBlockListL was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresenceBlockListCalled, "SubscribePresenceBlockListL was not called", ETrue, NULL );


    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );

    // ---------------------------------------------
    //  1. Cancel non-existing block (no blocks)
    
    // Setup status event listener for RequestComplete
    listener2->Reset();

    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    RPrBlockInfoImpArray* currentList(NULL);
    CPresenceBlockListEventImp* event =
            CreateBlockListEventLCX( KNullDesC,
                                     KNullDesC,
                                     MXIMPDataSubscriptionState::ESubscriptionActive,
                                     MXIMPDataSubscriptionState::EDataUnavailable,
                                     ETestPBlRemoved, currentList );
    listener2->ExpectL( event );
    CleanupStack::Pop( event );

    // cancel nonexisting block
    reqId = presAuth.CancelPresenceBlockFromPresentityL( *identity );
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Cancel failed" );
    // ---------------------------------------------

    // verify that method was called
    COMMONASSERT( messenger, EXIMPPlgTestCancelPresenceBlockFromPresentityCalled, "CancelPresenceBlockFromPresentityL was not called", ETrue, NULL );

    CleanupStack::PopAndDestroy( 4 ); // lists

    
    // ---------------------------------------------
    //  2. Cancel block 
    

    // Setup status event listener for RequestComplete
    listener2->Reset();

    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    currentList = NULL;  // previous is destroyed
    event = CreateBlockListEventLCX( identity->Identity(),
                                     KNullDesC,
                                     MXIMPDataSubscriptionState::ESubscriptionActive,
                                     MXIMPDataSubscriptionState::EDataAvailable,
                                     ETestPBlAdded, currentList );
    listener2->ExpectL( event );
    CleanupStack::Pop( event );

    // block a presentity
    reqId = presAuth.BlockPresenceForPresentityL( *identity );
    // Waqas: Fixed current list in event
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Block failed" );

    // verify that method was called
    COMMONASSERT( messenger, EXIMPPlgTestBlockPresenceForPresentityCalled, "BlockPresenceForPresentityL was not called", ETrue, NULL );
    CleanupStack::PopAndDestroy( 4 ); // lists
    // ---------------------------------------------


    // Setup status event listener for RequestComplete
    listener2->Reset();

    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // we need to create current list to give to the event
    currentList = new ( ELeave ) RPrBlockInfoImpArray; // previous is destroyed
    CleanupDeletePushL( currentList );
    CPresenceBlockInfoImp* blockInfoForEvent = 
                        CPresenceBlockInfoImp::NewLC( *identity, KNullDesC );
    currentList->AppendL( blockInfoForEvent );
    CleanupStack::Pop( blockInfoForEvent );
    CleanupStack::Pop( currentList );
    
    // now create event and give currentList to it
    event = CreateBlockListEventLCX( identity->Identity(),
                                     KNullDesC,
                                     MXIMPDataSubscriptionState::ESubscriptionActive,
                                     MXIMPDataSubscriptionState::EDataAvailable,
                                     ETestPBlRemoved, currentList );
    listener2->ExpectL( event );
    CleanupStack::Pop( event );

    // cancel the existing block
    reqId = presAuth.CancelPresenceBlockFromPresentityL( *identity );
    // Waqas: Fixed current list in event
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Cancel block failed" );

    // verify that method was called
    COMMONASSERT( messenger, EXIMPPlgTestCancelPresenceBlockFromPresentityCalled, "CancelPresenceBlockFromPresentityL was not called", ETrue, NULL );
    CleanupStack::PopAndDestroy( 4 ); // lists
    // ---------------------------------------------


    // ---------------------------------------------
    //  TODO 3. Cancel block (non-existing block, other blocks exist)


    // Deinitiliaze event source from listener.
    listener2->SetEventSourceL( NULL );

    UnbindL();
    
    CleanupStack::PopAndDestroy( identity );

    CleanupStack::PopAndDestroy( listener2 );
    }


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void T_PresenceBlocking::T_HandlePresenceBlocked_L()
    {
    EUNIT_PRINT( _L("Handle presence blocked") );

    // log in and subscribe a block list
    BindL();
    SubscribeL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();

    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    // Tell the protocol it should act normally
    wrapper->GetMessenger()->SetNoError();


    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    
    // ------------------------------------------------------
    // 1. Create faked server message about blocked
    SendBlockedSrvMsgL( identity->Identity(),
                        KNullDesC,
                        CXIMPTestFileSrvMsg::ETestSrvMsgPresenceBlocked );

    RPrBlockInfoImpArray* currentList(NULL);
    CPresenceBlockListEventImp* event = CreateBlockListEventLCX(
            identity->Identity(), KNullDesC,
            MXIMPDataSubscriptionState::ESubscriptionActive,
            MXIMPDataSubscriptionState::EDataAvailable,
            ETestPBlAdded, currentList );

    listener2->ExpectL( event );
    CleanupStack::Pop( event ); // 4 more items in cleanupstack

    // Waqas: Fixed current list handling
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "HandlePresenceBlocked failed" );
    
    CleanupStack::PopAndDestroy( 4 ); // lists
    // ------------------------------------------------------

    CleanupStack::PopAndDestroy( identity );
    CleanupStack::PopAndDestroy( listener2 );

    // clean it up
    UnsubscribeL();
    UnbindL();
    
    }
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void T_PresenceBlocking::T_HandlePresenceBlockCanceled_L()
    {
    
    EUNIT_PRINT( _L("Handle presence block canceled") );

    // log in and subscribe a block list
    BindL();
    SubscribeL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();

    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    // Tell the protocol it should act normally
    wrapper->GetMessenger()->SetNoError();


    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    
    // ------------------------------------------------------
    // 1. Cancel block (with no blocks)
    SendCanceledSrvMsgL( identity->Identity(),
                        KNullDesC,
                        CXIMPTestFileSrvMsg::ETestSrvMsgPresenceBlockCanceled );

    RPrBlockInfoImpArray* currentList(NULL);
    CPresenceBlockListEventImp* event = CreateBlockListEventLCX(
            KNullDesC, KNullDesC,
            MXIMPDataSubscriptionState::ESubscriptionActive,
            MXIMPDataSubscriptionState::EDataUnavailable,
            ETestPBlRemoved, currentList );

    listener2->ExpectL( event );
    CleanupStack::Pop( event ); // 4 more items in cleanupstack

    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "HandlePresenceBlockCanceled failed" );
    CleanupStack::PopAndDestroy( 4 ); // lists
    // ------------------------------------------------------

    CleanupStack::PopAndDestroy( identity );
    CleanupStack::PopAndDestroy( listener2 );

    // clean it up
    UnsubscribeL();
    UnbindL();    
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// send a faked server-originated message with group info made
// from the given uri and displayname
//
void T_PresenceBlocking::SendSrvMsgL(
        const TDesC& aUri, const TDesC& aDispName,
        TInt aMsgType )
    {
    // we'll use hard-coded wrapper 0 instance for now.
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );

    // create a grant req info contained within the server-originated
    // faked message
    RXIMPObjOwningPtrArray<CPresenceBlockInfoImp> blockList;
    CleanupClosePushL( blockList );

    CXIMPIdentityImp* blockId = CXIMPIdentityImp::NewLC( aUri );
    CPresenceBlockInfoImp* blockItem =
                CPresenceBlockInfoImp::NewLC( *blockId, aDispName );
    blockList.AppendL( blockItem );
    CleanupStack::Pop( blockItem );
    CleanupStack::PopAndDestroy( blockId );

    HBufC8* packedArray =
        TXIMPObjectPacker<CPresenceBlockInfoImp>::PackArrayL( blockList );
    CleanupStack::PushL( packedArray );

    // use filetool to send it for plugin.
    CXIMPTestFileSrvMsg* srvMsg = CXIMPTestFileSrvMsg::NewL( aMsgType,
            *packedArray );
    CleanupStack::PushL( srvMsg );
    wrapper->FileTool().CleanAllL();
    wrapper->FileTool().SrvMsgStoreL( srvMsg );
    CleanupStack::PopAndDestroy( 3 ); // srvMsg, packedArray, blockList
    }
    
void T_PresenceBlocking::SendBlockedSrvMsgL(
        const TDesC& aUri, const TDesC& aDispName,
        TInt aMsgType )
    {
    // we'll use hard-coded wrapper 0 instance for now.
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );

    // create a grant req info contained within the server-originated
    // faked message
    CXIMPIdentityImp* blockId = CXIMPIdentityImp::NewLC( aUri );
    CPresenceBlockInfoImp* blockItem =
                CPresenceBlockInfoImp::NewLC( *blockId, aDispName );


    HBufC8* packedArray =
        TXIMPObjectPacker<CPresenceBlockInfoImp>::PackL( *blockItem );
    CleanupStack::PushL( packedArray );

    // use filetool to send it for plugin.
    CXIMPTestFileSrvMsg* srvMsg = CXIMPTestFileSrvMsg::NewL( aMsgType,
            *packedArray );
    CleanupStack::PushL( srvMsg );
    wrapper->FileTool().CleanAllL();
    wrapper->FileTool().SrvMsgStoreL( srvMsg );
    CleanupStack::PopAndDestroy( 4 ); // blockId, blockItem, packedArray, srvMsg
    }
    
void T_PresenceBlocking::SendCanceledSrvMsgL(
        const TDesC& aUri, const TDesC& aDispName,
        TInt aMsgType )
    {
    // we'll use hard-coded wrapper 0 instance for now.
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );

    CXIMPIdentityImp* blockId = CXIMPIdentityImp::NewLC( aUri );

    HBufC8* packedArray =
        TXIMPObjectPacker<CXIMPIdentityImp>::PackL( *blockId );
    CleanupStack::PushL( packedArray );

    // use filetool to send it for plugin.
    CXIMPTestFileSrvMsg* srvMsg = CXIMPTestFileSrvMsg::NewL( aMsgType,
            *packedArray );
    CleanupStack::PushL( srvMsg );
    wrapper->FileTool().CleanAllL();
    wrapper->FileTool().SrvMsgStoreL( srvMsg );
    CleanupStack::PopAndDestroy( 3 ); // blockId, packedArray, srvMsg
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// send a faked server-originated message with empty contents
//
void T_PresenceBlocking::SendSrvMsgL( TInt aMsgType )
    {
    // we'll use hard-coded wrapper 0 instance for now.
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );

    // create a server-originated faked message with empty contents
    RXIMPObjOwningPtrArray<CPresenceBlockInfoImp> blockList;

    HBufC8* packedArray = TXIMPObjectPacker<CPresenceBlockInfoImp>::PackArrayL( blockList );
    CleanupStack::PushL( packedArray );

    // use filetool to send it for plugin.
    CXIMPTestFileSrvMsg* srvMsg = CXIMPTestFileSrvMsg::NewL( aMsgType,
            *packedArray );
    CleanupStack::PushL( srvMsg );
    wrapper->FileTool().CleanAllL();
    wrapper->FileTool().SrvMsgStoreL( srvMsg );
    CleanupStack::PopAndDestroy( 2 ); // srvMsg, packedArray
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// create a block list event with given arrays
//
CPresenceBlockListEventImp*
    T_PresenceBlocking::CreateBlockListEventLCX(
        const TDesC& aUri,
        const TDesC& aDispName,
        MXIMPDataSubscriptionState::TSubscriptionState aSubscriptionState,
        MXIMPDataSubscriptionState::TDataState aDataState,
        TTestPBlOperationSpecifier aOperation,
        RPrBlockInfoImpArray* aCurrentList)
    {
    // create the arrays

    // new list
    RPrBlockInfoImpArray* newList = new ( ELeave ) RPrBlockInfoImpArray;
    CleanupDeletePushL( newList );

    // current list, created by caller
    if(!aCurrentList) // if client didnt created the list
        aCurrentList = new ( ELeave ) RPrBlockInfoImpArray;
    CleanupDeletePushL( aCurrentList );

    // removed list
    RPrBlockInfoImpArray* removedList = new ( ELeave ) RPrBlockInfoImpArray;
    CleanupDeletePushL( removedList );

    // updated list
    RPrBlockInfoImpArray* updatedList = new ( ELeave ) RPrBlockInfoImpArray;
    CleanupDeletePushL( updatedList );
    
	// create subscription state
	CXIMPDataSubscriptionStateImp* subsState = CXIMPDataSubscriptionStateImp::NewLC();
	 

    // create the group info
    CPresenceBlockInfoImp* blockInfoForEvent1 = NULL;
            {
            CXIMPIdentityImp* idForEvent = CXIMPIdentityImp::NewLC( aUri );
            blockInfoForEvent1 = CPresenceBlockInfoImp::NewLC( *idForEvent, aDispName );
            
            CleanupStack::Pop( blockInfoForEvent1 );        
            CleanupStack::PopAndDestroy( idForEvent ); 
            CleanupStack::PushL( blockInfoForEvent1 );        
            }

    CPresenceBlockInfoImp* blockInfoForEvent2 = 
            TXIMPObjectCloner< CPresenceBlockInfoImp >::CloneLC( *blockInfoForEvent1 );
    
    TInt count(0);
    TBool found(EFalse);
    TInt i(0);

    // put the given group info into the specified array
    switch ( aOperation )
        {
        case ETestPBlAdded:
            {
            newList->AppendL( blockInfoForEvent2 );
            CleanupStack::Pop( blockInfoForEvent2 );
            
            aCurrentList->AppendL( blockInfoForEvent1 );
            CleanupStack::Pop( blockInfoForEvent1 );
            
            break;
            }

        case ETestPBlRemoved:
            {
            // search the given id in current list
            count = aCurrentList->Count();
            for(i=0;i<count;i++)
                {
                // if dispname and id are same
                if (( ((((*aCurrentList)[i])->BlockedEntityId()).Identity()) == aUri )
                    && ( (((*aCurrentList)[i])->BlockedEntityDisplayName()) == aDispName ))
                    {
                    found = ETrue;
            break;
            }
                }
            if(found) // do we need to leave if error? Waqas
            {
                delete (*aCurrentList)[i];
                aCurrentList->Remove(i);
                removedList->AppendL( blockInfoForEvent2 );
                CleanupStack::Pop( blockInfoForEvent2 );
                }
            else
                CleanupStack::PopAndDestroy( blockInfoForEvent2 );
            
            CleanupStack::PopAndDestroy( blockInfoForEvent1 );
            }
            break;
        case ETestPBlUpdated:
            {
            updatedList->AppendL( blockInfoForEvent2 );
            CleanupStack::Pop( blockInfoForEvent2);
            CleanupStack::PopAndDestroy( blockInfoForEvent1 );
            }
            break;

        case ETestPBlCurrent:
            {
            // don't add, thus return what user has given
            CleanupStack::PopAndDestroy( blockInfoForEvent2 ); // blockInfoForEvent2
            CleanupStack::PopAndDestroy( blockInfoForEvent1 ); // blockInfoForEvent1
            }
            break;
        
        default:
            {
            User::Leave( KErrArgument );
            }
            break;
        }

	subsState->SetSubscriptionStateL(aSubscriptionState);
    subsState->SetDataStateL(aDataState);
	
    // create the actual event
    CPresenceBlockListEventImp* tmp =
        CPresenceBlockListEventImp::NewLC(
                newList,
                aCurrentList,
                removedList,
                updatedList,
                subsState );

    // subState is owned by the event, we need to pop it out from the stack
    CleanupStack::Pop( tmp );        
    CleanupStack::Pop( subsState ); 
    CleanupStack::PushL( tmp );        

    /*
     * In cleanupstack there are 5 items, FIFO
     *   - added/new list
     *   - current list
     *   - removed list
     *   - updated list
     *   - event imp
     */

    return tmp;
    }

// ===========================================================================
// TEST CASE TABLE
// ===========================================================================
//
EUNIT_BEGIN_TEST_TABLE(
        T_PresenceBlocking,
        "XIMPFW presence blocking tests",
        "MODULE" )

PRFW_DECORATED_TEST(
        "Subscribe block list",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_SubscribeBlockList_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Refresh block list subscription",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_SubscribeBlockListRefresh_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Subscribe block list, multiple",
        "",
        "",
        "FUNCTIONALITY",
        SetupMultiple_L,
        T_SubscribeBlockList_Multiple_L,
        Teardown )


PRFW_DECORATED_TEST(
        "Handle presence block list",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_HandlePresenceBlockList_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Block presence",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_BlockPresence_L,
        Teardown )
        
PRFW_DECORATED_TEST(
        "Cancel presence block",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_CancelBlockedPresence_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Handle blocked and canceled blocks",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_HandlePresenceBlocked_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Handle blocked and canceled blocks",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_HandlePresenceBlockCanceled_L,
        Teardown )

EUNIT_END_TEST_TABLE


// ===========================================================================
// TEST SUITE CONSTRUCTION
// ===========================================================================
//
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    return T_PresenceBlocking::NewL();
    }


// end of file
