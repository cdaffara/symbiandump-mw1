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
#include <presenceauthorization.h>
#include <presencepublishing.h>
#include <presentitypresenceevent.h>
#include <ximpobjectfactory.h>

#include "prfwtestlistener.h"

#include <presentitygroups.h>
#include <presentitygroupinfo.h>
#include <presentitygrouplistevent.h>

#include "t_presenceauthorization.h"
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

#include "prfwtestrobustnesstools.h"


#include "presentitygroupcontenteventimp.h"
#include "ximprequestcompleteeventimp.h"
#include "presentitygroupinfoimp.h"
#include "presentitygrouplisteventimp.h"
#include "presentitygroupmemberinfoimp.h"
#include "ximprequestcompleteeventimp.h"
#include "ximprestrictedobjectcollection.h"
#include "ximpobjectfactoryimp.h"

_LIT8( KIdentity1, "Identity1" );
_LIT8( KIdentity2, "Identity2" );
_LIT8( KIdentity3, "Identity3" );

_LIT( KIdentity1_16, "Identity1" );
_LIT( KIdentity2_16, "Identity2" );
_LIT( KIdentity3_16, "Identity3" );


_LIT( KGroupId,  "Group" );
_LIT( KGroupId1, "Group1" );
_LIT( KGroupId2, "Group2" );


_LIT( KGroupDisplayName, "GroupDisplayName" );


_LIT8( KPrefix1, "PreA_" );
_LIT8( KPrefix2, "PreB_" );
_LIT8( KPrefix3, "PreC_" );
_LIT8( KPrefix4, "PreD_" );

_LIT( KIdentity, "TestId@hello.world" );

// ============================ MEMBER FUNCTIONS ===============================

// CONSTRUCTION
T_PresenceAuthorization* T_PresenceAuthorization::NewL()
    {
    T_PresenceAuthorization* self = new( ELeave ) T_PresenceAuthorization;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


T_PresenceAuthorization::~T_PresenceAuthorization()
    {
    }


void T_PresenceAuthorization::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


T_PresenceAuthorization::T_PresenceAuthorization()
    {
    }



// ===========================================================================
// TEST CASE SETUP & TEARDOWN METHODS
// ===========================================================================
//

void T_PresenceAuthorization::Setup_L()
    {
    PrfwTestRobustness::DoPreCleaning();

    iWrapperMgr = CXIMPTestContextWrapperMgr::NewL();
    iWrapperMgr->CreateWrapperL();
    }

void T_PresenceAuthorization::SetupMultiple_L()
    {
    PrfwTestRobustness::DoPreCleaning();

    iWrapperMgr = CXIMPTestContextWrapperMgr::NewL();
    iWrapperMgr->CreateWrapperL();
    iWrapperMgr->CreateWrapperL();
    iWrapperMgr->CreateWrapperL();
    }


void T_PresenceAuthorization::BindL()
    {
    CXIMPTestContextWrapper* wrapper0 = iWrapperMgr->GetWrapperL( 0 );

    wrapper0->BindL( 0 );
    }

void T_PresenceAuthorization::BindAllL()
    {
    for( TInt a = 0; a < iWrapperMgr->WrapperCount(); ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        wrapper->BindL( 0 );
        }
    }


void T_PresenceAuthorization::Teardown()
    {
    delete iWrapperMgr;
    iWrapperMgr = NULL;

    REComSession::FinalClose();

    PrfwTestRobustness::DoPreCleaning();
    }

void T_PresenceAuthorization::UnbindL()
    {
    // Do unbind
    CXIMPTestContextWrapper* wrapper0 = iWrapperMgr->GetWrapperL( 0 );
    wrapper0->UnbindL();
    }

void T_PresenceAuthorization::UnbindAllL()
    {
    for( TInt a = 0; a < iWrapperMgr->WrapperCount(); ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        wrapper->UnbindL();
        }
    }


// ===========================================================================
// TEST CASES for presentity authorization
// ===========================================================================

void T_PresenceAuthorization::T_GrantPresenceForPresentity_L()
    {
    EUNIT_PRINT( _L("Grant presence for presentity") );

// INITIALIZE

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif =
                XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

// GRANT

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // register interest
    TXIMPRequestId reqId = presAuth.GrantPresenceForPresentityL( *identity, *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForPresentityCalled, 
                        "GrantPresenceForPresentityL was not called", ETrue, NULL );

// GRANT AGAIN

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );
    messenger->SetValueFor( EXIMPPlgTestGrantPresenceForPresentityCalled, 0 );
    
    // register interest
    reqId = presAuth.GrantPresenceForPresentityL( *identity, *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForPresentityCalled, 
                        "GrantPresenceForPresentityL was not called", EFalse, 
                        "GrantPresenceForPresentityL was called" );

// WITHDRAW

    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );
    // register interest
    reqId = presAuth.WithdrawPresenceGrantFromPresentityL( *identity );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Grantin presence to presentity, complete with error: ") );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestWithdrawPresenceFromPresentityCalled, 
                            "WithdrawPresenceForPresentityL was not called", ETrue, NULL );

// DEINITIALIZE

    CleanupStack::PopAndDestroy( 2, identity ); //pif, identity
    UnbindL();
    }

// ==============================================================================================

void T_PresenceAuthorization::T_WithdrawNotGrantedPresenceFromPresentity_L()
    {
    EUNIT_PRINT( _L("Withdraw presence from presentity not granted") );

// INITIALIZE

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );

