// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "testserver.h"
#include "TestServerTest.h"

#include <f32file.h>
#include <s32file.h>
#include <e32base.h>
#include <e32test.h>

_LIT(KTest,"TestServer test");
_LIT(KDateString,"%D%M%Y%/0%1%/1%2%/2%3%/3  %-B%:0%J%:1%T%:2%S%.%*C4%:3%+B");
_LIT(KTempTime, "19940225:151032.0");
_LIT(KOutputMsg, "Setting Home time to");
_LIT(KFileName, "C:\\temp.txt");
_LIT(KFileName2, "C:\\temp2.txt");
	
static RTest test(KTest);

// ----------------------------------
// CTestServerTest Class implementation
// ==================================
CTestServerTest::~CTestServerTest()
	{
	iServ.Close();
	iFs.Close();
	}


CTestServerTest* CTestServerTest::NewL()
	{
	CTestServerTest* self = CTestServerTest::NewLC();
	CleanupStack::Pop();
	return self;
	}	


CTestServerTest* CTestServerTest::NewLC()
	{
	CTestServerTest* self = new(ELeave) CTestServerTest;
	CleanupStack::PushL(self);
	self->ConstructL();
	
	return self;
	}
	
	
void CTestServerTest::ConstructL()
	{
	User::LeaveIfError(iServ.Connect());
	User::LeaveIfError(iFs.Connect());	
	}

void CTestServerTest::PrintTTimeL(const TDesC& aMessage, const TTime& aTime) const
	{	
	TBuf<255> str;
	aTime.FormatL(str, KDateString);
	test.Printf(_L("%S: %S\n"), &aMessage, &str);
	}


void CTestServerTest::TestSetHomeTimeL()
	{
	test.Printf(_L("TestSetHomeTime()\n"));
	TTime ttime, 
		  settime;
	  
	ttime.Set(KTempTime);
	PrintTTimeL(KOutputMsg, ttime);

	iServ.SetHomeTime(ttime);
	settime.HomeTime();
	PrintTTimeL(KOutputMsg, settime);

	test(settime >= ttime);
	}

void CTestServerTest::TestFileSizeL()
	{
	test.Printf(_L("TestFileSize()\n"));
	TInt size = 999;
	TRAP(iErr, size = iServ.GetFileSizeL(KFileName));
	TestErrorL();
	if (iErr == KErrNone)
		{
		test(size == 0);
		}
	}

void CTestServerTest::TestDeleteFileL(const TDesC& aFileName)
	{
	test.Printf(_L("TestDeleteFile()\n"));
	TRAP(iErr, iServ.DeleteFileL(aFileName));
	TestErrorL();
	}

void CTestServerTest::TestKillProcess()
	{
	// KillProcessL is tested to fail at the moment as no process has been created.
	// Please update this function when new process is created in the future.
	test.Printf(_L("TestKillProcess()\n"));
	TRAP(iErr, iServ.KillProcessL(KFileName2));
	test(iErr == KErrNotFound);
	}
	
void CTestServerTest::TestChangeLocaleNameL()
	{
	test.Printf(_L("TestChangeLocaleName()\n"));
	TBuf<50> libraryName;
#ifdef SYMBIAN_DISTINCT_LOCALE_MODEL
	libraryName.Format(_L("elocl_lan.%03d"),ELangAmerican);
#else
	libraryName.Format(_L("ELOCL.%02d"),ELangAmerican);
#endif
	TRAP(iErr, iServ.ChangeLocaleNameL(libraryName));
	TestErrorL();	
	}

void CTestServerTest::CheckForFileL(const TDesC& aFileName, TBool aFilePresent)
	{
	test.Printf(_L("CheckForFile()\n"));
	iErr = iServ.CheckForFile(aFileName);
	if(aFilePresent)
		{
		TestErrorL();
		}
	else
		{
		test(iErr == KErrNotFound);
		}
	}

void CTestServerTest::CopyFileL()
	{
	test.Printf(_L("CopyFile()\n"));
	TRAP(iErr, iServ.CopyFileL(KFileName, KFileName2));
	TestErrorL();	
	}
	
void CTestServerTest::TestErrorL()const
	{
	if (iErr != KErrNone)
		{
		test.Printf(_L("Error No: %d \n"), iErr);
		User::LeaveIfError(iErr);
		}
	}

void CTestServerTest::TestTimeZoneL()
	{
	test.Printf(_L("TestTimeZone()\n"));
	TBuf8<256> buffer;
	TRAP(iErr, iServ.GetTimeZoneL(buffer));
	TestErrorL();
	
	TRAP(iErr, iServ.SetTimeZoneL(buffer));
	TestErrorL();	
	}

void CTestServerTest::CreateTestFileL(const TDesC& aFileName)
	{
	RFile file;
	iServ.CreateEmptyFileL(aFileName);											     
	file.Close();
	}

void CTestServerTest::DeleteTestFileL(const TDesC& aFileName)
	{
	iServ.DeleteFileL(aFileName);
	}

