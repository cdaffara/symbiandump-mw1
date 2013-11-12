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
* Description:  Presentity presence unsubscription operation
 *
*/


#include <ximpprotocolconnection.h>
#include <protocolpresencewatching.h>
#include <protocolpresencefeatures.h>

#include "operationunsubscribepresentitypresence.h"
#include "presenceinfofilterimp.h"
#include "ximpidentityimp.h"
#include "ximpobjecthelpers.h"
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
// COperationUnsubscribePresentityPresence::COperationUnsubscribePresentityPresence()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationUnsubscribePresentityPresence::COperationUnsubscribePresentityPresence()
    {
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribePresentityPresence::~COperationUnsubscribePresentityPresence()
// ---------------------------------------------------------------------------
//
COperationUnsubscribePresentityPresence::~COperationUnsubscribePresentityPresence()
    {
    delete iIdentity;
    if( iSubItem )
        {
        iSubItem->Close();
        }
    }

// ---------------------------------------------------------------------------
// COperationUnsubscribePresentityPresence::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribePresentityPresence::ConstructL( const TDesC8& aParamPck )
    {
    iIdentity = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iIdentity );
    TXIMPObjectPacker< CXIMPIdentityImp >::UnPackL( *iIdentity, aParamPck );
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribePresentityPresence::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribePresentityPresence::ProcessL()
    {
    TRACE(_L("COperationUnsubscribePresentityPresence::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();

    CPresentityPresenceSubscriptionItem& subItem =
                iMyHost->PresenceDataAccess().PresenceDataCache().PresentityPresenceSubscriptionItemLC( *iIdentity );
    CleanupStack::Pop(); // subItem
    iSubItem = &subItem;

    iSubscriptionStatus = iSubItem->SubscriptionStatus( iContext );

    MProtocolPresenceWatching& watching = iMyHost->GetConnection().ProtocolPresenceFeatures().PresenceWatching();

    iContext->SetPresenceInfoFilterL( MXIMPPscContext::EPresentityPresenceFilter,
            NULL, iIdentity );

    switch( iSubscriptionStatus )
        {
        case CPresentityPresenceSubscriptionItem::ESubscribedForCtxOnly:
            {
            TRACE(_L("COperationUnsubscribePresentityPresence::ProcessL() DoUnsubscribePresentityPresenceL" ) );
            TInt err ( KErrNone );
            // If this fails, subscription needs to be removed anyway,
            // if everything went ok, subscription is removed in RequestCompleted
            TRAP( err, watching.DoUnsubscribePresentityPresenceL( *iIdentity, iReqId ) );
            if ( KErrNone != err )
                {
                iSubItem->RemoveSubscriber( iContext );
                User::Leave( err );
                }
            break;
            }

        case CPresentityPresenceSubscriptionItem::ESubscribedForCtxAndOthers:
            {
            CPresenceInfoFilterImp* pifWithoutCtx =
                iSubItem->CollectSubscriptionPifWithoutCtxL( iContext );
            CleanupStack::PushL( pifWithoutCtx );

            if ( iSubItem->SubscriptionPif().Contains( *pifWithoutCtx ) )
                {
                TRACE(_L("COperationUnsubscribePresentityPresence::ProcessL() DoUpdatePresentityPresenceSubscriptionPifL" ) );
                TInt err ( KErrNone );
                // If this fails, subscription needs to be removed anyway,
                // if everything went ok, subscription is removed in RequestCompleted
                TRAP( err, watching.DoUpdatePresentityPresenceSubscriptionPifL( *iIdentity, *pifWithoutCtx, iReqId ) );
                if ( KErrNone != err )
                    {
                    iSubItem->RemoveSubscriber( iContext );
                    User::Leave( err );
                    }
                }
            else
                {
                TRACE(_L("COperationUnsubscribePresentityPresence::ProcessL() FakeCompleteRequest 1" ) );
                iMyHost->FakeCompleteRequest( iReqId, KErrNone );
                }

            CleanupStack::PopAndDestroy( pifWithoutCtx );
            break;
            }

        case CPresentityPresenceSubscriptionItem::ENotSubscribedAtAll:
        case CPresentityPresenceSubscriptionItem::ESubscribedForOtherCtxOnly:
            {
            TRACE(_L("COperationUnsubscribePresentityPresence::ProcessL() FakeCompleteRequest 2" ) );
            iMyHost->FakeCompleteRequest( iReqId, KErrNone );
            break;
            }
            
        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );            
        }
    }

// ---------------------------------------------------------------------------
// COperationUnsubscribePresentityPresence::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribePresentityPresence::RequestCompletedL()
    {
    TRACE(_L("COperationUnsubscribePresentityPresence::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();
    TRACE_1(_L("COperationUnsubscribePresentityPresence::RequestCompletedL() result=%d" ),iStatusObj->ResultCode() );
    if( iStatusObj->ResultCode() == KErrNone )
        {
        switch( iSubscriptionStatus )
            {
            case CPresentityPresenceSubscriptionItem::ESubscribedForCtxOnly:
            case CPresentityPresenceSubscriptionItem::ESubscribedForCtxAndOthers:
                {
                TRACE(_L("COperationUnsubscribePresentityPresence::RequestCompletedL() removing context" ) );
                iSubItem->RemoveSubscriber( iContext );
                break;
                }
            
            case CPresentityPresenceSubscriptionItem::ENotSubscribedAtAll:
            case CPresentityPresenceSubscriptionItem::ESubscribedForOtherCtxOnly:
                {
                TRACE(_L("COperationUnsubscribePresentityPresence::RequestCompletedL() nothing to do" ) );
                break;
                }
            
            XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
            }

        SynthesiseEventL();
        }
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribePresentityPresence::Type()
// ---------------------------------------------------------------------------
//
TInt COperationUnsubscribePresentityPresence::Type() const
    {
    return NPresenceOps::EPrUnsubscribePresentityPresence;
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribePresentityPresence::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribePresentityPresence::SynthesiseEventL()
    {
    COperationSynthesiseSubscriptionEvent* synthOp =
            new ( ELeave ) COperationSynthesiseSubscriptionEvent(
                             COperationSynthesiseSubscriptionEvent::EPresentityPresence, EFalse );
    CleanupStack::PushL( synthOp );
    synthOp->BaseConstructL( TXIMPRequestId(), iContext );
    synthOp->ConstructL( *iIdentity );
    iMyHost->AddNewOperationL( *synthOp );
    CleanupStack::Pop( synthOp );
    }


// End of file
