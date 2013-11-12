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

#include "DstUnitServer.h"

// Test step include files
#include "dstdatabasemanagerstep.h"
#include "encodedrulesstep.h"
#include "StepSetLocation.h"
#include "tzupdatenotification.h"
#include "TzCachingTestStep.h"
#include "IsDaylightSavingsOn.h"
#include "actualisedrules.h"
#include "tzrulesteststep.h"


_LIT(KServerName,"DstUnitTestServer");
_LIT(KTzUserDataDb, "c:\\private\\1020383e\\SQLite__tzuserdata.db");


//static RFs fsSession;


/**
Called inside the MainL() function to create and start the CTestServer derived
server.

@return - Instance of the test server
*/
CDstUnitServer* CDstUnitServer::NewL()
	{
	CDstUnitServer * server = new (ELeave) CDstUnitServer();
	CleanupStack::PushL(server);
	server-> ConstructL(KServerName);
	CleanupStack::Pop(server);
	return server;
	}


LOCAL_C void MainL()
	{
	RFs fsSession;
	
	User::LeaveIfError(fsSession.Connect());
	TInt ret = fsSession.Delete(KTzUserDataDb);
	if(ret != KErrNotFound)
		{
		User::LeaveIfError(ret);
		}
	fsSession.Close();
	
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().SecureApi(RProcess::ESecureApiOn);

	CActiveScheduler* sched = NULL;
	sched = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CDstUnitServer* server = NULL;

	// Create the CTestServer derived server.
	TRAPD(err, server = CDstUnitServer::NewL());

	if(!err)
		{
		// Sync with the client and enter the active scheduler.
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}

	delete server;
	delete sched;

	User::LeaveIfError(fsSession.Connect());
	ret = fsSession.Delete(KTzUserDataDb);
	if(ret != KErrNotFound)
		{
		User::LeaveIfError(ret);
		}
	fsSession.Close();
	}


/**
@return Standard Epoc error code on exit.
*/
GLDEF_C TInt E32Main()
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


/**
Implementation of CTestServer pure virtual method.

@param aStepName Name of the test step.

@return A CTestStep derived instance.
*/
CTestStep* CDstUnitServer::CreateTestStep(const TDesC& aStepName)
	{
	CTestStep* testStep = NULL;

	if(aStepName == KDstDatabaseManagerStep)
    	{
		testStep = new CDstDatabaseManagerStep(*this);
    	}
	else if (aStepName == KStepTZConfigAgent)
        {
		testStep = new CTZConfigAgentTest(*this);
        }
	else if (aStepName == KStepSetLocation)
        {
		testStep = new CSetLocation(*this);
        }
	else if (aStepName == KStepConvertTime)
        {
		testStep = new CConvertTime(*this);
        }
	else if (aStepName == KEncodedRulesStep)
        {
		testStep = new CEncodedRulesStep(*this);
        }
	else if (aStepName == KTzUpdateNotification)
        {
		testStep = new CTzUpdateNotification(*this);
        }
	else if (aStepName == KStepGetOffsets)
        {
		testStep = new CGetOffsetsTest(*this);
        }
	else if (aStepName == KTimezoneCacheTestName)
        {
		testStep = new CTzCachingTestStep(*this);
        }
	else if (aStepName == KDaylightSavingsStep)
        {
        testStep = new CDaylightSavingsTest( *this );
        }
	else if(aStepName == KActualisedRules)
	    {
		testStep = new CActualisedRules();
	    }
	else if(aStepName == KTzRules)
	    {
		testStep = new CTzRulesTestStep();
	    }

	return testStep;
	}
