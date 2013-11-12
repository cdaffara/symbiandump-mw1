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
* Description: XIMP Framework Test Code 
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
#include <ximprequestcompleteevent.h>
#include <ximpstatus.h>
#include <presencefeatures.h>
#include <presencewatching.h>
#include <presencepublishing.h>
#include <presentitypresenceevent.h>
#include <ximpobjectfactory.h>

#include <presentitygroups.h>
#include <presentitygroupinfo.h>
#include <presentitygrouplistevent.h>


#include <presentitygroups.h>
#include <presentitygroupmemberinfo.h>
#include <presentitygroupcontentevent.h>

#include <presenceauthorization.h>
#include <presenceblocklistevent.h>
#include <presenceblockinfo.h>

#include "t_presenceall.h"
#include "prfwteststatuseventlistener.h"
#include "prfwtestmessaging.h"
#include "prfwtestmessenger.h"
#include "prfwtestcontextwrappermgr.h"
#include "prfwtestcontextwrapper.h"

#include "presenceinfoimp.h"
#include "presenceinfofilterimp.h"
#include "prfwtestpresencedatautils.h"
#include "ximpidentityimp.h"
#include "presentitygroupmemberinfoimp.h"
#include "ximpobjecthelpers.h"
#include "prfwtestfilesrvmsg.h"
#include "prfwtestwaithelper.h"
#include "prfwtestlistener.h"


#include "presentitygroupcontenteventimp.h"
#include "ximprequestcompleteeventimp.h"
#include "presentitygroupinfoimp.h"
#include "presentitygrouplisteventimp.h"
#include "presentitygroupmemberinfoimp.h"
#include "ximprequestcompleteeventimp.h"
#include "ximprestrictedobjectcollection.h"
#include "ximpobjectfactoryimp.h"

#include "prfwtesteventfactory.h"

#include "prfwtestpresencedatautils.h"

#include "ximpobjecthelpers.h"
#include "presentitygroupinfoimp.h"
#include "presentitygrouplisteventimp.h"
#include "presentitygroupcontenteventimp.h"

#include "prfwtestfiletool.h"


#include "prfwtestrobustnesstools.h"

_LIT8( KPrefix1, "PreA_" );
_LIT8( KPrefix2, "PreB_" );
_LIT8( KPrefix3, "PreC_" );
_LIT8( KPrefix4, "PreD_" );

_LIT( KPrefix5, "PreXX_" );
_LIT( KPrefix6, "PreYY_" );

_LIT( KIdentity, "TestId@hello.world" );


_LIT( KMemberId1, "tel:user1@foo.bar" );
_LIT( KMemberDisplayName1,  "user1" );

_LIT( KMemberId2, "tel:user2@foo.bar" );
_LIT( KMemberDisplayName2,  "user2" );

_LIT( KGroupId, "PrsDemoGrp" );
_LIT( KGroupDisplayName,  "demo group" );



// ============================ MEMBER FUNCTIONS ===============================

