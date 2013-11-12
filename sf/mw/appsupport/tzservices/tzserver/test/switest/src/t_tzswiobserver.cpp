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
// Test that the following package file provides the notification of the files that 
// were installed. This is done by checking the logged file.
//

/*
;Package file for timezone database upgrade

;Languages
&EN

;Header
#{"TimeZone-Database-Eclipse"}, (0x200020B0), 1, 1, 0, TYPE=SP

%{"Symbian Software Ltd."}
:"Symbian Software Ltd."

;Ordinary file to selected drive
"tzdb.dbz"-"$:\private\1020383E\tzdb.dbz"
"tz.r01"-"$:\resource\timezonelocalization\timezones.r01"
"tz.r02"-"$:\resource\timezonelocalization\timezones.r02"
"tzg.r01"-"$:\resource\timezonelocalization\timezonegroups.r01"
"tzg.r02"-"$:\resource\timezonelocalization\timezonegroups.r02"

;End of file
*/

#include <e32test.h>
#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>
#include "cswiappinstall.h"
#include "ctzswiplugintester.h"
#include <swi/swiobservedinfo.h>
#include "testserver.h"

_LIT(KTextConsoleTitle, "Console");
_LIT(KTextFailed, " failed, leave code = %d");
_LIT(KTextEnd, " Test completed\n");
#if defined(_DEBUG)
_LIT(KNotificationLogFile, "c:\\logs\\Tz\\TzSwiObserver.log");
_LIT(KTzSISFileToBeCopied, "z:\\data\\TEFSigned_timezone_upgrade.SIS");
_LIT(KSisFile, "C:\\TEFSigned_timezone_upgrade.SIS");
_LIT(KTimeZonePackageUid, "200020B0");
_LIT(KTimeZonePackageName, "TimeZone-Database-Eclipse");
_LIT(KTimeZoneVendorName, "Symbian Software Ltd.");
_LIT8(KActionAdded, "Added ");
_LIT8(KActionDeleted, "Deleted ");
#endif

_LIT8(KTzdb_dbz,"c:\\private\\1020383E\\tzdb.dbz");
_LIT8(KTimezones_r01,"c:\\resource\\timezonelocalization\\timezones.r01");
_LIT8(KTimezones_r02,"c:\\resource\\timezonelocalization\\timezones.r02");
_LIT8(KTimezonegroups_r01,"c:\\resource\\timezonelocalization\\timezonegroups.r01");
_LIT8(KTimezonegroups_r02,"c:\\resource\\timezonelocalization\\timezonegroups.r02");

RTest test(_L("Swi Plugin"));

const TInt KNumOfFilesTest = 5;
const TPtrC8 KFilesEclipsed[KNumOfFilesTest] =
	{
	KTzdb_dbz (),
	KTimezones_r01 (),
	KTimezones_r02 (),
	KTimezonegroups_r01 (),
	KTimezonegroups_r02 (),
	};

LOCAL_D CConsoleBase* console; // write all messages to this
using namespace Swi;

EXPORT_C void CopyFileL(const TDesC& aCopyFromFile, const TDesC& aCopyToFile)
	{
	RPIMTestServer serv;	
	CleanupClosePushL(serv);
	User::LeaveIfError(serv.Connect());
	
	serv.CopyFileL(aCopyFromFile, aCopyToFile);
	
	CleanupStack::PopAndDestroy(&serv);
	}

#if defined(_DEBUG)

LOCAL_C TInt CheckLogFileL(RFs& fileSession, const TDesC8& aInstallAction)
	{
	RFile file;
	User::LeaveIfError(file.Open(fileSession, KNotificationLogFile(), EFileRead));
	TInt fileSizeBytes(0);
	User::LeaveIfError(file.Size(fileSizeBytes));
	HBufC8* readFile = HBufC8::NewLC(fileSizeBytes);
	TPtr8 ptr = readFile->Des(); 
	
	TInt err =  file.Read(ptr);
	if (err != KErrNone && err != KErrEof)
		{
		User::Leave(err);	
		}
	
	//Print the file contents
	TBuf<1024> fileData(fileSizeBytes);
	fileData.Copy(ptr);
	console->Write(fileData);
	
	TInt counter(0);
	for (TInt var = 0; var < KNumOfFilesTest; ++var)
		{
		TBuf8<256> actionAndPath;
		actionAndPath = aInstallAction;
		actionAndPath.Append(KFilesEclipsed[var]);
		if (ptr.Find(actionAndPath)!= KErrNotFound)
			{
			++counter;
			}
		}
	CleanupStack::PopAndDestroy(readFile);
	file.Close();
	return counter;
	}

