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

#include <eunitmacros.h>
#include <ecom/ecom.h>
#include <badesca.h>
#include <e32base.h>

#include "t_sessionmng.h"
#include "prfwteststatuseventlistener.h"
#include "prfwtestmessaging.h"
#include "prfwtestmessenger.h"
#include "prfwtestcontextwrappermgr.h"
#include "prfwtestcontextwrapper.h"
#include "prfwtestfiletool.h"
#include "userafter.h"

#include "eunittools.h"
#include "prfwtestrobustnesstools.h"
#include "prfwtestprotocols.h"

#include <ximpclient.h>
#include <ximpcontext.h>
#include <ximpcontextstateevent.h>
#include <ximprequestcompleteevent.h>
#include <ximpstatus.h>
#include <ximpfeatureinfo.h>
#include <ximpserviceinfo.h>







// ============================ MEMBER FUNCTIONS ===============================

// CONSTRUCTION
T_SessionMng* T_SessionMng::NewL()
    {
    T_SessionMng* self = new( ELeave ) T_SessionMng;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


T_SessionMng::~T_SessionMng()
    {
    delete iWrapperMgr;
    
    CXIMPTestFileTool::CleanAllL();
    }


void T_SessionMng::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    CXIMPTestFileTool::CleanAllL();
    }


T_SessionMng::T_SessionMng()
    {
    }



// ===========================================================================
// TEST CASE SETUP & TEARDOWN METHODS
// ===========================================================================
//

void T_SessionMng::Setup_L()
    {
    PrfwTestRobustness::DoPreCleaning();

    // now we're using only one connection
    iWrapperMgr = CXIMPTestContextWrapperMgr::NewL();
    iWrapperMgr->CreateWrapperL();
    }


void T_SessionMng::Setup_2_L()
    {
    iWrapperMgr = CXIMPTestContextWrapperMgr::NewL();

    // two wrappers
    iWrapperMgr->CreateWrapperL();
    iWrapperMgr->CreateWrapperL();
    }


void T_SessionMng::Setup_3_L()
    {
    iWrapperMgr = CXIMPTestContextWrapperMgr::NewL();

    // three wrappers for unbalanced bind/share unbind/unshare test
    iWrapperMgr->CreateWrapperL();
    iWrapperMgr->CreateWrapperL();
    iWrapperMgr->CreateWrapperL();
    }


void T_SessionMng::Teardown()
    {
    delete iWrapperMgr;
    iWrapperMgr = NULL;

    REComSession::FinalClose();

    PrfwTestRobustness::DoPreCleaning();
    }



// ===========================================================================
// TEST CASE
// ===========================================================================
void T_SessionMng::T_Simple_Bind_Wait_Unbind_L()
    {
    EUNIT_PRINT( _L("Simple context Bind/Unbind test.") );
    EUNIT_PRINT( _L("Client side waits bind completion before issuing unbind.") );

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestStatusEventListener* eventListener = wrapper->GetEventListener();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    //Do bind, wait and verify events
    wrapper->SetupListenerL( EXIMPTestStatusEvents_BindingOk );
    
    messenger->SetPluginIndex( 0 );
    messenger->SetNoError();

    TXIMPRequestId reqId;
    reqId = BindContextToDefaultL( context );
    EUNIT_ASSERT( reqId != TXIMPRequestId::Null() );

    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Binding single context: ") );

    messenger->AssertOpenSessionCalled( ETrue );

    //Verify features availability
    MXIMPFeatureInfo* ctxFeats = context->GetContextFeaturesLC();
    EUNIT_ASSERT_DESC( ctxFeats->FeatureIds().MdcaCount() > 0, "No features from context" );
    CleanupStack::PopAndDestroy(); //ctxFeats

    //Do unbind, wait and verify events
    wrapper->SetupListenerL( EXIMPTestStatusEvents_UnbindingOk );
    messenger->SetNoError();
    reqId = context->UnbindL();
    EUNIT_ASSERT( reqId != TXIMPRequestId::Null() );

    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Unbinding single context: ") );

    messenger->AssertCloseSessionCalled( ETrue );
    messenger->AssertPluginDied( ETrue );
    }

