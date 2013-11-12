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
* Description:  Client context data storage.
*
*/

#include <ximpprotocolconnection.h>
#include <ximpidentity.h>

#include "ximppsccontextimp.h"
#include "ximpcontexteventqueue.h"
#include "presenceinfoimp.h"
#include "presenceinfofilterimp.h"
#include "ximpoperationbase.h"
#include "ximpserviceinfoimp.h"
#include "ximpfeatureinfoimp.h"
#include "ximpoperationdefs.h"
#include "ximphostimp.h"
#include "ximpglobals.h"
#include "ximppanics.h"
#include "ximpoperationfactory.h"
#include "ximphostmanager.h"
#include "ximpcontextclientinfoimp.h"
#include "presencefilterstore.h"
#include "ximprequestcompleteeventimp.h"

#include "ximptrace.h"

const TInt KXIMPFeatGranularity = 5;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CXIMPPscContext::CXIMPHostManager()
// ---------------------------------------------------------------------------
//
CXIMPPscContext::CXIMPPscContext( TUint32 aSessionId )
: iSessionId( aSessionId )
    {
    }


// ---------------------------------------------------------------------------
// CXIMPPscContext::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPPscContext::ConstructL()
    {
    iEventQueue = CXIMPContextEventQueue::NewL();
    iEventQueue->SetSessionIdOfQueue( iSessionId );
    iPresentityFilterStore = CPresenceFilterStore::NewL();    
    iPresentityGroupFilterStore = CPresenceFilterStore::NewL();    
    iGrantFilterStore = CPresenceFilterStore::NewL();    
    iGrantGroupFilterStore = CPresenceFilterStore::NewL();    
    }

// ---------------------------------------------------------------------------
// CXIMPPscContext::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPPscContext* CXIMPPscContext::NewL( TUint32 aSessionId )
    {
    CXIMPPscContext* self = new( ELeave ) CXIMPPscContext( aSessionId );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPPscContext::~CXIMPPscContext()
// ---------------------------------------------------------------------------
//
CXIMPPscContext::~CXIMPPscContext()
    {
    if( iEventQueue )
        {
        iEventQueue->StopConsuming();
        }

    delete iEventQueue;

    // destroy all cached operations
    delete iCachedUnbind;

    if ( iHost )
        {
        iHost->Close();
        }

    delete iOwnPif;
    delete iServiceInfo;
    delete iClientInfo;
    delete iPresentityFilterStore;    
    delete iPresentityGroupFilterStore;    
    delete iGrantFilterStore;    
    delete iGrantGroupFilterStore;
    delete iGrantEveryonePif;
    }

// ---------------------------------------------------------------------------
// CXIMPPscContext::EventQueue()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPContextEventQueue& CXIMPPscContext::EventQueue() const
    {
    return *iEventQueue;
    }

// ---------------------------------------------------------------------------
// CXIMPPscContext::SessionId()
// ---------------------------------------------------------------------------
//
EXPORT_C TUint32 CXIMPPscContext::SessionId() const
    {
    return iSessionId;
    }

// ---------------------------------------------------------------------------
// CXIMPPscContext::OrderOfContexts()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CXIMPPscContext::OrderOfContexts( const CXIMPPscContext& aFirst,
                                                const CXIMPPscContext& aSecond )
    {
    TUint32 firstSessionId = aFirst.SessionId();
    TUint32 secondSessionId = aSecond.SessionId();
    if( firstSessionId < secondSessionId )
        {
        return -1;
        }
    else if( firstSessionId == secondSessionId )
        {
        return 0;
        }
    else
        {
        return 1;
        }
    }

// ---------------------------------------------------------------------------
// CXIMPPscContext::PresenceInfoFilter
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFilterImp* CXIMPPscContext::PresenceInfoFilter( 
                                                    MXIMPPscContext::TFilterStoreType aTypeOfFilter,
                                                    const CXIMPIdentityImp* aIdentity /*= NULL*/ ) const
    {
    switch ( aTypeOfFilter )
        {
        case EOwnPresenceFilter:
            {
            return iOwnPif;
            }
        case EPresentityPresenceFilter:
            {
            return iPresentityFilterStore->FilterForIdentity( *aIdentity );
            }
        case EPresentityGroupMemberPresenceFilter:
            {
            return iPresentityGroupFilterStore->FilterForIdentity( *aIdentity );
            }
        case EGrantPresenceToEveryoneFilter:
            {
            return iGrantEveryonePif;
            }
        case EGrantPresenceToPresentityFilter:
            {
            return iPresentityGroupFilterStore->FilterForIdentity( *aIdentity );
            }
        case EGrantPresenceToGroupMemberFilter:
            {
            return iPresentityGroupFilterStore->FilterForIdentity( *aIdentity );
            }
        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidFilterType );            
        }
    return NULL;
    }

