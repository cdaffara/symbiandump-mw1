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
#include <e32std.h>

#include <ximpclient.h>
#include <ximpcontext.h>
#include <ximpobjectfactory.h>
#include <ximpcontextstateevent.h>
#include <ximprequestcompleteevent.h>
#include <ximpstatus.h>
#include <presencefeatures.h>
#include <presentitygroups.h>
#include <presentitygroupmemberinfo.h>
#include <presentitygroupcontentevent.h>


#include "ximpeventcodec.h"

#include "t_presencegroupmembermngt.h"
#include "prfwteststatuseventlistener.h"
#include "prfwtestmessaging.h"
#include "prfwtestmessenger.h"
#include "prfwtestlistener.h"
#include "prfwtestcontextwrappermgr.h"
#include "prfwtestcontextwrapper.h"
#include "ximpcontextstateeventimp.h"
#include "ximpcontextstateevent.h"

#include "presenceinfoimp.h"
#include "presenceinfofilterimp.h"
#include "prfwtestpresencedatautils.h"
#include "ximpidentityimp.h"
#include "presentitygroupcontenteventimp.h"
#include "ximprequestcompleteeventimp.h"
#include "presentitygroupinfoimp.h"
#include "presentitygrouplisteventimp.h"
#include "presentitygroupmemberinfoimp.h"
#include "ximprequestcompleteeventimp.h"
#include "ximprestrictedobjectcollection.h"
#include "ximpobjectfactoryimp.h"

#include "prfwtestrobustnesstools.h"
#include "ximpobjecthelpers.h"
#include "prfwtestfilesrvmsg.h"
#include "prfwtestfiletool.h"
#include "prfwtestwaithelper.h"
#include "prfwtesteventfactory.h"


_LIT( KGroupId,   "wv:group/member@imps.com" );
_LIT( KGroupId2,  "wv:group2/chutney@naan.com" );
_LIT( KGroupDisplayName, "Salamapartio Z" );

_LIT( KMemberId, "wv:member@imps.com" );
_LIT( KMemberDisplayName,  "Stroustrup" );
_LIT( KMemberDisplayName2, "Bjarne" );

// ============================ MEMBER FUNCTIONS ===============================

// CONSTRUCTION
T_PresenceGroupMemberManagement* T_PresenceGroupMemberManagement::NewL()
    {
    T_PresenceGroupMemberManagement* self = new( ELeave ) T_PresenceGroupMemberManagement;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

T_PresenceGroupMemberManagement::~T_PresenceGroupMemberManagement()
    {
    delete iLastEvent;

    if ( iASWait && iASWait->IsStarted() )
        {
        iASWait->AsyncStop();
        }
    delete iASWait;

    delete iListener2;

    delete iCurrentGrId;
    }

void T_PresenceGroupMemberManagement::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    iASWait = new ( ELeave ) CActiveSchedulerWait();
    }


T_PresenceGroupMemberManagement::T_PresenceGroupMemberManagement()
    {
    }

// ===========================================================================
// TEST CASE SETUP & TEARDOWN METHODS
// ===========================================================================
//

void T_PresenceGroupMemberManagement::Setup_L()
    {
    PrfwTestRobustness::DoPreCleaning();

    iWrapperMgr = CXIMPTestContextWrapperMgr::NewL();
    iWrapperMgr->CreateWrapperL();

    // must reset these to EFalse
    iSkipSubscribe = EFalse;
    iSkipUnsubscribe = EFalse;
    }

void T_PresenceGroupMemberManagement::SetupMultiple_L()
    {
    PrfwTestRobustness::DoPreCleaning();

    iWrapperMgr = CXIMPTestContextWrapperMgr::NewL();
    iWrapperMgr->CreateWrapperL();
    iWrapperMgr->CreateWrapperL();
    iWrapperMgr->CreateWrapperL();

    // must reset these to EFalse
    iSkipSubscribe = EFalse;
    iSkipUnsubscribe = EFalse;
    }

// bind helper
void T_PresenceGroupMemberManagement::BindAllL()
    {
    for( TInt a = 0; a < iWrapperMgr->WrapperCount(); ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        wrapper->BindL( 0 );
        }
    }

// unbind helper
void T_PresenceGroupMemberManagement::UnbindAllL()
    {
    for( TInt a = 0; a < iWrapperMgr->WrapperCount(); ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        wrapper->UnbindL();
        }
    }

void T_PresenceGroupMemberManagement::TeardownTestSupport()
    {
    // anything NOT initialized by test case Setup method,
    // but initialized in the test case itself, goes here
    delete iCurrentGrId;
    iCurrentGrId = NULL;

    delete iListener2;
    iListener2 = NULL;

    delete iLastEvent;
    iLastEvent = NULL;
    }

void T_PresenceGroupMemberManagement::Teardown()
    {
    TeardownTestSupport();
    // just in case to avoid memleaks. should not have anything to
    // delete anyway

    delete iWrapperMgr;
    iWrapperMgr = NULL;

    REComSession::FinalClose();

    // must reset these to EFalse
    iSkipSubscribe = EFalse;
    iSkipUnsubscribe = EFalse;

    // it's best to nuke the servers during exit too.
    PrfwTestRobustness::DoPreCleaning();
    }

// ===========================================================================
// TEST CASES for presentity presence subscription
// ===========================================================================

// General test case for single member
void T_PresenceGroupMemberManagement::T_GenericSubAndUnsubL(
        TXIMPCallBack& aCbFuncL )
    {
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    wrapper->BindL( 0 );

    // ------------------------------------------------------
    // some startup stuff
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

    iListener2 = CXIMPTestListener::NewL( context );
    // ------------------------------------------------------

    // Tell the protocol it should act normally
    wrapper->GetMessenger()->SetNoError();

    iCurrentGrId = context->ObjectFactory().NewIdentityLC();
    CleanupStack::Pop(); // iCurrentGrId
    iCurrentGrId->SetIdentityL( KGroupId );

    // ------------------------------------------------------
    // create presentity group
    //
    // expect events:
    // - request complete.
    // NOTE: no presentitygrouplistevent as we've not subscribed the group lists

    // normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );

    // identity + displayname
    CPresentityGroupInfoImp* infoImp = CPresentityGroupInfoImp::NewLC( *iCurrentGrId, KGroupDisplayName );
    evReqComplete->AppendParamL( infoImp );
    CleanupStack::Pop( infoImp );

    iListener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    TXIMPRequestId reqId = presGroup.CreatePresentityGroupL( *iCurrentGrId, KGroupDisplayName );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == iListener2->WaitAndAssertL(), "CreatePresentityGroupL failed" );
    messenger->AssertCreatePresentityGroupCalled( ETrue );

    if ( ! iSkipSubscribe )
        {
        // ------------------------------------------------------
        // subscribe presentity group CONTENT
        //
        // expect events:
        // - request complete
        evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
        iListener2->ExpectL( evReqComplete );
        CleanupStack::Pop( evReqComplete );

        // do the call
        reqId = presGroup.SubscribePresentityGroupContentL( *iCurrentGrId );

        // and wait
        EUNIT_ASSERT_DESC( KErrNone == iListener2->WaitAndAssertL(), "SubscribePresentityGroupContentL failed" );

        // verify that subscribe was called
        messenger->AssertSubscribePresentityGroupContentCalled( ETrue );
        }

    // ------------------------------------------------------
    // do the actual test. this is a function pointer because the
    // "red tape" of each test is similar (create group, subscribe content, do
    // actual test, unsubscribe content, delete group) - this is the variable
    // part, the "do actual test".
    (this->*aCbFuncL)();

    if ( ! iSkipUnsubscribe )
        {
        // ------------------------------------------------------
        // unsubscribe presentity group content
        //
        // expect events:
        // - request complete
        // - presentity group content event

        evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
        iListener2->ExpectL( evReqComplete );
        CleanupStack::Pop( evReqComplete );

        CPresentityGroupContentEventImp* groupCntEvent =
            XIMPTestEventFactory::CreateGroupContentEventLCX(
                    KGroupId,
                    KNullDesC,
                    KNullDesC,
                    XIMPTestEventFactory::ETestPGLCntRemoved,
                    KNullDesC,
                    KNullDesC,
                    MXIMPDataSubscriptionState::ESubscriptionInactive,
                    MXIMPDataSubscriptionState::EDataUnavailable
                    );

        iListener2->ExpectL( groupCntEvent );
        CleanupStack::Pop( groupCntEvent ); // 5 more items in cleanupstack

        // do the call
        reqId = presGroup.UnsubscribePresentityGroupContentL( *iCurrentGrId );

        // and wait
        EUNIT_ASSERT_DESC( KErrNone == iListener2->WaitAndAssertL(), "UnsubscribePresentityGroupContentL failed" );
        CleanupStack::PopAndDestroy( 5 ); // lists

        // verify that unsubscribe was called
        messenger->AssertUnsubscribePresentityGroupContentCalled( ETrue );
        }

    // ------------------------------------------------------
    // delete presentity group
    //
    // expect events:
    // - request complete
    // NO presentity group content event as we've just unsubscribed it.

    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    iListener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // do the call
    reqId = presGroup.DeletePresentityGroupL( *iCurrentGrId );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == iListener2->WaitAndAssertL(), "DeletePresentityGroupL failed" );

    // ------------------------------------------------------

    // make sure the new listener is not listening for events!
    iListener2->SetEventSourceL( NULL );

    wrapper->UnbindL();

    TeardownTestSupport();
    }

