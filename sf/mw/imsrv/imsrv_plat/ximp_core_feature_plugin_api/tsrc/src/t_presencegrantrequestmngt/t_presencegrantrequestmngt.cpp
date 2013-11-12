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
#include <presencefeatures.h>
#include <presenceauthorization.h>

#include "t_presencegrantrequestmngt.h"

#include "ximprequestcompleteeventimp.h"

//#include "prfwtestpresencedatautils.h"

// grant req includes
#include "presencegrantrequestinfoimp.h"
#include "presencegrantrequestlisteventimp.h"


// other data model includes
#include "ximpidentityimp.h"

// utils includes
#include "ximpobjecthelpers.h"

// testing tool includes
#include "prfwtestmessaging.h"
#include "prfwtestmessenger.h"
#include "prfwtestcontextwrappermgr.h"
#include "prfwtestcontextwrapper.h"

#include "prfwtestfilesrvmsg.h"
#include "prfwtestfiletool.h"
#include "prfwtestlistener.h"

#include "prfwtestrobustnesstools.h"

_LIT8( KPrefix1, "PreA_" );
_LIT8( KPrefix2, "PreB_" );
_LIT8( KPrefix3, "PreC_" );
_LIT8( KPrefix4, "PreD_" );

// ============================ MEMBER FUNCTIONS ===============================

