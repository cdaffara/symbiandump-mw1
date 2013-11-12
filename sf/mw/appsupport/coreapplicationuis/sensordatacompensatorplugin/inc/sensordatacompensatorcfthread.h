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
* Description:  Plugin controller
*
*/


#ifndef SENSORDATACOMPENSATORCFTHREAD_H
#define SENSORDATACOMPENSATORCFTHREAD_H

// INCLUDES
#include <e32base.h>
#include <cflistener.h>
#include <cfkeyvaluepair.h>

/**
 * Thread controller abstraction.
 *
 * @since S60 5.1
 */
class MSensorDataCompensatorCfThreadController
    {
    public:
    
        // Callbacks run in context of listener thread
        virtual void ThreadInitL() = 0;
        virtual void ThreadRelease() = 0;
        
        // Callbacks run in context of client thread
        virtual void NotifyControllerL( const RKeyValueArray& aKeyValueArray ) = 0;
    };
    
/**
 * Thread wrapper class. Class is needed for setting up a thread
 * for listening compensator parameter changes via CF.
 *
 * @lib sensordatacompensatorplugin.lib
 * @since S60 5.1
 */
NONSHARABLE_CLASS( CSensorDataCompensatorCfTread ) : public CActive
    {
    public:
        // Constructors and destructor

        /**
        * Two-phase constructor. Thread is being started on construction.
        */
        static CSensorDataCompensatorCfTread* NewL( MSensorDataCompensatorCfThreadController& aController );

        /**
        * Destructor. Thread is being closed on destruction
        */
        ~CSensorDataCompensatorCfTread();

    public:

        /**
         * Used from listener thread to make local copy of values
         */
        void StoreValuesAndNotify( const CCFActionIndication& aActionToExecute );

    private:

        /**
         * Default constructor
         */
        CSensorDataCompensatorCfTread( MSensorDataCompensatorCfThreadController& aController );

        /**
         * 2nd phase of construction
         */
        void ConstructL();
        
        /**
         * Thread entry point
         */
        static TInt ThreadFunction( TAny* ptr );
        void ThreadFunctionL();

    private:

        // From CActive
        void RunL();
        void DoCancel();

    private:

        /**
         * Helper class for thread shutdown.
         */
        NONSHARABLE_CLASS( CfThreadShutdown ) : public CActive
            {
            public:
                static CfThreadShutdown* NewL(
                    CSensorDataCompensatorCfTread& aWrapper );
                ~CfThreadShutdown();
                void Complete();

            private: // From CActive
                void RunL();
                void DoCancel();

            private:
                CfThreadShutdown( CSensorDataCompensatorCfTread& aWrapper );

            private:
                CSensorDataCompensatorCfTread& iWrapper;
                TRequestStatus* iShutdownStatus;
            };
        
        /**
         * Helper class for client notification on parameter update.
         */
        NONSHARABLE_CLASS( CfTreadNotify ) : public CActive
            {
            public:
                static CfTreadNotify* NewL(
                    CSensorDataCompensatorCfTread& aWrapper );
                ~CfTreadNotify();
                void Complete();

            private: // From CActive
                void RunL();
                void DoCancel();

            private:
                CfTreadNotify( CSensorDataCompensatorCfTread& aWrapper );

            private:
                CSensorDataCompensatorCfTread& iWrapper;
                TRequestStatus* iNotifyStatus;
            };

    private: // data

        /**
         * Client controlling thread startup and shutdown
         */
        MSensorDataCompensatorCfThreadController& iController;
        
        /**
         * Client thread id
         */
        TThreadId iClientId;

        /**
         * Shutdown AO. Owned by listener thread and hence 
         * may not be deleted on destructor.
         */
        CfThreadShutdown* iCfThreadShutdown;

        /**
         * Notifier AO. Owned by client thread .
         */
        CfTreadNotify* iCfTreadNotify;

        /**
         * Local copy of key value pair array. Values get appended
         * on every cf action indication and array gets reseted
         * once values are loaded to compensator internal data structures.
         * Access to array from different threads is protected with mutex.
         */
        RKeyValueArray iKeyValueArray;
        
        /**
         * Listener thread handle. Used to notify shutdown request
         * when compensator plugin is being deleted.
         */
        RThread iThread;
        
        /**
         * Mutex used to control access to data when updating array 
         * after cf action indication and when notifying client 
         * about updated values.
         */
        RMutex iMutex;
    };

#endif // SENSORDATACOMPENSATORCFTHREAD_H
