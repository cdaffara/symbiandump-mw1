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

// System includes
#include <asclisoundplay.h>

// Constants
const TInt KWaitInterval = 10; // time delay after which alarms complete in seconds.
// Literal constants
_LIT8(KSomeAlarmData, "This is some 8-bit data");


static void testDataInvalidAlarmL()
//
//
//
	{
	TheAlarmTest.Test().Next(_L("Testing data methods with invalid alarm ids"));
	//
	TInt r;
	//
	r = TheAlarmTest.Session().AlarmDataSize(KNullAlarmId);
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//

	// AlarmDataDetatch has been deprecated (since it's spelt wrong).
	// We still call it here though, since removing it would break SC.
	r = TheAlarmTest.Session().AlarmDataDetatch(KNullAlarmId);
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataAttachL(KNullAlarmId, KSomeAlarmData);
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	HBufC8* buf = HBufC8::NewLC(KSomeAlarmData().Size());
	TPtr8 pBuf(buf->Des());
	r = TheAlarmTest.Session().GetAlarmData(KNullAlarmId, pBuf);
	TheAlarmTest(r == KErrNotFound, __LINE__);
	TheAlarmTest(*buf == KNullDesC8, __LINE__);
	CleanupStack::PopAndDestroy(buf);

	HBufC8* buf2 = NULL;
	r = TheAlarmTest.Session().GetAlarmData(KNullAlarmId, buf2);
	TheAlarmTest(r == KErrNotFound, __LINE__);
	TheAlarmTest(buf2 == NULL, __LINE__);
	}


static void testDataValidAlarmL()
//
//
//
	{
	
	TheAlarmTest.Test().Next(_L("Data API test"));

	TInt r;
	TAlarmId alarmId;
	TASShdAlarm alarm;
	static TRequestStatus status;
	//
	alarm.Message() = _L("Alarm with data");
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalSeconds(KWaitInterval);
	//
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r==KErrNone, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmChangeEventAlarmData, __LINE__);
	TheAlarmTest(alarmId = alarm.Id(), __LINE__);
	User::WaitForRequest(status);
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrInUse, __LINE__);
	TheAlarmTest(status == KRequestPending, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KSomeAlarmData().Size());
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDelete(alarm.Id());
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest.TestEventBuffer(EAlarmChangeEventAlarmData, status, alarmId);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalSeconds(KWaitInterval);
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r==KErrNone, __LINE__);
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmChangeEventAlarmData, __LINE__);
	TheAlarmTest(alarmId = alarm.Id(), __LINE__);
	User::WaitForRequest(status);
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest.TestEventBuffer(EAlarmChangeEventAlarmData, status, alarmId);
	//
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmChangeEventAlarmData, __LINE__);
	TheAlarmTest(alarmId = alarm.Id(), __LINE__);
	User::WaitForRequest(status);
	//
	HBufC8* buf = HBufC8::NewLC(KSomeAlarmData().Size());
	TPtr8 pBuf(buf->Des());
	r = TheAlarmTest.Session().GetAlarmData(alarm.Id(), pBuf);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(*buf == KSomeAlarmData, __LINE__);
	//
	HBufC8* buf2 = NULL;
	r = TheAlarmTest.Session().GetAlarmData(alarm.Id(), buf2);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(*buf2 == KSomeAlarmData, __LINE__);
	delete buf2;
	//
	
	
	
	TheAlarmTest.Test().Next(_L("Waiting for alarm to expire"));
	TheAlarmTest.WaitForEvent(EAlarmChangeEventTimerExpired, status, alarmId);
	TheAlarmTest(alarmId == alarm.Id(), __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrInUse, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KSomeAlarmData().Size(), __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDelete(alarm.Id());
	TheAlarmTest(r == KErrNone, __LINE__);
	//

	TheAlarmTest.Session().AlarmAddWithNotification(status, alarm, KSomeAlarmData);
	TheAlarmTest(alarmId = alarm.Id(), __LINE__);
	HBufC8* alarmdata = HBufC8::NewLC(KSomeAlarmData().Size());
	TPtr8 pAlarmdata(alarmdata->Des());
	r = TheAlarmTest.Session().GetAlarmData(alarm.Id(), pAlarmdata);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(*alarmdata == KSomeAlarmData, __LINE__);
	CleanupStack::PopAndDestroy(alarmdata);
	//
	r = TheAlarmTest.Session().GetAlarmData(alarm.Id(), buf2);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(*buf2 == KSomeAlarmData, __LINE__);
	delete buf2;
	//
	alarm.Reset();
	alarm.Message() = _L("Alarm with data");
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalSeconds(KWaitInterval);
	//
	r =	TheAlarmTest.Session().AlarmAdd(alarm, KSomeAlarmData);
	TheAlarmTest(alarmId = alarm.Id(), __LINE__);
	HBufC8* alarmdata1 = HBufC8::NewLC(KSomeAlarmData().Size());
	TPtr8 pAlarmdata1(alarmdata1->Des());
	r = TheAlarmTest.Session().GetAlarmData(alarm.Id(), pAlarmdata1);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(*alarmdata1 == KSomeAlarmData, __LINE__);
	CleanupStack::PopAndDestroy(alarmdata1);
	//
	r = TheAlarmTest.Session().GetAlarmData(alarm.Id(), buf2);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(*buf2 == KSomeAlarmData, __LINE__);
	delete buf2;
	//

#ifdef _DEBUG
#ifdef __WINS__
	
	
	TheAlarmTest.Test().Next(_L("Debug only - shutting down server"));
	TheAlarmTest.Session().__DbgShutDownServer();
	User::After(3 * 1000000);
	//
	
	
	TheAlarmTest.Test().Next(_L("Debug only - starting up server"));
	TheAlarmTest(TheAlarmTest.Session().Connect() == KErrNone, __LINE__);
	//
	
	
	TheAlarmTest.Test().Next(_L("Debug only - testing data restoration"));
	//
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrInUse, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KSomeAlarmData().Size(), __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDelete(alarm.Id());
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r==KErrNone, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);

