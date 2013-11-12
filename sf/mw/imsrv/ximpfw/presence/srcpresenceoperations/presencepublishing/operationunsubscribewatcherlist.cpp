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
#include <protocolpresencepublishing.h>
#include <protocolpresencefeatures.h>

#include "operationunsubscribewatcherlist.h"
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
// COperationUnsubscribeWatcherList::COperationUnsubscribeWatcherList()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationUnsubscribeWatcherList::COperationUnsubscribeWatcherList()
    {
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribeWatcherList::~COperationUnsubscribeWatcherList()
// ---------------------------------------------------------------------------
//
COperationUnsubscribeWatcherList::~COperationUnsubscribeWatcherList()
    {
    if( iWatcherList )
        {
        iWatcherList->Close();
        }
    }

// ---------------------------------------------------------------------------
// COperationUnsubscribeWatcherList::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeWatcherList::ConstructL( const TDesC8& /*aParamPck*/ )
    {
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribeWatcherList::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeWatcherList::ProcessL()
    {
    TRACE(_L("COperationUnsubscribeWatcherList::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();

    CWatcherListSubscriptionItem& watcherList =
                            iMyHost->PresenceDataAccess().PresenceDataCache().WatcherListSubscriptionItemLC();
    CleanupStack::Pop(); // watcherList
    iWatcherList = &watcherList;

    iSubscriptionStatus = iWatcherList->SubscriptionStatus( iContext );

    MProtocolPresencePublishing& publishing = iMyHost->GetConnection().ProtocolPresenceFeatures().PresencePublishing();

    switch( iSubscriptionStatus )
        {
        case CWatcherListSubscriptionItem::ESubscribedForCtxOnly:
            {
            publishing.DoUnsubscribePresenceWatcherListL( iReqId );
            break;
            }
            
        case CWatcherListSubscriptionItem::ESubscribedForCtxAndOthers:
            {
            iMyHost->FakeCompleteRequest( iReqId, KErrNone );
            break;
            }
            
        case CWatcherListSubscriptionItem::ENotSubscribedAtAll:
        case CWatcherListSubscriptionItem::ESubscribedForOtherCtxOnly:
            {
            iMyHost->FakeCompleteRequest( iReqId, KErrNone );
            break;
            }

        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
        }
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribeWatcherList::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeWatcherList::RequestCompletedL()
    {
    TRACE(_L("COperationUnsubscribeWatcherList::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    if( iStatusObj->ResultCode() == KErrNone )
        {
        switch( iSubscriptionStatus )
            {
            case CWatcherListSubscriptionItem::ESubscribedForCtxOnly:
            case CWatcherListSubscriptionItem::ESubscribedForCtxAndOthers:
                {
                iWatcherList->RemoveSubscriber( iContext );
                break;
                }
                
            case CWatcherListSubscriptionItem::ENotSubscribedAtAll:
            case CWatcherListSubscriptionItem::ESubscribedForOtherCtxOnly:
                {
                break;
                }
                
            XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
            }
        SynthesiseEventL();
        }
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribeWatcherList::Type()
// ---------------------------------------------------------------------------
//
TInt
    COperationUnsubscribeWatcherList::Type() const
    {
    return NPresenceOps::EPrUnsubscribePresenceWatcherList;
    }

// ---------------------------------------------------------------------------
// COperationUnsubscribeWatcherList::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeWatcherList::SynthesiseEventL()
    {
    COperationSynthesiseSubscriptionEvent* synthOp =
            new ( ELeave ) COperationSynthesiseSubscriptionEvent(
                COperationSynthesiseSubscriptionEvent::EPresenceWatcherList,
                EFalse );
    CleanupStack::PushL( synthOp );
    synthOp->BaseConstructL( TXIMPRequestId(), iContext );
    iMyHost->AddNewOperationL( *synthOp );
    CleanupStack::Pop( synthOp );
    }


// End of file
