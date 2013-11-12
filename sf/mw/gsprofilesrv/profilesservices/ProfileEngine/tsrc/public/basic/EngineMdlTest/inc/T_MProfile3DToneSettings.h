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
* Description:  MProfile3DToneSettings test class.
*
*/

////////////////////////////////////////////////////////////////////////
// T_MProfile3DToneSettings.h
//

// INCLUDES
#include <CEUnitTestSuiteClass.h>

#include <f32file.h>
#include <s32file.h>

#include <APGCLI.H>


// FORWARD DECLARATION
class MProfileEngine;
class MProfile;
class MProfileExtraSettings;
class MProfile3DToneSettings;
class CRepository;

/**
 * Test suite for class MProfile
 */
class T_MProfile3DToneSettings : public CEUnitTestSuiteClass
    {
    public:     // Construct / destruct
        static T_MProfile3DToneSettings* NewLC();
        ~T_MProfile3DToneSettings();

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

        void GetEffectTestL();
        void GetEchoTestL();

    private:    // Implementation

        EUNIT_DECLARE_TESTCASE_TABLE;

    private:    // Data
        MProfileEngine* iProfileEngine;
        MProfile* iProfile;
        TInt iActiveId;
        RFs iFs;

        const MProfileExtraSettings* iProfileExtraSettings;
        const MProfile3DToneSettings* iProfile3DToneSettings;
        CRepository* iCenRep;
    };

