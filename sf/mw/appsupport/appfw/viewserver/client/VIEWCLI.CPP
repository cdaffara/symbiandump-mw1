// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#include <e32svr.h>

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include "vwsinternal.h"
#include "viewclipartner.h"
#include "vwsdefpartner.h"
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS

#include "VIEWCLI.H"
#include "VWSPRIV.H"
#include "VWSAPPST.H"
#include "VWSERVER.H"


//
// Constants.
//

const TUint KDefaultMessageSlots=5;
const TInt KVwsDefaultScreenDeviceMode=0;
const TInt KVwsServerStackSize=KDefaultStackSize;
const TInt KVwsMinHeapSize=0x2000; 
const TInt KVwsMaxHeapSize=0x1000000; 

_LIT(KVwsViewServerThreadName,"ViewServerThread");
_LIT(KVwsViewServerName,"!ViewServer");


//
// Macros.
//

#define CONNECT_AS_REQUIRED	TInt err=CheckCreateViewServerSession();	if (err) {return err;}


//
// Global functions.
//



//
// RVwsSession.
//

RVwsSession::RVwsSession()
	{
	}

TInt RVwsSession::Connect()
	{
	TInt ret=CreateSession(KVwsViewServerName,Version(),KDefaultMessageSlots);
	if (ret == KErrNone)
		SendReceive(EVwsAsynchronousMessageForServerToPanicClientWith,TIpcArgs(), iPanicStatus);
	return ret;
	}

TVersion RVwsSession::Version() const
	{
	return(TVersion(KVwsMajorVersionNumber,KVwsMinorVersionNumber,KVwsBuildVersionNumber));
	}

TInt RVwsSession::Shutdown() const
	{
	return SendReceive(EVwsClose,TIpcArgs());
	}

TInt RVwsSession::AddView(const TVwsViewId& aViewId) const
	{
	const TPckgC<TVwsViewId> viewId(aViewId);
	return SendReceive(EVwsAddView,TIpcArgs(&viewId));
	}

TInt RVwsSession::SetClientRequestTimeOut(TTimeIntervalMicroSeconds32 aDuration) const
	{
	return SendReceive(EVwsSetClientRequestTimeOut,TIpcArgs(aDuration.Int()));
	}

TInt RVwsSession::SetServerEventTimeOut(TTimeIntervalMicroSeconds32 aDuration) const
	{
	return SendReceive(EVwsSetServerEventTimeOut,TIpcArgs(aDuration.Int()));
	}

TInt RVwsSession::EnableServerEventTimeOut(TBool aEnable) const
	{
	return SendReceive(EVwsEnableServerEventTimeOut,TIpcArgs(aEnable));
	}

TInt RVwsSession::SetSystemDefaultView(const TVwsViewId& aViewId,TInt aMode) const
	{
	const TPckgC<TVwsViewId> viewId(aViewId);
	return SendReceive(EVwsSetSystemDefaultView,TIpcArgs(&viewId,aMode));
	}

TInt RVwsSession::GetSystemDefaultView(TVwsViewId& aViewId)
	{
	TPckg<TVwsViewId> viewId(aViewId);
	return SendReceive(EVwsGetSystemDefaultView,TIpcArgs(&viewId));
	}

TInt RVwsSession::RemoveView(const TVwsViewId& aViewId) const
	{
	const TPckgC<TVwsViewId> viewId(aViewId);
	return SendReceive(EVwsRemoveView,TIpcArgs(&viewId));
	}

void RVwsSession::RequestViewEvent(TVwsViewEventBuf& aViewEventBuf,TInt aLastError,TRequestStatus& aStatus) const
	{
	SendReceive(EVwsRequestViewEvent,TIpcArgs(&aViewEventBuf,aLastError,0,0),aStatus);
	}
	
	void RVwsSession::RequestViewEvent(TVwsViewEventBuf& aViewEventBuf,TInt aLastError,const TVwsViewIdAndMessageBuf& aViewActivationBuf,const TDesC8& aMessage,TRequestStatus& aStatus) const
	{
	SendReceive(EVwsRequestViewEvent,TIpcArgs(&aViewEventBuf,aLastError,&aViewActivationBuf,&aMessage),aStatus);
	}

TInt RVwsSession::RequestViewEventCancel() const
	{
	return SendReceive(EVwsRequestViewEventCancel,TIpcArgs());
	}

void RVwsSession::ActivateView(const TVwsViewId& aViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage,TRequestStatus& aStatus, TInt aCustomControl)
	{
	iActivateViewId=aViewId;
	iCustomMessage.Set(aCustomMessage);
	SendReceive(EVwsActivateView,TIpcArgs(&iActivateViewId,aCustomMessageId.iUid,&iCustomMessage,aCustomControl),aStatus);
	}

TInt RVwsSession::ActivateView(const TVwsViewId& aViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage, TInt aCustomControl)
	{
	const TPckgC<TVwsViewId> viewId(aViewId);
	return SendReceive(EVwsCreateActivateViewEvent,TIpcArgs(&viewId,aCustomMessageId.iUid,&aCustomMessage,aCustomControl));
	}

