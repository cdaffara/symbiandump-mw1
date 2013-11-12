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

#ifndef CPRESENCEDATACACHEIMP_H
#define CPRESENCEDATACACHEIMP_H

#include "ximpapiobjbase.h"
#include "presenceapiobjbase.h"
#include "ximpitemparent.h"
#include "presencetypehelpers.h"
#include "ximpoperationdefs.h"
#include "ximpapieventbase.h"

class MXIMPHost;
class COwnPresenceSubscriptionItem;
class CPresentityPresenceSubscriptionItem;
class CGroupContentSubscriptionItem;
class CWatcherListSubscriptionItem;
class MPresentityGroupInfo;
class CXIMPIdentityImp;
class MXIMPPscContext;
class CXIMPSubscriptionItemBase;
class MPresenceWatcherInfo;
class MXIMPStatus;
class MPresenceBlockInfo;
class CBlockListSubscriptionItem;
class MXIMPIdentity;
class MPresenceInfo;
class MXIMPDataSubscriptionState;
class CXIMPDataSubscriptionStateImp;
class MPresenceCacheWriter;

//class CXIMPApiEventBase;
/**
 * PSC Presence Data Cache implementation.
 *
 * @since S60 v3.2
 */
class CPresenceDataCache : public CBase,
                           public MXIMPItemParent
    {
public: // Definitions

    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENCEDATACACHE };

public: // Construction and destruction

    /**
     * Construction
     */
    IMPORT_C static CPresenceDataCache* NewL( MXIMPHost& aHost );

    /**
     * Destruction
     */
    virtual ~CPresenceDataCache();

private:

    CPresenceDataCache( MXIMPHost& aHost );
    void ConstructL();
    
private: //MXIMPItemParentBase
	TAny* GetInterface(TInt aInterfaceId) ;
    // Expiring items
    void RegisterExpiringItemL( CXIMPSubscriptionItemBase* aExpireItem );
    void UnregisterExpiringItem( CXIMPSubscriptionItemBase* aExpireItem );
	
    
private: // From MXIMPItemParent

    /**
     * @see MXIMPItemParent
     */
    // Subscription items
    
    void RemoveMe( COwnPresenceSubscriptionItem* aChild );
    void RemoveMe( CGroupListSubscriptionItem* aChild );
    void RemoveMe( CGroupContentSubscriptionItem* aChild );
    void RemoveMe( CPresentityPresenceSubscriptionItem* aChild );
    void RemoveMe( CWatcherListSubscriptionItem* aChild );
    void RemoveMe( CPresentityGroupMembersPresenceSubscriptionItem* aChild );
    void RemoveMe( CGrantRequestListSubscriptionItem* aChild );
    void RemoveMe( CBlockListSubscriptionItem* aChild );

    void AddPresentityPresenceSubscriberL( const CXIMPIdentityImp& aIdentity, CXIMPSubscriptionItemBase* aItem, CXIMPIdentityImp* aGroup = NULL );
    void RemovePresentityPresenceSubscriber( const CXIMPIdentityImp& aIdentity, CXIMPSubscriptionItemBase* aItem );
    void ActivatePresentityPresenceSubscriberL( const CXIMPIdentityImp& aIdentity, CXIMPSubscriptionItemBase* aItem );
    void SynthesisePresentityPresenceSubscriptionEventToL( const CXIMPIdentityImp& aIdentity, MXIMPPscContext* aContext, TBool aForceEvent );

    // Configuration items
    void RemoveMe( CPresenceConfigurationItem* aChild );
    void RemoveMe( CPresenceToEveryoneConfigurationItem* aChild );
    void RemoveMe( CGroupMembersPresenceConfigurationItem* aChild );

    void AddPresenceConfiguratorL( const CXIMPIdentityImp& aIdentity, CXIMPConfigurationItemBase* aItem );
    void RemovePresenceConfigurator( const CXIMPIdentityImp& aIdentity, CXIMPConfigurationItemBase* aItem );


    // Other methods
    void AddEventL( CXIMPApiEventBase& aEvent, MXIMPPscContext* aContext );
    
    TInt32 ProtocolUID( );
    
    MPresenceCacheWriter* ExternalCache();

