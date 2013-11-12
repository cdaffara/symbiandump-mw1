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
* Description:  Declaration of CFormatterRfsPluginTestSuite class
*
*/


#ifndef C_FORMATTERRFSPLUGINTESTSUITE_H
#define C_FORMATTERRFSPLUGINTESTSUITE_H

#include <eunit/ceunittestsuiteclass.h>
#include <e32def.h>

class CRFSPlugin;

/**
*  Test cases for ?testedclassname class.
*
*  @lib None
*/
NONSHARABLE_CLASS( CFormatterRfsPluginTestSuite ) : public CEUnitTestSuiteClass
    {

public:

    /**
    * Constructs a CFormatterRfsPluginTestSuite object.
    *
    * @return The new object
    */
    static CFormatterRfsPluginTestSuite* NewL();
    static CFormatterRfsPluginTestSuite* NewLC();

    /**
    * Destructor.
    */
    virtual ~CFormatterRfsPluginTestSuite();

private:

    /**
    * First phase constructor.
    */
    CFormatterRfsPluginTestSuite();

    /**
    * Second phase constructor.
    */
    void ConstructL();

private:    // New methods

    void SetupEmptyL();
    void SetupConditionalL();
    void SetupL();
    void SetupExcludeListOnlyL();
    void SetupAppSpecExcludeListOnlyL();
    void SetupExcludeListL();
    void SetupAppSpecExcludeListL();
    void Teardown();
    void TeardownExcludeList();
    void TeardownAppSpecExcludeList();

    void TestConstructorL();
    void TestConstructorNoMemoryL();

    void TestRestoreConditionalL();
    void TestRestoreL();
    void TestRestoreWithExcludeListL();
    void TestRestoreNoMemoryL();
    void TestGetScriptConditionalL();
    void TestGetScriptL();
    void TestGetScriptEmptyL();
    void TestCustomCommandL();
    
    void AppExcludeListsExistL();

private: // data

    EUNIT_DECLARE_TEST_TABLE;

    /** Instance of the class that this test suite is testing. */
    CRFSPlugin* iTestObj;
    
    TBool iAppSpecificFilesExist;
    };

#endif // C_FORMATTERRFSPLUGINTESTSUITE_H
