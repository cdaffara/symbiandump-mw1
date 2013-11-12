// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "asclisoundplay.h"
#include "testserver.h"
#include <tz.h>
#include <e32property.h> 
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
#include <ssm/ssmstates.hrh>
#endif
#ifdef __WINS__
#include "consoleantestclient.h"
#endif

_LIT(KTestName, "@SYMTestCaseID:PIM-TWAKEUPALARMS-0001 Wakeup Alarms");
_LIT(KFormatDateTime, "%D%M%Y%/0%1%/1%2%/2%3%/3 %H:%T:%S.%C");
_LIT(KAlarmServerFile, "c:\\private\\101f5027\\AlarmServer.ini");
_LIT(KAlarmServerProcessNameMatch, "alarmserver.exe*");

const TTimeIntervalMicroSeconds32 KAlarmWaitToNotifyPeriod(500000);

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
_LIT8(KAlarmData, "sausage");

_LIT8(KTokyo, "Asia/Tokyo"); // No DST changes in Tokyo
_LIT8(KSydney, "Australia/Sydney"); // Large positive standard offset
#endif
_LIT8(KLondon, "Europe/London"); // Large positive standard offset
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
_LIT8(KChicago, "America/Chicago"); // Large negative standard offset

const TUid KAlarmServerUID = {0x101f5027};
const TInt KRTCValueKey = 200;
const TInt KSystemStateChangeKey = 301;
const TInt KSystemStateChangeAckKey = 302;

TAlarmCategory KAlarmCategory = {0x10101010};
#endif

class CAlarmTestApp : public CBase
	{
public:
	static CAlarmTestApp* NewLC();
	~CAlarmTestApp();
	
	void RunTestsL();
	
private: // the test cases

	void TestAlarmMissingHourRepeatOnce1L();
	void TestAlarmMissingHourRepeatOnce2L();
	void TestAlarmMissingHourRepeatOnce3L();
	void TestAlarmMissingHourRepeatDailyL();
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	void TestWakeupFlagPersistanceL();
	void TestSetWakeupSessionAlarmL();
	void TestDeleteWakeupAlarmL();
	void TestDisableWakeupAlarmL();
	void TestUnsetWakeupFlagL();
	void TestWakeupAlarmRobustnessL();
	
	void TestSnoozeWakeupAlarmL();
	
	void TestFixedAlarmL();
	
	void TestRTCSetOnDSTRolloverL();
	void TestSetRTCOnDeviceShutdownL();
	void TestRTCSetOnTimeZoneChangeL();
	void TestRTCSetOnSystemTimeChangeL();
	
	void TestHeadWakeupAlarmL();
	
	void TestWakeupAlarmTimeL();
	void DoTestWakeupAlarmTimeL(const TTime& aSystemLocalTime, const TTime& aAlarmTimeUTC, TTimeIntervalMinutes aNextUTCOffset, const TDesC8& aTimeZoneName);
	
	void TestReadOnlyInShutDownL();
	void TestAlarmFileNotWrittenOnTimeZoneChangeL();
	void TestAlarmFileNotWrittenOnSystemTimeChangeL();
		
	void TestWriteCommandsL(TASShdAlarm& aASShdAlarm, TInt aExpectedErrorCode);
	void TestReadCommandsL(TASShdAlarm& aASShdAlarm);
#endif
		
private:
	CAlarmTestApp();
	void ConstructL();

	// Utility methods
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	TASShdAlarm CreateAlarm(const TTime& aAlarmTime, TBool aWakeup);
#endif
	void AddAlarmL(TASShdAlarm& aAlarm);
	void DeleteAlarmL(TAlarmId aAlarmId);
	void DeleteAllAlarmsL();
	
	void TestAlarmNotifyL(TASShdAlarm& aAlarm, const TTime& aNotifyTime, TBool aExpectedToNotify);
	
	void SetTimeZoneL(const TDesC8& aTimeZoneName);
	void SetHomeTimeL(const TTime& aSystemLocalTime);
	
	void RestartAlarmServerL();
	
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	void TestRTCValueL(const TTime& aRTCValue);
#endif
	void PrintTimeL(const TTime& aTime, const TDesC& aPrefix);
	
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	void SendSsmSystemMessageL(TSsmMainSystemStates aSsmMessage);
#endif
	
private:
	RPIMTestServer iPIMTestServer;
	RTz iTz;
	CTzId* iSavedTzID;
#ifdef __WINS__
	RANTestClient iClient;
#endif
	};

CAlarmTestApp* CAlarmTestApp::NewLC()
	{
	CAlarmTestApp* self = new (ELeave) CAlarmTestApp();
	CleanupStack::PushL(self);
	self->ConstructL();
	//CleanupStack::Pop(self);
	return self;
	}
	
CAlarmTestApp::CAlarmTestApp()
	{
	}

void CAlarmTestApp::ConstructL()
	{
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	User::LeaveIfError(RProperty::Define(KAlarmServerUID, KRTCValueKey, RProperty::EByteArray, sizeof(TPckgC<TTime>) ) );
	User::LeaveIfError(RProperty::Define(KAlarmServerUID, KSystemStateChangeKey, RProperty::EInt));
	User::LeaveIfError(RProperty::Define(KAlarmServerUID, KSystemStateChangeAckKey, RProperty::EInt));
#endif
	
	// The time zone server must be running with auto dst updates enabled for this test to work 
	User::LeaveIfError(iTz.Connect());
	iTz.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOn);
	
	// save the current time zone
	iSavedTzID = iTz.GetTimeZoneIdL();
	User::LeaveIfError(iPIMTestServer.Connect());
	
	TheAlarmTest.TestStartServers();
	User::LeaveIfError(TheAlarmTest.Session().Connect());
	
	// Make sure the test starts without any alarms set
	DeleteAllAlarmsL();
	
#ifdef __WINS__
	User::LeaveIfError(iClient.Connect());
	User::LeaveIfError(iClient.SetExtendedMode());
#endif
	}

CAlarmTestApp::~CAlarmTestApp()
	{
#ifdef __WINS__
	iClient.UnsetExtendedMode();
	iClient.Close();
#endif
	
	iPIMTestServer.Close();
	
	// restore the saved time zone
	TRAP_IGNORE(SetTimeZoneL(iSavedTzID->TimeZoneNameID()));
	delete iSavedTzID;
	iTz.Close();
	}
	
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
void CAlarmTestApp::SendSsmSystemMessageL(TSsmMainSystemStates aSsmMessage)
	{
	TheAlarmTest.Test().Printf(_L("Entering system state %d"), aSsmMessage);

	RProperty ackProperty;
	User::LeaveIfError(ackProperty.Attach(KAlarmServerUID, KSystemStateChangeAckKey));
	TRequestStatus status;
	ackProperty.Subscribe(status);
	
	User::LeaveIfError(RProperty::Set(KAlarmServerUID, KSystemStateChangeKey, aSsmMessage));
	
	User::WaitForRequest(status);
	}
#endif

void CAlarmTestApp::SetHomeTimeL(const TTime& aSystemLocalTime)
	{
	PrintTimeL(aSystemLocalTime, _L("Set Home Time to : "));
	User::LeaveIfError(iTz.SetHomeTime(aSystemLocalTime));
	}

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
void CAlarmTestApp::TestRTCValueL(const TTime& aRTCValue)
	{
	TheAlarmTest.Test().Printf(_L("Test the RTC Value"));
	
	User::After(200000);
	TPckgBuf<TTime> rtcValuePckgBuf;
	RProperty::Get(KAlarmServerUID, KRTCValueKey, rtcValuePckgBuf);
	
	if (aRTCValue == Time::NullTTime())
		{
		TheAlarmTest.Test().Printf(_L("Check the RTC is not set"));
		}
	else
		{
		PrintTimeL(aRTCValue, _L("Check the RTC is set to : "));
		}
	
	if (rtcValuePckgBuf() == Time::NullTTime())
		{
		TheAlarmTest.Test().Printf(_L("The RTC is not set"));
		}
	else
		{
		PrintTimeL(rtcValuePckgBuf(), _L("The RTC is set to : "));
		}
		
	TheAlarmTest(rtcValuePckgBuf() == aRTCValue); 
	}

TASShdAlarm CAlarmTestApp::CreateAlarm(const TTime& aAlarmTime, TBool aWakeup)
	{
	TASShdAlarm alarm;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	_LIT(KMessage, "An alarm");
	alarm.Message() = KMessage;
	alarm.SetWakeup(aWakeup);
	
	if (aAlarmTime == Time::NullTTime())
		{
		alarm.NextDueTime().HomeTime();
		alarm.NextDueTime() += TTimeIntervalDays(1);
		}
	else
		{
		alarm.NextDueTime() = aAlarmTime;
		}
	
	return alarm;
	}
