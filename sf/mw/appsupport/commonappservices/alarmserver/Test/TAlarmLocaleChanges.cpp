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
// Test effects of changing TLocale on clock alarms
// Test alarms which are skipped over by advancing the time or revived by putting back the time.
// Test for when time is advanced or put back by changes to the locale, i.e:
// Turning daylight saving on and off and
// Increasing and decreasing GMT offset.
// The expected results for advancing or putting back the time are the same whether caused by changes
// to daylight saving or GMT offset.
// Test effects of altering workdays on workday repeated alarms
// 
//

#include "ASTstAlarmTest.h"
#include "testserver.h"
#include <tz.h>

static TRequestStatus TheStatus;
static TTime TheInitialAlarmTime;
static TTime TheExtendedAlarmTime;
static TTime TheOriginalTime;
static TTime TheCurrentTime;
static TAlarmId TheClockAlarmSlots[8] = { KNullAlarmId, KNullAlarmId, KNullAlarmId, KNullAlarmId, KNullAlarmId, KNullAlarmId, KNullAlarmId, KNullAlarmId };

// Use the Time Zone Server if available.
static RTz timezone;

// This gets us a time which isn't close to a minute boundary:
// It also sets the microseconds to 0, since alarms ignore it and comparison functions don't.
static void SafeHomeTime(TTime& aTime)
	{
	TInt sec;
	do
		{
		aTime.HomeTime();
		TDateTime dt(aTime.DateTime());
		sec = dt.Second();

		if(sec > 55 || sec < 5)
			{
			// dont loop full speed, there's no point:
			User::After(1000000);
			}
		else
			{
			dt.SetMicroSecond(0);
			aTime = dt;
			}
		}
	while(sec > 55 || sec < 5);
	}

static void CheckTimesAreEqual(const TTime& aExpectedTime, const TTime& aActualTime, TInt aLineNum)
//
// Compare times to required degree of precision
//
	{
	// determine the difference, in seconds
	TTimeIntervalSeconds	timeDiffSeconds;
	const TInt error = aExpectedTime.SecondsFrom(aActualTime, timeDiffSeconds);

	TheAlarmTest(error == KErrNone, __LINE__);

	// check the times are within 1min
	// as alarms are rounded to the minute
	const TTimeIntervalSeconds	maxDiff(60);
	const TTimeIntervalSeconds	minDiff(-60);

	if ((timeDiffSeconds > maxDiff) || (timeDiffSeconds < minDiff))
		{
		TheAlarmTest.Test().Printf(
			_L("Compared times more than 1 minute different (%u) at line %u"),
			timeDiffSeconds.Int(),
			aLineNum);
		TheAlarmTest(EFalse, __LINE__);
		}
	}

/**
Sets the system time of the device to the given local (wall-clock) time.

@param aTime The time to set.
@internalComponent
*/
static void SetHomeTime(const TTime& aTime)
    {
    TInt err = timezone.SetHomeTime(aTime);
    TheAlarmTest(err == KErrNone, __LINE__);
    }



// If aDszOn is true, set to a time in BST.
// If aDszOn is false, set to a time in GMT.
static void EnableDaylightSavingL(TBool aDszOn)
	{
	// Using TLocale to set daylight saving mode no longer works, since this is controlled by
	// the Time Zone Server. Instead the time must be set such that DST rules are in force in a
	// locale where such rules apply, e.g. 01-06-2004 12:00 BST.

	// Set the time zone to a known location.
	_LIT8(KTZName, "Europe/London");
	CTzId* zoneId = CTzId::NewL(KTZName);
	CleanupStack::PushL(zoneId);
	timezone.SetTimeZoneL(*zoneId);
	CleanupStack::PopAndDestroy(zoneId);

	// Set daylight saving time on/off. Note that when DST is set 'on' a DST change event occurs
	// almost immediately - this tests that the Alarm Queue is updated correctly for DST events.

	if (aDszOn)
		{
		// Set the system time to a time in the British Summer: 31-10-2004 01:59:58 BST = 00:59:00 UTC
		const TInt KBstOffset = 3600;
		_LIT(KTime, "20040930:005958.000000");  // Just before the DST change.
		TInt err = User::SetUTCTimeAndOffset(TTime(KTime), KBstOffset);
		TheAlarmTest(err == KErrNone, __LINE__);
		}
	else
		{
		// Set the system time to a time in the British Winter: 31-10-2004 01:00:00 GMT = 01:00:00 UTC
		const TInt KGmtOffset = 0;
		_LIT(KTime, "20040930:010000.000000"); // Just after the DST change.
		TInt err = User::SetUTCTimeAndOffset(TTime(KTime), KGmtOffset);
		TheAlarmTest(err == KErrNone, __LINE__);
		}
	}


static TTime SetClockAlarms(const TTime& aBaseTime, const TTimeIntervalMinutes& aOffset)
//
// Set Alarms.
// Extra alarm when putting clocks back to see what happen to
// weekly alarm set for 6 days & 23 1/2 hours time
//
	{
	TInt r;
	TASShdAlarm alarm;
	const TInt n = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n == 0, __LINE__);
	//
	TTime timeForAlarms(aBaseTime);
	timeForAlarms += aOffset;

	// Once Only
	alarm.Reset();
	alarm.Message().Num(0);
	alarm.NextDueTime() = timeForAlarms;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheClockAlarmSlots[0] = alarm.Id();

	// Next 24 hours
	alarm.Reset();
	alarm.Message().Num(1);
	alarm.NextDueTime() = timeForAlarms;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatNext24Hours;
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheClockAlarmSlots[1] = alarm.Id();

	// Clock daily
	alarm.Reset();
	alarm.Message().Num(2);
	alarm.NextDueTime() = timeForAlarms;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatDaily;
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheClockAlarmSlots[2] = alarm.Id();

	// Clock weekly
	alarm.Reset();
	alarm.Message().Num(3);
	alarm.NextDueTime() = timeForAlarms;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatWeekly;
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheClockAlarmSlots[3] = alarm.Id();

	if (aOffset.Int() < 0)
		{
		TheExtendedAlarmTime = timeForAlarms;
		TheExtendedAlarmTime += TTimeIntervalDays(7);
		alarm.Message().Num(4);
		alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatWeekly;
		alarm.NextDueTime() = TheExtendedAlarmTime;
		r = TheAlarmTest.Session().AlarmAdd(alarm);
		TheAlarmTest(r == KErrNone, __LINE__);
		TheClockAlarmSlots[4] = alarm.Id();
		}

	return timeForAlarms;
	}

