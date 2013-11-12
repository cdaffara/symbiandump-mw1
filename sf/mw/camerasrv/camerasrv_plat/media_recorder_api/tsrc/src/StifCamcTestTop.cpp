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
* Description: media_recorder_api
*
*
*/



// INCLUDE FILES
#include <e32math.h>
#include "StifCamcTestTop.h"
#include "TestFrameWork/testSuite.h"

#include "camcTest_1.h"
#include "camcTest_2.h"
#include "camcTest_3.h"
#include "camcTest_4.h"
#include "camcTest_5.h"
#include "camcTest_6.h"
#include "camcTest_7.h"
#include "camcTest_8.h"
#include "camcTest_9.h"
#include "camcTest_visualcheckdsp.h"
#include "camcTest_visualcheck.h"

// EXTERNAL DATA STRUCTURES
// None

// EXTERNAL FUNCTION PROTOTYPES  
// None

// CONSTANTS
// None

// MACROS
// None

// LOCAL CONSTANTS AND MACROS
// None

// MODULE DATA STRUCTURES
// None

// LOCAL FUNCTION PROTOTYPES
// None

// FORWARD DECLARATIONS
// None

// ==================== LOCAL FUNCTIONS =======================================

           
/*
-------------------------------------------------------------------------------

    DESCRIPTION
    
    This file (DemoModule.cpp) contains all STIF test framework related parts of
    this test module.

-------------------------------------------------------------------------------
*/

// ================= MEMBER FUNCTIONS =========================================


CTestModule::CTestModule()
    {
    }






void CTestModule::ConstructL()
    {  
    iTestSuite = CTestSuite::NewL(_L8("Test Suite Container"));

    iTestSuite->addTestL(CCamcTest_1::suiteL());
    iTestSuite->addTestL(CCamcTest_2::suiteL());
    iTestSuite->addTestL(CCamcTest_3::suiteL());
    iTestSuite->addTestL(CCamcTest_4::suiteL());
    iTestSuite->addTestL(CCamcTest_5::suiteL());   
    iTestSuite->addTestL(CCamcTest_6::suiteL());
    iTestSuite->addTestL(CCamcTest_7::suiteL());    
    iTestSuite->addTestL(CCamcTest_8::suiteL());
    iTestSuite->addTestL(CCamcTest_9::suiteL());

    // Visual checks are flagged out because tests are very long !
    
    // Only one of these suite should be selected, depending on 
    // HW used for the test.
    // iTestSuite->addTestL(CCamcTest_visualcheckdsp::suiteL()); 
    // iTestSuite->addTestL(CCamcTest_visualcheck::suiteL()); 

     /* Install an active scheduler */
    iScheduler = new(ELeave)CActiveScheduler;
    CActiveScheduler::Install(iScheduler);

    }





CTestModule* CTestModule::NewL()
    {

    // Construct new CTestModule instance.
    CTestModule* self = new ( ELeave ) CTestModule();    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
  
    }


CTestModule::~CTestModule()
    {
    delete iTestSuite;
    delete iScheduler;
    }


/*
-------------------------------------------------------------------------------

    Class: CTestModule

    Method: GetTestCases

    Description: GetTestCases is used to inquire test cases 
    from the test module. Because this test module have hard coded test cases
    (i.e cases are not read from file), paramter aConfigFile is not used.

    This function loops through all cases defined in Cases() function and 
    adds corresponding items to aTestCases array.

    Parameters: const TFileName&  : in: Configuration file name. Not used                                                       
                RPointerArray<TTestCaseInfo>& aTestCases: out: 
                      Array of TestCases.
    
    Return Values: KErrNone: No error

    Errors/Exceptions: Function leaves if any memory allocation operation fails

    Status: Proposal
    
-------------------------------------------------------------------------------
*/      
TInt CTestModule::GetTestCasesL( const TFileName& /*aConfigFile*/, 
                                     RPointerArray<TTestCaseInfo>& aTestCases )
    {

    // Loop through all test cases and create new
    // TTestCaseInfo items and append items to aTestCase array    
    for( TInt i = 0; i< iTestSuite->CountTestCases(); i++ )
        {
 
	    // Allocate new TTestCaseInfo from heap for a testcase definition.
        TTestCaseInfo* newCase = new( ELeave ) TTestCaseInfo();
    
	    // PushL TTestCaseInfo to CleanupStack.    
        CleanupStack::PushL( newCase );

	    // Set number for the testcase.
	    // When the testcase is run, this comes as a parameter to RunTestCaseL.
        newCase->iCaseNumber = i;

	    // Set title for the test case. This is shown in UI to user.
        newCase->iTitle.Copy( iTestSuite->TestCaseName(i) );

	    // Append TTestCaseInfo to the testcase array. After appended 
	    // successfully the TTestCaseInfo object is owned (and freed) 
	    // by the TestServer. 
        User::LeaveIfError(aTestCases.Append ( newCase ) );
        
	    // Pop TTestCaseInfo from the CleanupStack.
        CleanupStack::Pop( newCase );

        }
    
    return KErrNone;
  
    }

/*
-------------------------------------------------------------------------------

    Class: CTestModule

    Method: RunTestCase

    Description: Run a specified testcase.

    Function runs a test case specified by test case number. Test case file
    parameter is not used.

    If case number is valid, this function runs a test case returned by
    function Cases(). 
  
    Parameters: const TInt aCaseNumber: in: Testcase number 
                const TFileName& : in: Configuration file name. Not used
                TTestResult& aResult: out: Testcase result
    
    Return Values: KErrNone: Testcase ran.
                   KErrNotFound: Unknown testcase

    Errors/Exceptions: None
    
    Status: Proposal
    
-------------------------------------------------------------------------------
*/
TInt CTestModule::RunTestCaseL( const TInt aCaseNumber, 
                                    const TFileName& /*aConfig*/,
                                    TTestResult& aResult )
    {

    // Return value
    TInt execStatus = KErrNone;
    iTestSuite->ExecuteTestL(aResult, aCaseNumber);

    // Return case execution status (not the result of the case execution)
    return execStatus;
      
    }
    

// ================= OTHER EXPORTED FUNCTIONS =================================

/*
-------------------------------------------------------------------------------
   
    Function: LibEntryL

    Description: Polymorphic Dll Entry Point
    
    Test framework calls this function to obtain new instance of test module
    class. 

    Parameters:    None
    
    Return Values: CTestModule*    Pointer to CTestModule instance
    
    Errors/Exceptions: Leaves if CTestModule::NewL leaves
    
    Status: Approved

-------------------------------------------------------------------------------
*/
EXPORT_C CTestModule* LibEntryL()
    {
    return CTestModule::NewL();

    }
    
    
EXPORT_C TInt SetRequirements( CTestModuleParam*& aTestModuleParam, TUint32& aParameterValid )
    {
    aParameterValid = KStifTestModuleParameterChanged;

    CTestModuleParamVer01* param = CTestModuleParamVer01::NewL();

    // Stack size
    param->iTestThreadStackSize= 32768;// 32K stack 

    // Heap sizes
    param->iTestThreadMinHeap = 4096;   // 4K heap min
    param->iTestThreadMaxHeap = 8*1048576;// 4M heap max

    aTestModuleParam = param;

    return KErrNone;
    }

#ifndef EKA2
/*
-------------------------------------------------------------------------------
   
    Function: E32Dll

    Description: DLL entry point function
    
    Parameters:    TDllReason: Not used
    
    Return Values: KErrNone: always
    
    Errors/Exceptions: None

    Status: Approved

-------------------------------------------------------------------------------
*/
GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
    {
    return( KErrNone );

    }
#endif

// End of File
