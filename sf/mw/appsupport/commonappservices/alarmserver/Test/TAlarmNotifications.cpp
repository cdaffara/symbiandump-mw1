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
// Test session notification of alarms
// 
//

#include "ASTstAlarmTest.h"
#include "testserver.h"

// Constants
const TUint KHeapMinSize=0x01000;
const TUint KHeapMaxSize=0x10000;

// Globals
static TAlarmId TheAlarmId = KNullAlarmId;
static TBool TheNotifyFlag=EFalse;
static TBool TheNotifyThread=ETrue;
static TRequestStatus TheCurrentRequest=KErrNone;
static TRequestStatus TheSessionStatus;
//
static void testOtherNotify(TBool aExpected);
static void testThisNotify(TInt aExpected=KErrNone);


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


static void startNotifyThread()
//
//	Start the thread that determines NotifyOnChange
//
	{
	TheAlarmTest.Test().Next(_L("Starting the notification test thread"));
	RThread t;
	TInt r=t.Create(_L("NtfThread"),ntfThreadMain,KDefaultStackSize,KHeapMinSize,KHeapMaxSize,NULL);
	TheAlarmTest(r==KErrNone, __LINE__);
	TRequestStatus tStat;
	t.Logon(tStat);
	t.SetPriority(EPriorityMore);
	TheAlarmTest(tStat==KRequestPending, __LINE__);
	t.Resume();
	TheAlarmTest(tStat==KRequestPending, __LINE__);
	User::After(KTimeToWait);
	}


