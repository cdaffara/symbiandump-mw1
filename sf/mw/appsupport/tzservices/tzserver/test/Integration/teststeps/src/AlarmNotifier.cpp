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
 
#include "AlarmNotifier.h"
#include <asshdalarm.h>

CAlarmNotifier* CAlarmNotifier::NewL(RASCliSession& aAlarmServer)
	{
	CAlarmNotifier* self = new(ELeave) CAlarmNotifier(aAlarmServer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CAlarmNotifier::~CAlarmNotifier()
	{
	Cancel();
	}

CAlarmNotifier::CAlarmNotifier(RASCliSession& aAlarmServer):CActive(EPriorityStandard), iAlarmServer(aAlarmServer)
	{
	CActiveScheduler::Add(this);
	}

void CAlarmNotifier::ConstructL()
	{
	}

void CAlarmNotifier::RunL()
	{
	TASShdAlarm alarm;
	iAlarmServer.GetAlarmDetails(iAlarmId, alarm);
	
	if(alarm.State() == EAlarmStateNotifying)
		{
		CActiveScheduler::Stop();
		}
	else
		{
		iAlarmServer.NotifyChange(iStatus,iAlarmId);
		SetActive();	
		}
	}

void CAlarmNotifier::DoCancel()
	{
	iAlarmServer.NotifyChangeCancel();
	}
	
void CAlarmNotifier::InitialiseAlarm(const TTime aDueTime, TBool aIsUtcAlarm)
	{	
	iAlarm.Reset();
	_LIT(KAlarmMessage, "UTC Alarm Integration Test");
	iAlarm.Message() = KAlarmMessage();
	if (aIsUtcAlarm)
		{
		iAlarm.SetUtcNextDueTime(aDueTime);
		}
	else
		{
		iAlarm.NextDueTime() = aDueTime;
		}
	iAlarm.DayOrTimed() = EASShdAlarmTypeTimed;
	}
	
void CAlarmNotifier::Start(const TTime aDueTime, TBool aIsUtcAlarm)
	{
	// Start should not be called if request is already pending.
	// This is also pre-empting SetActive() PANICing
	__ASSERT_DEBUG(IsActive()==EFalse, User::Invariant());
	InitialiseAlarm(aDueTime, aIsUtcAlarm);

	iAlarmServer.AlarmAddWithNotification(iStatus, iAlarm);
	
	iAlarmId = iAlarm.Id();
	SetActive();
	}
