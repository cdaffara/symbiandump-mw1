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

// Globals
static TRequestStatus TheStatus;
static TTime TheTimeBefore;
static TTime TheTimeAfter;
static TAlarmId TheClockAlarmIds[10];


void TestInitAlarmsL()
//
//	Set up alarms for before test
//	1.	clock 0 between shutdown time and restart time
//	2.	clock 1 daily alarm
//	3.	clock 2 workday alarm
//	4.	clock 3 after restart time
//	5.	orphaned alarm
//	6.	session alarm
//	7.	clock 4 in 24 hours
//	8.	clock 5 in past (for review alarm)
//
	{
	TheAlarmTest.Test().Next(_L("Setting up alarms before shut down"));
	//
	TInt r;
	TInt count;
	TASShdAlarm alarm;
	TheTimeBefore.HomeTime();
	//
	count = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(count == 0, __LINE__);

	//	8. Clock 0 in past (for review alarm)
	TheTimeBefore -= TTimeIntervalHours(1);
	alarm.NextDueTime() = TheTimeBefore;
	alarm.Message() = _L("past");
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheClockAlarmIds[0] = alarm.Id();
	//
	User::After(KTimeToWait);
	r = TheAlarmTest.Session().GetAlarmDetails(alarm.Id(), alarm);
	TheAlarmTest(alarm.State() == EAlarmStateNotified, __LINE__);
	//
	count = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(count == 0, __LINE__);

	//	1. Clock 1 between shutdown time and restart time
	TheTimeBefore += TTimeIntervalDays(1);
	alarm.NextDueTime() = TheTimeBefore;
	alarm.Message() = _L("once");
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheClockAlarmIds[1] = alarm.Id();
	//
	r = TheAlarmTest.Session().GetAlarmDetails(alarm.Id(), alarm);
	TheAlarmTest(alarm.State() == EAlarmStateQueued, __LINE__);

	//	2. Clock 2 daily alarm
	alarm.NextDueTime() = TheTimeBefore;
	alarm.Message() = _L("daily");
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatDaily;
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheClockAlarmIds[2] = alarm.Id();
	//
	count = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(count == 2, __LINE__);

	//	3. Clock 3 workday alarm
	alarm.NextDueTime() = TheTimeBefore;
	alarm.Message() = _L("workday");
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatWorkday;
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheClockAlarmIds[3] = alarm.Id();

	//	4. Clock 4 after restart time
	TheTimeAfter = TheTimeBefore + TTimeIntervalDays(2);
	alarm.NextDueTime() = TheTimeAfter;
	alarm.Message() = _L("after");
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatWorkday;
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheClockAlarmIds[4] = alarm.Id();
	//
	count = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(count == 4, __LINE__);

	//	5. Orphaned alarm
	alarm.NextDueTime() = TheTimeAfter;
	alarm.Message() = _L("orphaned");
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	TheAlarmTest.Session().AlarmAddWithNotification(TheStatus, alarm);
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	TheAlarmTest(TheStatus == KRequestPending, __LINE__);
	r = TheAlarmTest.Session().SetAlarmCharacteristics(alarm.Id(), 0);
	TheAlarmTest(r == KErrNone, __LINE__);
	User::WaitForRequest(TheStatus);
	TheClockAlarmIds[5] = alarm.Id();
	//
	count = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(count == 5, __LINE__);

	//	7. Clock 5 in 24 hours
	alarm.Characteristics() = 0;
	alarm.NextDueTime() = TheTimeAfter;
	alarm.Message() = _L("next24");
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatNext24Hours;
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheClockAlarmIds[6] = alarm.Id();
	//
	count = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(count == 6, __LINE__);

	//	6. Session alarm
	alarm.NextDueTime() = TheTimeAfter;
	alarm.Message() =_L("different");
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	TheAlarmTest.Session().AlarmAddWithNotification(TheStatus, alarm);
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	TheAlarmTest(TheStatus == KRequestPending, __LINE__);
	TheClockAlarmIds[7] = alarm.Id();

	// Verification
	count = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(count == 7, __LINE__);
	//
	count = TheAlarmTest.CountOrphanedAlarmsL();
	TheAlarmTest(count == 1, __LINE__);
	}

	
