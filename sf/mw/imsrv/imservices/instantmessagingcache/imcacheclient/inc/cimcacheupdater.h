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
* Description:  class declaration for updating the cache
*
*/



#ifndef __CIMCACHEUPDATER_H__
#define __CIMCACHEUPDATER_H__

#include <e32base.h>
#include "mimcacheupdater.h"
#include "cimcacheclient.h"

class MIMCacheEventHandler;

/**
 * write right class declaration
 */
 
NONSHARABLE_CLASS( CIMCacheUpdater ): public CBase,
								 public MIMCacheUpdater
    {
	public:
		/**
		* Two-phased constructor.
		* @param aServiceId, service id to used
		* @param aUserId, user id who is conversing
		*/
		static CIMCacheUpdater* NewL(TInt aServiceId,
									const TDesC& aUserId,
									TBool aRegistrationNeeded );
		/**
		* virtual destructor.
		*/
    	virtual ~CIMCacheUpdater();

		
	private:
	
		/**
		* ConstructL.
		* @param aUserId, user id who is conversing
		*/
		
    	void ConstructL( const TDesC& aUserId, TBool aRegistrationNeeded );
    	
    	/**
		* default constructor.
		* @param aServiceId, service id to used
		*/
		CIMCacheUpdater(TInt aServiceId);
		
		/**
		* default constructor.
		* @param aServiceId, service id to used
		*/
		void InternalizeChatListL( RArray<SIMCacheChatItem>& aChatArray, TPtr8 aChatListBuffer, TBool& aMore );

	private: // from MIMCacheUpdater

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
        * start a new conversation 
        * @param aBuddyId a recipient user id
        */
		void StartNewConversationL(const TDesC& aBuddyId ) ;
		
		/**
        * append the new recieve message
        * @param aSender a sender user id
        * @param aRecipient a recipient user id
        * @param aText a text to append
        */
		void AppendReceiveMessageL( const TDesC& aBuddyId, 
								   const TDesC& aText ) ;
		
		/**
        * append the new send message
        * @param aText a text to append
        */
		void AppendSendMessageL(const TDesC& aText ) ;
		
		/**
        * append the normal information message
        * @param aBuddyId buddy id.
        * @param aText a text to append
        */
		void AppendMessageL(const TDesC& aBuddyId, const TDesC& aText) ;
		
		
		/**
		* closes conversation and commit operation
		* @param aServiceId service id.
		* @param aBuddyId buddy id.
		* @param aRecipientId recipient's user id.
		*/
		void CloseConversationL( const TDesC& aBuddyId ) ;
		
		/*
		 * close all conversation in service Id
		 */
		void CloseAllConversationL();
		
		/**
        * Deactivate all open conversations and commit operation
        */
		TInt DeactivateConversationL() ;
		
      	
		/**
        * get the chat list corresponding to service
        * @param aServiceId service id
        * @return Array of chats
        */
		RArray<SIMCacheChatItem> GetChatListL( const TInt aServiceId  ); 
		
		
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

