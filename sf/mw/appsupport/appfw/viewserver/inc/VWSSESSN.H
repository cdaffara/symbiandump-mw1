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

#ifndef __VWSSESSN_H__
#define __VWSSESSN_H__

#include <e32base.h>

#include "VWSDEF.H"
#include "VWSAPPST.H"
#include "VWSPRIV.H"
#include "VWSADDOB.H"


//
// Forward declarations.
//

class CVwsServer;
class CVwsEventQueue;
class TVwsViewEvent;
class CVwsClientMessage;
class CVwsSessionEvent;


/** 
 * The MVwsServerSessionObserver class specifies an interface for getting callbacks on completion of the stages of
 * a client request. The interface is implemented by the concrete server event classes.
 */
class MVwsSessionObserver
	{
public:
	enum TRequestType
		{
		EActivation,
		EDeactivation,
		EScreenDeviceChangeNotification
		};
public:
	virtual void RequestCompleted(TRequestType aType,TInt aError,const RMessage2& aMessage)=0;
	virtual void NowObserving(CVwsSessionEvent* aObserved)=0;
	virtual void NullifyObserved()=0;

	};


/**
 * The MVwsMessageHandler class specifies the interface through which a custom message request is passed onto a
 * session activation event.
 */
class MVwsMessageHandler
	{
public:
	virtual void WriteClientMessageL(const RMessage2& aMessage)=0;
	virtual void CheckSourceOfViewSwitchL(const RMessage2& aMessage)=0;
	};


/**
 * The TVwsViewSwitchNotification class encapsulates that attributes of a view switch notification request.
 */
class TVwsViewSwitchNotification
	{
public:
	TVwsViewSwitchNotification();
	void SetRequest(const TVwsViewId& aViewId);
	void ClearRequest();
	TBool IsViewToNotify(const TVwsViewId& aViewId) const;
public:
	TVwsViewId iViewId;
	TBool iIsOutstanding;
	};

/**
 * The CVwsSession class implements a view server session.
 */ 
