/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Base and helper classes for subthread use in mediarecorder
*
*/


// INCLUDE FILES

#include <e32math.h>
#include "CCMRSubThread.h"
#include "CCMRAudioThreadProxy.h"



// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

// ========================= RCMRSubThreadBase MEMBER FUNCTIONS =========================


// -----------------------------------------------------------------------------
// RCMRSubThreadBase::ReceiveEvents
// Allows a client to receive events from the thread
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RCMRSubThreadBase::ReceiveEvents(TMMFEventPckg& aEventPckg, TRequestStatus& aStatus) const
    {
    SendReceive(ECMRThreadReceiveEvents, TIpcArgs(reinterpret_cast<TDes8*>(&aEventPckg)), aStatus);
    }

// -----------------------------------------------------------------------------
// RCMRSubThreadBase::CancelReceiveEvents
// Cancels reception of events from the thread
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRSubThreadBase::CancelReceiveEvents() const
    {
    return SendReceive(ECMRThreadCancelReceiveEvents);
    }

// -----------------------------------------------------------------------------
// RCMRSubThreadBase::Shutdown
// Shutdown the thread
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RCMRSubThreadBase::Shutdown()
    {
    PRINT((_L("RCMRSubThreadBase::Shutdown() in")));
    //close the subthread and wait for its exit.
    //signal to the subthread to close down.

    // logon to thread first
    TRequestStatus death;
    iSubThread.Logon(death);

    TInt shutdownError = 0;
    if ( iSessionCreated )
        {
        // a session was created with the thread; even if there is no session, the thread may still exist
        shutdownError = SendReceive(ECMRThreadShutdown);
        PRINT((_L("RCMRSubThreadBase::Shutdown() shutdown msg to session sent, error %d"),shutdownError));
        }

    if ( shutdownError == KErrNone )
        {
        //now wait for the death of the subthread...

        RTimer timer;
        TInt err = timer.CreateLocal();
        //if we managed to create the timer, wait for both the death and the timeout to minimise the risk of deadlock
        if ( !err )
            {
            TRequestStatus timeout;
            timer.After(timeout, iShutdownTimeout);
            User::WaitForRequest(death, timeout);
            if ( death == KRequestPending )
                {
                //we have timed out.  Panic the thread in debug mode, kill it in release mode.
                PRINT((_L("RCMRSubThreadBase::Shutdown() timeout in waiting for subthread to exit")));
                iSubThread.LogonCancel(death);
                User::WaitForRequest(death);
#ifdef _DEBUG   
                if ( iSessionCreated )
                    {
                    Panic(ECMRSubThreadPanicTimedOut);
                    }
                else 
                    {
                    iSubThread.Kill(KErrDied);
                    }
#else
                iSubThread.Kill(KErrDied);
#endif
                }
            else
                {
                //subthread has exited. Cancel the timer.
                PRINT((_L("RCMRSubThreadBase::Shutdown() subthread exited ok")));
                timer.Cancel();
                User::WaitForRequest(timeout);
                }
            }
        else
            {
            //we have no timer so we'll just have to risk the deadlock
            User::WaitForRequest(death);
            }
        timer.Close();
        }
    else
        {
        PRINT((_L("RCMRSubThreadBase::Shutdown() sending shutdown msg failed, cancel logon")));
        iSubThread.LogonCancel(death);
        User::WaitForRequest(death);
        }
    //close the handles to the subthread
    iSubThread.Close();
	// close our parent
	RHandleBase::Close(); 
    PRINT((_L("RCMRSubThreadBase::Shutdown() out")));
    }

