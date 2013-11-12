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
// Non-secure versions will be xxxServer.Dll and require a thread to be started
// in the process of the client.\n
// The client initialises the server by calling the one and only ordinal.\n
// 
//

/**
 @file
 @internalComponent - Internal Symbian test code
*/
#include "TPrintTestServer.h"
#include "T_FprStep.h"
#include "T_StreamStep.h"


// Literal for ServerName
_LIT(KServerName,"TPrintTestServer");
CTPrintTestServer* CTPrintTestServer::NewL()

/**
  @return - Instance of the test server.\n
 
  Called inside the MainL() function to create and start the
  CTestServer derived server.\n
 */
	{
	CTPrintTestServer * server = new (ELeave) CTPrintTestServer();
	CleanupStack::PushL(server);
	// CServer base class call
	server->StartL(KServerName);
	CleanupStack::Pop(server);
	return server;
	}


// EKA2 much simpler
// Just an E32Main and a MainL()
LOCAL_C void MainL()
/**
  Much simpler, uses the new Rendezvous() call to sync with the client.\n
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

	CTPrintTestServer* server = NULL;
	// Create the CTestServer derived server

	TRAPD(err,server = CTPrintTestServer::NewL());
	if(!err)
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
  @return - Standard Epoc error code on exit.\n
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




CTestStep* CTPrintTestServer::CreateTestStep(const TDesC& aStepName)
/**
  @return - A CTestStep derived instance.\n
 
  Secure and non-secure variants.\n
  Implementation of CTestServer pure virtual.\n
 */
	{
	CTmsTestStep* testStep = NULL;
	// This server creates just one step but create as many as you want
	// They are created "just in time" when the worker thread is created
	if(aStepName == KT_FprStep)
		testStep = new CT_FprStep();
	else if(aStepName == KT_StreamStep)
		testStep = new CT_StreamStep();
	return testStep;
	}
