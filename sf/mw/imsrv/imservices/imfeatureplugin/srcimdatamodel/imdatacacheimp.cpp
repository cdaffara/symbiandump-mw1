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
* Description:  IM Service Connection data cache implementation.
 *
*/


#include <badesca.h>
#include <e32cmn.h>

#include "imdatacacheimp.h"
//#include "ximpidentityimp.h"
//#include "ximpobjecthelpers.h"
#include "ximphost.h"
//#include "ximppsccontext.h"
//#include "ximpoperationdefs.h"
#include "ximpapieventbase.h"
//#include "ximptrace.h"
#include "conversationsubscriptionitem.h"
#include "imconversationeventimp.h"

// ---------------------------------------------------------------------------
// CImDataCache::CImDataCache()
// ---------------------------------------------------------------------------
//
CImDataCache::CImDataCache( MXIMPHost& aHost )
:iHost( aHost )
    {
    }
// ---------------------------------------------------------------------------
// CImDataCache::ConstructL()
// ---------------------------------------------------------------------------
//
void CImDataCache::ConstructL()
    {
      
    }
// ---------------------------------------------------------------------------
// CImDataCache::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CImDataCache* CImDataCache::NewL( MXIMPHost& aHost )
    {
  	CImDataCache* self = new( ELeave ) CImDataCache( aHost );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self; 
    }
// ---------------------------------------------------------------------------
// CImDataCache::~CImDataCache()
// ---------------------------------------------------------------------------
//
CImDataCache::~CImDataCache()
    {

    }
// ---------------------------------------------------------------------------
// CImDataCache::RemoveMe
// ---------------------------------------------------------------------------
//
template< class Type >
EXPORT_C void CImDataCache::RemoveMe( Type*& aItem, Type* aChild )
    {
    if( aChild == aItem )
        {
        aItem = NULL;
        }
    }
// ---------------------------------------------------------------------------
// CImDataCache::AddEventL
// ---------------------------------------------------------------------------
//
void CImDataCache::AddEventL( CXIMPApiEventBase& aEvent, MXIMPPscContext* aContext )
    {
    iHost.AddEventL( aEvent, aContext );
    }

// ---------------------------------------------------------------------------
// CImDataCache::RegisterExpiringItemL
// ---------------------------------------------------------------------------
//
void CImDataCache::RegisterExpiringItemL( CXIMPSubscriptionItemBase* /*aExpireItem*/ )
    {
    
    }
// ---------------------------------------------------------------------------
// CImDataCache::UnregisterExpiringItemL
// ---------------------------------------------------------------------------
//
void CImDataCache::UnregisterExpiringItem( CXIMPSubscriptionItemBase* /*aExpireItem*/ )
    {
 
    }
// ---------------------------------------------------------------------------
// CImDataCache::ItemLC
// ---------------------------------------------------------------------------
//
template< class Type >
Type& CImDataCache::ItemLC( Type*& aItem )
    {
   if( !aItem )
        {
        aItem = Type::NewLC( *this );
        }
    else
        {
        User::LeaveIfError( aItem->Open() );
        CleanupClosePushL( *aItem );
        }
	return *aItem;
    }
// ---------------------------------------------------------------------------
// CImDataCache::CleanResources
// ---------------------------------------------------------------------------
//
EXPORT_C void CImDataCache::CleanResources( MXIMPPscContext& /*aContext*/ )
    {

    }
// ---------------------------------------------------------------------------
// CImDataCache::CleanExpired
// ---------------------------------------------------------------------------
//
EXPORT_C void CImDataCache::CleanExpired()
    {
    
    }
// ---------------------------------------------------------------------------
// CImDataCache::RemoveMe
// ---------------------------------------------------------------------------
//    
void CImDataCache::RemoveMe( CConversationSubscriptionItem* aChild )
    {
   RemoveMe< CConversationSubscriptionItem >( 
                                    iConversationSubscriptionItem, aChild ); 
    }
// ---------------------------------------------------------------------------
// CImDataCache::ConversationSubscriptionItemLC
// ---------------------------------------------------------------------------
//
EXPORT_C CConversationSubscriptionItem& CImDataCache::ConversationSubscriptionItemLC()
    {
    return ItemLC< CConversationSubscriptionItem >( iConversationSubscriptionItem );
    }
    
TAny* CImDataCache::GetInterface(TInt aInterfaceId)  
	{
		if(aInterfaceId == IM_ITEM_PARENT)
			{
			return static_cast<MXIMPIMItemParent*>(this) ;	
			}
		else
			{
			User::Panic( _L("CXIMPIMCache"), KErrExtensionNotSupported );
			}
		return NULL ;
	}

// End of file