#endif
#endif
	CleanupStack::PopAndDestroy(buf);
	}


static void testDataInvalidUtcAlarmL()
	{
	TheAlarmTest.Test().Next(_L("Testing data methods with invalid alarm ids"));
	//
	TInt r;
	//
	r = TheAlarmTest.Session().AlarmDataSize(KNullAlarmId);
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//

	// AlarmDataDetatch has been deprecated (since it's spelt wrong).
	// We still call it here though, since removing it would break SC.
	r = TheAlarmTest.Session().AlarmDataDetatch(KNullAlarmId);
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataAttachL(KNullAlarmId, KSomeAlarmData);
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	HBufC8* buf = HBufC8::NewLC(KSomeAlarmData().Size());
	TPtr8 pBuf(buf->Des());
	r = TheAlarmTest.Session().GetAlarmData(KNullAlarmId, pBuf);
	TheAlarmTest(r == KErrNotFound, __LINE__);
	TheAlarmTest(*buf == KNullDesC8, __LINE__);
	CleanupStack::PopAndDestroy(buf);
	//
	//
	HBufC8* buf2 = NULL;
	r = TheAlarmTest.Session().GetAlarmData(KNullAlarmId, buf2);
	TheAlarmTest(r == KErrNotFound, __LINE__);
	TheAlarmTest(buf2 == NULL, __LINE__);
	//
	}

