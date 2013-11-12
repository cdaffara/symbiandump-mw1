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
* Description:  instantmessagingcache server class declaration
*
*/

#ifndef __CIMCACHESERVERACTIVEHELPER_H__
#define __CIMCACHESERVERACTIVEHELPER_H__

#include <e32base.h>

#include <imcachedefs.h>
#include <cinstantmsgindicator.h>

// FORWARD DECLARATIONS
class CIMCacheServer;
class TIMCacheMessageFactory;
class MIMCacheMessageHeader;
class CIMCacheMessageBase;


/**
* This class is used by CIMCacheServer for dividing long running async.
* tasks into smaller units. The instantmessagingcache session itself was not made 
* active, since that results in c++ ambiguousness problems in CCacheServerActiveHelper
* @since S60 v5.0
*/    
class CCacheServerActiveHelper : public CBase
	{
	public: // public functions
	
	    /**
        * Default 1st phase factory method.
        * Creates an instance of CCacheServerActiveHelper
        * @return created instance of the CCacheServerActiveHelper
        */
        static CCacheServerActiveHelper* NewL(CIMCacheServer& aServer);
        
        /**
        * Destruct
        */
        ~CCacheServerActiveHelper( );
        
        	
	private: // private functions
        
        /**
        * Constructor
        * @param aServer, server reference
        */
        CCacheServerActiveHelper(CIMCacheServer& aServer); 

        /**
        * ConstructL
        */
		void ConstructL();
		
		/**
        * PackAndNotifyRecievedMessageL pack and notify about recieve message
        * @param aMsgHeader, header reference
        * @param aRecievedMsg, message reference
        */
		void PackAndNotifyRecievedMessageL(MIMCacheMessageHeader* aMsgHeader,
											CIMCacheMessageBase* aRecievedMsg);

		/**
        * PackAndNotifyEventL pack and notify about message
        * @param aNotification, type of change
        * @param aMsgHeader, header reference
        * @param aMessage, message reference
        */
		void PackAndNotifyEventL( TIMCacheOperationsCode aNotification, 
                                  TInt aServiceId,
								  MIMCacheMessageHeader * aMsgHeader = NULL,
								  CIMCacheMessageBase* aMessage = NULL );

		/**
        * find and give the chat header pointer
        * @param aServiceId, service id
        * @param aBuddyId, buddy id
        * @return chat header ,if not found NULL
        */
		MIMCacheMessageHeader* ChatHeaderL(const TInt& aServiceId, const TDesC& aBuddyId );
		
		 /**
          * Loads all the plugins
          */
		 void LoadPluginL( );
		 
		 /**
          * publishes the message info to the plugins
          * @param aSenderId, buddy id
          * @param aServiceId, service id
          */
		 void  PublishMessageInfoL(const TDesC& aSenderId,TInt aServiceId );

    										
public:	
     	/**
        * reset all header chat started flag to EFalse
        */
    	void ResetHeaderInoformation();
        /**
        * gets unread message count corresponding to a particular sender
        */
		void GetUnreadMessageCountL(const RMessage2& aMessage );
		
		/**
        * gets all unread message count from all open conversations
        */
		void GetAllUnreadMessageCountL(const RMessage2& aMessage );		

		/**
		* start the new conversation
		*/
        void StartNewConversationL( const RMessage2 &aMessage );

		/**
		* append the revieve new messgae 
		*/
        void AppendReceiveMessageL( const RMessage2 &aMessage );

		/**
		* append the new send messgae  
		*/
        void AppendSendMessageL( const RMessage2 &aMessage );
        
        /**
		* append the information messgae  
		*/
        void AppendMessageL( const RMessage2 &aMessage );
        
	        
        /**
        * close conversations overloaded method.
        */
        void CloseConversationL( const RMessage2 &aMessage );

        /**
         * close All conversations availabe in service.
         */
        void CloseAllConversationL( TInt aServiceId );
        
		/**
		* Deactivate ongoing conversation
		*/
        void DeactivateConversationL();
			
		/**
		* check if conversation already exist write ETrue if exist at 3rd index of message
		*/
		void CheckConversationExistL( const RMessage2 &aMessage );
		
      
		/**
        * find the add contact header at given index
        * @return contact header , NULL if not found
        */
		MIMCacheMessageHeader* ChatHeaderAt( TInt aIndex );

		/**
        * total contact header counts
        * @return the count
        */
		TInt ChatHeadersCount() ;
		
		/**
        * return array of chat headers corresponding to given service
        * @param aServiceId, service id
        * @param aServiceHeaderArray, filled with the headers corresponding to aServiceId,: out parameter.
        * @return array of chat headers
        */
		void GetServiceChatHeaderArrayL(TInt aServiceId , RPointerArray<MIMCacheMessageHeader>& aServiceHeaderArray );
private:
        /**
         * Internal class used as a wrapper around a plugin
         * and its uid.
         */
        class CPluginInfo : public CBase
            {
            public: // Constructor & Destructor
                CPluginInfo( CInstantMsgIndicator* aPlugin, TUid aUid );
                ~CPluginInfo();

            public: // New functions
                CInstantMsgIndicator& Plugin();
            private: // Data
                CInstantMsgIndicator* iPlugin;
                TUid iPluginUid;
            };		
private :		
        /**
        * pointer to CIMCacheServer& aServer doesnt own
        */
		CIMCacheServer& iServer;
		
		// owned, main cache array		
		RPointerArray<MIMCacheMessageHeader> iHeaderArray;
		
		// not owned, active conversation header
		MIMCacheMessageHeader* iActiveHeader;
		
		//owned, pointer to msg factory
		TIMCacheMessageFactory* iMessageFactory;
		
        // takes ownership of the plugins
        RPointerArray<CPluginInfo> iPluginInfo; 
	
	};
	
#endif      //  __CIMCACHESERVERACTIVEHELPER_H__


//  END OF FILE