static void testSetupNotify()
//
//	Set up the notify requests
//
	{
	TheNotifyFlag=EFalse;
	startNotifyThread();
	//
	TheAlarmTest.Test().Next(_L("Set up notification requests"));
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(TheCurrentRequest, TheAlarmId);
	TheAlarmTest.Session().NotifyChangeCancel();
	//
	TheAlarmTest(TheCurrentRequest == KErrCancel, __LINE__);
	TheAlarmTest(TheNotifyFlag == EFalse, __LINE__);
	//
#if defined(_DEBUG)
	TheAlarmTest.Session().__DbgPreventUserNotify(ETrue);
#endif
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(TheCurrentRequest, TheAlarmId);
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

static void testThisNotify(TInt aExpected)
//
//	Check there the notification of this thread. Because notifications
//	in the new Alarm Server are bufferred, then the first notification
//	may not be the one we are looking for. Therefore we continue to
//  request notifications until the buffer is exhausted, at which point,
//	if we haven't yet seent the notification we were looking for, we
//	panic.
//
	{
	if	(TheCurrentRequest != aExpected)
		{
		// Need to exhaust the event buffer

		// If the buffer is empty (server side) then the current
		// request will still be outstanding, therefore the test
		// has failed.
		User::After(KTimeToWait);
		TheAlarmTest(TheCurrentRequest != KRequestPending, __LINE__);

		// Eat current request
		User::WaitForRequest(TheCurrentRequest);

		// Now go through each item in the buffer, requesting them
		// one by one
		FOREVER
			{
			TheAlarmTest.Session().NotifyChange(TheCurrentRequest, TheAlarmId);
			User::After(KTimeToWait);
			TheAlarmTest(TheCurrentRequest != KRequestPending, __LINE__);
			User::WaitForRequest(TheCurrentRequest);
			if	(TheCurrentRequest == aExpected)
				break;
			}
		}

	// Request another notification
	if	(TheCurrentRequest != KRequestPending)
		{
		// Eat all the rest
		TheAlarmTest.WaitForNotificationBufferToBeEmptied(TheCurrentRequest, TheAlarmId);
		}
	}


static void testTimeChangeL()
//
//	Test that time notifications work
//
	{
	RPIMTestServer serv;
    User::LeaveIfError(serv.Connect());

	TheAlarmTest.Test().Next(_L("Time changes and alarms set with local wall-clock time."));
	//
	testOtherNotify(EFalse);
	testThisNotify(KRequestPending);
	//
	testThisNotify(KRequestPending);
	testOtherNotify(EFalse);
	//
	TTime home;
	home.HomeTime();
	home+=TTimeIntervalMinutes(2);

    serv.SetHomeTime(home);

	User::After(KTimeToWait);
	//
	testThisNotify(EAlarmChangeEventSystemDateTimeChanged);
	testOtherNotify(ETrue);
	testOtherNotify(EFalse);
	testThisNotify(KRequestPending);
	//
	TASShdAlarm alarm;
	alarm.Message() = _L("Time");
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalMinutes(30);
	TheAlarmTest.Session().AlarmAddWithNotification(TheSessionStatus, alarm);
	TheAlarmTest(TheSessionStatus == KRequestPending, __LINE__);
	//
	testOtherNotify(ETrue);
	testThisNotify(EAlarmChangeEventAlarmAddition);
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(TheCurrentRequest, TheAlarmId);
	//
	TInt r;
	TAlarmId nextDueAlarmId;
	//
	r = TheAlarmTest.Session().GetNextDueAlarmId(nextDueAlarmId);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(nextDueAlarmId = alarm.Id(), __LINE__);
	//
	TASShdAlarm retrievedAlarm;
	r = TheAlarmTest.Session().GetAlarmDetails(nextDueAlarmId, retrievedAlarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(retrievedAlarm.Message() == _L("Time"), __LINE__);
	//
    serv.SetHomeTime(retrievedAlarm.NextDueTime());
    serv.Close();

	User::After(KTimeToWait);
	//
	testOtherNotify(ETrue);
	testThisNotify(EAlarmChangeEventSystemDateTimeChanged);
	TheAlarmTest(TheSessionStatus != KRequestPending, __LINE__);
	User::WaitForRequest(TheSessionStatus);
	//
	r = TheAlarmTest.Session().GetNextDueAlarmId(nextDueAlarmId);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(nextDueAlarmId == KNullAlarmId, __LINE__);
	//
	TheAlarmTest.Session().AlarmNotificationCancelAndDequeue(alarm.Id());
	testThisNotify(EAlarmChangeEventAlarmDeletion);
	testOtherNotify(ETrue);
	//
	alarm.NextDueTime() += TTimeIntervalDays(1);
	alarm.Message() = _L("DayTime");
	TheAlarmTest.Session().AlarmAddWithNotification(TheSessionStatus, alarm);
	testThisNotify(EAlarmChangeEventAlarmAddition);
	//
	TheAlarmTest.Session().AlarmNotificationCancelAndDequeue(alarm.Id());
	testThisNotify(EAlarmChangeEventAlarmDeletion);
	//
	TheAlarmTest.Session().AlarmAddWithNotification(TheSessionStatus, alarm);
	testThisNotify(EAlarmChangeEventAlarmAddition);
	testOtherNotify(ETrue);
	//
	TheAlarmTest.Session().AlarmNotificationCancelAndDequeue(alarm.Id());
	testThisNotify(EAlarmChangeEventAlarmDeletion);
	//
	TheAlarmTest(TheSessionStatus==KErrCancel, __LINE__);
	testOtherNotify(ETrue);
	}

	
static void testSetAndCancelPast()
//
//	Test that each type of alarm goes off if set in the past.
//	Careful of DayAlarms - cant be sure if a time in today is past/before the current time.
//
	{
	TheAlarmTest.Test().Next(_L("Set alarms with local wall-clock time in past"));
	testOtherNotify(EFalse);
	//
	TASShdAlarm alarm;
	alarm.Message() = _L("Past");
	alarm.SoundName() = _L("none");
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalDays(-2);
	//
	testThisNotify(KRequestPending);
	testOtherNotify(EFalse);
	//
	TInt r;
	//
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	TAlarmId clockAlarmId = alarm.Id();
	//
	testThisNotify(EAlarmChangeEventAlarmAddition);
	testOtherNotify(ETrue);
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(TheCurrentRequest, TheAlarmId);
	//
	testThisNotify(KRequestPending);
	testOtherNotify(EFalse);
	testThisNotify(KRequestPending);
	//
	TheAlarmTest.Session().AlarmAddWithNotification(TheSessionStatus, alarm);
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	testOtherNotify(ETrue);
	testThisNotify(EAlarmChangeEventAlarmAddition);
	TheAlarmTest(TheSessionStatus == KErrNone, __LINE__);
	User::WaitForRequest(TheSessionStatus);
	//
	TheAlarmTest.Session().AlarmNotificationCancelAndDequeue(alarm.Id());
	testThisNotify(EAlarmChangeEventAlarmDeletion);
	testOtherNotify(ETrue);
	//
	r = TheAlarmTest.Session().AlarmDelete(clockAlarmId);
	TheAlarmTest(r == 0, __LINE__);
	testThisNotify(EAlarmChangeEventAlarmDeletion);
	testOtherNotify(ETrue);
	}


static void testSetAndCancelFuture()
//
//	Set alarms in future
//
	{
	TheAlarmTest.Test().Next(_L("Set alarms with local wall-clock time in future"));
	TheAlarmTest(TheNotifyFlag==EFalse, __LINE__);
	//
	TASShdAlarm alarm;
	alarm.Message() = _L("Future");
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalDays(1);
	alarm.NextDueTime() += TTimeIntervalMinutes(30);
	//
	testOtherNotify(EFalse);
	testThisNotify(KRequestPending);
	//
	TheAlarmTest.Session().AlarmAddWithNotification(TheSessionStatus, alarm);
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	TheAlarmTest(TheSessionStatus == KRequestPending, __LINE__);
	testOtherNotify(ETrue);
	testThisNotify(EAlarmChangeEventAlarmAddition);
	//
	TAlarmId id;
	TASShdAlarm retrievedAlarm;
	TInt r;
	//
	r = TheAlarmTest.Session().GetNextDueAlarmId(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(id == alarm.Id(), __LINE__);
	r = TheAlarmTest.Session().GetAlarmDetails(alarm.Id(), retrievedAlarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(retrievedAlarm.Id() == alarm.Id(), __LINE__);
	//
	TheAlarmTest.Session().AlarmNotificationCancelAndDequeue(alarm.Id());
	testOtherNotify(ETrue);
	TheAlarmTest(TheSessionStatus==KErrCancel, __LINE__);
	testThisNotify(EAlarmChangeEventAlarmDeletion);
	//
	r = TheAlarmTest.Session().GetNextDueAlarmId(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(r == KNullAlarmId, __LINE__);
	//
	alarm.Reset();
	alarm.Message() = _L("Clock");
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalDays(1);
	alarm.NextDueTime() += TTimeIntervalMinutes(30);
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	testOtherNotify(ETrue);
	testThisNotify(EAlarmChangeEventAlarmAddition);
	//
	r = TheAlarmTest.Session().GetNextDueAlarmId(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(id == alarm.Id(), __LINE__);
	r = TheAlarmTest.Session().GetAlarmDetails(alarm.Id(), retrievedAlarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(retrievedAlarm.Id() = alarm.Id(), __LINE__);
	//
	r = TheAlarmTest.Session().SetAlarmStatus(alarm.Id(), EAlarmStatusDisabled);
	TheAlarmTest(r == KErrNone, __LINE__);
	testOtherNotify(ETrue);
	testThisNotify(EAlarmChangeEventStatus);
	//
	r = TheAlarmTest.Session().AlarmDelete(alarm.Id());
	testOtherNotify(ETrue);
	testThisNotify(EAlarmChangeEventAlarmDeletion);
	}


static void testSoundChange()
//
//	Test that sound notifications work
//
	{
	TheAlarmTest.Test().Next(_L("Sound changes"));
	testOtherNotify(EFalse);
	testThisNotify(KRequestPending);
	TheAlarmTest.Session().CancelAlarmSilence();
	testThisNotify(KRequestPending);
	testOtherNotify(EFalse);
	TheAlarmTest.Session().SetAlarmSoundsSilentFor(60);
	testThisNotify(EAlarmChangeEventSoundSilence);
	testOtherNotify(ETrue);
	TheAlarmTest.Session().CancelAlarmSilence();
	testThisNotify(EAlarmChangeEventSoundSilence);
	testOtherNotify(ETrue);
	TheAlarmTest.Session().SetAlarmSoundsSilentFor(60);
	testThisNotify(EAlarmChangeEventSoundSilence);
	testOtherNotify(ETrue);
	TheAlarmTest.Session().SetAlarmSoundsSilentFor(0);
	testThisNotify(EAlarmChangeEventSoundSilence);
	testOtherNotify(ETrue);
	TheAlarmTest.Session().SetAlarmSoundState(EAlarmGlobalSoundStateOn);
	testThisNotify(KRequestPending);
	testOtherNotify(EFalse);
	TheAlarmTest.Session().SetAlarmSoundState(EAlarmGlobalSoundStateOff);
	testThisNotify(EAlarmChangeEventGlobalSoundStateChanged);
	testOtherNotify(ETrue);
	TheAlarmTest.Session().SetAlarmSoundState(EAlarmGlobalSoundStateOn);
	testThisNotify(EAlarmChangeEventGlobalSoundStateChanged);
	testOtherNotify(ETrue);
	}


static void testTimeChangeUtcL()
	{
	RPIMTestServer serv;
    User::LeaveIfError(serv.Connect());

	TheAlarmTest.Test().Next(_L("Time changes and alarms set with UTC time."));
	//
	testOtherNotify(EFalse);
	testThisNotify(KRequestPending);
	//
	testThisNotify(KRequestPending);
	testOtherNotify(EFalse);
	//
	TTime home;
	home.HomeTime();
	home+=TTimeIntervalMinutes(2);

    serv.SetHomeTime(home);

	User::After(KTimeToWait);
	//
	testThisNotify(EAlarmChangeEventSystemDateTimeChanged);
	testOtherNotify(ETrue);
	testOtherNotify(EFalse);
	testThisNotify(KRequestPending);
	//
	TASShdAlarm alarm;
	alarm.Message()=(_L("Time"));
	TTime time;
	time.UniversalTime();
	time += TTimeIntervalMinutes(30);
	alarm.SetUtcNextDueTime(time);
	TheAlarmTest.Session().AlarmAddWithNotification(TheSessionStatus, alarm);
	TheAlarmTest(TheSessionStatus == KRequestPending, __LINE__);
	//
	testOtherNotify(ETrue);
	testThisNotify(EAlarmChangeEventAlarmAddition);
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(TheCurrentRequest, TheAlarmId);
	//
	TInt r;
	TAlarmId nextDueAlarmId;
	//
	r = TheAlarmTest.Session().GetNextDueAlarmId(nextDueAlarmId);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(nextDueAlarmId = alarm.Id(), __LINE__);
	//
	TASShdAlarm retrievedAlarm;
	r = TheAlarmTest.Session().GetAlarmDetails(nextDueAlarmId, retrievedAlarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(retrievedAlarm.Message() == _L("Time"), __LINE__);
	//
    User::SetUTCTime(retrievedAlarm.NextDueTime());

    serv.Close();

	User::After(KTimeToWait);
	//
	testOtherNotify(ETrue);
	testThisNotify(EAlarmChangeEventSystemDateTimeChanged);
	TheAlarmTest(TheSessionStatus != KRequestPending, __LINE__);
	User::WaitForRequest(TheSessionStatus);
	//
	r = TheAlarmTest.Session().GetNextDueAlarmId(nextDueAlarmId);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(nextDueAlarmId == KNullAlarmId, __LINE__);
	//
	TheAlarmTest.Session().AlarmNotificationCancelAndDequeue(alarm.Id());
	testThisNotify(EAlarmChangeEventAlarmDeletion);
	testOtherNotify(ETrue);
	//
	time = alarm.NextDueTime();
	time += TTimeIntervalDays(1);
	alarm.SetUtcNextDueTime(time);
	alarm.Message()=(_L("DayTime"));
	TheAlarmTest.Session().AlarmAddWithNotification(TheSessionStatus, alarm);
	testThisNotify(EAlarmChangeEventAlarmAddition);
	//
	TheAlarmTest.Session().AlarmNotificationCancelAndDequeue(alarm.Id());
	testThisNotify(EAlarmChangeEventAlarmDeletion);
	//
	TheAlarmTest.Session().AlarmAddWithNotification(TheSessionStatus, alarm);
	testThisNotify(EAlarmChangeEventAlarmAddition);
	testOtherNotify(ETrue);
	//
	TheAlarmTest.Session().AlarmNotificationCancelAndDequeue(alarm.Id());
	testThisNotify(EAlarmChangeEventAlarmDeletion);
	//
	TheAlarmTest(TheSessionStatus==KErrCancel, __LINE__);
	testOtherNotify(ETrue);
	}


static void testSetAndCancelFutureUtc()
	{
	TheAlarmTest.Test().Next(_L("Set alarms with UTC time in future"));
	TheAlarmTest(TheNotifyFlag==EFalse, __LINE__);
	//
	TASShdAlarm alarm;
	alarm.Message()=(_L("Future"));
	TTime time;
	time.UniversalTime();
	time += TTimeIntervalDays(1);
	time += TTimeIntervalMinutes(30);
	alarm.SetUtcNextDueTime(time);
	//
	testOtherNotify(EFalse);
	testThisNotify(KRequestPending);
	//
	TheAlarmTest.Session().AlarmAddWithNotification(TheSessionStatus, alarm);
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	TheAlarmTest(TheSessionStatus == KRequestPending, __LINE__);
	testOtherNotify(ETrue);
	testThisNotify(EAlarmChangeEventAlarmAddition);
	//
	TAlarmId id;
	TASShdAlarm retrievedAlarm;
	TInt r;
	//
	r = TheAlarmTest.Session().GetNextDueAlarmId(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(id == alarm.Id(), __LINE__);
	r = TheAlarmTest.Session().GetAlarmDetails(alarm.Id(), retrievedAlarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(retrievedAlarm.Id() == alarm.Id(), __LINE__);
	//
	TheAlarmTest.Session().AlarmNotificationCancelAndDequeue(alarm.Id());
	testOtherNotify(ETrue);
	TheAlarmTest(TheSessionStatus==KErrCancel, __LINE__);
	testThisNotify(EAlarmChangeEventAlarmDeletion);
	//
	r = TheAlarmTest.Session().GetNextDueAlarmId(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(r == KNullAlarmId, __LINE__);
	//
	alarm.Reset();
	alarm.Message()=(_L("Clock"));
	time.UniversalTime();
	time += TTimeIntervalDays(1);
	time += TTimeIntervalMinutes(30);
	alarm.SetUtcNextDueTime(time);
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	testOtherNotify(ETrue);
	testThisNotify(EAlarmChangeEventAlarmAddition);
	//
	r = TheAlarmTest.Session().GetNextDueAlarmId(id);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(id == alarm.Id(), __LINE__);
	r = TheAlarmTest.Session().GetAlarmDetails(alarm.Id(), retrievedAlarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(retrievedAlarm.Id() == alarm.Id(), __LINE__);
	//
	r = TheAlarmTest.Session().SetAlarmStatus(alarm.Id(), EAlarmStatusDisabled);
	TheAlarmTest(r == KErrNone, __LINE__);
	testOtherNotify(ETrue);
	testThisNotify(EAlarmChangeEventStatus);
	//
	r = TheAlarmTest.Session().AlarmDelete(alarm.Id());
	testOtherNotify(ETrue);
	testThisNotify(EAlarmChangeEventAlarmDeletion);
	}
	
static void testSetAndCancelPastUtc()
	{
	TheAlarmTest.Test().Next(_L("Set alarms with UTC time in past"));
	testOtherNotify(EFalse);
	//
	TASShdAlarm alarm;
	alarm.Message()=(_L("Past"));
	alarm.SoundName()=(_L("none"));
	TTime time;
	time.UniversalTime();
	time += TTimeIntervalDays(-2);
	alarm.SetUtcNextDueTime(time);
	//
	testThisNotify(KRequestPending);
	testOtherNotify(EFalse);
	//
	TInt r;
	//
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	TAlarmId clockAlarmId = alarm.Id();
	//
	testThisNotify(EAlarmChangeEventAlarmAddition);
	testOtherNotify(ETrue);
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(TheCurrentRequest, TheAlarmId);
	//
	testThisNotify(KRequestPending);
	testOtherNotify(EFalse);
	testThisNotify(KRequestPending);
	//
	TheAlarmTest.Session().AlarmAddWithNotification(TheSessionStatus, alarm);
	TheAlarmTest(alarm.Id() != KNullAlarmId, __LINE__);
	testOtherNotify(ETrue);
	testThisNotify(EAlarmChangeEventAlarmAddition);
	TheAlarmTest(TheSessionStatus == KErrNone, __LINE__);
	User::WaitForRequest(TheSessionStatus);
	//
	TheAlarmTest.Session().AlarmNotificationCancelAndDequeue(alarm.Id());
	testThisNotify(EAlarmChangeEventAlarmDeletion);
	testOtherNotify(ETrue);
	//
	r = TheAlarmTest.Session().AlarmDelete(clockAlarmId);
	TheAlarmTest(r == 0, __LINE__);
	testThisNotify(EAlarmChangeEventAlarmDeletion);
	testOtherNotify(ETrue);
	}


static void doTestsL()
	{
	const TInt KGmtOffset = 3600;
	_LIT(KTime, "20040730:050000.000000"); //Summertime
	TInt err = User::SetUTCTimeAndOffset(TTime(KTime), KGmtOffset);
	TheAlarmTest(err == KErrNone, __LINE__);

	TheAlarmTest.TestStartServers();
	__UHEAP_FAILNEXT(1);
	TheAlarmTest(TheAlarmTest.Session().Connect() == KErrNone, __LINE__);
	__UHEAP_RESET;

	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	//
	TheAlarmTest.TestClearStoreL();
	testSetupNotify();
	testSoundChange();

	// Alarms set with local wall-clock times.
	testTimeChangeL();
	testSetAndCancelFuture();
	testSetAndCancelPast();

	// Alarms set with UTC times.
	testTimeChangeUtcL();
	testSetAndCancelFutureUtc();
	testSetAndCancelPastUtc();

	CleanupStack::PopAndDestroy(scheduler);

	TheAlarmTest.TestClearStoreL();
	}

/**
@SYMTestCaseID PIM-TALARMNOTIFICATIONS-0001
*/	
GLDEF_C TInt E32Main()
//
// Test the alarm server.
//
    {
    TInt ret = KErrNone;
	TheAlarmTest.Title();
	TheAlarmTest.Start(_L("@SYMTestCaseID PIM-TALARMNOTIFICATIONS-0001 Session alarms"));

	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	TRAPD(err, doTestsL());
	TheAlarmTest(err == KErrNone, __LINE__);
	delete cleanup;
	__UHEAP_MARKEND;

    TheAlarmTest.Session().Close();
    TRAP(ret,TheAlarmTest.EndL());
	TheAlarmTest.Test().Close();
	ASTstAlarmTest::Close();
	//
	return ret;
    }
