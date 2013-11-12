/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef __MT_CRFSHANDLER_H__
#define __MT_CRFSHANDLER_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>
#include <rfsHandler.h>
#include <W32STD.H>	// TKeyEvent
#include <eikclbd.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS


//  CLASS DEFINITION
/**
 * Auto-generated EUnit test suite
 *
 */
NONSHARABLE_CLASS( MT_CRfsHandler )
	: public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static MT_CRfsHandler* NewL();
        static MT_CRfsHandler* NewLC();
        /**
         * Destructor
         */
        ~MT_CRfsHandler();

    private:    // Constructors and destructors

        MT_CRfsHandler();
        void ConstructL();

    private:    // New methods
         void SetupEmptyL();        
         void TeardownEmpty();        
         void SetupL();        
         void Teardown(); 
         void SpecialSetupL();
         void SpecialTeardown();         
                
         
         void TestCRfsHandlerL();        
         void TestActivateRfsLL();        
         void TestCancelL();
         
         static TInt SimulateRightSoftkey( TAny* aPtr );
         void DoSimulateRightL();
         
        

    private:    // Data
		
        CRfsHandler* iCRfsHandler;
        CPeriodic* iPeriodic;
        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __MT_CRFSHANDLER_H__

// End of file
