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
* Description:  MProfileTones test class.
*
*/

////////////////////////////////////////////////////////////////////////
// T_MProfileTonesName.h
//

// INCLUDES
#include <CEUnitTestSuiteClass.h>

#include <f32file.h>
#include <s32file.h>

#include <APGCLI.H>


// FORWARD DECLARATION
class MProfileEngine;
class MProfile;
class MProfileTones;
class CRepository;

/**
 * Test suite for class MProfile
 */
class T_MProfileTones : public CEUnitTestSuiteClass
    {
    public:     // Construct / destruct
        static T_MProfileTones* NewLC();
        ~T_MProfileTones();

    private:
        void ConstructL();

    private:    // Assistance methods

    private:    // test methods
        // setups
        void EmptySetupL();
        void SetupL();
        void Setup0L();
        void Setup1L();
        void Setup2L();
        void Setup3L();
        void Setup4L();

        // teardowns
        void EmptyTeardown();
        void Teardown();

        // tests
        void EmptyTestL();

        void GetRingingTone1TestL();
        void GetRingingTone2TestL();
        void GetMessageAlertToneTestL();
        void GetProfileToneSettingsTestL();

    private:    // Implementation

        EUNIT_DECLARE_TESTCASE_TABLE;

    private:    // Data
        MProfileEngine* iProfileEngine;
        MProfile* iProfile;
        TInt iActiveId;
        RFs iFs;

        const MProfileTones* iProfileTones;
        CRepository* iCenRep;
    };

