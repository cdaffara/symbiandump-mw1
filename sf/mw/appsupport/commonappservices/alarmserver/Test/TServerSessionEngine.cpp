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

#include "TServerSessionEngine.h"

// System includes
#include <e32base.h>
#include <e32test.h>
#include <f32file.h>

// User includes
#include "ASSrvServerWideData.h"
#include "ASSrvTimer.h"
#include <consolealarmalertservermain.h>
//

// Constants
const TUid KServerUid2={0x1000008D};
const TUid KServerUid3={0x101F502A};
const TUidType serverUid(KNullUid,KServerUid2,KServerUid3);




//
// ----> CTestServerSessionEngine (source)
//

//*************************************************************************************
CTestServerSessionEngine::CTestServerSessionEngine()
	{
	iHomeTime.HomeTime();
	}


//*************************************************************************************
CTestServerSessionEngine::~CTestServerSessionEngine()
	{
	delete iTimerObserver;	
	delete iSessionEngine;
	delete iServerWideData;
	delete iSession;
	delete iChangeObserver;

	// delete the active scheduler
	delete CActiveScheduler::Current();
	}


//*************************************************************************************
void CTestServerSessionEngine::ConstructL()
	{
	CActiveScheduler* iScheduler = new(ELeave) CActiveScheduler();
	CActiveScheduler::Install(iScheduler);

	StartAlarmServerL();

	//
	iChangeObserver = new(ELeave) TASSrvAnyEventObserver();
	iSession = new(ELeave) TASSrvSession();
	iServerWideData = CASSrvServerWideData::NewL();
	iSessionEngine = CASSrvSessionEngine::NewL(*iServerWideData, *iChangeObserver, *iSession);

	iTimerObserver = new(ELeave) TASSrvAlarmTimerObserver();
	}


//*************************************************************************************
CTestServerSessionEngine* CTestServerSessionEngine::NewLC()
	{
	CTestServerSessionEngine* self = new(ELeave) CTestServerSessionEngine();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}


//
//
//


//*************************************************************************************
void CTestServerSessionEngine::StartAlarmServerL()
	{
#ifdef __WINS__ 
	// Have to start the alarm alert server otherwise the alarm
	// server cannot start.
	_LIT(KConsoleAlarmAlertServerImg,"ConsoleAlarmAlertServer");
	RProcess server;
	server.Create(KConsoleAlarmAlertServerImg, KNullDesC, serverUid);
	TRequestStatus stat;
	server.Rendezvous(stat);
	if (stat != KRequestPending)
		{
		server.Kill(0);		// abort startup
		}
	else
		{
		server.Resume();	// logon OK - start the server
		}
		
	User::WaitForRequest(stat);
	TInt result = stat.Int();
	if(result != KErrNone && result != KErrAlreadyExists)
		{
		User::LeaveIfError(result);	
		}		
#endif //  __WINS__ 
	}


//
//
//


//*************************************************************************************
TAlarmId CTestServerSessionEngine::AddAlarmWithSpecificTimeL(TInt aSecondsFromNow)
	{
	TASSrvAlarm alarm(ServerWideData());
	alarm.Category()=KASCliCategoryClock;
	alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatOnce;
	TTime time = iHomeTime;
	time += TTimeIntervalSeconds(aSecondsFromNow);
	// convert time to UTC
	time -= User::UTCOffset();
	alarm.SetUtcNextDueTime(time);
	iSessionEngine->AlarmAddL(alarm);

	const TAlarmId id = alarm.Id();
	if(id == KNullAlarmId)
		User::Leave(KErrGeneral);

	return id;
	}


//*************************************************************************************
TAlarmId CTestServerSessionEngine::AddAlarmWithSpecificCategoryL(TAlarmCategory aCategory)
	{
	TASSrvAlarm alarm(ServerWideData());
	alarm.Category()=aCategory;
	alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatOnce;
	TTime time = iHomeTime;
	// convert time to UTC
	time -= User::UTCOffset();
	time += TTimeIntervalSeconds(10);
	alarm.SetUtcNextDueTime(time);
	iSessionEngine->AlarmAddL(alarm);

	const TAlarmId id = alarm.Id();
	if(id == KNullAlarmId)
		User::Leave(KErrGeneral);

	return id;
	}

//*************************************************************************************
void CTestServerSessionEngine::AwaitNotificationL()
	{
	iServerWideData->Timer().NotifyAlarmExpiredL(*iTimerObserver);
	}

//*************************************************************************************
void CTestServerSessionEngine::ClearAlarmStoreL()
	{
	// Delete all alarms
	RArray<TAlarmCategory>* categories = iSessionEngine->AlarmCategoryListLC();
	TInt count = categories->Count();
	for(TInt i=0; i<count; i++)
		{
		const TAlarmCategory category = (*categories)[i];
		iSessionEngine->DeleteAllAlarmsByCategoryL(category, EFalse);
		}
	categories->Close();
	CleanupStack::PopAndDestroy(categories);
	}


//
//
//

//
// ----> TASSrvAnyEventObserver (source)
//

void TASSrvAnyEventObserver::MASSrvAnyEventHandleChange(TAlarmChangeEvent /*aEvent*/, TAlarmId /*aAlarmId*/)
	{
	}

//
//
//

//
// ----> TASSrvSession (source)
//

TASSrvSessionId TASSrvSession::MASSrvSessionId() const
	{
	return 0;
	}

void TASSrvSession::MASSrvSessionFullName(TDes& aDes) const
	{
	aDes = KNullDesC;
	}

//
//
//

//
// ----> TASSrvAlarmTimerObserver (source)
//

void TASSrvAlarmTimerObserver::MATimerHandleAlarmExpired(TAlarmTimerEvent aEvent, TAlarmId /*aAlarmId*/)
	{
	if (aEvent == EAlarmTimerEventAlarmExpired)
		{
		CActiveScheduler::Stop();
		}
	}
