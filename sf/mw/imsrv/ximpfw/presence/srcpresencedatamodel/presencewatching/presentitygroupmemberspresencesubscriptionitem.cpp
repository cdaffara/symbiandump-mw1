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
* Description:  Presence Service Connection group list subscription implementation.
*
*/


#include "presentitygroupmemberspresencesubscriptionitem.h"
#include "groupcontentsubscriptionitem.h"
#include "presenceinfofilterimp.h"
#include "ximpitemparent.h"
#include "ximppanics.h"
#include "ximppsccontext.h"
#include "documentutils.h" // Perhaps this should be renamed
#include "presentitypresenceeventimp.h"
#include "presentitygroupmemberinfoimp.h"
#include "ximpidentityimp.h"

#include "ximptrace.h"

// ---------------------------------------------------------------------------
// CPresentityGroupMembersPresenceSubscriptionItem::CPresentityGroupMembersPresenceSubscriptionItem()
// ---------------------------------------------------------------------------
//
CPresentityGroupMembersPresenceSubscriptionItem::CPresentityGroupMembersPresenceSubscriptionItem( MXIMPItemParentBase& aParent, CGroupContentSubscriptionItem& aContentItem )
: CXIMPSubscriptionItemBase( aParent ), iContentItem( aContentItem )
    {
    }


// ---------------------------------------------------------------------------
// CPresentityGroupMembersPresenceSubscriptionItem::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupMembersPresenceSubscriptionItem::ConstructL()
    {
    BaseConstructL();
    iContentItem.RegisterEventObserverL( this );
    }