// CONSTRUCTION
T_PresenceAll* T_PresenceAll::NewL()
    {
    T_PresenceAll* self = new( ELeave ) T_PresenceAll;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


T_PresenceAll::~T_PresenceAll()
    {
    }


void T_PresenceAll::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


T_PresenceAll::T_PresenceAll()
    {
    }



// ===========================================================================
// TEST CASE SETUP & TEARDOWN METHODS
// ===========================================================================
//

void T_PresenceAll::Setup_L()
    {
    __UHEAP_MARK;
    PrfwTestRobustness::DoPreCleaning();

    iWrapperMgr = CXIMPTestContextWrapperMgr::NewL();
    iWrapperMgr->CreateWrapperL();
    }

void T_PresenceAll::SetupMultiple_L()
    {
    PrfwTestRobustness::DoPreCleaning();

    iWrapperMgr = CXIMPTestContextWrapperMgr::NewL();
    iWrapperMgr->CreateWrapperL();
    iWrapperMgr->CreateWrapperL();
    iWrapperMgr->CreateWrapperL();
    }


void T_PresenceAll::BindL()
    {
    CXIMPTestContextWrapper* wrapper0 = iWrapperMgr->GetWrapperL( 0 );

    wrapper0->BindL( 0 );
    }

void T_PresenceAll::BindAllL()
    {
    for( TInt a = 0; a < iWrapperMgr->WrapperCount(); ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        wrapper->BindL( 0 );
        }
    }


void T_PresenceAll::Teardown()
    {
    delete iWrapperMgr;
    iWrapperMgr = NULL;

    REComSession::FinalClose();

    PrfwTestRobustness::DoPreCleaning();
    User::SetJustInTime( ETrue );
    __UHEAP_MARKEND;
    }

void T_PresenceAll::UnbindL()
    {
    // Do unbind
    CXIMPTestContextWrapper* wrapper0 = iWrapperMgr->GetWrapperL( 0 );
    wrapper0->UnbindL();
    }

void T_PresenceAll::UnbindAllL()
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

// Jani
void T_PresenceAll::T_TMO_demo_Startup_L()
    {
    EUNIT_PRINT( _L("T_TMO_demo_Startup_L") );

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();
    
        
    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresencePublishing& presPub = presFeat->PresencePublishing();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();
    MPresenceWatching& presWatch = presFeat->PresenceWatching();
    
    
    
    //////////////////////////////////////////////////////////////////////////
    // SUBSCRIBE OWN PRESENCE
    //////////////////////////////////////////////////////////////////////////

    // Create own presence info filter
    CPresenceInfoFilterImp* ownPif = CPresenceInfoFilterImp::NewLC(); // << ownPif
    ownPif->AcceptPersonFilterL( NPresenceInfo::NFieldType::KAcceptAll );
    //ownPif->AcceptPersonFilterL( NPresenceInfo::NFieldType::KStatusMessage );
    //ownPif->AcceptPersonFilterL( NPresenceInfo::NFieldType::KAvatar );

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // subscribe own presence
    TXIMPRequestId reqId = presPub.SubscribeOwnPresenceL( *ownPif );
    
    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Subscribing own presence, complete with error: ") );

    // verify that SubscribeOwnPresenceL was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribeOwnPresenceCalled, "SubscribeOwnPresenceL was not called", ETrue, NULL );
    
    //////////////////////////////////////////////////////////////////////////
    // SUBSCRIBE PRESENCE WATCHER LIST
    //////////////////////////////////////////////////////////////////////////

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // subscribe
    reqId = presPub.SubscribePresenceWatcherListL();
    
    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Subscribing presence watcher list, complete with error: ") );

    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresenceWatcherListCalled, "SubscribeWatcherListL was not called", ETrue, NULL );
    

    //////////////////////////////////////////////////////////////////////////
    // SUBSCRIBE PRESENCE BLOCK LIST
    //////////////////////////////////////////////////////////////////////////

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // subscribe
    reqId = presAuth.SubscribePresenceBlockListL();
    
    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Subscribing presence watcher list, complete with error: ") );

    // verify that SubscribeBlockListL was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresenceBlockListCalled, "SubscribeBlockListL was not called", ETrue, NULL );
    

    
    //////////////////////////////////////////////////////////////////////////
    // SUBSCRIBE PRESENTITY GROUP LIST
    //////////////////////////////////////////////////////////////////////////
    
    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // subscribe
    reqId = presGroup.SubscribePresentityGroupListL();
    
    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Subscribing presentity group list, complete with error: ") );
    
    // verify that SubscribePresentityGroupListL was called
    messenger->AssertSubscribePresentityGroupListCalled( ETrue );
    
    
    //////////////////////////////////////////////////////////////////////////
    // EMPTY GROUP LIST EVENT
    /////////////////////////////////////////////////////////////////////////
    // According to design, this should not be called
    // because the group list is empty, this can be removed
    // Create help listener

/*    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );    // << listener2    
    
    // Create faked server message about group list
    SendSrvMsgL( CXIMPTestFileSrvMsg::ETestSrvMsgPresentityGroupList );

    // expect empty MPresentityGroupList event.
    CPresentityGroupListEventImp* event = XIMPTestEventFactory::CreateGroupListEventLCX(
            KNullDesC, KNullDesC,
            XIMPTestEventFactory::ETestPGLEmpty,
            KNullDesC, KNullDesC,
            MXIMPDataSubscriptionState::ESubscriptionActive,
            MXIMPDataSubscriptionState::EDataAvailable
            );

    listener2->ExpectL( event );
    CleanupStack::Pop( event ); // 4 more items in cleanupstack

    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "HandlePresentityGroupListL failed" );
    CleanupStack::PopAndDestroy( 4 ); // lists
    CleanupStack::PopAndDestroy( listener2 );   // >>> listener2

*/        
    //////////////////////////////////////////////////////////////////////////
    // CREATE PRESENTITY GROUP
    /////////////////////////////////////////////////////////////////////////
    
    // Setup status event listener for RequestComplete
    // This will also produce HandlePresentityGroupListEvent, since the
    // the group is subscribed
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );


    MXIMPIdentity* groupId = context->ObjectFactory().NewIdentityLC();
    CleanupStack::Pop(); // groupId
    groupId->SetIdentityL( KGroupId );
    
    // subscribe
    reqId = presGroup.CreatePresentityGroupL( *groupId, KGroupDisplayName );
    
    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("CreatePresentityGroupL failed, complete with error: ") );
    
    // verify that SubscribePresentityGroupListL was called
    messenger->AssertCreatePresentityGroupCalled( ETrue );

    
  
    //////////////////////////////////////////////////////////////////////////
    // ADD PRESENTITY GROUP MEMBER
    ////////////////////////////////////////////////////////////////////////// 
    
    CXIMPTestListener* listener4 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener4 );    // << listener4
    
    MXIMPIdentity* member = context->ObjectFactory().NewIdentityLC(); // << member
    member->SetIdentityL( KMemberId1 );


    // request complete
    TXIMPRequestId reqIdDummy;
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );

    // group id
    CXIMPIdentityImp* identity2 = CXIMPIdentityImp::NewLC( groupId->Identity() );
    evReqComplete->AppendParamL( identity2 );
    CleanupStack::Pop(identity2); // identity2

    // id for member info
    CXIMPIdentityImp* miId = CXIMPIdentityImp::NewLC( KMemberId1 );
    // member info
    CPresentityGroupMemberInfoImp* memInfoImp =
        CPresentityGroupMemberInfoImp::NewLC( *miId, KMemberDisplayName1 );
    evReqComplete->AppendParamL( memInfoImp );
    CleanupStack::Pop(memInfoImp); // memInfoImp owned by AppendParamL
    CleanupStack::PopAndDestroy( miId ); // copy was made

    listener4->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // do the call
    reqId = presGroup.AddPresentityGroupMemberL(
            *groupId, *member, KMemberDisplayName1 );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == listener4->WaitAndAssertL(), "AddPresentityGroupMemberL (not subscribed) failed" );

    messenger->AssertAddPresentityGroupMemberCalled( ETrue );
    CleanupStack::PopAndDestroy(); // >>> member
    CleanupStack::PopAndDestroy( listener4 );// >>> listener4


    //////////////////////////////////////////////////////////////////////////
    // SUBSCRIBE PRESENTITY GROUP CONTENT
    //////////////////////////////////////////////////////////////////////////    

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );
    
    // subscribe group content
    MXIMPIdentity* identity = context->ObjectFactory().NewIdentityLC();
    identity->SetIdentityL( KGroupId ); // << identity

    reqId = presGroup.SubscribePresentityGroupContentL( *identity );
    
    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Subscribe presentity group content, complete with error: ") );

    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresentityGroupContentCalled, "SubscribeGroupContentL was not called", ETrue, NULL );
    
    //////////////////////////////////////////////////////////////////////////
    // SUBSCRIBE PRESENTITY GROUP MEMBERS PRESENCE
    ////////////////////////////////////////////////////////////////////////// 
        
    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // This is the group we are interested in
    MXIMPIdentity* group = context->ObjectFactory().NewIdentityLC();
    CleanupStack::Pop(); // group
    group->SetIdentityL( KGroupId );
    // Set filter for the group
    //CPresenceInfoFilterImp* gpif = CPresenceInfoFilterImp::NewLC();
    //gpif->AcceptPersonFilterL( NPresenceInfo::NFieldType::KStatusMessage  );
    //gpif->AcceptPersonFilterL( NPresenceInfo::NFieldType::KAvatar );
    
    
    
    CPresenceInfoFilterImp* gpif =
                XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );
    
    CleanupStack::Pop(gpif); // gpif

    // register interest
    reqId = presWatch.SubscribePresentityGroupMembersPresenceL( *group, *gpif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Subscribe presentity group members, complete with error: ") );
        
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresentityGroupMembersPresesenceCalled, "SubscribePresentityGroupMembersPresenceL was not called", ETrue, NULL );

    delete gpif;

    
    //////////////////////////////////////////////////////////////////////////
    // ADD PRESENTITY GROUP MEMBER, GROUP CONTENT SUBSCRIBED
    ////////////////////////////////////////////////////////////////////////// 
    
    // Setup status event listener for RequestComplete
    // This will also produce HandlePresentityGroupMemberAddedL, since the
    // the group is subscribed
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );
    
    MXIMPIdentity* member2 = context->ObjectFactory().NewIdentityLC(); // << member2
    member2->SetIdentityL( KMemberId2 );

    // do the call
    reqId = presGroup.AddPresentityGroupMemberL(
            *groupId, *member2, KMemberDisplayName2 );
            
    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("AddPresentityGroupMemberL (subscribed) failed, complete with error: ") );


    messenger->AssertAddPresentityGroupMemberCalled( ETrue );
    CleanupStack::PopAndDestroy(); // >>> member2


   
    //////////////////////////////////////////////////////////////////////////
    // GRANT PRESENTITY GROUP MEMBER
    //////////////////////////////////////////////////////////////////////////

    // This is the group we are interested in
    MXIMPIdentity* ggroup = context->ObjectFactory().NewIdentityLC();
    CleanupStack::Pop(); // ggroup
    ggroup->SetIdentityL( KGroupId );
    // Set filter for the group
    CPresenceInfoFilterImp* ggpif = CPresenceInfoFilterImp::NewLC();
    //ggpif->AcceptPersonFilterL( NPresenceInfo::NFieldType::KAcceptAll  );
    CleanupStack::Pop(); // ggpif

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // grant for all
    reqId = presAuth.GrantPresenceForPresentityGroupMembersL( *ggroup, *ggpif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForPresentityGroupMembersCalled, 
        "GrantPresenceForPresentityGroupMembersL was not called", ETrue, NULL );
        
    delete ggpif;    


    //////////////////////////////////////////////////////////////////////////
    // WITHDRAW PRESENTITY GROUP MEMBER
    //////////////////////////////////////////////////////////////////////////


    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // withdraw from all
    reqId = presAuth.WithdrawPresenceGrantFromPresentityGroupMembersL( *ggroup );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForPresentityGroupMembersCalled, 
        "GrantPresenceForPresentityGroupMembersL was not called", ETrue, NULL );
    
    delete ggroup;
            

    //////////////////////////////////////////////////////////////////////////
    // UNSUBSCRIBE PRESENTITY GROUP MEMBERS
    ////////////////////////////////////////////////////////////////////////// 
        
    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // unregister interest
    reqId = presWatch.UnsubscribePresentityGroupMembersPresenceL( *group );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Unsubscribe presentity group members, complete with error: ") );
        
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresentityGroupMembersPresesenceCalled, "UnsubscribePresentityGroupMembersPresenceL was not called", ETrue, NULL );


    
    //////////////////////////////////////////////////////////////////////////
    // UNSUBSCRIBE PRESENTITY GROUP CONTENT
    ////////////////////////////////////////////////////////////////////////// 
    
    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );
    
    // unsubscribe
    reqId = presGroup.UnsubscribePresentityGroupContentL( *identity );
        
    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Unsubscribe presentity group content, complete with error: ") );

    // verify that UnsubscribePresentityGroupContentL was called
    messenger->AssertUnsubscribePresentityGroupContentCalled( ETrue );
    
    CleanupStack::PopAndDestroy(); // >>> identity

    //////////////////////////////////////////////////////////////////////////
    // UNSUBSCRIBE PRESENCE BLOCK LIST
    //////////////////////////////////////////////////////////////////////////

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // subscribe
    reqId = presAuth.UnsubscribePresenceBlockListL();
    
    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Unsubscribing presence watcher list, complete with error: ") );

    // verify that SubscribeBlockListL was called
    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresenceBlockListCalled, "UnubscribeBlockListL was not called", ETrue, NULL );
    
    //////////////////////////////////////////////////////////////////////////
    // UNSUBSCRIBE PRESENTITY GROUP LIST
    //////////////////////////////////////////////////////////////////////////
    
    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // unsubscribe
    reqId = presGroup.UnsubscribePresentityGroupListL();
    
    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Unsubscribing presentity group list, complete with error: ") );
    
    // verify that SubscribePresentityGroupListL was called
    messenger->AssertUnsubscribePresentityGroupListCalled( ETrue );
   

    //////////////////////////////////////////////////////////////////////////
    // UNSUBSCRIBE PRESENCE WATCHER LIST
    //////////////////////////////////////////////////////////////////////////

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // unsubscribe
    reqId = presPub.UnsubscribePresenceWatcherListL();

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Unsubscribing presence watcher list, complete with error: ") );

    // verify that UnsubscribeWatcherListL was called
    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresenceWatcherListCalled, "UnsubscribeWatcherListL was not called", ETrue, NULL );


    ////////////////////////////////////////////////////////////////////////////
    // UNSUBSCRIBE OWN PRESENCE
    ////////////////////////////////////////////////////////////////////////////
    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );    
    
    // unsubscribe own presence
    reqId = presPub.UnsubscribeOwnPresenceL();
    
    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    
    wrapper->VerifyEventStackL( _L8("Unsubscribing own presence, complete with error: ") );
        
    // verify that UnsubscribeOwnPresenceL was called
    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribeOwnPresenceCalled, "UnsubscribeOwnPresenceL was not called", ETrue, NULL );

    CleanupStack::PopAndDestroy( ownPif ); // >>> ownPif
    delete group;
    delete groupId;
   

    UnbindL();
    
    wrapper = NULL;
    context = NULL;
    messenger = NULL;
    


    }