#endif

void CAlarmTestApp::AddAlarmL(TASShdAlarm& aAlarm)
	{
	TheAlarmTest.Test().Printf(_L("Adding an alarm"));
	TRequestStatus status;
	iPIMTestServer.FileServerNotifyChange(ENotifyAll, status, KAlarmServerFile);
	TInt addError = TheAlarmTest.Session().AlarmAdd(aAlarm);
	
	if (addError != KErrNone)
		{
		iPIMTestServer.FileServerNotifyChangeCancel();
		}
	else
		{
		TheAlarmTest.Test().Printf(_L("Added alarm %d"), aAlarm.Id());
		}

	User::WaitForRequest(status);
	User::LeaveIfError(addError);
	}

void CAlarmTestApp::DeleteAlarmL(TAlarmId aAlarmId)
	{
	TheAlarmTest.Test().Printf(_L("Deleting alarm %d"), aAlarmId);
	TRequestStatus status;
	iPIMTestServer.FileServerNotifyChange(ENotifyAll, status, KAlarmServerFile);
	TheAlarmTest.Session().AlarmDelete(aAlarmId);
	RTimer timeoutTimer;
	User::LeaveIfError(timeoutTimer.CreateLocal());
	TRequestStatus timeoutTimerStatus;
	timeoutTimer.After(timeoutTimerStatus, 2000000);
	User::WaitForRequest(status, timeoutTimerStatus);
	timeoutTimer.Cancel();
	iPIMTestServer.FileServerNotifyChangeCancel();
	User::WaitForRequest(status, timeoutTimerStatus);
	}

void CAlarmTestApp::SetTimeZoneL(const TDesC8& aTimeZoneName)
	{
	TBuf<50> timeZoneName;
	timeZoneName.Copy(aTimeZoneName);
	TheAlarmTest.Test().Printf(_L("Changing TimeZone to : %S"), &timeZoneName);
	
	CTzId* tzID = CTzId::NewL(aTimeZoneName);
	CleanupStack::PushL(tzID);
	iTz.SetTimeZoneL(*tzID);
	CleanupStack::PopAndDestroy(tzID);
	}

void CAlarmTestApp::PrintTimeL(const TTime& aTime, const TDesC& aPrefix)
	{
	TFileName dateString;
	aTime.FormatL(dateString, KFormatDateTime);
	TheAlarmTest.Test().Printf(_L("%S %S\n"),&aPrefix, &dateString);
	}

void CAlarmTestApp::DeleteAllAlarmsL()
	{
	RArray<TAlarmId> alarmIds;
	CleanupClosePushL(alarmIds);
	TheAlarmTest.Session().GetAlarmIdListL(alarmIds);
	
	const TInt KAlarmCount(alarmIds.Count());
	for (TInt i(0) ; i < KAlarmCount ; ++i )
		{
		TheAlarmTest.Test().Printf(_L("deleting an alarm"));
		User::LeaveIfError(TheAlarmTest.Session().AlarmDelete(alarmIds[i]));
		}
	
	CleanupStack::PopAndDestroy(&alarmIds);
	}

void CAlarmTestApp::RestartAlarmServerL()
	{
	TheAlarmTest.Test().Printf(_L("Restarting the alarm server"));
	TheAlarmTest.Session().Close();
	
	iPIMTestServer.KillProcessL(KAlarmServerProcessNameMatch);
	
	User::LeaveIfError(TheAlarmTest.Session().Connect());
	}

