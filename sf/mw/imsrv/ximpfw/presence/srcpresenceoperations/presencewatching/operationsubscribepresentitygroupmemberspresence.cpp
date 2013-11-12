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
* Description: implementation of COperationSubscribePresentityGroupMembersPresence
 *
*/


#include <ximpprotocolconnection.h>
#include <protocolpresencewatching.h>
#include <protocolpresencefeatures.h>

#include "operationsubscribepresentitygroupmemberspresence.h"
#include "presenceinfofilterimp.h"
#include "ximpidentityimp.h"
#include "ximpobjecthelpers.h"
#include "presencedatacacheimp.h"
#include "documentutils.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "operationsynthesisesubscriptionevent.h"
#include "ximphost.h"
#include "ximpstatusimp.h"
#include "ximppsccontextimp.h"
#include "ximppanics.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"
// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationSubscribePresentityGroupMembersPresence::COperationSubscribePresentityGroupMembersPresence()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationSubscribePresentityGroupMembersPresence::COperationSubscribePresentityGroupMembersPresence()
    {
    }


// ---------------------------------------------------------------------------
// COperationSubscribePresentityGroupMembersPresence::~COperationSubscribePresentityGroupMembersPresence()
// ---------------------------------------------------------------------------
//
COperationSubscribePresentityGroupMembersPresence::~COperationSubscribePresentityGroupMembersPresence()
    {
    delete iMergedPif;
    delete iPif;
    delete iGroupId;
    if( iSubItem )
        {
        iSubItem->Close();
        }
    }

// ---------------------------------------------------------------------------
// COperationSubscribePresentityGroupMembersPresence::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationSubscribePresentityGroupMembersPresence::ConstructL( const TDesC8& aParamPck )
    {
    RXIMPObjOwningPtrArray< HBufC8 > packArray;
    CleanupClosePushL( packArray );
    TXIMPHBuf8Packer::UnPackArrayL( packArray, aParamPck );

    iGroupId = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iGroupId );
    TXIMPObjectPacker< CXIMPIdentityImp >::UnPackL( *iGroupId, *( packArray[ 0 ] ) );

    iPif = CPresenceInfoFilterImp::NewLC();
    CleanupStack::Pop( iPif );
    TXIMPObjectPacker< CPresenceInfoFilterImp >::UnPackL( *iPif, *( packArray[ 1 ] ) );

    CleanupStack::PopAndDestroy(); // packArray    
    }


