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
#include <ximprequestcompleteevent.h>
#include <ximpstatus.h>
#include <presencefeatures.h>
#include <presencewatching.h>
#include <presencepublishing.h>
#include <presentitypresenceevent.h>
#include <ximpobjectfactory.h>

#include "t_presencewatching.h"
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
#include "presenceinfofieldimp.h"
#include "presenceinfofieldcollectionimp.h"
#include "presenceinfofieldvaluetextimp.h"
#include "personpresenceinfoimp.h"

#include <presencecachereader.h>

// For the protocol UID
#include "prfwtestprotocols.h"


_LIT8( KPrefix1, "PreA_" );
_LIT8( KPrefix2, "PreB_" );
_LIT8( KPrefix3, "PreC_" );
_LIT8( KPrefix4, "PreD_" );

_LIT( KIdentity, "TestId@hello.world" );
_LIT8( KIdentity8, "TestId@hello.world" );

_LIT( KText, "Hello World" );


const TUid KTestProtocolUid = { K_PRFW_TST_PROTOCOL_1_IMPLEMENTATION_UID };  // hardcoded...

// ============================ MEMBER FUNCTIONS ===============================

// CONSTRUCTION
T_PresenceWatching* T_PresenceWatching::NewL()
    {
    T_PresenceWatching* self = new( ELeave ) T_PresenceWatching;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


T_PresenceWatching::~T_PresenceWatching()
    {
    }


void T_PresenceWatching::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


T_PresenceWatching::T_PresenceWatching()
    {
    }



// ===========================================================================
// TEST CASE SETUP & TEARDOWN METHODS
// ===========================================================================
//

void T_PresenceWatching::Setup_L()
    {
    __UHEAP_MARK;
    PrfwTestRobustness::DoPreCleaning();

    iWrapperMgr = CXIMPTestContextWrapperMgr::NewL();
    iWrapperMgr->CreateWrapperL();
    
    iMPresenceCacheReader = MPresenceCacheReader::NewL();
    }

void T_PresenceWatching::SetupMultiple_L()
    {
    PrfwTestRobustness::DoPreCleaning();

    iWrapperMgr = CXIMPTestContextWrapperMgr::NewL();
    iWrapperMgr->CreateWrapperL();
    iWrapperMgr->CreateWrapperL();
    iWrapperMgr->CreateWrapperL();
    }


void T_PresenceWatching::BindL()
    {
    CXIMPTestContextWrapper* wrapper0 = iWrapperMgr->GetWrapperL( 0 );

    wrapper0->BindL( 0 );
    }

void T_PresenceWatching::BindAllL()
    {
    for( TInt a = 0; a < iWrapperMgr->WrapperCount(); ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        wrapper->BindL( 0 );
        }
    }


void T_PresenceWatching::Teardown()
    {
    delete iWrapperMgr;
    iWrapperMgr = NULL;
    
    delete iMPresenceCacheReader;

    REComSession::FinalClose();

    PrfwTestRobustness::DoPreCleaning();
    __UHEAP_MARKEND;
    }

void T_PresenceWatching::UnbindL()
    {
    // Do unbind
    CXIMPTestContextWrapper* wrapper0 = iWrapperMgr->GetWrapperL( 0 );
    wrapper0->UnbindL();
    }

void T_PresenceWatching::UnbindAllL()
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
void T_PresenceWatching::T_SubscribePresentityPresence_Single_L()
    {
    EUNIT_PRINT( _L("Single presentity subscription") );

// BIND AND SETUP

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceWatching& presWatch = presFeat->PresenceWatching();

// SUBSCRIBE
    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif = XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );
    
    // register interest
    TXIMPRequestId reqId = presWatch.SubscribePresentityPresenceL( *identity, *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Subscribing presentity presence, complete with error: ") );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresentityPresesenceCalled, "SubscribePresentityPresenceL was not called", ETrue, NULL );


// REFRESH
    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );
    messenger->SetValueFor( EXIMPPlgTestSubscribePresentityPresesenceCalled, 0 );

    // register interest
    reqId = presWatch.SubscribePresentityPresenceL( *identity, *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Subscribing presentity presence, complete with error: ") );
    
    // Wait for event because refresh happened
    SetupListenerReqCompleteL( wrapper, EXIMPTestStatusEvents_HandlePresentityPresencePending );
    // Wait for events
    wrapper->WaitAnyEvent();
    wrapper->VerifyEventStackL( _L8("Handle presence watcher list. Wrong event occurred.") );
    
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresentityPresesenceCalled, 
                 "SubscribePresentityPresenceL was not called", EFalse,
                 "SubscribePresentityPresenceL was called" );

// UNSUBSCRIBE

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk ); // allow events after reqcomplete

    // register interest
    reqId = presWatch.UnsubscribePresentityPresenceL( *identity );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("UnSubscribing presentity presence, complete with error: ") );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresentityPresesenceCalled, "UnsubscribePresentityPresenceL was not called", ETrue, NULL );

