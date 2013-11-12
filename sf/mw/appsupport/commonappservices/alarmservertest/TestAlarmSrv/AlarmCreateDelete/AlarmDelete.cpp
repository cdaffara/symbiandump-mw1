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

#include <e32base.h>
#include <asclisession.h>
#include <asshdalarm.h>


LOCAL_C void MainL()
	{
	// Connect to the Alarm Server
	RASCliSession asCliSession;
	asCliSession = RASCliSession();
	User::LeaveIfError(asCliSession.Connect());

	// Get list of alarms
	RArray<TAlarmId> alarmIds;
	asCliSession.GetAlarmIdListL(alarmIds);
	
	// Delete all alarms in the Alarm Server
	TInt i;
	for(i = 0; i < alarmIds.Count(); i++)
		{
		asCliSession.AlarmDelete(alarmIds[i]);
		}
	alarmIds.Reset();
	}

GLDEF_C int E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(!cleanup)
		{
		return KErrNoMemory;
		}
	TRAPD(err, MainL());
	delete cleanup;
	__UHEAP_MARKEND;
	return err;
	}