void CTestServerTest::TestNotifyChangeL()
	{
	test.Printf(_L("TestNotifyChangeL()\n"));
	
	_LIT(KTestFile, "TestFile.ini");
	CTestRegister* tempFiles = CTestRegister::NewLC();
	TFileName fileName;
	tempFiles->GetTempPath(fileName);
	fileName.Append(KTestFile);
	CleanupStack::PopAndDestroy(tempFiles);
	
	CreateTestFileL(fileName);

	const TTimeIntervalMicroSeconds32 KTimeOutPeriod(1000000);
	RTimer timeoutTimer;
	CleanupClosePushL(timeoutTimer);
	User::LeaveIfError(timeoutTimer.CreateLocal());
	
	//1. Assign TRequestStatus to the test file, active object will start
	TRequestStatus testFileStatus;
	iServ.FileServerNotifyChange(ENotifyFile, testFileStatus, fileName);

	//2. Cancel the active object
	iServ.FileServerNotifyChangeCancel();
	User::WaitForRequest(testFileStatus);
	test(testFileStatus == KErrCancel);
	
	//3. Assign TRequestStatus to the test file again
	iServ.FileServerNotifyChange(ENotifyFile, testFileStatus, fileName);

	//4. Delete the file
	TRequestStatus timerStatus;
	timeoutTimer.After(timerStatus, KTimeOutPeriod);
	DeleteTestFileL(fileName);
	
	User::WaitForRequest(timerStatus, testFileStatus);
	
	if (timerStatus.Int() == KRequestPending)
		{
		// The operation did not time out
		// so cancel the timer
		timeoutTimer.Cancel();
		User::WaitForRequest(timerStatus);
		}
	else
		{
		test.Printf(_L("The delete file operation timed out!!!"));
		test(EFalse);
		}

	test(testFileStatus == KErrNone);

	//5. Re-create the file	
	
	
	//6. Two different TRequestStatus' on the same file
	iServ.FileServerNotifyChange(ENotifyFile, testFileStatus, fileName);
	TRequestStatus testFileStatus2;
	iServ.FileServerNotifyChange(ENotifyFile, testFileStatus2, fileName);
	
	User::WaitForRequest(testFileStatus);
	test(testFileStatus == KErrCancel);
	test(testFileStatus2 == KRequestPending);
	
	//7.Cancel second TRequestStatus
	iServ.FileServerNotifyChangeCancel();
	User::WaitForRequest(testFileStatus2);
	test(testFileStatus2 == KErrCancel);	
	
	//8. Re-instate TRequestStatus' on the test file
	iServ.FileServerNotifyChange(ENotifyFile, testFileStatus, fileName);
	iServ.FileServerNotifyChange(ENotifyFile, testFileStatus2, fileName);

	//9. create the test file
	timeoutTimer.After(timerStatus, KTimeOutPeriod);
	
	CreateTestFileL(fileName);

	User::WaitForRequest(testFileStatus, timerStatus);
	test(testFileStatus.Int() == KErrCancel);
	
	User::WaitForRequest(testFileStatus2, timerStatus);

	if (timerStatus.Int() == KRequestPending)
		{
		// The operation did not time out
		// so cancel the timer
		timeoutTimer.Cancel();
		User::WaitForRequest(timerStatus);
		}
	else
		{
		test.Printf(_L("The delete file operation timed out!!!"));
		test(EFalse);
		}

	test(testFileStatus2 == KErrNone);
	
	// clean up
	DeleteTestFileL(fileName);
	CleanupStack::PopAndDestroy(&timeoutTimer);
	}


static void MainL()
	{
	//get the initial time
 	TTime homeTime;
	homeTime.HomeTime();
	
	CTestServerTest* testSrvTest = CTestServerTest::NewL();
	testSrvTest->TestSetHomeTimeL();
	
	//create test files
	testSrvTest->CreateTestFileL(KFileName);
	testSrvTest->CheckForFileL(KFileName, ETrue);
	
	testSrvTest->TestChangeLocaleNameL();
	testSrvTest->CopyFileL();
	testSrvTest->CheckForFileL(KFileName2, ETrue);
	
	testSrvTest->TestFileSizeL();
	testSrvTest->TestDeleteFileL(KFileName);
	testSrvTest->TestDeleteFileL(KFileName2);
	testSrvTest->CheckForFileL(KFileName, EFalse);
	testSrvTest->CheckForFileL(KFileName2, EFalse);

	testSrvTest->TestKillProcess();
	testSrvTest->TestTimeZoneL();
	
	testSrvTest->TestNotifyChangeL();

	
	testSrvTest->TestNotifyChangeL();

	
	//Set back the time to the initial time
    RPIMTestServer testserver;
    User::LeaveIfError(testserver.Connect());
    testserver.SetHomeTime(homeTime);
    
  // Return the locale to its original state.
#ifdef SYMBIAN_DISTINCT_LOCALE_MODEL
	TLanguage defaultLanguage =	User::Language();
    TBuf<50> libraryName;
    libraryName.Format(_L("elocl_lan.%03d"), defaultLanguage);
    TExtendedLocale myExtendedLocale;
    TRAPD(err, testserver.ChangeLocaleNameL(libraryName));
#else
	TRAPD(err, testserver.ChangeLocaleNameL(_L("ELOCL.loc")));
#endif
	User::LeaveIfError(err);
    
    testserver.Close();	
	delete testSrvTest;
	testSrvTest = 0;
	}
	
/**
@SYMTestCaseID PIM-TESTSERVERTEST-0001
*/

TInt E32Main()
	{
	__UHEAP_MARK;

	test.Title();
	_LIT(KInit, "@SYMTestCaseID PIM-TESTSERVERTEST-0001 initialising");
	test.Start(KInit);
	// start the loader
	CTrapCleanup* c=CTrapCleanup::New();
	test (c!=NULL);
	TRAPD(r,MainL());
	test (r==0);
	delete c;
	test.End();
	test.Close();
	__UHEAP_MARKEND;
	
	return KErrNone;
	}
