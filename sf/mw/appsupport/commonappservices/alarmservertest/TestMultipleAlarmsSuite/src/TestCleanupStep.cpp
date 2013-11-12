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
// Contains implementation of CTestCleanupStep class
// 
//

/**
 @file
 @internalTechnology 
*/

// User Includes
#include "TestCleanupStep.h"
#include "ProgressCallBack.h"

// System Includes
#include <asclisession.h>

#ifdef SYMBIAN_CALENDAR_V2
#include <calalarm.h>
#else
#include <agmalarm.h>
#endif

/**
Constructor. Sets the test step name. Testserver reference passed to make use 
of TEF's method of sharing data between test steps
@internalTechnology
@test
*/
CTestCleanupStep::CTestCleanupStep(CTestMultipleAlarmsServer& aTestServer) 
: CTestBaseStep(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestCleanupStep);
	}

/**
Base class pure virtual.
@return		EPass or EFail indicating the result of the test step.
@internalTechnology
@test
*/
TVerdict CTestCleanupStep::doTestStepL()
	{
	CActiveScheduler* sched = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	
	TRAPD(error, DoCleanupL());
	PrintIfError(error);
	
	delete sched;
	return TestStepResult();
	}
	
/**
Calls the cleanup functions
@internalTechnology
@test
*/
void CTestCleanupStep::DoCleanupL()
	{
	CCalSession* calSession = NULL;
	TRAPD(error, calSession = CreateAndInitializeCalSessionL(EFalse));
	
	if(error == KErrNone)
		{
		CleanupStack::PushL(calSession);
		CCalEntryView* calEntryView = CreateEntryViewL(calSession);
		CleanupStack::PushL(calEntryView);
	
		DeleteCalEntriesL(calEntryView);
	
		CleanupStack::PopAndDestroy(2, calSession);	// calEntryView and calSession
		}
	
	if(TestServer()->AlertServer())
		{
		DeleteAlarmsL();	
		}
	}
		
/**
Cleans up all the calendar entries. As the tests deal only with imminent alarms
this function makes an assumption and cleans up all the entries that are due 
between the time-range of last year to next year
@internalTechnology
@test
*/
void CTestCleanupStep::DeleteCalEntriesL(CCalEntryView* aCalEntryView)
	{
	// Initialize time range to last year - next year
	TTime now;
	now.HomeTime();
	TCalTime startTime;
	TCalTime endTime;
	startTime.SetTimeLocalL(now - TTimeIntervalYears(1));
	endTime.SetTimeLocalL(now + TTimeIntervalYears(1));
	CalCommon::TCalTimeRange calTimeRange(startTime, endTime);
	
	TRequestStatus status;
	CProgressCallBack* progress = new (ELeave) CProgressCallBack(status);
	CleanupStack::PushL(progress);

	// Delete entries
	aCalEntryView->DeleteL(calTimeRange, CalCommon::EIncludeAll, *progress);

	// Wait till delete is over
	StartActiveSchedAndWaitL(status);
	
	CleanupStack::PopAndDestroy(progress);
	}
	
/**
Cleans up all the alarms created by the agenda engine
@internalTechnology
@test
*/
void CTestCleanupStep::DeleteAlarmsL() 
	{
	// Connect to the Alarm Server
	RASCliSession asCliSession;
	asCliSession = RASCliSession();
	User::LeaveIfError(asCliSession.Connect());
	User::LeaveIfError(asCliSession.AlarmDeleteAllByCategory(KUidAgendaModelAlarmCategory, EFalse));
	asCliSession.Close();
	}
