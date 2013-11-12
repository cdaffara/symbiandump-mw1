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
// Contains declaration of CTestCleanupStep class, which is used for cleaning
// up the operations done by the previous test steps and reverting to initial state
// 
//

#ifndef __TEST_CLEANUP_STEP_H__
#define __TEST_CLEANUP_STEP_H__

// User Includes
#include "TestBaseStep.h"

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestCleanupStep, "TestCleanupStep");
/*@}*/

/**
Test step to cleanup and revert to initial state
@internalTechnology
@test
*/
class CTestCleanupStep : public CTestBaseStep
	{
public:
	CTestCleanupStep(CTestMultipleAlarmsServer& aTestServer);

	// TEF pure virtual
	TVerdict 				doTestStepL();
	
private:
	void 					DoCleanupL();
	void 					DeleteCalEntriesL(CCalEntryView* aCalEntryView);
	void 					DeleteAlarmsL();
	};

#endif		// __TEST_CLEANUP_STEP_H__