// Grant/Withdraw from group
void T_PresenceAll::T_GrantWithdrawGroup_L()
    {
    EUNIT_PRINT( _L("T_GrantWithdrawGroup_L") );

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();
    
        
    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresencePublishing& presPub = presFeat->PresencePublishing();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();
    MPresenceWatching& presWatch = presFeat->PresenceWatching();
    
    
    
    //////////////////////////////////////////////////////////////////////////
    // CREATE PRESENTITY GROUP
    /////////////////////////////////////////////////////////////////////////
    
    // Setup status event listener for RequestComplete
    // This will also produce HandlePresentityGroupListEvent, since the
    // the group is subscribed
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );


    MXIMPIdentity* groupId = context->ObjectFactory().NewIdentityLC();
    CleanupStack::Pop(); // groupId
    groupId->SetIdentityL( KGroupId );
    
    // subscribe
    TXIMPRequestId reqId = presGroup.CreatePresentityGroupL( *groupId, KGroupDisplayName );
    
    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("CreatePresentityGroupL failed, complete with error: ") );
    
    // verify that SubscribePresentityGroupListL was called
    messenger->AssertCreatePresentityGroupCalled( ETrue );

    
  
    //////////////////////////////////////////////////////////////////////////
    // ADD PRESENTITY GROUP MEMBER
    ////////////////////////////////////////////////////////////////////////// 
    
    CXIMPTestListener* listener4 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener4 );    // << listener4
    
    MXIMPIdentity* member = context->ObjectFactory().NewIdentityLC(); // << member
    member->SetIdentityL( KMemberId1 );


    // request complete
    TXIMPRequestId reqIdDummy;
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );

    // group id
    CXIMPIdentityImp* identity2 = CXIMPIdentityImp::NewLC( groupId->Identity() );
    evReqComplete->AppendParamL( identity2 );
    CleanupStack::Pop(); // identity2

    // id for member info
    CXIMPIdentityImp* miId = CXIMPIdentityImp::NewLC( KMemberId1 );
    // member info
    CPresentityGroupMemberInfoImp* memInfoImp =
        CPresentityGroupMemberInfoImp::NewLC( *miId, KMemberDisplayName1 );
    evReqComplete->AppendParamL( memInfoImp );
    CleanupStack::Pop(); // memInfoImp owned by AppendParamL
    CleanupStack::PopAndDestroy( miId ); // copy was made

    listener4->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // do the call
    reqId = presGroup.AddPresentityGroupMemberL(
            *groupId, *member, KMemberDisplayName1 );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == listener4->WaitAndAssertL(), "AddPresentityGroupMemberL (not subscribed) failed" );

    messenger->AssertAddPresentityGroupMemberCalled( ETrue );
    CleanupStack::PopAndDestroy(); // >>> member
    CleanupStack::PopAndDestroy( listener4 );// >>> listener4
    
    delete groupId;

    //////////////////////////////////////////////////////////////////////////
    // ADD PRESENTITY GROUP MEMBER, GROUP CONTENT SUBSCRIBED
    ////////////////////////////////////////////////////////////////////////// 
    
    // Setup status event listener for RequestComplete
    // This will also produce HandlePresentityGroupMemberAddedL, since the
    // the group is subscribed
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );
    
    MXIMPIdentity* member2 = context->ObjectFactory().NewIdentityLC(); // << member2
    member2->SetIdentityL( KMemberId2 );

    // do the call
    reqId = presGroup.AddPresentityGroupMemberL(
            *groupId, *member2, KMemberDisplayName2 );
            
    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("AddPresentityGroupMemberL (subscribed) failed, complete with error: ") );


    messenger->AssertAddPresentityGroupMemberCalled( ETrue );
    CleanupStack::PopAndDestroy(); // >>> member2


   
    //////////////////////////////////////////////////////////////////////////
    // GRANT PRESENTITY GROUP MEMBER
    //////////////////////////////////////////////////////////////////////////

    // This is the group we are interested in
    MXIMPIdentity* ggroup = context->ObjectFactory().NewIdentityLC();
    CleanupStack::Pop(); // ggroup
    ggroup->SetIdentityL( KGroupId );
    // Set filter for the group
    CPresenceInfoFilterImp* ggpif = CPresenceInfoFilterImp::NewLC();
    ggpif->AcceptPersonFilterL( NPresenceInfo::NFieldType::KAcceptAll  );
    CleanupStack::Pop(); // ggpif

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // grant for all
    reqId = presAuth.GrantPresenceForPresentityGroupMembersL( *ggroup, *ggpif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForPresentityGroupMembersCalled, 
        "GrantPresenceForPresentityGroupMembersL was not called", ETrue, NULL );
    
    delete ggpif;
    


    //////////////////////////////////////////////////////////////////////////
    // WITHDRAW PRESENTITY GROUP MEMBER
    //////////////////////////////////////////////////////////////////////////


    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // withdraw from all
    reqId = presAuth.WithdrawPresenceGrantFromPresentityGroupMembersL( *ggroup );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForPresentityGroupMembersCalled, 
        "GrantPresenceForPresentityGroupMembersL was not called", ETrue, NULL );
    
    delete ggroup;        

    UnbindL();
    
    wrapper = NULL;
    context = NULL;
    messenger = NULL;

    }