/**
Checks that the alarms are correct.

//
//	We expect and test that:
//
//	1)	Once alarm to remain unchanged at initial alarm time
//		unless allowed time to complete.
//
//	2)	Next24 to advance to the first 'initial alarm time'
//		falling due after the original time
//
//	3a)	If before clocks go back, Weekly (set previously) to
//		advance to the 'initial alarm time' in the next week
//
//	3b)	If after clocks go back, Weekly (set previously) to
//		return to the 'initial alarm time' in the current week
//
//	4a)	If before clocks go back, Weekly (set for next week)
//		to remain at the 'extended alarm time' in the next week
//
//	4b)	If after clocks go back, Weekly (set for next week) to
//		move back to 'initial alarm time' in the current week
//

@param aInterveningOffsetChange True when a change in the UTC offset occurs
between setting the alarms and testing them.
@param aOnceCompleted True if the "Repeat Once" alarm has completed.
@internalComponent
*/
static void VerifyAlarmsAreCorrect(TBool aInterveningOffsetChange, TBool aOnceCompleted)
	{
	TInt ret;
	TTime setTime;
	TASShdAlarm alarm;
	TAlarmId alarmId;
	TTime time;
	time.HomeTime();

	// Check Once repeat if it has not completed.
	if (!aOnceCompleted)
		{
		alarmId = TheClockAlarmSlots[0];
		TheAlarmTest(alarmId != KNullAlarmId, __LINE__);
		ret = TheAlarmTest.Session().GetAlarmDetails(alarmId, alarm);
		TheAlarmTest(ret == KErrNone, __LINE__);
		CheckTimesAreEqual(TheInitialAlarmTime, alarm.NextDueTime(), __LINE__);
		}

	// Check Next 24 hours repeat
	alarmId = TheClockAlarmSlots[1];
	TheAlarmTest(alarmId != KNullAlarmId, __LINE__);
	ret = TheAlarmTest.Session().GetAlarmDetails(alarmId, alarm);
	TheAlarmTest(ret == KErrNone, __LINE__);
	setTime = TheInitialAlarmTime;

    // The alarm was set in the past, so add repeat interval.
	setTime += TTimeIntervalHours(+24);

	CheckTimesAreEqual(setTime, alarm.NextDueTime(), __LINE__);

	// Check daily repeat
	alarmId = TheClockAlarmSlots[2];
	TheAlarmTest(alarmId != KNullAlarmId, __LINE__);
	ret = TheAlarmTest.Session().GetAlarmDetails(alarmId, alarm);
	TheAlarmTest(ret == KErrNone, __LINE__);
	setTime = TheInitialAlarmTime;

	if (aInterveningOffsetChange)
		{
		// The alarm was set in the past, so add repeat interval.
		setTime += TTimeIntervalHours(+24);
		}

	CheckTimesAreEqual(setTime, alarm.NextDueTime(), __LINE__);

	// Check weekly repeat
	alarmId = TheClockAlarmSlots[3];
	TheAlarmTest(alarmId != KNullAlarmId, __LINE__);
	ret = TheAlarmTest.Session().GetAlarmDetails(alarmId, alarm);
	TheAlarmTest(ret == KErrNone, __LINE__);
	setTime = TheInitialAlarmTime;

	if (aInterveningOffsetChange)
		{
		// The alarm was set in the past, so add repeat interval.
		setTime += TTimeIntervalDays(+7);
		}

	CheckTimesAreEqual(setTime, alarm.NextDueTime(), __LINE__);

	// Check weekly repeat
	alarmId = TheClockAlarmSlots[4];
	TheAlarmTest(alarmId != KNullAlarmId, __LINE__);
	ret = TheAlarmTest.Session().GetAlarmDetails(alarmId, alarm);
	TheAlarmTest(ret == KErrNone, __LINE__);

	if (aInterveningOffsetChange)
		{
		setTime = TheExtendedAlarmTime;
		}
	else
	    {
	    setTime = TheInitialAlarmTime;
	    }

	CheckTimesAreEqual(setTime, alarm.NextDueTime(), __LINE__);
	}

static void testAlarmsOnForwardingClocks(TBool aAfterClocksGoForward, TBool aAlarmsSetInPast, TBool aAlarmsShouldAllBeQueued = ETrue)
//
//	We expect and test that:
//	1)	Once alarm to remain unchanged at 'initial alarm time'
//	2)	Next24 to remain at the 'initial alarm time'
//	3a)	If before clocks go forward, Weekly to remain at the
//		'initial alarm time'
//	3b)	If after clocks go forward, Weekly to advance to 'initial
//		alarm time' in the next week
//
	{
	TInt ret;
	TASShdAlarm alarm;
	TAlarmId alarmId;
	TTime setTime;

	//	Check Once repeat
	alarmId = TheClockAlarmSlots[0];
	ret = TheAlarmTest.Session().GetAlarmDetails(alarmId, alarm);
	setTime = TheInitialAlarmTime;

	// Note that the time of the expected alarm should be the same as when
	// originally set. No adjustment for any UTC offset is required.
	if (aAlarmsShouldAllBeQueued)
	    {
	    TheAlarmTest(ret == KErrNone, __LINE__);
	    }

	if (ret == KErrNone && alarm.Status() == EAlarmStatusEnabled)
	    {
	    CheckTimesAreEqual(setTime, alarm.NextDueTime(), __LINE__);
	    }

	//	Check Next 24 hours repeat
	alarmId = TheClockAlarmSlots[1];
	ret = TheAlarmTest.Session().GetAlarmDetails(alarmId, alarm);
	setTime = TheInitialAlarmTime;

	if (aAlarmsShouldAllBeQueued)
	    {
	    TheAlarmTest(ret == KErrNone, __LINE__);
	    }

	if (ret == KErrNone && alarm.Status() == EAlarmStatusEnabled)
	    {
	    CheckTimesAreEqual(setTime, alarm.NextDueTime(), __LINE__);
	    }

	//	Check daily repeat
	alarmId = TheClockAlarmSlots[2];
	TheAlarmTest(alarmId != KNullAlarmId, __LINE__);
	ret = TheAlarmTest.Session().GetAlarmDetails(alarmId, alarm);
	setTime = TheInitialAlarmTime;

	if (aAlarmsShouldAllBeQueued)
	    {
	    TheAlarmTest(ret == KErrNone, __LINE__);
	    }

	CheckTimesAreEqual(setTime, alarm.NextDueTime(), __LINE__);

	//	Check weekly repeat
	alarmId = TheClockAlarmSlots[3];
	TheAlarmTest(alarmId != KNullAlarmId, __LINE__);
	ret = TheAlarmTest.Session().GetAlarmDetails(alarmId, alarm);

	if (aAlarmsShouldAllBeQueued)
	    {
	    TheAlarmTest(ret == KErrNone, __LINE__);
	    }
	//
	setTime = TheInitialAlarmTime;

#if defined(_DEBUG)
    // In udeb the alarm goes from the state EAlarmStateQueued to the state EAlarmStateNotified
    // (skipping the state ENotifying).In EAlarmStateNotified the nextTime for the alarm is
    // recalculated and the expected value (called setTime here) needs adjustment to reflect this.
    // In urel the EAlarmStateNotified state (where the NextTime for an alarm is recalculated)
    // is never reached so the adjustement in the expected value (called setTime) is not necessary .

	if (aAfterClocksGoForward && aAlarmsSetInPast)
		{
		setTime += TTimeIntervalDays(+7);
		}
#else
	//Just to get rid of warning
	(void)aAfterClocksGoForward;
	(void)aAlarmsSetInPast;
#endif

	CheckTimesAreEqual(setTime, alarm.NextDueTime(), __LINE__);
	}

static void TestAlarmsWithDaylightSavingChange1L()
//
//	Test effects of changing daylight saving on clock alarms1
//
	{
	TheAlarmTest.Test().Next(_L("Initialise with daylight saving on"));
	EnableDaylightSavingL(ETrue);
	TheCurrentTime.HomeTime();
	//
	TheAlarmTest.Test().Next(_L("Setting alarms half an hour in the past"));
	TheInitialAlarmTime = SetClockAlarms(TheCurrentTime, TTimeIntervalMinutes(-30));
	//
	TheAlarmTest.Test().Next(_L("Testing alarms when daylight saving is on"));

    // Wait for DST to end.
    User::After(3000000);
	VerifyAlarmsAreCorrect(ETrue, EFalse);

	//
	TheAlarmTest.Test().Next(_L("Turning daylight saving off"));
	EnableDaylightSavingL(EFalse);
	//
	TheAlarmTest.Test().Next(_L("Testing alarms when daylight saving is off"));

	VerifyAlarmsAreCorrect(EFalse, ETrue);

	//
	TheAlarmTest.Test().Next(_L("Removing all alarms"));
	TheAlarmTest.TestClearStoreL();
	}

static void TestAlarmsWithDaylightSavingChange2L()
//
//	Test effects of changing daylight saving on clock alarms2
//
	{
	TheAlarmTest.Test().Next(_L("Initialise with daylight saving off"));
	EnableDaylightSavingL(EFalse);
	TheCurrentTime.HomeTime();
	//
	TheAlarmTest.Test().Next(_L("Setting alarms an hour and a half in the future"));
	// Used to set 30 minutes into future, but a defect in User::SetHomeTime() requires
	// times not to be set between 31-10-2004 1.59 (BST) and 31-10-2004 1.59 (GMT).
	TheInitialAlarmTime = SetClockAlarms(TheCurrentTime, TTimeIntervalMinutes(90));
	//
	TheAlarmTest.Test().Next(_L("Testing alarms with daylight saving off"));
	testAlarmsOnForwardingClocks(EFalse, EFalse, ETrue);
	//
	TheAlarmTest.Test().Next(_L("Turning daylight saving on"));
	EnableDaylightSavingL(ETrue);
	//
	TheAlarmTest.Test().Next(_L("Testing alarms with daylight saving on"));
	testAlarmsOnForwardingClocks(ETrue, EFalse, EFalse);
	//
	TheAlarmTest.Test().Next(_L("Removing all alarms"));
	TheAlarmTest.TestClearStoreL();
	}

//
//	Test changes in workdays
//

