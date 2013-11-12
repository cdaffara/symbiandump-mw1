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
* Description:  Presence to everyone configuration item
*
*/


#include "presencetoeveryoneconfigurationitem.h"
#include "ximpitemparent.h"
#include "presenceinfofilterimp.h"
#include "ximppanics.h"
#include "ximppsccontext.h"
#include "documentutils.h"
#include "presentitypresenceeventimp.h"

#include "ximptrace.h"

// ---------------------------------------------------------------------------
// CPresenceToEveryoneConfigurationItem::CPresenceToEveryoneConfigurationItem()
// ---------------------------------------------------------------------------
//
CPresenceToEveryoneConfigurationItem::CPresenceToEveryoneConfigurationItem( MXIMPItemParentBase& aParent )
: CXIMPConfigurationItemBase( aParent )
    {
    }


// ---------------------------------------------------------------------------
// CPresenceToEveryoneConfigurationItem::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceToEveryoneConfigurationItem::ConstructL()
    {
    CXIMPConfigurationItemBase::BaseConstructL();
    }


// ---------------------------------------------------------------------------
// CPresenceToEveryoneConfigurationItem::NewLC()
// ---------------------------------------------------------------------------
//
CPresenceToEveryoneConfigurationItem*
    CPresenceToEveryoneConfigurationItem::NewLC( MXIMPItemParentBase& aParent )
    {
    CPresenceToEveryoneConfigurationItem* self =
        new( ELeave ) CPresenceToEveryoneConfigurationItem( aParent );
    CleanupClosePushL( *self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CPresenceToEveryoneConfigurationItem::~CPresenceToEveryoneConfigurationItem()
// ---------------------------------------------------------------------------
//
CPresenceToEveryoneConfigurationItem::~CPresenceToEveryoneConfigurationItem()
    {
    MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
    presCache->RemoveMe( this );

    delete iPif;    
    }

// ---------------------------------------------------------------------------
// CPresenceToEveryoneConfigurationItem::ConfigurationPif()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFilterImp& 
            CPresenceToEveryoneConfigurationItem::ConfigurationPif()
    {
    TRACE(_L("CPresenceToEveryoneConfigurationItem::ConfigurationPif" ) );
    __ASSERT_ALWAYS( iPif, User::Panic( NXIMPPrivPanic::KCategory, 
                                        NXIMPPrivPanic::EOwnPresencePifNotSet ) );
    return *iPif;
    }

// ---------------------------------------------------------------------------
// CPresenceToEveryoneConfigurationItem::SetSubscriptionPif()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceToEveryoneConfigurationItem::SetConfigurationPif(
                                                CPresenceInfoFilterImp* aPif )
    {
    TRACE(_L("CPresenceToEveryoneConfigurationItem::SetConfigurationPif" ) );
    if ( !aPif )
        {
        TRACE(_L("CPresenceToEveryoneConfigurationItem::SetConfigurationPif aPif NULL" ) );
        }
    delete iPif;
    iPif = aPif;
    }
    
// ---------------------------------------------------------------------------
// CPresenceToEveryoneConfigurationItem::CollectSubscriptionPifWithoutCtxL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFilterImp* 
    CPresenceToEveryoneConfigurationItem::CollectConfigurationPifWithoutCtxL( 
                                                        MXIMPPscContext* aContext )
    {
    TRACE(_L("CPresenceToEveryoneConfigurationItem::CollectConfigurationPifWithoutCtxL" ) );
    RPointerArray< CPresenceInfoFilterImp > arrayOfPifs;
    CleanupClosePushL( arrayOfPifs );
    
    TInt count = iConfigContexts.Count();
    for( TInt a = 0; a < count; ++a )
        {
        if( iConfigContexts[ a ] != aContext )
            {
            CPresenceInfoFilterImp* filter = iConfigContexts[ a ]->PresenceInfoFilter( 
                        MXIMPPscContext::EGrantPresenceToEveryoneFilter );
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
// From class MXIMPSubscriptionItem.
// CPresenceToEveryoneConfigurationItem::SynthesiseSubscriptionEventTo()
// ---------------------------------------------------------------------------
//
void CPresenceToEveryoneConfigurationItem::SynthesiseSubscriptionEventToL(
                                                    MXIMPPscContext* aContext,
                                                    TBool aForceEvent )
    {
    TRACE_1( _L("CPresenceToEveryoneConfigurationItem::SynthesiseSubscriptionEventTo() aForce=%d"), aForceEvent );
    XIMP_UNUSED_FORMAL_PARAM( aContext );
    XIMP_UNUSED_FORMAL_PARAM( aForceEvent );
    }

// ---------------------------------------------------------------------------
// CPresenceToEveryoneConfigurationItem::CleanExpired()
// ---------------------------------------------------------------------------
//
void CPresenceToEveryoneConfigurationItem::CleanExpired()
    {
    
    }

// End of file