// WITHDRAW

    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );
    // register interest
    TXIMPRequestId reqId = presAuth.WithdrawPresenceGrantFromPresentityL( *identity );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Grantin presence to presentity, complete with error: ") );
    // Changed for OPAA-73BCS(
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestWithdrawPresenceFromPresentityCalled, 
                        "WithdrawPresenceForPresentityL was not called", ETrue/*EFalse*/, 
                        "WithdrawPresenceForPresentityL was called" );

// DEINITIALIZE

    CleanupStack::PopAndDestroy( identity ); //identity
    UnbindL();
    }

// ==============================================================================================


void T_PresenceAuthorization::T_GrantPresenceForPresentityChangeId_L()
    {
    EUNIT_PRINT( _L("Grant presence for presentity change id") );

// INITIALIZE

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Get the management interface
    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();


    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif =
                XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

// ID CHANGE REQUEST TO ADAPTATION

    MXIMPIdentity* identityParam = context->ObjectFactory().NewIdentityLC();
    identityParam->SetIdentityL( KIdentity );
    
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
    CXIMPTestWaitHelper* wait = CXIMPTestWaitHelper::NewL();
    CleanupStack::PushL( wait );
    wait->WaitForL( 2 );
    CleanupStack::PopAndDestroy( wait );

// GRANT

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // register interest
    TXIMPRequestId reqId = presAuth.GrantPresenceForPresentityL( *identity, *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForPresentityCalled, 
                        "GrantPresenceForPresentityL was not called", ETrue, NULL );

// DEINITIALIZE

    CleanupStack::PopAndDestroy( 2, identity ); //pif, identity
    UnbindL();
    }

// ==============================================================================================

void T_PresenceAuthorization::T_WithdrawNotGrantedPresenceFromPresentity_Multiple_L()
    {
    EUNIT_PRINT( _L("Withdraw presence from presentity not granted multiple clients") );

    BindAllL();

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif =
                XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

// GRANT ALL BUT FIRST

    TInt countOfWrappers = iWrapperMgr->WrapperCount();
    TInt a(0);
    for( a = countOfWrappers - 1; a >= 1; --a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Tell the protocol it should act normally
        messenger->SetNoError();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

        // register interest
        TXIMPRequestId reqId = presAuth.GrantPresenceForPresentityL( *identity, *pif );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForPresentityCalled,
            "GrantPresenceForPresentityL was not called",
            ( a > 1 ) ? ETrue : EFalse,
            "GrantPresenceForPresentityL was called" );
        }
        
// WITHDRAW
        
    for( a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();
        
        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk ); // allow events after reqcomplete

        // register interest
        TXIMPRequestId reqId = presAuth.WithdrawPresenceGrantFromPresentityL( *identity );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        wrapper->VerifyEventStackL( _L8("Subscribing presentity presence, complete with error: ") );
        // Changed for OPAA-73BCS8
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestWithdrawPresenceFromPresentityCalled, 
            "WithdrawPresenceForPresentityL was not called",
            ( a < ( countOfWrappers -1 ) /*a < 2*/ ) ? EFalse : ETrue,
            "WithdrawPresenceForPresentityL was called" );
        }
    
    CleanupStack::PopAndDestroy( pif );
    CleanupStack::PopAndDestroy( identity );
        
    UnbindAllL();    
    }


// ==============================================================================================

void T_PresenceAuthorization::T_ErrFromPlg_LeaveOnGrantPresenceForPresentity_L()
    {
    EUNIT_PRINT( _L("Error from plug-in in grant presence for presentity") );    

// INITIALIZE

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();


    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif =
                XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

// GRANT

    messenger->SetLeave( KErrAbort );

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_PresentitySubscribeFailKErrAbort );

    // register interest
    TXIMPRequestId reqId = presAuth.GrantPresenceForPresentityL( *identity, *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForPresentityCalled, 
                        "GrantPresenceForPresentityL was not called", EFalse, 
                        "GrantPresenceForPresentityL was called" );

    messenger->SetLeave( KErrNone );

// DEINITIALIZE

    CleanupStack::PopAndDestroy( 2, identity ); //pif, identity
    UnbindL();
    }


// ==============================================================================================

void T_PresenceAuthorization::T_ErrFromPlg_LeaveOnWithdrawPresenceFromPresentity_L()
    {
    EUNIT_PRINT( _L("Error from plug-in in withdraw presence from presentity") );    

// INITIALIZE

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();


    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif =
                XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

// GRANT

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // register interest
    TXIMPRequestId reqId = presAuth.GrantPresenceForPresentityL( *identity, *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForPresentityCalled, 
                        "GrantPresenceForPresentityL was not called", ETrue, 
                        "GrantPresenceForPresentityL was called" );

// WITHDRAW

    messenger->SetLeave( KErrAbort );

    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_PresentitySubscribeFailKErrAbort );
    // register interest
    reqId = presAuth.WithdrawPresenceGrantFromPresentityL( *identity );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Grantin presence to presentity, complete with error: ") );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestWithdrawPresenceFromPresentityCalled, 
                        "WithdrawPresenceGrantFromPresentityL was not called", EFalse, 
                        "WithdrawPresenceGrantFromPresentityL was called" );

    messenger->SetLeave( KErrNone );

// DEINITIALIZE

    CleanupStack::PopAndDestroy( 2, identity ); //pif, identity
    UnbindL();
    }

