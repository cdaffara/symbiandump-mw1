/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  message change observer
*
*/



#ifndef __MIMCACHECLIENT_H
#define __MIMCACHECLIENT_H

#include <e32std.h>
#include <imcachedefs.h>

/**
 * An interface through which client gets information
 * about available  data updates.
 *
 * @since
 */
class MIMCacheClient
    {
    public:
    
    	
		/**
    	* Registeres the cache update observer to server
    	* @param aStatus status message to regisetr
		* @param aRight, operations
    	*/
        virtual void RegisterObserverToServerL( TRequestStatus& aStatus, EIMCacheOperations aRight ) = 0;
        
        /**
    	* unRegisteres the cache update  observer to server
    	* @param aStatus status message to regisetr
		* @param aRight, operations
    	*/
        virtual void UnRegisterObserverToServerL( TRequestStatus& aStatus, EIMCacheOperations aRight ) = 0 ;
              
        /**
        * Gets called when data update is available.
        * Client can then decide wether to use the old one
        * @param aRight, operations
        * @param abuuferptr 
        */
        virtual TPtr8 GetBufferedDataL(EIMCacheOperations aOperationType ) = 0 ;
        
        /**
         * cancels any outstanding requests pending from the server
         * @param aStatus status message to regisetr
         * @param aRight,right is operations
         */
        virtual void CancelRequestL( TRequestStatus& aStatus, EIMCacheOperations aRight) = 0;
         
protected:
    	/**
    	* Destructor
    	*/
    	virtual ~MIMCacheClient() {};
    };

#endif //__MIMCACHECLIENT_H


