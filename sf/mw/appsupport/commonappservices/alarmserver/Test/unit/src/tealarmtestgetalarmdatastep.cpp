// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "tealarmtestgetalarmdatastep.h"


CTEAlarmTestGetAlarmDataStep::CTEAlarmTestGetAlarmDataStep()
	{
	SetTestStepName(KTEAlarmTestGetAlarmDataStep);
	}


CTEAlarmTestGetAlarmDataStep::~CTEAlarmTestGetAlarmDataStep()
	{
	}


TVerdict CTEAlarmTestGetAlarmDataStep::doTestStepL()
	{
	__UHEAP_MARK;
	
	_LIT8(KSomeAlarmData, "This is some 8-bit data");
	const TInt KWaitAlarmTime = 10; // time delay for the test alarm
	
	TASShdAlarm alarm;
	alarm.Message() = _L("Alarm with data");
	TTime alarmTime;
	alarmTime.HomeTime();
	alarmTime += TTimeIntervalSeconds(KWaitAlarmTime);
	
	alarm.NextDueTime() = alarmTime;
	
	TInt err = iSession.AlarmAdd(alarm, KSomeAlarmData);
	TEST(err == KErrNone);
	
	// Test no memory case
	__UHEAP_FAILNEXT(1);
	HBufC8* buff = NULL;
	err = iSession.GetAlarmData(alarm.Id(), buff);
	TEST(err == KErrNoMemory);
	__UHEAP_RESET;

	// Test normal case that the retrieved data is the same
	err = iSession.GetAlarmData(alarm.Id(), buff);
	TEST(err == KErrNone);
	TEST(0 == buff->Compare(KSomeAlarmData));

	// now do cleanup, i.e. delete added alarm and buff
	delete buff;
	iSession.AlarmDelete(alarm.Id());

	__UHEAP_MARKEND;

	return TestStepResult();			
	}
