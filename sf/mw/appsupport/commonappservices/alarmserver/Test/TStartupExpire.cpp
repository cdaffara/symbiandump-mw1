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

#if defined(__WINS__)
#include "consoleantestclient.h" //for RASAltClientSession
// Globals
static RANTestClient TheAlertSrv;
#endif
TLocale locale;
//
// Test setting an alarm, closing down the alarm server,
// re-starting the server and seeing the alarm expire if the
// re-start is within 59 seconds of the intended expiry
//

// Literal constants
_LIT(KFormatTime,"%H:%T:%S.   ");
_LIT(KSeparator,": ");
_LIT(KCarriageReturn,"\n\r");

#if defined(_DEBUG) && defined(__WINS__)
static TBool IsNotifying()
	{
	User::After(1000000);
	return TheAlertSrv.IsNotifying();
	}
#endif

static void closeServers()
	{
	const TInt r = TheAlarmTest.Session().Connect();
	TheAlarmTest(r==KErrNone, __LINE__);
	TheAlarmTest.Test().Printf(_L("Server shut down\r\n"));
#if !defined(_DEBUG)
	TheAlarmTest.Test().Printf(_L("THIS TEST IS A DEBUG TEST ONLY\r\n"));
#else
#ifdef __WINS__
	TheAlarmTest.Test().Printf(_L("now closing server...\r\n"));
	TheAlarmTest.Session().__DbgShutDownServer();
    TheAlarmTest.Session().Close();
#endif
#endif
	}

static void logTimeL(const TDesC& aText,const TTime& aTime)
	{
	TBuf<128> timeTxt;
	aTime.FormatL(timeTxt,KFormatTime);
	TheAlarmTest.Test().Printf(aText);
	TheAlarmTest.Test().Printf(KSeparator);
	TheAlarmTest.Test().Printf(timeTxt);
	TheAlarmTest.Test().Printf(KCarriageReturn);
	}

static void logTimeSameLineL(const TDesC& aText,const TTime& aTime)
	{
	TPoint cursor=TheAlarmTest.Test().Console()->CursorPos();
	cursor.iX=0;
	TheAlarmTest.Test().Console()->SetCursorPosAbs(cursor);
	TheAlarmTest.Test().Console()->ClearToEndOfLine();
	TBuf<128> timeTxt;
	aTime.FormatL(timeTxt,KFormatTime);
	TheAlarmTest.Test().Printf(aText);
	TheAlarmTest.Test().Printf(KSeparator);
	TheAlarmTest.Test().Printf(timeTxt);
	}

void JumpJustBeforeL(const TTime& aDueTime)
	{
	TTime jumpTo = aDueTime - TTimeIntervalSeconds(5);

    RPIMTestServer serv;
    User::LeaveIfError(serv.Connect());
    serv.SetHomeTime(jumpTo);
    // wait for time change notifications to occur.
    User::After(10000);
    // set again to ensure time value is correct when update occurs.
    serv.SetHomeTime(jumpTo);
    serv.Close();

	logTimeSameLineL(_L("System time is set to: "), jumpTo);
	TheAlarmTest.Test().Printf(KCarriageReturn);

	jumpTo.HomeTime();
	}