void TestPostShutDownL()
//
//	Check persisted correct information
//	1.	Clock 0 alarm deleted - its added to the queue when the backup store
//		is internalized, but as soon as the server starts the scheduler, the
//		change notifier runs and the date/time change is reported. At this point
//		the alarm is more than 2 days in the past, and as such, any "Once only"
//		alarm (regardless of State()) which is set in the past is deleted from
//		the queue.
//	2.	Clock 1 set between shutdown and restart time. Again this is deleted
//		because when the change notifier runs, its more than a day old and
//		so its automatically dequeued.
//	3.	Clock 2 (daily repeat). This is just reset for the next valid repeat.
//	4.	Clock 3 (workday repeat). This is just reset for the next valid repeat.
//	5.	Clock 4 (workday repeat). This still uses the original "after the restart"
//		expiry time.
//	6.	Alarm 5 (orphaned alarm). This is still orphaned and is still valid.
//		Effectively, this is a change in behaviour from the old EALWL, where
//		old orphaned alarms were deleted when internalized.
//	7.	Clock 5 (was next 24 hours repeat, is now RepeatOnce and orphaned).
//	8.	Session alarm (which was pending when the server shut down). This is
//		cleaned up and dequeued as part of the server shut-down process. When
//		the session disconnects, any session alarms for that session are
//		dequeued (by matching against the session id).
//
	{
	TheAlarmTest.Test().Next(_L("Comparing persisted alarms"));
	//
	TInt c;
	c = TheAlarmTest.CountOrphanedAlarmsL();
	TheAlarmTest(c == 1, __LINE__);
	//
	c = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(c == 4, __LINE__);
	//
	TInt r;
	TASShdAlarm alarm;
	//
	r = TheAlarmTest.Session().GetAlarmDetails(TheClockAlarmIds[0], alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	r = TheAlarmTest.Session().GetAlarmDetails(TheClockAlarmIds[1], alarm);
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().GetAlarmDetails(TheClockAlarmIds[2], alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarm.Status() == EAlarmStatusEnabled, __LINE__);
	TheAlarmTest(alarm.Message() == _L("daily"), __LINE__);
	TheAlarmTest(alarm.RepeatDefinition() == EAlarmRepeatDefintionRepeatDaily, __LINE__);
	//
	r = TheAlarmTest.Session().GetAlarmDetails(TheClockAlarmIds[3], alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarm.Status() == EAlarmStatusEnabled, __LINE__);
	TheAlarmTest(alarm.Message() == _L("workday"), __LINE__);
	TheAlarmTest(alarm.RepeatDefinition() == EAlarmRepeatDefintionRepeatWorkday, __LINE__);
	//
	r = TheAlarmTest.Session().GetAlarmDetails(TheClockAlarmIds[4], alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarm.Status() == EAlarmStatusEnabled, __LINE__);
	TheAlarmTest(alarm.Message() == _L("after"), __LINE__);
	TheAlarmTest(alarm.RepeatDefinition() == EAlarmRepeatDefintionRepeatWorkday, __LINE__);
	//
	r = TheAlarmTest.Session().GetAlarmDetails(TheClockAlarmIds[5], alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarm.Status() == EAlarmStatusEnabled, __LINE__);
	TheAlarmTest(alarm.Message() == _L("orphaned"), __LINE__);
	TheAlarmTest(alarm.HasBecomeOrphaned(), __LINE__);
	TheAlarmTest(alarm.RepeatDefinition() == EAlarmRepeatDefintionRepeatOnce, __LINE__);
	//
	r = TheAlarmTest.Session().GetAlarmDetails(TheClockAlarmIds[6], alarm);
	TheAlarmTest(r == KErrNotFound, __LINE__);
	TheAlarmTest(alarm.Status() == EAlarmStatusEnabled, __LINE__);
	TheAlarmTest(alarm.Message() == _L("orphaned"), __LINE__);
	TheAlarmTest(alarm.RepeatDefinition() == EAlarmRepeatDefintionRepeatOnce, __LINE__);
	}

	
void TestInitUtcAlarmsL()
//
//	Set up alarms for before test
//	1.	clock 0 between shutdown time and restart time
//	2.	clock 1 daily alarm
//	3.	clock 2 workday alarm
//	4.	clock 3 after restart time
//	5.	orphaned alarm
//	6.	session alarm
//	7.	clock 4 in 24 hours
//	8.	clock 5 in past (for review alarm)
//
	{
	TheAlarmTest.Test().Next(_L("Setting up UTC alarms before shut down"));
	//
	TInt r;
	TInt count;
	TASShdAlarm alarm;
	TheTimeBefore.UniversalTime();
	//
	count = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(count == 0, __LINE__);

	//	8. Clock 0 in past (for review alarm)
	TheTimeBefore -= TTimeIntervalHours(1);
	alarm.SetUtcNextDueTime(TheTimeBefore);
	alarm.Message()=(_L("past"));
	alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatOnce;
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheClockAlarmIds[0] = alarm.Id();
	//
	User::After(KTimeToWait);
	r = TheAlarmTest.Session().GetAlarmDetails(alarm.Id(), alarm);
	TheAlarmTest(alarm.State() == EAlarmStateNotified, __LINE__);
	//
	count = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(count == 0, __LINE__);

	//	1. Clock 1 between shutdown time and restart time
	TheTimeBefore += TTimeIntervalDays(1);
	alarm.SetUtcNextDueTime(TheTimeBefore);
	alarm.Message()=(_L("once"));
	alarm.RepeatDefinition()=(EAlarmRepeatDefintionRepeatOnce);
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheClockAlarmIds[1] = alarm.Id();
	//
	r = TheAlarmTest.Session().GetAlarmDetails(alarm.Id(), alarm);
	TheAlarmTest(alarm.State() == EAlarmStateQueued, __LINE__);

	//	2. Clock 2 daily alarm
	alarm.SetUtcNextDueTime(TheTimeBefore);
	alarm.Message()=(_L("daily"));
	alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatDaily;
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheClockAlarmIds[2] = alarm.Id();
	//
	count = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(count == 2, __LINE__);

	//	3. Clock 3 workday alarm
	alarm.SetUtcNextDueTime(TheTimeBefore);
	alarm.Message()=(_L("workday"));
	alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatWorkday;
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheClockAlarmIds[3] = alarm.Id();

	//	4. Clock 4 after restart time
	TheTimeAfter = TheTimeBefore + TTimeIntervalDays(2);
	alarm.SetUtcNextDueTime(TheTimeAfter);
	alarm.Message()=(_L("after"));
	alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatWorkday;
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheClockAlarmIds[4] = alarm.Id();
	//
	count = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(count == 4, __LINE__);

	//	5. Orphaned alarm
	alarm.SetUtcNextDueTime(TheTimeAfter);
	alarm.Message()=(_L("orphaned"));
	alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatOnce;
	TheAlarmTest.Session().AlarmAddWithNotification(TheStatus, alarm);
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	TheAlarmTest(TheStatus == KRequestPending, __LINE__);
	r = TheAlarmTest.Session().SetAlarmCharacteristics(alarm.Id(), 0);
	TheAlarmTest(r == KErrNone, __LINE__);
	User::WaitForRequest(TheStatus);
	TheClockAlarmIds[5] = alarm.Id();
	//
	count = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(count == 5, __LINE__);

	//	7. Clock 5 in 24 hours
	alarm.Reset();
	alarm.Characteristics().ClearAll();
	alarm.SetUtcNextDueTime(TheTimeAfter);
	alarm.Message()=(_L("next24"));
	alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatNext24Hours;
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheClockAlarmIds[6] = alarm.Id();
	//
	count = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(count == 6, __LINE__);

	//	6. Session alarm
	alarm.SetUtcNextDueTime(TheTimeAfter);
	alarm.Message()=(_L("different"));
	alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatOnce;
	TheAlarmTest.Session().AlarmAddWithNotification(TheStatus, alarm);
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	TheAlarmTest(TheStatus == KRequestPending, __LINE__);
	TheClockAlarmIds[7] = alarm.Id();

	// Verification
	count = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(count == 7, __LINE__);
	//
	count = TheAlarmTest.CountOrphanedAlarmsL();
	TheAlarmTest(count == 1, __LINE__);
	}


void TestPostShutDownUtcL()
//
//	Check persisted correct information
//	1.	Clock 0 alarm deleted - its added to the queue when the backup store
//		is internalized, but as soon as the server starts the scheduler, the
//		change notifier runs and the date/time change is reported. At this point
//		the alarm is more than 2 days in the past, and as such, any "Once only"
//		alarm (regardless of State()) which is set in the past is deleted from
//		the queue.
//	2.	Clock 1 set between shutdown and restart time. Again this is deleted
//		because when the change notifier runs, its more than a day old and
//		so its automatically dequeued.
//	3.	Clock 2 (daily repeat). This is just reset for the next valid repeat.
//	4.	Clock 3 (workday repeat). This is just reset for the next valid repeat.
//	5.	Clock 4 (workday repeat). This still uses the original "after the restart"
//		expiry time.
//	6.	Alarm 5 (orphaned alarm). This is still orphaned and is still valid.
//		Effectively, this is a change in behaviour from the old EALWL, where
//		old orphaned alarms were deleted when internalized.
//	7.	Clock 5 (was next 24 hours repeat, is now RepeatOnce and orphaned).
//	8.	Session alarm (which was pending when the server shut down). This is
//		cleaned up and dequeued as part of the server shut-down process. When
//		the session disconnects, any session alarms for that session are
//		dequeued (by matching against the session id).
//
	{
	TheAlarmTest.Test().Next(_L("Comparing persisted UTC alarms"));
	//
	TInt c;
	c = TheAlarmTest.CountOrphanedAlarmsL();
	TheAlarmTest(c == 1, __LINE__);
	//
	c = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(c == 4, __LINE__);
	//
	TInt r;
	TASShdAlarm alarm;
	//
	r = TheAlarmTest.Session().GetAlarmDetails(TheClockAlarmIds[0], alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	r = TheAlarmTest.Session().GetAlarmDetails(TheClockAlarmIds[1], alarm);
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().GetAlarmDetails(TheClockAlarmIds[2], alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarm.Status() == EAlarmStatusEnabled, __LINE__);
	TheAlarmTest(alarm.Message() == _L("daily"), __LINE__);
	TheAlarmTest(alarm.RepeatDefinition() == EAlarmRepeatDefintionRepeatDaily, __LINE__);
	//
	r = TheAlarmTest.Session().GetAlarmDetails(TheClockAlarmIds[3], alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarm.Status() == EAlarmStatusEnabled, __LINE__);
	TheAlarmTest(alarm.Message() == _L("workday"), __LINE__);
	TheAlarmTest(alarm.RepeatDefinition() == EAlarmRepeatDefintionRepeatWorkday, __LINE__);
	//
	r = TheAlarmTest.Session().GetAlarmDetails(TheClockAlarmIds[4], alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarm.Status() == EAlarmStatusEnabled, __LINE__);
	TheAlarmTest(alarm.Message() == _L("after"), __LINE__);
	TheAlarmTest(alarm.RepeatDefinition() == EAlarmRepeatDefintionRepeatWorkday, __LINE__);
	//
	r = TheAlarmTest.Session().GetAlarmDetails(TheClockAlarmIds[5], alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarm.Status() == EAlarmStatusEnabled, __LINE__);
	TheAlarmTest(alarm.Message() == _L("orphaned"), __LINE__);
	TheAlarmTest(alarm.HasBecomeOrphaned(), __LINE__);
	TheAlarmTest(alarm.RepeatDefinition() == EAlarmRepeatDefintionRepeatOnce, __LINE__);
	//
	r = TheAlarmTest.Session().GetAlarmDetails(TheClockAlarmIds[6], alarm);
	TheAlarmTest(r == KErrNotFound, __LINE__);
	TheAlarmTest(alarm.Status() == EAlarmStatusEnabled, __LINE__);
	TheAlarmTest(alarm.Message() == _L("orphaned"), __LINE__);
	TheAlarmTest(alarm.RepeatDefinition() == EAlarmRepeatDefintionRepeatOnce, __LINE__);
	}

static void DoTestsL()
	{
	CActiveScheduler* scheduler = new(ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

#if defined(__WINS__) && defined(_DEBUG)
	RPIMTestServer pimServ;
	User::LeaveIfError(pimServ.Connect());
#endif

#ifdef __WINS__
	// This instance of TASShdAlarm is needed to avoid a link error under wins/urel
	// It's not actually used for anything
	TASShdAlarm dummy;
#endif  // __WINS__

	
	TheAlarmTest.Next(_L("Connecting to server"));
	TInt r = TheAlarmTest.Session().Connect();
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	TheAlarmTest.TestClearStoreL();
	TInt count = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(count == 0, __LINE__);
	//
#if !defined(_DEBUG)
	TheAlarmTest.Test().Printf(_L("THIS TEST IS A DEBUG TEST ONLY\n"));
#else   // _DEBUG
#ifdef __WINS__
	TheAlarmTest.Session().__DbgPreventUserNotify(ETrue);
	TestInitAlarmsL();
	
		
	TheAlarmTest.Test().Next(_L("Closing Alarm Server"));
	TheAlarmTest.Session().__DbgShutDownServer();
    TheAlarmTest.Session().Close();
	//
	TTime newTime;
	newTime.HomeTime();
	newTime += TTimeIntervalDays(2);
    User::LeaveIfError(pimServ.SetHomeTime(newTime));

	User::After(KTimeToWait);
	//
	
	
	TheAlarmTest.Next(_L("Restarting Alarm Server"));
	TheAlarmTest.TestStartServers();
	r=TheAlarmTest.Session().Connect();
	TheAlarmTest(r==KErrNone, __LINE__);
	//
	TestPostShutDownL();
#endif  // __WINS__
#endif  // _DEBUG

	
	TheAlarmTest.Next(_L("Connecting to server"));
	r = TheAlarmTest.Session().Connect();
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	TheAlarmTest.TestClearStoreL();
	count = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(count == 0, __LINE__);

	//
#if !defined(_DEBUG)
	TheAlarmTest.Test().Printf(_L("THIS TEST IS A DEBUG TEST ONLY\n"));
#else   // _DEBUG
#ifdef __WINS__
	TheAlarmTest.Session().__DbgPreventUserNotify(ETrue);
	TestInitUtcAlarmsL();
	
	TheAlarmTest.Test().Next(_L("Closing Alarm Server"));
	TheAlarmTest.Session().__DbgShutDownServer();
    TheAlarmTest.Session().Close();
	//
	newTime.HomeTime();
	newTime += TTimeIntervalDays(2);

    pimServ.SetHomeTime(newTime);

	User::After(KTimeToWait);
	//
	
	TheAlarmTest.Next(_L("Restarting Alarm Server"));
	TheAlarmTest.TestStartServers();
	r=TheAlarmTest.Session().Connect();
	TheAlarmTest(r==KErrNone, __LINE__);
	//
	TestPostShutDownUtcL();
#endif  // __WINS__
#endif  // _DEBUG

	TheAlarmTest.TestClearStoreL();
	TheAlarmTest.Session().Close();

#if defined(__WINS__) && defined(_DEBUG)
    pimServ.Close();
#endif

	CleanupStack::PopAndDestroy(scheduler);
	}

/**
@SYMTestCaseID PIM-TQUEUERESTORATION-0001
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

	TheAlarmTest.Title();
	TheAlarmTest.Start(_L("@SYMTestCaseID PIM-TQUEUERESTORATION-0001 TQueueRestoration"));
	TheAlarmTest.TestStartServers();
	//
	TRAPD(error, DoTestsL());
	TheAlarmTest(error == KErrNone, __LINE__);
	//
    TRAP(ret,TheAlarmTest.EndL());
    	TheAlarmTest.Test().Close();
	ASTstAlarmTest::Close();

	delete cleanup;
	__UHEAP_MARKEND;
	return ret;
    }