// UNBIND    

    CleanupStack::PopAndDestroy( pif );
    CleanupStack::PopAndDestroy( identity );
    
    UnbindL();
    }

// Single client, empty filter, presence cache checking
void T_PresenceWatching::T_SubscribeHandlePresentityPresence_Single_L()
    {
    EUNIT_PRINT( _L("Single presentity subscription") );

// BIND AND SETUP

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceWatching& presWatch = presFeat->PresenceWatching();

// SUBSCRIBE
    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KIdentity8 );
    CPresenceInfoFilterImp* pif = XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );
    
    // register interest
    TXIMPRequestId reqId = presWatch.SubscribePresentityPresenceL( *identity, *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Subscribing presentity presence, complete with error: ") );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresentityPresesenceCalled, "SubscribePresentityPresenceL was not called", ETrue, NULL );

// SEND DATA
    
    // Create some meaningful data
    CPresenceInfoImp* presenceInfo = CPresenceInfoImp::NewLC();

    MPresenceInfoFieldValueText* value = CPresenceInfoFieldValueTextImp::NewLC();
    value->SetTextValueL(KText);
    
    MPresenceInfoField* infoField = CPresenceInfoFieldImp::NewLC();
    infoField->SetFieldTypeL(NPresenceInfo::NFieldType::KStatusMessage);
    infoField->SetFieldValue(value);    
    
    MPersonPresenceInfo* personPres = CPersonPresenceInfoImp::NewLC();
    MPresenceInfoFieldCollection& fieldCol = personPres->Fields();
    fieldCol.AddOrReplaceFieldL(infoField);
    TInt fieldCount= fieldCol.FieldCount();
        
    presenceInfo->SetPersonPresenceL(personPres);
    CleanupStack::Pop(3); // value, infoField, personPres
    
    // Pack the data for FileTool
    HBufC8* packedInfo = TXIMPObjectPacker< CPresenceInfoImp >::PackL( *presenceInfo );
    CleanupStack::PushL( packedInfo );
    HBufC8* packedId = TXIMPObjectPacker< CXIMPIdentityImp >::PackL( *identity );
    CleanupStack::PushL( packedId );
    RPointerArray< HBufC8 > bufferArray;
    CleanupClosePushL( bufferArray );
    bufferArray.AppendL( packedId );
    bufferArray.AppendL( packedInfo );
    HBufC8* packedBufferArray = TXIMPHBuf8Packer::PackArrayL( bufferArray );
    CleanupStack::PopAndDestroy( 3, packedInfo ); // packedId, packedInfo, bufferArray
    CleanupStack::PushL( packedBufferArray );
    
    // Use filetool to send it for plugin.
    CXIMPTestFileSrvMsg* srvMsg = CXIMPTestFileSrvMsg::NewL( CXIMPTestFileSrvMsg::ETestSrvMsgPresentityPresence, 
                                                             *packedBufferArray );
    CleanupStack::PopAndDestroy( packedBufferArray );
    CleanupStack::PushL( srvMsg );
    wrapper->FileTool().SrvMsgStoreL( srvMsg );
    CleanupStack::PopAndDestroy( srvMsg );
    
    // Wait for event
    SetupListenerReqCompleteL( wrapper, EXIMPTestStatusEvents_HandlePresentityPresence );

    // Wait for events
    wrapper->WaitAnyEvent();
    wrapper->VerifyEventStackL( _L8("Handle presentity presence. Wrong event occurred.") );
    
    // Verify the cache
    MPresenceInfo* presInfoRead = iMPresenceCacheReader->PresenceInfoLC(*identity);

    CPresenceInfoImp* readInfo = 
        TXIMPGetImpClassOrPanic<CPresenceInfoImp>::From(*presInfoRead);    


    EUNIT_ASSERT( presenceInfo->EqualsContent(*readInfo) );
    
    CleanupStack::PopAndDestroy( 2 ); // readInfo, presenceInfo
            
        
// UNSUBSCRIBE

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk ); // allow events after reqcomplete

    // unsubscribe
    reqId = presWatch.UnsubscribePresentityPresenceL( *identity );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("UnSubscribing presentity presence, complete with error: ") );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresentityPresesenceCalled, "UnsubscribePresentityPresenceL was not called", ETrue, NULL );

// UNBIND    

    CleanupStack::PopAndDestroy( pif );
    CleanupStack::PopAndDestroy( identity );
    
    UnbindL();
    }


