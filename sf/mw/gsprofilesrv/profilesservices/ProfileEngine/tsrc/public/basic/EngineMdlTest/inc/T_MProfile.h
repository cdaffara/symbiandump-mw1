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
* Description:  MProfile test class.
*
*/

////////////////////////////////////////////////////////////////////////
// T_MProfile.h
//

// INCLUDES
#include <CEUnitTestSuiteClass.h>

#include <CPbkContactEngine.h>
#include <cntdef.h>
#include <f32file.h>
#include <s32file.h>

#include <APGCLI.H>


// FORWARD DECLARATION
class MProfileEngine;
class MProfile;

/**
 * Test suite for class MProfile
 */
class T_MProfile : public CEUnitTestSuiteClass
    {
    public:     // Construct / destruct
        static T_MProfile* NewLC();
        ~T_MProfile();

    private:
        void ConstructL();

    private:    // Assistance methods

    private:    // test methods
        // setups
        void SetupL();

        // teardowns
        void Teardown();

        // tests
        void EmptyTestL();
        void CreateTestL();

        // MProfile
        void GetAlertForTestL();
        void IsSilentTestL();
        void GetProfileNameTestL();
        void GetProfileTonesTestL();
        void GetProfileExtraTonesTestL();
        void GetProfileExtraSettingsTestL();

    private:    // Implementation

        EUNIT_DECLARE_TESTCASE_TABLE;

    private:    // Data
        MProfileEngine* iProfileEngine;
        MProfile* iProfile;
        TInt iActiveId;
        RFs iFs;
        CPbkContactEngine* iCE;
        RArray<TContactItemId> iArray;
    };
