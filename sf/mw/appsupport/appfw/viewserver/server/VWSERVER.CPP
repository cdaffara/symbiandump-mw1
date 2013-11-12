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

#include <apgtask.h>
#include <apgwgnam.h>

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include "vwsinternal.h"
#include "vwsdefpartner.h"
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS

#include "VWSERVER.H"
#include "VWSSESSN.H"
#include "VWSEVENT.H"
#include "VWSWEVNT.H"
#include "VWSADDOB.H"
#include "VWSPRIV.H"
#include "VWSDEBUG.H"
#include "vwspatchdata.h"


#include <barsc2.h>
#include <barsread2.h>
#include <f32file.h> 
#include <u32hal.h>

//
// Constants.
//

const TInt KVwsSystemDefaultViewArrayGranularity=2;
const TInt KDefaultClientRequestTimeOutDuration=1000000;
const TInt KDefaultServerEventTimeOutDuration=10000000;

_LIT(KPriorityFilename,"Z:\\resource\\apps\\PrioritySet.rsc");


const TUint8 KPolicyElementSID = 0;
const TUint8 KPolicyPowerMgmt = 1;

const TInt KEiksrvsSid=0x10003a4a;

const TUint KRangeCount = 5; 

const TInt KVwsServerRanges[KRangeCount] = 
	{	
	EFirstUnrestrictedOpcodeInVws,
	EFirstOpcodeNeedingCustomCheckInVws,
	EFirstOpcodeNeedingSecureIdInVws,	
	EFirstOpCodeNeedingPowerMgmt,
	EVwsFirstUnusedOpcode,
	};

const TUint8 KElementsIndex[KRangeCount] =
	{
	CPolicyServer::EAlwaysPass,		//Always passing no capability required [EFirstUnrestrictedOpcodeInVws-(EFirstOpcodeNeedingCustomCheckInVws-1)]
	CPolicyServer::ECustomCheck,	//Requires Custom check	[EFirstOpcodeNeedingCustomCheckInVws-(EFirstOpcodeNeedingSecureIdInVws-1)]
	KPolicyElementSID,				//Requires SID to be of Eiksrvs ie.0x10003a4a  [EFirstOpcodeNeedingSecureIdInVws-(EFirstOpCodeNeedingPowerMgmt-1)]
	KPolicyPowerMgmt,				//Requires PowerMgmt capability [EFirstOpCodeNeedingPowerMgmt-(EVwsFirstUnusedOpcode-1)]
	CPolicyServer::ENotSupported,	//Not Supported		
	};

const CPolicyServer::TPolicyElement KPolicyElements[] = 
	{
	{_INIT_SECURITY_POLICY_S0(KEiksrvsSid), CPolicyServer::EFailClient}, 
	{_INIT_SECURITY_POLICY_C1(ECapabilityPowerMgmt), CPolicyServer::EFailClient} 
	};

const CPolicyServer::TPolicy KVwsServerPolicy =
	{
	CPolicyServer::EAlwaysPass, 
	KRangeCount,
	KVwsServerRanges,
	KElementsIndex, 	 
	KPolicyElements 	
	};
 	
//
// CVwsServer.
//

#ifdef __DO_LOGGING__
CVwsServer::CVwsServer(TInt aPriority,MVwsAppStarter& aAppStarter)
	: CPolicyServer(aPriority,KVwsServerPolicy),
	  iAppStarter(aAppStarter),
	  iSystemDefaultViewIdArray(KVwsSystemDefaultViewArrayGranularity),
	  iClientRequestTimeOut(KDefaultClientRequestTimeOutDuration),
	  iServerEventTimeOut(KDefaultServerEventTimeOutDuration),
	  iForegroundChangeWhileEventsOutstanding(EFalse),
	  iIsServerEventTimeOutDisabled(0),	
	  iNoActivationRequests(0),
	  iEnableServerBlankScreen(ETrue),
	  iEnableBoostAppPriorityBeforePanic(KVwsBoostAppPriorityBeforePanic)
	{
	}