// Single client, empty filter
void T_PresenceWatching::T_SubscribePresentityPresenceChangeId_Single_L()
    {
    EUNIT_PRINT( _L("Single presentity subscription") );

// BIND AND SETUP

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceWatching& presWatch = presFeat->PresenceWatching();

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

// SUBSCRIBE
    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif = XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );
    
    // register interest
    TXIMPRequestId reqId = presWatch.SubscribePresentityPresenceL( *identity, *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Subscribing presentity presence, complete with error: ") );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresentityPresesenceCalled, "SubscribePresentityPresenceL was not called", ETrue, NULL );

// UNBIND    

    CleanupStack::PopAndDestroy( pif );
    CleanupStack::PopAndDestroy( identity );
    
    UnbindL();
    }


void T_PresenceWatching::T_UnsubscribeNotSubscribedPresentityPresence_Single_L()
    {
    EUNIT_PRINT( _L("Unsubscribe not subscribed presentity presence") );

// BIND AND SETUP

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceWatching& presWatch = presFeat->PresenceWatching();
    
    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif = XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );
    
// UNSUBSCRIBE

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk ); // allow events after reqcomplete

    // register interest
    TXIMPRequestId reqId = presWatch.UnsubscribePresentityPresenceL( *identity );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("UnSubscribing presentity presence, complete with error: ") );

    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresentityPresesenceCalled, "UnsubscribePresentityPresenceL was not called", EFalse, 
                                                                                "UnsbuscribePresentityPresenceL was called." );

// UNBIND    

    CleanupStack::PopAndDestroy( pif );
    CleanupStack::PopAndDestroy( identity );
    
    UnbindL();
    }

void T_PresenceWatching::T_UnsubscribeNotSubscribedPresentityPresence_Multiple_L()
    {
    EUNIT_PRINT( _L("Multiple client filter registration") );

    BindAllL();

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif = XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

    TInt countOfWrappers = iWrapperMgr->WrapperCount();
    for( TInt a = countOfWrappers - 1; a >= 1 ; --a ) // Don't subscribe the first one.
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Tell the protocol it should act normally
        messenger->SetNoError();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceWatching& presWatch = presFeat->PresenceWatching();

        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

        // register interest
        TXIMPRequestId reqId = presWatch.SubscribePresentityPresenceL( *identity, *pif );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestSubscribePresentityPresesenceCalled, 
            "SubscribePresentityPresenceL was not called",
            ( a > 1 ) ? ETrue : EFalse,
            "SubscribePresentityPresenceL was called" );
        }

    for( TInt a = 0; a < countOfWrappers; ++a ) // Try to unsubscribe first one first.
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceWatching& presWatch = presFeat->PresenceWatching();
        
        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk ); // allow events after reqcomplete

        // register interest
        TXIMPRequestId reqId = presWatch.UnsubscribePresentityPresenceL( *identity );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        wrapper->VerifyEventStackL( _L8("Subscribing presentity presence, complete with error: ") );
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresentityPresesenceCalled, 
            "UnsubscribePresentityPresenceL was not called",
            ( a < 2 ) ? EFalse : ETrue,
            "UnsubscribePresentityPresenceL was called" );
        }
    
    CleanupStack::PopAndDestroy( pif );
    CleanupStack::PopAndDestroy( identity );
        
    UnbindAllL();    
    }


void T_PresenceWatching::T_ErrFromPlg_LeaveOnSubscribePresentityPresence_L()
    {
    EUNIT_PRINT( _L("Error from plug-in in presentity presence subscribe") );
    EUNIT_PRINT( _L("Plug-in leaves from SubscribePresence().") );

// BIND AND SETUP

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    messenger->SetLeave( KErrAbort );

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceWatching& presWatch = presFeat->PresenceWatching();
    
// SUBSCRIBE

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_PresentitySubscribeFailKErrAbort );

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif = XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );
    
    // register interest
    TXIMPRequestId reqId = presWatch.SubscribePresentityPresenceL( *identity, *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Subscribing presentity presence, complete with error: ") );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresentityPresesenceCalled, "SubscribePresentityPresenceL was not called", EFalse, "SubscribePresentityPresenceL was called" );
    
// UNBIND

    CleanupStack::PopAndDestroy( pif );
    CleanupStack::PopAndDestroy( identity );

    messenger->SetLeave( KErrNone );

    UnbindL();
    }


