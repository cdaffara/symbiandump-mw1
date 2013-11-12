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
* Description:  Presence connection host
 *
*/

#include <ximpprotocolconnection.h>
#include <ximpprotocolconnectionhostobserver.h>

#include "ximpapiobjbase.h"
#include "ximpstatusimp.h"
#include "ximpoperationbase.h"
#include "ximphostimp.h"
#include "ximpglobals.h"
#include "ximppluginfactory.h"
#include "ximpeventmanager.h"
#include "ximphoststates.h"
#include "ximpcontextstateeventimp.h"
#include "ximpobjectfactoryimp.h"
#include "ximppanics.h"
#include "ximppsccontextimp.h"
#include "presencedatacacheimp.h"
#include "ximpserviceinfoimp.h"
#include "ximpoperationbase.h"
#include "ximpoperationfactory.h"
#include "ximpoperationfactory1.h"
#include "ximpobjecthelpers.h"
#include "ximpoperationdefs.h"
#include "ximpcontexteventqueue.h"
#include "ximphostmanager.h"
#include "ximpidentityimp.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximprequestcompleteeventimp.h"
#include "ximpcontextclientinfoimp.h"

#include "protocolpresencedatahostimp.h"

#include "protocolpresenceauthorizationdatahostimp.h"
#include "protocolpresencepublishingdatahostimp.h"
#include "protocolpresencewatchingdatahostimp.h"
#include "protocolpresentitygroupsdatahostimp.h"
#include "ximpprotocolrequestcompleteeventimp.h"

#include <protocoldatahost.h>

#include "ximpdlluids.hrh"
#include "ximptrace.h"

// TODO: illegal dependency to presence operations
#include "presenceoperationdefs.h"

// ======== LOCAL FUNCTIONS ========
const TInt KPluginArrayGranularity = 3;


// ---------------------------------------------------------------------------
// OrderOfObservers()
// ---------------------------------------------------------------------------
//
TInt OrderOfObservers( const MXIMPProtocolConnectionHostObserver& aA, 
                       const MXIMPProtocolConnectionHostObserver& aB )
    {
    if( &aA < &aB )
        {
        return -1;
        }
    else if( &aA > &aB )
        {
        return 1;
        }
    return 0;
    }

// ======== MEMBER FUNCTIONS ========

using namespace NXIMPHost;

// This should be variated. Branding variation can be one
// possibility. One is to leave adaptation deside how soon
// cleanup will be done.
// Now 5 sec used for testing purposes. Better value could be
// some minutes.
const TInt KPscHostCleanerTimerLength = 5; // in sec

// ---------------------------------------------------------------------------
// CXIMPHost::CXIMPHost()
// ---------------------------------------------------------------------------
//
CXIMPHost::CXIMPHost( MXIMPProtocolConnection& aConnection, TInt32 aProtocolUid  )
: iConnection( aConnection ), iProtocolUid( aProtocolUid )
    {
    iState = NXIMPHost::EXIMPNotConnected;

    iHostOriginatedOpIndex = 1;

    iReferences = 1;
    
    iReqIdBuilder.SetSessionId( (unsigned) this );
    
    iForcedTearDown = EFalse;
    }


// ---------------------------------------------------------------------------
// CXIMPHost::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPHost::ConstructL()
    {
    iObjFactory = CXIMPObjectFactoryImp::NewL();
    iEventManager = CXIMPEventManager::NewL();
    
    iPresenceDataHost = CProtocolPresenceDataHostImp::NewL( *this );

    iDataHostArray = new(ELeave)CArrayFixFlat<MProtocolDataHost*>(KPluginArrayGranularity);
    
    LoadDataHostArrayL() ;
    
/*    iDataCache = CPresenceDataCache::NewL( *this );
    iAuthDataHost = CProtocolPresenceAuthorizationDataHostImp::NewL( *this );
    iPublishingDataHost = CProtocolPresencePublishingDataHostImp::NewL( *this );
    iWatchingDataHost = CProtocolPresenceWatchingDataHostImp::NewL( *this );
    iGroupsDataHost = CProtocolPresentityGroupsDataHostImp::NewL( *this );
*/
    iCleanerTimer = CPeriodic::NewL( CActive::EPriorityStandard );

    for ( int i = 0;i < 5;i++ )
        {
        TInt Ret = iCallBack.Append( new ( ELeave ) CAsyncCallBack( CActive::EPriorityHigh ) );
        UNUSED_FORMAL_PARAM(Ret);	
        }
    
    iCallBack[ECompleteRequestCallback]->Set( TCallBack( CompleteRequestCallback, this ) );
    iCallBack[ECompleteAdaptationRequestCallback]->Set( TCallBack( CompleteAdaptationRequestCallback, this ) );
    iCallBack[ECompleteSessionMgmtRequestCallback]->Set( TCallBack( CompleteSessionMgmtRequestCallback, this ) );
    iCallBack[ECompleteInternalRequestCallback]->Set( TCallBack( CompleteInternalRequestCallback, this ) );
    iCallBack[EStartProcessCallback]->Set( TCallBack( StartProcessCallback, this ) );
    
    iAdaptationReqCompleteEvent = CXIMPProtocolRequestCompleteEventImp::NewL();

    StartCleanerTimer();
    }


