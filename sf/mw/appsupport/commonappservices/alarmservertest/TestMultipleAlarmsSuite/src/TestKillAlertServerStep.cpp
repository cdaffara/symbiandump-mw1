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
// Contains implementation of CTestKillAlertServerStep class
// 
//

/**
 @file
 @internalTechnology 
*/

// User Include
#include "TestKillAlertServerStep.h"
#include "AlarmControlsManager.h"

/**
Constructor. Sets the test step name. Testserver reference passed to make use 
of TEF's method of sharing data between test steps
@internalTechnology
@test
*/
CTestKillAlertServerStep::CTestKillAlertServerStep(CTestMultipleAlarmsServer& aTestServer) 
: CTestBaseStep(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestKillAlertServerStep);
	}

/**
Base class pure virtual.
@return		EPass or EFail indicating the result of the test step.
@internalTechnology
@test
*/
TVerdict CTestKillAlertServerStep::doTestStepL()
	{
	INFO_PRINTF1(_L("Killing Alert Server Thread..."));
	
	TRequestStatus status(KRequestPending);
	TestServer()->AlarmControlsManager()->EndAlertServerThreadL(&status);
	User::WaitForRequest(status);
	PrintIfError(status.Int());

	return TestStepResult();	
	}

