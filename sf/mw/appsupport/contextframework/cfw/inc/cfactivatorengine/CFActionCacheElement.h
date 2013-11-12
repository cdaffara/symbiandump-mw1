/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CFActionObject class declaration.
 *
*/


#ifndef C_CFACTIONCACHEELEMENT_H
#define C_CFACTIONCACHEELEMENT_H

#include <e32base.h>

#include "CFActionSubscriptionImpl.h"
#include "cfactionowner.h"

/**
 *  Action cache element.
 *  Combines action identifier with a subscriber.
 *  Subscriber is informed when this action is needed to be
 *  executed.
 *
 *  @lib -
 *  @since S60 5.0
 */
NONSHARABLE_CLASS( CCFActionCacheElement ): public CBase
    {
public: // Constructors and destructor

    /**
     * Two phased constructors.
     * 
     * @since S60 5.0
     * @param aActionObject Action object related to the cache.
     * @param aSecurityPolicy Security policy related to this action.
     * @return CCFActionCacheElement*
     */
    static CCFActionCacheElement* NewL(
        const TDesC& aActionId,
        const TSecurityPolicy& aSecurityPolicy,
        const TUid& aOwnerUid,
        MCFActionOwner* aOwner );
    static CCFActionCacheElement* NewLC(
        const TDesC& aActionId,
        const TSecurityPolicy& aSecurityPolicy,
        const TUid& aOwnerUid,
        MCFActionOwner* aOwner );

    // Destructor.
    virtual ~CCFActionCacheElement();

public:

    /**
     * Checks if the action object is already stored in
     * the cache.
     *
     * @since S60 5.0
     * @param aActionObject Action identifier to compare.
     * @return ETrue if same
     */
    TBool CompareByIdentifier( const TDesC& aActionId );

    /**
     * Adds a new subscription for the action.
     * 
     * @since S60 5.0
     * @param aSubscription New subscription.
     * @return None
     */
    void AddSubscriptionL( CCFActionSubscription* aSubscription );

    /**
     * Returns current subscribers.
     * 
     * @since S60 5.0
     * @param None
     * @return const RActionSubscriptionArray&
     */
    const RActionSubscriptionArray& Subscriptions() const;

    /**
     * Returns security level for the action.
     * Every subscriber (client) needs to have these
     * capabilities.
     * 
     * @since S60 5.0
     * @param None
     * @return const TSecurityPolicy&
     */
    const TSecurityPolicy& SecurityPolicy() const;

    /**
     * Removes a subscription from the cache.
     * 
     * @since S60 5.0
     * @param aSubscription Subscription to remove.
     * @return None
     */
    void RemoveSubscription( CCFActionSubscription& aSubscription );
    
    /**
     * Sets action owner.
     * @param aOwner Action owner.
     */
    void SetOwner( MCFActionOwner* aOwner );
    
    /**
     * Get action owner.
     * @return MCFActionOwner*.
     */
    MCFActionOwner* Owner() const;
    
    /**
     * Get owner uid.
     * @return const TUid&.
     */
    const TUid& OwnerUid() const;
    
    /**
     * Action ID.
     * @return const TDesC&.
     */
    const TDesC& ActionId() const;
    
    /**
     * Owner type.
     * @return MCFActionOwner::TCFActionOwner.
     */
    MCFActionOwner::TCFActionOwner OwnerType() const;
    
private:

    /**
     * C++ constructor.
     */
    CCFActionCacheElement( const TSecurityPolicy& aSecurityPolicy,
        const TUid& aOwnerUid, MCFActionOwner* aOwner );
    
    /**
     * 2nd phase constructor
     */
    void ConstructL( const TDesC& aActionId );

private: // Data

    // Own: Action identifier
    HBufC* iActionId;

    // Own: Subscriptions
    RActionSubscriptionArray iSubscriptions;

    // Own: Security policy
    TSecurityPolicy iSecurityPolicy;

    // Own: Action owner UID
    TUid iOwnerUid;
    
    // Ref: Pointer to the owner
    MCFActionOwner* iOwner;
    
    // Owner type to resolve where triggered action needs to be sent
    MCFActionOwner::TCFActionOwner iOwnerType;
    };
    
// Action cache array definition
typedef RPointerArray<CCFActionCacheElement> RActionCacheArray;

#endif