public: // New methods

    /**
     * - If OwnPresenceSubscriptionItem is not found, data cache
     *   initializes new one and returns ownership to caller
     * - If OwnPresenceSubscriptionItem is found, data cache increases
     *   refcount by one, and returns the old object
     */
    IMPORT_C COwnPresenceSubscriptionItem& OwnPresenceSubscriptionItemLC();
    IMPORT_C CGroupListSubscriptionItem& GroupListSubscriptionItemLC();
    IMPORT_C CWatcherListSubscriptionItem& WatcherListSubscriptionItemLC();
    IMPORT_C CGrantRequestListSubscriptionItem& GrantRequestListSubscriptionItemLC();
    IMPORT_C CBlockListSubscriptionItem& BlockListSubscriptionItemLC();
    IMPORT_C CGroupContentSubscriptionItem& GroupContentSubscriptionItemLC(
                                                    const CXIMPIdentityImp& aGroupId );
    IMPORT_C CPresentityPresenceSubscriptionItem& PresentityPresenceSubscriptionItemLC(
                                                    const CXIMPIdentityImp& aIdentity );
    IMPORT_C CPresentityGroupMembersPresenceSubscriptionItem&
                PresentityGroupMembersPresenceSubscriptionItemLC( const CXIMPIdentityImp& aIdentity );

    IMPORT_C CPresenceConfigurationItem& PresenceConfigurationItemLC( const CXIMPIdentityImp& aIdentity );
    IMPORT_C CPresenceToEveryoneConfigurationItem& PresenceToEveryoneConfigurationItemLC();
    IMPORT_C CGroupMembersPresenceConfigurationItem& GroupMembersPresenceConfigurationItemLC(
                                                                    const CXIMPIdentityImp& aGroupId );


    /**
     * Checks if group content subscription item with given group id is found.
     */
    IMPORT_C TBool GroupContentSubscriptionItemExists( const CXIMPIdentityImp& aGroupId );

    /**
     * Clean resources concerning context.
     */
    IMPORT_C void CleanResources( MXIMPPscContext& aContext );

    /**
     *
     */
    IMPORT_C void CleanExpired();

    /**
     * Helper method to pack group id.
     * @param aGroupId The group id to be packed.
     * @return HBufC8 containing the packed group id.
     */
    HBufC8* PackGroupIdLC( const MXIMPIdentity& aGroupId );
    
    IMPORT_C const MXIMPDataSubscriptionState& OwnPresenceDataSubscriptionState() const;
    IMPORT_C const MXIMPDataSubscriptionState& PresentityGroupListDataSubscriptionState() const;
    IMPORT_C const MXIMPDataSubscriptionState& PresentityGroupContentDataSubscriptionState( 
                const MXIMPIdentity& aGroupId ) const;
    IMPORT_C const MXIMPDataSubscriptionState& PresentityPresenceDataSubscriptionState( 
                const MXIMPIdentity& aPresentityId ) const;
    IMPORT_C const MXIMPDataSubscriptionState& PresenceWatcherListDataSubscriptionState() const;
    IMPORT_C const MXIMPDataSubscriptionState& PresenceBlockDataSubscriptionState() const;
    IMPORT_C const MXIMPDataSubscriptionState& PresenceGrantRequestDataSubscriptionState() const;

private: // Helper methods

    /**
     * Template method for removeMe when one subscription item is target.
     * @param aItem Reference to subscription item pointer to remove.
     * @param Pointer of item to remove.
     */
    template< class Type >
    IMPORT_C static void RemoveMe( Type*& aItem, Type* aChild );

    /**
     * Template method for removeme if target subscription item is in array.
     * @param aItemArray Array of subscription items where aChild should exists.
     * @param Pointer of item to remove.
     */
    template< class Type >
    void RemoveMeFromArray( RPointerArray< Type >& aItemArray, Type* aChild );

    /**
     * Templated item instance creation. If item is not found it is created.
     * @param aItem Reference to pointer where new item should be saved.
     * @return Reference to item created.
     */
    template< class Type > 
    Type& ItemLC( Type*& aItem );

    /**
     * Templated item instance creation. If item is not found it is created.
     * @param aItemArray Array of items where new item should be created.
     * @param aIdentity. Key for finding item or for creating the new one.
     * @return Reference to item created.
     */
    template< class Type >
    Type& ArrayItemLC( RPointerArray< Type >& aItemArray, const CXIMPIdentityImp& aIdentity );
    
    /**
     * Templated item instance creation. If item is not found it is created.
     * @param aItemArray Array of items where new item should be created.
     * @param aContentArray. Content array for passing to new created item.
     * @param aIdentity. Key for finding item or for creating the new one.
     * @return Reference to item created.
     */
    template< class Type, class ContentType >
    Type& ArrayItemLC( RPointerArray< Type >& aItemArray, 
                       RPointerArray< ContentType >& aContentArray,
                       const CXIMPIdentityImp& aIdentity );

    /**
     * Get data subscription state for item.
     * @param aItem Item which data subscription state should be found.
     * @return Data subscription state.
     */    
    template< class Type >
    const MXIMPDataSubscriptionState& DataSubscriptionState( Type* aItem ) const;
    
    /**
     * Get data subscription state for item.
     * @param aItemArray Item array where item should be found.
     * @aIdentity Key for finding right item.
     * @return Data subscription state.
     */    
    template< class Type >
    const MXIMPDataSubscriptionState& DataSubscriptionState( const RPointerArray< Type >& aItemArray,
                                                             const MXIMPIdentity& aIdentity ) const;
    
private: // Data

    /**
     * Access to host.
     */
    MXIMPHost& iHost;

    /**
     * Does not own. Removed when RemoveMe called.
     */
    COwnPresenceSubscriptionItem* iOwnSubscriptionItem;
    CGroupListSubscriptionItem* iGroupListSubscriptionItem;
    RPointerArray< CGroupContentSubscriptionItem > iGroupContentSubscriptionItems;
    RPointerArray< CPresentityPresenceSubscriptionItem > iPresentityPresenceSubscriptionItems;
    CWatcherListSubscriptionItem* iWatcherListSubscriptionItem;
    CGrantRequestListSubscriptionItem* iGrantRequestListSubscriptionItem;
    RPointerArray< CPresentityGroupMembersPresenceSubscriptionItem > iGroupMembersPresenceSubscriptionItems;
    CBlockListSubscriptionItem* iBlockListSubscriptionItem;
    
    RPointerArray< CXIMPSubscriptionItemBase > iExpiringSubscriptionItems;

    RPointerArray< CPresenceConfigurationItem > iPresenceConfigurationItems;
    CPresenceToEveryoneConfigurationItem* iPresenceToEveryoneConfigurationItem;
    RPointerArray< CGroupMembersPresenceConfigurationItem > iGroupMembersPresenceConfigurationItems;
    
    /**
     * Empty subscription state to pass adaptation exists request is subscription item does not exists.
     */
    CXIMPDataSubscriptionStateImp* iEmptySubscriptionState;
    
    MPresenceCacheWriter *iExtPresenceCache;
/* ======================================================================== */

    };
#endif // CPRESENCEDATACACHEIMP_H