static void tickUntilTimeL(const TTime& aStopTime)
	{
	TheAlarmTest.Test().Printf(_L("Entering tickUntilTime\n"));
	RTimer tickTimer;
	tickTimer.CreateLocal();
	TTime now;
	now.HomeTime();
	TInt loop=0;
	TheAlarmTest.Test().Printf(_L("Begin Loop\n"));
	while (now<aStopTime)
		{
		TTime nowx(now.Int64());
		TTime stopx(aStopTime.Int64());
		TheAlarmTest.Test().Printf(_L("\nStart of Loop %d\n"),++loop);
		TheAlarmTest.Test().Printf(_L("now<aStopTime %d\n"),now<aStopTime);
		TheAlarmTest.Test().Printf(_L("now>aStopTime %d\n"),now>aStopTime);
		TheAlarmTest.Test().Printf(_L("now<=aStopTime %d\n"),now<=aStopTime);
		TheAlarmTest.Test().Printf(_L("now>=aStopTime %d\n"),now>=aStopTime);
		logTimeL(_L("---Nowx at"),nowx);
		logTimeL(_L("---Stopx at"),stopx);
		TheAlarmTest.Test().Printf(_L("nowx<stopx %d\n"),nowx<stopx);

		TheAlarmTest.Test().Printf(_L("\nNow1 TInt64 %d\n"),now.Int64());
		TheAlarmTest.Test().Printf(_L("Stop1 TInt64 %d\n\n"),aStopTime.Int64());
		logTimeSameLineL(_L("Time is"),now);
		//TheAlarmTest.Test().Printf(_L("Now2 TInt64 %d\n"),now.Int64());
		//TheAlarmTest.Test().Printf(_L("Stop2 TInt64 %d\n"),aStopTime.Int64());
		TRequestStatus status;
		tickTimer.After(status,1000000);
		User::WaitForRequest(status);
		//TheAlarmTest.Test().Printf(_L("Now3 TInt64 %d\n"),now.Int64());
		//TheAlarmTest.Test().Printf(_L("Stop3 TInt64 %d\n"),aStopTime.Int64());
		now.HomeTime();
		//TheAlarmTest.Test().Printf(_L("Now4 TInt64 %d\n"),now.Int64());
		//TheAlarmTest.Test().Printf(_L("Stop4 TInt64 %d\n"),aStopTime.Int64());
		logTimeL(_L("\nDebug now      :"),now);
		logTimeL(_L("Debug aStopTime:"),aStopTime);
		TheAlarmTest.Test().Printf(_L("Now5 TInt64 %d\n"),now.Int64());
		TheAlarmTest.Test().Printf(_L("Stop5 TInt64 %d\n"),aStopTime.Int64());

		nowx=now;
		stopx=aStopTime;

		TheAlarmTest.Test().Printf(_L("\nnow<aStopTime %d\n"),now<aStopTime);
		TheAlarmTest.Test().Printf(_L("now>aStopTime %d\n"),now>aStopTime);
		TheAlarmTest.Test().Printf(_L("now<=aStopTime %d\n"),now<=aStopTime);
		TheAlarmTest.Test().Printf(_L("now>=aStopTime %d\n"),now>=aStopTime);
		TheAlarmTest.Test().Printf(_L("Nowx TInt64 %d\n"),nowx.Int64());
		TheAlarmTest.Test().Printf(_L("Stopx TInt64 %d\n"),stopx.Int64());
		TheAlarmTest.Test().Printf(_L("nowx<stopx %d\n"),nowx<stopx);
		TheAlarmTest.Test().Printf(_L("End of Loop %d\n\n"),loop);
		}
	TheAlarmTest.Test().Printf(_L("leaving tickUntilTime\n"));
	}

static void finishTestL(TTime& aExpireTime, const TBool aStartServerWithinMinute)
	{
	closeServers();
	//
	if	(aStartServerWithinMinute)
		aExpireTime += TTimeIntervalSeconds(5);
	else
		{
		aExpireTime += TTimeIntervalSeconds(60);
		JumpJustBeforeL(aExpireTime);
		}
	//
	logTimeL(_L("Starting servers at"),aExpireTime);
	tickUntilTimeL(aExpireTime);
	//
	TheAlarmTest.TestStartServers();
	//
	if	(aStartServerWithinMinute)
		{
		TheAlarmTest.Test().Printf(_L("You SHOULD see an Alert!\n\r"));
#if defined(_DEBUG) && defined(__WINS__)
		TheAlarmTest(IsNotifying());
#endif
		}
	else
		{
		TheAlarmTest.Test().Printf(_L("You SHOULD NOT see an Alert!\n\r"));
#if defined(_DEBUG) && defined(__WINS__)
		TheAlarmTest(!IsNotifying());
#endif
		}
	//

	TheAlarmTest.TestClearStoreL();
	closeServers();
	}


