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
#include <presencefeatures.h>
#include <presencepublishing.h>
#include <ximpdatasubscriptionstate.h>
#include <presentitygroups.h>
#include <ximpobjectfactory.h>
#include <presencewatching.h>
#include <presenceobjectfactory.h>
#include <ximpcontext.h>
#include <ximpcontextstateevent.h>
#include <ximprequestcompleteevent.h>
#include <ximpstatus.h>
#include <presentitypresenceevent.h>
#include "prfwtestpresencedatautils.h"



#include "t_presencewatcherlistmngt.h"

// watcher list related includes
#include "presencewatcherinfoimp.h"
#include "presencewatcherlisteventimp.h"

// other data model includes
#include "ximpidentityimp.h"
#include "ximpdatasubscriptionstateimp.h"
#include "presentitygroupmemberinfoimp.h"
#include "presenceinfofilterimp.h"

// utils includes
#include "ximpobjecthelpers.h"

#include "ximprequestcompleteeventimp.h"

// testing tool includes
#include "prfwtestmessaging.h"
#include "prfwtestmessenger.h"
#include "prfwtestcontextwrappermgr.h"
#include "prfwtestcontextwrapper.h"
#include "prfwtestwaithelper.h"

#include "prfwtestfilesrvmsg.h"
#include "prfwtestfiletool.h"
#include "prfwtestlistener.h"

#include "prfwtestrobustnesstools.h"





/*#include "presenceinfoimp.h"
#include "presenceinfofilterimp.h"
#include "prfwtestpresencedatautils.h"
#include "ximpidentityimp.h"
#include "presentitygroupmemberinfoimp.h"
#include "ximpobjecthelpers.h"
#include "prfwtestfilesrvmsg.h"
#include "prfwtestwaithelper.h"*/

_LIT8( KPrefix1, "PreA_" );
_LIT8( KPrefix2, "PreB_" );
_LIT8( KPrefix3, "PreC_" );
_LIT8( KPrefix4, "PreD_" );

// ============================ MEMBER FUNCTIONS ===============================

// CONSTRUCTION
T_PresenceWatcherListManagement* T_PresenceWatcherListManagement::NewL()
    {
    T_PresenceWatcherListManagement* self = new( ELeave ) T_PresenceWatcherListManagement;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


T_PresenceWatcherListManagement::~T_PresenceWatcherListManagement()
    {
    }


void T_PresenceWatcherListManagement::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


T_PresenceWatcherListManagement::T_PresenceWatcherListManagement()
    {
    }



// ===========================================================================
// TEST CASE SETUP & TEARDOWN METHODS
// ===========================================================================
//

void T_PresenceWatcherListManagement::Setup_L()
    {
        
    __UHEAP_MARK;
    PrfwTestRobustness::DoPreCleaning();

    iWrapperMgr = CXIMPTestContextWrapperMgr::NewL();
    iWrapperMgr->CreateWrapperL();
    }

void T_PresenceWatcherListManagement::SetupMultiple_L()
    {
    __UHEAP_MARK;
    PrfwTestRobustness::DoPreCleaning();

    iWrapperMgr = CXIMPTestContextWrapperMgr::NewL();
    iWrapperMgr->CreateWrapperL();
    iWrapperMgr->CreateWrapperL();
    iWrapperMgr->CreateWrapperL();
    }


void T_PresenceWatcherListManagement::BindL()
    {
    CXIMPTestContextWrapper* wrapper0 = iWrapperMgr->GetWrapperL( 0 );

    wrapper0->BindL( 0 );
    }

void T_PresenceWatcherListManagement::BindAllL()
    {
    for( TInt a = 0; a < iWrapperMgr->WrapperCount(); ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        wrapper->BindL( 0 );
        }
    }


void T_PresenceWatcherListManagement::Teardown()
    {
    delete iWrapperMgr;
    iWrapperMgr = NULL;

    REComSession::FinalClose();

    PrfwTestRobustness::DoPreCleaning();
        
    __UHEAP_MARKEND;
    }

void T_PresenceWatcherListManagement::UnbindL()
    {
    // Do unbind
    CXIMPTestContextWrapper* wrapper0 = iWrapperMgr->GetWrapperL( 0 );
    wrapper0->UnbindL();
    }

void T_PresenceWatcherListManagement::UnbindAllL()
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
// Check that this don't leak memory
void T_PresenceWatcherListManagement::T_BindUnbind_L()
    {
    BindL();
    UnbindL();
    }
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// Subscribe, unsubscribe - Single client
void T_PresenceWatcherListManagement::T_SubscribePresenceWatcherList_Single_L()
    {

    EUNIT_PRINT( _L("Single client presence watcher list subscription") );

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

    //-------------------------------------------------
    // subscribe

    // Setup status event listener for RequestComplete
    // ---------------------------------------------
    listener2->Reset();

    // normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );


    // Subscribe
    TXIMPRequestId reqId = presPub.SubscribePresenceWatcherListL();

    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe failed" );

    // verify that SubscribePresenceWatcherListL was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresenceWatcherListCalled, "SubscribePresenceWatcherListL was not called", ETrue, NULL );

    //-------------------------------------------------
    // refresh subscribe

    // Setup status event listener for RequestComplete
    listener2->Reset();

    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    RPrWatLstInfoImpArray* currentList(NULL);
    CPresenceWatcherListEventImp* event = CreateWatcherListEventLCX(
        KNullDesC, KNullDesC,
        MXIMPDataSubscriptionState::ESubscriptionActive,
        MXIMPDataSubscriptionState::EDataUnavailable,
        ETestPWlCurrent, currentList);
    listener2->ExpectL( event );
    CleanupStack::Pop(); //event 3 to go.
    // ---------------------------------------------

    // Subscribe second time
    reqId = presPub.SubscribePresenceWatcherListL();

    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Refresh failed" );
    CleanupStack::PopAndDestroy( 3 ); // rest of the items.

    // verify that SubscribePresenceWatcherListL was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresenceWatcherListCalled, "SubscribePresenceWatcherListL was not called", ETrue, NULL );


    //-------------------------------------------------
    // unsubscribe

    // Setup status event listener for RequestComplete
    listener2->Reset();

    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // register interest
    reqId = presPub.UnsubscribePresenceWatcherListL();

    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsubscribe failed" );

    // verify that UnsubscribePresenceWatcherListL was called
    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresenceWatcherListCalled, "UnsubscribePresenceWatcherListL was not called", ETrue, NULL );

    //-------------------------------------------------
    // unsubscribe nonsubscribed

    // Setup status event listener for RequestComplete
    listener2->Reset();

    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // register interest
    reqId = presPub.UnsubscribePresenceWatcherListL();

    // Wait for events on the request
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsubscribe failed" );

    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresenceWatcherListCalled, "UnsubscribePresenceWatcherListL was not called", ETrue, NULL );



    listener2->SetEventSourceL( NULL );

    UnbindL();

    CleanupStack::PopAndDestroy( listener2 );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// Subscribe, Unsubscribe - Multiple clients
