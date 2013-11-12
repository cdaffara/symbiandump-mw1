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
* Description:  Group members presence configuration item
*
*/


#include "groupmemberspresenceconfigurationitem.h"
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
// CGroupMembersPresenceConfigurationItem::CGroupMembersPresenceConfigurationItem()
// ---------------------------------------------------------------------------
//
CGroupMembersPresenceConfigurationItem::CGroupMembersPresenceConfigurationItem( MXIMPItemParentBase& aParent, CGroupContentSubscriptionItem& aContentItem )
: CXIMPConfigurationItemBase( aParent ), iContentItem( aContentItem )
    {
    }


// ---------------------------------------------------------------------------
// CGroupMembersPresenceConfigurationItem::ConstructL()
// ---------------------------------------------------------------------------
//
void CGroupMembersPresenceConfigurationItem::ConstructL()
    {
    BaseConstructL();
    iContentItem.RegisterEventObserverL( this );
    }


// ---------------------------------------------------------------------------
// CGroupMembersPresenceConfigurationItem::NewLC()
// ---------------------------------------------------------------------------
//
CGroupMembersPresenceConfigurationItem*
    CGroupMembersPresenceConfigurationItem::NewLC( MXIMPItemParentBase& aParent,
                                                   CGroupContentSubscriptionItem& aContentItem )
    {
    CGroupMembersPresenceConfigurationItem* self =
        new( ELeave ) CGroupMembersPresenceConfigurationItem( aParent, aContentItem );
    CleanupClosePushL( *self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CGroupMembersPresenceConfigurationItem::~CGroupMembersPresenceConfigurationItem()
// ---------------------------------------------------------------------------
//
CGroupMembersPresenceConfigurationItem::~CGroupMembersPresenceConfigurationItem()
    {
    MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
    presCache->RemoveMe( this );


    delete iIdentity;

    delete iPif;
    iContentItem.UnregisterEventObserver( this );
    }

// ---------------------------------------------------------------------------
// CGroupMembersPresenceConfigurationItem::ConfigurationPif()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFilterImp&
            CGroupMembersPresenceConfigurationItem::ConfigurationPif()
    {
    TRACE(_L("CGroupMembersPresenceConfigurationItem::ConfigurationPif" ) );
    __ASSERT_ALWAYS( iPif, User::Panic( NXIMPPrivPanic::KCategory,
                                        NXIMPPrivPanic::EOwnPresencePifNotSet ) );
    return *iPif;
    }

// ---------------------------------------------------------------------------
// CGroupMembersPresenceConfigurationItem::SetConfigurationPif()
// ---------------------------------------------------------------------------
//
EXPORT_C void CGroupMembersPresenceConfigurationItem::SetConfigurationPif(
                                                CPresenceInfoFilterImp* aPif )
    {
    TRACE(_L("CGroupMembersPresenceConfigurationItem::SetConfigurationPif" ) );
    if ( !aPif )
        {
        TRACE(_L("CGroupMembersPresenceConfigurationItem::SetConfigurationPif aPif NULL" ) );
        }
    delete iPif;
    iPif = aPif;
    }

// ---------------------------------------------------------------------------
// CGroupMembersPresenceConfigurationItem::CollectSubscriptionPifWithoutCtxL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFilterImp*
    CGroupMembersPresenceConfigurationItem::CollectConfigurationPifWithoutCtxL(
                                                        MXIMPPscContext* aContext )
    {
    TRACE(_L("CGroupMembersPresenceConfigurationItem::CollectConfigurationPifWithoutCtxL" ) );
    RPointerArray< CPresenceInfoFilterImp > arrayOfPifs;
    CleanupClosePushL( arrayOfPifs );

    TInt count = iConfigContexts.Count();
    for( TInt a = 0; a < count; ++a )
        {
        if( iConfigContexts[ a ] != aContext )
            {
            CPresenceInfoFilterImp* filter = iConfigContexts[ a ]->PresenceInfoFilter( 
                        MXIMPPscContext::EGrantPresenceToGroupMemberFilter, iIdentity );
            if ( filter )
                {
                arrayOfPifs.AppendL( filter );
                }
            }
        }
    CPresenceInfoFilterImp* pifWithoutCtx = DocumentUtils::InfoFilterUnionL( arrayOfPifs );
    CleanupStack::PopAndDestroy(); // arrayOfPifs
    TRACE(_L("CGroupMembersPresenceConfigurationItem::CollectConfigurationPifWithoutCtxL done" ) );
    return pifWithoutCtx;
    }

// ---------------------------------------------------------------------------
// CGroupMembersPresenceConfigurationItem::Order()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CGroupMembersPresenceConfigurationItem::Order(
                        const CGroupMembersPresenceConfigurationItem& aA,
                        const CGroupMembersPresenceConfigurationItem& aB )
    {
    return IdentityOrder( aA.iIdentity, aB );
    }
// ---------------------------------------------------------------------------
// CGroupMembersPresenceConfigurationItem::Order()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CGroupMembersPresenceConfigurationItem::IdentityOrder(
                        const CXIMPIdentityImp* aA,
                        const CGroupMembersPresenceConfigurationItem& aB )
    {
    return aA->Compare( *aB.iIdentity );
    }

