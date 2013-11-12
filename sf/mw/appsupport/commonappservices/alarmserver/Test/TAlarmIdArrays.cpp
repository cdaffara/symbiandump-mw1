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
#if defined(__WINS__)
static TInt TheNumberOfReviewAlarms=0;
#endif  // __WINS__


static void testSetAlarmsL()
//
//	Set alarms in future and past
//		3 orphan alarms (one, three, four)
//		1 clock alarm (two)
//		1 session alarm (five)
//		1 review alarm (six, clock)
//	This test is only performed when simulating, i.e. it is not active on hardware
//  The reason is that we can not give any instruction set to the alarm alert server on HW
	{
#if defined(__WINS__)
	TheAlarmTest.Test().Next(_L("Set orphan review clock and session alarms"));
	TheAlarmTest(TheAlarmTest.CountOrphanedAlarmsL() == 0, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().AlarmCountByState(EAlarmStateSnoozed) == 0, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().AlarmCountByState(EAlarmStateNotified) == TheNumberOfReviewAlarms, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue() == 0, __LINE__);
	//
	TInt r;
	TAlarmId id;
	TASShdAlarm alarm;
	TASShdAlarm alarmInfo;
	//
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalDays(1);
	alarm.Message() = _L("one");
	alarm.Characteristics() = EAlarmCharacteristicsSessionSpecific;
	TheAlarmTest.Session().AlarmAddWithNotification(TheStatus, alarm);
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	//
	r = TheAlarmTest.Session().GetNextDueAlarmId(id);
	TheAlarmTest(id == alarm.Id(), __LINE__);
	r = TheAlarmTest.Session().GetAlarmDetails(id, alarmInfo);
	TheAlarmTest(r==KErrNone, __LINE__);
	TheAlarmTest(alarmInfo.Message() == alarm.Message(), __LINE__);
	//
	TASShdAlarm alarm2;
	alarm2.NextDueTime().HomeTime();
	alarm2.NextDueTime() += TTimeIntervalDays(1);
	alarm2.NextDueTime() += TTimeIntervalMinutes(1);
	alarm2.Message() = _L("two");
	alarm2.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	r = TheAlarmTest.Session().AlarmAdd(alarm2);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarm2.Id() != KNullAlarmId, __LINE__);
	//
	r = TheAlarmTest.Session().GetNextDueAlarmId(id);
	TheAlarmTest(id == alarm.Id(), __LINE__);
	r = TheAlarmTest.Session().GetAlarmDetails(id, alarmInfo);
	TheAlarmTest(r==KErrNone, __LINE__);
	TheAlarmTest(alarmInfo.Message() == alarm.Message(), __LINE__);
	//
	r = TheAlarmTest.Session().SetAlarmCharacteristics(alarm.Id(), 0);
	TheAlarmTest(r==KErrNone, __LINE__);
	TheAlarmTest(TheStatus != KRequestPending, __LINE__);
	User::WaitForRequest(TheStatus);
	//
	r = TheAlarmTest.Session().GetNextDueAlarmId(id);
	TheAlarmTest(id == alarm.Id(), __LINE__);
	r = TheAlarmTest.Session().GetAlarmDetails(id, alarmInfo);
	TheAlarmTest(r==KErrNone, __LINE__);
	TheAlarmTest(alarmInfo.Message() == alarm.Message(), __LINE__);
	//
	alarm.NextDueTime() += TTimeIntervalDays(1);
	alarm.Message() = _L("three");
	alarm.Characteristics() = EAlarmCharacteristicsSessionSpecific;
	//
	TheAlarmTest.Session().AlarmAddWithNotification(TheStatus, alarm);
	TheAlarmTest(alarm.Id() != KErrNone, __LINE__);
	r = TheAlarmTest.Session().SetAlarmCharacteristics(alarm.Id(), 0);
	TheAlarmTest(r==KErrNone, __LINE__);
	TheAlarmTest(TheStatus != KRequestPending, __LINE__);
	User::WaitForRequest(TheStatus);
	//
	alarm.NextDueTime() += TTimeIntervalDays(1);
	alarm.Message() = _L("four");
	alarm.Characteristics() = EAlarmCharacteristicsSessionSpecific;
	//
	TheAlarmTest.Session().AlarmAddWithNotification(TheStatus, alarm);
	TheAlarmTest(alarm.Id() != KErrNone, __LINE__);
	r = TheAlarmTest.Session().SetAlarmCharacteristics(alarm.Id(), 0);
	TheAlarmTest(r==KErrNone, __LINE__);
	TheAlarmTest(TheStatus != KRequestPending, __LINE__);
	User::WaitForRequest(TheStatus);
	//
	alarm.Characteristics() = EAlarmCharacteristicsSessionSpecific;
	alarm.NextDueTime() += TTimeIntervalDays(1);
	alarm.NextDueTime() += TTimeIntervalMinutes(1);
	alarm.Message() = _L("five");
	TheAlarmTest.Session().AlarmAddWithNotification(TheStatus, alarm);
	const TAlarmId sessionAlarmId = alarm.Id();
	//
	TheAlarmTest(TheAlarmTest.CountOrphanedAlarmsL() == 3, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().AlarmCountByState(EAlarmStateSnoozed) == 0, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().AlarmCountByState(EAlarmStateNotified) == TheNumberOfReviewAlarms, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue() == 5, __LINE__);
	//
	User::After(5000000);
	//
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalMinutes(-1);
	alarm.Message() = _L("six");
	alarm.SoundName() = _L("rings");
	alarm.Characteristics() = 0;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	//
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	//
	User::After(5000000);
	//
	TInt count;
	count = TheAlarmTest.CountOrphanedAlarmsL();
	TheAlarmTest(count == 3, __LINE__);
	count = TheAlarmTest.Session().AlarmCountByState(EAlarmStateSnoozed);
	TheAlarmTest(count == 0, __LINE__);
	count = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(count == 5, __LINE__);
	count = TheAlarmTest.Session().AlarmCountByState(EAlarmStateNotified);
	TheAlarmTest(count == TheNumberOfReviewAlarms + 1, __LINE__);
	//
	r = TheAlarmTest.Session().SetAlarmCharacteristics(sessionAlarmId, 0);
	TheAlarmTest(r==KErrNone, __LINE__);
	TheAlarmTest(TheStatus != KRequestPending, __LINE__);
	User::WaitForRequest(TheStatus);
