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
#include <presentitygroups.h>
#include <presentitygroupinfo.h>
#include <presentitygrouplistevent.h>
#include <ximpobjectfactory.h>

#include "ximpstatusimp.h"
#include "ximprequestcompleteeventimp.h"
#include "t_presencegrouplistmngt.h"
#include "prfwteststatuseventlistener.h"
#include "prfwtestmessaging.h"
#include "prfwtestmessenger.h"
#include "prfwtestcontextwrappermgr.h"
#include "prfwtestcontextwrapper.h"
#include "prfwtesteventfactory.h"

#include "presenceinfoimp.h"
#include "presenceinfofilterimp.h"
#include "prfwtestpresencedatautils.h"
#include "ximpidentityimp.h"
#include "ximpobjecthelpers.h"
#include "presentitygroupinfoimp.h"
#include "presentitygrouplisteventimp.h"
#include "presentitygroupcontenteventimp.h"
#include "prfwtestfilesrvmsg.h"
#include "prfwtestfiletool.h"
#include "prfwtestlistener.h"

#include "prfwtestrobustnesstools.h"

// ============================ MEMBER FUNCTIONS ===============================

// CONSTRUCTION
T_PresenceGroupListManagement* T_PresenceGroupListManagement::NewL()
    {
    T_PresenceGroupListManagement* self = new( ELeave ) T_PresenceGroupListManagement;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


T_PresenceGroupListManagement::~T_PresenceGroupListManagement()
    {
    PrfwTestRobustness::DoPreCleaning();
    }


void T_PresenceGroupListManagement::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


T_PresenceGroupListManagement::T_PresenceGroupListManagement()
    {
    }



// ===========================================================================
// TEST CASE SETUP & TEARDOWN METHODS
// ===========================================================================
//

void T_PresenceGroupListManagement::Setup_L()
    {
    PrfwTestRobustness::DoPreCleaning();

    iWrapperMgr = CXIMPTestContextWrapperMgr::NewL();
    iWrapperMgr->CreateWrapperL();
    }

void T_PresenceGroupListManagement::SetupMultiple_L()
    {
    PrfwTestRobustness::DoPreCleaning();


    iWrapperMgr = CXIMPTestContextWrapperMgr::NewL();
    iWrapperMgr->CreateWrapperL();
    iWrapperMgr->CreateWrapperL();
    iWrapperMgr->CreateWrapperL();
    }


void T_PresenceGroupListManagement::BindL()
    {
    CXIMPTestContextWrapper* wrapper0 = iWrapperMgr->GetWrapperL( 0 );

    wrapper0->BindL( 0 );
    }

void T_PresenceGroupListManagement::BindAllL()
    {
    for( TInt a = 0; a < iWrapperMgr->WrapperCount(); ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        wrapper->BindL( 0 );
        }
    }


void T_PresenceGroupListManagement::Teardown()
    {
    delete iWrapperMgr;
    iWrapperMgr = NULL;

    REComSession::FinalClose();

    PrfwTestRobustness::DoPreCleaning();
    }

void T_PresenceGroupListManagement::UnbindL()
    {
    // Do unbind
    CXIMPTestContextWrapper* wrapper0 = iWrapperMgr->GetWrapperL( 0 );
    wrapper0->UnbindL();
    }

void T_PresenceGroupListManagement::UnbindAllL()
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


// Single client, empty filter
void T_PresenceGroupListManagement::T_SubscribePresentityGroup_Single_L()
    {
    EUNIT_PRINT( _L("Single presentity group list subscription") );

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();


    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // ---------------------------------------------
    // normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // subscribe
    TXIMPRequestId reqId = presGroup.SubscribePresentityGroupListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe failed" );
    messenger->AssertSubscribePresentityGroupListCalled( ETrue );

    // ---------------------------------------------
    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );



    CPresentityGroupListEventImp* event = XIMPTestEventFactory::CreateGroupListEventLCX(
            KNullDesC, KNullDesC,
            XIMPTestEventFactory::ETestPGLEmpty,
            KNullDesC, KNullDesC,
            MXIMPDataSubscriptionState::ESubscriptionInactive,
            MXIMPDataSubscriptionState::EDataUnavailable
            );

    listener2->ExpectL( event );
    CleanupStack::Pop( event ); // 4 more items in cleanupstack

    // unsubscribe
    // ---------------------------------------------
    reqId = presGroup.UnsubscribePresentityGroupListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsubscribe failed" );
    CleanupStack::PopAndDestroy( 4 ); // group list event imp
    messenger->AssertUnsubscribePresentityGroupListCalled( ETrue );

    CleanupStack::PopAndDestroy( listener2 );

    UnbindL();
    }

