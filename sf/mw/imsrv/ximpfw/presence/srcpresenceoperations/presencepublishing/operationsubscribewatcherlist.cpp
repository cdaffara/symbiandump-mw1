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
* Description:  Presence watcher list subscription operator
 *
*/


#include <ximpprotocolconnection.h>
#include <protocolpresencepublishing.h>
#include <protocolpresencefeatures.h>

#include "operationsubscribewatcherlist.h"
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
// COperationSubscribeWatcherList::COperationSubscribeWatcherList()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationSubscribeWatcherList::COperationSubscribeWatcherList()
    {
    }


// ---------------------------------------------------------------------------
// COperationSubscribeWatcherList::~COperationSubscribeWatcherList()
// ---------------------------------------------------------------------------
//
COperationSubscribeWatcherList::~COperationSubscribeWatcherList()
    {
    if( iWatcherList )
        {
        iWatcherList->Close();
        }
    }

// ---------------------------------------------------------------------------
// COperationSubscribeWatcherList::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationSubscribeWatcherList::ConstructL( const TDesC8& /*aParamPck*/ )
    {
    }


// ---------------------------------------------------------------------------
// COperationSubscribeWatcherList::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationSubscribeWatcherList::ProcessL()
    {
    TRACE(_L("COperationSubscribeWatcherList::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();

    CWatcherListSubscriptionItem& watcherList =
                            iMyHost->PresenceDataAccess().PresenceDataCache().WatcherListSubscriptionItemLC();
    CleanupStack::Pop(); // watcherList
    iWatcherList = &watcherList;

    iSubscriptionStatus = iWatcherList->SubscriptionStatus( iContext );
    MProtocolPresencePublishing& publishing = iMyHost->GetConnection().ProtocolPresenceFeatures().PresencePublishing();

    switch( iSubscriptionStatus )
        {
        case CWatcherListSubscriptionItem::ENotSubscribedAtAll:
            {
            publishing.DoSubscribePresenceWatcherListL( iReqId );
            break;
            }
            
        case CWatcherListSubscriptionItem::ESubscribedForOtherCtxOnly:
        case CWatcherListSubscriptionItem::ESubscribedForCtxOnly:
        case CWatcherListSubscriptionItem::ESubscribedForCtxAndOthers:
            {
            iMyHost->FakeCompleteRequest( iReqId, KErrNone );
            break;
            }

        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
        }
    }


// ---------------------------------------------------------------------------
// COperationSubscribeWatcherList::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationSubscribeWatcherList::RequestCompletedL()
    {
    TRACE(_L("COperationSubscribeWatcherList::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    if( iStatusObj->ResultCode() == KErrNone )
        {
        switch( iSubscriptionStatus )
            {
            case CWatcherListSubscriptionItem::ENotSubscribedAtAll:
            case CWatcherListSubscriptionItem::ESubscribedForOtherCtxOnly:
                {
                iWatcherList->AddSubscriberL( iContext );
                break;
                }
                
            case CWatcherListSubscriptionItem::ESubscribedForCtxOnly:
            case CWatcherListSubscriptionItem::ESubscribedForCtxAndOthers:
                {
                break;
                }
                
            XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
            }

        SynthesiseEventL();
        }
    }


// ---------------------------------------------------------------------------
// COperationSubscribeWatcherList::Type()
// ---------------------------------------------------------------------------
//
TInt
    COperationSubscribeWatcherList::Type() const
    {
    return NPresenceOps::EPrSubscribePresenceWatcherList;
    }

// ---------------------------------------------------------------------------
// COperationSubscribeWatcherList::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationSubscribeWatcherList::SynthesiseEventL()
    {
    TBool force =
    ( iSubscriptionStatus == CWatcherListSubscriptionItem::ESubscribedForCtxOnly ||
      iSubscriptionStatus == CWatcherListSubscriptionItem::ESubscribedForCtxAndOthers ) ? ETrue : EFalse;

    COperationSynthesiseSubscriptionEvent* synthOp =
            new ( ELeave ) COperationSynthesiseSubscriptionEvent(
                             COperationSynthesiseSubscriptionEvent::EPresenceWatcherList,
                             force );
    CleanupStack::PushL( synthOp );
    synthOp->BaseConstructL( TXIMPRequestId(), iContext );
    iMyHost->AddNewOperationL( *synthOp );
    CleanupStack::Pop( synthOp );
    }



// End of file
