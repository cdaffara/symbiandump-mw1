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

// System includes
#include <e32base.h>
#include <e32test.h>
#include <f32file.h>

// User includes
#include <asshdalarm.h>
#include <asclisession.h>
#include <consolealarmalertservermain.h>
#include <consolealarmalertservermain.h>
#include "testserver.h"
#include "ASTstAlarmTest.h"
#include <asclisoundplay.h>
#ifdef __WINS__
#include "ConsoleANTestClient.h"
#endif


// Type definitions
typedef void (*ThreadTestL)(void);
typedef CArrayFixFlat<TASCliSoundPlayDefinition> CPlayIntervalArray;

// Constants
const TUid KServerUid2={0x1000008D};
const TUid KServerUid3={0x101F502A};
const TUidType serverUid(KNullUid,KServerUid2,KServerUid3);

#define KNewAlarmCycleStartOffsetMinutes        {0,1,2,3,KErrNotFound};

_LIT(KTestIniFile, "c:\\private\\101f5027\\AlarmServer.ini");

// Globals
static RASCliSession TheAlarmServerSession;

#if defined(_DEBUG) && defined(__WINS__)
_LIT(KDateTimeFormat, "%F %*E %*N %D %Y %J:%T:%S%B");

static void PrintTimeL(const TTime& aTime, const TDesC& aPrefix)
	{
	TBuf<30> dateTimeString;
	aTime.FormatL(dateTimeString, KDateTimeFormat);
	TheAlarmTest.Test().Printf(_L("%S %S\n"), &aPrefix, &dateTimeString);
	}
#endif

static void TestClearStoreL()
	{
	// Delete all alarms
	RArray<TAlarmCategory> categories;
	TheAlarmServerSession.GetAvailableCategoryListL(categories);
	TInt count = categories.Count();
	for(TInt i=0; i<count; i++)
		{
		const TAlarmCategory category = categories[i];
		TheAlarmTest(TheAlarmServerSession.AlarmDeleteAllByCategory(category, EFalse) == KErrNone, __LINE__);
		}
	categories.Close();
	}

void WaitForNotificationBufferToBeEmptied()
	{
	TRequestStatus status = KErrNone;
	TAlarmId alarmId;
	if	(status == KRequestPending)
		return;

	TheAlarmServerSession.NotifyChange(status, alarmId);
	User::After(1000000);
	if	(status == KRequestPending)
		return;
	User::WaitForRequest(status);

	// Because of event buffering inside the Alarm Server
	// We need to wait until there aren't any more events
	// in the buffer, and then attempt to cancel the request.
	//
	// Trying to cancel a request when the server has
	// already completed the message (because something
	// was in the buffer) results in the server panicing
	// the client.
	FOREVER
		{
		TheAlarmTest(status >= EAlarmChangeEventState && status <= EAlarmChangeEventLast);
		TheAlarmServerSession.NotifyChange(status, alarmId);
		//
		// Wait a second so that we can tell if this really is
		// waiting for an event, or just for the Alarm Server
		// process to be given some scheduling time by the
		// kernel
		User::After(1000000);

		// If its still pending, then we should attempt
		// to cancel the request. Otherwise, if its not
		// pending, then the Alarm Server probably just
		// completed the request based upon the contents
		// of the event buffer.
		if	(status == KRequestPending)
			break;
		else
			{
			// Eat the request and try again
			User::WaitForRequest(status);
			}
		}
	}

void testTimesEqual(const TTime& aTime1, const TTime& aTime2)
	{
	// determine the difference, in seconds
	TTimeIntervalSeconds	timeDiffSeconds;
	const TInt error = aTime2.SecondsFrom(aTime1, timeDiffSeconds);
	TheAlarmTest(error == KErrNone);
	// check the times are within 1 second
	// as the tolerance for notifying alarms should be bigger than 1 second
	const TTimeIntervalSeconds	maxDiff(1);
	const TTimeIntervalSeconds	minDiff(-1);
	TheAlarmTest(((timeDiffSeconds <= maxDiff) && (timeDiffSeconds >= minDiff)));
	}


/**
@SYMTestCaseID PIM-TBASICALARMS-INC092682-0002
@SYMTestType UT
@SYMTestPriority High
@SYMDEF INC092682
@SYMTestCaseDesc Tests that re-enabling of a repeating alarm correctly maintains
the next due alarm ID (the head alarm in the alarm queue).

@SYMTestActions
1. Create a workday alarm due to activate in 30 minutes time.
2. Create a once only alarm due to activate in 90 minutes time.
3. Check that their are 2 active alarms in the queue.
4. Check that workday alarm is due to expire before once only alarm.
5. Disable workday alarm and check that once only alarm is the next due alarm.
6. Advance current time by 1 hour.  The repeating (workday) alarm should be
   rescheduled for the next workday, leaving the once only alarm as the next due
   alarm.
7. Enable workday alarm.
8. Check that once only alarm is next due alarm - workday alarm should be due
   next day now since time was advanced past today's workday alarm time.

@SYMTestExpectedResults The next due alarm is as expected.
*/


static void TestINC092682L()
	{
	TheAlarmTest.Next(_L("@SYMTestCaseID PIM-TBASICALARMS-INC092682-0002 Testing fix for INC092682."));

	//
	// Make sure we start with no alarms.
	//
	TestClearStoreL();

	//
	// 1. Create a workday alarm due to activate in 30 minutes time.
	//
	TASShdAlarm alarm;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatWorkday;
	alarm.Message() = _L("This is workday alarm message");
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalMinutes(30);
	TheAlarmTest(TheAlarmServerSession.AlarmAdd(alarm) == KErrNone, __LINE__);
	const TAlarmId workdayId = alarm.Id();

	//
	// 2. Create a once only alarm due to activate in 90 minutes time.
	//
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	alarm.Message() = _L("This is once only alarm message");
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalMinutes(90);
	TheAlarmTest(TheAlarmServerSession.AlarmAdd(alarm) == KErrNone, __LINE__);
	const TAlarmId onceOnlyId = alarm.Id();

	//
	// 3. Check that their are 2 active alarms in the queue.
	//
	TheAlarmTest(TheAlarmServerSession.NumberOfAlarmsActiveInQueue() == 2, __LINE__);

	//
	// 4. Check that workday alarm is due to expire before once only alarm.
	//
	TAlarmId nextAlarmId;
	TInt r = TheAlarmServerSession.GetNextDueAlarmId(nextAlarmId);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(nextAlarmId == workdayId, __LINE__);

	//
	// 5. Disable workday alarm and check that once only alarm is the next due
	// alarm.
	//
	TheAlarmTest(TheAlarmServerSession.SetAlarmStatus(workdayId, EAlarmStatusDisabled) == KErrNone, __LINE__);
	r = TheAlarmServerSession.GetNextDueAlarmId(nextAlarmId);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(nextAlarmId == onceOnlyId, __LINE__);

	//
	// 6. Advance current time by 1 hour.  The repeating (workday) alarm should
	// be rescheduled for the next workday, leaving the once only alarm as the
	// next due alarm.
	//
	TTime oneHourLater;
	oneHourLater.HomeTime();
	oneHourLater += TTimeIntervalHours(1);
	RPIMTestServer serv;
	User::LeaveIfError(serv.Connect());
	serv.SetHomeTime(oneHourLater);
	// Wait for time change notifications to occur.
	User::After(10000);
	// Set again to ensure time value is correct when update occurs.
	serv.SetHomeTime(oneHourLater);
	serv.Close();

	//
	// 7. Enable workday alarm.
	//
	TheAlarmTest(TheAlarmServerSession.SetAlarmStatus(workdayId, EAlarmStatusEnabled) == KErrNone, __LINE__);

	//
	// 8. Check that once only alarm is next due alarm - workday alarm should be
	// due next day now since time was advanced past today's workday alarm
	// time.
	//
	r = TheAlarmServerSession.GetNextDueAlarmId(nextAlarmId);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(nextAlarmId == onceOnlyId, __LINE__);

	//
	// Tidy up.
	//
	TestClearStoreL();
	WaitForNotificationBufferToBeEmptied();
	}


