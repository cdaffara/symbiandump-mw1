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
* Description:  IM Service Connection Conversation subscription implementation.
*
*/


#include <badesca.h>

#include "ximpimitemparent.h"
//#include "ximppanics.h"
//#include "ximppsccontext.h"
//#include "ximpobjecthelpers.h"
#include "imtypehelpers.h"
#include "imconversationeventimp.h"
//#include "ximpidentityimp.h"
//#include "ximpdatasubscriptionstateimp.h"
#include "conversationsubscriptionitem.h"

// ---------------------------------------------------------------------------
// CConversationSubscriptionItem::CConversationSubscriptionItem()
// ---------------------------------------------------------------------------
//
CConversationSubscriptionItem::CConversationSubscriptionItem( MXIMPItemParentBase& aParent )
: CXIMPSubscriptionItemBase( aParent )
    {
    }


// ---------------------------------------------------------------------------
// CConversationSubscriptionItem::ConstructL()
// ---------------------------------------------------------------------------
//
void CConversationSubscriptionItem::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// CConversationSubscriptionItem::NewL()
// ---------------------------------------------------------------------------
//
CConversationSubscriptionItem* CConversationSubscriptionItem::NewLC( MXIMPItemParentBase& aParent )
    {
    CConversationSubscriptionItem* self = new( ELeave ) CConversationSubscriptionItem( aParent );
    CleanupClosePushL( *self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CConversationSubscriptionItem::~CConversationSubscriptionItem()
// ---------------------------------------------------------------------------
//
EXPORT_C CConversationSubscriptionItem::~CConversationSubscriptionItem()
    {
    MXIMPIMItemParent* imCache = static_cast<MXIMPIMItemParent*>(iParent.GetInterface(IM_ITEM_PARENT));
    imCache->RemoveMe( this );
    }

// ---------------------------------------------------------------------------
// From MXIMPSubscriptionItem class.
// CConversationSubscriptionItem::SynthesiseSubscriptionEventTo()
// ---------------------------------------------------------------------------
//
void CConversationSubscriptionItem::SynthesiseSubscriptionEventToL(
        MXIMPPscContext* aContext, TBool /*aForceEvent*/ )
    {

    CImConversationEventImp* newEvent = CImConversationEventImp::NewLC( iConvinfo );      
    if( newEvent )
        {
         MXIMPIMItemParent* imCache = static_cast<MXIMPIMItemParent*>(iParent.GetInterface(IM_ITEM_PARENT));
        //static_cast<MXIMPIMItemParent&>(iParent).AddEventL( *newEvent, aContext ); 
         imCache->AddEventL( *newEvent, aContext ); 
        }
    CleanupStack::PopAndDestroy( newEvent ); //newEvent
    }


// ---------------------------------------------------------------------------
// CConversationSubscriptionItem::SetNewListL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CConversationSubscriptionItem::SetConversationInfoL(CImConversationInfoImp* aConvinfo)
    {
    iConvinfo = aConvinfo;
    }
// ---------------------------------------------------------------------------
// CGroupListSubscriptionItem::CleanExpired()
// ---------------------------------------------------------------------------
//
void CConversationSubscriptionItem::CleanExpired()
    {
    
    }
 
    
// End of file
