/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef __MT_CCFActionPlugIn_H__
#define __MT_CCFActionPlugIn_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>


//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CCFActionPlugIn;

//  CLASS DEFINITION
/**
 * Auto-generated test suite
 *
 */
NONSHARABLE_CLASS( MT_CCFActionPlugIn )
	: public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static MT_CCFActionPlugIn* NewL();
        static MT_CCFActionPlugIn* NewLC();
        /**
         * Destructor
         */
        ~MT_CCFActionPlugIn();

    private:    // Constructors and destructors

        MT_CCFActionPlugIn();
        void ConstructL();

    private:    // New methods

        void SetupL();
        void SetupEmptyL();
        void Teardown();
        void TeardownEmpty();

        void MT_CCFActionPlugIn_NewL();
        void MT_CCFActionPlugIn_NewLC();
        void MT_CCFActionPlugIn_AsyncExecutionCompletedL();
        void MT_CCFActionPlugIn_PrepareExecutionLL();
        void MT_CCFActionPlugIn_FinishedExecutionL();
        void MT_CCFActionPlugIn_ExtensionL();
        void MT_CCFActionPlugIn_InitializeLL();
        void MT_CCFActionPlugIn_ExecuteLL();
        void MT_CCFActionPlugIn_GetActionsLL();
        void MT_CCFActionPlugIn_SecurityPolicyL();

    private:    // Data
		
        CCFActionPlugIn* iCCFActionPlugIn;
        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __MT_CCFActionPlugIn_H__

// End of file
