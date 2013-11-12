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


#include "appfwk_test_utils_server.h"
#include "appfwk_test_utils.h"



/**
  Perform all server initialisation, in particular creation of the scheduler and server and then run the scheduler
*/
static void RunServerL()
	{
	// create and install the active scheduler we need
	CActiveScheduler* s = new(ELeave) CActiveScheduler;
	CleanupStack::PushL(s);
	CActiveScheduler::Install(s);

	// create the server (leave it on the cleanup stack)
	CSmlTestUtilsServer::NewLC();

	// naming the server thread after the server helps to debug panics
	User::LeaveIfError(RThread::RenameMe(KTestServerName));

	// Initialisation complete, now signal the client
	RProcess::Rendezvous(KErrNone);

	// Ready to run
	CActiveScheduler::Start();

	// Ready to exit. Cleanup the server and scheduler
	CleanupStack::PopAndDestroy(2);
	}
	

/**
  Main entry-point for the server thread
*/
static TInt RunSmlTestServer()
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	TInt r = KErrNoMemory;
	if (cleanup)
		{
		TRAP(r, RunServerL());
		delete cleanup;
		}
	__UHEAP_MARKEND;
	return r;
	}

TInt E32Main()
	{
	return RunSmlTestServer();
	}

