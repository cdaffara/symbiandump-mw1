/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* CRfsCustCmdTest class implementation.
*
*/

#include <stifparser.h>
#include <stiftestinterface.h>

#include "rfscustcmdtest.h"
#include "rfscustcmdtesthelper.h"
#include "trace.h"

// ---------------------------------------------------------
// CRfsCustCmdTest::LibEntryL
// Returns: Poiter to CRfsCustCmdTest class
// ---------------------------------------------------------
EXPORT_C CRfsCustCmdTest* LibEntryL( CTestModuleIf& aTestModuleIf )
    {
    FUNC_LOG;
    
    CRfsCustCmdTest* libEntry( CRfsCustCmdTest::NewL( aTestModuleIf ) );
    return libEntry;
    }

// ---------------------------------------------------------
// CRfsCustCmdTest::CRfsCustCmdTest
// ---------------------------------------------------------
CRfsCustCmdTest::CRfsCustCmdTest( CTestModuleIf& /*aTestModuleIf*/ )
    {
    FUNC_LOG;    
    }

// ---------------------------------------------------------
// CRfsCustCmdTest::ConstructL
// ---------------------------------------------------------
void CRfsCustCmdTest::ConstructL()
    {
    FUNC_LOG;    

    iScheduler = new( ELeave ) CActiveScheduler();
    CActiveScheduler::Install( iScheduler );
    }