static void startTestUtcL(TASShdAlarm& aAlarm, TAlarmMessage& aMessage, TInt aTestNumber)
	{
	TheAlarmTest.Test().Console()->ClearScreen();
	TheAlarmTest.Test().Printf(_L("Starting test [%2d/14]\n\r"), aTestNumber);

	TheAlarmTest.TestStartServers();
	//
	TInt r=TheAlarmTest.Session().Connect();
	TheAlarmTest(r==KErrNone, __LINE__);
	TheAlarmTest.TestClearStoreL();
	//
	TTime expireTime;
	expireTime.UniversalTime();
	TDateTime expireDateTime=expireTime.DateTime();
	logTimeL(_L("UTC Time now is"), expireTime);
	//
	expireTime.RoundUpToNextMinute();
	if	(expireDateTime.Second() > 40)
		expireTime += TTimeIntervalMinutes(1);
	//
	expireTime.FormatL(aMessage, _L("expire time %H:%T:%S.%C:"));
    // Note, JumpJustBefore takes a Floating Time and not a Universal Time.
	JumpJustBeforeL(expireTime + User::UTCOffset());
	aAlarm.SetUtcNextDueTime(expireTime);
	}

static void startTestL(TTime& aExpireTime, TAlarmMessage& aMessage, TInt aTestNumber)
	{
	TheAlarmTest.Test().Console()->ClearScreen();
	TheAlarmTest.Test().Printf(_L("Starting test [%2d/14]\n\r"), aTestNumber);

	TheAlarmTest.TestStartServers();
	//
	TInt r=TheAlarmTest.Session().Connect();
	TheAlarmTest(r==KErrNone, __LINE__);
	TheAlarmTest.TestClearStoreL();
	//
	aExpireTime.HomeTime();
	TDateTime expireDateTime=aExpireTime.DateTime();
	logTimeL(_L("Time now is"), aExpireTime);
	//
	aExpireTime.RoundUpToNextMinute();
	if	(expireDateTime.Second() > 40)
		aExpireTime += TTimeIntervalMinutes(1);
	//
	aExpireTime.FormatL(aMessage, _L("expire time %H:%T:%S.%C:"));

	JumpJustBeforeL(aExpireTime);
	}

static void clockAlarmTestL(TAlarmRepeatDefinition aRepeatType, TBool aStartServerWithinMinute, TInt aNumber)
// aRepeatType - standard repeat choice, allows testing of different style clock alarms
// aStartServerWithinMinute - whether to start the servers within the minute after expiry of the TheAlarmTest.Test() alarm
	{
	TASShdAlarm alarm;
	startTestL(alarm.NextDueTime(), alarm.Message(), aNumber);
	alarm.RepeatDefinition() = aRepeatType;
	TheAlarmTest.Test().Printf(_L("Setting clock alarm, type:%d\n\r"), aRepeatType);
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);

	logTimeL(_L("Set clock alarm to expire at"),alarm.NextDueTime());
	finishTestL(alarm.NextDueTime(),aStartServerWithinMinute);
	}

static void agendaAlarmTestL(TBool aStartServerWithinMinute, TInt aNumber)
// aSessionType - standard session choice, allows testing of different style agenda alarms
// aStartServerWithinMinute - whether to start the servers within the minute after expiry of the TheAlarmTest.Test() alarm
	{
	TASShdAlarm alarm;
	startTestL(alarm.NextDueTime(), alarm.Message(), aNumber);
	TheAlarmTest.Test().Printf(_L("Setting agenda alarm\n\r"));
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);

	logTimeL(_L("Set agenda alarm to expire at"),alarm.NextDueTime());

	finishTestL(alarm.NextDueTime(),aStartServerWithinMinute);
	}

static void orphanAlarmTestL(TBool aStartServerWithinMinute, TInt aNumber)
// aSessionType - standard session choice, allows testing of different style orphan alarms
// aStartServerWithinMinute - whether to start the servers within the minute after expiry of the TheAlarmTest.Test() alarm
	{
	TASShdAlarm alarm;
	alarm.Characteristics().Set(EAlarmCharacteristicsSessionSpecific);
	startTestL(alarm.NextDueTime(), alarm.Message(), aNumber);

	TheAlarmTest.Test().Printf(_L("Setting session alarm\n\r"));

	// Set the session alarm
	TRequestStatus status;
	TheAlarmTest.Session().AlarmAddWithNotification(status, alarm);

	// Now orphan it
	TheAlarmTest.Test().Printf(_L("Session alarm set, now orphaning\n\r"));
	TheAlarmTest.Session().SetAlarmCharacteristics(alarm.Id(), 0); // clear session specific flag

	// Get the abort
	User::WaitForRequest(status);
	TheAlarmTest(status.Int() == KErrCancel, __LINE__);

	logTimeL(_L("Created orphan alarm to expire at"),alarm.NextDueTime());

	finishTestL(alarm.NextDueTime(),aStartServerWithinMinute);
	}