// ---------------------------------------------------------------------------
// CXIMPHost::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPHost* CXIMPHost::NewLC( MXIMPProtocolConnection& aConnection, TInt32 aProtocolUid  )
    {
    CXIMPHost* self = new( ELeave ) CXIMPHost( aConnection, aProtocolUid );
    CleanupClosePushL( *self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPHost::~CXIMPHost()
// ---------------------------------------------------------------------------
//
CXIMPHost::~CXIMPHost()
    {
    __ASSERT_ALWAYS( iReferences == 0, User::Panic( NXIMPPrivPanic::KCategory, NXIMPPrivPanic::EHostStillReferenced ) );

    delete iCleanerTimer;

    delete iObjFactory;
    iOperations.Close();
    delete iEventManager;
    __ASSERT_ALWAYS( iCurrentOperation == 0,
                User::Panic( NXIMPPrivPanic::KCategory, NXIMPPrivPanic::EHostCurrentOperationStillActive ) );
    iRegSessions.Close();

    delete iPresenceDataHost;

    if ( iDataHostArray )
    {
    	TInt idx = KErrNotFound;
    	while( iDataHostArray->Count() )
        {
	        idx = iDataHostArray->Count() - 1;
	        // Delete object
	        delete iDataHostArray->At( idx );
	        // Delete element
	        iDataHostArray->Delete( idx );
        }
    }
    delete iDataHostArray;

  /*  delete iDataCache;
    delete iAuthDataHost;
    delete iPublishingDataHost;
    delete iWatchingDataHost;
    delete iGroupsDataHost;
*/
    // delete iCallBack;
    iCallBack.ResetAndDestroy();

    CXIMPGlobals::Instance()->HostManager()->DisassociateHost( this );
    
    delete iAdaptationReqCompleteEvent;
    iConnectionHostObservers.Reset();
    }

// ---------------------------------------------------------------------------
// CXIMPHost::Open()
// ---------------------------------------------------------------------------
//
void CXIMPHost::Open()
    {
    iReferences++;
    }

// ---------------------------------------------------------------------------
// CXIMPHost::Close()
// ---------------------------------------------------------------------------
//
void CXIMPHost::Close()
    {
    __ASSERT_ALWAYS( iReferences >= 0, User::Panic( NXIMPPrivPanic::KCategory, NXIMPPrivPanic::EHostNegativeReferenceCount ));

    iReferences--;

    if ( iReferences <= 0 )
        {
        delete this;
        }
    }


// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CXIMPHost, MXIMPProtocolConnectionHost )
XIMPIMP_IF_BASE_GET_INTERFACE_END()
XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CXIMPHost, MXIMPProtocolConnectionHost )
XIMPIMP_IF_BASE_GET_INTERFACE_END()



// ---------------------------------------------------------------------------
// CXIMPHost::ObjectFactory()
// ---------------------------------------------------------------------------
//
MXIMPObjectFactory& CXIMPHost::ObjectFactory()
    {
    return *iObjFactory;
    }

// ---------------------------------------------------------------------------
// From MXIMPProtocolConnectionHost class.
// CXIMPHost::AuthorizationDataHost()
// ---------------------------------------------------------------------------
//
MProtocolPresenceDataHost& CXIMPHost::ProtocolPresenceDataHost()
    {
    return *iPresenceDataHost;
    }


// ---------------------------------------------------------------------------
// CXIMPHost::RegisterObserverL()
// ---------------------------------------------------------------------------
//
void CXIMPHost::RegisterObserverL(
        MXIMPProtocolConnectionHostObserver& aObserver,
        const TArray<TInt32>* aEventFilter /*= NULL*/ )
    {
    XIMP_UNUSED_FORMAL_PARAM( aEventFilter );
    TLinearOrder< MXIMPProtocolConnectionHostObserver > order( *OrderOfObservers );
    TInt error = iConnectionHostObservers.InsertInOrder( &aObserver, order );
    if( error != KErrAlreadyExists )
        {
        User::LeaveIfError( error );
        }
    }

// ---------------------------------------------------------------------------
// CXIMPHost::UnregisterConnectionHostObserver()
// ---------------------------------------------------------------------------
//
void CXIMPHost::UnregisterConnectionHostObserver(
        MXIMPProtocolConnectionHostObserver& aObserver )
    {
    TLinearOrder< MXIMPProtocolConnectionHostObserver > order( *OrderOfObservers );
    TInt index = iConnectionHostObservers.FindInOrder( &aObserver, order );
    if( index > KErrNotFound )
        {
        iConnectionHostObservers.Remove( index );
        }
    }

// ---------------------------------------------------------------------------
// CXIMPHost::HandleRequestCompleted()
// ---------------------------------------------------------------------------
//
void CXIMPHost::HandleRequestCompleted(
    TXIMPRequestId aReqId,
    TInt aResultCode )
    {
    CXIMPOperationBase& operation = OperationForRequestId( aReqId );

    operation.SetResultCode( aResultCode );
    CompleteRequest( aReqId );
    }

// ---------------------------------------------------------------------------
// CXIMPHost::HandleRequestCompleted()
// ---------------------------------------------------------------------------
//
void CXIMPHost::HandleRequestCompleted( TXIMPRequestId aReqId,
                                       MXIMPStatus* aResult )
    {
    CXIMPOperationBase& operation = OperationForRequestId( aReqId );
    
    operation.SetStatus( aResult );
    CompleteRequest( aReqId );
    }


// ---------------------------------------------------------------------------
// CXIMPHost::HandleRequestCompleted()
// ---------------------------------------------------------------------------
//
void CXIMPHost::HandleRequestCompleted( TXIMPRequestId aReqId,
                                       MXIMPStatus* aResult,
                                       MXIMPBase* aParameter )
    {    
    CXIMPOperationBase& operation = OperationForRequestId( aReqId );
    
    operation.SetObjectCollection( aParameter );
    HandleRequestCompleted( aReqId, aResult );
    }

// ---------------------------------------------------------------------------
// CXIMPHost::CompleteRequest()
// ---------------------------------------------------------------------------
//
void CXIMPHost::CompleteRequest( TXIMPRequestId aReqId )
    {
    CXIMPOperationBase& operation = OperationForRequestId( aReqId );

#ifdef _DEBUG
	if ( operation.Context() )	
		TRACE_2( _L("Operation completed for client \"%S\" with result code: %d"), 
			 	&operation.Context()->ClientInfo()->ClientId().Identity(), 
			 	operation.Status().ResultCode() );
#endif			 
			 
    // iCallBack->Set( TCallBack( CompleteRequestCallback, this ) );
    iCallBack[ECompleteRequestCallback]->CallBack();
    }

// ---------------------------------------------------------------------------
// CXIMPHost::HandleConnectionTerminated()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CXIMPHost::HandleConnectionTerminated( MXIMPStatus* aReason )
    {
	TRACE_1( _L("CXIMPHost::HandleConnectionTerminated aReason = %d"), aReason );
    // This should be set for operation as a reason.
    XIMP_UNUSED_FORMAL_PARAM( aReason );
/*
    if ( iState != EXIMPConnected )
        {
        // we are not in a valid state
        User::Panic( NXIMPPrivPanic::KCategory,
                NXIMPPrivPanic::EInvalidStateForSessionLost );
        }
*/
    TXIMPRequestId reqId;

    // tear down
    // we must run the cached unbinds to force teardown
    // Right way to do this is using non leaving operation add. This can be done
    // if operation queue is implemented as linked list. When operation queue is fixed
    // assert should be removed.
    TRAPD( error, reqId = AddNewOperationL( NXIMPOps::EXIMPForcedTeardown ) );
    __ASSERT_ALWAYS( error == KErrNone, User::Panic( _L("HandleSessionLost"), -667 ) );

    iForcedTearDown = ETrue;
	
    return reqId;
    }


// ---------------------------------------------------------------------------
// CXIMPHost::UnregisterSession()
// ---------------------------------------------------------------------------
//
void CXIMPHost::UnregisterSession( CXIMPPscContext* aContext )
    {
    if( !aContext )
        {
        return;
        }
    iEventManager->UnregisterSessionQueue( aContext->EventQueue() );
    TLinearOrder< CXIMPPscContext > order( *CXIMPPscContext::OrderOfContexts );
    TInt index = iRegSessions.FindInOrder( aContext, order );
    if( index >= 0 )
        {
        iPresenceDataHost->PresenceDataCache().CleanResources( *aContext );
        iRegSessions.Remove( index );
        }
    }

// ---------------------------------------------------------------------------
// CXIMPHost::FakeCompleteRequest()
// ---------------------------------------------------------------------------
//
void CXIMPHost::FakeCompleteRequest( TXIMPRequestId aReqId, TInt aResult )
    {
    HandleRequestCompleted( aReqId, aResult );
    }

// ---------------------------------------------------------------------------
// CXIMPHost::HandleAdaptationRequestCompleted()
// ---------------------------------------------------------------------------
//
void CXIMPHost::HandleAdaptationRequestCompleted( TXIMPRequestId aReqId, TInt aResult )
    {
    CXIMPOperationBase& operation = OperationForRequestId( aReqId );
    
    operation.SetResultCode( aResult );
    // iCallBack->Set( TCallBack( CompleteAdaptationRequestCallback, this ) );
    iCallBack[ECompleteAdaptationRequestCallback]->CallBack();
    }

// ---------------------------------------------------------------------------
// CXIMPHost::HandleSessionMgmtRequestCompleted()
// ---------------------------------------------------------------------------
//
void CXIMPHost::HandleSessionMgmtRequestCompleted( TXIMPRequestId aReqId, TInt aResult )
    {
    CXIMPOperationBase& operation = OperationForRequestId( aReqId );

    operation.SetResultCode( aResult );
    // iCallBack->Set( TCallBack( CompleteSessionMgmtRequestCallback, this ) );
    iCallBack[ECompleteSessionMgmtRequestCallback]->CallBack();
    }

// ---------------------------------------------------------------------------
// CXIMPHost::HandleInternalRequestCompleted()
// ---------------------------------------------------------------------------
//
void CXIMPHost::HandleInternalRequestCompleted( TXIMPRequestId aReqId, TInt aResult )
    {
    CXIMPOperationBase& operation = OperationForRequestId( aReqId );

    operation.SetResultCode( aResult );
    // iCallBack->Set( TCallBack( CompleteInternalRequestCallback, this ) );
    iCallBack[ECompleteInternalRequestCallback]->CallBack();
    }

// ---------------------------------------------------------------------------
// CXIMPHost::PresenceDataAccess()
// ---------------------------------------------------------------------------
//
CProtocolPresenceDataHostImp& CXIMPHost::PresenceDataAccess()
    {
    return *iPresenceDataHost;
    }

// ---------------------------------------------------------------------------
// CXIMPHost::DataAccess()
// ---------------------------------------------------------------------------
//
/*CPresenceDataCache& CXIMPHost::DataAccess()
    {
    return *iDataCache;
    }

// ---------------------------------------------------------------------------
// CXIMPHost::AuthorizationDataAccess()
// ---------------------------------------------------------------------------
//
CProtocolPresenceAuthorizationDataHostImp& CXIMPHost::AuthorizationDataAccess()
    {
    return *iAuthDataHost;
    }

// ---------------------------------------------------------------------------
// CXIMPHost::PublishingDataAccess()
// ---------------------------------------------------------------------------
//
CProtocolPresencePublishingDataHostImp& CXIMPHost::PublishingDataAccess()
    {
    return *iPublishingDataHost;
    }

// ---------------------------------------------------------------------------
// CXIMPHost::WatchingDataAccess()
// ---------------------------------------------------------------------------
//
CProtocolPresenceWatchingDataHostImp& CXIMPHost::WatchingDataAccess()
    {
    return *iWatchingDataHost;
    }

// ---------------------------------------------------------------------------
// CXIMPHost::GroupsDataAccess()
// ---------------------------------------------------------------------------
//
CProtocolPresentityGroupsDataHostImp& CXIMPHost::GroupsDataAccess()
    {
    return *iGroupsDataHost;
    }
*/
// ---------------------------------------------------------------------------
// CXIMPHost::RegisterSessionL()
// ---------------------------------------------------------------------------
//
void CXIMPHost::RegisterSessionL( CXIMPPscContext* aContext )
    {
    if( !aContext )
        {
        User::Leave( KErrArgument );
        }
    iEventManager->RegisterSessionQueueL( aContext->EventQueue() );
    TLinearOrder< CXIMPPscContext > order( *CXIMPPscContext::OrderOfContexts );
    TInt err = iRegSessions.InsertInOrder( aContext, order );
    if( err != KErrNone )
        {
        iEventManager->UnregisterSessionQueue( aContext->EventQueue() );
        User::Leave( err );
        }
    }

// ---------------------------------------------------------------------------
// CXIMPHost::AddNewOperationL()
// ---------------------------------------------------------------------------
//
void CXIMPHost::AddNewOperationL( CXIMPOperationBase& aOperation )
    {
    iOperations.AppendL( &aOperation );
    aOperation.SetHost( *this );

    if ( !iProcessing )
        {
        ActivateProcessing();
        }
    }

// ---------------------------------------------------------------------------
// CXIMPHost::AddNewOperationL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CXIMPHost::AddNewOperationL( NXIMPOps::TXIMPOpTypes aOpType )
    {
    iReqIdBuilder.SetProtocolId( iProtocolUid );
    iReqIdBuilder.SetRequestId( iAdaptationOperationIndex );
    TXIMPRequestId reqId = iReqIdBuilder.BuildRequestId();

    iAdaptationOperationIndex++;
    CXIMPOperationBase* op =
        CXIMPGlobals::Instance()->OperationFactory()->CreateOperationL( aOpType );
    CleanupStack::PushL( op );
    op->BaseConstructL( reqId, NULL );
    op->ConstructL( KNullDesC8 );
    AddNewOperationL( *op );
    CleanupStack::Pop(); //operation
    return reqId;
    }

// ---------------------------------------------------------------------------
// CXIMPHost::AddNewOperationL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CXIMPHost::AddNewOperationL( CXIMPOperationBase* aOperation )
    {
    iReqIdBuilder.SetProtocolId( iProtocolUid );
    iReqIdBuilder.SetRequestId( iAdaptationOperationIndex );
    TXIMPRequestId reqId = iReqIdBuilder.BuildRequestId();

    iAdaptationOperationIndex++;
    aOperation->BaseConstructL( reqId, NULL );
    aOperation->ConstructL( KNullDesC8 );
    AddNewOperationL( *aOperation );
    return reqId;
    }


// ---------------------------------------------------------------------------
// CXIMPHost::GetConnection()
// ---------------------------------------------------------------------------
//
MXIMPProtocolConnection& CXIMPHost::GetConnection() const
    {
    return iConnection;
    }

// ---------------------------------------------------------------------------
// CXIMPHost::AddEventL()
// ---------------------------------------------------------------------------
//
void CXIMPHost::AddEventL( CXIMPApiEventBase& aEvent, MXIMPPscContext* aContext  )
    {
    AddEventL( aEvent, static_cast< CXIMPPscContext* >( aContext ) );
    }

// ---------------------------------------------------------------------------
// CXIMPHost::AddEventL()
// ---------------------------------------------------------------------------
//
void CXIMPHost::AddEventL( CXIMPApiEventBase& aEvent, CXIMPPscContext* aContext  )
    {
    if( aContext )
        {
        iEventManager->AddEventL( aEvent, aContext );
        }
    else
        {
        iEventManager->AddEventL( aEvent, iRegSessions );
        }
    }

// ---------------------------------------------------------------------------
// CXIMPHost::ActivateEventL()
// ---------------------------------------------------------------------------
//
void CXIMPHost::ActivateEventL( CXIMPApiEventBase& aEvent, MXIMPPscContext* aContext  )
    {
    ActivateEventL( aEvent, static_cast< CXIMPPscContext* >( aContext ) );
    }

// ---------------------------------------------------------------------------
// CXIMPHost::ActivateEventL()
// ---------------------------------------------------------------------------
//
void CXIMPHost::ActivateEventL( CXIMPApiEventBase& aEvent, CXIMPPscContext* aContext  )
    {
    iEventManager->ActivateEventL( aEvent, aContext );
    }

// ---------------------------------------------------------------------------
// CXIMPHost::StartProcessing()
// ---------------------------------------------------------------------------
//
void CXIMPHost::StartProcessing()
    {
    CXIMPOperationBase* currentOperation = iCurrentOperation;
    iCurrentOperation = NULL;

    iProcessing = EFalse;

    if( iOperations.Count() > 0 )
        {
        ActivateProcessing();
        }

    delete currentOperation; // might invoke destruction of host.
    }

// ---------------------------------------------------------------------------
// CXIMPHost::ActivateProcessing()
// ---------------------------------------------------------------------------
//
void CXIMPHost::ActivateProcessing()
    {
    // take the first operation from the queue
    if( !iCurrentOperation )
        {
        if( iOperations.Count() <= 0 )
            {
            return;
            }
        iCurrentOperation = iOperations[0];
        iOperations.Remove( 0 );
        }

    iCurrentOperation->BeginOperation();
    iProcessing = ETrue;
    // iCallBack->Set( TCallBack( StartProcessCallback, this ) );
    iCallBack[EStartProcessCallback]->CallBack();
    }

// ---------------------------------------------------------------------------
// CXIMPHost::StartProcessCallback()
// ---------------------------------------------------------------------------
//
TInt CXIMPHost::StartProcessCallback( TAny *aInstance )
    {
    ( static_cast< CXIMPHost* >( aInstance ) )->DoStartProcessCallback();
    return 0; // ignored by CAsychCallBack
    }

// ---------------------------------------------------------------------------
// CXIMPHost::DoStartProcessCallbackL()
// ---------------------------------------------------------------------------
//
void CXIMPHost::DoStartProcessCallback()
    {
#ifdef _DEBUG    
    if ( iCurrentOperation->Context() )
		TRACE_1( _L("Start processing operation for client \"%S\""), 
			 	&iCurrentOperation->Context()->ClientInfo()->ClientId().Identity() );
#endif

    TRAPD( err, iCurrentOperation->ProcessL() );

    if ( err != KErrNone )
        {
        HandleRequestCompleted( iCurrentOperation->RequestId(), err  );
        // internal operation End event and deletion of iCurrentOperation happen within CompleteRequest
        }
    }

// ---------------------------------------------------------------------------
// CXIMPHost::CompleteRequestCallback()
// ---------------------------------------------------------------------------
//
TInt CXIMPHost::CompleteRequestCallback( TAny *aInstance )
    {
    static_cast< CXIMPHost* >( aInstance )->DoCompleteRequestCallback();
    return 0; // ignored by CAsychCallBack
    }

// ---------------------------------------------------------------------------
// CXIMPHost::CompleteAdaptationRequestCallback()
// ---------------------------------------------------------------------------
//
TInt CXIMPHost::CompleteAdaptationRequestCallback( TAny *aInstance )
    {
    static_cast< CXIMPHost* >( aInstance )->DoCompleteAdaptationRequestCallback();
    return 0; // ignored by CAsychCallBack
    }

// ---------------------------------------------------------------------------
// CXIMPHost::CompleteSessionMgmtRequestCallback()
// ---------------------------------------------------------------------------
//
TInt CXIMPHost::CompleteSessionMgmtRequestCallback( TAny *aInstance )
    {
    static_cast< CXIMPHost* >( aInstance )->DoCompleteSessionMgmtRequestCallback();
    return 0; // ignored by CAsychCallBack
    }

// ---------------------------------------------------------------------------
// CXIMPHost::CompleteInternalRequestCallback()
// ---------------------------------------------------------------------------
//
TInt CXIMPHost::CompleteInternalRequestCallback( TAny *aInstance )
    {
    static_cast< CXIMPHost* >( aInstance )->DoCompleteInternalRequestCallback();
    return 0; // ignored by CAsychCallBack
    }

// ---------------------------------------------------------------------------
// CXIMPHost::DoCompleteRequestCallback()
// ---------------------------------------------------------------------------
//
void CXIMPHost::DoCompleteRequestCallback()
    {
    TRAPD( error, iCurrentOperation->RequestCompletedL() );

    // Get event and update result code to event's status container
    CXIMPRequestCompleteEventImp* reqCompEvent = iCurrentOperation->GetReqCompleteEvent();

    if( error != KErrNone )
        {
        reqCompEvent->CompletionResultImp().SetResultCode( error );
        }

	if(!(iCurrentOperation->IsServerOperation()))
    	{
        // send the event
        iEventManager->ActivateRequestCompleteEvent( *reqCompEvent, iCurrentOperation->Context() );
    	}

    iCurrentOperation->EndOperation();
    StartProcessing();
    }

// ---------------------------------------------------------------------------
// CXIMPHost::DoCompleteAdaptationRequestCallback()
// ---------------------------------------------------------------------------
//
void CXIMPHost::DoCompleteAdaptationRequestCallback()
    {
    TInt leaveError = KErrNone;
    TRAP( leaveError, iCurrentOperation->RequestCompletedL() ); // CSI: 42 #
    
    if( leaveError != KErrNone )
        {
        iAdaptationReqCompleteEvent->SetResultCode( leaveError );
        }
    else
        {
        const TXIMPRequestId reqId = iCurrentOperation->RequestId();        
        const TInt result = iCurrentOperation->Status().ResultCode();
        
        iAdaptationReqCompleteEvent->SetRequestId( reqId );
        iAdaptationReqCompleteEvent->SetResultCode( result );
        }        
        
    
    TInt count = iConnectionHostObservers.Count();
    for( TInt a = 0; a < count; ++a )
        {
        iConnectionHostObservers[ a ]->HandleProtocolConnectionHostEvent( *this,
                                                                          *iAdaptationReqCompleteEvent );
        }

    iCurrentOperation->EndOperation();

    StartProcessing();
    }

// ---------------------------------------------------------------------------
// CXIMPHost::DoCompleteSessionMgmtRequestCallback()
// ---------------------------------------------------------------------------
//
void CXIMPHost::DoCompleteSessionMgmtRequestCallback()
    {
    TRAP_IGNORE( 
        iCurrentOperation->RequestCompletedL();
        iCurrentOperation->EndOperation();
        )
        
    StartProcessing();
    }

// ---------------------------------------------------------------------------
// CXIMPHost::DoCompleteInternalRequestCallback()
// ---------------------------------------------------------------------------
//
void CXIMPHost::DoCompleteInternalRequestCallback()
    {
    TRAP_IGNORE( 
        iCurrentOperation->RequestCompletedL();
        iCurrentOperation->EndOperation();
        )

    StartProcessing();
    }

// ---------------------------------------------------------------------------
// CXIMPHost::ForcedTeardown()
// ---------------------------------------------------------------------------
//
void CXIMPHost::ForcedTeardownL()
    {
    // activate all cached unbinds in all contexts,
    // first ones will do unshare, the last one will be the actual unbind.
    // psc host FSA takes care of proper events.
    //
    // but if all binds failed, then there are no cached unbinds since
    // each bind operation has destroyed the cached unbind. in that
    // case we are already in PSCNotConnected state.

    TInt foundOps = 0;

    for ( TInt i = 0; i < iRegSessions.Count(); i++ )
        {
        CXIMPPscContext* context = iRegSessions[ i ];
        CXIMPOperationBase* op = context->GetCachedUnbind();

        // the operation might already be killed
        if ( op )
            {
            op->SetServerOperation();
            foundOps++;

            // This trap can be removed after event queue is changed to 
            // linked list. See CXIMPHost::HandleConnectionTerminated
            TRAPD( err, AddNewOperationL( *op ); );
            if ( err != KErrNone )
                {
                // could not append operation, so just delete it
                delete op;
                }
            }
        }

    //__ASSERT_ALWAYS( foundOps > 0, User::Panic( NXIMPPrivPanic::KCategory, NXIMPPrivPanic::ENoCachedUnbindsFound ) );
    }


// ---------------------------------------------------------------------------
// CXIMPHost::StartCleanerTimer()
// ---------------------------------------------------------------------------
//
void CXIMPHost::StartCleanerTimer()
    {
    if( !iCleanerTimer->IsActive() )
        {
        TInt time = 1000000 * KPscHostCleanerTimerLength;
        TCallBack callback( CleanerCallback, this );
        iCleanerTimer->Start( time, time, callback );
        }
    }


// ---------------------------------------------------------------------------
// CXIMPHost::CreateOperationL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CXIMPHost::AddNewOperationL( 
    TInt aOperation, 
    MXIMPBase* aParam, 
    MXIMPBase* aParam2 /*=NULL*/ )
    {
    CXIMPRestrictedObjectCollectionImp* collection = CXIMPRestrictedObjectCollectionImp::NewLC( EFalse );
    collection->AddOrReplaceTypeL( aParam );
    if( aParam2 )
        {
        collection->AddOrReplaceTypeL( aParam2 );
        }
    TXIMPRequestId reqId = AddNewOperationL( aOperation, collection );
    CleanupStack::Pop( collection );
    return reqId;
    } 
    
    