// ==============================================================================================

void T_PresenceAuthorization::T_GrantPresenceForPresentity_Multiple_L()
    {
    EUNIT_PRINT( _L("Grant presence for presentity multiple clients") );

    BindAllL();

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif =
                XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

// GRANT

    TInt countOfWrappers = iWrapperMgr->WrapperCount();
    TInt a(0);
    for( a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Tell the protocol it should act normally
        messenger->SetNoError();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();


        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

        // register interest
        TXIMPRequestId reqId = presAuth.GrantPresenceForPresentityL( *identity, *pif );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForPresentityCalled, 
            "GrantPresenceForPresentityL was not called",
            ( a == 0 ) ? ETrue : EFalse,
            "GrantPresenceForPresentityL was called" );
        }
        
// GRANT AGAIN        

    for( a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Tell the protocol it should act normally
        messenger->SetNoError();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );
        messenger->SetValueFor( EXIMPPlgTestGrantPresenceForPresentityCalled, 0 );

        // register interest
        TXIMPRequestId reqId = presAuth.GrantPresenceForPresentityL( *identity, *pif );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForPresentityCalled, 
            "GrantPresenceForPresentityL was not called", EFalse,
            "GrantPresenceForPresentityL was called" );
        }

        
// WITHDRAW
        
    for( a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();
        
    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();


        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk ); // allow events after reqcomplete

        // register interest
        TXIMPRequestId reqId = presAuth.WithdrawPresenceGrantFromPresentityL( *identity );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        wrapper->VerifyEventStackL( _L8("Subscribing presentity presence, complete with error: ") );
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestWithdrawPresenceFromPresentityCalled, 
            "WithdrawPresenceForPresentityL was not called",
            ( a < 2 ) ? EFalse : ETrue,
            "WithdrawPresenceForPresentityL was called" );
        }
    
    CleanupStack::PopAndDestroy( pif );
    CleanupStack::PopAndDestroy( identity );
        
    UnbindAllL();    
    }


// ==============================================================================================

void T_PresenceAuthorization::T_WithdrawNotGrantedPresenceFromGroupMembers_L()
    {
    EUNIT_PRINT( _L("Withdraw presence from group members not granted") );

// INITIALIZE

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();


    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );

// WITHDRAW

    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );
    // register interest
    TXIMPRequestId reqId = presAuth.WithdrawPresenceGrantFromPresentityGroupMembersL( *identity );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Withdraw not granted group members, complete with error: ") );
    // Changed for OPAA-73BCS8
    COMMONASSERT( messenger, EXIMPPlgTestWithdrawPresenceFromPresentityGroupMembersCalled, 
                        "WithdrawPresenceGrantFromPresentityGroupMembersL was not called", ETrue/*EFalse*/, 
                        "WithdrawPresenceGrantFromPresentityGroupMembersL was called" );

// DEINITIALIZE

    CleanupStack::PopAndDestroy( identity ); //identity
    UnbindL();
    }


// ==============================================================================================

void T_PresenceAuthorization::T_WithdrawNotGrantedPresenceFromGroupMembers_Multiple_L()
    {
    EUNIT_PRINT( _L("Withdraw presence from group members not granted multiple clients") );

    BindAllL();

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif =
                XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

// GRANT ALL BUT FIRST

    TInt countOfWrappers = iWrapperMgr->WrapperCount();
    TInt a(0);
    for( a = countOfWrappers - 1; a >= 1; --a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Tell the protocol it should act normally
        messenger->SetNoError();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();


        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

        // register interest
        TXIMPRequestId reqId = presAuth.GrantPresenceForPresentityGroupMembersL( *identity, *pif );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForPresentityGroupMembersCalled,
            "GrantPresenceForPresentityGroupMembersL was not called",
            ( a > 1 ) ? ETrue : EFalse,
            "GrantPresenceForPresentityGroupMembersL was called" );
        }
        
// WITHDRAW
        
    for( a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();
        
        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();


        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk ); // allow events after reqcomplete

        // register interest
        TXIMPRequestId reqId = presAuth.WithdrawPresenceGrantFromPresentityGroupMembersL( *identity );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        wrapper->VerifyEventStackL( _L8("Subscribing presentity presence, complete with error: ") );
        // Changed for OPAA-73BCS8
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestWithdrawPresenceFromPresentityGroupMembersCalled, 
            "WithdrawPresenceForPresentityGroupMembersL was not called",
            ( a < ( countOfWrappers - 1 )/*a < 2*/ ) ? EFalse : ETrue,
            "WithdrawPresenceForPresentityGroupMembersL was called" );
        }
    
    CleanupStack::PopAndDestroy( pif );
    CleanupStack::PopAndDestroy( identity );
        
    UnbindAllL();    
    }

// ==============================================================================================


void T_PresenceAuthorization::T_GrantPresenceForGroupMembers_Multiple_L()
    {
    EUNIT_PRINT( _L("Grant presence for group members multiple clients") );

    BindAllL();

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif =
                XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

// GRANT

    TInt countOfWrappers = iWrapperMgr->WrapperCount();
    TInt a(0);
    for( a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Tell the protocol it should act normally
        messenger->SetNoError();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();


        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

        // register interest
        TXIMPRequestId reqId = presAuth.GrantPresenceForPresentityGroupMembersL( *identity, *pif );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForPresentityGroupMembersCalled, 
            "GrantPresenceForPresentityGroupMembersL was not called",
            ( a == 0 ) ? ETrue : EFalse,
            "GrantPresenceForPresentityGroupMembersL was called" );
        }
        
