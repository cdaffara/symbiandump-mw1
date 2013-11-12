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
* Description:  cache message change observer
*
*/


#ifndef __CIMCACHEACCESSEVENTHANDLER_H__
#define __CIMCACHEACCESSEVENTHANDLER_H__

#include <e32std.h>
#include <e32base.h>
#include <s32mem.h>

#include <imcachedefs.h>

class MIMCacheClient;
class MIMCacheEventHandler;

/**
 * a class to listen to server cache message change
 */
NONSHARABLE_CLASS( CIMCacheAccessEventHandler ): public CActive
    {
	public:
		/**
		* Two-phased constructor.
		* @param aRegistrar ,client refence
		* @param aHandler a handler to send data to ui client
		*/
		static CIMCacheAccessEventHandler* NewL( MIMCacheClient& aRegistrar ,
										   MIMCacheEventHandler& aHandler );
		/**
		*  ~CIMCacheAccessEventHandler.
		*/
    	virtual ~CIMCacheAccessEventHandler();

    public: // From CActive

		/**
		*  RunL
		*/
        void RunL();  
        
        /**
		*  DoCancel
		*/
		void DoCancel();
		
		 /**
          *  RunError
          */
        TInt RunError( TInt /* aError */);
		        
		
		
		
			
	public : // new functions
			/**
			*  UnRegisterObserver from server
			*/
			void UnRegisterObserver();
	private:
	
		/**
		* default CIMCacheAccessEventHandler
		* @param aRegistrar ,client refence
		* @param aHandler a handler to send data to ui client
		*/
		CIMCacheAccessEventHandler( MIMCacheClient& aRegistrar, 
							  MIMCacheEventHandler& aHandler);
		/**
		*  default ConstructL
		*/
    	void ConstructL();
    	
   		
        /**
         * Gets called when data update is available.
         * Client can then decide wether to use the old one
         * @param aBuffer structure of SIMCacheMessageData type
         */
    	void GetBufferChatDataL();
    	

        /**
         * Gets called when data update is available.
         * extract the chat data and send to client
         * @param aChatDataBuffer ,buffer containing number of chat data
         * @param aMore, indicate that there is still pending packets
         */
    	void InternalizeChatDataL( TPtr8 aChatDataBuffer , TBool& aMore );
    	

        /**
        * Gets called when data update is available.
        * get data buffer from server
        * @param aEventType, type of data
        */  
        void GetBufferedChatItemL( TIMCacheEventType aEventType );
        
        /**
        * read the stream and write into buffer
        * @param aEventType, type of data
        * @param aChatItemBuffer structure of SIMCacheContactData type
        */
            
        void InternalizeChatItemL( TIMCacheEventType aEventType, 
                                   TPtr8 aChatItemBuffer );
   
	private: // data
	
		// doesn't own: registrar
		MIMCacheClient& iRegistrar;
		// owned , need to observer to server or not
		TBool iContinueObserving ;
		// client side observer event 
		MIMCacheEventHandler& iAccessHandler;
		
    };

#endif      //  __CIMCACHEACCESSEVENTHANDLER_H__


//  END OF FILE