#else
CVwsServer::CVwsServer(TInt aPriority,MVwsAppStarter& aAppStarter)
	: CPolicyServer(aPriority,KVwsServerPolicy),
	  iAppStarter(aAppStarter),
	  iSystemDefaultViewIdArray(KVwsSystemDefaultViewArrayGranularity),
	  iClientRequestTimeOut(KDefaultClientRequestTimeOutDuration),
	  iServerEventTimeOut(KDefaultServerEventTimeOutDuration),
	  iForegroundChangeWhileEventsOutstanding(EFalse),
	  iIsServerEventTimeOutDisabled(0),
	  iEnableServerBlankScreen(ETrue),
	  iEnableBoostAppPriorityBeforePanic(KVwsBoostAppPriorityBeforePanic)
	{
	}
#endif

EXPORT_C CVwsServer* CVwsServer::NewL(MVwsAppStarter& aAppStarter)
	{
	CVwsServer *pS=new CVwsServer(CActive::EPriorityStandard,aAppStarter);
	__ASSERT_ALWAYS(pS!=NULL,PanicServer(EVwsServerCreate));

	pS->ConstructL();

	pS->StartL(KViewServerName);
	return pS;
	}

CSession2* CVwsServer::NewSessionL(const TVersion &aVersion,const RMessage2& aMessage) const
	{
	// Check version supported.
	TVersion version(KVwsMajorVersionNumber,KVwsMinorVersionNumber,KVwsBuildVersionNumber);
	if (!User::QueryVersionSupported(version,aVersion))
		{
		User::Leave(KErrNotSupported);
		}

	RThread thread;
	User::LeaveIfError(aMessage.Client(thread));
	const TThreadId threadId(thread.Id());
	thread.Close();
	return CVwsSession::NewL(threadId,CONST_CAST(CVwsServer&,*this));
	}


EXPORT_C CVwsServer::~CVwsServer()
	{
	delete iWServSessionHandler;

	delete iEventQueue;
#ifdef __DO_LOGGING__
	CVwsLog::ShutdownLog();
#endif
	}

void CVwsServer::ConstructL()
	{
	// required for handling intercepting window
	iWServSessionHandler=CVwsWServSessionHandler::NewL(*this);

#ifdef __DO_LOGGING__
	iEventQueue=new(ELeave) CVwsEventQueue(_L("Server Queue"));
	CVwsLog::StartLogL();
#else
	iEventQueue=new(ELeave) CVwsEventQueue;
#endif
	RFs fs;
	iPrioritySet=CActive::EPriorityStandard;
	CleanupClosePushL(fs);
	if(fs.Connect()==KErrNone)
		{
		CResourceFile* file=NULL;
		TRAPD(err, file=CResourceFile::NewL(fs, KPriorityFilename, 0, 0));
		if(err == KErrNone)
			{
			CleanupStack::PushL(file);
			RResourceReader reader;
			reader.OpenL(file, 2);
			CleanupClosePushL(reader);
			iPrioritySet=reader.ReadUint32L();
			CleanupStack::PopAndDestroy(&reader);
			CleanupStack::PopAndDestroy(file);
			}
		fs.Close();
		}
	
	CleanupStack::PopAndDestroy(&fs);


	
		
		
#ifdef __WINS__
	// KVwsBoostAppPriorityBeforePanic is a Rom patchable constant, so need an emulator equivalent
	// if WINS then read value from epoc.ini requires licencees to set property in epoc.ini. 
	// Usage: In epoc.ini patchdata_viewsrv_dll_KVwsBoostAppPriorityBeforePanic 1
						
	TInt valueOfKVwsBoostAppPriorityBeforePanic = 0;
	if (UserSvr::HalFunction(EHalGroupEmulator,EEmulatorHalIntProperty,(TAny*)"patchdata_viesrv_dll_KVwsBoostAppPriorityBeforePanic",&valueOfKVwsBoostAppPriorityBeforePanic) == KErrNone)
		{
		iEnableBoostAppPriorityBeforePanic = valueOfKVwsBoostAppPriorityBeforePanic;
		}
#endif
	LOG3(CVwsLog::EQuiet,_L("CVwsServer::IsPriorityBoostBeforePanicEnabled(): iEnableBoostAppPriorityBeforePanic = [%d] "),iEnableBoostAppPriorityBeforePanic);
		
	if (iEnableBoostAppPriorityBeforePanic)
		{
		CVwsStartupAware* startupAware = new(ELeave)CVwsStartupAware(*this);
		TInt err = startupAware->Start();
		if (err != KErrNone)
			{
			HandleInitializationFinished();
			delete startupAware;
			}
		}
 	}

