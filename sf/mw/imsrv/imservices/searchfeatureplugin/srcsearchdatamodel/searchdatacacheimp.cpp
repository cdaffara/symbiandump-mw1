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
* Description:  search data cache implementation.
 *
*/


#include <badesca.h>
#include <e32cmn.h>
#include <ximphost.h>
#include <ximpapieventbase.h>

#include "searchkeyssubscriptionitem.h"
#include "searchdatacacheimp.h"
#include "searchsubscriptionitem.h"
#include "searcheventimp.h"

// ---------------------------------------------------------------------------
// CSearchDataCache ::CSearchDataCache ()
// ---------------------------------------------------------------------------
//
CSearchDataCache ::CSearchDataCache ( MXIMPHost& aHost )
:iHost( aHost )
    {
    }
// ---------------------------------------------------------------------------
// CSearchDataCache ::ConstructL()
// ---------------------------------------------------------------------------
//
void CSearchDataCache ::ConstructL()
    {
      
    }
// ---------------------------------------------------------------------------
// CSearchDataCache ::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CSearchDataCache * CSearchDataCache ::NewL( MXIMPHost& aHost )
    {
  	CSearchDataCache * self = new( ELeave ) CSearchDataCache ( aHost );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self; 
    }
// ---------------------------------------------------------------------------
// CSearchDataCache ::~CSearchDataCache ()
// ---------------------------------------------------------------------------
//
CSearchDataCache ::~CSearchDataCache ()
    {

    }
// ---------------------------------------------------------------------------
// CSearchDataCache ::RemoveMe
// ---------------------------------------------------------------------------
//
template< class Type >
EXPORT_C void CSearchDataCache ::RemoveMe( Type*& aItem, Type* aChild )
    {
    if( aChild == aItem )
        {
        aItem = NULL;
        }
    }
// ---------------------------------------------------------------------------
// CSearchDataCache ::AddEventL
// ---------------------------------------------------------------------------
//
void CSearchDataCache ::AddEventL( CXIMPApiEventBase& aEvent, MXIMPPscContext* aContext )
    {
    iHost.AddEventL( aEvent, aContext );
    }

// ---------------------------------------------------------------------------
// CSearchDataCache ::RegisterExpiringItemL
// ---------------------------------------------------------------------------
//
void CSearchDataCache ::RegisterExpiringItemL( CXIMPSubscriptionItemBase* /*aExpireItem*/ )
    {
    
    }
// ---------------------------------------------------------------------------
// CSearchDataCache ::UnregisterExpiringItemL
// ---------------------------------------------------------------------------
//
void CSearchDataCache ::UnregisterExpiringItem( CXIMPSubscriptionItemBase* /*aExpireItem*/ )
    {
 
    }
// ---------------------------------------------------------------------------
// CSearchDataCache ::ItemLC
// ---------------------------------------------------------------------------
//
template< class Type >
Type& CSearchDataCache ::ItemLC( Type*& aItem )
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
// CSearchDataCache ::CleanResources
// ---------------------------------------------------------------------------
//
EXPORT_C void CSearchDataCache ::CleanResources( MXIMPPscContext& /*aContext*/ )
    {

    }
// ---------------------------------------------------------------------------
// CSearchDataCache ::CleanExpired
// ---------------------------------------------------------------------------
//
EXPORT_C void CSearchDataCache ::CleanExpired()
    {
    
    }
// ---------------------------------------------------------------------------
// CSearchDataCache ::RemoveMe
// ---------------------------------------------------------------------------
//    
void CSearchDataCache ::RemoveMe( CSearchSubscriptionItem* aChild )
    {
   	RemoveMe< CSearchSubscriptionItem >( 
                                    iSearchSubscriptionItem, aChild ); 
    }
// ---------------------------------------------------------------------------
// CSearchDataCache ::CSearchSubscriptionItemLC
// ---------------------------------------------------------------------------
//
EXPORT_C CSearchSubscriptionItem& CSearchDataCache ::SearchSubscriptionItemLC()
    {
    return ItemLC< CSearchSubscriptionItem >( iSearchSubscriptionItem );
    }
// ---------------------------------------------------------------------------
// CSearchDataCache ::RemoveMe
// ---------------------------------------------------------------------------
//    
void CSearchDataCache ::RemoveMe( CSearchKeysSubscriptionItem* aChild )
    {
   	RemoveMe< CSearchKeysSubscriptionItem >( 
                                    iSearchKeySubscriptionItem, aChild ); 
    }
// ---------------------------------------------------------------------------
// CSearchDataCache ::CSearchKeysSubscriptionItemLC
// ---------------------------------------------------------------------------
//
EXPORT_C CSearchKeysSubscriptionItem& CSearchDataCache ::SearchKeysSubscriptionItemLC()
    {
    return ItemLC< CSearchKeysSubscriptionItem >( iSearchKeySubscriptionItem );
    }    
// ---------------------------------------------------------------------------
// CSearchDataCache ::GetInterface
// ---------------------------------------------------------------------------
//    
TAny* CSearchDataCache ::GetInterface(TInt aInterfaceId)  
	{
		if(aInterfaceId == SEARCH_ITEM_PARENT)
			{
			return static_cast<MSearchItemParent*>(this) ;	
			}
		else
			{
			User::Panic( _L("CXIMPIMCache"), KErrExtensionNotSupported );
			return NULL; // this line never gets executed :Just to remove warning
			}			
	}

// End of file
