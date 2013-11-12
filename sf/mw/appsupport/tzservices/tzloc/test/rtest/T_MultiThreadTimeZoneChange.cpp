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

#include <e32base.h>
#include <e32std.h>
#include <e32des16.h>
#include <e32test.h>
#include <tzlocalizer.h> 


_LIT(KFormatErrorCode, " the error code is %d \n");


_LIT(KThread1,"thread1");
_LIT(KThread2,"thread2");
_LIT(KThread3,"thread3");
_LIT(KThread4,"thread4");
_LIT(KThread5,"thread5");
_LIT(KThread6,"thread6");
_LIT(KThread7,"thread7");
_LIT(KThread8,"thread8");
_LIT(KThread9,"thread9");
_LIT(KThread10,"thread10");

_LIT(KTxtEPOC32EX,"EPOC32EX");


const TUint32 KLondon = 2592;
const TInt KHeapSize = 0x2000;


RTest test (_L("T_MultiThreadTimeZoneChange"));


LOCAL_C void TestConvertByUsingCTzConverterL()
	{
	//create a CTzLocalizer object for setting the timezone
	CTzLocalizer* localizer = CTzLocalizer::NewLC();  
	
	//Set timezone to be London
	localizer->SetTimeZoneL(KLondon);
	
	CleanupStack::PopAndDestroy(localizer);
	}


/**
 * Start converting function.
 */
LOCAL_C void StartConvertL()
    {
	CActiveScheduler* scheduler = new(ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);
	
    TestConvertByUsingCTzConverterL();
    
	CleanupStack::PopAndDestroy(scheduler);
	}

/**
 * Run single thread. 
 */
LOCAL_C TInt RunThread()
    {
	__UHEAP_MARK;
    CTrapCleanup* cleanup = CTrapCleanup::New();    // get clean-up stack

    TRAPD(error, StartConvertL());    // more initialization
    __ASSERT_ALWAYS( !error, User::Panic(KTxtEPOC32EX, error));
	
	delete cleanup;             // destroy clean-up stack
    __UHEAP_MARKEND;
    
	return error;
    }	

/**
 * Thread function entry  point 
 */
LOCAL_C TInt ThreadFunc(TAny* /*aAny*/)  
    {
	return RunThread();
	}

/**
 * Thread function entry  point 
 */
LOCAL_C void PrintStatus(TRequestStatus& aThreadResult, TInt aThreadNumber)
	{
	if(aThreadResult.Int() != KErrNone)
		{
		test.Printf(KFormatErrorCode, aThreadResult.Int());
		}
	else
		{
		test.Printf(_L("Thread %d was successfull\n"), aThreadNumber);
		}	
	}

/**
 * Run Multi threads for testing the converting performance
 * 
 */