void T_PresenceWatcherListManagement::T_SubscribePresenceWatcherList_Multiple_L()
    {
    EUNIT_PRINT( _L("Multiple client presence watcher list subscription") );

    BindAllL();

    TInt countOfWrappers = iWrapperMgr->WrapperCount();
    for( TInt a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Tell the protocol it should act normally
        messenger->SetNoError();

        CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
        CleanupStack::PushL( listener2 );

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresencePublishing& presPub = presFeat->PresencePublishing();

        // Setup status event listener for RequestComplete
        listener2->Reset();

        TXIMPRequestId reqIdDummy;  // not checked
        CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
        listener2->ExpectL( evReqComplete );
        CleanupStack::Pop( evReqComplete );

        // Subscribe
        TXIMPRequestId reqId = presPub.SubscribePresenceWatcherListL();

        // Wait for events on the request
        EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe failed" );

        // verify that SubscribePresenceWatcherListL was called
        COMMONASSERT( messenger, EXIMPPlgTestSubscribePresenceWatcherListCalled, "SubscribePresenceWatcherListL was not called", ETrue, NULL );

        CleanupStack::PopAndDestroy( listener2 );
        }
    for( TInt a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
        CleanupStack::PushL( listener2 );

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresencePublishing& presPub = presFeat->PresencePublishing();

        // Setup status event listener for RequestComplete
        listener2->Reset();

        TXIMPRequestId reqIdDummy;  // not checked
        CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
        listener2->ExpectL( evReqComplete );
        CleanupStack::Pop( evReqComplete );

        // Unsubscribe
        TXIMPRequestId reqId = presPub.UnsubscribePresenceWatcherListL();

        // Wait for events on the request
        EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsubscribe failed" );

        // verify that unsubscribe was called
        COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresenceWatcherListCalled,
            "UnsubscribePresenceWatcherListL was not called",
            ( a < 2 ) ? EFalse : ETrue,
            "UnsubscribePresenceWatcherListL was called" );

        CleanupStack::PopAndDestroy( listener2 );
        }

    UnbindAllL();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void T_PresenceWatcherListManagement::SubscribeL()
    {
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresencePublishing& presPub = presFeat->PresencePublishing();

    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    // Setup status event listener for RequestComplete
    listener2->Reset();

    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete =
                    CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // subscribe
    TXIMPRequestId reqId = presPub.SubscribePresenceWatcherListL();

    // Wait for events on the request
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe failed" );

    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresenceWatcherListCalled, "SubscribeWatcherListL was not called", ETrue, NULL );

    CleanupStack::PopAndDestroy( listener2 );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void T_PresenceWatcherListManagement::UnsubscribeL()
    {
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresencePublishing& presPub = presFeat->PresencePublishing();

    // Setup status event listener for RequestComplete
    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    // Setup status event listener for RequestComplete
    listener2->Reset();

    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete =
                    CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // unsubscribe
    TXIMPRequestId reqId = presPub.UnsubscribePresenceWatcherListL();

    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsbscribe failed" );

    // verify that UnsubscribeWatcherListL was called
    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresenceWatcherListCalled, "UnsubscribeWatcherListL was not called", ETrue, NULL );

    CleanupStack::PopAndDestroy( listener2 );

    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void T_PresenceWatcherListManagement::T_HandlePresenceWatcherList_L()
    {
    EUNIT_PRINT( _L("Handle presence watcher list.") );

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
    // 1. Create faked server message about watcher list (empty)
    SendSrvMsgL( CXIMPTestFileSrvMsg::ETestSrvMsgPresenceWatcherList );

    // ------------------------------------------------------
    // expect empty MPresencewatcherList event.

    RPrWatLstInfoImpArray* currentList(NULL);
    CPresenceWatcherListEventImp* event = CreateWatcherListEventLCX(
            KNullDesC, KNullDesC,
        MXIMPDataSubscriptionState::ESubscriptionActive,
        MXIMPDataSubscriptionState::EDataAvailable,
        ETestPWlCurrent, currentList);

    listener2->ExpectL( event );
    CleanupStack::Pop( event ); // 3 more items in cleanupstack

    //Waqas: Fixed task Ticket#22
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "HandlePresenceWatcherListL failed" );
    CleanupStack::PopAndDestroy( 3 ); // lists
    // ------------------------------------------------------


    // ------------------------------------------------------
    // 2. Create faked server message about watcher list (new)
    listener2->Reset();

    _LIT16( KWatcherUri1,      "http://carpe.diem.com" );
    _LIT16( KWatcherDispName1, "DispnameA, LoremI" );

    SendSrvMsgL( KWatcherUri1,
                 KWatcherDispName1,
                 CXIMPTestFileSrvMsg::ETestSrvMsgPresenceWatcherList );

    // ------------------------------------------------------
    // MPresencewatcherList event.

    currentList = NULL; // previous one was destroyed
    //Now create the actual event
    event = CreateWatcherListEventLCX(  // previous event was destroyed
            KWatcherUri1, KWatcherDispName1,
        MXIMPDataSubscriptionState::ESubscriptionActive,
        MXIMPDataSubscriptionState::EDataAvailable,
        ETestPWlNew, currentList);
    

    listener2->ExpectL( event );
    CleanupStack::Pop( event ); // 3 more items in cleanupstack

    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "HandlePresenceWatcherListL failed" );
    CleanupStack::PopAndDestroy( 3 ); // lists


    // ------------------------------------------------------
    // 3. Create faked server message about watcher list (disappeared)
    listener2->Reset();

    SendSrvMsgL( CXIMPTestFileSrvMsg::ETestSrvMsgPresenceWatcherList );
    // ------------------------------------------------------
    // MPresencewatcherList event.

    CXIMPIdentityImp* idForEvent = CXIMPIdentityImp::NewLC( KWatcherUri1 );
    // we need to create current list to give to the event
    currentList = new ( ELeave ) RPrWatLstInfoImpArray; // previous is destroyed
    CleanupDeletePushL( currentList );
            // create the group info
    CPresenceWatcherInfoImp* watcherInfoForEvent = 
                            CPresenceWatcherInfoImp::NewLC( (MPresenceWatcherInfo::TWatcherType)0, *idForEvent, KWatcherDispName1);
    currentList->AppendL( watcherInfoForEvent );
    CleanupStack::Pop( watcherInfoForEvent );
    CleanupStack::Pop( currentList );
    CleanupStack::PopAndDestroy( idForEvent );    


    //Now create the actual event
    event = CreateWatcherListEventLCX(
            KWatcherUri1, KWatcherDispName1,
        MXIMPDataSubscriptionState::ESubscriptionActive,
        MXIMPDataSubscriptionState::EDataAvailable,
        ETestPWlDisappeared, currentList);

    listener2->ExpectL( event );
    CleanupStack::Pop( event ); // 3 more items in cleanupstack

    //Waqas: Fixed task Ticket#22
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "HandlePresenceWatcherListL failed" );

    CleanupStack::PopAndDestroy( 3 ); // lists


    // clean it up
    UnsubscribeL();
    
    /* TODO later on better time.
    // ------------------------------------------------------
    // 4. Create faked server message about watcher list (no subscribers to list)
    //    Make sure no event is 
    listener2->Reset();
    
    event = CreateWatcherListEventLCX(
            KNullDesC, KNullDesC,
            ETestPWlEmpty );

    //EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "HandlePresenceWatcherListL failed" );

    */
    CleanupStack::PopAndDestroy( listener2 );
    
    UnbindL();
    }