// ---------------------------------------------------------------------------
// CGroupMembersPresenceConfigurationItem::Identity()
// ---------------------------------------------------------------------------
//
EXPORT_C const MXIMPIdentity& CGroupMembersPresenceConfigurationItem::Identity() const
    {
    return *iIdentity;
    }

// ---------------------------------------------------------------------------
// CGroupMembersPresenceConfigurationItem::SetIdentity()
// ---------------------------------------------------------------------------
//
EXPORT_C void CGroupMembersPresenceConfigurationItem::SetIdentity(
                                            CXIMPIdentityImp* aIdentity )
    {
    delete iIdentity;
    iIdentity = aIdentity;
    }

// ---------------------------------------------------------------------------
// From class MXIMPSubscriptionItem.
// CGroupMembersPresenceConfigurationItem::SynthesiseSubscriptionEventTo()
// ---------------------------------------------------------------------------
//
void CGroupMembersPresenceConfigurationItem::SynthesiseSubscriptionEventToL(
                                                    MXIMPPscContext* aContext,
                                                    TBool aForceEvent )
    {
    TRACE_1( _L("CGroupMembersPresenceConfigurationItem::SynthesiseSubscriptionEventTo() aForce=%d"), aForceEvent );
    // Synthesise event to all members
    TInt count = iContentItem.MemberCount();
    MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
    for( TInt a = 0; a < count; ++a )
        {
        CPresentityGroupMemberInfoImp& memberInfo = iContentItem.MemberAt( a );
        const CXIMPIdentityImp& identity = memberInfo.IdentityImp();

        presCache->SynthesisePresentityPresenceSubscriptionEventToL( identity, aContext, aForceEvent );
        }
    TRACE(_L("CGroupMembersPresenceConfigurationItem::SynthesiseSubscriptionEventTo done" ) );
    }


// ---------------------------------------------------------------------------
// CGroupMembersPresenceConfigurationItem::ActivateSubscriberL()
// ---------------------------------------------------------------------------
//
void CGroupMembersPresenceConfigurationItem::ActivateSubscriberL( MXIMPPscContext* aContext )
    {
    TRACE_1( _L("CGroupMembersPresenceConfigurationItem::ActivateSubscriberL() aContext=%d"), aContext );
    CXIMPSubscriptionItemBase::ActivateSubscriberL( aContext );

    // Subscribe presentity presence items.
    TInt count = iContentItem.MemberCount();
    TRACE_1( _L("CGroupMembersPresenceConfigurationItem::ActivateSubscriberL( MXIMPPscContext* aContext ) : iContentItem.MemberCount = %d"), count );
    MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
    for( TInt a = 0; a < count; ++a )
        {
        TRACE_1( _L("CGroupMembersPresenceConfigurationItem::ActivateSubscriberL( MXIMPPscContext* aContext ) : a = %d"), a );
        
        CPresentityGroupMemberInfoImp& memberInfo = iContentItem.MemberAt( a );
        const CXIMPIdentityImp& identity = memberInfo.IdentityImp();
        TInt error ( KErrNone );
        TRAP( error, presCache->ActivatePresentityPresenceSubscriberL( identity, this );)
        TRACE_1( _L("CGroupMembersPresenceConfigurationItem::ActivateSubscriberL( MXIMPPscContext* aContext ) : iParent.ActivatePresentityPresenceSubscriberL : error = %d"), error );

        }
    }


