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
// This file contains the functions to construct the Test Step and the 
// Exec_SendReceiveL() function that connects to the server and makes IPC calls.
// 
//

// Test Step header
#include "EASShdOpCodeSetAlarmDayOrTimed_DifferentSID1_CStep.h"

// TO BE SAFE
IMPORT_C TInt StartDialogThread();

/** 
Constructor called from the respective Suite.cpp from their "AddTestStep" function
Each test step initialises it's own name
*/
CEASShdOpCodeSetAlarmDayOrTimed_DifferentSID1Step::CEASShdOpCodeSetAlarmDayOrTimed_DifferentSID1Step()
	{
	// The server name and IPC number is obtained and all messages are checked Sync
	iSrServerName		= _L("!AlarmServer");
	iSrMessageType		= 2;
	iSrMessageId		= 106;
	iSrMessageMask		= 32;
	iServerPanic		= _L("ALARMSERVER");

	// Expected to be rejected as this test step will test for the condition: 
	// none capapbility and different SID, and so has to fail
	iExpectRejection = ETrue;
		
	iStepCap = 32;

	// Get a unique thread name
	iSrChildThread.Format(_L("ChildThread_%S_%d"),&iSrServerName,iSrMessageId);
	}

/**
This function is called by the Child Thread
1.	Create a session with the server
2.	Test an SendReceive call
3.	Informs the main thread about the status of the call using
	a.	iSessionCreated, if the a connection is established
	b.	iResultServer, holds the return value for connection 
	c.	iResultSr, the return value of SendReceive	call
*/
TInt CEASShdOpCodeSetAlarmDayOrTimed_DifferentSID1Step::Exec_SendReceiveL()
	{
	iResultServer = CreateSession(iSrServerName,Version(),KAlarmServerAsynchronousSlotCount);

	if (iResultServer!=KErrNone)
		{
		iResultServer=StartServer();
		if (iResultServer!=KErrNone)
			{
			return(iResultServer);
			}
		iResultServer = CreateSession(iSrServerName,Version(),KAlarmServerAsynchronousSlotCount);
		}
	if(iResultServer == 0)
		{
		iSessionCreated = ETrue;
		if(iSrMessageId >= 0)
			{
			// Get the alarm id
			TAlarmId alarmId;
			alarmId = CCapabilityTestStep::GetAlarmId();
			
			if(alarmId >= 0)
				{
				// If valid alarm id, perform IPC call
				TAlarmDayOrTimed dayOrTimed = EASShdAlarmTypeDay;
				TIpcArgs args(alarmId, dayOrTimed);
				iResultSr =	SendReceive(iSrMessageId, args);
				}
			else
				{
				// else leave
				User::Leave(alarmId);
				}	
			}
		}
	return iResultServer;
	}