////////////////////////////////////////////////////
//// INTIA CASE 
////////////////////////////////////////////////////
void T_PresenceWatcherListManagement::T_IndiaCase_L()
    {

    EUNIT_PRINT( _L("Presence notification subscribed list, Intia") );
   

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* presenceCtx = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    CXIMPTestListener* ctxObserver = CXIMPTestListener::NewL( presenceCtx );
    CleanupStack::PushL( ctxObserver );  // << ctxObserver
  

    // Tell the protocol it should act normally
    messenger->SetNoError();

       // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresencePublishing& presPub = presFeat->PresencePublishing();
    MPresentityGroups& presGroups = presFeat->PresentityGroups();    
    MPresenceWatching& presWatch = presFeat->PresenceWatching();
    
    MXIMPObjectFactory& objFactory = presenceCtx->ObjectFactory();
    
    _LIT( KGroupId, "rakesh.harsh/Friends" );
    _LIT( KGroupDisplayName, "my_friend" );
    _LIT( KMemberId1, "user1" );
    _LIT( KMember1DisplayName, "user1DisplayName" );
    _LIT( KMemberId2, "user2" );
    _LIT( KMember2DisplayName, "user2DisplayName" );
    
    
    // Create test variables
    MXIMPIdentity* testGroupId = objFactory.NewIdentityLC();   // << testGroupId
    testGroupId->SetIdentityL( KGroupId ); 
        
    MXIMPIdentity* member1 = objFactory.NewIdentityLC(); // << member1
    member1->SetIdentityL( KMemberId1 );
    
    MXIMPIdentity* member2 = objFactory.NewIdentityLC(); // << member2
    member2->SetIdentityL( KMemberId2 );
    
    //////////////////////////////////////////////////////////////////////////
    // SUBSCRIBE PRESENTITY GROUP LIST
    //////////////////////////////////////////////////////////////////////////
    
    // Setup status event listener for RequestComplete
    TXIMPRequestId reqId;  
    ctxObserver->Reset();
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqId );// << evReqComplete
    ctxObserver->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete ); // >> evReqComplete 
    
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );
    
    // subscribe PresentityGroupList
    reqId = presGroups.SubscribePresentityGroupListL();
    
    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Subscribing presentity group list, complete with error: ") );
    
    // verify that SubscribePresentityGroupListL was called
    messenger->AssertSubscribePresentityGroupListCalled( ETrue );
    
    
    // get list request
