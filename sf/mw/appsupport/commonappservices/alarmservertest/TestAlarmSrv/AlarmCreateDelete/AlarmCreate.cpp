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

LOCAL_D RASCliSession asCliSession;

LOCAL_C TBool AlarmExistsL()
	{
	// Get security info of this process.
	TSecurityInfo info;
	info.Set(RProcess());
	
	// Get list of alarms
	RArray<TAlarmId> alarmIds;
	//__UHEAP_MARK;
	asCliSession.GetAlarmIdListL(alarmIds);
	//__UHEAP_MARKEND;
	
	TInt i;
	for(i = 0; i < alarmIds.Count(); i++)
		{
		TASShdAlarm alarm;
		
		TInt err = asCliSession.GetAlarmDetails(alarmIds[i], alarm);
		
		TSecureId sidOfAlarm = alarm.GetSid();
		if(info.iSecureId == sidOfAlarm)
			{
			// Already an alarm with the same SID of this process exists.
			// No need to create another
			alarmIds.Reset();
			return ETrue;	
			}
		}
		alarmIds.Reset();
		return EFalse;
	}
	
LOCAL_C void MainL()
	{
	asCliSession = RASCliSession();
	User::LeaveIfError(asCliSession.Connect());

	if( !AlarmExistsL() )
		{
		TASShdAlarm alarm;
		alarm.Category() = KASCliCategoryClock;
		alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
		alarm.NextDueTime().HomeTime();
			
		TInt minutes = 60;
		alarm.NextDueTime() += TTimeIntervalMinutes(minutes);
			
		asCliSession.AlarmAdd(alarm);
		}
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