void T_PresenceWatching::T_ErrFromPlg_LeaveOnUnsubscribePresentityPresence_L()
    {
    EUNIT_PRINT( _L("Error from plug-in in presentity presence subscribe") );
    EUNIT_PRINT( _L("Plug-in leaves from SubscribePresence().") );

// BIND AND SETUP

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceWatching& presWatch = presFeat->PresenceWatching();

// SUBSCRIBE

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif = XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );
    
    // register interest
    TXIMPRequestId reqId = presWatch.SubscribePresentityPresenceL( *identity, *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Subscribing presentity presence, complete with error: ") );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresentityPresesenceCalled, "SubscribePresentityPresenceL was not called", ETrue, NULL );

// UNSUBSCRIBE

    messenger->SetLeave( KErrAbort );

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_PresentitySubscribeFailKErrAbort ); // allow events after reqcomplete

    // register interest
    reqId = presWatch.UnsubscribePresentityPresenceL( *identity );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("UnSubscribing presentity presence, complete with error: ") );

    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresentityPresesenceCalled, "UnsubscribePresentityPresenceL was not called", EFalse, 
                                                                                "UnsbuscribePresentityPresenceL was called." );
    
// UNBIND

    CleanupStack::PopAndDestroy( pif );
    CleanupStack::PopAndDestroy( identity );

    messenger->SetLeave( KErrNone );

    UnbindL();
    }


// Multiple client, empty filter
void T_PresenceWatching::T_SubscribePresentityPresence_Multiple_L()
    {
    EUNIT_PRINT( _L("Multiple client filter registration") );

    BindAllL();

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif = XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

    TInt countOfWrappers = iWrapperMgr->WrapperCount();
    for( TInt a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Tell the protocol it should act normally
        messenger->SetNoError();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceWatching& presWatch = presFeat->PresenceWatching();

        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

        // register interest
        TXIMPRequestId reqId = presWatch.SubscribePresentityPresenceL( *identity, *pif );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestSubscribePresentityPresesenceCalled, 
            "SubscribePresentityPresenceL was not called",
            ( a == 0 ) ? ETrue : EFalse,
            "SubscribePresentityPresenceL was called" );
        }
        
    for( TInt a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();
        
        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceWatching& presWatch = presFeat->PresenceWatching();
                
        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );
        messenger->SetValueFor( EXIMPPlgTestSubscribePresentityPresesenceCalled, 0 );

        // register interest
        TXIMPRequestId reqId = presWatch.SubscribePresentityPresenceL( *identity, *pif );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        wrapper->VerifyEventStackL( _L8("Subscribing presentity presence, complete with error: ") );

        // Wait for event because refresh happened
        SetupListenerReqCompleteL( wrapper, EXIMPTestStatusEvents_HandlePresentityPresencePending );
        // Wait for events
        wrapper->WaitAnyEvent();
        wrapper->VerifyEventStackL( _L8("Handle presence watcher list. Wrong event occurred.") );

        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestSubscribePresentityPresesenceCalled, 
                     "SubscribePresentityPresenceL was not called", EFalse,
                     "SubscribePresentityPresenceL was called" );
        }
        
        
    for( TInt a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();
        
        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceWatching& presWatch = presFeat->PresenceWatching();

        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk ); // allow events after reqcomplete

        // register interest
        TXIMPRequestId reqId = presWatch.UnsubscribePresentityPresenceL( *identity );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        wrapper->VerifyEventStackL( _L8("Subscribing presentity presence, complete with error: ") );
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresentityPresesenceCalled, 
            "UnsubscribePresentityPresenceL was not called",
            ( a < 2 ) ? EFalse : ETrue,
            "UnsubscribePresentityPresenceL was called" );
        }
    
    CleanupStack::PopAndDestroy( pif );
    CleanupStack::PopAndDestroy( identity );
    
    
    UnbindAllL();
    }


// ===========================================================================
// TEST CASES for presentity group members presence subscription
// ===========================================================================

// Single client, empty filter
void T_PresenceWatching::T_SubscribePresentityGroupMembersPresence_Single_L()
    {
    EUNIT_PRINT( _L("Single presentity group members subscription") );

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceWatching& presWatch = presFeat->PresenceWatching();
    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif = XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

// SUBSCRIBE

    // register interest
    TXIMPRequestId reqId = presWatch.SubscribePresentityGroupMembersPresenceL( *identity, *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresentityGroupMembersPresesenceCalled, "SubscribePresentityGroupMembersPresenceL was not called", ETrue, NULL );

// REFRESH

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );
    messenger->SetValueFor( EXIMPPlgTestSubscribePresentityGroupMembersPresesenceCalled, 0 );

    // register interest
    reqId = presWatch.SubscribePresentityGroupMembersPresenceL( *identity, *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresentityGroupMembersPresesenceCalled, 
                 "SubscribePresentityPresenceL was not called", EFalse,
                 "SubscribePresentityPresenceL was called" );

