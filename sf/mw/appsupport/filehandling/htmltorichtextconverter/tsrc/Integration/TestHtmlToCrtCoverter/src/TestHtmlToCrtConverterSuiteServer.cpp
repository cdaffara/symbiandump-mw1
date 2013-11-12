// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "TestHtmlToCrtConverterSuiteServer.h"
#include "TestHtmlToCrtConverterBufferStep.h"
#include "TestHtmlToCrtConverterStep.h"

CTestHtmlToCrtConverterSuite* CTestHtmlToCrtConverterSuite::NewL()
/**
 * @return - Instance of the test server
 * Same code for Secure and non-secure variants
 * Called inside the MainL() function to create and start the
 * CTestServer derived server.
 */
	{
	CTestHtmlToCrtConverterSuite *	server = new (ELeave) CTestHtmlToCrtConverterSuite();
	CleanupStack::PushL(server);
	// CServer base class call
	RProcess	handle = RProcess();
	TParsePtrC	serverName(handle.FileName());
	server->ConstructL(serverName.Name());
	CleanupStack::Pop(server);
	return server;
	}

// Secure variants much simpler
// For EKA2, just an E32Main and a MainL()
LOCAL_C void MainL()
/**
 * Secure variant
 * Much simpler, uses the new Rendezvous() call to sync with the client
 */
	{
	// Leave the hooks in for platform security
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().DataCaging(RProcess::ESecureApiOn);
	CActiveScheduler*	sched = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CTestHtmlToCrtConverterSuite*	server = NULL;
	// Create the CTestServer derived server
	TRAPD( err,server = CTestHtmlToCrtConverterSuite::NewL() );
	if ( err == KErrNone )
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
 * @return - Standard Epoc error code on process exit
 * Secure variant only
 * Process entry point. Called by client using RProcess API
 */
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
	TRAP_IGNORE(MainL());
	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
    }

CTestStep* CTestHtmlToCrtConverterSuite::CreateTestStep(const TDesC& aStepName)
/**
 * @return - A CTestStep derived instance
 * Secure and non-secure variants
 * Implementation of CTestServer pure virtual
 */
	{
	CTestStep* testStep = NULL;
	// They are created "just in time" when the worker thread is created
	// More test steps can be added below
	if( aStepName == KTestHtmlToCrtConverterBufferStep )
		{
		testStep = new CTestHtmlToCrtConverterBufferStep();
		}
	else if ( aStepName == KTestHtmlToCrtConverterStep )
		{
		testStep = new CTestHtmlToCrtConverterStep();
		}
	return testStep;
	}

