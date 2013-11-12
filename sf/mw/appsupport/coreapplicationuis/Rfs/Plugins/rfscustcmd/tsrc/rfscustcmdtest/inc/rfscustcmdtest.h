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
* CRfsCustCmdTest class declaration.
*
*/


#ifndef RFSCUSTCMDTEST_H
#define RFSCUSTCMDTEST_H

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

// SYSTEM INCLUDES
#include <stiftestmodule.h>
#include "rfscustcmd.h"

// FORWARD DECLARATIONS
class TCaseInfo;
class CRfsCustCmdTest;
class CRfsCustCmdTestHelper;

// DESCRIPTION
// This a Test Module interface template
// that does not really do anything.
typedef TInt ( CRfsCustCmdTest::*TestFunction )( TTestResult& );

NONSHARABLE_CLASS( CRfsCustCmdTest ) : public CTestModuleBase
    {
public: // Constructors and destructor

    /**
    * Two-phased constructor.
    */
    static CRfsCustCmdTest* NewL( CTestModuleIf& aTestModuleIf );

    /**
    * Destructor.
    */
    virtual ~CRfsCustCmdTest();

public: // Functions from base classes

    // @see CTestModuleBase
    virtual TInt GetTestCasesL( const TFileName& aTestCaseFile, 
        RPointerArray<TTestCaseInfo>& aTestCases );

    // @see CTestModuleBase
    virtual TInt RunTestCaseL( const TInt aCaseNumber, 
        const TFileName& aTestCaseFile,
        TTestResult& aResult );

protected: // New functions

    // Test cases
    TInt TestFirstBootRfs( TTestResult& aResult );
    TInt TestDeepRfs( TTestResult& aResult );
    TInt TestPreUiRfs( TTestResult& aResult );
    TInt TestPostUiRfs( TTestResult& aResult );
    TInt TestFirstBootRfsExecute( TTestResult& aResult );
    TInt TestDeepRfsExecute( TTestResult& aResult );
    TInt TestPreUiRfsExecute( TTestResult& aResult );
    TInt TestPostUiRfsExecute( TTestResult& aResult );
    TInt TestFirstBootRfsExecuteCancel( TTestResult& aResult );
    TInt TestDeepRfsExecuteCancel( TTestResult& aResult );
    TInt TestPreUiRfsExecuteCancel( TTestResult& aResult );
    TInt TestPostUiRfsExecuteCancel( TTestResult& aResult );

private:

    // Test case helper
    CRfsCustCmdTestHelper* HelperConstruct( TTestResult& aResult, CRfsCustCmd::TRfsCustCmdType aType );

    /**
    * C++ default constructor.
    */
    CRfsCustCmdTest( CTestModuleIf& aTestModuleIf );

    /**
    * By default Symbian OS constructor is private.
    */
    void ConstructL();

    /**
    * Function returning test case name and pointer to test case function
    */
    const TCaseInfo Case( const TInt aCaseNumber ) const;

private: // Data

    TestFunction iMethod;
    CActiveScheduler* iScheduler;
    };

// Function pointer related internal definitions

#ifndef __GCC32__
    #define GETPTR
#else
    #define GETPTR &
#endif


// An internal structure containing a test case name and
// the pointer to function doing the test
class TCaseInfoInternal
    {
    public:
        const TText* iCaseName;
        TestFunction iMethod;
    };

// An internal structure containing a test case name and
// the pointer to function doing the test
class TCaseInfo
    {
    public:
        TPtrC iCaseName;
        TestFunction iMethod;

    TCaseInfo( const TText* a ) : iCaseName( ( TText* ) a )
        {
        };
    };

#endif // RFSCUSTCMDTEST_H