CVwsStartupAware::CVwsStartupAware(CVwsServer& aServer)
	:CActive(CActive::EPriorityStandard),iServer(aServer)
	{
	CActiveScheduler::Add(this); 
	}
	
CVwsStartupAware::~CVwsStartupAware()
	{
	Cancel();
	iStateAwareSession.Close();
	}

TInt CVwsStartupAware::Start()
	{
	TInt err = iStateAwareSession.Connect(KSM2UiServicesDomain3);
	if(iStateAwareSession.State().MainState() != ESsmNormal && err == KErrNone)
		{
		iStateAwareSession.RequestStateNotification(iStatus);
		SetActive();
		}
	return err;
	}

void CVwsStartupAware::RunL()
	{
	if (iStatus == KErrNone)
		{
		if (iStateAwareSession.State().MainState() == ESsmNormal)
			{
		iServer.HandleInitializationFinished(); //Bootup is complete now, allowing Viewsrv 11 panics to occur here onwards
			delete this;
			}
		else
			{
			iStateAwareSession.AcknowledgeAndRequestStateNotification(KErrNone, iStatus);
			SetActive();
			}
		}
	}

void CVwsStartupAware::DoCancel()
	{
	iStateAwareSession.RequestStateNotificationCancel();
	}

void CVwsServer::PanicClient(TInt aPanic) const
	{
	 _LIT(KVwsSrvPanic, "ViewSrv");
 	 User::Panic(KVwsSrvPanic,aPanic); 
	}

void CVwsServer::SetSystemDefaultViewL(TInt aMode,const TVwsViewId& aViewId)
	{
	__ASSERT_DEBUG(aMode>=0,PanicClient(EVwsInvalidScreenMode));
	const TInt count = iSystemDefaultViewIdArray.Count();

	if ((aMode>count) || (aMode<0))
		{
		PanicClient(EVwsInvalidScreenMode);	
		}
	else if (aMode < count)
		{
		iSystemDefaultViewIdArray.At(aMode) = aViewId;
		}
	else if (aMode == count) 
		{
		iSystemDefaultViewIdArray.AppendL(aViewId);
		}
	}

void CVwsServer::SetClientRequestTimeOut(const TTimeIntervalMicroSeconds32& aDuration)
	{
	iClientRequestTimeOut=aDuration;
	}

void CVwsServer::SetServerEventTimeOut(const TTimeIntervalMicroSeconds32& aDuration)
	{
	iServerEventTimeOut=aDuration;
	}

void CVwsServer::EnableServerEventTimeOut(TBool aEnable)
	{
	if (!aEnable)
		{
		iIsServerEventTimeOutDisabled++;
		}
	else if (iIsServerEventTimeOutDisabled)
		{
		iIsServerEventTimeOutDisabled--;
		}
	}

void CVwsServer::EnableServerBlankScreen(TBool aEnable)
	{
	iEnableServerBlankScreen = aEnable;	
	}

void CVwsServer::GetSystemDefaultView(TVwsViewId& aViewId)
	{
	TInt mode=iWServSessionHandler->GetScreenMode();
	aViewId = (mode>=0 && mode<iSystemDefaultViewIdArray.Count()) ? iSystemDefaultViewIdArray[mode] : KNullViewId;
	}

TInt CVwsServer::ScreenMode()
	{
	return iWServSessionHandler->GetScreenMode();
	}

