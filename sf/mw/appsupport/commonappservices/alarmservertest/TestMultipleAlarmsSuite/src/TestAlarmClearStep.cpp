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
// Contains implementation of CTestAlarmClearStep class
// 
//

/**
 @file
 @internalTechnology 
*/

// User Include
#include "TestAlarmClearStep.h"
#include "DummyAlarmControl.h"

/**
Constructor. Sets the test step name. Testserver reference passed to make use 
of TEF's method of sharing data between test steps
@internalTechnology
@test
*/
CTestAlarmClearStep::CTestAlarmClearStep(CTestMultipleAlarmsServer& aTestServer) 
: CTestBaseStep(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestAlarmClearStep);
	}

/**
Base class pure virtual.
@return		EPass or EFail indicating the result of the test step.
@internalTechnology
@test
*/
TVerdict CTestAlarmClearStep::doTestStepL()
	{
	TPtrC alarmMessage;
	TPtrC clearAll;
	if(ReadStringsFromConfig(ConfigSection(), &KIniClearAll(), &clearAll, NULL) 
	&& clearAll.Compare(KYes) == KErrNone )
		{// Clear all request is to be issued
		TRAPD(error, ClearAllAlarmsL());
		PrintIfError(error);
		}
	else if(!ReadStringsFromConfig(ConfigSection(), &KIniAlarmMessage(), &alarmMessage, NULL))
		{// Not Clear all. And no alarm-message too
		IniProblem();
		}
	else
		{
		TRAPD(error, ClearAlarmL(alarmMessage));
		PrintIfError(error);
		}
	return TestStepResult();	
	}

/**
Asks the Alert Server to clear all the alarms
@internalTechnology
@test
*/
void CTestAlarmClearStep::ClearAllAlarmsL()
	{
	TestServer()->AlertServer()->ClearAllAlarmsL();
	}

/**
Clears the alarm whose message is the one that is passed as parameter
The clearing is actually done by the active object running from 
alert server thread
@param aAlarmMessage Message to identify the alarm control object
@internalTechnology
@test
*/
void CTestAlarmClearStep::ClearAlarmL(const TDesC& aAlarmMessage)
	{
	TRequestStatus status(KRequestPending);
	TestServer()->AlarmControlsManager()->ClearAlarmL(aAlarmMessage, &status);
	User::WaitForRequest(status);
	User::LeaveIfError(status.Int());
	}
	
