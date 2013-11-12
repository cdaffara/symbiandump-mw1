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


#ifndef __VWSEVENT_H__
#define __VWSEVENT_H__

#include <e32std.h>

#include "VWSQUEUE.H"
#include "VWSERVER.H"
#include "VWSSESSN.H"
#include "VWSADDOB.H"
#include "VWSSEVNT.H"


/**
 * The CVwsThreadWatcher class actively watches an application thread and makes a call back on thread death.
 */
class CVwsThreadWatcher : public CActive
	{
public:
	~CVwsThreadWatcher();
	CVwsThreadWatcher();
	TInt StartWatching(const TThreadId& aThreadId, const TCallBack& aCallBack);
protected:
	void RunL();
	void DoCancel();
protected:
	RThread iThread;
	TCallBack iCallBack;
	};


class MVwsEventTimerObserver;

/**
 * The CVwsEventTimer class times the processing of an event and notifies an observer if the processing time
 * exceeds server limits.
 */
class CVwsEventTimer : public CBase
	{
public:
	enum TTimeOut
		{
		EUndefined,
		EClientRequestTimeOut,
		EServerEventTimeOut,
		EIntermediateEventTimeOut
		};
public:
	~CVwsEventTimer();
	static CVwsEventTimer* NewLC(const TTimeIntervalMicroSeconds32& aClientRequestTimeOut, const TTimeIntervalMicroSeconds32& aServerEventTimeOut);
	void Start(MVwsEventTimerObserver* aObserver,const TBool& aScreenDeviceChangeEvent);
public:
	static TInt TimerCallBack(TAny* aSelf);
	void DoTimerCallBack();
private:
	CVwsEventTimer(const TTimeIntervalMicroSeconds32& aClientRequestTimeOut, const TTimeIntervalMicroSeconds32& aServerEventTimeOut);
	void ConstructL();
private:
	CPeriodic* iPeriodic;
	MVwsEventTimerObserver* iObserver;
	TTimeIntervalMicroSeconds32 iClientRequestTimeOut;
	TTimeIntervalMicroSeconds32 iServerEventTimeOut;
	TTimeOut iTimeOutState;
	TBool iScreenDeviceChangeEvent;
	};


/**
 * The MVwsEventTimerObserver class provides an interface to handle time out call-backs from the CVwsEventTimer
 *
 */
class MVwsEventTimerObserver
	{
public:
	virtual void HandleTimeOut(CVwsEventTimer::TTimeOut aTimeOut)=0;
	};


/**
 * The CVwsServerEvent class is the base class for server events generated on receipt of requests to the view server.
 */ 
class CVwsServerEvent : public CVwsEvent
	{
public:
	CVwsServerEvent(CVwsServer& aServer,TType aType,CVwsEventQueue& aQueue);
	~CVwsServerEvent();
private:
	virtual void HandleLastOnQueue();
protected:
	CVwsServer& iServer;
	CVwsSessionEvent* iObserved;
	};


/**
 * The CVwsServerEvent_Activate class represents a view activation request in the server. It owns the message contents buffer
 * associated with a view activation request.
 */ 
class CVwsServerEvent_Activate : public CVwsServerEvent, public MVwsSessionObserver, public MVwsViewAdditionObserver, public MVwsEventTimerObserver
	{
public:
	CVwsServerEvent_Activate(CVwsServer& aServer,TType aType,CVwsEventQueue& aQueue,const TVwsViewId& aViewIdToActivate,CVwsClientMessage* aClientMessage,const RMessage2& aMessage,const TThreadId& aClientThreadId,TVwsCompleteRequest aCompleteRequest,CVwsThreadWatcher* aThreadWatcher, CVwsEventTimer* aEventTimer,const RThread& aThreadOfClientInitiatingViewSwitch);
	~CVwsServerEvent_Activate();
	static TInt AppThreadWatcherCallBack(TAny* aPtr);

private: // From CVwsServerEvent.
	virtual void ProcessEventL();
protected:
	virtual void HandleSessionRemoval(const TThreadId& aClientThreadId);
protected: // From MVwsSessionObserver.
	virtual void RequestCompleted(TRequestType aType,TInt aError,const RMessage2& aMessage);
	virtual void NowObserving(CVwsSessionEvent* aObserved);
	virtual void NullifyObserved();
private: // From MVwsViewAdditionObserver.
	virtual void HandleViewAdded(const TVwsViewId& aViewId, const TThreadId& aNewViewClientThreadId);
public: // from MVwsEventTimerObserver
	virtual void HandleTimeOut(CVwsEventTimer::TTimeOut aTimeOut);
protected:
	void RequestActivateNewView();
	void RequestActivateSystemDefaultView();
	void DoRequestActivateNewView(CVwsSession& aSessionWithNewView);
	void RequestDeactivateOldView();
	void RequestAppStart();
	void Complete(TInt aError);
	void StartAppStartProfile();
	void StopAppStartProfile();
	void StartActivationProfile();
	void StopActivationProfile();
private:
	enum TState
		{
		EPending,
		EAppStartRequested,
		EActivationRequested,
		ESystemDefaultActivationRequested,
		EDeactivationRequested
		};
	enum TWhichProfile
		{
		ENone,
		EAppStart,
		EActivation
		};
private:
	TInt RequestClientActivation(CVwsSession& aSession,CVwsClientMessage* aClientMessage,TState aNewState);
	void CustomCrossCheck(CVwsSession& aSessionWithNewView);
protected:
	TVwsViewId iViewIdToActivate;
	CVwsClientMessage* iClientMessage;
	TThreadId iNewViewClientThreadId;
private:
	TState iState;
	TWhichProfile iWhichProfile;
	TVwsCompleteRequest iCompleteRequest;
	CVwsSession* iSessionWithNewView;
	TThreadId iClientThreadId;
	const RMessage2 iMessage;
	CVwsThreadWatcher* iThreadWatcher;
	CVwsEventTimer* iEventTimer;
	RThread iThreadOfClientInitiatingViewSwitch;
	};


