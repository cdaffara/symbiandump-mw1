/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Testing context wrapper
 *
*/


#ifndef CPRFWTESTCONTEXTWRAPPER_H
#define CPRFWTESTCONTEXTWRAPPER_H 

#include <e32base.h>

#include "prfwteststatuseventlistener.h"

class MXIMPClient;
class MXIMPContext;
class CXIMPTestStatusEventListener;
class CXIMPTestMessenger;
class CXIMPTestFileTool;
class MPresenceFeatures;

/**
 * Wrapper for testing.
 * Owns the things needed to test one host-plugin connection.
 *
 * @since S60 v4.0
 */
class CXIMPTestContextWrapper : public CBase
    {
    public:

        /**
         * @param aIndex Index of wrapper (used for pubsub-messaging
         * between test plugin and test code).
         */
        IMPORT_C static CXIMPTestContextWrapper* NewL( TInt aIndex, MXIMPClient* aClient );
        virtual ~CXIMPTestContextWrapper();

    private:

        CXIMPTestContextWrapper( TInt aIndex, MXIMPClient* aClient );
        void ConstructL();

    public:

        // accessors
        IMPORT_C MXIMPContext* GetContext();
        IMPORT_C CXIMPTestStatusEventListener* GetEventListener();
        IMPORT_C CXIMPTestMessenger* GetMessenger();
        IMPORT_C MXIMPClient* GetClient();
        IMPORT_C RArray<TXIMPTestStatusEventSnapshot>* GetStatusTraits();
        IMPORT_C MPresenceFeatures* GetPresenceFeatures();

        // low-level test support - see e.g. t_sessionmng test for usage
        //
        /**
         * Delete the presence context and set it to NULL.
         */
        IMPORT_C void DeletePresenceContext();

        /**
         * Like SetupListenerL but sets no event template. Use AppendToTraitsL to set
         * the desired templates. Useful for certain test cases.
         */
        IMPORT_C void SetupEmptyListener();

        /**
         * Setup a listener using the given event template for the traits.
         * @param aEventType The event template to listen to.
         */
        IMPORT_C void SetupListenerL( TXIMPTestStatusEventTemplate aEventType );

        /**
         * Append the given template to the internal traits.
         * @param aEventType The event template to add.
         */
        IMPORT_C void AppendToTraitsL( TXIMPTestStatusEventTemplate aEventType );

        /**
         * Setup a listener for a request complete event. All other events from the
         * given template are ignored (except those given by aAdditionalEvents).
         * @param aEventType The event template.
         * @param aAdditionalEvents Additional events to listen to.
         */
        IMPORT_C void SetupListenerReqCompleteL( 
                TXIMPTestStatusEventTemplate aEventType,
                RArray< TInt32 >* aAdditionalEvents = NULL );

        /**
         * Clear the listener.
         */
        IMPORT_C void ClearListener();
        
        /**
         * Back to begin state of listener.
         */
        IMPORT_C void ResetListener(); 

        /**
         * Wait for the events and a request complete event for the given request id.
         * @param aReqId The request id for which a req. complete ends the wait
         */
        IMPORT_C void WaitRequestAndStackEvents( TXIMPRequestId aReqId );

        /**
         * Wait for any event to occur. Event ends the wait.
         */
        IMPORT_C void WaitAnyEvent();

        /**
         * Verify the event stack against the previously set template(s).
         * The description is printed to the EUnit log.
         * @param aDesc The description
         */
        IMPORT_C void VerifyEventStackL( const TPtrC8& aDesc );

        // high-level test support
        /**
         * Happy-path binding.
         */
        IMPORT_C void BindL( 
                TUid aUid, 
                const TDesC& aServer,
                const TDesC& aUser,
                const TDesC& aPassword );
        
        /**
         * Happy-path binding alternative method.
         * The server, userid and password are automatically modified
         * based on the given id:
         * Server: www.imps%d.no/wv
         * User:   user%d
         * Password: password%d.
         * @param aId An id used to modify server login data.
         */
        IMPORT_C void BindL( TInt aId );
        
        IMPORT_C void BindWithSettingsL( TInt aId );
        /**
         * Happy-path unbinding
         */
        IMPORT_C void UnbindL();
        
        /**
         * Access to filetool of wrapper.
         */
        IMPORT_C CXIMPTestFileTool& FileTool() const;
        
        /**
         * @return Instance id
         */
        IMPORT_C const TDesC16& InstanceId() const;        

    private: // data

        // not owned
        MXIMPClient* iClient;

        // owned
        MXIMPContext* iContext;

        // owned
        MPresenceFeatures* iPresFeatures;


        // owned
        CXIMPTestStatusEventListener* iListener;

        // owned
        CXIMPTestMessenger* iMessenger;

        // owned
        RArray< TXIMPTestStatusEventSnapshot > iStatusEventTraits;

        TInt iIndex;
        
        /**
         * Filetool 
         */
        CXIMPTestFileTool* iFileTool;
        
        HBufC16* iInstance;     // the global instance id        

    };

#endif // CPRFWTESTCONTEXTWRAPPER_H 
