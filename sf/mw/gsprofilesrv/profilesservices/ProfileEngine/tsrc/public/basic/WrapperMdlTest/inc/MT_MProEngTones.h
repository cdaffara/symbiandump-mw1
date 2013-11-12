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
// MT_MProEngTones.h
//

#ifndef MT_MPROENGTONES_H
#define MT_MPROENGTONES_H

//  INCLUDES
#include <e32std.h>
#include <f32file.h>
#include <bamdesca.h>
#include <cntdef.h>
#include "MT_CSchedulerUtility.h"
#include <CEUnitTestSuiteClass.h>

// FORWARD DECLARATIONS
class MProEngTones;
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
class MT_MProEngTones
     : public CEUnitTestSuiteClass
    {
    public:

        /**
         * Two phase construction
         */
        static MT_MProEngTones* NewL();
        static MT_MProEngTones* NewLC();

        /**
        * Destructor.
        */
        virtual ~MT_MProEngTones();

    private:    // Constructors and destructors

        MT_MProEngTones();
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

        void GetRingingTone1TestL();
        void GetRingingTone2TestL();
        void GetMessageAlertToneTestL();
        void GetEmailAlertToneTestL();
        void GetVideoCallRingingToneTestL();

        void SetRingingTone1TestL();
        void SetRingingTone2TestL();
        void SetMessageAlertToneTestL();
        void SetEmailAlertToneTestL();
        void SetVideoCallRingingToneTestL();
        //void SetDRMRingingTone1TestL();
        //void SetRingingToneSizeLimitTestL();
        //int GetToneSizeLimitL();

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

#endif      // MT_MPROENGTONES_H

// End of File



