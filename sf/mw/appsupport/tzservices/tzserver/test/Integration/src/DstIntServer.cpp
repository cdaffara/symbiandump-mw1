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

#include "DstIntServer.h"

#include "SetAndCheckLocation.h"
#include "ConvertTime.h"
#include "ImportVcal.h"
#include "Performance.h"
#include "DSTUpdate.h"
#include "UTCOffset.h"
#include "AutoUpdateNotification.h"
#include "UTCAlarm.h"
#include "IsDaylightSavingOn.h"
#include "TestTZServer.h"
#include "SetTz.h"
#include "SetSysTime.h"
#include "ServerMsgSender.h"
#include "BackupRestore.h"
#include "TimeZoneOffsetCheck.h"
#include "NextDSTChange.h"
#include "SubscribeNextDSTChange.h"
#include "UnknownZoneTime.h"

#define UNUSED_VAR(a) a = a

// The name of this server 
_LIT(KServerName,"DstIntServer");

#ifdef __WINS__
_LIT(KConsoleAlarmAlertServerImg,"ConsoleAlarmAlertServer");
const TUid KServerUid2={0x1000008D};
const TUid KServerUid3={0x101F502A};
const TUidType serverUid(KNullUid,KServerUid2,KServerUid3);
#endif


CDstIntServer* CDstIntServer::NewL()
/**
 * @return - Instance of the test server
 * Called inside the MainL() function to create and start the
 * CTestServer derived server.
 */
	{
	CDstIntServer * server = new (ELeave) CDstIntServer();
	CleanupStack::PushL(server);
	
	// Either use a StartL or ConstructL, the latter will permit
	// Server Logging.

	//server->StartL(KServerName); 
	server-> ConstructL(KServerName);
	CleanupStack::Pop(server);
	return server;
	}

// EKA2 much simpler
// Just an E32Main and a MainL()
LOCAL_C void MainL()
/**
 * Much simpler, uses the new Rendezvous() call to sync with the client
 */
	{
	// Leave the hooks in for platform security
#if (defined __DATA_CAGING__)
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().SecureApi(RProcess::ESecureApiOn);
#endif
	CActiveScheduler* sched=NULL;
	sched=new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CDstIntServer* server = NULL;
	// Create the CTestServer derived server
	TRAPD(err,server = CDstIntServer::NewL());
	if(!err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		
		#ifdef __WINS__
		// Start the Alarm Server
		RProcess alarmServer;
		TInt err = alarmServer.Create(KConsoleAlarmAlertServerImg, KNullDesC, serverUid);
		TRequestStatus stat;
		alarmServer.Rendezvous(stat);
		if (stat != KRequestPending)
			{
			alarmServer.Kill(0);
			}
		else
			{
			alarmServer.Resume();
			}
			
		User::WaitForRequest(stat);
		TInt result = stat.Int();
		if(result != KErrNone && result != KErrAlreadyExists)
			{
			User::LeaveIfError(result);	
			}

		#endif
		
		sched->Start();
		}
	delete server;
	delete sched;
	}

// Only a DLL on emulator for typhoon and earlier

GLDEF_C TInt E32Main()
/**
 * @return - Standard Epoc error code on exit
 */
	{
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
	TRAP_IGNORE(MainL());

	delete cleanup;
	return KErrNone;
    }

// Create a thread in the calling process
// Emulator typhoon and earlier


CTestStep* CDstIntServer::CreateTestStep(const TDesC& aStepName)
/**
 * @return - A CTestStep derived instance
 * Implementation of CTestServer pure virtual
 */

/**
 * This method is not permitted to leave, so we don't use new(ELeave) and instead use the
 * standard implementation of new.  This returns NULL if sufficient memory cannot be
 * allocated, and the return from this method will then be NULL. The alternative would be
 * to panic at this point.
 */
	{
	CTestStep* testStep = NULL;

	if(aStepName == KSetAndCheckLocation)
	    {
		testStep = new CSetAndCheckLocation();
	    }
	else if(aStepName == KConvertTime)
	    {
		testStep = new CConvertTime();
	    }
	else if(aStepName == KConvertTimeViaServer)
	    {
		testStep = new CConvertTimeViaServer();
	    }
	else if(aStepName == KImportVCal)
	    {
		testStep = new CImportVCal();
	    }
	else if(aStepName == KPerformance)
	    {
		testStep = new CPerformance();
	    }
	else if(aStepName == KDSTUpdate)
	    {
		testStep = new CDSTUpdate();
	    }
	else if(aStepName == KUTCOffset)
	    {
		testStep = new CUTCOffset();
	    }
	else if(aStepName == KAutoUpdateNotification)
	    {
		testStep = new CAutoUpdateNotification();	    
	    }
	else if(aStepName == KUTCAlarm)
	    {
		testStep = new CUTCAlarm();
	    }
	else if(aStepName == KIsDaylightSavingOn)
	    {
		testStep = new CIsDaylightSavingOn();
	    }
	else if (aStepName == KServerMsgSenderStep)
	    {
		testStep = new CServerMsgSenderTestStep;
	    }
	else if(aStepName == KBackupRestoreTestStepName)
	    {
		testStep = new CBackupRestoreTestStep;
		}
	else if(aStepName == KUnknownZone)
	    {
		testStep = new CUnknownZoneTest();
        }
	else if(aStepName == KTestTZServer)
		{
		testStep = new CTestTZServer();
		}
	else if(aStepName == KSetTz)
		{
		testStep = new CSetTz();
		}
	else if(aStepName == KSetSysTime)
		{
		testStep = new CSetSysTime();
		}
	else if(aStepName == KTimeZoneOffsetCheckStep)
		{
		testStep = new CTimeZoneOffsetCheck();	
		}
	else if(aStepName == KNextDSTChange)
		{
		testStep = CNextDSTChange::NewL();
		}
	else if(aStepName == KSubscribeNextDSTChange)
		{
		testStep = CSubscribeNextDSTChange::NewL();
		}
	else if(aStepName == KUnknownZoneTime)
		{
		testStep = CUnknownZoneTime::NewL();
		}
	return testStep;
	}