// Single client, empty filter, error in unsubscribe
void T_PresenceGroupListManagement::T_UnsubscribePresentityGroup_Error_Single_L()
    {
    EUNIT_PRINT( _L("Single presentity group list unsubscribe with error") );

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // ---------------------------------------------
    // subscribe
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    TXIMPRequestId reqId = presGroup.SubscribePresentityGroupListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe failed" );
    messenger->AssertSubscribePresentityGroupListCalled( ETrue );

    // ---------------------------------------------
    // unsubscribe
    messenger->SetError( KErrArgument );
    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    evReqComplete->CompletionResultImp().SetResultCode( KErrArgument );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    reqId = presGroup.UnsubscribePresentityGroupListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsubscribe failed" );
    messenger->AssertUnsubscribePresentityGroupListCalled( ETrue );

    CleanupStack::PopAndDestroy( listener2 );

    UnbindL();
    }

// Single client, empty filter, error in sub
void T_PresenceGroupListManagement::T_SubscribePresentityGroup_Error_Single_L()
    {
    EUNIT_PRINT( _L("Single presentity group list subscribe with error") );

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    // Tell the protocol it should act normally
    messenger->SetError( KErrArgument );

    // ---------------------------------------------
    // subscribe
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    evReqComplete->CompletionResultImp().SetResultCode( KErrArgument );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    TXIMPRequestId reqId = presGroup.SubscribePresentityGroupListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe failed" );
    messenger->AssertSubscribePresentityGroupListCalled( ETrue );

    CleanupStack::PopAndDestroy( listener2 );

    UnbindL();
    }


// multiple clients, all subscribe + unsubscribe
void T_PresenceGroupListManagement::T_SubscribePresentityGroup_Multiple_L()
    {
    EUNIT_PRINT( _L("Multiple presentity group list subscription") );

    BindAllL();

    TInt countOfWrappers = iWrapperMgr->WrapperCount();
    for( TInt a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresentityGroups& presGroup = presFeat->PresentityGroups();

        CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
        CleanupStack::PushL( listener2 );

        // Tell the protocol it should act normally
        messenger->SetNoError();

        // expect normal request complete
        TXIMPRequestId reqIdDummy;  // not checked
        CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
        listener2->ExpectL( evReqComplete );
        CleanupStack::Pop( evReqComplete );

        // subscribe
        TXIMPRequestId reqId = presGroup.SubscribePresentityGroupListL();
        EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe failed" );
        messenger->AssertSubscribePresentityGroupListCalled( ETrue );

        CleanupStack::PopAndDestroy( listener2 );
        }

    for( TInt a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresentityGroups& presGroup = presFeat->PresentityGroups();

        CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
        CleanupStack::PushL( listener2 );

        // Tell the protocol it should act normally
        messenger->SetNoError();

        // expect normal request complete
        TXIMPRequestId reqIdDummy;  // not checked
        CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
        listener2->ExpectL( evReqComplete );
        CleanupStack::Pop( evReqComplete );

        CPresentityGroupListEventImp* event = XIMPTestEventFactory::CreateGroupListEventLCX(
                KNullDesC, KNullDesC,
                XIMPTestEventFactory::ETestPGLEmpty,
                KNullDesC, KNullDesC,
                MXIMPDataSubscriptionState::ESubscriptionInactive,
                MXIMPDataSubscriptionState::EDataUnavailable
                );

        listener2->ExpectL( event );
        CleanupStack::Pop( event ); // 4 more items in cleanupstack

        // unsubscribe
        TXIMPRequestId reqId = presGroup.UnsubscribePresentityGroupListL();
        EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsubscribe failed" );
        CleanupStack::PopAndDestroy( 4 ); // group list event imp
        messenger->AssertUnsubscribePresentityGroupListCalled( a == countOfWrappers-1 ? ETrue : EFalse );

        CleanupStack::PopAndDestroy( listener2 );
        }

    UnbindAllL();
    }

