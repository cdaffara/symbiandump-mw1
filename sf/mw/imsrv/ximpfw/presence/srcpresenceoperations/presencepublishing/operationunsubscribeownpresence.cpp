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
* Description: implementation of COperationUnsubscribeOwnPresence
 *
*/


#include <ximpprotocolconnection.h>
#include <protocolpresencepublishing.h>
#include <protocolpresencefeatures.h>

#include "operationunsubscribeownpresence.h"
#include "presenceinfofilterimp.h"
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
// COperationUnsubscribeOwnPresence::COperationUnsubscribeOwnPresence()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationUnsubscribeOwnPresence::COperationUnsubscribeOwnPresence()
    {
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribeOwnPresence::~COperationUnsubscribeOwnPresence()
// ---------------------------------------------------------------------------
//
COperationUnsubscribeOwnPresence::~COperationUnsubscribeOwnPresence()
    {
    if( iOwnPresence )
        {
        iOwnPresence->Close();
        }    
    delete iPifWithoutCtx;
    }

// ---------------------------------------------------------------------------
// COperationUnsubscribeOwnPresence::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeOwnPresence::ConstructL( const TDesC8& /*aParamPck*/ )
    {
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribeOwnPresence::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeOwnPresence::ProcessL()
    {
    TRACE(_L("COperationUnsubscribeOwnPresence::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();
    
    COwnPresenceSubscriptionItem& ownPresence = 
                            iMyHost->PresenceDataAccess().PresenceDataCache().OwnPresenceSubscriptionItemLC();
    CleanupStack::Pop(); // ownPresence    
    iOwnPresence = &ownPresence;
    
    iSubscriptionStatus = iOwnPresence->SubscriptionStatus( iContext );

    MProtocolPresencePublishing& publishing = iMyHost->GetConnection().ProtocolPresenceFeatures().PresencePublishing();

    switch( iSubscriptionStatus )
        {
        case COwnPresenceSubscriptionItem::ESubscribedForCtxOnly:
            {
            // Do subscribe
            publishing.DoUnsubscribeOwnPresenceL( iReqId );
            break;
            }
        case COwnPresenceSubscriptionItem::ESubscribedForCtxAndOthers:
            {
            iPifWithoutCtx = iOwnPresence->CollectSubscriptionPifWithoutCtxL( iContext );

            if ( iOwnPresence->SubscriptionPif().Contains( *iPifWithoutCtx ) ) 
                {
                publishing.DoUpdateOwnPresenceSubscriptionPifL( *iPifWithoutCtx, iReqId );
                }
            else
                {
                iMyHost->FakeCompleteRequest( iReqId, KErrNone );
                }                
            break;
            }

        case COwnPresenceSubscriptionItem::ENotSubscribedAtAll:
        case COwnPresenceSubscriptionItem::ESubscribedForOtherCtxOnly:
            {
            iMyHost->FakeCompleteRequest( iReqId, KErrNone );
            break;
            }

        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
        }    
    }

// ---------------------------------------------------------------------------
// COperationUnsubscribeOwnPresence::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeOwnPresence::RequestCompletedL()
    {
    TRACE(_L("COperationUnsubscribeWatcherList::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    if( iStatusObj->ResultCode() == KErrNone )
        {
        iContext->SetPresenceInfoFilterL( MXIMPPscContext::EOwnPresenceFilter,
                NULL, NULL );

        switch( iSubscriptionStatus )
            {
            case COwnPresenceSubscriptionItem::ESubscribedForCtxOnly:
            case COwnPresenceSubscriptionItem::ESubscribedForCtxAndOthers:
                {
                iOwnPresence->RemoveSubscriber( iContext );
                iContext->SetPresenceInfoFilterL( MXIMPPscContext::EOwnPresenceFilter, NULL );
                if( iPifWithoutCtx )
                    {
                    iOwnPresence->SetSubscriptionPif( iPifWithoutCtx );
                    iPifWithoutCtx = NULL;
                    }
                break;
                }
            
            case COwnPresenceSubscriptionItem::ENotSubscribedAtAll:
            case COwnPresenceSubscriptionItem::ESubscribedForOtherCtxOnly:
                {
                break;
                }
                
            XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );                
            }
            
        SynthesiseEventL();
        }     
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribeOwnPresence::Type()
// ---------------------------------------------------------------------------
//
TInt 
    COperationUnsubscribeOwnPresence::Type() const
    {
    return NPresenceOps::EPrUnsubscribeOwnPresence;
    }

// ---------------------------------------------------------------------------
// COperationUnsubscribeOwnPresence::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeOwnPresence::SynthesiseEventL()
    {
    COperationSynthesiseSubscriptionEvent* synthOp = 
            new ( ELeave ) COperationSynthesiseSubscriptionEvent( 
                             COperationSynthesiseSubscriptionEvent::EOwnPresence, EFalse );
    CleanupStack::PushL( synthOp );
    synthOp->BaseConstructL( TXIMPRequestId(), iContext );        
    iMyHost->AddNewOperationL( *synthOp );
    CleanupStack::Pop( synthOp );    
    }


// End of file
