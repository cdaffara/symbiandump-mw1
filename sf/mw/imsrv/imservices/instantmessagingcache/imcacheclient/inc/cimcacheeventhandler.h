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


#ifndef __CIMCACHEEVENTHANDLER_H__
#define __CIMCACHEEVENTHANDLER_H__

#include <e32std.h>
#include <e32base.h>
#include <s32mem.h>

#include <imcachedefs.h>

class MIMCacheClient;
class MIMCacheEventHandler;

/**
 * a class to listen to server cache message change
 */
NONSHARABLE_CLASS( CIMCacheEventHandler ): public CActive
    {
	public:
		/**
		* Two-phased constructor.
		* @param aRegistrar ,client refence
		* @param aHandler a handler to send data to ui client
		*/
		static CIMCacheEventHandler* NewL( MIMCacheClient& aRegistrar ,
													 MIMCacheEventHandler& aHandler  );
		/**
		*  ~CIMCacheEventHandler.
		*/
    	virtual ~CIMCacheEventHandler();

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
		* default CIMCacheEventHandler
		* @param aRegistrar ,client refence
		* @param aHandler a handler to send data to ui client
		*/
		CIMCacheEventHandler( MIMCacheClient& aRegistrar, 
							  MIMCacheEventHandler& aHandler );
		/**
		*  default ConstructL
		*/
    	void ConstructL();
    	
    	/**
    	* Gets called when data update is available.
    	* get data buffer from server
    	*/
    	void GetBufferChatDataL() ;
      
      	/**
    	* Gets called when data update is available.
    	* extract the data and send to client
    	* @param aChatDataBuffer structure of SIMCacheMessageData type
    	* @param aMore indicate more packets pending at server side
    	*/
    	void InternalizeChatDataL( TPtr8 aChatDataBuffer , 
    							   TBool& aMore );
		
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
		MIMCacheEventHandler& iUpdateHandler;
	
		
    };

#endif      //  __CIMCACHEEVENTHANDLER_H__


//  END OF FILE