// ===========================================================================
// TEST CASE
// ===========================================================================
void T_SessionMng::T_Settings_Bind_Wait_Unbind_L()
    {
    EUNIT_PRINT( _L("Context Bind with Settings id/Unbind test.") );
    EUNIT_PRINT( _L("Client side waits bind completion before issuing unbind.") );

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestStatusEventListener* eventListener = wrapper->GetEventListener();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    //Do bind, wait and verify events
    wrapper->SetupListenerL( EXIMPTestStatusEvents_BindingOk );
    
    messenger->SetPluginIndex( 0 );
    messenger->SetNoError();

    TXIMPRequestId reqId;
    TUid protocol = { K_PRFW_TST_PROTOCOL_1_IMPLEMENTATION_UID };
    reqId = context->BindToL( protocol, 0 );
    
    EUNIT_ASSERT( reqId != TXIMPRequestId::Null() );

    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Binding single context: ") );

    messenger->AssertOpenSessionCalled( ETrue );

    //Verify features availability
    MXIMPFeatureInfo* ctxFeats = context->GetContextFeaturesLC();
    EUNIT_ASSERT_DESC( ctxFeats->FeatureIds().MdcaCount() > 0, "No features from context" );
    CleanupStack::PopAndDestroy(); //ctxFeats

    //Do unbind, wait and verify events
    wrapper->SetupListenerL( EXIMPTestStatusEvents_UnbindingOk );
    messenger->SetNoError();
    reqId = context->UnbindL();
    EUNIT_ASSERT( reqId != TXIMPRequestId::Null() );

    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Unbinding single context: ") );

    messenger->AssertCloseSessionCalled( ETrue );
    messenger->AssertPluginDied( ETrue );
    }


// ===========================================================================
// TEST CASE
// ===========================================================================
void T_SessionMng::T_Simple_Bind_Wait_OOM_L()
    {
    EUNIT_PRINT( _L("Simple context Bind OOM test.") );
    EUNIT_PRINT( _L("Client side waits bind completion in alloc heaven.") );

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    //Do bind, wait and verify events
    wrapper->SetupListenerReqCompleteL( EXIMPTestStatusEvents_RequestOk );

    messenger->SetPluginIndex( 0 );
    messenger->SetNoError();

    TXIMPRequestId reqId;
    reqId = BindContextToDefaultL( context );
    EUNIT_ASSERT( reqId != TXIMPRequestId::Null() );

    wrapper->WaitRequestAndStackEvents( reqId );
    
    CXIMPTestStatusEventListener* eventListener = wrapper->GetEventListener();    
    TInt reqCompleteErr = eventListener->GetReqCompletionErrL( reqId );
    User::LeaveIfError( reqCompleteErr );
    
    wrapper->VerifyEventStackL( _L8("Binding single context in OOM: ") );
    }


// ===========================================================================
// TEST CASE
// ===========================================================================
void T_SessionMng::T_Simple_Bind_Wait_Unbind_WithReqCompleteEventFilter_L()
    {
    EUNIT_PRINT( _L("Simple context Bind/Unbind test.") );
    EUNIT_PRINT( _L("Client side waits bind completion before issuing unbind.") );
    EUNIT_PRINT( _L("Client event filter accepts only request completion events") );

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestStatusEventListener* eventListener = wrapper->GetEventListener();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    //Accept only ReqComplete events
    wrapper->SetupListenerReqCompleteL( EXIMPTestStatusEvents_RequestOk );
    
    messenger->SetPluginIndex( 0 );
    messenger->SetNoError();
    
    
    //Do bind, wait and verify events    
    TXIMPRequestId reqId = BindContextToDefaultL( context );

    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Binding single context, accepting only reqComplete events: ") );
    messenger->AssertOpenSessionCalled( ETrue );
    

    //Accept only ReqComplete events
    wrapper->SetupListenerReqCompleteL( EXIMPTestStatusEvents_RequestOk );
    messenger->SetNoError();

    //Do unbind, wait and verify events
    reqId = context->UnbindL();

    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Unbinding single context, accepting only reqComplete events: ") );

    messenger->AssertCloseSessionCalled( ETrue );
    }