// GRANT AGAIN        

    for( a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Tell the protocol it should act normally
        messenger->SetNoError();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();


        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );
        messenger->SetValueFor( EXIMPPlgTestGrantPresenceForPresentityGroupMembersCalled, 0 );        

        // register interest
        TXIMPRequestId reqId = presAuth.GrantPresenceForPresentityGroupMembersL( *identity, *pif );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForPresentityGroupMembersCalled, 
            "GrantPresenceForPresentityGroupMembersL was not called", EFalse,
            "GrantPresenceForPresentityGroupMembersL was called" );
        }

        
// WITHDRAW
        
    for( a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();
        
    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();


        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk ); // allow events after reqcomplete

        // register interest
        TXIMPRequestId reqId = presAuth.WithdrawPresenceGrantFromPresentityGroupMembersL( *identity );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        wrapper->VerifyEventStackL( _L8("Subscribing presentity presence, complete with error: ") );
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestWithdrawPresenceFromPresentityGroupMembersCalled, 
            "WithdrawPresenceForPresentityGroupMembersL was not called",
            ( a < 2 ) ? EFalse : ETrue,
            "WithdrawPresenceForPresentityGroupMembersL was called" );
        }
    
    CleanupStack::PopAndDestroy( pif );
    CleanupStack::PopAndDestroy( identity );
        
    UnbindAllL();    
    }

// ==============================================================================================

void T_PresenceAuthorization::T_ErrFromPlg_LeaveOnGrantPresenceForGroupMembers_L()
    {
    EUNIT_PRINT( _L("Error from plug-in in grant presence for group members") );    

// INITIALIZE

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif =
                XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

// GRANT

    messenger->SetLeave( KErrAbort );

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_PresentitySubscribeFailKErrAbort );

    // register interest
    TXIMPRequestId reqId = presAuth.GrantPresenceForPresentityGroupMembersL( *identity, *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForPresentityGroupMembersCalled, 
                        "GrantPresenceForPresentityGroupMembersL was not called", EFalse, 
                        "GrantPresenceForPresentityGroupMembersL was called" );

    messenger->SetLeave( KErrNone );

// DEINITIALIZE

    CleanupStack::PopAndDestroy( 2, identity ); //pif, identity
    UnbindL();
    }

// ==============================================================================================

void T_PresenceAuthorization::T_ErrFromPlg_LeaveOnWithdrawPresenceFromGroupMembers_L()
    {
    EUNIT_PRINT( _L("Error from plug-in in withdraw presence from group members") );    

// INITIALIZE

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif =
                XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

// GRANT

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // register interest
    TXIMPRequestId reqId = presAuth.GrantPresenceForPresentityGroupMembersL( *identity, *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForPresentityGroupMembersCalled, 
                        "GrantPresenceForPresentityGroupMembersL was not called", ETrue, 
                        "GrantPresenceForPresentityGroupMembersL was called" );

// WITHDRAW

    messenger->SetLeave( KErrAbort );

    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_PresentitySubscribeFailKErrAbort );
    // register interest
    reqId = presAuth.WithdrawPresenceGrantFromPresentityGroupMembersL( *identity );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Grantin presence to presentity, complete with error: ") );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestWithdrawPresenceFromPresentityGroupMembersCalled, 
                        "WithdrawPresenceGrantFromPresentityGroupMembersL was not called", EFalse, 
                        "WithdrawPresenceGrantFromPresentityGroupMembersL was called" );

    messenger->SetLeave( KErrNone );

// DEINITIALIZE

    CleanupStack::PopAndDestroy( 2, identity ); //pif, identity
    UnbindL();
    }

// ==============================================================================================

// To Everyone, empty filter
void T_PresenceAuthorization::T_GrantPresenceForEveryone_L()
    {
    EUNIT_PRINT( _L("Grant presence for everyone") );

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

    CPresenceInfoFilterImp* pif = XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

// GRANT

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    TXIMPRequestId reqId = presAuth.GrantPresenceForEveryoneL( *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForEveryoneCalled, "GrantPresenceForEveryoneL was not called", ETrue, NULL );

// GRANT AGAIN

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );
    messenger->SetValueFor( EXIMPPlgTestGrantPresenceForEveryoneCalled, 0 );    

    reqId = presAuth.GrantPresenceForEveryoneL( *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForEveryoneCalled, "GrantPresenceForEveryoneL was not called", EFalse, "GrantPresenceForEveryoneL was called" );

// WITHDRAW

    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );
    // register interest
    reqId = presAuth.WithdrawPresenceGrantFromEveryoneL();

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Granting presence to everyone, complete with error: ") );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestWithdrawPresenceFromEveryoneCalled, "WithdrawPresenceForEveryoneL was not called", ETrue, NULL );

    CleanupStack::PopAndDestroy( pif ); //pif
    UnbindL();
    }    
    
// ==============================================================================================

void T_PresenceAuthorization::T_ErrFromPlg_LeaveOnGrantPresenceForEveryone_L()
    {
    EUNIT_PRINT( _L("Error from plug-in in grant presence for everyone") );    

// INITIALIZE

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

    CPresenceInfoFilterImp* pif =
                XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

// GRANT

    messenger->SetLeave( KErrAbort );

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_PresentitySubscribeFailKErrAbort );

    // register interest
    TXIMPRequestId reqId = presAuth.GrantPresenceForEveryoneL( *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForEveryoneCalled, 
                        "GrantPresenceForEveryoneL was not called", EFalse, 
                        "GrantPresenceForEveryoneL was called" );

    messenger->SetLeave( KErrNone );

