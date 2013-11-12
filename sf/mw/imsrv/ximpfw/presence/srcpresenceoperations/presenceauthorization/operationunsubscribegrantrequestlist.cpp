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
* Description:  Presence grant request list unsubscription operation
 *
*/


#include <ximpprotocolconnection.h>
#include <protocolpresenceauthorization.h>
#include <protocolpresencefeatures.h>

#include "operationunsubscribegrantrequestlist.h"
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
// COperationUnsubscribeGrantRequestList::COperationUnsubscribeGrantRequestList()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationUnsubscribeGrantRequestList::COperationUnsubscribeGrantRequestList()
    {
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribeGrantRequestList::~COperationUnsubscribeGrantRequestList()
// ---------------------------------------------------------------------------
//
COperationUnsubscribeGrantRequestList::~COperationUnsubscribeGrantRequestList()
    {
    if( iGrantRequestList )
        {
        iGrantRequestList->Close();
        }
    }

// ---------------------------------------------------------------------------
// COperationUnsubscribeGrantRequestList::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeGrantRequestList::ConstructL( const TDesC8& /*aParamPck*/ )
    {
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribeGrantRequestList::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeGrantRequestList::ProcessL()
    {
    TRACE(_L("COperationUnsubscribeGrantRequestList::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();

    CGrantRequestListSubscriptionItem& grantReqList =
                            iMyHost->PresenceDataAccess().PresenceDataCache().GrantRequestListSubscriptionItemLC();
    CleanupStack::Pop(); // grantReqList
    iGrantRequestList = &grantReqList;

    iSubscriptionStatus = iGrantRequestList->SubscriptionStatus( iContext );

    MProtocolPresenceAuthorization& authorization = iMyHost->GetConnection().ProtocolPresenceFeatures().PresenceAuthorization();

    switch( iSubscriptionStatus )
        {
        case CGrantRequestListSubscriptionItem::ESubscribedForCtxOnly:
            {
            authorization.DoUnsubscribePresenceGrantRequestListL( iReqId );
            break;
            }

        case CGrantRequestListSubscriptionItem::ESubscribedForCtxAndOthers:
            {
            iMyHost->FakeCompleteRequest( iReqId, KErrNone );
            break;
            }

        case CGrantRequestListSubscriptionItem::ENotSubscribedAtAll:
        case CGrantRequestListSubscriptionItem::ESubscribedForOtherCtxOnly:
            {
            iMyHost->FakeCompleteRequest( iReqId, KErrNone );
            break;
            }

        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
        }
    }

// ---------------------------------------------------------------------------
// COperationUnsubscribeGrantRequestList::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeGrantRequestList::RequestCompletedL()
    {
    TRACE(_L("COperationUnsubscribeGrantRequestList::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    if( iStatusObj->ResultCode() == KErrNone )
        {
        switch( iSubscriptionStatus )
            {
            case CGrantRequestListSubscriptionItem::ESubscribedForCtxOnly:
            case CGrantRequestListSubscriptionItem::ESubscribedForCtxAndOthers:
                {
                iGrantRequestList->RemoveSubscriber( iContext );
                break;
                }
            case CGrantRequestListSubscriptionItem::ENotSubscribedAtAll:
            case CGrantRequestListSubscriptionItem::ESubscribedForOtherCtxOnly:
                {
                break;
                }

            XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
            }
        SynthesiseEventL();
        }
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribeGrantRequestList::Type()
// ---------------------------------------------------------------------------
//
TInt
    COperationUnsubscribeGrantRequestList::Type() const
    {
    return NPresenceOps::EPrUnsubscribePresenceGrantRequestList;
    }

// ---------------------------------------------------------------------------
// COperationUnsubscribeGrantRequestList::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeGrantRequestList::SynthesiseEventL()
    {
    COperationSynthesiseSubscriptionEvent* synthOp =
            new ( ELeave ) COperationSynthesiseSubscriptionEvent(
                COperationSynthesiseSubscriptionEvent::EPresenceGrantRequestList,
                EFalse );
    CleanupStack::PushL( synthOp );
    synthOp->BaseConstructL( TXIMPRequestId(), iContext );
    iMyHost->AddNewOperationL( *synthOp );
    CleanupStack::Pop( synthOp );
    }


// End of file
