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
// Test session alarms
// 
//

#include "ASTstAlarmTest.h"

// Type definitions
#define UNUSED_VAR(a) a = a

// Constants
const TUint KHeapMinSize=0x01000;
const TUint KHeapMaxSize=0x10000;

// Global data
static TAlarmId TheAlarmId;
static TBool TheNotifyFlag=EFalse;
static TBool TheNotifyThread=ETrue;
static TRequestStatus TheSsnStatus;
static TRequestStatus ThisRequest=KErrNone;
static TRequestStatus CurrentRequest=KErrNone;

// Prototypes
static void testOtherNotify(TBool aExpected);


static TInt ntfThreadMain(TAny* /*aPtr*/)
//
//	Main thread of notification thread
//
	{
	RASCliSession almSvr;
	const TInt r=almSvr.Connect();
	TheAlarmTest(r==KErrNone, __LINE__);
	//
	TAlarmId alarmId;
	TRequestStatus stat;
	FOREVER
		{
		FOREVER
			{
			almSvr.NotifyChange(stat, alarmId);
			User::After(KTimeToWait);
			if	(stat == KRequestPending)
				break;
			User::WaitForRequest(stat);
			}
		User::WaitForRequest(stat);
		//
		if (!TheNotifyThread)
			break;
		//
		TheAlarmTest(stat >= EAlarmChangeEventState && stat <= EAlarmChangeEventLast, __LINE__);
		TheNotifyFlag = ETrue;
		}
	//
	TheNotifyFlag=ETrue;
	return(KErrNone);
	}


static void startNotifyThread(TPtrC16 aThreadName)
//
//	Start the thread that determines NotifyOnChange
//
	{
	TheAlarmTest.Test().Next(_L("Starting the notification test thread"));
	RThread t;
	TInt r=t.Create(aThreadName,ntfThreadMain,KDefaultStackSize,KHeapMinSize,KHeapMaxSize,NULL);
	TheAlarmTest(r==KErrNone, __LINE__);
	TRequestStatus tStat;
	t.Logon(tStat);
	t.SetPriority(EPriorityMore);
	TheAlarmTest(tStat==KRequestPending, __LINE__);
	t.Resume();
	TheAlarmTest(tStat==KRequestPending, __LINE__);
	User::After(KTimeToWait);
	}

static void testSetupNotify(TPtrC16 aThreadName)
//
//	Set up the notify requests
//
	{
	TheNotifyFlag=EFalse;
	startNotifyThread(aThreadName);
	//
	TheAlarmTest.Test().Next(_L("Set up notification requests"));
	//
	TheAlarmTest.Session().NotifyChange(CurrentRequest, TheAlarmId);
	User::WaitForRequest(CurrentRequest);
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(CurrentRequest, TheAlarmId);
	TheAlarmTest.Session().NotifyChangeCancel();
	//
	TheAlarmTest(CurrentRequest == KErrCancel, __LINE__);
	TheAlarmTest(TheNotifyFlag == EFalse, __LINE__);
	//
#if defined(_DEBUG)
	TheAlarmTest.Session().__DbgPreventUserNotify(ETrue);
#endif
	}

static void testOtherNotify(TBool aExpected)
//
//	Check there was a notification to the notify thread
//
	{
	User::After(KTimeToWait);
	TheAlarmTest(TheNotifyFlag==aExpected, __LINE__);
	TheNotifyFlag=EFalse;
	}

static TAlarmId SetSessionAlarm(TASShdAlarm& aAlarm, const TTime& aTime, TRequestStatus& aStatus)
//
//	Set the session alarm
//
	{
	TheAlarmTest(TheNotifyFlag == EFalse, __LINE__);
	//
	aAlarm.Reset();
	aAlarm.Message() = _L("The session alarm");
	aAlarm.NextDueTime() = aTime;
	aAlarm.Characteristics().Set(EAlarmCharacteristicsSessionSpecific);
	//
	TheAlarmTest.Session().AlarmAddWithNotification(aStatus, aAlarm);
	TheAlarmTest(aStatus == KRequestPending, __LINE__);
	//
	TheAlarmTest(aAlarm.Id() != KNullAlarmId, __LINE__);
	return aAlarm.Id();
	}