TBool CVwsServer::IsSystemDefaultView(const TVwsViewId& aViewId)
	{
	TBool ret=EFalse;

	const TInt numViews=iSystemDefaultViewIdArray.Count();
	for (TInt ii=0;ii<numViews;ii++)
		{
		if (iSystemDefaultViewIdArray[ii] == aViewId)
			{
			return ETrue;
			}
		}

	return ret;
	}

/**
 * Activates the view identified by aViewId. Immediately takes ownership of the message aClientMessage.
 */
void CVwsServer::ActivateViewL(const TVwsViewId& aViewId,CVwsClientMessage* aClientMessage,const RMessage2& aMessage,const TThreadId& aClientThreadId,TVwsCompleteRequest aCompleteRequest)
	{
	TVwsViewId viewId=aViewId;
	if (viewId==KNullViewId)
		{
		GetSystemDefaultView(viewId);
		}
	CleanupStack::PushL(aClientMessage);
	CVwsThreadWatcher* threadWatcher=new(ELeave) CVwsThreadWatcher();
	CleanupStack::PushL(threadWatcher);
	CVwsEventTimer* eventTimer=CVwsEventTimer::NewLC(iClientRequestTimeOut,iServerEventTimeOut);
	RThread threadOfClientInitiatingViewSwitch;
	User::LeaveIfError(aMessage.Client(threadOfClientInitiatingViewSwitch));
	CleanupClosePushL(threadOfClientInitiatingViewSwitch);	
	CVwsServerEvent* activationEvent=new(ELeave) CVwsServerEvent_Activate(*this,CVwsServerEvent::ENormal,*iEventQueue,viewId,aClientMessage,aMessage,aClientThreadId,aCompleteRequest,threadWatcher,eventTimer,threadOfClientInitiatingViewSwitch);
	CleanupStack::Pop(4, aClientMessage); // the CVwsServerEvent_Activate object is now the owner of aClientMessage, threadWatcher, eventTimer and threadOfClientInitiatingViewSwitch
	iEventQueue->ProcessEventL(activationEvent);
#ifdef __DO_LOGGING__
	iNoActivationRequests++;
	LOG3(CVwsLog::ELoud,_L("Cumulative activation requests since server start - %d"),iNoActivationRequests);
#endif
	}

void CVwsServer::ActivateViewL(const TVwsViewId& aViewId,CVwsClientMessage* aClientMessage,const RMessage2& aMessage,const CVwsSession& aSession,TVwsCompleteRequest aCompleteRequest)
	{
	ActivateViewL(aViewId,aClientMessage,aMessage,aSession.ClientThreadId(),aCompleteRequest);
	}

TVwsViewId CVwsServer::ActiveView()
	{
	for (iSessionIter.SetToFirst();iSessionIter;iSessionIter++)
		{
		CSession2* baseSession=iSessionIter;
		CVwsSession* thisSession=STATIC_CAST(CVwsSession*,baseSession);

		if (thisSession->HasActiveView())
			{
			return thisSession->ActiveView();
			}
		}

	return KNullViewId;
	}

void CVwsServer::SetActiveView(const TThreadId& aThreadId,const TVwsViewId& aViewId)
	{
#ifdef _DEBUG
	TVwsViewId activeView=ActiveView();

	if (activeView != KNullViewId)
		{
		LOG6(CVwsLog::EQuiet,_L("Changing active view to \"%x,%x\" [previously \"%x,%x\"]"),aViewId.iAppUid.iUid,aViewId.iViewUid.iUid,activeView.iAppUid.iUid,activeView.iViewUid.iUid);
		}
	else
		{
		LOG4(CVwsLog::EQuiet,_L("Changing active view to \"%x,%x\" [no view previously active]"),aViewId.iAppUid.iUid,aViewId.iViewUid.iUid);
		}
#endif

	for (iSessionIter.SetToFirst();iSessionIter;iSessionIter++)
		{
		CSession2* baseSession=iSessionIter;
		CVwsSession* thisSession=STATIC_CAST(CVwsSession*,baseSession);

		if (thisSession->ClientThreadId()==aThreadId)
			{
			thisSession->SetActiveView(aViewId);
			}
		else if (thisSession->HasActiveView())
			{
			thisSession->ClearActiveView();
			}
		}
	}

