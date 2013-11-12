// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "TEAlarmTestAlarmPlayConfig.h"
#include <asclisession.h>
#include "consoleantestclient.h"

CTEAlarmTestAlarmPlayConfigStep::~CTEAlarmTestAlarmPlayConfigStep()
	{
	iSoundIntervals->Reset();
	delete iSoundIntervals;
	}
	
CTEAlarmTestAlarmPlayConfigStep::CTEAlarmTestAlarmPlayConfigStep()
	{
	SetTestStepName(KTEAlarmTestAlarmPlayConfigStep);
	}


TVerdict CTEAlarmTestAlarmPlayConfigStep::doTestStepL()
	{
	TESTL(GetAndCheckAlarmPlayIntervalsL());
	
	__UHEAP_MARK;
	// Get Alarm(s) from Config
	CArrayFixFlat<TASShdAlarm>*  alarmArray = new(ELeave) CArrayFixFlat<TASShdAlarm>(1);
	CleanupStack::PushL(alarmArray);
	
	TESTL(GetAlarmsFromConfigL(ConfigSection(), *alarmArray));

	TInt alarmCount = alarmArray->Count();

	for(TInt i = 0; i < alarmCount; i++)
		{
		// Add the Alarm
    	TESTL((iSession.AlarmAdd(alarmArray->At(i))==KErrNone));
		INFO_PRINTF5(_L("Alarm[%d] Added: Id=%d, State=%d, Status=%d"), i, alarmArray->At(i).Id(), alarmArray->At(i).State(), alarmArray->At(i).Status());
		}

	if( alarmArray->Count() > 0 )
		{
		TESTL(StartIntervalVerifierL(alarmArray->At(0).Id()));
		}
	else
		{
		INFO_PRINTF1(_L("AlarmPlayConfig: No Alarms specified; skipping Alarm Play Interval Tests"));
		}
	CleanupStack::PopAndDestroy(alarmArray);
	__UHEAP_MARKEND;
	return TestStepResult();			
	}


TBool CTEAlarmTestAlarmPlayConfigStep::GetAndCheckAlarmPlayIntervalsL()
	{
	// Get Alarm Play Interval from Config, and Verify that it matches what the client has.
	iSoundIntervals = new(ELeave) CArrayFixFlat<TASCliSoundPlayDefinition>(5);

	// Get Interval info from Config
	TESTL(GetIntervalsFromConfigL(ConfigSection(),*iSoundIntervals));

	// Verify that Interval info matches GetAlarmPlayIntervalsL().
	CArrayFixFlat<TASCliSoundPlayDefinition>* intervalArrayFromClient = new(ELeave) CArrayFixFlat<TASCliSoundPlayDefinition>(5);
	CleanupStack::PushL(intervalArrayFromClient);
	iSession.GetAlarmPlayIntervalsL(*intervalArrayFromClient);
	INFO_PRINTF2(_L("AlarmPlayConfig: NumIntervals from Client=%d"), intervalArrayFromClient->Count());

	TESTL(CompareAlarmPlayIntervals(*iSoundIntervals, *intervalArrayFromClient));

	CleanupStack::PopAndDestroy(intervalArrayFromClient);
	return ETrue;
	}