// UNSUBSCRIBE

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk ); // allow events after reqcomplete


    // register interest
    reqId = presWatch.UnsubscribePresentityGroupMembersPresenceL( *identity );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Subscribing presentity group members presence, complete with error: ") );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresentityGroupMembersPresesenceCalled, "UnsubscribePresentityGroupMembersPresenceL was not called", ETrue, NULL );

    
    CleanupStack::PopAndDestroy( pif );
    CleanupStack::PopAndDestroy( identity );
    
    UnbindL();
    }

void T_PresenceWatching::T_UnsubscribeNotSubscribedPresentityGroupMembersPresence_Single_L()
    {
    EUNIT_PRINT( _L("Unsubscribe not subscribed presentity group members presence") );

// BIND AND SETUP

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceWatching& presWatch = presFeat->PresenceWatching();    
    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif = XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );
    
// UNSUBSCRIBE

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk ); // allow events after reqcomplete

    // register interest
    TXIMPRequestId reqId = presWatch.UnsubscribePresentityGroupMembersPresenceL( *identity );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("UnSubscribing presentity presence, complete with error: ") );

    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresentityGroupMembersPresesenceCalled, "UnsubscribePresentityGroupMembersPresenceL was not called", EFalse, 
                                                                                            "UnsubscribePresentityGroupMembersPresenceL was called." );

// UNBIND    

    CleanupStack::PopAndDestroy( pif );
    CleanupStack::PopAndDestroy( identity );
    
    UnbindL();
    }


void T_PresenceWatching::T_UnsubscribeNotSubscribedPresentityGroupMembersPresence_Multiple_L()
    {
    EUNIT_PRINT( _L("Multiple client filter registration") );

    BindAllL();

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif = XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

    TInt countOfWrappers = iWrapperMgr->WrapperCount();
    for( TInt a = countOfWrappers - 1; a >= 1; --a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Tell the protocol it should act normally
        messenger->SetNoError();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceWatching& presWatch = presFeat->PresenceWatching();
    
        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

        // register interest
        TXIMPRequestId reqId = presWatch.SubscribePresentityGroupMembersPresenceL( *identity, *pif );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestSubscribePresentityGroupMembersPresesenceCalled,
            "SubscribePresentityPresenceL was not called",
            ( a > 1 ) ? ETrue : EFalse,
            "SubscribePresentityPresenceL was called" );
        }
        
    for( TInt a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceWatching& presWatch = presFeat->PresenceWatching();
    
        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk ); // allow events after reqcomplete

        // register interest
        TXIMPRequestId reqId = presWatch.UnsubscribePresentityGroupMembersPresenceL( *identity );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        wrapper->VerifyEventStackL( _L8("Subscribing presentity presence, complete with error: ") );
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresentityGroupMembersPresesenceCalled, 
            "UnsubscribePresentityPresenceL was not called",
            ( a < 2 ) ? EFalse : ETrue,
            "UnsubscribePresentityPresenceL was called" );
        }
    
    CleanupStack::PopAndDestroy( pif );
    CleanupStack::PopAndDestroy( identity );
        
    UnbindAllL();    
    }



// Multiple client, empty filter
void T_PresenceWatching::T_SubscribeGroupMembersPresence_Multiple_L()
    {
    EUNIT_PRINT( _L("Multiple client filter registration") );

    BindAllL();

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif = XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );

    TInt countOfWrappers = iWrapperMgr->WrapperCount();
    for( TInt a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();

        // Tell the protocol it should act normally
        messenger->SetNoError();

        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceWatching& presWatch = presFeat->PresenceWatching();

        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

        // register interest
        TXIMPRequestId reqId = presWatch.SubscribePresentityGroupMembersPresenceL( *identity, *pif );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestSubscribePresentityGroupMembersPresesenceCalled, 
            "SubscribePresentityPresenceL was not called",
            ( a == 0 ) ? ETrue : EFalse,
            "SubscribePresentityPresenceL was called" );
        }
        
    for( TInt a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();
        
        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceWatching& presWatch = presFeat->PresenceWatching();  
                      
        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );
        messenger->SetValueFor( EXIMPPlgTestSubscribePresentityGroupMembersPresesenceCalled, 0 );

        // register interest
        TXIMPRequestId reqId = presWatch.SubscribePresentityGroupMembersPresenceL( *identity, *pif );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        wrapper->VerifyEventStackL( _L8("Subscribing presentity presence, complete with error: ") );

        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestSubscribePresentityGroupMembersPresesenceCalled, 
                     "SubscribePresentityPresenceL was not called", EFalse,
                     "SubscribePresentityPresenceL was called" );
        }
        
        
    for( TInt a = 0; a < countOfWrappers; ++a )
        {
        CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( a );
        MXIMPContext* context = wrapper->GetContext();
        CXIMPTestMessenger* messenger = wrapper->GetMessenger();
    
        // Get the interfaces
        MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
        MPresenceWatching& presWatch = presFeat->PresenceWatching();
    
        // Setup status event listener for RequestComplete
        SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk ); // allow events after reqcomplete

        // register interest
        TXIMPRequestId reqId = presWatch.UnsubscribePresentityGroupMembersPresenceL( *identity );

        // Wait for events on the request
        wrapper->WaitRequestAndStackEvents( reqId );
        wrapper->VerifyEventStackL( _L8("Subscribing presentity presence, complete with error: ") );
        // verify that UpdateInterest was called
        COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresentityGroupMembersPresesenceCalled, 
            "UnsubscribePresentityPresenceL was not called",
            ( a < 2 ) ? EFalse : ETrue,
            "UnsubscribePresentityPresenceL was called" );
        }
    
    CleanupStack::PopAndDestroy( pif );
    CleanupStack::PopAndDestroy( identity );
    
    
    UnbindAllL();
    }