// DEINITIALIZE

    CleanupStack::PopAndDestroy( pif ); //pif
    UnbindL();
    }

// ==============================================================================================

void T_PresenceAuthorization::T_GrantPresenceForEveryone_Multiple_L()
    {
    EUNIT_PRINT( _L("Grant presence for presentity multiple clients") );

    BindAllL();

    CPresenceInfoFilterImp* pif =
                XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

// GRANT

    TInt countOfWrappers = iWrapperMgr->WrapperCount();
    TInt a(0);
    for( a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Tell the protocol it should act normally
        messenger->SetNoError();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

        // register interest
        TXIMPRequestId reqId = presAuth.GrantPresenceForEveryoneL( *pif );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForEveryoneCalled, 
            "GrantPresenceForEveryoneL was not called",
            ( a == 0 ) ? ETrue : EFalse,
            "GrantPresenceForEveryoneL was called" );
        }
        
// GRANT AGAIN        

    for( a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Tell the protocol it should act normally
        messenger->SetNoError();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );
        messenger->SetValueFor( EXIMPPlgTestGrantPresenceForEveryoneCalled, 0 );


        // register interest
        TXIMPRequestId reqId = presAuth.GrantPresenceForEveryoneL( *pif );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForEveryoneCalled, 
            "GrantPresenceForEveryoneL was not called", EFalse,
            "GrantPresenceForEveryoneL was called" );
        }

        
// WITHDRAW
        
    for( a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();
        
    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk ); // allow events after reqcomplete

        // register interest
        TXIMPRequestId reqId = presAuth.WithdrawPresenceGrantFromEveryoneL();

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        wrapper->VerifyEventStackL( _L8("Subscribing presentity presence, complete with error: ") );
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestWithdrawPresenceFromEveryoneCalled, 
            "WithdrawPresenceGrantFromEveryoneL was not called",
            ( a < 2 ) ? EFalse : ETrue,
            "WithdrawPresenceGrantFromEveryoneL was called" );
        }
    
    CleanupStack::PopAndDestroy( pif );
        
    UnbindAllL();    
    }


// ==============================================================================================

void T_PresenceAuthorization::T_ErrFromPlg_LeaveOnWithdrawPresenceFromEveryone_L()
    {
    EUNIT_PRINT( _L("Error from plug-in in withdraw presence from everyone") );    

// INITIALIZE

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

    CPresenceInfoFilterImp* pif =
                XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

// GRANT

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // register interest
    TXIMPRequestId reqId = presAuth.GrantPresenceForEveryoneL( *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForEveryoneCalled, 
                        "GrantPresenceForEveryoneL was not called", ETrue, 
                        "GrantPresenceForEveryoneL was called" );

// WITHDRAW

    messenger->SetLeave( KErrAbort );

    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_PresentitySubscribeFailKErrAbort );
    // register interest
    reqId = presAuth.WithdrawPresenceGrantFromEveryoneL();

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Grantin presence to presentity, complete with error: ") );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestWithdrawPresenceFromPresentityCalled, 
                        "WithdrawPresenceGrantFromEveryoneL was not called", EFalse, 
                        "WithdrawPresenceGrantFromEveryoneL was called" );

    messenger->SetLeave( KErrNone );

// DEINITIALIZE

    CleanupStack::PopAndDestroy( pif ); //pif
    UnbindL();
    }
    
    

// ==============================================================================================

void T_PresenceAuthorization::T_GrantPresenceForGroupMembers_L()
    {
    EUNIT_PRINT( _L("Grant presence for presentity group members") );

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif =
                XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

// GRANT

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // register interest
    TXIMPRequestId reqId = presAuth.GrantPresenceForPresentityGroupMembersL( *identity, *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForPresentityGroupMembersCalled, "GrantPresenceForPresentityGroupMembers was not called", ETrue, NULL );

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk ); // allow events after reqcomplete

// GRANT AGAIN

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );
    messenger->SetValueFor( EXIMPPlgTestGrantPresenceForPresentityGroupMembersCalled, 0 );
     
    reqId = presAuth.GrantPresenceForPresentityGroupMembersL( *identity, *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForPresentityGroupMembersCalled, "GrantPresenceForPresentityGroupMembers was not called", EFalse, "GrantPresenceForPresentityGroupMembers was called" );

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk ); // allow events after reqcomplete

// WITHDRAW
    // register interest
    reqId = presAuth.WithdrawPresenceGrantFromPresentityGroupMembersL( *identity );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Granting presence for presentity group members complete with error: ") );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestWithdrawPresenceFromPresentityGroupMembersCalled, "WithdrawPresenceFromPresentityGroupMembers was not called", ETrue, NULL );

    CleanupStack::PopAndDestroy( 2, identity ); //pif, identity
    UnbindL();
    }

// ==============================================================================================