TInt RVwsSession::RequestCustomMessage(TDes8& aMessageBuf) const
	{
	return SendReceive(EVwsRequestCustomMessage,TIpcArgs(&aMessageBuf));
	}

TInt RVwsSession::StartApp(TUid aAppToStart) const
	{
	return SendReceive(EVwsStartApp,TIpcArgs(aAppToStart.iUid));
	}

void RVwsSession::DeactivateActiveView(TRequestStatus& aStatus) const
	{
	SendReceive(EVwsDeactivateActiveView,TIpcArgs(),aStatus);
	}

TInt RVwsSession::DeactivateActiveView() const
	{
	return SendReceive(EVwsCreateDeactivateViewEvent,TIpcArgs());
	}
void RVwsSession::DeactivateActiveViewIfOwnerMatch(TRequestStatus& aStatus) const
 	{
 	SendReceive(EVwsDeactivateActiveViewIfOwnerMatch,TIpcArgs(), aStatus);
 	}
 
TInt RVwsSession::DeactivateActiveViewIfOwnerMatch() const
 	{
 	return SendReceive(EVwsDeactivateActiveViewIfOwnerMatch,TIpcArgs());
 	}
 


TInt RVwsSession::NotifyNextDeactivation(const TVwsViewId& aViewId) const
	{
	const TPckgC<TVwsViewId> viewId(aViewId);
	return SendReceive(EVwsNotifyNextDeactivation,TIpcArgs(&viewId));
	}

TInt RVwsSession::NotifyNextActivation(const TVwsViewId& aViewId) const
	{
	const TPckgC<TVwsViewId> viewId(aViewId);
	return SendReceive(EVwsNotifyNextActivation,TIpcArgs(&viewId));
	}

TInt RVwsSession::GetPriority()	
	{
	TInt eventHandlerPriority = CActive::EPriorityStandard;
	TPckg<TInt> package(eventHandlerPriority);
	TInt err = SendReceive(EVwsPriority,TIpcArgs(&package));
	if (err != KErrNone)
		{
		return CActive::EPriorityStandard;
		}
	return eventHandlerPriority;
	}
 
TInt RVwsSession::CheckSourceOfViewSwitch(TBool& aResult,const TSecurityPolicy& aSecurityPolicy,const char* aDiagnostic) const
	{
	const TPckgC<TSecurityPolicy> securityPolicy(aSecurityPolicy);
	TBool result;
	TPckg<TBool> resultPckg(result);
	TIpcArgs ipcArgs(&securityPolicy,NULL,&resultPckg);
	if (aDiagnostic == KSuppressPlatSecDiagnosticMagicValue)
    	{
    	ipcArgs.Set(1, reinterpret_cast<TInt>(KSuppressPlatSecDiagnosticMagicValue));
    	}
  	else if (aDiagnostic!=NULL)
	    	{
	    	TPtrC8 diagnostic(TPtrC8(REINTERPRET_CAST(const TUint8*,aDiagnostic)));
	    	ipcArgs.Set(1,&diagnostic);
	    	}

	const TInt ret=SendReceive(EVwsCheckSourceOfViewSwitch,ipcArgs);
	if (ret==KErrNone)
		{
		aResult=result;
		}
	return ret;
	}

TInt RVwsSession::EnableServerBlankScreen(TBool aEnable) const
	{
	return SendReceive(EVwsEnableServerBlankScreen,TIpcArgs(aEnable));
	}

TInt RVwsSession::SetProtected(TBool aEnable) const
	{
	return SendReceive(EVwsSetProtected,TIpcArgs(aEnable));
	}

TInt RVwsSession::SetCrossCheckUid(const TUid& aCrossCheckUid)
	{
	return SendReceive(EVwsSetCrossCheckUid,TIpcArgs(aCrossCheckUid.iUid));
	}
	
TInt RVwsSession::SetWindowBackgroundColor(const TRgb& aBgColor)
	{
	return SendReceive(EVwsSetWindowBackgroundColor, TIpcArgs(aBgColor.Internal()));
	}

TInt RVwsSession::GetCurrentActiveViewInSystem(TVwsViewId& aActiveViewId)
	{
	TPckg<TVwsViewId> pckgViewId(aActiveViewId);
	return SendReceive(EVwsCurrentActiveViewId, TIpcArgs(&pckgViewId));
	}

//
// CVwsSessionEventHandler.
//
//The priority is read from the resource file

CVwsSessionEventHandler::CVwsSessionEventHandler(MVwsSessionWrapperObserver& aObserver,RVwsSession& aViewServer,TInt aPriority)
	: CActive(aPriority),iObserver(aObserver),iVwsSession(aViewServer)
 	{
	CActiveScheduler::Add(this);
	Queue();
	}

CVwsSessionEventHandler::~CVwsSessionEventHandler()
	{
	Cancel();
	}

