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
* Description:  Presentity group members presence unsubscription operation
 *
*/


#include <ximpprotocolconnection.h>
#include <protocolpresencewatching.h>
#include <protocolpresencefeatures.h>

#include "operationunsubscribepresentitygroupmemberspresence.h"
#include "ximpobjecthelpers.h"
#include "ximpidentityimp.h"
#include "presencedatacacheimp.h"
#include "presenceinfofilterimp.h"
#include "operationsynthesisesubscriptionevent.h"
#include "ximphost.h"
#include "ximpstatusimp.h"
#include "ximppsccontextimp.h"
#include "ximppanics.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"
// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationUnsubscribePresentityGroupMembersPresence::COperationUnsubscribePresentityGroupMembersPresence()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationUnsubscribePresentityGroupMembersPresence::COperationUnsubscribePresentityGroupMembersPresence()
    {
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribePresentityGroupMembersPresence::~COperationUnsubscribePresentityGroupMembersPresence()
// ---------------------------------------------------------------------------
//
COperationUnsubscribePresentityGroupMembersPresence::~COperationUnsubscribePresentityGroupMembersPresence()
    {
    delete iGroupId;
    if( iSubItem )
        {
        iSubItem->Close();
        }    
    }

// ---------------------------------------------------------------------------
// COperationUnsubscribePresentityGroupMembersPresence::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribePresentityGroupMembersPresence::ConstructL( const TDesC8& aParamPck )
    {
    iGroupId = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iGroupId );
    TXIMPObjectPacker< CXIMPIdentityImp >::UnPackL( *iGroupId, aParamPck );
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribePresentityGroupMembersPresence::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribePresentityGroupMembersPresence::ProcessL()
    {
    TRACE(_L("COperationUnsubscribePresentityGroupMembersPresence::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();
    
    CPresentityGroupMembersPresenceSubscriptionItem& subItem =
                iMyHost->PresenceDataAccess().PresenceDataCache().PresentityGroupMembersPresenceSubscriptionItemLC( *iGroupId );
    CleanupStack::Pop(); // subItem
    iSubItem = &subItem;

    iSubscriptionStatus = iSubItem->SubscriptionStatus( iContext );

    MProtocolPresenceWatching& watching = iMyHost->GetConnection().ProtocolPresenceFeatures().PresenceWatching();

    switch( iSubscriptionStatus )
        {
        case CPresentityGroupMembersPresenceSubscriptionItem::ESubscribedForCtxOnly:
            {
            watching.DoUnsubscribePresentityGroupMembersPresenceL( *iGroupId, iReqId );
            iSubItem->RemoveSubscriber( iContext );
            break;
            }
            
        case CPresentityGroupMembersPresenceSubscriptionItem::ESubscribedForCtxAndOthers:
            {
            CPresenceInfoFilterImp* pifWithoutCtx =
                iSubItem->CollectSubscriptionPifWithoutCtxL( iContext );
            CleanupStack::PushL( pifWithoutCtx );

            if ( iSubItem->SubscriptionPif().Contains( *pifWithoutCtx ) )
                {
                watching.DoUpdatePresentityGroupMembersPresenceSubscriptionPifL( *iGroupId, *pifWithoutCtx, iReqId );
                }
            else
                {
                iMyHost->FakeCompleteRequest( iReqId, KErrNone );
                }

            CleanupStack::PopAndDestroy( pifWithoutCtx );
            break;
            }
            
        case CPresentityGroupMembersPresenceSubscriptionItem::ENotSubscribedAtAll:
        case CPresentityGroupMembersPresenceSubscriptionItem::ESubscribedForOtherCtxOnly:
            {
            iMyHost->FakeCompleteRequest( iReqId, KErrNone );
            break;
            }
            
        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
        }    
    }

// ---------------------------------------------------------------------------
// COperationUnsubscribePresentityGroupMembersPresence::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribePresentityGroupMembersPresence::RequestCompletedL()
    {
    TRACE(_L("COperationUnsubscribePresentityGroupMembersPresence::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    if( iStatusObj->ResultCode() == KErrNone )
        {
        iContext->SetPresenceInfoFilterL( MXIMPPscContext::EPresentityGroupMemberPresenceFilter,
                NULL, iGroupId );

        switch( iSubscriptionStatus )
            {
            case CPresentityGroupMembersPresenceSubscriptionItem::ESubscribedForCtxOnly:
            case CPresentityGroupMembersPresenceSubscriptionItem::ESubscribedForCtxAndOthers:
                {
                iSubItem->RemoveSubscriber( iContext );
                break;
                }
                
            case CPresentityGroupMembersPresenceSubscriptionItem::ENotSubscribedAtAll:
            case CPresentityGroupMembersPresenceSubscriptionItem::ESubscribedForOtherCtxOnly:
                {
                break;
                }
                
            XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
            }

        SynthesiseEventL();
        }
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribePresentityGroupMembersPresence::Type()
// ---------------------------------------------------------------------------
//
TInt
    COperationUnsubscribePresentityGroupMembersPresence::Type() const
    {
    return NPresenceOps::EPrUnsubscribePresentityGroupMembersPresence;
    }

// ---------------------------------------------------------------------------
// COperationUnsubscribePresentityGroupMembersPresence::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribePresentityGroupMembersPresence::SynthesiseEventL()
    {
    COperationSynthesiseSubscriptionEvent* synthOp =
            new ( ELeave ) COperationSynthesiseSubscriptionEvent(
                             COperationSynthesiseSubscriptionEvent::EPresentityGroupMembersPresence, EFalse );
    CleanupStack::PushL( synthOp );
    synthOp->BaseConstructL( TXIMPRequestId(), iContext );
    synthOp->ConstructL( *iGroupId );
    iMyHost->AddNewOperationL( *synthOp );
    CleanupStack::Pop( synthOp );
    }


// End of file