class CVwsSession : public CSession2
	{
public:
	enum TState
		{
		EWaitingForClientRequest,
		EClientRequestPending
		};
public:
	~CVwsSession();
	static CVwsSession* NewL(const TThreadId& aThreadId,CVwsServer& aServer);
	void CloseSession();
	static void PanicClient(const RMessage2& aMessage,TInt aPanic);
	void PanicClient(TInt aPanic);
	void CompleteViewEvent(TInt aNotification);
	void CompleteViewEventL(TInt aNotification,const TVwsViewEvent& aEvent);
	TUid AppUid() const;
	CVwsSession::TState State() const;
	void RequestClientActivationL(MVwsSessionObserver& aObserver,const TVwsViewId& aViewId,const TVwsViewId& aPreviousViewId,CVwsClientMessage* aClientMessage,RThread aThreadOfClientInitiatingViewSwitch);
	void RequestClientDeactivationL(MVwsSessionObserver& aObserver,const TVwsViewId& aViewId,const TVwsViewId& aActiveViewId, TBool aDifferentInstanceOfSameApp);
	void RequestScreenDeviceChangeNotificationL(MVwsSessionObserver& aObserver,const TVwsViewId& aViewId);
	void SetMessageHandler(MVwsMessageHandler& aMessageHandler);
	void ClearMessageHandler();
	TBool HasActiveView() const; 
	TVwsViewId ActiveView() const;
	void SetActiveView(const TVwsViewId& aViewId);
	void ClearActiveView();
	TInt CheckViewExists(const TVwsViewId& aViewId) const;
	TBool IsViewActive(const TVwsViewId& aViewId) const;
	void AddViewL(const TVwsViewId& aViewId);
	TInt RemoveView(const RMessage2& aMessage,const TVwsViewId& aViewId);
	TInt IndexById(const TVwsViewId& aViewId) const;
	TInt GetTopView(TVwsViewId& aViewId);
	inline TThreadId ClientThreadId() const;
	void HandleDeactivationL(const TVwsViewId& aDeactivatedViewId, const TVwsViewId& aActivatedViewId);
	void HandleActivationL(const TVwsViewId& aActivatedViewId, const TVwsViewId& ViewIdToBeDeactivated);
	inline TBool IsExiting() const;
	inline TBool IsLeaveAfterOwnershipTaken() const;
	inline void ResetLeaveAfterOwnershipTaken();
	TBool Protected() const;

public:
	CVwsClientMessage* iClientMessage;
	void SetClientMessageL( CVwsClientMessage* aClientMessage );

private:
	CVwsSession(const TThreadId& aThreadId,CVwsServer& aServer);
	void ConstructL();
private: // from CSession2
	virtual void ServiceL(const RMessage2& aMessage);
	virtual void ServiceError(const RMessage2& aMessage,TInt aError);
private:
	void AddViewL(const RMessage2& aMessage);
	void SetSystemDefaultViewL(const RMessage2& aMessage);
	void GetSystemDefaultViewL(const RMessage2& aMessage);
	void RemoveViewL(const RMessage2& aMessage);
	void RequestViewEventL(const RMessage2& aMessage);
	void CancelRequestViewEvent();
	void CancelDeactivateActiveViewEvent();
	void CancelActivateViewEvent();
	void ActivateViewL(const RMessage2& aMessage,TVwsCompleteRequest aCompleteRequest);
	void RequestCustomMessageL(const RMessage2& aMessage);
	TVwsViewId ViewIdFromMessageL(const RMessage2& aMessage);
	void StartAppL(const RMessage2& aMessage);
	void DeactivateActiveViewL(const RMessage2& aMessage,TVwsCompleteRequest aCompleteRequest);
	void DeactivateActiveViewIfOwnerMatchL(const RMessage2& aMessage,TVwsCompleteRequest aCompleteRequest);
	void NotifyNextDeactivationL(const RMessage2& aMessage);
	void NotifyNextActivationL(const RMessage2& aMessage);
	void SetClientRequestTimeOut(const RMessage2& aMessage);
	void SetServerEventTimeOut(const RMessage2& aMessage);
	void EnableServerEventTimeOut(const RMessage2& aMessage);
	void CheckSourceOfViewSwitchL(const RMessage2& aMessage);
	void GetPriorityForActiveObjectL(const RMessage2& aMessage);
	void EnableServerBlankScreen(const RMessage2& aMessage);
	void SetProtectedL(const RMessage2& aMessage);
	void GetCurrentActiveViewIdL(const RMessage2& aMessage);
private:
	RMessage2 iViewEventMessage;
	RMessage2 iPanicMessage;
	TVwsViewSwitchNotification iDeactivationNotification;
	TVwsViewSwitchNotification iActivationNotification;
	MVwsMessageHandler* iMessageHandler;
	CVwsServer& iServer;
	CVwsEventQueue* iEventQueue;
	TUid iAppUid;
	TState iState;
	CArrayFixFlat<TVwsViewId> iViewIdArray;
	TInt iActiveViewIndex;
	TInt iLastActiveViewIndex;
	TThreadId iClientThreadId;
	TBool iIsExiting;
	TInt iPanic;
	TBool iLeaveAfterOwnershipTaken;
	TBool iProtected;
	};


/**
 * The CVwsClientMessage class encapsulates the contents of the custom client mesages which can be sent when
 * activating a view. It is used server side to store the message contents associated with an activation event
 * which are recovered from the write to the server thread.
 */
class CVwsClientMessage : public CBase
	{
public:
	static CVwsClientMessage* New();
	static CVwsClientMessage* NewL();
	static CVwsClientMessage* NewL(const TUid& aMessageId);
	static CVwsClientMessage* NewL(const TUid& aMessageId, TInt aMessageLength, const RMessage2& aMessage, TInt aParameter);
	~CVwsClientMessage();
private:
	CVwsClientMessage();
	CVwsClientMessage(const TUid& aMessageId);
	void ConstructL(TInt aMessageLength, const RMessage2& aMessage, TInt aParameter);
public:
	TUid iMessageId;
	HBufC8* iMessage;
	};


//
// Inlines
//

inline TThreadId CVwsSession::ClientThreadId() const
	{return iClientThreadId;}

inline TBool CVwsSession::IsExiting() const
	{return iIsExiting;}

inline TBool CVwsSession::IsLeaveAfterOwnershipTaken() const
	{return iLeaveAfterOwnershipTaken;}

inline void CVwsSession::ResetLeaveAfterOwnershipTaken()
	{iLeaveAfterOwnershipTaken=EFalse;};


#endif