// ===========================================================================
// Variant methods, this is where the actual testing happens.
// Called by the generic subscribe and unsubscribe -method
// ===========================================================================

// ------------------------------------------------------
// empty test variant method
// test only the creation, subscribe and unsubcscribe
// ------------------------------------------------------
//
void T_PresenceGroupMemberManagement::T_EmptyFunc()
    {
    // nothing
    }

// ------------------------------------------------------
// test adding members
// ------------------------------------------------------
//
void T_PresenceGroupMemberManagement::T_TestAddMembersL()
    {
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // ------------------------------------------------------
    // add presentity group member
    //
    // expect events:
    // - request complete,
    // - presentitygroupcontentevent

    MXIMPIdentity* member = context->ObjectFactory().NewIdentityLC();
    member->SetIdentityL( KMemberId );

    // normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );

    // group id
    CXIMPIdentityImp* identity = CXIMPIdentityImp::NewLC( iCurrentGrId->Identity() );
    evReqComplete->AppendParamL( identity );
    CleanupStack::Pop(); // identity

    // id for member info
    CXIMPIdentityImp* miId = CXIMPIdentityImp::NewLC( KMemberId );
    // member info
    CPresentityGroupMemberInfoImp* memInfoImp =
        CPresentityGroupMemberInfoImp::NewLC( *miId, KMemberDisplayName );
    evReqComplete->AppendParamL( memInfoImp );
    CleanupStack::Pop(); // memInfoImp owned by AppendParamL
    CleanupStack::PopAndDestroy( miId ); // copy was made

    iListener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // group content event
    CPresentityGroupContentEventImp* groupCntEvent =
        XIMPTestEventFactory::CreateGroupContentEventLCX(
                iCurrentGrId->Identity(),
                KMemberId,
                KMemberDisplayName,
                XIMPTestEventFactory::ETestPGLCntAdded,
                KMemberId,
                KMemberDisplayName,
                MXIMPDataSubscriptionState::ESubscriptionActive,
                MXIMPDataSubscriptionState::EDataAvailable
                );

    iListener2->ExpectL( groupCntEvent );
    CleanupStack::Pop( groupCntEvent ); // 5 more items in cleanupstack

    // do the call
    TXIMPRequestId reqId = presGroup.AddPresentityGroupMemberL(
            *iCurrentGrId, *member, KMemberDisplayName );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == iListener2->WaitAndAssertL(), "AddPresentityGroupMemberL failed" );
    CleanupStack::PopAndDestroy( 5 ); // lists from groupCntEvent

    messenger->AssertAddPresentityGroupMemberCalled( ETrue );

    // ------------------------------------------------------
    CleanupStack::PopAndDestroy(); // member
    }

// ------------------------------------------------------
// test adding members when not subscribed
// ------------------------------------------------------
//
void T_PresenceGroupMemberManagement::T_TestAddMembers_NonSub_L()
    {
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // ------------------------------------------------------
    // add presentity group member
    //
    // expect events:
    // - request complete,

    MXIMPIdentity* member = context->ObjectFactory().NewIdentityLC();
    member->SetIdentityL( KMemberId );

    // normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );

    // group id
    CXIMPIdentityImp* identity = CXIMPIdentityImp::NewLC( iCurrentGrId->Identity() );
    evReqComplete->AppendParamL( identity );
    CleanupStack::Pop(); // identity

    // id for member info
    CXIMPIdentityImp* miId = CXIMPIdentityImp::NewLC( KMemberId );
    // member info
    CPresentityGroupMemberInfoImp* memInfoImp =
        CPresentityGroupMemberInfoImp::NewLC( *miId, KMemberDisplayName );
    evReqComplete->AppendParamL( memInfoImp );
    CleanupStack::Pop(); // memInfoImp owned by AppendParamL
    CleanupStack::PopAndDestroy( miId ); // copy was made

    iListener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // do the call
    TXIMPRequestId reqId = presGroup.AddPresentityGroupMemberL(
            *iCurrentGrId, *member, KMemberDisplayName );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == iListener2->WaitAndAssertL(), "AddPresentityGroupMemberL failed" );

    messenger->AssertAddPresentityGroupMemberCalled( ETrue );

    // ------------------------------------------------------
    CleanupStack::PopAndDestroy(); // member
    }

// ------------------------------------------------------
// test removing members
// ------------------------------------------------------
//
void T_PresenceGroupMemberManagement::T_TestRemoveMembersL()
    {
    // add a member
    T_TestAddMembersL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // ------------------------------------------------------
    // remove presentity group member
    //
    // expect events:
    // - request complete,
    // - presentitygroupcontentevent

    MXIMPIdentity* member = context->ObjectFactory().NewIdentityLC();
    member->SetIdentityL( KMemberId );

    // normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    iListener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // group content event
    CPresentityGroupContentEventImp* groupCntEvent =
        XIMPTestEventFactory::CreateGroupContentEventLCX(
                iCurrentGrId->Identity(),
                KMemberId,
                KNullDesC,
                XIMPTestEventFactory::ETestPGLCntRemoved,
                KNullDesC,
                KNullDesC,
                MXIMPDataSubscriptionState::ESubscriptionActive,
                MXIMPDataSubscriptionState::EDataAvailable
                );

    iListener2->ExpectL( groupCntEvent );
    CleanupStack::Pop( groupCntEvent ); // 5 more items in cleanupstack

    // do the call
    TXIMPRequestId reqId = presGroup.RemovePresentityGroupMemberL(
            *iCurrentGrId, *member );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == iListener2->WaitAndAssertL(), "RemovePresentityGroupMemberL failed" );
    CleanupStack::PopAndDestroy( 5 ); // lists from groupCntEvent

    messenger->AssertRemovePresentityGroupMemberCalled( ETrue );

    // ------------------------------------------------------
    CleanupStack::PopAndDestroy(); //member
    }

// ------------------------------------------------------
// test removing members when non-subscribed
// ------------------------------------------------------
//
void T_PresenceGroupMemberManagement::T_TestRemoveMembers_NonSub_L()
    {
    // add a member
    T_TestAddMembers_NonSub_L();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // ------------------------------------------------------
    // remove presentity group member
    //
    // expect events:
    // - request complete,
    // - presentitygroupcontentevent

    MXIMPIdentity* member = context->ObjectFactory().NewIdentityLC();
    member->SetIdentityL( KMemberId );

    // normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    iListener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // do the call
    TXIMPRequestId reqId = presGroup.RemovePresentityGroupMemberL(
            *iCurrentGrId, *member );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == iListener2->WaitAndAssertL(), "RemovePresentityGroupMemberL failed" );

    messenger->AssertRemovePresentityGroupMemberCalled( ETrue );

    // ------------------------------------------------------
    CleanupStack::PopAndDestroy(); //member
    }