void T_PresenceAuthorization::T_GrantPresenceForGroupMembersChangeId_L()
    {
    EUNIT_PRINT( _L("Grant presence for presentity group members change id") );

// INITIALIZE

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

// ID CHANGE REQUEST TO ADAPTATION

    MXIMPIdentity* identityParam = context->ObjectFactory().NewIdentityLC();
    identityParam->SetIdentityL( KIdentity );
    
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
    CXIMPTestWaitHelper* wait = CXIMPTestWaitHelper::NewL();
    CleanupStack::PushL( wait );
    wait->WaitForL( 2 );
    CleanupStack::PopAndDestroy( wait );

// GRANT

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif =
                XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

    // register interest
    TXIMPRequestId reqId = presAuth.GrantPresenceForPresentityGroupMembersL( *identity, *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForPresentityGroupMembersCalled, "GrantPresenceForPresentityGroupMembers was not called", ETrue, NULL );

// DEINITIALIZE

    CleanupStack::PopAndDestroy( 2, identity ); //pif, identity
    UnbindL();
    }


// ==============================================================================================

void T_PresenceAuthorization::T_WithdrawNotGrantedPresenceFromEveryone_L()
    {
    EUNIT_PRINT( _L("Withdraw presence from everyone not granted") );

// INITIALIZE

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

// WITHDRAW

    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );
    // register interest
    TXIMPRequestId reqId = presAuth.WithdrawPresenceGrantFromEveryoneL();

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    // Changed for OPAA-73BCS8
    wrapper->VerifyEventStackL( _L8("Withdraw not granted everyone, complete with error: ") );
    COMMONASSERT( messenger, EXIMPPlgTestWithdrawPresenceFromEveryoneCalled, 
                        "WithdrawPresenceGrantFromEveryoneL was not called", ETrue /*EFalse*/, 
                        "WithdrawPresenceGrantFromEveryoneL was called" );

// DEINITIALIZE
    UnbindL();
    }


// ==============================================================================================

void T_PresenceAuthorization::T_WithdrawNotGrantedPresenceFromEveryone_Multiple_L()
    {
    EUNIT_PRINT( _L("Withdraw presence from everyone not granted multiple clients") );

    BindAllL();

    CPresenceInfoFilterImp* pif =
                XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

// GRANT ALL BUT FIRST

    TInt countOfWrappers = iWrapperMgr->WrapperCount();
    TInt a(0);
    for( a = countOfWrappers - 1; a >= 1; --a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Tell the protocol it should act normally
        messenger->SetNoError();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

        // register interest
        TXIMPRequestId reqId = presAuth.GrantPresenceForEveryoneL( *pif );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForEveryoneCalled,
            "GrantPresenceForEveryoneL was not called",
            ( a > 1 ) ? ETrue : EFalse,
            "GrantPresenceForEveryoneL was called" );
        }
        
// WITHDRAW
        
    for( a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();
        
        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();

        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk ); // allow events after reqcomplete

        // register interest
        TXIMPRequestId reqId = presAuth.WithdrawPresenceGrantFromEveryoneL();

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        wrapper->VerifyEventStackL( _L8("Subscribing presentity presence, complete with error: ") );
        // verify that UpdateInterest was called
        // Changed for OPAA-73BCS8
        COMMONASSERT( messenger, EXIMPPlgTestWithdrawPresenceFromEveryoneCalled, 
            "WithdrawPresenceGrantFromEveryoneL was not called",
            ( a < ( countOfWrappers -1 )/*a < 2^*/ ) ? EFalse : ETrue,
            "WithdrawPresenceGrantFromEveryoneL was called" );
        }
    
    CleanupStack::PopAndDestroy( pif );
        
    UnbindAllL();    
    }