static void testSetSessionAlarm(const TDesC& aDes, const TTime& aDueTime)
//
//	Test setting a session alarm
//
	{
	TheAlarmTest.Test().Next(aDes);

	// Check there's nothing ready
	testOtherNotify(EFalse);

	// Get all events from the event buffer
	TAlarmId alarmId;
	TRequestStatus notificationStatus;
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(notificationStatus, alarmId);
	TheAlarmTest(notificationStatus == KRequestPending, __LINE__);

	// Queue session alarm
	TRequestStatus sessionAlarmStatus;
	TASShdAlarm alarm;
	TAlarmId sessionAlarmId = SetSessionAlarm(alarm, aDueTime, sessionAlarmStatus);

	// Check that the notification thread was updated
	testOtherNotify(ETrue);

	// Check that the notification request was correctly updated
	User::WaitForRequest(notificationStatus);
	TheAlarmTest(notificationStatus > 0, __LINE__);
	TheAlarmTest(alarmId == sessionAlarmId, __LINE__);

	// Get details
	TASShdAlarm alarm2;
	TInt r = TheAlarmTest.Session().GetAlarmDetails(sessionAlarmId, alarm2);
	TheAlarmTest(r == KErrNone, __LINE__);
	TDateTime dateTime = aDueTime.DateTime();
	TDateTime alarmTime = alarm2.NextDueTime().DateTime();

	// Check details
	TheAlarmTest(alarmTime.Day()==dateTime.Day(), __LINE__);
	TheAlarmTest(alarmTime.Hour()==dateTime.Hour(), __LINE__);
	TheAlarmTest(alarmTime.Minute()==dateTime.Minute(), __LINE__);
	alarmTime = alarm2.NextDueTime().DateTime();
	dateTime = aDueTime.DateTime();
	TheAlarmTest(alarmTime.Day()==dateTime.Day(), __LINE__);
	TheAlarmTest(alarmTime.Hour()==dateTime.Hour(), __LINE__);
	TheAlarmTest(alarmTime.Minute()==dateTime.Minute(), __LINE__);

	// Cancel alarm
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(notificationStatus, alarmId);
	TheAlarmTest(notificationStatus == KRequestPending, __LINE__);
	TheAlarmTest.Session().AlarmNotificationCancelAndDequeue(sessionAlarmId);
	User::WaitForRequest(sessionAlarmStatus);
	TheAlarmTest(sessionAlarmStatus == KErrCancel, __LINE__);

	// Check that the notification thread was updated
	testOtherNotify(ETrue);

	// Check that the notification request was correctly updated
	User::WaitForRequest(notificationStatus);
	TheAlarmTest(notificationStatus == EAlarmChangeEventAlarmDeletion, __LINE__);
	TheAlarmTest(alarmId == sessionAlarmId, __LINE__);

	// Add another alarm
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(notificationStatus, alarmId);
	sessionAlarmId = SetSessionAlarm(alarm, aDueTime, sessionAlarmStatus);
	testOtherNotify(ETrue);

	// Check that the notification request was correctly updated
	User::WaitForRequest(notificationStatus);
	TheAlarmTest(notificationStatus > 0, __LINE__);
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(notificationStatus, alarmId);

	// Orphan alarm
	r = TheAlarmTest.Session().SetAlarmCharacteristics(sessionAlarmId, EAlarmCharacteristicsSessionSpecific);
	User::WaitForRequest(sessionAlarmStatus);
	TheAlarmTest(sessionAlarmStatus == KErrCancel, __LINE__);
	TheAlarmTest(alarmId == sessionAlarmId, __LINE__);

	// Get details
	r = TheAlarmTest.Session().GetAlarmDetails(sessionAlarmId, alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarm.HasBecomeOrphaned(), __LINE__);
	testOtherNotify(ETrue);
	}

