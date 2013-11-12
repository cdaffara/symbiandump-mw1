/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Module test's header file for CSensorDataCompensator API.
*
*/


#ifndef SENSORDATACOMPENSATORPLGTEST_H
#define SENSORDATACOMPENSATORPLGTEST_H

// INCLUDES
#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#include <testscripterinternal.h>
#include <sensordatacompensator.h>
#include <sensrvaccelerometersensor.h>
#include <sensrvorientationsensor.h>
#include <sensrvmagnetometersensor.h>
#include <sensrvmagneticnorthsensor.h>
#include <sensrvtappingsensor.h>
#include <sensordatacompensatorplugin.h>
#include "sensordatacompensatorplgtesttypes.h"


// CONSTANTS
const TInt KCompTestMaxTypeCount = 8;
const TInt KCompTestSinleTypeCount = 1;

// MACROS
// None

// DATA TYPES
// None

// FUNCTION PROTOTYPES
// None

// FORWARD DECLARATIONS
// None

// CLASS DECLARATION
class CSensorDataCompensatorPlgTest;
class TCaseInfo;
// DESCRIPTION
// This a Test Module interface template
// that does not really do anything.

typedef TInt (CSensorDataCompensatorPlgTest::* TestFunction)(TTestResult&);

NONSHARABLE_CLASS(CSensorDataCompensatorPlgTest)
        :public CScriptBase,
        public MSensorDataCompensatorObserver
    {
    public: // Enumerations

    private: // Enumerations

    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CSensorDataCompensatorPlgTest* NewL(CTestModuleIf& aTestModuleIf);

        /**
        * Destructor.
        */
        ~CSensorDataCompensatorPlgTest();

    public: // New functions

    public: // Functions from base classes

        TInt RunMethodL( CStifItemParser& aItem );

    protected: // New functions

        TInt CreateCompensatorPluginL( CStifItemParser& aItem );
        TInt DestroyCompensatorPluginL( CStifItemParser& aItem );
        TInt SetStateL( CStifItemParser& aItem );
        TInt SetExpectedDataL( CStifItemParser& aItem );
        TInt VerifyTestResultL( CStifItemParser& aItem );
        TInt CheckCountL( CStifItemParser& aItem );

    protected: // Functions from base classes
        // from MSensorDataCompensatorObserver
        virtual void CompensationValuesUpdated();
    public:

    private:

             /**
        * C++ default constructor.
        */
        CSensorDataCompensatorPlgTest(CTestModuleIf& aTestModuleIf);

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

        CSensorDataCompensatorPlugin* iPlugin;

        RLibrary iLibrary;

        TSensorCompensationType iCompensationType;
        TSensrvChannelDataTypeId iDataType;

        TInt iExpectedItemCount;
        TFixedArray<TInt ,KCompTestMaxTypeCount> iExpectedXAxisValue;
        TFixedArray<TInt ,KCompTestMaxTypeCount> iExpectedYAxisValue;
        TFixedArray<TInt ,KCompTestMaxTypeCount> iExpedtedZAxisValue;
        TFixedArray<TInt ,KCompTestMaxTypeCount> iExpectedMagneticNorthValue;
        TFixedArray<TInt ,KCompTestMaxTypeCount> iExpectedTappingXAxisValue;
        TFixedArray<TInt ,KCompTestMaxTypeCount> iExpectedTappingYAxisValue;
        TFixedArray<TInt ,KCompTestMaxTypeCount> iExpectedTappingZAxisValue;
        TBool iTestResult;

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


#endif // SENSORDATACOMPENSATORPLGTEST_H

// End of File