// ===========================================================================
// TEST CASE
// ===========================================================================
//
void T_SessionMng::T_Simple_Bind_Wait_CloseHandle_L()
    {
    EUNIT_PRINT( _L("Simple context Bind/CloseHandle test.") );
    EUNIT_PRINT( _L("Client side waits bind completion before closing the handle.") );

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestStatusEventListener* eventListener = wrapper->GetEventListener();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    //Do bind, wait and verify events
    wrapper->SetupListenerL( EXIMPTestStatusEvents_BindingOk );

    messenger->SetPluginIndex( 0 );
    messenger->SetNoError();

    TXIMPRequestId reqId = BindContextToDefaultL( context );

    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Binding single context: ") );

    messenger->AssertOpenSessionCalled( ETrue );


    //Close handle and verify events
    wrapper->ClearListener();    
    messenger->SetNoError();
    wrapper->DeletePresenceContext();
    
    
    // we give 10 sec to plugin to die
    CUserAfter::AfterSecondsL( 10 );


    messenger->AssertCloseSessionCalled( ETrue );
    messenger->AssertPluginDied( ETrue );

    wrapper->VerifyEventStackL( _L8("Closing single context handle: ") );
    }



// ===========================================================================
// TEST CASE
// ===========================================================================
//
void T_SessionMng::T_Simple_Bind_Direct_Unbind_L()
    {
    EUNIT_PRINT( _L("Simple context Bind/Unbind test.") );
    EUNIT_PRINT( _L("Client side deletes context while the binding is still active.") );

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestStatusEventListener* eventListener = wrapper->GetEventListener();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    wrapper->SetupListenerL( EXIMPTestStatusEvents_BindingOk );
    wrapper->AppendToTraitsL( EXIMPTestStatusEvents_UnbindingOk );

    
    messenger->SetPluginIndex( 0 );
    messenger->SetNoError(); //For binding

    
    TXIMPRequestId bindReqId = BindContextToDefaultL( context );


    messenger->AssertOpenSessionCalled( ETrue );

    messenger->SetNoError(); // for unbinding
    TXIMPRequestId unbindReqId = context->UnbindL();

    wrapper->WaitRequestAndStackEvents( bindReqId );
    wrapper->WaitRequestAndStackEvents( unbindReqId );

    wrapper->VerifyEventStackL( _L8("Direct Bind/Unbind single context: ") );

    messenger->AssertCloseSessionCalled( ETrue );
    messenger->AssertPluginDied( ETrue );
    }




// ===========================================================================
// TEST CASE
// ===========================================================================
//
void T_SessionMng::T_Simple_Bind_Direct_CloseHandle_L()
    {
    EUNIT_PRINT( _L("Simple context Bind/Unbind test.") );
    EUNIT_PRINT( _L("Client side closes context handle while the binding is still active.") );

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestStatusEventListener* eventListener = wrapper->GetEventListener();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

   
    messenger->SetPluginIndex( 0 );
    messenger->SetNoError();   


    TXIMPRequestId reqId = BindContextToDefaultL( context );

    messenger->AssertOpenSessionCalled( ETrue );
    wrapper->DeletePresenceContext();

    // wait for plugin to die properly here
    CUserAfter::AfterSecondsL( 10 );

    messenger->AssertCloseSessionCalled( ETrue );
    messenger->AssertPluginDied( ETrue );
    }




