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
#include "testserver.h"
#include <tz.h>

// Globals
static TTime TheOriginalTime;
static TTime TheInitialAlarmTime;
static TTime TheCurrentTime;
TBool AlarmSetInThePastTest = EFalse;

// Definitions:

/* These two #defines make the code a lot safer, since the enums in TDay
	are not tied to any specific values. All code expecting specific values
	need to use them relative to EMonday  (DoW = Day of week) */
#define DoWNumber(x)   ((TInt)(x-EMonday))
#define DoWFlag(x)   (1<<((TInt)(x-EMonday)))

_LIT(KDateTimeFormat, "%F %*E %*N %D %Y %J:%T:%S%B");

static void PrintTimeL(const TTime& aTime, const TDesC& aPrefix)
	{
	TBuf<30> dateTimeString;
	aTime.FormatL(dateTimeString, KDateTimeFormat);
	TheAlarmTest.Test().Printf(_L("%S %S\n"), &aPrefix, &dateTimeString);
	}

static void AdjustToNextWorkDay(TTime& aSetTime, const TTime& aCurrentTime)
	{
	TDateTime setDT=aSetTime.DateTime();
	TDateTime currDT=aCurrentTime.DateTime();
	setDT.SetYear(currDT.Year());
	setDT.SetDay(0);	// ensure SetMonth cannot fail.
	setDT.SetMonth(currDT.Month());
	setDT.SetDay(currDT.Day());
	aSetTime=setDT;
	TUint workdays=TLocale().WorkDays();
	TheAlarmTest(workdays!=0, __LINE__);
	TUint dayFlag;


#if defined(_DEBUG)
		{
		FOREVER
			{
			dayFlag=DoWFlag(aSetTime.DayNoInWeek());
			if	(workdays&dayFlag && aSetTime>aCurrentTime)
				return;
			aSetTime+=TTimeIntervalDays(1);
			}
		}
#else
	if (AlarmSetInThePastTest)
		{
		//When moving forwards the system time the Next alarm time will be in the past
		FOREVER
			{
			dayFlag=DoWFlag(aSetTime.DayNoInWeek());
			if(workdays&dayFlag )
				return;
			aSetTime+=TTimeIntervalDays(1);
			}
		}
	else
		{
		FOREVER
			{
			dayFlag=DoWFlag(aSetTime.DayNoInWeek());
			if(workdays&dayFlag && aSetTime>aCurrentTime)
				return;
			aSetTime+=TTimeIntervalDays(1);
			}
		}
#endif
	}

static void TestCompareTimeL(const TTime& aExpectedTime, const TTime& aAlarmTime, TInt aLineNum)
//
//  Compare times to required degree of precision
//
	{
	// determine the difference, in seconds
	TTimeIntervalSeconds timeDiffSeconds;
	const TInt error = aExpectedTime.SecondsFrom(aAlarmTime, timeDiffSeconds);

	TheAlarmTest(error == KErrNone, __LINE__);

	// check the times are within 1min
	// as alarms are rounded to the minute
	const TTimeIntervalSeconds maxDiff(60);
	const TTimeIntervalSeconds minDiff(-60);
	if ((timeDiffSeconds > maxDiff) || (timeDiffSeconds < minDiff))
		{
		TheAlarmTest.Test().Printf(
			_L("Compared times more than 1 minute different (%is) at line %u"),
			timeDiffSeconds.Int(),
			aLineNum);
		TRAP_IGNORE(PrintTimeL(aExpectedTime, _L("Expected time: ")));
		TRAP_IGNORE(PrintTimeL(aAlarmTime, _L("Alarm time: ")));
		TheAlarmTest(EFalse, __LINE__);
		}
	}

