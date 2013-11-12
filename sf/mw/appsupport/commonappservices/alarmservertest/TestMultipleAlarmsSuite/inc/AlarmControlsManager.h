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
// Contains declaration of CAlarmControlsManager class, which implements the 
// MEikServAlarmFactory mixin
// 
//

#ifndef __ALARM_CONTROLS_MANAGER_H__
#define __ALARM_CONTROLS_MANAGER_H__

// User Includes
#include "TestMultipleAlarmsServer.h"

// System Includes
#include <uikon/eiksvfty.h>
#include <asshdalarm.h>
#include <test/testexecutelog.h>
#include <uikon/eiknotifyalert.h>

namespace TestMultipleAlarms
	{
	enum TAlarmControlState
		{
		EConstructed,
		ERelease,
		EShowAlarm,
		EHideAlarm,
		ECurrentServerState, 
		EUpdateSoundPauseTimeInterval,
		EUpdateForAlarmServerState, 
		EUpdateAlarmInfo, 
		EStartPlayAlarm, 
		EStopPlayAlarm
		};
		
	enum TAlarmAction
		{
		ESnooze,
		EClear,
		EStopActiveScheduler
		};		
	}
	
class CDummyAlarmControl;
class CAlarmActionPerformer;

/**
Class which implements the MEikServAlarmFactory mixin.
@internalTechnology
@test
*/
class CAlarmControlsManager : public CBase, public MEikServAlarmFactory
	{
public:
	
	// 1st phase Construction
	static CAlarmControlsManager* CAlarmControlsManager::NewL(CTestMultipleAlarmsServer& aTestServer);
	
	// Destruction
	~CAlarmControlsManager();
	
	// From MEikServAlarmFactory
	MEikServAlarm* 						NewAlarmL(CEikAlmControlSupervisor& aSupervisor);
	
	// Other functions
	MEikServAlarm* 						GetAlarmControlObjectL(const TDesC& aAlarmMessage);
	void 								IncrementNoOfCurrentlyNotifyingAlarms();
	void 								DecrementNoOfCurrentlyNotifyingAlarms();
	void 								UpdateCurrentlyPlayingAlarmId(const TAlarmId& aAlarmId);
	TInt 								CurrentMaxAlarms();
	const TDesC& 						GetFunctionName(TInt aCallBack);
	void 								PrintLog(const TDesC& aMessage) const;
	void 								RemoveFromList(CDummyAlarmControl* aDummyAlarmControl);
	TInt 								GetAlarmControlListCount();
	RPointerArray<MEikServAlarm> 		GetAlarmControlListL();
	void 								SnoozeAlarmL(const TDesC& aAlarmMessage, TRequestStatus* aStatus, const TInt& aMinsToSnooze);
	void 								ClearAlarmL(const TDesC& aAlarmMessage, TRequestStatus* aStatus);
	void 								EndAlertServerThreadL(TRequestStatus* aStatus);
	
private:
	// Constructor
	CAlarmControlsManager(CTestMultipleAlarmsServer& aTestServer);
	// 2nd phase Construction
	void ConstructL();
	
	void 								SignalAlertServerThread();
	void								SetAlarmActionCommonParamsL(const TDesC& aAlarmMessage, TRequestStatus* aStatus, const TestMultipleAlarms::TAlarmAction& aAlarmAction);

private:
	RPointerArray<MEikServAlarm> iAlarmControlList;
	CTestMultipleAlarmsServer& iTestServer;
	CAlarmActionPerformer* iAlarmActionPerformer;
	};

#endif		// __ALARM_CONTROLS_MANAGER_H__

