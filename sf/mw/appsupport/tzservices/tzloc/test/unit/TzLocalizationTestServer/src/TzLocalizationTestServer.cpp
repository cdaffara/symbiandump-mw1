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

#include "TzLocalizationTestServer.h"
#include "TzLocalizationTestStep.h"
#include "tzpassivetimezoneteststep.h"

// The Name of your test server
_LIT(KServerName,"TzLocalizationTestServer");

/**
 * @return - Instance of the test server
 * Called inside the MainL() function to create and start the
 * CTestServer derived server.
 */
CTzLocalizationTestServer* CTzLocalizationTestServer::NewL()
	{
	CTzLocalizationTestServer * server = new (ELeave) CTzLocalizationTestServer();
	CleanupStack::PushL(server);
	server-> ConstructL(KServerName);
	CleanupStack::Pop(server);
	return server;
	}

void CTzLocalizationTestServer::ConstructL(const TDesC& aName)
	{
	CTestServer::ConstructL(aName);
	}

CTzLocalizationTestServer::~CTzLocalizationTestServer()
	{
	}

LOCAL_C void MainL()
	{
	// Leave the hooks in for platform security
#if (defined __DATA_CAGING__)
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().SecureApi(RProcess::ESecureApiOn);
#endif
	CActiveScheduler* sched=NULL;
	sched=new(ELeave) CActiveScheduler;
	CleanupStack::PushL(sched);
	CActiveScheduler::Install(sched);

	CTzLocalizationTestServer* server = NULL;
	TRAPD(err,server = CTzLocalizationTestServer::NewL());
	if(!err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	delete server;
	CleanupStack::PopAndDestroy(sched);
	}

/**
 * @return - Standard Epoc error code on exit
 */
GLDEF_C TInt E32Main()
	{
	TInt err = KErrNone;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
	TRAP(err,MainL());
	delete cleanup;
	return err;
    }

/**
 * @return - A CTestStep derived instance
 * Implementation of CTestServer pure virtual
 */
CTestStep* CTzLocalizationTestServer::CreateTestStep(const TDesC& aStepName)
	{
	CTestStep* testStep = NULL;
	if(aStepName == KTzInvalidTimeZoneDbTestStep)
		{
		testStep = new CTzInvalidTimeZoneDbTestStep();
		}
	if(aStepName == KTzPassiveTimeZoneTestStep)
		{
		testStep = new CTzPassiveTimeZoneTestStep();
		}
	else if(aStepName == KTzPassiveAllTimeZonesTestStep)
		{
		testStep = new CTzPassiveAllTimeZonesTestStep();
		}
	else if(aStepName == KTzActiveTimeZoneTestStep)
		{
		testStep = new CTzActiveTimeZoneTestStep();
		}		
	else if(aStepName == KTzPassiveCityTestStep)
		{
		testStep = new CTzPassiveCityTestStep();
		}
	else if(aStepName == KTzPassiveAllCitiesTestStep)
		{
		testStep = new CTzPassiveAllCitiesTestStep();
		}
	else if(aStepName == KTzCitiesWithUTCOffsetTestStep)
		{
		testStep = new CTzCitiesWithUTCOffsetTestStep();
		}
	else if(aStepName == KTzTimeZonesWithUTCOffsetTestStep)
		{
		testStep = new CTzTimeZonesWithUTCOffsetTestStep();
		}
	else if(aStepName == KTzActiveCityTestStep)
		{
		testStep = new CTzActiveCityTestStep();
		}
	else if(aStepName == KTzGroupTestStep)
		{
		testStep = new CTzGroupTestStep();
		}		
	else if(aStepName == KTzAllGroupsTestStep)
		{
		testStep = new CTzAllGroupsTestStep();
		}		
	else if(aStepName == KTzSortingTestStep)
		{
		testStep = new CTzSortingTestStep();
		}		
	else if(aStepName == KTzFindingTestStep)
		{
		testStep = new CTzFindingTestStep;
		}		
	else if(aStepName == KTzTimeZonesSpeedTestStep)
		{
		testStep = new CTzTimeZonesSpeedTestStep;
		}		
	else if(aStepName == KTzCitiesSpeedTestStep)
		{
		testStep = new CTzCitiesSpeedTestStep;
		}		
	else if(aStepName == KTzGroupsSpeedTestStep)
		{
		testStep = new CTzGroupsSpeedTestStep;
		}
	else if(aStepName == KTzPubSubTestStep)
		{
		testStep = new CTzPubSubTestStep;
		}
	else if(aStepName == KTzRaceTestStep)
		{
		testStep = new CTzRaceTestStep;
		}		
	else if(aStepName == KTzPrepEnvDelFileTestStep)
		{
		testStep = new CPrepEnvDelFileStep;
		}
	else if(aStepName == KTzPrepEnvCopyFileTestStep)
		{
		testStep = new CPrepEnvCopyFileStep;
		}	

	return testStep;
	}