// ---------------------------------------------------------------------------
// CXIMPHost::AddNewOperationL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CXIMPHost::AddNewOperationL( 
    TInt aOperation, 
    CXIMPRestrictedObjectCollectionImp* aParams )
    {
    CXIMPOperationBase* operation = 
        CXIMPGlobals::Instance()->OperationFactory()->CreateOperationL( aOperation );
    CleanupStack::PushL( operation );
    TXIMPRequestId reqId = AddNewOperationL( operation );
    CleanupStack::Pop( operation );
    operation->SetObjectCollection( aParams );
    aParams->SetObjectOwnership();
    return reqId;
    }

// ---------------------------------------------------------------------------
// CXIMPHost::CleanerCallback()
// ---------------------------------------------------------------------------
//
TInt CXIMPHost::CleanerCallback( TAny* aMyself )
    {
    (static_cast<CXIMPHost*>(aMyself))->DoCleanerCallback();
    return 0; // ignored by CPeriodic
    }

// ---------------------------------------------------------------------------
// CXIMPHost::DoCleanerCallback()
// ---------------------------------------------------------------------------
//
void CXIMPHost::DoCleanerCallback()
    {
    TRAPD( err, DoCleanerCallbackL() );

    if ( err != KErrNone )
        {
        // Try to start cleaner again after interval when error happened.
        StartCleanerTimer();
        }
    }

