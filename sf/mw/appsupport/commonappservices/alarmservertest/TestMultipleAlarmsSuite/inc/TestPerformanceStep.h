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
// Contains declaration of CTestPerformanceStep class, which does some 
// performance tests like adding a very large no. of entries with imminent 
// alarms, with associated data, and check the time taken for 
// getting notfications from alarm server,  retreiving the data etc.
// 
//

#ifndef __TEST_PERFORMANCE_STEP_H__
#define __TEST_PERFORMANCE_STEP_H__

// User Includes
#include "TestAssociatedDataStep.h"
#include "DummyAlarmControl.h"

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestPerformanceStep, "TestPerformanceStep");
/*@}*/

/**
Test step to suspend test execution for some time
@internalTechnology
@test
*/
class CTestPerformanceStep : public CTestAssociatedDataStep
	{
public:
	CTestPerformanceStep(CTestMultipleAlarmsServer& aTestServer);

	// TEF pure virtual
	TVerdict 				doTestStepL();

private:
	void 					TestPerformanceL(const TReal32& aDataReadTime, const TReal32& aNotificationTime);
	void 					TestAssociatedDataPerformanceL(CDummyAlarmControl* alarmControl, const TReal32& aDataReadTime);
	void					TestAlarmNotificationPerformanceL(CDummyAlarmControl* alarmControl, const TReal32& aNotificationTime);
	};

#endif		// __TEST_PERFORMANCE_STEP_H__
