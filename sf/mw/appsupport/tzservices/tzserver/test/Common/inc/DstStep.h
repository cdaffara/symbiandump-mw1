/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/



#if (!defined __DSTSTEP_H__)
#define __DSTSTEP_H__

#include <test/testexecutestepbase.h>
#include "DstServer.h"
#include <tz.h>

class CDstStep;
/////////////////////////////////////////////////////////////////////////
// CDSTActiveWorker
//
class CDSTActiveWorker : public CActive
	{
public:
	static CDSTActiveWorker* NewL(const CDstStep& aTestStep);
	~CDSTActiveWorker();
	void Start();

private:
	CDSTActiveWorker(const CDstStep& aTestStep);
	void CompleteSelf();

	// from CActive
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);

private:
	CDstStep& iTestStep;
	};

/////////////////////////////////////////////////////////////////////////
// CDstStep
//
class CDstStep : public CTestStep
	{
public:
	friend class CDSTActiveWorker;
	CDstStep();
	~CDstStep();

	// Called by Asynchronous test steps when they complete their
	// test
	void TestCompleted(TInt aCondition);
	
	// Called by test step sub-class, 
	// immediately after connecting to the session and before
	// any other Component Under Test is created, 
	// to register the file session for stress testing.
	void SetFileSession(RFs& aFs);

	// Logs the description if error Condition
	// the whole test step is considered failed if any logged check is a failure
	void LogCheck(const TDesC& aDescription, TInt aCondition);

	// Reads a TTime-like format string but allows the use of proper month and day values
	static TTime ReadTimeParamStringL(const TDesC& aParamString);
	
protected:
	// Template Method Called by doTestStepL thru StartTest()
	virtual TInt CreateAndRunTestUnitL() = 0;

	// Template Method called by TestCompleted() thru FinishTest()
	virtual void DestroyTestUnit() = 0;

	// Template Method called by doTestStepPreambleL()
	virtual void PrepareTestEnvironmentL();

private:

	enum TState 
		{
		EIdle=0,
		ERunningLeakTest,
		ERunningFileTest,
		ERunningNormalTest,
		};

	enum TTestCmd 
		{
		ECreateUnit=0,
		EDestroyUnit
		};


	// Called by ActiveWorker to start the test
	TInt RunTest();

	// Tidies up after each run of a test. The test may be run several times
	// before the test step returns to TestExecute
	TInt FinishTest();


	// Kicks-off each run of a test. see FinishTest()
	TInt StartTest();

	// from CTestStep 
	TVerdict doTestStepL(); 			// Pure virtual
	TVerdict doTestStepPostambleL();	// virtual
	TVerdict doTestStepPreambleL();

protected:
	RTz iTimeZoneServer;
	RFs iFileServ;

private:
	TState iState;
	TTestCmd iTestCmd;
	TInt iTestCondition;
	TInt iMaxRunTestCount;				
	TInt iTestCount;
	TInt iMaxLeakTestCount;
	TInt iFirstLeakIter;
	TInt iMaxFileTestCount;
	TInt iStartPHandleCount;
	TInt iStartTHandleCount;
	TBool iCheckHandles;

	TInt iChecksPassed;	// accummulates the number of checks passed 
	TInt iChecksFailed;	// accummulates the number of checks failed

	CActiveScheduler* iScheduler;
	CDSTActiveWorker* iActiveWorker;
	RFs* iFs;							// not owned
	};

#endif


