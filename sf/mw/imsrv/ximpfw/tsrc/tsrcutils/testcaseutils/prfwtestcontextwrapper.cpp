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
* Description:  Testing context wrapper
 *
*/



#include <e32std.h>

#include <eunitmacros.h>

#include "prfwtestcontextwrapper.h"
#include <ximpclient.h>
#include <ximpcontext.h>
#include <ximpcontextstateevent.h>
#include <ximprequestcompleteevent.h>
#include <ximpstatus.h>
#include <presencefeatures.h>


#include "prfwteststatuseventlistener.h"
#include "prfwtestmessenger.h"
#include "prfwtestmessaging.h"
#include "prfwtestfiletool.h"
#include "prfwtestprotocols.h"

_LIT( KInstanceAsNumber, "%d" );
// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// Length of number in characters (e.g. 501 --> 3)
// ---------------------------------------------------------------------------
//
TInt NumLenInChars( TInt aNum )
    {
    TInt len = 0;
    TInt tmp = aNum;
    do
        {
        tmp /= 10;  // number is base-10
        len++;
        }
    while ( tmp > 0 );
    return len;
    }


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Instance id
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC16& CXIMPTestContextWrapper::InstanceId() const
    {
    return *iInstance;
    }


// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
CXIMPTestContextWrapper::CXIMPTestContextWrapper( TInt aIndex,
        MXIMPClient* aClient ) :
    iIndex( aIndex ),
    iClient( aClient )
    {
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
void CXIMPTestContextWrapper::ConstructL()
    {
    iContext = iClient->NewPresenceContextLC();
    CleanupStack::Pop(); // iContext
    iListener = CXIMPTestStatusEventListener::NewL( 20, iContext );
    iMessenger = CXIMPTestMessenger::NewForClientSideL( iIndex );
    iPresFeatures = MPresenceFeatures::NewL(iContext); 
    TInt instanceLength = NumLenInChars( iIndex );
    iInstance = HBufC::NewL( instanceLength );
    iInstance->Des().AppendFormat( KInstanceAsNumber, iIndex );
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPTestContextWrapper* CXIMPTestContextWrapper::NewL(
        TInt aIndex, MXIMPClient* aClient )
    {
    CXIMPTestContextWrapper* self = new( ELeave ) CXIMPTestContextWrapper( aIndex, aClient );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
CXIMPTestContextWrapper::~CXIMPTestContextWrapper()
    {
    iStatusEventTraits.Close();

    delete iMessenger;
    delete iListener;
    delete iContext;
    delete iFileTool;
    delete iInstance;
    delete iPresFeatures;
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C MXIMPContext* CXIMPTestContextWrapper::GetContext()
    {
    return iContext;
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C MPresenceFeatures* CXIMPTestContextWrapper::GetPresenceFeatures()
    {
    return iPresFeatures;
    }
// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPTestStatusEventListener* CXIMPTestContextWrapper::GetEventListener()
    {
    return iListener;
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPTestMessenger* CXIMPTestContextWrapper::GetMessenger()
    {
    return iMessenger;
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C MXIMPClient* CXIMPTestContextWrapper::GetClient()
    {
    return iClient;
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPTestContextWrapper::DeletePresenceContext()
    {
    // used to test abrupt handle closings
    iListener->SetEventSourceL( NULL );
    delete iContext;
    iContext = NULL;
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPTestContextWrapper::SetupEmptyListener()
    {
    iListener->ResetEventStack();
    iStatusEventTraits.Reset();
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C RArray<TXIMPTestStatusEventSnapshot>* CXIMPTestContextWrapper::GetStatusTraits()
    {
    return &iStatusEventTraits;
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPTestContextWrapper::SetupListenerL( TXIMPTestStatusEventTemplate aEventType )
    {
    ResetListener();
    iListener->GetEventTemplateL( iStatusEventTraits, aEventType );
    iMessenger->SetPluginIndex( iIndex );
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPTestContextWrapper::AppendToTraitsL( TXIMPTestStatusEventTemplate aEventType )
    {
    iListener->GetEventTemplateL( iStatusEventTraits, aEventType );
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPTestContextWrapper::SetupListenerReqCompleteL(
        TXIMPTestStatusEventTemplate aEventType,
        RArray< TInt32 >* aAdditionalEvents )
    {
    iListener->ResetEventStack();
    iListener->AcceptedEventTypes().Reset();
    iListener->AcceptedEventTypes().AppendL( MXIMPRequestCompleteEvent::KInterfaceId );

    if ( aAdditionalEvents )
        {
        for ( TInt i = 0; i < aAdditionalEvents->Count(); i++ )
            {
            iListener->AcceptedEventTypes().AppendL( (*aAdditionalEvents)[ i ] );
            }
        }

    iListener->ReRegisterEventFilterL();
    iStatusEventTraits.Reset();
    iListener->GetEventTemplateL( iStatusEventTraits, aEventType );

    iMessenger->SetPluginIndex( iIndex );
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPTestContextWrapper::ClearListener()
    {
    iListener->ResetEventStack();
    iListener->SetEventSourceL( NULL );

    iStatusEventTraits.Reset();
    }


// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPTestContextWrapper::ResetListener()
    {
    iListener->ResetEventStack();
    iListener->ResetEventTypeStack();
    iStatusEventTraits.Reset();
    iListener->ReRegisterEventFilterL();
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPTestContextWrapper::WaitRequestAndStackEvents( TXIMPRequestId aReqId )
    {
    iListener->WaitRequestAndStackEvents( aReqId );
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPTestContextWrapper::VerifyEventStackL( const TPtrC8& aDesc )
    {
    iListener->VerifyEventStackL( iStatusEventTraits.Array(), aDesc );
    }

// ---------------------------------------------------------------------------
// CXIMPTestContextWrapper::WaitAnyEvent
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPTestContextWrapper::WaitAnyEvent()
    {
    iListener->WaitAnyEvent();
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPTestContextWrapper::BindL(
        TUid aUid,
        const TDesC& aServer,
        const TDesC& aUser,
        const TDesC& aPassword )
    {
    SetupListenerL( EXIMPTestStatusEvents_BindingOk );
    iMessenger->SetNoError();
    iMessenger->SetPluginIndex( iIndex );

    delete iFileTool;
    iFileTool = NULL;
    iFileTool = CXIMPTestFileTool::NewL( aUid.iUid, iIndex );

    TXIMPRequestId reqId = iContext->BindToL(
            aUid,
            aServer,
            aUser,
            aPassword,
            *iInstance,
            iIndex );

    WaitRequestAndStackEvents( reqId );
    VerifyEventStackL( _L8("Binding (wrapper BindL): ") );

    EUNIT_ASSERT_DESC( iMessenger->GetBoolean( EXIMPPlgTestOpenSessionCalled ) ||
                       iMessenger->GetBoolean( EXIMPPlgTestShareSessionCalled ), "OpenSessionL was not called" );
    }

// ---------------------------------------------------------------------------
// Bind alternative
// ---------------------------------------------------------------------------
//
const TUid KTestProtocolUid = { K_PRFW_TST_PROTOCOL_1_IMPLEMENTATION_UID };  // hardcoded...

_LIT( KTestFakeServer,      "www.imps%d.no/wv" );
_LIT( KTestFakeUser,        "user%d" );
_LIT( KTestFakePassword,    "password%d" );

EXPORT_C void CXIMPTestContextWrapper::BindL( TInt aId )
    {
    SetupListenerL( EXIMPTestStatusEvents_BindingOk );
    iMessenger->SetNoError();
    iMessenger->SetPluginIndex( aId );

    // create fake info
    TInt extraLength = 8; // bad style... but length is enough

    HBufC* fakeSrv = HBufC::NewLC( KTestFakeServer().Length() + extraLength );
    fakeSrv->Des().AppendFormat( KTestFakeServer, aId );

    HBufC* fakeUser = HBufC::NewLC( KTestFakeUser().Length() + extraLength );
    fakeUser->Des().AppendFormat( KTestFakeUser, aId );

    HBufC* fakePass = HBufC::NewLC( KTestFakePassword().Length() + extraLength );
    fakePass->Des().AppendFormat( KTestFakePassword, aId );

    delete iFileTool;
    iFileTool = NULL;
    iFileTool = CXIMPTestFileTool::NewL( KTestProtocolUid.iUid, iIndex );

    TXIMPRequestId reqId = iContext->BindToL(
            KTestProtocolUid,
            *fakeSrv,
            *fakeUser,
            *fakePass,
            *iInstance,
            aId );

    CleanupStack::PopAndDestroy( 3, fakeSrv );

    WaitRequestAndStackEvents( reqId );
    VerifyEventStackL( _L8("Binding (wrapper BindL): ") );

    EUNIT_ASSERT_DESC( iMessenger->GetBoolean( EXIMPPlgTestOpenSessionCalled ) ||
                       iMessenger->GetBoolean( EXIMPPlgTestShareSessionCalled ), "OpenSessionL was not called" );
    }

  
    
 ///NEW BIND   
 EXPORT_C void CXIMPTestContextWrapper::BindWithSettingsL( TInt aId )
    {
    SetupListenerL( EXIMPTestStatusEvents_BindingOk );
    iMessenger->SetNoError();
    iMessenger->SetPluginIndex( 0/*aId*/ );

    // create fake info
    TInt extraLength = 8; // bad style... but length is enough

    HBufC* fakeSrv = HBufC::NewLC( KTestFakeServer().Length() + extraLength );
    fakeSrv->Des().AppendFormat( KTestFakeServer, aId );

    HBufC* fakeUser = HBufC::NewLC( KTestFakeUser().Length() + extraLength );
    fakeUser->Des().AppendFormat( KTestFakeUser, aId );

    HBufC* fakePass = HBufC::NewLC( KTestFakePassword().Length() + extraLength );
    fakePass->Des().AppendFormat( KTestFakePassword, aId );

    delete iFileTool;
    iFileTool = NULL;
    iFileTool = CXIMPTestFileTool::NewL( KTestProtocolUid.iUid, iIndex );

    TXIMPRequestId reqId = iContext->BindToL(
            KTestProtocolUid, aId );

    CleanupStack::PopAndDestroy( 3, fakeSrv );

    WaitRequestAndStackEvents( reqId );
    VerifyEventStackL( _L8("Binding (wrapper BindWithSettingsL): ") );

    EUNIT_ASSERT_DESC( iMessenger->GetBoolean( EXIMPPlgTestOpenSessionCalled ) ||
                       iMessenger->GetBoolean( EXIMPPlgTestShareSessionCalled ), "OpenSessionL was not called" );
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPTestContextWrapper::UnbindL()
    {
    SetupListenerL( EXIMPTestStatusEvents_UnbindingOk );
    iMessenger->SetNoError();
    TXIMPRequestId reqId = iContext->UnbindL();

    WaitRequestAndStackEvents( reqId );
    VerifyEventStackL( _L8("Unbinding (wrapper UnbindL): ") );

    EUNIT_ASSERT_DESC( iMessenger->GetBoolean( EXIMPPlgTestCloseSessionCalled ) ||
                       iMessenger->GetBoolean( EXIMPPlgTestUnshareSessionCalled ) , "CloseSession was not called" );
    if( iMessenger->GetBoolean( EXIMPPlgTestCloseSessionCalled ) )
        {
        DeletePresenceContext();
        //EUNIT_ASSERT_DESC( iMessenger->GetBoolean( EXIMPPlgTestDeath ), "Plugin did not die, destructor not called.");
        }
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPTestFileTool& CXIMPTestContextWrapper::FileTool() const
    {
    return *iFileTool;
    }

// End of file
