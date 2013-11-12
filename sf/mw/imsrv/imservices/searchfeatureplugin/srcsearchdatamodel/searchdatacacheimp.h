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
* Description:  SEARCH data cache implementation.
*
*/

#ifndef CSEARCHDATACACHEIMP_H
#define CSEARCHDATACACHEIMP_H

#include <ximpapiobjbase.h>
#include "searchapiobjbase.h"

#include "searchitemparent.h"
#include "searchtypehelpers.h"


class MXIMPHost;
class CCSearchSubscriptionItem;
class CXIMPIdentityImp;
class MXIMPPscContext;
class CXIMPSubscriptionItemBase;
class MXIMPStatus;
class MXIMPIdentity;
class CSearchKeysSubscriptionItem;
/**
 * search  data cache implementation
 *
 * @lib searchdatamodel.dll
 */
class CSearchDataCache : public CBase,
                     	public MSearchItemParent
    {
public: // Definitions

    /** The class ID. */
    enum { KClassId = IMP_CLSID_CSEARCHDATACACHE };


public: // Construction and destruction

    /**
     * Construction
     */
    IMPORT_C static CSearchDataCache  * NewL( MXIMPHost& aHost );

    /**
     * Destruction
     */
    virtual ~CSearchDataCache  ();

private:
 	/**
     * construction
     */
    CSearchDataCache  ( MXIMPHost& aHost );
     /**
     * leaving constructor
     */
    void ConstructL();
    
private:   // MXIMPItemParentBase
	/**
     * @see MXIMPItemParentBase
     */

	TAny* GetInterface(TInt aInterfaceId);
    /**
     * @see MXIMPItemParentBase
     */
    void RegisterExpiringItemL( CXIMPSubscriptionItemBase* aExpireItem );
    /**
     * @see MXIMPItemParentBase
     */
    void UnregisterExpiringItem( CXIMPSubscriptionItemBase* aExpireItem );
    
private: // From MSearchItemParent

    /**
     * @see MSearchItemParent
     */
    void RemoveMe( CSearchSubscriptionItem* aChild );
    
    void RemoveMe( CSearchKeysSubscriptionItem* aChild );
    
	/**
     * @see MSearchItemParent
     */
    void AddEventL( CXIMPApiEventBase& aEvent, MXIMPPscContext* aContext );

public: // New methods

    /**
     * - If CSearchSubscriptionItem is not found, data cache
     *   initializes new one and returns ownership to caller
     * - If CSearchSubscriptionItem is found, data cache increases
     *   refcount by one, and returns the old object
     */
	IMPORT_C CSearchSubscriptionItem& SearchSubscriptionItemLC();
	
	/**
     * - If CSearchKeysSubscriptionItem is not found, data cache
     *   initializes new one and returns ownership to caller
     * - If CSearchKeysSubscriptionItem is found, data cache increases
     *   refcount by one, and returns the old object
     */
	IMPORT_C CSearchKeysSubscriptionItem& SearchKeysSubscriptionItemLC();
	
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
     * Access to host. not owned
     */
    MXIMPHost& iHost;

    /**
     * Does not own. Removed when RemoveMe called.
     */
 	CSearchSubscriptionItem* iSearchSubscriptionItem;
 	CSearchKeysSubscriptionItem* iSearchKeySubscriptionItem;
    };

#endif // CSearchDataCache  IMP_H
