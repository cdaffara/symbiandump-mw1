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
// Contains declaration of CAlarmActionPerformer class, which is used to
// perform actions like snooze, clear etc. on an alarm
// 
//

#ifndef __ALARM_ACTION_PERFORMER_H__
#define __ALARM_ACTION_PERFORMER_H__

// User Includes
#include "DummyAlarmControl.h"
#include "AlarmControlsManager.h"

/**
Class used to perform actions like snooze, clear etc. on an alarm
@internalTechnology
@test
*/
class CAlarmActionPerformer : public CActive
	{
public:	
	CAlarmActionPerformer();
	
	// From CActive
	void					RunL();
	void 					DoCancel();

	TRequestStatus* 		Status();
	void 					SetCommonParams(CDummyAlarmControl* aAlarmControl, TRequestStatus* aStatus, RThread& aTestStepThreadDuplicateHandle, const TestMultipleAlarms::TAlarmAction& aAlarmAction);
	void 					SetMinsToSnooze(const TInt& aMinsToSnooze);
	
private:
	TRequestStatus* 					iExternalRequest;
	CDummyAlarmControl*					iAlarmControl;
	TInt 								iMinsToSnooze;
	RThread								iTestStepThreadDuplicateHandle;
	TestMultipleAlarms::TAlarmAction	iAlarmAction;
	};

#endif		// __ALARM_ACTION_PERFORMER_H__


