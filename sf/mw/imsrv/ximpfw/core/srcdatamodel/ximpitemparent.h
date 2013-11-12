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
* Description:  Item parent interface.
*
*/

#ifndef CXIMPITEMPARENT_H
#define CXIMPITEMPARENT_H

#include <ximpitemparentbase.h>

class COwnPresenceSubscriptionItem;
class CGroupListSubscriptionItem;
class CGroupContentSubscriptionItem;
class CPresentityPresenceSubscriptionItem;
class CPresentityGroupMembersPresenceSubscriptionItem;
class CWatcherListSubscriptionItem;
class CGrantRequestListSubscriptionItem;
//class CXIMPSubscriptionItemBase;
class CXIMPApiEventBase;
class MXIMPPscContext;
class CXIMPIdentityImp;
class CPresenceConfigurationItem;
class CGroupMembersPresenceConfigurationItem;
class CXIMPConfigurationItemBase;
class CBlockListSubscriptionItem;
class CPresenceToEveryoneConfigurationItem;
class MPresenceCacheWriter;

// FIX ME
#define PRESENCE_ITEM_PARENT	0xE0000009

/**
 * Item parent interface.
 *
 * @since S60 v3.2
 */
class MXIMPItemParent : public MXIMPItemParentBase
    {
	public:
		enum { KInterfaceId = PRESENCE_ITEM_PARENT};
    
public: // Definitions

    /**
     * Remove item
     * @param aChild Item pointer to remove.
     */
    virtual void RemoveMe( COwnPresenceSubscriptionItem* aChild ) = 0;
    virtual void RemoveMe( CGroupListSubscriptionItem* aChild ) = 0;
    virtual void RemoveMe( CGroupContentSubscriptionItem* aChild ) = 0;
    virtual void RemoveMe( CPresentityPresenceSubscriptionItem* aChild ) = 0;
    virtual void RemoveMe( CWatcherListSubscriptionItem* aChild ) = 0;
    virtual void RemoveMe( CPresentityGroupMembersPresenceSubscriptionItem* aChild ) = 0;
    virtual void RemoveMe( CGrantRequestListSubscriptionItem* aChild ) = 0;
    virtual void RemoveMe( CBlockListSubscriptionItem* aChild ) = 0;

    virtual void RemoveMe( CPresenceConfigurationItem* aChild ) = 0;
    virtual void RemoveMe( CPresenceToEveryoneConfigurationItem* aChild ) = 0;
    virtual void RemoveMe( CGroupMembersPresenceConfigurationItem* aChild ) = 0;

    /**
     * Add event
     */
    virtual void AddEventL( CXIMPApiEventBase& aEvent, MXIMPPscContext* aContext ) = 0;

    /**
     * Register item for expiration purposes.
     */   
    //virtual void RegisterExpiringItemL( CXIMPSubscriptionItemBase* aExpireItem ) = 0;
    
    /**
     * Unregister item from expiration purposes.
     */
    //virtual void UnregisterExpiringItem( CXIMPSubscriptionItemBase* aExpireItem ) = 0;
    
    /**
     * Add item as subscriber for presentity presence.
     */
    virtual void AddPresentityPresenceSubscriberL( const CXIMPIdentityImp& aIdentity, CXIMPSubscriptionItemBase* aItem, CXIMPIdentityImp* aGroup = NULL ) = 0;

    /**
     * Remove item from subscribing presentity presence.
     */
    virtual void RemovePresentityPresenceSubscriber( const CXIMPIdentityImp& aIdentity, CXIMPSubscriptionItemBase* aItem ) = 0;
    
    /**
     * Activate item for subscribing presentity presence
     */
    virtual void ActivatePresentityPresenceSubscriberL( const CXIMPIdentityImp& aIdentity, CXIMPSubscriptionItemBase* aItem ) = 0;

    /**
     * Synthesise presentity presence subscription event
     */    
    virtual void SynthesisePresentityPresenceSubscriptionEventToL( const CXIMPIdentityImp& aIdentity, MXIMPPscContext* aContext, TBool aForceEvent ) = 0;

    /**
     * Add item as configurator for presentity presence.
     */
    virtual void AddPresenceConfiguratorL( const CXIMPIdentityImp& aIdentity, CXIMPConfigurationItemBase* aItem ) = 0;

    /**
     * Remove item from configuring presentity presence.
     */
    virtual void RemovePresenceConfigurator( const CXIMPIdentityImp& aIdentity, CXIMPConfigurationItemBase* aItem ) = 0;
    
    /**
     * Return the protocol ID of the host of the parent
     */
    virtual TInt32 ProtocolUID( ) = 0;

    /**
     * Return the global presence cache object
     */
    virtual MPresenceCacheWriter* ExternalCache( ) = 0;
    
protected: // Destruction
    
    /**
     * Object cannot be delted through this interface
     */
    virtual ~MXIMPItemParent(){}
    };

/* ======================================================================== */



#endif // CXIMPITEMPARENT_H