void CVwsServer::ClearActiveView()
	{
	for (iSessionIter.SetToFirst();iSessionIter;iSessionIter++)
		{
		CSession2* baseSession=iSessionIter;
		CVwsSession* thisSession=STATIC_CAST(CVwsSession*,baseSession);

		if (thisSession->HasActiveView())
			{
			thisSession->ClearActiveView();
			break;
			}
		}
	}

void CVwsServer::SetViewAdditionObserver(MVwsViewAdditionObserver* aViewAdditionObserver)
	{
	ASSERT((iViewAdditionObserver==NULL) || (aViewAdditionObserver==NULL));
	iViewAdditionObserver=aViewAdditionObserver;
	}

void CVwsServer::HandleScreenDeviceChangedL()
	{
	LOG2(CVwsLog::ENormal,_L("Received screen device changed notification from WServ, creating event to handle it"));
	CVwsThreadWatcher* threadWatcher=new(ELeave) CVwsThreadWatcher();
	CleanupStack::PushL(threadWatcher);
	CVwsEventTimer* eventTimer=CVwsEventTimer::NewLC(iClientRequestTimeOut,iServerEventTimeOut);
	CVwsServerEvent* screenDeviceChangedEvent=new(ELeave) CVwsServerEvent_ScreenDeviceChanged(*this,*iEventQueue,threadWatcher,eventTimer);
	CleanupStack::Pop(2); // threadWatcher, eventTimer - _ScreenDeviceChanged event is now the owner.
	iEventQueue->ProcessEventL(screenDeviceChangedEvent);
	}

TInt CVwsServer::StartApp(const TUid& aAppUid,TThreadId& aThreadId)
	{
	LOG3(CVwsLog::ENormal,_L("Attempting to start app \"%x\""),aAppUid.iUid);
	TRAPD(err,iAppStarter.StartAppL(aAppUid,aThreadId));
	return err;
	}

TInt CVwsServer::CheckViewExists(const TThreadId& aThreadId,const TVwsViewId& aViewId)
	{
	for (iSessionIter.SetToFirst();iSessionIter;iSessionIter++)
		{
		CSession2* baseSession=iSessionIter;
		CVwsSession* thisSession=STATIC_CAST(CVwsSession*,baseSession);

		if (thisSession->ClientThreadId()==aThreadId)
			{
			return thisSession->CheckViewExists(aViewId);
			}
		}

	return KErrNotFound;
	}

CVwsSession* CVwsServer::ActiveViewSession()
	{
	for (iSessionIter.SetToFirst();iSessionIter;iSessionIter++)
		{
		CSession2* baseSession=iSessionIter;
		CVwsSession* thisSession=STATIC_CAST(CVwsSession*,baseSession);

		if (thisSession->HasActiveView())
			{
			return thisSession;
			}
		}

	return NULL;
	}

CVwsSession* CVwsServer::SessionByUid(const TUid& aAppUid)
	{
	for (iSessionIter.SetToFirst();iSessionIter;iSessionIter++)
		{
		CSession2* baseSession=iSessionIter;
		CVwsSession* thisSession=STATIC_CAST(CVwsSession*,baseSession);

		if (thisSession->AppUid()==aAppUid && !thisSession->Protected())
			{
			return thisSession;
			}
		}

	return NULL;
	}

CVwsSession* CVwsServer::SessionByThreadIdAndAppUid(const TThreadId& aThreadId,const TUid& aAppUid)
	{
	for (iSessionIter.SetToFirst();iSessionIter;iSessionIter++)
		{
		CSession2* baseSession=iSessionIter;
		CVwsSession* thisSession=STATIC_CAST(CVwsSession*,baseSession);

		if (thisSession->ClientThreadId()==aThreadId && thisSession->AppUid()==aAppUid)
			{
			return thisSession;
			}
		}

	return NULL;
	}

