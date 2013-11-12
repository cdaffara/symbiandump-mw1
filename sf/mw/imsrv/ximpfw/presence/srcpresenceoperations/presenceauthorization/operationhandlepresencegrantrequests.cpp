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
* Description: handle of operational grant presence request
 *
*/


#include "operationhandlepresencegrantrequests.h"
#include "ximpobjecthelpers.h"
#include "grantrequestlistsubscriptionitem.h"
#include "presencedatacacheimp.h"
#include "presencegrantrequestinfoimp.h"
#include "ximpidentityimp.h"
#include "ximphost.h"
#include "ximpstatusimp.h"
#include "ximppanics.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximpobjectcollectionimp.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"
// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationHandlePresenceGrantRequests::~COperationHandlePresenceGrantRequests()
// ---------------------------------------------------------------------------
//
COperationHandlePresenceGrantRequests::~COperationHandlePresenceGrantRequests()
    {
    delete iGrantReqList;
    delete iObsoletedId;
    delete iReceivedReq;
    }

// ---------------------------------------------------------------------------
// COperationHandlePresenceGrantRequests::COperationHandlePresenceGrantRequests()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationHandlePresenceGrantRequests::COperationHandlePresenceGrantRequests(
        TInt aMyType )
    {
    iMyType = aMyType;
    }

// ---------------------------------------------------------------------------
// COperationHandlePresenceGrantRequests::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationHandlePresenceGrantRequests::ConstructL( const TDesC8& /*aParamPck*/ )
    {
    iGrantReqList = new( ELeave ) RPrGrntReqInfoImpArray;
    }

// ---------------------------------------------------------------------------
// COperationHandlePresenceGrantRequests::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationHandlePresenceGrantRequests::ProcessL()
    {
    TRACE(_L("COperationHandlePresenceGrantRequests::ProcessL()") );
    CXIMPOperationBase::ProcessL();

    CGrantRequestListSubscriptionItem& subItem =
        iMyHost->PresenceDataAccess().PresenceDataCache().GrantRequestListSubscriptionItemLC();
    // clean out temporary lists
    subItem.Clean();
    switch ( iMyType )
        {
        case NPresenceOps::EPrHandlePresenceGrantRequestList:
            {
            MXIMPBase* object = NULL;
            iObjCollection->GetByType( object, MXIMPObjectCollection::KInterfaceId );
            if( object )
                {
                CXIMPObjectCollectionImp* objectCollection = 
                        TXIMPGetImpClassOrPanic< CXIMPObjectCollectionImp >::From( *object );
                CleanupStack::PushL( objectCollection );
                TLinearOrder< CPresenceGrantRequestInfoImp > order( CPresenceGrantRequestInfoImp::IdLinearOrder );
                TXIMPObjectMover< MPresenceGrantRequestInfo, CPresenceGrantRequestInfoImp >::
                                            MoveFromCollectionToArrayL( *objectCollection, *iGrantReqList, order );
                CleanupStack::PopAndDestroy( objectCollection );        
                }
            else
                {
                User::Leave( KErrArgument );
                }
            subItem.SetNewListL( iGrantReqList );
            iGrantReqList = NULL;
            // ownership was transferred            
            break;
            }

        case NPresenceOps::EPrHandlePresenceGrantRequestReceived:
            {
            MXIMPBase* object = NULL;
            iObjCollection->GetByType( object, MPresenceGrantRequestInfo::KInterfaceId );
            if( object )
                {
                iReceivedReq = 
                    TXIMPGetImpClassOrPanic< CPresenceGrantRequestInfoImp >::From( *object );
                }
            else
                {
                User::Leave( KErrArgument );
                }            
            subItem.SetReceivedRequestL( iReceivedReq );
            iReceivedReq = NULL; // ownership was transferred
            break;
            }

        case NPresenceOps::EPrHandlePresenceGrantRequestObsoleted:
            {
            MXIMPBase* object = NULL;
            iObjCollection->GetByType( object, MXIMPIdentity::KInterfaceId );
            if( object )
                {
                iObsoletedId = TXIMPGetImpClassOrPanic< CXIMPIdentityImp >::From( *object );
                }
            else
                {
                User::Leave( KErrArgument );
                }            
            subItem.SetObsoletedRequestL( iObsoletedId );
            iObsoletedId = NULL; // ownership was transferred
            break;
            }

        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidOperationType );
        }

    // synthesize the event to all contexts
    TRAPD( err, subItem.SynthesiseSubscriptionEventToAllCtxsL() );

    CleanupStack::PopAndDestroy(); // subItem

    err = KErrNone;
    iMyHost->HandleAdaptationRequestCompleted( iReqId, err );
    }

// ---------------------------------------------------------------------------
// COperationHandlePresenceGrantRequests::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationHandlePresenceGrantRequests::RequestCompletedL()
    {
    TRACE(_L("COperationHandlePresenceGrantRequests::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    // no special handling for error situations
    }


// ---------------------------------------------------------------------------
// COperationHandlePresenceGrantRequests::Type()
// ---------------------------------------------------------------------------
//
TInt COperationHandlePresenceGrantRequests::Type() const
    {
    return iMyType;
    }

// End of file