static void TestClockAlarmsL(const RArray<TAlarmId>& aIds)
//
//	We expect and TheAlarmTest.TheTest that:
//
//	1) Once alarm to remain unchanged at initial alarm time unless time in past
//	2) Next24 to advance to the first 'initial alarm time' falling due after the original time
//	3) Daily to advance to the first 'initial alarm time' falling due after the currrent time
//	4) Weekday to advance to the 'initial alarm time' on the next weekday
//	5) Weekly to advance to the 'initial alarm time' in the next week
//
	{
	TASShdAlarm alarm;
	TheCurrentTime.HomeTime();
	//
	for (TInt i=0; i<aIds.Count(); i++)
		{
		const TAlarmId id = aIds[i];
		//
		TInt r = TheAlarmTest.Session().GetAlarmDetails(id, alarm);
		if	(r != KErrNone)
			{
			// Its acceptable for the first item to be missing
			// due to a date/time change. The first item is a repeat-once
			// alarm, scheduled 1 hour in the past (i.e one hour before the
			// current time). As soon as the date/time changes, this alarm
			// will be removed from the alarm queue because it is before
			// the new date time.
			TheAlarmTest(r == KErrNotFound && i==0, __LINE__);
			}
		else
			TheAlarmTest(r == KErrNone, __LINE__);
		//
		if (alarm.Status() != EAlarmStatusEnabled)
			continue;
		//
		TTime setTime(TheInitialAlarmTime);
		switch(alarm.RepeatDefinition())
			{
		case EAlarmRepeatDefintionRepeatOnce:
			{
			TestCompareTimeL(TheInitialAlarmTime, alarm.NextDueTime(), __LINE__);
			break;
			}

		case EAlarmRepeatDefintionRepeatNext24Hours:
			{
			// Should be be within 24 hours of the original time
			TTimeIntervalDays daysBetweenInitialAndOriginal(TheOriginalTime.DaysFrom(TheInitialAlarmTime));
			setTime+=daysBetweenInitialAndOriginal;
			setTime+=TTimeIntervalHours(24);
			TestCompareTimeL(setTime, alarm.NextDueTime(), __LINE__);
			}
			break;

		case EAlarmRepeatDefintionRepeatDaily:
			{
			// Should be within next 24 hours of current time at initial time
			TTimeIntervalDays daysBetweenInitialAndCurrent(TheCurrentTime.DaysFrom(TheInitialAlarmTime));
			setTime+=daysBetweenInitialAndCurrent;

#if!defined(_DEBUG)
			// UREL
			if (!(alarm.NextDueTime()<TheCurrentTime))  // Alarm not in the past
#endif
			setTime+=TTimeIntervalHours(24);
			TestCompareTimeL(setTime, alarm.NextDueTime(), __LINE__);
			}
			break;

		case EAlarmRepeatDefintionRepeatWeekly:
			{
			TTimeIntervalDays daysBetweenInitialAndCurrent(TheCurrentTime.DaysFrom(TheInitialAlarmTime));
			TInt weeks=daysBetweenInitialAndCurrent.Int()/7;
			if (weeks!=0)
				setTime+=TTimeIntervalDays(weeks*7);
			setTime+=TTimeIntervalDays(7);
			TestCompareTimeL(setTime, alarm.NextDueTime(), __LINE__);
			}
			break;

		case EAlarmRepeatDefintionRepeatWorkday:
			{
			AdjustToNextWorkDay(setTime,TheCurrentTime);
			TestCompareTimeL(setTime, alarm.NextDueTime(), __LINE__);
			}
			break;

		default:
			TheAlarmTest(0, __LINE__);
			break;
			}
		}
	}

