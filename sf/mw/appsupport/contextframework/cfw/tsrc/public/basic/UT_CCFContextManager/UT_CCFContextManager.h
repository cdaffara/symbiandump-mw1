/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:
*
*/


#ifndef __UT_CCFCONTEXTMANAGER_H__
#define __UT_CCFCONTEXTMANAGER_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>

//  INTERNAL INCLUDES
#include "CFContextSubscriptionListener.h"
#include "CFSecurityChecker.h"
#include "cfcontextobject.h"
#include "cfcontextsourceinterface.h"

//  FORWARD DECLARATIONS
class CCFContextManager;
class CCFContextSubscription;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( UT_CCFContextManager )
     : public CEUnitTestSuiteClass, public MCFContextSubscriptionListener,
     public MCFSecurityChecker, public MCFContextSource
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CCFContextManager* NewL();
        static UT_CCFContextManager* NewLC();
        /**
         * Destructor
         */
        ~UT_CCFContextManager();

    private:    // Constructors and destructors

        UT_CCFContextManager();
        void ConstructL();

    public:     // From observer interface

        // from MCFContextSubscriptionListener
        void ContextIndicationL( CCFContextIndication* aIndication );

        // from MCFContextSubscriptionListener
        TInt Client( RThread& aThread ) const;
        
        // from MCFContextSubscriptionListener
        void HandleContextSubscriptionError( TInt aError,
            const TDesC& aSource,
            const TDesC& aType );        

    public:
    	// from MCFContextSource
    	void Subscribers( const TDesC& aContextSource,
            const TDesC& aContextType );

    	// from MCFContextSource
   		void NoSubscribers( const TDesC& aContextSource,
            const TDesC& aContextType );
        
        // from MCFContextSource
        void HandleCommand( const CCFContextSourceCommand& aCommand );
        
	public:
	
		// from MCFSecurityChecker
        TBool CheckClientSecurity( const RThread& aClientThread,
            const TSecurityPolicy& aSecurityPolicy );
        

    private:    // New methods

         void SetupL();
        
         void Teardown();

		 void StopWait();

         void SetupForNewL();
        
         void TeardownForNewL();

		 void SetupContextCacheL();
		 
		 TInt FindContext(RContextObjectArray& aArray, 
			const TDesC& aSource, const TDesC& aType, const TDesC& aValue);
	
        
         void UT_CCFContextManager_NewLL();
        
        
         void UT_CCFContextManager_NewLCL();
        
        
         void UT_CCFContextManager_PublishContextLL();
        
        
         void UT_CCFContextManager_RequestContextL_2L();
        
        
         void UT_CCFContextManager_DefineContextLL();
         
         void UT_CCFContextManager_DefineContextL2L();
        
        
         void UT_CCFContextManager_RequestContextL_3L();
        
        
         void UT_CCFContextManager_SubscribeContextLL();
        
        
         void UT_CCFContextManager_RemoveSubscriptionL();
        
         void UT_CCFContextManager_RemoveSubscription2L();
        
         void UT_CCFContextManager_RemoveSubscriptionsL();

         void UT_CCFContextManager_DeregisterPublisherL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 

		CActiveSchedulerWait* iWait;
		
		CCFContextManager* iCFContextManager;
		CCFContextManager* iCFContextManagerInterface;

		TBool iSubscriptionError;
		
		TBool iIndication;
		CCFContextIndication* iContextIndication;
		
		TInt iBypassSecurityCheck;

		TBool iSubscribersCalled;
		TBool iNoSubscribersCalled;
		TBuf<100> iExpectedSubscribersSource;
		TBuf<100> iExpectedSubscribersType;
    };

#endif      //  __UT_CCFCONTEXTMANAGER_H__

// End of file