CVwsSession* CVwsServer::SessionByThreadId(const TThreadId& aThreadId)
	{
	for (iSessionIter.SetToFirst();iSessionIter;iSessionIter++)
		{
		CSession2* baseSession=iSessionIter;
		CVwsSession* thisSession=STATIC_CAST(CVwsSession*,baseSession);

		if (thisSession->ClientThreadId()==aThreadId)
			{
			return thisSession;
			}
		}

	return NULL;
	}

TBool CVwsServer::IsViewActive(const TVwsViewId& aViewId)
	{
	for (iSessionIter.SetToFirst();iSessionIter;iSessionIter++)
		{
		CSession2* baseSession=iSessionIter;
		CVwsSession* thisSession=STATIC_CAST(CVwsSession*,baseSession);

		if (thisSession->HasActiveView())
			{
			return thisSession->IsViewActive(aViewId);
			}
		}

	return EFalse;
	}

void CVwsServer::RequestDeactivateActiveViewL(const RMessage2& aMessage,const CVwsSession& aSession,TVwsCompleteRequest aCompleteRequest)
	{
	LOG2(CVwsLog::ENormal,_L("Received deactivate active view request, creating event to handle it"));
	CVwsEventTimer* eventTimer=CVwsEventTimer::NewLC(iClientRequestTimeOut,iServerEventTimeOut);
	CVwsServerEvent* deactivateActiveViewEvent=new(ELeave) CVwsServerEvent_DeactivateActiveView(*this,*iEventQueue,aMessage,aSession.ClientThreadId(),aCompleteRequest,eventTimer);
	CleanupStack::Pop(); //  eventTimer - _DeactivateActiveView event is now the owner.
	iEventQueue->ProcessEventL(deactivateActiveViewEvent);
	}

void CVwsServer::RequestAppStartL(const RMessage2& aMessage,const TUid& aAppToStart)
	{
	LOG2(CVwsLog::ENormal,_L("Received start app request, creating event to handle it"));
	CVwsThreadWatcher* threadWatcher=new(ELeave) CVwsThreadWatcher();
	CleanupStack::PushL(threadWatcher);
	CVwsServerEvent* appStartEvent=new(ELeave) CVwsServerEvent_AppStart(*this,*iEventQueue,aMessage,aAppToStart,threadWatcher);
	CleanupStack::Pop();	// threadWatcher
	iEventQueue->ProcessEventL(appStartEvent);
	}

void CVwsServer::HandleDeactivation(const TVwsViewId& aDeactivatedViewId, const TVwsViewId& aActivatedViewId)
	{
	for (iSessionIter.SetToFirst();iSessionIter;iSessionIter++)
		{
		CSession2* baseSession=iSessionIter;
		CVwsSession* thisSession=STATIC_CAST(CVwsSession*,baseSession);
		TRAP_IGNORE(thisSession->HandleDeactivationL(aDeactivatedViewId,aActivatedViewId));
		}
	}

void CVwsServer::HandleActivation(const TVwsViewId& aActivatedViewId, const TVwsViewId& aViewIdToBeDeactivated)
	{
	for (iSessionIter.SetToFirst();iSessionIter;iSessionIter++)
		{
		CSession2* baseSession=iSessionIter;
		CVwsSession* thisSession=STATIC_CAST(CVwsSession*,baseSession);
		TRAP_IGNORE(thisSession->HandleActivationL(aActivatedViewId,aViewIdToBeDeactivated));
		}
	}

void CVwsServer::MakeInterceptingWindowVisible()
	{
	if (iEnableServerBlankScreen)
		{
		iWServSessionHandler->DisplayWindow();
		}
	}

void CVwsServer::MakeInterceptingWindowInvisible()
	{
	if (iEnableServerBlankScreen)
		{
		iWServSessionHandler->HideWindow();
		}
	}

void CVwsServer::MakeInterceptingWindowVisibleAndUpdateScreen()
	{
	if (iEnableServerBlankScreen)
		{
		iWServSessionHandler->UpdateScreenAndDisplayWindow();
		}
	}

