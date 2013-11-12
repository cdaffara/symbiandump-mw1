/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Classes for subthread handling in media recorder
*
*/

#ifndef CCMRSUBTHREAD_H
#define CCMRSUBTHREAD_H

// INCLUDES

//#include <mmf/server/mmfdatapathproxy.h>
#include <mmf/common/mmfcontrollerframework.h>

// DATA TYPES

/**
* Thread messages ids
*/ 
enum TCMRSubThreadMessageIds
    {
    ECMRThreadReceiveEvents,
    ECMRThreadCancelReceiveEvents,
    ECMRThreadShutdown,
    ECMRThreadLastCommonMsgId // dummy id needed when combining audio/video ids with this
    };

/** An attempt to close the subthread using RCMRSubThreadBase::Shutdown() has failed. The shutdown command
* waits for both the death of the thread and the time-out of a timer to minimise the risk of deadlock.
* If the timer times-out and the sub-thread is not dead then this thread is panicked in debug mode or,
* in release mode the sub-thread is killed.
*/
enum TCMRSubThreadPanicCode
	{
	ECMRSubThreadPanicTimedOut = 1
	};

/**
* The amount of time that is allowed for the thread to close down before the its thread is killed.
*/
#define  KCMRThreadProxyShutdownTimeout TTimeIntervalMicroSeconds32(10000000)

/**
* Thread's general error code used when sending events to clients; the same is used in MMF
*/
const TUid KCMRErrorCategoryThreadGeneralError = {0x101F76DC};
/**
* Defines the maximum number of event messages that will be held server-side
* while waiting for the client to request the next message in the queue.
*/
static const TInt KCMRSubThreadMaxCachedMessages = 4;


//CLASS DECLARATIONS

/**
*  Media recorder thread base class
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class RCMRSubThreadBase : public RSessionBase
    {
    public: // Constructor

        /**
        * C++ default constructor.
        */
        inline RCMRSubThreadBase(const TTimeIntervalMicroSeconds32& aShutdownTimeout) 
            : RSessionBase(), iSubThread(), iShutdownTimeout(aShutdownTimeout), iSessionCreated(EFalse) {};

    public: // New functions
        /**                     
        * Return the thread id allowing a client to logon to the thread to receive notifications.
        * @since 2.1
        * @return TThreadId id
        */
        TThreadId SubThreadId() const {return iSubThread.Id();};

        /**                     
        * Allows a client to receive events from the thread
        * @since 2.1
        * @param TMMFEventPckg& aEventPckg
        * @param TRequestStatus& aStatus
        * @return void
        */
        void ReceiveEvents(TMMFEventPckg& aEventPckg, TRequestStatus& aStatus) const;

        /**                     
        * Cancels reception of events from the thread
        * @since 2.1
        * @return TInt error code
        */
        TInt CancelReceiveEvents() const;

        /**                     
        * Shutdown the thread
        * Note: This function will not return until the subthread has exited, or a timeout has occurred.
        * @since 2.1
        * @return void
        */
        void Shutdown();

    protected:  // New functions
        /**                     
        * Derived classes call this to start the thread & create session with it
        * @since 3.0
        * @param TThreadFunction aFunction
        * @param const TVersion& aVersion
        * @param TBool& aThreadCreated
        * @return TInt error code
        */
        TInt DoCreateSubThread(TThreadFunction aFunction, const TVersion& aVersion, TBool& aThreadCreated);


        /**                     
        * Panic the thread
        * @since 2.1
        * @param TCMRSubThreadPanicCode aPanicCode
        * @return void
        */
        void Panic(TCMRSubThreadPanicCode aPanicCode);

    protected:  //data
        // handle of the thread
        RThread iSubThread;

        // shutdown timeout
        TTimeIntervalMicroSeconds32 iShutdownTimeout;

        // ETrue if session was created successfully (can send messages)
        TBool iSessionCreated;

    };


/**
*  Media recorder thread event monitor observer
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class MCMRSubThreadEventMonitorObserver
    {
    public: 
        /**                     
        * Handle the given event
        * @since 2.1
        * @param const TMMFEvent& aEvent
        * @return void
        */
        virtual void HandleEvent(const TMMFEvent& aEvent) = 0;
    };


/**
*  Media recorder thread event monitor
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class CCMRSubThreadEventMonitor : public CActive
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCMRSubThreadEventMonitor* NewL(MCMRSubThreadEventMonitorObserver& aObserver, 
            RCMRSubThreadBase& aSubThreadProxy);

        /**
        * Destructor.
        */
        virtual ~CCMRSubThreadEventMonitor();

    public: // New functions
        /**                     
        * Start the event monitor
        * @since 2.1
        * @return void
        */
        void Start();

    protected: // from CActive

        /**
        * From CActive active object main function
        */
        void RunL();
        /**
        * From CActive cancel active object
        */
        void DoCancel();

    private: // constructor

        /**
        * C++ default constructor.
        */
        CCMRSubThreadEventMonitor(MCMRSubThreadEventMonitorObserver& aObserver, 
            RCMRSubThreadBase& aSubThreadProxy);

    private: // Data
        // event monitor observer
        MCMRSubThreadEventMonitorObserver& iObserver;
        // sub thread proxy
        RCMRSubThreadBase& iSubThreadProxy;
        // events 
        TMMFEventPckg iEventPckg;
    };

