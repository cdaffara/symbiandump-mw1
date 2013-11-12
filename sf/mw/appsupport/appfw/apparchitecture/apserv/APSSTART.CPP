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
// The main startup of the AppArc server
// 
// apsstart.cpp
//
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#if !defined(__APA_INTERNAL_H__)
#include "apainternal.h"
#endif
#include "apaidpartner.h"
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS
#include "apsserv.h"
#include <fbs.h>

#include "APASVST.H"
#include "APAFLREC.H"

#include <eikdll.h>

NONSHARABLE_CLASS(CSvActiveScheduler) : public CActiveScheduler
	{
public:
	static void NewLC();
	virtual void Error(TInt anError) const;
	};

GLDEF_C void CSvActiveScheduler::NewLC()
//
// Create and install the active scheduler.
//
	{
	CSvActiveScheduler* pA=new(ELeave) CSvActiveScheduler;
	CleanupStack::PushL(pA);
	CActiveScheduler::Install(pA);
	}

GLDEF_C void CSvActiveScheduler::Error(TInt) const
//
// Called if any Run() method leaves.
//
	{
	}

static void CleanupRFbsSession(TAny*)
	{
	RFbsSession::Disconnect();
	}

static void RunServerL()
//
// Perform all server initialisation, in particular creation of the
// scheduler and server and then run the scheduler
//
	{
	// create and install the active scheduler we need
	CSvActiveScheduler::NewLC();

	// create a RFbsSession
	User::LeaveIfError(RFbsSession::Connect());
	CleanupStack::PushL(TCleanupItem(CleanupRFbsSession, NULL));

	// create the server (leave it on the cleanup stack)
	CApaAppArcServer* appArcServer = CApaAppArcServer::NewL();
	CleanupStack::PushL(appArcServer);

	// Initialisation complete, now signal the client
	RProcess::Rendezvous(KErrNone);

	// Ready to run
	CActiveScheduler::Start();

	// Cleanup the server, RFbsSession and scheduler
	CleanupStack::PopAndDestroy(3);
	}

static TInt RunServer()
//
// Main entry-point for the server thread
//
	{
	__UHEAP_MARK;
	TInt r;
	// naming the server thread after the server helps to debug panics
	r=RThread::RenameMe(NameApaServServerThread());
	//
	if (r == KErrNone)
		{
		CTrapCleanup* cleanup=CTrapCleanup::New();
		r=KErrNoMemory;
		if (cleanup)
			{
			TRAP(r,RunServerL());
			REComSession::FinalClose();
			delete cleanup;
			}
		}
	//
	__UHEAP_MARKEND;
	return r;
	}

/**
ApaServThreadStart

@internalTechnology
@released
*/
EXPORT_C TInt ApaServThreadStart(TAny* /*aUnused*/)
//
// thread entry-point function.
//
	{
	return RunServer();
	}
