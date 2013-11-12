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
#include "testserver.h"
#include <vtzrules.h>

_LIT8(KEuropeLondon, "Europe/London");
_LIT8(KEuropeParis, "Europe/Paris");
_LIT(KTzDatabaseToBeCopied, "z:\\testresourcefiles\\tzdb.dbz");
_LIT(KTzDatabaseCopiedToFlash, "c:\\private\\1020383e\\tzdb.dbz");
_LIT(KTzDatabaseToBeDeleted, "c:\\private\\1020383e\\tzdb.dbz");

RTest test (_L("t_databaseupdate"));

/**
@SYMTestCaseID      PIM-APPSERV-TZ-TZS-ECL-0001
@SYMTestCaseDesc    The purpose of this test is to verify the use of eclipsed system
					TZ rules database.  
@SYMTestActions     1.	Use the RTz API to obtain a copy of the first TZ rule (preUpdateRule)
						for a given time zone.
					2.	Copy a system TZ rules database that modifies the TZ rule obtained in 
						action 1 to the appropriate location the C: drive.
					3.	Use the RTz API to obtain a copy of the first TZ rule (postUpdateRule) 
						for the same time zone used in action 2.

@SYMTestExpectedResults     The preUpdateRule object is not the same as the postUpdateRule object.
@SYMREQ                     REQ9952
@SYMTestType                CT
@SYMTestPriority            1
*/
LOCAL_C void TestRulesInTzDatabaseL()
    {
   	RTz tz;
  	CleanupClosePushL(tz);
  	User::LeaveIfError(tz.Connect());
    	
  	CTzId* tzId = CTzId::NewL(KEuropeLondon);
  	CleanupStack::PushL(tzId);
  	tz.SetTimeZoneL(*tzId);
  	
  	TDateTime startDateTime(2008,EJanuary,0,0,0,0,0);
	TDateTime endDateTime(2008,EDecember,30,23,59,59,0);
   	CTzRules* tzRules = tz.GetTimeZoneRulesL(*tzId,startDateTime,endDateTime,ETzUtcTimeReference);
  	CleanupStack::PushL(tzRules);
    	
   	TInt count = tzRules->Count();
	test.Printf(_L(" @SYMTestCaseID PIM-APPSERV-TZ-TZS-ECL-0001"));
   	test.Printf(_L("The number of rules found in default database are %d\n"), count);
  	
   	CleanupStack::PopAndDestroy(tzRules);
    tz.Close();
  	
   	RPIMTestServer serv;	
	CleanupClosePushL(serv);
	TInt err = serv.Connect();
	if (err != KErrNone)
		{
		_LIT(KErrTestServerConnection, "Couldn't connect to PIMTestServer\n");
		test.Printf(KErrTestServerConnection);
		User::Leave(err);
		}
	TRAP(err, serv.CopyFileL(KTzDatabaseToBeCopied, KTzDatabaseCopiedToFlash));
	if (err != KErrNone)
		{
		_LIT(KErrCopyFiles, "Couldn't copy database file\n");
		test.Printf(KErrCopyFiles);
		User::Leave(err);
		}

	User::LeaveIfError(tz.Connect());
	
	CTzId* parisId = CTzId::NewL(KEuropeParis);
	CleanupStack::PushL(parisId);
	tz.SetTimeZoneL(*parisId);
	CleanupStack::PopAndDestroy(parisId);
  	
 	tz.SetTimeZoneL(*tzId);
  	CTzRules* tzRulesFlash = tz.GetTimeZoneRulesL(*tzId,startDateTime,endDateTime,ETzUtcTimeReference);
  	CleanupStack::PushL(tzRulesFlash);
   	  	
   	TInt countFlash = tzRulesFlash->Count();
  	test.Printf(_L("The number of rules found in the database on flash are %d\n"), countFlash);
  		
  	test(count != countFlash);
	
	serv.DeleteFileL(KTzDatabaseToBeDeleted);
	tz.SwiObsBeginL();
	tz.SwiObsFileChangedL(RTz::EFilterTzPrivate);
	tz.SwiObsEndL();
	
	CleanupStack::PopAndDestroy(tzRulesFlash);
	CleanupStack::PopAndDestroy(&serv);   	
  	CleanupStack::PopAndDestroy(tzId);
   	CleanupStack::PopAndDestroy(&tz);    	   	
	}