/**
*  Media recorder thread shutdown timer class
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class CCMRSubThreadShutdownTimer : public CTimer
    {
    enum {ECMRSubThreadShutdownDelay=5000000}; // 5s

    public: // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CCMRSubThreadShutdownTimer* NewL();

    public: // New functions

        /**                     
        * Start timer for forced shutdown if closing takes too long
        * @since 2.1
        * @return void
        */
        void Start();

        /**                     
        * Shutdown immediately
        * @since 2.1
        * @return void
        */
        void ShutdownNow() const;

    protected: // from CTimer
        /**
        * From CTimer/CActive active object main function
        */
        void RunL();

    private: // constructors
        /**
        * C++ default constructor.
        */
        CCMRSubThreadShutdownTimer();

        /**
        * Symbian 2nd phase constructor
        */
        void ConstructL();

    };


/**
*  Media recorder thread event receiver class
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class CCMRSubThreadEventReceiver : public CBase
    {
    public: // constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CCMRSubThreadEventReceiver* NewL(const RMessage2& aMessage);
        /**
        * Destructor.
        */
        virtual ~CCMRSubThreadEventReceiver();

    public: // new functions
        /**                     
        * Send an event via this class
        * @since 2.1
        * @param const TMMFEvent& aEvent
        * @return void
        */
        void SendEvent(const TMMFEvent& aEvent);

    private:
        /**
        * C++ default constructor.
        */
        CCMRSubThreadEventReceiver(const RMessage2& aMessage);

    private: // data

        // Message where event is put
        RMessage2 iMessage;
        // ETrue if message not completed
        TBool iNeedToCompleteMessage;
    };



/**
*  Media recorder thread proxy server base class
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class CCMRThreadProxyServer : public CServer2
    {
    public: // Constructors and destructor
        /**
        * C++ default constructor.
        */
        CCMRThreadProxyServer(TInt aPriority);
        /**
        * Destructor.
        */
        virtual ~CCMRThreadProxyServer();

    public: // New functions

        /**                     
        * Informs that session has been created successfully
        * @since 2.1
        * @return void
        */
        virtual void SessionCreated();

        /**                     
        * Request to shutdown immediately
        * @since 2.1
        * @return void
        */
        virtual void ShutdownNow();

    protected:
        /**
        * Symbian 2nd phase constructor
        */
        void ConstructL(RServer2* aServer2);

    protected: // from CServer2
        /**                     
        * From CServer2/CActive handle errors in RunL
        */
        TInt RunError(TInt aError);

        /**                     
        * From CServer2 create a new session. Each derived class must implement this.
        */
        CSession2* NewSessionL(const TVersion& aVersion, const RMessage2& aMessage) const = 0;

    private: // data
        // shutdown timer, needed if closing takes too much time
        CCMRSubThreadShutdownTimer* iShutdownTimer;
    };




/**
*  Media recorder thread proxy session base class
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class CCMRThreadProxySession : public CSession2, public MAsyncEventHandler
    {
    public: // Constructors and destructor
        /**
        * Destructor.
        */
        virtual ~CCMRThreadProxySession();

    public: // from base classes

        /**                     
        * From CSession2 Completes construction of this server-side client session object
        */
        void CreateL();
        /**                     
        * From CSession2 service a message. Each derived class must implement this.
        */
        virtual void ServiceL(const RMessage2& aMessage) = 0;

        /**                     
        * From MAsyncEventHandler send event to client
        */
        virtual TInt SendEventToClient(const TMMFEvent& aEvent);

    protected:
        /**
        * C++ default constructor.
        */
        inline CCMRThreadProxySession() {};

    protected: // New functions
        /**                     
        * Client requests to receive events
        * @since 2.1
        *
        * @return TBool
        */
        virtual TBool ReceiveEventsL(const RMessage2& aMessage);
        /**                     
        * Stop event receiver
        * @since 2.1
        * @return TBool
        */
        virtual TBool CancelReceiveEvents();
        /**                     
        * Shutdown the server
        * @since 2.1
        * @return TBool
        */
        virtual TBool ShutDown();

    protected: // data
        // server connected to the session
        CCMRThreadProxyServer* iServer;

    private: // data
        // event receiver
        CCMRSubThreadEventReceiver* iEventReceiver;

        // cached events
        RArray<TMMFEvent> iEvents;
    };

#endif      
            
// End of File