// Multiple clients, all subscribe and refresh
void T_PresenceGroupListManagement::T_RefreshPresentityGroup_Multiple_L()
    {
    EUNIT_PRINT( _L("Multiple presentity group list subscription refresh") );

    BindAllL();

    // SUBSCRIBE
    TInt countOfWrappers = iWrapperMgr->WrapperCount();
    for( TInt a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresentityGroups& presGroup = presFeat->PresentityGroups();

        CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
        CleanupStack::PushL( listener2 );

        // Tell the protocol it should act normally
        messenger->SetNoError();

        // expect normal request complete
        TXIMPRequestId reqIdDummy;  // not checked
        CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
        listener2->ExpectL( evReqComplete );
        CleanupStack::Pop( evReqComplete );

        // subscribe
        TXIMPRequestId reqId = presGroup.SubscribePresentityGroupListL();
        EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe failed" );
        messenger->AssertSubscribePresentityGroupListCalled( ETrue );

        CleanupStack::PopAndDestroy( listener2 );
        }

    // REFRESH
    for( TInt a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresentityGroups& presGroup = presFeat->PresentityGroups();

        CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
        CleanupStack::PushL( listener2 );

        // Tell the protocol it should act normally
        messenger->SetNoError();

        // refresh subscription
        TXIMPRequestId reqIdDummy;  // not checked
        CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
        listener2->ExpectL( evReqComplete );
        CleanupStack::Pop( evReqComplete );

        CPresentityGroupListEventImp* event = XIMPTestEventFactory::CreateGroupListEventLCX(
                KNullDesC, KNullDesC,
                XIMPTestEventFactory::ETestPGLUpdated,
                KNullDesC, KNullDesC,
                MXIMPDataSubscriptionState::ESubscriptionActive,
                MXIMPDataSubscriptionState::EDataUnavailable
                );

        listener2->ExpectL( event );
        CleanupStack::Pop( event ); // 4 more items in cleanupstack

        TXIMPRequestId reqId = presGroup.SubscribePresentityGroupListL();
        EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe failed" );
        CleanupStack::PopAndDestroy( 4 ); // group list event imp
        messenger->AssertSubscribePresentityGroupListCalled( ETrue );

        CleanupStack::PopAndDestroy( listener2 );
        }

    // unsubscribe all
    for( TInt a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresentityGroups& presGroup = presFeat->PresentityGroups();
        
        CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
        CleanupStack::PushL( listener2 );

        // Tell the protocol it should act normally
        messenger->SetNoError();

        // expect normal request complete
        TXIMPRequestId reqIdDummy;  // not checked
        CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
        listener2->ExpectL( evReqComplete );
        CleanupStack::Pop( evReqComplete );

        CPresentityGroupListEventImp* event = XIMPTestEventFactory::CreateGroupListEventLCX(
                KNullDesC, KNullDesC,
                XIMPTestEventFactory::ETestPGLEmpty,
                KNullDesC, KNullDesC,
                MXIMPDataSubscriptionState::ESubscriptionInactive,
                MXIMPDataSubscriptionState::EDataUnavailable
                );

        listener2->ExpectL( event );
        CleanupStack::Pop( event ); // 4 more items in cleanupstack

        // unsubscribe
        TXIMPRequestId reqId = presGroup.UnsubscribePresentityGroupListL();
        EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsubscribe failed" );
        CleanupStack::PopAndDestroy( 4 ); // group list event imp
        messenger->AssertUnsubscribePresentityGroupListCalled( a == countOfWrappers-1 ? ETrue : EFalse );

        CleanupStack::PopAndDestroy( listener2 );
        }

    UnbindAllL();
    }


