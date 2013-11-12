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
* Description: Module tests for Profiles Tones Wrapper interface.
*
*/
////////////////////////////////////////////////////////////////////////
// MT_MProEngToneSettings.h
//

#ifndef MT_MPROENGTONESETTINGS_H
#define MT_MPROENGTONESETTINGS_H

//  INCLUDES
#include <e32std.h>
#include <f32file.h>
#include <bamdesca.h>
#include <cntdef.h>
#include "MT_CSchedulerUtility.h"
#include <CEUnitTestSuiteClass.h>

// FORWARD DECLARATIONS
class MProEngToneSettings;
class MProEngProfile;
class MProEngEngine;
class MProfileEngineExtended;
class MProfile;
class MT_CProEngProfileObserver;

// CLASS DECLARATION

/**
* Module tests for Profiles Tones Wrapper interface.
*
*/
class MT_MProEngToneSettings
     : public CEUnitTestSuiteClass
    {
    public:

        /**
         * Two phase construction
         */
        static MT_MProEngToneSettings* NewL();
        static MT_MProEngToneSettings* NewLC();

        /**
        * Destructor.
        */
        virtual ~MT_MProEngToneSettings();

    private:    // Constructors and destructors

        MT_MProEngToneSettings();
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

        void GetRingingTypeTestL();
        void GetKeypadVolumeTestL();
        void GetRingingVolumeTestL();
        void GetVibraTestL();
        void GetWarningAndGameTonesTestL();
        void GetTextToSpeechTestL();

        void SetRingingTypeTestL();
        void SetKeypadVolumeTestL();
        void SetRingingVolumeTestL();
        void SetVibraTestL();
        void SetWarningAndGameTonesTestL();
        void SetTextToSpeechTestL();

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

#endif      // MT_MPROENGTONESETTINGS_H

// End of File