LOCAL_C void TestInstallL()
	{
	RFs fileSession;
	User::LeaveIfError(fileSession.Connect());
	
	test.Printf(_L("Upgrading the timezone rules....\n"));
	
	TRequestStatus reqStatus;
	CCTzSwiPluginTester* sisInstaller = CCTzSwiPluginTester::NewLC();
	CopyFileL(KTzSISFileToBeCopied(), KSisFile());
	sisInstaller->InstallSIS(KSisFile(), CCTzSwiPluginTester::EFileLog, reqStatus);
	CActiveScheduler::Start();
	CleanupStack::PopAndDestroy(sisInstaller);
	
	test.Printf(_L("timezone rules upgraded....\n"));
	//Expect 5 notifications for five files.
	// tzdb.dbz, timezones.r01, timezones.r02, timezonegroups.r01, timezonegroups.r02
	test (CheckLogFileL(fileSession, KActionAdded()) == 5);
	test.Printf(_L("timezone server notified correctly for install file changes....\n"));
	
	fileSession.Close();
	}


LOCAL_C void TestUninstallL()
	{
	RFs fileSession;
	User::LeaveIfError(fileSession.Connect());
	
	test.Printf(_L("Upgrading the timezone rules....\n"));
	
	TRequestStatus reqStatus;
	CCTzSwiPluginTester* sisInstaller = CCTzSwiPluginTester::NewLC();
	CopyFileL(KTzSISFileToBeCopied(), KSisFile());
	sisInstaller->UninstallSIS(KTimeZonePackageUid(), KTimeZonePackageName(), KTimeZoneVendorName(), CCTzSwiPluginTester::EFileLog, reqStatus);
	//Need this to allow the notification plug-in code to run
	User::After(1000000);
	CleanupStack::PopAndDestroy(sisInstaller);
	
	test.Printf(_L("timezone rules uninstalled....\n"));
	//Expect 5 notifications for five files.
	// tzdb.dbz, timezones.r01, timezones.r02, timezonegroups.r01, timezonegroups.r02
	test (CheckLogFileL(fileSession, KActionDeleted()) == 5);
	test.Printf(_L("timezone server notified correctly for uninstall file changes....\n"));
	
	fileSession.Close();
	}

#endif


LOCAL_C void DoStartL()
	{
	// Create active scheduler (to run active objects)
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);
	
#if defined(_DEBUG) 
	
	/**
	@SYMTestCaseID PIM-TTZSWIOBSERVER-0001
	*/
	test.Start(_L("@SYMTestCaseID PIM-TTZSWIOBSERVER-0001 Test install notifications\n"));
	TestInstallL();
	
	test.Next(_L("Test uninstall notifications\n"));
	TestUninstallL();
#else
	
	/**
	@SYMTestCaseID PIM-TTZSWIOBSERVER-0002
	*/
	test.Start(_L("@SYMTestCaseID PIM-TTZSWIOBSERVER-0002 INFO: These tests can only be run for a debug build as the plugin code which it relies on is only switched on in debug mode.\n"));
#endif

	test.End();
	test.Close();
	// Delete active scheduler
	CleanupStack::PopAndDestroy(scheduler);
	}

GLDEF_C TInt E32Main()
	{
	// Create cleanup stack
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();

	// Create output console
	TRAPD(createError, console = Console::NewL(KTextConsoleTitle, TSize(KConsFullScreen,KConsFullScreen)));
	if (createError)
	return createError;

	// Run application code inside TRAP harness
	TRAPD(mainError, DoStartL());
	if (mainError)
		{
		console->Printf(KTextFailed, mainError);
		}
	
	console->Printf(KTextEnd);

	delete console;
	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
	}
