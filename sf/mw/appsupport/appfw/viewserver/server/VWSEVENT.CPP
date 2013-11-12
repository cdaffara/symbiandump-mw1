// Copyright (c) 1999-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include "vwsinternal.h"
#include "vwsdefpartner.h"
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS
#include "VWSEVENT.H"
#include "VWSERVER.H"
#include "VWSDEBUG.H"
#include "VWSPRIV.H"
#include "vwspatchdata.h"

#include <u32hal.h>



//
// class CVwsThreadWatcher
//

CVwsThreadWatcher::~CVwsThreadWatcher()
	{
	Cancel();
	iThread.Close();
	}

CVwsThreadWatcher::CVwsThreadWatcher()
	: CActive(EPriorityStandard)
	{
	}
	
TInt CVwsThreadWatcher::StartWatching(const TThreadId& aThreadId, const TCallBack& aCallBack)
	{
	iCallBack=aCallBack;
	if(aThreadId!=iThread.Id())
		{
		if(IsAdded())
			{
			Deque();
			iThread.Close();
			}
		TInt err=iThread.Open(aThreadId);
		if (err==KErrNone)
			{
			CActiveScheduler::Add(this);
			iThread.Logon(iStatus);
			SetActive();
			}
		return err;
		}
	return KErrNone;
	}

void CVwsThreadWatcher::DoCancel()
	{
	iThread.LogonCancel(iStatus);
	}

void CVwsThreadWatcher::RunL()
	{
	iCallBack.CallBack();
	}


//
// CVwsEventTimer.
//

CVwsEventTimer::~CVwsEventTimer()
	{
	delete iPeriodic;
	}

