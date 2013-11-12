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
// MT_MProEngNotifyHandler.h
//

#ifndef MT_MPROENGNOTIFYHANDLER_H
#define MT_MPROENGNOTIFYHANDLER_H

//  INCLUDES
#include <e32std.h>
#include "MT_CSchedulerUtility.h"
#include <f32file.h>
#include <cntdef.h>
#include <Profile.hrh>
#include <CEUnitTestSuiteClass.h>
#include <MProEngProfileActivationObserver.h>
#include <MProEngActiveProfileObserver.h>
#include <MProEngProfileObserver.h>
#include <MProEngProfileNameArrayObserver.h>

// FORWARD DECLARATIONS
class MProEngNotifyHandler;
class MProEngProfile;
class MProEngEngine;
class MT_CProEngActiveIdObserver;
class MT_CProEngActiveProfileObserver;
class MT_CProEngProfileObserver;
class MT_CProEngNameArrayObserver;
class CPbkContactEngine;
class CMyDebugObserver;
class CRepository;

// CLASS DECLARATION

/**
* Module tests for Profiles change notification handler.
*
*/
class MT_MProEngNotifyHandler
     : public CEUnitTestSuiteClass
    {
    public:

        /**
         * Two phase construction
         */
        static MT_MProEngNotifyHandler* NewL();
        static MT_MProEngNotifyHandler* NewLC();

        /**
        * Destructor.
        */
        virtual ~MT_MProEngNotifyHandler();

    private:    // Constructors and destructors

        MT_MProEngNotifyHandler();
        void ConstructL();

    private: // Setup functions

        void SetupL(  );

        void SetupRequestProfileActivationNotificationsTestL();

        void SetupRequestActiveProfileNotificationsTestL(  );

        void SetupRequestProfileNotificationsTestL();

        void SetupRequestProfileNotificationsTest0L();

        void SetupRequestProfileNotificationsTest1L();

        void SetupRequestProfileNotificationsTest2L();

        void SetupRequestProfileNotificationsTest3L();

        void SetupRequestProfileNotificationsTest4L();

        void SetupRequestProfileNotificationsTest5L();

        void SetupRequestProfileNameArrayNotificationsTestL(  );

        void SetupRequestProfileNameArrayNotificationsTest0L(  );

        void SetupRequestProfileNameArrayNotificationsTest1L(  );

        void SetupRequestProfileNameArrayNotificationsTest2L(  );

        void SetupRequestProfileNameArrayNotificationsTest3L(  );

        void SetupRequestProfileNameArrayNotificationsTest4L(  );

        void SetupRequestProfileNameArrayNotificationsTest5L(  );

        void SetupCancelAllTestL();

    private: // Teardown functions

        void TeardownCommon(  );

        void TeardownRequestProfileActivationNotificationsTest(  );

        void TeardownRequestActiveProfileNotificationsTest(  );

        void TeardownRequestProfileNotificationsTest(  );

        void TeardownRequestProfileNameArrayNotificationsTest(  );

        void TeardownCancelAllTest();

    private: // Test functions

        void RequestProfileActivationNotificationsTestL();

        void RequestActiveProfileNotificationsTestL();

        void RequestProfileNotificationsTestL();

        void RequestProfileNameArrayNotificationsTestL();

        void CancelProfileActivationNotificationsTestL();

        void CancelActiveProfileNotificationsTestL();

        void CancelProfileNotificationsTestL();

        void CancelProfileNameArrayNotificationsTestL();

        void CancelAllTestL();

    private:    // Implementation

		EUNIT_DECLARE_TEST_TABLE; 
		
    private:    // Data

        MProEngNotifyHandler* iHandler;
        MProEngProfile* iProfile;
        MProEngEngine* iEngine;
        TInt iProfileId;
        MT_CProEngActiveIdObserver* iActiveIdObserver;
        MT_CProEngActiveProfileObserver* iActiveProfileObserver;
        MT_CProEngProfileObserver* iProfileObserver;
        MT_CProEngNameArrayObserver* iNameArrayObserver;
        RArray<TContactItemId> iContactGroups;
        TProfileRingingVolume iOrigVolume;
        CPbkContactEngine* iContactEngine;
        RFs iFs;
        CMyDebugObserver* iDebugObs;

    };

// Helper classes to test notifications:
//

class MT_CProEngActiveIdObserver: public CBase,
                                  public MProEngProfileActivationObserver
    {
    public:
        static MT_CProEngActiveIdObserver* NewL()
            {return new (ELeave) MT_CProEngActiveIdObserver;}
        ~MT_CProEngActiveIdObserver() {};
        void HandleProfileActivatedL( TInt aProfileId )
            { iError = KErrNone; iProfileId = aProfileId; MT_CSchedulerUtility::InstanceL()->Stop();}
    
        void HandleProfileActivationNotificationError( TInt aError )
            { iError = aError; MT_CSchedulerUtility::InstanceL()->Stop();}

        TInt iProfileId;
        TInt iError;
    };

class MT_CProEngActiveProfileObserver: public CBase,
                                       public MProEngActiveProfileObserver
    {
    public:
        static MT_CProEngActiveProfileObserver* NewL()
            {return new (ELeave) MT_CProEngActiveProfileObserver;}
        ~MT_CProEngActiveProfileObserver() {};
        void HandleActiveProfileModifiedL()
            { iError = KErrNone; iNotified = ETrue; MT_CSchedulerUtility::InstanceL()->Stop();}
        void HandleActiveProfileNotificationError( TInt aError )
            { iError = aError; MT_CSchedulerUtility::InstanceL()->Stop();}

        TBool iNotified;
        TInt iError;
    };

class MT_CProEngProfileObserver: public CBase,
                                 public MProEngProfileObserver
    {
    public:
        static MT_CProEngProfileObserver* NewL()
            {return new (ELeave) MT_CProEngProfileObserver;}
        ~MT_CProEngProfileObserver() {};
        void HandleProfileModifiedL( TInt aProfileId )
            { iError = KErrNone; iProfileId = aProfileId; MT_CSchedulerUtility::InstanceL()->Stop();}
        void HandleProfileNotificationError( TInt aError )
            { iError = aError; MT_CSchedulerUtility::InstanceL()->Stop();}

        TInt iProfileId;
        TInt iError;
    };

class MT_CProEngNameArrayObserver: public CBase,
                                   public MProEngProfileNameArrayObserver
    {
    public:
        static MT_CProEngNameArrayObserver* NewL()
            {return new (ELeave) MT_CProEngNameArrayObserver;}
        ~MT_CProEngNameArrayObserver() {};
        void HandleProfileNameArrayModificationL()
            { iError = KErrNone; iNotified = ETrue; MT_CSchedulerUtility::InstanceL()->Stop();}
        void HandleProfileNameArrayNotificationError( TInt aError )
            { iError = aError; MT_CSchedulerUtility::InstanceL()->Stop();}

        TBool iNotified;
        TInt iError;
    };

class CMyDebugObserver: public CActive
    {
    public:
        static CMyDebugObserver* NewL();
        ~CMyDebugObserver();
        void RunL();
        TInt RunError( TInt aError );
        void DoCancel();
        void NotifyReqL();

    private:
        CMyDebugObserver();
        void ConstructL();
        CRepository* iRep;
    };

#endif      // MT_MPROENGNOTIFYHANDLER_H

// End of File