static void testSetWithNoWorkdays()
//
//	Test what happens when setting a workday alarm with no workdays set
//
	{
	TheAlarmTest.Test().Next(_L("Setting workday alarm with no workday set"));

	// Set no workdays
	TUint workdayMask = 0;
	TLocale locale;
	locale.SetWorkDays(workdayMask);
	locale.Set();

	// Have to wait a bit here because the kernel takes some time
	// to notify client-side environment change objects and so
	// we need to allow the alarm server to do its thing...
	User::After(100000);
	//
	TInt ret = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(ret == 0, __LINE__);
	//
	TheOriginalTime.HomeTime();
	TheInitialAlarmTime = TheOriginalTime;
	TheInitialAlarmTime += TTimeIntervalHours(1);
	//
	TASShdAlarm alarm;
	alarm.Message() = _L("Clock alarm in slot 0");
	alarm.NextDueTime() = TheInitialAlarmTime;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatWorkday;
	ret = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(ret == KErrNone, __LINE__);
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	TheClockAlarmSlots[0] = alarm.Id();

	// Test alarm state, should be disabled
	TASShdAlarm alarm2;
	ret = TheAlarmTest.Session().GetAlarmDetails(alarm.Id(), alarm2);
	TheAlarmTest(ret == KErrNone, __LINE__);
	TheAlarmTest(alarm2.Status() == EAlarmStatusDisabled, __LINE__);
	}

static void testDisabledAlarmWithWorkdays()
//
//  Test before-disabled alarm when workdays set is changed.
//
	{
	TheAlarmTest.Test().Next(_L("Testing disabled alarm when workdays set is changed"));
	
	const TAlarmId id = TheClockAlarmSlots[0];
	TheAlarmTest(id != KNullAlarmId, __LINE__);

	//Set workdays on Monday
	TLocale locale;
	locale.SetWorkDays(0x1); //Monday
	locale.Set();
	User::After(100000);

	//Make sure the status of alarm being changed to enabled automatically
	TAlarmStatus alarmStatus;
	TInt r = TheAlarmTest.Session().GetAlarmStatus(id, alarmStatus);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarmStatus == EAlarmStatusEnabled, __LINE__);

	//Set the alarm manually disabled
	alarmStatus = EAlarmStatusDisabled;
	r = TheAlarmTest.Session().SetAlarmStatus(id, alarmStatus);
	TheAlarmTest(r == KErrNone, __LINE__);
	r = TheAlarmTest.Session().GetAlarmStatus(id, alarmStatus);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarmStatus == EAlarmStatusDisabled, __LINE__);

	//Change the workday to check if it is still disabled(not be enabled automatically)
	locale.SetWorkDays(0x2); //Tuesday
	locale.Set();
	User::After(100000);
	r = TheAlarmTest.Session().GetAlarmStatus(id, alarmStatus);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarmStatus == EAlarmStatusDisabled, __LINE__);

	//Set locale to be no workdays
	locale.SetWorkDays(0);
	locale.Set();
	User::After(100000);
	r = TheAlarmTest.Session().GetAlarmStatus(id, alarmStatus);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarmStatus == EAlarmStatusDisabled, __LINE__);

	//Alarm should not be enabled in no workdays
	alarmStatus = EAlarmStatusEnabled;
	r = TheAlarmTest.Session().SetAlarmStatus(id, alarmStatus);
	TheAlarmTest(r != KErrNone, __LINE__);
	r = TheAlarmTest.Session().GetAlarmStatus(id, alarmStatus);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarmStatus == EAlarmStatusDisabled, __LINE__);

	//Change the workday to check if it is still disabled(not be enabled automatically)
	locale.SetWorkDays(0x2); //Tuesday
	locale.Set();
	User::After(100000);
	r = TheAlarmTest.Session().GetAlarmStatus(id, alarmStatus);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarmStatus == EAlarmStatusDisabled, __LINE__);

	//Alarm should not be enabled in no workdays
	alarmStatus = EAlarmStatusEnabled;
	r = TheAlarmTest.Session().SetAlarmStatus(id, alarmStatus);
	TheAlarmTest(r == KErrNone, __LINE__);
	r = TheAlarmTest.Session().GetAlarmStatus(id, alarmStatus);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarmStatus == EAlarmStatusEnabled, __LINE__);
	}

static void testSetWorkdays()
//
//	Create workdays and test
//
	{
	TheAlarmTest.Test().Next(_L("Creating and testing workdays"));
	//
	TInt todayMaskOffset = TheInitialAlarmTime.DayNoInWeek();
	TUint workdayMask = 1;
	//
	for (TInt count = 0; count < 7; count++)
		{
		workdayMask = 1;
		const TInt dayOfWeek = (todayMaskOffset + count) % 7;
		workdayMask <<= dayOfWeek;
		//
		TLocale locale;
		locale.SetWorkDays(workdayMask);
		locale.Set();
		User::After(100000);
		//
		const TAlarmId id = TheClockAlarmSlots[0];
		TheAlarmTest(id != KNullAlarmId, __LINE__);
		//
		TASShdAlarm alarm;
		const TInt r = TheAlarmTest.Session().GetAlarmDetails(id, alarm);
		TheAlarmTest(r == KErrNone, __LINE__);
		TheAlarmTest(alarm.Status() == EAlarmStatusEnabled, __LINE__);
		//
		TTime expectedTime = TheInitialAlarmTime;
		expectedTime += TTimeIntervalDays(count);
		CheckTimesAreEqual(expectedTime, alarm.NextDueTime(), __LINE__);
		}
	}

static void testMakeAllWorkdaysL()
//
//	Make all days workdays
//	Advance time a day at a time to test alarm for each day of the week
//
	{
	TheAlarmTest.Test().Next(_L("Make all days workdays"));
	//
	TBuf<60> dateTimeStr;
	_LIT(KTimeDateFormat,"%*E%*D%X%*N%*Y %1 %2 '%3 - %-B%:0%J%:1%T%:2%S%:3%+B");

	const TUint workdayMask = 127;// Seven 1's
	TLocale locale;
	locale.SetWorkDays(workdayMask);
	locale.Set();
	User::After(100000);

	for (TInt count = 0; count < 7; count++)
		{
		if (count)
			{
			// Advance time
			TTime advTime(TheOriginalTime + TTimeIntervalDays(count));
            User::SetUTCTime(advTime);
			advTime.FormatL(dateTimeStr, KTimeDateFormat);
			TheAlarmTest.Test().Printf(_L("Advance time : %S\n"), &dateTimeStr);
			}

		const TAlarmId id = TheClockAlarmSlots[0];
		TheAlarmTest(id != KNullAlarmId, __LINE__);
		//
		TASShdAlarm alarm;
		const TInt r = TheAlarmTest.Session().GetAlarmDetails(id, alarm);
		TheAlarmTest(r == KErrNone, __LINE__);
		TheAlarmTest(alarm.Status() == EAlarmStatusEnabled, __LINE__);
		//
		TTime expectedTime = TheInitialAlarmTime;
		TheInitialAlarmTime.FormatL(dateTimeStr,KTimeDateFormat);
		TheAlarmTest.Test().Printf(_L("TheInitialAlarmTime: %S\n"), &dateTimeStr);

		expectedTime += TTimeIntervalDays(count);
		//
		const TInt days = expectedTime.DaysFrom(alarm.NextDueTime()).Int();

		if (days != 0)  //In that case, the next due time is within 24 hours in the past
		    {
		    expectedTime -= TTimeIntervalDays(1);
		    }
		//
		expectedTime.FormatL(dateTimeStr,KTimeDateFormat);
		TheAlarmTest.Test().Printf(_L("expectedTime : %S\n"), &dateTimeStr);

		alarm.NextDueTime().FormatL(dateTimeStr,KTimeDateFormat);
		TheAlarmTest.Test().Printf(_L("NextDueTime(): %S\n"), &dateTimeStr);

		CheckTimesAreEqual(expectedTime, alarm.NextDueTime(), __LINE__);
		}
	}

static void testRemoveWorkdays()
//
//	Remove all workdays and test
//
	{
	const TAlarmId id = TheClockAlarmSlots[0];
	TheAlarmTest(id != KNullAlarmId, __LINE__);
	//
	TLocale locale;
	locale.SetWorkDays(0);
	locale.Set();
	User::After(100000);
	//
	TASShdAlarm alarm;
	const TInt r = TheAlarmTest.Session().GetAlarmDetails(id, alarm);
	//
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarm.Status() == EAlarmStatusDisabled, __LINE__);
	}