TBool CTEAlarmTestAlarmPlayConfigStep::StartIntervalVerifierL(TAlarmId aAlarmId)
	{
	TESTL(GetIntervalRepeatFromConfig(ConfigSection(), iRepeatSetting));
	TESTL(GetTimerContingencyFromConfig(ConfigSection(), iTimerContingency));

	// Instantiate the various objects we need:
	CActiveScheduler* scheduler = new(ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);
	
	CActiveAlertListener* alarmListener = new(ELeave) CActiveAlertListener(*this, iCAASClient);
	CleanupStack::PushL(alarmListener);
	CActiveAlertListener* soundStartListener = new(ELeave) CActiveAlertListener(*this, iCAASClient);
	CleanupStack::PushL(soundStartListener);
	CActiveAlertListener* soundStopListener = new(ELeave) CActiveAlertListener(*this, iCAASClient);
	CleanupStack::PushL(soundStopListener);
	CInactivityTimer* inactivityTimer = CInactivityTimer::NewLC(*this);

	INFO_PRINTF3(_L("IntervalVerifier::Verify %d Intervals, with AlarmId %d"), (iSoundIntervals->Count()), (aAlarmId));
	
	TInt timeout;
	if ( iRepeatSetting == EAlarmSoundRepeatSettingStop &&
		 GetIntFromConfig(ConfigSection(), _L("Timeout"), timeout) )
		{
		INFO_PRINTF2(_L("IntervalVerifier::InactivyTimer set for %d seconds."), (timeout));
		inactivityTimer->After(timeout* 1000000);
		}
	
	if (iCAASClient.SetExtendedMode() != KErrNone)
		{
		INFO_PRINTF1(_L("IntervalVerifier::iCAASClient.SetExtendedMode() failed"));
		return EFalse;
		}

	// Setup the control variables
	iExpectedAlarmId = aAlarmId;
	iExpectedEventType = MAlarmEventHandler::EAlarmExpired;
	iExpectedInterval = 0;

	// Start the Active Listeners.
	INFO_PRINTF1(_L("IntervalVerifier::Start Listeners"));
	alarmListener->Start(MAlarmEventHandler::EAlarmExpired);
	soundStartListener->Start(MAlarmEventHandler::ESoundStarted);
	soundStopListener->Start(MAlarmEventHandler::ESoundStopped);

	// Start the Active Scheduler. The HandleAlarmEvent is responsible for stopping it.
	INFO_PRINTF1(_L("IntervalVerifier::Start Scheduler"));
	CActiveScheduler::Start();
	
	INFO_PRINTF1(_L("IntervalVerifier::Scheduler Completed"));

	// Cleanup after yourself.
	CleanupStack::PopAndDestroy(inactivityTimer);
	CleanupStack::PopAndDestroy(soundStopListener);
	CleanupStack::PopAndDestroy(soundStartListener);
	CleanupStack::PopAndDestroy(alarmListener);

	CActiveScheduler::Install(NULL);
	CleanupStack::PopAndDestroy(scheduler);

	if (TestStepResult()==EPass)
		{
		return ETrue;
		}
	else
		{
		INFO_PRINTF1(_L("IntervalVerifier::TestStepResult() != EPass"));
		return EFalse;
		}
	}

void CTEAlarmTestAlarmPlayConfigStep::HandleAlertEvent(MAlarmEventHandler::TAlertEvent aEventType, TAlarmId aAlarmId)
	{
	INFO_PRINTF4(_L("IntervalVerifier::AlertEvent Type=%d, AlarmId=%d, Interval=%d"), aEventType, aAlarmId, iExpectedInterval);

	if (aAlarmId != iExpectedAlarmId)
		{
		return;
		}

	if (aEventType != iExpectedEventType)
		{
		INFO_PRINTF3(_L("IntervalVerifier::AlertEvent Type Mismatch (%d != %d)"), aEventType, iExpectedEventType);
		SetTestStepResult(EFail);
		StopIntervalVerifier();
		}
	else if (TimeDiff(aAlarmId, aEventType) > iTimerContingency) // seconds
		{
		INFO_PRINTF2(_L("IntervalVerifier::EventTime exceeds contingency (%d seconds) "), iTimerContingency.Int());
		SetTestStepResult(EFail);
		StopIntervalVerifier();
		}
	else
		{
		switch(aEventType)
			{
		case(MAlarmEventHandler::EAlarmExpired):
			iExpectedEventType = ESoundStarted;
			break;
		case(MAlarmEventHandler::ESoundStarted):
			iExpectedEventType = ESoundStopped;
			break;
		case(MAlarmEventHandler::ESoundStopped):
			iExpectedEventType = ESoundStarted;
			// To complete the test, we want to make sure that we get an
			// "extra" interval.
			if (++iExpectedInterval > iSoundIntervals->Count())
				{
				StopIntervalVerifier();
				}
			break;
			}
		}
	}

