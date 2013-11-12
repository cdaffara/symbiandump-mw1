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
* Description:  Presence grant request list subscribe operation
 *
*/


#include <ximpprotocolconnection.h>
#include <protocolpresenceauthorization.h>
#include <protocolpresencefeatures.h>

#include "operationsubscribegrantrequestlist.h"
#include "presencedatacacheimp.h"
#include "ximppanics.h"
#include "operationsynthesisesubscriptionevent.h"
#include "ximphost.h"
#include "ximpstatusimp.h"
#include "ximppsccontextimp.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"
// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationSubscribeGrantRequestList::COperationSubscribeGrantRequestList()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationSubscribeGrantRequestList::COperationSubscribeGrantRequestList()
    {
    }


// ---------------------------------------------------------------------------
// COperationSubscribeGrantRequestList::~COperationSubscribeGrantRequestList()
// ---------------------------------------------------------------------------
//
COperationSubscribeGrantRequestList::~COperationSubscribeGrantRequestList()
    {
    if( iGrantRequestList )
        {
        iGrantRequestList->Close();
        }
    }

// ---------------------------------------------------------------------------
// COperationSubscribeGrantRequestList::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationSubscribeGrantRequestList::ConstructL( const TDesC8& /*aParamPck*/ )
    {
    }


// ---------------------------------------------------------------------------
// COperationSubscribeGrantRequestList::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationSubscribeGrantRequestList::ProcessL()
    {
    TRACE(_L("COperationSubscribeGrantRequestList::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();

    CGrantRequestListSubscriptionItem& grantReqList =
                            iMyHost->PresenceDataAccess().PresenceDataCache().GrantRequestListSubscriptionItemLC();
    CleanupStack::Pop(); // grantReqList
    iGrantRequestList = &grantReqList;

    iSubscriptionStatus = iGrantRequestList->SubscriptionStatus( iContext );
    MProtocolPresenceAuthorization& authorization = iMyHost->GetConnection().ProtocolPresenceFeatures().PresenceAuthorization();

    switch( iSubscriptionStatus )
        {
        case CGrantRequestListSubscriptionItem::ENotSubscribedAtAll:
            {
            authorization.DoSubscribePresenceGrantRequestListL( iReqId );
            break;
            }
        case CGrantRequestListSubscriptionItem::ESubscribedForOtherCtxOnly:
        case CGrantRequestListSubscriptionItem::ESubscribedForCtxOnly:
        case CGrantRequestListSubscriptionItem::ESubscribedForCtxAndOthers:
            {
            iMyHost->FakeCompleteRequest( iReqId, KErrNone );
            break;
            }

        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );            
        }
    }


// ---------------------------------------------------------------------------
// COperationSubscribeGrantRequestList::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationSubscribeGrantRequestList::RequestCompletedL()
    {
    TRACE(_L("COperationSubscribeGrantRequestList::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    if( iStatusObj->ResultCode() == KErrNone )
        {
        switch( iSubscriptionStatus )
            {
            case CGrantRequestListSubscriptionItem::ENotSubscribedAtAll:
            case CGrantRequestListSubscriptionItem::ESubscribedForOtherCtxOnly:
                {
                iGrantRequestList->AddSubscriberL( iContext );
                break;
                }
            case CGrantRequestListSubscriptionItem::ESubscribedForCtxOnly:
            case CGrantRequestListSubscriptionItem::ESubscribedForCtxAndOthers:
                {
                break;
                }

            XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
            }

        SynthesiseEventL();
        }
    }


// ---------------------------------------------------------------------------
// COperationSubscribeGrantRequestList::Type()
// ---------------------------------------------------------------------------
//
TInt
    COperationSubscribeGrantRequestList::Type() const
    {
    return NPresenceOps::EPrSubscribePresenceGrantRequestList;
    }

// ---------------------------------------------------------------------------
// COperationSubscribeGrantRequestList::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationSubscribeGrantRequestList::SynthesiseEventL()
    {
    TBool force =
    ( iSubscriptionStatus == CGrantRequestListSubscriptionItem::ESubscribedForCtxOnly ||
      iSubscriptionStatus == CGrantRequestListSubscriptionItem::ESubscribedForCtxAndOthers ) ? ETrue : EFalse;

    COperationSynthesiseSubscriptionEvent* synthOp =
            new ( ELeave ) COperationSynthesiseSubscriptionEvent(
                             COperationSynthesiseSubscriptionEvent::EPresenceGrantRequestList,
                             force );
    CleanupStack::PushL( synthOp );
    synthOp->BaseConstructL( TXIMPRequestId(), iContext );
    iMyHost->AddNewOperationL( *synthOp );
    CleanupStack::Pop( synthOp );
    }



// End of file