void serverRestartWithNotifyingAlarmTestL()
	{
#if defined(_DEBUG) && defined(__WINS__)
	TheAlarmTest.TestStartServers();
	TheAlarmTest.TestClearStoreL();
	TheAlarmTest(TheAlertSrv.Connect() == KErrNone);
	// Add an alarm due immediately
	TASShdAlarm alarm;
	TTime time;
	time.HomeTime();
	alarm.NextDueTime() = time;
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone);
	TAlarmId id = alarm.Id();
	// Check that the alarm is notifying
	TheAlarmTest(IsNotifying());
	// Shut down and restart the server
	closeServers();
	TheAlarmTest.TestStartServers();
	TheAlarmTest(TheAlarmTest.Session().Connect() == KErrNone);
	// Check that the alarm is still notifying
	TheAlarmTest.Test().Printf(_L("Check alarm still notifying after shutdown and restart\n"));
	TheAlarmTest(IsNotifying());
	// Snooze the alarm, and restart the server
	TTime timeToSnoozeUntil = time + TTimeIntervalMinutes(1);
	TheAlarmTest(TheAlarmTest.Session().__DbgSnoozeAlarm(id, timeToSnoozeUntil - User::UTCOffset()) == KErrNone);
	TheAlarmTest(!IsNotifying());
	closeServers();
	TheAlarmTest.TestStartServers();
	TheAlarmTest(TheAlarmTest.Session().Connect() == KErrNone);
	// Check that the alarm is still snoozed
	TheAlarmTest(!IsNotifying());
	// Check the alarm notifies again after the snooze period
	User::At(timeToSnoozeUntil);
	TheAlarmTest.Test().Printf(_L("Check snoozed alarm notifies after shutdown and restart\n"));
	TheAlarmTest(IsNotifying());
	// Snooze the alarm again and shut down the server -
	// Restart the server 3 months in the future
	timeToSnoozeUntil.HomeTime();
	timeToSnoozeUntil += TTimeIntervalMinutes(1);
	TheAlarmTest(TheAlarmTest.Session().__DbgSnoozeAlarm(id, timeToSnoozeUntil - User::UTCOffset()) == KErrNone);
	TheAlarmTest(!IsNotifying());
	closeServers();

    RPIMTestServer serv;
    User::LeaveIfError(serv.Connect());
    serv.SetHomeTime(timeToSnoozeUntil + TTimeIntervalMonths(3));
    serv.Close();

	TheAlarmTest.TestStartServers();
	TheAlarmTest(TheAlarmTest.Session().Connect() == KErrNone);
	// Check the alarm is notifying
	TheAlarmTest.Test().Printf(_L("Check snoozed alarm notifies again after shutdown and restart\n"));
	TheAlarmTest(IsNotifying());

	TheAlertSrv.Close();
#else
	TheAlarmTest.Test().Printf(_L("THIS TEST IS A DEBUG, WINS TEST ONLY\r\n"));
#endif
	}

// UTC tests


static void clockUtcAlarmTestL(TAlarmRepeatDefinition aRepeatType, TBool aStartServerWithinMinute, TInt aNumber)
// aRepeatType - standard repeat choice, allows testing of different style clock alarms
// aStartServerWithinMinute - whether to start the servers within the minute after expiry of the TheAlarmTest.Test() alarm
	{
	TASShdAlarm alarm;
	TBuf<KMaxAlarmMessageLength> message = alarm.Message();
	startTestUtcL(alarm, message, aNumber);
	alarm.RepeatDefinition()=aRepeatType;
	TheAlarmTest.Test().Printf(_L("Setting clock alarm, type:%d\n\r"), aRepeatType);
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);

	TTime time = alarm.NextDueTime() + User::UTCOffset();
	logTimeL(_L("Set clock alarm to expire at"), time);

	finishTestL(time, aStartServerWithinMinute);
	}