// ---------------------------------------------------------------------------
// COperationSubscribePresentityGroupMembersPresence::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationSubscribePresentityGroupMembersPresence::ProcessL()
    {
    TRACE(_L("COperationSubscribePresentityGroupMembersPresence::ProcessL() - begin" ) );
    CXIMPOperationBase::ProcessL();
    
    CPresentityGroupMembersPresenceSubscriptionItem& subItem =  
                iMyHost->PresenceDataAccess().PresenceDataCache().PresentityGroupMembersPresenceSubscriptionItemLC( *iGroupId );
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

    TRACE(_L("COperationSubscribePresentityGroupMembersPresence::ProcessL() - start action" ) );
    
    switch( iSubscriptionStatus )
        {
        case CPresentityGroupMembersPresenceSubscriptionItem::ENotSubscribedAtAll:
            {
            TRACE(_L("COperationSubscribePresentityGroupMembersPresence::ProcessL() - case CPresentityGroupMembersPresenceSubscriptionItem::ENotSubscribedAtAll" ) );
            
            iMergedPif = TXIMPObjectCloner< CPresenceInfoFilterImp >::CloneL( *iPif );
            watching.DoSubscribePresentityGroupMembersPresenceL( *iGroupId,
                                                                   *iMergedPif,
                                                                   iReqId );
            break;
            }
        case CPresentityGroupMembersPresenceSubscriptionItem::ESubscribedForOtherCtxOnly:
            {
            TRACE(_L("COperationSubscribePresentityGroupMembersPresence::ProcessL() - case CPresentityGroupMembersPresenceSubscriptionItem::ESubscribedForOtherCtxOnly" ) );
            
            // Aggregate subscription and context pifs
            // Check aggregate and if needed.. update subscription
            iMergedPif = DocumentUtils::InfoFilterUnionL( iSubItem->SubscriptionPif(), *iPif );
            if( iMergedPif->Contains( iSubItem->SubscriptionPif() ) )
                {
                TRACE(_L("COperationSubscribePresentityGroupMembersPresence::ProcessL() -  if( iMergedPif->Contains( iSubItem->SubscriptionPif() ) ) " ) );                            
                            
                watching.DoUpdatePresentityGroupMembersPresenceSubscriptionPifL( *iGroupId, 
                                                                                *iMergedPif, 
                                                                                iReqId );
                }
            else
                {
                TRACE(_L("COperationSubscribePresentityGroupMembersPresence::ProcessL() - iMyHost->FakeCompleteRequest" ) );                            
                iMyHost->FakeCompleteRequest( iReqId, KErrNone );
                }
            break;
            }
        case CPresentityGroupMembersPresenceSubscriptionItem::ESubscribedForCtxOnly:
        case CPresentityGroupMembersPresenceSubscriptionItem::ESubscribedForCtxAndOthers:
            {
            TRACE(_L("COperationSubscribePresentityGroupMembersPresence::ProcessL() - case CPresentityGroupMembersPresenceSubscriptionItem::ESubscribedForCtxOnly and ESubscribedForCtxAndOthers" ) );
            
            CPresenceInfoFilterImp* oldPif = 
                    iContext->PresenceInfoFilter( MXIMPPscContext::EPresentityGroupMemberPresenceFilter, iGroupId );
            CPresenceInfoFilterImp& oldMergedPif = iSubItem->SubscriptionPif();
            TBool makeUnion( EFalse );
            if( !oldPif )
                {
                TRACE(_L("COperationSubscribePresentityGroupMembersPresence::ProcessL() no old pif" ) );
                makeUnion = ETrue;
                }
            else if ( *oldPif != *iPif ) 
                {
                makeUnion = ETrue;
                }
            if( makeUnion )
                {
                TRACE(_L("COperationSubscribePresentityGroupMembersPresence::ProcessL() - if( *oldPif != *iPif )" ) );                            
                
                iDoNotForce = ETrue;
                CPresenceInfoFilterImp* withoutPif = 
                            iSubItem->CollectSubscriptionPifWithoutCtxL( iContext );
                iMergedPif = DocumentUtils::InfoFilterUnionL( *withoutPif, *iPif );
                }
            else
                {
                TRACE(_L("COperationSubscribePresentityGroupMembersPresence::ProcessL() -  iMergedPif = TXIMPObjectCloner< CPresenceInfoFilterImp >::CloneL( oldMergedPif );" ) );                            
                
                iMergedPif = TXIMPObjectCloner< CPresenceInfoFilterImp >::CloneL( oldMergedPif );
                }
                
            if( iMergedPif->Contains( oldMergedPif ) )
                {
                TRACE(_L("COperationSubscribePresentityGroupMembersPresence::ProcessL() -  if( iMergedPif->Contains( oldMergedPif ) )" ) );                            
                
                iDoNotForce = ETrue;
                watching.DoUpdatePresentityGroupMembersPresenceSubscriptionPifL( *iGroupId, *iMergedPif, iReqId );
                }
            else
                {
                TRACE(_L("COperationSubscribePresentityGroupMembersPresence::ProcessL() -  iMyHost->FakeCompleteRequest( iReqId, KErrNone );" ) );                            
                
                iMyHost->FakeCompleteRequest( iReqId, KErrNone );
                }
            break;
            }
        
        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionStatus );            
        }
        
    TRACE(_L("COperationSubscribePresentityGroupMembersPresence::ProcessL() - end" ) );
    }


