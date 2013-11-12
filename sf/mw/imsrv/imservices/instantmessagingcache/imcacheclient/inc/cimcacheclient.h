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
* Description:  client class to connect to server
*
*/



#ifndef __CIMCACHECLIENT_H__
#define __CIMCACHECLIENT_H__

#include <e32base.h>
#include <imcachedefs.h>
#include "mimcacheclient.h"

class MIMCacheEventHandler;
class CIMCacheEventHandler;
class MIMCacheEventHandler;
class CIMCacheAccessEventHandler;
/**
 * CIMCacheClient ClientServer 
  */
class CIMCacheClient : public CBase,
					   public RSessionBase,
					   public MIMCacheClient
    {
    
    public:     // Constructor and destructor
    	/*
    	* CIMCacheClient
       	*/
         CIMCacheClient();
        
        /*
    	* ~CIMCacheClient
       	*/
        ~CIMCacheClient();


    public:     // New methods

		/**
    	* Connect
       	*/
        TInt Connect();      
       
       	/**
    	* Register to observer converstaionitems data updates
    	* @param aObserver the observer interface
    	*/
        void RegisterUpdateObserverL( MIMCacheEventHandler& aObserver );

    	/**
    	* Unregister a converstaionitems data update observer
    	* @param aObserver the observer interface
    	*/
        void UnRegisterUpdateObserver( MIMCacheEventHandler& aObserver );
		/**
    	* Register to observer converstaionitems data access
    	* @param aObserver the observer interface
    	*/
        void RegisterAccessObserverL( MIMCacheEventHandler& aObserver );

    	/**
    	* Unregister a converstaionitems data access observer
    	* @param aObserver the observer interface
    	*/
        void UnRegisterAccessObserver( MIMCacheEventHandler& aObserver);

		/**
    	* start the transaction and perform the operation
    	* @param aOperationType , operation to perform
    	*/
        TInt StartTransactionL( EIMCacheOperations aOperationType );
        
		/**
    	* start the transaction and perform the operation 
    	* @param aText ,text to send to server
		* @param aOperationType operation to perform
    	*/
		void StartTransactionL( EIMCacheOperations aOperationType,
								const TDesC& aText
								) ;
									   
		/**
    	* start the transaction and perform the operation 
    	* @param aServiceId service id to use
		* @param aUserId user id to use
		* @param aOperationType operation to perform
    	*/
		void StartTransactionL(	EIMCacheOperations aOperationType ,
								TInt aServiceId,
								const TDesC& aText
								);
        
       	/**
    	* start the transaction and perform the operation 
    	* @param aServiceId service id to use
		* @param aSender user id to use
		* @param aRecipient,recipient user id
		* @param aOperationType operation to perform
    	*/						
		void StartTransactionL( EIMCacheOperations aOperationType,
								TInt aServiceId,
								const TDesC& aText1 ,
						   	  	const TDesC& aText2
						   		) ;
									   		
		 
		/**
    	* to get the all unread message count from server
    	* @param , aOwnUserId logged in user id
    	* @return number of unread msg count
    	*/
		TInt GetInformationL(EIMCacheOperations aOperationType , TInt aServiceId,const TDesC& aBuddyId );
		
	   /**
    	* to get the count from server
        * @param aOperationType operation to perform
    	* @param aServiceId, service id used
    	* @param , aOwnUserId logged in user id
    	* @return number of all unread msg count
    	*/
        TInt GetInformationL(EIMCacheOperations aOperationType, TInt aServiceId );	
      
		
		/**
    	* to get the list of availabe chats from server
       	* @param aServiceId, service id used
       	* @return list buffer
    	*/
	    TPtr8 GetChatListL( const TInt& aServiceId = -1);
									   										   		
private: 
		
		/**
    	* CreateArgumentsL pack the given parameters 
    	* @param aServiceId service id to use
		* @param aUserId user id to use
		* @param aText,a text to send 
		* @return TIpcArgs, packed arguments
	   	*/									   		
		TIpcArgs CreateArgumentsL( TInt aServiceId, const TDesC& aText1  ,
									   const TDesC& aText2  );
									   
	   	/**
    	* CreateArgumentsL pack the given parameters  
    	* @param aServiceId service id to use
		* @param aUserId user id to use
		* @return TIpcArgs, packed arguments
    	*/	
        TIpcArgs CreateArgumentsL( TInt aServiceId ,
								  const TDesC& aText  );
		



public: // MIMCacheClient

		/**
    	* Registeres the cache update observer to server
    	* @param aStatus status message to regisetr
		* @param aRight,right is operations
    	*/
        void RegisterObserverToServerL( TRequestStatus& aStatus,EIMCacheOperations aRight ) ;
        
       
        /**
    	* unRegisteres the cache update  observer to server
    	* @param aStatus status message to regisetr
		* @param aRight,right is operations
    	*/
        void UnRegisterObserverToServerL(  TRequestStatus& aStatus, EIMCacheOperations aRight ) ;
        
        /**
         * cancels any outstanding requests pending from the server
         * @param aStatus status message to regisetr
         * @param aRight,right is operations
         */
        void CancelRequestL( TRequestStatus& aStatus, EIMCacheOperations aRight);
         
        /**
        * Gets called when data update is available.
        * Client can then decide wether to use the old one
        * @param aRight, operations
        * @param abuuferptr 
        */
        TPtr8 GetBufferedDataL(EIMCacheOperations aOperationType );
       
	private:
		
		 // owned , a array for all client who has 
		 // register update data change event 
		 CIMCacheEventHandler* iUpdateObserver;
		 // owned , a array for all client who has 
		 // register update data change event 
		 CIMCacheAccessEventHandler* iAccessObserver;
		 //owned buffer to get chats from server.
		 HBufC8* iChatBuffer;
		
    };

#endif      //  __CIMCACHECLIENT_H__


//  END OF FILE