/**
@SYMTestCaseID      PIM-DATABASEUPDATE-0001
@SYMTestCaseDesc    The purpose of this test is to verify that eclipsing is taken into account
					even if the time zone server is running.  
@SYMTestActions     1.	Use the RTz API to obtain a copy of the first TZ rule (preUpdateRule)
						for a given time zone.
					2.	Copy a system TZ rules database that modifies the TZ rule obtained in 
						action 1 to the appropriate location the C: drive. The server is still
						running when this happens.
					3.	Use the RTz API to obtain a copy of the first TZ rule (postUpdateRule) 
						for the same time zone used in action 2.

@SYMTestExpectedResults     The preUpdateRule object is not the same as the postUpdateRule object.
@SYMREQ                     REQ9952
@SYMTestType                CT
@SYMTestPriority            1
*/
LOCAL_C void TestRulesInTzDatabaseLiveUpdateL()
    {
    RTz tz;
  	CleanupClosePushL(tz);
  	User::LeaveIfError(tz.Connect());
  	
  	CTzId* tzId = CTzId::NewL(KEuropeLondon);
  	CleanupStack::PushL(tzId);
  	tz.SetTimeZoneL(*tzId);
  	
  	TDateTime startDateTime(2008,EJanuary,0,0,0,0,0);
	TDateTime endDateTime(2008,EDecember,30,23,59,59,0);
   	CTzRules* tzRules = tz.GetTimeZoneRulesL(*tzId,startDateTime,endDateTime,ETzUtcTimeReference);
  	CleanupStack::PushL(tzRules);
    TInt count = tzRules->Count();
   	test.Printf(_L("The number of rules found in default database are %d\n"), count);
  	CleanupStack::PopAndDestroy(tzRules);
  	
  	RPIMTestServer serv;	
	CleanupClosePushL(serv);
	TInt err = serv.Connect();
	if (err != KErrNone)
		{
		_LIT(KErrTestServerConnection, "Couldn't connect to PIMTestServer\n");
		test.Printf(KErrTestServerConnection);
		User::Leave(err);
		}
	TRAP(err, serv.CopyFileL(KTzDatabaseToBeCopied, KTzDatabaseCopiedToFlash));
	if (err != KErrNone)
		{
		_LIT(KErrCopyFiles, "Couldn't copy database file\n");
		test.Printf(KErrCopyFiles);
		User::Leave(err);
		}
	tz.SwiObsBeginL();
	tz.SwiObsFileChangedL(RTz::EFilterTzPrivate);
	tz.SwiObsEndL();
   	
	CTzRules* tzRulesFlash = tz.GetTimeZoneRulesL(*tzId,startDateTime,endDateTime,ETzUtcTimeReference);
  	CleanupStack::PushL(tzRulesFlash);
   	  	
   	TInt countFlash = tzRulesFlash->Count();
	test.Printf(_L(" @SYMTestCaseID PIM-DATABASEUPDATE-0001 "));
  	test.Printf(_L("The number of rules found in the database on flash are %d\n"), countFlash);
  		
  	test(count != countFlash);
	
	serv.DeleteFileL(KTzDatabaseToBeDeleted);
	
	CleanupStack::PopAndDestroy(tzRulesFlash);
	CleanupStack::PopAndDestroy(&serv);   	
  	CleanupStack::PopAndDestroy(tzId);
	CleanupStack::PopAndDestroy(&tz);    	
    }

LOCAL_C void DoStartL()
	{
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	
	CActiveScheduler::Install(scheduler);

	TestRulesInTzDatabaseL();
	TestRulesInTzDatabaseLiveUpdateL();
	
	// Delete active scheduler
	CleanupStack::PopAndDestroy(scheduler);	
	}

//  Global Functions
	
	
GLDEF_C TInt E32Main()
	{
	// Create cleanup stack
	__UHEAP_MARK;
	test.Start(_L("Starting Test t_databaseupdate"));
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