// ==============================================================================================
void T_PresenceAuthorization::T_MakeAndGrantGroup_WithdrawOneID_L()
    {
    EUNIT_PRINT( _L("T_MakeAndGrantGroup_WithdrawOneID_L") );

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();
            
    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceAuthorization& presAuth = presFeat->PresenceAuthorization();
    MPresentityGroups& presGroup = presFeat->PresentityGroups();
    
    
    // CREATE PRESENTITY GROUP
    
    // Setup status event listener for RequestComplete
    // This will also produce HandlePresentityGroupListEvent, since the
    // the group is subscribed
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );


    MXIMPIdentity* groupId = context->ObjectFactory().NewIdentityLC();
    CleanupStack::Pop(); // groupId
    groupId->SetIdentityL( KGroupId );
    
    // subscribe
    TXIMPRequestId reqId;
    reqId = presGroup.CreatePresentityGroupL( *groupId, KGroupDisplayName );
    
    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("CreatePresentityGroupL failed, complete with error: ") );
    
    // verify that SubscribePresentityGroupListL was called
    messenger->AssertCreatePresentityGroupCalled( ETrue );

    
    // ADD PRESENTITY GROUP MEMBER 1
    
    CXIMPTestListener* listener4 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener4 );    // << listener4
    
    MXIMPIdentity* member = context->ObjectFactory().NewIdentityLC(); // << member
    member->SetIdentityL( KIdentity1_16 );


    // request complete
    TXIMPRequestId reqIdDummy;
    CXIMPRequestCompleteEventImp* evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );

    // group id
    CXIMPIdentityImp* identity2 = CXIMPIdentityImp::NewLC( groupId->Identity() );
    evReqComplete->AppendParamL( identity2 );
    CleanupStack::Pop(identity2); // identity2

    // id for member info
    CXIMPIdentityImp* miId = CXIMPIdentityImp::NewLC( KIdentity1_16 );
    // member info
    CPresentityGroupMemberInfoImp* memInfoImp =
        CPresentityGroupMemberInfoImp::NewLC( *miId, KIdentity1_16 );
    evReqComplete->AppendParamL( memInfoImp );
    CleanupStack::Pop(memInfoImp); // memInfoImp owned by AppendParamL
    CleanupStack::PopAndDestroy( miId ); // copy was made

    listener4->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // do the call
    reqId = presGroup.AddPresentityGroupMemberL(
            *groupId, *member, KIdentity1_16 );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == listener4->WaitAndAssertL(), "AddPresentityGroupMemberL (not subscribed) failed" );

    messenger->AssertAddPresentityGroupMemberCalled( ETrue );
    CleanupStack::PopAndDestroy(); // >>> member
    CleanupStack::PopAndDestroy( listener4 );// >>> listener4
    
    // ADD PRESENTITY GROUP MEMBER 2
    
    listener4 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener4 );    // << listener4
    
    member = context->ObjectFactory().NewIdentityLC(); // << member
    member->SetIdentityL( KIdentity2_16 );


    // request complete
    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );

    // group id
    //delete identity2;
    identity2 = CXIMPIdentityImp::NewLC( groupId->Identity() );
    evReqComplete->AppendParamL( identity2 );
    CleanupStack::Pop(identity2); // identity2

    // id for member info
    miId = CXIMPIdentityImp::NewLC( KIdentity2_16 );
    // member info
    memInfoImp = CPresentityGroupMemberInfoImp::NewLC( *miId, KIdentity2_16 );
    evReqComplete->AppendParamL( memInfoImp );
    CleanupStack::Pop(memInfoImp); // memInfoImp owned by AppendParamL
    CleanupStack::PopAndDestroy( miId ); // copy was made

    listener4->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // do the call
    reqId = presGroup.AddPresentityGroupMemberL( *groupId, *member, KIdentity2_16 );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == listener4->WaitAndAssertL(), "AddPresentityGroupMemberL (not subscribed) failed" );

    messenger->AssertAddPresentityGroupMemberCalled( ETrue );
    CleanupStack::PopAndDestroy(); // >>> member
    CleanupStack::PopAndDestroy( listener4 );// >>> listener4

    // ADD PRESENTITY GROUP MEMBER 3
    
    //delete identity2;
    listener4 = CXIMPTestListener::NewL( context );
    CleanupStack::PushL( listener4 );    // << listener4
    
    member = context->ObjectFactory().NewIdentityLC(); // << member
    member->SetIdentityL( KIdentity3_16 );


    // request complete
    evReqComplete = CXIMPRequestCompleteEventImp::NewLC( reqIdDummy );

    // group id
    identity2 = CXIMPIdentityImp::NewLC( groupId->Identity() );
    evReqComplete->AppendParamL( identity2 );
    CleanupStack::Pop(identity2); // identity2

    // id for member info
    miId = CXIMPIdentityImp::NewLC( KIdentity3_16 );
    // member info
    memInfoImp = CPresentityGroupMemberInfoImp::NewLC( *miId, KIdentity3_16 );
    evReqComplete->AppendParamL( memInfoImp );
    CleanupStack::Pop(memInfoImp); // memInfoImp owned by AppendParamL
    CleanupStack::PopAndDestroy( miId ); // copy was made

    listener4->ExpectL( evReqComplete );
    CleanupStack::Pop( evReqComplete );

    // do the call
    reqId = presGroup.AddPresentityGroupMemberL(
            *groupId, *member, KIdentity3_16 );

    // and wait
    EUNIT_ASSERT_DESC( KErrNone == listener4->WaitAndAssertL(), "AddPresentityGroupMemberL (not subscribed) failed" );

    messenger->AssertAddPresentityGroupMemberCalled( ETrue );
    CleanupStack::PopAndDestroy(); // >>> member
    CleanupStack::PopAndDestroy( listener4 );// >>> listener4


    // GRANT PRESENTITY GROUP MEMBER

    // This is the group we are interested in
    MXIMPIdentity* ggroup = context->ObjectFactory().NewIdentityLC();
    CleanupStack::Pop(); // ggroup
    ggroup->SetIdentityL( KGroupId );
    // Set filter for the group
    CPresenceInfoFilterImp* ggpif = CPresenceInfoFilterImp::NewLC();
    //ggpif->AcceptPersonFilterL( NPresenceInfo::NFieldType::KAcceptAll  );
    CleanupStack::Pop(ggpif); // ggpif

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // grant for all
    reqId = presAuth.GrantPresenceForPresentityGroupMembersL( *ggroup, *ggpif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestGrantPresenceForPresentityGroupMembersCalled, 
        "GrantPresenceForPresentityGroupMembersL was not called", ETrue, NULL );
    delete ggroup;
    delete ggpif;


    // Withdraw one of group's member's ID
    
    MXIMPIdentity* identity = context->ObjectFactory().NewIdentityLC();
    identity->SetIdentityL( KIdentity2_16 ); // << identity

    
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );
    // register interest
    reqId = presAuth.WithdrawPresenceGrantFromPresentityL( *identity );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Grantin presence to presentity, complete with error: ") );
    
    // Changed for OPAA-73BCS8
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestWithdrawPresenceFromPresentityCalled, 
                        "WithdrawPresenceForPresentityL was not called", ETrue/*EFalse*/, 
                        "WithdrawPresenceForPresentityL was called" );
                            
    CleanupStack::PopAndDestroy(); // identity
    delete groupId;

    UnbindL();
    
    wrapper = NULL;
    context = NULL;
    messenger = NULL;
    }


