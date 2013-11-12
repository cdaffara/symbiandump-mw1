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
* Description:  MProfileExtraSettings test class.
*
*/

////////////////////////////////////////////////////////////////////////
// T_MProfileExtraSettings.h
//

// INCLUDES
#include <CEUnitTestSuiteClass.h>

#include <f32file.h>
#include <s32file.h>
#include <CNTDEF.H>
#include <APGCLI.H>


// FORWARD DECLARATION
class MProfileEngine;
class MProfile;

/**
 * Test suite for class MProfileExtraSettings
 */
class T_MProfileExtraSettings : public CEUnitTestSuiteClass
    {
    public:     // Construct / destruct
        static T_MProfileExtraSettings* NewLC();
        ~T_MProfileExtraSettings();

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
        void ProfilePttSettingsTestL();
        void ProfileFeedbackSettingsTestL();
        void Profile3DToneSettingsTestL();

    private:    // Implementation

        EUNIT_DECLARE_TESTCASE_TABLE;

    private:    // Data
        MProfileEngine* iProfileEngine;
        MProfile* iProfile;

    };

