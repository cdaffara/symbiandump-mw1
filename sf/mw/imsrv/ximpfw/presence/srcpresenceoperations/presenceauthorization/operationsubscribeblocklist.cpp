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
* Description:  Presence block list subscribe operation
 *
*/

#include <ximpprotocolconnection.h>
#include <protocolpresenceauthorization.h>
#include <protocolpresencefeatures.h>

#include "operationsubscribeblocklist.h"
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
// COperationSubscribeBlockList::COperationSubscribeBlockList()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationSubscribeBlockList::COperationSubscribeBlockList()
    {
    }


// ---------------------------------------------------------------------------
// COperationSubscribeBlockList::~COperationSubscribeBlockList()
// ---------------------------------------------------------------------------
//
COperationSubscribeBlockList::~COperationSubscribeBlockList()
    {
    if( iBlockList )
        {
        iBlockList->Close();
        }
    }

// ---------------------------------------------------------------------------
// COperationSubscribeBlockList::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationSubscribeBlockList::ConstructL( const TDesC8& /*aParamPck*/ )
    {
    }


// ---------------------------------------------------------------------------
// COperationSubscribeBlockList::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationSubscribeBlockList::ProcessL()
    {
    TRACE(_L("COperationSubscribeBlockList::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();

    CBlockListSubscriptionItem& subItem =
                            iMyHost->PresenceDataAccess().PresenceDataCache().BlockListSubscriptionItemLC();
    CleanupStack::Pop(); // subItem
    iBlockList = &subItem;

    iSubscriptionStatus = iBlockList->SubscriptionStatus( iContext );

    MProtocolPresenceAuthorization& authorization = iMyHost->GetConnection().ProtocolPresenceFeatures().PresenceAuthorization();

    switch( iSubscriptionStatus )
        {
        case CBlockListSubscriptionItem::ENotSubscribedAtAll:
            {
            authorization.DoSubscribePresenceBlockListL( iReqId );
            break;
            }

        case CBlockListSubscriptionItem::ESubscribedForOtherCtxOnly:
        case CBlockListSubscriptionItem::ESubscribedForCtxOnly:
        case CBlockListSubscriptionItem::ESubscribedForCtxAndOthers:
            {
            iMyHost->FakeCompleteRequest( iReqId, KErrNone );
            break;
            }
        
        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
        }
    }


// ---------------------------------------------------------------------------
// COperationSubscribeBlockList::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationSubscribeBlockList::RequestCompletedL()
    {
    TRACE(_L("COperationSubscribeBlockList::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    if( iStatusObj->ResultCode() == KErrNone )
        {
        switch( iSubscriptionStatus )
            {
            case CBlockListSubscriptionItem::ENotSubscribedAtAll:
            case CBlockListSubscriptionItem::ESubscribedForOtherCtxOnly:
                {
                iBlockList->AddSubscriberL( iContext );
                break;
                }
                
            case CBlockListSubscriptionItem::ESubscribedForCtxOnly:
            case CBlockListSubscriptionItem::ESubscribedForCtxAndOthers:
                {
                break;
                }
                
            XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
            }
        SynthesiseEventL();
        }
    }


// ---------------------------------------------------------------------------
// COperationSubscribeBlockList::Type()
// ---------------------------------------------------------------------------
//
TInt
    COperationSubscribeBlockList::Type() const
    {
    return NPresenceOps::EPrSubscribeBlockList;
    }

// ---------------------------------------------------------------------------
// COperationSubscribeBlockList::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationSubscribeBlockList::SynthesiseEventL()
    {
    TBool force =
    ( iSubscriptionStatus == CBlockListSubscriptionItem::ESubscribedForCtxOnly ||
      iSubscriptionStatus == CBlockListSubscriptionItem::ESubscribedForCtxAndOthers ) ? ETrue : EFalse;

    COperationSynthesiseSubscriptionEvent* synthOp =
            new ( ELeave ) COperationSynthesiseSubscriptionEvent(
                             COperationSynthesiseSubscriptionEvent::EPresenceBlockList,
                             force );
    CleanupStack::PushL( synthOp );
    synthOp->BaseConstructL( TXIMPRequestId(), iContext );
    iMyHost->AddNewOperationL( *synthOp );
    CleanupStack::Pop( synthOp );
    }



// End of file
