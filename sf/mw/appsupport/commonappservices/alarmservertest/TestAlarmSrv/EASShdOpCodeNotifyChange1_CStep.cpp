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
#include "EASShdOpCodeNotifyChange1_CStep.h"

// TO BE SAFE
IMPORT_C TInt StartDialogThread();

/** 
Constructor called from the respective Suite.cpp from their "AddTestStep" function
Each test step initialises it's own name
*/
CEASShdOpCodeNotifyChange1Step::CEASShdOpCodeNotifyChange1Step()
	{
	// The server name and IPC number is obtained and all messages are checked Sync
	iSrServerName		= _L("!AlarmServer");
	iSrMessageType		= 2;
	iSrMessageId		= 19;
	iSrMessageMask		= 0;
	iServerPanic		= _L("ALARMSERVER");

	TCapability cap[] = {ECapability_None, ECapability_Limit};
	
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
	iStepCap = 0;

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
TInt CEASShdOpCodeNotifyChange1Step::Exec_SendReceiveL()
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
			// Modifications made to tool generated code
			// Construct the TIpcArgs with necessary info so as to
			// make a valid NotifyChange call
			TAlarmId alarmId;
			TRequestStatus requestStatus;
			TPtr8 alarmIdPointer(NULL, 0, 0);
			alarmIdPointer.Set((TUint8*) &alarmId, sizeof(TAlarmId), sizeof(TAlarmId));
			TIpcArgs args(&alarmIdPointer, EFalse);
				
			// Make the IPC call
			SendReceive(iSrMessageId,args, requestStatus);
				
			// Wait till there is an event. Releases when the alarm
			// created by CEASShdOpCodeAlarmAdd1Step goes off.
			User::WaitForRequest(requestStatus);
				
			// Get the result
			iResultSr = requestStatus.Int();
			}
		}
	return iResultServer;
	}