//*************************************************************************************
static void CreateSingleAlarmL()
	{
#ifdef __WINS__
	
	
	TheAlarmTest.Next(_L("Adding disabled alarm"));
	//
	TASShdAlarm alarm;
	alarm.Category() = KASCliCategoryClock;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatDaily;
	alarm.Message() = _L("This is an alarm message - alarm 1");
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalMinutes(1);
	alarm.SoundName() = _L("Arf Arf Woop");
	//
	TTime now;
	now.HomeTime();
	//
	TheAlarmTest(TheAlarmServerSession.AlarmAdd(alarm) == KErrNone, __LINE__);
	const TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);

		
	
	TheAlarmTest.Next(_L("Get alarm details"));
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone, __LINE__);

		
	
	TheAlarmTest.Next(_L("Enabling a disabled alarm"));
	TheAlarmTest(TheAlarmServerSession.SetAlarmStatus(id, EAlarmStatusEnabled) == KErrNone, __LINE__);

		
	
	TheAlarmTest.Next(_L("Waiting for alarm to expire..."));

	TAlarmId alarmId;
	TRequestStatus status;
	FOREVER
		{
		TheAlarmServerSession.NotifyChange(status, alarmId);
		User::WaitForRequest(status);
		if	(status.Int() == EAlarmChangeEventTimerExpired)
			{
			TheAlarmTest(id == alarmId, __LINE__);
			break;
			}
		}
#endif
	}


//*************************************************************************************
static void CreateAlarmAndIgnoreL()
    {
#ifdef __WINS__
	
    TheAlarmTest.Next(_L("Adding disabled alarm"));
    //
    TASShdAlarm alarm;
    alarm.Category() = KASCliCategoryClock;
    alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatDaily;
    alarm.Message() = _L("This is an alarm message - alarm 1");
    alarm.NextDueTime().HomeTime();
    alarm.NextDueTime() += TTimeIntervalMinutes(1);
    alarm.SoundName() = _L("Arf Arf Woop");

    //
    TTime now;
    now.HomeTime();
    //
    TheAlarmTest(TheAlarmServerSession.AlarmAdd(alarm) == KErrNone, __LINE__);
    const TAlarmId id = alarm.Id();
    TheAlarmTest(id != KNullAlarmId, __LINE__);

    // copy the current soundsettings
    CPlayIntervalArray* originalSoundArray = new(ELeave) CPlayIntervalArray(20);
    CleanupStack::PushL(originalSoundArray);

    TheAlarmServerSession.GetAlarmPlayIntervalsL(*originalSoundArray);

    // use shorted one
    CPlayIntervalArray* newSoundArray = new(ELeave) CPlayIntervalArray(10);
    CleanupStack::PushL(newSoundArray);
    const TInt minuteCycle[]=KNewAlarmCycleStartOffsetMinutes;
    TInt i(0);
    TInt offset(minuteCycle[i]);
    while (offset != KErrNotFound)
        {
        const TASCliSoundPlayDefinition item(offset, KDefaultSoundPlayDurationInSeconds);
        newSoundArray->AppendL(item);
        offset = minuteCycle[++i];
        }
    TheAlarmServerSession.SetAlarmPlayIntervalsL(*newSoundArray);

    
    TheAlarmTest.Next(_L("Get alarm details"));
    TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone, __LINE__);

    
    TheAlarmTest.Next(_L("Enabling a disabled alarm"));
    TheAlarmTest(TheAlarmServerSession.SetAlarmStatus(id, EAlarmStatusEnabled) == KErrNone, __LINE__);

    
    TheAlarmTest.Next(_L("Waiting for alarm to expire..."));
    TInt numberOfIntervals(newSoundArray->Count());
    TInt count(0);

    TAlarmId alarmId;
    TRequestStatus status;
    FOREVER
        {
        TheAlarmServerSession.NotifyChange(status, alarmId);
        User::WaitForRequest(status);
        if    (status.Int() == EAlarmChangeEventSoundPlaying)
            {
            if(count > numberOfIntervals)
                break;
            count++;
            }
        }
    // this to check that we have gone through all the time intervals in array
    TheAlarmTest(count > newSoundArray->Count(), __LINE__);
    // restore original sound settings
	
    TheAlarmTest.Test().Next(_L("reset default sound intervals"));
    TheAlarmServerSession.SetAlarmPlayIntervalsL(*originalSoundArray);
    CleanupStack::PopAndDestroy(2);// originalSoundArray, newSoundArray

#endif
    }

//*************************************************************************************

	
static void CreateNotificationAlarmL()
	{
	TheAlarmTest.Next(_L("Creating notification alarm"));

	// AlarmAddWithNotification
	TASShdAlarm alarm;
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalMinutes(1);
	TRequestStatus status;
	TheAlarmServerSession.AlarmAddWithNotification(status, alarm);
	TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);
	TheAlarmTest(status == KRequestPending, __LINE__);

	// Wait for completion
	TheAlarmTest(TheAlarmServerSession.SetAlarmStatus(id, EAlarmStatusEnabled) == KErrNone, __LINE__);
	User::WaitForRequest(status);

	// AlarmNotificationCancel
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalMinutes(1);
	TheAlarmServerSession.AlarmAddWithNotification(status, alarm);
	id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);
	TheAlarmTest(status == KRequestPending, __LINE__);
	TheAlarmServerSession.AlarmNotificationCancelAndDequeue(id);

	// Eat request
	User::WaitForRequest(status);
	TheAlarmTest(status == KErrCancel, __LINE__);
	}


//*************************************************************************************
	

static void CreateDailyRepeatingAlarmL()
	{
	TheAlarmTest.Next(_L("Creating daily repeating alarm"));

	// AlarmAddWithNotification
	TASShdAlarm alarm;
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalMinutes(1);
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatDaily;

	TheAlarmServerSession.AlarmAdd(alarm);
	TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);

	TAlarmId alarmId;
	TRequestStatus status;
	TheAlarmServerSession.NotifyChange(status, alarmId);
	TheAlarmTest(TheAlarmServerSession.SetAlarmStatus(id, EAlarmStatusEnabled) == KErrNone, __LINE__);
	User::WaitForRequest(status);
	}


