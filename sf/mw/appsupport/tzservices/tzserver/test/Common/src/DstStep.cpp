// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#include "DstStep.h"

// Type definitions
#define UNUSED_VAR(a) a = a
 
const TInt KErrTestFileHandle = -7400;

_LIT(KRunTestCount, "MaxRunLimit");
_LIT(KLeakTestCount, "MaxOOMLimit");
_LIT(KFirstLeakIter, "MinOOMLimit");
_LIT(KFileTestCount, "MaxOOFLimit");
_LIT(KCheckHandles, "CheckHandles");

_LIT(KNormalTestStarted, "Normal Test Started");
_LIT(KNormalTestComplete, "Normal Test Completed; Attempts: %d");
_LIT(KOOMTestStarted, "Out-of-Memory test started");
_LIT(KOOMTestTerminated, "Out-of-Memory test ended; Allocations made: %d");
_LIT(KOOFTestStarted, "Out-of-File-Handle Test Started");
_LIT(KOOFTestTerminated, "Out-of-File-Handle test ended; Handles used: %d");
_LIT(KOOFTestAborted, "Out-of-File-Handle Test Aborted; No File Session supplied.");


//
// CDSTActiveWorker
//
CDSTActiveWorker::~CDSTActiveWorker()
	{
	Deque();
	}

CDSTActiveWorker::CDSTActiveWorker(const CDstStep& aTestStep) : 
	CActive(CActive::EPriorityStandard), iTestStep(const_cast<CDstStep&>(aTestStep))
	{}

void CDSTActiveWorker::Start()
	{
	CompleteSelf();
	}


void CDSTActiveWorker::CompleteSelf()
	{
	SetActive();
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status,KErrNone);
	}

CDSTActiveWorker* CDSTActiveWorker::NewL(const CDstStep& aTestStep)
	{
	CDSTActiveWorker* self = new(ELeave) CDSTActiveWorker(aTestStep);
	CActiveScheduler::Add(self);
	self->CompleteSelf();
	return self;
	}

void CDSTActiveWorker::RunL()
	{
	iTestStep.RunTest();
	}

void CDSTActiveWorker::DoCancel()
	{
	// do nothing
	}

TInt CDSTActiveWorker::RunError(TInt aError)
	{
	iTestStep.TestCompleted(aError);
	return KErrNone;
	}

//
// CDstStep
//
CDstStep::CDstStep() : 
	iState(ERunningNormalTest),
	iTestCmd(ECreateUnit),
	iMaxRunTestCount (1),				
	iMaxLeakTestCount (0),
	iFirstLeakIter (0),
	iMaxFileTestCount (0),
	iCheckHandles(EFalse)
	{
	}

CDstStep::~CDstStep()
	{
	delete iActiveWorker;
	delete iScheduler;
	}

void CDstStep::PrepareTestEnvironmentL()
	{
	// do nothing
	// sub-classes derive from this to load any script settings they require
	}


TVerdict CDstStep::doTestStepPreambleL()
	{
	iScheduler = new CActiveScheduler();
	CActiveScheduler::Install(iScheduler);

	GetIntFromConfig(ConfigSection(), KRunTestCount, iMaxRunTestCount);
	GetIntFromConfig(ConfigSection(), KLeakTestCount, iMaxLeakTestCount);
	GetIntFromConfig(ConfigSection(), KFirstLeakIter, iFirstLeakIter);
	GetIntFromConfig(ConfigSection(), KFileTestCount, iMaxFileTestCount);
	GetBoolFromConfig(ConfigSection(), KCheckHandles, iCheckHandles);

	// there must be at least a single normal run;
	if (iMaxRunTestCount <= 0) iMaxRunTestCount = 1;
	INFO_PRINTF1(KNormalTestStarted);

	PrepareTestEnvironmentL();

	SetTestStepResult(EPass);
	return TestStepResult();
	}

TVerdict CDstStep::doTestStepL()
	{
	if(TestStepResult() == EPass)
		{
		// the activeWorker is responsible for starting the actual test
		// from its RunL
		iActiveWorker = CDSTActiveWorker::NewL(*this);
		CActiveScheduler::Start();

		delete iActiveWorker;
		iActiveWorker = NULL;
		}

	return TestStepResult();
	}


TVerdict CDstStep::doTestStepPostambleL()
	{
	delete iScheduler;
	iScheduler=NULL;
	CActiveScheduler::Install(NULL);

	return TestStepResult();
	}


void CDstStep::SetFileSession(RFs& aFs)
	{
	if (ERunningFileTest == iState)
		{
		iFs = &aFs;

		// set handles to allow
		iFs->SetErrorCondition(KErrTestFileHandle, iTestCount);
		}
	}

 
void CDstStep::TestCompleted(TInt aCondition)
	{

	iTestCmd = EDestroyUnit;

	iTestCondition = aCondition;
	iFs = NULL;

	// Puts the test clean-up into our own active object RunL.
	// we don't want to delete the test unit in a call from its own RunL or
	// that of its delegates
	iActiveWorker->Start();

	}


