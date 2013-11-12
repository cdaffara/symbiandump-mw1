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
// Contains declaration of CTestAlarmSnoozeStep class, which is used for snoozing a 
// a particular alarm.
// 
//

#ifndef __TEST_ALARM_SNOOZE_STEP_H__
#define __TEST_ALARM_SNOOZE_STEP_H__

// User Includes
#include "TestBaseStep.h"

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestAlarmSnoozeStep, "TestAlarmSnoozeStep");
/*@}*/

// Default minutes to snooze
const TInt KDefaultMinsToSnooze = 3;

/**
Test step to snooze a particular alarm
@internalTechnology
@test
*/
class CTestAlarmSnoozeStep : public CTestBaseStep
	{
public:
	CTestAlarmSnoozeStep(CTestMultipleAlarmsServer& aTestServer);

	// TEF pure virtual
	TVerdict 				doTestStepL();
	
private:
	void					SnoozeAlarmL(const TDesC& aAlarmMessage);
	};

#endif		// __TEST_ALARM_SNOOZE_STEP_H__
