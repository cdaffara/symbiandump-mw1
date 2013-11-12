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
* Description:  IM data cache implementation.
*
*/

#ifndef CIMDATACACHEIMP_H
#define CIMDATACACHEIMP_H

#include "ximpapiobjbase.h"
#include "imapiobjbase.h"
//#include "ximpitemparent.h"
#include "ximpimitemparent.h"
#include "imtypehelpers.h"
//#include "ximpoperationdefs.h"
#include "ximpapieventbase.h"

class MXIMPHost;
class CConversationSubscriptionItem;
class CXIMPIdentityImp;
class MXIMPPscContext;
class CXIMPIMSubscriptionItemBase;
class MXIMPStatus;
class MXIMPIdentity;
class MXIMPDataSubscriptionState;
class CXIMPDataSubscriptionStateImp;


/**
 * PSC Im Data Cache implementation.
 *
 * @since S60 v3.2
 */
class CImDataCache : public CBase,
                           public MXIMPIMItemParent
    {
public: // Definitions

    /** The class ID. */
    enum { KClassId = IMIMP_CLSID_CIMDATACACHE };


public: // Construction and destruction

    /**
     * Construction
     */
    IMPORT_C static CImDataCache* NewL( MXIMPHost& aHost );

    /**
     * Destruction
     */
    virtual ~CImDataCache();

private:

    CImDataCache( MXIMPHost& aHost );
    void ConstructL();
    
private:   // MXIMPIMItemParentBase

	TAny* GetInterface(TInt aInterfaceId);
    // Expiring items
    void RegisterExpiringItemL( CXIMPSubscriptionItemBase* aExpireItem );
    void UnregisterExpiringItem( CXIMPSubscriptionItemBase* aExpireItem );
    
private: // From MXIMPIMItemParent

    /**
     * @see MXIMPIMItemParent
     */
    void RemoveMe( CConversationSubscriptionItem* aChild );

    // Other methods
    void AddEventL( CXIMPApiEventBase& aEvent, MXIMPPscContext* aContext );

public: // New methods

    /**
     * - If CConversationSubscriptionItem is not found, data cache
     *   initializes new one and returns ownership to caller
     * - If CConversationSubscriptionItem is found, data cache increases
     *   refcount by one, and returns the old object
     */
	IMPORT_C CConversationSubscriptionItem& ConversationSubscriptionItemLC();
    /**
     * Clean resources concerning context.
     */
    IMPORT_C void CleanResources( MXIMPPscContext& aContext );

    /**
     *
     */
    IMPORT_C void CleanExpired();
    
private: // Helper methods

    /**
     * Template method for removeMe when one subscription item is target.
     * @param aItem Reference to subscription item pointer to remove.
     * @param Pointer of item to remove.
     */
    template< class Type >
    IMPORT_C static void RemoveMe( Type*& aItem, Type* aChild );

    
    /**
     * Templated item instance creation. If item is not found it is created.
     * @param aItem Reference to pointer where new item should be saved.
     * @return Reference to item created.
     */
    template< class Type > 
    Type& ItemLC( Type*& aItem );

   
    
private: // Data

    /**
     * Access to host.
     */
    MXIMPHost& iHost;

    /**
     * Does not own. Removed when RemoveMe called.
     */
 	CConversationSubscriptionItem* iConversationSubscriptionItem;
 
    };

#endif // CIMDATACACHEIMP_H