// ---------------------------------------------------------
// CRfsCustCmdTest::NewL
// ---------------------------------------------------------
CRfsCustCmdTest* CRfsCustCmdTest::NewL( CTestModuleIf& aTestModuleIf )
    {
    FUNC_LOG;
    
    CRfsCustCmdTest* self = new (ELeave) CRfsCustCmdTest( aTestModuleIf );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------
// CRfsCustCmdTest::~CRfsCustCmdTest
// ---------------------------------------------------------
CRfsCustCmdTest::~CRfsCustCmdTest()
    {
    FUNC_LOG;
    
    delete iScheduler;
    }

// -----------------------------------------------------------------------------
// CRfsCustCmdTest::Case
// Returns a test case by number.
//
// This function contains an array of all available test cases 
// i.e pair of case name and test function. If case specified by parameter
// aCaseNumber is found from array, then that item is returned.
// 
// The reason for this rather complicated function is to specify all the
// test cases only in one place. It is not necessary to understand how
// function pointers to class member functions works when adding new test
// cases. See function body for instructions how to add new test case.
// -----------------------------------------------------------------------------
//
const TCaseInfo CRfsCustCmdTest::Case( const TInt aCaseNumber ) const
    {
    FUNC_LOG;
    
    /**
    * To add new test cases, implement new test case function and add new 
    * line to KCases array specify the name of the case and the function 
    * doing the test case
    * In practice, do following
    * 1) Make copy of existing test case function and change its name
    *    and functionality. Note that the function must be added to 
    *    rfscustcmdtest.cpp file and to rfscustcmdtest.h
    *    header file.
    * 2) Add entry to following KCases array either by using FUNCENTRY or 
    *    ENTRY macro. 
    * ENTRY macro takes two parameters: test case name and test case 
    * function name.
    * FUNCENTRY macro takes only test case function name as a parameter and uses
    * that as a test case name and test case function name.
    */
    
    static TCaseInfoInternal const KCases[] =
        {
		//{ (TText*) L"Construct test", GETPTR CRfsCustCmdTest::TestConstruct },
		{ (TText*) L"First boot rfs init test", GETPTR CRfsCustCmdTest::TestFirstBootRfs },
		{ (TText*) L"Deep rfs init test", GETPTR CRfsCustCmdTest::TestDeepRfs },
		{ (TText*) L"Pre-UI rfs init test", GETPTR CRfsCustCmdTest::TestPreUiRfs },
		{ (TText*) L"Post-UI rfs init test", GETPTR CRfsCustCmdTest::TestPostUiRfs },
        { (TText*) L"First boot rfs execute test", GETPTR CRfsCustCmdTest::TestFirstBootRfsExecute },
        { (TText*) L"Deep rfs execute test", GETPTR CRfsCustCmdTest::TestDeepRfsExecute },
        { (TText*) L"Pre-UI rfs execute test", GETPTR CRfsCustCmdTest::TestPreUiRfsExecute },
        { (TText*) L"Post-UI rfs execute test", GETPTR CRfsCustCmdTest::TestPostUiRfsExecute },
        { (TText*) L"First boot rfs execute cancel test", GETPTR CRfsCustCmdTest::TestFirstBootRfsExecuteCancel },
        { (TText*) L"Deep rfs execute cancel test", GETPTR CRfsCustCmdTest::TestDeepRfsExecuteCancel },
        { (TText*) L"Pre-UI rfs execute cancel test", GETPTR CRfsCustCmdTest::TestPreUiRfsExecuteCancel },
        { (TText*) L"Post-UI rfs execute cancel test", GETPTR CRfsCustCmdTest::TestPostUiRfsExecuteCancel }
        };

    // Verify that case number is valid
    if( (TUint) aCaseNumber >= sizeof( KCases ) / sizeof( TCaseInfoInternal ) )
        {
        // Invalid case, construct empty object
        TCaseInfo null( (const TText*) L"" );
        null.iMethod = NULL;
        return null;
        } 

    // Construct TCaseInfo object and return it
    TCaseInfo tmp ( KCases[aCaseNumber].iCaseName );
    tmp.iMethod = KCases[aCaseNumber].iMethod;
    
    return tmp;
    }

// -----------------------------------------------------------------------------
// CRfsCustCmdTest::GetTestCasesL
// GetTestCases is used to inquire test cases from the Test Module. Test
// cases are stored to array of test cases. The Test Framework will be 
// the owner of the data in the RPointerArray after GetTestCases return
// and it does the memory deallocation. 
// -----------------------------------------------------------------------------
//
TInt CRfsCustCmdTest::GetTestCasesL( const TFileName& /*aConfig*/, 
    RPointerArray<TTestCaseInfo>& aTestCases )
    {
    FUNC_LOG;
    
    // Loop through all test cases and create new
    // TTestCaseInfo items and append items to aTestCase array    
    for( TInt i = 0; Case(i).iMethod != NULL; i++ )
        {
 
	    // Allocate new TTestCaseInfo from heap for a testcase definition.
        TTestCaseInfo* newCase = new( ELeave ) TTestCaseInfo;
    
	    // PushL TTestCaseInfo to CleanupStack.    
        CleanupStack::PushL( newCase );

	    // Set number for the testcase.
	    // When the testcase is run, this comes as a parameter to RunTestCaseL.
        newCase->iCaseNumber = i;

	    // Set title for the test case. This is shown in UI to user.
        newCase->iTitle.Copy( Case(i).iCaseName );

	    // Append TTestCaseInfo to the testcase array. After appended 
	    // successfully the TTestCaseInfo object is owned (and freed) 
	    // by the TestServer. 
        User::LeaveIfError(aTestCases.Append ( newCase ) );
        
	    // Pop TTestCaseInfo from the CleanupStack.
        CleanupStack::Pop( newCase );
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CRfsCustCmdTest::RunTestCaseL
// RunTestCaseL is used to run an individual test case specified 
// by aTestCase. Test cases that can be run may be requested from 
// Test Module by GetTestCases method before calling RunTestCase.
// -----------------------------------------------------------------------------
//
TInt CRfsCustCmdTest::RunTestCaseL( const TInt aCaseNumber,   
    const TFileName& /*aConfig*/,
    TTestResult& aResult )
    {    
    FUNC_LOG;
    
    // Return value
    TInt execStatus = KErrNone;

    // Get the pointer to test case function
    TCaseInfo tmp = Case ( aCaseNumber );
    
    // Copy exclude list to prevent C drive erasure.
    _LIT( KExcludeList, "Z:\\private\\100059C9\\excludelist.txt" );
    _LIT( KTargetExcludeListPath, "C:\\private\\100059C9\\" );
    _LIT( KTargetExcludeList, "C:\\private\\100059C9\\excludelist.txt" );
    RFs fs;
    TInt err = fs.Connect();
    INFO_1( "fs.Connect err %d", err );
    if( err == KErrNone )
        {
        err = fs.MkDirAll( KTargetExcludeListPath );
        INFO_1( "fs.Connect err %d", err );
        CFileMan* fileMan = CFileMan::NewL( fs );
        err = fileMan->Copy( KExcludeList, KTargetExcludeList, CFileMan::EOverWrite );
        ERROR( err, "Copying file failed" );
        delete fileMan;
        fs.Close();
        }
    
    // Check that case number was valid
    if ( tmp.iMethod != NULL )
        {
        // Valid case was found, call it via function pointer
        iMethod = tmp.iMethod;        
        execStatus  = ( this->*iMethod )( aResult );                  
        }
    else
        {        
        // Valid case was not found, return error.
        execStatus = KErrNotFound;
        }

    // Return case execution status (not the result of the case execution)
    return execStatus;
    }

//------------------------------------------------------------------------------
// TEST CASE IMPLEMENTATION
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// CRfsCustCmdTest::TestFirstBootRfs
//------------------------------------------------------------------------------
TInt CRfsCustCmdTest::TestFirstBootRfs( TTestResult& aResult )
    {
    FUNC_LOG;
    
    CRfsCustCmdTestHelper* cmd = HelperConstruct( aResult, CRfsCustCmd::ERfsCustCmdFirstBoot );
    if( cmd )
        {
        cmd->RfsInitialize();
        delete cmd;
        }
    
    return KErrNone;
    }

//------------------------------------------------------------------------------
// CRfsCustCmdTest::TestDeepRfs
//------------------------------------------------------------------------------
TInt CRfsCustCmdTest::TestDeepRfs( TTestResult& aResult )
    {
    FUNC_LOG;
    
    CRfsCustCmdTestHelper* cmd = HelperConstruct( aResult, CRfsCustCmd::ERfsCustCmdDeep );
    if( cmd )
        {
        cmd->RfsInitialize();
        delete cmd;
        }
    
    return KErrNone;
    }

//------------------------------------------------------------------------------
// CRfsCustCmdTest::TestPreUiRfs
//------------------------------------------------------------------------------
TInt CRfsCustCmdTest::TestPreUiRfs( TTestResult& aResult )
    {
    FUNC_LOG;
    
    CRfsCustCmdTestHelper* cmd = HelperConstruct( aResult, CRfsCustCmd::ERfsCustCmdPreUi );
    if( cmd )
        {
        cmd->RfsInitialize();
        delete cmd;
        }
    
    return KErrNone;
    }

//------------------------------------------------------------------------------
// CRfsCustCmdTest::TestPostUiRfs
//------------------------------------------------------------------------------
TInt CRfsCustCmdTest::TestPostUiRfs( TTestResult& aResult )
    {
    FUNC_LOG;
    
    CRfsCustCmdTestHelper* cmd = HelperConstruct( aResult, CRfsCustCmd::ERfsCustCmdPostUi );
    if( cmd )
        {
        cmd->RfsInitialize();
        delete cmd;
        }
    
    return KErrNone;
    }


//------------------------------------------------------------------------------
// CRfsCustCmdTest::TestFirstBootRfsExecute
//------------------------------------------------------------------------------
TInt CRfsCustCmdTest::TestFirstBootRfsExecute( TTestResult& aResult )
    {
    FUNC_LOG;
    
    CRfsCustCmdTestHelper* cmd = HelperConstruct( aResult, CRfsCustCmd::ERfsCustCmdFirstBoot );
    if( cmd )
        {
        cmd->RfsExecute();
        delete cmd;
        }
    
    return KErrNone;
    }

//------------------------------------------------------------------------------
// CRfsCustCmdTest::TestDeepRfsExecute
//------------------------------------------------------------------------------
TInt CRfsCustCmdTest::TestDeepRfsExecute( TTestResult& aResult )
    {
    FUNC_LOG;
    
    CRfsCustCmdTestHelper* cmd = HelperConstruct( aResult, CRfsCustCmd::ERfsCustCmdDeep );
    if( cmd )
        {
        cmd->RfsExecute();
        delete cmd;
        }
    
    return KErrNone;
    }

//------------------------------------------------------------------------------
// CRfsCustCmdTest::TestPreUiRfsExecute
//------------------------------------------------------------------------------
TInt CRfsCustCmdTest::TestPreUiRfsExecute( TTestResult& aResult )
    {
    FUNC_LOG;
    
    CRfsCustCmdTestHelper* cmd = HelperConstruct( aResult, CRfsCustCmd::ERfsCustCmdPreUi );
    if( cmd )
        {
        cmd->RfsExecute();
        delete cmd;
        }
    
    return KErrNone;
    }

//------------------------------------------------------------------------------
// CRfsCustCmdTest::TestPostUiRfsExecute
//------------------------------------------------------------------------------
TInt CRfsCustCmdTest::TestPostUiRfsExecute( TTestResult& aResult )
    {
    FUNC_LOG;
    
    CRfsCustCmdTestHelper* cmd = HelperConstruct( aResult, CRfsCustCmd::ERfsCustCmdPostUi );
    if( cmd )
        {
        cmd->RfsExecute();
        delete cmd;
        }
    
    return KErrNone;
    }


//------------------------------------------------------------------------------
// CRfsCustCmdTest::TestFirstBootRfsExecuteCancel
//------------------------------------------------------------------------------
TInt CRfsCustCmdTest::TestFirstBootRfsExecuteCancel( TTestResult& aResult )
    {
    FUNC_LOG;
    
    CRfsCustCmdTestHelper* cmd = HelperConstruct( aResult, CRfsCustCmd::ERfsCustCmdFirstBoot );
    if( cmd )
        {
        cmd->RfsExecuteCancel();
        delete cmd;
        }
    
    return KErrNone;
    }

//------------------------------------------------------------------------------
// CRfsCustCmdTest::TestDeepRfsExecuteCancel
//------------------------------------------------------------------------------
TInt CRfsCustCmdTest::TestDeepRfsExecuteCancel( TTestResult& aResult )
    {
    FUNC_LOG;
    
    CRfsCustCmdTestHelper* cmd = HelperConstruct( aResult, CRfsCustCmd::ERfsCustCmdDeep );
    if( cmd )
        {
        cmd->RfsExecuteCancel();
        delete cmd;
        }
    
    return KErrNone;
    }

//------------------------------------------------------------------------------
// CRfsCustCmdTest::TestPreUiRfsExecuteCancel
//------------------------------------------------------------------------------
TInt CRfsCustCmdTest::TestPreUiRfsExecuteCancel( TTestResult& aResult )
    {
    FUNC_LOG;
    
    CRfsCustCmdTestHelper* cmd = HelperConstruct( aResult, CRfsCustCmd::ERfsCustCmdPreUi );
    if( cmd )
        {
        cmd->RfsExecuteCancel();
        delete cmd;
        }
    
    return KErrNone;
    }

//------------------------------------------------------------------------------
// CRfsCustCmdTest::TestPostUiRfsExecuteCancel
//------------------------------------------------------------------------------
TInt CRfsCustCmdTest::TestPostUiRfsExecuteCancel( TTestResult& aResult )
    {
    FUNC_LOG;
    
    CRfsCustCmdTestHelper* cmd = HelperConstruct( aResult, CRfsCustCmd::ERfsCustCmdPostUi );
    if( cmd )
        {
        cmd->RfsExecuteCancel();
        delete cmd;
        }
    
    return KErrNone;
    }


//------------------------------------------------------------------------------
// CRfsCustCmdTest::HelperConstruct
//------------------------------------------------------------------------------
CRfsCustCmdTestHelper* CRfsCustCmdTest::HelperConstruct( TTestResult& aResult,
    CRfsCustCmd::TRfsCustCmdType aType )
    {
    FUNC_LOG;
    
    CRfsCustCmdTestHelper* base = NULL;
    TRAPD( err, base = CRfsCustCmdTestHelper::NewL( aType ) );
    if( err != KErrNone )
        {
        _LIT( KDescription, "HelperConstruct failed" );
        aResult.SetResult( err, KDescription );
        }
    
    return base;
    }