void CVwsServer::HandleSessionViewAddition(const TVwsViewId& aViewId, const TThreadId& aNewViewClientThreadId)
	{
	if (iViewAdditionObserver)
		{
		iViewAdditionObserver->HandleViewAdded(aViewId, aNewViewClientThreadId);
		}
	}

void CVwsServer::HandleSessionRemoval(const TThreadId& aThreadId)
	{
	iEventQueue->HandleSessionRemoval(aThreadId);
	}

/* Check that the thread that has keyboard focus also has the active view.
 * Activate a new view if not.
 */
void CVwsServer::CrossCheckForegroundTask()
	{
	//Get the focused window group;
	RWsSession& wsSession=iWServSessionHandler->WsSession();
	TInt fgAppWgId=wsSession.GetFocusWindowGroup();
	if (fgAppWgId)
		{
		CVwsSession* activeSession=ActiveViewSession();
		TThreadId fgAppThreadId;
		//Get the thread that owns this Window group.
		if (wsSession.GetWindowGroupClientThreadId(fgAppWgId,fgAppThreadId)==KErrNotFound)
			{
			fgAppThreadId=TThreadId(0);
			}
		if (!(activeSession && activeSession->ClientThreadId()==fgAppThreadId) && SessionByThreadId(fgAppThreadId))
			{
			LOG2(CVwsLog::ELoud,_L("Foreground app cross-check failed - notifying foreground app that it needs to activate a view"));
			if(iCrossCheckUid!=KNullUid)
				{
				// We can't do anything about the leave here, so trap and ignore it
				TRAP_IGNORE(SendCrossCheckToLauncherL(wsSession, fgAppWgId, ActiveView()));
				}
			else
				{
				TWsEvent event;
				event.SetType(EEventUser);
				*(TApaSystemEvent*)(event.EventData())=EApaSystemEventBroughtToForeground;
				event.SetTimeNow();
				wsSession.SendEventToWindowGroup(fgAppWgId,event);
				}
			}
		}
	}

void CVwsServer::HandleLastServerEventOnQueue()
	{
	MakeInterceptingWindowInvisible();
	if (iForegroundChangeWhileEventsOutstanding)
		{
		iForegroundChangeWhileEventsOutstanding=EFalse;
		CrossCheckForegroundTask();
		}
	}

void CVwsServer::HandleForegroundTaskChange()
	{
	if (iEventQueue->Count()>0)
		{
		iForegroundChangeWhileEventsOutstanding=ETrue;
		}
	else
		{
		CrossCheckForegroundTask();
		}
	}

 void CVwsServer::KickStartEventQ()
     {
     iEventQueue->KickStart();          
     }

CPolicyServer::TCustomResult CVwsServer::CustomSecurityCheckL(const RMessage2& aMsg, TInt& /*aAction*/, TSecurityInfo& /*aMissing*/)
//aAction is not set because default value to it is already set(CPolicyServer::EFailClient)
//aMissing is not set because it is not needed as we aren't overriding CheckFailedL
	{
	CPolicyServer::TCustomResult returnValue = CPolicyServer::EFail;	

	// Check that the client either has SID of Eiksrvs or WriteDeviceData capability
	if(aMsg.SecureId()==KEiksrvsSid || aMsg.HasCapability(ECapabilityWriteDeviceData))
		{
		returnValue = CPolicyServer::EPass;
		}
	
	return(returnValue);
	}
void CVwsServer::GetPriorityForActiveObjectL(TInt& aPriority)
	{
	aPriority=iPrioritySet;
	}

struct TCrossCheckParam
	{
	TVwsViewId	iActiveView;
	TInt		iFgAppWgId;
	};

void CVwsServer::SetCrossCheckUid(const RMessage2& aMessage)
	{
	iCrossCheckUid.iUid=aMessage.Int0();
	}

TUid CVwsServer::CrossCheckUid() const
	{
	return iCrossCheckUid;
	}