// test removing nonexisting members
void T_PresenceGroupMemberManagement::T_TestRemoveMembers_NonExisting_L()
    {
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // ------------------------------------------------------
    // remove presentity group member
    //
    // expect events:
    // - request complete,

    MXIMPIdentity* member = context->ObjectFactory().NewIdentityLC();
    member->SetIdentityL( KMemberId );

    // normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    iListener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // do the call
    TXIMPRequestId reqId = presGroup.RemovePresentityGroupMemberL(
            *iCurrentGrId, *member );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == iListener2->WaitAndAssertL(), "RemovePresentityGroupMemberL failed" );

    messenger->AssertRemovePresentityGroupMemberCalled( ETrue );

    // ------------------------------------------------------
    CleanupStack::PopAndDestroy(); //member
    }

// ------------------------------------------------------
// test updating members
// ------------------------------------------------------
//
void T_PresenceGroupMemberManagement::T_TestUpdateMembers_L()
    {
    // add a member
    T_TestAddMembersL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // ------------------------------------------------------
    // update presentity group member
    //
    // expect events:
    // - request complete,
    // - presentitygroupcontentevent

    MXIMPIdentity* member = context->ObjectFactory().NewIdentityLC();
    member->SetIdentityL( KMemberId );

    // group content request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    CXIMPIdentityImp* identity = CXIMPIdentityImp::NewLC( iCurrentGrId->Identity() );
    evReqComplete->AppendParamL( identity );
    CleanupStack::Pop( identity );

    CPresentityGroupMemberInfoImp* mbrInfo = CPresentityGroupMemberInfoImp::NewLC( *member, KMemberDisplayName2 );
    evReqComplete->AppendParamL( mbrInfo );
    CleanupStack::Pop( mbrInfo );

    iListener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // group content event
    CPresentityGroupContentEventImp* groupCntEvent =
        XIMPTestEventFactory::CreateGroupContentEventLCX(
                iCurrentGrId->Identity(),
                KMemberId,
                KMemberDisplayName2,
                XIMPTestEventFactory::ETestPGLCntUpdated,
                KMemberId,
                KMemberDisplayName2,
                MXIMPDataSubscriptionState::ESubscriptionActive,
                MXIMPDataSubscriptionState::EDataAvailable
                );

    iListener2->ExpectL( groupCntEvent );
    CleanupStack::Pop( groupCntEvent ); // 5 more items in cleanupstack

    // do the call
    TXIMPRequestId reqId = presGroup.UpdatePresentityGroupMemberDisplayNameL(
            *iCurrentGrId, *member, KMemberDisplayName2 );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == iListener2->WaitAndAssertL(), "UpdatePresentityGroupMemberDisplayNameL failed" );
    CleanupStack::PopAndDestroy( 5 ); // lists from groupCntEvent

    messenger->AssertUpdatePresentityGroupMemberDisplayNameCalled( ETrue );

    // ------------------------------------------------------
    CleanupStack::PopAndDestroy();
    }

// ------------------------------------------------------
// test updating members (nonsubscribed)
// ------------------------------------------------------
//
void T_PresenceGroupMemberManagement::T_TestUpdateMembers_NonSub_L()
    {
    // add a member
    T_TestAddMembers_NonSub_L();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // ------------------------------------------------------
    // update presentity group member
    //
    // expect events:
    // - request complete,

    MXIMPIdentity* member = context->ObjectFactory().NewIdentityLC();
    member->SetIdentityL( KMemberId );

    // group content request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    CXIMPIdentityImp* identity = CXIMPIdentityImp::NewLC( iCurrentGrId->Identity() );
    evReqComplete->AppendParamL( identity );
    CleanupStack::Pop( identity );

    CPresentityGroupMemberInfoImp* mbrInfo = CPresentityGroupMemberInfoImp::NewLC( *member, KMemberDisplayName2 );
    evReqComplete->AppendParamL( mbrInfo );
    CleanupStack::Pop( mbrInfo );

    iListener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // do the call
    TXIMPRequestId reqId = presGroup.UpdatePresentityGroupMemberDisplayNameL(
            *iCurrentGrId, *member, KMemberDisplayName2 );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == iListener2->WaitAndAssertL(), "UpdatePresentityGroupMemberDisplayNameL failed" );

    messenger->AssertUpdatePresentityGroupMemberDisplayNameCalled( ETrue );

    // ------------------------------------------------------
    CleanupStack::PopAndDestroy();
    }

// ------------------------------------------------------
// test updating members with non-existing id
// ------------------------------------------------------
//
void T_PresenceGroupMemberManagement::T_TestUpdateMembers_NonExisting_L()
    {
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();
    
    // Tell the protocol it should act normally
    messenger->SetNoError();

    // ------------------------------------------------------
    // update presentity group member
    //
    // expect events:
    // - request complete,

    MXIMPIdentity* member = context->ObjectFactory().NewIdentityLC();
    member->SetIdentityL( KMemberId );

    // group content request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    CXIMPIdentityImp* identity = CXIMPIdentityImp::NewLC( iCurrentGrId->Identity() );
    evReqComplete->AppendParamL( identity );
    CleanupStack::Pop( identity );

    CPresentityGroupMemberInfoImp* mbrInfo = CPresentityGroupMemberInfoImp::NewLC( *member, KMemberDisplayName2 );
    evReqComplete->AppendParamL( mbrInfo );
    CleanupStack::Pop( mbrInfo );

    iListener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // do the call
    TXIMPRequestId reqId = presGroup.UpdatePresentityGroupMemberDisplayNameL(
            *iCurrentGrId, *member, KMemberDisplayName2 );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == iListener2->WaitAndAssertL(), "UpdatePresentityGroupMemberDisplayNameL failed" );

    messenger->AssertUpdatePresentityGroupMemberDisplayNameCalled( ETrue );

    // ------------------------------------------------------
    CleanupStack::PopAndDestroy();
    }

// ===========================================================================
// Test case methods
// ===========================================================================

// Subscribe and unsubscribe test
void T_PresenceGroupMemberManagement::T_SubUnsubGroupContent_Single_L()
    {
    EUNIT_PRINT( _L("Single presentity group content sub and unsub") );

    TXIMPCallBack callback = &T_PresenceGroupMemberManagement::T_EmptyFunc;
    T_GenericSubAndUnsubL( callback );
    }

