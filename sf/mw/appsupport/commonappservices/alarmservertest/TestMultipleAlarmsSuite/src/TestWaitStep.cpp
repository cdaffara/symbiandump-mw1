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
// Contains implementation of CTestWaitStep class
// 
//

/**
 @file
 @internalTechnology 
*/

// User Include
#include "TestWaitStep.h"

/**
Constructor. Sets the test step name. Testserver reference passed to make use 
of TEF's method of sharing data between test steps
@internalTechnology
@test
*/
CTestWaitStep::CTestWaitStep(CTestMultipleAlarmsServer& aTestServer) 
: CTestBaseStep(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestWaitStep);
	}

/**
Base class pure virtual.
@return		EPass or EFail indicating the result of the test step.
@internalTechnology
@test
*/
TVerdict CTestWaitStep::doTestStepL()
	{
	TInt error = KErrNone;
	TInt after = 0;
	TPtrC at(KNullDesC);
	if(ReadIntsFromConfig(ConfigSection(), &KIniAfter(), &after, NULL))
		{
		TRAP(error, SleepAndWakeUpL(after));
		PrintIfError(error);
		}
	else if(ReadStringsFromConfig(ConfigSection(), &KIniAt(), &at, NULL))
		{
		TRAP(error, SleepAndWakeUpL(at));
		PrintIfError(error);
		}
	else
		{
		IniProblem();
		}	
	return TestStepResult();	
	}

/**
Starts a timer and delays execution till it expires
@param aAfter Minutes after which the timer must wake-up. 
YYYYMMDD:HHMMSS.MMMMMM.
@internalTechnology
@test
*/
void CTestWaitStep::SleepAndWakeUpL(const TInt& aAfter)
	{
	RTimer timer;
	TRequestStatus status;
	
	// Leave if not able to create Kernel object
	User::LeaveIfError(timer.CreateLocal());
	// Convert minutes into microseconds and start timer
	timer.After(status, TTimeIntervalMicroSeconds32(aAfter * 60 * 1000000));
	// Wait for timer to expire	
	User::WaitForRequest(status);
	// Eat signal. Leave if something is wrong
	User::LeaveIfError(status.Int());	
	}

/**
Starts a timer and delays execution till it expires
@param aAt Time at which the timer must wake-up. Format is 
YYYYMMDD:HHMMSS.MMMMMM.
@internalTechnology
@test
*/
void CTestWaitStep::SleepAndWakeUpL(const TDesC& aAt)
	{
	TTime time;
	RTimer timer;
	TRequestStatus status;
		
	// Leave if not able to create Kernel object
	User::LeaveIfError(timer.CreateLocal());
	// Create TTime from string
	User::LeaveIfError(time.Set(aAt));
	// start timer
	timer.At(status, time);
	// Wait for timer to expire	
	User::WaitForRequest(status);
	// Eat signal. Leave if something is wrong
	User::LeaveIfError(status.Int());	
	}
