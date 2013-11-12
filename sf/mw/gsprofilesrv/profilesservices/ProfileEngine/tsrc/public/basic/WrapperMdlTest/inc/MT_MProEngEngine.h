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
// MT_MProEngEngine.h
//

#ifndef MT_MPROENGENGINE_H
#define MT_MPROENGENGINE_H

//  INCLUDES
#include <e32std.h>
#include <f32file.h>
#include <bamdesca.h>
#include <cntdef.h>
#include "MT_CSchedulerUtility.h"
#include <CEUnitTestSuiteClass.h>

// FORWARD DECLARATIONS
class MProEngEngine;
class MProEngProfile;
class MProEngEngine;
class MProfileEngineExtended;
class MProfilesNamesArray;
class MProfile;
class MT_CProEngProfileObserver;

// CLASS DECLARATION

/**
* Module tests for Profile name array wrapper.
*
*/
class MT_MProEngEngine
     : public CEUnitTestSuiteClass
    {
    public:

        /**
         * Two phase construction
         */
        static MT_MProEngEngine* NewL();
        static MT_MProEngEngine* NewLC();

        /**
        * Destructor.
        */
        virtual ~MT_MProEngEngine();

    private:    // Constructors and destructors

        MT_MProEngEngine();
        void ConstructL();

    private: // Setup functions

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

#ifdef __DYNAMIC_PROFILES
        void SetupDynamicProfilesL();
        void Setup30L();
        void Setup31L();
        void Setup32L();
        void Setup33L();
        void Setup34L();
        void Setup35L();
        void Setup36L();
        void Setup37L();
        void Setup38L();
        void Setup39L();
        void Setup40L();
        void Setup41L();
        void Setup42L();
        void Setup43L();
        void Setup44L();
        void Setup45L();
        void Setup46L();
        void Setup47L();
        void Setup48L();
        void Setup49L();
#endif // __DYNAMIC_PROFILES

    private: // Teardown functions

        void Teardown();

#ifdef __DYNAMIC_PROFILES
        void TeardownDynamicProfiles();
#endif // __DYNAMIC_PROFILES

    private: // Test functions

        void ReleaseTestL();
        void ActiveProfileLCTestL();
        void ActiveProfileTestL();
        void ActiveProfileIdTestL();
        void ProfileNameArrayLCTestL();
        void SetActiveProfileTestL();
        void ProfileLCTestL();
        void ProfileTestL();

#ifdef __DYNAMIC_PROFILES
        void SetActiveProfileDynamicProfilesTestL();
        void ProfileDynamicProfilesTestL();

    private: // Helper functions

        void ForcedLeaveL();

#endif // __DYNAMIC_PROFILES

    private:    // Implementation

		EUNIT_DECLARE_TEST_TABLE; 
		
    private:    // Data

        TInt iOrigId;
        TInt iActiveId;
        // owned
        MProfileEngineExtended* iInternalEngine;
        // owned
        MProEngEngine* iEngine;
    };

#endif      // MT_MPROENGENGINE_H

// End of File




