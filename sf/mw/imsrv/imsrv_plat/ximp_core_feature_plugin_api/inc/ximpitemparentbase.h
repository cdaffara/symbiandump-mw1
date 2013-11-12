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

#ifndef CXIMPITEMPARENTBASE_H
#define CXIMPITEMPARENTBASE_H

class CXIMPSubscriptionItemBase;

class MXIMPItemParentBase
	{
	public:
	 /**
     * Register item for expiration purposes.
     */   
    virtual void RegisterExpiringItemL( CXIMPSubscriptionItemBase* aExpireItem ) = 0;
    
    /**
     * Unregister item from expiration purposes.
     */
    virtual void UnregisterExpiringItem( CXIMPSubscriptionItemBase* aExpireItem ) = 0;
    
    /**
     * Get requested object
     */
    virtual TAny* GetInterface(TInt aInterfaceId) = 0;
    
protected: // Destruction
    
    /**
     * Object cannot be delted through this interface
     */
    virtual ~MXIMPItemParentBase(){}

	};


#endif //CXIMPITEMPARENTBASE_H
