// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32test.h>
#include <tz.h>


_LIT(KTestName,"Bootup Performance Test");
RTest test(KTestName);
//This is the base line of the boot-up time (ms) of the time zone server. 
//The value should be set the first time of running this test.
//And then, the test sould fail if the actural boot-up is %10 bigger.
TInt64 bootTimeBaseLine = 911875;

//Define the following to get the base line of the boot-up time of the time zone server. 

#define GetPerformanceBaseline

class CTzBootPerformanceTest : public CBase
	{
public:
    static CTzBootPerformanceTest* NewLC();
	~CTzBootPerformanceTest();
	
	// Time the performance when the time zone server is connected.
	void TestBootupPerformanceL();

private:
	CTzBootPerformanceTest();
	};



CTzBootPerformanceTest* CTzBootPerformanceTest::NewLC()
	{
	CTzBootPerformanceTest* self = new (ELeave) CTzBootPerformanceTest();
	CleanupStack::PushL(self);
	return (self);
	}


CTzBootPerformanceTest::CTzBootPerformanceTest()
	{
	}	


CTzBootPerformanceTest::~CTzBootPerformanceTest()	
	{
	}

void CTzBootPerformanceTest::TestBootupPerformanceL()
	{
	TTime startTime;
	TTime endTime;
	startTime.UniversalTime();

	RTz tz;
	User::LeaveIfError(tz.Connect());
	endTime.UniversalTime();
	tz.Close();
	
	TTimeIntervalMicroSeconds micros = endTime.MicroSecondsFrom(startTime);
	_LIT(KBootTime, "Time to connect time zone server = %d (ms) \n");
	test.Printf(KBootTime,micros);
	
#ifndef __WINS__
	
#ifdef GetPerformanceBaseline
	bootTimeBaseLine = micros.Int64();
	_LIT(KBootTimeBaseLine, "The baseline of boot-up time = %d (ms) \n");
	test.Printf(KBootTimeBaseLine,bootTimeBaseLine);
#else
	test((micros.Int64()-bootTimeBaseLine)/bootTimeBaseLine<0.1);
#endif
	
#endif
	}
		
/**
@SYMTestCaseID PIM-TBOOTPERFORMANCE-0001
*/	
static void DoTestsL()
	{
		
	test.Title();

	CTzBootPerformanceTest* bootupPerformanceTest = CTzBootPerformanceTest::NewLC();

	test.Start(_L("@SYMTestCaseID PIM-TBOOTPERFORMANCE-0001 Test the Performance when the server is connected"));
	bootupPerformanceTest->TestBootupPerformanceL();
	CleanupStack::PopAndDestroy(bootupPerformanceTest);
	}


TInt E32Main()
    {
	__UHEAP_MARK;

	CTrapCleanup* trapCleanup = CTrapCleanup::New();
	if(!trapCleanup)
		{
		return KErrNoMemory;
		}

	CActiveScheduler* scheduler = new CActiveScheduler;
	if(!scheduler)
		{
		return KErrNoMemory;
		}
	CActiveScheduler::Install(scheduler);	
	
	TRAPD(ret, DoTestsL());
	test.Printf(_L("Trapped return value from DoTestsL(): %d\n"), ret);
	test(ret == KErrNone);

	test.Close();
		
	delete scheduler;
	delete trapCleanup;	

	__UHEAP_MARKEND;

	return (KErrNone);
    }