/*    TXIMPRequestId req = presGroups.SubscribePresentityGroupListL(); 
    ctxObserver->WaitOpToCompleteL( req );

    CUserAfter::AfterL( 1000000 );
*/
    
    //////////////////////////////////////////////////////////////////////////
    // SUBSCRIBE PRESENTITY GROUP CONTENT
    //////////////////////////////////////////////////////////////////////////    

    // Setup status event listener for RequestComplete
    ctxObserver->Reset();
    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqId );
    ctxObserver->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );
    SetupListenerReqCompleteL( wrapper, EXIMPTestStatusEvents_RequestOk );
            
    // subscribe group content
    reqId = presGroups.SubscribePresentityGroupContentL( *testGroupId );
    
    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Subscribe presentity group content, complete with error: ") );

    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresentityGroupContentCalled, "SubscribeGroupContentL was not called", ETrue, NULL );
    
/*    TInt count = ctxObserver->gidcount;
    // List manage request
     for(TInt i = 0; i<count;i++)
     { 
      //get the content of the lists
       MXIMPIdentity*   tgid = objFactory.NewIdentityLC();
     tgid->SetIdentityL( ctxObserver->gid[i] ); 
    // tgid->SetIdentityL( _L("rakesh.harsh/basu2@nokia.com") );
      
      TXIMPRequestId  req = presGroups.SubscribePresentityGroupContentL( *tgid );
    
      ctxObserver->WaitOpToCompleteL( req );
    
      CleanupStack::PopAndDestroy( 1 ); //  tgid
     }  */
     
    //////////////////////////////////////////////////////////////////////////
    // CREATE PRESENTITY GROUP
    /////////////////////////////////////////////////////////////////////////
    
    // Setup status event listener for RequestComplete
    // This will also produce HandlePresentityGroupListEvent, since the
    // the group is subscribed 
    ctxObserver->Reset();
    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqId );
    ctxObserver->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );//TODO???

    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // Create group
    reqId = presGroups.CreatePresentityGroupL( *testGroupId, KGroupDisplayName );
    
    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("CreatePresentityGroupL failed, complete with error: ") );
    
    // verify that SubscribePresentityGroupListL was called
    messenger->AssertCreatePresentityGroupCalled( ETrue );

    
                   
    // first make a test group in our roster
 /*   MXIMPIdentity* newGroupId = objFactory.NewIdentityLC();//1
    newGroupId->SetIdentityL( _L("rakesh.harsh/Friends") );

    req = presGroups.CreatePresentityGroupL( *newGroupId,_L("my_friend") );
    // wait completion
    ctxObserver->WaitOpToCompleteL( req );
      
    CUserAfter::AfterL( 1000000 );*/
    
    //////////////////////////////////////////////////////////////////////////
    // ADD PRESENTITY GROUP MEMBER
    ////////////////////////////////////////////////////////////////////////// 
    
    // Setup event listener for RequestComplete
    ctxObserver->Reset();
    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqId ); // << evReqComplete

    // group id
    CXIMPIdentityImp* grid = CXIMPIdentityImp::NewLC( testGroupId->Identity() ); // << grid
    evReqComplete->AppendParamL( grid );
    CleanupStack::Pop(); // >> grid

    // id for member info
    CXIMPIdentityImp* miId = CXIMPIdentityImp::NewLC( KMemberId1 ); // << miId
    // member info
    CPresentityGroupMemberInfoImp* memInfoImp =
        CPresentityGroupMemberInfoImp::NewLC( *miId, KMember1DisplayName ); // << memInfoImp
    evReqComplete->AppendParamL( memInfoImp );
    CleanupStack::Pop(); // memInfoImp owned by AppendParamL    // >> memInfoImp
    CleanupStack::PopAndDestroy( miId ); // copy was made   // >>> miId

    ctxObserver->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete ); // >> evReqComplete

    // do the call
    reqId = presGroups.AddPresentityGroupMemberL(
            *testGroupId, *member1, KMember1DisplayName );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    messenger->AssertAddPresentityGroupMemberCalled( ETrue );

 
    //add a member
