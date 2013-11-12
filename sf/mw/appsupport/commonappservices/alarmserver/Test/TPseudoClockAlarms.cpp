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

// Constants
const TInt KMaxClockAlarms = 8;


static void testSetClockAlarms()
//
//	Set all clock alarms
//
	{
	TheAlarmTest.Test().Next(_L("Set Clock Alarms"));
	TInt n = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n==0, __LINE__);
	//
	TASShdAlarm alarm;
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalDays(3);
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	//
	for (TInt i=0; i<KMaxClockAlarms; i++)
		{
		alarm.Message().Num(i);
		TInt r = TheAlarmTest.Session().AlarmAdd(alarm);
		TheAlarmTest(r == KErrNone, __LINE__);
		}
	n = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n==KMaxClockAlarms, __LINE__);
	}

static TAlarmId SetAlarmLastMonday(TAlarmRepeatDefinition aRepeat, TTimeIntervalMinutes aMinuteOffset = 0)
//
//	Set clock alarm 0 for monday
//
	{
	TTime time;
	time.HomeTime();
	time+=TTimeIntervalDays(-time.DayNoInWeek());
	time+=aMinuteOffset;
	//
	TASShdAlarm alarm;
	alarm.NextDueTime() = time;
	alarm.Message() = _L("test alarm repeat");
	alarm.RepeatDefinition() = aRepeat;
	//
	TInt r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	return alarm.Id();
	}

TAlarmId SetAlarmNextMonday(TAlarmRepeatDefinition aRepeat)
//
//	Set clock alarm 0 for monday
//
	{
	TTime time;
	time.HomeTime();
	time+=TTimeIntervalDays(7-time.DayNoInWeek());
	//
	TASShdAlarm alarm;
	alarm.NextDueTime() = time;
	alarm.Message() = _L("test alarm repeat");
	alarm.RepeatDefinition() = aRepeat;
	//
	TInt r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	return alarm.Id();
	}


static void testSetClockUtcAlarms()
//
//	Set all clock UTC alarms
//
	{
	TheAlarmTest.Test().Next(_L("Set Clock Alarms"));
	TInt n = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n==0, __LINE__);
	//
	TASShdAlarm alarm;
	TTime time;
	time.UniversalTime();
	time += TTimeIntervalDays(3);
	alarm.SetUtcNextDueTime(time);
	alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatOnce;
	//
	for (TInt i=0; i<KMaxClockAlarms; i++)
		{
		TAlarmMessage m = alarm.Message();
		m.Num(i);
		alarm.Message()=m;
		TInt r = TheAlarmTest.Session().AlarmAdd(alarm);
		TheAlarmTest(r == KErrNone, __LINE__);
		}
	n = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n==KMaxClockAlarms, __LINE__);
	}

static TAlarmId SetUtcAlarmLastMonday(TAlarmRepeatDefinition aRepeat, TTimeIntervalMinutes aMinuteOffset = 0)
//
//	Set clock UTC alarm 0 for monday
//
	{
	TTime time;
	time.UniversalTime();
	time+=TTimeIntervalDays(-time.DayNoInWeek());
	time+=aMinuteOffset;
	//
	TASShdAlarm alarm;
	alarm.SetUtcNextDueTime(time);
	alarm.Message()=(_L("test alarm repeat"));
	alarm.RepeatDefinition()=aRepeat;
	//
	TInt r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	return alarm.Id();
	}

TAlarmId SetUtcAlarmNextMonday(TAlarmRepeatDefinition aRepeat)
//
//	Set clock UTC alarm 0 for monday
//
	{
	TTime time;
	time.UniversalTime();
	time+=TTimeIntervalDays(7-time.DayNoInWeek());
	//
	TASShdAlarm alarm;
	alarm.SetUtcNextDueTime(time);
	alarm.Message()=(_L("test alarm repeat"));
	alarm.RepeatDefinition()=aRepeat;
	//
	TInt r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	return alarm.Id();
	}


