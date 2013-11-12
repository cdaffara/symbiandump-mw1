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
#include "VWSSEVNT.H"
#include "VWSDEBUG.H"


//
// CVwsSessionEvent.
//

CVwsSessionEvent::CVwsSessionEvent(CVwsSession& aSession,TType aType,CVwsEventQueue& aQueue)
	:CVwsEvent(aType,aQueue),iSession(aSession)
	{
	}

CVwsSessionEvent::CVwsSessionEvent(CVwsSession& aSession,TType aType,CVwsEventQueue& aQueue,MVwsSessionObserver& aObserver)
	:CVwsEvent(aType,aQueue),iSession(aSession),iObserver(&aObserver)
	{
	}

CVwsSessionEvent::~CVwsSessionEvent()
	{
	}

void CVwsSessionEvent::ReportRequestCompleted(MVwsSessionObserver::TRequestType aType,TInt aError,const RMessage2& aMessage)
	{
	if (iObserver)
		{
		iObserver->RequestCompleted(aType,aError,aMessage);
		}
	}

void CVwsSessionEvent::NullifyObserver()
	{
	iObserver = NULL;
	}

//
// CVwsSessionEvent_Activation.
//

CVwsSessionEvent_Activation::CVwsSessionEvent_Activation(CVwsSession& aSession,CVwsEventQueue& aQueue,MVwsSessionObserver& aObserver,const TVwsViewId& aViewId,const TVwsViewId& aPreviousViewId,CVwsClientMessage* aClientMessage,const RThread& aThreadOfClientInitiatingViewSwitch)
	:CVwsSessionEvent(aSession,CVwsEvent::ENormal,aQueue,aObserver),iViewId(aViewId),iPreviousViewId(aPreviousViewId),iClientMessage(aClientMessage), iThreadOfClientInitiatingViewSwitch(aThreadOfClientInitiatingViewSwitch)
 	{
	iSession.SetMessageHandler(*this);
	}

CVwsSessionEvent_Activation::~CVwsSessionEvent_Activation()
	{
	//When a session event is completed, the observer(ie., the server event) of this session event is made as NULL.
	if (iObserver)
		{
		iObserver->NullifyObserved();
		}
	delete iClientMessage;
	iThreadOfClientInitiatingViewSwitch.Close();
	}

void CVwsSessionEvent_Activation::ProcessEventL()
	{
	ASSERT(iState==EPending);
	LOG6(CVwsLog::ENormal,_L("Starting processing of session activation event of \"%x,%x\" [previous view: \"%x,%x\"]"),
		iViewId.iAppUid.iUid,iViewId.iViewUid.iUid,iPreviousViewId.iAppUid.iUid,iPreviousViewId.iViewUid.iUid);
	
	if (iSession.State()==CVwsSession::EClientRequestPending)
		{	// Client request already pending so request activation imemdiately.
		RequestActivationL();
		}
	else
		{	// Otherwise, wait for call to HandleViewEventRequestL from session.
		iState=EWaitingForClientViewEventRequest;
		}
	}

void CVwsSessionEvent_Activation::HandleViewEventRequestL(TInt aError,const RMessage2& aMessage)
	{
	switch (iState)
		{
		default:
		case EPending:
			ASSERT(EFalse);
			break;
		case EWaitingForClientViewEventRequest:
			RequestActivationL();
			break;
		case EWaitingForCustomMessageCollection:
			LOG4(CVwsLog::ENormal,_L("CLIENT PANIC: Client \"%x\" requested view event [session state: EWaitingForCustomMessageCollection, last error: %d]"),iSession.AppUid().iUid,aError);
			if (aError==KErrNone)
				{
				CVwsSession::PanicClient(aMessage,EVwsCustomMessageNotCollected);
				}
			Complete(aError,aMessage);
			break;
		case EWaitingForAcknowledgementOfActivation:
			Complete(aError,aMessage);
			break;
		}
	}

