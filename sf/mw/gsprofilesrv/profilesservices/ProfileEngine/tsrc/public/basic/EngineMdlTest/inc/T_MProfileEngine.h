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
* Description:  MProfileEngine test class.
*
*/

////////////////////////////////////////////////////////////////////////
// T_MProfileEngine.h
//

// INCLUDES
#include <CEUnitTestSuiteClass.h>

#include <f32file.h>
#include <s32file.h>

#include <APGCLI.H>


// FORWARD DECLARATION
class MProfileEngine;
class MProfileEngineExtended;
class CRepository;


/**
 * Test suite for class MProfileEngine
 */
class T_MProfileEngine : public CEUnitTestSuiteClass
    {
    public:     // Construct / destruct
        static T_MProfileEngine* NewLC();
        ~T_MProfileEngine();

    private:    // test methods
        // setups
        void EmptySetupL();
        void SetupL();
        void Setup0L();
        void SetupGetRingingVolumeL();
        void SetupGetMediaVolumeL();

        // teardowns
        void EmptyTeardown();
        void Teardown();

        // tests
        void EmptyTestL();
        void CreateTestL();

        void GetActiveProfileTestL();
        void GetActiveProfileIdTestL();
        void SetActiveProfileTestL();
        void GetProfilesNamesArrayTestL();
        void SetTempRingingVolumeTestL();
        void SetTempMediaVolumeTestL();
        void GetTempRingingVolumeTestL();
        void GetTempMediaVolumeTestL();
        void IsActiveProfileTimedLTestL();

    private:    // Implementation

        EUNIT_DECLARE_TESTCASE_TABLE;

    private:    // Data
        MProfileEngine* iProfileEngine;
        MProfileEngineExtended* iProfileEngineExtended;
        TInt iActiveId;
        RArray<TInt> iArray;
        RFs iFs;
        CRepository* iCenRep;
        TInt iOrigMediaVolume;
        TInt iOrigRingingVolume;
        TInt iTimed;

    };
