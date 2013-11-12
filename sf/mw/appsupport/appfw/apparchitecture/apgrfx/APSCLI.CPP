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
// Client access to the AppArc server
// 
// apscli.cpp
//

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#if !defined(__APA_INTERNAL_H__)
#include "apainternal.h"
#endif
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS

#include <e32base.h>
#include <e32svr.h>
#include <e32math.h>
#include <apasvst.h>
#include <e32uid.h>

_LIT(KNameApaImage,"APSEXE");
const TUid KServerUid3  = { 0x10003A3F };

/**
StartupApaServerProcess
Start the server in a process. Simultaneous launching of two such processes 
should be detected when the second one attempts to create the server object, 
failing with KErrAlreadyExists.

@publishedPartner 
@released
*/
EXPORT_C TInt StartupApaServerProcess()
	{
	const TUidType uidType(KNullUid, KNullUid, KServerUid3);
	TInt r=KErrNone;
	
	// On ARM or any EKA2 target, the server is an EXE.
	// Create a process from it.
	RProcess server;
	r=server.Create(KNameApaImage,KNullDesC,uidType);
	
	if (r!=KErrNone)
		return r;
	TRequestStatus stat;
	server.Rendezvous(stat);
	if (stat!=KRequestPending)
		server.Kill(0);		// abort startup
	else
		server.Resume();	// logon OK - start the server
	User::WaitForRequest(stat);		// wait for start or death
	server.Close();
	return stat.Int();
	}