// ---------------------------------------------------------------------------
// CPresentityGroupMembersPresenceSubscriptionItem::NewLC()
// ---------------------------------------------------------------------------
//
CPresentityGroupMembersPresenceSubscriptionItem*
    CPresentityGroupMembersPresenceSubscriptionItem::NewLC( MXIMPItemParentBase& aParent,
                                                            CGroupContentSubscriptionItem& aContentItem )
    {
    CleanupClosePushL( aContentItem );
    CPresentityGroupMembersPresenceSubscriptionItem* self =
        new( ELeave ) CPresentityGroupMembersPresenceSubscriptionItem( aParent, aContentItem );
    CleanupStack::Pop(); //aContentItem
    CleanupClosePushL( *self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CPresentityGroupMembersPresenceSubscriptionItem::~CPresentityGroupMembersPresenceSubscriptionItem()
// ---------------------------------------------------------------------------
//
CPresentityGroupMembersPresenceSubscriptionItem::~CPresentityGroupMembersPresenceSubscriptionItem()
    {
    MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
    presCache->RemoveMe( this );


    delete iIdentity;
    delete iPresenceInfo;

    delete iPif;
    iContentItem.UnregisterEventObserver( this );
    }

// ---------------------------------------------------------------------------
// CPresentityGroupMembersPresenceSubscriptionItem::SubscriptionPif()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFilterImp&
            CPresentityGroupMembersPresenceSubscriptionItem::SubscriptionPif()
    {
    TRACE(_L("CPresentityGroupMembersPresenceSubscriptionItem::SubscriptionPif" ) );
    __ASSERT_ALWAYS( iPif, User::Panic( NXIMPPrivPanic::KCategory,
                                        NXIMPPrivPanic::EOwnPresencePifNotSet ) );
    return *iPif;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupMembersPresenceSubscriptionItem::SetSubscriptionPif()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresentityGroupMembersPresenceSubscriptionItem::SetSubscriptionPif(
                                                CPresenceInfoFilterImp* aPif )
    {
    TRACE(_L("CPresentityGroupMembersPresenceSubscriptionItem::SetSubscriptionPif" ) );
    delete iPif;
    iPif = aPif;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupMembersPresenceSubscriptionItem::CollectSubscriptionPifWithoutCtxL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFilterImp*
    CPresentityGroupMembersPresenceSubscriptionItem::CollectSubscriptionPifWithoutCtxL(
                                                        MXIMPPscContext* aContext )
    {
    TRACE(_L("CPresentityGroupMembersPresenceSubscriptionItem::CollectSubscriptionPifWithoutCtxL" ) );
    RPointerArray< CPresenceInfoFilterImp > arrayOfPifs;
    CleanupClosePushL( arrayOfPifs );

    TInt count = iContexts.Count();
    for( TInt a = 0; a < count; ++a )
        {
        if( iContexts[ a ] != aContext )
            {
            CPresenceInfoFilterImp* filter = iContexts[ a ]->PresenceInfoFilter( 
                        MXIMPPscContext::EPresentityGroupMemberPresenceFilter, iIdentity );
            if ( filter )
                {
                arrayOfPifs.AppendL( filter );
                }
            }
        }
    CPresenceInfoFilterImp* pifWithoutCtx = DocumentUtils::InfoFilterUnionL( arrayOfPifs );
    CleanupStack::PopAndDestroy(); // arrayOfPifs
    TRACE(_L("CPresentityGroupMembersPresenceSubscriptionItem::CollectSubscriptionPifWithoutCtxL done" ) );
    return pifWithoutCtx;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupMembersPresenceSubscriptionItem::SetPresenceInfoL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresentityGroupMembersPresenceSubscriptionItem::SetPresenceInfoL(
                                            CPresenceInfoImp* aPresenceInfo )
    {
    TRACE(_L("CPresentityGroupMembersPresenceSubscriptionItem::SetPresenceInfoL" ) );
    delete iPresenceInfo;
    iPresenceInfo = aPresenceInfo;
    iSubscriptionState->SetDataStateL( MXIMPDataSubscriptionState::EDataAvailable );
    }

// ---------------------------------------------------------------------------
// CPresentityGroupMembersPresenceSubscriptionItem::Order()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPresentityGroupMembersPresenceSubscriptionItem::Order(
                        const CPresentityGroupMembersPresenceSubscriptionItem& aA,
                        const CPresentityGroupMembersPresenceSubscriptionItem& aB )
    {
    return IdentityOrder( aA.iIdentity, aB );
    }
// ---------------------------------------------------------------------------
// CPresentityGroupMembersPresenceSubscriptionItem::Order()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPresentityGroupMembersPresenceSubscriptionItem::IdentityOrder(
                        const CXIMPIdentityImp* aA,
                        const CPresentityGroupMembersPresenceSubscriptionItem& aB )
    {
    return aA->Compare( *aB.iIdentity );
    }

// ---------------------------------------------------------------------------
// CPresentityGroupMembersPresenceSubscriptionItem::Identity()
// ---------------------------------------------------------------------------
//
EXPORT_C const MXIMPIdentity& CPresentityGroupMembersPresenceSubscriptionItem::Identity() const
    {
    return *iIdentity;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupMembersPresenceSubscriptionItem::SetIdentity()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresentityGroupMembersPresenceSubscriptionItem::SetIdentity(
                                            CXIMPIdentityImp* aIdentity )
    {
    delete iIdentity;
    iIdentity = aIdentity;
    }

// ---------------------------------------------------------------------------
// From class MXIMPSubscriptionItem.
// CPresentityGroupMembersPresenceSubscriptionItem::SynthesiseSubscriptionEventTo()
// ---------------------------------------------------------------------------
//
void CPresentityGroupMembersPresenceSubscriptionItem::SynthesiseSubscriptionEventToL(
                                                    MXIMPPscContext* aContext,
                                                    TBool aForceEvent )
    {
    TRACE_1( _L("CPresentityGroupMembersPresenceSubscriptionItem::SynthesiseSubscriptionEventTo() aForce=%d"), aForceEvent );
    // Synthesise event to all members
    TInt count = iContentItem.MemberCount();
    TRACE_1( _L("CPresentityGroupMembersPresenceSubscriptionItem::SynthesiseSubscriptionEventTo() iContentItem.MemberCount=%d"), count );

	MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));    
    for( TInt a = 0; a < count; ++a )
        {
        CPresentityGroupMemberInfoImp& memberInfo = iContentItem.MemberAt( a );
        const CXIMPIdentityImp& identity = memberInfo.IdentityImp();

        presCache->SynthesisePresentityPresenceSubscriptionEventToL( identity, aContext, aForceEvent );
        }
    }


// ---------------------------------------------------------------------------
// CPresentityGroupMembersPresenceSubscriptionItem::ActivateSubscriberL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupMembersPresenceSubscriptionItem::ActivateSubscriberL( MXIMPPscContext* aContext )
    {
    TRACE_1( _L("CPresentityGroupMembersPresenceSubscriptionItem::ActivateSubscriberL(aContext ) : aContext = %d"), aContext );
    
    CXIMPSubscriptionItemBase::ActivateSubscriberL( aContext );

    // Subscribe presentity presence items.
    TInt count = iContentItem.MemberCount();
    TRACE_1( _L("CPresentityGroupMembersPresenceSubscriptionItem::ActivateSubscriberL(aContext ) : iContentItem.MemberCount = %d"), count );
    MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
    for( TInt a = 0; a < count; ++a )
        {
        TRACE_1( _L("CPresentityGroupMembersPresenceSubscriptionItem::ActivateSubscriberL(aContext ) : a = %d"), a );
        CPresentityGroupMemberInfoImp& memberInfo = iContentItem.MemberAt( a );
        const CXIMPIdentityImp& identity = memberInfo.IdentityImp();
        TInt error ( KErrNone );
        TRAP( error , presCache->ActivatePresentityPresenceSubscriberL( identity, this ););
        TRACE_1( _L("CPresentityGroupMembersPresenceSubscriptionItem::ActivateSubscriberL( aContext ) i...eSubscriberL = %d"), error );
        }
    }


// ---------------------------------------------------------------------------
// CPresentityGroupMembersPresenceSubscriptionItem::CleanExpired()
// ---------------------------------------------------------------------------
//
void CPresentityGroupMembersPresenceSubscriptionItem::CleanExpired()
    {

    }

// ---------------------------------------------------------------------------
// CPresentityGroupMembersPresenceSubscriptionItem::AddSubscriberL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresentityGroupMembersPresenceSubscriptionItem::AddSubscriberL( MXIMPPscContext* aContext )
    {
    TRACE_1( _L("CPresentityGroupMembersPresenceSubscriptionItem::AddSubscriberL( MXIMPPscContext* aContext ) : aContext = %d"), aContext );

    CXIMPSubscriptionItemBase::AddSubscriberL( aContext );

    // Subscribe presentity presence items.
    TInt count = iContentItem.MemberCount();
    TRACE_1( _L("CPresentityGroupMembersPresenceSubscriptionItem::AddSubscriberL( MXIMPPscContext* aContext ) : iContentItem.MemberCount = %d"), count );
    MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
    for( TInt a = 0; a < count; ++a )
        {
        TRACE_1( _L("CPresentityGroupMembersPresenceSubscriptionItem::AddSubscriberL( MXIMPPscContext* aContext ) : a = %d"), a );
        
        CPresentityGroupMemberInfoImp& memberInfo = iContentItem.MemberAt( a );
        const CXIMPIdentityImp& identity = memberInfo.IdentityImp();
        TInt error ( KErrNone );
        TRAP( error , presCache->AddPresentityPresenceSubscriberL( identity, this, iIdentity ););
        TRACE_1( _L("CPresentityGroupMembersPresenceSubscriptionItem::AddSubscriberL( MXIMPPscContext* aContext ) : TRAPD( error , iParent.AddPresentityPresenceSubscriberL = %d"), error );

        }
    }


// ---------------------------------------------------------------------------
// CPresentityGroupMembersPresenceSubscriptionItem::RemoveSubscriber()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresentityGroupMembersPresenceSubscriptionItem::RemoveSubscriber( MXIMPPscContext* aContext )
    {
    TRACE_1( _L("CPresentityGroupMembersPresenceSubscriptionItem::RemoveSubscriber( MXIMPPscContext* aContext ) : aContext = %d"), aContext );
    
    // Subscribe presentity presence items.
    TInt count = iContentItem.MemberCount();
    TRACE_1( _L("CPresentityGroupMembersPresenceSubscriptionItem::RemoveSubscriber( MXIMPPscContext* aContext ) : iContentItem.MemberCount = %d"), count );
    MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
    for( TInt a = 0; a < count; ++a )
        {
        TRACE_1( _L("CPresentityGroupMembersPresenceSubscriptionItem::RemoveSubscriber( MXIMPPscContext* aContext ) : a = %d"), a );
        
        CPresentityGroupMemberInfoImp& memberInfo = iContentItem.MemberAt( a );
        const CXIMPIdentityImp& identity = memberInfo.IdentityImp();

        presCache->RemovePresentityPresenceSubscriber( identity, this );
        }

    TRACE( _L("CPresentityGroupMembersPresenceSubscriptionItem::RemoveSubscriber( MXIMPPscContext* aContext ) : CXIMPSubscriptionItemBase::RemoveSubscriber( aContext )") );
    CXIMPSubscriptionItemBase::RemoveSubscriber( aContext );
    }

// ---------------------------------------------------------------------------
// CPresentityGroupMembersPresenceSubscriptionItem::HandleChangeL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupMembersPresenceSubscriptionItem::HandleChangeL(
                                RPrGrpMemInfoImpArray& aAdded, RPrGrpMemInfoImpArray& aRemoved )
    {
    TRACE_2( _L("CPresentityGroupMembersPresenceSubscriptionItem::HandleChangeL aAdded =%d aRemoved=%d"), aAdded.Count(), aRemoved.Count() );
    TInt count = aAdded.Count();
    MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
    for( TInt a = 0; a < count; ++a )
        {
        CPresentityGroupMemberInfoImp& memberInfo = *aAdded[ a ];
        const CXIMPIdentityImp& identity = memberInfo.IdentityImp();

        presCache->AddPresentityPresenceSubscriberL( identity, this, iIdentity );
        }
    count = aRemoved.Count();
    for( TInt a = 0; a < count; ++a )
        {
        CPresentityGroupMemberInfoImp& memberInfo = *aRemoved[ a ];
        const CXIMPIdentityImp& identity = memberInfo.IdentityImp();

        presCache->RemovePresentityPresenceSubscriber( identity, this );
        }
    }

// ---------------------------------------------------------------------------
// CPresentityGroupMembersPresenceSubscriptionItem::HandleSynthesiseL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupMembersPresenceSubscriptionItem::HandleSynthesiseL(
                                                        RPrGrpMemInfoImpArray& aTarget )
    {
    TInt count = aTarget.Count();
    TRACE_1( _L("CPresentityGroupMembersPresenceSubscriptionItem::HandleSynthesiseL aTarget=%d"), aTarget.Count() );
    MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
    for( TInt a = 0; a < count; ++a )
        {
        CPresentityGroupMemberInfoImp& memberInfo = *aTarget[ a ];
        const CXIMPIdentityImp& identity = memberInfo.IdentityImp();

        presCache->ActivatePresentityPresenceSubscriberL( identity, this );
        TInt contextCount = iContexts.Count();
        for( TInt b = 0; b < contextCount; ++b )
            {
            presCache->SynthesisePresentityPresenceSubscriptionEventToL( identity, iContexts[ a ], EFalse );
            }
        }
    }

// End of file
