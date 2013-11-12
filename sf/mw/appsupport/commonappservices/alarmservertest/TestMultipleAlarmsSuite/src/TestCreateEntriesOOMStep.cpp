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
// Contains implementation of CTestCreateEntriesOOMStep class
// 
//

/**
 @file
 @internalTechnology 
*/

// User Include
#include "TestCreateEntriesOOMStep.h"

/**
Constructor. Sets the test step name. Testserver reference passed to make use 
of TEF's method of sharing data between test steps
@internalTechnology
@test
*/
CTestCreateEntriesOOMStep::CTestCreateEntriesOOMStep(CTestMultipleAlarmsServer& aTestServer) 
: CTestCreateCalEntriesStep(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestCreateEntriesOOMStep);
	}

/**
Base class pure virtual.
@return		EPass or EFail indicating the result of the test step.
@internalTechnology
@test
*/
TVerdict CTestCreateEntriesOOMStep::doTestStepL()
	{
	CActiveScheduler* sched = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	TestOOML();
	delete sched;
	return TestStepResult();
	}

/**
Tests creation of calendar entries in an OOM loop
@internalTechnology
@test
*/	
void CTestCreateEntriesOOMStep::TestOOML()
	{
	TInt error = KErrNone;
	TInt failRate = 0;
	
	do
		{// The OOM loop
		++failRate;
		__UHEAP_SETFAIL(RHeap::EDeterministic, failRate);
		__UHEAP_MARK;
	
		// Do the tests
		TRAP(error, CreateCalEntriesL());
		
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