// subscribe and unsubscribe, altered id
void T_PresenceGroupMemberManagement::T_SubUnsubGroupContent_Altered_Single_L()
    {
    EUNIT_PRINT( _L("Single presentity group content sub and unsub with altered id") );

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    wrapper->BindL( 0 );

    // ------------------------------------------------------
    // some startup stuff
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

    iListener2 = CXIMPTestListener::NewL( context );
    // ------------------------------------------------------

    // Tell the protocol it should act normally
    wrapper->GetMessenger()->SetNoError();

    iCurrentGrId = context->ObjectFactory().NewIdentityLC();
    CleanupStack::Pop(); // iCurrentGrId
    iCurrentGrId->SetIdentityL( KGroupId );

    // ------------------------------------------------------
    // create presentity group
    //
    // expect events:
    // - request complete.
    // NOTE: no presentitygrouplistevent as we've not subscribed the group lists

    // normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );

    // identity + displayname
    CPresentityGroupInfoImp* infoImp = CPresentityGroupInfoImp::NewLC( *iCurrentGrId, KGroupDisplayName );
    evReqComplete->AppendParamL( infoImp );
    CleanupStack::Pop( infoImp );

    iListener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    TXIMPRequestId reqId = presGroup.CreatePresentityGroupL( *iCurrentGrId, KGroupDisplayName );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == iListener2->WaitAndAssertL(), "CreatePresentityGroupL failed" );
    messenger->AssertCreatePresentityGroupCalled( ETrue );

    // ------------------------------------------------------
    // subscribe presentity group CONTENT
    //
    // expect events:
    // - request complete
    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    iListener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // tell the test protocol to send back an altered id
    MXIMPIdentity* identityParam = context->ObjectFactory().NewIdentityLC();
    identityParam->SetIdentityL( KGroupId2 );

    CXIMPIdentityImp* identityParamImp =
             ( CXIMPIdentityImp* ) identityParam->GetInterface(
                                        CXIMPIdentityImp::KClassId,
                                        MXIMPBase::EPanicIfUnknown );
    HBufC8* packedId = TXIMPObjectPacker< CXIMPIdentityImp >::PackL( *identityParamImp );
    CleanupStack::PopAndDestroy(); // identityParam
    CleanupStack::PushL( packedId );
    CXIMPTestFileSrvMsg* srvMsg = CXIMPTestFileSrvMsg::NewL( CXIMPTestFileSrvMsg::ETestSrvMsgReqParamIdentity,
                                                             *packedId );
    CleanupStack::PopAndDestroy( packedId );
    CleanupStack::PushL( srvMsg );
    wrapper->FileTool().SrvMsgStoreL( srvMsg );
    CleanupStack::PopAndDestroy( srvMsg );

    // Wait for server to handle srvmsg.
    User::After( 2 );
    // now the adaptation should know how to answer

    // do the call
    reqId = presGroup.SubscribePresentityGroupContentL( *iCurrentGrId );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == iListener2->WaitAndAssertL(), "SubscribePresentityGroupContentL failed" );

    // verify that subscribe was called
    messenger->AssertSubscribePresentityGroupContentCalled( ETrue );

    // ------------------------------------------------------

    // make sure the new listener is not listening for events!
    iListener2->SetEventSourceL( NULL );

    wrapper->UnbindL();

    TeardownTestSupport();
    }

// subscribe and unsubscribe, altered id is the same
void T_PresenceGroupMemberManagement::T_SubUnsubGroupContent_AlteredSame_Single_L()
    {
    EUNIT_PRINT( _L("Single presentity group content sub and unsub with altered but same id") );

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    wrapper->BindL( 0 );

    // ------------------------------------------------------
    // some startup stuff
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

    iListener2 = CXIMPTestListener::NewL( context );
    // ------------------------------------------------------

    // Tell the protocol it should act normally
    wrapper->GetMessenger()->SetNoError();

    iCurrentGrId = context->ObjectFactory().NewIdentityLC();
    CleanupStack::Pop(); // iCurrentGrId
    iCurrentGrId->SetIdentityL( KGroupId );

    // ------------------------------------------------------
    // create presentity group
    //
    // expect events:
    // - request complete.
    // NOTE: no presentitygrouplistevent as we've not subscribed the group lists

    // normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );

    // identity + displayname
    CPresentityGroupInfoImp* infoImp = CPresentityGroupInfoImp::NewLC( *iCurrentGrId, KGroupDisplayName );
    evReqComplete->AppendParamL( infoImp );
    CleanupStack::Pop( infoImp );

    iListener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    TXIMPRequestId reqId = presGroup.CreatePresentityGroupL( *iCurrentGrId, KGroupDisplayName );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == iListener2->WaitAndAssertL(), "CreatePresentityGroupL failed" );
    messenger->AssertCreatePresentityGroupCalled( ETrue );

    // ------------------------------------------------------
    // subscribe presentity group CONTENT
    //
    // expect events:
    // - request complete
    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    iListener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // tell the test protocol to send back an altered id
    MXIMPIdentity* identityParam = context->ObjectFactory().NewIdentityLC();
    identityParam->SetIdentityL( KGroupId );

    CXIMPIdentityImp* identityParamImp =
             ( CXIMPIdentityImp* ) identityParam->GetInterface(
                                        CXIMPIdentityImp::KClassId,
                                        MXIMPBase::EPanicIfUnknown );
    HBufC8* packedId = TXIMPObjectPacker< CXIMPIdentityImp >::PackL( *identityParamImp );
    CleanupStack::PopAndDestroy(); // identityParam
    CleanupStack::PushL( packedId );
    CXIMPTestFileSrvMsg* srvMsg = CXIMPTestFileSrvMsg::NewL( CXIMPTestFileSrvMsg::ETestSrvMsgReqParamIdentity,
                                                             *packedId );
    CleanupStack::PopAndDestroy( packedId );
    CleanupStack::PushL( srvMsg );
    wrapper->FileTool().SrvMsgStoreL( srvMsg );
    CleanupStack::PopAndDestroy( srvMsg );

    // Wait for server to handle srvmsg.
    User::After( 2 );
    // now the adaptation should know how to answer

    // do the call
    reqId = presGroup.SubscribePresentityGroupContentL( *iCurrentGrId );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == iListener2->WaitAndAssertL(), "SubscribePresentityGroupContentL failed" );

    // verify that subscribe was called
    messenger->AssertSubscribePresentityGroupContentCalled( ETrue );

    // ------------------------------------------------------

    // make sure the new listener is not listening for events!
    iListener2->SetEventSourceL( NULL );

    wrapper->UnbindL();

    TeardownTestSupport();
    }


// Add members
void T_PresenceGroupMemberManagement::T_AddGroupMembers_Single_L()
    {
    EUNIT_PRINT( _L("Single add members to group") );

    TXIMPCallBack callback = &T_PresenceGroupMemberManagement::T_TestAddMembersL;
    T_GenericSubAndUnsubL( callback );
    }

// Remove members
void T_PresenceGroupMemberManagement::T_RemoveGroupMembers_Single_L()
    {
    EUNIT_PRINT( _L("Single remove member of group") );

    TXIMPCallBack callback = &T_PresenceGroupMemberManagement::T_TestRemoveMembersL;
    T_GenericSubAndUnsubL( callback );
    }

// Update members
void T_PresenceGroupMemberManagement::T_UpdateGroupMembers_Single_L()
    {
    EUNIT_PRINT( _L("Single update member of group") );

    TXIMPCallBack callback = &T_PresenceGroupMemberManagement::T_TestUpdateMembers_L;
    T_GenericSubAndUnsubL( callback );
    }

// Update members without subscription
void T_PresenceGroupMemberManagement::T_UpdateGroupMembers_NonSub_Single_L()
    {
    EUNIT_PRINT( _L("Single update member of group, unsubscribed") );

    TXIMPCallBack callback = &T_PresenceGroupMemberManagement::T_TestUpdateMembers_NonSub_L;
    iSkipSubscribe = ETrue;
    iSkipUnsubscribe = ETrue;
    T_GenericSubAndUnsubL( callback );
    }

// Update members with non-existing member
void T_PresenceGroupMemberManagement::T_UpdateGroupMembers_NonExist_Single_L()
    {
    EUNIT_PRINT( _L("Single update nonexisting member of group") );

    TXIMPCallBack callback = &T_PresenceGroupMemberManagement::T_TestUpdateMembers_NonExisting_L;
    T_GenericSubAndUnsubL( callback );
    }

// Remove members without subscription
void T_PresenceGroupMemberManagement::T_RemoveGroupMembers_NonSub_Single_L()
    {
    EUNIT_PRINT( _L("Single remove member of group, unsubscribed") );

    TXIMPCallBack callback = &T_PresenceGroupMemberManagement::T_TestRemoveMembers_NonSub_L;
    iSkipSubscribe = ETrue;
    iSkipUnsubscribe = ETrue;
    T_GenericSubAndUnsubL( callback );
    }

// Remove members with non-existing member
void T_PresenceGroupMemberManagement::T_RemoveGroupMembers_NonExist_Single_L()
    {
    EUNIT_PRINT( _L("Single remove nonexisting member of group") );

    TXIMPCallBack callback = &T_PresenceGroupMemberManagement::T_TestRemoveMembers_NonExisting_L;
    T_GenericSubAndUnsubL( callback );
    }