void CVwsSessionEvent_Activation::WriteClientMessageL(const RMessage2& aMessage)
	{
	ASSERT(iState==EWaitingForCustomMessageCollection);
	LOG3(CVwsLog::ENormal,_L("Client \"%x\" requested message"),iSession.AppUid().iUid);
	TRAP_IGNORE(aMessage.WriteL(0,*iClientMessage->iMessage));
	iState=EWaitingForAcknowledgementOfActivation;
	}

void CVwsSessionEvent_Activation::CheckSourceOfViewSwitchL(const RMessage2& aMessage)
	{
	LOG3(CVwsLog::ENormal,_L("Client \"%x\" checked source of view-switch"),iSession.AppUid().iUid);
	TPckgBuf<TSecurityPolicy> securityPolicy;
	aMessage.ReadL(0,securityPolicy);
	const char* diagnostic_charString=NULL;
	HBufC8* diagnostic_HBufC8=NULL;
	if (aMessage.Int1() == reinterpret_cast<TInt>(KSuppressPlatSecDiagnosticMagicValue))
   		{
   		diagnostic_charString = KSuppressPlatSecDiagnosticMagicValue;
   		}
	else if (aMessage.Ptr1()!=NULL)
		{
		diagnostic_HBufC8=HBufC8::NewLC(aMessage.GetDesLengthL(1));
		TPtr8 diagnostic_TPtr8(diagnostic_HBufC8->Des());
		aMessage.ReadL(1,diagnostic_TPtr8);
		diagnostic_TPtr8.ZeroTerminate();
		diagnostic_charString=REINTERPRET_CAST(const char*,diagnostic_HBufC8->Ptr());
		}
	TPckgBuf<TBool> result;
	result = securityPolicy().CheckPolicy(iThreadOfClientInitiatingViewSwitch,diagnostic_charString);
	aMessage.WriteL(2,result);
	aMessage.Complete(KErrNone);
	if (diagnostic_HBufC8!=NULL)
		{
		CleanupStack::PopAndDestroy(diagnostic_HBufC8);
		}
	}

void CVwsSessionEvent_Activation::RequestActivationL()
	{
	LOG6(CVwsLog::ENormal,_L("Requesting activation of \"%x,%x\" [previous view: \"%x,%x\"]"),
		iViewId.iAppUid.iUid,iViewId.iViewUid.iUid,iPreviousViewId.iAppUid.iUid,iPreviousViewId.iViewUid.iUid);

	TVwsViewEvent viewEvent(TVwsViewEvent::EVwsActivateView,iViewId,iPreviousViewId,iClientMessage->iMessageId,(iClientMessage->iMessage==NULL)? 0: iClientMessage->iMessage->Length());
	iSession.CompleteViewEventL(KErrNone,viewEvent);
	iState=(iClientMessage->iMessage!=NULL)? EWaitingForCustomMessageCollection: EWaitingForAcknowledgementOfActivation;
 
	}

void CVwsSessionEvent_Activation::Complete(TInt aError,const RMessage2& aMessage)
	{
	LOG4(CVwsLog::ENormal,_L("Completed session activation event [Client: %x, Client error: %d]"),iSession.AppUid().iUid,aError);
	iSession.ClearMessageHandler();
	ReportRequestCompleted(MVwsSessionObserver::EActivation,aError,aMessage);
	ReportEventProcessed();
	}
#ifdef _DEBUG
void CVwsSessionEvent_Activation::Complete(TInt aError)
#else
void CVwsSessionEvent_Activation::Complete(TInt /*aError*/)
#endif // _DEBUG
	{
	LOG4(CVwsLog::ENormal,_L("Completed session activation event [Client: %x, Client error: %d]"),iSession.AppUid().iUid,aError);
	iSession.ClearMessageHandler();
	ReportEventProcessed();
// Disable unused parameter warning
#pragma warning (disable: 4100) 
	}
#pragma warning (default: 4100)

//
// CVwsSessionEvent_Deactivation.
//