// Multiple clients, all but one subscribe and all do unsubscribe
void T_PresenceGroupListManagement::T_SubscribeAll_One_Unsub_Multiple_L()
    {
    EUNIT_PRINT( _L("Multiple, all but one subscribes and all unsubscribe") );

    BindAllL();

    // SUBSCRIBE
    TInt countOfWrappers = iWrapperMgr->WrapperCount();
    TInt skipIndex = 1;
    for( TInt a = 0; a < countOfWrappers; ++a )
        {
        if ( skipIndex != a )
            {
            CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
            MXIMPContext* context = wrapper->GetContext();
            CXIMPTestMessenger* messenger = wrapper->GetMessenger();

            // Get the interfaces
            MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
            MPresentityGroups& presGroup = presFeat->PresentityGroups();

            CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
            CleanupStack::PushL( listener2 );

            // Tell the protocol it should act normally
            messenger->SetNoError();

            // expect normal request complete
            TXIMPRequestId reqIdDummy;  // not checked
            CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
            listener2->ExpectL( evReqComplete );
            CleanupStack::Pop( evReqComplete );

            // subscribe
            TXIMPRequestId reqId = presGroup.SubscribePresentityGroupListL();
            EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe failed" );
            messenger->AssertSubscribePresentityGroupListCalled( ETrue );

            CleanupStack::PopAndDestroy( listener2 );
            }
        }

    // unsubscribe all
    for( TInt a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresentityGroups& presGroup = presFeat->PresentityGroups();

        CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
        CleanupStack::PushL( listener2 );

        // Tell the protocol it should act normally
        messenger->SetNoError();

        // expect normal request complete
        TXIMPRequestId reqIdDummy;  // not checked
        CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
        listener2->ExpectL( evReqComplete );
        CleanupStack::Pop( evReqComplete );

        CPresentityGroupListEventImp* event = XIMPTestEventFactory::CreateGroupListEventLCX(
                KNullDesC, KNullDesC,
                XIMPTestEventFactory::ETestPGLEmpty,
                KNullDesC, KNullDesC,
                MXIMPDataSubscriptionState::ESubscriptionInactive,
                MXIMPDataSubscriptionState::EDataUnavailable
                );

        listener2->ExpectL( event );
        CleanupStack::Pop( event ); // 4 more items in cleanupstack

        // unsubscribe
        TXIMPRequestId reqId = presGroup.UnsubscribePresentityGroupListL();
        EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsubscribe failed" );
        CleanupStack::PopAndDestroy( 4 ); // group list event imp

        messenger->AssertUnsubscribePresentityGroupListCalled( a == countOfWrappers-1 ? ETrue : EFalse );

        CleanupStack::PopAndDestroy( listener2 );
        }

    UnbindAllL();
    }

// Multiple clients, refresh case for one
void T_PresenceGroupListManagement::T_RefreshPresentityGroup_One_Multiple_L()
    {
    EUNIT_PRINT( _L("Multiple, one context does presentity group list refresh") );

    BindAllL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();
 
     // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();
    
    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // ---------------------------------------------
    // subscribe
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    TXIMPRequestId reqId = presGroup.SubscribePresentityGroupListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe failed" );
    messenger->AssertSubscribePresentityGroupListCalled( ETrue );

    // ---------------------------------------------
    // refresh subscribe

    messenger->SetValueFor(
            EXIMPPlgTestSubscribeGroupListCalled,
            EFalse );

    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    CPresentityGroupListEventImp* event = XIMPTestEventFactory::CreateGroupListEventLCX(
            KNullDesC, KNullDesC,
            XIMPTestEventFactory::ETestPGLUpdated,
            KNullDesC, KNullDesC,
            MXIMPDataSubscriptionState::ESubscriptionActive,
            MXIMPDataSubscriptionState::EDataUnavailable
            );

    listener2->ExpectL( event );
    CleanupStack::Pop( event ); // 4 more items in cleanupstack

    reqId = presGroup.SubscribePresentityGroupListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe refresh failed" );
    CleanupStack::PopAndDestroy( 4 ); // group list event imp
    messenger->AssertSubscribePresentityGroupListCalled( EFalse );

    // ---------------------------------------------
    // unsubscribe
    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    event = XIMPTestEventFactory::CreateGroupListEventLCX(
            KNullDesC, KNullDesC,
            XIMPTestEventFactory::ETestPGLEmpty,
            KNullDesC, KNullDesC,
            MXIMPDataSubscriptionState::ESubscriptionInactive,
            MXIMPDataSubscriptionState::EDataUnavailable
            );

    listener2->ExpectL( event );
    CleanupStack::Pop( event ); // 4 more items in cleanupstack

    reqId = presGroup.UnsubscribePresentityGroupListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsubscribe failed" );
    CleanupStack::PopAndDestroy( 4 ); // group list event imp
    messenger->AssertUnsubscribePresentityGroupListCalled( ETrue );

    CleanupStack::PopAndDestroy( listener2 );

    UnbindAllL();
    }