// ------------------------------------------------------
// expire non-subscribed data
// ------------------------------------------------------
//
void T_PresenceGroupMemberManagement::T_ExpireNonSubscribedData_L()
    {
    EUNIT_PRINT( _L("Expire non subscribed data") );

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    wrapper->BindL( 0 );

    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

    iCurrentGrId = context->ObjectFactory().NewIdentityLC();
    CleanupStack::Pop(); // iCurrentGrId
    iCurrentGrId->SetIdentityL( KGroupId );

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // 1. Create notification
    RXIMPObjOwningPtrArray<CPresentityGroupMemberInfoImp> groupMemberList;
    CXIMPIdentityImp* identityImp =
             ( CXIMPIdentityImp* ) iCurrentGrId->GetInterface(
                                        CXIMPIdentityImp::KClassId,
                                        MXIMPBase::EPanicIfUnknown );
    HBufC8* packedId = TXIMPObjectPacker< CXIMPIdentityImp >::PackL( *identityImp );
    CleanupStack::PushL( packedId );
    HBufC8* packedArray = TXIMPObjectPacker<CPresentityGroupMemberInfoImp>::PackArrayL( groupMemberList );
    CleanupStack::PushL( packedArray );
    RPointerArray< HBufC8 > bufferArray;
    CleanupClosePushL( bufferArray );
    bufferArray.AppendL( packedId );
    bufferArray.AppendL( packedArray );
    HBufC8* packedBufferArray = TXIMPHBuf8Packer::PackArrayL( bufferArray );
    CleanupStack::PopAndDestroy( 2, packedArray ); // bufferArray, packedArray
    CleanupStack::PushL( packedBufferArray );

    // 2. Use filetool to send it for plugin.
    CXIMPTestFileSrvMsg* srvMsg = CXIMPTestFileSrvMsg::NewL( CXIMPTestFileSrvMsg::ETestSrvMsgPresentityGroupMemberList,
                                                             *packedBufferArray );
    CleanupStack::PopAndDestroy( packedBufferArray );
    CleanupStack::PushL( srvMsg );
    wrapper->FileTool().CleanAllL();
    wrapper->FileTool().SrvMsgStoreL( srvMsg );
    CleanupStack::PopAndDestroy( srvMsg );

    // Wait for server to handle srvmsg.
    CXIMPTestWaitHelper* wait = CXIMPTestWaitHelper::NewL();
    CleanupStack::PushL( wait );
    wait->WaitForL( 2 );

    // 3. Create and send check notification to server
    srvMsg = CXIMPTestFileSrvMsg::NewL( CXIMPTestFileSrvMsg::ETestCheckIfGroupMemberExists,
                                                             *packedId );
    CleanupStack::PushL( srvMsg );
    wrapper->FileTool().SrvMsgStoreL( srvMsg );

    // Wait for server to handle srvmsg.
    wait->WaitForL( 2 );

    TInt value = messenger->GetValueFor( EXIMPPlgTestExistsGroupContentInCache );
    EUNIT_ASSERT_DESC( value == 1, "Data not cached temporarly" );

   // 4. Wait for cache to clear.
    wait->WaitForL( 15 );

    // 5. Send check notification to server
    wrapper->FileTool().SrvMsgStoreL( srvMsg );
    CleanupStack::PopAndDestroy( srvMsg );

    // Wait for server to handle srvmsg.
    wait->WaitForL( 2 );
    CleanupStack::PopAndDestroy( wait );
    CleanupStack::PopAndDestroy( packedId );
    value = messenger->GetValueFor( EXIMPPlgTestExistsGroupContentInCache );
    EUNIT_ASSERT_DESC( value == KErrNotFound, "Data not cleared like it should" );
    wrapper->UnbindL();
    }

// ------------------------------------------------------
// expire non-subscribed data, update multiple
// ------------------------------------------------------
//
void T_PresenceGroupMemberManagement::T_ExpireNonSubscribedDataUpdateMultiple_L()
    {
    EUNIT_PRINT( _L("Expire non subscribed data, update multiple") );

    // Wait for server to handle srvmsg.
    CXIMPTestWaitHelper* wait = CXIMPTestWaitHelper::NewL();
    CleanupStack::PushL( wait );

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    wrapper->BindL( 0 );

    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

    iCurrentGrId = context->ObjectFactory().NewIdentityLC();
    CleanupStack::Pop(); // iCurrentGrId
    iCurrentGrId->SetIdentityL( KGroupId );

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // 1. Create notification
    RXIMPObjOwningPtrArray<CPresentityGroupMemberInfoImp> groupMemberList;
    CXIMPIdentityImp* identityImp =
             ( CXIMPIdentityImp* ) iCurrentGrId->GetInterface(
                                        CXIMPIdentityImp::KClassId,
                                        MXIMPBase::EPanicIfUnknown );
    HBufC8* packedId = TXIMPObjectPacker< CXIMPIdentityImp >::PackL( *identityImp );
    CleanupStack::PushL( packedId );
    HBufC8* packedArray = TXIMPObjectPacker<CPresentityGroupMemberInfoImp>::PackArrayL( groupMemberList );
    CleanupStack::PushL( packedArray );
    RPointerArray< HBufC8 > bufferArray;
    CleanupClosePushL( bufferArray );
    bufferArray.AppendL( packedId );
    bufferArray.AppendL( packedArray );
    HBufC8* packedBufferArray = TXIMPHBuf8Packer::PackArrayL( bufferArray );
    CleanupStack::PopAndDestroy( 2, packedArray ); // bufferArray, packedArray
    CleanupStack::PushL( packedBufferArray );

    // 2. Use filetool to send it for plugin.
    CXIMPTestFileSrvMsg* srvMsg = CXIMPTestFileSrvMsg::NewL( CXIMPTestFileSrvMsg::ETestSrvMsgPresentityGroupMemberList,
                                                             *packedBufferArray );
    CleanupStack::PopAndDestroy( packedBufferArray );
    CleanupStack::PushL( srvMsg );
    wrapper->FileTool().CleanAllL();
    wrapper->FileTool().SrvMsgStoreL( srvMsg );

    wait->WaitForL( 2 );

    // 3. Create and send check notification to server
    CXIMPTestFileSrvMsg* srvMsg2 = CXIMPTestFileSrvMsg::NewL( CXIMPTestFileSrvMsg::ETestCheckIfGroupMemberExists,
                                                             *packedId );
    CleanupStack::PushL( srvMsg2 );
    wrapper->FileTool().SrvMsgStoreL( srvMsg2 );

    // Wait for server to handle srvmsg.
    wait->WaitForL( 2 );

    TInt value = messenger->GetValueFor( EXIMPPlgTestExistsGroupContentInCache );
    EUNIT_ASSERT_DESC( value == 1, "Data not cached temporarly" );

   // 4. Wait for cache to clear.
    wait->WaitForL( 5 );

    // Refresh temporar cache.
    wrapper->FileTool().SrvMsgStoreL( srvMsg );

   // 4. Wait for cache to clear.
    wait->WaitForL( 10 );

    // 5. Send check notification to server
    wrapper->FileTool().SrvMsgStoreL( srvMsg2 );
    CleanupStack::PopAndDestroy( 3, packedId ); //packedId, srvMsg, srvMsg2

    // Wait for server to handle srvmsg.
    wait->WaitForL( 2 );
    value = messenger->GetValueFor( EXIMPPlgTestExistsGroupContentInCache );
    EUNIT_ASSERT_DESC( value == 1, "Data not cached temporarly, but it should be" );

    wrapper->UnbindL();

    CleanupStack::PopAndDestroy( wait );
    }