TTimeIntervalSeconds CTEAlarmTestAlarmPlayConfigStep::TimeDiff(TAlarmId aAlarmId, MAlarmEventHandler::TAlertEvent aEventType)
	{
	// Get the current time:
	TTime timeNow;
	timeNow.UniversalTime();

	// Determine original expiry time for alarm
	TASShdAlarm alarm;
	if (iSession.GetAlarmDetails(aAlarmId, alarm) != KErrNone)
	// We didn't find the currently expired alarm. This should never happen.
		{
		INFO_PRINTF2(_L("IntervalVerifier::TimeDiff failed to find alarm with Id=%d"), aAlarmId);
		return iTimerContingency.Int() + 1; // Forces the test to fail.
		}

	TTime expectedTime;
	expectedTime = alarm.OriginalExpiryTime();
	TInt err = GetExpectedEventTime(expectedTime, aEventType);
	if(err != KErrNone)
		{
		INFO_PRINTF2(_L("IntervalVerifier::TimeDiff->GetExpectedEventTime() returned %d"), err);
		return iTimerContingency.Int() + 1; // Forces the test to fail.
		}

	TDateTime expTimeDT = expectedTime.DateTime();
	TDateTime timeNowDT = timeNow.DateTime();
	// Displayed date => DD/MM/YYYY HH:MM:SS
	INFO_PRINTF7(_L("IntervalVerifier::TimeDiff - Expected Time=%02d:%02d:%02d, Event Time=%02d:%02d:%02d"),
			expTimeDT.Hour(), expTimeDT.Minute(), expTimeDT.Second(),
			timeNowDT.Hour(), timeNowDT.Minute(), timeNowDT.Second());

	TTimeIntervalSeconds diff;
	(timeNow > expectedTime)?
			timeNow.SecondsFrom(expectedTime, diff):
			expectedTime.SecondsFrom(timeNow, diff);
	return diff;
	}

TInt CTEAlarmTestAlarmPlayConfigStep::GetExpectedEventTime(TTime& aExpectedTime, MAlarmEventHandler::TAlertEvent aEventType)
	{
	if ( iSoundIntervals->Count() == 0 )
		{
		// We only expect one event: EAlarmExpired, so don't change aExpectedTime:
		return KErrNone;
		}

	if ( iExpectedInterval < iSoundIntervals->Count() )
		{
		// Get original expiry time
		if (aEventType != EAlarmExpired)
			{
			aExpectedTime += TTimeIntervalMinutes(iSoundIntervals->At(iExpectedInterval).Offset().Int());
			if (aEventType != ESoundStarted)
				{
				// Only event left is ESoundStopped.
				aExpectedTime += TTimeIntervalSeconds(iSoundIntervals->At(iExpectedInterval).Duration().Int());
				}
			}
		}
	else
		{
		// we've exceeded the intervals; our test depends on the repeat setting
		switch (iRepeatSetting)
			{
		case(EAlarmSoundRepeatSettingLoop):
			{
			TASCliSoundPlayDefinition lastInterval = iSoundIntervals->At(iSoundIntervals->Count()-1);

			// How many times have we been around?
			TInt loopCount = (iExpectedInterval) / (iSoundIntervals->Count());
			// Restart time is offset by (last Offset + last Duration) * loopCount.
			aExpectedTime += TTimeIntervalMinutes(lastInterval.Offset().Int() * loopCount);
			aExpectedTime += TTimeIntervalSeconds(lastInterval.Duration().Int() * loopCount);
			
			TInt adjustedInterval = (iExpectedInterval) % (iSoundIntervals->Count());
			// We can safely assume that the aEventType is *not* EAlarmExpired
			aExpectedTime += TTimeIntervalMinutes(iSoundIntervals->At(adjustedInterval).Offset().Int());
			if (aEventType == ESoundStopped)
				{
				aExpectedTime += TTimeIntervalSeconds(iSoundIntervals->At(adjustedInterval).Duration().Int());
				}
			}
			break;
		case(EAlarmSoundRepeatSettingRepeatLast):
			{
			TASCliSoundPlayDefinition secondToLastInterval = iSoundIntervals->At(iSoundIntervals->Count()-2);
			TASCliSoundPlayDefinition lastInterval = iSoundIntervals->At(iSoundIntervals->Count()-1);

			TTimeIntervalMinutes lastOffsetDuration = lastInterval.Offset().Int() - secondToLastInterval.Offset().Int();
			

			// Determine the time that the last interval's sound stopped:
			TInt repeatCount = 1 + iExpectedInterval - (iSoundIntervals->Count());

			aExpectedTime += TTimeIntervalMinutes(lastOffsetDuration.Int() * repeatCount );

			// We can safely assume that the aEventType is *not* EAlarmExpired
			aExpectedTime += TTimeIntervalMinutes(lastInterval.Offset().Int());
			if (aEventType == ESoundStopped)
				{
				aExpectedTime += TTimeIntervalSeconds(lastInterval.Duration().Int());
				}
			}
			break;
		case(EAlarmSoundRepeatSettingStop):
			INFO_PRINTF3(_L("IntervalVerifier::Interval Index(%d) >= Interval Count(%d). RepeatSetting=Stop"),
						iExpectedInterval, iSoundIntervals->Count());
			return KErrGeneral;
		default:
			INFO_PRINTF3(_L("IntervalVerifier::Unexpected RepeatSetting(%d) with Interval Index(%d)"),
						iRepeatSetting, iExpectedInterval);
			return KErrCorrupt;
			}
		}
	return KErrNone;
	}

