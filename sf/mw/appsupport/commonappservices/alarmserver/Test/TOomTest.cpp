// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ASTstAlarmTest.h"

// System includes
#include <asclisoundplay.h>

// Globals
static TTime time;
static TAlarmId id;
static TInt ret;

// Type definitions
typedef CArrayFixFlat<TASCliSoundPlayDefinition> CPlayIntervalArray;

// This define appears in sa_mng.cpp - update it if it changes there!
#define KDefaultAlarmCycleStartOffsetMinutes	{0,1,2,3,5,10,20,30,45,60,90,120,180,240,KErrNotFound};

static TAlarmId AddAlarm(TTime aTime)
	{
	TASShdAlarm alarm;
	alarm.Category() = KASCliCategoryClock;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	alarm.NextDueTime() = aTime;
	//
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
	const TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);

	return id;
	}

static TAlarmId AddUtcAlarm(TTime aTime)
	{
	TASShdAlarm alarm;
	alarm.Category()=KASCliCategoryClock;
	alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatOnce;
	alarm.SetUtcNextDueTime(aTime);
	//
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
	const TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);

	return id;
	}



static void TestDefaultSoundInfoL()
//
//	Get default sound info details
//
	{
	TheAlarmTest.Test().Next(_L("Test the default sound play intervals"));
	// When the server is started, the alarm sound intervals should currently be
	// set to their defaults defined in KDefaultAlarmCycleStartOffsetMinutes

	// Initialise sound array with default values
	CPlayIntervalArray* array = new(ELeave) CPlayIntervalArray(20);
	CleanupStack::PushL(array);

	// Get default sound intervals
	TheAlarmTest.Session().GetAlarmPlayIntervalsL(*array);

	// Check that the default values are correct
	const TInt minuteCycle[]=KDefaultAlarmCycleStartOffsetMinutes;
	TInt i = 0;
	TInt offset = minuteCycle[i];
	while (offset != KErrNotFound)
		{
		const TASCliSoundPlayDefinition& item = array->At(i);
		TheAlarmTest(item.Offset().Int() == offset, __LINE__);
		TheAlarmTest(item.Duration().Int() == KDefaultSoundPlayDurationInSeconds, __LINE__);
		offset = minuteCycle[++i];
		}
	CleanupStack::PopAndDestroy();
	}

	
static void TestAlarmFunctionalityL()
	{
	TheAlarmTest.Test().Next(_L("Test alarm functionality"));

	TheAlarmTest.TestClearStoreL();

	time.HomeTime();
	time += TTimeIntervalMinutes(1);
	id = AddAlarm(time);
	TASShdAlarm alarm;
	ret = TheAlarmTest.Session().GetAlarmDetails(id, alarm);
	TheAlarmTest(ret == KErrNone, __LINE__);
	TAlarmCategory category;
	ret = TheAlarmTest.Session().GetAlarmCategory(id, category);
	TheAlarmTest(ret == KErrNone, __LINE__);
	TheAlarmTest(category == KASCliCategoryClock, __LINE__);
	ret = TheAlarmTest.Session().SetAlarmStatus(id, EAlarmStatusDisabled);
	TheAlarmTest(ret == KErrNone, __LINE__);
	TAlarmStatus status;
	ret = TheAlarmTest.Session().GetAlarmStatus(id, status);
	TheAlarmTest(ret == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmStatusDisabled, __LINE__);
	}


	
static void TestAlarmDataFunctionalityL()
	{
	TheAlarmTest.Test().Next(_L("Test alarm data functionality"));

	time.HomeTime();
	time += TTimeIntervalMinutes(1);
	id = AddAlarm(time);

	_LIT8(KAlarmData, "some 8-bit data to attach to the alarm");

	ret = TheAlarmTest.Session().AlarmDataAttachL(id, KAlarmData());
	TheAlarmTest(ret == KErrNone, __LINE__);
	ret = TheAlarmTest.Session().AlarmDataSize(id);
	TheAlarmTest(ret > 0, __LINE__);

	HBufC8* temp = HBufC8::NewLC(KAlarmData().Size());
	TPtr8 pData(temp->Des());
	ret = TheAlarmTest.Session().GetAlarmData(id, pData);
	TheAlarmTest(ret == KErrNone, __LINE__);
	TheAlarmTest(pData.CompareC(KAlarmData()) == 0, __LINE__);
	CleanupStack::PopAndDestroy(temp);
	}