// ------------------------------------------------------
// General subscribe and unsubscribe for multiple
// ------------------------------------------------------
//
void T_PresenceGroupMemberManagement::T_SubUnsubGroupContent_Multi_L()
    {
    BindAllL();

    TInt countOfWrappers = iWrapperMgr->WrapperCount();

    CXIMPObjectFactoryImp* factory = CXIMPObjectFactoryImp::NewL();
    CleanupStack::PushL( factory );

    MXIMPIdentity* currentGrId = factory->NewIdentityLC();
    currentGrId->SetIdentityL( KGroupId );

    // create group to all connections
    for ( TInt i = 0; i < countOfWrappers; i++ )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( i );

        // ------------------------------------------------------
        // some startup stuff
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresentityGroups& presGroup = presFeat->PresentityGroups();

        CXIMPTestListener* listener = CXIMPTestListener::NewL( context );
        CleanupStack::PushL( listener );
        // ------------------------------------------------------

        // Tell the protocol it should act normally
        wrapper->GetMessenger()->SetNoError();

        // ------------------------------------------------------
        // create presentity group
        //
        // expect events:
        // - request complete.
        // NOTE: no presentitygrouplistevent as we've not subscribed the group lists

        // normal request complete
        TXIMPRequestId reqIdDummy;  // not checked
        CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );

        // identity + displayname
        CPresentityGroupInfoImp* infoImp = CPresentityGroupInfoImp::NewLC( *currentGrId, KGroupDisplayName );
        evReqComplete->AppendParamL( infoImp );
        CleanupStack::Pop( infoImp );

        listener->ExpectL( evReqComplete );
        CleanupStack::Pop( evReqComplete );

        TXIMPRequestId reqId = presGroup.CreatePresentityGroupL( *currentGrId, KGroupDisplayName );

        // and wait
        EUNIT_ASSERT_DESC( KErrNone == listener->WaitAndAssertL(), "CreatePresentityGroupL failed" );

        // verify that create was called
        messenger->AssertCreatePresentityGroupCalled( ETrue );

        CleanupStack::PopAndDestroy( listener );
        }

    // subscribe presentity group content in all connections
    for ( TInt i = 0; i < countOfWrappers; i++ )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( i );

        // ------------------------------------------------------
        // some startup stuff
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresentityGroups& presGroup = presFeat->PresentityGroups();

        CXIMPTestListener* listener = CXIMPTestListener::NewL( context );
        CleanupStack::PushL( listener );
        // ------------------------------------------------------

        // ------------------------------------------------------
        // subscribe presentity group CONTENT
        //
        // expect events:
        // - request complete
        TXIMPRequestId reqIdDummy;  // not checked
        CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
        listener->ExpectL( evReqComplete );
        CleanupStack::Pop( evReqComplete );

        // do the call
        TXIMPRequestId reqId = presGroup.SubscribePresentityGroupContentL( *currentGrId );

        // and wait
        EUNIT_ASSERT_DESC( KErrNone == listener->WaitAndAssertL(), "SubscribePresentityGroupContentL failed" );

        // verify that subscribe was called
        messenger->AssertSubscribePresentityGroupContentCalled( ETrue );

        CleanupStack::PopAndDestroy( listener );
        }

    // unsubscribe presentity group content in all connections
    for ( TInt i = 0; i < countOfWrappers; i++ )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( i );

        // ------------------------------------------------------
        // some startup stuff
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresentityGroups& presGroup = presFeat->PresentityGroups();

        CXIMPTestListener* listener = CXIMPTestListener::NewL( context );
        CleanupStack::PushL( listener );
        // ------------------------------------------------------

        // ------------------------------------------------------
        // unsubscribe presentity group content
        //
        // expect events:
        // - request complete
        // - presentity group content event

        TXIMPRequestId reqIdDummy;  // not checked
        CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
        listener->ExpectL( evReqComplete );
        CleanupStack::Pop( evReqComplete );

        CPresentityGroupContentEventImp* groupCntEvent =
            XIMPTestEventFactory::CreateGroupContentEventLCX(
                    KGroupId,
                    KNullDesC,
                    KNullDesC,
                    XIMPTestEventFactory::ETestPGLCntRemoved,
                    KNullDesC,
                    KNullDesC,
                    MXIMPDataSubscriptionState::ESubscriptionInactive,
                    MXIMPDataSubscriptionState::EDataUnavailable
                    );

        listener->ExpectL( groupCntEvent );
        CleanupStack::Pop( groupCntEvent ); // 5 more items in cleanupstack

        // do the call
        TXIMPRequestId reqId = presGroup.UnsubscribePresentityGroupContentL( *currentGrId );

        // and wait
        EUNIT_ASSERT_DESC( KErrNone == listener->WaitAndAssertL(), "UnsubscribePresentityGroupContentL failed" );
        CleanupStack::PopAndDestroy( 5 ); // lists

        // verify that unsubscribe was called
        // unsubscribe really called only on the last one
        messenger->AssertUnsubscribePresentityGroupContentCalled( i == countOfWrappers-1 ? ETrue : EFalse );

        CleanupStack::PopAndDestroy( listener );
        }

    // delete presentity group in all connections
    for ( TInt i = 0; i < countOfWrappers; i++ )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( i );

        // ------------------------------------------------------
        // some startup stuff
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresentityGroups& presGroup = presFeat->PresentityGroups();

        CXIMPTestListener* listener = CXIMPTestListener::NewL( context );
        CleanupStack::PushL( listener );
        // ------------------------------------------------------

        // ------------------------------------------------------
        // delete presentity group
        //
        // expect events:
        // - request complete
        // NO presentity group content event as we've just unsubscribed it.

        TXIMPRequestId reqIdDummy;  // not checked
        CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
        listener->ExpectL( evReqComplete );
        CleanupStack::Pop( evReqComplete );

        // do the call
        TXIMPRequestId reqId = presGroup.DeletePresentityGroupL( *currentGrId );

        // and wait
        EUNIT_ASSERT_DESC( KErrNone == listener->WaitAndAssertL(), "DeletePresentityGroupL failed" );

        CleanupStack::PopAndDestroy( listener );
        }

    // ------------------------------------------------------

    CleanupStack::PopAndDestroy( 2 ); // currentGrId, factory


    UnbindAllL();

    TeardownTestSupport();
    }

