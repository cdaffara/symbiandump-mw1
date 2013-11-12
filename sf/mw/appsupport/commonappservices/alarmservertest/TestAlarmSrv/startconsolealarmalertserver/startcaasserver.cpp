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

#include "startcaasserver.h"
#include "startcaasstep.h"

#define UNUSED_VAR(a) a = a

_LIT(KServerName,"StartCAAS");


CStartCAASServer* CStartCAASServer::NewL()
/**
 * @return - Instance of the test server
 * Called inside the MainL() function to create and start the CTestServer derived server.
 */
	{
	CStartCAASServer* server = new (ELeave) CStartCAASServer();
	CleanupStack::PushL(server);
	
	//server->StartL(KServerName); 			// either use a StartL or ConstructL, 
	server->ConstructL(KServerName);		// the latter permits Server Logging.
	
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
	CActiveScheduler* sched  =NULL;
	sched = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);

	CStartCAASServer* server = NULL;
	TRAPD(err,server = CStartCAASServer::NewL());
	if(!err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
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
	TRAPD(err,MainL());
	UNUSED_VAR(err);	
	delete cleanup;
	return KErrNone;
    }


// Create a thread in the calling process
// Emulator typhoon and earlier


CTestStep* CStartCAASServer::CreateTestStep(const TDesC& aStepName)
/**
 * @return - A CTestStep derived instance
 * Implementation of CTestServer pure virtual
 */
	{
	CTestStep* testStep = NULL;
	if (aStepName == KStepNameStartCAAS)
		{
		testStep =  new(ELeave) CStartCAASStep; // Method could leave while construction
		}
	return testStep;
	}
