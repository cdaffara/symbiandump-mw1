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
* Description:  CProfileChangeNotifyHandler test class.
*
*/

////////////////////////////////////////////////////////////////////////
// T_CProfileChangeNotifyHandler.h
//

// INCLUDES
#include <CEUnitTestSuiteClass.h>

#include <f32file.h>
#include <s32file.h>

#include <APGCLI.H>
#include <MProfileChangeObserver.h>

// FORWARD DECLARATION
class CProfileChangeNotifyHandler;
class CTestObserver;
class MProfileEngineExtended;
class MProfileExtended;



/**
 * Test suite for class CProfileChangeNotifyHandler
 */
class T_CProfileChangeNotifyHandler : public CEUnitTestSuiteClass
    {
    public:     // Construct / destruct
        static T_CProfileChangeNotifyHandler* NewLC();
        ~T_CProfileChangeNotifyHandler();

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

        void HandleNotifyTestL();


    private:    // Implementation

        EUNIT_DECLARE_TESTCASE_TABLE;

    private:    // Data
        CProfileChangeNotifyHandler* iHandler;
        CTestObserver* iObserver;
        MProfileExtended* iProfile;
        MProfileEngineExtended* iProfileEngine;
        TInt iOrigRingingVolume;
        TFileName iOrigRingingTone;

    };