static void testImmediateClose()
//
//	Test closing a session after the alarm goes off
//	The session alarm must be on the notification queue but not first
//	So that its session paramter is not cleared when its owner's name needs
//	to be found.
//
	{

	TTime time;
	time.HomeTime();
	TheAlarmTest(TheNotifyFlag == EFalse, __LINE__);

	TASShdAlarm alarm;
	alarm.SoundName() = _L("Signal");
	alarm.NextDueTime() = time - TTimeIntervalSeconds(75);
	alarm.Message() = _L("Session alarm");
	//
	RASCliSession almSvr;
	TheAlarmTest(almSvr.Connect() == KErrNone, __LINE__);
	almSvr.AlarmAddWithNotification(TheSsnStatus, alarm);
	//
	User::After(1000000);
	const TAlarmId sessionAlarmId = alarm.Id();
	TheAlarmTest(sessionAlarmId != KNullAlarmId, __LINE__);
	//
	alarm.Reset();
	alarm.SoundName() = _L("Signal");
	alarm.NextDueTime() = time - TTimeIntervalSeconds(65);
	alarm.Message() = _L("Clock alarm");
	//
	TheAlarmTest(almSvr.AlarmAdd(alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.Id() != KNullAlarmId && alarm.Id() != sessionAlarmId, __LINE__);
	//
	almSvr.Close();
	//
	}

static void testSetAlarms()
//
//	Test setting a day alarm
//
	{
	TTime inAFewMinutes;
	inAFewMinutes.HomeTime();
	inAFewMinutes += TTimeIntervalMinutes(2);

	TTime inThreeDays;
	inThreeDays.HomeTime();
	inThreeDays += TTimeIntervalDays(3);
	//
	
	testSetSessionAlarm(_L("Session alarm, in a few minutes"), inAFewMinutes);
	
	
	testSetSessionAlarm(_L("Session alarm, in a few days"), inThreeDays);
	}


static void testTakeOverOrphanL()
//
//	Test whether a session alarm takes over an orphaned alarm
//
	{
	TheAlarmTest.Test().Next(_L("Take over orphaned"));
	TheAlarmTest.TestClearStoreL();
	testOtherNotify(ETrue);

	// Get all events from the event buffer
	TAlarmId alarmId;
	TRequestStatus notificationStatus;
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(notificationStatus, alarmId);
	TheAlarmTest(notificationStatus == KRequestPending, __LINE__);

	// Add session alarm
	TASShdAlarm alarm;
	TTime time;
	time.HomeTime();
	time += TTimeIntervalDays(3);

	// Create session alarm
	TAlarmId sessionAlarmId = SetSessionAlarm(alarm, time, TheSsnStatus);
	TheAlarmTest(TheSsnStatus == KRequestPending, __LINE__);

	// Check that the notification thread was updated
	testOtherNotify(ETrue);

	// Check that the notification request was correctly updated
	User::WaitForRequest(notificationStatus);
	TheAlarmTest(notificationStatus > 0, __LINE__);
	TheAlarmTest(alarmId == sessionAlarmId, __LINE__);

	// Check the number of orphaned alarms is 0
	TInt n = TheAlarmTest.CountOrphanedAlarmsL();
	TheAlarmTest(n == 0, __LINE__);

	// Orphan the alarm
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(notificationStatus, alarmId);
	TheAlarmTest(TheAlarmTest.Session().SetAlarmCharacteristics(sessionAlarmId, 0) == KErrNone, __LINE__);
	TheAlarmTest(TheSsnStatus == KErrCancel, __LINE__);

	// Check that the notification thread was updated
	testOtherNotify(ETrue);

	// Check that the notification request was correctly updated
	User::WaitForRequest(notificationStatus);
	TheAlarmTest(notificationStatus > 0, __LINE__);
	TheAlarmTest(alarmId == sessionAlarmId, __LINE__);

	// Check the number of orphaned alarms is 1
	n = TheAlarmTest.CountOrphanedAlarmsL();
	TheAlarmTest(n == 1, __LINE__);

	// Create another session alarm
	time.HomeTime();
	time += TTimeIntervalDays(4);
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(notificationStatus, alarmId);
	sessionAlarmId = SetSessionAlarm(alarm, time, TheSsnStatus);
	TheAlarmTest(TheSsnStatus == KRequestPending, __LINE__);

	// Check that the notification thread was updated
	testOtherNotify(ETrue);

	// Check that the notification request was correctly updated
	User::WaitForRequest(notificationStatus);
	TheAlarmTest(notificationStatus > 0, __LINE__);
	TheAlarmTest(alarmId == sessionAlarmId, __LINE__);

	// Check the number of orphaned alarms is 1
	n = TheAlarmTest.CountOrphanedAlarmsL();
	TheAlarmTest(n == 1, __LINE__);

	// Orphan the alarm
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(notificationStatus, alarmId);
	TheAlarmTest(TheAlarmTest.Session().SetAlarmCharacteristics(sessionAlarmId, 0) == KErrNone, __LINE__);
	TheAlarmTest(TheSsnStatus == KErrCancel, __LINE__);

	// Check that the notification thread was updated
	testOtherNotify(ETrue);

	// Check that the notification request was correctly updated
	User::WaitForRequest(notificationStatus);
	TheAlarmTest(notificationStatus > 0, __LINE__);
	TheAlarmTest(alarmId == sessionAlarmId, __LINE__);

	// Check the number of orphaned alarms is 2
	n = TheAlarmTest.CountOrphanedAlarmsL();
	TheAlarmTest(n == 2, __LINE__);

	// Create another session alarm
	time.HomeTime();
	time += TTimeIntervalDays(5);
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(notificationStatus, alarmId);
	sessionAlarmId = SetSessionAlarm(alarm, time, TheSsnStatus);
	TheAlarmTest(TheSsnStatus == KRequestPending, __LINE__);

	// Check that the notification thread was updated
	testOtherNotify(ETrue);

	// Check that the notification request was correctly updated
	User::WaitForRequest(notificationStatus);
	TheAlarmTest(notificationStatus > 0, __LINE__);
	TheAlarmTest(alarmId == sessionAlarmId, __LINE__);

	// Check the number of orphaned alarms is 2
	n = TheAlarmTest.CountOrphanedAlarmsL();
	TheAlarmTest(n == 2, __LINE__);

	// Cancel session alarm
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(notificationStatus, alarmId);
	TheAlarmTest(notificationStatus == KRequestPending, __LINE__);
	TheAlarmTest.Session().AlarmNotificationCancelAndDequeue(sessionAlarmId);
	User::WaitForRequest(TheSsnStatus);
	TheAlarmTest(TheSsnStatus == KErrCancel, __LINE__);

	// Check that the notification thread was updated
	testOtherNotify(ETrue);

	// Check that the notification request was correctly updated
	User::WaitForRequest(notificationStatus);
	TheAlarmTest(notificationStatus > 0, __LINE__);
	TheAlarmTest(alarmId == sessionAlarmId, __LINE__);

	// Check the number of orphaned alarms is 2
	n = TheAlarmTest.CountOrphanedAlarmsL();
	TheAlarmTest(n == 2, __LINE__);
	}

static TAlarmId SetSessionUtcAlarm(TASShdAlarm& aAlarm, const TTime& aTime, TRequestStatus& aStatus)
//
//	Set the session UTC alarm
//
	{
	TheAlarmTest(TheNotifyFlag == EFalse, __LINE__);
	//
	aAlarm.Reset();
	aAlarm.Message()=(_L("The session UTC alarm"));
	aAlarm.SetUtcNextDueTime(aTime);
	aAlarm.Characteristics().Set(EAlarmCharacteristicsSessionSpecific);
	//
	TheAlarmTest.Session().AlarmAddWithNotification(aStatus, aAlarm);
	TheAlarmTest(aStatus == KRequestPending, __LINE__);
	//
	TheAlarmTest(aAlarm.Id() != KNullAlarmId, __LINE__);
	return aAlarm.Id();
	}

static void testSetSessionUtcAlarm(const TDesC& aDes, const TTime& aDueTime)
//
//	Test setting a session UTC alarm
//
	{
	TheAlarmTest.Test().Next(aDes);

	// Check there's nothing ready
	testOtherNotify(EFalse);

	// Get all events from the event buffer
	TAlarmId alarmId;
	TRequestStatus notificationStatus;
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(notificationStatus, alarmId);
	TheAlarmTest(notificationStatus == KRequestPending, __LINE__);

	// Queue session alarm
	TRequestStatus sessionAlarmStatus;
	TASShdAlarm alarm;
	TAlarmId sessionAlarmId = SetSessionUtcAlarm(alarm, aDueTime, sessionAlarmStatus);

	// Check that the notification thread was updated
	testOtherNotify(ETrue);

	// Check that the notification request was correctly updated
	User::WaitForRequest(notificationStatus);
	TheAlarmTest(notificationStatus > 0, __LINE__);
	TheAlarmTest(alarmId == sessionAlarmId, __LINE__);

	// Get details
	TASShdAlarm alarm2;
	TInt r = TheAlarmTest.Session().GetAlarmDetails(sessionAlarmId, alarm2);
	TheAlarmTest(r == KErrNone, __LINE__);
	TDateTime dateTime = aDueTime.DateTime();
	TDateTime alarmTime = alarm2.NextDueTime().DateTime();

	// Check details
	TheAlarmTest(alarmTime.Day()==dateTime.Day(), __LINE__);
	TheAlarmTest(alarmTime.Hour()==dateTime.Hour(), __LINE__);
	TheAlarmTest(alarmTime.Minute()==dateTime.Minute(), __LINE__);
	alarmTime = alarm2.NextDueTime().DateTime();
	dateTime = aDueTime.DateTime();
	TheAlarmTest(alarmTime.Day()==dateTime.Day(), __LINE__);
	TheAlarmTest(alarmTime.Hour()==dateTime.Hour(), __LINE__);
	TheAlarmTest(alarmTime.Minute()==dateTime.Minute(), __LINE__);

	// Cancel alarm
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(notificationStatus, alarmId);
	TheAlarmTest(notificationStatus == KRequestPending, __LINE__);
	TheAlarmTest.Session().AlarmNotificationCancelAndDequeue(sessionAlarmId);
	User::WaitForRequest(sessionAlarmStatus);
	TheAlarmTest(sessionAlarmStatus == KErrCancel, __LINE__);

	// Check that the notification thread was updated
	testOtherNotify(ETrue);

	// Check that the notification request was correctly updated
	User::WaitForRequest(notificationStatus);
	TheAlarmTest(notificationStatus == EAlarmChangeEventAlarmDeletion, __LINE__);
	TheAlarmTest(alarmId == sessionAlarmId, __LINE__);

	// Add another alarm
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(notificationStatus, alarmId);
	sessionAlarmId = SetSessionUtcAlarm(alarm, aDueTime, sessionAlarmStatus);
	testOtherNotify(ETrue);

	// Check that the notification request was correctly updated
	User::WaitForRequest(notificationStatus);
	TheAlarmTest(notificationStatus > 0, __LINE__);
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(notificationStatus, alarmId);

	// Orphan alarm
	r = TheAlarmTest.Session().SetAlarmCharacteristics(sessionAlarmId, EAlarmCharacteristicsSessionSpecific);
	User::WaitForRequest(sessionAlarmStatus);
	TheAlarmTest(sessionAlarmStatus == KErrCancel, __LINE__);
	TheAlarmTest(alarmId == sessionAlarmId, __LINE__);

	// Get details
	r = TheAlarmTest.Session().GetAlarmDetails(sessionAlarmId, alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarm.HasBecomeOrphaned(), __LINE__);
	testOtherNotify(ETrue);
	}

static void testImmediateCloseUtc()
//
//	Test closing a session after the alarm goes off
//	The session alarm must be on the notification queue but not first
//	So that its session paramter is not cleared when its owner's name needs
//	to be found.
//
	{
	TTime time;
	time.UniversalTime();
	TheAlarmTest(TheNotifyFlag == EFalse, __LINE__);

	TASShdAlarm alarm;
	alarm.SoundName()=(_L("Signal"));
	alarm.SetUtcNextDueTime(time - TTimeIntervalSeconds(75));
	alarm.Message()=(_L("Session alarm"));
	//
	RASCliSession almSvr;
	TheAlarmTest(almSvr.Connect() == KErrNone, __LINE__);
	almSvr.AlarmAddWithNotification(TheSsnStatus, alarm);
	//
	User::After(1000000);
	const TAlarmId sessionAlarmId = alarm.Id();
	TheAlarmTest(sessionAlarmId != KNullAlarmId, __LINE__);
	//
	alarm.Reset();
	alarm.SoundName()=(_L("Signal"));
	alarm.SetUtcNextDueTime(time - TTimeIntervalSeconds(65));
	alarm.Message()=(_L("Clock alarm"));
	//
	TheAlarmTest(almSvr.AlarmAdd(alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.Id() != KNullAlarmId && alarm.Id() != sessionAlarmId, __LINE__);
	//
	almSvr.Close();
	//
	}

static void testSetUtcAlarms()
//
//	Test setting a day alarm
//
	{
	TTime inAFewMinutes;
	inAFewMinutes.UniversalTime();
	inAFewMinutes += TTimeIntervalMinutes(2);

	TTime inThreeDays;
	inThreeDays.UniversalTime();
	inThreeDays += TTimeIntervalDays(3);
	//
	
	
	testSetSessionUtcAlarm(_L("Session alarm, in a few minutes"), inAFewMinutes);
	
	
	testSetSessionUtcAlarm(_L("Session alarm, in a few days"), inThreeDays);
	}


static void testTakeOverOrphanUtcL()
//
//	Test whether a session alarm takes over an orphaned alarm
//
	{
	TheAlarmTest.Test().Next(_L("Take over orphaned UTC"));
	TheAlarmTest.TestClearStoreL();
	testOtherNotify(ETrue);

	// Get all events from the event buffer
	TAlarmId alarmId;
	TRequestStatus notificationStatus;
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(notificationStatus, alarmId);
	TheAlarmTest(notificationStatus == KRequestPending, __LINE__);

	// Add session alarm
	TASShdAlarm alarm;
	TTime time;
	time.HomeTime();
	time += TTimeIntervalDays(3);

	// Create session alarm
	TAlarmId sessionAlarmId = SetSessionUtcAlarm(alarm, time, TheSsnStatus);
	TheAlarmTest(TheSsnStatus == KRequestPending, __LINE__);

	// Check that the notification thread was updated
	testOtherNotify(ETrue);

	// Check that the notification request was correctly updated
	User::WaitForRequest(notificationStatus);
	TheAlarmTest(notificationStatus > 0, __LINE__);
	TheAlarmTest(alarmId == sessionAlarmId, __LINE__);

	// Check the number of orphaned alarms is 0
	TInt n = TheAlarmTest.CountOrphanedAlarmsL();
	TheAlarmTest(n == 0, __LINE__);

	// Orphan the alarm
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(notificationStatus, alarmId);
	TheAlarmTest(TheAlarmTest.Session().SetAlarmCharacteristics(sessionAlarmId, 0) == KErrNone, __LINE__);
	TheAlarmTest(TheSsnStatus == KErrCancel, __LINE__);

	// Check that the notification thread was updated
	testOtherNotify(ETrue);

	// Check that the notification request was correctly updated
	User::WaitForRequest(notificationStatus);
	TheAlarmTest(notificationStatus > 0, __LINE__);
	TheAlarmTest(alarmId == sessionAlarmId, __LINE__);

	// Check the number of orphaned alarms is 1
	n = TheAlarmTest.CountOrphanedAlarmsL();
	TheAlarmTest(n == 1, __LINE__);

	// Create another session alarm
	time.HomeTime();
	time += TTimeIntervalDays(4);
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(notificationStatus, alarmId);
	sessionAlarmId = SetSessionUtcAlarm(alarm, time, TheSsnStatus);
	TheAlarmTest(TheSsnStatus == KRequestPending, __LINE__);

	// Check that the notification thread was updated
	testOtherNotify(ETrue);

	// Check that the notification request was correctly updated
	User::WaitForRequest(notificationStatus);
	TheAlarmTest(notificationStatus > 0, __LINE__);
	TheAlarmTest(alarmId == sessionAlarmId, __LINE__);

	// Check the number of orphaned alarms is 1
	n = TheAlarmTest.CountOrphanedAlarmsL();
	TheAlarmTest(n == 1, __LINE__);

	// Orphan the alarm
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(notificationStatus, alarmId);
	TheAlarmTest(TheAlarmTest.Session().SetAlarmCharacteristics(sessionAlarmId, 0) == KErrNone, __LINE__);
	TheAlarmTest(TheSsnStatus == KErrCancel, __LINE__);

	// Check that the notification thread was updated
	testOtherNotify(ETrue);

	// Check that the notification request was correctly updated
	User::WaitForRequest(notificationStatus);
	TheAlarmTest(notificationStatus > 0, __LINE__);
	TheAlarmTest(alarmId == sessionAlarmId, __LINE__);

	// Check the number of orphaned alarms is 2
	n = TheAlarmTest.CountOrphanedAlarmsL();
	TheAlarmTest(n == 2, __LINE__);

	// Create another session alarm
	time.HomeTime();
	time += TTimeIntervalDays(5);
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(notificationStatus, alarmId);
	sessionAlarmId = SetSessionUtcAlarm(alarm, time, TheSsnStatus);
	TheAlarmTest(TheSsnStatus == KRequestPending, __LINE__);

	// Check that the notification thread was updated
	testOtherNotify(ETrue);

	// Check that the notification request was correctly updated
	User::WaitForRequest(notificationStatus);
	TheAlarmTest(notificationStatus > 0, __LINE__);
	TheAlarmTest(alarmId == sessionAlarmId, __LINE__);

	// Check the number of orphaned alarms is 2
	n = TheAlarmTest.CountOrphanedAlarmsL();
	TheAlarmTest(n == 2, __LINE__);

	// Cancel session alarm
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(notificationStatus, alarmId);
	TheAlarmTest(notificationStatus == KRequestPending, __LINE__);
	TheAlarmTest.Session().AlarmNotificationCancelAndDequeue(sessionAlarmId);
	User::WaitForRequest(TheSsnStatus);
	TheAlarmTest(TheSsnStatus == KErrCancel, __LINE__);

	// Check that the notification thread was updated
	testOtherNotify(ETrue);

	// Check that the notification request was correctly updated
	User::WaitForRequest(notificationStatus);
	TheAlarmTest(notificationStatus > 0, __LINE__);
	TheAlarmTest(alarmId == sessionAlarmId, __LINE__);

	// Check the number of orphaned alarms is 2
	n = TheAlarmTest.CountOrphanedAlarmsL();
	TheAlarmTest(n == 2, __LINE__);
	}


static void doTestsL()
	{
	RArray<TInt> instructionSet;
	instructionSet.AppendL(EKeyEscape);
	instructionSet.AppendL(EKeyEscape);
	TheAlarmTest.TestStartServers(&instructionSet);
	TheAlarmTest(TheAlarmTest.Session().Connect() == KErrNone, __LINE__);

#if defined(_DEBUG)
// The alarm should only be allowed to display notification in WINS builds.
#if defined(__WINSCW__)
	TheAlarmTest.Session().__DbgPreventUserNotify(EFalse);
#else
	TheAlarmTest.Session().__DbgPreventUserNotify(ETrue);
#endif
#endif

	TheAlarmTest.TestClearStoreL();
	testImmediateClose();
	//
	testSetupNotify(_L("NtfThread1"));
	//

	testSetAlarms();
	testTakeOverOrphanL();
	//
	TheAlarmTest.TestClearStoreL();
	// Reset the notify flag for the UTC test.
	TheNotifyFlag = EFalse;
	// Stop the notify thread so it can be restarted without causing an error.
	TheNotifyThread = EFalse;
	// Allow time for the thread to stop.
	User::After(10000);
	// Enable the thread so it can be subsequently restarted.
	TheNotifyThread = ETrue;

	TheAlarmTest.TestClearStoreL();
	testImmediateCloseUtc();
	//
	testSetupNotify(_L("NtfThread2"));
	//

	testSetUtcAlarms();
	testTakeOverOrphanUtcL();
	//
	TheAlarmTest.TestClearStoreL();
	instructionSet.Close();
	}

/**
@SYMTestCaseID PIM-TSESSIONALARMS-0001
*/	
GLDEF_C TInt E32Main()
//
// Test the alarm server.
//
    {
    TInt ret = KErrNone;
	TheAlarmTest.Title();
	TheAlarmTest.Start(_L("@SYMTestCaseID PIM-TSESSIONALARMS-0001 Session alarms"));

	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	TRAPD(err, doTestsL());
	UNUSED_VAR(err); //Used to supress build warnings

	delete cleanup;
	__UHEAP_MARKEND;

    TheAlarmTest.Session().Close();
    TRAP(ret,TheAlarmTest.EndL());
	TheAlarmTest.Test().Close();
	ASTstAlarmTest::Close();
	//
	return ret;
    }
