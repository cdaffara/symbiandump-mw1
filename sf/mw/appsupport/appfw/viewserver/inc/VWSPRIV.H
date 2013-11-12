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


#ifndef __VWSPRIV_H__
#define __VWSPRIV_H__

#include <e32base.h>
#include <gdi.h>

#include "VWSDEF.H"

#if defined(_DEBUG)
#	if defined(ASSERT)
#		undef ASSERT
#	endif
// Redefine assert to be a little more useful to us, i.e. to include file & line number
#	define __ASSERT_FILE__(s) _LIT(KPanicFileName,s)
#	define __ASSERT_PANIC__(l) User::Panic(KPanicFileName().Right(12),l)
#	define ASSERT(x) { __ASSERT_FILE__(__FILE__); __ASSERT_DEBUG(x, __ASSERT_PANIC__(__LINE__) ); }
#endif

//
// Constants.
//


_LIT(KViewServerName,"!ViewServer");
_LIT(KViewServerSemaphore,"ViewServerSemaphore");

const TUint KVwsMajorVersionNumber=0;
const TUint KVwsMinorVersionNumber=1;
const TUint KVwsBuildVersionNumber=1;


//
// Panic codes.
//

enum TVwsServerPanic
	{
	EVwsServerCreate=1,
	EVwsServerStart,
	EVwsBadRequest,
	EVwsBadDescriptor,
	EVwsMainSchedulerError,
	EVwsViewNotFound,
	EVwsInvalidViewUid,
	EVwsCustomMessageNotCollected,
	EVwsViewActive,
	EVwsViewEventRequestAlreadyPending,
	EVwsViewActivationEventTimeOut,
	EVwsNoClientMessage,
	EVwsInvalidScreenMode,
	EVwsViewDeactivationEventTimeOut //A view has taken too long to complete its implementation of MCoeView::ViewDeactivated()
	};

GLREF_C void PanicServer(TVwsServerPanic aPanic);

enum TVwsRanges
	{
	EFirstUnrestrictedOpcodeInVws,
	EFirstOpcodeNeedingCustomCheckInVws=100,
	EFirstOpcodeNeedingSecureIdInVws=101,
	EFirstOpCodeNeedingPowerMgmt=110,
	};
//
// Request opcodes.
//

enum TVwsRequestOpcode
	{
	//No Capability requirement
	EVwsFirst=EFirstUnrestrictedOpcodeInVws,
	EVwsCreate,
	EVwsAddView,
	EVwsRemoveView,
	EVwsRequestViewEvent,
	EVwsRequestViewEventCancel,
	EVwsActivateView,
	EVwsRequestCustomMessage,
	EVwsStartApp,
	EVwsDeactivateActiveView,
	EVwsNotifyNextDeactivation,
	EVwsNotifyNextActivation,
	EVwsCreateActivateViewEvent,
	EVwsCreateDeactivateViewEvent,
	EVwsGetSystemDefaultView,
	EVwsCheckSourceOfViewSwitch,
	EVwsAsynchronousMessageForServerToPanicClientWith,
	EVwsSetProtected,
	EVwsSetCrossCheckUid,
	EVwsDeactivateActiveViewIfOwnerMatch,
	EVwsPriority,
	EVwsSetWindowBackgroundColor,
	EVwsCurrentActiveViewId,
	//Custom Check
	EVwsSetSystemDefaultView=EFirstOpcodeNeedingCustomCheckInVws,
	//SID of Eiksrvs
	EVwsSetClientRequestTimeOut=EFirstOpcodeNeedingSecureIdInVws,
	EVwsSetServerEventTimeOut,
	EVwsEnableServerEventTimeOut,
	EVwsEnableServerBlankScreen,
	//PowerMgmt capability check
	EVwsClose=EFirstOpCodeNeedingPowerMgmt,
	// End Marker no Capability
	EVwsFirstUnusedOpcode
	};

enum TVwsCompleteRequest
	{
	ECompleteRequest,
	EDoNotCompleteRequest
	};

/**
 * The RVwsSession class implements a client session connection to the view server.
 */ 
class RVwsSession : public RSessionBase
	{
public:
	RVwsSession();
	TInt Connect();
	TVersion Version() const;
	TInt Shutdown() const;
	TInt AddView(const TVwsViewId& aViewId) const;
	TInt SetSystemDefaultView(const TVwsViewId& aViewId,TInt aMode) const;
	TInt GetSystemDefaultView(TVwsViewId& aViewId);
	TInt RemoveView(const TVwsViewId& aViewId) const;
	void RequestViewEvent(TVwsViewEventBuf& aViewEventBuf,TInt aLastError,TRequestStatus& aStatus) const;
	void RequestViewEvent(TVwsViewEventBuf& aViewEventBuf,TInt aLastError,const TVwsViewIdAndMessageBuf& aViewActivationBuf,const TDesC8& aMessage,TRequestStatus& aStatus) const;
	TInt RequestViewEventCancel() const;
	void ActivateView(const TVwsViewId& aViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage,TRequestStatus& aStatus, TInt aCustomControl);
	TInt ActivateView(const TVwsViewId& aViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage, TInt aCustomControl);
	TInt RequestCustomMessage(TDes8& aMessageBuf) const;
	TInt StartApp(TUid aAppToStart) const;
	void DeactivateActiveView(TRequestStatus& aStatus) const;
	TInt DeactivateActiveView() const;
	void DeactivateActiveViewIfOwnerMatch(TRequestStatus& aStatus) const;
	TInt DeactivateActiveViewIfOwnerMatch() const;
	TInt NotifyNextDeactivation(const TVwsViewId& aViewId) const;
	TInt NotifyNextActivation(const TVwsViewId& aViewId) const;
	TInt SetClientRequestTimeOut(TTimeIntervalMicroSeconds32 aDuration) const;
	TInt SetServerEventTimeOut(TTimeIntervalMicroSeconds32 aDuration) const;
	TInt EnableServerEventTimeOut(TBool aEnable) const;
	TInt CheckSourceOfViewSwitch(TBool& aResult,const TSecurityPolicy& aSecurityPolicy,const char* aDiagnostic) const;
	TInt GetPriority();
	TInt EnableServerBlankScreen(TBool aEnable) const;
	TInt SetProtected(TBool aEnable) const;
	TInt SetCrossCheckUid(const TUid& aCrossCheckUid);
	TInt SetWindowBackgroundColor(const TRgb& aBgColor);
	TInt GetCurrentActiveViewInSystem(TVwsViewId& aActiveViewId);
private:
	TPckgBuf<TVwsViewId> iActivateViewId;
	TPtrC8 iCustomMessage;
	TRequestStatus iPanicStatus;
	};


class MVwsSessionWrapperObserver;

/**
 * The CVwsEventHandler class actively requests view events from the client session and
 * forwards these events to any session handler observer.
 */ 
class CVwsSessionEventHandler : public CActive
	{
public:
	CVwsSessionEventHandler(MVwsSessionWrapperObserver& aObserver,RVwsSession& aSession,TInt aPriority);
 	~CVwsSessionEventHandler();
	void Queue();
	void OnNextQueueActivateView(const TVwsViewIdAndMessage& aViewIdAndMessage);
	void DoRunL();
private: // From CActive.
	void RunL();
	void DoCancel();
private:
	MVwsSessionWrapperObserver& iObserver;
	RVwsSession& iVwsSession;
	TVwsViewEventBuf iViewEventBuf;
	TVwsViewIdAndMessageBuf iViewActivationBuf;
	TPtrC8 iCustomMessage;
	TBool iDoActivationNextQueue;
 	TInt iLastError;
	};

#endif