void T_PresenceGroupListManagement::SubscribeL()
    {
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

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
    TXIMPRequestId reqId = presGroup.SubscribePresentityGroupListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe failed" );
    messenger->AssertSubscribePresentityGroupListCalled( ETrue );

    CleanupStack::PopAndDestroy( listener2 );
    }



void T_PresenceGroupListManagement::SubscribeGroupL( const TDesC& aGroupId )
    {
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    // ------------------------------------------------------
    // expect normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );
    // ------------------------------------------------------

    // subscribe group content
    MXIMPIdentity* identity = context->ObjectFactory().NewIdentityLC();
    identity->SetIdentityL( aGroupId );

    TXIMPRequestId reqId = presGroup.SubscribePresentityGroupContentL( *identity );
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Subscribe content failed" );

    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresentityGroupContentCalled, "SubscribeGroupContentL was not called", ETrue, NULL );

    CleanupStack::PopAndDestroy(); // identity
    CleanupStack::PopAndDestroy( listener2 );
    }



void T_PresenceGroupListManagement::UnsubscribeL()
    {
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    // ------------------------------------------------------
    // expect normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    CPresentityGroupListEventImp* event = XIMPTestEventFactory::CreateGroupListEventLCX(
            KNullDesC, KNullDesC,
            XIMPTestEventFactory::ETestPGLEmpty,
            KNullDesC, KNullDesC,
            MXIMPDataSubscriptionState::ESubscriptionInactive,
            MXIMPDataSubscriptionState::EDataUnavailable
            );

    listener2->ExpectL( event );
    CleanupStack::Pop( event ); // 4 more items in cleanupstack

    // unsubscribe
    TXIMPRequestId reqId = presGroup.UnsubscribePresentityGroupListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsubscribe failed" );
    CleanupStack::PopAndDestroy( 4 ); // group list event imp
    messenger->AssertUnsubscribePresentityGroupListCalled( ETrue );

    CleanupStack::PopAndDestroy( listener2 );
    }


void T_PresenceGroupListManagement::UnsubscribeGroupL( const TDesC& aGroupId )
    {
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    // ------------------------------------------------------
    // expect normal request complete
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // and a group content event, IF we have subscribed the group beforehand
    // i.e. SubscribeGroupL has been called before this

    MXIMPIdentity* identity = context->ObjectFactory().NewIdentityLC();
    identity->SetIdentityL( aGroupId );

    CPresentityGroupContentEventImp* groupCntEvent =
        XIMPTestEventFactory::CreateGroupContentEventLCX(
                aGroupId,
                KNullDesC,
                KNullDesC,
                XIMPTestEventFactory::ETestPGLCntRemoved,
                KNullDesC,
                KNullDesC,
                MXIMPDataSubscriptionState::ESubscriptionInactive,
                MXIMPDataSubscriptionState::EDataUnavailable
                );

    listener2->ExpectL( groupCntEvent );
    CleanupStack::Pop( groupCntEvent );

    // ------------------------------------------------------

    // unsubscribe

    TXIMPRequestId reqId = presGroup.UnsubscribePresentityGroupContentL( *identity );
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsubscribe group content failed" );
    messenger->AssertUnsubscribePresentityGroupContentCalled( ETrue );

    CleanupStack::PopAndDestroy( 5 ); // rest of the group content event
    CleanupStack::PopAndDestroy(); // identity

    CleanupStack::PopAndDestroy( listener2 );
    }