static void TestClockUtcAlarmsL(const RArray<TAlarmId>& aIds)
//
//	We expect and TheAlarmTest.TheTest that:
//
//	1) Once alarm to remain unchanged at initial alarm time unless time in past
//	2) Next24 to advance to the first 'initial alarm time' falling due after the original time
//	3) Daily to advance to the first 'initial alarm time' falling due after the currrent time
//	4) Weekday to advance to the 'initial alarm time' on the next weekday
//	5) Weekly to advance to the 'initial alarm time' in the next week
//
	{
	TASShdAlarm alarm;
	TheCurrentTime.HomeTime();
	//
	for (TInt i=0; i<aIds.Count(); i++)
		{
		const TAlarmId id = aIds[i];
		//
		TInt r = TheAlarmTest.Session().GetAlarmDetails(id, alarm);
		if	(r != KErrNone)
			{
			// Its acceptable for the first item to be missing
			// due to a date/time change. The first item is a repeat-once
			// alarm, scheduled 1 hour in the past (i.e one hour before the
			// current time). As soon as the date/time changes, this alarm
			// will be removed from the alarm queue because it is before
			// the new date time.
			TheAlarmTest(r == KErrNotFound && i==0, __LINE__);
			}
		else
			TheAlarmTest(r == KErrNone, __LINE__);
		//
		if	(alarm.Status() != EAlarmStatusEnabled)
			continue;
		//
		TTime setTime(TheInitialAlarmTime);
		switch(alarm.RepeatDefinition())
			{
		case EAlarmRepeatDefintionRepeatOnce:
			{
			TestCompareTimeL(TheInitialAlarmTime, (alarm.NextDueTime() + User::UTCOffset()), __LINE__);
			break;
			}

		case EAlarmRepeatDefintionRepeatNext24Hours:
			{
			// Should be be within 24 hours of the original time
			TTimeIntervalDays daysBetweenInitialAndOriginal(TheOriginalTime.DaysFrom(TheInitialAlarmTime));
			setTime+=daysBetweenInitialAndOriginal;
			setTime+=TTimeIntervalHours(24);
			TestCompareTimeL(setTime, (alarm.NextDueTime() + User::UTCOffset()), __LINE__);
			}
			break;

		case EAlarmRepeatDefintionRepeatDaily:
			{
			// Should be within next 24 hours of current time at initial time
			TTimeIntervalDays daysBetweenInitialAndCurrent(TheCurrentTime.DaysFrom(TheInitialAlarmTime));
			setTime+=daysBetweenInitialAndCurrent;

#if!defined(_DEBUG)
			// UREL
			if (!((alarm.NextDueTime() + User::UTCOffset()) < TheCurrentTime ))  // Alarm not in the past
#endif
			setTime+=TTimeIntervalHours(24);
			TestCompareTimeL(setTime, (alarm.NextDueTime() + User::UTCOffset()), __LINE__);
			}
			break;

		case EAlarmRepeatDefintionRepeatWeekly:
			{
			TTimeIntervalDays daysBetweenInitialAndCurrent(TheCurrentTime.DaysFrom(TheInitialAlarmTime));
			TInt weeks=daysBetweenInitialAndCurrent.Int()/7;
			if (weeks!=0)
				setTime+=TTimeIntervalDays(weeks*7);
			setTime+=TTimeIntervalDays(7);
			TestCompareTimeL(setTime, (alarm.NextDueTime() + User::UTCOffset()), __LINE__);
			}
			break;

		case EAlarmRepeatDefintionRepeatWorkday:
			{
			AdjustToNextWorkDay(setTime,TheCurrentTime);
			TestCompareTimeL(setTime, (alarm.NextDueTime() + User::UTCOffset()), __LINE__);
			}
			break;

		default:
			TheAlarmTest(0, __LINE__);
			break;
			}
		}
	}


static void TestAdvanceTimeToBeforeNextAlarmsL()
//
//	Advance time by 22 hours, just before next alarms.
//
	{
	TTime newTime=TheOriginalTime;
	newTime+=TTimeIntervalHours(22);

    RPIMTestServer serv;
    User::LeaveIfError(serv.Connect());
    serv.SetHomeTime(newTime);
    serv.Close();

	User::After(KTimeToWait);
	}

static void TestAdvanceTimeAfterAlarmsL()
//
//	Advance time by a further 24 hours past daily, next24, and weekday
//	alarms if tomorrow is a weekday.
//
	{
	TTime newTime=TheOriginalTime;
	newTime+=TTimeIntervalHours(24);

    RPIMTestServer serv;
    User::LeaveIfError(serv.Connect());
    serv.SetHomeTime(newTime);
    serv.Close();

	User::After(KTimeToWait);
	}

