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

#include <asshdalarm.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include "ASShdAlarm_internal.h"
#endif

// NOTE: Remember to change this version whenever the data format changes!
const TUint16 KAlarmExternalizedVersion = 2;
#ifndef SYMBIAN_SYSTEM_STATE_MANAGEMENT
const TUint16 KAlarmExternalizedSkipCount = 0; // Number of bytes to be skipped in data stream
#endif

/**
Default constructor. 

Initializes all member data to default values.
*/
EXPORT_C TASShdAlarm::TASShdAlarm()
	{
	Reset();
	}

/**
Internalizes the alarm's data from a read stream.

@param aStream The stream to read from.
*/
EXPORT_C void TASShdAlarm::InternalizeL(RReadStream& aStream)
	{
	// Read and ignore version (until its needed)
	aStream.ReadUint16L();
	iFlags.SetValue(aStream.ReadInt8L());
	// Session no longer valid!
	iFlags.Clear(EASShdAlarmFlagsHasOwningSession);
	iCharacteristics.SetValue(aStream.ReadInt8L());
	// Session alarms should be imported as orphaned
	// because there is no valid TRequestStatus to notify
	if (iCharacteristics.IsSet(EAlarmCharacteristicsSessionSpecific))
		{
		iCharacteristics.Clear(EAlarmCharacteristicsSessionSpecific);
		iFlags.Set(EASShdAlarmFlagsHasBecomeOrphaned);
		}
	iAlarmId = aStream.ReadInt32L();
	iStatus = static_cast<TAlarmStatus>(aStream.ReadInt8L());
	iState = static_cast<TAlarmState>(aStream.ReadInt8L());
	iDayOrTimed = static_cast<TAlarmDayOrTimed>(aStream.ReadInt8L());
	iRepeatDefinition = static_cast<TAlarmRepeatDefinition>(aStream.ReadInt8L());
	aStream >> iCategory;
	TInt64 tempInt64;
	aStream >> tempInt64;
	iNextDueTime = TTime(tempInt64);
	aStream >> tempInt64;
	iOriginalExpiryTime = TTime(tempInt64);
	aStream >> iMessage;
	aStream >> iSoundName;
	iClientFlags.SetValue(aStream.ReadInt16L());
	iClientData1 = aStream.ReadInt32L();
	iClientData2 = aStream.ReadInt32L();
	//Only read the SID in the secured platform
	iTASShdAlarmSID = aStream.ReadUint32L();
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	iFlags2.SetValue(aStream.ReadInt16L());
#else
	// Read and ignore the skip count until its needed. When new data is written
	// to the end of the alarm, be sure to use the version information as a means
	// of establishing exactly how much data can be read. This enables the alarm
	// server to remain data compatible going forwards...
	aStream.ReadUint16L();
#endif
	}

/**
Externalizes the alarm's member data to a write stream.

@param aStream The stream to write to.
*/
EXPORT_C void TASShdAlarm::ExternalizeL(RWriteStream& aStream) const
	{
	// Write version
	aStream.WriteUint16L(KAlarmExternalizedVersion);
	aStream.WriteInt8L(iFlags.Value());
	aStream.WriteInt8L(iCharacteristics.Value());
	aStream.WriteInt32L(iAlarmId);
	aStream.WriteInt8L(static_cast<TInt8>(iStatus));
	aStream.WriteInt8L(static_cast<TInt8>(iState));
	aStream.WriteInt8L(static_cast<TInt8>(iDayOrTimed));
	aStream.WriteInt8L(static_cast<TInt8>(iRepeatDefinition));
	aStream << iCategory;
	aStream << iNextDueTime.Int64();
	aStream << iOriginalExpiryTime.Int64();
	aStream << iMessage;
	aStream << iSoundName;
	aStream.WriteInt16L(iClientFlags.Value());
	aStream.WriteInt32L(iClientData1);
	aStream.WriteInt32L(iClientData2);
	//Only store the SID in the secured platform
	aStream.WriteUint32L(iTASShdAlarmSID);
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	aStream.WriteInt16L(iFlags2.Value());
#else
	// Write skip count
	aStream.WriteUint16L(KAlarmExternalizedSkipCount);
#endif
	}