/**
 * The CVwsServerEvent_ScreenDeviceChanged class represents a screen device change request in the server.
 */ 
class CVwsServerEvent_ScreenDeviceChanged : public CVwsServerEvent_Activate
	{
public:
	CVwsServerEvent_ScreenDeviceChanged(CVwsServer& aServer,CVwsEventQueue& aQueue,CVwsThreadWatcher* aThreadWatcher, CVwsEventTimer* aEventTimer);
	~CVwsServerEvent_ScreenDeviceChanged();
	static TInt AppThreadWatcherCallBack(TAny* aPtr);
private: // From CVwsServerEvent.
	virtual void ProcessEventL();
	virtual void HandleAddedToQueue();
	virtual void HandleSessionRemoval(const TThreadId& aClientThreadId);
private: // From MVwsSessionObserver.
	virtual void RequestCompleted(TRequestType aType,TInt aError,const RMessage2& aMessage);
	virtual void NowObserving(CVwsSessionEvent* aObserved);
	virtual void NullifyObserved();
private:
	virtual void HandleTimeOut(CVwsEventTimer::TTimeOut aTimeOut);
private:
	void RequestChangeNotification();
	void RequestActivationAsRequiredL(const RMessage2& aMessage);
	void Complete(TInt aError);
private:
	enum TScreenDeviceNotificationState
		{
		EPending,
		EScreenDeviceChangeNotificationRequested,
		EViewActivationRequested,
		EDefaultSystemViewActivationRequested
		};
private:
	TScreenDeviceNotificationState iScreenDeviceNotificationState;
	CVwsThreadWatcher* iThreadWatcher;
	CVwsEventTimer* iEventTimer;
	};


/**
 * The CVwsServerEvent_DeactivateActiveView class represents a deactivation request in the server. A client view activation is
 * implemented as an activation followed by a deactivation event in the server.
 */
class CVwsServerEvent_DeactivateActiveView : public CVwsServerEvent, public MVwsSessionObserver, public MVwsEventTimerObserver
	{
public:
	CVwsServerEvent_DeactivateActiveView(CVwsServer& aServer,CVwsEventQueue& aQueue,const RMessage2& aMessage,const TThreadId& aClientThreadId,TVwsCompleteRequest aCompleteRequest,CVwsEventTimer* aEventTimer);
	~CVwsServerEvent_DeactivateActiveView();
private: // From CVwsServerEvent.
	virtual void ProcessEventL();
	virtual void HandleSessionRemoval(const TThreadId& aClientThreadId);
private: // From MVwsSessionObserver.
	virtual void RequestCompleted(TRequestType aType,TInt aError,const RMessage2& aMessage);
	virtual void NowObserving(CVwsSessionEvent* aObserved);
	virtual void NullifyObserved();
private: // from MVwsEventTimerObserver
	virtual void HandleTimeOut(CVwsEventTimer::TTimeOut aTimeOut);
private:
	void RequestDeactivationL();
	void Complete(TInt aError);
private:
	enum TState
		{
		EPending,
		EDeactivationRequested
		};
private:
	TState iState;
	TVwsViewId iActiveView;
	const RMessage2 iMessage;
	TThreadId iClientThreadId;
	TVwsCompleteRequest iCompleteRequest;
	CVwsEventTimer* iEventTimer;
	};


/**
 * The CVwsServerEvent_AppStart class represents a direct server-side app start request.
 */
class CVwsServerEvent_AppStart : public CVwsServerEvent, public MVwsViewAdditionObserver
	{
public:
	CVwsServerEvent_AppStart(CVwsServer& aServer,CVwsEventQueue& aQueue,const RMessage2& aMessage,const TUid& aAppToStart,CVwsThreadWatcher* aThreadWatcher);
	~CVwsServerEvent_AppStart();
	static TInt AppThreadWatcherCallBack(TAny* aPtr);
private: // From CVwsServerEvent.
	virtual void ProcessEventL();
private: // From MVwsViewAdditionObserver.
	virtual void HandleViewAdded(const TVwsViewId& aViewId, const TThreadId& aNewViewClientThreadId);
private:
	void RequestAppStart();
	void Complete(TInt aError);
private:
	enum TState
		{
		EPending,
		EAppStartRequested
		};
private:
	TState iState;
	TUid iAppToStart;
	const RMessage2 iMessage;
	CVwsSession* iSessionWithNewApp;
	CVwsThreadWatcher* iThreadWatcher;
	};


#endif