LOCAL_C void RunMultiThreadsL()
    {
	RThread thread1;
	RThread thread2;
	RThread thread3;	
	RThread thread4;
	RThread thread5;
	RThread thread6;
	RThread thread7;
	RThread thread8;	
	RThread thread9;
	RThread thread10;
	
	TInt tResult = KErrNone;
	
	tResult = thread1.Create(KThread1, ThreadFunc, KDefaultStackSize, KHeapSize, KHeapSize, NULL);
	User::LeaveIfError(tResult);
	
	tResult = thread2.Create(KThread2, ThreadFunc, KDefaultStackSize, KHeapSize, KHeapSize, NULL);
	User::LeaveIfError(tResult);
	
	tResult = thread3.Create(KThread3, ThreadFunc, KDefaultStackSize, KHeapSize, KHeapSize, NULL);
	User::LeaveIfError(tResult);
	
	tResult = thread4.Create(KThread4, ThreadFunc, KDefaultStackSize, KHeapSize, KHeapSize, NULL);
	User::LeaveIfError(tResult);
	
	tResult = thread5.Create(KThread5, ThreadFunc, KDefaultStackSize, KHeapSize, KHeapSize, NULL);
	User::LeaveIfError(tResult);
	
	tResult = thread6.Create(KThread6, ThreadFunc, KDefaultStackSize, KHeapSize, KHeapSize, NULL);
	User::LeaveIfError(tResult);
	
	tResult = thread7.Create(KThread7, ThreadFunc, KDefaultStackSize, KHeapSize, KHeapSize, NULL);
	User::LeaveIfError(tResult);
	
	tResult = thread8.Create(KThread8, ThreadFunc, KDefaultStackSize, KHeapSize, KHeapSize, NULL);
	User::LeaveIfError(tResult);
	
	tResult = thread9.Create(KThread9, ThreadFunc, KDefaultStackSize, KHeapSize, KHeapSize, NULL);
	User::LeaveIfError(tResult);
	
	tResult = thread10.Create(KThread10, ThreadFunc, KDefaultStackSize, KHeapSize, KHeapSize, NULL);
	User::LeaveIfError(tResult);
	
	// Request notification when these threads die using Logon
	// function.
	TRequestStatus status1;
	TRequestStatus status2;
	TRequestStatus status3;
	TRequestStatus status4;
	TRequestStatus status5;
	TRequestStatus status6;
	TRequestStatus status7;
	TRequestStatus status8; 
	TRequestStatus status9;
	TRequestStatus status10;
	
	thread1.Logon(status1);
	thread2.Logon(status2);
	thread3.Logon(status3);
	thread4.Logon(status4);
	thread5.Logon(status5);
	thread6.Logon(status6);
	thread7.Logon(status7);
	thread8.Logon(status8);
	thread9.Logon(status9);
	thread10.Logon(status10);
		
	CTzLocalizer* localizer = CTzLocalizer::NewLC();
	//set timezone to be London
	localizer->SetTimeZoneL(KLondon);		
	CleanupStack::PopAndDestroy(localizer);
	
	// Make all threads eligible for execution using Resume
	// function.
	thread1.Resume();
	thread2.Resume();
	thread3.Resume();
	thread4.Resume();
	thread5.Resume();
	thread6.Resume();
	thread7.Resume();
	thread8.Resume();
	thread9.Resume();
	thread10.Resume();

	// Wait for the above requests to complete. Use 
	// WaitForRequest of the User class. 
	
	User::WaitForRequest(status1);
	PrintStatus(status1, 1);
	
	User::WaitForRequest(status2);
	PrintStatus(status2, 2);
	
	User::WaitForRequest(status3);
	PrintStatus(status3, 3);
	
	User::WaitForRequest(status4);
	PrintStatus(status4, 4);

	User::WaitForRequest(status5);
	PrintStatus(status5, 5);
		
	User::WaitForRequest(status6);
	PrintStatus(status6, 6);
	
	User::WaitForRequest(status7);
	PrintStatus(status7, 7);
	
	User::WaitForRequest(status8);
	PrintStatus(status8, 8);
	
	User::WaitForRequest(status9);
	PrintStatus(status9, 9);

	User::WaitForRequest(status10);
	PrintStatus(status10, 10);

	
	// End threads. Use Kill function.
	thread1.Kill(KErrNone);
	thread2.Kill(KErrNone);
	thread3.Kill(KErrNone);
	thread4.Kill(KErrNone);
	thread5.Kill(KErrNone);
	thread6.Kill(KErrNone);
	thread7.Kill(KErrNone);
	thread8.Kill(KErrNone);
	thread9.Kill(KErrNone);
	thread10.Kill(KErrNone);
	
	// Close the handle to threads
	thread1.Close();
	thread2.Close();
	thread3.Close();
	thread4.Close();
	thread5.Close();
	thread6.Close();
	thread7.Close();
	thread8.Close();
	thread9.Close();
	thread10.Close();	
	}


LOCAL_C void DoStartL()
	{
	// Create active scheduler (to run active objects)
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	RunMultiThreadsL();

	// Delete active scheduler
	CleanupStack::PopAndDestroy(scheduler);
	}

//  Global Functions

/**
@SYMTestCaseID PIM-TMULTITHREADTIMEZONECHANGE-0001
*/	
GLDEF_C TInt E32Main()
	{
	// Create cleanup stack
	__UHEAP_MARK;
	test.Start(_L("@SYMTestCaseID PIM-TMULTITHREADTIMEZONECHANGE-0001 Starting Test T_MultiThreadTimeZoneChange"));
	CTrapCleanup* cleanup = CTrapCleanup::New();

	// Run application code inside TRAP harness.
	TRAPD(mainError, DoStartL());
	
	test(mainError == KErrNone);
	
	delete cleanup;
	test.End();
	test.Close();
	__UHEAP_MARKEND;
	return KErrNone;
	}