/**
Resets the alarm back to a default, uninitialized state.
*/
EXPORT_C void TASShdAlarm::Reset()
	{
	iFlags = 0,
	iCharacteristics = 0;
	iAlarmId = KNullAlarmId;
	iStatus = EAlarmStatusDisabled;
	iState = EAlarmStateInPreparation;
	iDayOrTimed = EASShdAlarmTypeTimed;
	iRepeatDefinition = EAlarmRepeatDefintionRepeatOnce;
	iCategory = KNullUid;
	iNextDueTime = Time::NullTTime();
	iOriginalExpiryTime = Time::NullTTime();
	iMessage = KNullDesC;
	iSoundName = KNullDesC;
	iClientFlags.ClearAll();
	iClientData1 = 0;
	iClientData2 = 0;
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	iFlags2 = 0;
#endif
#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
	iFlags2 = 0;
	// By default, an alarm with a repeat definition of EAlarmRepeatDefinition-
	// DailyOnGivenDays is active on all days.
	SetAlarmDays(EAlarmDayMonday | EAlarmDayTuesday | EAlarmDayWednesday |
		EAlarmDayThursday | EAlarmDayFriday | EAlarmDaySaturday |
		EAlarmDaySunday);
#endif
	}	

/**
Using this API to set the next due time of an alarm will make it a fixed alarm, that
is going to expire at the given UTC time. The alarm will not be rescheduled when the UTC offset is changed.

@param aUtcTime The due time of the alarm should be given in UTC.
*/
EXPORT_C void TASShdAlarm::SetUtcNextDueTime(TTime aUtcTime)
	{
	iNextDueTime = aUtcTime;
	iCharacteristics.Set(EAlarmCharacteristicsIsFixed);	
	}

/**
Using this API to tell the alarmserver not to notify the Agenda alarms if
its due time is in the past after system time has been changed.
*/
EXPORT_C void TASShdAlarm::SetDeQueueIfDueTimeInPast()
	{
	iCharacteristics.Set(EAlarmCharacteristicsDeQueueIfDueTimeInPast);	
	}

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
EXPORT_C void TASShdAlarm::SetWakeup(TBool aEnabled)
	{
	if (aEnabled)
		{
		iFlags2.Set(EASShdAlarmFlag2Wakeup);
		}
	else
		{
		iFlags2.Clear(EASShdAlarmFlag2Wakeup);
		}
	}

EXPORT_C TBool TASShdAlarm::IsWakeup() const
	{
	return iFlags2.IsSet(EASShdAlarmFlag2Wakeup);
	}
#endif

#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
/**
Sets which days of the week an alarm with a repeat definition of
EAlarmRepeatDefintionRepeatDailyOnGivenDays activates on.  By default, an alarm
with a repeat definition of EAlarmRepeatDefinitionRepeatDailyOnGivenDays will be
active on all days of the week.

@prototype

@param aAlarmDays
	The days of the week that the alarm activates on.  Days are combined using
	the bitwise OR operator e.g. EAlarmDayMonday|EAlarmDayTuesday.

@return
	KErrNone if successful, KErrArgument if aAlarmDays is an invalid value.
*/
EXPORT_C TInt TASShdAlarm::SetAlarmDays(TUint8 aAlarmDays)
	{
	TUint8 alarmDaysMask = ~(EAlarmDayMonday | EAlarmDayTuesday |
		EAlarmDayWednesday | EAlarmDayThursday | EAlarmDayFriday |
		EAlarmDaySaturday | EAlarmDaySunday);
	
	if ((aAlarmDays == 0) || ((aAlarmDays & alarmDaysMask) != 0))
		{
		return KErrArgument;
		}
		
	iFlags2.Assign(EASShdAlarmFlag2AlarmDayMonday,
		aAlarmDays & EAlarmDayMonday);

	iFlags2.Assign(EASShdAlarmFlag2AlarmDayTuesday,
		aAlarmDays & EAlarmDayTuesday);
	
	iFlags2.Assign(EASShdAlarmFlag2AlarmDayWednesday,
		aAlarmDays & EAlarmDayWednesday);

	iFlags2.Assign(EASShdAlarmFlag2AlarmDayThursday,
		aAlarmDays & EAlarmDayThursday);

	iFlags2.Assign(EASShdAlarmFlag2AlarmDayFriday,
		aAlarmDays & EAlarmDayFriday);

	iFlags2.Assign(EASShdAlarmFlag2AlarmDaySaturday,
		aAlarmDays & EAlarmDaySaturday);

	iFlags2.Assign(EASShdAlarmFlag2AlarmDaySunday,
		aAlarmDays & EAlarmDaySunday);

	return KErrNone;
	}

