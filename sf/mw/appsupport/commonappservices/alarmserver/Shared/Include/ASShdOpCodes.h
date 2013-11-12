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

#ifndef __ASSHDOPCODES_H__
#define __ASSHDOPCODES_H__

// System includes
#include <e32std.h>

// Constants
const TInt KAlarmServerMajorVN = 2;
const TInt KAlarmServerMinorVN = 0;
const TInt KAlarmServerBuildVN = 0;
//
const TInt KAlarmServerAsynchronousSlotCount	= 5;
//
const TUid KAlarmServerUid = { 0x101F5027 };

// Literal constants


_LIT(KAlarmServerName, "!AlarmServer");

_LIT(KAlarmServerImageName, "AlarmServer");

#define KCapabilityNone 0
#define KCapabilityCustomCheckData 100
#define KCapabilityWriteUserData 200
#define KCapabilityWriteDeviceData 300

// Enumerations
/**
 * The op-codes used to communicate between the
 * alarm client and alarm server.
 */
enum TASShdOpCode
	{
	//No Capability requirement
	EASShdOpCodeFirst = KCapabilityNone,				
	EASShdOpCodeGetAlarmCategory,					
	EASShdOpCodeGetAlarmOwner,						
	EASShdOpCodeGetAlarmStatus,						
	EASShdOpCodeGetAlarmDayOrTimed,					
	EASShdOpCodeGetAlarmCharacteristics,			
	EASShdOpCodeAlarmDataSize,					
	EASShdOpCodeGetAlarmCountForCategory,			
	EASShdOpCodeGetAvailableCategoryList,			
	EASShdOpCodeGetAlarmIdListForCategory,			
	EASShdOpCodeAlarmCountByState,					
	EASShdOpCodeGetAlarmIdListByState,				
	EASShdOpCodeGetAlarmIdList,					
	EASShdOpCodeGetNextDueAlarmId,					
	EASShdOpCodeNumberOfAlarmsActiveInQueue,		
	EASShdOpCodeGetAlarmSoundState,					
	EASShdOpCodeGetAlarmSoundsSilentUntil,				
	EASShdOpCodeAlarmSoundsTemporarilySilenced,			
	EASShdOpCodeGetAlarmPlayIntervals,				
	EASShdOpCodeNotifyChange,					
	EASShdOpCodeNotifyChangeCancel,					
	EASShdOpCodeFetchTransferBuffer,				
	EASShdOpCodeAlarmCount,							
	EASShdOpCodeDbgShutDownServer,					
	EASShdOpCodeDbgFailAlloc,						
	EASShdOpCodeDbgPreventUserNotify,				
	EASShdOpCodeDbgSnoozeAlarm,					
	EASShdOpCodeFlushServer,
#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
	EASShdOpCodeGetAlarmDays,
	EASShdOpCodeGetContinuous,
#endif
	EASShdOpCodeDbgSetEnvironmentChangesHandling,

	//No capability if client owns the alarm 
	//otherwise ReadUserData 	
	EASShdOpCodeGetAlarmDetails = KCapabilityCustomCheckData,	
	EASShdOpCodeGetAlarmData,						
	EASShdOpCodeAlarmAtIndex,						
	
	// No capability if client owns the alarm otherwise WriteDeviceData.
	EASShdOpCodeAlarmNotificationCancelAndDeQueue,	
	EASShdOpCodeAlarmDelete,						
	EASShdOpCodeSetAlarmStatus,						
	EASShdOpCodeSetAlarmDayOrTimed,					
	EASShdOpCodeSetAlarmCharacteristics,
	EASShdOpCodeSetClientData,						
	EASShdOpCodeAlarmDataAttach,					
	EASShdOpCodeAlarmDataDetach,
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	EASShdOpCodeSetWakeup,
#endif
#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
	EASShdOpCodeSetAlarmDays,
	EASShdOpCodeSetContinuous,
#endif
	
	// WriteUserData 
	EASShdOpCodeAlarmAdd = KCapabilityWriteUserData,	
	EASShdOpCodeAlarmAddWithNotification,			
	EASShdOpCodeAllocateAndReturnAlarmId,			
	EASShdOpCodeOrphanAlarm, 						

	// WriteDeviceData 
	EASShdOpCodeSetAlarmSoundState = KCapabilityWriteDeviceData,
	EASShdOpCodeSetAlarmSoundsSilentUntil,			
	EASShdOpCodeSetAlarmSoundsSilentFor,			
	EASShdOpCodeCancelAlarmSilence,					
	EASShdOpCodeSetAlarmPlayIntervals,				
	EASShdOpCodeSetAlarmStatusByCategory,			
	EASShdOpCodeAlarmDeleteAllByCategory,			
	EASShdOpCodeAlarmDeleteByCategory,			
	EASShdOpCodeAlarmDeleteByCalendarFile,
	EASShdOpCodeSetAlarmStatusForCalendarFile,
	// End Marker no Capability
	EASShdOpCodeLast								
	};

#endif