#ifdef _DEBUG
void ChangeDayToTuesday()
//
//	Change the system time/date to Tuesday
//
	{
	TTime time;
	time.UniversalTime();
	time+=TTimeIntervalDays(7-time.DayNoInWeek()+ETuesday);

    User::SetUTCTime(time);
	User::After(KTimeToWait);
	}
#endif


static void testNextAlarm(TDay aDay, TInt aLineNum, TInt aAlarmNumber=1)
//
//	Test the day of the next alarm
//
	{
	// Wait for a fraction of a second before testing. The alarm
	// server takes some time to update the states of alarm objects
	// and if we request information from the server too quickly after
	// adding an alarm to its queue, it won't have had time to update
	// the state of the objects accordingly.
	User::After(KTimeToWait);
	//
	TInt n = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	if (n != aAlarmNumber)
		{
		TheAlarmTest.Test().Printf(_L("testNextAlarm: aDay %i, aAlarmNumber %i, called from line %u"),
			aDay, aAlarmNumber, aLineNum);
		TheAlarmTest(EFalse, __LINE__);
		}
	//
	if	(aAlarmNumber)
		{
		TInt r;
		TAlarmId nextDueAlarmId;
		//
		r = TheAlarmTest.Session().GetNextDueAlarmId(nextDueAlarmId);
		if (r == KErrNotFound)
			{
			TheAlarmTest.Test().Printf(_L("testNextAlarm: aDay %i, aAlarmNumber %i, called from line %u"),
				aDay, aAlarmNumber, aLineNum);
			TheAlarmTest(EFalse, __LINE__);
			}
		//
		TASShdAlarm alarm;
		r = TheAlarmTest.Session().GetAlarmDetails(nextDueAlarmId, alarm);
		if (r != KErrNone)
			{
			TheAlarmTest.Test().Printf(_L("testNextAlarm: aDay %i, aAlarmNumber %i, called from line %u"),
				aDay, aAlarmNumber, aLineNum);
			TheAlarmTest(EFalse, __LINE__);
			}
		//
		const TDay day = alarm.NextDueTime().DayNoInWeek();
		if (day != aDay)
			{
			TheAlarmTest.Test().Printf(_L("testNextAlarm: aDay %i, aAlarmNumber %i, called from line %u"),
				aDay, aAlarmNumber, aLineNum);
			TheAlarmTest(EFalse, __LINE__);
			}
		}
	}

	
