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


#ifndef __UT_CCFENGINE_H__
#define __UT_CCFENGINE_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>

//  INTERNAL INCLUDES
#include "CFContextSubscriptionListener.h"
#include "CFSecurityChecker.h"

//  FORWARD DECLARATIONS
class CCFEngine;
class MCFContextInterface;
class CCFTestDelay;
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
NONSHARABLE_CLASS( UT_CCFEngine ) : public CEUnitTestSuiteClass,
    public MCFContextSubscriptionListener,
    public MCFSecurityChecker
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CCFEngine* NewL();
        static UT_CCFEngine* NewLC();
        /**
         * Destructor
         */
        ~UT_CCFEngine();

    private:    // Constructors and destructors

        UT_CCFEngine();
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

		// from MCFSecurityChecker
		TBool CheckClientSecurity( const RThread& aClientThread,
			const TSecurityPolicy& aSecurityPolicy );

    private:    // New methods

         void SetupL();
        
         void Teardown();

		 void StopWait();

         void SetupForNewL();
        
         void TeardownForNewL();
        
         void UT_CCFEngine_NewLL();
        
         void UT_CCFEngine_NewLCL();


		 void UT_PublishContextL();
		 
		 void UT_SubscribeContextL();
		 
		 void UT_DefineContextL();
		 
		 void UT_RemoveContextSubscriptionL();

         // Simulates the possible situation of Context Source or script
         // operation (deactivating) removing subscription when processing
         // context indication.
         void UT_RemoveSubscriptionDuringIdicationL();
         void UT_RemoveSubscriptionDuringIdication2L();


    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 

		CActiveSchedulerWait* iWait;
		CCFTestDelay* iTestDelay;
		
		CCFEngine* iCFEngine;
		MCFContextInterface* iCFContextInterface;

		TBool iSubscriptionError;
		TBool iIndication;
		
		TInt iDoSecurityCheck;

        CCFContextSubscription* iRemoveSubscription;
        TBool iRemoveSubscriptions;
    };

#endif      //  __UT_CCFENGINE_H__

// End of file