CVwsSessionEvent_Deactivation::CVwsSessionEvent_Deactivation(CVwsSession& aSession,CVwsEventQueue& aQueue,MVwsSessionObserver& aObserver,const TVwsViewId& aViewId,const TVwsViewId& aActiveViewId, TBool aDifferentInstanceOfSameApp)
	:CVwsSessionEvent(aSession,CVwsEvent::ENormal,aQueue,aObserver),iViewId(aViewId),iActiveViewId(aActiveViewId),iDifferentInstanceOfSameApp(aDifferentInstanceOfSameApp)
	{
	}

CVwsSessionEvent_Deactivation::~CVwsSessionEvent_Deactivation()
	{
	//When a session event is completed, the observer(ie., the server event) of this session event is made as NULL.
	if (iObserver)
		{
		iObserver->NullifyObserved();
		}
	}

void CVwsSessionEvent_Deactivation::ProcessEventL()
	{
	ASSERT(iState==EPending);
	LOG6(CVwsLog::ENormal,_L("Starting processing of session deactivation event of \"%x,%x\" [view about to become active: \"%x,%x\"]"),
		iViewId.iAppUid.iUid,iViewId.iViewUid.iUid,iActiveViewId.iAppUid.iUid,iActiveViewId.iViewUid.iUid);

	if (iSession.State()==CVwsSession::EClientRequestPending)
		{	// Client request already pending so request deactivation immediately.
		RequestDeactivationL();
		}
	else
		{	// Otherwise, wait for call to HandleViewEventRequestL from session.
		iState=EWaitingForClientViewEventRequest;
		}
	}

void CVwsSessionEvent_Deactivation::HandleViewEventRequestL(TInt aError,const RMessage2& aMessage)
	{
	switch (iState)
		{
		default:
		case EPending:
			ASSERT(EFalse);
			break;
		case EWaitingForClientViewEventRequest:
			RequestDeactivationL();
			break;
		case EWaitingForAcknowledgementOfDeactivation:
			Complete(aError,aMessage);
			break;
		}
	}

void CVwsSessionEvent_Deactivation::RequestDeactivationL()
	{
	LOG4(CVwsLog::ENormal,_L("Starting processing of session deactivation event of \"%x,%x\""),iViewId.iAppUid.iUid,iViewId.iViewUid.iUid);
	if (iDifferentInstanceOfSameApp)
		iSession.CompleteViewEventL(KErrNone,TVwsViewEvent(TVwsViewEvent::EVwsDeactivateViewDifferentInstance,iViewId,iActiveViewId));
    else
		iSession.CompleteViewEventL(KErrNone,TVwsViewEvent(TVwsViewEvent::EVwsDeactivateView,iViewId,iActiveViewId));
	iState=EWaitingForAcknowledgementOfDeactivation;
	}

void CVwsSessionEvent_Deactivation::Complete(TInt aError,const RMessage2& aMessage)
	{
	LOG4(CVwsLog::ENormal,_L("Completed session deactivation event [Client: %x, Client error: %d]"),iSession.AppUid().iUid,aError);
	ReportRequestCompleted(MVwsSessionObserver::EDeactivation,aError,aMessage);
	ReportEventProcessed();
	}


//
// CVwsSessionEvent_ScreenDeviceChangeNotification.
//

CVwsSessionEvent_ScreenDeviceChangeNotification::CVwsSessionEvent_ScreenDeviceChangeNotification(CVwsSession& aSession,CVwsEventQueue& aQueue,MVwsSessionObserver& aObserver,const TVwsViewId& aViewId)
	:CVwsSessionEvent(aSession,CVwsEvent::ENormal,aQueue,aObserver),iViewId(aViewId)
	{
	}

CVwsSessionEvent_ScreenDeviceChangeNotification::~CVwsSessionEvent_ScreenDeviceChangeNotification()
	{
	//When a session event is completed, the observer(ie., the server event) of this session event is made as NULL.
	if (iObserver)
		{
		iObserver->NullifyObserved();
		}
	}