static void testClockRepeats()
//
//	Test clock alarm repeats
//
	{
	TheAlarmTest.Test().Next(_L("Clock Alarm repeats"));
#if !defined(_DEBUG)
	TheAlarmTest.Test().Printf(_L("This test can only be done in debug builds\n"));
#else
	TAlarmId id;
	TInt n;
	TInt r;
	//
	n = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n==0, __LINE__);
	//
	id = SetAlarmNextMonday(EAlarmRepeatDefintionRepeatOnce);
	ChangeDayToTuesday();
	testNextAlarm(EMonday, __LINE__, 0);
	n = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n==0, __LINE__);
	// Don't need to delete it - it shouldn't be there anyway
	//
	id = SetAlarmNextMonday(EAlarmRepeatDefintionRepeatDaily);
	ChangeDayToTuesday();
	testNextAlarm(EWednesday, __LINE__);
	r = TheAlarmTest.Session().AlarmDelete(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	id = SetAlarmNextMonday(EAlarmRepeatDefintionRepeatWeekly);
	ChangeDayToTuesday();
	testNextAlarm(EMonday, __LINE__);
	r = TheAlarmTest.Session().AlarmDelete(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	id = SetAlarmNextMonday(EAlarmRepeatDefintionRepeatWorkday);
	ChangeDayToTuesday();
	testNextAlarm(EWednesday, __LINE__);
	r = TheAlarmTest.Session().AlarmDelete(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	n = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n==0, __LINE__);
#endif
	}

	
static void testPastClockRepeats()
//
//	Test past clock alarm repeats
//
	{
	TheAlarmTest.Test().Next(_L("Past Clock Repeats"));
	//
	TAlarmId id;
	TInt n;
	TInt r;
	//
	n = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n == 0, __LINE__);
	//
	id = SetAlarmLastMonday(EAlarmRepeatDefintionRepeatOnce);
	testNextAlarm(EMonday, __LINE__, 0);
	r = TheAlarmTest.Session().AlarmDelete(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	n = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n == 0, __LINE__);
	//
	TTime time;
	time.HomeTime();
	time+=TTimeIntervalDays(1);
	TDay day=time.DayNoInWeek();
	//
	id = SetAlarmLastMonday(EAlarmRepeatDefintionRepeatDaily);
	testNextAlarm(day, __LINE__);
	r = TheAlarmTest.Session().AlarmDelete(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	id = SetAlarmLastMonday(EAlarmRepeatDefintionRepeatWeekly);
	testNextAlarm(EMonday, __LINE__);
	r = TheAlarmTest.Session().AlarmDelete(id);
	TheAlarmTest(r == KErrNone, __LINE__);

	// Assumes clock repeats use current locale
	TLocale locale;
	locale.SetWorkDays(2); // Tuesday
	locale.Set();
	User::After(KTimeToWait);
	//
	id = SetAlarmLastMonday(EAlarmRepeatDefintionRepeatWorkday);
	testNextAlarm(ETuesday, __LINE__);
	r = TheAlarmTest.Session().AlarmDelete(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	locale.SetWorkDays(4); // Wednesday
	locale.Set();
	User::After(KTimeToWait);
	//
	id = SetAlarmLastMonday(EAlarmRepeatDefintionRepeatWorkday);
	testNextAlarm(EWednesday, __LINE__);
	r = TheAlarmTest.Session().AlarmDelete(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	locale.SetWorkDays(0); // None
	locale.Set();
	User::After(KTimeToWait);
	//
	id = SetAlarmLastMonday(EAlarmRepeatDefintionRepeatWorkday);
	r = TheAlarmTest.Session().AlarmDelete(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	n = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n == 0, __LINE__);
	//
	locale.SetWorkDays(1|2|4|8|16); // Mon-Fri
	locale.Set();
	User::After(KTimeToWait);
	//
	TTimeIntervalMinutes minuteOffSet = -10;
	id = SetAlarmLastMonday(EAlarmRepeatDefintionRepeatNext24Hours,minuteOffSet);
	time.HomeTime();
	time+=minuteOffSet;
	time+=TTimeIntervalDays(1);
	day=time.DayNoInWeek();
	testNextAlarm(day, __LINE__);
	r = TheAlarmTest.Session().AlarmDelete(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	minuteOffSet = 30;
	id = SetAlarmLastMonday(EAlarmRepeatDefintionRepeatNext24Hours,minuteOffSet);
	time.HomeTime();
	//DEF062643 - Increase the time by the same number of minutes as the offset in
	//SetAlarmLastMonday which has the effect of going back 23.5 hrs in
	//the past so that we can be sure that the day does switch over just
	//incase the testcase runs between 23.30 and 24.00 hrs
	time+=minuteOffSet;
	day=time.DayNoInWeek();
	testNextAlarm(day, __LINE__);
	r = TheAlarmTest.Session().AlarmDelete(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	}

static void testNextUtcAlarm(TDay aDay, TInt aLineNum, TInt aAlarmNumber=1)
//
//	Test the day of the next UTC alarm
//
	{
	// Wait for a fraction of a second before testing. The alarm
	// server takes some time to update the states of alarm objects
	// and if we request information from the server too quickly after
	// adding an alarm to its queue, it won't have had time to update
	// the state of the objects accordingly.
	User::After(KTimeToWait);
	//
	TInt n = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	if (!n==aAlarmNumber)
		{
		TheAlarmTest.Test().Printf(_L("testNextUtcAlarm: aDay %i, aAlarmNumber %i, called from line %u"),
			aDay, aAlarmNumber, aLineNum);
		TheAlarmTest(EFalse, __LINE__);
		}
	//
	if	(aAlarmNumber)
		{
		TInt r;
		TAlarmId nextDueAlarmId;
		//
		r = TheAlarmTest.Session().GetNextDueAlarmId(nextDueAlarmId);
		if (r == KErrNotFound)
			{
			TheAlarmTest.Test().Printf(_L("testNextUtcAlarm: aDay %i, aAlarmNumber %i, called from line %u"),
				aDay, aAlarmNumber, aLineNum);
			TheAlarmTest(EFalse, __LINE__);
			}
		//
		TASShdAlarm alarm;
		r = TheAlarmTest.Session().GetAlarmDetails(nextDueAlarmId, alarm);
		if (r != KErrNone)
			{
			TheAlarmTest.Test().Printf(_L("testNextUtcAlarm: aDay %i, aAlarmNumber %i, called from line %u"),
				aDay, aAlarmNumber, aLineNum);
			TheAlarmTest(EFalse, __LINE__);
			}
		//
		const TDay day = alarm.NextDueTime().DayNoInWeek();
		if (day != aDay)
			{
			TheAlarmTest.Test().Printf(_L("testNextUtcAlarm: aDay %i, aAlarmNumber %i, called from line %u"),
				aDay, aAlarmNumber, aLineNum);
			TheAlarmTest(EFalse, __LINE__);
			}
		}
	}


static void testClockRepeatsUtc()
//
//	Test clock UTC alarm repeats
//
	{
	TheAlarmTest.Test().Next(_L("Clock UTC Alarm repeats"));
#if !defined(_DEBUG)
	TheAlarmTest.Test().Printf(_L("This test can only be done in debug builds\n"));
#else
	TAlarmId id;
	TInt n;
	TInt r;
	//
	n = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n==0, __LINE__);
	//
	id = SetUtcAlarmNextMonday(EAlarmRepeatDefintionRepeatOnce);
	ChangeDayToTuesday();
	testNextUtcAlarm(EMonday, __LINE__, 0);
	n = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n==0, __LINE__);
	// Don't need to delete it - it shouldn't be there anyway
	//
	id = SetUtcAlarmNextMonday(EAlarmRepeatDefintionRepeatDaily);
	ChangeDayToTuesday();
	testNextUtcAlarm(EWednesday, __LINE__);
	r = TheAlarmTest.Session().AlarmDelete(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	id = SetUtcAlarmNextMonday(EAlarmRepeatDefintionRepeatWeekly);
	ChangeDayToTuesday();
	testNextUtcAlarm(EMonday, __LINE__);
	r = TheAlarmTest.Session().AlarmDelete(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	id = SetUtcAlarmNextMonday(EAlarmRepeatDefintionRepeatWorkday);
	ChangeDayToTuesday();
	testNextUtcAlarm(EWednesday, __LINE__);
	r = TheAlarmTest.Session().AlarmDelete(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	n = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n==0, __LINE__);
#endif
	}

	
static void testPastClockRepeatsUtc()
//
//	Test past clock UTC alarm repeats
//
	{
	TheAlarmTest.Test().Next(_L("Past Clock Repeats for UTC alarms"));
	//
	TAlarmId id;
	TInt n;
	TInt r;
	//
	n = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n == 0, __LINE__);
	//
	id = SetUtcAlarmLastMonday(EAlarmRepeatDefintionRepeatOnce);
	testNextUtcAlarm(EMonday, __LINE__, 0);
	r = TheAlarmTest.Session().AlarmDelete(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	n = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n == 0, __LINE__);
	//
	TTime time;
	time.UniversalTime();
	time+=TTimeIntervalDays(1);
	TDay day=time.DayNoInWeek();
	//
	id = SetUtcAlarmLastMonday(EAlarmRepeatDefintionRepeatDaily);
	testNextUtcAlarm(day, __LINE__);
	r = TheAlarmTest.Session().AlarmDelete(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	id = SetUtcAlarmLastMonday(EAlarmRepeatDefintionRepeatWeekly);
	testNextUtcAlarm(EMonday, __LINE__);
	r = TheAlarmTest.Session().AlarmDelete(id);
	TheAlarmTest(r == KErrNone, __LINE__);

	// Assumes clock repeats use current locale
	TLocale locale;
	locale.SetWorkDays(2); // Tuesday
	locale.Set();
	User::After(KTimeToWait);
	//
	id = SetUtcAlarmLastMonday(EAlarmRepeatDefintionRepeatWorkday);
	testNextUtcAlarm(ETuesday, __LINE__);
	r = TheAlarmTest.Session().AlarmDelete(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	locale.SetWorkDays(4); // Wednesday
	locale.Set();
	User::After(KTimeToWait);
	//
	id = SetUtcAlarmLastMonday(EAlarmRepeatDefintionRepeatWorkday);
	testNextUtcAlarm(EWednesday, __LINE__);
	r = TheAlarmTest.Session().AlarmDelete(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	locale.SetWorkDays(0); // None
	locale.Set();
	User::After(KTimeToWait);
	//
	id = SetUtcAlarmLastMonday(EAlarmRepeatDefintionRepeatWorkday);
	r = TheAlarmTest.Session().AlarmDelete(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	n = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n == 0, __LINE__);
	//
	locale.SetWorkDays(1|2|4|8|16); // Mon-Fri
	locale.Set();
	User::After(KTimeToWait);
	//
	TTimeIntervalMinutes minuteOffSet = -10;
	id = SetUtcAlarmLastMonday(EAlarmRepeatDefintionRepeatNext24Hours,minuteOffSet);
	time.UniversalTime();
	time+=minuteOffSet;
	time+=TTimeIntervalDays(1);
	day=time.DayNoInWeek();
	testNextUtcAlarm(day, __LINE__);
	r = TheAlarmTest.Session().AlarmDelete(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	//

	minuteOffSet = 30;
	id = SetUtcAlarmLastMonday(EAlarmRepeatDefintionRepeatNext24Hours,minuteOffSet);
	time.UniversalTime();
	//DEF062643 - Increase the time by the same number of minutes as the offset in
	//SetAlarmLastMonday which has the effect of going back 23.5 hrs in
	//the past so that we can be sure that the day does switch over just
	//incase the testcase runs between 23.30 and 24.00 hrs
	time+=minuteOffSet;
	day=time.DayNoInWeek();
	testNextUtcAlarm(day, __LINE__);
	r = TheAlarmTest.Session().AlarmDelete(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	}

/**
@SYMTestCaseID PIM-TPSEUDOCLOCKALARMS-0001 
 */

static void doTestsL()
	{
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	TheAlarmTest.Test().Start(_L("@SYMTestCaseID PIM-TPSEUDOCLOCKALARMS-0001 Connecting to server"));
	TInt r = TheAlarmTest.Session().Connect();
	TheAlarmTest(r == KErrNone, __LINE__);

	TheAlarmTest.TestClearStoreL();
#if defined(_DEBUG)
	TheAlarmTest.Session().__DbgPreventUserNotify(ETrue);
#endif

	testSetClockAlarms();
	TheAlarmTest.TestClearStoreL();
	testClockRepeats();
	testPastClockRepeats();
	TheAlarmTest.TestClearStoreL();

	testSetClockUtcAlarms();
	TheAlarmTest.TestClearStoreL();
	testClockRepeatsUtc();
	testPastClockRepeatsUtc();
	TheAlarmTest.TestClearStoreL();

	CleanupStack::PopAndDestroy(scheduler);
	}

GLDEF_C TInt E32Main()
//
// Test the alarm server.
//
    {
	__UHEAP_MARK;
	TInt ret = KErrNone;
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

