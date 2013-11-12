/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*
*/




#include "t_oomclient.h"
#include "../inc/clientserver.h"

const TInt KServerDefaultMessageSlots = 1; //Number of message slots available per session.
const TInt KServerRetryCount = 2;

static TInt StartServer()
//
// Start the server process. Simultaneous launching
// of two such processes should be detected when the second one attempts to
// create the server object, failing with KErrAlreadyExists.
//
	{
	const TUidType serverUid(KNullUid,KNullUid,KServerUid3);
	RProcess server;
	TInt r=server.Create(KAllocServerImg,KNullDesC,serverUid);
	if (r!=KErrNone)
		return r;
	TRequestStatus stat;
	server.Rendezvous(stat);
	if (stat!=KRequestPending)
		server.Kill(0);		// abort startup
	else
		server.Resume();	// logon OK - start the server
	User::WaitForRequest(stat);		// wait for start or death
	// we can't use the 'exit reason' if the server panicked as this
	// is the panic 'reason' and may be '0' which cannot be distinguished
	// from KErrNone
	r=(server.ExitType()==EExitPanic) ? KErrGeneral : stat.Int();
	server.Close();
	return r;
	}

EXPORT_C TInt ROOMAllocSession::Connect()
//
// Connect to the server, attempting to start it if necessary
//
	{
	TInt retry=KServerRetryCount;
	for (;;)
		{
		TInt r=CreateSession(KAllocServerName,TVersion(0,0,0),KServerDefaultMessageSlots);
		if (r!=KErrNotFound && r!=KErrServerTerminated)
			return r;
		if (--retry==0)
			return r;
		r=StartServer();
		if (r!=KErrNone && r!=KErrAlreadyExists)
			return r;
		}
	}

EXPORT_C TInt ROOMAllocSession::Reset()
	{
	return SendReceive(EAllocServReset);
	}

EXPORT_C TInt ROOMAllocSession::StartAllocating()
	{
	return SendReceive(EAllocServStart);
	}

EXPORT_C TInt ROOMAllocSession::StopAllocating()
	{
	return SendReceive(EAllocServStop);
	}

EXPORT_C TInt ROOMAllocSession::MemoryLow(TUid aPlugin)
	{
	return SendReceive(EAllocServMemoryLow, TIpcArgs(aPlugin.iUid));
	}

EXPORT_C TInt ROOMAllocSession::MemoryGood(TUid aPlugin)
	{
	return SendReceive(EAllocServMemoryGood, TIpcArgs(aPlugin.iUid));
	}

EXPORT_C TInt ROOMAllocSession::Configure(TUid aPlugin, TUint aAllocRate, TUint aAllocInitial, TUint aAllocLimit)
	{
	return SendReceive(EAllocServConfig,TIpcArgs(aPlugin.iUid, aAllocRate, aAllocInitial, aAllocLimit));
	}