// ===========================================================================
// SETUP HELPERS FOR THE LISTENER
// ===========================================================================
//
void T_PresenceAll::SetupListenerReqCompleteL( CXIMPTestContextWrapper* aWrapper, TXIMPTestStatusEventTemplate aEventType )
    {
    RArray< TInt32 > array;
    CleanupClosePushL( array );
    array.Append( MXIMPContextStateEvent::KInterfaceId );
    array.Append( MPresentityPresenceEvent::KInterfaceId );
    aWrapper->SetupListenerReqCompleteL( aEventType, &array );
    CleanupStack::PopAndDestroy(); // array
    }

void T_PresenceAll::SetupListenerReqCompleteAllowEventsL(  CXIMPTestContextWrapper* aWrapper, TXIMPTestStatusEventTemplate aEventType )
    {
    RArray< TInt32 > array;
    CleanupClosePushL( array );
    array.AppendL( MXIMPRequestCompleteEvent::KInterfaceId );
    array.AppendL( MXIMPContextStateEvent::KInterfaceId );
    array.AppendL( MPresentityPresenceEvent::KInterfaceId );
    aWrapper->SetupListenerReqCompleteL( aEventType, &array );
    CleanupStack::PopAndDestroy(); // array
    }

void T_PresenceAll::SendSrvMsgL(
        const TDesC& aUri, const TDesC& aDispName,
        TInt aMsgType )
    {
    // we'll use hard-coded wrapper 0 instance for now.
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );

    // create a group info contained within the server-originated
    // faked message
    CXIMPIdentityImp* groupId = CXIMPIdentityImp::NewLC( aUri );
    CPresentityGroupInfoImp* groupItem = CPresentityGroupInfoImp::NewLC( *groupId, aDispName );
    HBufC8* packet = NULL;
    if( aMsgType == CXIMPTestFileSrvMsg::ETestSrvMsgPresentityGroupDelete )
        {
        packet = TXIMPObjectPacker< CXIMPIdentityImp >::PackL( *groupId );        
        }
    else
        {
        packet = TXIMPObjectPacker< CPresentityGroupInfoImp >::PackL( *groupItem );        
        }
    CleanupStack::PushL( packet );
    
    // use filetool to send it for plugin.
    CXIMPTestFileSrvMsg* srvMsg = CXIMPTestFileSrvMsg::NewL( aMsgType, *packet );
    CleanupStack::PushL( srvMsg );
    wrapper->FileTool().CleanAllL();
    wrapper->FileTool().SrvMsgStoreL( srvMsg );
    CleanupStack::PopAndDestroy( 4, groupId ); // srvMsg, packet, groupItem, groupId
    }

