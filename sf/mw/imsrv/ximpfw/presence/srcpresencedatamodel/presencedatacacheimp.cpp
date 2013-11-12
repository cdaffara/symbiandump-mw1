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
* Description:  Presence Service Connection data cache implementation.
 *
*/


#include <badesca.h>
#include <e32cmn.h>

#include "presencedatacacheimp.h"
#include "ximpidentityimp.h"
#include "presenceinfoimp.h"
#include "presencewatcherinfoimp.h"
#include "watcherlistsubscriptionitem.h"
#include "ximpobjecthelpers.h"
#include "ownpresenceeventimp.h"
#include "ownpresencesubscriptionitem.h"
#include "grouplistsubscriptionitem.h"
#include "groupcontentsubscriptionitem.h"
#include "presentitypresencesubscriptionitem.h"
#include "presentitygroupmemberspresencesubscriptionitem.h"
#include "presentitygroupinfoimp.h"
#include "presentitygroupmemberinfoimp.h"
#include "presencegrantrequestinfoimp.h"
#include "grantrequestlistsubscriptionitem.h"
#include "ximphost.h"
#include "ximppsccontext.h"
#include "ximpoperationdefs.h"
#include "presenceconfigurationitem.h"
#include "presencetoeveryoneconfigurationitem.h"
#include "groupmemberspresenceconfigurationitem.h"
#include "blocklistsubscriptionitem.h"
#include "presenceblockinfoimp.h"
#include "presencefilterstore.h"
//#include "presencecacheclient.h"

#include "ximpapieventbase.h"
#include <presencecachewriter.h>

#include "ximptrace.h"


// ---------------------------------------------------------------------------
// CPresenceDataCache::CPresenceDataCache()
// ---------------------------------------------------------------------------
//
CPresenceDataCache::CPresenceDataCache( MXIMPHost& aHost )
:iHost( aHost )
    {
    }


// ---------------------------------------------------------------------------
// CPresenceDataCache::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceDataCache::ConstructL()
    {
    iEmptySubscriptionState = CXIMPDataSubscriptionStateImp::NewL();    
    iExtPresenceCache = MPresenceCacheWriter::NewL();
    }


