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
* Description:  cache session class declaration
*
*/

#ifndef __CIMCACHESESSION_H__
#define __CIMCACHESESSION_H__

#include <e32base.h>
#include <s32mem.h>
#include <imcachedefs.h>

// FARWORD DECLARATION
class CIMCacheServer;
class CCacheServerActiveHelper;
class MIMCacheMessageHeader ;
class CIMCacheMessageBase;
class CIMCacheBufferArray;

/**
 * Service side session.
 */
 class CIMCacheSession : public CSession2
    {
    
      
    public:     
    
    	/**
        * NewL.
        * Constructors
        */
        static CIMCacheSession* NewL();
        /**
        * ~CIMCacheSession
        * destructors
        */
        virtual ~CIMCacheSession();


    protected:  // C++ constructors
    	/**
        * C++ default constructor.
        */
        CIMCacheSession();

        /**
        * Symbian OS constructor
        */
		void ConstructL();

    public:     // Methods derived from CSession2
    	/**
    	* From CSession2
    	*/
        void CreateL();
    	/**
    	* From CSession2
    	*/
        void ServiceL( const RMessage2 &aMessage );
    	/**
    	* From CSession2
    	*/
        void ServiceError( const RMessage2& aMessage,
                           TInt aError );
		
   

	public: // New methods
		
        /**
        * Server , 
        * @return server reference
        */
        CIMCacheServer* Server() 
	        { 
	        return (CIMCacheServer*) CSession2::Server(); 
	        }

		/**
         * pack and notify msg notification  
         * @param aChangeType , notification type
         * @param aMsgHeader, chat header 
         * @param aMessage , a message
         * @param aServiceId ,service id
         */	
        void PackAndNotifyEventL( TIMCacheOperationsCode aChangeType ,
								  MIMCacheMessageHeader* aMsgHeader = NULL , 
								  CIMCacheMessageBase* aMessage = NULL );
       
        /*
         * return the service Id of session
         */
        TInt ServiceId() const ;
        
     private:    // New methods

		
        /**
		*  Dispatch clients message
	    *  @since
		*  @param aMessage client's message
        */
        TBool DispatchMessageL( const RMessage2 &aMessage );


		/**
		* Initializes the server
		* @param aMessage client's message
		*/
        void InitSessionL( const RMessage2& aMessage );
        
    
        
        /**
		* reset all conversation, start flag to EFalse
		*/
        void ResetHeaderInoformation();
        
      
		/**
		* Externalizes all chat data
		* @param aArray, array of buffered headers..
		* @param aSize total size of buffered messages.
		* @param aMore , still data is pending to be send
		*/
		void ExternalizeBufferedDataL(TIMCacheOperationsCode aOperationCode,
									  RPointerArray<HBufC8>& aArray , 
									  TInt aTotalSize , 
									  TBool aMore = EFalse );
	
		/**
		* Externalizes chat headers
		* @param aChangeType, event type
		* @param aMsgHeader, chat header
		* @param aMessage , chat message
		* @param aServiceId , serviceid
		*/
		void ExternalizeChatItemL( TIMCacheOperationsCode aChangeType ,
								   TInt aServiceId = KErrNotFound );
		
		/**
		* Externalizes all  chat header messages.
		* @param aChangeType, event type
		* @param aMsgHeader, chat header
		*/						    
		void SendChatListDataL( TIMCacheOperationsCode aChangeType ,
							    MIMCacheMessageHeader* aMsgHeader );
		
		/**
		* send unread change notification
		* @param aMsgHeader, chat header
		* @param aMessage , chat message
		*/					    
		void SendUnreadChangeNotificationL(MIMCacheMessageHeader* aMsgHeader, 
										   CIMCacheMessageBase* aMessage );
		
		/**
		* Externalizes single chat item.
		* @param aMsgHeader, chat header
		*/								   
		void ExternalizeSingleChatItemL( TIMCacheOperationsCode aChangeType, MIMCacheMessageHeader* aMsgHeader );
		
		
		/**
		* Externalizes single chat messages.
		* @param aMsgHeader, chat header
		* @param aMessage , chat message
		*/
		void ExternalizeSingleChatDataL(TIMCacheOperationsCode aChangeType,
										MIMCacheMessageHeader* aChatHeader, 
										CIMCacheMessageBase* aMessage );
		
		/**
		* Externalizes all  chat messages in a message header
		* @param aChangeType, event type
		* @param aMsgHeader, chat header
		*/
		void ExternalizeChatDataL(TIMCacheOperationsCode aChangeType ,MIMCacheMessageHeader* aChatHeader );
		
		/**
		* write data to stream
		* @param aChatHeader, chat header
		* @param aSize total size of buffered messages.
		* @return a HBufC8 stream buffer pointer
		*/
		HBufC8* GetPackedChatItemBufferL(MIMCacheMessageHeader* aChatHeader, TInt& aSize);
		
		/**
		* write data to stream 
		* @param aMsg, message
		* @param aMsgHeader, chat header
		* @param aSize total size of buffered messages.
		* @return a HBufC8 stream buffer pointer
		*/
		HBufC8* GetPackedChatDataBufferL(CIMCacheMessageBase* aMsg , 
										 MIMCacheMessageHeader* aActiveHeader ,
										 const TInt aSize);
		/**
		* do request complete 
		* @param aChangeType, type of request
		*/								 
	//	void DoRequestComplete( TIMCacheOperationsCode aChangeType );								 	
    private:    // Data
    	//owned, message completed state
    	TBool iMessageCompleted;
             	
        //does observer is active
        TBool iObserverActive;
        
        //msg observer
        RMessage2 iObserverMessage;
                        
		// not owned, active helper
		CCacheServerActiveHelper* iHelper;
	
		// owns, pending packets to be sent to ui client
		RPointerArray<CIMCacheBufferArray> iPacketsArray;
	
    	// read only right initialized
		TBool iAccessorInitialized;
		// conversation r/w initialized
		TBool iConvesationInitialized;
		
		// owns : add request fetch completed or not
		TBool iOpenChatFetchCompleted;		   
		
		// service id for this session
		TInt iServiceId;
	
		};

#endif      //  __CIMCACHESESSION_H__


//  END OF FILE