static void TestSetClockAlarmsL(TInt aAlarmOffsetHours,TInt aAlarmOffsetDays)
//
//	Set five alarms with each having a different alarm repeat mode.
//	Each is set with the same past time an hour ago.
//
	{
			
	TheAlarmTest.Test().Next(_L("Clock alarms are set properly"));
	TInt n=TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n==0, __LINE__);

	TheCurrentTime=TheOriginalTime;
	TheInitialAlarmTime=TheOriginalTime;

	if (aAlarmOffsetHours!=0)
		TheInitialAlarmTime+=TTimeIntervalHours(aAlarmOffsetHours);
	if (aAlarmOffsetDays!=0)
		TheInitialAlarmTime+=TTimeIntervalDays(aAlarmOffsetDays);
	//
	TASShdAlarm alarm;
	RArray<TInt> array;
	//
	for (TInt i=EAlarmRepeatDefintionRepeatOnce; i<=EAlarmRepeatDefintionRepeatWeekly; i++)
		{
		alarm.Reset();
		alarm.Message().Num(i);
		alarm.NextDueTime() = TheInitialAlarmTime;
		alarm.RepeatDefinition() = TAlarmRepeatDefinition(i);
		const TInt error = TheAlarmTest.Session().AlarmAdd(alarm);
		TheAlarmTest(error == KErrNone, __LINE__);
		array.AppendL(alarm.Id());
		}

	// Check that the alarms were all queued okay
	TestClockAlarmsL(array);

	
	TheAlarmTest.Test().Next(_L("Time adjusted to before repeats +22hr"));
	//	We expect and test that:
	//
	//	1) Once alarm to remain unchanged at initial time
	//	2) Next24 to remain at 24 hours from initial time
	//	3) Daily to remain at 24 hours from initial time
	//	4) Weekday to remain at next weekday from initial time
	//	5) Weekly to remain at 7 days from initial time
	//
	TestAdvanceTimeToBeforeNextAlarmsL();
	TestClockAlarmsL(array);

	

	TheAlarmTest.Test().Next(_L("Time adjusted to after repeats +24hr"));
	//	We expect and test that;
	//
	//	1) Once alarm to remain unchanged at initial time
	//	2) Next24 to remain at 24 hours from initial time
	//	3) Daily to advance to 48 hours in udeb and 24 hours in urel from initial time.
	//	4) Weekday to advance two second weekday after
	//	   initial time if day after initial time was a weekday
	//	5) Weekly to remain at 7 days from initial time
	//
	AlarmSetInThePastTest = ETrue;
	TestAdvanceTimeAfterAlarmsL();
	TestClockAlarmsL(array);
	AlarmSetInThePastTest = EFalse;

	// Tidy up and finish
	// Delete the alarm currently tested
	while(array.Count())
		{
		TheAlarmTest(array[array.Count()-1]);
		const TInt error = TheAlarmTest.Session().AlarmDelete(array[array.Count()-1]);
		TheAlarmTest(error == KErrNone || error == KErrNotFound);
		array.Remove(array.Count()-1);
		}
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue() == 0);
	TheAlarmTest.TestClearStoreL();
	array.Close();
	}

	
static void testNext24hours()
//
//	Test that an alarm set for 1 second ago gets set for tomorrow
//
	{
	TheAlarmTest.Test().Next(_L("Next 24 hours"));
	TInt n=TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n==0, __LINE__);

	TASShdAlarm alarm;
	TTime timeNow;
	timeNow.HomeTime();
	alarm.NextDueTime() = timeNow-TTimeIntervalSeconds(1);
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatNext24Hours;
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	TheAlarmTest(alarm.State() == EAlarmStateQueued, __LINE__);
	TheAlarmTest(alarm.Status() == EAlarmStatusEnabled, __LINE__);
	//
	TASShdAlarm alarm2;
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(alarm.Id(), alarm2) == KErrNone, __LINE__);
	TheAlarmTest(alarm2.NextDueTime() == alarm.NextDueTime(), __LINE__);
	TheAlarmTest(alarm2.NextDueTime() > timeNow, __LINE__);
	TInt error = TheAlarmTest.Session().AlarmDelete(alarm.Id());
	TheAlarmTest(error == KErrNone, __LINE__);
	}