// Single client, empty filter
void T_PresenceGroupListManagement::T_HandlePresentityGroupList_L()
    {
    EUNIT_PRINT( _L("Handle presentity group list.") );

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
    // 1. Create faked server message about group list
    SendSrvMsgL( CXIMPTestFileSrvMsg::ETestSrvMsgPresentityGroupList );

    // ------------------------------------------------------
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
    // ------------------------------------------------------

    CleanupStack::PopAndDestroy( listener2 );

    // clean it up
    UnsubscribeL();
    UnbindL();
    }



_LIT16(  KPresentUri1,      "http://lorem.ipsum.com" );
_LIT16(  KPresentUri2,      "http://carpe.diem.com" );
_LIT16( KPresentDispName1, "DispnameA, LoremI" );
_LIT16( KPresentDispName2, "DispnameB, CarpeD" );
_LIT16( KPresentProtAddr1, "ABC" );
_LIT16( KPresentProtAddr2, "HIJ" );

void T_PresenceGroupListManagement::T_HandlePresentityGroupCreateAndDelete_L()
    {
    EUNIT_PRINT( _L("Handle presentity group creation and deletion.") );

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
    // 1. Create faked notification about create
    SendSrvMsgL( KPresentUri1, KPresentDispName1,
            CXIMPTestFileSrvMsg::ETestSrvMsgPresentityGroupCreate );

    // ------------------------------------------------------
    // expect MPresentityGroupList event.
    CPresentityGroupListEventImp* event = XIMPTestEventFactory::CreateGroupListEventLCX(
            KPresentUri1, KPresentDispName1,
            XIMPTestEventFactory::ETestPGLCreated,

            KPresentUri1, KPresentDispName1,
            MXIMPDataSubscriptionState::ESubscriptionActive,
            MXIMPDataSubscriptionState::EDataAvailable
            );

    listener2->ExpectL( event );
    CleanupStack::Pop( event ); // 4 more items in cleanupstack

    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "HandlePresentityGroupListL failed (Creation)" );
    CleanupStack::PopAndDestroy( 4 ); // lists

    // ------------------------------------------------------
    // Send a faked delete message

    SendSrvMsgL( KPresentUri1, KPresentDispName1,
            CXIMPTestFileSrvMsg::ETestSrvMsgPresentityGroupDelete );

    // ------------------------------------------------------
    // expect MPresentityGroupList event.

    event = XIMPTestEventFactory::CreateGroupListEventLCX(
            KPresentUri1, KNullDesC,
            XIMPTestEventFactory::ETestPGLDeleted,
            KNullDesC, KNullDesC,
            MXIMPDataSubscriptionState::ESubscriptionActive,
            MXIMPDataSubscriptionState::EDataAvailable
            );

    listener2->ExpectL( event );
    CleanupStack::Pop( event ); // 4 more items in cleanupstack

    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "HandlePresentityGroupListL failed (Deletion)" );
    CleanupStack::PopAndDestroy( 4 ); // lists
    // ------------------------------------------------------

    CleanupStack::PopAndDestroy( listener2 );

    // unsubscribe group list and log out
    UnsubscribeL();
    UnbindL();
    }

