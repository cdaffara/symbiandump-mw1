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

// Globals
static TRequestStatus TheStatus;

TAlarmId addAlarm(const TTime& aTime)
	{
	TheAlarmTest.Test().Printf(_L("Adding alarm\n"));
	//
	TASShdAlarm alarm;
	alarm.Category() = KASCliCategoryClock;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	alarm.NextDueTime()=aTime;
	//
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
	const TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);

	return id;
	}

TAlarmId addUtcAlarm(const TTime& aTime)
	{
	TheAlarmTest.Test().Printf(_L("Adding UTC alarm\n"));
	//
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


void waitForAlarmToExpire()
	{
	TheAlarmTest.Test().Printf(_L("Waiting for alarm to expire...\n"));

	TAlarmId alarmId;
	TRequestStatus status;
	FOREVER
		{
		TheAlarmTest.Session().NotifyChange(status, alarmId);
		User::WaitForRequest(status);
		if	(status.Int() == EAlarmChangeEventTimerExpired)
			break;
		}
	}

void verifyStates(TInt aQueued, TInt aNotifying=0, TInt aWaitingToNotify=0, TInt aNotified=0, TInt aSnoozed=0)
	{
	TInt queued = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
	TInt notifying = TheAlarmTest.Session().AlarmCountByState(EAlarmStateNotifying);
	TInt waitingToNotify = TheAlarmTest.Session().AlarmCountByState(EAlarmStateWaitingToNotify);
	TInt notified = TheAlarmTest.Session().AlarmCountByState(EAlarmStateNotified);
	TInt snoozed = TheAlarmTest.Session().AlarmCountByState(EAlarmStateSnoozed);
	//
	TheAlarmTest(queued == aQueued, __LINE__);
	TheAlarmTest(notifying == aNotifying, __LINE__);
	TheAlarmTest(waitingToNotify == aWaitingToNotify, __LINE__);
	TheAlarmTest(notified == aNotified, __LINE__);
	TheAlarmTest(snoozed == aSnoozed, __LINE__);
	}

void checkGlobalSoundState(TAlarmGlobalSoundState aGlobalSoundState)
	{
	TAlarmGlobalSoundState soundState;
	TInt r = TheAlarmTest.Session().GetAlarmSoundState(soundState);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(soundState == aGlobalSoundState, __LINE__);
	}

//
//
//
//
//

void simpleTestOfGlobalSoundState()
	{
	TheAlarmTest.Test().Next(_L("test global sound state"));

	checkGlobalSoundState(EAlarmGlobalSoundStateOn);
	TheAlarmTest.Session().SetAlarmSoundState(EAlarmGlobalSoundStateOff);
	checkGlobalSoundState(EAlarmGlobalSoundStateOff);
	TheAlarmTest.Session().SetAlarmSoundState(EAlarmGlobalSoundStateOn);
	checkGlobalSoundState(EAlarmGlobalSoundStateOn);

	TheAlarmTest.Test().Printf(_L("- test silent period\n"));
	TheAlarmTest.Session().SetAlarmSoundsSilentFor(TTimeIntervalMinutes(1));
	checkGlobalSoundState(EAlarmGlobalSoundStateOff);
	//wait for the silent period to expire
	User::After(1000000 * 61);
	checkGlobalSoundState(EAlarmGlobalSoundStateOn);
	}

void testSoundsSilentUntilL()
	{
	TheAlarmTest.Test().Next(_L("test sounds silent until"));

	TheAlarmTest.TestClearStoreL();

	// Add an alarm
	TTime time;
	time.HomeTime();
	time += TTimeIntervalSeconds(10);
	TInt alarmId = addAlarm(time);

	// Silence alarms until 1 minute after this alarm expires - this gets rounded
	// down to the nearest minute - do here so we have the actual time silenced until
	time += TTimeIntervalMinutes(1);
	TDateTime temp(time.DateTime());
	temp.SetSecond(0);
	temp.SetMicroSecond(0);
	time = temp;

	TInt r = TheAlarmTest.Session().SetAlarmSoundsSilentUntil(time);
	TheAlarmTest(r == KErrNone, __LINE__);

  	// Check silent until time
  	TTime checkTime;
  	r = TheAlarmTest.Session().GetAlarmSoundsSilentUntil(checkTime);
  	TheAlarmTest(r == KErrNone, __LINE__);
  	TheAlarmTest(time == checkTime, __LINE__);

	// Wait for the alarm to expire
	waitForAlarmToExpire();

	// Test sound state is correct
	TheAlarmTest.Test().Printf(_L("- alarm expired, check silent\r\n"));
	checkGlobalSoundState(EAlarmGlobalSoundStateOff);
	TheAlarmTest(TheAlarmTest.Session().AlarmSoundsTemporarilySilenced(), __LINE__);

	// Wait until sounds are allowed again
	User::At(time);

	TheAlarmTest.Test().Printf(_L("- silence expired, check sounding\r\n"));
	checkGlobalSoundState(EAlarmGlobalSoundStateOn);
	TheAlarmTest(!TheAlarmTest.Session().AlarmSoundsTemporarilySilenced(), __LINE__);

	// delete this alarm
	r = TheAlarmTest.Session().AlarmDelete(alarmId);
	TheAlarmTest(r == KErrNone, __LINE__);
	}

void simpleTestOfSoundsSilentForL()
	{
	TheAlarmTest.Test().Next(_L("test sounds silent for - 1"));

	TheAlarmTest.TestClearStoreL();

	// Add 3 alarms, due in 1, 2 and 4 minutes
	TTime time;
	time.HomeTime();
	time += TTimeIntervalMinutes(1);
	addAlarm(time);
	time += TTimeIntervalMinutes(1);
	addAlarm(time);
	time += TTimeIntervalMinutes(2);
	addAlarm(time);

	// Silence alarms for 3 minutes
	TInt r = TheAlarmTest.Session().SetAlarmSoundsSilentFor(3);
	TheAlarmTest(r == KErrNone, __LINE__);

	//wait for first alarm to expire - should be silent
	waitForAlarmToExpire();
	TheAlarmTest.Test().Printf(_L("- alarm 1 expired, check silent\r\n"));
	checkGlobalSoundState(EAlarmGlobalSoundStateOff);
	TheAlarmTest(TheAlarmTest.Session().AlarmSoundsTemporarilySilenced(), __LINE__);

	//wait for the next alarm to expire - should still be silent
	waitForAlarmToExpire();
	TheAlarmTest.Test().Printf(_L("- alarm 2 expired, check silent\r\n"));
	checkGlobalSoundState(EAlarmGlobalSoundStateOff);
	TheAlarmTest(TheAlarmTest.Session().AlarmSoundsTemporarilySilenced(), __LINE__);

	//wait 1 minute for the silent period to expire
	User::After(1000000 * 61);
	TheAlarmTest.Test().Printf(_L("- silence expired, check sound state\r\n"));
	checkGlobalSoundState(EAlarmGlobalSoundStateOn);
	TheAlarmTest(!TheAlarmTest.Session().AlarmSoundsTemporarilySilenced(), __LINE__);

	//wait for the next alarm to expire - sounds should be back on
	waitForAlarmToExpire();
	TheAlarmTest.Test().Printf(_L("- alarm 3 expired, check sound state\r\n"));
	checkGlobalSoundState(EAlarmGlobalSoundStateOn);
	TheAlarmTest(!TheAlarmTest.Session().AlarmSoundsTemporarilySilenced(), __LINE__);
	}

void testSoundsSilentFor()
	{
	TheAlarmTest.Test().Next(_L("test sounds silent for - 2"));

	// add 2 alarms to expire immediately
	TTime time;
	time.HomeTime();
	addAlarm(time);
	addAlarm(time);
	checkGlobalSoundState(EAlarmGlobalSoundStateOn);
	TheAlarmTest(!TheAlarmTest.Session().AlarmSoundsTemporarilySilenced(), __LINE__);

	// Silence alarms for 1 minute, wait 10 seconds, then silence again for 1 minute
	TInt r = TheAlarmTest.Session().SetAlarmSoundsSilentFor(1);
	TheAlarmTest(r == KErrNone, __LINE__);
	User::After(1000000 * 10);
	r = TheAlarmTest.Session().SetAlarmSoundsSilentFor(1);
	TheAlarmTest(r == KErrNone, __LINE__);
	// expect to be silent for 70 seconds (from alarm time)
	time += TTimeIntervalSeconds(65);
	User::At(time);
	// approx 5 seconds before silence ends - check still silent
	TheAlarmTest.Test().Printf(_L("- check sound state is still silent\r\n"));
	checkGlobalSoundState(EAlarmGlobalSoundStateOff);
	TheAlarmTest(TheAlarmTest.Session().AlarmSoundsTemporarilySilenced(), __LINE__);

	time += TTimeIntervalSeconds(7);
	User::At(time);
	// 2 seconds after silence should end - check state
	TheAlarmTest.Test().Printf(_L("- check sound state is now sounding\r\n"));
	checkGlobalSoundState(EAlarmGlobalSoundStateOn);
	TheAlarmTest(!TheAlarmTest.Session().AlarmSoundsTemporarilySilenced(), __LINE__);

	// test cancel alarm silence
	TheAlarmTest.Test().Printf(_L("- check sound state is silent\r\n"));
	r = TheAlarmTest.Session().SetAlarmSoundsSilentFor(10);
	TheAlarmTest(r == KErrNone, __LINE__);
	checkGlobalSoundState(EAlarmGlobalSoundStateOff);
	TheAlarmTest(TheAlarmTest.Session().AlarmSoundsTemporarilySilenced(), __LINE__);

	TheAlarmTest.Test().Printf(_L("- check silence is canceled\r\n"));
	r = TheAlarmTest.Session().CancelAlarmSilence();
	checkGlobalSoundState(EAlarmGlobalSoundStateOn);
	TheAlarmTest(!TheAlarmTest.Session().AlarmSoundsTemporarilySilenced(), __LINE__);
	}


void testSoundsSilentUntilUtcL()
	{
	TheAlarmTest.Test().Next(_L("test sounds silent until"));

	TheAlarmTest.TestClearStoreL();

	// Add an alarm
	TTime time;
	time.UniversalTime();
	time += TTimeIntervalSeconds(10);
	TInt alarmId = addUtcAlarm(time);

	// Silence alarms until 1 minute after this alarm expires - this gets rounded
	// down to the nearest minute - do here so we have the actual time silenced until
	TTimeIntervalSeconds offset = User::UTCOffset();
	time += TTimeIntervalMinutes(1);
	time += offset;
	TDateTime temp(time.DateTime());
	temp.SetSecond(0);
	temp.SetMicroSecond(0);
	time = temp;

	TInt r = TheAlarmTest.Session().SetAlarmSoundsSilentUntil(time);
	TheAlarmTest(r == KErrNone, __LINE__);

  	// Check silent until time
  	TTime checkTime;
  	r = TheAlarmTest.Session().GetAlarmSoundsSilentUntil(checkTime);
  	TheAlarmTest(r == KErrNone, __LINE__);
  	TheAlarmTest(time == checkTime, __LINE__);

	// Wait for the alarm to expire
	waitForAlarmToExpire();

	// Test sound state is correct
	TheAlarmTest.Test().Printf(_L("- alarm expired, check silent\r\n"));
	checkGlobalSoundState(EAlarmGlobalSoundStateOff);
	TheAlarmTest(TheAlarmTest.Session().AlarmSoundsTemporarilySilenced(), __LINE__);

	// Wait until sounds are allowed again
	User::At(time);

	TheAlarmTest.Test().Printf(_L("- silence expired, check sounding\r\n"));
	checkGlobalSoundState(EAlarmGlobalSoundStateOn);
	TheAlarmTest(!TheAlarmTest.Session().AlarmSoundsTemporarilySilenced(), __LINE__);

	// delete this alarm
	r = TheAlarmTest.Session().AlarmDelete(alarmId);
	TheAlarmTest(r == KErrNone, __LINE__);
	}


void simpleTestOfSoundsSilentForUtcL()
	{
	TheAlarmTest.Test().Next(_L("test sounds silent for - 1"));

	TheAlarmTest.TestClearStoreL();

	// Add 3 alarms, due in 1, 2 and 4 minutes
	TTime time;
	time.UniversalTime();
	time += TTimeIntervalMinutes(1);
	addUtcAlarm(time);
	time += TTimeIntervalMinutes(1);
	addUtcAlarm(time);
	time += TTimeIntervalMinutes(2);
	addUtcAlarm(time);

	// Silence alarms for 3 minutes
	TInt r = TheAlarmTest.Session().SetAlarmSoundsSilentFor(3);
	TheAlarmTest(r == KErrNone, __LINE__);

	//wait for first alarm to expire - should be silent
	waitForAlarmToExpire();
	TheAlarmTest.Test().Printf(_L("- alarm 1 expired, check silent\r\n"));
	checkGlobalSoundState(EAlarmGlobalSoundStateOff);
	TheAlarmTest(TheAlarmTest.Session().AlarmSoundsTemporarilySilenced(), __LINE__);

	//wait for the next alarm to expire - should still be silent
	waitForAlarmToExpire();
	TheAlarmTest.Test().Printf(_L("- alarm 2 expired, check silent\r\n"));
	checkGlobalSoundState(EAlarmGlobalSoundStateOff);
	TheAlarmTest(TheAlarmTest.Session().AlarmSoundsTemporarilySilenced(), __LINE__);

	//wait 1 minute for the silent period to expire
	User::After(1000000 * 61);
	TheAlarmTest.Test().Printf(_L("- silence expired, check sound state\r\n"));
	checkGlobalSoundState(EAlarmGlobalSoundStateOn);
	TheAlarmTest(!TheAlarmTest.Session().AlarmSoundsTemporarilySilenced(), __LINE__);

	//wait for the next alarm to expire - sounds should be back on
	waitForAlarmToExpire();
	TheAlarmTest.Test().Printf(_L("- alarm 3 expired, check sound state\r\n"));
	checkGlobalSoundState(EAlarmGlobalSoundStateOn);
	TheAlarmTest(!TheAlarmTest.Session().AlarmSoundsTemporarilySilenced(), __LINE__);
	}
	
void testSoundsSilentForUtc()
	{
	TheAlarmTest.Test().Next(_L("test sounds silent for - 2"));

	// add 2 alarms to expire immediately
	TTimeIntervalSeconds offset = User::UTCOffset();

	TTime time;
	time.UniversalTime();
	addUtcAlarm(time);
	addUtcAlarm(time);
	checkGlobalSoundState(EAlarmGlobalSoundStateOn);
	TheAlarmTest(!TheAlarmTest.Session().AlarmSoundsTemporarilySilenced(), __LINE__);

	// Silence alarms for 1 minute, wait 10 seconds, then silence again for 1 minute
	TInt r = TheAlarmTest.Session().SetAlarmSoundsSilentFor(1);
	TheAlarmTest(r == KErrNone, __LINE__);
	User::After(1000000 * 10);
	r = TheAlarmTest.Session().SetAlarmSoundsSilentFor(1);
	TheAlarmTest(r == KErrNone, __LINE__);
	// expect to be silent for 70 seconds (from alarm time)
	time += TTimeIntervalSeconds(65);
	User::At(time + offset);
	// approx 5 seconds before silence ends - check still silent
	TheAlarmTest.Test().Printf(_L("- check sound state is still silent\r\n"));
	checkGlobalSoundState(EAlarmGlobalSoundStateOff);
	TheAlarmTest(TheAlarmTest.Session().AlarmSoundsTemporarilySilenced(), __LINE__);

	time += TTimeIntervalSeconds(7);
	User::At(time + offset);
	// 2 seconds after silence should end - check state
	TheAlarmTest.Test().Printf(_L("- check sound state is now sounding\r\n"));
	checkGlobalSoundState(EAlarmGlobalSoundStateOn);
	TheAlarmTest(!TheAlarmTest.Session().AlarmSoundsTemporarilySilenced(), __LINE__);

	// test cancel alarm silence
	TheAlarmTest.Test().Printf(_L("- check sound state is silent\r\n"));
	r = TheAlarmTest.Session().SetAlarmSoundsSilentFor(10);
	TheAlarmTest(r == KErrNone, __LINE__);
	checkGlobalSoundState(EAlarmGlobalSoundStateOff);
	TheAlarmTest(TheAlarmTest.Session().AlarmSoundsTemporarilySilenced(), __LINE__);

	TheAlarmTest.Test().Printf(_L("- check silence is canceled\r\n"));
	r = TheAlarmTest.Session().CancelAlarmSilence();
	checkGlobalSoundState(EAlarmGlobalSoundStateOn);
	TheAlarmTest(!TheAlarmTest.Session().AlarmSoundsTemporarilySilenced(), __LINE__);
	}

/**
@SYMTestCaseID PIM-TSOUNDCONTROL-0001
*/
static void doTestsL()
	{
	TheAlarmTest.Start(_L("@SYMTestCaseID PIM-TSOUNDCONTROL-0001 Connecting to server"));

	const TInt KGmtOffset = 3600;
	_LIT(KTime, "20040730:050000.000000"); //In summer time
	TInt err = User::SetUTCTimeAndOffset(TTime(KTime), KGmtOffset);
	TheAlarmTest(err == KErrNone, __LINE__);


	TInt r = TheAlarmTest.Session().Connect();
	TheAlarmTest(r == KErrNone, __LINE__);

	TheAlarmTest.TestClearStoreL();

	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	simpleTestOfGlobalSoundState();

	testSoundsSilentUntilL();
	simpleTestOfSoundsSilentForL();
	testSoundsSilentFor();

	testSoundsSilentUntilUtcL();
	simpleTestOfSoundsSilentForUtcL();
	testSoundsSilentForUtc();

	TheAlarmTest.TestClearStoreL();

	TheAlarmTest.Session().Close();

	CleanupStack::PopAndDestroy(scheduler);
	}

GLDEF_C TInt E32Main()
//
// Test the alarm server.
//
    {
    TInt ret = KErrNone;
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if	(!cleanup)
		return KErrNoMemory;

	TheAlarmTest.Title();
	TheAlarmTest.TestStartServers();
	//
	TRAPD(error, doTestsL());
	TheAlarmTest(error == KErrNone, __LINE__);
	//
    TRAP(ret,TheAlarmTest.EndL());
	TheAlarmTest.Test().Close();
	ASTstAlarmTest::Close();

	delete cleanup;
	__UHEAP_MARKEND;
	return ret;
    }