static void testLocaleChangeWorkdaysL()
//
//	Test effects of changing workdays on alarms
//
	{
	TheAlarmTest.TestClearStoreL();
	testSetWithNoWorkdays();
	testDisabledAlarmWithWorkdays();
	testSetWorkdays();
	testMakeAllWorkdaysL();
	testRemoveWorkdays();
	TheAlarmTest.TestClearStoreL();
	}

static void test24HourToOnceOnlyAlarms()
	{
	TheAlarmTest.Test().Next(_L("Test update of 24 hour alarms to once only alarms"));

	// Next 24 hour alarms which are more than 1 day in the future after a system time
	// update should be updated to once only alarms

	// Set 'next 24 hour' alarms due in 5, just under and over 18, and 21 hours time
	TTime now;
	now.HomeTime();
	TTime alarmTime(now);

	TASShdAlarm alarm;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatNext24Hours;

	alarmTime += TTimeIntervalHours(5);
	alarm.NextDueTime() = alarmTime;
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
	TAlarmId id1 = alarm.Id();
	TheAlarmTest(id1 != KNullAlarmId, __LINE__);

	alarmTime += TTimeIntervalHours(13);
	alarmTime -= TTimeIntervalMinutes(1);
	alarm.NextDueTime() = alarmTime;
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
	TAlarmId id2 = alarm.Id();
	TheAlarmTest(id2 != KNullAlarmId, __LINE__);

	alarmTime += TTimeIntervalMinutes(2);
	alarm.NextDueTime() = alarmTime;
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
	TAlarmId id3 = alarm.Id();
	TheAlarmTest(id3 != KNullAlarmId, __LINE__);

	alarmTime += TTimeIntervalHours(3);
	alarm.NextDueTime() = alarmTime;
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
	TAlarmId id4 = alarm.Id();
	TheAlarmTest(id4 != KNullAlarmId, __LINE__);

	// Set system time back 6 hours - alarms are now due in 11, just under and over 24, and 27 hours

    SetHomeTime(now - TTimeIntervalHours(6));

	User::After(KTimeToWait);

	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id1,alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.RepeatDefinition() == EAlarmRepeatDefintionRepeatNext24Hours, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id2,alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.RepeatDefinition() == EAlarmRepeatDefintionRepeatNext24Hours, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id3,alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.RepeatDefinition() == EAlarmRepeatDefintionRepeatOnce, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id4,alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.RepeatDefinition() == EAlarmRepeatDefintionRepeatOnce, __LINE__);
	}

static void testAlarmPersistenceAfterSystemTimeUpdateL()
	{
	// Following a system time update, queued alarms may or may not
	// be removed, depending on the following conditions
	//
	// * EAlarmRepeatDefintionRepeatOnce
	// * EAlarmRepeatDefintionRepeatNext24Hours
	// Alarm is removed if more than 1 day in the past
	//
	// * EAlarmRepeatDefintionRepeatDaily
	// * EAlarmRepeatDefintionRepeatWorkday
	// * EAlarmRepeatDefintionRepeatWeekly
	// All repeating alarms queue the next repeat of the alarm

	TheAlarmTest.Test().Next(_L("update system time whilst alarm notifying"));

#if defined(_DEBUG)
	TheAlarmTest.Session().__DbgPreventUserNotify(ETrue);
#define KAlarmStateNotify EAlarmStateNotified
#else
#define KAlarmStateNotify EAlarmStateNotifying
#endif


	for (TInt ii = 0; ii < 4; ++ii)
		{
		// Get current alarm counts
		TInt previousQueued = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
		TInt previousNotifying = TheAlarmTest.Session().AlarmCountByState(KAlarmStateNotify) +
				TheAlarmTest.Session().AlarmCountByState(EAlarmStateWaitingToNotify);

		TTime time;
		SafeHomeTime(time);
		time += TTimeIntervalSeconds(2);

		TTime systemTime(time);

		switch (ii)
			{
			case 0:
				TheAlarmTest.Test().Next(_L("set time back 1 hour"));
				systemTime -= TTimeIntervalHours(1);
				break;
			case 1:
				TheAlarmTest.Test().Next(_L("set time forward 1 hour"));
				systemTime += TTimeIntervalHours(1);
				break;
			case 2:
				TheAlarmTest.Test().Next(_L("set time back 2 days"));
				systemTime -= TTimeIntervalDays(2);
				break;
			case 3:
				TheAlarmTest.Test().Next(_L("set time forward 2 days"));
				systemTime += TTimeIntervalDays(2);
				break;
			default:
				TheAlarmTest(EFalse, __LINE__);
				break;
			}

		// add an alarm of each repeat type due now
		TASShdAlarm alarm;

		alarm.NextDueTime() = time;
		alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
		TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
		TAlarmId idOnce = alarm.Id();
		TheAlarmTest(idOnce != KNullAlarmId, __LINE__);

		alarm.NextDueTime() = time;
		alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatNext24Hours;
		TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
		TAlarmId id24hr = alarm.Id();
		TheAlarmTest(id24hr != KNullAlarmId, __LINE__);

		alarm.NextDueTime() = time;
		alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatDaily;
		TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
		TAlarmId idDaily = alarm.Id();
		TheAlarmTest(idDaily != KNullAlarmId, __LINE__);

		alarm.NextDueTime() = time;
		alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatWorkday;
		TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
		TAlarmId idWorkday = alarm.Id();
		TheAlarmTest(idWorkday != KNullAlarmId, __LINE__);

		alarm.NextDueTime() = time;
		alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatWeekly;
		TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
		TAlarmId idWeekly = alarm.Id();
		TheAlarmTest(idWeekly != KNullAlarmId, __LINE__);

		TInt queued = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
		TInt notifying = TheAlarmTest.Session().AlarmCountByState(KAlarmStateNotify) +
			TheAlarmTest.Session().AlarmCountByState(EAlarmStateWaitingToNotify);
		TheAlarmTest(queued == previousQueued + 5, __LINE__);
		TheAlarmTest(notifying == previousNotifying, __LINE__);

		// wait for the alarms to go off
		User::After(10 * KTimeToWait);
		queued = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
		notifying = TheAlarmTest.Session().AlarmCountByState(KAlarmStateNotify) +
			TheAlarmTest.Session().AlarmCountByState(EAlarmStateWaitingToNotify);
		TheAlarmTest(queued == previousQueued + 4, __LINE__);
		TheAlarmTest(notifying == previousNotifying + 1, __LINE__);

		// update the system time

        SetHomeTime(systemTime);

		User::After(KTimeToWait);

		// check the queue of alarms
		queued = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
		notifying = TheAlarmTest.Session().AlarmCountByState(KAlarmStateNotify) +
			TheAlarmTest.Session().AlarmCountByState(EAlarmStateWaitingToNotify);

		switch (ii)
			{
			case 0: // set time back 1 hour
			case 1: // set time forward 1 hour
			case 2: // set time back 2 days
				{
				// No alarms should be removed
				TheAlarmTest(queued == previousQueued + 4, __LINE__);
				TheAlarmTest(notifying == previousNotifying + 1, __LINE__);
				TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(idOnce,alarm) == KErrNone, __LINE__);
				TheAlarmTest(alarm.State() == EAlarmStateQueued || alarm.State() == KAlarmStateNotify || alarm.State() == EAlarmStateWaitingToNotify, __LINE__);
				TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id24hr,alarm) == KErrNone, __LINE__);
				TheAlarmTest(alarm.State() == EAlarmStateQueued || alarm.State() == KAlarmStateNotify || alarm.State() == EAlarmStateWaitingToNotify, __LINE__);
				TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(idDaily,alarm) == KErrNone, __LINE__);
				TheAlarmTest(alarm.State() == EAlarmStateQueued || alarm.State() == KAlarmStateNotify || alarm.State() == EAlarmStateWaitingToNotify, __LINE__);
				TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(idWorkday,alarm) == KErrNone, __LINE__);
				TheAlarmTest(alarm.State() == EAlarmStateQueued || alarm.State() == KAlarmStateNotify || alarm.State() == EAlarmStateWaitingToNotify, __LINE__);
				TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(idWeekly,alarm) == KErrNone, __LINE__);
				TheAlarmTest(alarm.State() == EAlarmStateQueued || alarm.State() == KAlarmStateNotify || alarm.State() == EAlarmStateWaitingToNotify, __LINE__);
				break;
				}
			case 3: // set time forward 2 days
				{
				// once only and 24hr alarms should be removed
				TheAlarmTest(queued + notifying == previousQueued + previousNotifying + 3, __LINE__);
				TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(idOnce,alarm) == KErrNotFound, __LINE__);
				TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id24hr,alarm) == KErrNotFound, __LINE__);
				TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(idDaily,alarm) == KErrNone, __LINE__);
				TheAlarmTest(alarm.State() == EAlarmStateQueued || alarm.State() == KAlarmStateNotify || alarm.State() == EAlarmStateWaitingToNotify, __LINE__);
				TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(idWorkday,alarm) == KErrNone, __LINE__);
				TheAlarmTest(alarm.State() == EAlarmStateQueued || alarm.State() == KAlarmStateNotify || alarm.State() == EAlarmStateWaitingToNotify, __LINE__);
				TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(idWeekly,alarm) == KErrNone, __LINE__);
				TheAlarmTest(alarm.State() == EAlarmStateQueued || alarm.State() == KAlarmStateNotify || alarm.State() == EAlarmStateWaitingToNotify, __LINE__);
				break;
				}
			default:
				TheAlarmTest(EFalse, __LINE__);
				break;
			}

		TheAlarmTest.Session().AlarmDelete(idOnce);
		TheAlarmTest.Session().AlarmDelete(id24hr);
		TheAlarmTest.Session().AlarmDelete(idDaily);
		TheAlarmTest.Session().AlarmDelete(idWorkday);
		TheAlarmTest.Session().AlarmDelete(idWeekly);
		}
	}

