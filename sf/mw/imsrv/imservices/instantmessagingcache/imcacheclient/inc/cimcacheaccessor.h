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
* Description:  class declaration for accessing the cache
*
*/



#ifndef __CIMCACHEACCESSOR_H__
#define __CIMCACHEACCESSOR_H__

#include <e32base.h>
#include "mimcacheaccessor.h"
#include "cimcacheclient.h"

class MIMCacheEventHandler;

/**
 * write right class declaration
 */
 
NONSHARABLE_CLASS( CIMCacheAccessor ): public CBase,
								 public MIMCacheAccessor
    {
	public:
		/**
		* Two-phased constructor.
		* @param aServiceId, service id to used
		* @param aUserId, user id who is conversing
		*/
		static CIMCacheAccessor* NewL(TInt aServiceId,
										const TDesC& aUserId );
		/**
		* virtual destructor.
		*/
    	virtual ~CIMCacheAccessor();

		
	private:
	
		/**
		* ConstructL.
		* @param aUserId, user id who is conversing
		*/
		
    	void ConstructL( const TDesC& aUserId );
    	
    	/**
		* default constructor.
		* @param aServiceId, service id to used
		*/
		CIMCacheAccessor(TInt aServiceId);

	
	private: // from MIMCacheAccessor

		/**
    	* Register to observer instantmessagingcache data updates
    	* @param aObserver the observer interface
    	*/
        void RegisterObserverL( MIMCacheEventHandler& aObserver );

    	/**
    	* Unregister a instantmessagingcache data update observer
    	* @param aObserver the observer interface
    	*/
        void UnRegisterObserver( MIMCacheEventHandler& aObserver);
        	
       
       	/**
    	* to get the unread message count from server
    	* @param aOwnUserId , logged in user id
    	* @return number of unread msg count
    	*/
       TInt GetUnreadMessageCountL(const TDesC& aBuddyId );
		
		 /**
    	* to get the all unread message count from server
    	* @param , aOwnUserId logged in user id
    	* @return number of all unread msg count
    	*/
        TInt GetAllUnreadMessageCountL();		
			 						
		/**
         * check if already conversation opened
         * @param aServiceId, serviceid to use
         * @param aOwnUserId, own user id 
         * @param aRecipientId, the selected user id with whom conversation is started
         * @return ETrue if already exist else EFlase.
         */
        TBool IsConversationExistL( const TDesC& aBuddyId )  ;
        
        /**
        * closes conversation and commit operation
        * @param aServiceId service id.
        * @param aOwnId own user id.
        * @param aRecipientId recipient's user id.
        */
        void CloseConversationL( const TDesC& aBuddyId );
        
        
        /**
        * get the chat list corresponding to service
        * @param aServiceId service id
        * @return Array of chats
        */
        RArray<SIMCacheChatItem> GetChatListL( const TInt aServiceId  ); 
   
	private :
		/**
         * default constructor.
         * @param aServiceId, service id to used
         */
        void InternalizeChatListL( RArray<SIMCacheChatItem>& aChatArray, TPtr8 aChatListBuffer, TBool& aMore );
        	 
    public : // new api
    	
       /**
		* return the serviceid
		*/
    	TInt ServiceId() const ;	        
		
	private: // data

		// owned , user id
		HBufC* iUserId;
		
		// not owned , client
		CIMCacheClient iClient;
		
		// owned, service id
		TInt iServiceId;
	
    };

#endif      //  __CIMCACHEUPDATER_H__


//  END OF FILE

