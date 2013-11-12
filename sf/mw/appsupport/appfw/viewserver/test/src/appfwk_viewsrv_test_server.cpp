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
//

/**
 @file
 @test
 @internalComponent - Internal Symbian test code 
*/

#include <ecom/ecom.h>
#include "appfwk_viewsrv_test_server.h"
#include "appfwk_viewsrv_test_step_Capability1.h"
#include "appfwk_viewsrv_test_step_Capability2.h"
#include "appfwk_viewsrv_test_step_tvwa.h"

_LIT(KCAppfwkViewSrvTestServerName,"appfwk_viewsrv_test_server");

/**
  @return - A CTestStep derived instance
  @param - aStepName the test case name to be executed
  Implementation of CTestServer pure virtual
 */
CTestStep* CAppfwkViewSrvTestServer::CreateTestStep(const TDesC& aStepName)
	{
	CTestStep* testStep = NULL;
	
	if(aStepName == KCAppfwkViewsrvTestStepCapability1)
		{
		testStep = new CAppfwkViewsrvTestStepCapability1();
		}
	else if(aStepName == KCAppfwkViewsrvTestStepCapability2)
		{
		testStep = new CAppfwkViewsrvTestStepCapability2();
		}
	else if(aStepName == KTVwaStep)
		{
		testStep = new CTVwaStep();
		}
		
	return testStep;
	}

/**
  @return - Instance of the test server
  Called inside the MainL() function to create and start the
  CTestServer derived server.
 */
CAppfwkViewSrvTestServer* CAppfwkViewSrvTestServer::NewL()
	{
	CAppfwkViewSrvTestServer * server = new (ELeave) CAppfwkViewSrvTestServer();
	CleanupStack::PushL(server);
	// CServer base class call
	TParsePtrC serverName(RProcess().FileName());	
	server->StartL(serverName.Name());
	CleanupStack::Pop(server);
	return server;
	}

LOCAL_C void MainL()
	{
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().SecureApi(RProcess::ESecureApiOn);
	CActiveScheduler* sched=NULL;
	sched=new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CAppfwkViewSrvTestServer* server = NULL;
	// Create the CTestServer derived server
	TRAPD(err,server = CAppfwkViewSrvTestServer::NewL());
	if(!err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	delete server;
	delete sched;
	}

/**
  @return - Standard Epoc error code on process exit
  Process entry point. Called by client using RProcess API
  */
GLDEF_C TInt E32Main(void)
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
	TRAPD(err,MainL());
	
	if (err)
		{
		RDebug::Print(_L("CAppfwkViewSrvTestServer::MainL - Error: %d"), err);
	   	User::Panic(KCAppfwkViewSrvTestServerName,err);
		}
		
	delete cleanup;
	REComSession::FinalClose();	
	__UHEAP_MARKEND;
	return KErrNone;
    }

