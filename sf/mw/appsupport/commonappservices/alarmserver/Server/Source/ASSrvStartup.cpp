// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ASSrvStartup.h"
#include "ASSrvServer.h"
#include "ASSrvStaticUtils.h"

//
// ----> ASSrvStartup (source)
//

//*************************************************************************************
TInt ASSrvStartup::RunServer()
	{
	__UHEAP_MARK;
	//
	CTrapCleanup* cleanup = CTrapCleanup::New();
	TInt ret = KErrNoMemory;
	if	(cleanup)
		{
		TRAP(ret, ASSrvStartup::RunServerL());
		delete cleanup;
		}
	//
	__UHEAP_MARKEND;
	return ret;
	}

//*************************************************************************************
void ASSrvStartup::RunServerL()
	{
	// Create and install the active scheduler we need
	CActiveScheduler* scheduler = new(ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	// Create server
	CASSrvServer::NewLC();

	// Initialisation complete, now signal the client
	User::LeaveIfError(RThread().RenameMe(ASSrvStaticUtils::ASName()));
	RProcess::Rendezvous(KErrNone);

	// Ready to run
	CActiveScheduler::Start();

	// Cleanup the server and scheduler
	CleanupStack::PopAndDestroy(2, scheduler);
	}

//*************************************************************************************
TInt E32Main()
	{
	return ASSrvStartup::RunServer();
	}
