// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "TEAlarmTestServer.h"
#include "TEAlarmTestSilentForStep.h"
#include "TEAlarmTestSilentUntilStep.h"

#include "TEAlarmTestAlarmPlayConfig.h"
#include "TEAlarmTestMultipleAlarmsStep.h"
#include "tealarmtestgetalarmdatastep.h"

#include <consolealarmalertservermain.h>


_LIT(KServerName, "alarmserverunittestserver");


CTEAlarmTestServer* CTEAlarmTestServer::NewL()
	{
	CTEAlarmTestServer * server = new (ELeave) CTEAlarmTestServer();
	CleanupStack::PushL(server);
	server->StartL(KServerName);
	CleanupStack::Pop(server);
	
	return server;
	}

CTestStep* CTEAlarmTestServer::CreateTestStep(const TDesC& aStepName)
	{
	CTestStep* testStep = NULL;
	
	if (aStepName == KTEAlarmTestSilentForStep)
		{
		testStep = new CTEAlarmTestSilentForStep();
		}
	else if(aStepName == KTEAlarmTestSilentUntilStep)
		{
		testStep = new CTEAlarmTestSilentUntilStep();
		}
	else if(aStepName == KTEAlarmTestAlarmPlayConfigStep)
		{
		testStep = new CTEAlarmTestAlarmPlayConfigStep();
		}
	else if(aStepName == KTEAlarmTestAPCSetIntervalsStep)
		{
		testStep = new CTEAlarmTestAPCSetIntervalsStep();
		}
	else if(aStepName == KTEAlarmTestMultipleAlarmsStep)
		{
		testStep = new CTEAlarmTestMultipleAlarmsStep();
		}
	else if(aStepName == KTEAlarmTestGetAlarmDataStep)
		{
		testStep = new CTEAlarmTestGetAlarmDataStep();
		}

	return testStep;
	}

LOCAL_C void MainL()
	{
	__UHEAP_MARK;
	CActiveScheduler* sched=NULL;
	sched=new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CTEAlarmTestServer* server = NULL;
	// Create the CTestServer derived server
	TRAPD(err,server = CTEAlarmTestServer::NewL());
	
	if(!err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	delete server;
	delete sched;
	__UHEAP_MARKEND;
	}

GLDEF_C TInt E32Main()
	{
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
	
	TRAP_IGNORE(MainL());
	delete cleanup;
	return KErrNone;
	}