#endif  // __WINS__
	}


static void testSnoozeArrayL()
//
//	Test the snooze debug alarm
//
	{
	TheAlarmTest.Test().Next(_L("Snoozed alarms"));
#if !defined(_DEBUG)
	TheAlarmTest.Test().Printf(_L("This is a debug test only!\n"));
	return;
#else   // _DEBUG
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue() == 0, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().AlarmCountByState(EAlarmStateSnoozed) == 0, __LINE__);
	TheAlarmTest(TheAlarmTest.CountOrphanedAlarmsL() == 0, __LINE__);

	TASShdAlarm alarm;
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalDays(1);
	alarm.NextDueTime().RoundUpToNextMinute();
	alarm.Message() =_L("one");
	TheAlarmTest.Session().AlarmAddWithNotification(TheStatus, alarm);
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	TheAlarmTest(TheStatus == KRequestPending, __LINE__);
	//
	TASShdAlarm alarm2;
	TInt r = TheAlarmTest.Session().GetAlarmDetails(alarm.Id(), alarm2);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue() == 1, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().AlarmCountByState(EAlarmStateSnoozed) == 0, __LINE__);
	TheAlarmTest(TheAlarmTest.CountOrphanedAlarmsL() == 0, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDelete(alarm.Id());
	TheAlarmTest(r == KErrAccessDenied, __LINE__);
	//
	TTime snooze = alarm.NextDueTime() + TTimeIntervalMinutes(20);
	r = TheAlarmTest.Session().__DbgSnoozeAlarm(alarm.Id(), snooze - User::UTCOffset());
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	TInt count;
	count = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(count == 1, __LINE__);
	count = TheAlarmTest.Session().AlarmCountByState(EAlarmStateSnoozed);
	TheAlarmTest(count == 1, __LINE__);
	count = TheAlarmTest.CountOrphanedAlarmsL();
	TheAlarmTest(count == 0, __LINE__);
	//
	TAlarmId nextAlarmId;
	r = TheAlarmTest.Session().GetNextDueAlarmId(nextAlarmId);
	TheAlarmTest(r == KErrNone, __LINE__);
	TASShdAlarm nextAlarm;
	r = TheAlarmTest.Session().GetAlarmDetails(nextAlarmId, nextAlarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	TheAlarmTest(nextAlarm.State() == EAlarmStateSnoozed, __LINE__);
	TheAlarmTest(nextAlarm.NextDueTime() == snooze, __LINE__);
	//
	RArray<TAlarmId> ids;
	TheAlarmTest.Session().GetAlarmIdListByStateL(EAlarmStateSnoozed, ids);
	TheAlarmTest(ids.Count() == 1, __LINE__);
	TheAlarmTest(ids[0] == nextAlarm.Id(), __LINE__);
	TheAlarmTest(nextAlarmId == alarm.Id(), __LINE__);
	TheAlarmTest.Session().AlarmNotificationCancelAndDequeue(alarm.Id());
	ids.Close();
#endif  // _DEBUG
	}

	
static void testSnoozeArrayUtcL()
	{
	TheAlarmTest.Test().Next(_L("Snoozed UTC alarms"));
#if !defined(_DEBUG)
	TheAlarmTest.Test().Printf(_L("THis is a debug test only!\n"));
	return;
#else   // _DEBUG
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue() == 0, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().AlarmCountByState(EAlarmStateSnoozed) == 0, __LINE__);
	TheAlarmTest(TheAlarmTest.CountOrphanedAlarmsL() == 0, __LINE__);

	TASShdAlarm alarm;
	TTime time;
	time.UniversalTime();
	time += TTimeIntervalDays(1);
	time.RoundUpToNextMinute();
	alarm.SetUtcNextDueTime(time);
	alarm.Message()=(_L("one"));
	TheAlarmTest.Session().AlarmAddWithNotification(TheStatus, alarm);
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	TheAlarmTest(TheStatus == KRequestPending, __LINE__);
	//
	TASShdAlarm alarm2;
	TInt r = TheAlarmTest.Session().GetAlarmDetails(alarm.Id(), alarm2);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue() == 1, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().AlarmCountByState(EAlarmStateSnoozed) == 0, __LINE__);
	TheAlarmTest(TheAlarmTest.CountOrphanedAlarmsL() == 0, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDelete(alarm.Id());
	TheAlarmTest(r == KErrAccessDenied, __LINE__);
	//
	TTime snooze = alarm.NextDueTime() + TTimeIntervalMinutes(20);
	r = TheAlarmTest.Session().__DbgSnoozeAlarm(alarm.Id(), snooze);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	TInt count;
	count = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(count == 1, __LINE__);
	count = TheAlarmTest.Session().AlarmCountByState(EAlarmStateSnoozed);
	TheAlarmTest(count == 1, __LINE__);
	count = TheAlarmTest.CountOrphanedAlarmsL();
	TheAlarmTest(count == 0, __LINE__);
	//
	TAlarmId nextAlarmId;
	r = TheAlarmTest.Session().GetNextDueAlarmId(nextAlarmId);
	TheAlarmTest(r == KErrNone, __LINE__);
	TASShdAlarm nextAlarm;
	r = TheAlarmTest.Session().GetAlarmDetails(nextAlarmId, nextAlarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	TheAlarmTest(nextAlarm.State() == EAlarmStateSnoozed, __LINE__);
	TheAlarmTest(nextAlarm.NextDueTime() == snooze, __LINE__);
	//
	RArray<TAlarmId> ids;
	TheAlarmTest.Session().GetAlarmIdListByStateL(EAlarmStateSnoozed, ids);
	TheAlarmTest(ids.Count() == 1, __LINE__);
	TheAlarmTest(ids[0] == nextAlarm.Id(), __LINE__);
	TheAlarmTest(nextAlarmId == alarm.Id(), __LINE__);
	TheAlarmTest.Session().AlarmNotificationCancelAndDequeue(alarm.Id());
	ids.Close();
#endif  // _DEBUG
	}


static void testSetUtcAlarmsL()
	{
#if defined(__WINS__)
	TheAlarmTest.Test().Next(_L("Set orphan review clock and session alarms"));
	TheAlarmTest(TheAlarmTest.CountOrphanedAlarmsL() == 0, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().AlarmCountByState(EAlarmStateSnoozed) == 0, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().AlarmCountByState(EAlarmStateNotified) == TheNumberOfReviewAlarms, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue() == 0, __LINE__);
	//
	TInt r;
	TAlarmId id;
	TASShdAlarm alarm;
	TASShdAlarm alarmInfo;
	TLocale locale;
	//
	TTime time;
	time.UniversalTime();
	time += TTimeIntervalDays(1);
	alarm.SetUtcNextDueTime(time);
	alarm.Message()=(_L("one"));
	alarm.Characteristics().Set(EAlarmCharacteristicsSessionSpecific);
	TheAlarmTest.Session().AlarmAddWithNotification(TheStatus, alarm);
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	//
	r = TheAlarmTest.Session().GetNextDueAlarmId(id);
	TheAlarmTest(id == alarm.Id(), __LINE__);
	r = TheAlarmTest.Session().GetAlarmDetails(id, alarmInfo);
	TheAlarmTest(r==KErrNone, __LINE__);
	TheAlarmTest(alarmInfo.Message() == alarm.Message(), __LINE__);
	//
	TASShdAlarm alarm2;
	time.HomeTime();
	time -= locale.UniversalTimeOffset();
	time += TTimeIntervalDays(1);
	time += TTimeIntervalMinutes(1);
	alarm2.SetUtcNextDueTime(time);
	alarm2.Message()=(_L("two"));
	alarm2.RepeatDefinition()=(EAlarmRepeatDefintionRepeatOnce);
	r = TheAlarmTest.Session().AlarmAdd(alarm2);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarm2.Id() != KNullAlarmId, __LINE__);
	//
	r = TheAlarmTest.Session().GetNextDueAlarmId(id);
	TheAlarmTest(id == alarm.Id(), __LINE__);
	r = TheAlarmTest.Session().GetAlarmDetails(id, alarmInfo);
	TheAlarmTest(r==KErrNone, __LINE__);
	TheAlarmTest(alarmInfo.Message() == alarm.Message(), __LINE__);
	//
	r = TheAlarmTest.Session().SetAlarmCharacteristics(alarm.Id(), 0);
	TheAlarmTest(r==KErrNone, __LINE__);
	User::WaitForRequest(TheStatus);
	TheAlarmTest(TheStatus != KRequestPending, __LINE__);
	//
	r = TheAlarmTest.Session().GetNextDueAlarmId(id);
	TheAlarmTest(id == alarm.Id(), __LINE__);
	r = TheAlarmTest.Session().GetAlarmDetails(id, alarmInfo);
	TheAlarmTest(r==KErrNone, __LINE__);
	TheAlarmTest(alarmInfo.Message() == alarm.Message(), __LINE__);
	//
	time = alarm.NextDueTime() + TTimeIntervalDays(1);
	alarm.SetUtcNextDueTime(time);
	alarm.Message()=(_L("three"));
	alarm.Characteristics().Set(EAlarmCharacteristicsSessionSpecific);
	//
	TheAlarmTest.Session().AlarmAddWithNotification(TheStatus, alarm);
	TheAlarmTest(alarm.Id() != KErrNone, __LINE__);
	r = TheAlarmTest.Session().SetAlarmCharacteristics(alarm.Id(), 0);
	TheAlarmTest(r==KErrNone, __LINE__);
	TheAlarmTest(TheStatus != KRequestPending, __LINE__);
	User::WaitForRequest(TheStatus);
	//
	time = alarm.NextDueTime() + TTimeIntervalDays(1);
	alarm.SetUtcNextDueTime(time);
	alarm.Message()=(_L("four"));
	alarm.Characteristics().Set(EAlarmCharacteristicsSessionSpecific);
	//
	TheAlarmTest.Session().AlarmAddWithNotification(TheStatus, alarm);
	TheAlarmTest(alarm.Id() != KErrNone, __LINE__);
	r = TheAlarmTest.Session().SetAlarmCharacteristics(alarm.Id(), 0);
	TheAlarmTest(r==KErrNone, __LINE__);
	TheAlarmTest(TheStatus != KRequestPending, __LINE__);
	User::WaitForRequest(TheStatus);
	//
	alarm.Characteristics().Set(EAlarmCharacteristicsSessionSpecific);
	time = alarm.NextDueTime() + TTimeIntervalDays(1);
	time += TTimeIntervalMinutes(1);
	alarm.SetUtcNextDueTime(time);
	alarm.Message()=(_L("five"));
	TheAlarmTest.Session().AlarmAddWithNotification(TheStatus, alarm);
	const TAlarmId sessionAlarmId = alarm.Id();
	//
	TheAlarmTest(TheAlarmTest.CountOrphanedAlarmsL() == 3, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().AlarmCountByState(EAlarmStateSnoozed) == 0, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().AlarmCountByState(EAlarmStateNotified) == TheNumberOfReviewAlarms, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue() == 5, __LINE__);
	//
	User::After(5000000);
	//
	time.UniversalTime();
	time += TTimeIntervalMinutes(-1);
	alarm.SetUtcNextDueTime(time);
	alarm.Message()=(_L("six"));
	alarm.SoundName()=(_L("rings"));
	alarm.Characteristics().Set(0);
	alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatOnce;
	//
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	//
	User::After(5000000);
	//
	TInt count;
	count = TheAlarmTest.CountOrphanedAlarmsL();
	TheAlarmTest(count == 3, __LINE__);
	count = TheAlarmTest.Session().AlarmCountByState(EAlarmStateSnoozed);
	TheAlarmTest(count == 0, __LINE__);
	count = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(count == 5, __LINE__);
	count = TheAlarmTest.Session().AlarmCountByState(EAlarmStateNotified);
	TheAlarmTest(count == TheNumberOfReviewAlarms + 1, __LINE__);
	//
	r = TheAlarmTest.Session().SetAlarmCharacteristics(sessionAlarmId, 0);
	TheAlarmTest(r==KErrNone, __LINE__);
	TheAlarmTest(TheStatus != KRequestPending, __LINE__);
	User::WaitForRequest(TheStatus);
#endif  // __WINS__
	}


