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
* Description: implementation of COperationSubscribePresentityPresence
 *
*/


#include <ximpprotocolconnection.h>
#include <protocolpresencewatching.h>
#include <protocolpresencefeatures.h>

#include "operationsubscribepresentitypresence.h"
#include "presenceinfofilterimp.h"
#include "ximpidentityimp.h"
#include "ximpobjecthelpers.h"
#include "documentutils.h"
#include "presencedatacacheimp.h"
#include "ximppanics.h"
#include "operationsynthesisesubscriptionevent.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximphost.h"
#include "ximpstatusimp.h"
#include "ximppsccontextimp.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationSubscribePresentityPresence::COperationSubscribePresentityPresence()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationSubscribePresentityPresence::COperationSubscribePresentityPresence()
    {
    }


// ---------------------------------------------------------------------------
// COperationSubscribePresentityPresence::~COperationSubscribePresentityPresence()
// ---------------------------------------------------------------------------
//
COperationSubscribePresentityPresence::~COperationSubscribePresentityPresence()
    {
    delete iPif;
    delete iMergedPif;
    delete iIdentity;
    if( iSubItem )
        {
        iSubItem->Close();
        }
    }

// ---------------------------------------------------------------------------
// COperationSubscribePresentityPresence::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationSubscribePresentityPresence::ConstructL( const TDesC8& aParamPck )
    {
    RXIMPObjOwningPtrArray< HBufC8 > packArray;
    CleanupClosePushL( packArray );
    TXIMPHBuf8Packer::UnPackArrayL( packArray, aParamPck );

    iIdentity = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iIdentity );
    TXIMPObjectPacker< CXIMPIdentityImp >::UnPackL( *iIdentity, *( packArray[ 0 ] ) );

    iPif = CPresenceInfoFilterImp::NewLC();
    CleanupStack::Pop( iPif );
    TXIMPObjectPacker< CPresenceInfoFilterImp >::UnPackL( *iPif, *( packArray[ 1 ] ) );

    CleanupStack::PopAndDestroy(); // packArray
    }