void CVwsSessionEvent_ScreenDeviceChangeNotification::ProcessEventL()
	{
	ASSERT(iState==EPending);
	LOG4(CVwsLog::ENormal,_L("Starting processing of session screen device changed notification event in \"%x,%x\""),iViewId.iAppUid.iUid,iViewId.iViewUid.iUid);

	if (iSession.State()==CVwsSession::EClientRequestPending)
		{	// Client request already pending so request screen device change notification immediately.
		RequestScreenDeviceChangeNotificationL();
		}
	else
		{	// Otherwise, wait for call to HandleViewEventRequestL from session.
		iState=EWaitingForClientViewEventRequest;
		}
	}

void CVwsSessionEvent_ScreenDeviceChangeNotification::HandleViewEventRequestL(TInt aError,const RMessage2& aMessage)
	{
	switch (iState)
		{
		default:
		case EPending:
			ASSERT(EFalse);
			break;
		case EWaitingForClientViewEventRequest:
			RequestScreenDeviceChangeNotificationL();
			break;
		case EWaitingForAcknowledgementOfScreenDeviceChange:
			Complete(aError,aMessage);
			break;
		}
	}

void CVwsSessionEvent_ScreenDeviceChangeNotification::RequestScreenDeviceChangeNotificationL()
	{
	LOG4(CVwsLog::ENormal,_L("Starting processing of session screen device change notification event [active view: \"%x,%x\"]"),iViewId.iAppUid.iUid,iViewId.iViewUid.iUid);

	iSession.CompleteViewEventL(KErrNone,TVwsViewEvent(TVwsViewEvent::EVwsScreenDeviceChanged,iViewId));
	iState=EWaitingForAcknowledgementOfScreenDeviceChange;
	}

void CVwsSessionEvent_ScreenDeviceChangeNotification::Complete(TInt aError,const RMessage2& aMessage)
	{
	LOG4(CVwsLog::ENormal,_L("Completed session screen device changed notification event [Client: %x, Client error: %d]"),iSession.AppUid().iUid,aError);
	ReportRequestCompleted(MVwsSessionObserver::EScreenDeviceChangeNotification,aError,aMessage);
	ReportEventProcessed();
	}


//
// CVwsSessionEvent_DeactivationNotification.
//

CVwsSessionEvent_DeactivationNotification::CVwsSessionEvent_DeactivationNotification(CVwsSession& aSession,CVwsEventQueue& aQueue,const TVwsViewId& aViewId,const TVwsViewId& aActiveViewId)
	:CVwsSessionEvent(aSession,CVwsEvent::ENormal,aQueue),iViewId(aViewId),iActiveViewId(aActiveViewId)
	{
	}

CVwsSessionEvent_DeactivationNotification::~CVwsSessionEvent_DeactivationNotification()
	{
	//When a session event is completed, the observer(ie., the server event) of this session event is made as NULL.
	if (iObserver)
		{
		iObserver->NullifyObserved();
		}
	}

void CVwsSessionEvent_DeactivationNotification::ProcessEventL()
	{
	LOG3(CVwsLog::ENormal,_L("Starting processing of deactivation notification event in \"%x\""),iSession.AppUid());
	if (iSession.State()==CVwsSession::EClientRequestPending)
		{	// Client request already pending so request deactivation notification immediately.
		RequestDeactivationNotificationL();
		}
	else
		{	// Otherwise, wait for call to HandleViewEventRequestL from session.
		iState=EWaitingForClientViewEventRequest;
		}
	}

void CVwsSessionEvent_DeactivationNotification::HandleViewEventRequestL(TInt aError,const RMessage2&)
	{
	switch (iState)
		{
		default:
		case EPending:
			ASSERT(EFalse);
			break;
		case EWaitingForClientViewEventRequest:
			RequestDeactivationNotificationL();
			break;
		case EWaitingForAcknowledgementOfDeactivationNotification:
			Complete(aError);
			break;
		}
	}