void T_PresenceWatching::T_ErrFromPlg_LeaveOnSubscribePresentityGroupMembersPresence_L()
    {
    EUNIT_PRINT( _L("Error from plug-in in presentity group members presence subscribe") );
    EUNIT_PRINT( _L("Plug-in leaves from SubscribeGroupMembersPresence().") );

// BIND AND SETUP

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    messenger->SetLeave( KErrAbort );

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceWatching& presWatch = presFeat->PresenceWatching();

// SUBSCRIBE

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_PresentitySubscribeFailKErrAbort );

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif = XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );
    
    // register interest
    TXIMPRequestId reqId = presWatch.SubscribePresentityGroupMembersPresenceL( *identity, *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Subscribing presentity presence, complete with error: ") );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresentityGroupMembersPresesenceCalled, "SubscribePresentityPresenceL was not called", EFalse, "SubscribePresentityPresenceL was called" );
    
// UNBIND

    CleanupStack::PopAndDestroy( pif );
    CleanupStack::PopAndDestroy( identity );

    messenger->SetLeave( KErrNone );

    UnbindL();
    }


void T_PresenceWatching::T_ErrFromPlg_LeaveOnUnsubscribePresentityGroupMembersPresence_L()
    {
    EUNIT_PRINT( _L("Error from plug-in in presentity group members presence unsubscribe") );
    EUNIT_PRINT( _L("Plug-in leaves from UnsubscribeGroupMembersPresence().") );

// BIND AND SETUP

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

     // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceWatching& presWatch = presFeat->PresenceWatching();

// SUBSCRIBE

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif = XIMPTestPresenceDataUtils::GenerateInfoFilterLC( &KPrefix1 );
    
    // register interest
    TXIMPRequestId reqId = presWatch.SubscribePresentityGroupMembersPresenceL( *identity, *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Subscribing presentity presence, complete with error: ") );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresentityGroupMembersPresesenceCalled, "SubscribePresentityPresenceL was not called", ETrue, "SubscribePresentityPresenceL was called" );

// UNSUBSCRIBE

    messenger->SetLeave( KErrAbort );

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_PresentitySubscribeFailKErrAbort ); // allow events after reqcomplete

    // register interest
    reqId = presWatch.UnsubscribePresentityGroupMembersPresenceL( *identity );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("UnSubscribing presentity presence, complete with error: ") );

    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresentityGroupMembersPresesenceCalled, "UnsubscribePresentityGroupMembersPresenceL was not called", EFalse, 
                                                                                            "UnsubscribePresentityGroupMembersPresenceL was called." );

// UNBIND

    CleanupStack::PopAndDestroy( pif );
    CleanupStack::PopAndDestroy( identity );

    messenger->SetLeave( KErrNone );

    UnbindL();
    }