static TTime SetUtcClockAlarms(const TTime& aBaseTime, const TTimeIntervalMinutes& aOffset)
//
// Set Alarms.
// Extra alarm when putting clocks back to see what happen to
// weekly alarm set for 6 days & 23 1/2 hours time
//
	{
	TInt r;
	TASShdAlarm alarm;
	const TInt n = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n == 0, __LINE__);
	TBuf<KMaxAlarmMessageLength> message;
	//
	TTime timeForAlarms(aBaseTime);
	timeForAlarms += aOffset;

	// Once Only
	alarm.Reset();
	message.Num(0);
	alarm.Message()=message;
	alarm.SetUtcNextDueTime(timeForAlarms);
	alarm.RepeatDefinition()=(EAlarmRepeatDefintionRepeatOnce);
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheClockAlarmSlots[0] = alarm.Id();

	// Next 24 hours
	alarm.Reset();
	message.Num(1);
	alarm.Message()=(message);
	alarm.SetUtcNextDueTime(timeForAlarms);
	alarm.RepeatDefinition()=(EAlarmRepeatDefintionRepeatNext24Hours);
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheClockAlarmSlots[1] = alarm.Id();

	// Clock daily
	alarm.Reset();
	message.Num(2);
	alarm.Message()=(message);
	alarm.SetUtcNextDueTime(timeForAlarms);
	alarm.RepeatDefinition()=(EAlarmRepeatDefintionRepeatDaily);
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheClockAlarmSlots[2] = alarm.Id();

	// Clock weekly
	alarm.Reset();
	message.Num(3);
	alarm.Message()=message;
	alarm.SetUtcNextDueTime(timeForAlarms);
	alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatWeekly;
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheClockAlarmSlots[3] = alarm.Id();

	if (aOffset.Int() < 0)
		{
		TheExtendedAlarmTime = timeForAlarms;
		TheExtendedAlarmTime += TTimeIntervalDays(7);
		message.Num(4);
		alarm.Message()=message;
		alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatWeekly;
		alarm.SetUtcNextDueTime(TheExtendedAlarmTime);
		r = TheAlarmTest.Session().AlarmAdd(alarm);
		TheAlarmTest(r == KErrNone, __LINE__);
		TheClockAlarmSlots[4] = alarm.Id();
		}
	return timeForAlarms;
	}

/**
Checks that the alarms are correct.

//
//	We expect and test that:
//
//	1)	Once alarm to remain unchanged at initial alarm time
//		unless allowed time to complete.
//
//	2)	Next24 to advance to the first 'initial alarm time'
//		falling due after the original time
//
//	3a)	If before clocks go back, Weekly (set previously) to
//		advance to the 'initial alarm time' in the next week
//
//	3b)	If after clocks go back, Weekly (set previously) to
//		return to the 'initial alarm time' in the current week
//
//	4a)	If before clocks go back, Weekly (set for next week)
//		to remain at the 'extended alarm time' in the next week
//
//	4b)	If after clocks go back, Weekly (set for next week) to
//		move back to 'initial alarm time' in the current week
//

@param aInterveningOffsetChange True when a change in the UTC offset occurs
between setting the alarms and testing them.
@param aOnceCompleted True if the "Repeat Once" alarm has completed.
@internalComponent
*/
static void VerifyUtcAlarmsAreCorrect(TBool aInterveningOffsetChange, TBool aOnceCompleted)
	{
	TInt ret;
	TTime setTime;
	TASShdAlarm alarm;
	TAlarmId alarmId;
	TTime time;
	time.UniversalTime();

	// Check Once repeat if it has not completed.
	if (!aOnceCompleted)
		{
		alarmId = TheClockAlarmSlots[0];
		TheAlarmTest(alarmId != KNullAlarmId, __LINE__);
		ret = TheAlarmTest.Session().GetAlarmDetails(alarmId, alarm);
		TheAlarmTest(ret == KErrNone, __LINE__);
		CheckTimesAreEqual(TheInitialAlarmTime, alarm.NextDueTime(), __LINE__);
		}

	// Check Next 24 hours repeat
	alarmId = TheClockAlarmSlots[1];
	TheAlarmTest(alarmId != KNullAlarmId, __LINE__);
	ret = TheAlarmTest.Session().GetAlarmDetails(alarmId, alarm);
	TheAlarmTest(ret == KErrNone, __LINE__);
	setTime = TheInitialAlarmTime;

	if (aInterveningOffsetChange)
		{
		// The alarm was set in the past, so add repeat interval.
		setTime += TTimeIntervalHours(+24);
		}

	CheckTimesAreEqual(setTime, alarm.NextDueTime(), __LINE__);

	// Check daily repeat
	alarmId = TheClockAlarmSlots[2];
	TheAlarmTest(alarmId != KNullAlarmId, __LINE__);
	ret = TheAlarmTest.Session().GetAlarmDetails(alarmId, alarm);
	TheAlarmTest(ret == KErrNone, __LINE__);
	setTime = TheInitialAlarmTime;

	if (aInterveningOffsetChange)
		{
		// The alarm was set in the past, so add repeat interval.
		setTime += TTimeIntervalHours(+24);
		}

	CheckTimesAreEqual(setTime, alarm.NextDueTime(), __LINE__);

	// Check weekly repeat
	alarmId = TheClockAlarmSlots[3];
	TheAlarmTest(alarmId != KNullAlarmId, __LINE__);
	ret = TheAlarmTest.Session().GetAlarmDetails(alarmId, alarm);
	TheAlarmTest(ret == KErrNone, __LINE__);
	setTime = TheInitialAlarmTime;

	if (aInterveningOffsetChange)
		{
		// The alarm was set in the past, so add repeat interval.
		setTime += TTimeIntervalDays(+7);
		}

	CheckTimesAreEqual(setTime, alarm.NextDueTime(), __LINE__);

	// Check weekly repeat
	alarmId = TheClockAlarmSlots[4];
	TheAlarmTest(alarmId != KNullAlarmId, __LINE__);
	ret = TheAlarmTest.Session().GetAlarmDetails(alarmId, alarm);
	TheAlarmTest(ret == KErrNone, __LINE__);

	if (aInterveningOffsetChange)
		{
		setTime = TheExtendedAlarmTime;
		}
	else
	    {
	    setTime = TheInitialAlarmTime;
	    }

	CheckTimesAreEqual(setTime, alarm.NextDueTime(), __LINE__);
	}