/*    MXIMPIdentity* testContactId = objFactory.NewIdentityLC();
    testContactId->SetIdentityL( _L("mukesh") );
    req = presGroups.AddPresentityGroupMemberL( *newGroupId,
                                                *testContactId,
                                                _L("h2") );
    // wait completion
    ctxObserver->WaitOpToCompleteL( req );
    
    
    CUserAfter::AfterL( 1000000 );*/
    
    
    //////////////////////////////////////////////////////////////////////////
    // ADD PRESENTITY GROUP MEMBER
    ////////////////////////////////////////////////////////////////////////// 

    // Setup event listener for RequestComplete
    ctxObserver->Reset();
    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqId ); // << evReqComplete

    // group id
    CXIMPIdentityImp* grid2 = CXIMPIdentityImp::NewLC( testGroupId->Identity() );
    evReqComplete->AppendParamL( grid2 );
    CleanupStack::Pop(); // grid2

    // id for member info
    CXIMPIdentityImp* miId2 = CXIMPIdentityImp::NewLC( KMemberId2 );
    // member info
    CPresentityGroupMemberInfoImp* memInfoImp2 =
        CPresentityGroupMemberInfoImp::NewLC( *miId2, KMember2DisplayName );
    evReqComplete->AppendParamL( memInfoImp2 );
    CleanupStack::Pop(); // memInfoImp owned by AppendParamL
    CleanupStack::PopAndDestroy( miId2 ); // copy was made

    ctxObserver->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );
    // reset the variable
    messenger->SetValueFor( EXIMPPlgTestAddPresentityGroupMemberCalled, 0 );
    // do the call
    reqId = presGroups.AddPresentityGroupMemberL(
            *testGroupId, *member2, KMember2DisplayName );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    messenger->AssertAddPresentityGroupMemberCalled( ETrue );

    
/*    testContactId->SetIdentityL( _L("bb0") );
    req = presGroups.AddPresentityGroupMemberL( *newGroupId,
                                                *testContactId,
                                                _L("b1") );
    // wait completion
    ctxObserver->WaitOpToCompleteL( req );
    
*/    
    //////////////////////////////////////////////////////////////////////////
    // SUBSCRIBE PRESENTITY GROUP MEMBERS PRESENCE
    ////////////////////////////////////////////////////////////////////////// 
        
    // Setup status event listener for RequestComplete
    ctxObserver->Reset();
    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqId );
    ctxObserver->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );
    SetupListenerReqCompleteL( wrapper, EXIMPTestStatusEvents_RequestOk );

    
    // Set filter for the group
    MPresenceInfoFilter* gpif = presFeat->PresenceObjectFactory().NewPresenceInfoFilterLC();
    gpif->AcceptPersonFilterL( NPresenceInfo::NFieldType::KStatusMessage  );
    gpif->AcceptPersonFilterL( NPresenceInfo::NFieldType::KAvatar );

    // register interest
    reqId = presWatch.SubscribePresentityGroupMembersPresenceL( *testGroupId, *gpif );
    
    CleanupStack::PopAndDestroy(); // >>> gpif

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Subscribe presentity group members, complete with error: ") );
        
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresentityGroupMembersPresesenceCalled, "SubscribePresentityGroupMembersPresenceL was not called", ETrue, NULL );

     