TInt CDstStep::FinishTest()
	{

	TRAPD(error, DestroyTestUnit());
	UNUSED_VAR(error); //Used to supress build warnings

	++iTestCount;

	switch (iState)
		{
	case ERunningNormalTest:
		if (iTestCount == iMaxRunTestCount)
			{
			TInt endPHandleCount;
			TInt endTHandleCount;
			RThread().HandleCount(endPHandleCount, endTHandleCount);

			_LIT(KThreadHandleLeaks, "This test leaked %d thread handles");
			_LIT(KThreadHandlesLeakCheck, "Thread Handle Leak Check");

			// we cannot test process handles because other threads may be
			// running in the server

			if (endTHandleCount != iStartTHandleCount)
				{
				// TODO: check if EFF_PRINTF2 is better
				INFO_PRINTF2(KThreadHandleLeaks, endTHandleCount - iStartTHandleCount);
				}

			if (iCheckHandles)
				{
				LogCheck(KThreadHandlesLeakCheck(), 
					(endTHandleCount == iStartTHandleCount) ? KErrNone : KErrBadHandle);
				}

			INFO_PRINTF2(KNormalTestComplete, iTestCount);

			iTestCount = 0;
			if (iMaxLeakTestCount)
				{
				iState = ERunningLeakTest;
				INFO_PRINTF1(KOOMTestStarted);
				}
			else if (iMaxFileTestCount)
				{
				iState = ERunningFileTest;
				INFO_PRINTF1(KOOFTestStarted);
				}
			else
				{
				iState = EIdle;
				}
			}
		break;

	case ERunningLeakTest:
		__UHEAP_MARKEND;
		User::Heap().Check();
		__UHEAP_RESET;

		if ((iTestCondition != KErrNoMemory) || (iTestCount == iMaxLeakTestCount ))
			{

			INFO_PRINTF2(KOOMTestTerminated, iTestCount);
			iTestCount = 0;
			// TODO: save memory leak test result

			// out of memory test completed
			if (iMaxFileTestCount)
				{
				iState = ERunningFileTest;
				INFO_PRINTF1(KOOFTestStarted);
				}
			else
				{
				iState = EIdle;
				}
			}

		break;

	case ERunningFileTest:
		if (!iFs)
			{
			// no file session object was provided, stop the file test
			INFO_PRINTF1(KOOFTestAborted);
			iTestCount = 0;
			// TODO: save file handle leak test result

			iState = EIdle;

			// _LIT(KFileTestTerm, "Out Of File Handles test terminated: No file session provided");
			// Utils().LogIt(KFileTestTerm);
			}
		else
			{
			// clean-up file session handles restriction
			iFs->SetErrorCondition(KErrNone);

			if ((iTestCondition != KErrTestFileHandle) || (iTestCount == iMaxFileTestCount))
				{
				// out of file handle test completed
				INFO_PRINTF2(KOOFTestTerminated, iTestCount);
				iTestCount = 0;

				// TODO: save file handle leak test result
				iState = EIdle;

				}
			}
		break;

	default:
		break;
		}

	// prepare for the next test state
	iTestCmd = ECreateUnit;

	iActiveWorker->Start();
	return iTestCondition;
	}


TInt CDstStep::StartTest()
	{
	RThread().HandleCount(iStartPHandleCount, iStartTHandleCount);
	SetTestStepResult(EFail);
	TInt results = KErrNone;
	switch (iState)
		{
		case ERunningLeakTest:
			__UHEAP_MARK;
			__UHEAP_FAILNEXT(iTestCount);
		case ERunningFileTest:
		case ERunningNormalTest:
			TRAPD(error, results = CreateAndRunTestUnitL());
			if ((results != KRequestPending) || (error != KErrNone))
				{
				// TODO: if there is a leave; log a message to that effect
				// Test is Synchronous, or leave condition
				if (results == KErrNone)
					{
					results = error;
					}
				TestCompleted(results);
				}
			break;

		default:
			// stop the scheduler
			// All test states covered

			SetTestStepResult(((iTestCondition == KErrNone) && (iChecksFailed == 0)) ? EPass : EFail);
			CActiveScheduler::Stop();
			break;
		}

	return (results);
	}


TInt CDstStep::RunTest()
	{
	TInt results = KErrNone;

	switch (iTestCmd)
		{
		case ECreateUnit:
			results = StartTest();
			break;

		case EDestroyUnit:
			results = FinishTest();
			break;

		default:
			// stop the scheduler
			// something bad has happened
			// TODO: Panic
			SetTestStepResult(EFail);
			CActiveScheduler::Stop();
			break;
		}

	return (results);
	}


void CDstStep::LogCheck(const TDesC& aDescription, TInt aCondition)
	{

	// set the result for the temporary benefit of any dependent test step
	// it will be overwritten when the test completes
	if (aCondition == KErrNone)
		{
		SetTestStepResult(EPass);
		++iChecksPassed;
		}
	else
		{
		SetTestStepResult(EFail);
		++iChecksFailed;
		if (iState == ERunningNormalTest)
			{
			// only do the logging in normal test mode.
			// we don't want to use any resources that interferes with stress testing
			ERR_PRINTF3(_L("Check: %S: Error Code: %d"), &aDescription, aCondition);
			}
		}
	}


TTime CDstStep::ReadTimeParamStringL(const TDesC& aParamString)
	{
	// Format of buffer to construct a TTime is YYYYMMDD:HHMMSS (15 characters).
	// TTime uses zero-based values for month and day - which is confusing for scripting.
	// In our script, we use actual month and day numbers to make things simpler so we
	// must modify the string here to take account of this.
	TBuf<32> buf;
	TInt m, d;

	buf.Zero();
	buf.Copy(aParamString.Left(4));		// The year
	TLex lexMonth = aParamString.Mid(4, 2);
	lexMonth.Val(m);
	TLex lexDay = aParamString.Mid(6, 2);
	lexDay.Val(d);
	buf.AppendFormat(_L("%02d%02d"), m - 1, d - 1);	// The month and day
	buf.Append(aParamString.Right(7));

	return TTime(buf);
	}
