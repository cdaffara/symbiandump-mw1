
// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "timezoneserver.h"
#include <s32file.h>

CTZSrvScheduler::~CTZSrvScheduler()
	{
	delete iServer;
	}

CTZSrvScheduler* CTZSrvScheduler::NewL()
	{
	CTZSrvScheduler* self = new (ELeave) CTZSrvScheduler;
	CleanupStack::PushL(self);
	CActiveScheduler::Install(self);
	// create the server
	self->iServer=CTzServer::NewL();
	CleanupStack::Pop(); 	
	return self;
	}

void CTZSrvScheduler::Error(TInt aError) const
	{
	if (iServer)
		{
		iServer->Error(aError);
		}
	}

static void RunServerL()
	//
	// Thread top level
	// Perform all server initialisation, in particular creation of the
	// scheduler and server and then run the scheduler
	//
	{
	// create and install the active scheduler we need
	CTZSrvScheduler* s = CTZSrvScheduler::NewL();
	CleanupStack::PushL(s);

	//
	// naming the server thread after the server helps to debug panics
	User::LeaveIfError(RThread().RenameMe(KTimeZoneServerName()));

	
	//
	// Initialisation complete, now signal the client
	RProcess::Rendezvous(KErrNone);
	//
	// Ready to run
	CActiveScheduler::Start();
	//
	// Cleanup the scheduler
	CleanupStack::PopAndDestroy(s);
	}

GLDEF_C TInt E32Main()
//
// Timezone server entrypoint
//
	{
	__UHEAP_MARK;
	//
	CTrapCleanup* cleanup = CTrapCleanup::New();
	TInt r = KErrNoMemory;
	if (cleanup)
		{
		TRAP(r, RunServerL());
		delete cleanup;
		}
	//
	__UHEAP_MARKEND;
	return r;
	}