// ---------------------------------------------------------------------------
// CXIMPPscContext::SetPresenceInfoFilterL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPPscContext::SetPresenceInfoFilterL( 
                                              MXIMPPscContext::TFilterStoreType aTypeOfFilter,
                                              CPresenceInfoFilterImp* aPif,
                                              const CXIMPIdentityImp* aIdentity /*= NULL*/ )
    {
    CPresenceFilterStore* filterStore = NULL;
    switch ( aTypeOfFilter )
        {
        case EOwnPresenceFilter:
            {
            delete iOwnPif;
            iOwnPif = aPif;
            break;
            }
        case EPresentityPresenceFilter:
            {
            filterStore = iPresentityFilterStore;
            break;
            }
        case EPresentityGroupMemberPresenceFilter:
            {
            filterStore = iPresentityGroupFilterStore;
            break;
            }
        case EGrantPresenceToEveryoneFilter:
            {
            delete iGrantEveryonePif;
            iGrantEveryonePif = aPif;
            break;
            }
        case EGrantPresenceToPresentityFilter:
            {
            filterStore = iPresentityGroupFilterStore;
            break;
            }
        case EGrantPresenceToGroupMemberFilter:
            {
            filterStore = iPresentityGroupFilterStore;
            break;
            }
        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidFilterType );            
        }

    if( filterStore )
        {        
        filterStore->Disassociate( *aIdentity );
        if( aPif )
            {
            filterStore->AssociateL( *aIdentity, aPif );       
            }
        }
    }

// ---------------------------------------------------------------------------
// CXIMPPscContext::GetCachedUnbind()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPOperationBase* CXIMPPscContext::GetCachedUnbind()
    {
    CXIMPOperationBase* retVal = iCachedUnbind;
    iCachedUnbind = NULL;
    return retVal;
    }

// ---------------------------------------------------------------------------
// CXIMPPscContext::SetServiceInfo()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPPscContext::SetServiceInfo( CXIMPServiceInfoImp* aServiceInfo )
    {
    delete iServiceInfo;
    iServiceInfo = aServiceInfo;
    }

// ---------------------------------------------------------------------------
// CXIMPPscContext::SetServiceInfo()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPPscContext::SetClientInfo( CXIMPContextClientInfoImp* aClientInfo )
    {
    delete iClientInfo;
    iClientInfo = aClientInfo;
    }

// ---------------------------------------------------------------------------
// CXIMPPscContext::ServiceInfo()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPServiceInfoImp* CXIMPPscContext::ServiceInfo() const
    {
    return iServiceInfo;
    }

// ---------------------------------------------------------------------------
// CXIMPPscContext::DestroyCachedUnbind()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPPscContext::DestroyCachedUnbind()
    {
    if( iCachedUnbind )
        {
        iEventQueue->RemoveRequestCompleteEvent( *iCachedUnbind->GetReqCompleteEvent() );
        }    
    delete iCachedUnbind;
    iCachedUnbind = NULL;
    }

// ---------------------------------------------------------------------------
// CXIMPPscContext::AppendToOpQueueL()
// ---------------------------------------------------------------------------
//
EXPORT_C TXIMPRequestId CXIMPPscContext::AppendToOpQueueUpdateReqIdL(
        CXIMPOperationBase* aOperation )
    {
    iReqIdBuilder.SetSessionId( iSessionId );
    iReqIdBuilder.SetRequestId( iOperationIndex );
    TXIMPRequestId reqId = iReqIdBuilder.BuildRequestId();
    
    iOperationIndex++;
    aOperation->SetReqId( reqId );

    AppendToOpQueueL( aOperation );
    return reqId;
    }

// ---------------------------------------------------------------------------
// CXIMPPscContext::AppendToOpQueueL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPPscContext::AppendToOpQueueL(
        CXIMPOperationBase* aOperation )
    {
    __ASSERT_ALWAYS( iHost, User::Panic( NXIMPPrivPanic::KCategory, NXIMPPrivPanic::EContextHasNoHost ) );
    iHost->AddNewOperationL( *aOperation );
    }