static void TestOtherFunctionalityL()
	{
	TheAlarmTest.Test().Next(_L("Test other functionality"));

	TheAlarmTest.TestClearStoreL();

	time.HomeTime();
	time += TTimeIntervalMinutes(1);
	id = AddAlarm(time);
	time -= TTimeIntervalSeconds(30);
	id = AddAlarm(time);

	ret = TheAlarmTest.Session().SetAlarmStatusByCategory(KASCliCategoryClock, EAlarmStatusEnabled);
	TheAlarmTest(ret == KErrNone, __LINE__);

	ret = TheAlarmTest.Session().GetAlarmCountForCategory(KASCliCategoryClock);
	TheAlarmTest(ret == 2, __LINE__);

	RArray<TAlarmCategory> categories;
	TheAlarmTest.Session().GetAvailableCategoryListL(categories);
	TheAlarmTest(categories.Count() == 1, __LINE__);
	categories.Reset();

	RArray<TAlarmId> ids;
	TheAlarmTest.Session().GetAlarmIdListForCategoryL(KASCliCategoryClock, ids);
	TheAlarmTest(ids.Count() == 2, __LINE__);
	ids.Reset();

	ret = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
	TheAlarmTest(ret == 2, __LINE__);

	TheAlarmTest.Session().GetAlarmIdListL(ids);
	TheAlarmTest(ids.Count() == 2, __LINE__);
	ids.Reset();

	TAlarmId nextDueId;
	ret = TheAlarmTest.Session().GetNextDueAlarmId(nextDueId);
	TheAlarmTest(ret == KErrNone && nextDueId == id, __LINE__);

	ret = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(ret == 2, __LINE__);

	ret = TheAlarmTest.Session().AlarmDeleteAllByCategory(KASCliCategoryClock, EFalse);
	TheAlarmTest(ret == KErrNone, __LINE__);
	}

static void TestUtcAlarmFunctionalityL()
	{
	TheAlarmTest.Test().Next(_L("Test UTC alarm functionality"));

	TheAlarmTest.TestClearStoreL();

	time.HomeTime();
	time += TTimeIntervalMinutes(1);
	id = AddUtcAlarm(time);
	TASShdAlarm alarm;
	ret = TheAlarmTest.Session().GetAlarmDetails(id, alarm);
	TheAlarmTest(ret == KErrNone, __LINE__);
	TAlarmCategory category;
	ret = TheAlarmTest.Session().GetAlarmCategory(id, category);
	TheAlarmTest(ret == KErrNone, __LINE__);
	TheAlarmTest(category == KASCliCategoryClock, __LINE__);
	ret = TheAlarmTest.Session().SetAlarmStatus(id, EAlarmStatusDisabled);
	TheAlarmTest(ret == KErrNone, __LINE__);
	TAlarmStatus status;
	ret = TheAlarmTest.Session().GetAlarmStatus(id, status);
	TheAlarmTest(ret == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmStatusDisabled, __LINE__);
	}


	
static void TestUtcAlarmDataFunctionalityL()
	{
	TheAlarmTest.Test().Next(_L("Test alarm data functionality"));

	time.HomeTime();
	time += TTimeIntervalMinutes(1);
	id = AddUtcAlarm(time);

	_LIT8(KAlarmData, "some 8-bit data to attach to the alarm");

	ret = TheAlarmTest.Session().AlarmDataAttachL(id, KAlarmData());
	TheAlarmTest(ret == KErrNone, __LINE__);
	ret = TheAlarmTest.Session().AlarmDataSize(id);
	TheAlarmTest(ret > 0, __LINE__);

	HBufC8* temp = HBufC8::NewLC(KAlarmData().Size());
	TPtr8 pData(temp->Des());
	ret = TheAlarmTest.Session().GetAlarmData(id, pData);
	TheAlarmTest(ret == KErrNone, __LINE__);
	TheAlarmTest(pData.CompareC(KAlarmData()) == 0, __LINE__);
	CleanupStack::PopAndDestroy(temp);
	}