static void CreateAlarmsForPausingL()
	{
#ifdef __WINS__

	TestClearStoreL();

	TheAlarmTest.Next(_L("Test pausing alarm sounds"));
	//
	// Add an alarm, due in 3 seconds
	TASShdAlarm alarm;
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalSeconds(1);

	TheAlarmTest(TheAlarmServerSession.AlarmAdd(alarm) == KErrNone, __LINE__);
	const TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);
	// Wait for the alarm to go off, and be paused
	User::After(5 * 1000000);

	// The alarm should still be notifying
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.State() == EAlarmStateNotifying, __LINE__);
	TheAlarmTest(TheAlarmServerSession.AlarmDelete(id) == KErrNone, __LINE__);

	// Add 2 alarms, due in 3 and 13 seconds
	TTime alarm1time;
	TTime alarm2time;
	alarm1time.HomeTime();
	alarm1time += TTimeIntervalSeconds(3);
	alarm2time = alarm1time + TTimeIntervalSeconds(10);

	TASShdAlarm alarm1;
	alarm1.NextDueTime() = alarm1time;
	alarm1.Message() = _L("alarm 1");
	TASShdAlarm alarm2;
	alarm2.NextDueTime() = alarm2time;
	alarm2.Message() = _L("alarm 2");

	TheAlarmTest(TheAlarmServerSession.AlarmAdd(alarm1) == KErrNone, __LINE__);
	const TAlarmId id1 = alarm1.Id();
	TheAlarmTest(id1 != KNullAlarmId, __LINE__);
	TheAlarmTest(TheAlarmServerSession.AlarmAdd(alarm2) == KErrNone, __LINE__);
	const TAlarmId id2 = alarm2.Id();
	TheAlarmTest(id2 != KNullAlarmId, __LINE__);
	//
	// Wait for the 1st alarm to go off, and be paused
	User::After(5 * 1000000);

	// The 1st alarm should still be notifying
	alarm1.Reset();
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id1, alarm1) == KErrNone, __LINE__);
	TheAlarmTest(alarm1.State() == EAlarmStateNotifying, __LINE__);

	// Wait for the time the 2nd alarm is due
	User::After(10 * 1000000);
	// The 1st alarm should be moved to snoozed, so as not to block the 2nd one
	alarm1.Reset();
	alarm2.Reset();
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id1, alarm1) == KErrNone, __LINE__);
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id2, alarm2) == KErrNone, __LINE__);
	TheAlarmTest.Test().Printf(_L("\tALARM1 State => %u \n"),alarm1.State());
	TheAlarmTest.Test().Printf(_L("\tALARM2 State => %u \n"),alarm2.State());
	TheAlarmTest(alarm1.State() == EAlarmStateSnoozed, __LINE__);
	TheAlarmTest(alarm2.State() == EAlarmStateNotified, __LINE__);
	// the 1st alarm was paused, so the next due time is updated to
	// the end of the pause period (1 minute for console alarm alert server)
	testTimesEqual(alarm1.NextDueTime(), alarm1time + TTimeIntervalMinutes(1));
	TheAlarmTest(alarm2.NextDueTime() == alarm2time, __LINE__);

	// tidy up
	TheAlarmTest(TheAlarmServerSession.AlarmDelete(id1) == KErrNone, __LINE__);
	TheAlarmTest(TheAlarmServerSession.AlarmDelete(id2) == KErrNone, __LINE__);
	WaitForNotificationBufferToBeEmptied();
#endif
	}

//

	
static void CreateUtcAlarmsForPausingL()
	{
#ifdef __WINS__
	TestClearStoreL();

	TheAlarmTest.Next(_L("Test pausing alarm sounds"));
	//
	// Add an alarm, due in 3 seconds
	TASShdAlarm alarm;
	TTime time;
	time.UniversalTime();
	time += TTimeIntervalSeconds(3);
	/*
	_LIT(KFormatTime,"%D %H:%T:%S.   ");
	TBuf<128> timeTxt;
	time.FormatL(timeTxt,KFormatTime);
	*/
	alarm.SetUtcNextDueTime(time);
	TheAlarmTest(TheAlarmServerSession.AlarmAdd(alarm) == KErrNone, __LINE__);
	const TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);
	// Wait for the alarm to go off, and be paused
	User::After(5 * 1000000);

	// The alarm should still be notifying
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.State() == EAlarmStateNotifying, __LINE__);
	TheAlarmTest(TheAlarmServerSession.AlarmDelete(id) == KErrNone, __LINE__);

	// Add 2 alarms, due in 3 and 13 seconds
	TTime alarm1time;
	TTime alarm2time;
	alarm1time.UniversalTime();
	alarm1time += TTimeIntervalSeconds(3);
	alarm2time = alarm1time + TTimeIntervalSeconds(10);

	TASShdAlarm alarm1;
	alarm1.SetUtcNextDueTime(alarm1time);
	alarm1.Message()=(_L("alarm 1"));
	TASShdAlarm alarm2;
	alarm2.SetUtcNextDueTime(alarm2time);
	alarm2.Message()=(_L("alarm 2"));

	TheAlarmTest(TheAlarmServerSession.AlarmAdd(alarm1) == KErrNone, __LINE__);
	const TAlarmId id1 = alarm1.Id();
	TheAlarmTest(id1 != KNullAlarmId, __LINE__);
	TheAlarmTest(TheAlarmServerSession.AlarmAdd(alarm2) == KErrNone, __LINE__);
	const TAlarmId id2 = alarm2.Id();
	TheAlarmTest(id2 != KNullAlarmId, __LINE__);
	//
	// Wait for the 1st alarm to go off, and be paused
	User::After(5 * 1000000);

	// The 1st alarm should still be notifying
	alarm1.Reset();
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id1, alarm1) == KErrNone, __LINE__);
	// delete below
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id2, alarm2) == KErrNone, __LINE__);
	TheAlarmTest(alarm1.State() == EAlarmStateNotifying, __LINE__);

	// Wait for the time the 2nd alarm is due
	User::After(10 * 1000000);
	// The 1st alarm should be moved to snoozed, so as not to block the 2nd one
	alarm1.Reset();
	alarm2.Reset();
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id1, alarm1) == KErrNone, __LINE__);
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id2, alarm2) == KErrNone, __LINE__);
	TheAlarmTest.Test().Printf(_L("\tALARM1 State => %u \n"),alarm1.State());
	TheAlarmTest.Test().Printf(_L("\tALARM2 State => %u \n"),alarm2.State());
	TheAlarmTest(alarm1.State() == EAlarmStateSnoozed, __LINE__);
	TheAlarmTest(alarm2.State() == EAlarmStateNotified, __LINE__);
	// the 1st alarm was paused, so the next due time is updated to
	// the end of the pause period (1 minute for console alarm alert server)
	testTimesEqual(alarm1.NextDueTime(), alarm1time + TTimeIntervalMinutes(1));
	TheAlarmTest(alarm2.NextDueTime() == alarm2time, __LINE__);

	// tidy up
	TheAlarmTest(TheAlarmServerSession.AlarmDelete(id1) == KErrNone, __LINE__);
	TheAlarmTest(TheAlarmServerSession.AlarmDelete(id2) == KErrNone, __LINE__);
	WaitForNotificationBufferToBeEmptied();
