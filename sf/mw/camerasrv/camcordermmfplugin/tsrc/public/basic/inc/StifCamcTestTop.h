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
* Description: Camcorder MMF API Test DLL
*
*
*/



/*
-------------------------------------------------------------------------------

    DESCRIPTION

    This file contains the header file of DemoModule of 
    STIF TestFramework.

-------------------------------------------------------------------------------
*/

#ifndef DEMOMODULE_H
#define DEMOMODULE_H

// INCLUDES
#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#include <StifTestModule.h>
#include "TestFramework/testsuite.h"

// CONSTANTS
// None

// MACROS
// None

// DATA TYPES
// None

// FUNCTION PROTOTYPES
// None

// FORWARD DECLARATIONS
class CTestModule;
class TCaseInfo;

// CLASS DECLARATION
 

// DESCRIPTION
// Demo testmodule class definition.
NONSHARABLE_CLASS(CTestModule):public CTestModuleBase
    {
    public: // Enumerations
    	// None

    private: // Enumerations
    	// None

    public: // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CTestModule* NewL();
        
        /**
        * Destructor.
        */
        ~CTestModule();

    public: // New functions
        // None
        
    public: // Functions from base classes

        /**
        * Test cases are inquired from the Test Module by calling GetTestCases. 
        * Test cases are appended to RPointerArray<TTestCaseInfo>& aTestCases
        * that is a list consisting of several TTestCaseInfo objects.
        */
        TInt GetTestCasesL( const TFileName& aConfigFile, 
            RPointerArray<TTestCaseInfo>& aTestCases );
        /**
        * RunTestCase is used to run an individual test case.
        */
        TInt RunTestCaseL( const TInt aCaseNumber, 
                          const TFileName& aConfig,
                          TTestResult& aResult );
        
    protected: // New functions
    	// None
    	
    protected: // Functions from base classes
        // None
    	
    private:

        /**
        * C++ default constructor.
        */
        CTestModule();

        /**
        * Symbian OS constructor.
        */
        void ConstructL();        

        /**
        * Function returning test case name and pointer to test case function
        */
        const TCaseInfo Case ( const TInt aCaseNumber ) const;
        
            
	public: //Data
	    // None
    	
	protected: // Data
        // None
    	
    private: // Data

       CTestSuite *iTestSuite ;
       CActiveScheduler* iScheduler;
    	
    public: // Friend classes
        // None
    	
    protected: // Friend classes
        // None
    	
    private: // Friend classes
        // None
    	
    };




#endif // DEMOMODULE_H

// End of File