// ===========================================================================
// TEST CASE
// ===========================================================================
//
void T_SessionMng::T_ErrFromPlg_LeaveOnOpenSession_L()
    {
    EUNIT_PRINT( _L("Error from plug-in in bind/unbind." ) );
    EUNIT_PRINT( _L("Plug-in leaves from OpenSessionL()." ) );


    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestStatusEventListener* eventListener = wrapper->GetEventListener();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        {
        wrapper->SetupListenerL( EXIMPTestStatusEvents_BindingFailKErrAbort );
        messenger->SetLeave( KErrAbort );
        messenger->SetPluginIndex( 0 );
        
        TXIMPRequestId reqId = BindContextToDefaultL( context );

        wrapper->WaitRequestAndStackEvents( reqId );
        wrapper->VerifyEventStackL( _L8("Binding single context: ") );

        //Verify that features are not available
        TRAPD( featLoadErr,
                MXIMPFeatureInfo* ctxFeats = context->GetContextFeaturesLC();
                CleanupStack::PopAndDestroy(); //ctxFeats
             );
        EUNIT_ASSERT_NOT_EQUALS( featLoadErr, KErrNone );

        // plugin should've been unloaded without calling CloseSession
        EUNIT_ASSERT_DESC( ! messenger->GetBoolean( EXIMPPlgTestCloseSessionCalled ), "CloseSession was called" );

        messenger->AssertPluginDied( ETrue );
        }
    }


// ===========================================================================
// TEST CASE:
// ===========================================================================
//
void T_SessionMng::T_ErrFromPlg_ErrorResultOnOpenSession_L()
    {
    EUNIT_PRINT( _L("Error from plug-in in bind/unbind") );
    EUNIT_PRINT( _L("Plug-in completes OpenSessionL() with error") );
    EUNIT_PRINT( _L("Clint ctx can be reused for second bind.") );

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestStatusEventListener* eventListener = wrapper->GetEventListener();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        {
        wrapper->SetupListenerL( EXIMPTestStatusEvents_BindingFailKErrAbort );

        messenger->SetError( KErrAbort );

        messenger->SetPluginIndex( 0 );

        TXIMPRequestId reqId = BindContextToDefaultL( context );

        wrapper->WaitRequestAndStackEvents( reqId );
        wrapper->VerifyEventStackL( _L8("Binding single context: ") );


        // plugin should've been unloaded without calling CloseSession
        EUNIT_ASSERT_DESC( ! messenger->GetBoolean( EXIMPPlgTestCloseSessionCalled ), "CloseSession was called" );
        messenger->AssertPluginDied( ETrue );
        }

    
    //It should be possible to reuse client ctx for second bind
        {
        //Do bind, wait and verify events
        wrapper->SetupListenerL( EXIMPTestStatusEvents_BindingOk );
        
        messenger->SetPluginIndex( 0 );
        messenger->SetNoError();

        TXIMPRequestId reqId;
        reqId = BindContextToDefaultL( context );
        EUNIT_ASSERT( reqId != TXIMPRequestId::Null() );

        wrapper->WaitRequestAndStackEvents( reqId );
        wrapper->VerifyEventStackL( _L8("Binding single context: ") );

        messenger->AssertOpenSessionCalled( ETrue );

        //Verify features availability
        MXIMPFeatureInfo* ctxFeats = context->GetContextFeaturesLC();
        EUNIT_ASSERT_DESC( ctxFeats->FeatureIds().MdcaCount() > 0, "No features from context" );
        CleanupStack::PopAndDestroy(); //ctxFeats

        //Do unbind, wait and verify events
        wrapper->SetupListenerL( EXIMPTestStatusEvents_UnbindingOk );
        messenger->SetNoError();
        reqId = context->UnbindL();
        EUNIT_ASSERT( reqId != TXIMPRequestId::Null() );

        wrapper->WaitRequestAndStackEvents( reqId );
        wrapper->VerifyEventStackL( _L8("Unbinding single context: ") );

        messenger->AssertCloseSessionCalled( ETrue );
        messenger->AssertPluginDied( ETrue );
        }    
    }