#endif
	}


static void CreateSingleUtcAlarmL()
	{
#ifdef __WINS__

	
	TheAlarmTest.Next(_L("Adding disabled alarm"));
	//
	TASShdAlarm alarm;
	alarm.Category()=KASCliCategoryClock;
	alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatDaily;
	alarm.Message()=(_L("This is an alarm message - alarm 1"));
	TTime time;
	time.UniversalTime();
	time += TTimeIntervalMinutes(1);
	alarm.SetUtcNextDueTime(time);
	alarm.SoundName()=(_L("Arf Arf Woop"));
	//
	TTime now;
	now.HomeTime();
//	TDateTime nowDT = now.DateTime();
//	TDateTime dueDT = alarm.NextDueTime().DateTime();
	//
	TheAlarmTest(TheAlarmServerSession.AlarmAdd(alarm) == KErrNone, __LINE__);
	const TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);

	//
	
	TheAlarmTest.Next(_L("Get alarm details"));
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone, __LINE__);

	//
	
	
	TheAlarmTest.Next(_L("Enabling a disabled alarm"));
	TheAlarmTest(TheAlarmServerSession.SetAlarmStatus(id, EAlarmStatusEnabled) == KErrNone, __LINE__);

	//
	
	
	TheAlarmTest.Next(_L("Waiting for alarm to expire..."));

	TAlarmId alarmId;
	TRequestStatus status;
	FOREVER
		{
		TheAlarmServerSession.NotifyChange(status, alarmId);
		User::WaitForRequest(status);
		if	(status.Int() == EAlarmChangeEventTimerExpired)
			{
			TheAlarmTest(id == alarmId, __LINE__);
			break;
			}
		}
#endif
	}


static void CreateNotificationUtcAlarmL()
	{
	TheAlarmTest.Next(_L("Creating notification alarm"));

	// AlarmAddWithNotification
	TASShdAlarm alarm;
	TTime time;
	time.UniversalTime();
	time += TTimeIntervalMinutes(1);
	alarm.SetUtcNextDueTime(time);
	TRequestStatus status;
	TheAlarmServerSession.AlarmAddWithNotification(status, alarm);
	TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);
	TheAlarmTest(status == KRequestPending, __LINE__);

	// Wait for completion
	TheAlarmTest(TheAlarmServerSession.SetAlarmStatus(id, EAlarmStatusEnabled) == KErrNone, __LINE__);
	User::WaitForRequest(status);

	// AlarmNotificationCancel
	time.UniversalTime();
	time += TTimeIntervalMinutes(1);
	alarm.SetUtcNextDueTime(time);
	TheAlarmServerSession.AlarmAddWithNotification(status, alarm);
	id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);
	TheAlarmTest(status == KRequestPending, __LINE__);
	TheAlarmServerSession.AlarmNotificationCancelAndDequeue(id);

	// Eat request
	User::WaitForRequest(status);
	TheAlarmTest(status == KErrCancel, __LINE__);
	}

	
static void CreateDailyRepeatingUtcAlarmL()
	{
	TheAlarmTest.Next(_L("Creating daily repeating alarm"));

	// AlarmAddWithNotification
	TASShdAlarm alarm;
	TTime time;
	time.UniversalTime();
	time += TTimeIntervalMinutes(1);
	alarm.SetUtcNextDueTime(time);
	alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatDaily;

	TheAlarmServerSession.AlarmAdd(alarm);
	TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);

	TAlarmId alarmId;
	TRequestStatus status;
	TheAlarmServerSession.NotifyChange(status, alarmId);
	TheAlarmTest(TheAlarmServerSession.SetAlarmStatus(id, EAlarmStatusEnabled) == KErrNone, __LINE__);
	User::WaitForRequest(status);
	}


//*************************************************************************************
// defect INC082426

#if defined(_DEBUG) && defined(__WINS__)
static void CreateAlarmAndSnoozeL(TAlarmRepeatDefinition aRepeatDef)
	{
	TestClearStoreL();

	
	TheAlarmTest.Next(_L("Adding alarm and snooze it"));


	//
	TASShdAlarm alarm;
	alarm.Category() = KASCliCategoryClock;
	alarm.RepeatDefinition() = aRepeatDef;
	alarm.Message() = _L("This is an alarm message - alarm to snooze");

	TTime now1;
	now1.UniversalTime();
	now1 += TTimeIntervalMinutes(1);
	alarm.SetUtcNextDueTime(now1);
	alarm.SoundName() = _L("Hic hic");

	//
	TheAlarmTest(TheAlarmServerSession.AlarmAdd(alarm) == KErrNone, __LINE__);
	const TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);

	
	TheAlarmTest.Next(_L("Get alarm details"));
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone, __LINE__);

	//
	
	TheAlarmTest.Next(_L("Enabling a disabled alarm"));
	TheAlarmTest(TheAlarmServerSession.SetAlarmStatus(id, EAlarmStatusEnabled) == KErrNone, __LINE__);

	
	TheAlarmTest.Next(_L("Waiting for alarm to expire..."));

	TAlarmId alarmId;
	TRequestStatus status;


	FOREVER
		{
		TheAlarmServerSession.NotifyChange(status, alarmId);
		User::WaitForRequest(status);

		if	(status.Int() == EAlarmChangeEventTimerExpired)
			{
			TheAlarmTest(id == alarmId, __LINE__);
			break;
			}
		}

		
	
	TheAlarmTest.Next(_L("Alarm expired"));

	TBuf<100> buf;

	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.State() == EAlarmStateNotifying, __LINE__);
	alarm.OriginalExpiryTime().FormatL(buf, _L("OriginalExpiryTime: %D %H:%T:%S\n"));
	TheAlarmTest.Test().Printf(buf);
	alarm.NextDueTime().FormatL(buf, _L("NextDueTime: %D %H:%T:%S\n"));
	TheAlarmTest.Test().Printf(buf);

	TheAlarmTest.Next(_L("Snooze alarm"));

	TTime snooze(alarm.NextDueTime());
	snooze += TTimeIntervalMinutes(2);
	TInt r = TheAlarmServerSession.__DbgSnoozeAlarm(id, snooze);
	TheAlarmTest(r == KErrNone, __LINE__);


	// check the state is the correct one

	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.State() == EAlarmStateSnoozed, __LINE__);
	alarm.OriginalExpiryTime().FormatL(buf, _L("OriginalExpiryTime: %D %H:%T:%S\n"));
	TheAlarmTest.Test().Printf(buf);
	alarm.NextDueTime().FormatL(buf, _L("NextDueTime: %D %H:%T:%S\n"));
	TheAlarmTest.Test().Printf(buf);

	TheAlarmTest((alarm.OriginalExpiryTime() + TTimeIntervalMinutes(2)) == alarm.NextDueTime(), __LINE__);


	FOREVER
		{
		TheAlarmServerSession.NotifyChange(status, alarmId);
		User::WaitForRequest(status);

		if (status.Int() == EAlarmChangeEventSoundStopped)
			{
			TheAlarmTest(id == alarmId, __LINE__);
			break;
			}
		}


	// check the state is the correct one

	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone, __LINE__);

	alarm.OriginalExpiryTime().FormatL(buf, _L("OriginalExpiryTime: %D %H:%T:%S\n"));
	TheAlarmTest.Test().Printf(buf);
	alarm.NextDueTime().FormatL(buf, _L("NextDueTime: %D %H:%T:%S\n"));
	TheAlarmTest.Test().Printf(buf);

	TheAlarmTest(alarm.State() == EAlarmStateSnoozed, __LINE__);
	TheAlarmTest((alarm.OriginalExpiryTime() + TTimeIntervalMinutes(2)) == alarm.NextDueTime(), __LINE__);

	//wait for the alarm to expire
	FOREVER
		{
		TheAlarmServerSession.NotifyChange(status, alarmId);
		User::WaitForRequest(status);

		if (status.Int() == EAlarmChangeEventTimerExpired)
			{
			TheAlarmTest(id == alarmId, __LINE__);
			break;
			}
		}

	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone, __LINE__);

	alarm.OriginalExpiryTime().FormatL(buf, _L("OriginalExpiryTime: %D %H:%T:%S\n"));
	TheAlarmTest.Test().Printf(buf);
	alarm.NextDueTime().FormatL(buf, _L("NextDueTime: %D %H:%T:%S\n"));
	TheAlarmTest.Test().Printf(buf);

	TheAlarmTest(alarm.State() == EAlarmStateNotifying, __LINE__);
	TheAlarmTest((alarm.OriginalExpiryTime() + TTimeIntervalMinutes(2)) == alarm.NextDueTime(), __LINE__);
	}
