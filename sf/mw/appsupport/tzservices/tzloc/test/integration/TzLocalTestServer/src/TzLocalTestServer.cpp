// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// The TzLocalTestServer calls a number of steps dependant on the step name
// supplied from the .ini file. The server is designed to test the PIM and Internet
// component TzLocalization.
// 
//

#include "TzLocalTestServer.h"
#include "TestSetTimeZoneStep.h"
#include "TestGetTzInfoFromIdStep.h"
#include "TestGetTzInfoFromDefCityStep.h"
#include "TestAddCityStep.h"
#include "TestSetCachedZoneStep.h"
#include "TestGetCachedZoneInfoStep.h"
#include "TestRemoveCityStep.h"
#include "TestGetSortedCitiesStep.h"
#include "TestTzOOMStep.h"
#include "TestCityOOMStep.h"
#include "TestGroupsOOMStep.h"
#include "TestTzManipOOMStep.h"
#include "TestFindFunctionsOOMStep.h"
#include "TestGetCityGroupInfoStep.h"
#include "TestGetAllTzStep.h"
#include "TestChangeLanguageStep.h"
#include "TestGetCitiesUTCOffStep.h"
#include "TestGetTzUTCOffStep.h"
#include "TestCheckCurrentZoneStep.h"

_LIT(KServerName,"TzLocalTestServer");

CTzLocalTestServer* CTzLocalTestServer::NewL()
/**
 * @return - Instance of the test server
 * Called inside the MainL() function to create and start the
 * CTestServer derived server.
 */
	{
	CTzLocalTestServer* server = new (ELeave) CTzLocalTestServer();
	CleanupStack::PushL(server);
	
	// Either use a StartL or ConstructL, the latter will permit
	// Server Logging.

	server-> ConstructL(KServerName);
	CleanupStack::Pop(server);
	return server;
	}


LOCAL_C void MainL()
/**
 * Uses the new Rendezvous() call to sync with the client
 */
	{
	// Leave the hooks in for platform security
#if (defined __DATA_CAGING__)
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().SecureApi(RProcess::ESecureApiOn);
#endif
	CActiveScheduler* sched = NULL;
	sched = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CTzLocalTestServer* server = NULL;
	// Create the CTestServer derived server
	TRAPD(err,server = CTzLocalTestServer::NewL());
	if(KErrNone == err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	delete server;
	delete sched;
	}


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
	TRAPD(err,MainL());
	
	if (err != KErrNone)
	    {
	    // Error	
	    }	
	    
	delete cleanup;
	return KErrNone;
    }
    

CTestStep* CTzLocalTestServer::CreateTestStep(const TDesC& aStepName)
/**
 * @return - A CTestStep derived instance
 * Implementation of CTestServer pure virtual
 */
	{
	CTestStep* testStep = NULL;

    if(aStepName == KSetTimeZoneStep)
        {
	    testStep = new CTestSetTimeZoneStep();
        }
    
    else if(aStepName == KTestGetTzInfoFromId)
        {    	
        testStep = new CTestGetTzInfoFromIdStep();
        }
    
    else if(aStepName == KTestGetTzInfoFromDefCity)
        {
        testStep = new CTestGetTzInfoFromDefCityStep();
        }
    
    else if(aStepName == KTestAddCityStep)
        {    	
        testStep = new CTestAddCityStep();
        }
    
    else if (aStepName == KTestSetFrequentlyUsedZoneStep)
        {
        testStep = new CTestSetFrequentlyUsedZoneStep();
        }
    
    else if (aStepName == KTestGetFrequentlyUsedZoneInfoStep)
        {    	
        testStep = new CTestGetFrequentlyUsedZoneInfoStep();
        }
    
    else if (aStepName == KTestRemoveCityStep)
        {
        testStep = new CTestRemoveCityStep();
        }
    
    else if (aStepName == KTestGetSortedCitiesStep)
        {
        testStep = new CTestGetSortedCitiesStep();
        }
        
    else if (aStepName == KTestTzOOMStep)
        {
        testStep = new CTestTzOOMStep();
        }
    
    else if (aStepName == KTestCityOOMStep)
        {
        testStep = new CTestCityOOMStep();
        }
    
    else if (aStepName == KTestGroupsOOMStep)
        {
        testStep = new CTestGroupsOOMStep();
        }
    
    else if (aStepName == KTestTzManipOOMStep)
        {
        testStep = new CTestTzManipOOMStep();
        }
    
    else if (aStepName == KTestFindFunctionsOOMStep)
        {
        testStep = new CTestFindFunctionsOOMStep();
        }
        
    else if (aStepName == KTestGetCityGroupInfoStep)
        {
        testStep = new CTestGetCityGroupInfoStep();
        }
        
    else if (aStepName == KTestGetAllTzStep)
        {
        testStep = new CTestGetAllTzStep();
        }
    
    else if (aStepName == KTestChangeLanguageStep)
        {
        testStep = new CTestChangeLanguageStep();
        }
        
    else if (aStepName == KTestGetCitiesUTCOffStep)
        {
    	testStep = new CTestGetCitiesUTCOffStep;
        }

    else if (aStepName == KTestGetTzUTCOffStep)
        {
    	testStep = new CTestGetTzUTCOffStep;
        }
        
    else if (aStepName == KTestCheckCurrentZoneStep)
        {
        testStep = new CTestCheckCurrentZoneStep();
        }

	return testStep;
	}