// ===========================================================================
// TEST CASE:
// Simple bind, bind error
// ===========================================================================
//
void T_SessionMng::T_ErrFromPlg_InvalidReqIdOnOpenSession_L()
    {
    EUNIT_PRINT( _L("Error from plug-in in bind/unbind") );
    EUNIT_PRINT( _L("Plug-in completes OpenSessionL() with faulty ReqId") );

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestStatusEventListener* eventListener = wrapper->GetEventListener();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        {
        wrapper->SetupListenerL( EXIMPTestStatusEvents_BindingFailInvalidReqId );
        messenger->SetBoolean( EXIMPTestPlgBehaveInvalidReqId );
        messenger->SetPluginIndex( 0 );
        
        TXIMPRequestId reqId = BindContextToDefaultL( context );

        wrapper->WaitRequestAndStackEvents( reqId );
        wrapper->VerifyEventStackL( _L8("Binding single context: ") );
        }

    }



// ===========================================================================
// TEST CASE:
// ===========================================================================
//
void T_SessionMng::T_ErrFromPlg_ErrorResultOnCloseSession_L()
    {
    EUNIT_PRINT( _L("Error from plug-in in bind/unbind") );
    EUNIT_PRINT( _L("Plug-in completes CloseSession() with error") );

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestStatusEventListener* eventListener = wrapper->GetEventListener();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    //Do bind, wait and verify events
    wrapper->SetupListenerL( EXIMPTestStatusEvents_BindingOk );

    messenger->SetNoError();

    messenger->SetPluginIndex( 0 );
    
    TXIMPRequestId reqId = BindContextToDefaultL( context );    


    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Binding single context: ") );

    messenger->AssertOpenSessionCalled( ETrue );

    //Do unbind, wait and verify events
    wrapper->SetupListenerL( EXIMPTestStatusEvents_UnbindingOk );

    messenger->SetError( KErrCouldNotDisconnect );
    reqId = context->UnbindL();

    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Unbinding single context: ") );

    messenger->AssertPluginDied( ETrue );
    }

  



// ===========================================================================
// TEST CASE
// ===========================================================================
//
void T_SessionMng::T_Multi_Bind_Unbind_WithWaits_L()
    {
    EUNIT_PRINT( _L("Multiple context Bind/Unbind test.") );
    EUNIT_PRINT( _L("Client performs multiple bind & unbinds for a separated context sharing a connection.") );

    TInt ix;

    // bind contexts
    for ( ix = 0; ix < 3; ix++ )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( ix );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestStatusEventListener* eventListener = wrapper->GetEventListener();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        if ( ix == 1 )
            {
            // fail the middle one
            wrapper->SetupListenerL( EXIMPTestStatusEvents_BindingFailKErrAbort );
            messenger->SetError( KErrAbort );   // for binding
            }
        else
            {
            wrapper->SetupListenerL( EXIMPTestStatusEvents_BindingOk );
            messenger->SetNoError();   // for binding
            }

        messenger->SetPluginIndex( ix );

        TUid protocol = { K_PRFW_TST_PROTOCOL_1_IMPLEMENTATION_UID };
        const TDesC16& instanceId = wrapper->InstanceId();
        TXIMPRequestId reqId = context->BindToL( protocol,
                                                _L("www.imps.no/wv"),
                                                _L("user"),
                                                _L("password"),
                                                instanceId,
                                                ix );

        wrapper->WaitRequestAndStackEvents( reqId );

        TBuf8<100> title;
        title.AppendFormat( _L8("Bind [%d]: "), ix );
        wrapper->VerifyEventStackL( title );

        switch ( ix )
            {
            case 0 :
            case 1 :
            case 2 :
                {
                messenger->AssertOpenSessionCalled( ETrue );
                break;
                }
            default :
                {
                break;
                }
            }
    }


    // now context has been bound.
    // one has failed, but two are okay. do 3 unbinds.
    for( ix = 0; ix < 3; ix++ )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( ix );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestStatusEventListener* eventListener = wrapper->GetEventListener();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        if ( ix == 1 )
            {
            // the middle one has failed
            TRAPD( err, context->UnbindL() );
            EUNIT_ASSERT_EQUALS( err, KErrNotReady );
            }

        else
            {
            // we expect all unbinds to be okay from client point of view
            wrapper->SetupListenerL( EXIMPTestStatusEvents_UnbindingOk );
            messenger->SetNoError();   // For unbinding

            TXIMPRequestId reqId = context->UnbindL();
            wrapper->WaitRequestAndStackEvents( reqId );

            TBuf8<100> title;
            title.AppendFormat( _L8("Unbind [%d]: "), ix );
            wrapper->VerifyEventStackL( title );
            messenger->AssertCloseSessionCalled( ETrue );
            }
        }
    }
    
    
