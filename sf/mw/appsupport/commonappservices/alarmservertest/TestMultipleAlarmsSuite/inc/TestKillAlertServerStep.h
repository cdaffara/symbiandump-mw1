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
// Contains declaration of CTestKillAlertServerStep class, which is used for 
// killing the alert server thread
// 
//

#ifndef __TEST_KILL_ALERT_SERVER_STEP_H__
#define __TEST_KILL_ALERT_SERVER_STEP_H__

// User Includes
#include "TestBaseStep.h"

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestKillAlertServerStep, "TestKillAlertServerStep");
/*@}*/

/**
Test step to kill the alert server thread
@internalTechnology
@test
*/
class CTestKillAlertServerStep : public CTestBaseStep
	{
public:
	CTestKillAlertServerStep(CTestMultipleAlarmsServer& aTestServer);

	// TEF pure virtual
	TVerdict 				doTestStepL();
	};

#endif		// __TEST_KILL_ALERT_SERVER_STEP_H__