static void testSetClockUtcAlarmsL(TInt aAlarmOffsetHours,TInt aAlarmOffsetDays)
//
//	Set five alarms with each having a different alarm repeat mode.
//	Each is set with the same past time an hour ago.
//
	{
			
	TheAlarmTest.Test().Next(_L("Clock UTC alarms are set properly"));
	TInt n=TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n==0, __LINE__);

	TheCurrentTime=TheOriginalTime;
	TheInitialAlarmTime=TheOriginalTime;

	if (aAlarmOffsetHours!=0)
		TheInitialAlarmTime+=TTimeIntervalHours(aAlarmOffsetHours);
	if (aAlarmOffsetDays!=0)
		TheInitialAlarmTime+=TTimeIntervalDays(aAlarmOffsetDays);
	//
	TASShdAlarm alarm;
	RArray<TInt> array;
	//
	for (TInt i=EAlarmRepeatDefintionRepeatOnce; i<=EAlarmRepeatDefintionRepeatWeekly; i++)
		{
		alarm.Reset();
		TAlarmMessage m = alarm.Message();
		m.Num(i);
		alarm.Message()=m;
		alarm.SetUtcNextDueTime(TheInitialAlarmTime - User::UTCOffset());
		alarm.RepeatDefinition()=TAlarmRepeatDefinition(i);
		const TInt error = TheAlarmTest.Session().AlarmAdd(alarm);
		TheAlarmTest(error == KErrNone, __LINE__);
		array.AppendL(alarm.Id());
		}

	// Check that the alarms were all queued okay
	TestClockUtcAlarmsL(array);

		
	TheAlarmTest.Test().Next(_L("Time adjusted to before repeats +22hr"));
	//	We expect and test that:
	//
	//	1) Once alarm to remain unchanged at initial time
	//	2) Next24 to remain at 24 hours from initial time
	//	3) Daily to remain at 24 hours from initial time
	//	4) Weekday to remain at next weekday from initial time
	//	5) Weekly to remain at 7 days from initial time
	//
	TestAdvanceTimeToBeforeNextAlarmsL();
	TestClockUtcAlarmsL(array);

	
	TheAlarmTest.Test().Next(_L("Time adjusted to after repeats +24hr"));
	//	We expect and test that;
	//
	//	1) Once alarm to remain unchanged at initial time
	//	2) Next24 to remain at 24 hours from initial time
	//	3) Daily to advance to 48 hours in udeb and 24 hours in urel from initial time.
	//	4) Weekday to advance two second weekday after
	//	   initial time if day after initial time was a weekday
	//	5) Weekly to remain at 7 days from initial time
	//
	AlarmSetInThePastTest = ETrue;
	TestAdvanceTimeAfterAlarmsL();
	TestClockUtcAlarmsL(array);
	AlarmSetInThePastTest = EFalse;

	// Tidy up and finish
	// Delete the alarm currently tested
	while(array.Count())
		{
		TheAlarmTest(array[array.Count()-1]);
		const TInt error = TheAlarmTest.Session().AlarmDelete(array[array.Count()-1]);
		TheAlarmTest(error == KErrNone || error == KErrNotFound);
		array.Remove(array.Count()-1);
		}
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue() == 0);
	TheAlarmTest.TestClearStoreL();
	array.Close();
	}

	
static void testNext24hoursUtc()
//
//	Test that an alarm set for 1 second ago gets set for tomorrow
//
	{
	TheAlarmTest.Test().Next(_L("Next 24 hours UTC"));
	TInt n=TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n==0, __LINE__);

	TASShdAlarm alarm;
	TTime timeNow;
	timeNow.UniversalTime();
	alarm.SetUtcNextDueTime(timeNow-TTimeIntervalSeconds(1));
	alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatNext24Hours;
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	TheAlarmTest(alarm.State() == EAlarmStateQueued, __LINE__);
	TheAlarmTest(alarm.Status() == EAlarmStatusEnabled, __LINE__);
	//
	TASShdAlarm alarm2;
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(alarm.Id(), alarm2) == KErrNone, __LINE__);
	TheAlarmTest(alarm2.NextDueTime() == alarm.NextDueTime(), __LINE__);
	TheAlarmTest(alarm2.NextDueTime() > timeNow, __LINE__);
	TInt error = TheAlarmTest.Session().AlarmDelete(alarm.Id());
	TheAlarmTest(error == KErrNone, __LINE__);
	}

	
