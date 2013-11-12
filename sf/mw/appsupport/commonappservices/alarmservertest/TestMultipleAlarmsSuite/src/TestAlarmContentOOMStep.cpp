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
// Contains implementation of CTestAlarmContentOOMStep class
// 
//

/**
 @file
 @internalTechnology 
*/

// User Include
#include "TestAlarmContentOOMStep.h"

// System Include
#include <calalarm.h>

/**
Constructor. Sets the test step name. Testserver reference passed to make use 
of TEF's method of sharing data between test steps
@internalTechnology
@test
*/
CTestAlarmContentOOMStep::CTestAlarmContentOOMStep(CTestMultipleAlarmsServer& aTestServer) 
: CTestAssociatedDataStep(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestAlarmContentOOMStep);
	}

/**
Base class pure virtual.
@return		EPass or EFail indicating the result of the test step.
@internalTechnology
@test
*/
TVerdict CTestAlarmContentOOMStep::doTestStepL()
	{
	TestOOML();
	return TestStepResult();
	}
	
/**
Tests accessing the calendar entry and retreiving the alarm content
in an OOM loop
@internalTechnology
@test
*/	
void CTestAlarmContentOOMStep::TestOOML()
	{
	TInt error = KErrNone;
	TInt failRate = 0;
	do
		{// The OOM loop
		++failRate;
		__UHEAP_SETFAIL(RHeap::EDeterministic,failRate);
		__UHEAP_MARK;
		
		// Do the tests
		TRAP(error, GetAllAlarmContentsL());
		
		__UHEAP_MARKEND;
		__UHEAP_RESET;
		
		if(error != KErrNone && error != KErrNoMemory)
			{
			INFO_PRINTF3(_L("Unexpected error occured in %S: %D"), &TestStepName(), error);
			SetTestStepResult(EFail);
			break;
			}
		}while(error != KErrNone);
	}
	
/**
Retreives all the alarm control objects, that the alert server is 
maintaining currently and does the test
@internalTechnology
@test
*/	
void CTestAlarmContentOOMStep::GetAllAlarmContentsL()
	{
	RPointerArray<MEikServAlarm> alarmControlList = TestServer()->AlarmControlsManager()->GetAlarmControlListL();
	for(TInt index = 0; index < alarmControlList.Count(); ++index)
		{
		CDummyAlarmControl* alarmControl = dynamic_cast<CDummyAlarmControl*>(alarmControlList[index]);
		DoGetAlarmContentL(alarmControl);
		}
	}
	
/**
Accesses the calendar entry associated with the alarm control object, and
retrieves its alarm content
@internalTechnology
@test
*/	
void CTestAlarmContentOOMStep::DoGetAlarmContentL(CDummyAlarmControl* aAlarmControl)
	{
	CCalEntryId* entryId = NULL;
	if(GetEntryIdL(aAlarmControl->AlarmMessage(), entryId))
		{
		CCalEntry* calEntry = NULL;
		CleanupStack::PushL(entryId);
		
		CCalSession* calSession = CreateAndInitializeCalSessionL(EFalse);
		CleanupStack::PushL(calSession);
		
		if(!GetCalEntryL(entryId, calEntry, calSession))
			{// An unexpected situation, that must not occur
			ERR_PRINTF2(_L("The calendar entry was not found for %S. Failing the test..."), &(aAlarmControl->AlarmMessage()));
			SetTestStepResult(EFail);
			}
		else
			{
			CleanupStack::PushL(calEntry);
			
			// Get the alarm content
			CCalAlarm* alarm = calEntry->AlarmL();
			
			// Do nothing with it. Clean up.
			CleanupStack::PopAndDestroy(calEntry);	
			delete alarm;
			}
		CleanupStack::PopAndDestroy(2, entryId); // calSession, entryId
		}
		
	}