/*    CUserAfter::AfterL( 1000000 );
    MPresenceWatching& presWatch = presenceCtx->PresenceWatching();
    MPresenceInfoFilter* infoFilt = objFactory.NewPresenceInfoFilterLC();
    req = presWatch.SubscribePresentityGroupMembersPresenceL( *newGroupId, *infoFilt );
    // wait completion
    ctxObserver->WaitOpToCompleteL( req );
    
   // req = presWatch.SubscribePresentityPresenceL( *testContactId, *infoFilt );
    // wait completion
   // ctxObserver->WaitOpToCompleteL( req );
    //MXIMPClient* presClient1 = NULL;
   // MXIMPContext* presenceCtx1 = NULL;
    //CUSContextObserver* ctxObserver1  = NULL;
    
    /*TInt pushCount1 = DoSetupDefaultContext1LC( presClient1,
                                              presenceCtx1,
                                              ctxObserver1 );
    
    ctxObserver->WaitOpToCompleteL( req );*/
 //   CUserAfter::AfterL( 60000000 ); //60 seconds
    
    /*if ( !iWait.IsStarted() )
			{    
			iWait.Start();  // CSI: 10 # iWait is not an active object
			}
     */
    //code is for login with diff id 
     //Initialize XIMP client
 /*  #if 0
    MXIMPClient* presClient1 = MXIMPClient::NewClientL();
    CleanupDeletePushL( presClient );
      
    //Create new sink to receive presence context events
    CUSContextObserver* eventObserver1 = CUSContextObserver::NewLC();

    //Create new presence context
    MXIMPContext* presecenCtx1 = presClient->NewPresenceContextLC();
    presecenCtx1->RegisterObserverL( *eventObserver1 );

    //Bind context to desired presence service
    TUid protocolUid = TUid::Uid( KProtocolUid );


    //Bind context to desired presence service
     req = presecenCtx1->BindToL( protocolUid,
                                               KServerAddress,
                                               KUserName2,
                                               KPassword2,
                                               KIAP );
 
    //Wait with CActiveSchedulerWait the binding to complete
    //If binding fails, it is handled with leave
    eventObserver1->WaitOpToCompleteL( req );
    MXIMPIdentity* newGroupId1 = objFactory.NewIdentityLC();//1
    newGroupId1->SetIdentityL( _L("bb0/jaya") );

     req = presGroups.CreatePresentityGroupL( *newGroupId1,_L("my_sweety") );
    // wait completion
    ctxObserver->WaitOpToCompleteL( req );
      
    CUserAfter::AfterL( 1000000 );
    presecenCtx1->UnbindL();
    eventObserver1->WaitOpToCompleteL( req );
    CleanupStack::PopAndDestroy( 4 ); //newGroupId1,presecenCtx1,eventObserver1,presClient1
    
    #endif*/
    //code is for login with diff id end
    
    //add this user to bb0's contact list.i should  recieve notification
    
    //   delete group friend which is created above statement 
    //req = presGroups.DeletePresentityGroupL( *newGroupId );
    // wait completion
   // ctxObserver->WaitOpToCompleteL( req );
 //   CUserAfter::AfterL( 60000000 );
    //Unbind the context and wait completion
  //  req = presenceCtx->UnbindL();
  //  ctxObserver->WaitOpToCompleteL( req );

    //Destroy the context (it is automaticly unbind)
 //   CleanupStack::PopAndDestroy( 3 ); //  infoFilt,testContactId,newGroupId
 //   CleanupStack::PopAndDestroy( pushCount );	
 
 
    //////////////////////////////////////////////////////////////////////////
    // SEND DATA FOR GROUP MEMBER -> HANDLE PRESENTITY PRESENCE
    ////////////////////////////////////////////////////////////////////////// 

     
     // create test message
    CPresenceInfoImp* presenceInfo = CPresenceInfoImp::NewLC(); // << presenceInfo
    HBufC8* packedInfo = TXIMPObjectPacker< CPresenceInfoImp >::PackL( *presenceInfo );
    CleanupStack::PopAndDestroy( presenceInfo );// >>> presenceInfo
    CleanupStack::PushL( packedInfo );  // << packedInfo
    
    CXIMPIdentityImp* changed = CXIMPIdentityImp::NewLC( ); // << changed
    changed->SetIdentityL( KMemberId1 );
    HBufC8* packedId = TXIMPObjectPacker< CXIMPIdentityImp >::PackL( *changed );
    CleanupStack::PopAndDestroy(); // >>> changed
    CleanupStack::PushL( packedId );// << packedId
    
    RPointerArray< HBufC8 > bufferArray;
    CleanupClosePushL( bufferArray );   // << bufferArray
    bufferArray.AppendL( packedId );
    bufferArray.AppendL( packedInfo );
    
    HBufC8* packedBufferArray = TXIMPHBuf8Packer::PackArrayL( bufferArray );
    CleanupStack::PopAndDestroy( 2 ); // >>> packedId, packedInfo
    CleanupStack::PopAndDestroy(); // >>> bufferArray
    CleanupStack::PushL( packedBufferArray );   // << packedBufferArray
    
    // Send it to plugin 
    CXIMPTestFileSrvMsg* srvMsg = CXIMPTestFileSrvMsg::NewL( CXIMPTestFileSrvMsg::ETestSrvMsgPresentityPresence, 
                                                             *packedBufferArray );
    CleanupStack::PopAndDestroy( packedBufferArray );   // >>> packedBufferArray
    
    CleanupStack::PushL( srvMsg );  // << srvMsg
    wrapper->FileTool().SrvMsgStoreL( srvMsg );
    CleanupStack::PopAndDestroy( srvMsg );  // >>> srvMsg
    wrapper->FileTool().CleanAllL();

    // Wait for server to handle srvmsg.
    User::After( 4 * 1000000 );       

    COMMONASSERT( messenger, EXIMPPlgTestHandlePresentityPresenceCalled, "HandlePresentityPresenceL was not called", ETrue, NULL );

    CleanupStack::PopAndDestroy( 3 ); // >>> testGroupId, member1, member2
    CleanupStack::PopAndDestroy( ctxObserver ); // >>> ctxObserver
    
    UnbindL(); 

}
// ===========================================================================
// SETUP HELPERS FOR THE LISTENER
// ===========================================================================
//

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// send a faked server-originated message with watcher info made
// from the given uri and displayname
//
void T_PresenceWatcherListManagement::SendSrvMsgL(
        const TDesC& aUri, const TDesC& aDispName,
        TInt aMsgType )
    {
    // we'll use hard-coded wrapper 0 instance for now.
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );

    // create a grant req info contained within the server-originated
    // faked message
    RXIMPObjOwningPtrArray<CPresenceWatcherInfoImp> watcherList;
    CleanupClosePushL( watcherList );

    CXIMPIdentityImp* watcherId = CXIMPIdentityImp::NewLC( aUri );
    CPresenceWatcherInfoImp* watcherItem =
                CPresenceWatcherInfoImp::NewLC(
                            (MPresenceWatcherInfo::TWatcherType)0,
                            *watcherId,
                            aDispName );
    watcherList.AppendL( watcherItem );
    CleanupStack::Pop( watcherItem );
    CleanupStack::PopAndDestroy( watcherId );

    HBufC8* packedArray =
        TXIMPObjectPacker<CPresenceWatcherInfoImp>::PackArrayL( watcherList );
    CleanupStack::PushL( packedArray );

    // use filetool to send it for plugin.
    CXIMPTestFileSrvMsg* srvMsg = CXIMPTestFileSrvMsg::NewL( aMsgType,
            *packedArray );
    CleanupStack::PushL( srvMsg );
    wrapper->FileTool().CleanAllL();
    wrapper->FileTool().SrvMsgStoreL( srvMsg );
    CleanupStack::PopAndDestroy( 3 ); // srvMsg, packedArray, watcherList
    }


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// send a faked server-originated message with empty contents
//
void T_PresenceWatcherListManagement::SendSrvMsgL( TInt aMsgType )
    {
    // we'll use hard-coded wrapper 0 instance for now.
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );

    // create a server-originated faked message with empty contents
    RXIMPObjOwningPtrArray<CPresenceWatcherInfoImp> watcherList;

    HBufC8* packedArray = TXIMPObjectPacker<CPresenceWatcherInfoImp>::PackArrayL( watcherList );
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
// create a watcher list event with given arrays
//
CPresenceWatcherListEventImp* T_PresenceWatcherListManagement::CreateWatcherListEventLCX(
        const TDesC& aUri,
        const TDesC& aDispName,
            MXIMPDataSubscriptionState::TSubscriptionState aSubscriptionState,
            MXIMPDataSubscriptionState::TDataState aDataState,
            TTestPWlOperation aOperation,
            RPrWatLstInfoImpArray* aCurrentList)
    {
    // create the arrays


    // new list
    RPrWatLstInfoImpArray* newList = new ( ELeave ) RPrWatLstInfoImpArray;
    CleanupDeletePushL( newList );

    // current list, created by caller
    if(!aCurrentList) // if client didnt created the list
        aCurrentList = new ( ELeave ) RPrWatLstInfoImpArray;
    CleanupDeletePushL( aCurrentList );

    // disappeared list
    RPrWatLstInfoImpArray* disappearedList = new ( ELeave ) RPrWatLstInfoImpArray;
    CleanupDeletePushL( disappearedList );

    // create the watcher info
    CPresenceWatcherInfoImp* watcherInfoForEvent1 = NULL; 
            {
            CXIMPIdentityImp* idForEvent = CXIMPIdentityImp::NewLC( aUri );
            watcherInfoForEvent1 = CPresenceWatcherInfoImp::NewLC( (MPresenceWatcherInfo::TWatcherType)0, *idForEvent, aDispName );
            CleanupStack::Pop( watcherInfoForEvent1 );        
            CleanupStack::PopAndDestroy( idForEvent ); 
            CleanupStack::PushL( watcherInfoForEvent1 );        
            }
    
    CPresenceWatcherInfoImp* watcherInfoForEvent2 = 
            TXIMPObjectCloner< CPresenceWatcherInfoImp >::CloneLC( *watcherInfoForEvent1 );
    
    TInt count(0);
    TBool found(EFalse);
    TInt i(0);
    
    // put the given watcher info into the specified array
    switch ( aOperation )
        {
        case ETestPWlNew:
            {
            newList->AppendL( watcherInfoForEvent2 );
            CleanupStack::Pop( watcherInfoForEvent2 );
            
            aCurrentList->AppendL( watcherInfoForEvent1 );
            CleanupStack::Pop( watcherInfoForEvent1 );
            }
            break;
        case ETestPWlCurrent:
            {
            // don't add, thus return what user has given
            CleanupStack::PopAndDestroy( watcherInfoForEvent2 ); // watcherInfoForEvent2
            CleanupStack::PopAndDestroy( watcherInfoForEvent1 ); // watcherInfoForEvent1
            }
            break;
        case ETestPWlDisappeared:
            {
            // search the given id in current list
            count = aCurrentList->Count();
            for(i=0;i<count;i++)
                {
                // if dispname and id are same
                if (( ((((*aCurrentList)[i])->WatcherId()).Identity()) == aUri )
                    && ( (((*aCurrentList)[i])->WatcherDisplayName()) == aDispName ))
                    {
                    found = ETrue;
            break;            
                    }
                }
            if(found) // do we need to leave if error? Waqas
            {
                delete (*aCurrentList)[i];
                aCurrentList->Remove(i);
                disappearedList->AppendL( watcherInfoForEvent2 );
                CleanupStack::Pop( watcherInfoForEvent2 );
                }
            else
                CleanupStack::PopAndDestroy( watcherInfoForEvent2 );
            
            CleanupStack::PopAndDestroy( watcherInfoForEvent1 );
            }
            break;
        default:
            {
            User::Leave( KErrArgument );
            }
            break;            
        };

    CXIMPDataSubscriptionStateImp* stateImp = CXIMPDataSubscriptionStateImp::NewLC();
    stateImp->SetDataStateL( aDataState );
    stateImp->SetSubscriptionStateL( aSubscriptionState );
    CleanupStack::Pop( stateImp );

    // create the actual event
    CPresenceWatcherListEventImp* tmp =
        CPresenceWatcherListEventImp::NewLC(
                newList,
                aCurrentList,
                disappearedList,
                stateImp );
     

    /*
     * In cleanupstack there are 4 items, FIFO
     *   - new list
     *   - current list
     *   - disappeared list
     *   - event imp
     */

    return tmp;
    }