// ---------------------------------------------------------------------------
// CXIMPHost::DoCleanerCallbackL()
// ---------------------------------------------------------------------------
//
void CXIMPHost::DoCleanerCallbackL()
    {
    // create CleanerOperation

    iCleanerTimer->Cancel();  // CPeriodic must be stopped

    // host originated operations have session id of 0
    iReqIdBuilder.SetProtocolId( 0 );
    iReqIdBuilder.SetRequestId( iHostOriginatedOpIndex );
    TXIMPRequestId reqId = iReqIdBuilder.BuildRequestId();

    iHostOriginatedOpIndex++;
	//// for Presence Operation Cleaner
    CXIMPOperationBase* op =
        CXIMPGlobals::Instance()->OperationFactory()->CreateOperationL(
                NPresenceOps::EXIMPCleaner );
    op->BaseConstructL( reqId, NULL );
    CleanupStack::PushL( op );
    AddNewOperationL( *op ); // ownership was transferred to operation queue
    CleanupStack::Pop( op );
    

	// let every protocol host create their cleanup operation
	TInt count = iDataHostArray->Count() ;

	for(TInt i=0; i<count; i++)
		{
		TXIMPRequestId reqIdim = iReqIdBuilder.BuildRequestId();
		
		TInt cleanupid = iDataHostArray->At(i)->GetCleanupOperation();
	    CXIMPOperationBase* cleanupop =
	        		CXIMPGlobals::Instance()->OperationFactory()->CreateOperationL(cleanupid);
					
		if ( cleanupop )
			{
			cleanupop->BaseConstructL( reqIdim, NULL );
			CleanupStack::PushL( cleanupop );
			AddNewOperationL( *cleanupop ); // ownership was transferred to operation queue
			CleanupStack::Pop( cleanupop );
			}
		}
    }