void CVwsSessionEvent_DeactivationNotification::RequestDeactivationNotificationL()
	{
	LOG6(CVwsLog::ENormal,_L("Starting processing of session deactivation notification event [ deactivated view \"%x,%x\", new active view: \"%x,%x\"]"),
		iViewId.iAppUid.iUid,iViewId.iViewUid.iUid,iActiveViewId.iAppUid.iUid,iActiveViewId.iViewUid.iUid);

	iSession.CompleteViewEventL(KErrNone,TVwsViewEvent(TVwsViewEvent::EVwsDeactivationNotification,iViewId,iActiveViewId));
	iState=EWaitingForAcknowledgementOfDeactivationNotification;
	}

#ifdef _DEBUG
void CVwsSessionEvent_DeactivationNotification::Complete(TInt aError)
#else
void CVwsSessionEvent_DeactivationNotification::Complete(TInt /*aError*/)
#endif //_DEBUG
	{
	LOG4(CVwsLog::ENormal,_L("Completed session deactivation notification event [Client: %x, Client error: %d]"),iSession.AppUid().iUid,aError);
	ReportEventProcessed();

// Disable unused parameter warning
#pragma warning (disable: 4100) 
	}
#pragma warning (default: 4100)


//
// CVwsSessionEvent_ActivationNotification.
//

CVwsSessionEvent_ActivationNotification::CVwsSessionEvent_ActivationNotification(CVwsSession& aSession,CVwsEventQueue& aQueue,const TVwsViewId& aViewId,const TVwsViewId& aViewToBeDeactivatedId)
	:CVwsSessionEvent(aSession,CVwsEvent::ENormal,aQueue),iViewId(aViewId),iViewToBeDeactivatedId(aViewToBeDeactivatedId)
	{
	}

CVwsSessionEvent_ActivationNotification::~CVwsSessionEvent_ActivationNotification()
	{
	//When a session event is completed, the observer(ie., the server event) of this session event is made as NULL.
	if (iObserver)
		{
		iObserver->NullifyObserved();
		}
	}

void CVwsSessionEvent_ActivationNotification::ProcessEventL()
	{
	LOG3(CVwsLog::ENormal,_L("Starting processing of activation notification event in \"%x\""),iSession.AppUid());

	if (iSession.State()==CVwsSession::EClientRequestPending)
		{	// Client request already pending so request activation notification immediately.
		RequestActivationNotificationL();
		}
	else
		{	// Otherwise, wait for call to HandleViewEventRequestL from session.
		iState=EWaitingForClientViewEventRequest;
		}
	}

void CVwsSessionEvent_ActivationNotification::HandleViewEventRequestL(TInt aError,const RMessage2&)
	{
	switch (iState)
		{
		default:
		case EPending:
			ASSERT(EFalse);
			break;
		case EWaitingForClientViewEventRequest:
			RequestActivationNotificationL();
			break;
		case EWaitingForAcknowledgementOfActivationNotification:
			Complete(aError);
			break;
		}
	}

void CVwsSessionEvent_ActivationNotification::RequestActivationNotificationL()
	{
	LOG6(CVwsLog::ENormal,_L("Starting processing of session activation notification event [ new active view \"%x,%x\", view to be deactivationprevious view: \"%x,%x\"]"),
		iViewId.iAppUid.iUid,iViewId.iViewUid.iUid,iViewToBeDeactivatedId.iAppUid.iUid,iViewToBeDeactivatedId.iViewUid.iUid);

	iSession.CompleteViewEventL(KErrNone,TVwsViewEvent(TVwsViewEvent::EVwsActivationNotification,iViewId,iViewToBeDeactivatedId));
	iState=EWaitingForAcknowledgementOfActivationNotification;
	}
#ifdef _DEBUG
void CVwsSessionEvent_ActivationNotification::Complete(TInt aError)
#else
void CVwsSessionEvent_ActivationNotification::Complete(TInt /*aError*/)
#endif //_DEBUG
	{
	LOG4(CVwsLog::ENormal,_L("Completed session activation notification event [Client: %x, Client error: %d]"),iSession.AppUid().iUid,aError);
	ReportEventProcessed();

// Disable unused parameter warning
#pragma warning (disable: 4100) 
	}
#pragma warning (default: 4100)