TXIMPRequestId T_SessionMng::BindContextToDefaultL( MXIMPContext* aContext )
    {
    TUid protocol = { K_PRFW_TST_PROTOCOL_1_IMPLEMENTATION_UID };
    return aContext->BindToL( protocol,
                             _L("www.imps.no/wv"),
                             _L("user"),
                             _L("password"),
                             0 ); 
    }



// ===========================================================================
// TEST CASE TABLE
// ===========================================================================
//
EUNIT_BEGIN_TEST_TABLE(
        T_SessionMng,
        "XIMP session management tests",
        "MODULE" )


PRFW_NOT_DECORATED_TEST(
        "Simple bind wait unbind",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_Simple_Bind_Wait_Unbind_L,
        Teardown )

 PRFW_NOT_DECORATED_TEST(
        "Settings bind wait unbind",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_Settings_Bind_Wait_Unbind_L,
        Teardown )

EUNIT_ALLOC_TEST(
        "Simple bind in OOM",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_Simple_Bind_Wait_OOM_L,
        Teardown )


PRFW_NOT_DECORATED_TEST(
        "Simple bind wait unbind with reqComplete event filter",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_Simple_Bind_Wait_Unbind_WithReqCompleteEventFilter_L,
        Teardown )

PRFW_NOT_DECORATED_TEST(
        "Simple bind wait close handle",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_Simple_Bind_Wait_CloseHandle_L,
        Teardown )


PRFW_NOT_DECORATED_TEST(
        "Simple bind direct unbind",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_Simple_Bind_Direct_Unbind_L,
        Teardown )


PRFW_NOT_DECORATED_TEST(
        "Simple bind direct close handle",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_Simple_Bind_Direct_CloseHandle_L,
        Teardown )


// opensession errors
PRFW_NOT_DECORATED_TEST(
        "ErrFromPlg leave OpenSession",
        "",
        "",
        "ERROHANDLING",
        Setup_L,
        T_ErrFromPlg_LeaveOnOpenSession_L,
        Teardown )

PRFW_NOT_DECORATED_TEST(
        "ErrFromPlg OpenSession",
        "",
        "",
        "ERROHANDLING",
        Setup_L,
        T_ErrFromPlg_ErrorResultOnOpenSession_L,
        Teardown )



PRFW_NOT_DECORATED_TEST(
        "ErrFromPlg Invalid ReqId",
        "",
        "",
        "ERROHANDLING",
        Setup_L,
        T_ErrFromPlg_InvalidReqIdOnOpenSession_L,
        Teardown )


// closesession errors
PRFW_NOT_DECORATED_TEST(
        "ErrFromPlg CloseSession",
        "",
        "",
        "ERROHANDLING",
        Setup_L,
        T_ErrFromPlg_ErrorResultOnCloseSession_L,
        Teardown )


// multiple bind + unbind tests
PRFW_NOT_DECORATED_TEST(
        "Multiple context Bind/Unbind test.",
        "",
        "",
        "ERROHANDLING",
        Setup_3_L,
        T_Multi_Bind_Unbind_WithWaits_L,
        Teardown )


EUNIT_END_TEST_TABLE


// ===========================================================================
// TEST SUITE CONSTRUCTION
// ===========================================================================
//
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    return T_SessionMng::NewL();
    }


// end of file






