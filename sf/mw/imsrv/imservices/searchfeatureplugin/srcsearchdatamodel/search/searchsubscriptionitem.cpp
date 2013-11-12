/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Search subscription implementation.
*
*/


#include <badesca.h>
#include "ximpitemparentbase.h"
#include "ximppanics.h"
#include "ximppsccontext.h"
#include "ximpobjecthelpers.h"
#include "searchtypehelpers.h"
#include "searcheventimp.h"
#include "ximpidentityimp.h"
#include "searchsubscriptionitem.h"

#include "searchitemparent.h"

// ---------------------------------------------------------------------------
// CSearchSubscriptionItem::CSearchSubscriptionItem()
// ---------------------------------------------------------------------------
//
CSearchSubscriptionItem::CSearchSubscriptionItem( MXIMPItemParentBase& aParent )
: CXIMPSubscriptionItemBase( aParent )
    {
    }


// ---------------------------------------------------------------------------
// CSearchSubscriptionItem::ConstructL()
// ---------------------------------------------------------------------------
//
void CSearchSubscriptionItem::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// CSearchSubscriptionItem::NewL()
// ---------------------------------------------------------------------------
//
CSearchSubscriptionItem* CSearchSubscriptionItem::NewLC( MXIMPItemParentBase& aParent )
    {
    CSearchSubscriptionItem* self = new( ELeave ) CSearchSubscriptionItem( aParent );
    CleanupClosePushL( *self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CSearchSubscriptionItem::~CSearchSubscriptionItem()
// ---------------------------------------------------------------------------
//
EXPORT_C CSearchSubscriptionItem::~CSearchSubscriptionItem()
    {
    
    MSearchItemParent* searchCache = static_cast<MSearchItemParent*>
                                     (iParent.GetInterface(SEARCH_ITEM_PARENT));
    searchCache->RemoveMe( this ); 

    }

// ---------------------------------------------------------------------------
// From MXIMPSubscriptionItem class.
// CSearchSubscriptionItem::SynthesiseSubscriptionEventTo()
// ---------------------------------------------------------------------------
//
void CSearchSubscriptionItem::SynthesiseSubscriptionEventToL(
        MXIMPPscContext* aContext, TBool /*aForceEvent*/ )
    {

    CSearchEventImp* newEvent = CSearchEventImp::NewLC(*iSearchId, iSearchResults);      
    if( newEvent )
        {
        MSearchItemParent* searchCache = static_cast<MSearchItemParent*>(iParent.GetInterface(SEARCH_ITEM_PARENT));
        searchCache->AddEventL( *newEvent, aContext ); 
        CleanupStack::PopAndDestroy( newEvent ); //newEvent*/     
        }
    
    }


// ---------------------------------------------------------------------------
// CSearchSubscriptionItem::SetNewListL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CSearchSubscriptionItem::SetSearchResults(RSearchInfoImpArray* aSearchResults)
    {
    iSearchResults =aSearchResults;
    }
// ---------------------------------------------------------------------------
// CSearchSubscriptionItem::SetNewListL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CSearchSubscriptionItem::SetSearchIdL(CXIMPIdentityImp& aSearchId)
    {
    iSearchId = &aSearchId;
    }    
// --------- ------------------------------------------------------------------
// CGroupListSubscriptionItem::CleanExpired()
// ---------------------------------------------------------------------------
//
void CSearchSubscriptionItem::CleanExpired()
    {
    
    }
 
    
// End of file