static void agendaUtcAlarmTestL(TBool aStartServerWithinMinute, TInt aNumber)
// aSessionType - standard session choice, allows testing of different style agenda alarms
// aStartServerWithinMinute - whether to start the servers within the minute after expiry of the TheAlarmTest.Test() alarm
	{
	TASShdAlarm alarm;
	TBuf<KMaxAlarmMessageLength> message = alarm.Message();
	startTestUtcL(alarm, message, aNumber);
	TheAlarmTest.Test().Printf(_L("Setting agenda alarm\n\r"));
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);

	TTime time = alarm.NextDueTime() + User::UTCOffset();
	logTimeL(_L("Set agenda alarm to expire at"), time);

	finishTestL(time, aStartServerWithinMinute);
	}

static void orphanUtcAlarmTestL(TBool aStartServerWithinMinute, TInt aNumber)
// aSessionType - standard session choice, allows testing of different style orphan alarms
// aStartServerWithinMinute - whether to start the servers within the minute after expiry of the TheAlarmTest.Test() alarm
	{
	TASShdAlarm alarm;
	alarm.Characteristics().Set(EAlarmCharacteristicsSessionSpecific);
	TBuf<KMaxAlarmMessageLength> message = alarm.Message();
	startTestUtcL(alarm, message, aNumber);

	TheAlarmTest.Test().Printf(_L("Setting session alarm\n\r"));

	// Set the session alarm
	TRequestStatus status;
	TheAlarmTest.Session().AlarmAddWithNotification(status, alarm);

	// Now orphan it
	TheAlarmTest.Test().Printf(_L("Session alarm set, now orphaning\n\r"));
	TheAlarmTest.Session().SetAlarmCharacteristics(alarm.Id(), 0); // clear session specific flag

	// Get the abort
	User::WaitForRequest(status);
	TheAlarmTest(status.Int() == KErrCancel, __LINE__);

	TTime time = alarm.NextDueTime() + User::UTCOffset();
	logTimeL(_L("Created orphan alarm to expire at"), time);

	finishTestL(time, aStartServerWithinMinute);
	}

void serverRestartWithNotifyingUtcAlarmTestL()
	{
#if defined(_DEBUG) && defined(__WINS__)
	TheAlarmTest.TestStartServers();
	TheAlarmTest.TestClearStoreL();
	TheAlarmTest(TheAlertSrv.Connect() == KErrNone);
	// Add an alarm due immediately
	TASShdAlarm alarm;
	TTime time;
	time.UniversalTime();
	alarm.SetUtcNextDueTime(time);
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone);
	TAlarmId id = alarm.Id();
	// Check that the alarm is notifying
	TheAlarmTest(IsNotifying());
	// Shut down and restart the server
	closeServers();
	TheAlarmTest.TestStartServers();
	TheAlarmTest(TheAlarmTest.Session().Connect() == KErrNone);
	// Check that the alarm is still notifying
	TheAlarmTest.Test().Printf(_L("Check alarm still notifying after shutdown and restart\n"));
	TheAlarmTest(IsNotifying());
	// Snooze the alarm, and restart the server
	TTime timeToSnoozeUntil = time + TTimeIntervalMinutes(1);
	TheAlarmTest(TheAlarmTest.Session().__DbgSnoozeAlarm(id, timeToSnoozeUntil) == KErrNone);
	TheAlarmTest(!IsNotifying());
	closeServers();
	TheAlarmTest.TestStartServers();
	TheAlarmTest(TheAlarmTest.Session().Connect() == KErrNone);
	// Check that the alarm is still snoozed
	TheAlarmTest(!IsNotifying());
	// Check the alarm notifies again after the snooze period
	// Note, User::At takes a Floating Time (wall-time) and not a Universal Time
	User::At(timeToSnoozeUntil+User::UTCOffset());
	TheAlarmTest.Test().Printf(_L("Check snoozed alarm notifies after shutdown and restart\n"));
	TheAlarmTest(IsNotifying());
	// Snooze the alarm again and shut down the server -
	// Restart the server 3 months in the future
	timeToSnoozeUntil.UniversalTime();
	timeToSnoozeUntil += TTimeIntervalMinutes(1);
	TheAlarmTest(TheAlarmTest.Session().__DbgSnoozeAlarm(id, timeToSnoozeUntil) == KErrNone);
	TheAlarmTest(!IsNotifying());
	closeServers();

    User::SetUTCTime(timeToSnoozeUntil + TTimeIntervalMonths(3));

	TheAlarmTest.TestStartServers();
	TheAlarmTest(TheAlarmTest.Session().Connect() == KErrNone);
	// Check the alarm is notifying
	TheAlarmTest.Test().Printf(_L("Check snoozed alarm notifies again after shutdown and restart\n"));
	TheAlarmTest(IsNotifying());

	TheAlertSrv.Close();