// ------------------------------------------------------
// General subscribe and unsubscribe for multiple
// Subscribed for all contexts, force refresh
// ------------------------------------------------------
//
void T_PresenceGroupMemberManagement::T_SubUnsubGroupContent_All_Multi_L()
    {
    BindAllL();

    TInt countOfWrappers = iWrapperMgr->WrapperCount();

    CXIMPObjectFactoryImp* factory = CXIMPObjectFactoryImp::NewL();
    CleanupStack::PushL( factory );

    MXIMPIdentity* currentGrId = factory->NewIdentityLC();
    currentGrId->SetIdentityL( KGroupId );

    // create group to all connections
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );

    // ------------------------------------------------------
    // some startup stuff
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

    CXIMPTestListener* listener = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener );
    // ------------------------------------------------------

    // Tell the protocol it should act normally
    wrapper->GetMessenger()->SetNoError();

    // ------------------------------------------------------
    // create presentity group
    //
    // expect events:
    // - request complete.
    // NOTE: no presentitygrouplistevent as we've not subscribed the group lists

    // normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );

    // identity + displayname
    CPresentityGroupInfoImp* infoImp = CPresentityGroupInfoImp::NewLC( *currentGrId, KGroupDisplayName );
    evReqComplete->AppendParamL( infoImp );
    CleanupStack::Pop( infoImp );

    listener->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    TXIMPRequestId reqId = presGroup.CreatePresentityGroupL( *currentGrId, KGroupDisplayName );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == listener->WaitAndAssertL(), "CreatePresentityGroupL failed" );

    // verify that create was called
    messenger->AssertCreatePresentityGroupCalled( ETrue );

    // we'll make a new listener below, disable temporarily
    listener->SetEventSourceL( NULL );

    // subscribe presentity group content in all connections
    for ( TInt i = 0; i < countOfWrappers; i++ )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( i );

        // ------------------------------------------------------
        // some startup stuff
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresentityGroups& presGroup = presFeat->PresentityGroups();

        CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
        CleanupStack::PushL( listener2 );
        // ------------------------------------------------------

        // ------------------------------------------------------
        // subscribe presentity group content for ALL contexts
        //
        // expect events:
        // - request complete
        evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
        listener2->ExpectL( evReqComplete );
        CleanupStack::Pop( evReqComplete );

        // do the call
        reqId = presGroup.SubscribePresentityGroupContentL( *currentGrId );

        // and wait
        EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "SubscribePresentityGroupContentL failed" );

        // verify that subscribe was called for first only
        if ( i == 0 )
            {
            messenger->AssertSubscribePresentityGroupContentCalled( ETrue );
            // reset flag
            messenger->SetValueFor(
                    EXIMPPlgTestSubscribePresentityGroupContentCalled,
                    EFalse );
            }
        else
            {
            messenger->AssertSubscribePresentityGroupContentCalled( EFalse );
            }

        CleanupStack::PopAndDestroy( listener2 );
        }

    for ( TInt i = 0; i < countOfWrappers; i++ )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( i );

        // ------------------------------------------------------
        // some startup stuff
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresentityGroups& presGroup = presFeat->PresentityGroups();

        CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
        CleanupStack::PushL( listener2 );
        // ------------------------------------------------------

        // ------------------------------------------------------
        // subscribe presentity group content, making a REFRESH case
        // with multiple contexts
        //
        // expect events:
        // - request complete
        // - group content event

        evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
        listener2->ExpectL( evReqComplete );
        CleanupStack::Pop( evReqComplete );

        CPresentityGroupContentEventImp* groupCntEvent =
            XIMPTestEventFactory::CreateGroupContentEventLCX(
                    KGroupId,
                    KNullDesC,
                    KNullDesC,
                    XIMPTestEventFactory::ETestPGLCntUpdated,
                    KNullDesC,
                    KNullDesC,
                    MXIMPDataSubscriptionState::ESubscriptionActive,
                    MXIMPDataSubscriptionState::EDataUnavailable
                    );

        listener2->ExpectL( groupCntEvent );
        CleanupStack::Pop( groupCntEvent ); // 5 more items in cleanupstack

        // do the call
        reqId = presGroup.SubscribePresentityGroupContentL( *currentGrId );

        // and wait
        EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "SubscribePresentityGroupContentL failed" );

        CleanupStack::PopAndDestroy( 5 ); // rest of group the content event

        // subscribe is not called in the adaptation, this is a refresh case
        messenger->AssertSubscribePresentityGroupContentCalled( EFalse );

        CleanupStack::PopAndDestroy( listener2 );
        }

    // continue listening
    listener->SetEventSourceL( context );

    // ------------------------------------------------------
    // unsubscribe presentity group content
    //
    // expect events:
    // - request complete
    // - presentity group content event

    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    CPresentityGroupContentEventImp* groupCntEvent =
        XIMPTestEventFactory::CreateGroupContentEventLCX(
                KGroupId,
                KNullDesC,
                KNullDesC,
                XIMPTestEventFactory::ETestPGLCntRemoved,
                KNullDesC,
                KNullDesC,
                MXIMPDataSubscriptionState::ESubscriptionInactive,
                MXIMPDataSubscriptionState::EDataUnavailable
                );

    listener->ExpectL( groupCntEvent );
    CleanupStack::Pop( groupCntEvent ); // 5 more items in cleanupstack

    // do the call
    reqId = presGroup.UnsubscribePresentityGroupContentL( *currentGrId );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == listener->WaitAndAssertL(), "UnsubscribePresentityGroupContentL failed" );
    CleanupStack::PopAndDestroy( 5 ); // lists

    // verify that unsubscribe was NOT called (other contexts are listening too),
    // and it will be unscubscribed on the last call
    messenger->AssertUnsubscribePresentityGroupContentCalled( EFalse );

    // ------------------------------------------------------
    // delete presentity group
    //
    // expect events:
    // - request complete
    // NO presentity group content event as we've just unsubscribed it.

    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // do the call
    reqId = presGroup.DeletePresentityGroupL( *currentGrId );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == listener->WaitAndAssertL(), "DeletePresentityGroupL failed" );

    // ------------------------------------------------------

    CleanupStack::PopAndDestroy( 3 ); // listener, currentGrId, factory

    UnbindAllL();

    TeardownTestSupport();
    }

// ------------------------------------------------------
// General subscribe and unsubscribe for multiple
// Subscribed for one context only
// ------------------------------------------------------
//
void T_PresenceGroupMemberManagement::T_SubUnsubGroupContent_One_Multi_L()
    {
    BindAllL();

    CXIMPObjectFactoryImp* factory = CXIMPObjectFactoryImp::NewL();
    CleanupStack::PushL( factory );

    MXIMPIdentity* currentGrId = factory->NewIdentityLC();
    currentGrId->SetIdentityL( KGroupId );

    // create group to all connections
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );

    // ------------------------------------------------------
    // some startup stuff
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

    CXIMPTestListener* listener = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener );
    // ------------------------------------------------------

    // Tell the protocol it should act normally
    wrapper->GetMessenger()->SetNoError();

    // ------------------------------------------------------
    // create presentity group
    //
    // expect events:
    // - request complete.
    // NOTE: no presentitygrouplistevent as we've not subscribed the group lists

    // normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );

    // identity + displayname
    CPresentityGroupInfoImp* infoImp = CPresentityGroupInfoImp::NewLC( *currentGrId, KGroupDisplayName );
    evReqComplete->AppendParamL( infoImp );
    CleanupStack::Pop( infoImp );

    listener->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    TXIMPRequestId reqId = presGroup.CreatePresentityGroupL( *currentGrId, KGroupDisplayName );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == listener->WaitAndAssertL(), "CreatePresentityGroupL failed" );

    // verify that create was called
    messenger->AssertCreatePresentityGroupCalled( ETrue );

    // ------------------------------------------------------
    // subscribe presentity group CONTENT
    //
    // expect events:
    // - request complete
    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // do the call
    reqId = presGroup.SubscribePresentityGroupContentL( *currentGrId );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == listener->WaitAndAssertL(), "SubscribePresentityGroupContentL failed" );

    // verify that subscribe was called
    messenger->AssertSubscribePresentityGroupContentCalled( ETrue );

    // ------------------------------------------------------
    // subscribe presentity group content, making a REFRESH case
    // with multiple contexts
    //
    // expect events:
    // - request complete
    // - group content event

    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    CPresentityGroupContentEventImp* groupCntEvent =
        XIMPTestEventFactory::CreateGroupContentEventLCX(
                KGroupId,
                KNullDesC,
                KNullDesC,
                XIMPTestEventFactory::ETestPGLCntUpdated,
                KNullDesC,
                KNullDesC,
                MXIMPDataSubscriptionState::ESubscriptionActive,
                MXIMPDataSubscriptionState::EDataUnavailable
                );

    listener->ExpectL( groupCntEvent );
    CleanupStack::Pop( groupCntEvent ); // 5 more items in cleanupstack

    // do the call
    reqId = presGroup.SubscribePresentityGroupContentL( *currentGrId );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == listener->WaitAndAssertL(), "SubscribePresentityGroupContentL failed" );

    CleanupStack::PopAndDestroy( 5 ); // rest of group the content event

    // verify that subscribe was called
    messenger->AssertSubscribePresentityGroupContentCalled( ETrue );

    // ------------------------------------------------------
    // unsubscribe presentity group content
    //
    // expect events:
    // - request complete
    // - presentity group content event

    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    groupCntEvent =
        XIMPTestEventFactory::CreateGroupContentEventLCX(
                KGroupId,
                KNullDesC,
                KNullDesC,
                XIMPTestEventFactory::ETestPGLCntRemoved,
                KNullDesC,
                KNullDesC,
                MXIMPDataSubscriptionState::ESubscriptionInactive,
                MXIMPDataSubscriptionState::EDataUnavailable
                );

    listener->ExpectL( groupCntEvent );
    CleanupStack::Pop( groupCntEvent ); // 5 more items in cleanupstack

    // do the call
    reqId = presGroup.UnsubscribePresentityGroupContentL( *currentGrId );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == listener->WaitAndAssertL(), "UnsubscribePresentityGroupContentL failed" );
    CleanupStack::PopAndDestroy( 5 ); // lists

    // verify that unsubscribe was called
    // unsubscribe really called only on the last one
    messenger->AssertUnsubscribePresentityGroupContentCalled( ETrue );

    // ------------------------------------------------------
    // delete presentity group
    //
    // expect events:
    // - request complete
    // NO presentity group content event as we've just unsubscribed it.

    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // do the call
    reqId = presGroup.DeletePresentityGroupL( *currentGrId );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == listener->WaitAndAssertL(), "DeletePresentityGroupL failed" );

    // ------------------------------------------------------

    CleanupStack::PopAndDestroy( 3 ); // listener, currentGrId, factory

    UnbindAllL();

    TeardownTestSupport();
    }