static void testUtcAlarmsOnForwardingClocks(TBool aAfterClocksGoForward, TBool aAlarmsSetInPast, TBool aAlarmsShouldAllBeQueued = ETrue)
//
//	We expect and test that:
//	1)	Once alarm to remain unchanged at 'initial alarm time'
//	2)	Next24 to remain at the 'initial alarm time'
//	3a)	If before clocks go forward, Weekly to remain at the
//		'initial alarm time'
//	3b)	If after clocks go forward, Weekly to advance to 'initial
//		alarm time' in the next week
//
	{
	TInt ret;
	TASShdAlarm alarm;
	TAlarmId alarmId;
	TTime setTime;

	//	Check Once repeat
	alarmId = TheClockAlarmSlots[0];
	ret = TheAlarmTest.Session().GetAlarmDetails(alarmId, alarm);
	setTime = TheInitialAlarmTime;

	if (aAlarmsShouldAllBeQueued)
	    {
	    TheAlarmTest(ret == KErrNone, __LINE__);
	    }

	if (ret == KErrNone && alarm.Status() == EAlarmStatusEnabled)
	    {
	    CheckTimesAreEqual(setTime, alarm.NextDueTime(), __LINE__);
	    }

	//	Check Next 24 hours repeat
	alarmId = TheClockAlarmSlots[1];
	ret = TheAlarmTest.Session().GetAlarmDetails(alarmId, alarm);
	setTime = TheInitialAlarmTime;

	if (aAlarmsShouldAllBeQueued)
	    {
	    TheAlarmTest(ret == KErrNone, __LINE__);
	    }

	if (ret == KErrNone && alarm.Status() == EAlarmStatusEnabled)
	    {
	    CheckTimesAreEqual(setTime, alarm.NextDueTime(), __LINE__);
	    }

	//	Check daily repeat
	alarmId = TheClockAlarmSlots[2];
	TheAlarmTest(alarmId != KNullAlarmId, __LINE__);
	ret = TheAlarmTest.Session().GetAlarmDetails(alarmId, alarm);
	setTime = TheInitialAlarmTime;

	if (aAlarmsShouldAllBeQueued)
	    {
	    TheAlarmTest(ret == KErrNone, __LINE__);
	    }

	CheckTimesAreEqual(setTime, alarm.NextDueTime(), __LINE__);

	//	Check weekly repeat
	alarmId = TheClockAlarmSlots[3];
	TheAlarmTest(alarmId != KNullAlarmId, __LINE__);
	ret = TheAlarmTest.Session().GetAlarmDetails(alarmId, alarm);

	if (aAlarmsShouldAllBeQueued)
	    {
	    TheAlarmTest(ret == KErrNone, __LINE__);
	    }
	//
	setTime=TheInitialAlarmTime;

#if defined(_DEBUG)
    // In udeb the alarm goes from the state EAlarmStateQueued to the state EAlarmStateNotified
    // (skipping the state ENotifying).In EAlarmStateNotified the nextTime for the alarm is
    // recalculated and the expected value (called setTime here) needs adjustmentto reflect this.
    // In urel the the EAlarmStateNotified state (where the NextTime for an alarm is recalculated)
    // is never reached so the  adjustement in the expected value (called setTime)is not necessary .
	if (aAfterClocksGoForward && aAlarmsSetInPast)
		{
		setTime += TTimeIntervalDays(+7);
		}
#else
	//Just to get rid of warning
	(void)aAfterClocksGoForward;
	(void)aAlarmsSetInPast;
#endif

	CheckTimesAreEqual(setTime, alarm.NextDueTime(), __LINE__);
	}

static void TestUtcAlarmsWithDaylightSavingChange1L()
//
//	Test effects of changing daylight saving on clock alarms1
//
	{
	TheAlarmTest.Test().Next(_L("Initialise with daylight saving on"));
	EnableDaylightSavingL(ETrue);
	TheCurrentTime.UniversalTime();
	//
	TheAlarmTest.Test().Next(_L("Setting UTC alarms half an hour in the past"));
	TheInitialAlarmTime = SetUtcClockAlarms(TheCurrentTime, TTimeIntervalMinutes(-30));
	//
	TheAlarmTest.Test().Next(_L("Testing UTC alarms when daylight saving is on"));
	VerifyUtcAlarmsAreCorrect(ETrue, EFalse);
	//
	TheAlarmTest.Test().Next(_L("Turning daylight saving off"));
	EnableDaylightSavingL(EFalse);
	//
	TheAlarmTest.Test().Next(_L("Testing UTC alarms when daylight saving is off"));
	VerifyUtcAlarmsAreCorrect(ETrue, ETrue);
	//
	TheAlarmTest.Test().Next(_L("Removing all alarms"));
	TheAlarmTest.TestClearStoreL();
	}

static void TestUtcAlarmsWithDaylightSavingChange2L()
//
//	Test effects of changing daylight saving on clock alarms2
//
	{
	TheAlarmTest.Test().Next(_L("Initialise with daylight saving off"));
	EnableDaylightSavingL(EFalse);
	TheCurrentTime.UniversalTime();
	//
	TheAlarmTest.Test().Next(_L("Setting UTC alarms an hour and a half in the future"));
	// Used to set 30 minutes into future, but a defect in User::SetHomeTime() requires times
	// not to be set between 31-10-2004 1.59 (BST) and 31-10-2004 1.59 (GMT).
	TheInitialAlarmTime = SetUtcClockAlarms(TheCurrentTime, TTimeIntervalMinutes(90));
	//
	TheAlarmTest.Test().Next(_L("Testing UTC alarms with daylight saving off"));
	testUtcAlarmsOnForwardingClocks(EFalse, EFalse, ETrue);
	//
	TheAlarmTest.Test().Next(_L("Turning daylight saving on"));
	EnableDaylightSavingL(ETrue);
	//
	TheAlarmTest.Test().Next(_L("Testing alarms with daylight saving on"));
	testUtcAlarmsOnForwardingClocks(ETrue, EFalse, EFalse);
	//
	TheAlarmTest.Test().Next(_L("Removing all UTC alarms"));
	TheAlarmTest.TestClearStoreL();
	}

//
//	Test changes in workdays
//

static void testSetWithNoWorkdaysUtc()
//
//	Test what happens when setting a workday alarm with no workdays set
//
	{
	TheAlarmTest.Test().Next(_L("Setting workday UTC alarm with no workday set"));

	// Set no workdays
	TUint workdayMask = 0;
	TLocale locale;
	locale.SetWorkDays(workdayMask);
	locale.Set();

	// Have to wait a bit here because the kernel takes some time
	// to notify client-side environment change objects and so
	// we need to allow the alarm server to do its thing...
	User::After(100000);
	//
	TInt ret = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(ret == 0, __LINE__);
	//
	TheOriginalTime.UniversalTime();
	TheInitialAlarmTime = TheOriginalTime;
	TheInitialAlarmTime += TTimeIntervalHours(1);
	//
	TASShdAlarm alarm;
	alarm.Message()=(_L("Clock alarm in slot 0"));
	alarm.SetUtcNextDueTime(TheInitialAlarmTime);
	alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatWorkday;
	ret = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(ret == KErrNone, __LINE__);
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	TheClockAlarmSlots[0] = alarm.Id();

	// Test alarm state, should be disabled
	TASShdAlarm alarm2;
	ret = TheAlarmTest.Session().GetAlarmDetails(alarm.Id(), alarm2);
	TheAlarmTest(ret == KErrNone, __LINE__);
	TheAlarmTest(alarm2.Status() == EAlarmStatusDisabled, __LINE__);
	}

static void testSetWorkdaysUtc()
//
//	Create workdays and test
//
	{
	TheAlarmTest.Test().Next(_L("Creating and testing workdays"));
	//
	TInt todayMaskOffset = TheInitialAlarmTime.DayNoInWeek();
	TUint workdayMask = 1;
	//
	for (TInt count = 0; count < 7; count++)
		{
		workdayMask = 1;
		const TInt dayOfWeek = (todayMaskOffset+count)%7;
		workdayMask <<= dayOfWeek;
		//
		TLocale locale;
		locale.SetWorkDays(workdayMask);
		locale.Set();
		User::After(100000);
		//
		const TAlarmId id = TheClockAlarmSlots[0];
		TheAlarmTest(id != KNullAlarmId, __LINE__);
		//
		TASShdAlarm alarm;
		const TInt r = TheAlarmTest.Session().GetAlarmDetails(id, alarm);
		TheAlarmTest(r == KErrNone, __LINE__);
		TheAlarmTest(alarm.Status() == EAlarmStatusEnabled, __LINE__);
		//
		TTime expectedTime = TheInitialAlarmTime;
		expectedTime += TTimeIntervalDays(count);
		CheckTimesAreEqual(expectedTime, alarm.NextDueTime(), __LINE__);
		}
	}

