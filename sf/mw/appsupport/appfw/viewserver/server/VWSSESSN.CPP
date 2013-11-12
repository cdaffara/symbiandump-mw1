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

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include "vwsinternal.h"
#include "vwsdefpartner.h"
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS

#include "VWSSESSN.H"
#include "VWSERVER.H"
#include "VWSQUEUE.H"
#include "VWSSEVNT.H"
#include "VWSPRIV.H"
#include "VWSDEBUG.H"



//
// TVwsViewSwitchNotification
//

TVwsViewSwitchNotification::TVwsViewSwitchNotification()
	: iViewId(KNullViewId), iIsOutstanding(EFalse)
	{}

void TVwsViewSwitchNotification::SetRequest(const TVwsViewId& aViewId)
	{
	iViewId=aViewId;
	iIsOutstanding=ETrue;
	}

void TVwsViewSwitchNotification::ClearRequest()
	{
	iViewId=KNullViewId;
	iIsOutstanding=EFalse;
	}

TBool TVwsViewSwitchNotification::IsViewToNotify(const TVwsViewId& aViewId) const
	{
	return (iIsOutstanding && (aViewId==iViewId || iViewId==KNullViewId));
	}


//
// CVwsSession.
//

const TInt KVwsViewArrayGranularity=4;


CVwsSession::~CVwsSession()
	{
	iIsExiting=ETrue;
	iServer.HandleSessionRemoval(iClientThreadId);
	delete iEventQueue;
	delete iClientMessage;
	}