static void testNextWorkdayAlarmTimePositiveOffsetL()
//
//  Test the next workday alarm is calculated correctly for
//  when the UTC and Local times of the alarms are on different days.
//  for a positive UTC offset
//
	{
	TheAlarmTest.Test().Next(_L("Next due workday alarm when UTc and local current time are on different days (with positive UTC offset)"));

	RTz tz;
	CleanupClosePushL(tz);
	User::LeaveIfError(tz.Connect());

	// Store the current time zone
	CTzId* storedTzId = tz.GetTimeZoneIdL();
	CleanupStack::PushL(storedTzId);

	// Set the current local time to be 10:58am
	// in a time zone that is GMT+11
	TTime newSystemTimeUTC(TDateTime(2007, EFebruary, 16, 23, 58, 0, 0));
	const TInt KNumberOfMinutesInElevenHours(660);
	tz.SetUnknownZoneTimeL(newSystemTimeUTC, KNumberOfMinutesInElevenHours);

	// Add a workday alarm for 05:00am local time
	const TTime KAlarmTimeLocal(TDateTime(2000, EJanuary, 0, 5, 0, 0, 0));
	TASShdAlarm alarm;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatWorkday;
	alarm.NextDueTime() = KAlarmTimeLocal;
	User::LeaveIfError(TheAlarmTest.Session().AlarmAdd(alarm));

	// Test that the next due alarm is on Monday
	const TTime KNextDueTimeLocal = alarm.NextDueTime();
	const TTime KExpectedNextDueTimeLocal(TDateTime(2007, EFebruary, 18, 5, 0, 0, 0));
	TheAlarmTest(KNextDueTimeLocal == KExpectedNextDueTimeLocal, __LINE__);

	// Set the system local time to be just after 11:00am
	// This means that UTC and local system times are on the same day.
	newSystemTimeUTC += TTimeIntervalMinutes(3);
	tz.SetUnknownZoneTimeL(newSystemTimeUTC, KNumberOfMinutesInElevenHours);

	// Test that the next due alarm is still on Monday
	User::LeaveIfError(TheAlarmTest.Session().GetAlarmDetails(alarm.Id(), alarm));
	const TTime KNextDueTimeLocal2 = alarm.NextDueTime();
	TheAlarmTest(KNextDueTimeLocal == KNextDueTimeLocal2, __LINE__);

	User::LeaveIfError(TheAlarmTest.Session().AlarmDelete(alarm.Id()));

	// Restore the stored time zone
	tz.SetTimeZoneL(*storedTzId);

	CleanupStack::PopAndDestroy(storedTzId);
	CleanupStack::PopAndDestroy(&tz);
	}


static void testNextWorkdayAlarmTimeNegativeOffsetL()
//
//  Test the next workday alarm is calculated correctly for
//  when the UTC and Local times of the alarms are on different days.
//  for a negative UTC offset
//
	{
	TheAlarmTest.Test().Next(_L("Next due workday alarm when UTc and local current time are on different days (with negative UTC offset)"));

	RTz tz;
	CleanupClosePushL(tz);
	User::LeaveIfError(tz.Connect());

	// Store the current time zone
	CTzId* storedTzId = tz.GetTimeZoneIdL();
	CleanupStack::PushL(storedTzId);

	// Set the current local time to be 01:02pm
	// in a time zone that is GMT-11
	TTime newSystemTimeUTC(TDateTime(2007, EFebruary, 18, 0, 02, 0, 0));
	const TInt KNumberOfMinutesInNegativeElevenHours(-660);
	tz.SetUnknownZoneTimeL(newSystemTimeUTC, KNumberOfMinutesInNegativeElevenHours);

	// Add a workday alarm for 06:00pm local time
	const TTime KAlarmTimeLocal(TDateTime(2000, EJanuary, 0, 18, 0, 0, 0));
	TASShdAlarm alarm;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatWorkday;
	alarm.NextDueTime() = KAlarmTimeLocal;
	User::LeaveIfError(TheAlarmTest.Session().AlarmAdd(alarm));
	// Test that the next due alarm is on Monday
	const TTime KNextDueTimeLocal = alarm.NextDueTime();
	const TTime KExpectedNextDueTimeLocal(TDateTime(2007, EFebruary, 18, 18, 0, 0, 0));
	TheAlarmTest(KNextDueTimeLocal == KExpectedNextDueTimeLocal, __LINE__);

	// Set the system local time to be just before 01:00pm
	// This means that UTC and local system times are on the same day.
	newSystemTimeUTC -= TTimeIntervalMinutes(4);
	tz.SetUnknownZoneTimeL(newSystemTimeUTC, KNumberOfMinutesInNegativeElevenHours);

	// Test that the next due alarm is still on Monday
	User::LeaveIfError(TheAlarmTest.Session().GetAlarmDetails(alarm.Id(), alarm));
	const TTime KNextDueTimeLocal2 = alarm.NextDueTime();
	TheAlarmTest(KNextDueTimeLocal == KNextDueTimeLocal2, __LINE__);

	User::LeaveIfError(TheAlarmTest.Session().AlarmDelete(alarm.Id()));

	// Restore the stored time zone
	tz.SetTimeZoneL(*storedTzId);

	CleanupStack::PopAndDestroy(storedTzId);
	CleanupStack::PopAndDestroy(&tz);
	}

