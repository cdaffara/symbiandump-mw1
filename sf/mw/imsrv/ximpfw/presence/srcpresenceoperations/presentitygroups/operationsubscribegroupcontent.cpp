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
* Description:  Presentity group content subscription operator
 *
*/


#include <ximpprotocolconnection.h>
#include <protocolpresentitygroups.h>
#include <protocolpresencefeatures.h>

#include "operationsubscribegroupcontent.h"
#include "ximpobjecthelpers.h"
#include "presencedatacacheimp.h"
#include "ximppanics.h"
#include "operationsynthesisesubscriptionevent.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximpidentityimp.h"
#include "ximphost.h"
#include "ximpstatusimp.h"
#include "ximppsccontextimp.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationSubscribeGroupContent::COperationSubscribeGroupContent()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationSubscribeGroupContent::COperationSubscribeGroupContent()
    {
    }


// ---------------------------------------------------------------------------
// COperationSubscribeGroupContent::~COperationSubscribeGroupContent()
// ---------------------------------------------------------------------------
//
COperationSubscribeGroupContent::~COperationSubscribeGroupContent()
    {
    if( iSubItem )
        {
        iSubItem->Close();
        }
    delete iGroupId;
    }

// ---------------------------------------------------------------------------
// COperationSubscribeGroupContent::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationSubscribeGroupContent::ConstructL( const TDesC8& aParamPck )
    {
    iGroupId = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iGroupId );
    TXIMPObjectPacker< CXIMPIdentityImp >::UnPackL( *iGroupId, aParamPck );
    }


// ---------------------------------------------------------------------------
// COperationSubscribeGroupContent::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationSubscribeGroupContent::ProcessL()
    {
    TRACE(_L("COperationSubscribeGroupContent::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();

    CGroupContentSubscriptionItem& subItem =
                            iMyHost->PresenceDataAccess().PresenceDataCache().GroupContentSubscriptionItemLC( *iGroupId );
    CleanupStack::Pop(); // subItem
    iSubItem = &subItem;
    iSubscriptionStatus = iSubItem->SubscriptionStatus( iContext );
    MProtocolPresentityGroups& presentityGroups = iMyHost->GetConnection().ProtocolPresenceFeatures().PresentityGroups();

    switch( iSubscriptionStatus )
        {
        case CGroupContentSubscriptionItem::ENotSubscribedAtAll:
            {
            presentityGroups.DoSubscribePresentityGroupContentL( *iGroupId, iReqId );
            break;
            }
        
        case CGroupContentSubscriptionItem::ESubscribedForOtherCtxOnly:
        case CGroupContentSubscriptionItem::ESubscribedForCtxOnly:
        case CGroupContentSubscriptionItem::ESubscribedForCtxAndOthers:
            {
            iMyHost->FakeCompleteRequest( iReqId, KErrNone );
            break;
            }
            
        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );            
        }
    }


// ---------------------------------------------------------------------------
// COperationSubscribeGroupContent::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationSubscribeGroupContent::RequestCompletedL()
    {
    TRACE(_L("COperationSubscribeGroupContent::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    if( iStatusObj->ResultCode() == KErrNone )
        {
        switch( iSubscriptionStatus )
            {
            case CGroupContentSubscriptionItem::ENotSubscribedAtAll:
                {
                CXIMPIdentityImp* altGroupId = NULL;
                MXIMPBase* object = NULL;
                iObjCollection->GetByType( object, MXIMPIdentity::KInterfaceId );
                if( object )
                    {
                    // Got altered identity
                    altGroupId = TXIMPGetImpClassOrPanic< CXIMPIdentityImp >::From( *object );
                    CleanupStack::PushL( altGroupId );
                    }

                if ( altGroupId && iGroupId->Identity().Compare( altGroupId->Identity() ) != 0 )
                    {
                    CGroupContentSubscriptionItem& altSubItem =
                        iMyHost->PresenceDataAccess().PresenceDataCache().GroupContentSubscriptionItemLC( *altGroupId );
                    CleanupStack::Pop(); // altSubItem
                    iSubItem->Close();
                    iSubItem = &altSubItem;
                    iSubItem->AddOrRenewSubscriberL( iContext );
                    }
                else
                    {
                    iSubItem->AddSubscriberL( iContext );
                    }

                if( altGroupId )
                    {
                    CleanupStack::PopAndDestroy( altGroupId );
                    }                
                break;
                }
            
            case CGroupContentSubscriptionItem::ESubscribedForOtherCtxOnly:
                {
                iSubItem->AddSubscriberL( iContext );
                break;
                }
            
            case CGroupContentSubscriptionItem::ESubscribedForCtxOnly:
            case CGroupContentSubscriptionItem::ESubscribedForCtxAndOthers:
                {
                break;
                }

            XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );            
            }

        SynthesiseEventL();
        }
    }


// ---------------------------------------------------------------------------
// COperationSubscribeGroupContent::Type()
// ---------------------------------------------------------------------------
//
TInt
    COperationSubscribeGroupContent::Type() const
    {
    return NPresenceOps::EPrSubscribeGroupContent;
    }

// ---------------------------------------------------------------------------
// COperationSubscribeGroupContent::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationSubscribeGroupContent::SynthesiseEventL()
    {
    TBool force =
    ( iSubscriptionStatus == CGroupContentSubscriptionItem::ESubscribedForCtxOnly ||
      iSubscriptionStatus == CGroupContentSubscriptionItem::ESubscribedForCtxAndOthers ) ? ETrue : EFalse;

    COperationSynthesiseSubscriptionEvent* synthOp =
            new ( ELeave ) COperationSynthesiseSubscriptionEvent(
                             COperationSynthesiseSubscriptionEvent::EPresentityGroupContent, force );
    CleanupStack::PushL( synthOp );
    synthOp->BaseConstructL( TXIMPRequestId(), iContext );
    synthOp->ConstructL( *iGroupId );
    iMyHost->AddNewOperationL( *synthOp );
    CleanupStack::Pop( synthOp );
    }



// End of file
