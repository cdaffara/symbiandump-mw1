/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Presence context event pump.
*
*/

#include "ximpcontexteventpump.h"
#include "ximpcntxteventobsarray.h"
#include "ximpundotask.h"

#include "ximpcontexteventfilter.h"
#include "ximpsrvcontextclient.h"
#include "ximpapieventbase.h"
#include "ximpeventcodec.h"
#include "ximpcontextclientproxy.h"
#include "ximpobjecthelpers.h"
#include "ximppanics.h"
#include "ximprequestcompleteeventimp.h"
#include "ximpstatusimp.h"
#include "ximpcontextstateeventimp.h"
#include "ximpcontextstateimp.h"

#include "ximpfeaturepluginholder.h"
#include "apidataobjfactoryaccessor.h"
#include "eventcodec.h"


#include <ximpcontextobserver.h>
#include <ximprequestcompleteevent.h>


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CXIMPContextEventPump::NewL()
// Instantiate new event pump.
// ---------------------------------------------------------------------------
//
CXIMPContextEventPump* CXIMPContextEventPump::NewL(
    MXIMPContext& aCntxtInterface,
    CXIMPContextClientProxy& aContextProxy )
    {
    CXIMPContextEventPump* self;
    self = new( ELeave ) CXIMPContextEventPump( aCntxtInterface,
                                                aContextProxy );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPContextEventPump::~CXIMPContextEventPump()
// Destroy the event pump.
// Cancels possibly pending asynhronous requests.
// ---------------------------------------------------------------------------
//
CXIMPContextEventPump::~CXIMPContextEventPump()
    {
    Cancel();
    delete iObservers;
    delete iSpareReqCompleteEvent;
    delete iSpareCtxStateEvent;
    
    FeaturePluginHolder::Release();
    }


// ---------------------------------------------------------------------------
// CXIMPContextEventPump::CXIMPContextEventPump()
// ---------------------------------------------------------------------------
//
CXIMPContextEventPump::CXIMPContextEventPump(
    MXIMPContext& aCntxtInterface,
    CXIMPContextClientProxy& aContextProxy )
    : CActive(CActive::EPriorityStandard),
      iCntxtInterface( aCntxtInterface ),
      iCtxClientProxy( &aContextProxy )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// CXIMPContextEventPump::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPContextEventPump::ConstructL()
    {
    iObservers = CXIMPCntxtEventObsArray::NewL();


    iSpareReqCompleteEvent = CXIMPRequestCompleteEventImp::NewL();
    iSpareReqCompleteEvent->CompletionResultImp().SetProtocolResultDescriptionL( KNullDesC );
    iSpareReqCompleteEvent->CompletionResultImp().SetProtocolResultCode( KErrNone );

    
    iSpareCtxStateEvent = CXIMPContextStateEventImp::NewL();
    
    CXIMPStatusImp* status = CXIMPStatusImp::NewLC();
    status->SetProtocolResultDescriptionL( KNullDesC );
    status->SetProtocolResultCode( KErrNone );
    
    iSpareCtxStateEvent->SetStateChangeReasonImp( status );
    CleanupStack::Pop( status );
    
    // Initialize the datamodel's feature plugin holder.
    // That stores the different internal IF accessor as global data 
    FeaturePluginHolder::InitializeL();    
    }


// ---------------------------------------------------------------------------
// CXIMPContextEventPump::RegisterObserverL()
// Reqisters new event observer.
// ---------------------------------------------------------------------------
//
void CXIMPContextEventPump::RegisterObserverL(
    MXIMPContextObserver& aObserver,
    const TArray< TInt32 >* aEventFilter )
    {
    MXIMPUndoTask* undo = NULL;

    if( iObservers->IsReqistered( aObserver ) )
        {
        //Update existing observer with new filter. UpdateObserverLC()
        //returns undoTask object that will undo the observer update
        //if there happens a leave before undo->CancelUndo() call.
        undo = iObservers->UpdateObserverLC( aObserver, aEventFilter );
        }
    else
        {
        //Add new event observer. AddObserverLC() returns undoTask object
        //that will undo the adding of new entry if there happens a leave before
        //undo->CancelUndo() call.
        undo = iObservers->AddObserverLC( aObserver, aEventFilter );
        }

    //Collect event filter from observers
    CXIMPContextEventFilter* filter = iObservers->CollectEventFilterLC();


    //Update the event filter to the server side.
    //Update is done everytime when observer is registered
    //since filter isn't updated when observer is removed
    TryInstallEventFilterL( *filter );

    //Issue event request to server side
    TryRefreshEventRequest();

    CleanupStack::PopAndDestroy( filter );

    undo->CancelUndo();
    CleanupStack::PopAndDestroy(); //undo
    }


// ---------------------------------------------------------------------------
// CXIMPContextEventPump::UnRegisterObserver()
// Unreqisters event observer.
// ---------------------------------------------------------------------------
//
void CXIMPContextEventPump::UnRegisterObserver(
    MXIMPContextObserver& aObserver )
    {
    //To simplify logic, event types are not unregistered here
    //but on the next time when the new observer is registered
    //and cobined event filer is rebuild
    iObservers->RemoveObserver( aObserver );

    if( iObservers->HasObservers() )
        {
        Cancel();
        }
    }


// ---------------------------------------------------------------------------
// CXIMPContextEventPump::StartEventPumpLC()
// ---------------------------------------------------------------------------
//
void CXIMPContextEventPump::StartEventPumpLC()
    {
    CleanupClosePushL( *this );

    CXIMPContextEventFilter* filter = iObservers->CollectEventFilterLC();
    TryInstallEventFilterL( *filter );
    CleanupStack::PopAndDestroy( filter );

    TryRefreshEventRequest();
    }



// ---------------------------------------------------------------------------
// CXIMPContextEventPump::Close()
// ---------------------------------------------------------------------------
//
void CXIMPContextEventPump::Close()
    {
    Cancel();
    }



// -----------------------------------------------------------------------------
// CXIMPContextEventPump::RunL()
//
// Handles one and only one event from XIMP server side event queue.
// (Only one event is handled in one EventPump RunL() round.)
// Steps:
// 1. Retrieve event data from server to client side
// 2. Convert event data to concrete event object
// 3. Delete the event data from server side 
// 4. Dispatch event to observers
// 5. Request new event notification (Request must be issued after event
//    dispatch, because concrete observer may start nested active scheduler
//    loop, and it could cause re-entrance to EventPump::RunL())
// -----------------------------------------------------------------------------
//
void CXIMPContextEventPump::RunL()
    {
    TInt status ( iStatus.Int() );
    User::LeaveIfError( status );


    HBufC8* eventData = iCtxClientProxy->EventPumpSessionL().FetchTopEventDataLC();
    TInt32 typeOfEvent;
    CXIMPApiEventBase* eventObject = XIMPEventCodec::UnPackL( *eventData,
                                                              typeOfEvent );

    MApiDataObjFactoryAccessor* presenceObjFactoryAcc = FeaturePluginHolder::PresenceObjFactoryAccessor();
    if ( !eventObject && presenceObjFactoryAcc )
        {
        // Try to create presence event
        eventObject = presenceObjFactoryAcc->EventCodec().UnPackL( *eventData,
                                                                 typeOfEvent );
        }

    if ( !eventObject )
        {
        TInt accesorCount = FeaturePluginHolder::AccessorCount() ;
        for (TInt i=0; i<accesorCount; i++)
	        {
        	MApiDataObjFactoryAccessor* pFactoryAcc = FeaturePluginHolder::Accessor(i) ;
        	if(pFactoryAcc)
	        	{
	        	eventObject = pFactoryAcc->EventCodec().UnPackL( *eventData, 
	        														   typeOfEvent );
	        	if(eventObject)
	    	    	{
	        		break ;
	        		}
	        	}
        	}
        }
        
        

    // Check whether have received an unknown event
    __ASSERT_ALWAYS( eventObject,
                    User::Panic( _L("XIMPContextEventPump"), KErrUnknown ) );
                        
    CleanupStack::PopAndDestroy(); //eventData
    CleanupStack::PushL( eventObject );
    
    User::LeaveIfError( iCtxClientProxy->EventPumpSessionL().DropTopEvent() );
    if( eventObject->Base().GetInterfaceId() == MXIMPRequestCompleteEvent::KInterfaceId )
        {
        const MXIMPRequestCompleteEvent* complEvent =
            TXIMPGetInterface< const MXIMPRequestCompleteEvent >::From( eventObject->Base(),
                                                                        MXIMPBase::EPanicIfUnknown );
        
        iCtxClientProxy->HandleReqCompleted( complEvent->RequestId(), 
                                             complEvent->CompletionResult().ResultCode() );
        }

    iObservers->DeliverEvent( iCntxtInterface, eventObject->Base() );
    CleanupStack::PopAndDestroy( eventObject );

    TryRefreshEventRequest();
    }


// -----------------------------------------------------------------------------
// CXIMPContextEventPump::RunError()
//
// Handle leave from CXIMPContextEventPump::RunL()
// by restarting the event request.
// -----------------------------------------------------------------------------
//
TInt CXIMPContextEventPump::RunError( TInt aError )
    {
    if( aError == KErrServerTerminated )
        {
        //Server has died.
        //Complete all pending client requests with KErrServerTerminated
        //and unbind the client context. 

        //Note: This should be perhaps splitted to asynhronous steps 
        //to follow general paradigm of XIMP observer callbacks. 
        //Also Ctx state events should be properly indicated to client
        //(Now they are notified after possible Bind() completion notification)
        
        TXIMPRequestId reqId;
        while( iCtxClientProxy->GetTopPendingReq( reqId ) )
            {
            iSpareReqCompleteEvent->SetRequestId( reqId );
            iSpareReqCompleteEvent->CompletionResultImp().SetResultCode( aError );
            
            iObservers->DeliverEvent( iCntxtInterface, *iSpareReqCompleteEvent );

            iCtxClientProxy->HandleReqCompleted( iSpareReqCompleteEvent->RequestId(), 
                                                 iSpareReqCompleteEvent->CompletionResult().ResultCode() );
            }

        iSpareCtxStateEvent->StateChangeReasonImp()->SetResultCode( aError );
        iSpareCtxStateEvent->ContextStateImp().SetStateValue( MXIMPContextState::EInactive );
        iObservers->DeliverEvent( iCntxtInterface, *iSpareCtxStateEvent );
        }

    else
        {
        //If it was a RequestComplete event, which handling failed here 
        //on the client side, here is manufactured artificially matching 
        //completion event to notify failure to client

        __ASSERT_ALWAYS( iCtxClientProxy->EventPumpSession(),
                         User::Panic( NXIMPPrivPanic::KCategory,
                                      NXIMPPrivPanic::EFrontEndEventPumpNoServerClient ) );

        
        TXIMPRequestId reqId = TXIMPRequestId::Null(); 

        iCtxClientProxy->EventPumpSession()->FetchTopEventReqId( reqId );
        iCtxClientProxy->EventPumpSession()->DropTopEvent();
        
        if( reqId != TXIMPRequestId::Null() )
            {
            iSpareReqCompleteEvent->SetRequestId( reqId );
            iSpareReqCompleteEvent->CompletionResultImp().SetResultCode( aError );
            
            iObservers->DeliverEvent( iCntxtInterface, *iSpareReqCompleteEvent );

            iCtxClientProxy->HandleReqCompleted( iSpareReqCompleteEvent->RequestId(), 
                                                 iSpareReqCompleteEvent->CompletionResult().ResultCode() );
            }

        TryRefreshEventRequest();
        }

    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CXIMPContextEventPump::DoCancel()
// -----------------------------------------------------------------------------
//
void CXIMPContextEventPump::DoCancel()
    {
    __ASSERT_ALWAYS( iCtxClientProxy->EventPumpSession(),
                     User::Panic( NXIMPPrivPanic::KCategory,
                                  NXIMPPrivPanic::EFrontEndEventPumpNoServerClient ) );

    //Eventpump is stopping.
    //Cancel pending event notification request
    iCtxClientProxy->EventPumpSession()->CancelListening();
    }


// -----------------------------------------------------------------------------
// CXIMPContextEventPump::TryRefreshEventRequest()
// -----------------------------------------------------------------------------
//
void CXIMPContextEventPump::TryRefreshEventRequest()
    {
    if( !IsActive() && 
        iObservers->HasObservers() && 
        iCtxClientProxy->EventPumpSession() )
        {
        iCtxClientProxy->EventPumpSession()->ListenEvent( iStatus );
        SetActive();
        }
    }


// -----------------------------------------------------------------------------
// CXIMPContextEventPump::TryInstallEventFilterL()
// -----------------------------------------------------------------------------
//
void CXIMPContextEventPump::TryInstallEventFilterL(
    const CXIMPContextEventFilter& aFilter )
    {
    if( iCtxClientProxy->EventPumpSession() )
        {
        HBufC8* pack = TXIMPObjectPacker<CXIMPContextEventFilter>::PackL( aFilter );
        CleanupStack::PushL( pack );
        iCtxClientProxy->EventPumpSession()->InstallEventFilterL( *pack );
        CleanupStack::PopAndDestroy( pack );
        }
    }