#ifndef __WINS__
void CAlarmTestApp::TestAlarmNotifyL(TASShdAlarm& /*aAlarm*/, const TTime& /*aNotifyTime*/, TBool /*aExpectedToNotify*/)
	{
	TheAlarmTest.Test().Printf(_L("This cannot be tested on hardware"));
#else
void CAlarmTestApp::TestAlarmNotifyL(TASShdAlarm& aAlarm, const TTime& aNotifyTime, TBool aExpectedToNotify)
	{
	aExpectedToNotify?
	TheAlarmTest.Test().Printf(_L("Test that the alarm will notify")):
	TheAlarmTest.Test().Printf(_L("Test that the alarm will NOT notify"));
	
	TheAlarmTest.Test().Printf(_L("Close the alarm server"));
	TheAlarmTest.Session().Close();
	iPIMTestServer.KillProcessL(KAlarmServerProcessNameMatch);
	
	SetHomeTimeL(aNotifyTime);	
	
	// start observing the alarm notification
	TRequestStatus alarmStatus;
	iClient.NotifyOnAlarm(aAlarm, alarmStatus);
	
	TheAlarmTest.Test().Printf(_L("Start the alarm server"));
	User::LeaveIfError(TheAlarmTest.Session().Connect());
	
	// Create timeout timer
	RTimer timeoutTimer;
	User::LeaveIfError(timeoutTimer.CreateLocal());
	TRequestStatus timeoutTimerStatus;
	timeoutTimer.After(timeoutTimerStatus, KAlarmWaitToNotifyPeriod);
	
	// Wait for the alarm to expire
	User::WaitForRequest(alarmStatus, timeoutTimerStatus);
	
	if (alarmStatus.Int() == KErrNone)
		{
		TheAlarmTest.Test().Printf(_L("The alarm notified"));
		
		iClient.AcknowledgeAlarm(aAlarm.Id());
		timeoutTimer.Cancel();
		User::WaitForRequest(timeoutTimerStatus);
		
		TheAlarmTest(aExpectedToNotify);
		}
	else
		{
		TheAlarmTest.Test().Printf(_L("The alarm DID NOT notify"));
		iClient.CancelNotifications();
		User::WaitForRequest(alarmStatus);
		
		TheAlarmTest(!aExpectedToNotify);
		}
	
	// Make sure there are no other alarms notifying
	RArray<TAlarmId> alarmIds;
	CleanupClosePushL(alarmIds);
	TheAlarmTest.Session().GetAlarmIdListByStateL(EAlarmStateNotifying, alarmIds);
	TheAlarmTest(alarmIds.Count() == 0);
	CleanupStack::PopAndDestroy(&alarmIds);
#endif
	}

void CAlarmTestApp::TestAlarmMissingHourRepeatDailyL()
	{
	TheAlarmTest.Test().Next(_L("Test a repeating alarm in the missing hour"));
	SetTimeZoneL(KLondon);
	
	// Set home time to (before DST threshold)
	// Saturday, 29 March 2008, 00:12:00
	SetHomeTimeL(TDateTime(2008, EMarch, 28, 12, 0, 0, 0));	
	
	// Set a daily repeating alarm during the missing local hour
	// Sunday, 30 March 2008, 00:01:15
	TASShdAlarm alarm;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatDaily;
	alarm.NextDueTime() = TTime(TDateTime(2008, EMarch, 29, 1, 15, 0, 0));
	alarm.Message().Append(_L("Missing hour alarm"));
	
	AddAlarmL(alarm);
	
	TestAlarmNotifyL(alarm, TTime(TDateTime(2008, EMarch, 29, 2, 14, 30, 0)), EFalse);
	TestAlarmNotifyL(alarm, TTime(TDateTime(2008, EMarch, 29, 2, 15, 30, 0)), ETrue);
	
#ifdef __WINS__
	TheAlarmTest.Test().Printf(_L("make sure that the next repeat of the alarm is correct"));
	// Monday, 31 March 2008, 00:01:15
	TASShdAlarm fetchedAlarm;
	TheAlarmTest.Session().GetAlarmDetails(alarm.Id(), fetchedAlarm);
	
	TTime fetchedAlarmTime = fetchedAlarm.NextDueTime();
	TheAlarmTest(fetchedAlarmTime == TTime(TDateTime(2008, EMarch, 30, 1, 15, 0, 0)));
	
#endif // __WINS__
	
	// delete the alarm
	DeleteAlarmL(alarm.Id());
	}

/* Use case 1:
Missing hour is from 1am (local time) to 2am (local time).
Alarm is set for 1:15am (local time).
No other alarm is set.

The alarm will expire at 2:15 (local time) instead of 1:15 (local time).
*/
void CAlarmTestApp::TestAlarmMissingHourRepeatOnce1L()
	{
	TheAlarmTest.Test().Next(_L("Test an alarm in the missing hour"));
	SetTimeZoneL(KLondon);
	
	// set home time to (before DST threshold)
	// Saturday, 29 March 2008, 00:12:00
	SetHomeTimeL(TDateTime(2008, EMarch, 28, 12, 0, 0, 0));	
	
	// set an alarm during the missing local hour
	// Sunday, 30 March 2008, 00:01:15
	TASShdAlarm alarm;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	alarm.NextDueTime() = TTime(TDateTime(2008, EMarch, 29, 1, 15, 0, 0));
	alarm.Message().Append(_L("Missing hour alarm"));
	
	AddAlarmL(alarm);
	
	TestAlarmNotifyL(alarm, TTime(TDateTime(2008, EMarch, 29, 2, 14, 30, 0)), EFalse);
	TestAlarmNotifyL(alarm, TTime(TDateTime(2008, EMarch, 29, 2, 15, 30, 0)), ETrue);
	
	// delete the alarm
	DeleteAlarmL(alarm.Id());
	}

/** Use case 2:
Missing hour is from 1am (local time) to 2am (local time).
Alarm1 is set for 1:15am (local time)
Alarm2 is set for 1:45am (local time)

The alarms will expire at 2:15 (local time)  and 2:45 (local time).
*/
void CAlarmTestApp::TestAlarmMissingHourRepeatOnce2L()
	{
	TheAlarmTest.Test().Next(_L("Test two alarms in the missing hour"));
	SetTimeZoneL(KLondon);
	
	// set home time to (before DST threshold)
	// Saturday, 29 March 2008, 00:12:00
	SetHomeTimeL(TDateTime(2008, EMarch, 28, 12, 0, 0, 0));	
	
	// set an alarm during the missing local hour
	// Sunday, 30 March 2008, 00:01:15
	TASShdAlarm alarm1;
	alarm1.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	alarm1.NextDueTime() = TTime(TDateTime(2008, EMarch, 29, 1, 15, 0, 0));
	alarm1.Message().Append(_L("Missing hour alarm"));
	
	AddAlarmL(alarm1);
	
	// Set an alarm during the missing local hour
	// Sunday, 30 March 2008, 00:01:45
	TASShdAlarm alarm2;
	alarm2.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	alarm2.NextDueTime() = TTime(TDateTime(2008, EMarch, 29, 1, 45, 0, 0));
	alarm2.Message().Append(_L("Missing hour alarm"));
	
	AddAlarmL(alarm2);
	
	TestAlarmNotifyL(alarm1, TTime(TDateTime(2008, EMarch, 29, 2, 14, 30, 0)), EFalse);
	TestAlarmNotifyL(alarm1, TTime(TDateTime(2008, EMarch, 29, 2, 15, 30, 0)), ETrue);
	TestAlarmNotifyL(alarm2, TTime(TDateTime(2008, EMarch, 29, 2, 44, 30, 0)), EFalse);
	TestAlarmNotifyL(alarm2, TTime(TDateTime(2008, EMarch, 29, 2, 45, 30, 0)), ETrue);
	
	// delete the alarm
	DeleteAlarmL(alarm1.Id());
	DeleteAlarmL(alarm2.Id());
	}

/** Use case 3:
Missing hour  is from 1am (local time) to 2am (local time).
Alarm1 is set for 1:15am (local time).
Alarm2 is set for 2:05am (local time).
Alarm3 is set for 3:05am (local time).
Alarm1 is the only alarm that happens during the missing hour
but the presence of other alarms may complicate the behaviour as well.

The expiry times of the alarms become:
2:05 : Alarm2
2:15 : Alarm1
3:05 : Alarm3
Notice the change in the order of the alarms. This is because we remap
the alarms of the missing hour in the hour afterwards so between 2am and 3am
in our example we have the alarms for 1am to 3am and so they can get reordered.
Alarms after 3am are not impacted.
*/
void CAlarmTestApp::TestAlarmMissingHourRepeatOnce3L()
	{
	TheAlarmTest.Test().Next(_L("Test alarms changing order because of the missing hour"));
	SetTimeZoneL(KLondon);
	
	// set home time to (before DST threshold)
	// Saturday, 29 March 2008, 00:12:00
	SetHomeTimeL(TDateTime(2008, EMarch, 28, 12, 0, 0, 0));	
	
	// set an alarm during the missing local hour
	// Sunday, 30 March 2008, 00:01:15
	TASShdAlarm alarm1;
	alarm1.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	alarm1.NextDueTime() = TTime(TDateTime(2008, EMarch, 29, 1, 15, 0, 0));
	alarm1.Message().Append(_L("Missing hour alarm"));
	AddAlarmL(alarm1);
	
	// Set an alarm during the missing local hour
	// Sunday, 30 March 2008, 00:02:05
	TASShdAlarm alarm2;
	alarm2.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	alarm2.NextDueTime() = TTime(TDateTime(2008, EMarch, 29, 2, 05, 0, 0));
	alarm2.Message().Append(_L("Missing hour alarm"));
	AddAlarmL(alarm2);
	
	// Set an alarm during the missing local hour
	// Sunday, 30 March 2008, 00:03:05
	TASShdAlarm alarm3;
	alarm3.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	alarm3.NextDueTime() = TTime(TDateTime(2008, EMarch, 29, 3, 05, 0, 0));
	alarm3.Message().Append(_L("Missing hour alarm"));
	AddAlarmL(alarm3);
	
	TestAlarmNotifyL(alarm2, TTime(TDateTime(2008, EMarch, 29, 2, 04, 30, 0)), EFalse);
	TestAlarmNotifyL(alarm2, TTime(TDateTime(2008, EMarch, 29, 2, 05, 30, 0)), ETrue);
	TestAlarmNotifyL(alarm1, TTime(TDateTime(2008, EMarch, 29, 2, 14, 30, 0)), EFalse);
	TestAlarmNotifyL(alarm1, TTime(TDateTime(2008, EMarch, 29, 2, 15, 30, 0)), ETrue);
	TestAlarmNotifyL(alarm3, TTime(TDateTime(2008, EMarch, 29, 3, 04, 30, 0)), EFalse);
	TestAlarmNotifyL(alarm3, TTime(TDateTime(2008, EMarch, 29, 3, 05, 30, 0)), ETrue);
	
	// delete the alarm
	DeleteAlarmL(alarm1.Id());
	DeleteAlarmL(alarm2.Id());
	DeleteAlarmL(alarm3.Id());
	}

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
void CAlarmTestApp::TestWakeupAlarmRobustnessL()
	{
	TheAlarmTest.Test().Next(_L("Test that the RTC is not set when a wakeup alarm fails to be written to file"));
	
	TheAlarmTest.Test().Printf(_L("Create a wakeup alarm"));
	TASShdAlarm alarm;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	// Set the wakeup flag for the alarm
	alarm.SetWakeup(ETrue);
	alarm.Message() = KTestName;
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalMinutes(90);
	
	TheAlarmTest.Test().Printf(_L("Add the wakeup alarm"));
	User::LeaveIfError(TheAlarmTest.Session().AlarmAdd(alarm));
	
	TheAlarmTest.Test().Printf(_L("kill the alarm server before the alarm has been written to file"));
	RestartAlarmServerL();
	
	TheAlarmTest.Test().Printf(_L("Test that the alarm was not written to the file"));
	
	RArray<TAlarmId> alarmIds;
	CleanupClosePushL(alarmIds);
	TheAlarmTest.Session().GetAlarmIdListL(alarmIds);
	TheAlarmTest.Test().Printf(_L("There should be zero alarms in the queue.  There are %d alarms"), alarmIds.Count());
	TheAlarmTest(alarmIds.Count() == 1  || alarmIds.Count()== 0);
	if(alarmIds.Count()== 0)
		{
		TestRTCValueL(Time::NullTTime());
		}
	CleanupStack::PopAndDestroy(&alarmIds);
	}

void CAlarmTestApp::TestWakeupFlagPersistanceL()
	{
	TheAlarmTest.Test().Next(_L("TestWakeupFlagPersistanceL"));

	TheAlarmTest.Test().Printf(_L("Create a wakeup alarm"));
	TASShdAlarm alarm;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	// Set the wakeup flag for the alarm
	alarm.SetWakeup(ETrue);
	_LIT(KMessage, "Wakeup alarm");
	alarm.Message() = KMessage;
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalMinutes(90);
	
	AddAlarmL(alarm);
	User::After(100000);
	RestartAlarmServerL();
	
	TheAlarmTest.Test().Printf(_L("Fetch the wakeup alarm characteristics and make sure the flag is set"));
	TASShdAlarm alarm2;
	TheAlarmTest.Session().GetAlarmDetails(alarm.Id(), alarm2);
	TheAlarmTest(alarm2.IsWakeup());
	
	DeleteAlarmL(alarm.Id());
	
	TestRTCValueL(Time::NullTTime());
	}


void CAlarmTestApp::TestSetWakeupSessionAlarmL()
	{
	SetTimeZoneL(KLondon);
	
	TheAlarmTest.Test().Next(_L("TestSetWakeupFlagOnSessionAlarmL"));
	
	TheAlarmTest.Test().Printf(_L("Create a wakeup alarm"));
	TASShdAlarm alarm;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	// Set the wakeup flag for the alarm
	alarm.SetWakeup(ETrue);
	alarm.Characteristics().Set(EAlarmCharacteristicsSessionSpecific);
	alarm.Message() = KTestName;
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalMinutes(90);
	
	TRAPD(err, AddAlarmL(alarm));
	
	TheAlarmTest(err == KErrArgument);
	
	alarm.SetWakeup(EFalse);
	
	TheAlarmTest.Test().Printf(_L("add the session alarm"));
	AddAlarmL(alarm);
	
	TInt setWakeupError1 = TheAlarmTest.Session().SetWakeup(alarm.Id(), ETrue);
	
	TheAlarmTest(setWakeupError1 == KErrArgument);
	
	TheAlarmTest.Test().Printf(_L("clear the session specific alarm flag"));
	TAlarmCharacteristicsFlags alarmCharacteristicsFlags;
	TheAlarmTest.Session().GetAlarmCharacteristics(alarm.Id(), alarmCharacteristicsFlags);
	alarmCharacteristicsFlags.Clear(EAlarmCharacteristicsSessionSpecific);
	TheAlarmTest.Session().SetAlarmCharacteristics(alarm.Id(), alarmCharacteristicsFlags);
	
	TheAlarmTest.Test().Printf(_L("make it a wakeup alarm"));
	TInt setWakeupError2 = TheAlarmTest.Session().SetWakeup(alarm.Id(), ETrue);
	
	TheAlarmTest.Test().Printf(_L("set the  session specific alarm flag"));
	TheAlarmTest.Session().GetAlarmCharacteristics(alarm.Id(), alarmCharacteristicsFlags);
	alarmCharacteristicsFlags.Set(EAlarmCharacteristicsSessionSpecific);
	TInt setSessionSpecificError = TheAlarmTest.Session().SetAlarmCharacteristics(alarm.Id(), alarmCharacteristicsFlags);
	
	TheAlarmTest(setSessionSpecificError == KErrArgument);
	
	DeleteAlarmL(alarm.Id());
	}

void CAlarmTestApp::TestDeleteWakeupAlarmL()
	{
	TheAlarmTest.Test().Next(_L("TestDeleteWakeupAlarmL"));
	
	TheAlarmTest.Test().Printf(_L("Create a wakeup alarm"));
	TASShdAlarm alarm;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	// Set the wakeup flag for the alarm
	alarm.SetWakeup(ETrue);
	alarm.Message() = KTestName;
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalMinutes(90);
	
	AddAlarmL(alarm);
	
	DeleteAlarmL(alarm.Id());
	
	// Test that the RTC is not set
	TestRTCValueL(Time::NullTTime());
	}

void CAlarmTestApp::TestDisableWakeupAlarmL()
	{
	SetTimeZoneL(KLondon);
	
	TheAlarmTest.Test().Next(_L("set a wakeup alarm"));
	TTime systemLocalTime(TDateTime(2006, EMarch, 3, 12, 0, 0, 0)); // March the 23rd
	TTime alarmTimeUTC(TDateTime(2006, EMarch, 6, 12, 0, 0, 0)); // March the 29th
	
	SetHomeTimeL(systemLocalTime);
	
	TheAlarmTest.Test().Printf(_L("Create a wakeup alarm"));
	TASShdAlarm alarm;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	// Set the wakeup flag for the alarm
	alarm.SetWakeup(ETrue);
	alarm.Message() = KTestName;
	alarm.NextDueTime() = alarmTimeUTC; // there is no utc offset in london at this time of year
	
	AddAlarmL(alarm);
	
	// test that the rtc is set to the uts alarm time
	TestRTCValueL(alarmTimeUTC);
	
	TheAlarmTest.Test().Printf(_L("Disable the alarm"));
	TRequestStatus status;
	iPIMTestServer.FileServerNotifyChange(ENotifyAll, status, KAlarmServerFile);
	TheAlarmTest.Session().SetAlarmStatus(alarm.Id(), EAlarmStatusDisabled);
	User::WaitForRequest(status);
	
	// test that the RTC is unset now that we have disabled the alarm
	TestRTCValueL(Time::NullTTime());
	
	TheAlarmTest.Test().Printf(_L("Enable the alarm"));
	iPIMTestServer.FileServerNotifyChange(ENotifyAll, status, KAlarmServerFile);
	TheAlarmTest.Session().SetAlarmStatus(alarm.Id(), EAlarmStatusEnabled);
	User::WaitForRequest(status);
	
	// test that the rtc is set now that we have re-enabled the alarm
	TestRTCValueL(alarmTimeUTC);
	
	DeleteAlarmL(alarm.Id());
	}

void CAlarmTestApp::TestUnsetWakeupFlagL()
	{
	SetTimeZoneL(KLondon);
	
	TheAlarmTest.Test().Next(_L("set a wakeup alarm"));
	TTime systemLocalTime(TDateTime(2006, EMarch, 3, 12, 0, 0, 0)); // March the 23rd
	TTime alarmTimeUTC(TDateTime(2006, EMarch, 4, 12, 0, 0, 0)); // March the 29th
	
	SetHomeTimeL(systemLocalTime);
	
	TheAlarmTest.Test().Printf(_L("Create a wakeup alarm"));
	TASShdAlarm alarm;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	// Set the wakeup flag for the alarm
	alarm.SetWakeup(ETrue);
	alarm.Message() = KTestName;
	alarm.NextDueTime() = alarmTimeUTC;
	
	AddAlarmL(alarm);
	
	// Test that the rtc is set to the utc alarm time
	TestRTCValueL(alarmTimeUTC);
	
	TheAlarmTest.Test().Printf(_L("unset the wakeup flag"));
	TRequestStatus status;
	iPIMTestServer.FileServerNotifyChange(ENotifyAll, status, KAlarmServerFile);
	TheAlarmTest.Session().SetWakeup(alarm.Id(), EFalse);
	User::WaitForRequest(status);
	
	// Test that the rtc is not set now that the alarm is no longer a wakeup alarm
	TestRTCValueL(Time::NullTTime());
	
	TheAlarmTest.Test().Printf(_L("re-set the wakeup flag"));
	iPIMTestServer.FileServerNotifyChange(ENotifyAll, status, KAlarmServerFile);
	TheAlarmTest.Session().SetWakeup(alarm.Id(), ETrue);
	User::WaitForRequest(status);
	
	// Test that the rtc is set now that the alarm has been set back to a wakeup alarm
	TestRTCValueL(alarmTimeUTC);
	
	DeleteAlarmL(alarm.Id());
	}

void CAlarmTestApp::TestWakeupAlarmTimeL()
	{
	TheAlarmTest.Test().Next(_L("Test London over March DST rollover 2006"));
	DoTestWakeupAlarmTimeL(TDateTime(2006, EMarch, 22, 12, 0, 0, 0),
							TDateTime(2006, EMarch, 28, 12, 0, 0, 0),
							60,
							KLondon);
	
	TheAlarmTest.Test().Next(_L("Test London over October DST rollover 2006"));
	DoTestWakeupAlarmTimeL(TDateTime(2006, EOctober, 26, 12, 0, 0, 0),
							TDateTime(2006, EOctober, 30, 12, 0, 0, 0),
							0,
							KLondon);
	
	TheAlarmTest.Test().Next(_L("Test Chicago over April DST rollover 2006"));
	DoTestWakeupAlarmTimeL(TDateTime(2006, EApril, 1, 12, 0, 0, 0),
							TDateTime(2006, EApril, 5, 12, 0, 0, 0),
							-300,
							KChicago);
	
	TheAlarmTest.Test().Next(_L("Test Chicago over October DST rollover 2006"));
	DoTestWakeupAlarmTimeL(TDateTime(2006, EOctober, 27, 12, 0, 0, 0),
							TDateTime(2006, EOctober, 29, 12, 0, 0, 0),
							-360,
							KChicago);
	
	TheAlarmTest.Test().Next(_L("Test Sydney over April DST rollover 2006"));
	DoTestWakeupAlarmTimeL(TDateTime(2006, EApril, 1, 12, 0, 0, 0),
							TDateTime(2006, EApril, 5, 12, 0, 0, 0),
							600,
							KSydney);
	
	TheAlarmTest.Test().Next(_L("Test Sydney over October DST rollover 2006"));
	DoTestWakeupAlarmTimeL(TDateTime(2006, EOctober, 27, 12, 0, 0, 0),
							TDateTime(2006, EOctober, 29, 12, 0, 0, 0),
							660,
							KSydney);
	
	TheAlarmTest.Test().Next(_L("Test Tokyo (there are no DST events in Tokyo)"));
	DoTestWakeupAlarmTimeL(TDateTime(2006, EApril, 1, 12, 0, 0, 0),
							TDateTime(2006, ENovember, 5, 12, 0, 0, 0),
							540,
							KTokyo);
	}

void CAlarmTestApp::DoTestWakeupAlarmTimeL(const TTime& aSystemLocalTime,
											const TTime& aAlarmTimeUTC,
											TTimeIntervalMinutes aNextUTCOffset,
											const TDesC8& aTimeZoneName)
	{
	SetTimeZoneL(aTimeZoneName);
	SetHomeTimeL(aSystemLocalTime);
	
	TASShdAlarm alarm;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	// Set the wakeup flag for the alarm
	alarm.SetWakeup(ETrue);
	alarm.Message() = KTestName;
	// Add the UTC offset from when the alarm will happen to get the local time
	alarm.NextDueTime() = aAlarmTimeUTC + aNextUTCOffset;
	
	AddAlarmL(alarm);
	
	TestRTCValueL(aAlarmTimeUTC);
	
	DeleteAlarmL(alarm.Id());
	}

void CAlarmTestApp::TestFixedAlarmL()
	{
	SetHomeTimeL(TDateTime(2006, EMarch, 20, 12, 0, 0, 0));
	SetTimeZoneL(KLondon);
	
	TheAlarmTest.Test().Printf(_L("Create fixed a wakeup alarm"));
	
	TASShdAlarm alarm;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	// Set the wakeup flag for the alarm
	alarm.SetWakeup(ETrue);
	alarm.Message() = KTestName;
	
	// set the time of the alarm so that it is fixed
	alarm.SetUtcNextDueTime(TDateTime(2006, EMarch, 21, 12, 0, 0, 0));
	AddAlarmL(alarm);
	
	TestRTCValueL(TDateTime(2006, EMarch, 21, 12, 0, 0, 0));
	
	DeleteAlarmL(alarm.Id());
	
	// set the time of the alarm so that it is fixed
	alarm.SetUtcNextDueTime(TDateTime(2006, EApril, 5, 12, 0, 0, 0));
	AddAlarmL(alarm);
	
	TestRTCValueL(TDateTime(2006, EApril, 5, 12, 0, 0, 0));
	
	DeleteAlarmL(alarm.Id());
	}
	
void CAlarmTestApp::TestSetRTCOnDeviceShutdownL()
	{
	TheAlarmTest.Test().Next(_L("Test that the RTC is set on device shutdown"));

	TASShdAlarm alarm = CreateAlarm(Time::NullTTime(), ETrue);

	RProperty property;
	CleanupClosePushL(property);
	User::LeaveIfError(property.Attach(KAlarmServerUID, KRTCValueKey));
	TRequestStatus status;
	property.Subscribe(status);

	TheAlarmTest.Test().Printf(_L("Add a wakeup alarm"));
	User::LeaveIfError(TheAlarmTest.Session().AlarmAdd(alarm));

	TheAlarmTest.Test().Printf(_L("Wait for the alarm to be set when it is added"));
	User::WaitForRequest(status);

	property.Subscribe(status);
	SendSsmSystemMessageL(ESsmShutdown);

	TheAlarmTest.Test().Printf(_L("Wait for the RTC to be set on shutdown"));
	User::WaitForRequest(status);

	TheAlarmTest.Test().Printf(_L("Return to a normal system state"));
	SendSsmSystemMessageL(ESsmNormal);

	User::LeaveIfError(TheAlarmTest.Session().AlarmDelete(alarm.Id()));

	CleanupStack::PopAndDestroy(&property);
	}

void CAlarmTestApp::TestRTCSetOnDSTRolloverL()
	{
	TheAlarmTest.Test().Next(_L("Test that the RTC is set on DST Rollover"));
	
	// make sure we are in london
	SetTimeZoneL(KLondon);
	
	// 30 secodns before DST Rollover
	SetHomeTimeL(TDateTime(2006, EMarch, 25, 0, 59, 58, 0));

	TASShdAlarm alarm;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	_LIT(KMessage, "An alarm");
	alarm.SetWakeup(ETrue);
	alarm.Message() = KMessage;
	// 5 seconds after the DST Rollover
	alarm.NextDueTime() = TTime(TDateTime(2006, EMarch, 25, 2, 0, 5, 0));

	TheAlarmTest.Test().Printf(_L("Add a wakeup alarm"));
	AddAlarmL(alarm);
	
	RProperty property;
	CleanupClosePushL(property);
	User::LeaveIfError(property.Attach(KAlarmServerUID, KRTCValueKey));
	TRequestStatus rtcStatus;
	property.Subscribe(rtcStatus);
	
	RTimer timer;
	CleanupClosePushL(timer);
	User::LeaveIfError(timer.CreateLocal());
	TRequestStatus timerStatus;
	// 2 seconds after the DST rollover
	timer.After(timerStatus, 4000000);

	TheAlarmTest.Test().Printf(_L("Wait for the RTC to be set on the DST rollover"));
	User::WaitForRequest(rtcStatus, timerStatus);
	
	TheAlarmTest.Test().Printf(_L("Test that the RTC was set"));
	TheAlarmTest(rtcStatus == KErrNone);
	TheAlarmTest.Test().Printf(_L("Test that we did not time out"));
	TheAlarmTest(timerStatus == KRequestPending);
	
	timer.Cancel();
	User::WaitForRequest(timerStatus);
	TheAlarmTest(timerStatus == KErrCancel);
	CleanupStack::PopAndDestroy(&timer);

	DeleteAlarmL(alarm.Id());

	CleanupStack::PopAndDestroy(&property);
	}

void CAlarmTestApp::TestRTCSetOnTimeZoneChangeL()
	{
	TheAlarmTest.Test().Next(_L("Test that the RTC is set on time zone change"));
	
	// make sure we are in london
	SetTimeZoneL(KLondon);
	
	// 30 secodns before DST Rollover
	SetHomeTimeL(TDateTime(2006, EMarch, 25, 0, 59, 58, 0));

	TASShdAlarm alarm;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	_LIT(KMessage, "An alarm");
	alarm.SetWakeup(ETrue);
	alarm.Message() = KMessage;
	// 5 seconds after the DST Rollover
	alarm.NextDueTime() = TTime(TDateTime(2006, EMarch, 25, 2, 0, 5, 0));

	TheAlarmTest.Test().Printf(_L("Add a wakeup alarm"));
	AddAlarmL(alarm);
	
	RProperty property;
	CleanupClosePushL(property);
	User::LeaveIfError(property.Attach(KAlarmServerUID, KRTCValueKey));
	TRequestStatus rtcStatus;
	property.Subscribe(rtcStatus);
	
	SetTimeZoneL(KChicago);

	TheAlarmTest.Test().Printf(_L("Wait for the RTC to be set on the time zone change"));
	User::WaitForRequest(rtcStatus);
	
	TheAlarmTest.Test().Printf(_L("Test that the RTC was set"));
	TheAlarmTest(rtcStatus == KErrNone);

	DeleteAlarmL(alarm.Id());

	CleanupStack::PopAndDestroy(&property);
	}

void CAlarmTestApp::TestRTCSetOnSystemTimeChangeL()
	{
	TheAlarmTest.Test().Next(_L("Test that the RTC is set on system time change"));
	
	// make sure we are in london
	SetTimeZoneL(KLondon);
	
	// 30 secodns before DST Rollover
	SetHomeTimeL(TDateTime(2006, EMarch, 25, 0, 59, 58, 0));

	TASShdAlarm alarm;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	_LIT(KMessage, "An alarm");
	alarm.SetWakeup(ETrue);
	alarm.Message() = KMessage;
	// 5 seconds after the DST Rollover
	alarm.NextDueTime() = TTime(TDateTime(2006, EMarch, 25, 3, 0, 5, 0));

	TheAlarmTest.Test().Printf(_L("Add a wakeup alarm"));
	AddAlarmL(alarm);
	
	RProperty property;
	CleanupClosePushL(property);
	User::LeaveIfError(property.Attach(KAlarmServerUID, KRTCValueKey));
	TRequestStatus rtcStatus;
	property.Subscribe(rtcStatus);
	
	// set the system time to a timejust after the DST rollover
	SetHomeTimeL(TDateTime(2006, EMarch, 25, 2, 0, 5, 0));
	
	TheAlarmTest.Test().Printf(_L("Wait for the RTC to be set on the system time change"));
	User::WaitForRequest(rtcStatus);
	
	TheAlarmTest.Test().Printf(_L("Test that the RTC was set"));
	TheAlarmTest(rtcStatus == KErrNone);

	DeleteAlarmL(alarm.Id());

	CleanupStack::PopAndDestroy(&property);
	}

#ifdef __WINS__
void CAlarmTestApp::TestSnoozeWakeupAlarmL()
	{
	TheAlarmTest.Test().Next(_L("Test snoozing a wakeup alarm"));
	
	const TTime KAlarmTime(TDateTime(2006, EMarch, 25, 12, 0, 0, 0));
	
	SetTimeZoneL(KLondon);
	SetHomeTimeL(KAlarmTime - TTimeIntervalSeconds(1));
	
	// add a wakeup alarm
	TASShdAlarm alarm;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	_LIT(KMessage, "An alarm");
	alarm.SetWakeup(ETrue);
	alarm.Message() = KMessage;
	alarm.NextDueTime() = KAlarmTime;
	AddAlarmL(alarm);
	
	// wait for it to go off
	TRequestStatus status;
	iClient.NotifyOnAlarm(alarm, status);
	User::WaitForRequest(status);
	
	// subscribe to the RTC
	RProperty property;
	CleanupClosePushL(property);
	User::LeaveIfError(property.Attach(KAlarmServerUID, KRTCValueKey));
	TRequestStatus propertyStatus;
	property.Subscribe(propertyStatus);
	
	// snooze the alarm
	TheAlarmTest.Test().Printf(_L("Snooze the alarm"));
	User::LeaveIfError(iClient.SnoozeAlarm(alarm.Id(), KAlarmTime + TTimeIntervalMinutes(5)));
	
	// wait for the RTC to change
	User::WaitForRequest(propertyStatus);
	CleanupStack::PopAndDestroy(&property);
	
	// make sure the RTC is set for the snooze time
	TestRTCValueL(KAlarmTime + TTimeIntervalMinutes(5));
	
	// delete the alarm
	DeleteAlarmL(alarm.Id());
	}
#endif

void CAlarmTestApp::TestHeadWakeupAlarmL()
	{
	TheAlarmTest.Test().Next(_L("Test the head wakeup alarm"));
	
	SetTimeZoneL(KLondon);
	SetHomeTimeL(TDateTime(2006, EApril, 9, 12, 0, 0, 0));
	
	// Add a few wakeup alarms
	TASShdAlarm wakeupAlarm1 = CreateAlarm(TDateTime(2006, EApril, 10, 15, 0, 0, 0), ETrue);
	AddAlarmL(wakeupAlarm1);
	TASShdAlarm wakeupAlarm2 = CreateAlarm(TDateTime(2006, EApril, 11, 15, 0, 0, 0), ETrue);
	AddAlarmL(wakeupAlarm2);
	TASShdAlarm wakeupAlarm3 = CreateAlarm(TDateTime(2006, EApril, 12, 15, 0, 0, 0), ETrue);
	AddAlarmL(wakeupAlarm3);
	TASShdAlarm wakeupAlarm4 = CreateAlarm(TDateTime(2006, EApril, 13, 15, 0, 0, 0), ETrue);
	AddAlarmL(wakeupAlarm4);
	
	// Add a few non-wakeup alarms
	TASShdAlarm normAlarm1 = CreateAlarm(TDateTime(2006, EApril, 10, 11, 0, 0, 0), EFalse);
	AddAlarmL(normAlarm1);
	TASShdAlarm normAlarm2 = CreateAlarm(TDateTime(2006, EApril, 11, 11, 0, 0, 0), EFalse);
	AddAlarmL(normAlarm2);
	TASShdAlarm normAlarm3 = CreateAlarm(TDateTime(2006, EApril, 12, 11, 0, 0, 0), EFalse);
	AddAlarmL(normAlarm2);
	TASShdAlarm normAlarm4 = CreateAlarm(TDateTime(2006, EApril, 13, 11, 0, 0, 0), EFalse);
	AddAlarmL(normAlarm4);
	
	
	RProperty property;
	CleanupClosePushL(property);
	User::LeaveIfError(property.Attach(KAlarmServerUID, KRTCValueKey));
	TRequestStatus rtcStatus;
	property.Subscribe(rtcStatus);
	
	// Set system time to somewhere in the middle of the alarms
	SetHomeTimeL(TDateTime(2006, EApril, 12, 9, 0, 0, 0));
	
	// wait for the RTC to change
	User::WaitForRequest(rtcStatus);
	property.Subscribe(rtcStatus);
	
	// test that the head wakeup alarm is set with the RTC and not any other times 
	TestRTCValueL(TDateTime(2006, EApril, 12, 14, 0, 0, 0));
	
	TheAlarmTest.Session().SetAlarmStatus(wakeupAlarm3.Id(), EAlarmStatusDisabled);
	
	User::WaitForRequest(rtcStatus);
	property.Subscribe(rtcStatus);
	
	// test that the second alarm is now set with the RTC
	TestRTCValueL(TDateTime(2006, EApril, 13, 14, 0, 0, 0));
	
	TheAlarmTest.Session().SetAlarmStatus(wakeupAlarm3.Id(), EAlarmStatusEnabled);
	
	User::WaitForRequest(rtcStatus);
	property.Subscribe(rtcStatus);
	
	// test that the first alarm is set with the RTC again
	TestRTCValueL(TDateTime(2006, EApril, 12, 14, 0, 0, 0));
	
	TheAlarmTest.Session().SetWakeup(wakeupAlarm3.Id(), EFalse);
	User::WaitForRequest(rtcStatus);
	property.Subscribe(rtcStatus);
	
	// test that the first alarm is set with the RTC again
	TestRTCValueL(TDateTime(2006, EApril, 13, 14, 0, 0, 0));
	
	TheAlarmTest.Session().SetWakeup(wakeupAlarm3.Id(), ETrue);
	User::WaitForRequest(rtcStatus);
	property.Subscribe(rtcStatus);
	
	// test that the first alarm is set with the RTC again
	TestRTCValueL(TDateTime(2006, EApril, 12, 14, 0, 0, 0));
	
	DeleteAlarmL(wakeupAlarm3.Id());
	
	User::WaitForRequest(rtcStatus);
	
	// test that the second alarm is now set with the RTC
	TestRTCValueL(TDateTime(2006, EApril, 13, 14, 0, 0, 0));
	
	DeleteAllAlarmsL();
	
	CleanupStack::PopAndDestroy(&property);
	}

void CAlarmTestApp::TestAlarmFileNotWrittenOnTimeZoneChangeL()
	{
	TRequestStatus alarmServerFileStatus;
	iPIMTestServer.FileServerNotifyChange(ENotifyAll, alarmServerFileStatus, KAlarmServerFile);

	SetTimeZoneL(KSydney);

	RTimer timer;
	CleanupClosePushL(timer);
	User::LeaveIfError(timer.CreateLocal());
	TRequestStatus timerStatus;
	timer.After(timerStatus, 2000000);

	TheAlarmTest.Test().Printf(_L("Wait for two seconds"));

	User::WaitForRequest(alarmServerFileStatus, timerStatus);

	TheAlarmTest.Test().Printf(_L("Test the timer completed"));
	TheAlarmTest(timerStatus.Int() == KErrNone);
	TheAlarmTest.Test().Printf(_L("Test the alarm server file did not change completed"));
	TheAlarmTest(alarmServerFileStatus.Int() == KRequestPending);

	iPIMTestServer.FileServerNotifyChangeCancel();

	User::WaitForRequest(alarmServerFileStatus);

	CleanupStack::PopAndDestroy(&timer);

	SetTimeZoneL(KLondon);
	}

void CAlarmTestApp::TestAlarmFileNotWrittenOnSystemTimeChangeL()
	{
	TRequestStatus alarmServerFileStatus;
	iPIMTestServer.FileServerNotifyChange(ENotifyAll, alarmServerFileStatus, KAlarmServerFile);

	TTime now;
	now.HomeTime();
	
	SetHomeTimeL(TDateTime(2005, EMarch, 15, 12, 0, 0, 0));

	RTimer timer;
	CleanupClosePushL(timer);
	User::LeaveIfError(timer.CreateLocal());
	TRequestStatus timerStatus;
	timer.After(timerStatus, 2000000);

	TheAlarmTest.Test().Printf(_L("Wait for two seconds"));

	User::WaitForRequest(alarmServerFileStatus, timerStatus);

	TheAlarmTest.Test().Printf(_L("Test the timer completed"));
	TheAlarmTest(timerStatus.Int() == KErrNone);
	TheAlarmTest.Test().Printf(_L("Test the alarm server file did not change completed"));
	TheAlarmTest(alarmServerFileStatus.Int() == KRequestPending);

	iPIMTestServer.FileServerNotifyChangeCancel();

	User::WaitForRequest(alarmServerFileStatus);

	CleanupStack::PopAndDestroy(&timer);

	SetHomeTimeL(now);
	}

void CAlarmTestApp::TestWriteCommandsL(TASShdAlarm& aASShdAlarm, TInt aExpectedError)
	{	
	if (aExpectedError != KErrNone)
		{
		TASShdAlarm alarm = CreateAlarm(Time::NullTTime(), EFalse);
		TheAlarmTest.Test().Printf(_L("AlarmAdd"));
		TInt alarmAddError = TheAlarmTest.Session().AlarmAdd(alarm);
		TheAlarmTest(alarmAddError == aExpectedError);
		}

	TASShdAlarm alarmAddWithNotification = CreateAlarm(Time::NullTTime(), EFalse);
	TheAlarmTest.Test().Printf(_L("AlarmAddWithNotification"));
	TRequestStatus status;
	TheAlarmTest.Session().AlarmAddWithNotification(status, alarmAddWithNotification);

	if (aExpectedError != KErrNone)
		{
		// we expected an error so wait for failure with the error code
		User::WaitForRequest(status);
		TheAlarmTest(status.Int() == aExpectedError);
		}
	else
		{
		// the alarm should be added sucessfully so cancel the notification and dequeue it
		TheAlarmTest.Test().Printf(_L("AlarmNotificationCancelAndDequeue"));
		TheAlarmTest.Session().AlarmNotificationCancelAndDequeue(alarmAddWithNotification.Id());
		User::WaitForRequest(status);
		TheAlarmTest(status.Int() == KErrCancel);
		}

	TheAlarmTest.Test().Printf(_L("AlarmDataAttachL"));
	TInt alarmDataAttachError = TheAlarmTest.Session().AlarmDataAttachL(aASShdAlarm.Id(), KAlarmData);
	TheAlarmTest(alarmDataAttachError == aExpectedError);

	TheAlarmTest.Test().Printf(_L("AlarmDataDetach"));
	TInt alarmDataDetachError = TheAlarmTest.Session().AlarmDataDetach(aASShdAlarm.Id());
	TheAlarmTest(alarmDataDetachError == aExpectedError);

	TheAlarmTest.Test().Printf(_L("SetAlarmCharacteristics"));
	TAlarmCharacteristicsFlags alarmCharacteristicsFlags; 
	TInt setAlarmCharacteristicsError = TheAlarmTest.Session().SetAlarmCharacteristics(aASShdAlarm.Id(), alarmCharacteristicsFlags);
	TheAlarmTest(setAlarmCharacteristicsError == aExpectedError);

	TheAlarmTest.Test().Printf(_L("SetAlarmDayOrTimed"));
	TInt setAlarmDayOrTimedError = TheAlarmTest.Session().SetAlarmDayOrTimed(aASShdAlarm.Id(), EASShdAlarmTypeTimed);
	TheAlarmTest(setAlarmDayOrTimedError == aExpectedError);

	TheAlarmTest.Test().Printf(_L("SetAlarmPlayIntervalsL"));
	CArrayFix<TASCliSoundPlayDefinition>* intervals = new(ELeave) CArrayFixFlat<TASCliSoundPlayDefinition>(8);
	CleanupStack::PushL(intervals);
	TASCliSoundPlayDefinition soundPlayDefinition1(TTimeIntervalMinutes(0), TTimeIntervalSeconds(30));
	intervals->AppendL(soundPlayDefinition1);
	TASCliSoundPlayDefinition soundPlayDefinition2(TTimeIntervalMinutes(5), TTimeIntervalSeconds(30));
	intervals->AppendL(soundPlayDefinition2);
	TRAPD(setAlarmPlayIntervalsError, TheAlarmTest.Session().SetAlarmPlayIntervalsL(*intervals));
	TheAlarmTest(setAlarmPlayIntervalsError == aExpectedError);
	CleanupStack::PopAndDestroy(intervals);

	TheAlarmTest.Test().Printf(_L("SetAlarmStatus"));
	TInt setAlarmStatusError = TheAlarmTest.Session().SetAlarmStatus(aASShdAlarm.Id(), EAlarmStatusEnabled);
	TheAlarmTest(setAlarmStatusError == aExpectedError);

	TheAlarmTest.Test().Printf(_L("SetAlarmStatusByCategory"));
	TInt SetAlarmStatusByCategoryError = TheAlarmTest.Session().SetAlarmStatusByCategory(KAlarmCategory, EAlarmStatusEnabled);
	TheAlarmTest(SetAlarmStatusByCategoryError == aExpectedError);

	TheAlarmTest.Test().Printf(_L("AlarmDelete"));
	TInt alarmDeleteError = TheAlarmTest.Session().AlarmDelete(aASShdAlarm.Id());
	TheAlarmTest(alarmDeleteError == aExpectedError);

	if (alarmDeleteError == KErrNone)
		{
		// We deleted the alarm so add it again
		User::LeaveIfError(TheAlarmTest.Session().AlarmAdd(aASShdAlarm));
		}

	TheAlarmTest.Test().Printf(_L("AlarmDeleteAllByCategory"));
	TInt alarmDeleteAllByCategoryError = TheAlarmTest.Session().AlarmDeleteAllByCategory(KAlarmCategory, EFalse);
	TheAlarmTest(alarmDeleteAllByCategoryError == aExpectedError);

	TheAlarmTest.Test().Printf(_L("AlarmDeleteByCategory"));
	TInt alarmDeleteByCategoryError = TheAlarmTest.Session().AlarmDeleteByCategory(KAlarmCategory, 0);
	TheAlarmTest(alarmDeleteByCategoryError == aExpectedError);
	}

void CAlarmTestApp::TestReadCommandsL(TASShdAlarm& aASShdAlarm)
	{	
	TheAlarmTest.Test().Printf(_L("SetClientData"));
	TInt setClientDataError = TheAlarmTest.Session().SetClientData(aASShdAlarm);
	TheAlarmTest(setClientDataError == KErrNone);

	TheAlarmTest.Test().Printf(_L("SetAlarmSoundState"));
	TInt setAlarmSoundStateError = TheAlarmTest.Session().SetAlarmSoundState(EAlarmGlobalSoundStateOn);
	TheAlarmTest(setAlarmSoundStateError == KErrNone);

	TheAlarmTest.Test().Printf(_L("SetAlarmSoundsSilentFor"));
	TInt setAlarmSoundsSilentForError = TheAlarmTest.Session().SetAlarmSoundsSilentFor(30);
	TheAlarmTest(setAlarmSoundsSilentForError == KErrNone);

	TheAlarmTest.Test().Printf(_L("SetAlarmSoundsSilentUntil"));
	TTime silentUntilTime;
	silentUntilTime.HomeTime();
	silentUntilTime += TTimeIntervalMinutes(5);
	TInt setAlarmSoundsSilentUntilError = TheAlarmTest.Session().SetAlarmSoundsSilentUntil(silentUntilTime);
	TheAlarmTest(setAlarmSoundsSilentUntilError == KErrNone);

	TheAlarmTest.Test().Printf(_L("CancelAlarmSilence"));
	TInt cancelAlarmSilenceError = TheAlarmTest.Session().CancelAlarmSilence();
	TheAlarmTest(cancelAlarmSilenceError == KErrNone);

	TheAlarmTest.Test().Printf(_L("AlarmCountByState"));
	TInt alarmCountQueued = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
	TheAlarmTest(alarmCountQueued == 1);

	TheAlarmTest.Test().Printf(_L("AlarmDataSize"));
	TInt alarmDataSize = TheAlarmTest.Session().AlarmDataSize(aASShdAlarm.Id());
	TheAlarmTest(alarmDataSize == KErrNotFound);

	TheAlarmTest.Test().Printf(_L("AlarmSoundsTemporarilySilenced"));
	TBool alarmSoundsTemporarilySilenced = TheAlarmTest.Session().AlarmSoundsTemporarilySilenced();
	TheAlarmTest(alarmSoundsTemporarilySilenced == EFalse);

	TheAlarmTest.Test().Printf(_L("GetAlarmCategory"));
	TAlarmCategory alarmCategory;
	TInt getAlarmCategoryError = TheAlarmTest.Session().GetAlarmCategory(aASShdAlarm.Id(), alarmCategory);
	TheAlarmTest(getAlarmCategoryError == KErrNone);

	TheAlarmTest.Test().Printf(_L("GetAlarmCharacteristics"));
	TAlarmCharacteristicsFlags TAlarmCharacteristicsFlags;
	TInt getAlarmCharacteristicsError = TheAlarmTest.Session().GetAlarmCharacteristics(aASShdAlarm.Id(), TAlarmCharacteristicsFlags);
	TheAlarmTest(getAlarmCharacteristicsError == KErrNone);

	TheAlarmTest.Test().Printf(_L("GetAlarmCountForCategory"));
	TInt AlarmCountForCategory = TheAlarmTest.Session().GetAlarmCountForCategory(KAlarmCategory);
	TheAlarmTest(AlarmCountForCategory == 0);

	TheAlarmTest.Test().Printf(_L("GetAlarmData"));
	HBufC8* sink = NULL;
	TInt getAlarmDataError = TheAlarmTest.Session().GetAlarmData(aASShdAlarm.Id(), sink);
	TheAlarmTest(getAlarmDataError == KErrNone || getAlarmDataError == KErrNotFound);
	delete sink;

	TheAlarmTest.Test().Printf(_L("GetAlarmDayOrTimed"));
	TAlarmDayOrTimed alarmDayOrTimed;
	TInt getAlarmDayOrTimedError = TheAlarmTest.Session().GetAlarmDayOrTimed(aASShdAlarm.Id(), alarmDayOrTimed);
	TheAlarmTest(getAlarmDayOrTimedError == KErrNone);

	TheAlarmTest.Test().Printf(_L("GetAlarmDetails"));
	TASShdAlarm alarm;
	TInt getAlarmDetails = TheAlarmTest.Session().GetAlarmDetails(aASShdAlarm.Id(), alarm);
	TheAlarmTest(getAlarmDetails == KErrNone);

	TheAlarmTest.Test().Printf(_L("GetAlarmIdListByStateL"));
	RArray<TAlarmId> alarmIds;
	CleanupClosePushL(alarmIds);
	TheAlarmTest.Session().GetAlarmIdListByStateL(EAlarmStateQueued, alarmIds);
	TheAlarmTest.Test().Printf(_L("GetAlarmIdListByStateL = %d"), alarmIds.Count());
	TheAlarmTest(alarmIds.Count() == 1);
	alarmIds.Reset();

	TheAlarmTest.Test().Printf(_L("GetAlarmIdListForCategoryL"));
	TheAlarmTest.Session().GetAlarmIdListForCategoryL(KAlarmCategory, alarmIds);
	TheAlarmTest.Test().Printf(_L("GetAlarmIdListForCategoryL = %d"), alarmIds.Count());	
	TheAlarmTest(alarmIds.Count() == 0);
	alarmIds.Reset();

	TheAlarmTest.Test().Printf(_L("GetAlarmIdListL"));
	TheAlarmTest.Session().GetAlarmIdListL(alarmIds);
	TheAlarmTest.Test().Printf(_L("GetAlarmIdListL = %d"), alarmIds.Count());	
	TheAlarmTest(alarmIds.Count() == 1);
	CleanupStack::PopAndDestroy(&alarmIds); // alarmIds.Close()

	TheAlarmTest.Test().Printf(_L("GetAlarmOwner"));
	TFullName fullName;
	TInt getAlarmOwnerError = TheAlarmTest.Session().GetAlarmOwner(aASShdAlarm.Id(), fullName);
	TheAlarmTest(getAlarmOwnerError == KErrNone);

	TheAlarmTest.Test().Printf(_L("GetAlarmPlayIntervalsL"));
	CArrayFix<TASCliSoundPlayDefinition>* intervals = new(ELeave) CArrayFixFlat<TASCliSoundPlayDefinition>(8);
	CleanupStack::PushL(intervals);
	TheAlarmTest.Session().GetAlarmPlayIntervalsL(*intervals);
	CleanupStack::PopAndDestroy(intervals);

	TheAlarmTest.Test().Printf(_L("GetAlarmSoundState"));
	TAlarmGlobalSoundState alarmGlobalSoundState;
	TInt getAlarmSoundStateError = TheAlarmTest.Session().GetAlarmSoundState(alarmGlobalSoundState);
	TheAlarmTest(getAlarmSoundStateError == KErrNone);

	TheAlarmTest.Test().Printf(_L("GetAlarmSoundsSilentUntil"));
	TTime silentUntil;
	TInt getAlarmSoundsSilentUntilError = TheAlarmTest.Session().GetAlarmSoundsSilentUntil(silentUntil);
	TheAlarmTest.Test().Printf(_L("getAlarmSoundsSilentUntilError = %d"), getAlarmSoundsSilentUntilError);	
	//TheAlarmTest(getAlarmSoundsSilentUntilError == KErrNone);

	TheAlarmTest.Test().Printf(_L("GetAlarmStatus"));
	TAlarmStatus alarmStatus;
	TInt getAlarmStatusError = TheAlarmTest.Session().GetAlarmStatus(aASShdAlarm.Id(), alarmStatus);
	TheAlarmTest(getAlarmStatusError == KErrNone);

	TheAlarmTest.Test().Printf(_L("GetAvailableCategoryListL"));
	RArray<TAlarmCategory> categories;
	CleanupClosePushL(categories);
	TheAlarmTest.Session().GetAvailableCategoryListL(categories);
	CleanupStack::PopAndDestroy(&categories);

	TheAlarmTest.Test().Printf(_L("GetNextDueAlarmId"));
	TAlarmId alarmId;
	TInt getNextDueAlarmIdError = TheAlarmTest.Session().GetNextDueAlarmId(alarmId);
	TheAlarmTest(getNextDueAlarmIdError == KErrNone);

	TheAlarmTest.Test().Printf(_L("NumberOfAlarmsActiveInQueue"));
	TInt numberOfAlarmsActiveInQueue = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest.Test().Printf(_L("numberOfAlarmsActiveInQueue = %d"), numberOfAlarmsActiveInQueue);
	}

void CAlarmTestApp::TestReadOnlyInShutDownL()
	{
	TASShdAlarm alarm = CreateAlarm(Time::NullTTime(), EFalse);
	User::LeaveIfError(TheAlarmTest.Session().AlarmAdd(alarm));

	SendSsmSystemMessageL(ESsmShutdown);
	TheAlarmTest.Next(_L("Test that write commands leave with locked"));
	TestWriteCommandsL(alarm, KErrLocked);
	TheAlarmTest.Next(_L("Test alarm file not written on time zone change"));
	TestAlarmFileNotWrittenOnTimeZoneChangeL();
	TheAlarmTest.Next(_L("Test alarm file not written on system time change"));
	TestAlarmFileNotWrittenOnSystemTimeChangeL();
	TheAlarmTest.Next(_L("Test that read commands still work"));
	TestReadCommandsL(alarm);

	SendSsmSystemMessageL(ESsmNormal);
	TheAlarmTest.Next(_L("Test that write commands work again"));
	TestWriteCommandsL(alarm, KErrNone);
	TheAlarmTest.Next(_L("Test that read commands still work"));
	TestReadCommandsL(alarm);

	User::LeaveIfError(TheAlarmTest.Session().AlarmDelete(alarm.Id()));
	}
#endif

void CAlarmTestApp::RunTestsL()
	{
	DeleteAllAlarmsL();
	TestAlarmMissingHourRepeatDailyL();
	TestAlarmMissingHourRepeatOnce1L();
	TestAlarmMissingHourRepeatOnce2L();
	TestAlarmMissingHourRepeatOnce3L();
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	TestWakeupFlagPersistanceL();
	TestSetWakeupSessionAlarmL();
	TestDeleteWakeupAlarmL();
	TestDisableWakeupAlarmL();
	TestUnsetWakeupFlagL();
	TestWakeupAlarmTimeL();	
	TestWakeupAlarmRobustnessL();
	TestFixedAlarmL();
#ifdef __WINS__
	TestSnoozeWakeupAlarmL();
#endif	
	TestReadOnlyInShutDownL();
	TestSetRTCOnDeviceShutdownL();
	TestRTCSetOnDSTRolloverL();
	TestRTCSetOnTimeZoneChangeL();
	TestRTCSetOnSystemTimeChangeL();
	TestHeadWakeupAlarmL();
#endif
	}

void doTestL()
	{
	CAlarmTestApp* alarmTestApp = CAlarmTestApp::NewLC();
	alarmTestApp->RunTestsL();
	CleanupStack::PopAndDestroy(alarmTestApp);
	}

/**

@SYMTestCaseID PIM-TWAKEUPALARMS-0001

*/

GLDEF_C TInt E32Main()
    {
    __UHEAP_MARK;
    
	CActiveScheduler* scheduler = new  CActiveScheduler;
	CActiveScheduler::Install(scheduler);
	
	CTrapCleanup* cleanup = CTrapCleanup::New();
	
	TTime now;
	now.HomeTime();
	TheAlarmTest.Title();
	TheAlarmTest.Start(KTestName);
	
	TRAPD(testError, doTestL());
		
	// Check there are no alarm left active
	TInt n=TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest(n==0, __LINE__);
	
    TheAlarmTest.Session().Close();
	TheAlarmTest.EndL();
	TheAlarmTest.Test().Close();
	ASTstAlarmTest::Close();
	
	delete cleanup;
	delete scheduler;

	__UHEAP_MARKEND;
	
	return testError;
    }