// ===========================================================================
// SETUP HELPERS FOR THE LISTENER
// ===========================================================================
//
void T_PresenceAuthorization::SetupListenerReqCompleteL( CXIMPTestContextWrapper* aWrapper, TXIMPTestStatusEventTemplate aEventType )
    {
    RArray< TInt32 > array;
    CleanupClosePushL( array );
    array.Append( MXIMPContextStateEvent::KInterfaceId );
    array.Append( MPresentityPresenceEvent::KInterfaceId );
    aWrapper->SetupListenerReqCompleteL( aEventType, &array );
    CleanupStack::PopAndDestroy(); // array
    }

void T_PresenceAuthorization::SetupListenerReqCompleteAllowEventsL(  CXIMPTestContextWrapper* aWrapper, TXIMPTestStatusEventTemplate aEventType )
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
// TEST CASE TABLE
// ===========================================================================
//
EUNIT_BEGIN_TEST_TABLE(
        T_PresenceAuthorization,
        "XIMP presence authorization tests",
        "MODULE" )

PRFW_DECORATED_TEST(
        "Grant presence for presentity",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_GrantPresenceForPresentity_L,
        Teardown )

        
PRFW_DECORATED_TEST(
        "MakeAndGrantGroup_WithdrawOneID",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_MakeAndGrantGroup_WithdrawOneID_L,
        Teardown )
        
        
PRFW_DECORATED_TEST(
        "Grant presence for presentity multiple",
        "",
        "",
        "FUNCTIONALITY",
        SetupMultiple_L,
        T_GrantPresenceForPresentity_Multiple_L,
        Teardown )


PRFW_DECORATED_TEST(
        "Withdraw not granted presence for presentity",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_WithdrawNotGrantedPresenceFromPresentity_L,
        Teardown )


PRFW_DECORATED_TEST(
        "Grant presence for presentity change id",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_GrantPresenceForPresentityChangeId_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Grant presence for group members change id",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_GrantPresenceForGroupMembersChangeId_L,
        Teardown )


PRFW_DECORATED_TEST(
        "Withdraw not granted presence from presentity multiple",
        "",
        "",
        "FUNCTIONALITY",
        SetupMultiple_L,
        T_WithdrawNotGrantedPresenceFromPresentity_Multiple_L,
        Teardown )

PRFW_DECORATED_TEST(
        "ErrorPlugin - Leave on grant",
        "",
        "",
        "ERRORHANDLING",
        Setup_L,
        T_ErrFromPlg_LeaveOnGrantPresenceForPresentity_L,
        Teardown )

PRFW_DECORATED_TEST(
        "ErrorPlugin - Leave on withdraw",
        "",
        "",
        "ERRORHANDLING",
        Setup_L,
        T_ErrFromPlg_LeaveOnWithdrawPresenceFromPresentity_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Withdraw not granted presence for group members",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_WithdrawNotGrantedPresenceFromGroupMembers_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Withdraw not granted presence for group members multiple",
        "",
        "",
        "FUNCTIONALITY",
        SetupMultiple_L,
        T_WithdrawNotGrantedPresenceFromGroupMembers_Multiple_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Grant presence for group members",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_GrantPresenceForGroupMembers_L,
        Teardown )


PRFW_DECORATED_TEST(
        "Grant presence for group members multiple",
        "",
        "",
        "FUNCTIONALITY",
        SetupMultiple_L,
        T_GrantPresenceForGroupMembers_Multiple_L,
        Teardown )


PRFW_DECORATED_TEST(
        "ErrorPlugin - Leave on withdraw for group members",
        "",
        "",
        "ERRORHANDLING",
        Setup_L,
        T_ErrFromPlg_LeaveOnGrantPresenceForGroupMembers_L,
        Teardown )

PRFW_DECORATED_TEST(
        "ErrorPlugin - Leave on withdraw from group members",
        "",
        "",
        "ERRORHANDLING",
        Setup_L,
        T_ErrFromPlg_LeaveOnWithdrawPresenceFromGroupMembers_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Grant presence for everyone",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_GrantPresenceForEveryone_L,
        Teardown )
        
PRFW_DECORATED_TEST(
        "ErrorPlugin - Leave on grant to everyone",
        "",
        "",
        "ERRORHANDLING",
        Setup_L,
        T_ErrFromPlg_LeaveOnGrantPresenceForEveryone_L,
        Teardown )

PRFW_DECORATED_TEST(
        "ErrorPlugin - Leave on withdraw from everyone",
        "",
        "",
        "ERRORHANDLING",
        Setup_L,
        T_ErrFromPlg_LeaveOnWithdrawPresenceFromEveryone_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Grant presence for everyone multiple ",
        "",
        "",
        "FUNCTIONALITY",
        SetupMultiple_L,
        T_GrantPresenceForEveryone_Multiple_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Withdraw not granted presence from everyone",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_WithdrawNotGrantedPresenceFromEveryone_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Withdraw not granted presence from everyone multiple",
        "",
        "",
        "FUNCTIONALITY",
        SetupMultiple_L,
        T_WithdrawNotGrantedPresenceFromEveryone_Multiple_L,
        Teardown )
        

EUNIT_END_TEST_TABLE


// ===========================================================================
// TEST SUITE CONSTRUCTION
// ===========================================================================
//
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    return T_PresenceAuthorization::NewL();
    }



// end of file
