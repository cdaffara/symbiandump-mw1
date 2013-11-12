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


#ifndef __CIMCACHESERVER_H__
#define __CIMCACHESERVER_H__

#include <e32base.h>
#include <imcachedefs.h>

// FORWARD DECLARATIONS
class CIMCacheSession;
class CCacheServerActiveHelper;
class MIMCacheMessageHeader ;
class CIMCacheMessageBase;
/**
 * Service server.
 */
NONSHARABLE_CLASS( CIMCacheServer ): public CPolicyServer
    {
        	
    public:     // Execution and destructors
         static void ExecuteL();
        
        virtual ~CIMCacheServer();

    private:    // C++ constructor
        CIMCacheServer();

    protected:     // Methods from CPolicyServer

        CSession2* NewSessionL( const TVersion& aVersion,
                                const RMessage2& aMessage ) const;

  
    public:     //New methods
    
    	/**
         * SessionCreatedL  
         * @param aSession a session created 
         */
	    void SessionCreatedL( CIMCacheSession* aSession );
	   
	    /**
         * SessionDied  
         * @param aSession a session died 
         */
	    void SessionDied( CIMCacheSession* aSession );
	    
	    			
		/**
         * helper is owned by server class   
         * may any session requied to access
         * to get the same instance
         * @return pointer to the active helper
         */								 
		CCacheServerActiveHelper* GetActiveHelper();
					
		
		/**
         * pack and notify msg notification  
         * @param aChangeType , notification type
         * @param aMsgHeader, chat header 
         * @param aMessage , a message
         */	
		void PackAndNotifyEventL( TIMCacheOperationsCode aChangeType,
                                  TInt aServiceId,
								  MIMCacheMessageHeader* aMsgHeader = NULL, 
								  CIMCacheMessageBase* aMessage = NULL ) ; 
	    
          

	private: // New methods
	
	
		/**
         * initialization will be done here
         */
	    void InitializeL(CIMCacheServer& aServer);
	    
	   
	    
    private:    //Data
    	// owned, session count
        TInt iSessionCount;
        // owned, array of all open sessions
        RPointerArray<CIMCacheSession> iSessions;
    	// owned, active helper	
      	CCacheServerActiveHelper* iActiveHelper;
    };


#endif      //  __CIMCACHESERVER_H__


//  END OF FILE