void T_PresenceWatcherListManagement::SetupListenerReqCompleteL( CXIMPTestContextWrapper* aWrapper, TXIMPTestStatusEventTemplate aEventType )
    {
    RArray< TInt32 > array;
    CleanupClosePushL( array );
    array.AppendL( MXIMPRequestCompleteEvent::KInterfaceId );
    array.Append( MXIMPContextStateEvent::KInterfaceId );
    array.Append( MPresentityPresenceEvent::KInterfaceId );
    aWrapper->SetupListenerReqCompleteL( aEventType, &array );
    CleanupStack::PopAndDestroy(); // array
    }

void T_PresenceWatcherListManagement::SetupListenerReqCompleteAllowEventsL(  CXIMPTestContextWrapper* aWrapper, TXIMPTestStatusEventTemplate aEventType )
    {
    RArray< TInt32 > array;
    CleanupClosePushL( array );
    array.AppendL( MXIMPRequestCompleteEvent::KInterfaceId );
    array.AppendL( MXIMPContextStateEvent::KInterfaceId );
    array.AppendL( MPresentityPresenceEvent::KInterfaceId );
    aWrapper->SetupListenerReqCompleteL( aEventType, &array );
    CleanupStack::PopAndDestroy(); // array
    }

// ===========================================================================
// EVENT OBSERVER METHODS
// ===========================================================================
//
void T_PresenceWatcherListManagement::HandlePresenceContextEvent(
    const MXIMPContext& aContext,
    const MXIMPBase& aEvent )
    {
    iLastError = KErrNone;
    TInt ifId = aEvent.GetInterfaceId();
    TInt gg = MPresentityPresenceEvent::KInterfaceId;
    if( ifId == MPresenceWatcherListEvent::KInterfaceId )
        {
        const MPresenceWatcherListEvent* watcherListEvent = 
        TXIMPGetInterface< const MPresenceWatcherListEvent >::From( aEvent, MXIMPBase::EPanicIfUnknown );
        
        if( watcherListEvent->NewWatchersCount() )
            {
            iLastEvent = EWatcherList;
            TInt count = watcherListEvent->NewWatchersCount();
            for( TInt a = 0; a < count; ++a )
                {
                TRAPD( error,
                    CPresenceWatcherInfoImp* watcherInfo = CPresenceWatcherInfoImp::NewLC(
                        watcherListEvent->NewWatcher( a ).WatcherType(),
                        watcherListEvent->NewWatcher( a ).WatcherId(),
                        watcherListEvent->NewWatcher( a ).WatcherDisplayName() );
                    iValidateArray.AppendL( watcherInfo );
                    CleanupStack::Pop( watcherInfo );
                    );
                if( error != KErrNone )
                    {
                    iLastError = error;
                    return;
                    }
                }
            }
            }
    else if( ifId == MPresentityPresenceEvent::KInterfaceId )
        {
        const MPresentityPresenceEvent* presentityPresenceEvent = 
        TXIMPGetInterface< const MPresentityPresenceEvent >::From( aEvent, MXIMPBase::EPanicIfUnknown );
        const MXIMPIdentity& prId = presentityPresenceEvent->PresentityId();
        const MPresenceInfo& prPif = presentityPresenceEvent->SubscribedPresence(); 
        const MXIMPDataSubscriptionState& prDataStae =  presentityPresenceEvent->DataSubscriptionState();
        }
    else
        {
        iLastEvent = EInvalid;
        }
    }

// ===========================================================================
// TEST CASE TABLE
// ===========================================================================
//
EUNIT_BEGIN_TEST_TABLE(
        T_PresenceWatcherListManagement,
        "XIMP presence watching tests",
        "MODULE" )

PRFW_DECORATED_TEST(
        "Bind Unbind",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_BindUnbind_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Single client watcher list subscription",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_SubscribePresenceWatcherList_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Multiple client watcher list subscription",
        "",
        "",
        "FUNCTIONALITY",
        SetupMultiple_L,
        T_SubscribePresenceWatcherList_Multiple_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Handle Presence Watcher List",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_HandlePresenceWatcherList_L,
        Teardown )
        
 PRFW_DECORATED_TEST(
        "Test India Case",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_IndiaCase_L,
        Teardown )


EUNIT_END_TEST_TABLE


// ===========================================================================
// TEST SUITE CONSTRUCTION
// ===========================================================================
//
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    return T_PresenceWatcherListManagement::NewL();
    }


// end of file
