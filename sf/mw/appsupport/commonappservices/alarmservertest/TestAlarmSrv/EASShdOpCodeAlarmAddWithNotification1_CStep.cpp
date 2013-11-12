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
#include "EASShdOpCodeAlarmAddWithNotification1_CStep.h"

// TO BE SAFE
IMPORT_C TInt StartDialogThread();

/** 
Constructor called from the respective Suite.cpp from their "AddTestStep" function
Each test step initialises it's own name
*/
CEASShdOpCodeAlarmAddWithNotification1Step::CEASShdOpCodeAlarmAddWithNotification1Step()
	{
	// The server name and IPC number is obtained and all messages are checked Sync
	iSrServerName		= _L("!AlarmServer");
	iSrMessageType		= 2;
	iSrMessageId		= 201;
	iSrMessageMask		= 65536;
	iServerPanic		= _L("ALARMSERVER");

	TCapability cap[] = {ECapabilityWriteUserData, ECapability_Limit};
	
	TSecurityInfo info;
	info.Set(RProcess());
	TBool result = EFalse;
	
	TInt i;
	for (i = 0; cap[i] != ECapability_Limit; i++) 
		{
		if (!(info.iCaps.HasCapability(cap[i])))
			{
			result=ETrue;
			}
		}
		
	iExpectRejection = result;
	iStepCap = 65536;

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
TInt CEASShdOpCodeAlarmAddWithNotification1Step::Exec_SendReceiveL()
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
			// Build the parameters
			TASShdAlarm alarm;
			TRequestStatus status;
			TDesC8 data(KNullDesC8);
			TPtr8 package(NULL, 0, 0);
			TIpcArgs args(&package, data.Length(), &data);
			package.Set((TUint8*) &alarm, sizeof(TASShdAlarm), sizeof(TASShdAlarm));
			alarm.Id() = KNullAlarmId;
			
			// Make the IPC call
			SendReceive(iSrMessageId,args, status);
			
			// Wait for the notification
			User::WaitForRequest(status);
			
			// Get the result
			iResultSr =	status.Int();
			}
		}
	return iResultServer;
	}

