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
// Contains declaration of CTestWaitStep class, which is used for delaying
// test execution for a time period or until a specified time.
// 
//

#ifndef __TEST_WAIT_STEP_H__
#define __TEST_WAIT_STEP_H__

// User Includes
#include "TestBaseStep.h"

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestWaitStep, "TestWaitStep");
/*@}*/

/**
Test step to suspend test execution for some time
@internalTechnology
@test
*/
class CTestWaitStep : public CTestBaseStep
	{
public:
	CTestWaitStep(CTestMultipleAlarmsServer& aTestServer);

	// TEF pure virtual
	TVerdict 				doTestStepL();
	
private:
	void 			SleepAndWakeUpL(const TInt& aAfter);
	void 			SleepAndWakeUpL(const TDesC& aAt);
	};

#endif		// __TEST_WAIT_STEP_H__
