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


#ifndef __VWSERVER_H__
#define __VWSERVER_H__

#include <e32base.h>
#include <w32std.h>

#include "VWSAPPST.H"
#include "VWSDEF.H"
#include "VWSPRIV.H"
#include "VWSDEBUG.H"
#include <e32property.h>
#include <startupdomaindefs.h>
#include <ssm/ssmsubstates.hrh>
#include <ssm/ssmstateawaresession.h>
//
// Forward declarations.
//

class CVwsLog;
class CVwsServer;
class CVwsSession;
class CVwsEventQueue;
class CVwsWServSessionHandler;
class CVwsWServEventHandler;
class CVwsWServRedrawer;
class MVwsViewAdditionObserver;
class MVwsDeactivationObserver;
class MVwsActivationObserver;
class MVwsSessionObserver;
class CVwsServerEvent;
class CVwsClientMessage;


/**
 * The CVwsServer class implements the view server.
 */ 
class CVwsServer : public CPolicyServer
	{
public:
	IMPORT_C static CVwsServer* NewL(MVwsAppStarter& aAppStarter);
public:
	IMPORT_C ~CVwsServer();
public: //from CServer2
	virtual CSession2* NewSessionL(const TVersion& aVersion,const RMessage2& aMessage) const;
public:
	TName ClientName() const;
	void SetSystemDefaultViewL(TInt aMode,const TVwsViewId& aViewId);
	void GetSystemDefaultView(TVwsViewId& aViewId);
	void ActivateViewL(const TVwsViewId& aViewId,CVwsClientMessage* aClientMessage,const RMessage2& aMessage,const CVwsSession& aSession,TVwsCompleteRequest aCompleteRequest);
	void ActivateViewL(const TVwsViewId& aViewId,CVwsClientMessage* aClientMessage,const RMessage2& aMessage,const TThreadId& aClientThreadId,TVwsCompleteRequest aCompleteRequest);
	CVwsSession* SessionByUid(const TUid& aAppUid);
	CVwsSession* SessionByThreadId(const TThreadId& aThreadId);
	CVwsSession* SessionByThreadIdAndAppUid(const TThreadId& aThreadId,const TUid& aAppUid);
	CVwsSession* ActiveViewSession();
	TVwsViewId ActiveView();
	void SetActiveView(const TThreadId& aThreadId,const TVwsViewId& aViewId);
	void ClearActiveView();
	void SetViewAdditionObserver(MVwsViewAdditionObserver* aViewAdditionObserver);
	void HandleScreenDeviceChangedL();
	TInt StartApp(const TUid& aAppUid,TThreadId& aThreadId);
	TInt CheckViewExists(const TThreadId& aThreadId,const TVwsViewId& aViewId);
	TBool IsViewActive(const TVwsViewId& aViewId);
	void RequestDeactivateActiveViewL(const RMessage2& aMessage,const CVwsSession& aSession,TVwsCompleteRequest aCompleteRequest);
	void RequestAppStartL(const RMessage2& aMessage,const TUid& aAppToStart);
	void HandleDeactivation(const TVwsViewId& aDeactivatedViewId, const TVwsViewId& aActivatedViewId);
	void HandleActivation(const TVwsViewId& aActivatedViewId, const TVwsViewId& ViewIdToBeDeactivated);
	void MakeInterceptingWindowVisible();
	void MakeInterceptingWindowInvisible();
	void MakeInterceptingWindowVisibleAndUpdateScreen();
	TInt ScreenMode();
	void HandleSessionViewAddition(const TVwsViewId& aViewId, const TThreadId& aNewViewClientThreadId);
	TBool IsSystemDefaultView(const TVwsViewId& aViewId);
	void HandleSessionRemoval(const TThreadId& aThreadId);
	void SetClientRequestTimeOut(const TTimeIntervalMicroSeconds32& aDuration);
	void SetServerEventTimeOut(const TTimeIntervalMicroSeconds32& aDuration);
	void HandleLastServerEventOnQueue();
	void HandleForegroundTaskChange();
	void EnableServerEventTimeOut(TBool aEnable);
	inline TBool IsServerEventTimeOutEnabled() const;
	void PanicClient(TInt aPanic) const;
	void GetPriorityForActiveObjectL(TInt& aPriority);
	void EnableServerBlankScreen(TBool aEnable);
	void SetCrossCheckUid(const RMessage2& aMessage);
	TUid CrossCheckUid() const;
	void SendCrossCheckToLauncherL(RWsSession& aWsSession, TInt aFgAppWgId, const TVwsViewId& aActiveView);
	void KickStartEventQ();
	void SetWindowBackgroundColor(const RMessage2& aMessage);
	void GetCurrentActiveViewId(TVwsViewId& aActiveViewId);
	TBool IsAppInForeground();
	TBool InitializationFinished() const;
	TInt IsPriorityBoostBeforePanicEnabled() const;
	void  HandleInitializationFinished();
	void  BoostPriority(CVwsSession* aClient);
private:
	CVwsServer(TInt aPriority,MVwsAppStarter& aAppStarter);
	void ConstructL();
	void CrossCheckForegroundTask();
	TCustomResult CustomSecurityCheckL(const RMessage2& aMsg, TInt& aAction, TSecurityInfo& aMissing);
private:
	MVwsAppStarter& iAppStarter;
	CVwsWServSessionHandler* iWServSessionHandler;
	CArrayFixFlat<TVwsViewId> iSystemDefaultViewIdArray;
	CVwsEventQueue* iEventQueue;
	MVwsViewAdditionObserver* iViewAdditionObserver;
	TTimeIntervalMicroSeconds32 iClientRequestTimeOut;
	TTimeIntervalMicroSeconds32 iServerEventTimeOut;
	TBool iForegroundChangeWhileEventsOutstanding;
	TInt iIsServerEventTimeOutDisabled;
#ifdef __DO_LOGGING__
	TInt iNoActivationRequests;
#endif
	TInt iPrioritySet;
	TBool iEnableServerBlankScreen;
	TUid iCrossCheckUid;
	TInt iEnableBoostAppPriorityBeforePanic;
	TBool iInitializationFinished;
	};

NONSHARABLE_CLASS (CVwsStartupAware) : public CActive
	{
public:
	CVwsStartupAware(CVwsServer& aServer);
	~CVwsStartupAware();
	TInt Start();
		
private:
	void DoCancel();
	void RunL();

private:
	CVwsServer& iServer;
	RSsmStateAwareSession iStateAwareSession;
	};

//
// Inlines
//

inline TBool CVwsServer::IsServerEventTimeOutEnabled() const
	{ 
	return (iIsServerEventTimeOutDisabled ? EFalse : ETrue);
	}

#endif