void T_PresenceWatching::T_ContentChangePresentityGroupMembersPresence_Single_L()
    {
    EUNIT_PRINT( _L("Presentity group members content change") );

    BindL();

    CXIMPTestContextWrapper* wrapper = iWrapperMgr->GetWrapperL( 0 );
    MXIMPContext* context = wrapper->GetContext();
    CXIMPTestMessenger* messenger = wrapper->GetMessenger();

    // Tell the protocol it should act normally
    messenger->SetNoError();

    // Get the interfaces
    MPresenceFeatures* presFeat = wrapper->GetPresenceFeatures();
    MPresenceWatching& presWatch = presFeat->PresenceWatching();
    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    CXIMPIdentityImp* identity = XIMPTestPresenceDataUtils::GenerateIdentityLC( &KNullDesC8 );
    CPresenceInfoFilterImp* pif = CPresenceInfoFilterImp::NewLC();


    // register interest
    TXIMPRequestId reqId = presWatch.SubscribePresentityGroupMembersPresenceL( *identity, *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestSubscribePresentityGroupMembersPresesenceCalled, "SubscribePresentityGroupMembersPresenceL was not called", ETrue, NULL );


// TODO THINGSIES
    // Get the interface
    MPresencePublishing& presPub = presFeat->PresencePublishing();

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk );

    // register interest
    reqId = presPub.SubscribeOwnPresenceL( *pif );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );



    // Create member to grouplist we are interested in.
    // 1. Create notification    
    RXIMPObjOwningPtrArray<CPresentityGroupMemberInfoImp> groupMemberList;
    CleanupClosePushL( groupMemberList );
    CPresentityGroupMemberInfoImp* info = CPresentityGroupMemberInfoImp::NewLC( *identity, KNullDesC );
    User::LeaveIfError( groupMemberList.Append( info ) );
    CleanupStack::Pop(); //info
    HBufC8* packedId = TXIMPObjectPacker< CXIMPIdentityImp >::PackL( *identity );
    CleanupStack::PushL( packedId );
    HBufC8* packedArray = TXIMPObjectPacker<CPresentityGroupMemberInfoImp>::PackArrayL( groupMemberList );
    CleanupStack::PushL( packedArray );    
    RPointerArray< HBufC8 > bufferArray;
    CleanupClosePushL( bufferArray );
    bufferArray.AppendL( packedId );
    bufferArray.AppendL( packedArray );
    HBufC8* packedBufferArray = TXIMPHBuf8Packer::PackArrayL( bufferArray );
    CleanupStack::PopAndDestroy( 4 ); // packedId, bufferArray, packedArray, groupMemberList
    CleanupStack::PushL( packedBufferArray );
    
    // 2. Use filetool to send it for plugin.
    CXIMPTestFileSrvMsg* srvMsg = CXIMPTestFileSrvMsg::NewL( CXIMPTestFileSrvMsg::ETestSrvMsgPresentityGroupMemberList, 
                                                             *packedBufferArray );
    CleanupStack::PopAndDestroy( packedBufferArray );
    CleanupStack::PushL( srvMsg );
    wrapper->FileTool().CleanAllL();
    wrapper->FileTool().SrvMsgStoreL( srvMsg );
    CleanupStack::PopAndDestroy( srvMsg );

    // 3. Send data to member
    CPresenceInfoImp* presenceInfo = CPresenceInfoImp::NewLC();
    HBufC8* packedInfo = TXIMPObjectPacker< CPresenceInfoImp >::PackL( *presenceInfo );
    CleanupStack::PopAndDestroy( presenceInfo );
    CleanupStack::PushL( packedInfo );
    packedId = TXIMPObjectPacker< CXIMPIdentityImp >::PackL( *identity );
    CleanupStack::PushL( packedId );
    CleanupClosePushL( bufferArray );
    bufferArray.AppendL( packedId );
    bufferArray.AppendL( packedInfo );
    packedBufferArray = TXIMPHBuf8Packer::PackArrayL( bufferArray );
    CleanupStack::PopAndDestroy( 3, packedInfo ); // packedId, packedInfo
    CleanupStack::PushL( packedBufferArray );
    
    // 4. Use filetool to send it for plugin.
    srvMsg = CXIMPTestFileSrvMsg::NewL( CXIMPTestFileSrvMsg::ETestSrvMsgPresentityPresence, 
                                                             *packedBufferArray );
    CleanupStack::PopAndDestroy( packedBufferArray );
    CleanupStack::PushL( srvMsg );
    wrapper->FileTool().SrvMsgStoreL( srvMsg );
    CleanupStack::PopAndDestroy( srvMsg );
    
    // Wait for event
    // 5. Wait for event.
    SetupListenerReqCompleteL( wrapper, EXIMPTestStatusEvents_HandlePresentityPresence );
    // Wait for events
    wrapper->WaitAnyEvent();
    wrapper->VerifyEventStackL( _L8("Handle presence watcher list. Wrong event occurred.") );
        
        
    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk ); // allow events after reqcomplete

    // register interest
    reqId = presPub.UnsubscribeOwnPresenceL();

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Subscribing own presence, complete with error: ") );
        
