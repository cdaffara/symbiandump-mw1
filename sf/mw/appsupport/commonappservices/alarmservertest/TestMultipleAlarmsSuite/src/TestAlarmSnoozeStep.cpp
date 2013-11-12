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
// Contains implementation of CTestAlarmSnoozeStep class
// 
//

/**
 @file
 @internalTechnology 
*/

// User Include
#include "TestAlarmSnoozeStep.h"
#include "DummyAlarmControl.h"

/**
Constructor. Sets the test step name. Testserver reference passed to make use 
of TEF's method of sharing data between test steps
@internalTechnology
@test
*/
CTestAlarmSnoozeStep::CTestAlarmSnoozeStep(CTestMultipleAlarmsServer& aTestServer) 
: CTestBaseStep(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestAlarmSnoozeStep);
	}

/**
Base class pure virtual.
@return		EPass or EFail indicating the result of the test step.
@internalTechnology
@test
*/
TVerdict CTestAlarmSnoozeStep::doTestStepL()
	{
	TPtrC alarmMessage;
	if(!ReadStringsFromConfig(ConfigSection(), &KIniAlarmMessage(), &alarmMessage, NULL))
		{
		IniProblem();
		}
	else
		{
		TRAPD(error, SnoozeAlarmL(alarmMessage));
		PrintIfError(error);
		}
	return TestStepResult();	
	}

/**
Snoozes the alarm whose message is the one that is passed as parameter
The snoozing is actually done by the active object running from 
alert server thread, as we can not snooze from another thread, as RTimer
(used by CPeriodic) which is used in the alert server thread will raise a 
bad handle panic.
@param aAlarmMessage Message to identify the alarm control object
@internalTechnology
@test
*/
void CTestAlarmSnoozeStep::SnoozeAlarmL(const TDesC& aAlarmMessage)
	{
	TInt minsToSnooze;
	if(!ReadIntsFromConfig(ConfigSection(), &KIniMinsToSnooze(), &minsToSnooze, NULL))
		{// assign default
		minsToSnooze = KDefaultMinsToSnooze;
		}
	TRequestStatus status(KRequestPending);
	TestServer()->AlarmControlsManager()->SnoozeAlarmL(aAlarmMessage, &status, minsToSnooze);
	User::WaitForRequest(status);
	User::LeaveIfError(status.Int());
	}
