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


#ifndef __VWSSEVNT_H__
#define __VWSSEVNT_H__

#include <e32std.h>

#include "VWSQUEUE.H"
#include "VWSSESSN.H"


/**
 * The CVwsSessionEvent class is the base class for session events generated WHEN ?
 */
class CVwsSessionEvent : public CVwsEvent
	{
public:
	CVwsSessionEvent(CVwsSession& aSession,TType aType,CVwsEventQueue& aQueue);
	CVwsSessionEvent(CVwsSession& aSession,TType aType,CVwsEventQueue& aQueue,MVwsSessionObserver& aObserver);
	~CVwsSessionEvent();
public:
	virtual void HandleViewEventRequestL(TInt aError,const RMessage2& aMessage)=0;
	void ReportRequestCompleted(MVwsSessionObserver::TRequestType aType,TInt aError,const RMessage2& aMessage);
	void NullifyObserver();
protected:
	CVwsSession& iSession;
	MVwsSessionObserver* iObserver;
	};


/**
 * The CVwsSessionEvent_Activation represents an activation event in a session's event queue
 */
class CVwsSessionEvent_Activation : public CVwsSessionEvent, public MVwsMessageHandler
	{
public:
	CVwsSessionEvent_Activation(CVwsSession& aSession,CVwsEventQueue& aQueue,MVwsSessionObserver& iObserver,const TVwsViewId& aViewId,const TVwsViewId& aPreviousViewId,CVwsClientMessage* aClientMessage,const RThread& aThreadOfClientInitiatingViewSwitch);
	~CVwsSessionEvent_Activation();
private:	// From CVwsEvent.
	virtual void ProcessEventL();
	virtual void Complete(TInt aError);
private:	// From CVwsSessionEvent.
	virtual void HandleViewEventRequestL(TInt aError,const RMessage2& aMessage);
private:	// From MVwsMessageHandler.
	void WriteClientMessageL(const RMessage2& aMessage);
	void CheckSourceOfViewSwitchL(const RMessage2& aMessage);
private:
	void RequestActivationL();
	void Complete(TInt aError,const RMessage2& aMessage);
private:
	enum TState
		{
		EPending,
		EWaitingForClientViewEventRequest,
		EWaitingForCustomMessageCollection,
		EWaitingForAcknowledgementOfActivation
		};
private:
	const TVwsViewId iViewId;
	const TVwsViewId iPreviousViewId;
	CVwsClientMessage* iClientMessage;
	TState iState;
	RThread iThreadOfClientInitiatingViewSwitch;
	};


/**
 * The CVwsSessionEvent_Deactivation represents a deactivation event in a session's event queue
 */
class CVwsSessionEvent_Deactivation : public CVwsSessionEvent
	{
public:
	CVwsSessionEvent_Deactivation(CVwsSession& aSession,CVwsEventQueue& aQueue,MVwsSessionObserver& iObserver,const TVwsViewId& aViewId,const TVwsViewId& aActiveViewId, TBool aDifferentInstanceOfSameApp);
	~CVwsSessionEvent_Deactivation();
private:	// From CVwsEvent.
	virtual void ProcessEventL();
private:	// From CVwsSessionEvent.
	virtual void HandleViewEventRequestL(TInt aError,const RMessage2& aMessage);
private:
	void RequestDeactivationL();
	void Complete(TInt aError,const RMessage2& aMessage);
private:
	enum TState
		{
		EPending,
		EWaitingForClientViewEventRequest,
		EWaitingForAcknowledgementOfDeactivation
		};
private:
	const TVwsViewId iViewId;
	const TVwsViewId iActiveViewId;
	TState iState;
	TBool iDifferentInstanceOfSameApp;
	};


/**
 * The CVwsSessionEvent_ScreenDeviceChangeNotification represents a screen device changes notification event in a
 * session's event queue
 */
class CVwsSessionEvent_ScreenDeviceChangeNotification : public CVwsSessionEvent
	{
public:
	CVwsSessionEvent_ScreenDeviceChangeNotification(CVwsSession& aSession,CVwsEventQueue& aQueue,MVwsSessionObserver& iObserver,const TVwsViewId& aViewId);
	~CVwsSessionEvent_ScreenDeviceChangeNotification();
private:	// From CVwsEvent.
	virtual void ProcessEventL();
private:	// From CVwsSessionEvent.
	virtual void HandleViewEventRequestL(TInt aError,const RMessage2& aMessage);
private:
	void RequestScreenDeviceChangeNotificationL();
	void Complete(TInt aError,const RMessage2& aMessage);
private:
	enum TState
		{
		EPending,
		EWaitingForClientViewEventRequest,
		EWaitingForAcknowledgementOfScreenDeviceChange
		};
private:
	const TVwsViewId iViewId;
	TState iState;
	};


/**
 * The CVwsSessionEvent_DeactivationNotification represents a deactivation notification event in a session's event queue.
 */
class CVwsSessionEvent_DeactivationNotification : public CVwsSessionEvent
	{
public:
	CVwsSessionEvent_DeactivationNotification(CVwsSession& aSession,CVwsEventQueue& aQueue,const TVwsViewId& aViewId,const TVwsViewId& aActiveViewId);
	~CVwsSessionEvent_DeactivationNotification();
private:	// From CVwsEvent.
	virtual void ProcessEventL();
private:	// From CVwsSessionEvent.
	virtual void HandleViewEventRequestL(TInt aError,const RMessage2& aMessage);
private:
	void RequestDeactivationNotificationL();
	void Complete(TInt aError);
private:
	enum TState
		{
		EPending,
		EWaitingForClientViewEventRequest,
		EWaitingForAcknowledgementOfDeactivationNotification
		};
private:
	const TVwsViewId iViewId;
	const TVwsViewId iActiveViewId;
	TState iState;
	};


/**
 * The CVwsSessionEvent_ActivationNotification represents a deactivation notification event in a session's event queue.
 */
class CVwsSessionEvent_ActivationNotification : public CVwsSessionEvent
	{
public:
	CVwsSessionEvent_ActivationNotification(CVwsSession& aSession,CVwsEventQueue& aQueue,const TVwsViewId& aViewId,const TVwsViewId& aViewToBeDeactivatedId);
	~CVwsSessionEvent_ActivationNotification();
private:	// From CVwsEvent.
	virtual void ProcessEventL();
private:	// From CVwsSessionEvent.
	virtual void HandleViewEventRequestL(TInt aError,const RMessage2& aMessage);
private:
	void RequestActivationNotificationL();
	void Complete(TInt aError);
private:
	enum TState
		{
		EPending,
		EWaitingForClientViewEventRequest,
		EWaitingForAcknowledgementOfActivationNotification
		};
private:
	const TVwsViewId iViewId;
	const TVwsViewId iViewToBeDeactivatedId;
	TState iState;
	};

#endif
