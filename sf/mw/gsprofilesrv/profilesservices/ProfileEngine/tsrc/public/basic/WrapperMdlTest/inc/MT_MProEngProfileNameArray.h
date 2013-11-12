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
// MT_MProEngProfileNameArray.h
//

#ifndef MT_MPROENGPROFILENAMEARRAY_H
#define MT_MPROENGPROFILENAMEARRAY_H

//  INCLUDES
#include <e32std.h>
#include <f32file.h>
#include <bamdesca.h>
#include <cntdef.h>
#include "MT_CSchedulerUtility.h"
#include <CEUnitTestSuiteClass.h>

// FORWARD DECLARATIONS
class MProEngProfileNameArray;
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
class MT_MProEngProfileNameArray
     : public CEUnitTestSuiteClass
    {
    public:

        /**
         * Two phase construction
         */
        static MT_MProEngProfileNameArray* NewL();
        static MT_MProEngProfileNameArray* NewLC();

        /**
        * Destructor.
        */
        virtual ~MT_MProEngProfileNameArray();

    private:    // Constructors and destructors

        MT_MProEngProfileNameArray();
        void ConstructL();

    private: // Setup functions

        void SetupL();

        void SetupDynamicProfilesL();

    private: // Teardown functions

        void Teardown();

        void TeardownDynamicProfiles();

    private: // Test functions

        void ProfileIdTestL();

        void FindByIdTestL();

        void FindByNameTestL();

        void FindByIdDynamicProfileTestL();

        void FindByNameDynamicProfileTestL();

    private:    // Implementation

		EUNIT_DECLARE_TEST_TABLE; 
		
    private:    // Data

        TInt iProfileId;
        // owned
        MProfileEngineExtended* iInternalEngine;
        // owned
        MProEngEngine* iEngine;
        // owned
        MProEngProfileNameArray* iArray;
        // owned
        MProfilesNamesArray* iInternalArray;
    };

#endif      // MT_MPROENGPROFILENAMEARRAY_H

// End of File