void CTEAlarmTestAlarmPlayConfigStep::InactivityTimerExpired()
	{
	INFO_PRINTF1(_L("IntervalVerifier::InactivityTimerExpired."));
	TEST(iRepeatSetting == EAlarmSoundRepeatSettingStop);
	TEST(iExpectedInterval <= iSoundIntervals->Count() );
	StopIntervalVerifier();
	}

void CTEAlarmTestAlarmPlayConfigStep::StopIntervalVerifier()
	{
	INFO_PRINTF1(_L("IntervalVerifier::Stop Scheduler"));
	CActiveScheduler::Stop();
	}

/*
 * Initializes the Active Object and adds itself to the Scheduler.
 */
CActiveAlertListener::CActiveAlertListener(MAlarmEventHandler& aHandler, RANTestClient& aClient) : CActive(EPriorityStandard), iHandler(aHandler), iClient(aClient)
	{
	CActiveScheduler::Add(this);
	}

/*
 * Cleanups after the Active Object.
 */
CActiveAlertListener::~CActiveAlertListener()
	{
	Deque();
	}

/*
 * Starts the Active Objects by regitering for a asynchronous notification
 * Called by the test step.
 */
TBool CActiveAlertListener::Start(MAlarmEventHandler::TAlertEvent aEventTypeToWaitFor)
	{
	if (IsActive())
		{
		return EFalse;
		}

	switch(aEventTypeToWaitFor)
		{
	case(MAlarmEventHandler::EAlarmExpired):
	case(MAlarmEventHandler::ESoundStarted):
	case(MAlarmEventHandler::ESoundStopped):
		iEventType = aEventTypeToWaitFor;
		break;
	default:
		return EFalse;
		}
	RegisterForNextEvent();
	return ETrue;
	}

void CActiveAlertListener::DoCancel()
	{
	iClient.CancelNotifications();
	}

/*
 * We've receive an event, be sure to pass it along to the test step for proper handling...
 */
void CActiveAlertListener::RunL()
	{
	if (iStatus == KErrNone)
		{
		if (iEventType == MAlarmEventHandler::EAlarmExpired)
			{
			iAlarmId = iExpiredAlarm.Id();
			}
		RegisterForNextEvent();
		}
	iHandler.HandleAlertEvent(iEventType, iAlarmId);
	}

/*
 * Register For the next event.
 */
void CActiveAlertListener::RegisterForNextEvent()
	{
	switch(iEventType)
		{
	case(MAlarmEventHandler::EAlarmExpired):
		iClient.NotifyOnAlarm(iExpiredAlarm, iStatus);
		break;
	case(MAlarmEventHandler::ESoundStarted):
		iClient.NotifyOnSoundStart(iAlarmId, iFileName, iStatus);
		break;
	case(MAlarmEventHandler::ESoundStopped):
		iClient.NotifyOnSoundStop(iAlarmId, iStatus);
		break;
	default:
		// We should have never gotten to here:
		User::Invariant();
		}
	SetActive();
	}

/*
 * This object is used for verifying the RepeatSetting=Stop behavior.
 */
CInactivityTimer* CInactivityTimer::NewLC(MAlarmEventHandler& aHandler)
	{
	CInactivityTimer* timer = new(ELeave) CInactivityTimer(aHandler);
	CleanupStack::PushL(timer);
	timer->ConstructL();
	return timer;
	}

/*
 * Let's the test step know that the timer expired.
 */
void CInactivityTimer::RunL()
	{
	iHandler.InactivityTimerExpired();
	}
	
/*
 * Initializes the object
 */
