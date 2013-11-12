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
* Description:  Search Keys  subscription implementation.
*
*/


#include <badesca.h>
#include "ximpitemparentbase.h"
#include "ximppanics.h"
#include "ximppsccontext.h"
#include "ximpobjecthelpers.h"

#include "ximpidentityimp.h"
#include "searchkeyssubscriptionitem.h"

#include "searchitemparent.h"
#include "searchkeyseventimp.h"
// ---------------------------------------------------------------------------
// CSearchKeysSubscriptionItem::CSearchKeysSubscriptionItem()
// ---------------------------------------------------------------------------
//
CSearchKeysSubscriptionItem::CSearchKeysSubscriptionItem( MXIMPItemParentBase& aParent )
: CXIMPSubscriptionItemBase( aParent )
    {
    }


// ---------------------------------------------------------------------------
// CSearchKeysSubscriptionItem::ConstructL()
// ---------------------------------------------------------------------------
//
void CSearchKeysSubscriptionItem::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// CSearchKeysSubscriptionItem::NewL()
// ---------------------------------------------------------------------------
//
CSearchKeysSubscriptionItem* CSearchKeysSubscriptionItem::NewLC( MXIMPItemParentBase& aParent )
    {
    CSearchKeysSubscriptionItem* self = new( ELeave ) CSearchKeysSubscriptionItem( aParent );
    CleanupClosePushL( *self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CSearchKeysSubscriptionItem::~CSearchKeysSubscriptionItem()
// ---------------------------------------------------------------------------
//
EXPORT_C CSearchKeysSubscriptionItem::~CSearchKeysSubscriptionItem()
    {
    
    MSearchItemParent* searchCache = static_cast<MSearchItemParent*>
                                     (iParent.GetInterface(SEARCH_ITEM_PARENT));
    searchCache->RemoveMe( this ); 

    }

// ---------------------------------------------------------------------------
// CSearchSubscriptionItem::SetKeyInfoArray()
// ---------------------------------------------------------------------------
//
EXPORT_C void CSearchKeysSubscriptionItem::SetKeyInfoArray(RSearchKeyInfoImpArray* aKeyInfoArray)
    {
    iKeyInfoArray = aKeyInfoArray;
    }

// ---------------------------------------------------------------------------
// From MXIMPSubscriptionItem class.
// CSearchKeysSubscriptionItem::SynthesiseSubscriptionEventToL()
// ---------------------------------------------------------------------------
//
void CSearchKeysSubscriptionItem::SynthesiseSubscriptionEventToL(
        MXIMPPscContext* aContext, TBool /*aForceEvent*/ )
    {

    CSearchKeysEventImp* newEvent = CSearchKeysEventImp::NewLC( iKeyInfoArray );      
    if( newEvent )
        {
        MSearchItemParent* searchCache = static_cast<MSearchItemParent*>(iParent.GetInterface(SEARCH_ITEM_PARENT));
        searchCache->AddEventL( *newEvent, aContext ); 
        CleanupStack::PopAndDestroy( newEvent ); //newEvent    
        }
    
    }


    
// --------- ------------------------------------------------------------------
// CGroupListSubscriptionItem::CleanExpired()
// ---------------------------------------------------------------------------
//
void CSearchKeysSubscriptionItem::CleanExpired()
    {
    
    }
 
    
// End of file