static void TestOtherFunctionalityUtcL()
	{
	TheAlarmTest.Test().Next(_L("Test other functionality"));

	TheAlarmTest.TestClearStoreL();

	time.HomeTime();
	time += TTimeIntervalMinutes(1);
	id = AddUtcAlarm(time);
	time -= TTimeIntervalSeconds(30);
	id = AddUtcAlarm(time);

	ret = TheAlarmTest.Session().SetAlarmStatusByCategory(KASCliCategoryClock, EAlarmStatusEnabled);
	TheAlarmTest(ret == KErrNone, __LINE__);

	ret = TheAlarmTest.Session().GetAlarmCountForCategory(KASCliCategoryClock);
	TheAlarmTest(ret == 2, __LINE__);

	RArray<TAlarmCategory> categories;
	TheAlarmTest.Session().GetAvailableCategoryListL(categories);
	TheAlarmTest(categories.Count() == 1, __LINE__);
	categories.Reset();

	RArray<TAlarmId> ids;
	TheAlarmTest.Session().GetAlarmIdListForCategoryL(KASCliCategoryClock, ids);
	TheAlarmTest(ids.Count() == 2, __LINE__);
	ids.Reset();

	ret = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
	TheAlarmTest(ret == 2, __LINE__);

	TheAlarmTest.Session().GetAlarmIdListL(ids);
	TheAlarmTest(ids.Count() == 2, __LINE__);
	ids.Reset();

	TAlarmId nextDueId;
	ret = TheAlarmTest.Session().GetNextDueAlarmId(nextDueId);
	TheAlarmTest(ret == KErrNone && nextDueId == id, __LINE__);

	ret = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(ret == 2, __LINE__);

	ret = TheAlarmTest.Session().AlarmDeleteAllByCategory(KASCliCategoryClock, EFalse);
	TheAlarmTest(ret == KErrNone, __LINE__);
	}


static void TestSoundControlL()
	{
	TheAlarmTest.Test().Next(_L("Test sound control"));

	TheAlarmTest.TestClearStoreL();

	ret = TheAlarmTest.Session().SetAlarmSoundState(EAlarmGlobalSoundStateOff);
	TheAlarmTest(ret == KErrNone, __LINE__);

	TAlarmGlobalSoundState soundState;
	ret = TheAlarmTest.Session().GetAlarmSoundState(soundState);
	TheAlarmTest(ret == KErrNone && soundState == EAlarmGlobalSoundStateOff, __LINE__);

	time.HomeTime();
	time += TTimeIntervalMinutes(1);
	ret = TheAlarmTest.Session().SetAlarmSoundsSilentUntil(time);
	TheAlarmTest(ret == KErrNone, __LINE__);

	ret = TheAlarmTest.Session().CancelAlarmSilence();
	TheAlarmTest(ret == KErrNone, __LINE__);

	ret = TheAlarmTest.Session().SetAlarmSoundsSilentFor(1);
	TheAlarmTest(ret == KErrNone, __LINE__);

	ret = TheAlarmTest.Session().GetAlarmSoundsSilentUntil(time);
	TheAlarmTest(ret == KErrNone, __LINE__);

	if (!TheAlarmTest.Session().AlarmSoundsTemporarilySilenced())
		{
		User::Leave(KErrGeneral);
		}
	}

	
static void TestStartUpAndShutDownL()
	{
#ifdef _DEBUG
#ifdef __WINS__
	TheAlarmTest.Test().Next(_L("Test start up and shut down"));

	TheAlarmTest.Session().__DbgShutDownServer();
	TheAlarmTest.TestStartServers();
	TheAlarmTest.TestClearStoreL();

	time.HomeTime();
	time += TTimeIntervalMinutes(5);
	id = AddAlarm(time);

	TheAlarmTest.Test().Printf(_L("\nDebug only - shutting down server\n"));
	TheAlarmTest.Session().__DbgShutDownServer();
	User::After(3 * 1000000);

	TheAlarmTest.TestStartServers();

	TAlarmId nextDueId;
	ret = TheAlarmTest.Session().GetNextDueAlarmId(nextDueId);
	TheAlarmTest(ret == KErrNone && nextDueId == id, __LINE__);

	time -= TTimeIntervalMinutes(1);
	id = AddAlarm(time);

	TheAlarmTest.Session().__DbgShutDownServer();
	User::After(3 * 1000000);

	TheAlarmTest.TestStartServers();

	ret = TheAlarmTest.Session().GetNextDueAlarmId(nextDueId);
	TheAlarmTest(ret == KErrNone && nextDueId == id, __LINE__);
#endif
#endif
	}


