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

#include <ascliclientutils.h>

// System includes
#include <e32math.h>

// User includes
#include <asclidefinitions.h>

//
// ----> AlarmClientUtils (source)
//

/*
 * Panic the client
 */
void AlarmClientUtils::Panic(TAlarmClientPanic aPanic)
	{
	_LIT(KAlarmServerClientPanic, "ALMSRVCLI");
	User::Panic(KAlarmServerClientPanic, aPanic);
	}


/*
 * Panic the client, indicating some form of logic error or terminal
 * fault.
 */
void AlarmClientUtils::Fault(TAlarmClientFault aFault)
	{
	_LIT(KAlarmServerClientFault, "ALMSRVCLIFAULT");
	User::Panic(KAlarmServerClientFault, aFault);
	}


EXPORT_C TInt AlarmClientUtils::StartAlarmServer()
/** Starts the alarm server.

@return KErrNone if successful, KErrAlreadyExists if the server is already 
running, otherwise a system-wide error. */
	{
	//
	// EPOC is easy, we just create a new server process. Simultaneous launching
	// of two such processes should be detected when the second one attempts to
	// create the server object, failing with KErrAlreadyExists.
	//
	RProcess server;
	TInt ret = server.Create(ASCliDefinitions::ServerImageName(), KNullDesC, ASCliDefinitions::ServerUidType());

	// Did we manage to create the thread/process?
	if	(ret != KErrNone)
		return ret;

	// Wait to see if the thread/process died during construction
	TRequestStatus serverDiedRequestStatus;
	server.Rendezvous(serverDiedRequestStatus);
	
	// do we have to abort?
	if(serverDiedRequestStatus != KRequestPending)
		{
		server.Kill(0); // abort the sartup here
		}
	else
		{
		server.Resume(); // start server
		}

    User::WaitForRequest(serverDiedRequestStatus);
    // determine the reason for the server exit
    TInt exitReason =(server.ExitType()==EExitPanic) ? KErrGeneral : serverDiedRequestStatus.Int();
	server.Close();
	return exitReason;
	}