static void testMakeAllWorkdaysUtcL()
//
//	Make all days workdays
//	Advance time a day at a time to test alarm for each day of the week
//
	{
	TheAlarmTest.Test().Next(_L("Make all days workdays"));
	//
	TBuf<60> dateTimeStr;
	_LIT(KTimeDateFormat,"%*E%*D%X%*N%*Y %1 %2 '%3 - %-B%:0%J%:1%T%:2%S%:3%+B");

	const TUint workdayMask = 127;// Seven 1's
	TLocale locale;
	locale.SetWorkDays(workdayMask);
	locale.Set();
	User::After(100000);

	for (TInt count = 0; count < 7; count++)
		{
		if (count)
			{
			// Advance time
			TTime advTime(TheOriginalTime + TTimeIntervalDays(count));
            User::SetUTCTime(advTime);
			advTime.FormatL(dateTimeStr, KTimeDateFormat);
			TheAlarmTest.Test().Printf(_L("Advance time : %S\n"), &dateTimeStr);
			}

		const TAlarmId id = TheClockAlarmSlots[0];
		TheAlarmTest(id != KNullAlarmId, __LINE__);
		//
		TASShdAlarm alarm;
		const TInt r = TheAlarmTest.Session().GetAlarmDetails(id, alarm);
		TheAlarmTest(r == KErrNone, __LINE__);
		TheAlarmTest(alarm.Status() == EAlarmStatusEnabled, __LINE__);
		//
		TTime expectedTime = TheInitialAlarmTime;
		TheInitialAlarmTime.FormatL(dateTimeStr,KTimeDateFormat);
		TheAlarmTest.Test().Printf(_L("TheInitialAlarmTime: %S\n"), &dateTimeStr);

		expectedTime += TTimeIntervalDays(count);
		//
		const TInt days = expectedTime.DaysFrom(alarm.NextDueTime()).Int();

		if (days != 0)  //In that case, the next due time is within 24 hours in the past
		    {
		    expectedTime -= TTimeIntervalDays(1);
		    }

		expectedTime.FormatL(dateTimeStr,KTimeDateFormat);
		TheAlarmTest.Test().Printf(_L("expectedTime : %S\n"), &dateTimeStr);

		alarm.NextDueTime().FormatL(dateTimeStr,KTimeDateFormat);
		TheAlarmTest.Test().Printf(_L("NextDueTime(): %S\n"), &dateTimeStr);

		CheckTimesAreEqual(expectedTime, alarm.NextDueTime(), __LINE__);
		}
	}

static void testRemoveWorkdaysUtc()
//
//	Remove all workdays and test
//
	{
	const TAlarmId id = TheClockAlarmSlots[0];
	TheAlarmTest(id != KNullAlarmId, __LINE__);
	//
	TLocale locale;
	locale.SetWorkDays(0);
	locale.Set();
	User::After(100000);
	//
	TASShdAlarm alarm;
	const TInt r = TheAlarmTest.Session().GetAlarmDetails(id, alarm);
	//
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarm.Status() == EAlarmStatusDisabled, __LINE__);
	}

static void testLocaleChangeWorkdaysUtcL()
//
//	Test effects of changing workdays on alarms
//
	{
	TheAlarmTest.TestClearStoreL();
	testSetWithNoWorkdaysUtc();
	testSetWorkdaysUtc();
	testMakeAllWorkdaysUtcL();
	testRemoveWorkdaysUtc();
	TheAlarmTest.TestClearStoreL();
	}

static void test24HourToOnceOnlyUtcAlarms()
	{
	TheAlarmTest.Test().Next(_L("Test update of 24 hour alarms to once only alarms"));

	// Next 24 hour alarms which are more than 1 day in the future after a system time
	// update should be updated to once only alarms

	// Set 'next 24 hour' alarms due in 5, just under and over 18, and 21 hours time
	TTime now;
	now.UniversalTime();
	TTime alarmTime(now);

	TASShdAlarm alarm;
	alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatNext24Hours;

	alarmTime += TTimeIntervalHours(5);
	alarm.SetUtcNextDueTime(alarmTime);
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
	TAlarmId id1 = alarm.Id();
	TheAlarmTest(id1 != KNullAlarmId, __LINE__);

	alarmTime += TTimeIntervalHours(13);
	alarmTime -= TTimeIntervalMinutes(1);
	alarm.SetUtcNextDueTime(alarmTime);
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
	TAlarmId id2 = alarm.Id();
	TheAlarmTest(id2 != KNullAlarmId, __LINE__);

	alarmTime += TTimeIntervalMinutes(2);
	alarm.SetUtcNextDueTime(alarmTime);
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
	TAlarmId id3 = alarm.Id();
	TheAlarmTest(id3 != KNullAlarmId, __LINE__);

	alarmTime += TTimeIntervalHours(3);
	alarm.SetUtcNextDueTime(alarmTime);
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
	TAlarmId id4 = alarm.Id();
	TheAlarmTest(id4 != KNullAlarmId, __LINE__);

	// Set system time back 6 hours - alarms are now due in 11, just under and over 24, and 27 hours

    now -= TTimeIntervalHours(6);
    User::SetUTCTime(now);

	User::After(KTimeToWait);

	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id1,alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.RepeatDefinition() == EAlarmRepeatDefintionRepeatNext24Hours, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id2,alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.RepeatDefinition() == EAlarmRepeatDefintionRepeatNext24Hours, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id3,alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.RepeatDefinition() == EAlarmRepeatDefintionRepeatOnce, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id4,alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.RepeatDefinition() == EAlarmRepeatDefintionRepeatOnce, __LINE__);
	}