CInactivityTimer::CInactivityTimer(MAlarmEventHandler& aHandler) : CTimer(EPriorityStandard), iHandler(aHandler)
	{
	CActiveScheduler::Add(this);
	}
	
CTEAlarmTestAPCSetIntervalsStep::~CTEAlarmTestAPCSetIntervalsStep()
	{
	}
	
CTEAlarmTestAPCSetIntervalsStep::CTEAlarmTestAPCSetIntervalsStep()
	{
	SetTestStepName(KTEAlarmTestAPCSetIntervalsStep);
	}

TVerdict CTEAlarmTestAPCSetIntervalsStep::doTestStepL()
	{
	// Check result from preamble and setup test step result.
	if(TestStepResult() != EPass)
		{
		return TestStepResult();
		}
	
	__UHEAP_MARK;
	// Get Alarm Play Interval from Config, and Verify that it matches what the client has.
	CArrayFixFlat<TASCliSoundPlayDefinition>* intervalsFromConfig = new(ELeave) CArrayFixFlat<TASCliSoundPlayDefinition>(5);
	CleanupStack::PushL(intervalsFromConfig);

	// Get Interval info from Config
	TESTL(GetIntervalsFromConfigL(ConfigSection(),*intervalsFromConfig));

	INFO_PRINTF2(_L("Loaded Intervals From Config - Count=%d"), intervalsFromConfig->Count());

	// Verify that Interval info matches GetAlarmPlayIntervalsL().
	CArrayFixFlat<TASCliSoundPlayDefinition>* origIntervalsFromClient = new(ELeave) CArrayFixFlat<TASCliSoundPlayDefinition>(5);
	CleanupStack::PushL(origIntervalsFromClient);
		
	iSession.GetAlarmPlayIntervalsL(*origIntervalsFromClient);
	INFO_PRINTF2(_L("Loaded Intervals From Client - Count=%d"), origIntervalsFromClient->Count());

	INFO_PRINTF1(_L("Make sure there are differences between the current and new intervals:"));
	TESTL(!CompareAlarmPlayIntervals(*intervalsFromConfig, *origIntervalsFromClient));

	INFO_PRINTF2(_L("Setting AlarmPlayIntervals, Count = %d"), intervalsFromConfig->Count());

	if( 0 == intervalsFromConfig->Count() )
		{
		TRAPD(err, iSession.SetAlarmPlayIntervalsL(*intervalsFromConfig));
		TESTL(err == KErrArgument || err == KErrGeneral);
		}
	else if ( 0 == origIntervalsFromClient->Count() )
		{
		TRAPD(err, iSession.SetAlarmPlayIntervalsL(*intervalsFromConfig));
		TESTL(err == KErrNotSupported );
		}
	else
		{
		iSession.SetAlarmPlayIntervalsL(*intervalsFromConfig);
		}
		
	CArrayFixFlat<TASCliSoundPlayDefinition>* updatedIntervalsFromClient = new(ELeave) CArrayFixFlat<TASCliSoundPlayDefinition>(5);
	CleanupStack::PushL(updatedIntervalsFromClient);

	iSession.GetAlarmPlayIntervalsL(*updatedIntervalsFromClient);
	INFO_PRINTF2(_L("Loaded Intervals From Client - Count=%d"), updatedIntervalsFromClient->Count());

	if (origIntervalsFromClient->Count() == 0)
		{
		INFO_PRINTF1(_L("SoundIntervals are disabled in Client - Setting should have failed"));
		TESTL(CompareAlarmPlayIntervals(*origIntervalsFromClient, *updatedIntervalsFromClient));
		}
	else if (intervalsFromConfig->Count() == 0)
		{
		INFO_PRINTF1(_L("SoundIntervals are enabled in Client - Disabling should have failed"));
		TESTL(CompareAlarmPlayIntervals(*origIntervalsFromClient, *updatedIntervalsFromClient));
		}
	else
		{
		TESTL(CompareAlarmPlayIntervals(*intervalsFromConfig, *updatedIntervalsFromClient));
		}

	CleanupStack::PopAndDestroy(updatedIntervalsFromClient);
	CleanupStack::PopAndDestroy(origIntervalsFromClient);
	CleanupStack::PopAndDestroy(intervalsFromConfig);
	__UHEAP_MARKEND;
	return TestStepResult();
	}