// send a faked server-originated message with empty contents
//
void T_PresenceAll::SendSrvMsgL( TInt aMsgType )
    {
    // we'll use hard-coded wrapper 0 instance for now.
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );

    // create a server-originated faked message with empty contents
    RXIMPObjOwningPtrArray<CPresentityGroupInfoImp> groupList;

    HBufC8* packedArray = TXIMPObjectPacker<CPresentityGroupInfoImp>::PackArrayL( groupList );
    CleanupStack::PushL( packedArray );

    // use filetool to send it for plugin.
    CXIMPTestFileSrvMsg* srvMsg = CXIMPTestFileSrvMsg::NewL( aMsgType,
            *packedArray );
    CleanupStack::PushL( srvMsg );
    wrapper->FileTool().CleanAllL();
    wrapper->FileTool().SrvMsgStoreL( srvMsg );
    CleanupStack::PopAndDestroy( 2 ); // srvMsg, packedArray
    }
    
    
void T_PresenceAll::T_BindUnbindL()
    {
    EUNIT_PRINT( _L("T_BindUnbindL") );

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();
    
        
    UnbindL();
    
    wrapper = NULL;
    context = NULL;
    messenger = NULL;
    }
// ===========================================================================
// TEST CASE TABLE
// ===========================================================================
//
EUNIT_BEGIN_TEST_TABLE(
        T_PresenceAll,
        "XIMP presence all tests",
        "MODULE" )
        
PRFW_DECORATED_TEST(
        "TMO_demo_Startup",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_TMO_demo_Startup_L,
        Teardown )
        
PRFW_DECORATED_TEST(
        "BindUnbind",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_BindUnbindL,
        Teardown )
        
PRFW_DECORATED_TEST(
        "GrantWithdrawGroup",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_GrantWithdrawGroup_L,
        Teardown )

EUNIT_END_TEST_TABLE


// ===========================================================================
// TEST SUITE CONSTRUCTION
// ===========================================================================
//
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    return T_PresenceAll::NewL();
    }


// end of file