#endif

//*************************************************************************************
//
// Using the RepeatDefinition passed in  aRepeatDef,
//
// Create an alarm
// Use NextDueTime() to set the expiry time
// Add alarm to server
// Wait for alarm to expire
// Snooze alarm using __DbgSnoozeAlarm() API
// Retrieve alarm details from server
// Test that the alarm snooze time was set correctly
// Delete the alarm
//
// Repeat this using SetUtcNextDueTime() to set the original
// expiry time
//
// NOTE: __DbgSnoozeAlarm expects time to be passed as UTC,
//       depending how NextDueTime() was set, the time may have to be
//       adjusted by removing the UTC offset
//
// Instigated by DEF083031
//
//*************************************************************************************
#if defined(_DEBUG) && defined(__WINS__)

static void DbgSnoozeAlarmSetsTimeCorrectlyL(TAlarmRepeatDefinition aRepeatDef)
        {
			
			
           TheAlarmTest.Next(_L("Checking DbgSnoozeTime with NextDueTime"));
           TASShdAlarm alarm;
           alarm.RepeatDefinition() = aRepeatDef;
           TTime now;
           now.HomeTime();  //using local time representation

           //      eliminate secs and usecs -- simplifies TTime comparison
           now.RoundUpToNextMinute();

           alarm.NextDueTime() = now;

           //      set alarm time one minute in future
           alarm.NextDueTime()+= TTimeIntervalMinutes(1);

           //      server adds alarm
           AlarmTest(TheAlarmServerSession.AlarmAdd(alarm) == KErrNone);

           //      wait for alarm to expire
           TAlarmId checkID;
           TRequestStatus status;

           FOREVER
           {
              TheAlarmServerSession.NotifyChange(status, checkID);
              User::WaitForRequest(status);
              if (status.Int() == EAlarmChangeEventTimerExpired)
                {
                   AlarmTest(alarm.Id() == checkID);
                   break;
                }
           }

           TTime snoozeTime = alarm.NextDueTime();

           //      set snooze for two minutes in future
           snoozeTime+= TTimeIntervalMinutes(2);

           // *** important bit ***
           //=====================================================
           //  snooze alarm
           //
           //  NOTE:  time passed to __DbgSnoozeAlarm must be UTC
           //  must 'undo' the offset within local time first
           //=====================================================
           AlarmTest(TheAlarmServerSession.__DbgSnoozeAlarm(alarm.Id(), snoozeTime - User::UTCOffset()) == KErrNone);

           //      server's view of alarm retrieved
           AlarmTest(TheAlarmServerSession.GetAlarmDetails(alarm.Id(), alarm) == KErrNone);

           //      check the time for the alarm
           AlarmTest(alarm.NextDueTime() == snoozeTime);

           // cleanup
           AlarmTest(TheAlarmServerSession.AlarmDelete(alarm.Id()) == KErrNone);

		   
           TheAlarmTest.Next(_L("Checking DbgSnoozeTime with SetUtcNextDueTime"));
           now.UniversalTime();   //using UTC time representation
           now.RoundUpToNextMinute();

           alarm.SetUtcNextDueTime(now);

           //      set alarm time one minute in future
           alarm.NextDueTime() += TTimeIntervalMinutes(1);

           //      server adds alarm
           AlarmTest(TheAlarmServerSession.AlarmAdd(alarm) == KErrNone);

           //      wait for alarm to expire
           FOREVER
           {
              TheAlarmServerSession.NotifyChange(status, checkID);
              User::WaitForRequest(status);
              if (status.Int() == EAlarmChangeEventTimerExpired)
                {
                   AlarmTest(alarm.Id() == checkID);
                   break;
                }
          }

          snoozeTime = alarm.NextDueTime();

          //      set snooze for one hour and four minutes in future
          snoozeTime+= TTimeIntervalMinutes(4);
          snoozeTime+= TTimeIntervalHours(1);

          // *** important bit ***
          //=====================================================
          //  snooze alarm
          //
          //  NOTE:  time passed to __DbgSnoozeAlarm must be UTC
          //=====================================================
          AlarmTest(TheAlarmServerSession.__DbgSnoozeAlarm(alarm.Id(), snoozeTime) == KErrNone);

          //      server's view of alarm retrieved
          AlarmTest(TheAlarmServerSession.GetAlarmDetails(alarm.Id(), alarm) == KErrNone);

          //      check the time for the alarm
          AlarmTest(alarm.NextDueTime() == snoozeTime);

          // cleanup
          AlarmTest(TheAlarmServerSession.AlarmDelete(alarm.Id()) == KErrNone);

    }// end DbgSnoozeAlarmSetsTimeCorrectlyL

#endif

