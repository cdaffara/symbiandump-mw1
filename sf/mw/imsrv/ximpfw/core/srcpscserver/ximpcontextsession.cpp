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
* Description:  Context session.
 *
*/

#include "ximpcontextsession.h"
#include "ximpsrvmessage.h"
#include "ximpserverdefs.h"

#include "ximpcontexteventqueue.h"
#include "ximpcontexteventfilter.h"
#include "ximphostmanager.h"
#include "ximpglobals.h"
#include "ximppsccontextimp.h"

#include "ximpfeatureinfoimp.h"
#include "ximpobjecthelpers.h"
#include "ximppanics.h"
#include "ximptrace.h"
#include "ximpoperationdefs.h"
#include "ximpoperationbase.h"

#include "ximphoststates.h"

#include <ximpbase.h>
#include <e32base.h>

using namespace NXIMPSrv;



// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CXIMPContextSession::NewL()
// ---------------------------------------------------------------------------
//
CXIMPContextSession* CXIMPContextSession::NewL( TUint32 aSessionId )
    {
    CXIMPContextSession* self = new( ELeave ) CXIMPContextSession();
    CleanupStack::PushL( self );
    self->ConstructL( aSessionId );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPContextSession::~CXIMPContextSession()
// ---------------------------------------------------------------------------
//
CXIMPContextSession::~CXIMPContextSession()
    {
    TRACE_1( _L("CXIMPContextSession[%d]::~CXIMPContextSession()"), this );

    delete iPreparedData;

    if( iEventListenScout )
        {
        iEventListenScout->Complete( KErrCancel );
        delete iEventListenScout;
        }

    iClientData->EventQueue().StopConsuming();

    // Unbind when client has died, and unbind not send.
    CXIMPOperationBase* operation = iClientData->GetCachedUnbind();

    if( operation )
        {
        TRAP_IGNORE(
            {
            CleanupStack::PushL( operation );
            iClientData->AppendToOpQueueL( operation );
            CleanupStack::Pop( operation );
            } );
        }

    iClientData->Close();
    }


// ---------------------------------------------------------------------------
// CXIMPContextSession::CXIMPContextSession()
// ---------------------------------------------------------------------------
//
CXIMPContextSession::CXIMPContextSession()
    {
    TRACE_1( _L("CXIMPContextSession[%d]::CXIMPContextSession()"), this );
    }


// -----------------------------------------------------------------------------
// CXIMPContextSession::ConstructL()
// -----------------------------------------------------------------------------
//
void CXIMPContextSession::ConstructL( TUint32 aSessionId )
    {
    iClientData = CXIMPPscContext::NewL( aSessionId );
    }


// -----------------------------------------------------------------------------
// CXIMPContextSession::TryHandleMessageL()
// From MXIMPSrvSession
// -----------------------------------------------------------------------------
//
void CXIMPContextSession::TryHandleMessageL( MXIMPSrvMessage& aMessage )
    {
    switch( aMessage.Function() )
        {
        //Asynchronous context operations related
        case NRequest::ECtxSsQueueOperation:
            {
            DoQueueOperationL( aMessage );
            break;
            }
        //Asynchronous context operations related
        case NRequest::ECtxSsQueueBindOperation:
            {
            DoQueueOperationL( aMessage );
            break;
            }

        //Event handling related
        case NRequest::ECtxSsInstallEventFilter:
            {
            DoInstallEventFilterL( aMessage );
            break;
            }

        case NRequest::ECtxSsSetEventListenScout:
            {
            DoSetEventListenScoutL( aMessage );
            break;
            }

        case NRequest::ECtxSsCancelEventListenScout:
            {
            DoCancelEventListenScout( aMessage );
            break;
            }

        case NRequest::ECtxSsFetchTopEventDataSize:
            {
            DoFetchTopEventDataSizeL( aMessage );
            break;
            }

        case NRequest::ECtxSsFetchTopEventData:
            {
            DoFetchTopEventDataL( aMessage );
            break;
            }

        case NRequest::ECtxSsFetchTopEventReqId:
            {
            DoFetchTopEventReqIdL( aMessage );
            break;
            }

        case NRequest::ECtxSsDropTopEvent:
            {
            DoDropTopEventL( aMessage );
            break;
            }

        //Synchronous context data access
        case NRequest::ECtxSsPrepareContextFeatures:
            {
            DoPrepareContextFeaturesL( aMessage );
            break;
            }

        case NRequest::ECtxSsGetPreparedData:
            {
            DoGetPreparedDataL( aMessage );
            break;
            }

        default:
            {
            //Unknown request type
            //Nothing to do here.
            }
        }
    }


// -----------------------------------------------------------------------------
// CXIMPContextSession::NewEventAvailable()
// From MXIMPEventQueueObserver
// -----------------------------------------------------------------------------
//
void CXIMPContextSession::NewEventAvailable()
    {
    if( iEventListenScout )
        {
        iEventListenScout->Complete( KErrNone );
        delete iEventListenScout;
        iEventListenScout = NULL;
        }
    }


// -----------------------------------------------------------------------------
// CXIMPContextSession::DoQueueOperationL()
// -----------------------------------------------------------------------------
//
void CXIMPContextSession::DoQueueOperationL(
    MXIMPSrvMessage& aMessage )
    {
    const TInt operationType = aMessage.Int( MXIMPSrvMessage::Ep0 );

    TPckgBuf< TXIMPRequestId > reqIdBuf;

    // We can take ready unbind from context if it exists.
    CXIMPOperationBase* operation = NULL;
    if( operationType == NXIMPOps::EXIMPUnbindContext )
        {
        operation = iClientData->GetCachedUnbind();
        // ownership transferred
        }

    if( operation )
        {
        // found an unbind, so put it to the queue to be processed
        CleanupStack::PushL( operation );
        iClientData->AppendToOpQueueL( operation );
        reqIdBuf() = operation->RequestId();
        CleanupStack::Pop( operation );
        }
    else
        {
        const TInt pckSize = aMessage.GetDesLengthL( MXIMPSrvMessage::Ep1 );
        HBufC8* paramPck = HBufC8::NewLC( pckSize );
        TPtr8 paramPckPtr = paramPck->Des();
        aMessage.ReadL( MXIMPSrvMessage::Ep1, paramPckPtr );
        reqIdBuf() = iClientData->AddNewOperationL( operationType,
                *paramPck );
        CleanupStack::PopAndDestroy( paramPck );
        }

    aMessage.WriteL( MXIMPSrvMessage::Ep2, reqIdBuf );
    aMessage.Complete( KErrNone );
    }


// -----------------------------------------------------------------------------
// CXIMPContextSession::DoInstallEventFilterL()
// -----------------------------------------------------------------------------
//
void CXIMPContextSession::DoInstallEventFilterL(
    MXIMPSrvMessage& aMessage )
    {
    const TInt pckSize = aMessage.GetDesLengthL( MXIMPSrvMessage::Ep0 );
    HBufC8* filterPck = HBufC8::NewLC( pckSize );
    TPtr8 filterPckPtr( filterPck->Des() );
    aMessage.ReadL( MXIMPSrvMessage::Ep0, filterPckPtr );

    CXIMPContextEventFilter* newFilter = CXIMPContextEventFilter::NewLC();
    TXIMPObjectPacker< CXIMPContextEventFilter >::UnPackL( *newFilter,
                                                                   *filterPck );

    iClientData->EventQueue().SetEventFilter( *newFilter );
    CleanupStack::Pop( newFilter );
    CleanupStack::PopAndDestroy( filterPck );

    aMessage.Complete( KErrNone );
    }


// -----------------------------------------------------------------------------
// CXIMPContextSession::DoSetEventListenScoutL()
// -----------------------------------------------------------------------------
//
void CXIMPContextSession::DoSetEventListenScoutL(
    MXIMPSrvMessage& aMessage )
    {
    __ASSERT_ALWAYS( !iEventListenScout,
                     aMessage.PanicClientAndLeaveL(
                     NXIMPPanic::EOnSrvAlreadyListeningEvents ) );

    iClientData->EventQueue().StartConsuming( *this );

    //If there is already a events in que,
    //signal client right a way
    if( iClientData->EventQueue().HasElements() )
        {
        aMessage.Complete( KErrNone );
        }
    else
        {
        aMessage.PlaceOwnershipHere( iEventListenScout );
        }
    }


// -----------------------------------------------------------------------------
// CXIMPContextSession::DoCancelEventListenScout()
// -----------------------------------------------------------------------------
//
void CXIMPContextSession::DoCancelEventListenScout(
    MXIMPSrvMessage& aMessage )
    {
    if( iEventListenScout )
        {
        iEventListenScout->Complete( KErrCancel );
        delete iEventListenScout;
        iEventListenScout = NULL;
        }

    iClientData->EventQueue().StopConsuming();
    aMessage.Complete( KErrNone );
    }


// -----------------------------------------------------------------------------
// CXIMPContextSession::DoFetchTopEventDataSizeL()
// -----------------------------------------------------------------------------
//
void CXIMPContextSession::DoFetchTopEventDataSizeL(
    MXIMPSrvMessage&  aMessage )
    {
    iClientData->EventQueue().SelectTopEventIfNeededL();
    aMessage.Complete( iClientData->EventQueue().TopEventDataL().Size() );
    }


// -----------------------------------------------------------------------------
// CXIMPContextSession::DoFetchTopEventDataL()
// -----------------------------------------------------------------------------
//
void CXIMPContextSession::DoFetchTopEventDataL(
    MXIMPSrvMessage& aMessage )
    {
    iClientData->EventQueue().SelectTopEventIfNeededL();
    aMessage.WriteL( MXIMPSrvMessage::Ep0, iClientData->EventQueue().TopEventDataL() );
    aMessage.Complete( KErrNone );
    }


// -----------------------------------------------------------------------------
// CXIMPContextSession::DoFetchTopEventReqIdL()
// -----------------------------------------------------------------------------
//
void CXIMPContextSession::DoFetchTopEventReqIdL(
    MXIMPSrvMessage& aMessage )
    {
    TPckgBuf< TXIMPRequestId > reqIdBuf;
    
    iClientData->EventQueue().SelectTopEventIfNeededL();
    reqIdBuf() = iClientData->EventQueue().TopEventReqIdL();

    aMessage.WriteL( MXIMPSrvMessage::Ep0, reqIdBuf );
    aMessage.Complete( KErrNone );
    }


// -----------------------------------------------------------------------------
// CXIMPContextSession::DoDropTopEventL()
// -----------------------------------------------------------------------------
//
void CXIMPContextSession::DoDropTopEventL(
    MXIMPSrvMessage& aMessage )
    {
    iClientData->EventQueue().DropTopEvent();
    aMessage.Complete( KErrNone );
    }


// -----------------------------------------------------------------------------
// CXIMPContextSession::DoPrepareContextFeaturesL()
// -----------------------------------------------------------------------------
//
void CXIMPContextSession::DoPrepareContextFeaturesL(
    MXIMPSrvMessage& aMessage )
    {
    __ASSERT_ALWAYS( !iPreparedData,
                     aMessage.PanicClientAndLeaveL(
                     NXIMPPanic::EOnSrvAlreadyHavingPrepearedData ) );


    CXIMPFeatureInfoImp* feats = iClientData->FeaturesForSessionLC();
    iPreparedData = TXIMPObjectPacker< CXIMPFeatureInfoImp >::PackL( *feats );
    CleanupStack::PopAndDestroy( feats );

    aMessage.Complete( iPreparedData->Size() );
    }


// -----------------------------------------------------------------------------
// CXIMPContextSession::DoGetPreparedDataL()
// -----------------------------------------------------------------------------
//
void CXIMPContextSession::DoGetPreparedDataL(
    MXIMPSrvMessage& aMessage )
    {
    //Release the prepared data from member variable
    //Buffer gets cleaned even the copying to client side fails
    HBufC8* tmpBuf = iPreparedData;
    iPreparedData = NULL;
    CleanupStack::PushL( tmpBuf );

    //Assert that there is a data to copy to client
    __ASSERT_ALWAYS( tmpBuf,
                     aMessage.PanicClientAndLeaveL(
                     NXIMPPanic::EOnSrvNoPrepearedDataToCopy ) );

    aMessage.WriteL( MXIMPSrvMessage::Ep0, *tmpBuf );

    CleanupStack::PopAndDestroy( tmpBuf );
    aMessage.Complete( KErrNone );
    }


// End of file
