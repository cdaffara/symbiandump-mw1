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
// Contains implementation of CTestPerformanceStep class
// 
//

/**
 @file
 @internalTechnology 
*/

// User Include
#include "TestPerformanceStep.h"

// System Include
#include <calalarm.h>

// Initializes to current time
#define NOW(time) TTime time; time.HomeTime();

/**
Constructor. Sets the test step name. Testserver reference passed to make use 
of TEF's method of sharing data between test steps
@internalTechnology
@test
*/
CTestPerformanceStep::CTestPerformanceStep(CTestMultipleAlarmsServer& aTestServer) 
: CTestAssociatedDataStep(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestPerformanceStep);
	}

/**
Base class pure virtual.
@return		EPass or EFail indicating the result of the test step.
@internalTechnology
@test
*/
TVerdict CTestPerformanceStep::doTestStepL()
	{
	TPtrC dataReadTime;
	TPtrC notificationTime;
	if(!ReadStringsFromConfig(ConfigSection(), &KIniDataReadTime(), &dataReadTime, 
										       &KIniNotificationTime(), &notificationTime, NULL))
		{
		IniProblem();
		}
	else
		{
		TRAPD(error, TestPerformanceL(GetTReal32(dataReadTime), GetTReal32(notificationTime)));
		PrintIfError(error);
		}	
	return TestStepResult();	
	}

/**
For all the alarm control object which the alarm controls manager holds,
tests the associated data retreival performance, and the alert-server
notification performance
@param aDataReadTime Maximum time allowed to read the associated data
@param aNotificationTime Maximum time allowed for alarm control object to 
get notified of an alarm after it expires
@internalTechnology
@test
*/	
void CTestPerformanceStep::TestPerformanceL(const TReal32& aDataReadTime, const TReal32& aNotificationTime)
	{
	RPointerArray<MEikServAlarm> alarmControlList = TestServer()->AlarmControlsManager()->GetAlarmControlListL();
	for(TInt index = 0; index < alarmControlList.Count(); ++index)
		{
		CDummyAlarmControl* alarmControl = dynamic_cast<CDummyAlarmControl*>(alarmControlList[index]);
		
		INFO_PRINTF2(_L("Testing Associated Data Performance for %S"), &(alarmControl->AlarmMessage()));
		TestAssociatedDataPerformanceL(alarmControl,aDataReadTime);
		
		INFO_PRINTF2(_L("Testing Notification Time Performance for %S"), &(alarmControl->AlarmMessage()));
		TestAlarmNotificationPerformanceL(alarmControl, aNotificationTime);
		}
	}

/**
Tests the time difference in original expiry time of alarm and the actual 
notification time.
@param alarmControl The alarm control object
@param aNotificationTime Maximum time allowed for alarm control object to 
get notified of an alarm after it expires
@internalTechnology
@test
*/	
void CTestPerformanceStep::TestAlarmNotificationPerformanceL(CDummyAlarmControl* aAlarmControl, const TReal32& aNotificationTime)
	{
	CheckTimeDifference(aAlarmControl->OriginalExpiryTime(), aAlarmControl->ActualNotificationTime(), aNotificationTime);
	}

/**
Tests the time taken for reading associated data of an alarm
@param alarmControl The alarm control object
@param aDataReadTime Maximum time allowed to read the associated data
@internalTechnology
@test
*/	
void CTestPerformanceStep::TestAssociatedDataPerformanceL(CDummyAlarmControl* aAlarmControl, const TReal32& aDataReadTime)
	{
	CCalEntryId* entryId = NULL;
	if(!GetEntryIdL(aAlarmControl->AlarmMessage(), entryId))
		{
		ERR_PRINTF2(_L("No associated data attached with the alarm %S"), &(aAlarmControl->AlarmMessage()));
		}
	else
		{
		CCalEntry* calEntry = NULL;
		CleanupStack::PushL(entryId);
		
		CCalSession* calSession = CreateAndInitializeCalSessionL(EFalse);
		CleanupStack::PushL(calSession);
		
		if(!GetCalEntryL(entryId, calEntry, calSession))
			{
			ERR_PRINTF2(_L("The calendar entry was not found for %S. Failing the test..."), &(aAlarmControl->AlarmMessage()));
			SetTestStepResult(EFail);
			}
		else
			{
			CleanupStack::PushL(calEntry);
			
			// Time the operation and perform the operation
			NOW(startTime)
			CCalAlarm* alarm = calEntry->AlarmL();
			NOW(endTime)
			
			delete alarm; 
			
			// Check
			CheckTimeDifference(startTime, endTime, aDataReadTime);
				
			CleanupStack::PopAndDestroy(calEntry);	
			}
		CleanupStack::PopAndDestroy(2, entryId); // calSession and entryId
		}
		
	}