#else
	TheAlarmTest.Test().Printf(_L("THIS TEST IS A DEBUG, WINS TEST ONLY\r\n"));
#endif
	}

	
void serverRestartWithNotifiedUtcAlarmTestL()
	{
#if defined(_DEBUG) && defined(__WINS__)
    TheAlarmTest.Next(_L("Acknowledge repeating alarm after restart in less then 1 minute) does not notify"));	

    TheAlarmTest.TestStartServers();
    TheAlarmTest.TestClearStoreL();
    TheAlarmTest(TheAlertSrv.Connect() == KErrNone);

    // Add an alarm due in the future
    TheAlarmTest.Test().Printf(_L("Add a repeating alarm to expire in the future\n"));
    TASShdAlarm alarm;
    alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatDaily;	

    TTime alarmTime;
    alarmTime.UniversalTime();
    alarmTime.RoundUpToNextMinute();
    alarmTime += TTimeIntervalMinutes(1);

    alarm.SetUtcNextDueTime(alarmTime);

    TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone);
    TAlarmId id = alarm.Id();

    // wait until that time has occurred before checking
    TheAlarmTest.Test().Printf(_L("Wait for alarm time to expire\n"));
     
	User::At(alarmTime + User::UTCOffset());

	TheAlarmTest.Test().Printf(_L("Check that alarm is notifying\n"));
    TheAlarmTest(IsNotifying());
    TheAlarmTest.Test().Printf(_L("Acknowledge alarm\n"));
    TheAlertSrv.AcknowledgeAlarm();
    TheAlarmTest.Test().Printf(_L("Check that alarm is cleared\n"));
    TheAlarmTest(!IsNotifying());

    TheAlarmTest.Test().Printf(_L("Shutdown servers\n"));
    // Shut down and restart the server
    TheAlertSrv.Close();
    closeServers();

    TheAlarmTest.TestStartServers();
    TheAlarmTest(TheAlarmTest.Session().Connect() == KErrNone);
    TheAlarmTest(TheAlertSrv.Connect() == KErrNone);

    TheAlarmTest.Test().Printf(_L("Servers restarted\n"));

    // Make sure that server was restarted in the same minute as the alarm
    TTime currentTime;
    currentTime.UniversalTime();
    TTimeIntervalHours hours;
    TTimeIntervalMinutes minutes;

    TheAlarmTest.Test().Printf(_L("Check servers restarted in 1 minute\n"));
    TheAlarmTest(currentTime.HoursFrom(alarmTime, hours) == KErrNone);
    TheAlarmTest(currentTime.MinutesFrom(alarmTime, minutes) == KErrNone);
    TheAlarmTest((hours == TTimeIntervalHours(0)) && (minutes == TTimeIntervalMinutes(0)));


    // Check that the alarm is not notifying
    TheAlarmTest.Test().Printf(_L("Check alarm still not notifying after shutdown and restart\n"));
    TheAlarmTest(!IsNotifying());

    closeServers();
    TheAlertSrv.Close();
#else
    TheAlarmTest.Test().Printf(_L("THIS TEST IS A DEBUG, WINS TEST ONLY\r\n"));
#endif
	}