// ---------------------------------------------------------------------------
// CGroupMembersPresenceConfigurationItem::CleanExpired()
// ---------------------------------------------------------------------------
//
void CGroupMembersPresenceConfigurationItem::CleanExpired()
    {

    }

// ---------------------------------------------------------------------------
// CGroupMembersPresenceConfigurationItem::AddConfiguratorL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CGroupMembersPresenceConfigurationItem::AddConfiguratorL( MXIMPPscContext* aContext )
    {
    CXIMPConfigurationItemBase::AddConfiguratorL( aContext );

	MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
    // Subscribe presentity presence items.
    TInt count = iContentItem.MemberCount();
    for( TInt a = 0; a < count; ++a )
        {
        CPresentityGroupMemberInfoImp& memberInfo = iContentItem.MemberAt( a );
        const CXIMPIdentityImp& identity = memberInfo.IdentityImp();

        presCache->AddPresenceConfiguratorL( identity, this );
        }
    }


// ---------------------------------------------------------------------------
// CGroupMembersPresenceConfigurationItem::RemoveConfigurator()
// ---------------------------------------------------------------------------
//
EXPORT_C void CGroupMembersPresenceConfigurationItem::RemoveConfigurator( MXIMPPscContext* aContext )
    {
    MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
    // Subscribe presentity presence items.
    TInt count = iContentItem.MemberCount();
    for( TInt a = 0; a < count; ++a )
        {
        CPresentityGroupMemberInfoImp& memberInfo = iContentItem.MemberAt( a );
        const CXIMPIdentityImp& identity = memberInfo.IdentityImp();

        presCache->RemovePresenceConfigurator( identity, this );
        }
    CXIMPConfigurationItemBase::RemoveConfigurator( aContext );
    }


// ---------------------------------------------------------------------------
// CGroupMembersPresenceConfigurationItem::HandleEventL()
// ---------------------------------------------------------------------------
//
void CGroupMembersPresenceConfigurationItem::HandleChangeL(
                                RPrGrpMemInfoImpArray& aAdded, RPrGrpMemInfoImpArray& aRemoved )
    {
    TRACE(_L("CGroupMembersPresenceConfigurationItem::ConfigurationPif" ) );
    MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
    TInt count = aAdded.Count();
    for( TInt a = 0; a < count; ++a )
        {
        CPresentityGroupMemberInfoImp& memberInfo = *aAdded[ a ];
        const CXIMPIdentityImp& identity = memberInfo.IdentityImp();

        presCache->AddPresenceConfiguratorL( identity, this );
        }
    count = aRemoved.Count();
    for( TInt a = 0; a < count; ++a )
        {
        CPresentityGroupMemberInfoImp& memberInfo = *aRemoved[ a ];
        const CXIMPIdentityImp& identity = memberInfo.IdentityImp();

        presCache->RemovePresenceConfigurator( identity, this );
        }
    }

// ---------------------------------------------------------------------------
// CGroupMembersPresenceConfigurationItem::HandleEventL()
// ---------------------------------------------------------------------------
//
void CGroupMembersPresenceConfigurationItem::HandleSynthesiseL(
                                                        RPrGrpMemInfoImpArray& aTarget )
    {
    TRACE(_L("CGroupMembersPresenceConfigurationItem::ConfigurationPif" ) );
    TInt count = aTarget.Count();
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
