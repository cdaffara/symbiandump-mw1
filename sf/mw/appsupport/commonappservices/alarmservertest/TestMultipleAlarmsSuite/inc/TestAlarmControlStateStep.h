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
// Contains declaration of CTestAlarmControlStateStep class, which is used 
// to test whether the expected alarms are notifying and other checks such 
// as number of alarm control objects <= maxalarms value etc.
// 
//

#ifndef __TEST_ALARM_CONTROL_STATE_STEP_H__
#define __TEST_ALARM_CONTROL_STATE_STEP_H__

// User Includes
#include "TestBaseStep.h"

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestAlarmControlStateStep, "TestAlarmControlStateStep");
/*@}*/

/**
Test step to test whether the expected alarms are notifying and 
other checks such as number of alarm control objects <= maxalarms value etc.
@internalTechnology
@test
*/
class CTestAlarmControlStateStep : public CTestBaseStep
	{
public:
	CTestAlarmControlStateStep(CTestMultipleAlarmsServer& aTestServer);

	// TEF virtuals
	virtual enum TVerdict 	doTestStepL();
	
private:
	void 					TestAlarmControlStateL(const TPtrC& aAlarmMessage);
	};

#endif		// __TEST_ALARM_CONTROL_STATE_STEP_H__
