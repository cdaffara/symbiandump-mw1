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
* Description: Module tests for Profiles alert tone seeker.
*
*/
////////////////////////////////////////////////////////////////////////
// MT_MProEngProfileName.h
//

#ifndef MT_MPROENGPROFILENAME_H
#define MT_MPROENGPROFILENAME_H

//  INCLUDES
#include <e32std.h>
#include <f32file.h>
#include <bamdesca.h>
#include <cntdef.h>
#include "MT_CSchedulerUtility.h"
#include <CEUnitTestSuiteClass.h>

// FORWARD DECLARATIONS
class MProEngProfileName;
class MProEngProfile;
class MProEngEngine;
class MProfileEngineExtended;
class MProfile;
class MT_CProEngProfileObserver;

// CLASS DECLARATION

/**
* Module tests for Profiles alert tone seeker.
*
*/
class MT_MProEngProfileName
     : public CEUnitTestSuiteClass
    {
    public:

        /**
         * Two phase construction
         */
        static MT_MProEngProfileName* NewL();
        static MT_MProEngProfileName* NewLC();

        /**
        * Destructor.
        */
        virtual ~MT_MProEngProfileName();

    private:    // Constructors and destructors

        MT_MProEngProfileName();
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

    private: // Teardown functions

        void Teardown();

    private: // Test functions

        void GetIdTestL();

        void GetNameTestL();

        void SetNameTestL();

    private:    // Implementation

		EUNIT_DECLARE_TEST_TABLE; 
		
    private:    // Data

        TInt iProfileId;
        // owned
        MProfileEngineExtended* iInternalEngine;
        // owned
        MProfile* iInternalProfile;
        // owned
        MProEngEngine* iEngine;
        // owned
        MProEngProfile* iProfile;

    };

#endif      // MT_MPROENGPROFILENAME_H

// End of File