void CVwsServer::SendCrossCheckToLauncherL(RWsSession& aWsSession, TInt aFgAppWgId, const TVwsViewId& aActiveView)
	{
	TInt launcherWgId = 0;
	const TInt KPriority = -1;
	TInt wgCount = aWsSession.NumWindowGroups(KPriority);
	if (wgCount)
		{
		CApaWindowGroupName* wgName = CApaWindowGroupName::NewLC(aWsSession);
		CArrayFixFlat<TInt>* wgIdArray = new(ELeave) CArrayFixFlat<TInt>(wgCount);
		CleanupStack::PushL(wgIdArray);
		User::LeaveIfError(aWsSession.WindowGroupList(KPriority, wgIdArray));
		const TInt KCount = wgIdArray->Count();
		for (TInt i = 0; i < KCount; ++i)
			{
			const TInt wgId = (*wgIdArray)[i];
			wgName->ConstructFromWgIdL(wgId);
			if (wgName->AppUid() == iCrossCheckUid)
				{
				launcherWgId = wgId;
				break;
				}
			}
		CleanupStack::PopAndDestroy(2, wgName);		// wgIdArray, wgName
		}

	if (launcherWgId)
		{
		TWsEvent wsEvent;
		wsEvent.SetType(EEventUser + 256);	// Change this later
		wsEvent.SetTimeNow();
		TCrossCheckParam* param = reinterpret_cast<TCrossCheckParam*>(wsEvent.EventData());
		param->iActiveView = aActiveView;
		param->iFgAppWgId = aFgAppWgId;
		aWsSession.SendEventToWindowGroup(launcherWgId, wsEvent);
		}	
	};
	
void CVwsServer::SetWindowBackgroundColor(const RMessage2& aMessage)
	{
	TRgb color;
	color.SetInternal(aMessage.Int0());
	iWServSessionHandler->SetWindowBackgroundColor(color);
	}
	
// Gets the active viewid of the foreground application
void CVwsServer::GetCurrentActiveViewId(TVwsViewId& aActiveViewId)
	{
	aActiveViewId = ActiveView();
	}

// Tells if the application whose view is getting activated is in foreground or background
TBool CVwsServer::IsAppInForeground()
	{
	TBool isAppInForeground = EFalse;
	RWsSession& wsSession = iWServSessionHandler->WsSession();
	TInt fgAppWgId = wsSession.GetFocusWindowGroup();
	if (fgAppWgId)
		{
		CVwsSession* activeSession = ActiveViewSession();
		TThreadId fgAppThreadId;
		// Get the thread that owns this Window group.
		// GetWindowGroupClientThreadId() either returns KErrNone or KErrNotFound error code
		if (wsSession.GetWindowGroupClientThreadId(fgAppWgId,fgAppThreadId) == KErrNotFound)
			{
			fgAppThreadId = TThreadId(0);
			}

		if (!(activeSession && (activeSession->ClientThreadId() == fgAppThreadId)) && SessionByThreadId(fgAppThreadId))
			{
			isAppInForeground = ETrue;
			}
		}
	return isAppInForeground;
	}
	
TBool CVwsServer::InitializationFinished() const
	{
	return iInitializationFinished;
	}
	
TInt CVwsServer::IsPriorityBoostBeforePanicEnabled()const
	{
	return iEnableBoostAppPriorityBeforePanic;
	}

void CVwsServer::HandleInitializationFinished()
 	{
 	iInitializationFinished = ETrue;
 	}

void CVwsServer::BoostPriority(CVwsSession* aClient)
	{
	if (!aClient)
		{
		return;
		}
	if (!IsPriorityBoostBeforePanicEnabled())
		{
		return;
		}
		
	RThread starved;
	if (starved.Open(aClient->ClientThreadId()) != KErrNone)
		{
		return;
		}
	
	if (starved.ProcessPriority() < EPriorityForeground)
		{
		RProcess owningProcess;
		TInt err = starved.Process(owningProcess);
		if (err != KErrNone)
			{
			starved.Close();
			return;
			}
		owningProcess.SetPriority(EPriorityForeground);
		owningProcess.Close();
		}
	
	starved.Close();
	}

