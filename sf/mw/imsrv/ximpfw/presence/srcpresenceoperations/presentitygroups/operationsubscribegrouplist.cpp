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
* Description:  Presentity group list subscription operator
 *
*/


#include <ximpprotocolconnection.h>
#include <protocolpresentitygroups.h>
#include <protocolpresencefeatures.h>

#include "operationsubscribegrouplist.h"
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
// COperationSubscribeGroupList::COperationSubscribeGroupList()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationSubscribeGroupList::COperationSubscribeGroupList()
    {
    }


// ---------------------------------------------------------------------------
// COperationSubscribeGroupList::~COperationSubscribeGroupList()
// ---------------------------------------------------------------------------
//
COperationSubscribeGroupList::~COperationSubscribeGroupList()
    {
    if( iGroupList )
        {
        iGroupList->Close();
        }      
    }

// ---------------------------------------------------------------------------
// COperationSubscribeGroupList::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationSubscribeGroupList::ConstructL( const TDesC8& /*aParamPck*/ )
    {
    }


// ---------------------------------------------------------------------------
// COperationSubscribeGroupList::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationSubscribeGroupList::ProcessL()
    {
    TRACE(_L("COperationSubscribeGroupList::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();    
    
    CGroupListSubscriptionItem& groupList = 
                            iMyHost->PresenceDataAccess().PresenceDataCache().GroupListSubscriptionItemLC();
    CleanupStack::Pop(); // groupList
    iGroupList = &groupList;
    
    iSubscriptionStatus = iGroupList->SubscriptionStatus( iContext );
    MProtocolPresentityGroups& presentityGroups = iMyHost->GetConnection().ProtocolPresenceFeatures().PresentityGroups();
    
    switch( iSubscriptionStatus )
        {
        case CGroupListSubscriptionItem::ENotSubscribedAtAll:
            {
            // Do subscribe
            presentityGroups.DoSubscribePresentityGroupListL( iReqId );
            break;
            }
        case CGroupListSubscriptionItem::ESubscribedForOtherCtxOnly:
        case CGroupListSubscriptionItem::ESubscribedForCtxOnly:
        case CGroupListSubscriptionItem::ESubscribedForCtxAndOthers:
            {
            iMyHost->FakeCompleteRequest( iReqId, KErrNone );
            break;
            }
        
        
        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
        }    
    }

// ---------------------------------------------------------------------------
// COperationSubscribeGroupList::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationSubscribeGroupList::RequestCompletedL()
    {
    TRACE(_L("COperationSubscribeGroupList::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    if( iStatusObj->ResultCode() == KErrNone )
        {
        switch( iSubscriptionStatus )
            {
            case CGroupListSubscriptionItem::ENotSubscribedAtAll:
            case CGroupListSubscriptionItem::ESubscribedForOtherCtxOnly:
                {
                iGroupList->AddSubscriberL( iContext );
                break;
                }
            case CGroupListSubscriptionItem::ESubscribedForCtxOnly:
            case CGroupListSubscriptionItem::ESubscribedForCtxAndOthers:
                {
                break;
                }
                
            XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
            }        
        SynthesiseEventL();
        }     
    }


// ---------------------------------------------------------------------------
// COperationSubscribeGroupList::Type()
// ---------------------------------------------------------------------------
//
TInt 
    COperationSubscribeGroupList::Type() const
    {
    return NPresenceOps::EPrSubscribeGroupList;
    }

// ---------------------------------------------------------------------------
// COperationSubscribeGroupList::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationSubscribeGroupList::SynthesiseEventL()
    {
    TBool force = 
    ( iSubscriptionStatus == CGroupListSubscriptionItem::ESubscribedForCtxOnly ||
      iSubscriptionStatus == CGroupListSubscriptionItem::ESubscribedForCtxAndOthers ) ? ETrue : EFalse;
    
    COperationSynthesiseSubscriptionEvent* synthOp = 
            new ( ELeave ) COperationSynthesiseSubscriptionEvent( 
                             COperationSynthesiseSubscriptionEvent::EPresentityGroupList, force );
    CleanupStack::PushL( synthOp );
    synthOp->BaseConstructL( TXIMPRequestId(), iContext );        
    iMyHost->AddNewOperationL( *synthOp );
    CleanupStack::Pop( synthOp );    
    }



// End of file
