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
* Description:  Presence block list unsubscription operation
 *
*/


#include <ximpprotocolconnection.h>
#include <protocolpresenceauthorization.h>
#include <protocolpresencefeatures.h>

#include "operationunsubscribeblocklist.h"
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
// COperationUnsubscribeBlockList::COperationUnsubscribeBlockList()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationUnsubscribeBlockList::COperationUnsubscribeBlockList()
    {
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribeBlockList::~COperationUnsubscribeBlockList()
// ---------------------------------------------------------------------------
//
COperationUnsubscribeBlockList::~COperationUnsubscribeBlockList()
    {
    if( iBlockList )
        {
        iBlockList->Close();
        }
    }

// ---------------------------------------------------------------------------
// COperationUnsubscribeBlockList::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeBlockList::ConstructL( const TDesC8& /*aParamPck*/ )
    {
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribeBlockList::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeBlockList::ProcessL()
    {
    TRACE(_L("COperationUnsubscribeBlockList::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();

    CBlockListSubscriptionItem& subItem =
                            iMyHost->PresenceDataAccess().PresenceDataCache().BlockListSubscriptionItemLC();
    CleanupStack::Pop(); // subItem
    iBlockList = &subItem;

    iSubscriptionStatus = iBlockList->SubscriptionStatus( iContext );

    MProtocolPresenceAuthorization& authorization = iMyHost->GetConnection().ProtocolPresenceFeatures().PresenceAuthorization();


    switch( iSubscriptionStatus )
        {
        case CBlockListSubscriptionItem::ESubscribedForCtxOnly:
            {
            authorization.DoUnsubscribePresenceBlockListL( iReqId );
            break;
            }
            
        case CBlockListSubscriptionItem::ESubscribedForCtxAndOthers:
            {
            iMyHost->FakeCompleteRequest( iReqId, KErrNone );
            break;
            }
            
        case CBlockListSubscriptionItem::ENotSubscribedAtAll:
        case CBlockListSubscriptionItem::ESubscribedForOtherCtxOnly:
            {
            iMyHost->FakeCompleteRequest( iReqId, KErrNone );
            break;
            }
            
        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
        }
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribeBlockList::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeBlockList::RequestCompletedL()
    {
    TRACE(_L("COperationUnsubscribeBlockList::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    if( iStatusObj->ResultCode() == KErrNone )
        {
        switch( iSubscriptionStatus )
            {
            case CBlockListSubscriptionItem::ESubscribedForCtxOnly:
            case CBlockListSubscriptionItem::ESubscribedForCtxAndOthers:
                {
                iBlockList->RemoveSubscriber( iContext );
                break;
                }
            case CBlockListSubscriptionItem::ENotSubscribedAtAll:
            case CBlockListSubscriptionItem::ESubscribedForOtherCtxOnly:
                {
                break;
                }
                
            XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );                
            }
        SynthesiseEventL();
        }
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribeBlockList::Type()
// ---------------------------------------------------------------------------
//
TInt
    COperationUnsubscribeBlockList::Type() const
    {
    return NPresenceOps::EPrUnsubscribeBlockList;
    }

// ---------------------------------------------------------------------------
// COperationUnsubscribeBlockList::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeBlockList::SynthesiseEventL()
    {
    COperationSynthesiseSubscriptionEvent* synthOp =
            new ( ELeave ) COperationSynthesiseSubscriptionEvent(
                COperationSynthesiseSubscriptionEvent::EPresenceBlockList,
                EFalse );
    CleanupStack::PushL( synthOp );
    synthOp->BaseConstructL( TXIMPRequestId(), iContext );
    iMyHost->AddNewOperationL( *synthOp );
    CleanupStack::Pop( synthOp );
    }


// End of file