// CONSTRUCTION
T_PresenceGrantRequestManagement* T_PresenceGrantRequestManagement::NewL()
    {
    T_PresenceGrantRequestManagement* self = new( ELeave ) T_PresenceGrantRequestManagement;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


T_PresenceGrantRequestManagement::~T_PresenceGrantRequestManagement()
    {
    PrfwTestRobustness::DoPreCleaning();
    }


void T_PresenceGrantRequestManagement::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


T_PresenceGrantRequestManagement::T_PresenceGrantRequestManagement()
    {
    }



// ===========================================================================
// TEST CASE SETUP & TEARDOWN METHODS
// ===========================================================================
//

void T_PresenceGrantRequestManagement::Setup_L()
    {
    PrfwTestRobustness::DoPreCleaning();

    iWrapperMgr = CXIMPTestContextWrapperMgr::NewL();
    iWrapperMgr->CreateWrapperL();
    }

void T_PresenceGrantRequestManagement::SetupMultiple_L()
    {
    PrfwTestRobustness::DoPreCleaning();

    iWrapperMgr = CXIMPTestContextWrapperMgr::NewL();
    iWrapperMgr->CreateWrapperL();
    iWrapperMgr->CreateWrapperL();
    iWrapperMgr->CreateWrapperL();
    }


void T_PresenceGrantRequestManagement::BindL()
    {
    CXIMPTestContextWrapper* wrapper0 = iWrapperMgr->GetWrapperL( 0 );

    wrapper0->BindL( 0 );
    }

void T_PresenceGrantRequestManagement::BindAllL()
    {
    for( TInt a = 0; a < iWrapperMgr->WrapperCount(); ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        wrapper->BindL( 0 );
        }
    }


void T_PresenceGrantRequestManagement::Teardown()
    {
    delete iWrapperMgr;
    iWrapperMgr = NULL;

    REComSession::FinalClose();

    PrfwTestRobustness::DoPreCleaning();
    }

void T_PresenceGrantRequestManagement::UnbindL()
    {
    // Do unbind
    CXIMPTestContextWrapper* wrapper0 = iWrapperMgr->GetWrapperL( 0 );
    wrapper0->UnbindL();
    }

void T_PresenceGrantRequestManagement::UnbindAllL()
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
void T_PresenceGrantRequestManagement::T_SubscribeGrantRequestList_Single_L()
    {
    EUNIT_PRINT( _L("Single presence grant request list subscription") );

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

    // ---------------------------------------------
    // 1. Subscribe grant request list

    // Setup status event listener for RequestComplete
    listener2->Reset();

    // normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );
    // ---------------------------------------------

    // subscribe
    // ---------------------------------------------
    TXIMPRequestId reqId = presAuth.SubscribePresenceGrantRequestListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe failed" );
    // ---------------------------------------------

    // verify that SubscribeGrantListL was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresenceGrantRequestListCalled, "SubscribeGrantListL was not called", ETrue, NULL );


    // ---------------------------------------------
    // 2. refresh subscription
    
    // Setup status event listener for RequestComplete
    listener2->Reset();

    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    RPrGrntReqInfoImpArray* pendingList(NULL);
    CPresenceGrantRequestListEventImp* event = CreateGrantRequestListEventLCX(
        KNullDesC, KNullDesC,
        MXIMPDataSubscriptionState::ESubscriptionActive,
        MXIMPDataSubscriptionState::EDataUnavailable,
        ETestPGRPending, pendingList );
    listener2->ExpectL( event );
    CleanupStack::Pop(); //event 3 to go.

    // refresh subscription
    // ---------------------------------------------
    reqId = presAuth.SubscribePresenceGrantRequestListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Refresh failed" );
    CleanupStack::PopAndDestroy( 3 ); // rest of the items.


    // ---------------------------------------------
    // 3. Unsubscribe grant request list

    // Setup status event listener for RequestComplete
    listener2->Reset();

    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // unsubscribe
    // ---------------------------------------------
    reqId = presAuth.UnsubscribePresenceGrantRequestListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsubscribe failed" );


    // verify that UnsubscribeGrantListL was called
    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresenceGrantRequestListCalled, "UnsubscribeGrantListL was not called", ETrue, NULL );
    

    // ---------------------------------------------
    // 4. Unsubscribe grant request list (non-subscribed)

    // Setup status event listener for RequestComplete
    listener2->Reset();

    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // unsubscribe
    // ---------------------------------------------
    reqId = presAuth.UnsubscribePresenceGrantRequestListL();

    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsubscribe failed" );

    // verify that UnsubscribeGrantListL was called
    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresenceGrantRequestListCalled, "UnsubscribeGrantListL was not called", ETrue, NULL );

    // Deinitiliaze event source from listener.
    listener2->SetEventSourceL( NULL );

    UnbindL();

    CleanupStack::PopAndDestroy( listener2 );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void T_PresenceGrantRequestManagement::T_SubscribeGrantRequestList_Multiple_L()
    {
    EUNIT_PRINT( _L("Multiple presence grant request list subscription") );

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
        TXIMPRequestId reqId = presAuth.SubscribePresenceGrantRequestListL();

        EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe failed" );

        // verify that SubscribePresenceGrantRequestListL was called
        COMMONASSERT( messenger, EXIMPPlgTestSubscribePresenceGrantRequestListCalled, "SubscribePresenceGrantRequestListL was not called", ETrue, NULL );

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
        TXIMPRequestId reqId = presAuth.UnsubscribePresenceGrantRequestListL();

        EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsubscribe failed" );

        // verify that EXIMPPlgTestUnsubscribeGroupListCalled was called
        COMMONASSERT( messenger, EXIMPPlgTestUnsubscribeGroupListCalled,
                "UnsubscribePresenceGrantRequestListL was not called",
                EFalse,
                "UnsubscribePresenceGrantRequestListL was called" );

        CleanupStack::PopAndDestroy( listener2 );
        }

    UnbindAllL();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void T_PresenceGrantRequestManagement::SubscribeL()
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
    TXIMPRequestId reqId = presAuth.SubscribePresenceGrantRequestListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe failed" );

    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresenceGrantRequestListCalled, "SubscribeGrantRequestListL was not called", ETrue, NULL );

    CleanupStack::PopAndDestroy( listener2 );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void T_PresenceGrantRequestManagement::UnsubscribeL()
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
    TXIMPRequestId reqId = presAuth.UnsubscribePresenceGrantRequestListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsubscribe failed" );

    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresenceGrantRequestListCalled, "UnsubscribeGrantRequestListL was not called", ETrue, NULL );

    CleanupStack::PopAndDestroy( listener2 );
    }


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void T_PresenceGrantRequestManagement::T_HandlePresenceGrantRequestList_L()
    {
    EUNIT_PRINT( _L("Handle presence grant request list.") );

    // log in and subscribe a group list
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
    // 1. Create faked server message about grant req list (empty)
    SendSrvMsgL( CXIMPTestFileSrvMsg::ETestSrvMsgPresenceGrantRequestList );

    // ------------------------------------------------------
    // expect empty MPresentityGroupList event.

    RPrGrntReqInfoImpArray* pendingList(NULL);
    CPresenceGrantRequestListEventImp* event = CreateGrantRequestListEventLCX(
            KNullDesC, KNullDesC,
            MXIMPDataSubscriptionState::ESubscriptionActive,
            MXIMPDataSubscriptionState::EDataAvailable,
            ETestPGRPending, pendingList );

    listener2->ExpectL( event );
    CleanupStack::Pop( event ); // 3 more items in cleanupstack

    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "HandlePresenceGrantRequestListL failed" );
    CleanupStack::PopAndDestroy( 3 ); // lists
    // ------------------------------------------------------


    _LIT16( KGrantReqUri,      "http://carpe.diem.com" );
    _LIT16( KGrantReqDispName, "DispnameA, LoremI" );

    // ------------------------------------------------------
    // 2. Create faked server message about grant req list (new)
    listener2->Reset();

    pendingList = NULL; // previous one was destroyed
    event = CreateGrantRequestListEventLCX( KGrantReqUri,
                                            KGrantReqDispName,
                                            MXIMPDataSubscriptionState::ESubscriptionActive,
                                            MXIMPDataSubscriptionState::EDataAvailable,
                                            ETestPGRNewReq, pendingList  );

    SendSrvMsgL( KGrantReqUri,
                     KGrantReqDispName,
                     CXIMPTestFileSrvMsg::ETestSrvMsgPresenceGrantRequestList );

    listener2->ExpectL( event );
    CleanupStack::Pop( event ); // 3 more items in cleanupstack

    // Waqas: Fixed Ticket #18
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "HandlePresenceGrantRequestListL failed" );
    
    CleanupStack::PopAndDestroy( 3 ); // lists
    // ------------------------------------------------------


    CleanupStack::PopAndDestroy( listener2 );

    // clean it up
    UnsubscribeL();
    UnbindL();
    }


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
_LIT16(  KPresentUri1,      "http://lorem.ipsum.com" );
_LIT16(  KPresentUri2,      "http://carpe.diem.com" );
_LIT16( KPresentDispName1, "DispnameA, LoremI" );
_LIT16( KPresentDispName2, "DispnameB, CarpeD" );
_LIT16( KPresentProtAddr1, "ABC" );
_LIT16( KPresentProtAddr2, "HIJ" );