static void doTestsL()
	{
	const TInt KGmtOffset = 0;
	_LIT(KTime, "20041130:050000.000000"); //In winter time
	TInt err = User::SetUTCTimeAndOffset(TTime(KTime), KGmtOffset);

	RArray<TInt> instructionSet;
	instructionSet.AppendL(EKeyEscape);
	instructionSet.AppendL(EKeyEscape);
	TheAlarmTest.TestStartServers(&instructionSet);
	TheAlarmTest.Test().Next(_L("Connecting to the alarm server"));
	TheAlarmTest(TheAlarmTest.Session().Connect()==KErrNone, __LINE__);
	//
#if defined(__WINS__)
	TheNumberOfReviewAlarms = TheAlarmTest.Session().AlarmCountByState(EAlarmStateNotified);
#else   // __WINS__
	TheAlarmTest.Session().AlarmCountByState(EAlarmStateNotified);
#endif  // __WINS__

	TheAlarmTest.TestClearStoreL();
	testSnoozeArrayL();
	testSetAlarmsL();
	//
	TheAlarmTest.TestClearStoreL();
	testSnoozeArrayUtcL();
	testSetUtcAlarmsL();

	TheAlarmTest.TestClearStoreL();
	TheAlarmTest.Session().Close();
	instructionSet.Close();
	}

/**
@SYMTestCaseID PIM-TALARMIDARRAYS-0001
*/
GLDEF_C TInt E32Main()
//
// Test the alarm server.
//
    {
	__UHEAP_MARK;
	//
	TInt ret = KErrNone;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	//
	TheAlarmTest.Title();
	TheAlarmTest.Start(_L("@SYMTestCaseID PIM-TALARMIDARRAYS-0001 Alarm arrays"));
	TRAPD(err, doTestsL());
	TheAlarmTest(err == KErrNone, __LINE__);
	TRAP(ret,TheAlarmTest.EndL());
	TheAlarmTest.Test().Close();
	ASTstAlarmTest::Close();
	//
	delete cleanup;
	//
	__UHEAP_MARKEND;
	return ret;
    }