/**
Returns the days of the week that the alarm activates on.  This value is only
applicable if the alarm’s repeat definition is
EAlarmRepeatDefinitionRepeatDailyOnGivenDays.  By default, an alarm with a
repeat definition of EAlarmRepeatDefinitionRepeatDailyOn-GivenDays will be
active on all days of the week.

@prototype

@return
	The days of the week the alarm activates on.
*/
EXPORT_C TUint8 TASShdAlarm::AlarmDays() const
	{
	TUint8 alarmDays = 0;
	
	if (iFlags2.IsSet(EASShdAlarmFlag2AlarmDayMonday))
		{
		alarmDays |= EAlarmDayMonday;
		}
	if (iFlags2.IsSet(EASShdAlarmFlag2AlarmDayTuesday))
		{
		alarmDays |= EAlarmDayTuesday;
		}
	if (iFlags2.IsSet(EASShdAlarmFlag2AlarmDayWednesday))
		{
		alarmDays |= EAlarmDayWednesday;
		}
	if (iFlags2.IsSet(EASShdAlarmFlag2AlarmDayThursday))
		{
		alarmDays |= EAlarmDayThursday;
		}
	if (iFlags2.IsSet(EASShdAlarmFlag2AlarmDayFriday))
		{
		alarmDays |= EAlarmDayFriday;
		}
	if (iFlags2.IsSet(EASShdAlarmFlag2AlarmDaySaturday))
		{
		alarmDays |= EAlarmDaySaturday;
		}
	if (iFlags2.IsSet(EASShdAlarmFlag2AlarmDaySunday))
		{
		alarmDays |= EAlarmDaySunday;
		}
	
	return alarmDays;
	}

/**
Sets whether or not the alarm is continuous.

@prototype

@param aContinuous
	ETrue if the alarm is continuous, EFalse if the alarm is not continuous.
*/
EXPORT_C void TASShdAlarm::SetContinuous(TBool aContinuous)
	{
	iFlags2.Assign(EASShdAlarmFlag2Continuous, aContinuous);
	}

/**
Returns the continuous state for the alarm.

@prototype

@return
	ETrue if the alarm is continuous, EFalse if the alarm is not continuous.
*/
EXPORT_C TBool TASShdAlarm::Continuous()
	{
	return iFlags2.IsSet(EASShdAlarmFlag2Continuous);
	}

#endif
	

/** Tests whether the alarm has any associated data. 
	
@return True if the alarm has associated data. */
EXPORT_C TBool TASShdAlarm::HasAssociatedData() const

	{
	return iFlags.IsSet(EASShdAlarmFlagsHasAssociatedData);
	}


/** Tests whether the alarm has an active owning session.

@return True if the alarm has an active owning session. */
EXPORT_C TBool TASShdAlarm::HasOwningSession() const

	{
	return iFlags.IsSet(EASShdAlarmFlagsHasOwningSession);
	}

EXPORT_C TBool TASShdAlarm::HasBecomeOrphaned() const
/** Tests whether the alarm is orphaned. 

An alarm is ophaned if it used to have an owning session, but no longer does. 
If an alarm is owned by a session, it is removed from the queue when the session 
disconnects. However, orphaned alarms stay in the queue.
	
@return True if the alarm has becomed orphaned. */
	{
	return iFlags.IsSet(EASShdAlarmFlagsHasBecomeOrphaned);
	}
	