void CVwsSessionEventHandler::Queue()
	{
	ASSERT(!IsActive());

	if (iDoActivationNextQueue)
		{
		iDoActivationNextQueue=EFalse;
		iVwsSession.RequestViewEvent(iViewEventBuf,iLastError,iViewActivationBuf,iCustomMessage,iStatus);
		}
	else
		{
		iVwsSession.RequestViewEvent(iViewEventBuf,iLastError,iStatus);
		}
	SetActive();
	}

void CVwsSessionEventHandler::OnNextQueueActivateView(const TVwsViewIdAndMessage& aViewIdAndMessage)
	{
	iViewActivationBuf=aViewIdAndMessage;
	iCustomMessage.Set(aViewIdAndMessage.iCustomMessage);
	iDoActivationNextQueue=ETrue;
	}

void CVwsSessionEventHandler::RunL()
	{
	TRAP(iLastError,iObserver.HandleViewEventL(iViewEventBuf()));
	Queue();
	}

void CVwsSessionEventHandler::DoRunL()
	{
	RunL();
	}

void CVwsSessionEventHandler::DoCancel()
	{
	iVwsSession.RequestViewEventCancel();
	}


//
// CVwsSessionWrapper.
//


EXPORT_C CVwsSessionWrapper* CVwsSessionWrapper::NewL()
/**
Creates a new CVwsSessionWrapper object.

The object returned by this function cannot be used to add new views or request 
notification of view activations or deactivations. To create an instance without these
limitations, an MVwsSessionWrapperObserver must be specified.

@return A pointer to the new CVwsSessionWrapper object.
 */
	{
	CVwsSessionWrapper* self=NewLC();
	CleanupStack::Pop();
	return self;
	}