static void TestStartUpAndShutDownUtcL()
	{
#ifdef _DEBUG
#ifdef __WINS__
	TheAlarmTest.Test().Next(_L("Test start up and shut down with UTC Alarms"));

	TheAlarmTest.Session().__DbgShutDownServer();
	TheAlarmTest.TestStartServers();
	TheAlarmTest.TestClearStoreL();

	time.HomeTime();
	time += TTimeIntervalMinutes(5);
	id = AddUtcAlarm(time);

	TheAlarmTest.Test().Printf(_L("\nDebug only - shutting down server\n"));
	TheAlarmTest.Session().__DbgShutDownServer();
	User::After(3 * 1000000);

	TheAlarmTest.TestStartServers();

	TAlarmId nextDueId;
	ret = TheAlarmTest.Session().GetNextDueAlarmId(nextDueId);
	TheAlarmTest(ret == KErrNone && nextDueId == id, __LINE__);

	time -= TTimeIntervalMinutes(1);
	id = AddUtcAlarm(time);

	TheAlarmTest.Session().__DbgShutDownServer();
	User::After(3 * 1000000);

	TheAlarmTest.TestStartServers();

	ret = TheAlarmTest.Session().GetNextDueAlarmId(nextDueId);
	TheAlarmTest(ret == KErrNone && nextDueId == id, __LINE__);
#endif
#endif
	}


static void DoTestsL()
	{
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	TheAlarmTest.Next(_L("Connecting to server"));
	const TInt r = TheAlarmTest.Session().Connect();
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest.TestClearStoreL();

	TestAlarmFunctionalityL();
	TestAlarmDataFunctionalityL();
	TestOtherFunctionalityL();

	TestStartUpAndShutDownL();

	TheAlarmTest.TestClearStoreL();

	TestDefaultSoundInfoL();
	TestUtcAlarmFunctionalityL();
	TestUtcAlarmDataFunctionalityL();
	TestOtherFunctionalityUtcL();
	TestSoundControlL();

	TestStartUpAndShutDownUtcL();

	CleanupStack::PopAndDestroy(scheduler);
	}

void DoOOMTestL()
	{
	TheAlarmTest.Test().Printf(_L("starting OOM test\n"));

	//reserve space on cleanup stack
	for(TInt i=0; i<1000; i++)
		{
		CleanupStack::PushL(&i);
		}
	CleanupStack::Pop(1000);

	TInt ret=KErrNoMemory;
	TInt failAt=0;
	while (ret!=KErrNone)
		{
		failAt++;
		__UHEAP_SETFAIL(RHeap::EDeterministic,failAt);
		__UHEAP_MARK;
		TRAP(ret, DoTestsL());
		if (ret!=KErrNone)
			{
			__UHEAP_MARKEND;
			}
		__UHEAP_RESET;
		TheAlarmTest(ret==KErrNoMemory||ret==KErrNone, __LINE__);
		}
	}

/**
@SYMTestCaseID PIM-TOOMTEST-0001
*/	
GLDEF_C TInt E32Main()
//
// Test the alarm server.
//
	{
	__UHEAP_MARK;
	TInt ret = KErrNone;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if (!cleanup)
		{
		return KErrNoMemory;
		}

	TheAlarmTest.Start(_L("@SYMTestCaseID PIM-TOOMTEST-0001"));
	TheAlarmTest.Title();
	TheAlarmTest.TestStartServers();

	TRAPD(error, DoOOMTestL());

	TheAlarmTest(error == KErrNone, __LINE__);
	TRAP(ret,TheAlarmTest.EndL());
    TheAlarmTest.Test().Close();

	ASTstAlarmTest::Close();

	delete cleanup;
	__UHEAP_MARKEND;
	return ret;
	}