CVwsSession* CVwsSession::NewL(const TThreadId& aThreadId,CVwsServer& aServer)
	{
	CVwsSession* self=new(ELeave) CVwsSession(aThreadId,aServer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

CVwsSession::CVwsSession(const TThreadId& aThreadId,CVwsServer& aServer)
	:iServer(aServer),
	iViewIdArray(KVwsViewArrayGranularity),
	iActiveViewIndex(-1),
	iLastActiveViewIndex(-1),
	iClientThreadId(aThreadId),
	iIsExiting(EFalse)
	{
	}

void CVwsSession::ConstructL()
	{
#ifdef __DO_LOGGING__
	iEventQueue=new(ELeave) CVwsEventQueue(_L("Session Queue (client unknown)"));
#else
	iEventQueue=new(ELeave) CVwsEventQueue;
#endif
	}

void CVwsSession::ServiceL(const RMessage2& aMessage)
	{
	TBool completeMessage=ETrue;
	switch (aMessage.Function())
		{
		case EVwsAsynchronousMessageForServerToPanicClientWith:
			iPanicMessage=aMessage;
			completeMessage=EFalse;
			break;
		case EVwsClose:
			CActiveScheduler::Stop();
			break;
		case EVwsAddView:
			AddViewL(aMessage);
			break;
		case EVwsSetSystemDefaultView:
			SetSystemDefaultViewL(aMessage);
			break;
		case EVwsGetSystemDefaultView:
			GetSystemDefaultViewL(aMessage);
			break;
		case EVwsRemoveView:
			RemoveViewL(aMessage);
			break;
		case EVwsRequestViewEvent:
			RequestViewEventL(aMessage);
			completeMessage=EFalse;
			break;
		case EVwsRequestViewEventCancel:
			CancelRequestViewEvent();
			break;
		case EVwsActivateView:
			ActivateViewL(aMessage,ECompleteRequest);
			completeMessage=EFalse;
			break;
		case EVwsCreateActivateViewEvent:
			ActivateViewL(aMessage,EDoNotCompleteRequest);
			break;
		case EVwsRequestCustomMessage:
			RequestCustomMessageL(aMessage);
			break;
		case EVwsStartApp:
			StartAppL(aMessage);
			completeMessage=EFalse;
			break;
		case EVwsDeactivateActiveView:
			DeactivateActiveViewL(aMessage,ECompleteRequest);
			completeMessage=EFalse;
			break;
		case EVwsDeactivateActiveViewIfOwnerMatch:
 			DeactivateActiveViewIfOwnerMatchL(aMessage,ECompleteRequest);
 			completeMessage=EFalse;
 			break;
		case EVwsCreateDeactivateViewEvent:
			DeactivateActiveViewL(aMessage,EDoNotCompleteRequest);
			break;
		case EVwsNotifyNextDeactivation:
			NotifyNextDeactivationL(aMessage);
			break;
		case EVwsNotifyNextActivation:
			NotifyNextActivationL(aMessage);
			break;
		case EVwsSetClientRequestTimeOut:
			SetClientRequestTimeOut(aMessage);
			break;
		case EVwsSetServerEventTimeOut:
			SetServerEventTimeOut(aMessage);
			break;
		case EVwsEnableServerEventTimeOut:
			EnableServerEventTimeOut(aMessage);
			break;
		case EVwsCheckSourceOfViewSwitch:
			CheckSourceOfViewSwitchL(aMessage);
			break;
		case EVwsPriority:
			GetPriorityForActiveObjectL(aMessage);
			break; 
		case EVwsEnableServerBlankScreen:
			EnableServerBlankScreen(aMessage);
			break;
		case EVwsSetProtected:
			SetProtectedL(aMessage);
			break;
		case EVwsSetCrossCheckUid:
			iServer.SetCrossCheckUid(aMessage);
			break;
		case EVwsSetWindowBackgroundColor:
			iServer.SetWindowBackgroundColor(aMessage);
			break;
		case EVwsCurrentActiveViewId:
			GetCurrentActiveViewIdL(aMessage);
			break;
		default:
			PanicClient(aMessage,EVwsBadRequest);
		}
	if (completeMessage && !aMessage.IsNull())
		{
		LOG3(CVwsLog::ENormal,_L("Auto completing with %d"),KErrNone);
		aMessage.Complete(KErrNone);
		}
	}

void CVwsSession::ServiceError(const RMessage2& aMessage,TInt aError)
	{
	LOG3(CVwsLog::ENormal,_L("Auto completing with %d"),aError);
	CSession2::ServiceError(aMessage,aError);
	}


void CVwsSession::PanicClient(const RMessage2& aMessage,TInt aPanic)
	{
	if (!aMessage.IsNull())
		{
		aMessage.Panic(_L("ViewSrv"),aPanic);
		}
	}

void CVwsSession::PanicClient(TInt aPanic)
	{
	__ASSERT_DEBUG(aPanic!=0,User::Invariant());
	PanicClient(iPanicMessage,aPanic);
	}


TUid CVwsSession::AppUid() const
	{
	return iAppUid;
	}

CVwsSession::TState CVwsSession::State() const
	{
	return iState;
	}

void CVwsSession::RequestClientActivationL(MVwsSessionObserver& aObserver,const TVwsViewId& aViewId,
										   const TVwsViewId& aPreviousViewId,CVwsClientMessage* aClientMessage,RThread aThreadOfClientInitiatingViewSwitch)
	{
	User::LeaveIfError(aThreadOfClientInitiatingViewSwitch.Duplicate(RThread()));
	CleanupClosePushL(aThreadOfClientInitiatingViewSwitch);
	CVwsSessionEvent* activationEvent=new(ELeave) CVwsSessionEvent_Activation(*this,*iEventQueue,aObserver,aViewId,aPreviousViewId,aClientMessage,aThreadOfClientInitiatingViewSwitch);
	aObserver.NowObserving(activationEvent);
	CleanupStack::Pop(&aThreadOfClientInitiatingViewSwitch);
	TRAPD(err, iEventQueue->ProcessEventL(activationEvent))
	if(err!=KErrNone)
		{
		iLeaveAfterOwnershipTaken=ETrue;
		User::Leave(err);
		};
	}

void CVwsSession::RequestClientDeactivationL(MVwsSessionObserver& aObserver,const TVwsViewId& aViewId,const TVwsViewId& aActiveViewId, TBool aDifferentInstanceOfSameApp)
	{
	CVwsSessionEvent* deactivationEvent=new(ELeave) CVwsSessionEvent_Deactivation(*this,*iEventQueue,aObserver,aViewId,aActiveViewId, aDifferentInstanceOfSameApp);
	aObserver.NowObserving(deactivationEvent);
	iEventQueue->ProcessEventL(deactivationEvent);
	}

void CVwsSession::RequestScreenDeviceChangeNotificationL(MVwsSessionObserver& aObserver,const TVwsViewId& aViewId)
	{
	CVwsSessionEvent* screenDeviceChangeEvent=new(ELeave) CVwsSessionEvent_ScreenDeviceChangeNotification(*this,*iEventQueue,aObserver,aViewId);
	aObserver.NowObserving(screenDeviceChangeEvent);
	iEventQueue->ProcessEventL(screenDeviceChangeEvent);
	}

void CVwsSession::SetMessageHandler(MVwsMessageHandler& aMessageHandler)
	{
	ASSERT(iMessageHandler==NULL);
	iMessageHandler=&aMessageHandler;
	}

void CVwsSession::ClearMessageHandler()
	{
	ASSERT(iMessageHandler);
	iMessageHandler=NULL;
	}

void CVwsSession::AddViewL(const RMessage2& aMessage)
	{
	TVwsViewId viewId(ViewIdFromMessageL(aMessage));

	if (iAppUid.iUid==0)
		{
		iAppUid.iUid=viewId.iAppUid.iUid;
#ifdef __DO_LOGGING__
		TBuf<64> queueName;
		queueName.Format(_L("Session Queue for \"%x\""),iAppUid.iUid);
		iEventQueue->SetName(queueName);
#endif
		}
	else if (iAppUid!=viewId.iAppUid)
		{
		// All views added by the same client should have the same app uid.
		PanicClient(aMessage,EVwsInvalidViewUid);
		}

	LOG4(CVwsLog::ENormal,_L("Adding view \"%x,%x\""),viewId.iAppUid.iUid,viewId.iViewUid.iUid);
	AddViewL(viewId);
	}

void CVwsSession::RemoveViewL(const RMessage2& aMessage)
	{
	TVwsViewId viewId(ViewIdFromMessageL(aMessage));
	LOG4(CVwsLog::ENormal,_L("Removing view \"%x,%x\""),viewId.iAppUid.iUid,viewId.iViewUid.iUid);
	if (RemoveView(aMessage,viewId)==KErrNotFound)
		{
		PanicClient(aMessage,EVwsViewNotFound);
		}
	}

void CVwsSession::SetSystemDefaultViewL(const RMessage2& aMessage)
	{
	TPckgBuf<TVwsViewId> viewId;
	aMessage.ReadL(0,viewId);
	iServer.SetSystemDefaultViewL(aMessage.Int1(),viewId());
	}

void CVwsSession::GetSystemDefaultViewL(const RMessage2& aMessage)
	{
	TVwsViewId viewId;
	iServer.GetSystemDefaultView(viewId);
	TRAP_IGNORE(aMessage.WriteL(0,TPckgC<TVwsViewId>(viewId)));
	}

void CVwsSession::RequestViewEventL(const RMessage2& aMessage)
	{
	iViewEventMessage=aMessage;
	const TInt error=aMessage.Int1();
	switch (iState)
		{
		case EWaitingForClientRequest:
			{
			LOG4(CVwsLog::ENormal,_L("Client \"%x\" requested view event [session state: EWaitingClientRequest, last error: %d]"),iAppUid.iUid,error);
			iState=EClientRequestPending;
			CVwsEvent* headEvent=iEventQueue->Head();
			if (headEvent)
				{
				STATIC_CAST(CVwsSessionEvent*,headEvent)->HandleViewEventRequestL(error,iViewEventMessage);
				}
			}
			break;
		case EClientRequestPending:
			LOG3(CVwsLog::ENormal,_L("PANIC Client \"%x\" for requesting view event when one is already pending"),iAppUid.iUid);
			PanicClient(aMessage,EVwsViewEventRequestAlreadyPending);
			break;
		default:
			ASSERT(EFalse);
		}
	}

void CVwsSession::CancelRequestViewEvent()
	{
	LOG3(CVwsLog::ENormal,_L("Client \"%x\" requested cancelation of view event request"),iAppUid.iUid);
	if (iState==EClientRequestPending)
		{
		CompleteViewEvent(KErrCancel);
		}
	}

void CVwsSession::CompleteViewEvent(TInt aNotification)
	{
	ASSERT(iState==EClientRequestPending);

	if (iState==EClientRequestPending)
		{
		LOG4(CVwsLog::ENormal,_L("Completing view event in client \"%x\" with \"%d\""),iAppUid.iUid,aNotification);
		iViewEventMessage.Complete(aNotification);
		iState=EWaitingForClientRequest;
		}
	else
		{
		LOG3(CVwsLog::ELoud,_L("Completing view event in client \"%x\" - ERROR: No pending client request!"),iAppUid.iUid);
		}
	}

void CVwsSession::CompleteViewEventL(TInt aNotification,const TVwsViewEvent& aEvent)
	{
	ASSERT(iState==EClientRequestPending);

	if (aNotification==KErrNone && iState==EClientRequestPending)
		{
		LOG3(CVwsLog::ENormal,_L("Writing view event buffer to client \"%x\""),iAppUid.iUid);
		iViewEventMessage.WriteL(0,TPckgC<TVwsViewEvent>(aEvent));
		}
	CompleteViewEvent(aNotification);
	}

void CVwsSession::ActivateViewL(const RMessage2& aMessage,TVwsCompleteRequest aCompleteRequest)
	{
	TPckgBuf<TVwsViewId> viewId;
	aMessage.ReadL(0,viewId);
	LOG5(CVwsLog::ENormal,_L("Client \"%x\" requested activation of \"%x,%x\""),iAppUid.iUid,viewId().iAppUid.iUid,viewId().iViewUid.iUid);
	CVwsClientMessage* clientMessage=CVwsClientMessage::NewL(TUid::Uid(aMessage.Int1()),User::LeaveIfError(aMessage.GetDesLength(2)),aMessage,2);
	LOG2(CVwsLog::ENormal,_L("Allocated custom message"));
	iServer.ActivateViewL(viewId(),clientMessage,aMessage,*this,aCompleteRequest);
	}

void CVwsSession::RequestCustomMessageL(const RMessage2& aMessage)
	{
	if (iMessageHandler==NULL)
		{
		User::Leave(KErrUnknown);
		}
	iMessageHandler->WriteClientMessageL(aMessage);
	}

TVwsViewId CVwsSession::ViewIdFromMessageL(const RMessage2& aMessage)
	{
	TPckgBuf<TVwsViewId> viewIdBuf;
	aMessage.ReadL(0,viewIdBuf);
	return viewIdBuf();
	}

void CVwsSession::StartAppL(const RMessage2& aMessage)
	{
	TUid appToStart={aMessage.Int0()};
	iServer.RequestAppStartL(aMessage,appToStart);
	}

void CVwsSession::DeactivateActiveViewL(const RMessage2& aMessage,TVwsCompleteRequest aCompleteRequest)
	{
	iServer.RequestDeactivateActiveViewL(aMessage,*this,aCompleteRequest);
	}
void CVwsSession::DeactivateActiveViewIfOwnerMatchL(const RMessage2& aMessage,TVwsCompleteRequest aCompleteRequest)
 	{
 	if(iServer.ActiveViewSession() == this)
 		{
 		return DeactivateActiveViewL(aMessage, aCompleteRequest);
 		}
 	else if(aCompleteRequest==ECompleteRequest)
 		{
 		aMessage.Complete(KErrNone);
 		}
 	}

void CVwsSession::NotifyNextDeactivationL(const RMessage2& aMessage)
	{
	iDeactivationNotification.SetRequest(ViewIdFromMessageL(aMessage));
	}

void CVwsSession::HandleDeactivationL(const TVwsViewId& aDeactivatedViewId, const TVwsViewId& aActivatedViewId)
	{
	if (iDeactivationNotification.IsViewToNotify(aDeactivatedViewId))
		{
		LOG3(CVwsLog::ENormal,_L("Requesting deactivation notification in \"%x\""),iAppUid);
		iDeactivationNotification.ClearRequest();
		CVwsSessionEvent* deactivationNotificationEvent=new(ELeave) CVwsSessionEvent_DeactivationNotification(*this,*iEventQueue,aDeactivatedViewId,aActivatedViewId);
		iEventQueue->ProcessEventL(deactivationNotificationEvent);
		}
	}

void CVwsSession::NotifyNextActivationL(const RMessage2& aMessage)
	{
	iActivationNotification.SetRequest(ViewIdFromMessageL(aMessage));
	}

void CVwsSession::HandleActivationL(const TVwsViewId& aActivatedViewId, const TVwsViewId& aViewToBeDeactivatedId)
	{
	if (iActivationNotification.IsViewToNotify(aActivatedViewId))
		{
		LOG3(CVwsLog::ENormal,_L("Requesting activation notification in \"%x\""),iAppUid);
		iActivationNotification.ClearRequest();
		CVwsSessionEvent* activationNotificationEvent=new(ELeave) CVwsSessionEvent_ActivationNotification(*this,*iEventQueue,aActivatedViewId,aViewToBeDeactivatedId);
		iEventQueue->ProcessEventL(activationNotificationEvent);
		}
	}

void CVwsSession::SetClientRequestTimeOut(const RMessage2& aMessage)
	{
	iServer.SetClientRequestTimeOut(TTimeIntervalMicroSeconds32(REINTERPRET_CAST(TInt32,aMessage.Ptr0())));
	}

void CVwsSession::SetServerEventTimeOut(const RMessage2& aMessage)
	{
	iServer.SetServerEventTimeOut(TTimeIntervalMicroSeconds32(REINTERPRET_CAST(TInt32,aMessage.Ptr0())));
	}

void CVwsSession::EnableServerEventTimeOut(const RMessage2& aMessage)
	{
	iServer.EnableServerEventTimeOut(TBool(REINTERPRET_CAST(TInt32,aMessage.Ptr0())));
	}

void CVwsSession::EnableServerBlankScreen(const RMessage2& aMessage)
	{
	iServer.EnableServerBlankScreen(aMessage.Int0());
	}

void CVwsSession::CheckSourceOfViewSwitchL(const RMessage2& aMessage)
	{
	if (iMessageHandler==NULL)
		{
		User::Leave(KErrUnknown);
		}
	iMessageHandler->CheckSourceOfViewSwitchL(aMessage);
	}

void CVwsSession::SetProtectedL(const RMessage2& aMessage)
	{
	iProtected = (aMessage.Int0() != 0);
	}


TInt CVwsSession::IndexById(const TVwsViewId& aViewId) const
	{
	const TInt numViews=iViewIdArray.Count();
	for (TInt ii=0;ii<numViews;ii++)
		{
		if (iViewIdArray[ii] == aViewId)
			{
			return ii;
			}
		}

	return KErrNotFound;
	}

TInt CVwsSession::GetTopView(TVwsViewId& aViewId)
	{
	TInt ret=KErrNone;
	const TInt numViews=iViewIdArray.Count();
	if (numViews && iLastActiveViewIndex!=-1 && iLastActiveViewIndex<numViews)
		{
		aViewId=iViewIdArray[(iLastActiveViewIndex) ? iLastActiveViewIndex : 0];
		}
	else
		{
		ret=KErrNotFound;
		}
	return ret;
	}

void CVwsSession::AddViewL(const TVwsViewId& aViewId)
	{
	iViewIdArray.AppendL(aViewId);
	iServer.HandleSessionViewAddition(aViewId, iClientThreadId);
	}

TInt CVwsSession::RemoveView(const RMessage2& /*aMessage*/,const TVwsViewId& aViewId)
	{
	TBool systemView=iServer.IsSystemDefaultView(aViewId);
	TInt index=IndexById(aViewId);
	if (index==iActiveViewIndex && !systemView)
		{
		LOG4(CVwsLog::ENormal,_L("Not removing the currently active view [\"%x,%x\"]"),aViewId.iAppUid.iUid,aViewId.iViewUid.iUid);
		return KErrInUse;
		}
	else if (index>=0)
		{
		iViewIdArray.Delete(index);
		if (iActiveViewIndex>index)
			{
			iActiveViewIndex--;
			iLastActiveViewIndex--;
			}
#ifdef _DEBUG
		if (systemView)
			{
			LOG4(CVwsLog::ENormal,_L("Removed system default view [\"%x,%x\"]"),aViewId.iAppUid.iUid,aViewId.iViewUid.iUid);
			}
#endif
		return KErrNone;
		}

	return KErrNotFound;
	}

void CVwsSession::SetActiveView(const TVwsViewId& aViewId)
	{
	iActiveViewIndex=IndexById(aViewId);
	iLastActiveViewIndex=iActiveViewIndex;
	}

TVwsViewId CVwsSession::ActiveView() const
	{
	if (iViewIdArray.Count()>0 && iActiveViewIndex>=0)
		{
		return iViewIdArray[iActiveViewIndex];
		}

	return KNullViewId;
	}

void CVwsSession::ClearActiveView()
	{
	iActiveViewIndex=-1;
	}

TBool CVwsSession::HasActiveView() const
	{
	return (iActiveViewIndex==-1) ? EFalse : ETrue;
	}

TInt CVwsSession::CheckViewExists(const TVwsViewId& aViewId) const
	{
	return IndexById(aViewId);
	}

TBool CVwsSession::IsViewActive(const TVwsViewId& aViewId) const
	{
	if (iViewIdArray.Count()>0 && iActiveViewIndex>=0)
		{
		if (iViewIdArray[iActiveViewIndex]==aViewId)
			{
			return ETrue;
			}
		}

	return EFalse;
	}

TBool CVwsSession::Protected() const
	{
	return iProtected;
	}


//
// CVwsClientMessage.
//

CVwsClientMessage* CVwsClientMessage::New()
	{
	CVwsClientMessage* self=new CVwsClientMessage();
	return self;
	}

CVwsClientMessage* CVwsClientMessage::NewL()
	{
	CVwsClientMessage* self=new(ELeave) CVwsClientMessage();
	return self;
	}

CVwsClientMessage* CVwsClientMessage::NewL(const TUid& aMessageId)
	{
	CVwsClientMessage* self=new(ELeave) CVwsClientMessage(aMessageId);
	return self;
	}

CVwsClientMessage* CVwsClientMessage::NewL(const TUid& aMessageId,TInt aMessageLength,const RMessage2& aMessage,TInt aParameter)
	{
	CVwsClientMessage* self=new(ELeave) CVwsClientMessage(aMessageId);
	CleanupStack::PushL(self);
	self->ConstructL(aMessageLength,aMessage,aParameter);
	CleanupStack::Pop();
	return self;
	}


CVwsClientMessage::~CVwsClientMessage()
	{
	delete iMessage;
	}

CVwsClientMessage::CVwsClientMessage()	
	{
	}

CVwsClientMessage::CVwsClientMessage(const TUid& aMessageId)
	: iMessageId(aMessageId)
	{
	}

void CVwsClientMessage::ConstructL(TInt aMessageLength,const RMessage2& aMessage,TInt aParameter)
	{
	if (aMessageLength)
		{
		iMessage=HBufC8::NewL(aMessageLength);
		TPtr8 ptr(iMessage->Des());
		aMessage.ReadL(aParameter,ptr);
		}
	}

void CVwsSession::GetPriorityForActiveObjectL(const RMessage2& aMessage)
	{
	TInt priority;
	iServer.GetPriorityForActiveObjectL(priority);
	aMessage.WriteL(0,TPckgBuf<TInt>(priority));
	}

void CVwsSession::GetCurrentActiveViewIdL(const RMessage2& aMessage)
	{
	TVwsViewId activeViewId;
	iServer.GetCurrentActiveViewId(activeViewId);
	aMessage.WriteL(0,TPckgBuf<TVwsViewId>(activeViewId));
	}
