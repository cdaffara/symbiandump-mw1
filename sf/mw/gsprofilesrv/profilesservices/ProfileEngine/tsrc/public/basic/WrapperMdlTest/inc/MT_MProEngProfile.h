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
* Description: Module tests for Profile name array wrapper.
*
*/
////////////////////////////////////////////////////////////////////////
// MT_MProEngProfile.h
//

#ifndef MT_MPROENGPROFILE_H
#define MT_MPROENGPROFILE_H

//  INCLUDES
#include <e32std.h>
#include <f32file.h>
#include <bamdesca.h>
#include <cntdef.h>
#include "MT_CSchedulerUtility.h"
#include <CEUnitTestSuiteClass.h>
#include <MProEngProfileObserver.h>

// FORWARD DECLARATIONS
class MProEngProfile;
class MProEngEngine;
class MProfileEngineExtended;
class MProfileExtended;
class MT_CProEngProfileObserver;
class CPbkContactEngine;

// CLASS DECLARATION

/**
* Module tests for Profiles alert tone seeker.
*
*/
class MT_MProEngProfile
     : public CEUnitTestSuiteClass
    {
    public:

        /**
         * Two phase construction
         */
        static MT_MProEngProfile* NewL();
        static MT_MProEngProfile* NewLC();

        /**
        * Destructor.
        */
        virtual ~MT_MProEngProfile();

    private:    // Constructors and destructors

        MT_MProEngProfile();
        void ConstructL();

    private: // Setup functions

        void EmptySetup();
        void SetupL();
        void Setup0L();
        void Setup1L();
        void Setup2L();
        void Setup3L();
        void Setup4L();
#ifdef __OFFLINE_MODE
        void Setup5L();
#endif // __OFFLINE_MODE
#ifdef __DRIVE_MODE
        void Setup6L();
#endif // __DRIVE_MODE

        void SetupSetAlertForTestL();

        void SetupAlertForTestL();

        void SetupAlertForTest0L();

        void SetupSetAlertForTest0L();

        void SetupCommitChangeTest0L();


        void SetupAlertForTest1L();

        void SetupSetAlertForTest1L();

        void SetupCommitChangeTest1L();


        void SetupAlertForTest2L();

        void SetupSetAlertForTest2L();

        void SetupCommitChangeTest2L();


        void SetupAlertForTest3L();

        void SetupSetAlertForTest3L();

        void SetupCommitChangeTest3L();


        void SetupAlertForTest4L();

        void SetupSetAlertForTest4L();

        void SetupCommitChangeTest4L();

#ifdef __OFFLINE_MODE
        void SetupAlertForTest5L();

        void SetupSetAlertForTest5L();

        void SetupCommitChangeTest5L();
#endif // __OFFLINE_MODE


#ifdef __DRIVE_MODE
        void SetupAlertForTest6L();

        void SetupSetAlertForTest6L();

        void SetupCommitChangeTest6L();
#endif // __DRIVE_MODE

    private: // Teardown functions

        void EmptyTeardown();

        void Teardown();

        void TeardownAlertForGroups();

    private: // Test functions

        void ReleaseTestL();

        void AlertForTestL();

        void SetAlertForTestL();

        void IsSilentTestL();

        void ProfileNameTestL();

        void ProfileTonesTestL();

        void ToneSettingsTestL();

        void CommitChangeTestL();

    private:    // Implementation

		EUNIT_DECLARE_TEST_TABLE; 
		
    private:    // Data

        TInt iProfileId;
        MProfileEngineExtended* iInternalEngine;
        MProfileExtended* iInternalProfile;
        MProEngEngine* iEngine;
        MProEngProfile* iProfile;
        RArray<TContactItemId> iContactGroups;
        CPbkContactEngine* iContactEngine;
        RFs iFs;

    };

#endif      // MT_MPROENGPROFILE_H

// End of File