// ---------------------------------------------------------------------------
// CXIMPHost::OperationForRequestId()
// ---------------------------------------------------------------------------
//
CXIMPOperationBase& CXIMPHost::OperationForRequestId( TXIMPRequestId aReqId )
    {
    __ASSERT_ALWAYS( iCurrentOperation, User::Panic( NXIMPPrivPanic::KCategory,
            NXIMPPrivPanic::ECurrentOperationNull ) );

    if ( iCurrentOperation->RequestId() != aReqId )
        {
        // we were not processing this request, panic the plugin (and the
        // server, as it is...)
        User::Panic( NXIMPPlugin::KCategory, NXIMPPlugin::EInvalidRequestId );
        }
    return *iCurrentOperation;
    }

// ---------------------------------------------------------------------------
// CXIMPHost::ProtocolUID()
// ---------------------------------------------------------------------------
//
TInt32 CXIMPHost::ProtocolUID()
	{
	return iProtocolUid;
	}


// ---------------------------------------------------------------------------
// CXIMPHost::SetProtocolHosts()
// ---------------------------------------------------------------------------
//
void CXIMPHost::SetProtocolHosts()
	{
	// enumurate through all the host plugins to set the host in corresponding protocol features 
	TInt count = iDataHostArray->Count() ;

	for(TInt i=0; i<count; i++)
		{
		iDataHostArray->At(i)->SetHostToProtocol() ;
		}
	}