static void TestWorkdaysRepeatingAlarmL()
    {
    const TInt KGmtOffset = 3600;//London summer offset
    _LIT(KBaselineTime, "20060416:225900.000000"); //In summer time
    User::SetUTCTimeAndOffset(TTime(KBaselineTime), KGmtOffset);

    TestClearStoreL();

	
    TheAlarmTest.Next(_L("Adding repeating alarm"));

    //
    TASShdAlarm alarm;
    alarm.Category() = KASCliCategoryClock;
    alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatWorkday;
    alarm.Message() = _L("This is an alarm message");

    TTime now1;
    now1.UniversalTime();
    now1 += TTimeIntervalMinutes(2);//step over the midnight
    alarm.SetUtcNextDueTime(now1);
    alarm.SoundName() = _L("Hic hic");

    //
    TheAlarmTest(TheAlarmServerSession.AlarmAdd(alarm) == KErrNone, __LINE__);
    const TAlarmId id = alarm.Id();
    TheAlarmTest(id != KNullAlarmId, __LINE__);

    
    TheAlarmTest.Next(_L("Get alarm details"));
    TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone, __LINE__);

    
    TheAlarmTest.Next(_L("Enabling a disabled alarm"));
    TheAlarmTest(TheAlarmServerSession.SetAlarmStatus(id, EAlarmStatusEnabled) == KErrNone, __LINE__);

    	
    TheAlarmTest.Next(_L("Waiting for alarm to expire..."));

    TAlarmId alarmId;
    TRequestStatus status;


    FOREVER
        {
        TheAlarmServerSession.NotifyChange(status, alarmId);
        User::WaitForRequest(status);
#ifdef __PRINTDEBUGGING__
        PrintEventDetails(status.Int());
#endif
        if    (status.Int() == EAlarmChangeEventTimerExpired)
            {
            TheAlarmTest(id == alarmId, __LINE__);
            break;
            }
        }

		
	
    TheAlarmTest.Next(_L("Alarm expired"));

    TBuf<100> buf;

    TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone, __LINE__);
    TheAlarmTest(alarm.State() == EAlarmStateNotifying, __LINE__);
    TTime dueTime = alarm.NextDueTime();
    alarm.OriginalExpiryTime().FormatL(buf, _L("OriginalExpiryTime: %D %H:%T:%S\n"));
    TheAlarmTest.Test().Printf(buf);
    alarm.NextDueTime().FormatL(buf, _L("NextDueTime: %D %H:%T:%S\n"));
    TheAlarmTest.Test().Printf(buf);

    //Press 'Done'
    TheAlarmServerSession.SetAlarmStatus(alarm.Id(), EAlarmStatusDisabled);
    TheAlarmServerSession.SetAlarmStatus(alarm.Id(), EAlarmStatusEnabled);

    TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone, __LINE__);
    TTime newDueTime = alarm.NextDueTime();

    TheAlarmTest(newDueTime > dueTime);//the alarm was rescheduled

    }

/**
Create an alarm that repeats on given days of the week.
Change the days on which a repeating alarm expires.

@SYMTestCaseID		PIM-APPSRV-ALARMSERVER-WEEKLYGIVENDAYS-0001
										
@SYMTestCaseDesc	To verify that is possible to create an
 					alarm that repeats on the same days every week.
 					
 					To verify that is possible to change the days 
 					on which an alarm that repeats on the same 
 					days every week will expire after it has been
 					created.    

@SYMTestActions 
For PIM-APPSRV-ALARMSERVER-WEEKLYGIVENDAYS-0001:   
1.	Create an alarm that repeats on Tuesday and Wednesday every week. [1]

For PIM-APPSRV-ALARMSERVER-WEEKLYGIVENDAYS-0002:
1.	Create an alarm that repeats on Tuesday and Wednesday every week.
2. 	Update the days on which it expires to Friday. [2]

@SYMTestExpectedResults [1] The alarm is created and expires on the 
							given days each week.
							
						[2] The alarm is updated and expires on the
						 	expected days each week.

@SYMTestType                CT
@SYMTestPriority            1
*/
void TestDailyOnGivenDaysAlarmExpiryL(void)
	{
#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
#if defined(_DEBUG) && defined(__WINS__)
	TheAlarmTest.Next(_L("@SYMTestCaseID PIM-APPSRV-ALARMSERVER-WEEKLYGIVENDAYS-0001   Test 'Daily On Given Days' Alarm Expiry"));

	RANTestClient conAlarmAlertSrvSession;
	User::LeaveIfError(conAlarmAlertSrvSession.Connect());
	User::LeaveIfError(conAlarmAlertSrvSession.SetExtendedMode());
	CleanupClosePushL(conAlarmAlertSrvSession);

	// Set baseline time.  This date/time falls on a Tuesday.
	_LIT(KBaselineTime, "20080000:120055");
	TheAlarmTest(User::SetUTCTimeAndOffset(TTime(KBaselineTime), 0) == KErrNone, __LINE__);

	TTime now;
	now.HomeTime();
	PrintTimeL(now, _L("Current date/time is: "));

	// Create a 'daily on given days' alarm that is active on Tuesday and
	// Wednesday of each week and that expires at 12:01pm (5 seconds from
	// baseline time).
	TASShdAlarm alarm;
	alarm.RepeatDefinition() = EAlarmRepeatDefinitionRepeatDailyOnGivenDays;
	alarm.SetAlarmDays(EAlarmDayTuesday | EAlarmDayWednesday);
	alarm.NextDueTime() = TTime(KBaselineTime) + TTimeIntervalSeconds(5);
	
	TheAlarmTest(TheAlarmServerSession.AlarmAdd(alarm) == KErrNone, __LINE__);
	const TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);

	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone, __LINE__);
	PrintTimeL(alarm.NextDueTime(), _L("Alarm's next due date/time is: "));
	
	TheAlarmTest.Next(_L("Waiting for Tuesday alarm to expire..."));
	TRequestStatus status;
	conAlarmAlertSrvSession.NotifyOnAlarm(alarm, status);
	User::WaitForRequest(status);
	now.HomeTime();
	PrintTimeL(now, _L("Date/time of alarm expiry: "));
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.NextDueTime().DayNoInWeek() == ETuesday);
	conAlarmAlertSrvSession.AcknowledgeAlarm(id);
    
    // Advance baseline time by 1 day to Wednesday.
	TheAlarmTest(User::SetUTCTimeAndOffset(TTime(KBaselineTime) +
		TTimeIntervalDays(1), 0) == KErrNone, __LINE__);
	now.HomeTime();
	PrintTimeL(now, _L("Current date/time is: "));

	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone,
		__LINE__);
	PrintTimeL(alarm.NextDueTime(), _L("Alarm's next due date/time is: "));

	TheAlarmTest.Next(_L("Waiting for Wednesday alarm to expire..."));
	conAlarmAlertSrvSession.NotifyOnAlarm(alarm, status);
	User::WaitForRequest(status);
	now.HomeTime();
	PrintTimeL(now, _L("Date/time of alarm expiry: "));
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone,
			__LINE__);
	TheAlarmTest(alarm.NextDueTime().DayNoInWeek() == EWednesday);
	conAlarmAlertSrvSession.AcknowledgeAlarm(id);

    // Advance baseline time by 7 days to the following Tuesday to check that
	// subsequent repeat of the first active alarm day works. 
	TheAlarmTest(User::SetUTCTimeAndOffset(TTime(KBaselineTime) +
		TTimeIntervalDays(7), 0) == KErrNone, __LINE__);
	now.HomeTime();
	PrintTimeL(now, _L("Current date/time is: "));

	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone,
		__LINE__);
	PrintTimeL(alarm.NextDueTime(), _L("Alarm's next due date/time is: "));

	TheAlarmTest.Next(_L("Waiting for following Tuesday alarm to expire..."));
	conAlarmAlertSrvSession.NotifyOnAlarm(alarm, status);
	User::WaitForRequest(status);
	now.HomeTime();
	PrintTimeL(now, _L("Date/time of alarm expiry: "));
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone,
			__LINE__);
	TheAlarmTest(alarm.NextDueTime().DayNoInWeek() == ETuesday);
	conAlarmAlertSrvSession.AcknowledgeAlarm(id);
	
	// Update alarm's active days to Friday only and check that the next day
	// that the alarm expires on is Friday not Wednesday. 
	TheAlarmTest(TheAlarmServerSession.SetAlarmDays(id, EAlarmDayFriday) ==
	    KErrNone, __LINE__);
	
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone,
		__LINE__);
	PrintTimeL(alarm.NextDueTime(), _L("Alarm's next due date/time is: "));

    // Advance baseline time by 10 days to the following Friday to check that
	// the previous active day of Wednesday does not cause an alarm to expire
	// on that day and the expiry happens on Friday. 
	TheAlarmTest(User::SetUTCTimeAndOffset(TTime(KBaselineTime) +
		TTimeIntervalDays(10), 0) == KErrNone, __LINE__);
	now.HomeTime();
	PrintTimeL(now, _L("Current date/time is: "));

	TheAlarmTest.Next(_L(" @SYMTestCaseID Waiting for Friday's alarm to expire..."));
	conAlarmAlertSrvSession.NotifyOnAlarm(alarm, status);
	User::WaitForRequest(status);
	now.HomeTime();
	PrintTimeL(now, _L("Date/time of alarm expiry: "));
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone,
			__LINE__);
	TheAlarmTest(alarm.NextDueTime().DayNoInWeek() == EFriday);
	conAlarmAlertSrvSession.AcknowledgeAlarm(id);
    
	conAlarmAlertSrvSession.UnsetExtendedMode();
	CleanupStack::PopAndDestroy();