// ---------------------------------------------------------------------------
// CPresenceDataCache::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceDataCache* CPresenceDataCache::NewL( MXIMPHost& aHost )
    {
    CPresenceDataCache* self = new( ELeave ) CPresenceDataCache( aHost );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CPresenceDataCache::~CPresenceDataCache()
// ---------------------------------------------------------------------------
//
CPresenceDataCache::~CPresenceDataCache()
    {

    TInt count = iExpiringSubscriptionItems.Count();
    for( TInt a = 0; a < count; ++a )
        {
        iExpiringSubscriptionItems[ a ]->ForceClose();
        }

    if( iOwnSubscriptionItem )
        {
        iOwnSubscriptionItem->ForceClose();
        }
    if( iGroupListSubscriptionItem )
        {
        iGroupListSubscriptionItem->ForceClose();
        }

    count = iGroupContentSubscriptionItems.Count();
    for( TInt a = 0; a < count; ++a )
        {
        iGroupContentSubscriptionItems[ a ]->ForceClose();
        }

    count = iPresentityPresenceSubscriptionItems.Count();
    for( TInt a = 0; a < count; ++a )
        {
        iPresentityPresenceSubscriptionItems[ a ]->ForceClose();
        }

    if ( iWatcherListSubscriptionItem )
        {
        iWatcherListSubscriptionItem->ForceClose();
        }
    if ( iGrantRequestListSubscriptionItem )
        {
        iGrantRequestListSubscriptionItem->ForceClose();
        }
    if ( iBlockListSubscriptionItem )
        {
        iBlockListSubscriptionItem->ForceClose();
        }
    if( iPresenceToEveryoneConfigurationItem )
        {
        iPresenceToEveryoneConfigurationItem->ForceClose();
        }

    count = iGroupMembersPresenceSubscriptionItems.Count();
    for( TInt a = 0; a < count; ++a )
        {
        iGroupMembersPresenceSubscriptionItems[ a ]->ForceClose();
        }
    count = iPresenceConfigurationItems.Count();
    for( TInt a = 0; a < count; ++a )
        {
        iPresenceConfigurationItems[ a ]->ForceClose();
        }
    count = iGroupMembersPresenceConfigurationItems.Count();
    for( TInt a = 0; a < count; ++a )
        {
        iGroupMembersPresenceConfigurationItems[ a ]->ForceClose();
        }
    
    iGroupContentSubscriptionItems.Close();
    iPresentityPresenceSubscriptionItems.Close();
    iExpiringSubscriptionItems.Close();
    iGroupMembersPresenceSubscriptionItems.Close();
    iPresenceConfigurationItems.Close();
    iGroupMembersPresenceConfigurationItems.Close();
    
    delete iEmptySubscriptionState;
    delete iExtPresenceCache;
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::RemoveMe
// ---------------------------------------------------------------------------
//
template< class Type >
EXPORT_C void CPresenceDataCache::RemoveMe( Type*& aItem, Type* aChild )
    {
    if( aChild == aItem )
        {
        aItem = NULL;
        }
    }


// ---------------------------------------------------------------------------
// CPresenceDataCache::RemoveMeFromArray
// ---------------------------------------------------------------------------
//
template< class Type >
void CPresenceDataCache::RemoveMeFromArray( RPointerArray< Type >& aItemArray, Type* aChild )
    {
    TInt itemCount( aItemArray.Count() );
    for ( TInt i( 0 ); i < itemCount; i++ )
        {
        if ( aChild == aItemArray[ i ] )
            {
            aItemArray.Remove( i );
            break;
            }
        }    
    }



// ---------------------------------------------------------------------------
// CPresenceDataCache::RemoveMe
// ---------------------------------------------------------------------------
//
void CPresenceDataCache::RemoveMe( COwnPresenceSubscriptionItem* aChild )
    {
    RemoveMe< COwnPresenceSubscriptionItem >( iOwnSubscriptionItem, aChild );
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::RemoveMe
// ---------------------------------------------------------------------------
//
void CPresenceDataCache::RemoveMe( CGroupListSubscriptionItem* aChild )
    {
    RemoveMe< CGroupListSubscriptionItem >( iGroupListSubscriptionItem, aChild );
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::RemoveMe
// ---------------------------------------------------------------------------
//
void CPresenceDataCache::RemoveMe( CGroupContentSubscriptionItem* aChild )
    {
    RemoveMeFromArray< CGroupContentSubscriptionItem >( 
                                    iGroupContentSubscriptionItems, aChild );
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::RemoveMe
// ---------------------------------------------------------------------------
//
void CPresenceDataCache::RemoveMe( CPresentityPresenceSubscriptionItem* aChild )
    {
    RemoveMeFromArray< CPresentityPresenceSubscriptionItem >( 
                                    iPresentityPresenceSubscriptionItems, aChild );
    }


// ---------------------------------------------------------------------------
// CPresenceDataCache::RemoveMe
// ---------------------------------------------------------------------------
//
void CPresenceDataCache::RemoveMe( CWatcherListSubscriptionItem* aChild )
    {
    RemoveMe< CWatcherListSubscriptionItem >( iWatcherListSubscriptionItem, aChild );
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::RemoveMe
// ---------------------------------------------------------------------------
//
void CPresenceDataCache::RemoveMe( CPresenceConfigurationItem* aChild )
    {
    RemoveMeFromArray< CPresenceConfigurationItem >( 
                                    iPresenceConfigurationItems, aChild );
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::RemoveMe
// ---------------------------------------------------------------------------
//
void CPresenceDataCache::RemoveMe( CPresenceToEveryoneConfigurationItem* aChild )
    {
    RemoveMe< CPresenceToEveryoneConfigurationItem >( iPresenceToEveryoneConfigurationItem, aChild );
    }


// ---------------------------------------------------------------------------
// CPresenceDataCache::RemoveMe
// ---------------------------------------------------------------------------
//
void CPresenceDataCache::RemoveMe( CGroupMembersPresenceConfigurationItem* aChild )
    {
    RemoveMeFromArray< CGroupMembersPresenceConfigurationItem >( 
                                    iGroupMembersPresenceConfigurationItems, aChild );
    }


// ---------------------------------------------------------------------------
// CPresenceDataCache::RemoveMe
// ---------------------------------------------------------------------------
//
void CPresenceDataCache::RemoveMe( CPresentityGroupMembersPresenceSubscriptionItem* aChild )
    {
    RemoveMeFromArray< CPresentityGroupMembersPresenceSubscriptionItem >( 
                                    iGroupMembersPresenceSubscriptionItems, aChild );
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::RemoveMe
// ---------------------------------------------------------------------------
//
void CPresenceDataCache::RemoveMe( CGrantRequestListSubscriptionItem* aChild )
    {
    RemoveMe< CGrantRequestListSubscriptionItem >( iGrantRequestListSubscriptionItem, aChild );
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::RemoveMe
// ---------------------------------------------------------------------------
//
void CPresenceDataCache::RemoveMe( CBlockListSubscriptionItem* aChild )
    {
    RemoveMe< CBlockListSubscriptionItem >( iBlockListSubscriptionItem, aChild );
    }


// ---------------------------------------------------------------------------
// CPresenceDataCache::AddEventL
// ---------------------------------------------------------------------------
//
void CPresenceDataCache::AddEventL( CXIMPApiEventBase& aEvent, MXIMPPscContext* aContext )
    {
    iHost.AddEventL( aEvent, aContext );
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::RegisterExpiringItemL
// ---------------------------------------------------------------------------
//
void CPresenceDataCache::RegisterExpiringItemL( CXIMPSubscriptionItemBase* aExpireItem )
    {
    TInt index = iExpiringSubscriptionItems.Find( aExpireItem );
    if( index == KErrNotFound )
        {
        iExpiringSubscriptionItems.AppendL( aExpireItem );
        User::LeaveIfError( aExpireItem->Open() );
        }
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::UnregisterExpiringItemL
// ---------------------------------------------------------------------------
//
void CPresenceDataCache::UnregisterExpiringItem( CXIMPSubscriptionItemBase* aExpireItem )
    {
    TInt index = iExpiringSubscriptionItems.Find( aExpireItem );
    if( index != KErrNotFound )
        {
        iExpiringSubscriptionItems[ index ]->Close();
        iExpiringSubscriptionItems.Remove( index );
        }
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::AddPresentityPresenceSubscriberL
// ---------------------------------------------------------------------------
//
void CPresenceDataCache::AddPresentityPresenceSubscriberL( const CXIMPIdentityImp& aIdentity,
                                                          CXIMPSubscriptionItemBase* aItem,
                                                          CXIMPIdentityImp* aGroup )
    {
    TRACE(_L("CPresenceDataCache::AddPresentityPresenceSubscriberL() - begin" ) );
    
    CPresentityPresenceSubscriptionItem& presentityItem =
                                        PresentityPresenceSubscriptionItemLC( aIdentity );
                                       
    presentityItem.SetGroupIdentity( aGroup );
    
    TInt error( KErrNone );
    TRAP( error, presentityItem.AddSubscriberL( aItem ); );
    TRACE_1( _L("CPresenceDataCache::AddPresentityPresenceSubscriberL() - presentityItem.AddSubscriberL( aItem )  = %d"), error );
    
    CleanupStack::PopAndDestroy(); // presentityItem
    
    TRACE(_L("CPresenceDataCache::AddPresentityPresenceSubscriberL() - end" ) ); 
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::RemovePresentityPresenceSubscriberL
// ---------------------------------------------------------------------------
//
void CPresenceDataCache::RemovePresentityPresenceSubscriber( const CXIMPIdentityImp& aIdentity,
                                                             CXIMPSubscriptionItemBase* aItem )
    {
    // PresentityPresenceSubscriptionItemLC does not leave if item already exists. It just
    // finds it. If item exists it does not matter if alloc fails or not because there will not
    // be any subscribers in that case. This one is safe to ignore.
    TRAP_IGNORE(
        CPresentityPresenceSubscriptionItem& presentityItem =
                                            PresentityPresenceSubscriptionItemLC( aIdentity );
        presentityItem.RemoveSubscriber( aItem );
        CleanupStack::PopAndDestroy(); // presentityItem
        )
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::AddPresentityPresenceConfiguratorL
// ---------------------------------------------------------------------------
//
void CPresenceDataCache::AddPresenceConfiguratorL( const CXIMPIdentityImp& aIdentity,
                                                          CXIMPConfigurationItemBase* aItem )
    {
    CPresenceConfigurationItem& configItem = PresenceConfigurationItemLC( aIdentity );
    configItem.AddConfiguratorL( aItem );
    CleanupStack::PopAndDestroy(); // configItem
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::RemovePresentityPresenceConfigurator
// ---------------------------------------------------------------------------
//
void CPresenceDataCache::RemovePresenceConfigurator( const CXIMPIdentityImp& aIdentity,
                                                             CXIMPConfigurationItemBase* aItem )
    {
    // PresentityPresenceSubscriptionItemLC does not leave if item already exists. It just
    // finds it. If item exists it does not matter if alloc fails or not because there will not
    // be any subscribers in that case. This one is safe to ignore.
    TRAP_IGNORE(
        CPresenceConfigurationItem& configItem = PresenceConfigurationItemLC( aIdentity );
        configItem.RemoveSubscriber( aItem );
        CleanupStack::PopAndDestroy(); // configItem
        )
    }


// ---------------------------------------------------------------------------
// CPresenceDataCache::ActivatePresentityPresenceSubscriberL
// ---------------------------------------------------------------------------
//
void CPresenceDataCache::ActivatePresentityPresenceSubscriberL( const CXIMPIdentityImp& aIdentity,
                                                               CXIMPSubscriptionItemBase* aItem )
    {
    CPresentityPresenceSubscriptionItem& presentityItem =
                                        PresentityPresenceSubscriptionItemLC( aIdentity );
    presentityItem.ActivateSubscriberL( aItem );
    CleanupStack::PopAndDestroy(); // presentityItem
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::SynthesisePresentityPresenceSubscriptionEventToL
// ---------------------------------------------------------------------------
//
void CPresenceDataCache::SynthesisePresentityPresenceSubscriptionEventToL(
                                            const CXIMPIdentityImp& aIdentity,
                                            MXIMPPscContext* aContext,
                                            TBool aForceEvent )
    {
    CPresentityPresenceSubscriptionItem& presentityItem =
                                        PresentityPresenceSubscriptionItemLC( aIdentity );
    presentityItem.SynthesiseSubscriptionEventToL( aContext, aForceEvent );
    CleanupStack::PopAndDestroy(); // presentityItem
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::ItemLC
// ---------------------------------------------------------------------------
//
template< class Type >
Type& CPresenceDataCache::ItemLC( Type*& aItem )
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
// CPresenceDataCache::ArrayItemLC
// ---------------------------------------------------------------------------
//
template< class Type >
Type& CPresenceDataCache::ArrayItemLC( RPointerArray< Type >& aItemArray, const CXIMPIdentityImp& aIdentity )
    {
    // Search for the subscription
    TInt pos = aItemArray.FindInOrder( aIdentity, Type::IdentityOrder );
    if( pos == KErrNotFound )
        {
        // Create a new subscription item.
        // Shared ownership of item. Item will inform this instance when it is deleted.
        Type* newItem = Type::NewLC( *this, aIdentity );
        
        TLinearOrder< Type > linearOrder( Type::Order );
        aItemArray.InsertInOrder( newItem, linearOrder );
        return *newItem;
        }
    else // Found
        {
        // Get the existing found item
        Type* item = aItemArray[ pos ];
        // Increase reference count for the existing
        User::LeaveIfError( item->Open() );
        CleanupClosePushL( *item );
        return *item;
        }    
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::ArrayItemLC
// ---------------------------------------------------------------------------
//
template< class Type, class ContentType >
Type& CPresenceDataCache::ArrayItemLC( RPointerArray< Type >& aItemArray, 
                                      RPointerArray< ContentType>& aContentArray,
                                      const CXIMPIdentityImp& aIdentity )
    {
    // Search for the subscription
    TInt pos = aItemArray.FindInOrder( aIdentity, Type::IdentityOrder );
    if( pos == KErrNotFound )
        {
        ContentType& contentItem = ArrayItemLC< ContentType >( aContentArray, aIdentity );

        // Create a new subscription item.
        // Shared ownership of item. Item will inform this instance when it is deleted.
        Type* newItem = Type::NewLC( *this, contentItem );
        CleanupStack::Pop(); // newItem
        CleanupStack::PopAndDestroy(); // contentItem NewItem has taken reference to ownership of contentItem
        CleanupClosePushL( *newItem ); // back to stack
        CXIMPIdentityImp* identity = TXIMPObjectCloner< CXIMPIdentityImp >::CloneL( aIdentity );
        // Set the identity
        newItem->SetIdentity( identity ); // Ownership is transferred to callee.
        
        TLinearOrder< Type > linearOrder( Type::Order );
        aItemArray.InsertInOrderL( newItem, linearOrder );
        return *newItem;
        }
    else // Found
        {
        // Get the existing found item
        Type* item = aItemArray[ pos ];
        // Increase reference count for the existing
        User::LeaveIfError( item->Open() );
        CleanupClosePushL( *item );
        return *item;
        }    
    }

    
// ---------------------------------------------------------------------------
// CPresenceDataCache::OwnPresenceSubscriptionItemLC
// ---------------------------------------------------------------------------
//
EXPORT_C COwnPresenceSubscriptionItem& CPresenceDataCache::OwnPresenceSubscriptionItemLC()
    {
    return ItemLC< COwnPresenceSubscriptionItem >( iOwnSubscriptionItem );
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::GroupListSubscriptionItemLC
// ---------------------------------------------------------------------------
//
EXPORT_C CGroupListSubscriptionItem& CPresenceDataCache::GroupListSubscriptionItemLC()
    {
    return ItemLC< CGroupListSubscriptionItem >( iGroupListSubscriptionItem );
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::WatcherListSubscriptionItemLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CWatcherListSubscriptionItem& CPresenceDataCache::WatcherListSubscriptionItemLC()
    {
    return ItemLC< CWatcherListSubscriptionItem >( iWatcherListSubscriptionItem );
    }


// ---------------------------------------------------------------------------
// CPresenceDataCache::GrantRequestListSubscriptionItemLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CGrantRequestListSubscriptionItem&
                CPresenceDataCache::GrantRequestListSubscriptionItemLC()
    {
    return ItemLC< CGrantRequestListSubscriptionItem >( iGrantRequestListSubscriptionItem );
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::BlockListSubscriptionItemLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CBlockListSubscriptionItem&
                CPresenceDataCache::BlockListSubscriptionItemLC()
    {
    return ItemLC< CBlockListSubscriptionItem >( iBlockListSubscriptionItem );
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::GroupContentSubscriptionItemLC
// ---------------------------------------------------------------------------
//
EXPORT_C CGroupContentSubscriptionItem& CPresenceDataCache::GroupContentSubscriptionItemLC(
                                                                    const CXIMPIdentityImp& aGroupId )
    {
    return ArrayItemLC< CGroupContentSubscriptionItem >( iGroupContentSubscriptionItems, aGroupId );
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::GroupContentSubscriptionItemExistsL
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPresenceDataCache::GroupContentSubscriptionItemExists(
                                            const CXIMPIdentityImp& aGroupId )
    {
    // Search for the subscription
    return iGroupContentSubscriptionItems.FindInOrder( 
            aGroupId, CGroupContentSubscriptionItem::IdentityOrder ) != KErrNotFound;
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::PresentityPresenceSubscriptionItemLC
// ---------------------------------------------------------------------------
//
EXPORT_C CPresentityPresenceSubscriptionItem&
            CPresenceDataCache::PresentityPresenceSubscriptionItemLC(
                                            const CXIMPIdentityImp& aIdentity )
    {
    return ArrayItemLC<CPresentityPresenceSubscriptionItem>( iPresentityPresenceSubscriptionItems, 
                                                                    aIdentity );
    }
    
// ---------------------------------------------------------------------------
// CPresenceDataCache::PresentityGroupMembersPresenceSubscriptionItemLC
// ---------------------------------------------------------------------------
//
EXPORT_C CPresentityGroupMembersPresenceSubscriptionItem&
            CPresenceDataCache::PresentityGroupMembersPresenceSubscriptionItemLC(
                                            const CXIMPIdentityImp& aIdentity )
    {
    return ArrayItemLC< CPresentityGroupMembersPresenceSubscriptionItem,
                        CGroupContentSubscriptionItem >( iGroupMembersPresenceSubscriptionItems,
                                                                iGroupContentSubscriptionItems,
                                                                aIdentity );
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::PresenceConfigurationItemLC
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceConfigurationItem& CPresenceDataCache::PresenceConfigurationItemLC(
                                                             const CXIMPIdentityImp& aIdentity )
    {
    return ArrayItemLC< CPresenceConfigurationItem >( iPresenceConfigurationItems, aIdentity );
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::PresenceToEveryoneConfigurationItemLC
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceToEveryoneConfigurationItem& 
                    CPresenceDataCache::PresenceToEveryoneConfigurationItemLC()
    {
    return ItemLC< CPresenceToEveryoneConfigurationItem >( iPresenceToEveryoneConfigurationItem );
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::PresenceConfigurationItemLC
// ---------------------------------------------------------------------------
//
EXPORT_C CGroupMembersPresenceConfigurationItem&
     CPresenceDataCache::GroupMembersPresenceConfigurationItemLC( const CXIMPIdentityImp& aGroupId )
    {
    return ArrayItemLC< CGroupMembersPresenceConfigurationItem,
                        CGroupContentSubscriptionItem >( iGroupMembersPresenceConfigurationItems,
                                                                iGroupContentSubscriptionItems,
                                                                aGroupId );
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::CleanResources
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceDataCache::CleanResources( MXIMPPscContext& aContext )
    {
    TRACE(_L("CPresenceDataCache::CleanResources()" ) ); 
    if( iOwnSubscriptionItem )
        {
        iOwnSubscriptionItem->RemoveSubscriber( &aContext );
        }
    if( iGroupListSubscriptionItem )
        {
        iGroupListSubscriptionItem->RemoveSubscriber( &aContext );
        }

    // Groupmember items have to be cleaned before contentitems.
    TInt count = iGroupMembersPresenceSubscriptionItems.Count();
    for( TInt a = 0; a < count; ++a )
        {
        iGroupMembersPresenceSubscriptionItems[ 0 ]->RemoveSubscriber( &aContext );
        }
    count = iGroupMembersPresenceConfigurationItems.Count();
    for( TInt a = 0; a < count; ++a )
        {
        iGroupMembersPresenceConfigurationItems[ 0 ]->RemoveConfigurator( &aContext );
        }    

    count = iPresentityPresenceSubscriptionItems.Count();
    for( TInt a = 0; a < count; ++a )
        {
        iPresentityPresenceSubscriptionItems[ 0 ]->RemoveSubscriber( &aContext );
        }

    count = iGroupContentSubscriptionItems.Count();
    for( TInt a = 0; a < count; ++a )
        {
        iGroupContentSubscriptionItems[ 0 ]->RemoveSubscriber( &aContext );
        }
        
    if( iWatcherListSubscriptionItem )
        {
        iWatcherListSubscriptionItem->RemoveSubscriber( &aContext );
        }

    if( iGrantRequestListSubscriptionItem )
        {
        iGrantRequestListSubscriptionItem->RemoveSubscriber( &aContext );
        }
        
    count = iGroupMembersPresenceSubscriptionItems.Count();
    for( TInt a = 0; a < count; ++a )
        {
        iGroupMembersPresenceSubscriptionItems[ 0 ]->RemoveSubscriber( &aContext );
        }

    if( iBlockListSubscriptionItem )
        {
        iBlockListSubscriptionItem->RemoveSubscriber( &aContext );
        }

    count = iExpiringSubscriptionItems.Count();
    for( TInt a = 0; a < count; ++a )
        {
        iExpiringSubscriptionItems[ 0 ]->RemoveSubscriber( &aContext );
        }

    count = iPresenceConfigurationItems.Count();
    for( TInt a = 0; a < count; ++a )
        {
        iPresenceConfigurationItems[ 0 ]->RemoveConfigurator( &aContext );
        }

    if( iPresenceToEveryoneConfigurationItem )
        {
        iPresenceToEveryoneConfigurationItem->RemoveConfigurator( &aContext );
        }
    TRACE(_L("CPresenceDataCache::CleanResources done()" ) );
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::CleanExpired
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceDataCache::CleanExpired()
    {
    TRACE(_L("CPresenceDataCache::CleanExpired()" ) );
    // Clean those which were expired.
    TInt count = iExpiringSubscriptionItems.Count();
    for( TInt a = count - 1; a >= 0; --a )
        {
        if( iExpiringSubscriptionItems[ a ]->CleanIfExpired() )
            {
            iExpiringSubscriptionItems[ a ]->Close();
            iExpiringSubscriptionItems.Remove( a );
            }
        }
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::PackGroupIdLC
// ---------------------------------------------------------------------------
//
HBufC8* CPresenceDataCache::PackGroupIdLC( const MXIMPIdentity& aGroupId )
    {
    const CXIMPIdentityImp* groupId =
        TXIMPGetImpClassOrPanic< const CXIMPIdentityImp >::From( aGroupId );
    
    HBufC8* packedGroupId = TXIMPObjectPacker< const CXIMPIdentityImp >::PackL( *groupId );
    CleanupStack::PushL( packedGroupId );
    return packedGroupId;
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::DataSubscriptionState
// ---------------------------------------------------------------------------
//
template< class Type >
const MXIMPDataSubscriptionState& CPresenceDataCache::DataSubscriptionState( Type* aItem ) const
    {
    if( aItem )
        {
        return aItem->DataSubscriptionState();
        }
    return *iEmptySubscriptionState;
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::DataSubscriptionState
// ---------------------------------------------------------------------------
//
template< class Type >
const MXIMPDataSubscriptionState& CPresenceDataCache::DataSubscriptionState( 
                const RPointerArray< Type >& aItemArray, const MXIMPIdentity& aIdentity ) const
    {    
    const CXIMPIdentityImp* identity =
        TXIMPGetImpClassOrPanic< const CXIMPIdentityImp >::From( aIdentity );
    TInt index = aItemArray.FindInOrder( *identity, Type::IdentityOrder );
    if( index >=0 )
        {
        return aItemArray[ index ]->DataSubscriptionState();
        }
    return *iEmptySubscriptionState;    
    }


// ---------------------------------------------------------------------------
// CPresenceDataCache::OwnPresenceDataSubscriptionState
// ---------------------------------------------------------------------------
//
EXPORT_C const MXIMPDataSubscriptionState& CPresenceDataCache::OwnPresenceDataSubscriptionState() const
    {
    return DataSubscriptionState< COwnPresenceSubscriptionItem >( iOwnSubscriptionItem );
    }
    
// ---------------------------------------------------------------------------
// CPresenceDataCache::PresentityGroupDataSubscriptionState
// ---------------------------------------------------------------------------
//
EXPORT_C const MXIMPDataSubscriptionState& CPresenceDataCache::PresentityGroupListDataSubscriptionState() const
    {
    return DataSubscriptionState< CGroupListSubscriptionItem >( iGroupListSubscriptionItem );
    }
    
// ---------------------------------------------------------------------------
// CPresenceDataCache::PresentityGroupMemberDataSubscriptionState
// ---------------------------------------------------------------------------
//
EXPORT_C const MXIMPDataSubscriptionState& CPresenceDataCache::PresentityGroupContentDataSubscriptionState( 
            const MXIMPIdentity& aGroupId ) const
    {
    return DataSubscriptionState< CGroupContentSubscriptionItem >( iGroupContentSubscriptionItems, 
                                                                          aGroupId );
    }
    
// ---------------------------------------------------------------------------
// CPresenceDataCache::PresentityPresenceDataSubscriptionState
// ---------------------------------------------------------------------------
//
EXPORT_C const MXIMPDataSubscriptionState& CPresenceDataCache::PresentityPresenceDataSubscriptionState( 
            const MXIMPIdentity& aPresentityId ) const
    {
    return DataSubscriptionState< CPresentityPresenceSubscriptionItem >( iPresentityPresenceSubscriptionItems, 
                                                                                aPresentityId );
    }
    
// ---------------------------------------------------------------------------
// CPresenceDataCache::PresenceWatcherListDataSubscriptionState
// ---------------------------------------------------------------------------
//
EXPORT_C const MXIMPDataSubscriptionState& CPresenceDataCache::PresenceWatcherListDataSubscriptionState() const
    {    
    return DataSubscriptionState< CWatcherListSubscriptionItem >( iWatcherListSubscriptionItem );
    }
    
// ---------------------------------------------------------------------------
// CPresenceDataCache::PresenceBlockDataSubscriptionState
// ---------------------------------------------------------------------------
//
EXPORT_C const MXIMPDataSubscriptionState& CPresenceDataCache::PresenceBlockDataSubscriptionState() const
    {
    return DataSubscriptionState< CBlockListSubscriptionItem >( iBlockListSubscriptionItem );
    }
    
// ---------------------------------------------------------------------------
// CPresenceDataCache::PresenceGrantRequestDataSubscriptionState
// ---------------------------------------------------------------------------
//
EXPORT_C const MXIMPDataSubscriptionState& CPresenceDataCache::PresenceGrantRequestDataSubscriptionState() const
    {    
    return DataSubscriptionState< CGrantRequestListSubscriptionItem >( iGrantRequestListSubscriptionItem );
    }

// ---------------------------------------------------------------------------
// CPresenceDataCache::ProtocolID
// ---------------------------------------------------------------------------
//
TInt32 CPresenceDataCache::ProtocolUID()
	{
	return iHost.ProtocolUID();
	}

// ---------------------------------------------------------------------------
// CPresenceDataCache::ProtocolID
// ---------------------------------------------------------------------------
//
MPresenceCacheWriter* CPresenceDataCache::ExternalCache()
	{
	return iExtPresenceCache;
	}

TAny* CPresenceDataCache::GetInterface(TInt aInterfaceId)  
	{
		if(aInterfaceId != PRESENCE_ITEM_PARENT)
			{
			User::Panic( _L("CXIMPPresenceCache"), KErrExtensionNotSupported );
			}

		return static_cast<MXIMPItemParent*>(this) ;	

	}

// End of file