// Single client, subscribed to the group, then subscribe list
void T_PresenceGroupListManagement::T_HandlePresentityGroupList_Subscribed_L()
    {
    EUNIT_PRINT( _L("Handle presentity group list, subscribed to the group.") );

    // log in
    BindL();
    SubscribeGroupL( KPresentUri1 );
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
    // 1. Create faked server message about group list
    SendSrvMsgL( KPresentUri1,
            KPresentDispName1,
            CXIMPTestFileSrvMsg::ETestSrvMsgPresentityGroupCreate );

    // ------------------------------------------------------
    // expect empty MPresentityGroupList event.

    CPresentityGroupListEventImp* event = XIMPTestEventFactory::CreateGroupListEventLCX(
            KPresentUri1, KPresentDispName1,
            XIMPTestEventFactory::ETestPGLCreated,
            KPresentUri1, KPresentDispName1,
            MXIMPDataSubscriptionState::ESubscriptionActive,
            MXIMPDataSubscriptionState::EDataAvailable
            );

    listener2->ExpectL( event );
    CleanupStack::Pop( event ); // 4 more items in cleanupstack

    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "HandlePresentityGroupListL failed" );
    CleanupStack::PopAndDestroy( 4 ); // lists
    // ------------------------------------------------------

    CleanupStack::PopAndDestroy( listener2 );

    // clean it up
    UnsubscribeL();
    UnsubscribeGroupL( KPresentUri1 );
    UnbindL();
    }

void T_PresenceGroupListManagement::T_HandlePresentityGroupUpdate_L()
    {
    EUNIT_PRINT( _L("Handle presentity group displayname update.") );

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
    // 1. Create faked notification about create
    SendSrvMsgL( KPresentUri1, KPresentDispName1,
            CXIMPTestFileSrvMsg::ETestSrvMsgPresentityGroupCreate );

    // ------------------------------------------------------
    // expect MPresentityGroupList event.

    CPresentityGroupListEventImp* event = XIMPTestEventFactory::CreateGroupListEventLCX(
            KPresentUri1, KPresentDispName1,
            XIMPTestEventFactory::ETestPGLCreated,
            KPresentUri1, KPresentDispName1,
            MXIMPDataSubscriptionState::ESubscriptionActive,
            MXIMPDataSubscriptionState::EDataAvailable
            );

    listener2->ExpectL( event );
    CleanupStack::Pop( event ); // 4 more items in cleanupstack

    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "HandlePresentityGroupListL failed (Creation)" );
    CleanupStack::PopAndDestroy( 4 ); // lists

    // ------------------------------------------------------
    // 4. Change displayname of group and send faked notification about
    // update

    SendSrvMsgL( KPresentUri1, KPresentDispName2,
            CXIMPTestFileSrvMsg::ETestSrvMsgPresentityGroupUpdate );

    // ------------------------------------------------------
    // expect MPresentityGroupList event.

    event = XIMPTestEventFactory::CreateGroupListEventLCX(
            KPresentUri1, KPresentDispName2,
            XIMPTestEventFactory::ETestPGLUpdated,
            KPresentUri1, KPresentDispName2,
            MXIMPDataSubscriptionState::ESubscriptionActive,
            MXIMPDataSubscriptionState::EDataAvailable
            );

    listener2->ExpectL( event );
    CleanupStack::Pop( event ); // 4 more items in cleanupstack

    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "HandlePresentityGroupListL failed (Update)" );
    CleanupStack::PopAndDestroy( 4 ); // lists
    // ------------------------------------------------------

    CleanupStack::PopAndDestroy( listener2 );

    // unsubscribe group list and log out
    UnsubscribeL();
    UnbindL();
    }

// ===========================================================================
// SETUP HELPERS FOR THE LISTENER
// ===========================================================================
//

// send a faked server-originated message with group info made
// from the given uri and displayname
//
void T_PresenceGroupListManagement::SendSrvMsgL(
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
void T_PresenceGroupListManagement::SendSrvMsgL( TInt aMsgType )
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

// update display name of group without having subscribed to it
void T_PresenceGroupListManagement::T_UpdateDisplayNameL(
        const TDesC& aGroupId,
        const TDesC& aDispName )
    {
    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

    wrapper->GetMessenger()->SetNoError();

    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    MXIMPIdentity* groupId = context->ObjectFactory().NewIdentityLC();
    groupId->SetIdentityL( aGroupId );

    // ------------------------------------------------------
    // expect request complete with a group info imp inside
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );

    CPresentityGroupInfoImp* grpInfo = CPresentityGroupInfoImp::NewLC(
            *groupId,
            aDispName );

    evReqComplete->AppendParamL( grpInfo );
    CleanupStack::Pop(); // grpInfo

    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // ------------------------------------------------------
    // update
    TXIMPRequestId reqId = presGroup.UpdatePresentityGroupDisplayNameL(
            *groupId,
            aDispName );

    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsubscribe group content failed" );

    messenger->AssertUpdatePresentityGroupDisplayNameCalled( ETrue );

    CleanupStack::PopAndDestroy(); // groupId
    CleanupStack::PopAndDestroy( listener2 );
    }