#else
    TheAlarmTest.Test().Printf(_L("THIS TEST IS A DEBUG, WINS TEST ONLY\r\n"));
#endif
#endif
	}

/** Create an alarm that plays continuously.
@SYMTestCaseID		PIM-APPSRV-ALARMSERVER-CONTINUOUS-0001
@SYMTestCaseDesc	To verify that it is possible to 
					create an alarm that plays continuously.    

@SYMTestActions    
1.	Create an alarm that plays continuously.

@SYMTestExpectedResults The alarm plays continuously when it expires.

@SYMTestType                CT
@SYMTestPriority            1
*/
static void TestCreateAndSnoozeContinuousAlarmL()
	{
#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
#if defined(_DEBUG) && defined(__WINS__)
	TheAlarmTest.Next(_L(" @SYMTestCaseID PIM-APPSRV-ALARMSERVER-CONTINUOUS-0001  Test creating and snoozing continuous alarms \r\n"));
	TestClearStoreL();
	
	TASShdAlarm alarm;
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalMinutes(1);
	alarm.SetContinuous(ETrue);
	// Add an alarm due in a minute
	TheAlarmTest.Test().Printf(_L("Add an alarm to go off in one minute \r\n"));
	TheAlarmTest(TheAlarmServerSession.AlarmAdd(alarm) == KErrNone, __LINE__);
	const TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);
	TheAlarmTest.Test().Printf(_L("Check if the alarm is continuous \r\n"));
	TBool isContinuous = alarm.Continuous();
	TheAlarmTest(isContinuous, __LINE__);
	
	// For an existing alarm, verify that continuous state can be obtained and 
	// set.
	TheAlarmTest(TheAlarmServerSession.GetContinuous(id, isContinuous) ==
		KErrNone, __LINE__);
	TheAlarmTest(isContinuous, __LINE__);
	TheAlarmTest(TheAlarmServerSession.SetContinuous(id, EFalse) == KErrNone,
		__LINE__);
	TheAlarmTest(TheAlarmServerSession.GetContinuous(id, isContinuous) ==
		KErrNone, __LINE__);
	TheAlarmTest(!isContinuous, __LINE__);
	TheAlarmTest(TheAlarmServerSession.SetContinuous(id, ETrue) == KErrNone,
		__LINE__);
	
	TTime snoozeTime;
	snoozeTime.HomeTime();
	snoozeTime += TTimeIntervalMinutes(1);
	TheAlarmTest.Test().Printf(_L("Snooze the alarm for a minute and get the alarm details \r\n"));
	TheAlarmTest(TheAlarmServerSession.__DbgSnoozeAlarm(id, snoozeTime) == KErrNone);
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.State() == EAlarmStateSnoozed, __LINE__);
	
	TheAlarmTest.Test().Printf(_L("Delete the alarm from server's list \r\n"));
	TheAlarmTest(TheAlarmServerSession.AlarmDelete(id) == KErrNone, __LINE__);
#else
    TheAlarmTest.Test().Printf(_L("THIS TEST IS A DEBUG, WINS TEST ONLY\r\n"));
#endif
#endif
	}

/** Alarm that expires when a continuous alarm is played.
@SYMTestCaseID		PIM-APPSRV-ALARMSERVER-CONTINUOUS-0002
@SYMTestCaseDesc	To verify that the alarm that expires
 					when the continuous alarm is playing
 					will be played when the continuous 
 					alarm is stopped.

@SYMTestActions    
1.	Create an alarm that plays continuously.
2. 	Create an alarm that expires 1 minute after the continuous alarm

@SYMTestExpectedResults The second alarm will play after the continuous
 						alarm is stopped.

@SYMTestType                CT
@SYMTestPriority            1
*/

static void TestTwoAlarmsExpirySequenceL()
	{
#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
#if defined(_DEBUG) && defined(__WINS__)
	TheAlarmTest.Next(_L(" @SYMTestCaseID PIM-APPSRV-ALARMSERVER-CONTINUOUS-0002  Test expiry of two alarms, one being continuous"));
	TestClearStoreL();
	
	RANTestClient consoleAlarmAlertSrvSession;
	User::LeaveIfError(consoleAlarmAlertSrvSession.Connect());
	User::LeaveIfError(consoleAlarmAlertSrvSession.SetExtendedMode());
	CleanupClosePushL(consoleAlarmAlertSrvSession);

	TASShdAlarm alarmContinuous;
	alarmContinuous.NextDueTime().HomeTime();
	alarmContinuous.NextDueTime() += TTimeIntervalSeconds(30);
	alarmContinuous.SetContinuous(ETrue);
	
	TheAlarmTest(TheAlarmServerSession.AlarmAdd(alarmContinuous) == KErrNone, __LINE__);
	TAlarmId idContinuous = alarmContinuous.Id();
	TheAlarmTest(idContinuous != KNullAlarmId, __LINE__);
	
	TASShdAlarm alarm;
	alarm.NextDueTime().HomeTime();	
	alarm.NextDueTime() += TTimeIntervalSeconds(30);
	TheAlarmTest(TheAlarmServerSession.AlarmAdd(alarm) == KErrNone, __LINE__);
	TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);
	
	TheAlarmTest(alarmContinuous.State() == EAlarmStateQueued);
	TheAlarmTest(alarm.State() == EAlarmStateQueued);
	
	TheAlarmTest.Test().Printf(_L("Wait for the first alarm to expire \r\n"));
	TRequestStatus status;
	consoleAlarmAlertSrvSession.NotifyOnAlarm(alarm, status);
	User::WaitForRequest(status);
	
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(idContinuous, alarmContinuous) == KErrNone, __LINE__);
	TheAlarmTest(alarmContinuous.State() == EAlarmStateNotifying);
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.State() == EAlarmStateWaitingToNotify);
	
	TheAlarmTest.Test().Printf(_L("Acknowledge the first alarm \r\n"));
	consoleAlarmAlertSrvSession.AcknowledgeAlarm(idContinuous);
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(idContinuous, alarmContinuous) == KErrNone, __LINE__);
	TheAlarmTest(alarmContinuous.State() == EAlarmStateNotified);
	TheAlarmTest.Test().Printf(_L("The second alarm should immediately expire, check the alarm details \r\n"));
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.State() == EAlarmStateNotifying);
	
	consoleAlarmAlertSrvSession.AcknowledgeAlarm(id);
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.State() == EAlarmStateNotified);
		
	consoleAlarmAlertSrvSession.UnsetExtendedMode();
	CleanupStack::PopAndDestroy();
	TestClearStoreL();