/**
Create an alarm that repeats on given days of the week but 
don’t select any days. Try to update an alarm that repeats
on given days of the week so that no days are selected.

@SYMTestCaseID	PIM-APPSRV-ALARMSERVER-WEEKLYGIVENDAYS-0003
					
@SYMTestCaseDesc	To verify that it is not possible to 
					create a repeating alarm that doesn’t
					expire on any days of the week.
					
					To verify that it is not possible to
					update a repeating alarm so that it 
					doesn’t expire on any days of the week.    

@SYMTestActions
For PIM-APPSRV-ALARMSERVER-WEEKLYGIVENDAYS-0003:    
1.	Create an alarm and call SetAlarmDays with 0 as argument
 	(this means no days are selected).
2. 	Try to add this alarm using RAsCliSession::AddAlarm. [1]

For PIM-APPSRV-ALARMSERVER-WEEKLYGIVENDAYS-0004:
1. Create an alarm that repeats on Saturday and Sunday.
2. Try to update this alarm so that no days are selected. [2]

@SYMTestExpectedResults [1] The AddAlarm function returns KErrArgument.
						[2] The update function returns KErrArgument.

@SYMTestType                CT
@SYMTestPriority            1
*/
void TestDailyOnGivenDaysL()
	{
#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
	TheAlarmTest.Test().Next(_L("@SYMTestCaseID:PIM-APPSRV-ALARMSERVER-WEEKLYGIVENDAYS-0003  Test 'Daily On Given Days' Repeat Definition"));

	// Precondition - there must be no active alarms in the alarm queue.
	TInt numActiveAlarms = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(numActiveAlarms == 0, __LINE__);

	TASShdAlarm alarm;
	
	// Verify that attempting to set the active alarm days to no days returns
	// the appropriate error code.
	TheAlarmTest(alarm.SetAlarmDays(0) == KErrArgument, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrArgument, __LINE__);

	// Verify that attempting to set the active alarm days to an invalid day
	// returns the appropriate error code.
	TheAlarmTest(alarm.SetAlarmDays(EAlarmDaySunday << 1) == KErrArgument, __LINE__);

	// Verify that active days of the week can be set individually.
	TheAlarmTest(alarm.SetAlarmDays(EAlarmDayMonday) == KErrNone, __LINE__);
	TheAlarmTest(alarm.AlarmDays() == EAlarmDayMonday, __LINE__);
	
	// Verify that active days of the week can be combined.
	TheAlarmTest(alarm.SetAlarmDays(EAlarmDaySaturday | EAlarmDaySunday) == KErrNone, __LINE__);
	TheAlarmTest(alarm.AlarmDays() == EAlarmDaySaturday | EAlarmDaySunday, __LINE__);
	
	// Set alarm to expire 1s from now.
	TTime timeNow;
	timeNow.HomeTime();
	alarm.NextDueTime() = timeNow - TTimeIntervalSeconds(1);
		
	// Set alarm repeat defintion.
	alarm.RepeatDefinition() = EAlarmRepeatDefinitionRepeatDailyOnGivenDays;

	// Add alarm to the Alarm Server.
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
		
	// For an existing alarm, verify that attempting to update the active alarm
	// days to no days returns the appropriate error code.
	TheAlarmTest(TheAlarmTest.Session().SetAlarmDays(alarm.Id(), 0) == KErrArgument, __LINE__);

	// For an existing alarm, verify that active days of the week can be set.
	TheAlarmTest(TheAlarmTest.Session().SetAlarmDays(alarm.Id(), EAlarmDayMonday) == KErrNone, __LINE__);
	TUint8 alarmDays;
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDays(alarm.Id(), alarmDays) == KErrNone, __LINE__);
	TheAlarmTest(alarmDays == EAlarmDayMonday, __LINE__);
		
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	TheAlarmTest(alarm.State() == EAlarmStateQueued, __LINE__);
	TheAlarmTest(alarm.Status() == EAlarmStatusEnabled, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().AlarmDelete(alarm.Id()) == KErrNone, __LINE__);
