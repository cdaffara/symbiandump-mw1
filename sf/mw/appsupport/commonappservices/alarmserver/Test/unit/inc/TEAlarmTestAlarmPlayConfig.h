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

#ifndef __TEALARMTESTALARMPLAYCONFIG_H__
#define __TEALARMTESTALARMPLAYCONFIG_H__

#include "TEAlarmTestStepBase.h"
#include "consoleantestclient.h"

class MAlarmEventHandler
	{
	public:
	enum TAlertEvent
		{
		EAlarmExpired,
		ESoundStarted,
		ESoundStopped
		};

	virtual void HandleAlertEvent(TAlertEvent aEventType, TAlarmId aAlarmId) = 0;
	virtual void InactivityTimerExpired() = 0;
	};

class CTEAlarmTestAlarmPlayConfigStep : public CTEAlarmTestStepBase, MAlarmEventHandler
	{
public:
	CTEAlarmTestAlarmPlayConfigStep();
	~CTEAlarmTestAlarmPlayConfigStep();
	virtual TVerdict doTestStepL();
	
protected:
	TBool GetAndCheckAlarmPlayIntervalsL();
	TBool StartIntervalVerifierL(TAlarmId aAlarmId);
	void HandleAlertEvent(TAlertEvent aEventType, TAlarmId aAlarmId);
	void InactivityTimerExpired();
	void StopIntervalVerifier();
	TTimeIntervalSeconds TimeDiff(TAlarmId aAlarmId, MAlarmEventHandler::TAlertEvent aEventType);
	TInt GetExpectedEventTime(TTime& aExpectedTime, MAlarmEventHandler::TAlertEvent aEventType);
private:
	CArrayFixFlat<TASCliSoundPlayDefinition>* iSoundIntervals;
	TAlarmSoundRepeatSetting iRepeatSetting;
	
	TAlarmId iExpectedAlarmId;
	MAlarmEventHandler::TAlertEvent iExpectedEventType;
	TTime iExpectedEventTime;
	TInt iExpectedInterval;
	TTimeIntervalSeconds iTimerContingency;
	};

class CActiveAlertListener : public CActive
	{
public:
	CActiveAlertListener(MAlarmEventHandler& aHandler, RANTestClient& aClient);
	~CActiveAlertListener();
	TBool Start(MAlarmEventHandler::TAlertEvent aEventTypeToWaitFor);
	virtual void DoCancel();
	virtual void RunL();
private:
	void RegisterForNextEvent();
protected:
	MAlarmEventHandler::TAlertEvent iEventType;
	TASShdAlarm iExpiredAlarm;
	TAlarmId iAlarmId;
	MAlarmEventHandler& iHandler;
	RANTestClient& iClient;
	TBuf<32> iFileName;
	};

class CInactivityTimer : public CTimer
	{
public:
	static CInactivityTimer* NewLC(MAlarmEventHandler& aHandler);
	virtual void RunL();
private:
	CInactivityTimer(MAlarmEventHandler& aHandler);
private:
	MAlarmEventHandler& iHandler;
	};

class CTEAlarmTestAPCSetIntervalsStep : public CTEAlarmTestStepBase
	{
public:
	CTEAlarmTestAPCSetIntervalsStep();
	~CTEAlarmTestAPCSetIntervalsStep();
	virtual TVerdict doTestStepL();
	
protected:
private:
	};

		
_LIT(KTEAlarmTestAlarmPlayConfigStep, "AlarmPlayConfig");
_LIT(KTEAlarmTestAPCSetIntervalsStep, "APCSetIntervals");

#endif // __TEALARMTESTALARMPLAYCONFIG_H__
