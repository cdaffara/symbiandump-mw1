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
* Description: implementation of COperationSubscribeOwnPresence
 *
*/


#include <ximpprotocolconnection.h>
#include <protocolpresencepublishing.h>
#include <protocolpresencefeatures.h>

#include "operationsubscribeownpresence.h"
#include "presenceinfofilterimp.h"
#include "ximpobjecthelpers.h"
#include "documentutils.h"
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
// COperationSubscribeOwnPresence::COperationSubscribeOwnPresence()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationSubscribeOwnPresence::COperationSubscribeOwnPresence()
    {
    }


// ---------------------------------------------------------------------------
// COperationSubscribeOwnPresence::~COperationSubscribeOwnPresence()
// ---------------------------------------------------------------------------
//
COperationSubscribeOwnPresence::~COperationSubscribeOwnPresence()
    {
    delete iPif;
    delete iMergedPif;
    if( iOwnPresence )
        {
        iOwnPresence->Close();
        }    
    }

// ---------------------------------------------------------------------------
// COperationSubscribeOwnPresence::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationSubscribeOwnPresence::ConstructL( const TDesC8& aParamPck )
    {
    iPif = CPresenceInfoFilterImp::NewL();
    TXIMPObjectPacker< CPresenceInfoFilterImp >::UnPackL( *iPif,
            aParamPck );
    }


// ---------------------------------------------------------------------------
// COperationSubscribeOwnPresence::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationSubscribeOwnPresence::ProcessL()
    {
    TRACE(_L("COperationSubscribeOwnPresence::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();

    COwnPresenceSubscriptionItem& ownPresence = 
        iMyHost->PresenceDataAccess().PresenceDataCache().OwnPresenceSubscriptionItemLC();
    CleanupStack::Pop(); // ownPresence    
    iOwnPresence = &ownPresence;

    iSubscriptionStatus = iOwnPresence->SubscriptionStatus( iContext );

    MProtocolPresencePublishing& publishing = iMyHost->GetConnection().ProtocolPresenceFeatures().PresencePublishing();

    switch( iSubscriptionStatus )
        {
        case COwnPresenceSubscriptionItem::ENotSubscribedAtAll:
            {
            // Do subscribe
            iMergedPif = TXIMPObjectCloner< CPresenceInfoFilterImp >::CloneL( *iPif );
            publishing.DoSubscribeOwnPresenceL( *iMergedPif, iReqId );
            break;
            }
        case COwnPresenceSubscriptionItem::ESubscribedForOtherCtxOnly:
            {
            // Check aggregate and if needed.. update subscription
            iMergedPif = DocumentUtils::InfoFilterUnionL( iOwnPresence->SubscriptionPif(), *iPif );
            if( iMergedPif->Contains( *iPif ) )
                {
                publishing.DoUpdateOwnPresenceSubscriptionPifL( *iMergedPif, iReqId );
                }
            else
                {
                iMyHost->FakeCompleteRequest( iReqId, KErrNone );
                }
            break;
            }
            
        case COwnPresenceSubscriptionItem::ESubscribedForCtxOnly:
        case COwnPresenceSubscriptionItem::ESubscribedForCtxAndOthers:
            {
            CPresenceInfoFilterImp* oldPif = 
                    iContext->PresenceInfoFilter( MXIMPPscContext::EOwnPresenceFilter );
            CPresenceInfoFilterImp& oldMergedPif = iOwnPresence->SubscriptionPif();
            TBool fake = ETrue;
            TBool makeUnion( EFalse );
            if( !oldPif )
                {
                TRACE(_L("COperationSubscribeOwnPresence::ProcessL() no old pif" ) );
                makeUnion = ETrue;
                }
            else if ( *oldPif != *iPif ) 
                {
                makeUnion = ETrue;
                }
            if( makeUnion )
                {
                iDoNotForce = ETrue;
                CPresenceInfoFilterImp* withoutPif = 
                            iOwnPresence->CollectSubscriptionPifWithoutCtxL( iContext );
                CleanupStack::PushL( withoutPif );
                iMergedPif = DocumentUtils::InfoFilterUnionL( *withoutPif, *iPif );
                CleanupStack::PopAndDestroy( withoutPif );
                if( iMergedPif->Contains( oldMergedPif ) )
                    {
                    fake = EFalse;
                    publishing.DoUpdateOwnPresenceSubscriptionPifL( *iMergedPif, iReqId );
                    }
                }
            else
                {
                iMergedPif = TXIMPObjectCloner< CPresenceInfoFilterImp >::CloneL( oldMergedPif );
                }
                
            if( fake )
                {
                iMyHost->FakeCompleteRequest( iReqId, KErrNone );
                }
            break;
            }
        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
        }
    }


// ---------------------------------------------------------------------------
// COperationSubscribeOwnPresence::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationSubscribeOwnPresence::RequestCompletedL()
    {
    TRACE(_L("COperationSubscribeOwnPresence::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    if( iStatusObj->ResultCode() == KErrNone )
        {
        iContext->SetPresenceInfoFilterL( MXIMPPscContext::EOwnPresenceFilter,
                iPif, NULL );
        iPif = NULL;
        iOwnPresence->SetSubscriptionPif( iMergedPif );
        iMergedPif = NULL;

        switch( iSubscriptionStatus )
            {
            case COwnPresenceSubscriptionItem::ENotSubscribedAtAll:
            case COwnPresenceSubscriptionItem::ESubscribedForOtherCtxOnly:
                {
                iOwnPresence->AddSubscriberL( iContext );
                break;
                }
            case COwnPresenceSubscriptionItem::ESubscribedForCtxOnly:
            case COwnPresenceSubscriptionItem::ESubscribedForCtxAndOthers:
                {
                break;
                }

            XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
            }
        
        SynthesiseEventL();
        }     
    }


// ---------------------------------------------------------------------------
// COperationSubscribeOwnPresence::Type()
// ---------------------------------------------------------------------------
//
TInt 
    COperationSubscribeOwnPresence::Type() const
    {
    return NPresenceOps::EPrSubscribeOwnPresence;
    }

// ---------------------------------------------------------------------------
// COperationSubscribeOwnPresence::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationSubscribeOwnPresence::SynthesiseEventL()
    {
    TBool force = 
    ( iSubscriptionStatus == COwnPresenceSubscriptionItem::ESubscribedForCtxOnly ||
      iSubscriptionStatus == COwnPresenceSubscriptionItem::ESubscribedForCtxAndOthers ) ? ETrue : EFalse;
    
    COperationSynthesiseSubscriptionEvent* synthOp = 
            new ( ELeave ) COperationSynthesiseSubscriptionEvent( 
                             COperationSynthesiseSubscriptionEvent::EOwnPresence, force && !iDoNotForce );
    CleanupStack::PushL( synthOp );
    synthOp->BaseConstructL( TXIMPRequestId(), iContext );        
    iMyHost->AddNewOperationL( *synthOp );
    CleanupStack::Pop( synthOp );    
    }



// End of file
