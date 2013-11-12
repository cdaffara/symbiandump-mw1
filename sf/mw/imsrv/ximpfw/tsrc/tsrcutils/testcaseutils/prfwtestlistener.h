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
* Description:  XIMP Framework CXIMPTestListener
*
*/


#ifndef CPRFWTESTLISTENER_H__
#define CPRFWTESTLISTENER_H__


#include <e32base.h>
#include <ximpcontextobserver.h>
#include <ximpcontext.h>

#include "prfwwaitobserver.h"

// CLASS DESCRIPTION
class CXIMPApiObjBase;

/**
 * Presence test listener to record events
 * and to wait request completion.
 */
class CXIMPTestListener : public CBase,
    public MXIMPContextObserver,
    public T_MWaitObserver
    {
    public:

        /**
         * Instantiates new listener object.
         *
         * If valid event source is given, reqisters this object as
         * event observer to it.
         */
        IMPORT_C static CXIMPTestListener* CXIMPTestListener::NewL(
                MXIMPContext* aEventSource );

        ~CXIMPTestListener();

    private:

        CXIMPTestListener( MXIMPContext* aEventSource );

        void ConstructL();

    public:

        /**
         * Sets new event source from where to listen events.
         * Unregisters from previous one.
         * Can be NULL, in which case just performs the
         * unregistration.
         */
        IMPORT_C void SetEventSourceL( MXIMPContext* aEventSource );

        /**
         * Clears the event trait template. After this call,
         * you must call ExpectL to tell listener what events to expect.
         */
        IMPORT_C void Reset();

        /**
         * Add the given event to the list of expected events.
         * The order of ExpectL calls is significant.
         * Ownership of given event is transferred to this class.
         * @param aEvent The event to expect
         */
        IMPORT_C void ExpectL( CXIMPApiObjBase* aEvent );

        /**
         * Method to wait with CActiveSchedulerWait
         * a request complete, then any events which are expected.
         * When all events are received, the method returns. If not all events
         * come, the test case will timeout. If a wrong event or event with wrong
         * data is received, the method will barf in EUNIT_ASSERT.
         * 
         * @return Error code, if any
         */
        IMPORT_C TInt WaitAndAssertL();

        /**
         * Forward the received events to the given event observer, if set.
         * Set to NULL to disable forwarding.
         * @param aObserver The observer 
         */
        IMPORT_C void SetEventObserver( MXIMPContextObserver* aObserver );

    public:  // from T_MWaitObserver

        void WaitCompleted( TInt aRetVal );

    private:  //From MXIMPContextObserver

        void HandlePresenceContextEvent(
                const MXIMPContext& aContext,
                const MXIMPBase& aEvent );

        void DoHandlePresenceContextEventL(
                const MXIMPContext& aContext,
                const MXIMPBase& aEvent );


    private: //Helpers

    private:  //Data

        //OWN: Active scheduler wait for waiting the completion
        CActiveSchedulerWait    iWait;

        //REF: Context from where receiving events
        MXIMPContext* iEventSource;

        //OWN: Description buffer to hold the failure description
        mutable RBuf8 iDescription;

        // To handle event by observer.
        MXIMPContextObserver* iObserver;

        // OWN: the event stack (and the objects)
        RXIMPObjOwningPtrArray< CXIMPApiObjBase > iEventStack;

        // current error state. WaitAndAssertL
        // always clears the error
        TInt iError;

        // number of received events
        TInt iReceivedEventCount;

        // ETrue if we're doing a "cool-down" period, that is,
        // we are waiting for any events which might come after
        // we've successfully exhausted our stack of expected events.
        // This is so that we can spot any excess events
        // during testing.
        TBool iCoolDownActive;
    };

#endif //CPRFWTESTLISTENER_H__