// ---------------------------------------------------------------------------
// CXIMPHost::GetProtocolHost()
// ---------------------------------------------------------------------------
//
TAny* CXIMPHost::GetProtocolHost(TInt aInterfaceId)
	{
	// provide interface by enumurating and getting the one
	TAny* ptr = NULL ;
	TInt count = iDataHostArray->Count() ;

	for(TInt i=0; i<count; i++)
		{
		ptr = iDataHostArray->At(i)->GetInterface(aInterfaceId);
		if(ptr)
			{
				break ;		
			}
		}
	return ptr;
	}

void CXIMPHost::LoadDataHostArrayL() 
	{
	RImplInfoPtrArray pluginArray;		
	MProtocolDataHost::ListAllImplementationsL(pluginArray) ;	
	if ( pluginArray.Count() )
		{
		for( TInt i = 0; i < pluginArray.Count(); i++ )
			{
			MProtocolDataHost* implementation = NULL ;			
			CImplementationInformation* info = pluginArray[ i ];        
        	TUid id = info->ImplementationUid();        	
        	TRAPD(err, implementation = MProtocolDataHost::NewL( id,*this) );
        	CleanupStack::PushL(implementation);
        	if(err == KErrNone)
        		{
        		iDataHostArray->AppendL( implementation );
        		}
        	CleanupStack::Pop(implementation);
			}
		}
	pluginArray.ResetAndDestroy();

	}
// End of file