EXPORT_C CVwsSessionWrapper* CVwsSessionWrapper::NewLC()
/**
Creates a new CVwsSessionWrapper object, leaving it on the cleanup stack

The object returned by this function cannot be used to add new views or request 
notification of view activations or deactivations. To create an instance without these
limitations, an MVwsSessionWrapperObserver must be specified.

@return A pointer to the new CVwsSessionWrapper object.
 */
	{
	CVwsSessionWrapper* self=new(ELeave) CVwsSessionWrapper();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

EXPORT_C CVwsSessionWrapper* CVwsSessionWrapper::NewL(MVwsSessionWrapperObserver& aObserver)
/**
Creates a new CVwsSessionWrapper object.

@param aObserver An implementation of MVwsSessionWrapperObserver.
                 The session observer should handle server events such as view
                 activation/deactivation requests for views owned by this session
                 and notifications of view activations/deactivations that have been
                 requested.

@return A pointer to the new CVwsSessionWrapper object.
 */
	{
	CVwsSessionWrapper* self=NewLC(aObserver);
	CleanupStack::Pop();
	return self;
	}

EXPORT_C CVwsSessionWrapper* CVwsSessionWrapper::NewLC(MVwsSessionWrapperObserver& aObserver)
/**
Creates a new CVwsSessionWrapper object, leaving it on the cleanup stack.

@param aObserver An implementation of MVwsSessionWrapperObserver.
                 The session observer should handle server events such as view
                 activation/deactivation requests for views owned by this session
                 and notifications of view activations/deactivations that have been
                 requested.

@return A pointer to the new CVwsSessionWrapper object.
 */
	{
	CVwsSessionWrapper* self=new(ELeave) CVwsSessionWrapper(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

EXPORT_C CVwsSessionWrapper::~CVwsSessionWrapper()
	{
	delete iViewEventHandler;
	if (iVwsSession)
		{
		iVwsSession->Close();
		delete iVwsSession;
		}
	}

// static
EXPORT_C TInt CVwsSessionWrapper::StartViewServer(MVwsAppStarter& aAppStarter)
/**
Starts the view server, spawning a new thread in which the server will run.
 
Takes a reference to the implementation of the MVwsAppStarter interface aAppStarter
which it will share from the heap and which should be for the exclusive use of the view
server only.

@param aAppStarer An implementation of MVwsAppStarter that the server can use for
                  starting applications.

@return A system-wide error code if the server cannot be started or is already running.
 */
	{
	TFindServer findServer(KVwsViewServerName);
	TFullName name;
	if (findServer.Next(name)==KErrNone)
	    {
	    return KErrAlreadyExists;
	    }

	RThread viewServerThread;
	SVwsCommandLine comLine;
	comLine.iAppStarter=&aAppStarter;
    TInt err = viewServerThread.Create(KVwsViewServerThreadName,ViewServerThreadStart,KVwsServerStackSize,KVwsMinHeapSize,KVwsMaxHeapSize,&comLine,EOwnerThread);
	if (err==KErrNone)
        {
		TRequestStatus requestStatus;
		viewServerThread.Rendezvous(requestStatus);

		viewServerThread.Resume();
		
		User::WaitForRequest(requestStatus);
		err=requestStatus.Int();
		viewServerThread.Close();
		}

	return err;
	}

EXPORT_C TInt CVwsSessionWrapper::ShutdownViewServer()
/**
Shut down the view server. 

Establishes a connection to the view server if necessary.

@return KErrNone if successful, otherwise one of the system-wide error codes.
 */
	{
	CONNECT_AS_REQUIRED
	return iVwsSession->Shutdown();
	}

EXPORT_C TInt CVwsSessionWrapper::AddView(const TVwsViewId& aViewId)
/**
Adds the view to the list maintained by the view server.

Establishes a connection to the view server if necessary.

@param aViewId The view to be added to the view server's list.

@return KErrNone if successful, otherwise one of the system-wide error codes.
 */	
	{
	CONNECT_AS_REQUIRED
	return iVwsSession->AddView(aViewId);
	}

EXPORT_C TInt CVwsSessionWrapper::RemoveView(const TVwsViewId& aViewId) const
/**
Removes a view from the list maintained by the view server.

@param aViewId The view to be removed from the view server's list.

@return KErrNone if successful, otherwise one of the system-wide error codes.

@panic USER 0 if a connection to the view server has not previously been established.
 */		
	{
	ASSERT(iVwsSession);
	return iVwsSession->RemoveView(aViewId);
	}

EXPORT_C TInt CVwsSessionWrapper::SetSystemDefaultView(const TVwsViewId& aViewId,TInt aMode)
/**
Sets the system default view for a specific screen device mode.

The system default view is the view used when an unrecoverable error in view activation occurs
or when an application exits.

Establishes a connection to the view server if necessary.

@param aViewId The view to use as the system default view in the specified
               screen device mode
@param aMode The screen device mode that should use aViewId as the system
             default view. Valid values for aMode are those listed in the Wserv
             initialization file.
             
@return KErrNone if successful, otherwise one of the system-wide error codes.
 */
	{
	CONNECT_AS_REQUIRED
	return iVwsSession->SetSystemDefaultView(aViewId,aMode);
	}

EXPORT_C TInt CVwsSessionWrapper::SetSystemDefaultView(const TVwsViewId& aViewId)
/**
Sets the system default view for the default screen device mode.

Establishes a connection to the view server if necessary.

@param aViewId The view to use as the system default view in the default screen
               device mode
             
@return KErrNone if successful, otherwise one of the system-wide error codes.
 */
	{
	return SetSystemDefaultView(aViewId,KVwsDefaultScreenDeviceMode);
	}

EXPORT_C TInt CVwsSessionWrapper::GetSystemDefaultView(TVwsViewId& aViewId)
/**
Gets the system default view for the current screen device mode

@param aViewId The ViewId of the system default view will be returned in this
               paramater.

@return KErrNone if successful, otherwise one of the system-wide error codes.
 */
	{
	return iVwsSession->GetSystemDefaultView(aViewId);
	}

EXPORT_C TInt CVwsSessionWrapper::SetClientRequestTimeOut(TTimeIntervalMicroSeconds32 aDuration)
/**
Sets the time out duration for client requests.

If the time taken to process a view event exceeds this limit any outstanding
client request will silently be completed.

Establishes a connection to the view server if necessary.

@param aDuration The duration of the timeout, in microseconds.

@return KErrNone if successful, otherwise one of the system-wide error codes.

@since App-Framework_6.1
 */
	{
	CONNECT_AS_REQUIRED
	return iVwsSession->SetClientRequestTimeOut(aDuration);
	}

EXPORT_C TInt CVwsSessionWrapper::SetServerEventTimeOut(TTimeIntervalMicroSeconds32 aDuration)
/**
Sets the time out duration for server events.

If the time taken to process a view event exceeds this limit the server event
will be completed. If a client is preventing the view event from completing
the server will panic that client.

Establishes a connection to the view server if necessary.

@param aDuration The duration of the timeout, in microseconds.

@return KErrNone if successful, otherwise one of the system-wide error codes.

@since App-Framework_6.1
 */
	{
	CONNECT_AS_REQUIRED
	return iVwsSession->SetServerEventTimeOut(aDuration);
	}

EXPORT_C TInt CVwsSessionWrapper::EnableServerEventTimeOut(TBool aEnable)
/**
Enables or disables the server event time out.

This may be called, for example, when debugging on target hardware since server
event processing can be interrupted indefinitely by the debugger.

Establishes a connection to the view server if necessary.

@param aEnable The timeout is enabled if aEnable is set to ETrue, and disabled
               if aEnable is set to EFalse

@return KErrNone if successful, otherwise one of the system-wide error codes.

@since App-Framework_6.1
 */
	{
	CONNECT_AS_REQUIRED
	return iVwsSession->EnableServerEventTimeOut(aEnable);
	}

/**
Sets the session default view for the screen device mode aMode to aViewId.
Valid values for aMode are those listed in the Wserv initialisation file.

As of R6 this function has no effect.

@deprecated

@return KErrNone.
 */
EXPORT_C TInt CVwsSessionWrapper::SetDefaultView(const TVwsViewId& /*aViewId*/,TInt /*aMode*/) const
	{
	return KErrNone;
	}

EXPORT_C TInt CVwsSessionWrapper::SetDefaultView(const TVwsViewId& aViewId) const
/**
Overloaded member funtion that sets the session default view for the
default screen device mode to aViewId.

As of R6 this funtion has no effect.

@deprecated

@return KErrNone.
 */
	{
	return SetDefaultView(aViewId,KVwsDefaultScreenDeviceMode);
	}

EXPORT_C TInt CVwsSessionWrapper::ActivateView(const TVwsViewId& aViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage)
/**
Activates a view.

Supplies the narrow message text descriptor aCustomMessage for the message type
identified by aCustomMessageId to the server.

Establishes a connection to the view server if necessary.
Starts the application which owns the view if necessary.

Once a new view is activated the current view is deactivated and the new view
is marked as current.

If this thread has an active environment the method returns when view
activation completes. Otherwise the method returns
immediately after creating an activation event in the view server.

@param aViewId The view to activate
@param aCustomMessageId The message type being passed in aCustomMessage.
@param aCustomMessage  A custom message for the view being activated.

@return KErrNone if successful, otherwise one of the system-wide error codes.
 */			
	{
	return ActivateView(aViewId, aCustomMessageId, aCustomMessage, 0);
	}

EXPORT_C TInt CVwsSessionWrapper::ActivateView(const TVwsViewId& aViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage, TInt aCustomControl)
	{
	CONNECT_AS_REQUIRED

	if (!IsSchedulerRunning())
		{
		return iVwsSession->ActivateView(aViewId,aCustomMessageId,aCustomMessage, aCustomControl);
		}
	else
		{
		TRequestStatus status;
		iVwsSession->ActivateView(aViewId,aCustomMessageId,aCustomMessage,status, aCustomControl);
		TInt error=KErrNone;
		for (;;)
			{
			if (iViewEventHandler)
				{
				User::WaitForRequest(status, iViewEventHandler->iStatus);
				}
			else
				{
				User::WaitForRequest(status);
				}
			if (status!=KRequestPending)
				break;
			if (iViewEventHandler && iViewEventHandler->iStatus!=KRequestPending)
				{
				// Set the event handler active object to inactive to avoid it running
				// a second time if the DoRunL call results in a new active scheduler
				// loop being started.
				TRequestStatus* eventHandlerStatus = &iViewEventHandler->iStatus;
				User::RequestComplete(eventHandlerStatus, iViewEventHandler->iStatus.Int());
				iViewEventHandler->Cancel();
				TRAP(error,iViewEventHandler->DoRunL(););	// Run event handler directly and request new view event.
				if (error != KErrNone)
					{
					break;
					}
				}
			}

		return (error != KErrNone) ? error : status.Int();
		}
	}

EXPORT_C void CVwsSessionWrapper::ActivateView(
									const TVwsViewId& aViewId,
									TUid aCustomMessageId,
									const TDesC8& aCustomMessage,
									TRequestStatus& aStatus)
/**
Activates a view asynchronously.

Supplies the narrow message text descriptor aCustomMessage for the message type
identified by aCustomMessageId to the server.

Establishes a connection to the view server if necessary.
Starts the application which owns the view if necessary.

Once a new view is activated the current view is deactivated and the new view
is marked as current.

This function will return immediately

@param aViewId The view to activate
@param aCustomMessageId The message type being passed in aCustomMessage.
@param aCustomMessage  A custom message for the view being activated.
@param aStatus A TRequestStatus for the view activation. The request will be
               completed when the the view has been activated or when the
               view activation has failed.

@return KErrNone if the request was dispatched successfully, otherwise one of
        the system-wide error codes.
 */
	{
	ActivateView(aViewId, aCustomMessageId, aCustomMessage, aStatus, 0);
	}
	
EXPORT_C void CVwsSessionWrapper::ActivateView(
									const TVwsViewId& aViewId,
									TUid aCustomMessageId,
									const TDesC8& aCustomMessage,
									TRequestStatus& aStatus, TInt aCustomControl)
	{
	TInt res = CheckCreateViewServerSession();
	if (res != KErrNone)
		{
		TRequestStatus* status = &aStatus;
		User::RequestComplete(status, res);
		return;
		}
	iVwsSession->ActivateView(aViewId, aCustomMessageId, aCustomMessage, aStatus, aCustomControl);
	}

EXPORT_C TInt CVwsSessionWrapper::CreateActivateViewEvent(const TVwsViewId& aViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage)
/**
Creates an event to be processed to activate the view identified by aViewId and
returns without waiting for the activation to complete. Passes the view the
message text descriptor aCustomMessage for a message of type aCustomMessageId.

Note that this method may return without error but the activation may subsequently fail.

Establishes a connection to the view server if necessary.

@param aViewId The view to activate
@param aCustomMessageId The message type being passed in aCustomMessage.
@param aCustomMessage  A custom message for the view being activated.

@return KErrNone if the request was dispatched successfully, otherwise one of
        the system-wide error codes.
 */
	{
	return CreateActivateViewEvent(aViewId, aCustomMessageId, aCustomMessage, 0);
	}

EXPORT_C TInt CVwsSessionWrapper::CreateActivateViewEvent(const TVwsViewId& aViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage, TInt aCustomControl)
	{
	CONNECT_AS_REQUIRED

	return iVwsSession->ActivateView(aViewId, aCustomMessageId, aCustomMessage, aCustomControl);
	}

EXPORT_C TInt CVwsSessionWrapper::ActivateViewViaViewEvent(const TVwsViewIdAndMessage& aViewIdAndMessage)
/**
Activates the view identified in aViewIdAndMessage on the next queue of the
session's view event handler. This activation path should be used for
activation requests that come via view events.

For example, a screen device change will generate a view event in the active
view. The active view is then responsible for choosing the next view to
activate. Using this activation path ensures that the view activation is
processed immediately by the server.

Establishes a connection to the view server if necessary.

@param aViewIdAndMessage An object encapsulating the view to activate and
                         the message to be passed to it with the activation
                         event

@return KErrNone
*/
	{
	CONNECT_AS_REQUIRED

	if (iViewEventHandler)
		{
		iViewEventHandler->OnNextQueueActivateView(aViewIdAndMessage);
		}
	return KErrNone;
	}

/**
Retrives the custom message contents for the event currently being processsed
from the server.

This method is typically called when the MVwsSessionObserver is notified that
it should activate a new view.

@param aMessageBufPtr The narrow descriptor for the custom message contents to
                      be written to.
 
@return KErrNone if successful, otherwise one of the system-wide error codes.

@panic USER 0 If a connection to the view server has not previously been established
 */
EXPORT_C TInt CVwsSessionWrapper::RequestCustomMessage(TDes8& aMessageBufPtr) const
	{
	ASSERT(iVwsSession);
	return iVwsSession->RequestCustomMessage(aMessageBufPtr);
	}

/**
Queues an asynchronous view server event request. This method should be called
by the active scheduler so that an outstanding request for view server event
notification is maintained. The view server interprets the first queue made by
a session as the sign that the session application is up and runnning.

This method is now deprecated becasue the requests are now automatically queued
by CVwsSessionWrapper.

@deprecated
 */
EXPORT_C void CVwsSessionWrapper::QueueAsyncRequest()
	{
	if (iViewEventHandler)
		{
		iViewEventHandler->Queue();
		}
	}

/**
Starts the application with Uid aAppToStart.

Establishes a connection to the view server if necessary.

The request is queued in the server and processed in turn. The application will
be started by a call to the implementation of the MVwsAppStarter interface that
is provided at server start up. 

This method provides an alternative to apparc for starting apps on systems
where use of the view server is preferred to use of the apparc server.
 

@param aAppToStart The Uid of the app that should be started.

@return KErrNone if successful, otherwise one of the system-wide error codes.
 */
EXPORT_C TInt CVwsSessionWrapper::StartApp(TUid aAppToStart)
	{
	CONNECT_AS_REQUIRED
	return iVwsSession->StartApp(aAppToStart);
	}

/**
Note: DeactivateActiveViewIfOwnerMatch should be used instead of this function
      where it is appropriate.
      
Deactivates the active view. This is required before exiting an application
with an active view.

Establishes a connection to the view server if necessary.

If this thread has an active environment the method returns when view
deactivation completes. Otherwise the method returns immediately after creating
a deactivation event in the view server.

@return KErrNone if successful, otherwise one of the system-wide error codes.

@see CVwsSessionWrapper::DeactivateActiveViewIfOwnerMatch
*/
EXPORT_C TInt CVwsSessionWrapper::DeactivateActiveView()
	{
	CONNECT_AS_REQUIRED

	if (!IsSchedulerRunning())
		{
		return iVwsSession->DeactivateActiveView();
		}
	else
		{
		TRequestStatus status;
		iVwsSession->DeactivateActiveView(status);
		TInt error=KErrNone;
		for (;;)
			{
			if (iViewEventHandler)
				{
				User::WaitForRequest(status, iViewEventHandler->iStatus);
				}
			else
				{
				User::WaitForRequest(status);
				}
			if (status!=KRequestPending)
				break;
			if (iViewEventHandler && iViewEventHandler->iStatus!=KRequestPending)
				{
				// Set the event handler active object to inactive to avoid it running
				// a second time if the DoRunL call results in a new active scheduler
				// loop being started.
				TRequestStatus* eventHandlerStatus = &iViewEventHandler->iStatus;
				User::RequestComplete(eventHandlerStatus, iViewEventHandler->iStatus.Int());
				iViewEventHandler->Cancel();
				TRAP(error,iViewEventHandler->DoRunL(););	// Run event handler directly and request new view event.
				if (error != KErrNone)
					{
					break;
					}
				}
			}
		return (error != KErrNone) ? error : status.Int();
		}
	}

/**
Deactivates the active view, but only if the active view is owned by this session.
 This is required before exiting an application with an active view.
 
 Establishes a connection to the view server if necessary.
 
 If this thread has an active environment the method returns when view
 deactivation completes. Otherwise the method returns immediately after creating
 a deactivation event in the view server.
 
 @return KErrNone if the active view was deactivated or is owned by another session, otherwise one of the system-wide error codes. 
  */
 EXPORT_C TInt CVwsSessionWrapper::DeactivateActiveViewIfOwnerMatch()
 	{
 	CONNECT_AS_REQUIRED
 
 	if (!IsSchedulerRunning())
 		{
 		return iVwsSession->DeactivateActiveViewIfOwnerMatch();
		}
 	else
 		{
 		TRequestStatus status;
 		iVwsSession->DeactivateActiveViewIfOwnerMatch(status);
 		TInt error=KErrNone;
 		for (;;)
 			{
 			if (iViewEventHandler)
 				{
 				User::WaitForRequest(status, iViewEventHandler->iStatus);
 				}
 			else
 				{
 				User::WaitForRequest(status);
 				}
 			if (status!=KRequestPending)
 				break;
 			if (iViewEventHandler && iViewEventHandler->iStatus!=KRequestPending)
 				{
				// Set the event handler active object to inactive to avoid it running
 				// a second time if the DoRunL call results in a new active scheduler
 				// loop being started.
 				TRequestStatus* eventHandlerStatus = &iViewEventHandler->iStatus;
 				User::RequestComplete(eventHandlerStatus, iViewEventHandler->iStatus.Int());
 				iViewEventHandler->Cancel();
 				TRAP(error,iViewEventHandler->DoRunL(););	// Run event handler directly and request new view event.
 				if (error != KErrNone)
 					{
 					break;
 					}
 				}
 			}
 		return (error != KErrNone) ? error : status.Int();
 		}
 	}
 
/**

Requests that the next deactivation of the view identified by aViewId be
notified to this session. The request is cleared after the next notification.
Only the last made notification request is honoured.

For example, can be used when a screensaver view is deactivated by new user
activity to reset the system state.

Establishes a connection to the view server if necessary.

@param aViewId The view to monitor for deactivation.

@return KErrNone if successful, otherwise one of the system-wide error codes.
 */
EXPORT_C TInt CVwsSessionWrapper::NotifyNextDeactivation(const TVwsViewId& aViewId)
	{
	CONNECT_AS_REQUIRED
	return iVwsSession->NotifyNextDeactivation(aViewId);
	}

/**
Requests that the next deactivation of any view be notified to this session.
The request is cleared after the next notification. Only the last made
notification request is honoured.

Establishes a connection to the view server if necessary.

@return KErrNone if successful, otherwise one of the system-wide error codes.
 */
EXPORT_C TInt CVwsSessionWrapper::NotifyNextDeactivation()
	{
	return NotifyNextDeactivation(KNullViewId);
	}

/**
Requests that the next activation of the view identified by aViewId be
notified to this session. The request is cleared after the next notification.
Only the last made notification request is honoured.

This allows any client to track the system current active state.

Establishes a connection to the view server if necessary.

@param aViewId The view to monitor for activation.

@return KErrNone if successful, otherwise one of the system-wide error codes.
 */
EXPORT_C TInt CVwsSessionWrapper::NotifyNextActivation(const TVwsViewId& aViewId)
	{
	CONNECT_AS_REQUIRED
	return iVwsSession->NotifyNextActivation(aViewId);
	}

/**
Requests that the next activation of any view be notified to this session. The
request is cleared after the next notification. Only the last made notification
request is honoured.

Establishes a connection to the view server if necessary.

@return KErrNone if successful, otherwise one of the system-wide error codes.
 */
EXPORT_C TInt CVwsSessionWrapper::NotifyNextActivation()
	{
	return NotifyNextActivation(KNullViewId);
	}

/**
Tests whether the thread that requested the current view switch satisfies a
security policy. This function can only be called from the thread of the
session that owns the view being activated, and only during the activation of
a view.

Establishes a connection to the view server if necessary.

@param aResult If the requesting thread satisfies the security policy, aResult
               will be set to ETrue, otherwise it will be set to EFalse.
               If this function does not complete successfully, aResult is
               undefined.
@param aSecurityPolicy The security policy to check the requesting thread
                       against.

@return KErrNone if successful, KErrUnknown if this function is called from the
        wrong session or at the wrong time, otherwise one of the system-wide
        error codes.

@see CCoeAppUi::CheckSourceOfViewSwitchL
 */
EXPORT_C TInt CVwsSessionWrapper::CheckSourceOfViewSwitch(TBool& aResult,const TSecurityPolicy& aSecurityPolicy,const char* aDiagnostic)
	{
	CONNECT_AS_REQUIRED
	return iVwsSession->CheckSourceOfViewSwitch(aResult,aSecurityPolicy,aDiagnostic);
	}

/**
Enables or disables screen blanking between screen device changes.

If screen blanking is enabled, the screen will be covered by a solid window. The color of this blanking window
would be as set in CVwsSessionWrapper::SetWindowBackgroundColor method if its used, otherwise by default the 
color would be solid-white.
@see CVwsSessionWrapper::SetWindowBackgroundColor

Establishes a connection to the view server if necessary.

@param aEnable If aEnable is set to ETrue, screen blanking will take place as
               described above, if aEnable is set to EFalse, screen blanking
               will not happen.

@return KErrNone if successful, otherwise one of the system-wide error codes.
 */
EXPORT_C TInt CVwsSessionWrapper::EnableServerBlankScreen(TBool aEnable)
	{
	CONNECT_AS_REQUIRED
	return iVwsSession->EnableServerBlankScreen(aEnable);
	}

/**
Enable or disable protection from external view switches for this session.

Views owned by protected sessions can only be activated by view activation
requests originating from the same session. Activation requests originating
from other sessions will cause a second instance of the application providing
the requested view to be started.
By default, sessions are not protected from external view switches.
 
Establishes a connection to the view server if necessary.

@param aEnable If set to ETrue, this session's views will be protected from
               external view switches. If set to EFalse, they will have no such
               protection.

@return KErrNone if successful, otherwise one of the system-wide error codes.
 */
EXPORT_C TInt CVwsSessionWrapper::EnableExternalViewSwitches(TBool aEnable)
	{
	CONNECT_AS_REQUIRED
	return iVwsSession->SetProtected(!aEnable);
	}

EXPORT_C TInt CVwsSessionWrapper::SetCrossCheckUid(const TUid& aCrossCheckUid)
	{
	CONNECT_AS_REQUIRED
	return iVwsSession->SetCrossCheckUid(aCrossCheckUid);
	}

EXPORT_C TInt CVwsSessionWrapper::SetWindowBackgroundColor(const TRgb& aBgColor)
	{
	CONNECT_AS_REQUIRED
	return iVwsSession->SetWindowBackgroundColor(aBgColor);
	}

EXPORT_C TInt CVwsSessionWrapper::GetCurrentActiveViewInSystem(TVwsViewId& aActiveViewId)
	{
	CONNECT_AS_REQUIRED
	return iVwsSession->GetCurrentActiveViewInSystem(aActiveViewId);
	}

CVwsSessionWrapper::CVwsSessionWrapper()
	{
	}

CVwsSessionWrapper::CVwsSessionWrapper(MVwsSessionWrapperObserver& aObserver) : iObserver(&aObserver)
	{
	}

void CVwsSessionWrapper::ConstructL()
	{
	}

TBool CVwsSessionWrapper::IsSchedulerRunning()
	{
	if (iViewEventHandler)
		{
		return ETrue;
		}
	CActiveScheduler* activeScheduler=CActiveScheduler::Current();
	return (activeScheduler && activeScheduler->StackDepth()>0);
	}

TInt CVwsSessionWrapper::CheckCreateViewServerSession()
// Possible return values:
// KErrNone - connection suceeded.
// KErrNotConnected- connection failed because server was not running.
// KErrNoMemory - unable to allocate client side objects.
	{
	if (iVwsSession)
		{
		return KErrNone;
		}

	TRAPD(err,iVwsSession=new(ELeave) RVwsSession);
	if(err!=KErrNone)
		{
		return err;
		}

	err=iVwsSession->Connect();
	if (err==KErrNone)
		{
		if (iObserver) {
			TInt priority=iVwsSession->GetPriority();
			iViewEventHandler=new CVwsSessionEventHandler(*iObserver,*iVwsSession,priority);
 			if (iViewEventHandler==NULL)
				{
				iVwsSession->Close();
				delete iVwsSession;
				iVwsSession=NULL;
				return KErrNoMemory;
				}
			}
		return KErrNone;
		}
	else
		{
		delete iVwsSession;
		iVwsSession=NULL;
		if (err==KErrNotFound)
			{	// It's not the view that's not found, but the server. Therefore switch to a more helpful error code.
			return KErrCouldNotConnect;
			}
		else 
			{
			return err;
			}
		}
	}


//
// Global functions.
//

GLDEF_C TInt ViewServerThreadStart(TAny* aPtr)
	{
	RThread thread;
	TInt err=User::RenameThread(KVwsViewServerThreadName);
	if (err!=KErrNone && err!=KErrAlreadyExists)
		return err;
	User::SetCritical(User::ESystemCritical);
	thread.Close();

	CActiveScheduler* scheduler=new CActiveScheduler;
	if (!scheduler)
		return KErrNoMemory;
	CActiveScheduler::Install(scheduler);

	CTrapCleanup* trapCleanup=CTrapCleanup::New();
	if (!trapCleanup)
		return KErrNoMemory;
	SVwsCommandLine* comLine=STATIC_CAST(SVwsCommandLine*,aPtr);
	CVwsServer* viewServer=NULL;
	TRAP(err,viewServer=CVwsServer::NewL(*(comLine->iAppStarter)));
	if (err!=KErrNone)
		return err;
	RThread::Rendezvous(KErrNone);

	CActiveScheduler::Start();
	delete viewServer;
	delete CActiveScheduler::Current();
	delete trapCleanup;
	return KErrNone;
	}

GLREF_C void Panic(TVwsPanic aPanic)
	{
	_LIT(KViewServerPanic,"View Server client panic");
	User::Panic(KViewServerPanic,aPanic);
	}
