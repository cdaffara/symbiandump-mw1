/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef __UT_CMCEEVENTREFERTERMINATEDSTATE_H__
#define __UT_CMCEEVENTREFERTERMINATEDSTATE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "MMCETestEventObserver.h"

//  FORWARD DECLARATIONS
class CMceEventReferTerminatedState;
class CMceComEvent;
class CMceSipEvent;
class CSIPMessageElements;
class CMceSipConnection;
class CSIPProfile;


//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceEventReferTerminatedState
     : public CEUnitTestSuiteClass,
       public MMCETestEventObserver

    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceEventReferTerminatedState* NewL();
        static UT_CMceEventReferTerminatedState* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceEventReferTerminatedState();

    private:    // Constructors and destructors

        UT_CMceEventReferTerminatedState();
        void ConstructL();
 
	public: 	// From observer interface
		
		//From  test event observer
	    void IncomingSubscribeL( CDesC8Array* aHeaders,
                                 HBufC8* aContentType,
					             HBufC8* aBody );
		
        void IncomingReferL( HBufC8* aReferTo,
					         CDesC8Array* aHeaders,
                             HBufC8* aContentType,
					         HBufC8* aBody );
		
		void EventStateChangedL( TUint32 aState );
		
		void NotifyReceivedL( CDesC8Array* aHeaders,
                    		  HBufC8* aContentType,
							  HBufC8* aBody );
							

	private:    // New methods

         private:    // New methods

         void SetupL();
         
         void SetupFullL();
        
         void SetupEmptyL();
         
         void Teardown();
        
         void UT_CMceEventReferTerminatedState_HandleLL();
        
         void UT_CMceEventReferTerminatedState_HandleReceiveRequestLL();
         
         void UT_CMceEventReferTerminatedState_HandleReceiveRequestL2L();
         
         void UT_CMceEventReferTerminatedState_HandleReceiveRequestL3L();
         
         void UT_CMceEventReferTerminatedState_HandleReceiveRequestL4L();
           
         void UT_CMceEventReferTerminatedState_HandleResponseReceivedLL();


    private:    // Data

        CMceEventReferTerminatedState* iState;		
    	CMceSipEvent* iSIPEvent;
        CMceComEvent* iEvent;
        CSIPProfile* iSipProfile;
		CMceSipConnection* iConnection;
		TUint iReceived;
        TUint iEventState;
        CSIPMessageElements* iMsgElem; // not owned
        
		EUNIT_DECLARE_TEST_TABLE; 
		friend class CMceSipEvent;

    };

#endif      //  __UT_CMCEEVENTREFERTERMINATEDSTATE_H__

// End of file