static void doTestsL()
	{
#if defined(_DEBUG) && defined(__WINS__)
	TheAlarmTest(TheAlertSrv.Connect() == KErrNone);
#endif

#if defined(_DEBUG)
// The alarm should only be allowed to display notification in WINS builds.
#if defined(__WINSCW__)
	TheAlarmTest.Session().__DbgPreventUserNotify(EFalse);
#else
	TheAlarmTest.Session().__DbgPreventUserNotify(ETrue);
#endif
#endif

	// Set all days to workdays.
	TLocale previous;
	locale.SetWorkDays(127);
	locale.Set(); 
	orphanAlarmTestL(ETrue, 1);
	orphanAlarmTestL(EFalse, 2);

	agendaAlarmTestL(ETrue, 3);
	agendaAlarmTestL(EFalse, 4);


	clockAlarmTestL(EAlarmRepeatDefintionRepeatWorkday,ETrue, 5);
	clockAlarmTestL(EAlarmRepeatDefintionRepeatWorkday,EFalse, 6);

	clockAlarmTestL(EAlarmRepeatDefintionRepeatWeekly,ETrue, 7);
	clockAlarmTestL(EAlarmRepeatDefintionRepeatWeekly,EFalse, 8);

	clockAlarmTestL(EAlarmRepeatDefintionRepeatDaily,ETrue, 9);
	clockAlarmTestL(EAlarmRepeatDefintionRepeatDaily,EFalse, 10);

	clockAlarmTestL(EAlarmRepeatDefintionRepeatNext24Hours,ETrue, 11);
	clockAlarmTestL(EAlarmRepeatDefintionRepeatNext24Hours,EFalse, 12);

	clockAlarmTestL(EAlarmRepeatDefintionRepeatOnce,ETrue, 13);
	clockAlarmTestL(EAlarmRepeatDefintionRepeatOnce,EFalse, 14);

#if defined(_DEBUG) && defined(__WINS__)
	TheAlertSrv.Close();
#endif

	serverRestartWithNotifyingAlarmTestL();

#if defined(_DEBUG) && defined(__WINS__)
	TheAlarmTest(TheAlertSrv.Connect() == KErrNone);
#endif 
	
	orphanUtcAlarmTestL(ETrue, 1);
	orphanUtcAlarmTestL(EFalse, 2);

	agendaUtcAlarmTestL(ETrue, 3);
	agendaUtcAlarmTestL(EFalse, 4);

	clockUtcAlarmTestL(EAlarmRepeatDefintionRepeatWorkday,ETrue, 5);
	clockUtcAlarmTestL(EAlarmRepeatDefintionRepeatWorkday,EFalse, 6);

	clockUtcAlarmTestL(EAlarmRepeatDefintionRepeatWeekly,ETrue, 7);
	clockUtcAlarmTestL(EAlarmRepeatDefintionRepeatWeekly,EFalse, 8);

	clockUtcAlarmTestL(EAlarmRepeatDefintionRepeatDaily,ETrue, 9);
	clockUtcAlarmTestL(EAlarmRepeatDefintionRepeatDaily,EFalse, 10);

	clockUtcAlarmTestL(EAlarmRepeatDefintionRepeatNext24Hours,ETrue, 11);
	clockUtcAlarmTestL(EAlarmRepeatDefintionRepeatNext24Hours,EFalse, 12);

	clockUtcAlarmTestL(EAlarmRepeatDefintionRepeatOnce,ETrue, 13);
	clockUtcAlarmTestL(EAlarmRepeatDefintionRepeatOnce,EFalse, 14);

#if defined(_DEBUG) && defined(__WINS__)
	TheAlertSrv.Close();
#endif

	serverRestartWithNotifyingUtcAlarmTestL();
	serverRestartWithNotifiedUtcAlarmTestL();

	// Reset the previous locale.
	previous.Set();
	}

/**
@SYMTestCaseID PIM-TSTARTUPEXPIRE-0001
*/	
GLDEF_C TInt E32Main()
    {
	__UHEAP_MARK;
	TInt ret = KErrNone;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if	(!cleanup)
		return KErrNoMemory;

	TheAlarmTest.Title();
	TheAlarmTest.Start(_L("@SYMTestCaseID PIM-TSTARTUPEXPIRE-0001"));
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