// -----------------------------------------------------------------------------
// RCMRSubThreadBase::DoCreateSubThread
// Derived classes call this to start the thread
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRSubThreadBase::DoCreateSubThread(TThreadFunction aFunction, 
                                            const TVersion& aVersion, TBool& aThreadCreated)
    {
    TInt error = KErrNone;
    RServer2 server2;

    error = iSubThread.Create(
        KNullDesC, 
        aFunction,
        KDefaultStackSize, 
        &User::Heap(), 
        &server2, 
        EOwnerThread);

    if (error)
        {
        return error;
        }

	TRequestStatus rendezvous;
	iSubThread.Rendezvous(rendezvous);
	
	if (rendezvous!=KRequestPending)
        {
        // logon failed - server is not yet running, so cannot have terminated
		iSubThread.Kill(0);
		}
	else
		{
        // logon OK - start the thread
		iSubThread.Resume();
		}
	User::WaitForRequest(rendezvous); // wait for start or death
	
	if (rendezvous!=KErrNone)
        {
		iSubThread.Close();
		return rendezvous.Int();
		}

    aThreadCreated = ETrue;
    		
	return CreateSession( server2, aVersion);
    }

// -----------------------------------------------------------------------------
// RCMRSubThreadBase::Panic
// Panic the thread
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RCMRSubThreadBase::Panic(TCMRSubThreadPanicCode aPanicCode)
    {
    _LIT(KMMFSubThreadPanicCategory, "MMFSubThread");
    iSubThread.Panic(KMMFSubThreadPanicCategory, aPanicCode);
    }




// ==================== CCMRSubThreadEventMonitor MEMBER FUNCTIONS =======================


// -----------------------------------------------------------------------------
// CCMRSubThreadEventMonitor::CCMRSubThreadEventMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCMRSubThreadEventMonitor::CCMRSubThreadEventMonitor(MCMRSubThreadEventMonitorObserver& aObserver, 
                                                       RCMRSubThreadBase& aSubThreadProxy) :
    CActive(EPriorityStandard),
    iObserver(aObserver), 
    iSubThreadProxy(aSubThreadProxy)
    {
    CActiveScheduler::Add(this);
    }


// -----------------------------------------------------------------------------
// CCMRSubThreadEventMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCMRSubThreadEventMonitor* CCMRSubThreadEventMonitor::NewL(MCMRSubThreadEventMonitorObserver& aObserver, 
                                                             RCMRSubThreadBase& aSubThreadProxy)
    {
    return (new(ELeave) CCMRSubThreadEventMonitor(aObserver, aSubThreadProxy));
    }

// Destructor
CCMRSubThreadEventMonitor::~CCMRSubThreadEventMonitor()
    {
    Cancel();
    }

// -----------------------------------------------------------------------------
// CCMRSubThreadEventMonitor::Start
// Start the event monitor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRSubThreadEventMonitor::Start()
    {
    iSubThreadProxy.ReceiveEvents(iEventPckg, iStatus);
    SetActive();
    }

// -----------------------------------------------------------------------------
// CCMRSubThreadEventMonitor::RunL
// Active object main function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRSubThreadEventMonitor::RunL()
    {
    if (iStatus.Int() == KErrNone)
        {
        PRINT((_L("CCMRSubThreadEventMonitor::RunL() handle event %x"),iEventPckg().iEventType));
        iObserver.HandleEvent(iEventPckg());
        Start();
        }
    else
        {
        //something's gone wrong with trying to receive events (e.g. server died etc)
        PRINT((_L("CCMRSubThreadEventMonitor::RunL() event %x but there is a general error"),iEventPckg().iEventType));
        TMMFEvent event(KCMRErrorCategoryThreadGeneralError, iStatus.Int());
        iObserver.HandleEvent(event);
        //we don't want to receive events again here...
        }
    }

// -----------------------------------------------------------------------------
// CCMRSubThreadEventMonitor::DoCancel
// Cancel active object
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRSubThreadEventMonitor::DoCancel()
    {
    iSubThreadProxy.CancelReceiveEvents();
    }






// ======================= CCMRSubThreadShutdownTimer MEMBER FUNCTIONS ======================