// Unsubscribe non-subscribed
void T_PresenceGroupMemberManagement::T_UnsubscribeNonSubscribed_L()
    {
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    wrapper->BindL( 0 );

    // ------------------------------------------------------
    // some startup stuff
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

    iListener2 = CXIMPTestListener::NewL( context );
    // ------------------------------------------------------

    // Tell the protocol it should act normally
    wrapper->GetMessenger()->SetNoError();

    iCurrentGrId = context->ObjectFactory().NewIdentityLC();
    CleanupStack::Pop(); // iCurrentGrId
    iCurrentGrId->SetIdentityL( KGroupId );

    // ------------------------------------------------------
    // create presentity group
    //
    // expect events:
    // - request complete.
    // NOTE: no presentitygrouplistevent as we've not subscribed the group lists

    // normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );

    // identity + displayname
    CPresentityGroupInfoImp* infoImp = CPresentityGroupInfoImp::NewLC( *iCurrentGrId, KGroupDisplayName );
    evReqComplete->AppendParamL( infoImp );
    CleanupStack::Pop( infoImp );

    iListener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    TXIMPRequestId reqId = presGroup.CreatePresentityGroupL( *iCurrentGrId, KGroupDisplayName );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == iListener2->WaitAndAssertL(), "CreatePresentityGroupL failed" );
    messenger->AssertCreatePresentityGroupCalled( ETrue );

    // ------------------------------------------------------
    // unsubscribe presentity group content
    //
    // expect events:
    // - request complete
    // - presentity group content event

    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    iListener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    CPresentityGroupContentEventImp* groupCntEvent =
        XIMPTestEventFactory::CreateGroupContentEventLCX(
                KGroupId,
                KNullDesC,
                KNullDesC,
                XIMPTestEventFactory::ETestPGLCntRemoved,
                KNullDesC,
                KNullDesC,
                MXIMPDataSubscriptionState::ESubscriptionInactive,
                MXIMPDataSubscriptionState::EDataUnavailable
                );

    iListener2->ExpectL( groupCntEvent );
    CleanupStack::Pop( groupCntEvent ); // 5 more items in cleanupstack

    // do the call
    reqId = presGroup.UnsubscribePresentityGroupContentL( *iCurrentGrId );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == iListener2->WaitAndAssertL(), "UnsubscribePresentityGroupContentL failed" );

    CleanupStack::PopAndDestroy( 5 ); // lists

    // verify that unsubscribe was not called, not subscribed, so won't
    // unsubscribe
    messenger->AssertUnsubscribePresentityGroupContentCalled( EFalse );

    // make sure the new listener is not listening for events!
    iListener2->SetEventSourceL( NULL );

    wrapper->UnbindL();

    TeardownTestSupport();
    }

// ===========================================================================
// SETUP HELPERS FOR THE LISTENER
// ===========================================================================
//
void T_PresenceGroupMemberManagement::SetupListenerReqCompleteL( CXIMPTestContextWrapper* aWrapper, TXIMPTestStatusEventTemplate aEventType )
    {
    RArray< TInt32 > array;
    CleanupClosePushL( array );
    array.Append( MXIMPContextStateEvent::KInterfaceId );
    array.Append( MPresentityGroupListEvent::KInterfaceId );
    aWrapper->SetupListenerReqCompleteL( aEventType, &array );
    CleanupStack::PopAndDestroy(); // array
    }

void T_PresenceGroupMemberManagement::SetupListenerReqCompleteAllowEventsL(  CXIMPTestContextWrapper* aWrapper, TXIMPTestStatusEventTemplate aEventType )
    {
    RArray< TInt32 > array;
    CleanupClosePushL( array );
    array.AppendL( MXIMPRequestCompleteEvent::KInterfaceId );
    array.AppendL( MXIMPContextStateEvent::KInterfaceId );
    array.Append( MPresentityGroupListEvent::KInterfaceId );
    array.Append( MXIMPContextStateEvent::KInterfaceId );
    array.Append( MPresentityGroupListEvent::KInterfaceId );
    array.Append( MPresentityGroupContentEvent::KInterfaceId );
    aWrapper->SetupListenerReqCompleteL( aEventType, &array );
    CleanupStack::PopAndDestroy(); // array
    }

// ===========================================================================
// TEST CASE TABLE
// ===========================================================================
//
EUNIT_BEGIN_TEST_TABLE(
        T_PresenceGroupMemberManagement,
        "XIMP presence group member management tests",
        "MODULE" )

PRFW_DECORATED_TEST(
        "Subscribe and unsubscribe presentity group content",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_SubUnsubGroupContent_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Subscribe and unsubscribe multiple",
        "",
        "",
        "FUNCTIONALITY",
        SetupMultiple_L,
        T_SubUnsubGroupContent_Multi_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Refresh subscription for one, multiple",
        "",
        "",
        "FUNCTIONALITY",
        SetupMultiple_L,
        T_SubUnsubGroupContent_One_Multi_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Refresh subscription for all, multiple",
        "",
        "",
        "FUNCTIONALITY",
        SetupMultiple_L,
        T_SubUnsubGroupContent_All_Multi_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Subscribe content and receive altered id",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_SubUnsubGroupContent_Altered_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Subscribe content and receive altered but same id",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_SubUnsubGroupContent_AlteredSame_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Unsubscribe non-subscribed",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_UnsubscribeNonSubscribed_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Add members to group",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_AddGroupMembers_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Remove members from group",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_RemoveGroupMembers_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Remove members from unsubscribed group",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_RemoveGroupMembers_NonSub_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Remove nonexisting member from group",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_RemoveGroupMembers_NonExist_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Updated members from group",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_UpdateGroupMembers_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Updated members from nonsubscribed group",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_UpdateGroupMembers_NonSub_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Updated nonexisting member of group",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_UpdateGroupMembers_NonExist_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Expire non-subscribed data",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_ExpireNonSubscribedData_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Expire non-subscribed data. Update multiple times",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_ExpireNonSubscribedDataUpdateMultiple_L,
        Teardown )


EUNIT_END_TEST_TABLE


// ===========================================================================
// TEST SUITE CONSTRUCTION
// ===========================================================================
//
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    return T_PresenceGroupMemberManagement::NewL();
    }

// TODO implement this (split this file to multiple parts):
/*
// ===========================================================================
// TEST SUITE CONSTRUCTION
// ===========================================================================
//
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* suiteCollection = CEUnitTestSuite::NewLC( _L("XIMP group list management tests") );

    CEUnitTestSuite* suite = T_PresenceGroupListManagement::NewL();
    CleanupStack::PushL( suite );
    suiteCollection->AddL( suite );
    CleanupStack::Pop( suite );

    suite = T_xxx::NewL();
    CleanupStack::PushL( suite );
    suiteCollection->AddL( suite );
    CleanupStack::Pop( suite );

    CleanupStack::Pop( suiteCollection );
    return suiteCollection;
    }

*/



// ===========================================================================
// MISC
// ===========================================================================
//
//Exception handler function
void MyExceptionHandler( TExcType /*aType*/ )
    {
    __BREAKPOINT();
    }

void T_PresenceGroupMemberManagement::UseMyOwnExceptionHandler()
    {
    //Register exeption handler
    TInt r = User::SetExceptionHandler( MyExceptionHandler, 0xFFFFFFFF );
    if (r != KErrNone)
        {
        User::Panic( _L("MyExHandler"), 1 );
        }
    }


// end of file