// ---------------------------------------------------------------------------
// CXIMPPscContext::AddNewOperationL()
// ---------------------------------------------------------------------------
//
EXPORT_C TXIMPRequestId CXIMPPscContext::AddNewOperationL(
        TInt aOperation,
        const TDesC8& aReqParams )
    {
    iReqIdBuilder.SetSessionId( iSessionId );
    iReqIdBuilder.SetRequestId( iOperationIndex );
    TXIMPRequestId reqId = iReqIdBuilder.BuildRequestId();

    iOperationIndex++;
    // XXX should the index++ be after Pop() below?

    // check what kind of operation is offered and create the object
    CXIMPOperationBase* op =
        CXIMPGlobals::Instance()->OperationFactory()->CreateOperationL( aOperation );
    // we have op at this point or we've panicked
    CleanupStack::PushL( op );
    op->BaseConstructL( reqId, this );
    op->ConstructL( aReqParams );
    TRACE_1( _L("New request from client \"%S\""), &ClientInfo()->ClientId().Identity() );

    // Create unbind skeleton in advance for teardown bind.
    if( NXIMPOps::EXIMPBindContext == aOperation
        || NXIMPOps::EXIMPBindContext2 == aOperation )
        {
        // Locate connection from adaptation
        ResolveHostL();

        CXIMPOperationBase* unbindOp =
            CXIMPGlobals::Instance()->OperationFactory()->CreateOperationL( NXIMPOps::EXIMPUnbindContext );

		iReqIdBuilder.SetRequestId( iOperationIndex );
        TXIMPRequestId unbindReq = iReqIdBuilder.BuildRequestId();
        iOperationIndex++;

        unbindOp->BaseConstructL( unbindReq, this );
        unbindOp->ConstructL( KNullDesC8 );
        iCachedUnbind = unbindOp;
        iHost->AddEventL( *iCachedUnbind->GetReqCompleteEvent(), this );
        }
    __ASSERT_ALWAYS( iHost, User::Panic( NXIMPPrivPanic::KCategory, NXIMPPrivPanic::EContextHasNoHost ) );
    
    iHost->AddEventL( *op->GetReqCompleteEvent(), this );
    
    iHost->AddNewOperationL( *op );
    CleanupStack::Pop(); // op
    return reqId;
    }

// ---------------------------------------------------------------------------
// CXIMPPscContext::FeaturesForSessionLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPFeatureInfoImp* CXIMPPscContext::FeaturesForSessionLC()
    {
    CXIMPFeatureInfoImp* feats = CXIMPFeatureInfoImp::NewLC();

    CDesC8ArraySeg* arr = new ( ELeave ) CDesC8ArraySeg( KXIMPFeatGranularity );
    CleanupStack::PushL( arr );

/*    if ( GetStateOfHost() == NXIMPHost::EXIMPNotConnected )
        {
        // feature fetching not allowed. for feature fetching to be
        // allowed there has to have been a bind operation
        User::Leave( KErrNotReady );
        }*/

    iHost->GetConnection().GetSupportedFeaturesL( *arr );

    for( TInt i = 0; i < arr->Count(); i++ )
        {
        feats->AddFeatureL( arr->MdcaPoint( i ) );
        }
    CleanupStack::PopAndDestroy( arr );

    return feats;
    }

// ---------------------------------------------------------------------------
// CXIMPPscContext::ClientInfo()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPContextClientInfoImp* CXIMPPscContext::ClientInfo() const
    {
    return iClientInfo;
    }

// ---------------------------------------------------------------------------
// CXIMPPscContext::IsHostAssigned()
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CXIMPPscContext::IsHostAssigned() const
    {
    return iHost != NULL;
    }
    
// ---------------------------------------------------------------------------
// CXIMPPscContext::ResolveHostForOperationL()
// ---------------------------------------------------------------------------
//
void CXIMPPscContext::ResolveHostL()
    {
    TRACE(_L("CXIMPPscContext::ResolveHostL() : begin" ) );
    
    CXIMPHost* host = CXIMPGlobals::Instance()->HostManager()->AcquireHostForContextLC( *this );
    
    TRACE_3(_L("CXIMPPscContext::ResolveHostL() : this =%d, host =%d , iHost=%d" ), this, host, iHost );
    
    if( !iHost )
        {
        host->RegisterSessionL( this );
        iHost = host;
        CleanupStack::Pop(); //host
        }
    else
        {
        CleanupStack::PopAndDestroy(); // host
        }
        
    TRACE(_L("CXIMPPscContext::ResolveHostL() : end" ) );        
    }
// End of file
