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
* Description:  base class declaration for updating the cache
*
*/



#ifndef __MIMCACHEUPDATER_H
#define __MIMCACHEUPDATER_H
 
#include "imcachedefs.h"

class MIMCacheEventHandler;
 /**
  * An interface for updating conversation information
  *
  * @since 5.0
  */
 class MIMCacheUpdater
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
        * start a new conversation 
        * @param aRecipient a recipient user id
        */
		virtual void StartNewConversationL(const TDesC& aBuddyId ) = 0;
		
		/**
        * append the new recieve message
        * @param aBuddyId a sender user id
        * @param aText a text to append
        */
		virtual void AppendReceiveMessageL(const TDesC& aBuddyId,
										   const TDesC& aText ) = 0;
		
		/**
        * append the new send message
        * @param aText a text to append
        */
		virtual void AppendSendMessageL(const TDesC& aText ) = 0;
		
		/**
        * append the normal information message message
        * @param aBuddyId buddy id.
        * @param aText a text to append
        */
		virtual void AppendMessageL(const TDesC& aBuddyId, const TDesC& aText)  = 0 ;
		
		
		/**
		* closes conversation and commit operation
		* @param aServiceId service id.
		* @param aRecipientId recipient's user id.
		*/
		virtual void CloseConversationL( const TDesC& aBuddyId ) = 0;
		
		
		/*
		 * close all conversation in service Id
		 */
		virtual void CloseAllConversationL() = 0 ;
		
	
		/**
        * clos conversation and commit operation
        */
		virtual TInt DeactivateConversationL() = 0;
		
		                               
			
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
     	virtual ~MIMCacheUpdater() {};
     };

 #endif //__MIMCACHEUPDATER_H
