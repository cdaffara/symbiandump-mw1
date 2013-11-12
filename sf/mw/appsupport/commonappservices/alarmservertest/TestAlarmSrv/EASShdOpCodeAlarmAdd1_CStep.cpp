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
#include "EASShdOpCodeAlarmAdd1_CStep.h"

// TO BE SAFE
IMPORT_C TInt StartDialogThread();

/** 
Constructor called from the respective Suite.cpp from their "AddTestStep" function
Each test step initialises it's own name
*/
CEASShdOpCodeAlarmAdd1Step::CEASShdOpCodeAlarmAdd1Step()
	{
	// The server name and IPC number is obtained and all messages are checked Sync
	iSrServerName		= _L("!AlarmServer");
	iSrMessageType		= 2;
	iSrMessageId		= 200;
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
TInt CEASShdOpCodeAlarmAdd1Step::Exec_SendReceiveL()
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
			// Create a TASShdAlarm object and set its properties
			TASShdAlarm alarm;
			alarm.Category() = KASCliCategoryClock;
			alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
			alarm.NextDueTime().HomeTime();
			
			// Set the alarm to go off after 20 seconds.
			// The alarm is intended to go off when EASShdOpCodeNotifyChange1_CStep, 
			// the next test step that will be executed, is waiting for an event to occur.
			TInt seconds = 20;
			alarm.NextDueTime() += TTimeIntervalSeconds(seconds);
			
			// Construct the TIpcArgs for SendReceive
			TPckg<TASShdAlarm> package(alarm);
			TDesC8 emptydata = KNullDesC8;
			TIpcArgs args(&package, emptydata.Length(), &emptydata);
			
			// Make the IPC call
			iResultSr =	SendReceive(iSrMessageId, args);
			
			// Create one more alarm as AlarmCancelAndDeque test step will 
			// remove one from the server
			TASShdAlarm alarm1;
			alarm1.Category() = KASCliCategoryClock;
			alarm1.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
			alarm1.NextDueTime().HomeTime();
			seconds = 50;
			alarm1.NextDueTime() += TTimeIntervalSeconds(seconds);
			TPckg<TASShdAlarm> package1(alarm1);
			TDesC8 emptydata1 = KNullDesC8;
			TIpcArgs args1(&package1, emptydata1.Length(), &emptydata1);
			iResultSr =	SendReceive(iSrMessageId, args1);
			
			// And one more just in case
			TASShdAlarm alarm2;
			alarm2.Category() = KASCliCategoryClock;
			alarm2.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
			alarm2.NextDueTime().HomeTime();
			seconds = 70;
			alarm2.NextDueTime() += TTimeIntervalSeconds(seconds);
			TPckg<TASShdAlarm> package2(alarm2);
			TDesC8 emptydata2 = KNullDesC8;
			TIpcArgs args2(&package2, emptydata2.Length(), &emptydata2);
			iResultSr =	SendReceive(iSrMessageId, args2);
			}
		}
	return iResultServer;
	}
