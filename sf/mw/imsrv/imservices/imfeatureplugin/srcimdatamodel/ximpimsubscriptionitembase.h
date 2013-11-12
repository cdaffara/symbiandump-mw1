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
* Description:  IM Service Connection subscription item base.
*
*/

#ifndef CXIMPPSCSUBSCRIPTIONITEMBASE_H
#define CXIMPPSCSUBSCRIPTIONITEMBASE_H

#include <e32base.h>
#include <ximpdatasubscriptionstate.h>
#include "ximpsubscriptionitem.h"


class MXIMPIMItemParent;
class MXIMPPscContext;
class CXIMPDataSubscriptionStateImp;

/**
 * PSC subscription implementation
 *
 * @since S60 v3.2
 */
class CXIMPIMSubscriptionItemBase : public CObject, public MXIMPSubscriptionItem
    {
public: // Definitions

    /**
     * Subscription status
     */
    enum TSubscriptionStatus
        {
        ENotSubscribedAtAll,
        ESubscribedForOtherCtxOnly,
        ESubscribedForCtxOnly,
        ESubscribedForCtxAndOthers
        };

    /**
     * Subscription update event.
     * Update subscription based on event.
     */
    enum TSubscriptionEvent
        {
        ECompleteDataReceived
        };
	/**
     * For linear ordering by pointers
     */
    static TInt OrderOfContexts( const MXIMPPscContext& aFirst,
                                 const MXIMPPscContext& aSecond );

    /**
     * For linear ordering by pointers
     */
    static TInt OrderOfItems( const CXIMPIMSubscriptionItemBase& aFirst,
                              const CXIMPIMSubscriptionItemBase& aSecond );

protected: // Construction and destruction

    /**
     * Destruction
     * - Remember to inform parent about delete.
     */
    IMPORT_C virtual ~CXIMPIMSubscriptionItemBase();

public: // Construction and destruction

    /**
     * Construction
     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
     */
    IMPORT_C CXIMPIMSubscriptionItemBase( MXIMPIMItemParent& aParent );

    /**
     * Base construction. Must be called from inherited classes ConstructL
     */
    IMPORT_C void BaseConstructL();


public: // New methods

    IMPORT_C TSubscriptionStatus SubscriptionStatus( MXIMPPscContext* aContext );

    /**
     * Add subscriber for item.
     * @param aContext. Context to be added.
     */
    IMPORT_C void AddSubscriberL( MXIMPPscContext* aContext );

    /**
     * Remove subscriber from item
     * @param aContext. Context to be removed.
     */
    IMPORT_C void RemoveSubscriber( MXIMPPscContext* aContext );

    /**
     * Add subscriber for item.
     * @param aItem. Item to be added.
     */
    IMPORT_C void AddSubscriberL( CXIMPIMSubscriptionItemBase* aItem );

    /**
     * Remove subscriber from item
     * @param aContext. Item to be removed.
     */
    IMPORT_C void RemoveSubscriber( CXIMPIMSubscriptionItemBase* aItem );

    /**
     * Activate subscriber for item.
     * @param aItem. Item to be activated.
     */
    IMPORT_C void ActivateSubscriberL( CXIMPIMSubscriptionItemBase* aItem );

    /**
     * Call RemoveSubscriber for all contexts registered to item.
     */
    IMPORT_C void ForceClose();

    /**
     * Clean item if expired.
     * @return ETrue if expired and cleaned.EFalse if not.
     */
    IMPORT_C TBool CleanIfExpired();

    /**
     * Update subscription based on event happened.
     * @param aUpdateState. Event which happened.
     */
    IMPORT_C void UpdateSubscriptionStateL( TSubscriptionEvent aUpdateState );
    
    /**
     *
     */
    IMPORT_C const MXIMPDataSubscriptionState& DataSubscriptionState();
        
    /**
     * Count of subscribed contexts.
     */
    IMPORT_C TInt ContextCount() const;

    /**
     * Reference to context in index.
     */
    IMPORT_C MXIMPPscContext& Context( TInt aIndex );

    /**
     * Can context be found from contexts.
     */
    IMPORT_C TBool IsContext( MXIMPPscContext* aContext ) const;

public: // From MXIMPSubscriptionItem

    /**
     * @see MXIMPSubscriptionItem
     */
    IMPORT_C void SynthesiseSubscriptionEventToAllCtxsL();

    /**
     * @see MXIMPSubscriptionItem
     */
    IMPORT_C void ActivateSubscriberL( MXIMPPscContext* aContext );
        
    /**
     * @see MXIMPSubscriptionItem
     */
    IMPORT_C void SetDataSubscriptionStateL( CXIMPDataSubscriptionStateImp& aNewState );

public: // Helpers

    /**
     * Status for context
     */
    IMPORT_C CXIMPDataSubscriptionStateImp* StatusLC( MXIMPPscContext* aContext ) const;

    /**
     * Update time to new expire time.
     */
    IMPORT_C void UpdateExpirationL();

    /**
     * Change status after subscriber is added/removed.
     */
    IMPORT_C void ChangeStatusAfterAdd();
    IMPORT_C void ChangeStatusAfterRemove();

    /**
     * Count of subscribers. Items counted as 1.
     */
    IMPORT_C TInt CountOfSubscribers();

protected: // New interface methods.

    /**
     * Clean data because expiration
     */
    virtual void CleanExpired() = 0;

protected: // Data

    /**
     * Parent object to inform when object is deleted.
     * Used also for sending events.
     */
    MXIMPIMItemParent& iParent;


    /**
     * Subscribed contexts.
     */
    RPointerArray< MXIMPPscContext > iContexts;
    RPointerArray< MXIMPPscContext > iPreContexts;
    RPointerArray< CXIMPIMSubscriptionItemBase > iItems;
    RPointerArray< CXIMPIMSubscriptionItemBase > iPreItems;

    /**
     * Current IM info status.
     */
    CXIMPDataSubscriptionStateImp* iSubscriptionState;

    /**
     * Time when ready for expire.
     */
    TTime iExpireTime;
    };


/* ======================================================================== */



#endif // CXIMPPSCSUBSCRIPTIONITEMBASE_H
