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
// Contains declaration of CDummyAlarmControl class, which implements the 
// MEikServAlarm mixin
// 
//

#ifndef __DUMMY_ALARM_CONTROL__
#define __DUMMY_ALARM_CONTROL__

// User Includes
#include "TestMultipleAlarmsServer.h"
#include "AlarmControlsManager.h"

// System Includes
#include <uikon/eiksvfty.h>
#include <asshdalarm.h>
#include <test/testexecutelog.h>
#include <uikon/eiknotifyalert.h>

/**
Class which implements the MEikServAlarm mixin.
@internalTechnology
@test
*/
class CDummyAlarmControl : public CBase, public MEikServAlarm
	{
public:	
	// Construction
	CDummyAlarmControl(CEikAlmControlSupervisor* aSupervisor, CAlarmControlsManager* aAlarmControlsManager);
	
	// Destruction
	~CDummyAlarmControl();

	// From MEikServAlarm
	virtual void Release();
	virtual void ShowAlarm();
	virtual void HideAlarm();
	virtual TInt CurrentServerState() const;
	virtual void UpdateSoundPauseTimeInterval(TInt aMinutes);
	virtual void UpdateForAlarmServerState(TInt aNewAlarmServerState);
	virtual void UpdateAlarmInfo(const TASShdAlarm& aAlarm,const TFullName& aOwner);
	virtual void StartPlayAlarmL(const TDesC& aAlarmName);
	virtual void StopPlayAlarm();
	
	// Other functions
	const TDesC& AlarmMessage();
	const TDesC& AlarmSoundName();
	CEikAlmControlSupervisor* Supervisor();
	const TASShdAlarm& AlarmObject();
	TBool IsSnoozed();
	TBool IsShowing();
	void PrintWhoWasCalledL(TestMultipleAlarms::TAlarmControlState aAlarmControlState) const;
	void PrintLog(const TDesC& aMessage) const;
	TTime ActualNotificationTime();
	TTime OriginalExpiryTime();
	TInt MinsToSnooze();
	
private:
	CEikAlmControlSupervisor* iSupervisor;
	CAlarmControlsManager* iMyManager;
	TInt iCurrentAlarmServerState;
	TPtrC iAlarmMessage;
	TTime iMyNotificationTime;
	TTime iOrigExpTime;
	TInt iMinsToSnooze;
	TASShdAlarm iAlarm;
	TBool iIsSnoozed;
	TBool iIsShowing;
	TBool iIsPlaying;
	
	HBufC* iAlarmSoundName;
	};

#endif		// __DUMMY_ALARM_CONTROL__