// ---------------------------------------------------------------------------
// COperationSubscribePresentityGroupMembersPresence::RequestCompletedL()
// ---------------------------------------------------------------------------
//
void COperationSubscribePresentityGroupMembersPresence::RequestCompletedL()
    {
    TRACE(_L("COperationSubscribePresentityGroupMembersPresence::RequestCompletedL() - begin" ) );
    CXIMPOperationBase::RequestCompletedL();

    TRACE(_L("COperationSubscribePresentityGroupMembersPresence::RequestCompletedL() : after CXIMPOperationBase::RequestCompletedL" ) );
    if( iStatusObj->ResultCode() == KErrNone )
        {
        TRACE(_L("COperationSubscribePresentityGroupMembersPresence::RequestCompletedL() : iStatusObj->ResultCode() == KErrNone " ) );
        
        if( iSubscriptionStatus == 
                    CPresentityGroupMembersPresenceSubscriptionItem::ENotSubscribedAtAll )
            {
            TRACE(_L("COperationSubscribePresentityGroupMembersPresence::RequestCompletedL() : iSubscriptionStatus == ENotSubscribedAtAll " ) );
            
            MXIMPBase* object = NULL; 
            iObjCollection->GetByType( object, MXIMPIdentity::KInterfaceId );
            if( object )
                {
                TRACE(_L("COperationSubscribePresentityGroupMembersPresence::RequestCompletedL got altered identity()"));
                delete iGroupId;
                iGroupId = TXIMPGetImpClassOrPanic< CXIMPIdentityImp >::From( *object );

                // Got altered identity. If it differs from original
                // we'll create a new subscription item and close the old.
                CPresentityGroupMembersPresenceSubscriptionItem& altSubItem = 
                    iMyHost->PresenceDataAccess().PresenceDataCache().PresentityGroupMembersPresenceSubscriptionItemLC( *iGroupId );

                iSubItem->Close();
                iSubItem = &altSubItem;

                CleanupStack::Pop(); // altSubItem                    
                }            
            }
            
        TRACE(_L("COperationSubscribePresentityGroupMembersPresence::RequestCompletedL() : before iSubItem->AddSubscriberL"));            
        TRACE_1(_L(" iSubscriptionStatus = %d" ), (TInt)iSubscriptionStatus );  
        if( iSubscriptionStatus == 
                    CPresentityGroupMembersPresenceSubscriptionItem::ENotSubscribedAtAll ||
            iSubscriptionStatus == 
                    CPresentityGroupMembersPresenceSubscriptionItem::ESubscribedForOtherCtxOnly )
            {
            iSubItem->AddSubscriberL( iContext );        
            TRACE(_L("COperationSubscribePresentityGroupMembersPresence::RequestCompletedL():  iSubItem->AddSubscriberL( iContext ) called:  " ) );     
            }
            
        iContext->SetPresenceInfoFilterL( MXIMPPscContext::EPresentityGroupMemberPresenceFilter,
                iPif, iGroupId );
        iPif = NULL;
        iSubItem->SetSubscriptionPif( iMergedPif );
        iMergedPif = NULL;

        SynthesiseEventL();
        }
    
    TRACE(_L("COperationSubscribePresentityGroupMembersPresence::RequestCompletedL():  end " ) );        
    }

// ---------------------------------------------------------------------------
// COperationSubscribePresentityGroupMembersPresence::Type()
// ---------------------------------------------------------------------------
//
TInt
    COperationSubscribePresentityGroupMembersPresence::Type() const
    {
    return NPresenceOps::EPrSubscribePresentityGroupMembersPresence;
    }

// ---------------------------------------------------------------------------
// COperationSubscribePresentityGroupMembersPresence::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationSubscribePresentityGroupMembersPresence::SynthesiseEventL()
    {
    TBool force = 
    ( iSubscriptionStatus == CPresentityGroupMembersPresenceSubscriptionItem::ESubscribedForCtxOnly ||
      iSubscriptionStatus == CPresentityGroupMembersPresenceSubscriptionItem::ESubscribedForCtxAndOthers ) ? ETrue : EFalse;
    TRACE_1(_L("COperationSubscribePresentityGroupMembersPresence::SynthesiseEventL - force=%d" ) , force);
    COperationSynthesiseSubscriptionEvent* synthOp =
        new ( ELeave ) COperationSynthesiseSubscriptionEvent(
            COperationSynthesiseSubscriptionEvent::EPresentityGroupMembersPresence, force && !iDoNotForce );
    CleanupStack::PushL( synthOp );    
    synthOp->BaseConstructL( TXIMPRequestId(), iContext );
    synthOp->ConstructL( *iGroupId );
    iMyHost->AddNewOperationL( *synthOp );
    CleanupStack::Pop( synthOp );    
    }



// End of file