CVwsEventTimer* CVwsEventTimer::NewLC(const TTimeIntervalMicroSeconds32& aClientRequestTimeOut, const TTimeIntervalMicroSeconds32& aServerEventTimeOut)
	{ // static
	CVwsEventTimer* self = new(ELeave) CVwsEventTimer(aClientRequestTimeOut, aServerEventTimeOut);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CVwsEventTimer::CVwsEventTimer(const TTimeIntervalMicroSeconds32& aClientRequestTimeOut, const TTimeIntervalMicroSeconds32& aServerEventTimeOut)
	: iClientRequestTimeOut(aClientRequestTimeOut),
	  iServerEventTimeOut(aServerEventTimeOut),
	  iTimeOutState(EUndefined),
	  iScreenDeviceChangeEvent(EFalse)
	{}

void CVwsEventTimer::ConstructL()
	{
	iPeriodic=CPeriodic::NewL(0);
	}

void CVwsEventTimer::Start(MVwsEventTimerObserver* aObserver,const TBool& aScreenDeviceChangeEvent)
	{
	iObserver=aObserver;

	if (iPeriodic->IsActive())
		{
		iPeriodic->Cancel();
		}
	
	TTimeIntervalMicroSeconds32 firstDelay(0);
	iTimeOutState=EUndefined;

	if (iClientRequestTimeOut < iServerEventTimeOut)
		{
		iTimeOutState=EClientRequestTimeOut;
		firstDelay=iClientRequestTimeOut;
		}
	else 
		{
		iTimeOutState=EServerEventTimeOut;
		firstDelay=iServerEventTimeOut;
		}

	LOG3(CVwsLog::EQuiet,_L("CVwsEventTimer::Start : firstDelay [%d] "),firstDelay.Int());
	TTimeIntervalMicroSeconds32 delay(firstDelay);
	iPeriodic->Start(delay,delay,TCallBack(TimerCallBack,this));	
	iScreenDeviceChangeEvent = aScreenDeviceChangeEvent;

	}

TInt CVwsEventTimer::TimerCallBack(TAny* aSelf)
	{ // static
	REINTERPRET_CAST(CVwsEventTimer*,aSelf)->DoTimerCallBack();
	return 0;
	}

void CVwsEventTimer::DoTimerCallBack()
	{
	if (iPeriodic != NULL && iPeriodic->IsActive())
		{
		iPeriodic->Cancel();
		}
	if (iScreenDeviceChangeEvent && iObserver)
		{
		iObserver->HandleTimeOut(iTimeOutState);
		}
	else
		{
		if (iObserver)
			{
			iObserver->HandleTimeOut(iTimeOutState);
			}
		if (iTimeOutState == EClientRequestTimeOut || iTimeOutState == EIntermediateEventTimeOut) 
			{
			TTimeIntervalMicroSeconds32 delay;
			if (iTimeOutState == EClientRequestTimeOut) 
				{
				//Normal delay is given before considering boosting the priority of an application
				delay = iServerEventTimeOut.Int()-iClientRequestTimeOut.Int();
				iTimeOutState = EIntermediateEventTimeOut;
				}
			else
				{
				//Delay of KTimeoutValueForPreemptedProcess is given after boosting priority of an application
				TInt patchableConst = KTimeoutValueForPreemptedProcess;
				#ifdef __WINS__
				// For the emulator allow the constant to be patched via epoc.ini
				UserSvr::HalFunction(EHalGroupEmulator, EEmulatorHalIntProperty,
					(TAny*)"KTimeoutValueForPreemptedProcess", &patchableConst); // read emulator property (if present)
				#endif
				delay = patchableConst;
				iTimeOutState = EServerEventTimeOut;
                LOG2(CVwsLog::ELoud,_L("CVwsEventTimer::DoTimerCallBack : iTimeOutState == EIntermediateEventTimeOut"));
				}
            LOG3(CVwsLog::EQuiet,_L("CVwsEventTimer::DoTimerCallBack : delay = [%d] "),delay.Int());
            iPeriodic->Start(delay,delay,TCallBack(TimerCallBack,this));
			}
		}
	}

//
// CVwsServerEvent.
//

CVwsServerEvent::CVwsServerEvent(CVwsServer& aServer,TType aType,CVwsEventQueue& aQueue)
	:CVwsEvent(aType,aQueue),iServer(aServer)
	{
	}

CVwsServerEvent::~CVwsServerEvent()
	{
	if (iObserved)
		{
		iObserved->NullifyObserver();
		}
	}


void CVwsServerEvent::HandleLastOnQueue()
	{
	iServer.HandleLastServerEventOnQueue();
	}


//
// CVwsServerEvent_Activate.
//

CVwsServerEvent_Activate::CVwsServerEvent_Activate(CVwsServer& aServer,TType aType,CVwsEventQueue& aQueue,const TVwsViewId& aViewIdToActivate,CVwsClientMessage* aClientMessage,const RMessage2& aMessage,const TThreadId& aClientThreadId,TVwsCompleteRequest aCompleteRequest,CVwsThreadWatcher* aThreadWatcher,CVwsEventTimer* aEventTimer,const RThread& aThreadOfClientInitiatingViewSwitch)
	: CVwsServerEvent(aServer,aType,aQueue),
	iViewIdToActivate(aViewIdToActivate),
	iClientMessage(aClientMessage),
	iNewViewClientThreadId(0),
	iCompleteRequest(aCompleteRequest),
	iClientThreadId(aClientThreadId),
	iMessage(aMessage),
	iThreadWatcher(aThreadWatcher),
	iEventTimer(aEventTimer),
	iThreadOfClientInitiatingViewSwitch(aThreadOfClientInitiatingViewSwitch)
	{
	}

CVwsServerEvent_Activate::~CVwsServerEvent_Activate()
	{
	delete iClientMessage;
	delete iThreadWatcher;
	delete iEventTimer;
	iThreadOfClientInitiatingViewSwitch.Close();
	}

void CVwsServerEvent_Activate::HandleTimeOut(CVwsEventTimer::TTimeOut aTimeOut)
	{
	if (aTimeOut==CVwsEventTimer::EServerEventTimeOut && iServer.IsServerEventTimeOutEnabled())
		{
		if (iState==EActivationRequested)
			{
			LOG3(CVwsLog::ENormal,_L("PANIC CLIENT \"%x\" for timing out activation view event - activation request was not completed"),iViewIdToActivate.iAppUid.iUid);
			if (TUint(iNewViewClientThreadId)!=0)
				{
				CVwsSession* newViewSession=iServer.SessionByThreadId(iNewViewClientThreadId);
				if (newViewSession)
					{
					if (iServer.IsPriorityBoostBeforePanicEnabled())
						{
						if (iServer.InitializationFinished())
							{
							newViewSession->PanicClient(EVwsViewActivationEventTimeOut);
							}
						}
					else
						{
						newViewSession->PanicClient(EVwsViewActivationEventTimeOut);
						}
					}
				}
			}
		else if (iState==EDeactivationRequested)
			{
			LOG3(CVwsLog::ENormal,_L("PANIC CLIENT \"%x\" for timing out activation view event - deactivation request was not completed"),iServer.ActiveView().iAppUid.iUid);
			CVwsSession* sessionWithActiveView=iServer.ActiveViewSession();
			if (sessionWithActiveView)
				{
				if (iServer.IsPriorityBoostBeforePanicEnabled())
					{
					if (iServer.InitializationFinished())
						{
						sessionWithActiveView->PanicClient(EVwsViewDeactivationEventTimeOut);
						}
					}
				else
					{
					sessionWithActiveView->PanicClient(EVwsViewDeactivationEventTimeOut);
					}
				}
			}
		else
			{
			LOG4(CVwsLog::EQuiet,_L("Aborting activation of view \"%x,%x\" - server event processing timed-out"),iViewIdToActivate.iAppUid.iUid,iViewIdToActivate.iViewUid.iUid);
			Complete(KErrCancel);
			}
		}
	else if (aTimeOut==CVwsEventTimer::EClientRequestTimeOut)
			{
			LOG2(CVwsLog::ELoud,_L("Activation event timer has passed client request time-out"));
			if (iCompleteRequest==ECompleteRequest && iServer.SessionByThreadId(iClientThreadId)!=NULL)
				{
				LOG2(CVwsLog::ELoud,_L("Client request timed-out - silently completing request"));
				iCompleteRequest=EDoNotCompleteRequest;
				iMessage.Complete(KErrNone);
				}
			}
		else if (aTimeOut==CVwsEventTimer::EIntermediateEventTimeOut) 
           { 
           if (iState==EActivationRequested) 
               { 
               iServer.BoostPriority(iServer.SessionByThreadId(iNewViewClientThreadId)); 
               } 
           else if (iState==EDeactivationRequested) 
               { 
               iServer.BoostPriority(iServer.ActiveViewSession()); 
               } 
           } 

	}

void CVwsServerEvent_Activate::ProcessEventL()
	{
LOG4(CVwsLog::ELoud,_L("Starting to process activation of \"%x,%x\""),iViewIdToActivate.iAppUid.iUid,iViewIdToActivate.iViewUid.iUid);

	RequestActivateNewView();
	}

#ifdef _DEBUG
void CVwsServerEvent_Activate::RequestCompleted(TRequestType aType,TInt aError,const RMessage2&)
#else
void CVwsServerEvent_Activate::RequestCompleted(TRequestType /*aType*/,TInt aError,const RMessage2&)
#endif // _DEBUG
	{
	switch (iState)
		{
		case EPending:
			ASSERT(EFalse);
			break;
		case EActivationRequested:
			ASSERT(aType==MVwsSessionObserver::EActivation);
			LOG5(CVwsLog::ELoud,_L("Session request complete [activation event state: EActivationRequested; view to activate: \"%x,%x\", error: %d]"),iViewIdToActivate.iAppUid.iUid,iViewIdToActivate.iViewUid.iUid,aError);
			if (aError)
				{
				if (aError==KErrViewWrongMode)
					{
					RequestActivateSystemDefaultView();
					}
				else
					{
					Complete(aError);
					}
				}
			else
				{
				TVwsViewId viewToDeactivate(iServer.ActiveView());
				// When an application whose view has to be activated is in background, the currently active view will not be deactivated.
				// Note: Switching between different views of the same application must be followed by deactivation of old view
				if ((iMessage.Int3() && !(iServer.IsAppInForeground())) && (viewToDeactivate.iAppUid != iViewIdToActivate.iAppUid))
					{
					LOG4(CVwsLog::ELoud,_L("\"%x,%x\", is activated at the background, so foreground view will not be deactivated"),iViewIdToActivate.iAppUid.iUid,iViewIdToActivate.iViewUid.iUid);
					Complete(KErrNone);
					}
				else
					{
				RequestDeactivateOldView();
					}
				}
			break;
		case ESystemDefaultActivationRequested:
			ASSERT(aType==MVwsSessionObserver::EActivation);
			if (aError)
				{
				LOG3(CVwsLog::EQuiet,_L("Failed to activate system default view, error: %d"),aError);
				RequestActivateSystemDefaultView();
				}
			else
				{
				RequestDeactivateOldView();
				}
			break;
		case EDeactivationRequested:
			ASSERT(aType==MVwsSessionObserver::EDeactivation);
			LOG5(CVwsLog::ELoud,_L("Session request complete [activation event state: EDeactivationRequested; view to activate: \"%x,%x\", error: %d]"),iViewIdToActivate.iAppUid.iUid,iViewIdToActivate.iViewUid.iUid,aError);
			iServer.HandleDeactivation(iServer.ActiveView(),iViewIdToActivate);
			Complete(KErrNone);
			break;
		default:
		case EAppStartRequested:
			ASSERT(EFalse);
		}

// Disable unused parameter warning under release builds
#pragma warning (disable: 4100) 
	}
#pragma warning (default: 4100)

void CVwsServerEvent_Activate::NowObserving(CVwsSessionEvent* aObserved)
	{
	if(iObserved)
	    {
	    iObserved->NullifyObserver();
	    }
       	iObserved = aObserved;
	}

void CVwsServerEvent_Activate::NullifyObserved()
	{
	iObserved = NULL;	
	}

void CVwsServerEvent_Activate::HandleViewAdded(const TVwsViewId& aViewId, const TThreadId& aNewViewClientThreadId)
	{
	ASSERT(iState==EAppStartRequested);
	if (aViewId.iAppUid==iViewIdToActivate.iAppUid
	  &&aViewId.iAppUid==aViewId.iViewUid)
		{
		iViewIdToActivate = aViewId;
		}
	if (aViewId==iViewIdToActivate)
		{
		iServer.SetViewAdditionObserver(NULL);
		
		// set iNewViewClientThreadId to indicate that the new view should come from the session specified if possible
		iNewViewClientThreadId = aNewViewClientThreadId;
		RequestActivateNewView();
		}
	}

void CVwsServerEvent_Activate::RequestActivateNewView()
	{
	CVwsSession* sessionWithNewView=iServer.SessionByThreadIdAndAppUid(iClientThreadId,iViewIdToActivate.iAppUid);
	
	// iNewViewClientThreadId may have been set by the caller to indicate a preference between
	// sessions with the same App Uid
	if (sessionWithNewView==NULL && iNewViewClientThreadId!=TThreadId(0))
		{
		sessionWithNewView=iServer.SessionByThreadIdAndAppUid(iNewViewClientThreadId,iViewIdToActivate.iAppUid);
		}

	if (sessionWithNewView==NULL)
		{
		sessionWithNewView=iServer.SessionByUid(iViewIdToActivate.iAppUid);
		}

	if (sessionWithNewView==NULL || (sessionWithNewView && sessionWithNewView->IsExiting()))
		{
		RequestAppStart();
		}
	else
		{
		iNewViewClientThreadId=sessionWithNewView->ClientThreadId();
		DoRequestActivateNewView(*sessionWithNewView);
		}
	}

void CVwsServerEvent_Activate::RequestActivateSystemDefaultView()
	{
	iServer.GetSystemDefaultView(iViewIdToActivate);
	CVwsSession* sessionWithSystemDefaultView=iServer.SessionByUid(iViewIdToActivate.iAppUid);
	// need an empty client message 
	CVwsClientMessage* emptyClientMessage=CVwsClientMessage::New();
	if (emptyClientMessage==NULL)
		{
		Complete(KErrNoMemory);
		}

	if (sessionWithSystemDefaultView)
		{
		iNewViewClientThreadId=sessionWithSystemDefaultView->ClientThreadId();
		RequestClientActivation(*sessionWithSystemDefaultView,emptyClientMessage,ESystemDefaultActivationRequested);
		}
	else if (iThreadWatcher==NULL)
		{
		// app died
		StopActivationProfile();
		ASSERT(iState!=EAppStartRequested);	// Already asked to start app once.
		StartAppStartProfile();
		iServer.SetViewAdditionObserver(this);
		TThreadId appThreadId;
		iClientMessage = emptyClientMessage;
		TInt err=iServer.StartApp(iViewIdToActivate.iAppUid,appThreadId);
		if (err)
			{
			LOG3(CVwsLog::EQuiet,_L("App start FAILED with %d"),err);
			Complete(err);
			}
		else
			{
			iState=EAppStartRequested;
			}
		}
	else
		{
		delete emptyClientMessage;
		}
	}

void CVwsServerEvent_Activate::RequestAppStart()
	{
	ASSERT(iState!=EAppStartRequested);	// Already asked to start app once.
	StartAppStartProfile();
	iServer.SetViewAdditionObserver(this);
	TThreadId appThreadId;
	TInt err=iServer.StartApp(iViewIdToActivate.iAppUid,appThreadId);
	if (err)
		{
		LOG3(CVwsLog::EQuiet,_L("App start FAILED with %d"),err);
		Complete(err);
		}
	else
		{
		iState=EAppStartRequested;
		err=iThreadWatcher->StartWatching(appThreadId,TCallBack(&AppThreadWatcherCallBack,this));
		if (err)
			{
			LOG3(CVwsLog::EQuiet,_L("App start FAILED with %d"),err);
			Complete(err);
			}
		}
	}

TInt CVwsServerEvent_Activate::AppThreadWatcherCallBack(TAny* aPtr)
	// static
	{
	LOG2(CVwsLog::EQuiet,_L("App start FAILED with app client thread death"));
	REINTERPRET_CAST(CVwsServerEvent_Activate*,aPtr)->Complete(KErrCancel);
	return 0;
	}

TInt CVwsServerEvent_Activate::RequestClientActivation(CVwsSession& aSession,CVwsClientMessage* aClientMessage,TState aNewState)
	{
	TRAPD(error,
			aSession.RequestClientActivationL(*this,iViewIdToActivate,iServer.ActiveView(),aClientMessage,iThreadOfClientInitiatingViewSwitch));
	if (error)
		{
		LOG3(CVwsLog::ELoud,_L("RequestClientActivationL FAILED with %d - unable to activate new view"),error);
		if(aSession.IsLeaveAfterOwnershipTaken())
			{
			aSession.ResetLeaveAfterOwnershipTaken();
			iClientMessage=NULL;
			}
		Complete(error);
		}
	else
		{
		iClientMessage=NULL; // The session is now the owner of the custom message buf.
		iState=aNewState;
		}
	return error;
	}

void CVwsServerEvent_Activate::CustomCrossCheck(CVwsSession& aSessionWithNewView)
	{
	if (iClientMessage->iMessageId == KUidCustomCrossCheck && iClientMessage->iMessage == NULL)
		{
		iClientMessage->iMessageId = KNullUid;
		TVwsViewId viewId;
		if (aSessionWithNewView.GetTopView(viewId) == KErrNone)
			{
#ifdef _DEBUG
			if (iViewIdToActivate != viewId)
				{
				LOG4(CVwsLog::ELoud,_L("Cross check to \"%x,%x\""), viewId.iAppUid.iUid, viewId.iViewUid.iUid);
				}
#endif //_DEBUG
			iViewIdToActivate = viewId;
			iClientMessage->iMessageId = KNullUid;
			}
		}
	}

void CVwsServerEvent_Activate::DoRequestActivateNewView(CVwsSession& aSessionWithNewView)
	{
	StopAppStartProfile();
	delete iThreadWatcher;
	iThreadWatcher=NULL;

	iEventTimer->Start(this,EFalse);

	if (iClientMessage)
		{
		CustomCrossCheck(aSessionWithNewView);
		}

	if (iServer.CheckViewExists(aSessionWithNewView.ClientThreadId(),iViewIdToActivate)==KErrNotFound)
		{
		LOG4(CVwsLog::EQuiet,_L("Aborting activation - view \"%x,%x\" not found"),iViewIdToActivate.iAppUid.iUid,iViewIdToActivate.iViewUid.iUid);
		Complete(KErrNotFound);
		return;
		}

	StartActivationProfile();
	RequestClientActivation(aSessionWithNewView,iClientMessage,EActivationRequested);
	}

void CVwsServerEvent_Activate::RequestDeactivateOldView()
	{
	StopActivationProfile();
	TVwsViewId viewToDeactivate(iServer.ActiveView());
	CVwsSession* sessionWithOldView=iServer.ActiveViewSession();
	CVwsSession* sessionWithNewView=iServer.SessionByThreadIdAndAppUid(iClientThreadId,iViewIdToActivate.iAppUid);
		
	//If the client that is requesting view activation is same as the client which has the new active view, ie, reactivation of same view, 
	//then the session with new active view is taken. 
	if (sessionWithNewView==NULL && iNewViewClientThreadId!=TThreadId(0))
		{
		sessionWithNewView=iServer.SessionByThreadIdAndAppUid(iNewViewClientThreadId,iViewIdToActivate.iAppUid);
		}
	
	if (sessionWithNewView==NULL)
		{
		sessionWithNewView=iServer.SessionByUid(iViewIdToActivate.iAppUid);
		}
	
	if (viewToDeactivate.iAppUid.iUid && (viewToDeactivate != iViewIdToActivate || sessionWithOldView != sessionWithNewView))
		{
		if (sessionWithOldView==NULL)
			{	// Session with old view has disconnected, so abort deactivation.
			LOG4(CVwsLog::ELoud,_L("Aborting deactivation of view \"%x,%x\" - client disconnected"),viewToDeactivate.iAppUid.iUid,viewToDeactivate.iViewUid.iUid);
			Complete(KErrNone);
			}
		else
			{
			TBool differentInstanceOfSameApp = EFalse;
			if (viewToDeactivate.iAppUid == iViewIdToActivate.iAppUid && sessionWithOldView != sessionWithNewView)
				differentInstanceOfSameApp = ETrue;
			TRAPD(err,sessionWithOldView->RequestClientDeactivationL(*this,viewToDeactivate,iViewIdToActivate, differentInstanceOfSameApp));
			if (err)
				{
				LOG3(CVwsLog::ELoud,_L("RequestClientDeactivationL FAILED with %d - unable to deactivate currently active view"),err);
				Complete(err);
				}
			else
				{
				iState=EDeactivationRequested;
				}
			}
		}
	else
		{
		// No view to deactivate, so complete.
		LOG2(CVwsLog::ELoud,_L("Aborting deactivation - no view to deactivate"));
		Complete(KErrNone);
		}
	}

void CVwsServerEvent_Activate::Complete(TInt aError)
	{
	LOG5(CVwsLog::ELoud,_L("Completing activation of \"%x,%x\" with %d"),iViewIdToActivate.iAppUid,iViewIdToActivate.iViewUid,aError);
	if (aError==KErrNone && TUint(iNewViewClientThreadId)!=0) 
		{
		iServer.HandleActivation(iViewIdToActivate,iServer.ActiveView());
		TVwsViewId viewToDeactivate(iServer.ActiveView());
		// The active active viewid will be set in the server when
		// 1. Application whose view is getting activated is in foreground
		// 2. Switching between different views of the same application
		if(iServer.IsAppInForeground() || (iViewIdToActivate.iAppUid == viewToDeactivate.iAppUid))
			{
			iServer.SetActiveView(iNewViewClientThreadId,iViewIdToActivate);	
			}
		}
	iServer.SetViewAdditionObserver(NULL);
	if ((iCompleteRequest==ECompleteRequest) && (iServer.SessionByThreadId(iClientThreadId)!=NULL))
		{
		if(!iMessage.IsNull())
			{
			iMessage.Complete(aError);
			}
		}
#ifdef __DO_LOGGING__
	else if (iCompleteRequest==ECompleteRequest)
		{
		LOG2(CVwsLog::ELoud,_L("Client requesting activation not current - completing activation will not complete message handle"));
		}
#endif
	ReportEventProcessed();
	}

void CVwsServerEvent_Activate::HandleSessionRemoval(const TThreadId& aClientThreadId)
	{
	TThreadId threadId(aClientThreadId);
	TBool iCancel=EFalse;
	if (iState==EDeactivationRequested)
		{
		CVwsSession* sessionWithActiveView=iServer.ActiveViewSession();
		if (sessionWithActiveView && threadId==sessionWithActiveView->ClientThreadId())
			{
			LOG2(CVwsLog::ELoud,_L("Client with active view removed while session deactivation request outstanding"));
			iCancel=ETrue;
			}
#ifdef __DO_LOGGING__
		else if (threadId==iNewViewClientThreadId)
			{
			LOG2(CVwsLog::ELoud,_L("Client with new view removed while session deactivation request outstanding - waiting for deactivation to complete"));
			}
#endif
		}
	else if (threadId==iNewViewClientThreadId)
		{
		LOG2(CVwsLog::ELoud,_L("Client with new view removed"));
		iCancel=ETrue;
		}
	if (iCancel)
		{
		LOG2(CVwsLog::ELoud,_L("Client removal means activation event will not complete normally - activation cancelled"));
		Complete(KErrCancel);
		}
	}

void CVwsServerEvent_Activate::StartAppStartProfile()
	{
	ASSERT(iWhichProfile==ENone);
	iWhichProfile=EAppStart;
	}

void CVwsServerEvent_Activate::StopAppStartProfile()
	{
	if (iWhichProfile==EAppStart)
		{
		iWhichProfile=ENone;
		}
	}

void CVwsServerEvent_Activate::StartActivationProfile()
	{
	ASSERT(iWhichProfile==ENone);
	iWhichProfile=EActivation;
	}

void CVwsServerEvent_Activate::StopActivationProfile()
	{
	if (iWhichProfile==EActivation)
		{
		iWhichProfile=ENone;
		}
	}


//
// CVwsServerEvent_ScreenDeviceChanged.
//

CVwsServerEvent_ScreenDeviceChanged::CVwsServerEvent_ScreenDeviceChanged(CVwsServer& aServer,CVwsEventQueue& aQueue,CVwsThreadWatcher* aThreadWatcher,CVwsEventTimer* aEventTimer)
	: CVwsServerEvent_Activate(aServer,CVwsServerEvent::ERejectPairs,aQueue,KNullViewId,NULL,RMessage2(),0,EDoNotCompleteRequest,aThreadWatcher,aEventTimer,RThread()),
	iThreadWatcher(aThreadWatcher) ,iEventTimer(aEventTimer)
	{
	}

CVwsServerEvent_ScreenDeviceChanged::~CVwsServerEvent_ScreenDeviceChanged()
	{
	}

void CVwsServerEvent_ScreenDeviceChanged::ProcessEventL()
	{
	LOG2(CVwsLog::ELoud,_L("Started processing of screen device changed event"));
	if(iServer.ActiveViewSession()!=NULL)
		{
		TThreadId activeClientThread = iServer.ActiveViewSession()->ClientThreadId();
		TRAPD(err,iThreadWatcher->StartWatching(activeClientThread,TCallBack(&AppThreadWatcherCallBack,this)));
		iEventTimer->Start(this,ETrue);
		if (err)
			{
			Complete(err);
			}
		}
	TVwsViewId activeView(iServer.ActiveView());
	if (activeView.iAppUid.iUid)
		{
		RequestChangeNotification();
		}
	else
		{
		LOG2(CVwsLog::ELoud,_L("No active view, so ignoring screen device changed event"));
		Complete(KErrNone);
		}
	}
TInt CVwsServerEvent_ScreenDeviceChanged::AppThreadWatcherCallBack(TAny* aPtr)
	// static
	{
	LOG2(CVwsLog::ELoud,_L("Client thread died while processing Screendevice change event"));
	REINTERPRET_CAST(CVwsServerEvent_ScreenDeviceChanged*,aPtr)->Complete(KErrCancel);
	return 0;
	}

void CVwsServerEvent_ScreenDeviceChanged::RequestCompleted(TRequestType aType,TInt aError,const RMessage2& aMessage)
	{
	switch (iScreenDeviceNotificationState)
		{
		case EPending:
			ASSERT(EFalse);
			break;
		case EScreenDeviceChangeNotificationRequested:
			{
			ASSERT(aType==EScreenDeviceChangeNotification);
			TRAPD(err,RequestActivationAsRequiredL(aMessage));
			if (err)
				{
				LOG3(CVwsLog::ELoud,_L("CVwsClientMessage::NewL FAILED with %d, unable to activate view in responce to screen device changed event"),err);
				RequestActivateSystemDefaultView();
				}
			}
			break;
		case EViewActivationRequested:
		case EDefaultSystemViewActivationRequested:
			CVwsServerEvent_Activate::RequestCompleted(aType,aError,aMessage);
			break;
		default:
			ASSERT(EFalse);
		}

// Disable unused parameter warning under release builds
#pragma warning (disable: 4100) 
	}
#pragma warning (default: 4100)

void CVwsServerEvent_ScreenDeviceChanged::NowObserving(CVwsSessionEvent* aObserved)
	{
	if(iObserved)
	    {
	    iObserved->NullifyObserver();
	    }
	iObserved = aObserved;
	}

void CVwsServerEvent_ScreenDeviceChanged::NullifyObserved()
	{
	iObserved = NULL;	
	}

void CVwsServerEvent_ScreenDeviceChanged::RequestChangeNotification()
	{
	const TVwsViewId activeView(iServer.ActiveView());
	CVwsSession* sessionWithActiveView=iServer.ActiveViewSession();
	ASSERT(sessionWithActiveView); // Session with active view MUST be found.
	TRAPD(err,sessionWithActiveView->RequestScreenDeviceChangeNotificationL(*this,activeView));
	if (err)
		{
		LOG3(CVwsLog::ELoud,_L("RequestScreenDeviceChangeNotificationL FAILED with %d, unable to notify screen device changed"),err);
		iScreenDeviceNotificationState=EDefaultSystemViewActivationRequested;
		RequestActivateSystemDefaultView();
		}
	else
		{
		iScreenDeviceNotificationState=EScreenDeviceChangeNotificationRequested;
		}
	}

void CVwsServerEvent_ScreenDeviceChanged::RequestActivationAsRequiredL(const RMessage2& aMessage)
	{
	TVwsViewIdAndMessageBuf viewToActivateBuf;
	aMessage.ReadL(2,viewToActivateBuf);
	TVwsViewIdAndMessage viewToActivate(viewToActivateBuf());
	iViewIdToActivate=viewToActivate.iViewId;
	if (iViewIdToActivate==TVwsViewId())
		{	// Null view id, so ignore.
		Complete(KErrNone);
		}
	else if (iServer.IsViewActive(iViewIdToActivate))
		{
		LOG4(CVwsLog::ELoud,_L("View \"%x,%x\" already active - aborting activation"),iViewIdToActivate.iAppUid.iUid,iViewIdToActivate.iViewUid.iUid);
		Complete(KErrNone);
		}
	else
		{
		iClientMessage=CVwsClientMessage::NewL(viewToActivate.iCustomMessageId,viewToActivate.iCustomMessageLength,aMessage,3);
		iScreenDeviceNotificationState=EViewActivationRequested;
		
		// Setting the iNewViewClientThreadId here indicates that if there are multiple sessions
		// with the same app uid and we are switching views within one app then the one with the
		// currently active view should be preferred
		iNewViewClientThreadId = iServer.ActiveViewSession()->ClientThreadId();
		RequestActivateNewView();
		}
	}

#ifdef _DEBUG
void CVwsServerEvent_ScreenDeviceChanged::Complete(TInt aError)
#else
void CVwsServerEvent_ScreenDeviceChanged::Complete(TInt /*aError*/)
#endif // _DEBUG
	{
	LOG3(CVwsLog::ELoud,_L("Completed processing of screen device changed event with %d"),aError);
	ReportEventProcessed();

// Disable unused parameter warning under release builds
#pragma warning (disable: 4100) 
	}
#pragma warning (default: 4100)

void CVwsServerEvent_ScreenDeviceChanged::HandleAddedToQueue()
	{
	// display opaque intercepting window
	iServer.MakeInterceptingWindowVisibleAndUpdateScreen();
	}

void CVwsServerEvent_ScreenDeviceChanged::HandleSessionRemoval(const TThreadId& aClientThreadId)
	{
	switch (iScreenDeviceNotificationState)
		{
	case EScreenDeviceChangeNotificationRequested:
		{
		CVwsSession* sessionWithActiveView=iServer.ActiveViewSession();
		if(sessionWithActiveView && aClientThreadId==sessionWithActiveView->ClientThreadId())
			{
			LOG2(CVwsLog::ELoud,_L("Client with active view removed while screen device change request outstanding"));
			RequestActivateSystemDefaultView();
			iScreenDeviceNotificationState=EDefaultSystemViewActivationRequested;
			}
		}
	case EViewActivationRequested:
	case EDefaultSystemViewActivationRequested:
		CVwsServerEvent_Activate::HandleSessionRemoval(aClientThreadId);
		break;
	case EPending:
	default:
		break;
	}
}

void CVwsServerEvent_ScreenDeviceChanged::HandleTimeOut(CVwsEventTimer::TTimeOut /*aTimeOut*/)
	{
	LOG2(CVwsLog::ELoud,_L("CVwsServerEvent_ScreenDeviceChanged view event timer has passed client request time-out"));
	switch (iScreenDeviceNotificationState)
		{
		case EScreenDeviceChangeNotificationRequested:
			Complete(KErrNone);
			break;

		case EViewActivationRequested:
		case EDefaultSystemViewActivationRequested:
			CVwsServerEvent_Activate::HandleTimeOut(CVwsEventTimer::EClientRequestTimeOut);
			break;

		case EPending:
		default:
			break;
		}
	}

//
// CVwsServerEvent_DeactivateActiveView.
//

CVwsServerEvent_DeactivateActiveView::CVwsServerEvent_DeactivateActiveView(CVwsServer& aServer,CVwsEventQueue& aQueue,const RMessage2& aMessage,const TThreadId& aClientThreadId,TVwsCompleteRequest aCompleteRequest,CVwsEventTimer* aEventTimer)
	: CVwsServerEvent(aServer,CVwsServerEvent::ENormal,aQueue),
	iMessage(aMessage),
	iClientThreadId(aClientThreadId),
	iCompleteRequest(aCompleteRequest),
	iEventTimer(aEventTimer)
	{
	}

CVwsServerEvent_DeactivateActiveView::~CVwsServerEvent_DeactivateActiveView()
	{
	delete iEventTimer;
	}

void CVwsServerEvent_DeactivateActiveView::ProcessEventL()
	{
	LOG2(CVwsLog::ELoud,_L("Started processing of deactivate active view event"));
	const TVwsViewId activeView(iServer.ActiveView());

	//Check if the Cross Check UID is set, if it is then enable customised behaviour here
	if(iServer.CrossCheckUid()!=KNullUid)
		{
		CVwsSession* self = iServer.SessionByThreadId(iClientThreadId);
		CVwsSession* active = iServer.ActiveViewSession();
		if (self != active)
			{
			LOG2(CVwsLog::ELoud,_L("Requested session does not have Active view, so ignoring deactivate active view event"));
			if(iCompleteRequest==ECompleteRequest&& iServer.SessionByThreadId(iClientThreadId)!=NULL)
				{
				iMessage.Complete(KErrNone);
				}
			ReportEventProcessed();
			return;
			}
		}

	if (activeView.iAppUid.iUid)
		{
		TRAPD(err,RequestDeactivationL());
		if (err)
			{
			Complete(err);
			}
		}
	else
		{
		LOG2(CVwsLog::ELoud,_L("No active view, so ignoring deactivate active view event"));
		Complete(KErrNone);
		}
	}

#ifdef _DEBUG
void CVwsServerEvent_DeactivateActiveView::RequestCompleted(TRequestType aType,TInt,const RMessage2&)
#else
void CVwsServerEvent_DeactivateActiveView::RequestCompleted(TRequestType /*aType*/,TInt,const RMessage2&)
#endif // _DEBUG
	{
	switch (iState)
		{
		case EPending:
			ASSERT(EFalse);
			break;
		case EDeactivationRequested:
			ASSERT(aType==EDeactivation);
			Complete(KErrNone);
			break;
		default:
			ASSERT(EFalse);
		}

// Disable unused parameter warning under release builds
#pragma warning (disable: 4100) 
	}
#pragma warning (default: 4100)

void CVwsServerEvent_DeactivateActiveView::NowObserving(CVwsSessionEvent* aObserved)
	{
	if(iObserved)
	    {
	    iObserved->NullifyObserver();
	    }
	iObserved = aObserved;
	}

void CVwsServerEvent_DeactivateActiveView::NullifyObserved()
	{
	iObserved = NULL;	
	}

void CVwsServerEvent_DeactivateActiveView::RequestDeactivationL()
	{
	ASSERT(iState==EPending);

	iEventTimer->Start(this,EFalse);

	iActiveView=iServer.ActiveView();
	CVwsSession* sessionWithActiveView=iServer.ActiveViewSession();
	// If the session with currently active view is NULL, then the deactivation request is
 	// completed with KErrNotFound and the previously active view is activated.
	if (sessionWithActiveView)
 		{
 		//EFalse always because the deactivation of view is requested by the application directly
 		sessionWithActiveView->RequestClientDeactivationL(*this,iActiveView,TVwsViewId(),EFalse);
 		iState=EDeactivationRequested;
 		}
 	else
 		{
 		LOG2(CVwsLog::ELoud,_L("Session with active view is NULL, so completing the deactivation request with KErrNotFound"));
 		User::Leave(KErrNotFound);
 		}
	}

void CVwsServerEvent_DeactivateActiveView::Complete(TInt aErr)
	{
	LOG3(CVwsLog::ELoud,_L("Completing deactivate active view event with %d"),aErr);
	if (aErr)
		{
		iServer.HandleDeactivation(iActiveView,TVwsViewId());
		}
	iServer.ClearActiveView();
	if(iCompleteRequest==ECompleteRequest&& iServer.SessionByThreadId(iClientThreadId)!=NULL)
		{
		iMessage.Complete(aErr);
		}
	ReportEventProcessed();
	}

void CVwsServerEvent_DeactivateActiveView::HandleSessionRemoval(const TThreadId& aClientThreadId)
	{
	TThreadId threadId(aClientThreadId);
	if (iState==EDeactivationRequested)
		{
		CVwsSession* sessionWithActiveView=iServer.ActiveViewSession();
		if (sessionWithActiveView && threadId==sessionWithActiveView->ClientThreadId())
			{
			LOG2(CVwsLog::ELoud,_L("Client with active view removed while session deactivation request outstanding"));
			LOG2(CVwsLog::ELoud,_L("Client removal means deactivate active view event will not complete normally - deactivation cancelled"));
			Complete(KErrCancel);
			}
		}
	}

void CVwsServerEvent_DeactivateActiveView::HandleTimeOut(CVwsEventTimer::TTimeOut aTimeOut)
	{
	if (aTimeOut==CVwsEventTimer::EServerEventTimeOut && iServer.IsServerEventTimeOutEnabled())
		{
		if (iState==EDeactivationRequested)
			{
			LOG3(CVwsLog::ENormal,_L("PANIC CLIENT \"%x\" for timing out activation view event - deactivation request was not completed"),iServer.ActiveView().iAppUid.iUid);
			CVwsSession* sessionWithActiveView=iServer.ActiveViewSession();
			if (sessionWithActiveView)
				{
				if(iServer.IsPriorityBoostBeforePanicEnabled())
					{
					if (iServer.InitializationFinished())
						{
						sessionWithActiveView->PanicClient(EVwsViewDeactivationEventTimeOut);
						}
					}
				else
					{
					sessionWithActiveView->PanicClient(EVwsViewDeactivationEventTimeOut);
					}
				}
			}
		else
			{
			LOG4(CVwsLog::EQuiet,_L("Aborting deactivation of active view \"%x,%x\" - server event processing timed-out"),iServer.ActiveView().iAppUid.iUid,iServer.ActiveView().iViewUid.iUid);
			Complete(KErrCancel);
			}
		}
	else if (aTimeOut==CVwsEventTimer::EClientRequestTimeOut)
			{
			LOG2(CVwsLog::ELoud,_L("Deactivate active view event timer has passed client request time-out"));
			if (iCompleteRequest==ECompleteRequest && iServer.SessionByThreadId(iClientThreadId)!=NULL)
				{
				LOG2(CVwsLog::ELoud,_L("Client request timed-out - silently completing request"));
				iCompleteRequest=EDoNotCompleteRequest;
				iMessage.Complete(KErrNone);
				}
			}
		else if (aTimeOut==CVwsEventTimer::EIntermediateEventTimeOut) 
           { 
           if (iState==EDeactivationRequested) 
               { 
               iServer.BoostPriority(iServer.ActiveViewSession()); 
               } 
           }
	}

//
// CVwsServerEvent_AppStart.
//

CVwsServerEvent_AppStart::CVwsServerEvent_AppStart(CVwsServer& aServer,CVwsEventQueue& aQueue,const RMessage2& aMessage,const TUid& aAppToStart,CVwsThreadWatcher* aThreadWatcher)
	: CVwsServerEvent(aServer,CVwsServerEvent::ENormal,aQueue),iAppToStart(aAppToStart),iMessage(aMessage),iThreadWatcher(aThreadWatcher)
	{
	}

CVwsServerEvent_AppStart::~CVwsServerEvent_AppStart()
	{
	delete iThreadWatcher;
	}

void CVwsServerEvent_AppStart::ProcessEventL()
	{
	LOG2(CVwsLog::ELoud,_L("Started processing of app start event"));

	iSessionWithNewApp=iServer.SessionByUid(iAppToStart);
	if (iSessionWithNewApp==NULL)
		{
		RequestAppStart();
		}
	else
		{
		Complete(KErrNone);
		}
	}

void CVwsServerEvent_AppStart::HandleViewAdded(const TVwsViewId& aViewId, const TThreadId& aNewViewClientThreadId)
	{
	ASSERT(iState==EAppStartRequested);

	if (aViewId.iAppUid==iAppToStart)
		{
		delete iThreadWatcher;
		iThreadWatcher=NULL;
		iServer.SetViewAdditionObserver(NULL);
		ASSERT(iSessionWithNewApp==NULL);
		iSessionWithNewApp=iServer.SessionByThreadIdAndAppUid(aNewViewClientThreadId,aViewId.iAppUid);
		ASSERT(iSessionWithNewApp);
		Complete(KErrNone);
		}
	}

void CVwsServerEvent_AppStart::RequestAppStart()
	{
	iServer.SetViewAdditionObserver(this);
	TThreadId appThreadId;
	TInt err=iServer.StartApp(iAppToStart,appThreadId);
	if (err)
		{
		LOG3(CVwsLog::EQuiet,_L("App start FAILED with %d"),err);
		Complete(err);
		}
	else
		{
		iState=EAppStartRequested;
		err=iThreadWatcher->StartWatching(appThreadId,TCallBack(&AppThreadWatcherCallBack,this));
		if (err)
			{
			LOG3(CVwsLog::EQuiet,_L("App start FAILED with %d"),err);
			Complete(err);
			}
		}
	}

TInt CVwsServerEvent_AppStart::AppThreadWatcherCallBack(TAny* aPtr)
	// static
	{
	LOG2(CVwsLog::EQuiet,_L("App start FAILED with app client thread death"));
	REINTERPRET_CAST(CVwsServerEvent_AppStart*,aPtr)->Complete(KErrCancel);
	return 0;
	}

void CVwsServerEvent_AppStart::Complete(TInt aErr)
	{
	LOG3(CVwsLog::EQuiet,_L("App start event completed with %d"),aErr);
	iServer.SetViewAdditionObserver(NULL);
	iMessage.Complete(aErr);
	ReportEventProcessed();
	}
