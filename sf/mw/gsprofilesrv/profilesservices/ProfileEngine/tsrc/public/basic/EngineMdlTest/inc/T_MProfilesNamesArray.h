/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MProfilesNamesArray test class.
*
*/

////////////////////////////////////////////////////////////////////////
// T_MProfilesNamesArray.h
//

// INCLUDES
#include <CEUnitTestSuiteClass.h>

#include <f32file.h>
#include <s32file.h>

#include <APGCLI.H>


// FORWARD DECLARATION
class MProfilesNamesArray;
class MProfileEngine;

/**
 * Test suite for class MProfilesNamesArray
 */
class T_MProfilesNamesArray : public CEUnitTestSuiteClass
    {
    public:     // Construct / destruct
        static T_MProfilesNamesArray* NewLC();
        ~T_MProfilesNamesArray();

    private:
        void ConstructL();

    private:    // Assistance methods

    private:    // test methods
        // setups
        void EmptySetupL();
        void SetupL();

        // teardowns
        void EmptyTeardown();
        void Teardown();

        // tests
        void EmptyTestL();
        void CreateTestL();

        void ProfileNameTestL();
        void FindIdTestL();
        void FindNameTestL();
        void MdcaCountTestL();
        void MdcaPointTestL();

    private:    // Implementation

        EUNIT_DECLARE_TESTCASE_TABLE;

    private:    // Data
        RFs iFs;
        MProfileEngine* iProfileEngine;
        MProfilesNamesArray* iProfilesNames;

    };

