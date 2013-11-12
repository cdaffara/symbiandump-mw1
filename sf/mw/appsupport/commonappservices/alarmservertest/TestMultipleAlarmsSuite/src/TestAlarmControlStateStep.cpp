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
// Contains implementation of CTestAlarmControlStateStep class
// 
//

/**
 @file
 @internalTechnology 
*/

// User Include
#include "TestAlarmControlStateStep.h"
#include "DummyAlarmControl.h"

/**
Constructor. Sets the test step name. Testserver reference passed to make use 
of TEF's method of sharing data between test steps
@internalTechnology
@test
*/
CTestAlarmControlStateStep::CTestAlarmControlStateStep(CTestMultipleAlarmsServer& aTestServer) 
: CTestBaseStep(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestAlarmControlStateStep);
	} 

/**
Base class pure virtual.
@return		EPass or EFail indicating the result of the test step.
@internalTechnology
@test
*/
TVerdict CTestAlarmControlStateStep::doTestStepL()
	{
	TPtrC alarmMessage;
	TPtrC expectedCalls;
	if(!ReadStringsFromConfig(ConfigSection(), &KIniAlarmMessage(), &alarmMessage, NULL))
		{
		IniProblem();
		}
	else
		{
		TRAPD(error, TestAlarmControlStateL(alarmMessage));
		PrintIfError(error);
		}
	return TestStepResult();	
	}

/**
Tests the state of the alarm control object associated with an alarm whose message 
is the one that is passed as parameter. Also performs other checks such as if the
no. of currently notifying alarms <= max alarms value etc.
@param aAlarmMessage Message to identify the alarm control object
@internalTechnology
@test
*/
void CTestAlarmControlStateStep::TestAlarmControlStateL(const TPtrC& aAlarmMessage)
	{
	TPtrC temp;
	TBool isPlaying = EFalse;
	TBool isSnoozed = EFalse;
	TBool isShowing = EFalse;
	
	if(TestServer()->CurrentMaxAlarms() == 1)
		{// if maxalarms == 1, then only one alarm control object must have been created
		INFO_PRINTF1(_L("MaxAlarms = 1. Testing no. of alarm control objects created to be 1"));
		TEST(TestServer()->AlarmControlsManager()->GetAlarmControlListCount() == 1);
		}
	
	// In case of maxalarms value more than 1, the number of currently 
	// notifying alarms must never be more than the maxalarms value
	INFO_PRINTF1(_L("Testing if no. of currently notifying alarms <= maxalarms value"));
	TEST(TestServer()->NoOfCurrentlyNotifyingAlarms() <= TestServer()->CurrentMaxAlarms());
	
	// Read the expected flags from ini. If not provided, will be considedered
	// as EFalse by default
	if(GetStringFromConfig(ConfigSection(), KIniIsPlaying(), temp))
		{
		isPlaying = GetBool(temp);
		}
	if(GetStringFromConfig(ConfigSection(), KIniIsShowing(), temp))
		{
		isShowing = GetBool(temp);
		}
	if(GetStringFromConfig(ConfigSection(), KIniIsSnoozed(), temp))
		{
		isSnoozed = GetBool(temp);
		}
	
	// Get the owner of the alarm message provided in ini.
	CDummyAlarmControl* alarmControl = dynamic_cast<CDummyAlarmControl*>(TestServer()->AlarmControlsManager()->GetAlarmControlObjectL(aAlarmMessage));
	
	INFO_PRINTF1(_L("Testing IsSnoozed"));
	TEST(!alarmControl->IsSnoozed() == !isSnoozed);
	
	if(isSnoozed)
		{
		TInt minsToSnooze = 0;
		if(GetIntFromConfig(ConfigSection(), KIniMinsToSnooze(), minsToSnooze))
			{
			INFO_PRINTF1(_L("Testing Mins To Snooze"));
			TEST(alarmControl->MinsToSnooze() == minsToSnooze);
			}
		}
	
	INFO_PRINTF1(_L("Testing IsShowing"));
	TEST(!alarmControl->IsShowing() == !isShowing);
	
	INFO_PRINTF1(_L("Testing IsPlaying"));
	// Whoever is playing now, would have updated the test server's 
	// CurrentlyPlayingAlarmId with his id. ! operator inserted to keep 
	// TBool comparison safe.
	User::After(1000000);
	TEST(!isPlaying == !(alarmControl->AlarmObject().Id() == TestServer()->CurrentlyPlayingAlarmId()));
	}
	

