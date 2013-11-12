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
* Description:  base class declaration for accessing the cache
*
*/



#ifndef __MIMCACHEACCESSOR_H
#define __MIMCACHEACCESSOR_H
 
#include "imcachedefs.h"

class MIMCacheEventHandler;
 /**
  * An interface for updating conversation information
  *
  * @since 5.0
  */
 class MIMCacheAccessor
     {
     public:
     
 		/**
    	* Register to observer conversation data updates
    	* @param aObserver the observer interface
    	*/
        virtual void RegisterObserverL( MIMCacheEventHandler& aObserver ) = 0;

    	/**
    	* Unregister a conversation data update observer
    	* @param aObserver the observer interface
    	*/
        virtual void UnRegisterObserver( MIMCacheEventHandler& aObserver ) = 0;
		
       	/**
    	* to get the unread message count from server
    	* @param aOwnUserId , logged in user id
    	* @return number of unread msg count
    	*/
		virtual TInt GetUnreadMessageCountL( const TDesC& aBuddyId ) = 0;
		
	   /**
    	* to get the all unread message count from server
    	* @param , aOwnUserId logged in user id
    	* @return number of all unread msg count
    	*/
        virtual TInt GetAllUnreadMessageCountL() = 0 ;
        						
		 /**
         * check if already conversation opened
         * @param aServiceId, serviceid to use
         * @param aOwnUserId, own user id 
         * @param aRecipientId, the selected user id with whom conversation is started
         * @return ETrue if already exist else EFlase.
         */
        virtual TBool IsConversationExistL( const TDesC& aBuddyId ) = 0 ;	

		/**
		* closes conversation and commit operation
		* @param aServiceId service id.
		* @param aOwnId own user id.
		* @param aRecipientId recipient's user id.
		*/
		virtual void CloseConversationL( const TDesC& aBuddyId ) = 0;
		
		  
        /**
        * get the chat list corresponding to service
        * @param aServiceId service id
        * @return Array of chats
        */
		virtual RArray<SIMCacheChatItem> GetChatListL( const TInt aServiceId  ) = 0 ; 
		                           
       
		 
	protected:
     	/**
     	* Destructor
     	*/
     	virtual ~MIMCacheAccessor() {};
     };

 #endif //__MIMCACHEUPDATER_H