static void testUtcAlarmPersistenceAfterSystemTimeUpdateL()
	{
	// Following a system time update, queued alarms may or may not
	// be removed, depending on the following conditions
	//
	// * EAlarmRepeatDefintionRepeatOnce
	// * EAlarmRepeatDefintionRepeatNext24Hours
	// Alarm is removed if more than 1 day in the past
	//
	// * EAlarmRepeatDefintionRepeatDaily
	// * EAlarmRepeatDefintionRepeatWorkday
	// * EAlarmRepeatDefintionRepeatWeekly
	// All repeating alarms queue the next repeat of the alarm

	TheAlarmTest.Test().Next(_L("update system time whilst UTC alarm notifying"));

#if defined(_DEBUG)
	TheAlarmTest.Session().__DbgPreventUserNotify(ETrue);
#define KAlarmStateNotify EAlarmStateNotified
#else
#define KAlarmStateNotify EAlarmStateNotifying
#endif

	for (TInt ii = 0; ii < 4; ++ii)
		{
		// Get current alarm counts
		TInt previousQueued = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
		TInt previousNotifying = TheAlarmTest.Session().AlarmCountByState(KAlarmStateNotify) +
				TheAlarmTest.Session().AlarmCountByState(EAlarmStateWaitingToNotify);

		TTime time;
		SafeHomeTime(time);
		time -= User::UTCOffset();
		time += TTimeIntervalSeconds(2);

		TTime systemTime(time);

		switch (ii)
			{
			case 0:
				TheAlarmTest.Test().Next(_L("set time back 1 hour"));
				systemTime -= TTimeIntervalHours(1);
				break;
			case 1:
				TheAlarmTest.Test().Next(_L("set time forward 1 hour"));
				systemTime += TTimeIntervalHours(1);
				break;
			case 2:
				TheAlarmTest.Test().Next(_L("set time back 2 days"));
				systemTime -= TTimeIntervalDays(2);
				break;
			case 3:
				TheAlarmTest.Test().Next(_L("set time forward 2 days"));
				systemTime += TTimeIntervalDays(2);
				break;
			default:
				TheAlarmTest(EFalse, __LINE__);
				break;
			}

		// add a UTC alarm of each repeat type due now
		TASShdAlarm alarm;

		alarm.SetUtcNextDueTime(time);
		alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatOnce;
		TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
		TAlarmId idOnce = alarm.Id();
		TheAlarmTest(idOnce != KNullAlarmId, __LINE__);

		alarm.SetUtcNextDueTime(time);
		alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatNext24Hours;
		TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
		TAlarmId id24hr = alarm.Id();
		TheAlarmTest(id24hr != KNullAlarmId, __LINE__);

		alarm.SetUtcNextDueTime(time);
		alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatDaily;
		TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
		TAlarmId idDaily = alarm.Id();
		TheAlarmTest(idDaily != KNullAlarmId, __LINE__);

		alarm.SetUtcNextDueTime(time);
		alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatWorkday;
		TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
		TAlarmId idWorkday = alarm.Id();
		TheAlarmTest(idWorkday != KNullAlarmId, __LINE__);

		alarm.SetUtcNextDueTime(time);
		alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatWeekly;
		TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
		TAlarmId idWeekly = alarm.Id();
		TheAlarmTest(idWeekly != KNullAlarmId, __LINE__);

		TInt queued = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
		TInt notifying = TheAlarmTest.Session().AlarmCountByState(KAlarmStateNotify) +
			TheAlarmTest.Session().AlarmCountByState(EAlarmStateWaitingToNotify);
		TheAlarmTest(queued == previousQueued + 5, __LINE__);
		TheAlarmTest(notifying == previousNotifying, __LINE__);

		// wait for the alarms to go off
		User::After(10 * KTimeToWait);
		queued = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
		notifying = TheAlarmTest.Session().AlarmCountByState(KAlarmStateNotify) +
			TheAlarmTest.Session().AlarmCountByState(EAlarmStateWaitingToNotify);
		TheAlarmTest(queued == previousQueued + 4, __LINE__);
		TheAlarmTest(notifying == previousNotifying + 1, __LINE__);

		// update the system time

        User::SetUTCTime(systemTime);

		User::After(KTimeToWait);

		// check the queue of alarms
		queued = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
		notifying = TheAlarmTest.Session().AlarmCountByState(KAlarmStateNotify) +
			TheAlarmTest.Session().AlarmCountByState(EAlarmStateWaitingToNotify);

		switch (ii)
			{
			case 0: // set time back 1 hour
			case 1: // set time forward 1 hour
			case 2: // set time back 2 days
				{
				// No alarms should be removed
				TheAlarmTest(queued == previousQueued + 4, __LINE__);
				TheAlarmTest(notifying == previousNotifying + 1, __LINE__);
				TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(idOnce,alarm) == KErrNone, __LINE__);
				TheAlarmTest(alarm.State() == EAlarmStateQueued || alarm.State() == KAlarmStateNotify || alarm.State() == EAlarmStateWaitingToNotify, __LINE__);
				TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id24hr,alarm) == KErrNone, __LINE__);
				TheAlarmTest(alarm.State() == EAlarmStateQueued || alarm.State() == KAlarmStateNotify || alarm.State() == EAlarmStateWaitingToNotify, __LINE__);
				TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(idDaily,alarm) == KErrNone, __LINE__);
				TheAlarmTest(alarm.State() == EAlarmStateQueued || alarm.State() == KAlarmStateNotify || alarm.State() == EAlarmStateWaitingToNotify, __LINE__);
				TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(idWorkday,alarm) == KErrNone, __LINE__);
				TheAlarmTest(alarm.State() == EAlarmStateQueued || alarm.State() == KAlarmStateNotify || alarm.State() == EAlarmStateWaitingToNotify, __LINE__);
				TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(idWeekly,alarm) == KErrNone, __LINE__);
				TheAlarmTest(alarm.State() == EAlarmStateQueued || alarm.State() == KAlarmStateNotify || alarm.State() == EAlarmStateWaitingToNotify, __LINE__);
				break;
				}
			case 3: // set time forward 2 days
				{
				// once only and 24hr alarms should be removed
				TheAlarmTest(queued + notifying == previousQueued + previousNotifying + 3, __LINE__);
				TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(idOnce,alarm) == KErrNotFound, __LINE__);
				TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id24hr,alarm) == KErrNotFound, __LINE__);
				TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(idDaily,alarm) == KErrNone, __LINE__);
				TheAlarmTest(alarm.State() == EAlarmStateQueued || alarm.State() == KAlarmStateNotify || alarm.State() == EAlarmStateWaitingToNotify, __LINE__);
				TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(idWorkday,alarm) == KErrNone, __LINE__);
				TheAlarmTest(alarm.State() == EAlarmStateQueued || alarm.State() == KAlarmStateNotify || alarm.State() == EAlarmStateWaitingToNotify, __LINE__);
				TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(idWeekly,alarm) == KErrNone, __LINE__);
				TheAlarmTest(alarm.State() == EAlarmStateQueued || alarm.State() == KAlarmStateNotify || alarm.State() == EAlarmStateWaitingToNotify, __LINE__);
				break;
				}
			default:
				TheAlarmTest(EFalse, __LINE__);
				break;
			}

		TheAlarmTest.Session().AlarmDelete(idOnce);
		TheAlarmTest.Session().AlarmDelete(id24hr);
		TheAlarmTest.Session().AlarmDelete(idDaily);
		TheAlarmTest.Session().AlarmDelete(idWorkday);
		TheAlarmTest.Session().AlarmDelete(idWeekly);
		}
	}


static void doTestsL()
	{
	//get the initial time
 	TTime homeTime;
	homeTime.HomeTime();

	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	User::LeaveIfError(timezone.Connect());

	// safeguard against leaving
	CleanupClosePushL(timezone);

	timezone.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOn);

	TheAlarmTest.TestStartServers();
	TheAlarmTest(TheAlarmTest.Session().Connect() == KErrNone, __LINE__);
	TheAlarmTest.TestClearStoreL();

#if defined(_DEBUG)
	TheAlarmTest.Session().__DbgPreventUserNotify(ETrue);
#endif


	// Disable Daylight Saving
	EnableDaylightSavingL(EFalse);

	User::After(100000);

	//
	// Test the local API.

	TheAlarmTest.Test().Printf(_L("Begin local API tests."));

	TestAlarmsWithDaylightSavingChange1L();
	TestAlarmsWithDaylightSavingChange2L();
	/*
	Changes to the UTC offset due to a DST change produce equivalent behaviour
	to changes to the UTC offset due to a locale change, therefore, there is no
	need to test DST and locale changes separately.
	*/

	//	Reset locale for workday testing
	// Disable Daylight Saving
	EnableDaylightSavingL(EFalse);
	User::After(100000);
	testLocaleChangeWorkdaysL();
	test24HourToOnceOnlyAlarms();
	//
	// The workdays enabled.
	TInt workdays = (1 << EMonday) | (1 << ETuesday) | (1 << EWednesday) | (1 << EThursday) | (1 << EFriday);

	TLocale locale;
	locale.SetWorkDays(workdays);
	locale.Set();
	User::After(100000);

	// Update system time while alarm is notifying
	testAlarmPersistenceAfterSystemTimeUpdateL();

	// Clean up after the local API test if it was run.
	TheAlarmTest.TestClearStoreL();

	//
	// Test the UTC API.

	TheAlarmTest.Test().Printf(_L("Begin UTC API tests."));

	TestUtcAlarmsWithDaylightSavingChange1L();
	TestUtcAlarmsWithDaylightSavingChange2L();
	/*
	Changes to the UTC offset due to a DST change produce equivalent behaviour
	to changes to the UTC offset due to a locale change, therefore, there is no
	need to test DST and locale changes separately.
	*/

	//	Reset locale for workday testing
	// Disable Daylight Saving
	EnableDaylightSavingL(EFalse);
	User::After(100000);
	testLocaleChangeWorkdaysUtcL();
	test24HourToOnceOnlyUtcAlarms();
	//
	workdays = (1 << EMonday) | (1 << ETuesday) | (1 << EWednesday) | (1 << EThursday) | (1 << EFriday);
	locale.SetWorkDays(workdays);
	locale.Set();
	User::After(100000);

	// Update system time while alarm is notifying
	testUtcAlarmPersistenceAfterSystemTimeUpdateL();

    CleanupStack::PopAndDestroy(&timezone);


	//Set back the time to the initial time
    RPIMTestServer testserver;
    User::LeaveIfError(testserver.Connect());
    testserver.SetHomeTime(homeTime);
    testserver.Close();

	CleanupStack::PopAndDestroy(scheduler);
	}

/**

@SYMTestCaseID PIM-TALARMLOCALECHANGES-0001

*/

GLDEF_C TInt E32Main()
//
// Test the alarm server.
//
    {
    TInt ret = KErrNone;
	TheAlarmTest.Title();
	TheAlarmTest.Start(_L("@SYMTestCaseID:PIM-TALARMLOCALECHANGES-0001 \"Clock\" alarms"));

	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	TRAP_IGNORE(doTestsL());
	delete cleanup;
	__UHEAP_MARKEND;

    TheAlarmTest.Session().Close();
    TRAP(ret,TheAlarmTest.EndL());

	TheAlarmTest.Test().Close();
	ASTstAlarmTest::Close();
	//
	return ret;
    }