void T_PresenceGrantRequestManagement::T_HandlePresenceGrantRequestReceivedObsoleted_L()
    {
    EUNIT_PRINT( _L("Handle presence grant request received") );

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
    // 1. Create faked server message about grant req received
    SendSrvMsgInfoL( KPresentUri1,
                     KPresentDispName1,
                     CXIMPTestFileSrvMsg::ETestSrvMsgPresenceGrantRequestReceived );

    // ------------------------------------------------------
    // expect MPresenceGrantRequestList event.

    CPresenceGrantRequestListEventImp* event = CreateGrantRequestListEventLCX(
            KPresentUri1, KPresentDispName1,
            MXIMPDataSubscriptionState::ESubscriptionActive,
            MXIMPDataSubscriptionState::EDataAvailable,
            ETestPGRNewReq, NULL );

    listener2->ExpectL( event );
    CleanupStack::Pop( event ); // 3 more items in cleanupstack

    // Waqas: Fixed Ticket #18
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "HandlePresenceGrantRequesReceivedL failed" );


    CleanupStack::PopAndDestroy( 3 ); // lists
    // ------------------------------------------------------


    // ------------------------------------------------------
    // 2. Create faked server message about grant req obsoleted
    listener2->Reset();

    SendSrvMsgInfoL( KPresentUri1,
                     KPresentDispName1,
                     CXIMPTestFileSrvMsg::ETestSrvMsgPresenceGrantRequestObsoleted );

                     
    // we need to create pending list to give to the event
    RPrGrntReqInfoImpArray* pendingList = new ( ELeave ) RPrGrntReqInfoImpArray;
    CleanupDeletePushL( pendingList );
            // create the group info
    CXIMPIdentityImp* idForEvent = CXIMPIdentityImp::NewLC( KPresentUri1 );
    CPresenceGrantRequestInfoImp* grantInfoForEvent = 
                            CPresenceGrantRequestInfoImp::NewLC( *idForEvent, KPresentDispName1 );
    pendingList->AppendL( grantInfoForEvent );
    CleanupStack::Pop( grantInfoForEvent );
    CleanupStack::PopAndDestroy( idForEvent );
    CleanupStack::Pop( pendingList );
    
    //Now create the event
    event = CreateGrantRequestListEventLCX(
            KPresentUri1, KPresentDispName1,
            MXIMPDataSubscriptionState::ESubscriptionActive,
            MXIMPDataSubscriptionState::EDataAvailable,
            ETestPGRObsoleted, pendingList );

    listener2->ExpectL( event );
    CleanupStack::Pop( event ); // 3 more items in cleanupstack

    // Waqas: Fixed Ticket #18
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "HandlePresenceGrantRequesObsoletedL failed" );

    CleanupStack::PopAndDestroy( 3 ); // lists
    // ------------------------------------------------------

    CleanupStack::PopAndDestroy( listener2 );

    // clean it up
    UnsubscribeL();
    UnbindL();
    }


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// send a faked server-originated message with grant request list made
// from the given uri and displayname
//
void T_PresenceGrantRequestManagement::SendSrvMsgL(
        const TDesC& aUri, const TDesC& aDispName,
        TInt aMsgType )
    {
    // we'll use hard-coded wrapper 0 instance for now.
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );

    // create a grant req info contained within the server-originated
    // faked message
    RXIMPObjOwningPtrArray<CPresenceGrantRequestInfoImp> grantList;
    CleanupClosePushL( grantList );

    CXIMPIdentityImp* grantId = CXIMPIdentityImp::NewLC( aUri );
    CPresenceGrantRequestInfoImp* grantItem =
                CPresenceGrantRequestInfoImp::NewLC( *grantId, aDispName );
    grantList.AppendL( grantItem );
    CleanupStack::Pop( grantItem );
    CleanupStack::PopAndDestroy( grantId );

    HBufC8* packedArray =
        TXIMPObjectPacker<CPresenceGrantRequestInfoImp>::PackArrayL( grantList );
    CleanupStack::PushL( packedArray );

    // use filetool to send it for plugin.
    CXIMPTestFileSrvMsg* srvMsg = CXIMPTestFileSrvMsg::NewL( aMsgType,
            *packedArray );
    CleanupStack::PushL( srvMsg );
    wrapper->FileTool().CleanAllL();
    wrapper->FileTool().SrvMsgStoreL( srvMsg );
    CleanupStack::PopAndDestroy( 3 ); // srvMsg, packedArray, grantList
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// send a faked server-originated message with grant request info made
// from the given uri and displayname
//
void T_PresenceGrantRequestManagement::SendSrvMsgInfoL(
        const TDesC& aUri, const TDesC& aDispName,
        TInt aMsgType )
    {
    // we'll use hard-coded wrapper 0 instance for now.
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );

    CXIMPIdentityImp* grantId = CXIMPIdentityImp::NewLC( aUri );
    CPresenceGrantRequestInfoImp* grantItem =
                CPresenceGrantRequestInfoImp::NewLC( *grantId, aDispName );

    HBufC8* packedInfo =
        TXIMPObjectPacker<CPresenceGrantRequestInfoImp>::PackL( *grantItem );
    CleanupStack::PushL( packedInfo );

    // use filetool to send it for plugin.
    CXIMPTestFileSrvMsg* srvMsg = CXIMPTestFileSrvMsg::NewL( aMsgType,
            *packedInfo );
    CleanupStack::PushL( srvMsg );
    wrapper->FileTool().CleanAllL();
    wrapper->FileTool().SrvMsgStoreL( srvMsg );
    CleanupStack::PopAndDestroy( 4 ); // srvMsg, packedItem, grantItem, grantId
    }


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// send a faked server-originated message with empty contents
//
void T_PresenceGrantRequestManagement::SendSrvMsgL( TInt aMsgType )
    {
    // we'll use hard-coded wrapper 0 instance for now.
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );

    // create a server-originated faked message with empty contents
    RXIMPObjOwningPtrArray<CPresenceGrantRequestInfoImp> grantList;

    HBufC8* packedArray = TXIMPObjectPacker<CPresenceGrantRequestInfoImp>::PackArrayL( grantList );
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
// create a grant request list event with given arrays
//
CPresenceGrantRequestListEventImp*
    T_PresenceGrantRequestManagement::CreateGrantRequestListEventLCX(
        const TDesC& aUri,
        const TDesC& aDispName,
        MXIMPDataSubscriptionState::TSubscriptionState aSubscriptionState,
        MXIMPDataSubscriptionState::TDataState aDataState,
        TTestPGROperation aOperation,
        RPrGrntReqInfoImpArray* aPendingList
        )
    {
    // create the arrays


    // new list
    RPrGrntReqInfoImpArray* newList = new ( ELeave ) RPrGrntReqInfoImpArray;
    CleanupDeletePushL( newList );

    // pending list
    if(!aPendingList) // if client didnt created the list
        aPendingList = new ( ELeave ) RPrGrntReqInfoImpArray;
    CleanupDeletePushL( aPendingList );

    // obsoleted list
    RPrGrntReqInfoImpArray* obsoletedList = new ( ELeave ) RPrGrntReqInfoImpArray;
    CleanupDeletePushL( obsoletedList );

	// create subscription state
	CXIMPDataSubscriptionStateImp* subsState = CXIMPDataSubscriptionStateImp::NewLC();

    // create the group info
    CPresenceGrantRequestInfoImp* grantInfoForEvent1 = NULL;
            {
            CXIMPIdentityImp* idForEvent = CXIMPIdentityImp::NewLC( aUri );
            grantInfoForEvent1 = CPresenceGrantRequestInfoImp::NewLC( *idForEvent, aDispName );
            CleanupStack::Pop( grantInfoForEvent1 );        
            CleanupStack::PopAndDestroy( idForEvent ); 
            CleanupStack::PushL( grantInfoForEvent1 );        
            }
    
    CPresenceGrantRequestInfoImp* grantInfoForEvent2 = 
            TXIMPObjectCloner< CPresenceGrantRequestInfoImp >::CloneLC( *grantInfoForEvent1 );
    
    TInt count(0);
    TBool found(EFalse);
    TInt i(0);
   

    // put the given grant info into the specified array
    switch ( aOperation )
        {
        case ETestPGRNewReq:
            {
            newList->AppendL( grantInfoForEvent2 );
            CleanupStack::Pop( grantInfoForEvent2 );
            
            aPendingList->AppendL( grantInfoForEvent1 );
            CleanupStack::Pop( grantInfoForEvent1 );
            
            break;
            }
        case ETestPGRPending:
            {
            // don't add, thus return what user has given
            CleanupStack::PopAndDestroy( grantInfoForEvent2 ); // grantInfoForEvent2
            CleanupStack::PopAndDestroy( grantInfoForEvent1 ); // grantInfoForEvent1
            break;
            }

        case ETestPGRObsoleted:
            {
            // search the given id in pending list
            count = aPendingList->Count();
            for(i=0;i<count;i++)
                {
                // if dispname and id are same
                if (( ((((*aPendingList)[i])->RequestorId()).Identity()) == aUri )
                    && ( (((*aPendingList)[i])->RequestorDisplayName()) == aDispName ))
                    {
                    found = ETrue;
            break;
            }
                }
            if(found) // do we need to leave if error? Waqas
            {
                delete (*aPendingList)[i];
                aPendingList->Remove(i);
                obsoletedList->AppendL( grantInfoForEvent2 );
                CleanupStack::Pop( grantInfoForEvent2 );
                }
            else
                CleanupStack::PopAndDestroy( grantInfoForEvent2 );
            
            CleanupStack::PopAndDestroy( grantInfoForEvent1 );
            break;
            }

        default:
            {
            User::Leave( KErrArgument );
            break;
            }
        };

	subsState->SetSubscriptionStateL(aSubscriptionState);
    subsState->SetDataStateL(aDataState);

    // create the actual event
    CPresenceGrantRequestListEventImp* tmp =
        CPresenceGrantRequestListEventImp::NewLC(
                newList,
                aPendingList,
                obsoletedList,
                subsState );

    // subState is owned by the event, we need to pop it out from the stack
    CleanupStack::Pop( tmp );        
    CleanupStack::Pop( subsState ); 
    CleanupStack::PushL( tmp );        

    /*
     * In cleanupstack there are 4 items, FIFO:
     *   - new list
     *   - pending list
     *   - obsoleted list
     *   - event imp
     */

    return tmp;
    }

// ===========================================================================
// TEST CASE TABLE
// ===========================================================================
//
EUNIT_BEGIN_TEST_TABLE(
        T_PresenceGrantRequestManagement,
        "XIMP presence grant request management tests",
        "MODULE" )

PRFW_DECORATED_TEST(
        "Grant request list subscription, single client",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_SubscribeGrantRequestList_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Grant request list subscription, multiple clients",
        "",
        "",
        "FUNCTIONALITY",
        SetupMultiple_L,
        T_SubscribeGrantRequestList_Multiple_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Handle presence grant request List",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_HandlePresenceGrantRequestList_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Handle presence grant request received and obsoleted",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_HandlePresenceGrantRequestReceivedObsoleted_L,
        Teardown )


/*
PRFW_DECORATED_TEST(
        "New and obsolete request.",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_HandlePresentityGroupCreateAndDelete_L,
        Teardown )
*/

EUNIT_END_TEST_TABLE


// ===========================================================================
// TEST SUITE CONSTRUCTION
// ===========================================================================
//
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    return T_PresenceGrantRequestManagement::NewL();
    }


// end of file
