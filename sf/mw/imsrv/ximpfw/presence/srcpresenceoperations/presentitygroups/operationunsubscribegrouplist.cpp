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
* Description:  Presentity group list unsubscription operation
 *
*/


#include <ximpprotocolconnection.h>
#include <protocolpresentitygroups.h>
#include <protocolpresencefeatures.h>

#include "operationunsubscribegrouplist.h"
#include "presencedatacacheimp.h"
#include "ximppanics.h"
#include "operationsynthesisesubscriptionevent.h"
#include "ximphost.h"
#include "ximppsccontextimp.h"
#include "ximpstatusimp.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"
// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationUnsubscribeGroupList::COperationUnsubscribeGroupList()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationUnsubscribeGroupList::COperationUnsubscribeGroupList()
    {
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribeGroupList::~COperationUnsubscribeGroupList()
// ---------------------------------------------------------------------------
//
COperationUnsubscribeGroupList::~COperationUnsubscribeGroupList()
    {
    if( iGroupList )
        {
        iGroupList->Close();
        }
    }

// ---------------------------------------------------------------------------
// COperationUnsubscribeGroupList::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeGroupList::ConstructL( const TDesC8& /*aParamPck*/ )
    {
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribeGroupList::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeGroupList::ProcessL()
    {
    TRACE(_L("COperationUnsubscribeGroupList::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();

    CGroupListSubscriptionItem& groupList =
                            iMyHost->PresenceDataAccess().PresenceDataCache().GroupListSubscriptionItemLC();
    CleanupStack::Pop(); // ownPresence
    iGroupList = &groupList;

    iSubscriptionStatus = iGroupList->SubscriptionStatus( iContext );

    MProtocolPresentityGroups& presentityGroups = iMyHost->GetConnection().ProtocolPresenceFeatures().PresentityGroups();

    switch( iSubscriptionStatus )
        {
        case CGroupListSubscriptionItem::ESubscribedForCtxOnly:
            {
            // Do subscribe
            presentityGroups.DoUnsubscribePresentityGroupListL( iReqId );
            break;
            }
        case CGroupListSubscriptionItem::ESubscribedForCtxAndOthers:
        case CGroupListSubscriptionItem::ENotSubscribedAtAll:
        case CGroupListSubscriptionItem::ESubscribedForOtherCtxOnly:
            {
            // Everything is ok. We are not handling updates through this one.
            // Already subscribed -> complete request.
            iMyHost->FakeCompleteRequest( iReqId, KErrNone );
            break;
            }

        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
        }
    }

// ---------------------------------------------------------------------------
// COperationUnsubscribeGroupList::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeGroupList::RequestCompletedL()
    {
    TRACE(_L("COperationUnsubscribeGroupList::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    if( iStatusObj->ResultCode() == KErrNone )
        {
        switch( iSubscriptionStatus )
            {
            case CGroupListSubscriptionItem::ESubscribedForCtxOnly:
            case CGroupListSubscriptionItem::ESubscribedForCtxAndOthers:
                {
                iGroupList->RemoveSubscriber( iContext );
                break;
                }
            case CGroupListSubscriptionItem::ENotSubscribedAtAll:
            case CGroupListSubscriptionItem::ESubscribedForOtherCtxOnly:
                {
                break;
                }
                
            XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );                
            }
        SynthesiseEventL();
        }
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribeGroupList::Type()
// ---------------------------------------------------------------------------
//
TInt
    COperationUnsubscribeGroupList::Type() const
    {
    return NPresenceOps::EPrUnsubscribeGroupList;
    }

// ---------------------------------------------------------------------------
// COperationUnsubscribeGroupList::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeGroupList::SynthesiseEventL()
    {
    COperationSynthesiseSubscriptionEvent* synthOp =
            new ( ELeave ) COperationSynthesiseSubscriptionEvent(
                             COperationSynthesiseSubscriptionEvent::EPresentityGroupList, ETrue );
    CleanupStack::PushL( synthOp );
    synthOp->BaseConstructL( TXIMPRequestId(), iContext );
    iMyHost->AddNewOperationL( *synthOp );
    CleanupStack::Pop( synthOp );
    }


// End of file