// TODO THINGSIES

    // Setup status event listener for RequestComplete
    SetupListenerReqCompleteAllowEventsL( wrapper, EXIMPTestStatusEvents_RequestOk ); // allow events after reqcomplete

    // register interest
    reqId = presWatch.UnsubscribePresentityGroupMembersPresenceL( *identity );

    // Wait for events on the request
    wrapper->WaitRequestAndStackEvents( reqId );
    wrapper->VerifyEventStackL( _L8("Subscribing presentity group members presence, complete with error: ") );
    // verify that UpdateInterest was called
    COMMONASSERT( messenger, EXIMPPlgTestUnsubscribePresentityGroupMembersPresesenceCalled, "UnsubscribePresentityGroupMembersPresenceL was not called", ETrue, NULL );

    CleanupStack::PopAndDestroy( 2, identity ); //pif, identity
    
    UnbindL();
    }


// ===========================================================================
// SETUP HELPERS FOR THE LISTENER
// ===========================================================================
//
void T_PresenceWatching::SetupListenerReqCompleteL( CXIMPTestContextWrapper* aWrapper, TXIMPTestStatusEventTemplate aEventType )
    {
    RArray< TInt32 > array;
    CleanupClosePushL( array );
    array.Append( MXIMPContextStateEvent::KInterfaceId );
    array.Append( MPresentityPresenceEvent::KInterfaceId );
    aWrapper->SetupListenerReqCompleteL( aEventType, &array );
    CleanupStack::PopAndDestroy(); // array
    }

void T_PresenceWatching::SetupListenerReqCompleteAllowEventsL(  CXIMPTestContextWrapper* aWrapper, TXIMPTestStatusEventTemplate aEventType )
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
        T_PresenceWatching,
        "XIMP presence watching tests",
        "MODULE" )

PRFW_DECORATED_TEST(
        "Subscribe single presence",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_SubscribePresentityPresence_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Subscribe single presence and handle the presence data with cache checking",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_SubscribeHandlePresentityPresence_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Subscribe single presence change id",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_SubscribePresentityPresenceChangeId_Single_L,
        Teardown )


PRFW_DECORATED_TEST(
        "Unsubscribe single presence",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_UnsubscribeNotSubscribedPresentityPresence_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Unsubscribe multiple presence",
        "",
        "",
        "FUNCTIONALITY",
        SetupMultiple_L,
        T_UnsubscribeNotSubscribedPresentityPresence_Multiple_L,
        Teardown )


PRFW_DECORATED_TEST(
        "Error From Plugin -> Leave when presentity subscribe is called",
        "",
        "",
        "ERROHANDLING",
        Setup_L,
        T_ErrFromPlg_LeaveOnSubscribePresentityPresence_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Error From Plugin -> Leave when presentity unsubscribe is called",
        "",
        "",
        "ERROHANDLING",
        Setup_L,
        T_ErrFromPlg_LeaveOnUnsubscribePresentityPresence_L,
        Teardown )


PRFW_DECORATED_TEST(
        "Subscribe multiple presence",
        "",
        "",
        "FUNCTIONALITY",
        SetupMultiple_L,
        T_SubscribePresentityPresence_Multiple_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Subscribe group members single presence",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_SubscribePresentityGroupMembersPresence_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Unsubscribe single group members presence",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_UnsubscribeNotSubscribedPresentityGroupMembersPresence_Single_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Unsubscribe multiple group members presence",
        "",
        "",
        "FUNCTIONALITY",
        SetupMultiple_L,
        T_UnsubscribeNotSubscribedPresentityGroupMembersPresence_Multiple_L,
        Teardown )


PRFW_DECORATED_TEST(
        "Subscribe multiple presence for group members",
        "",
        "",
        "FUNCTIONALITY",
        SetupMultiple_L,
        T_SubscribeGroupMembersPresence_Multiple_L,
        Teardown )

        
PRFW_DECORATED_TEST(
        "Error From Plugin -> Leave when presentity group members subscribe is called",
        "",
        "",
        "ERROHANDLING",
        Setup_L,
        T_ErrFromPlg_LeaveOnSubscribePresentityGroupMembersPresence_L,
        Teardown )

PRFW_DECORATED_TEST(
        "Error From Plugin -> Leave when presentity group members unsubscribe is called",
        "",
        "",
        "ERROHANDLING",
        Setup_L,
        T_ErrFromPlg_LeaveOnUnsubscribePresentityGroupMembersPresence_L,
        Teardown )
        

PRFW_DECORATED_TEST(
        "Handle group members single presence",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_ContentChangePresentityGroupMembersPresence_Single_L,
        Teardown )


EUNIT_END_TEST_TABLE


// ===========================================================================
// TEST SUITE CONSTRUCTION
// ===========================================================================
//
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    return T_PresenceWatching::NewL();
    }


// end of file
