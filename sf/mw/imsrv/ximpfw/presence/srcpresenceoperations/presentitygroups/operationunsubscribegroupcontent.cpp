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
* Description:  Presentity group content unsubscribe operator
 *
*/


#include <ximpprotocolconnection.h>
#include <protocolpresentitygroups.h>
#include <protocolpresencefeatures.h>

#include "operationunsubscribegroupcontent.h"
#include "ximpobjecthelpers.h"
#include "presencedatacacheimp.h"
#include "ximppanics.h"
#include "operationsynthesisesubscriptionevent.h"
#include "ximpidentityimp.h"
#include "ximphost.h"
#include "ximpstatusimp.h"
#include "ximppsccontextimp.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"
// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationUnsubscribeGroupContent::COperationUnsubscribeGroupContent()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationUnsubscribeGroupContent::COperationUnsubscribeGroupContent()
    {
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribeGroupContent::~COperationUnsubscribeGroupContent()
// ---------------------------------------------------------------------------
//
COperationUnsubscribeGroupContent::~COperationUnsubscribeGroupContent()
    {
    if( iSubItem )
        {
        iSubItem->Close();
        }
    delete iGroupId;
    }

// ---------------------------------------------------------------------------
// COperationUnsubscribeGroupContent::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeGroupContent::ConstructL( const TDesC8& aParamPck )
    {
    iGroupId = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iGroupId );
    TXIMPObjectPacker< CXIMPIdentityImp >::UnPackL( *iGroupId, aParamPck );
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribeGroupContent::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeGroupContent::ProcessL()
    {
    TRACE(_L("COperationUnsubscribeGroupContent::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();

    CGroupContentSubscriptionItem& subItem =
                            iMyHost->PresenceDataAccess().PresenceDataCache().GroupContentSubscriptionItemLC( *iGroupId );
    CleanupStack::Pop(); // subItem
    iSubItem = &subItem;

    iSubscriptionStatus = iSubItem->SubscriptionStatus( iContext );
    MProtocolPresentityGroups& presentityGroups = iMyHost->GetConnection().ProtocolPresenceFeatures().PresentityGroups();

    switch( iSubscriptionStatus )
        {
        case CGroupContentSubscriptionItem::ESubscribedForCtxOnly:
            {
            presentityGroups.DoUnsubscribePresentityGroupContentL( *iGroupId, iReqId );
            break;
            }

        case CGroupContentSubscriptionItem::ENotSubscribedAtAll:
        case CGroupContentSubscriptionItem::ESubscribedForCtxAndOthers:
        case CGroupContentSubscriptionItem::ESubscribedForOtherCtxOnly:
            {
            iMyHost->FakeCompleteRequest( iReqId, KErrNone );
            break;
            }

        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus ); 
        }
    }

// ---------------------------------------------------------------------------
// COperationUnsubscribeGroupContent::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeGroupContent::RequestCompletedL()
    {
    TRACE(_L("COperationUnsubscribeGroupContent::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    if( iStatusObj->ResultCode() == KErrNone )
        {
        switch( iSubscriptionStatus )
            {
            case CGroupContentSubscriptionItem::ESubscribedForCtxOnly:
            case CGroupContentSubscriptionItem::ESubscribedForCtxAndOthers:
                {
                iSubItem->RemoveSubscriber( iContext );
                break;
                }
            case CGroupContentSubscriptionItem::ENotSubscribedAtAll:
            case CGroupContentSubscriptionItem::ESubscribedForOtherCtxOnly:
                {
                break;
                }
                
            XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );                 
            }

        SynthesiseEventL();
        }
    }


// ---------------------------------------------------------------------------
// COperationUnsubscribeGroupContent::Type()
// ---------------------------------------------------------------------------
//
TInt
    COperationUnsubscribeGroupContent::Type() const
    {
    return NPresenceOps::EPrUnsubscribeGroupContent;
    }

// ---------------------------------------------------------------------------
// COperationUnsubscribeGroupContent::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationUnsubscribeGroupContent::SynthesiseEventL()
    {
    COperationSynthesiseSubscriptionEvent* synthOp =
            new ( ELeave ) COperationSynthesiseSubscriptionEvent(
                             COperationSynthesiseSubscriptionEvent::EPresentityGroupContent, ETrue );
    CleanupStack::PushL( synthOp );
    synthOp->BaseConstructL( TXIMPRequestId(), iContext );
    synthOp->ConstructL( *iGroupId );    
    iMyHost->AddNewOperationL( *synthOp );
    CleanupStack::Pop( synthOp );
    }



// End of file