#else
    TheAlarmTest.Test().Printf(_L("THIS TEST IS A DEBUG, WINS TEST ONLY\r\n"));
#endif
#endif
	}


static void TestDoNotNotifyAlarmAlertServerL()
	{
	TheAlarmTest.Test().Next(_L("Test that EAlarmCharacteristicsDoNotNotifyAlarmAlertServer do not panic the alarm server when they expire"));
	
	TheAlarmTest.Test().Printf(_L("Set the system time to 1 second before the alarm time"));
	TTime alarmTime(TTime(TDateTime(2008, EApril, 1, 12, 0, 0, 0)));
	User::SetHomeTime(alarmTime - TTimeIntervalSeconds(1));
	
	TheAlarmTest.Test().Printf(_L("Add an alarm with EAlarmCharacteristicsDoNotNotifyAlarmAlertServer set"));
	TASShdAlarm alarm;
	alarm.Message() = _L("Do not notify the alarm alert server!!!");
	alarm.NextDueTime() = alarmTime;
	alarm.Characteristics().Set(EAlarmCharacteristicsDoNotNotifyAlarmAlertServer);
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	
	User::LeaveIfError(TheAlarmServerSession.AlarmAdd(alarm));
	
	TheAlarmTest.Test().Printf(_L("wait until the alarm sounds starts playing"));
	FOREVER
		{
		TRequestStatus status;
		TAlarmId id;
		TheAlarmServerSession.NotifyChange(status, id);
		User::WaitForRequest(status);
		
		TheAlarmTest.Test().Printf(_L("alarms event = %d\n"), status.Int());
		User::LeaveIfError(status.Int());
		
		if (status.Int() == EAlarmChangeEventSoundPlaying)
			{
			// the sound is now playing which is where the panic would have happened
			// so we can stop waiting now
			break;
			}
		}
	
	TheAlarmTest.Test().Printf(_L("delete the alarm"));
	
	// delete the alarm
	User::LeaveIfError(TheAlarmServerSession.AlarmDelete(alarm.Id()));
	}

//*************************************************************************************
	
	
static void RunTestsL()
	{
	//get the initial time
 	TTime homeTime;
	homeTime.HomeTime();

	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

    RPIMTestServer serv;
    User::LeaveIfError(serv.Connect());
    serv.DeleteFileL(KTestIniFile);

	// Set time before starting Alarm Server
	const TInt KGmtOffset = 3600;
	_LIT(KBaselineTime, "20040730:050000.000000"); //In summer time
	TInt err = User::SetUTCTimeAndOffset(TTime(KBaselineTime), KGmtOffset);
	TheAlarmTest(err == KErrNone, __LINE__);
	
#ifdef __WINS__
	_LIT(KConsoleAlarmAlertServerImg,"ConsoleAlarmAlertServer");
    TBuf<256> cmdline;
	cmdline.Append('p');
	cmdline.Append('p');
	cmdline.Append(EKeyEscape);
	cmdline.Append(0);
	cmdline.Append('p');
	cmdline.Append('p');
	cmdline.Append(EKeyEscape);
	cmdline.Append(0);
	RProcess server;
	server.Create(KConsoleAlarmAlertServerImg,cmdline,serverUid);
	TRequestStatus stat;
	server.Rendezvous(stat);
	if (stat!=KRequestPending)
		{
		server.Kill(0);		// abort startup
		}
	else
		{
		server.Resume();	// logon OK - start the server
		}
#endif //  __WINS__
	// Connect to the alarm server
	TheAlarmTest.Next(_L("Creating alarm server"));
	User::LeaveIfError(TheAlarmServerSession.Connect());
	
	TestDailyOnGivenDaysAlarmExpiryL();
	TestCreateAndSnoozeContinuousAlarmL();
	TestTwoAlarmsExpirySequenceL();
	
	// The real tests
    // The following 2 tests are only for WINS and WINSCW for console interface

	CreateAlarmsForPausingL();
	CreateSingleAlarmL();
	CreateNotificationAlarmL();
	CreateDailyRepeatingAlarmL();

	CreateUtcAlarmsForPausingL();
	CreateSingleUtcAlarmL();
	CreateNotificationUtcAlarmL();
	CreateDailyRepeatingUtcAlarmL();

	CreateAlarmAndIgnoreL();
    TestWorkdaysRepeatingAlarmL();

#if defined(_DEBUG) && defined(__WINS__)

	CreateAlarmAndSnoozeL(EAlarmRepeatDefintionRepeatDaily);
	CreateAlarmAndSnoozeL(EAlarmRepeatDefintionRepeatWorkday);
	CreateAlarmAndSnoozeL(EAlarmRepeatDefintionRepeatWeekly);

	DbgSnoozeAlarmSetsTimeCorrectlyL(EAlarmRepeatDefintionRepeatOnce);
	DbgSnoozeAlarmSetsTimeCorrectlyL(EAlarmRepeatDefintionRepeatNext24Hours);
	DbgSnoozeAlarmSetsTimeCorrectlyL(EAlarmRepeatDefintionRepeatDaily);
	DbgSnoozeAlarmSetsTimeCorrectlyL(EAlarmRepeatDefintionRepeatWorkday);
	DbgSnoozeAlarmSetsTimeCorrectlyL(EAlarmRepeatDefintionRepeatWeekly);

#endif //  __WINS__

	TestINC092682L();
	TestDoNotNotifyAlarmAlertServerL();

	TestClearStoreL();

	//Set back the time to the initial time
    serv.SetHomeTime(homeTime);
    serv.Close();
 	CleanupStack::PopAndDestroy(scheduler);
	}




//
// -----> Global Exports
//

//*************************************************************************************
	
/**
@SYMTestCaseID PIM-TBASICALARMS-0001
*/	
TInt E32Main()
	{
	__UHEAP_MARK;
	//
	TInt ret = KErrNone;
	TheAlarmTest.Title();
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if	(!cleanup)
	    {
	    return KErrNoMemory;
	    }
	//
	TheAlarmTest.Start(_L("@SYMTestCaseID PIM-TBASICALARMS-0001"));
	//
	TRAPD(err, RunTestsL());
	TheAlarmTest(err == KErrNone, __LINE__);

	delete cleanup;
    TRAP(ret,TheAlarmTest.EndL());
	TheAlarmTest.Test().Close();
	ASTstAlarmTest::Close();

	__UHEAP_MARKEND;
	return ret;
	}