// -----------------------------------------------------------------------------
// CCMRSubThreadShutdownTimer::CCMRSubThreadShutdownTimer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCMRSubThreadShutdownTimer::CCMRSubThreadShutdownTimer() : 
    CTimer(EPriorityLow)
    {
    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// CCMRSubThreadShutdownTimer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCMRSubThreadShutdownTimer* CCMRSubThreadShutdownTimer::NewL() 
    {
    CCMRSubThreadShutdownTimer* s = new(ELeave) CCMRSubThreadShutdownTimer();
    CleanupStack::PushL(s);
    s->ConstructL();
    CleanupStack::Pop();
    return s;
    }


// -----------------------------------------------------------------------------
// CCMRSubThreadShutdownTimer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCMRSubThreadShutdownTimer::ConstructL()
    {
    CTimer::ConstructL();
    }

// -----------------------------------------------------------------------------
// CCMRSubThreadShutdownTimer::Start
// Start timer for forced shutdown if closing takes too long
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRSubThreadShutdownTimer::Start()
    {
    After(ECMRSubThreadShutdownDelay);
    }

// -----------------------------------------------------------------------------
// CCMRSubThreadShutdownTimer::ShutdownNow
// Shutdown immediately
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRSubThreadShutdownTimer::ShutdownNow() const
    {
    CActiveScheduler::Stop();
    }

// -----------------------------------------------------------------------------
// CCMRSubThreadShutdownTimer::RunL
// Active object main function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRSubThreadShutdownTimer::RunL()
    {
    ShutdownNow();
    }







// ==================== CCMRSubThreadEventReceiver MEMBER FUNCTIONS =======================


// -----------------------------------------------------------------------------
// CCMRSubThreadEventReceiver::CCMRSubThreadEventReceiver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCMRSubThreadEventReceiver::CCMRSubThreadEventReceiver(const RMessage2& aMessage) : 
    iMessage(aMessage), iNeedToCompleteMessage(ETrue)
    {
    }

// -----------------------------------------------------------------------------
// CCMRSubThreadEventReceiver::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCMRSubThreadEventReceiver* CCMRSubThreadEventReceiver::NewL(const RMessage2& aMessage)
    {
    return new(ELeave) CCMRSubThreadEventReceiver(aMessage);
    }

// Destructor
CCMRSubThreadEventReceiver::~CCMRSubThreadEventReceiver()
    {
    if (iNeedToCompleteMessage)
        {
        iMessage.Complete(KErrDied);
        }
    }


// -----------------------------------------------------------------------------
// CCMRSubThreadEventReceiver::SendEvent
// Send an event via this class
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRSubThreadEventReceiver::SendEvent(const TMMFEvent& aEvent)
    {
    TMMFEventPckg eventpckg(aEvent);
    PRINT((_L("CCMRSubThreadEventReceiver::SendEvent() event %x"),aEvent.iEventType));
	TRAPD(err, iMessage.WriteL(0, eventpckg));
    iMessage.Complete(err);
    iNeedToCompleteMessage = EFalse;
    }












// ==================== CCMRThreadProxyServer MEMBER FUNCTIONS =======================



// -----------------------------------------------------------------------------
// CCMRThreadProxyServer::CCMRThreadProxyServer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCMRThreadProxyServer::CCMRThreadProxyServer(TInt aPriority) :
    CServer2(aPriority, CServer2::ESharableSessions)
    {
    }

// Destructor
CCMRThreadProxyServer::~CCMRThreadProxyServer()
    {
    delete iShutdownTimer;
    }



// -----------------------------------------------------------------------------
// CCMRThreadProxyServer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCMRThreadProxyServer::ConstructL(RServer2* aServer2)
    {
    StartL(KNullDesC);
    // store a reference to the given object, to be used in when creating session with the server
    *aServer2 = Server();
    
    iShutdownTimer = CCMRSubThreadShutdownTimer::NewL();
    iShutdownTimer->Start();
    }

// -----------------------------------------------------------------------------
// CCMRThreadProxyServer::SessionCreated
// Informs that session has been created successfully
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRThreadProxyServer::SessionCreated()
    {
    //stop the shutdown timer
    iShutdownTimer->Cancel();
    }

// -----------------------------------------------------------------------------
// CCMRThreadProxyServer::ShutdownNow
// Request to shutdown immediately
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRThreadProxyServer::ShutdownNow()
    {
    iShutdownTimer->ShutdownNow();
    }

// -----------------------------------------------------------------------------
// CCMRThreadProxyServer::RunError
// Handle errors in RunL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRThreadProxyServer::RunError(TInt aError)
    {
    //signal the client
    Message().Complete(aError);
    ReStart();
    return KErrNone;
    }

 



// ==================== CCMRThreadProxySession MEMBER FUNCTIONS =======================




// Destructor
CCMRThreadProxySession::~CCMRThreadProxySession()
    {
    PRINT((_L("CCMRThreadProxySession::~CCMRThreadProxySession() deleted iEventReceiver %x"),iEventReceiver));
    delete iEventReceiver;
    iEvents.Close();
    iServer = NULL;
    }

// -----------------------------------------------------------------------------
// CCMRThreadProxySession::CreateL
// Completes construction of this server-side client session object
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRThreadProxySession::CreateL()
    {
    iServer = static_cast<CCMRThreadProxyServer*>(const_cast<CServer2*>(Server()));
    iServer->SessionCreated();
    }


// -----------------------------------------------------------------------------
// CCMRThreadProxySession::SendEventToClient
// Send event to client
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRThreadProxySession::SendEventToClient(const TMMFEvent& aEvent)
    {
    TInt error = KErrNone;
    if ( iEventReceiver )
        {
        //send event to client now
        iEventReceiver->SendEvent(aEvent);
        PRINT((_L("CCMRThreadProxySession::SendEventToClient() sent event and deleted iEventReceiver %x"),iEventReceiver));
        delete iEventReceiver;
        iEventReceiver=NULL;
        error = KErrNone;
        }
    else
        {
        //queue the request for later
        PRINT((_L("CCMRThreadProxySession::SendEventToClient() queued event for later delivery")));
        TMMFEvent event(aEvent);
        //if we've exceeded the max number of cached messages, delete the first and append this one to the end
        if ( iEvents.Count() >= KCMRSubThreadMaxCachedMessages )
            {
            iEvents.Remove(0);
            }
        error = iEvents.Append(event);
        }

    return error;
    }

// -----------------------------------------------------------------------------
// CCMRThreadProxySession::ReceiveEventsL
// Client requests to receive events
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRThreadProxySession::ReceiveEventsL(const RMessage2& aMessage)
    {
    if (iEventReceiver)
        {
        PRINT((_L("CCMRThreadProxySession::ReceiveEventsL() iEventReceiver already exists")));
        User::Leave(KErrAlreadyExists);
        }
    iEventReceiver = CCMRSubThreadEventReceiver::NewL(aMessage);
    PRINT((_L("CCMRThreadProxySession::ReceiveEventsL() created iEventReceiver %x"),iEventReceiver));
    //send the next cached event (if any) to the client
    if ( iEvents.Count() > 0 )
        {
        TMMFEvent& event = iEvents[0];
        iEventReceiver->SendEvent(event);
        PRINT((_L("CCMRThreadProxySession::ReceiveEventsL() sent event and deleted iEventReceiver %x"),iEventReceiver));
        delete iEventReceiver;
        iEventReceiver=NULL;
        iEvents.Remove(0);
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CCMRThreadProxySession::CancelReceiveEvents
// Stop event receiver
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRThreadProxySession::CancelReceiveEvents()
    {
    PRINT((_L("CCMRThreadProxySession::CancelReceiveEvents() deleted iEventReceiver %x"),iEventReceiver));
    delete iEventReceiver;
    iEventReceiver = NULL;
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRThreadProxySession::ShutDown
// Shutdown the server
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRThreadProxySession::ShutDown()
    {
    iServer->ShutdownNow();
    return ETrue;
    }