#endif
	}

static void DoTestsL()
	{
	TTime t;
	t.HomeTime();
	PrintTimeL(t,_L("Starting tests at: "));

	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	TheAlarmTest.TestStartServers();
	TheAlarmTest(TheAlarmTest.Session().Connect()==KErrNone);
	TheAlarmTest.TestClearStoreL();

	TestDailyOnGivenDaysL();
	
	//
	testNext24hours();
	//
	TUint workdays=DoWFlag(EMonday)|DoWFlag(ETuesday)|DoWFlag(EWednesday)|DoWFlag(EThursday)|DoWFlag(EFriday);
	TLocale locale;
	locale.SetWorkDays(workdays);
	locale.Set();
	User::After(KTimeToWait);

#if defined(_DEBUG)
	TheAlarmTest.Session().__DbgPreventUserNotify(ETrue);
#endif
	//
	TheOriginalTime.HomeTime();
	
	
	TheAlarmTest.Test().Next(_L("Alarm 1 hour before current time"));
	TestSetClockAlarmsL(-1,  0);
	//
    RPIMTestServer serv;
    User::LeaveIfError(serv.Connect());
    serv.SetHomeTime(TheOriginalTime);

	TheAlarmTest.Test().Next(_L("Alarm 1 hour, 1 day before current time"));
	TestSetClockAlarmsL(-1, -1);
	//
    serv.SetHomeTime(TheOriginalTime);
    serv.Close();
	
	TheAlarmTest.Test().Next(_L("Alarm 1 hour, 8 days before current time"));
	TestSetClockAlarmsL(-1, -8);
	//
	TheAlarmTest.TestClearStoreL();

	//
	testNext24hoursUtc();
	//

	workdays=DoWFlag(EMonday)|DoWFlag(ETuesday)|DoWFlag(EWednesday)|DoWFlag(EThursday)|DoWFlag(EFriday);
	locale.Refresh();
	locale.SetWorkDays(workdays);
	locale.Set();
	User::After(KTimeToWait);

#if defined(_DEBUG)
	TheAlarmTest.Session().__DbgPreventUserNotify(ETrue);
#endif
	//
	TheOriginalTime.HomeTime();
	
	TheAlarmTest.Test().Next(_L("UTC Alarm 1 hour before current time"));
	testSetClockUtcAlarmsL(-1,  0);
	//
    RPIMTestServer serv2;
    User::LeaveIfError(serv2.Connect());
    serv2.SetHomeTime(TheOriginalTime);
	
	TheAlarmTest.Test().Next(_L("UTC Alarm 1 hour, 1 day before current time"));
	testSetClockUtcAlarmsL(-1, -1);
	//
    serv2.SetHomeTime(TheOriginalTime);
    serv2.Close();

	
	TheAlarmTest.Test().Next(_L("UTC Alarm 1 hour, 8 days before current time"));
	testSetClockUtcAlarmsL(-1, -8);
	//
	TheAlarmTest.TestClearStoreL();

	testNextWorkdayAlarmTimePositiveOffsetL();
	testNextWorkdayAlarmTimeNegativeOffsetL();

	CleanupStack::PopAndDestroy(scheduler);
	}

/**
 @SYMTestCaseID PIM-TREPEATDEFINITIONS-0001
 */
	
GLDEF_C TInt E32Main()
    {
    TInt ret = KErrNone;
	TTime t;
	t.HomeTime();
	TRAP_IGNORE(PrintTimeL(t,_L("Starting test at: ")));

	// Begin test
	TheAlarmTest.Title();
	TheAlarmTest.Start(_L("@SYMTestCaseID PIM-TREPEATDEFINITIONS-0001"));

	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	TRAP_IGNORE(DoTestsL());
	delete cleanup;
	__UHEAP_MARKEND;

	// Postcondition - there must be no active alarms in the alarm queue.
	TInt numActiveAlarms = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(numActiveAlarms == 0, __LINE__);

	TheAlarmTest.Session().Close();
	TRAP(ret,TheAlarmTest.EndL());

	TheAlarmTest.Test().Close();
	ASTstAlarmTest::Close();
    	
	return ret;
	}
