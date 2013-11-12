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
* Description:  Declaration of test class
*
*/


#ifndef C_MSGCENTERRFSPLUGINTEST_H
#define C_MSGCENTERRFSPLUGINTEST_H

// INCLUDES
#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#include <testscripterinternal.h>
#include "trace.h"

// CONSTANTS

// MACROS
// None

// DATA TYPES
// None

// FUNCTION PROTOTYPES
// None

// FORWARD DECLARATIONS
class CRFSPlugin;

// CLASS DECLARATION
class CMsgCentreRfsPluginTest;
class TCaseInfo;
// DESCRIPTION
// This a Test Module interface template
// that does not really do anything.

typedef TInt (CMsgCentreRfsPluginTest::* TestFunction)(TTestResult&);

NONSHARABLE_CLASS(CMsgCentreRfsPluginTest) : public CScriptBase
    {
    public: // Enumerations

    private: // Enumerations

    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CMsgCentreRfsPluginTest* NewL(CTestModuleIf& aTestModuleIf);

        /**
        * Destructor.
        */
        ~CMsgCentreRfsPluginTest();

    public: // New functions

    public: // Functions from base classes

        TInt RunMethodL( CStifItemParser& aItem );

    protected: // New functions

        TInt CreatePlugin( CStifItemParser& aItem );
        TInt DestroyPlugin( CStifItemParser& aItem );
        TInt RestoreFactorySettings( CStifItemParser& aItem );
        TInt GetScript( CStifItemParser& aItem );
        TInt ExecuteCustomCommand( CStifItemParser& aItem );

    public:

    private:
        /**
        * C++ default constructor.
        */
        CMsgCentreRfsPluginTest(CTestModuleIf& aTestModuleIf);

        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();

        /**
        * Function returning test case name and pointer to test case function
        */
        const TCaseInfo Case ( const TInt aCaseNumber ) const;

        void ResetDataItems();

    public: // Data

    protected: // Data

    private: // Data

        CRFSPlugin* iPlugin;
        TBool iTestResult;
        LOG_OBJECT_NEW

    public: // Friend classes

    protected: // Friend classes

    private: // Friend classes

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

    TCaseInfo( const TText* a ) : iCaseName( (TText*) a )
        {
        };

    };


#endif // C_MSGCENTERRFSPLUGINTEST_H

// End of File
