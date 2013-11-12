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
* Contributors: Module tests for Profiles alert tone seeker.
*
* Description:
*
*/
////////////////////////////////////////////////////////////////////////
// MT_MProEngAlertToneSeeker.h
//

#ifndef MT_MPROENGALERTTONESEEKER_H
#define MT_MPROENGALERTTONESEEKER_H

//  INCLUDES
#include <e32std.h>
#include "bamdesca.h"
#include "MT_CSchedulerUtility.h"
#include <CEUnitTestSuiteClass.h>
#include <MProEngAlertToneSeekerObserver.h>

// FORWARD DECLARATIONS
class MProEngAlertToneSeeker;
class MT_CProEngAlertToneSeekerObserver;

// CLASS DECLARATION

/**
* Module tests for Profiles alert tone seeker.
*
*/
class MT_MProEngAlertToneSeeker
     : public CEUnitTestSuiteClass
    {
    public:

        /**
         * Two phase construction
         */
        static MT_MProEngAlertToneSeeker* NewL();
        static MT_MProEngAlertToneSeeker* NewLC();

        /**
        * Destructor.
        */
        virtual ~MT_MProEngAlertToneSeeker();

    private:    // Constructors and destructors

        MT_MProEngAlertToneSeeker();
        void ConstructL();

    private: // Setup functions

        void SetupL();
        void SetupCancelFetchTestL();

    private: // Teardown functions

        void Teardown();

    private: // Test functions

        void FetchAlertToneListTestL();
        void CancelFetchTestL();

    private:    // Implementation

		EUNIT_DECLARE_TEST_TABLE; 
		
    private:    // Data

        MProEngAlertToneSeeker* iSeeker;
        MT_CProEngAlertToneSeekerObserver* iObserver;

    };

// Helper classes to test notifications:
//

class MT_CProEngAlertToneSeekerObserver: public CBase,
        public MProEngAlertToneSeekerObserver
    {
    public:
        static MT_CProEngAlertToneSeekerObserver* NewL()
            {return new (ELeave) MT_CProEngAlertToneSeekerObserver;}
        ~MT_CProEngAlertToneSeekerObserver() { delete iToneList; }
        virtual void HandleAlertToneListCompletedL( MDesCArray* aToneList )
            { iToneList = aToneList; iNotified = ETrue; MT_CSchedulerUtility::InstanceL()->Stop();}

        virtual void HandleError( TInt aError )
            { iError = aError; iNotified = ETrue; MT_CSchedulerUtility::InstanceL()->Stop();}

        MDesCArray* iToneList;
        TInt iNotified;
        TInt iError;
    };

#endif      // MT_MPROENGALERTTONESEEKER_H

// End of File

