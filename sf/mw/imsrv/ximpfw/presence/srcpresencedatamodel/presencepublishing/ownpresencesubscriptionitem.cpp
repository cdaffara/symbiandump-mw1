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
* Description:  Presence Service Connection own presence subscription implementation.
*
*/


#include "ownpresencesubscriptionitem.h"
#include "presenceinfofilterimp.h"
#include "ximpitemparent.h"
#include "ximppanics.h"
#include "ximppsccontext.h"
#include "documentutils.h"
#include "ownpresenceeventimp.h"

#include "ximptrace.h"

// ---------------------------------------------------------------------------
// COwnPresenceSubscriptionItem::COwnPresenceSubscriptionItem()
// ---------------------------------------------------------------------------
//
COwnPresenceSubscriptionItem::COwnPresenceSubscriptionItem( MXIMPItemParentBase& aParent )
: CXIMPSubscriptionItemBase( aParent )
    {
    }


// ---------------------------------------------------------------------------
// COwnPresenceSubscriptionItem::ConstructL()
// ---------------------------------------------------------------------------
//
void COwnPresenceSubscriptionItem::ConstructL()
    {
    BaseConstructL();
    }


// ---------------------------------------------------------------------------
// COwnPresenceSubscriptionItem::NewL()
// ---------------------------------------------------------------------------
//
COwnPresenceSubscriptionItem* COwnPresenceSubscriptionItem::NewLC( MXIMPItemParentBase& aParent )
    {
    COwnPresenceSubscriptionItem* self = new( ELeave ) COwnPresenceSubscriptionItem( aParent );
    CleanupClosePushL( *self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPHost::~CXIMPHost()
// ---------------------------------------------------------------------------
//
COwnPresenceSubscriptionItem::~COwnPresenceSubscriptionItem()
    {
    MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
    presCache->RemoveMe( this );

    delete iPif;
    delete iPresenceInfo;
    }

// ---------------------------------------------------------------------------
// COwnPresenceSubscriptionItem::SubscriptionPif()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFilterImp& COwnPresenceSubscriptionItem::SubscriptionPif()
    {
    TRACE(_L("COwnPresenceSubscriptionItem::SubscriptionPif" ) );
    __ASSERT_ALWAYS( iPif, User::Panic( NXIMPPrivPanic::KCategory,
                                        NXIMPPrivPanic::EOwnPresencePifNotSet ) );
    return *iPif;
    }
// ---------------------------------------------------------------------------
// COwnPresenceSubscriptionItem::SynthesiseSubscriptionEventToL()
// ---------------------------------------------------------------------------
//
void COwnPresenceSubscriptionItem::SynthesiseSubscriptionEventToL(
                                       MXIMPPscContext* aContext, TBool aForceEvent )
    {
    TRACE_1( _L("COwnPresenceSubscriptionItem::SynthesiseSubscriptionEventTo() aForce=%d"), aForceEvent );
    CXIMPDataSubscriptionStateImp* status = StatusLC( aContext );

    COwnPresenceEventImp* newEvent = NULL;
    if( status->DataState() == MXIMPDataSubscriptionState::EDataAvailable )
        {
        __ASSERT_ALWAYS( iPresenceInfo, User::Panic( NXIMPPrivPanic::KCategory,
                                            NXIMPPrivPanic::EOwnPresencePresenceInfoDoesNotExist ) );

        CPresenceInfoImp* filteredPresenceInfo = DocumentUtils::CollectFilteredPresenceInfoLC(
                *( aContext->PresenceInfoFilter( MXIMPPscContext::EOwnPresenceFilter ) ), *iPresenceInfo );

        newEvent = COwnPresenceEventImp::NewL( status ); // CSI: 35 # Event is put to cleanupstack in next branch.
        newEvent->SetPresenceInfo( filteredPresenceInfo );
        CleanupStack::Pop( filteredPresenceInfo );
        }
    else if( aForceEvent )
        {
        newEvent = COwnPresenceEventImp::NewL( status ); // CSI: 35 # Event is put to cleanupstack in next branch.
        }

    if( newEvent )
        {
        CleanupStack::Pop(); //status
        CleanupStack::PushL( newEvent );
        MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
        presCache->AddEventL( *newEvent, aContext );
        }
    CleanupStack::PopAndDestroy(); //status || newEvent depending on branch
    }

// ---------------------------------------------------------------------------
// COwnPresenceSubscriptionItem::SetSubscriptionPif()
// ---------------------------------------------------------------------------
//
EXPORT_C void COwnPresenceSubscriptionItem::SetSubscriptionPif( CPresenceInfoFilterImp* aPif )
    {
    TRACE(_L("COwnPresenceSubscriptionItem::SetSubscriptionPif" ) );
    delete iPif;
    iPif = aPif;
    }

// ---------------------------------------------------------------------------
// COwnPresenceSubscriptionItem::CollectSubscriptionPifWithoutCtxL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFilterImp*
    COwnPresenceSubscriptionItem::CollectSubscriptionPifWithoutCtxL(
                                                        MXIMPPscContext* aContext )
    {
    TRACE(_L("COwnPresenceSubscriptionItem::CollectSubscriptionPifWithoutCtxL" ) );
    RPointerArray< CPresenceInfoFilterImp > arrayOfPifs;
    CleanupClosePushL( arrayOfPifs );

    TInt count = iContexts.Count();
    for( TInt a = 0; a < count; ++a )
        {
        if( iContexts[ a ] != aContext )
            {
            CPresenceInfoFilterImp* filter = iContexts[ a ]->PresenceInfoFilter( 
                        MXIMPPscContext::EOwnPresenceFilter );
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
// COwnPresenceSubscriptionItem::SetPresenceInfoL()
// ---------------------------------------------------------------------------
//
EXPORT_C void COwnPresenceSubscriptionItem::SetPresenceInfoL(
                                            CPresenceInfoImp* aPresenceInfo )
    {
    TRACE(_L("COwnPresenceSubscriptionItem::SetPresenceInfoL" ) );
    delete iPresenceInfo;
    iPresenceInfo = aPresenceInfo;
    iSubscriptionState->SetDataStateL( MXIMPDataSubscriptionState::EDataAvailable );
    }

// ---------------------------------------------------------------------------
// COwnPresenceSubscriptionItem::CleanExpired()
// ---------------------------------------------------------------------------
//
void COwnPresenceSubscriptionItem::CleanExpired()
    {

    }

// End of file
