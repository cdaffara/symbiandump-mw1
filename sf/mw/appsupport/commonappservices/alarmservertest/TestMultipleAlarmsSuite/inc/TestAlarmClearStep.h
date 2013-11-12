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
// Contains declaration of CTestAlarmClearStep class, which is used for clearing a 
// a particular alarm.
// 
//

#ifndef __TEST_ALARM_CLEAR_STEP_H__
#define __TEST_ALARM_CLEAR_STEP_H__

// User Includes
#include "TestBaseStep.h"

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestAlarmClearStep, "TestAlarmClearStep");
/*@}*/

/**
Test step to clear a particular alarm
@internalTechnology
@test
*/
class CTestAlarmClearStep : public CTestBaseStep
	{
public:
	CTestAlarmClearStep(CTestMultipleAlarmsServer& aTestServer);

	// TEF pure virtual
	TVerdict 				doTestStepL();
	
private:
	void					ClearAlarmL(const TDesC& aAlarmMessage);
	void					ClearAllAlarmsL();
	};

#endif		// __TEST_ALARM_CLEAR_STEP_H__
