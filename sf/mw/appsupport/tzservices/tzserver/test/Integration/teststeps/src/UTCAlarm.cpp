// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "UTCAlarm.h"
#include "DstStep.h"
#include "AlarmNotifier.h"


// time format string for logs
_LIT(KTimeFormat, "%F%Y/%M/%D %H%:2%T%:2%S%");
CUTCAlarm::CUTCAlarm()
	{}
	
CUTCAlarm::~CUTCAlarm()
	{
	iTzServer.Close();
	iAlarmServer.Close();
	delete iActiveScheduler;
	}
	
	
TVerdict CUTCAlarm::doTestStepPreambleL()
	{
	iActiveScheduler = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(iActiveScheduler);
	SetTestStepResult(EFail);	
	
	User::LeaveIfError(iTzServer.Connect());
	User::LeaveIfError(iAlarmServer.Connect());
	iAlarmServer.AlarmDeleteAllByCategory(TUid::Null(), EFalse);
	
	SetTestType();
	TRAPD(err,InitialiseTestStepL());
	if(err)
		return TestStepResult();
	
	
	return TestStepResult();
	}

void CUTCAlarm::InitialiseTestStepL()
	{
	
	_LIT(KInitialLocationTag, "InitialLocation");
	_LIT(KInitialTimeTag,"InitialTime");
	_LIT(KDueTime, "DueTime");
	_LIT(KUTCTag, "UTC");
	
	TPtrC ptr;
	GetStringFromConfig(ConfigSection(),KInitialLocationTag, ptr);
	iInitialLocation.Copy(ptr);
	
	GetStringFromConfig(ConfigSection(), KInitialTimeTag, ptr);
	iInitialTime = CDstStep::ReadTimeParamStringL(ptr);
	
	GetStringFromConfig(ConfigSection(), KDueTime, ptr);
	iDueTime = CDstStep::ReadTimeParamStringL(ptr);
	
	GetBoolFromConfig(ConfigSection(), KUTCTag, iIsUtcAlarm);
	
	// set the initial time zone
	CTzId* tzId = CTzId::NewL(iInitialLocation);
	CleanupStack::PushL(tzId);
	iTzServer.SetTimeZoneL(*tzId);
	CleanupStack::PopAndDestroy(tzId);
		
	switch(iTestType)
		{
	case ETimeZoneChange:
	
		_LIT(KChanedLocationTag, "ChangedLocation");
		GetStringFromConfig(ConfigSection(),KChanedLocationTag, ptr);
		iChangedLocation.Copy(ptr);
		
		// set the initial time
		User::LeaveIfError(iTzServer.SetHomeTime(iInitialTime));
		
		break;
	case EDSTEvent:
		break;
	default:
		User::Leave(KErrNotFound);
		}
		User::After(1);
	}
	
	
void CUTCAlarm::RunTestL()
	{
	switch(iTestType)
		{
	case ETimeZoneChange:
		{
		// set alarm
		CAlarmNotifier* notifier = CAlarmNotifier::NewL(iAlarmServer);
		notifier->Start(iDueTime, iIsUtcAlarm);
	
		// change the time zone
		CTzId* tzId = CTzId::NewL(iChangedLocation);
		CleanupStack::PushL(tzId);
		iTzServer.SetTimeZoneL(*tzId);
		CleanupStack::PopAndDestroy(tzId);
		
		// give some time for the alarm server to take affect.
		User::After(1000000);
		
		// set the system time close to the due time (i.e. 2 seconds 
		// before the due time)
		if(iIsUtcAlarm)
			User::SetUTCTime(iDueTime - TTimeIntervalSeconds(2));
		else
			iTzServer.SetHomeTime(iDueTime - TTimeIntervalSeconds(2));
		
		// wait for the alarm to expire.
		CActiveScheduler::Start();
		
		// get the alarm details
		TASShdAlarm alarm;
		CheckDueTimeL(notifier->AlarmId(), alarm);
		delete notifier;
		iAlarmServer.AlarmDelete(alarm.Id());
		}
		break;
	case EDSTEvent:
		{
		iTzServer.SetHomeTime(iInitialTime-TTimeIntervalDays(3));
		User::After(1);
		TASShdAlarm alarm;
		_LIT(KAlarmMessage, "UTC Alarm Integration Test");
		alarm.Message() = KAlarmMessage();
		alarm.DayOrTimed() = EASShdAlarmTypeTimed;	
	
		if(iIsUtcAlarm)
			{
			alarm.SetUtcNextDueTime(iDueTime);
			}
		else
			{
			alarm.NextDueTime() = iDueTime;
			}
		iAlarmServer.AlarmAdd(alarm);	
		TAlarmId alarmId = alarm.Id();
		iTzServer.SetHomeTime(iInitialTime);		
		// wait for DST event.
		User::After(2000000);	
		CheckDueTimeL(alarmId, alarm);
		iAlarmServer.AlarmDelete(alarm.Id());
		}
		break;
	default:
		User::Leave(KErrNotFound);
		}
	}
	
TVerdict CUTCAlarm::doTestStepL()
	{
	
	RunTestL();
	
	return TestStepResult();
	}


	
   void CUTCAlarm::CheckDueTimeL(const TAlarmId aAlarmId, TASShdAlarm& aAlarm)
   	{
   	aAlarm.Reset();
   	iAlarmServer.GetAlarmDetails(aAlarmId, aAlarm);

   	if(aAlarm.NextDueTime() == iDueTime)
   		{
   		SetTestStepResult(EPass);
   		}
   	else
   		{
   		TBuf<128> str;
   		TTime time = aAlarm.NextDueTime();
   		time.FormatL(str,KTimeFormat);
   		
   		TBuf<128> org;
   		iDueTime.FormatL(org, KTimeFormat); 			
  		ERR_PRINTF3(_L("Initial next due time: %S, after time zone change: %S"), &org, &str);	
   		}	
   	}
	
TVerdict CUTCAlarm::doTestStepPostamble()
	{
	return TestStepResult();
	}
	
void CUTCAlarm::SetTestType()
	{
	_LIT(KTestTypeTag, "TestType");
	_LIT(KTimeZoneChanged, "TimeZoneChange");
	_LIT(KDSTEvent, "DSTEvent");
	
	TPtrC strPtr;
	GetStringFromConfig(ConfigSection(), KTestTypeTag, strPtr);
	if(!strPtr.Compare(KTimeZoneChanged))
		iTestType = ETimeZoneChange;
	else if(!strPtr.Compare(KDSTEvent))
		iTestType = EDSTEvent;
	}