// ---------------------------------------------------------------------------
// COperationSubscribePresentityPresence::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationSubscribePresentityPresence::ProcessL()
    {
    TRACE(_L("COperationSubscribePresentityPresence::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();

    // MatchAddressTo( subscriptions, iIdentity )
    CPresentityPresenceSubscriptionItem& subItem =
                iMyHost->PresenceDataAccess().PresenceDataCache().PresentityPresenceSubscriptionItemLC( *iIdentity );
    CleanupStack::Pop(); // subItem
    iSubItem = &subItem;

    // Get the subscription status
    // Notice that if there is more than one client and clients are using identity
    // which will alternate for request complete, this will affect wrong logic mapping
    // for subscription operation. 
    
    // For example: Client 1 is subscription "name" and server alters it to name@domain.com
    // Client 2 subscribes with "name" and it will get subscription status ENotSubscribedAtAll
    // when it should get ESubscribedForOtherCtxOnly.
    iSubscriptionStatus = iSubItem->SubscriptionStatus( iContext );

    MProtocolPresenceWatching& watching = iMyHost->GetConnection().ProtocolPresenceFeatures().PresenceWatching();

    switch( iSubscriptionStatus )
        {
        case CPresentityPresenceSubscriptionItem::ENotSubscribedAtAll:
            {
            TRACE(_L("COperationSubscribePresentityPresence::ProcessL() not subscribed" ) );
            iMergedPif = TXIMPObjectCloner< CPresenceInfoFilterImp >::CloneL( *iPif );
            // If this leaves, subscription is not created
            watching.DoSubscribePresentityPresenceL( iSubItem->Identity(),
                    *iMergedPif,
                    iReqId );
            break;
            }
        case CPresentityPresenceSubscriptionItem::ESubscribedForOtherCtxOnly:
            {
            TRACE(_L("COperationSubscribePresentityPresence::ProcessL() subscribed for other ctx" ) );
            // Aggregate subscription and context pifs
            // Check aggregate and if needed.. update subscription
            iMergedPif = DocumentUtils::InfoFilterUnionL( iSubItem->SubscriptionPif(), *iPif );
            if( iMergedPif->Contains( iSubItem->SubscriptionPif() ) )
                {
                TRACE(_L("COperationSubscribePresentityPresence::ProcessL() DoUpdatePresentityPresenceSubscriptionPifL 1" ) );
                // If this leaves, subscription is not created
                watching.DoUpdatePresentityPresenceSubscriptionPifL( *iIdentity, *iMergedPif, iReqId );
                }
            else
                {
                TRACE(_L("COperationSubscribePresentityPresence::ProcessL() FakeCompleteRequest" ) );
                iMyHost->FakeCompleteRequest( iReqId, KErrNone );
                }
            break;
            }
        case CPresentityPresenceSubscriptionItem::ESubscribedForCtxOnly:
        case CPresentityPresenceSubscriptionItem::ESubscribedForCtxAndOthers:
            {
            TRACE(_L("COperationSubscribePresentityPresence::ProcessL() subscribed" ) );
            CPresenceInfoFilterImp* oldPif = 
                    iContext->PresenceInfoFilter( MXIMPPscContext::EPresentityPresenceFilter, iIdentity );
            CPresenceInfoFilterImp& oldMergedPif = iSubItem->SubscriptionPif();
            TBool fake( ETrue );
            TBool makeUnion( EFalse );
            if( !oldPif )
                {
                TRACE(_L("COperationSubscribePresentityPresence::ProcessL() no old pif" ) );
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
                            iSubItem->CollectSubscriptionPifWithoutCtxL( iContext );
                iMergedPif = DocumentUtils::InfoFilterUnionL( *withoutPif, *iPif );
                if( iMergedPif->Contains( oldMergedPif ) )
                    {
                    fake = EFalse;
                    TRACE(_L("COperationSubscribePresentityPresence::ProcessL() DoUpdatePresentityPresenceSubscriptionPifL 2" ) );
                    watching.DoUpdatePresentityPresenceSubscriptionPifL( *iIdentity, *iMergedPif, iReqId );
                    }
                }
            else
                {
                iMergedPif = TXIMPObjectCloner< CPresenceInfoFilterImp >::CloneL( oldMergedPif );
                }

            if( fake )
                {
                TRACE(_L("COperationSubscribePresentityPresence::ProcessL() FakeCompleteRequest" ) );
                iMyHost->FakeCompleteRequest( iReqId, KErrNone );
                }
            break;
            }

        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );
        }
    }

// ---------------------------------------------------------------------------
// COperationSubscribePresentityPresence::RequestCompletedL()
// ---------------------------------------------------------------------------
//
void COperationSubscribePresentityPresence::RequestCompletedL()
    {
    TRACE(_L("COperationSubscribePresentityPresence::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    if( iStatusObj->ResultCode() == KErrNone )
        {
        if( iSubscriptionStatus == CPresentityPresenceSubscriptionItem::ENotSubscribedAtAll )
            {
            TRACE(_L("COperationSubscribePresentityPresence::RequestCompletedL() ENotSubscribedAtAll" ) );
            MXIMPBase* object = NULL; 
            iObjCollection->GetByType( object, MXIMPIdentity::KInterfaceId );
            if( object )
                {
                TRACE(_L("COperationSubscribePresentityPresence::RequestCompletedL() identity change" ) );
                delete iIdentity;
                iIdentity = TXIMPGetImpClassOrPanic< CXIMPIdentityImp >::From( *object );

                // Got altered identity. If it differs from original
                // we'll create a new subscription item and close the old.
                CPresentityPresenceSubscriptionItem& altSubItem = 
                    iMyHost->PresenceDataAccess().PresenceDataCache().PresentityPresenceSubscriptionItemLC( *iIdentity );

                iSubItem->Close();
                iSubItem = &altSubItem;

                CleanupStack::Pop(); // altSubItem                    
                }            
            }
            
        if( iSubscriptionStatus == CPresentityPresenceSubscriptionItem::ENotSubscribedAtAll ||
            iSubscriptionStatus == CPresentityPresenceSubscriptionItem::ESubscribedForOtherCtxOnly )
            {
            TRACE(_L("COperationSubscribePresentityPresence::RequestCompletedL() calling iSubItem->AddSubscriberL" ) );
            TInt error ( KErrNone );
            TRAP( error, iSubItem->AddSubscriberL( iContext ) );
            TRACE_1( _L("COperationSubscribePresentityPresence::RequestCompletedL():  iSubItem->AddSubscriberL : error =%d"), error );
            }
            
        iContext->SetPresenceInfoFilterL( MXIMPPscContext::EPresentityPresenceFilter,
                iPif, iIdentity );
        iPif = NULL;
        iSubItem->SetSubscriptionPif( iMergedPif );
        iMergedPif = NULL;

        SynthesiseEventL();
        }
    }


// ---------------------------------------------------------------------------
// COperationSubscribePresentityPresence::Type()
// ---------------------------------------------------------------------------
//
TInt
    COperationSubscribePresentityPresence::Type() const
    {
    return NPresenceOps::EPrSubscribePresentityPresence;
    }

// ---------------------------------------------------------------------------
// COperationSubscribePresentityPresence::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationSubscribePresentityPresence::SynthesiseEventL()
    {
    TBool force =
    ( iSubscriptionStatus == CPresentityPresenceSubscriptionItem::ESubscribedForCtxOnly ||
      iSubscriptionStatus == CPresentityPresenceSubscriptionItem::ESubscribedForCtxAndOthers ) ? ETrue : EFalse;

    COperationSynthesiseSubscriptionEvent* synthOp =
            new ( ELeave ) COperationSynthesiseSubscriptionEvent(
                COperationSynthesiseSubscriptionEvent::EPresentityPresence, force && !iDoNotForce  );
    CleanupStack::PushL( synthOp );
    synthOp->BaseConstructL( TXIMPRequestId(), iContext );
    synthOp->ConstructL( *iIdentity );
    iMyHost->AddNewOperationL( *synthOp );
    CleanupStack::Pop( synthOp );
    }



// End of file