static void testDataValidUtcAlarmL()
	{
		
	TheAlarmTest.Test().Next(_L("Data API test"));

	TInt r;
	TAlarmId alarmId;
	TASShdAlarm alarm;
	static TRequestStatus status;
	TTime time;
	//
	alarm.Message()=(_L("Alarm with data"));
	time.UniversalTime();
	time += TTimeIntervalSeconds(KWaitInterval);
	alarm.SetUtcNextDueTime(time);
	//
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r==KErrNone, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmChangeEventAlarmData, __LINE__);
	TheAlarmTest(alarmId = alarm.Id(), __LINE__);
	User::WaitForRequest(status);
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrInUse, __LINE__);
	TheAlarmTest(status == KRequestPending, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KSomeAlarmData().Size());
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDelete(alarm.Id());
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest.TestEventBuffer(EAlarmChangeEventAlarmData, status, alarmId);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	time.UniversalTime();
	time += TTimeIntervalSeconds(KWaitInterval);
	alarm.SetUtcNextDueTime(time);
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r==KErrNone, __LINE__);
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmChangeEventAlarmData, __LINE__);
	TheAlarmTest(alarmId = alarm.Id(), __LINE__);
	User::WaitForRequest(status);
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest.TestEventBuffer(EAlarmChangeEventAlarmData, status, alarmId);
	//
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmChangeEventAlarmData, __LINE__);
	TheAlarmTest(alarmId = alarm.Id(), __LINE__);
	User::WaitForRequest(status);
	//
	HBufC8* buf = HBufC8::NewLC(KSomeAlarmData().Size());
	TPtr8 pBuf(buf->Des());
	r = TheAlarmTest.Session().GetAlarmData(alarm.Id(), pBuf);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(*buf == KSomeAlarmData, __LINE__);
	//
	HBufC8* buf2 = NULL;
	r = TheAlarmTest.Session().GetAlarmData(alarm.Id(), buf2);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(*buf2 == KSomeAlarmData, __LINE__);
	delete buf2;
	//
	
	
	TheAlarmTest.Test().Next(_L("Waiting for alarm to expire"));
	TheAlarmTest.WaitForEvent(EAlarmChangeEventTimerExpired, status, alarmId);
	TheAlarmTest(alarmId == alarm.Id(), __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrInUse, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KSomeAlarmData().Size(), __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDelete(alarm.Id());
	TheAlarmTest(r == KErrNone, __LINE__);
	//

	TheAlarmTest.Session().AlarmAddWithNotification(status, alarm, KSomeAlarmData);
	TheAlarmTest(alarmId = alarm.Id(), __LINE__);
	HBufC8* alarmdata = HBufC8::NewLC(KSomeAlarmData().Size());
	TPtr8 pAlarmdata(alarmdata->Des());
	r = TheAlarmTest.Session().GetAlarmData(alarm.Id(), pAlarmdata);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(*alarmdata == KSomeAlarmData, __LINE__);
	CleanupStack::PopAndDestroy(alarmdata);
	//
	r = TheAlarmTest.Session().GetAlarmData(alarm.Id(), buf2);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(*buf2 == KSomeAlarmData, __LINE__);
	delete buf2;
	//
	alarm.Reset();
	alarm.Message()=(_L("Alarm with data"));
	time.UniversalTime();
	time += TTimeIntervalSeconds(KWaitInterval);
	alarm.SetUtcNextDueTime(time);
	//
	r =	TheAlarmTest.Session().AlarmAdd(alarm, KSomeAlarmData);
	TheAlarmTest(alarmId = alarm.Id(), __LINE__);
	HBufC8* alarmdata1 = HBufC8::NewLC(KSomeAlarmData().Size());
	TPtr8 pAlarmdata1(alarmdata1->Des());
	r = TheAlarmTest.Session().GetAlarmData(alarm.Id(), pAlarmdata1);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(*alarmdata1 == KSomeAlarmData, __LINE__);
	CleanupStack::PopAndDestroy(alarmdata1);
	//
	r = TheAlarmTest.Session().GetAlarmData(alarm.Id(), buf2);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(*buf2 == KSomeAlarmData, __LINE__);
	delete buf2;
	//

#ifdef _DEBUG
#ifdef __WINS__
	
	
	TheAlarmTest.Test().Next(_L("Debug only - shutting down server"));
	TheAlarmTest.Session().__DbgShutDownServer();
	User::After(3 * 1000000);
	//
	
	
	TheAlarmTest.Test().Next(_L("Debug only - starting up server"));
	TheAlarmTest(TheAlarmTest.Session().Connect() == KErrNone, __LINE__);
	//
	
	
	TheAlarmTest.Test().Next(_L("Debug only - testing data restoration"));
	//
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrInUse, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KSomeAlarmData().Size(), __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDelete(alarm.Id());
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r==KErrNone, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);

#endif
#endif
	CleanupStack::PopAndDestroy(buf);
	}

/**
@SYMTestCaseID PIM-TALARMDATA-0001
*/
static void doTestsL()
	{
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	const TInt KGmtOffset = 3600;
	_LIT(KTime, "20040730:050000.000000"); //In summer time
	TInt err = User::SetUTCTimeAndOffset(TTime(KTime), KGmtOffset);

	TheAlarmTest.Start(_L("@SYMTestCaseID PIM-TALARMDATA-0001 Connecting to server"));
	TInt r = TheAlarmTest.Session().Connect();
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest.TestClearStoreL();

	TheAlarmTest.Session().__DbgPreventUserNotify(ETrue);

	// Test valid alarm set with local wall-clock time.
	TheAlarmTest.Test().Printf(_L("Alarms set with local wall-clock time.\n"));
	testDataValidAlarmL();
	TheAlarmTest.TestClearStoreL();
	TheAlarmTest.Test().Printf(_L("Alarms set with UTC time.\n"));

	// Test valid alarm set with UTC time.
	testDataValidUtcAlarmL();

	// Test invalid alarm set with local wall-clock time.
	TheAlarmTest.Test().Printf(_L("Alarms set with ocal wall-clock time.\n"));
	testDataInvalidAlarmL();

	// Tests invalid alarm set with UTC time.
	TheAlarmTest.Test().Printf(_L("Alarms set with UTC time.\n"));
	testDataInvalidUtcAlarmL();

	TheAlarmTest.TestClearStoreL();
	TheAlarmTest.Session().Close();

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
