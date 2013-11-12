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
* Description:  Presence configuration item
*
*/


#include "presenceconfigurationitem.h"
#include "ximpitemparent.h"
#include "ximpidentityimp.h"
#include "presenceinfofilterimp.h"
#include "ximppanics.h"
#include "ximppsccontext.h"
#include "documentutils.h"
#include "presentitypresenceeventimp.h"
#include "ximpobjecthelpers.h"

#include "ximptrace.h"

// ---------------------------------------------------------------------------
// CPresenceConfigurationItem::CPresenceConfigurationItem()
// ---------------------------------------------------------------------------
//
CPresenceConfigurationItem::CPresenceConfigurationItem( MXIMPItemParentBase& aParent )
: CXIMPConfigurationItemBase( aParent )
    {
    }


// ---------------------------------------------------------------------------
// CPresenceConfigurationItem::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceConfigurationItem::ConstructL( const CXIMPIdentityImp& aIdentity )
    {
    CXIMPConfigurationItemBase::BaseConstructL();
    
    iIdentity = TXIMPObjectCloner< CXIMPIdentityImp >::CloneL( aIdentity );
    }


// ---------------------------------------------------------------------------
// CPresenceConfigurationItem::NewLC()
// ---------------------------------------------------------------------------
//
CPresenceConfigurationItem*
    CPresenceConfigurationItem::NewLC( MXIMPItemParentBase& aParent, const CXIMPIdentityImp& aIdentity )
    {
    CPresenceConfigurationItem* self =
        new( ELeave ) CPresenceConfigurationItem( aParent );
    CleanupClosePushL( *self );
    self->ConstructL( aIdentity );
    return self;
    }


// ---------------------------------------------------------------------------
// CPresenceConfigurationItem::~CPresenceConfigurationItem()
// ---------------------------------------------------------------------------
//
CPresenceConfigurationItem::~CPresenceConfigurationItem()
    {
    MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
    presCache->RemoveMe( this );

    delete iIdentity;    
    delete iPif;    
    }

// ---------------------------------------------------------------------------
// CPresenceConfigurationItem::ConfigurationPif()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFilterImp& 
            CPresenceConfigurationItem::ConfigurationPif()
    {
    TRACE(_L("CPresenceConfigurationItem::ConfigurationPif" ) );
    __ASSERT_ALWAYS( iPif, User::Panic( NXIMPPrivPanic::KCategory, 
                                        NXIMPPrivPanic::EOwnPresencePifNotSet ) );
    return *iPif;
    }

// ---------------------------------------------------------------------------
// CPresenceConfigurationItem::SetSubscriptionPif()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceConfigurationItem::SetConfigurationPif(
                                                CPresenceInfoFilterImp* aPif )
    {
    TRACE(_L("CPresenceConfigurationItem::SetConfigurationPif" ) );
    if ( !aPif )
        {
        TRACE(_L("CPresenceConfigurationItem::SetConfigurationPif aPif NULL" ) );
        }
    delete iPif;
    iPif = aPif;
    }
    
// ---------------------------------------------------------------------------
// CPresenceConfigurationItem::CollectSubscriptionPifWithoutCtxL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFilterImp* 
    CPresenceConfigurationItem::CollectConfigurationPifWithoutCtxL( 
                                                        MXIMPPscContext* aContext )
    {
    TRACE(_L("CPresenceConfigurationItem::CollectSubscriptionPifWithoutCtxL" ) );
    RPointerArray< CPresenceInfoFilterImp > arrayOfPifs;
    CleanupClosePushL( arrayOfPifs );
    
    TInt count = iConfigContexts.Count();
    for( TInt a = 0; a < count; ++a )
        {
        if( iConfigContexts[ a ] != aContext )
            {
            CPresenceInfoFilterImp* filter = iConfigContexts[ a ]->PresenceInfoFilter( 
                        MXIMPPscContext::EGrantPresenceToPresentityFilter, iIdentity );
            if ( filter )
                {
                arrayOfPifs.AppendL( filter );
                }
            }        
        }
    CPresenceInfoFilterImp* pifWithoutCtx = DocumentUtils::InfoFilterUnionL( arrayOfPifs );    
    CleanupStack::PopAndDestroy(); // arrayOfPifs
    return pifWithoutCtx;
    }
    
    
// ---------------------------------------------------------------------------
// CPresenceConfigurationItem::Identity()
// ---------------------------------------------------------------------------
//    
EXPORT_C const MXIMPIdentity& CPresenceConfigurationItem::Identity() const
    {
    return *iIdentity;
    }
    
// ---------------------------------------------------------------------------
// CPresenceConfigurationItem::SetIdentity()
// ---------------------------------------------------------------------------
//    
EXPORT_C void CPresenceConfigurationItem::SetIdentity(
                                            CXIMPIdentityImp* aIdentity )
    {
    delete iIdentity;
    iIdentity = aIdentity;
    }

// ---------------------------------------------------------------------------
// CPresenceConfigurationItem::Order()
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt CPresenceConfigurationItem::Order(
                        const CPresenceConfigurationItem& aA,
                        const CPresenceConfigurationItem& aB )
    {
    return IdentityOrder( aA.iIdentity, aB );
    }
// ---------------------------------------------------------------------------
// CPresenceConfigurationItem::Order()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPresenceConfigurationItem::IdentityOrder(
                        const CXIMPIdentityImp* aA,
                        const CPresenceConfigurationItem& aB )
    {
    return aA->Compare( *aB.iIdentity );
    }

// ---------------------------------------------------------------------------
// From class MXIMPSubscriptionItem.
// CPresenceConfigurationItem::SynthesiseSubscriptionEventTo()
// ---------------------------------------------------------------------------
//
void CPresenceConfigurationItem::SynthesiseSubscriptionEventToL(
                                                    MXIMPPscContext* aContext,
                                                    TBool aForceEvent )
    {
    TRACE_1( _L("CPresenceConfigurationItem::SynthesiseSubscriptionEventTo() aForce=%d"), aForceEvent );
    XIMP_UNUSED_FORMAL_PARAM( aContext );
    XIMP_UNUSED_FORMAL_PARAM( aForceEvent );
    }

// ---------------------------------------------------------------------------
// CPresenceConfigurationItem::CleanExpired()
// ---------------------------------------------------------------------------
//
void CPresenceConfigurationItem::CleanExpired()
    {
    
    }

// End of file