// test method for updating display name of group without having subscribed
// to it
void T_PresenceGroupListManagement::T_UpdateDisplayName_Single_L()
    {
    EUNIT_PRINT( _L("Update presentity group displayname, single.") );

    BindL();

    T_UpdateDisplayNameL( KPresentUri1, KPresentDispName1 );

    UnbindL();
    }



// Single client, unsubscribe nonsubscribed
void T_PresenceGroupListManagement::T_UnsubNonsubscribed_Single_L()
    {
    EUNIT_PRINT( _L("Single unsubscribe nonsubscribed") );

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();

    CXIMPTestListener* listener2 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener2 );

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // ---------------------------------------------
    // unsubscribe
    TXIMPRequestId reqIdDummy;  // not checked
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );
    listener2->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // group list event with Unsubscribed
    CPresentityGroupListEventImp* event = XIMPTestEventFactory::CreateGroupListEventLCX(
            KNullDesC, KNullDesC,
            XIMPTestEventFactory::ETestPGLEmpty,

            KNullDesC, KNullDesC,
            MXIMPDataSubscriptionState::ESubscriptionInactive,
            MXIMPDataSubscriptionState::EDataUnavailable
            );

    listener2->ExpectL( event );
    CleanupStack::Pop( event ); // 4 more items in cleanupstack

    TXIMPRequestId reqId = presGroup.UnsubscribePresentityGroupListL();
    EUNIT_ASSERT_DESC( KErrNone == listener2->WaitAndAssertL(), "Unsubscribe failed" );
    CleanupStack::PopAndDestroy( 4 ); // rest of the group list event
    messenger->AssertUnsubscribePresentityGroupListCalled( EFalse );  // should not be called, no subscriptions

    CleanupStack::PopAndDestroy( listener2 );

    UnbindL();
    }

// ===========================================================================
// TEST CASE TABLE
// ===========================================================================
//
EUNIT_BEGIN_TEST_TABLE(
        T_PresenceGroupListManagement,
        "XIMP presence group list management tests",
        "MODULE" )

PRFW_DECORATED_TEST(
        "Subscribe single presence",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_SubscribePresentityGroup_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Subscribe single presence with error",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_SubscribePresentityGroup_Error_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Unsubscribe single presence with error",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_UnsubscribePresentityGroup_Error_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Unsubscribe nonsubscribed",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_UnsubNonsubscribed_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Subscribe multiple presence",
        "",
        "",
        "FUNCTIONALITY",
        SetupMultiple_L,
        T_SubscribePresentityGroup_Multiple_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Multiple subscribe all but one, unsub all",
        "",
        "",
        "FUNCTIONALITY",
        SetupMultiple_L,
        T_SubscribeAll_One_Unsub_Multiple_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Single refresh multiple presence",
        "",
        "",
        "FUNCTIONALITY",
        SetupMultiple_L,
        T_RefreshPresentityGroup_One_Multiple_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Multiple refresh presence",
        "",
        "",
        "FUNCTIONALITY",
        SetupMultiple_L,
        T_RefreshPresentityGroup_Multiple_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Handle Presentity Group List",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_HandlePresentityGroupList_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Handle Presentity Group List (subscribed group)",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_HandlePresentityGroupList_Subscribed_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Create and delete group.",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_HandlePresentityGroupCreateAndDelete_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Handle update displayname",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_HandlePresentityGroupUpdate_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Update displayname",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_UpdateDisplayName_Single_L,
        Teardown )

EUNIT_END_TEST_TABLE


// ===========================================================================
// TEST SUITE CONSTRUCTION
// ===========================================================================
//
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    return T_PresenceGroupListManagement::NewL();
    }


// end of file
