// Copyright (c) 1999-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include <asclisession.h>

// System includes
#include <s32mem.h>

// User includes
#include <asshdalarm.h>
#include "ASShdPragma.h"
#include "ASShdOpCodes.h"
//
#include <asclisoundplay.h>
#include <ascliclientutils.h>
#include <asclidefinitions.h>

// Type definitions

// Constants
const TInt KNumberOfServerStartupAttempts = 2;

// Enumerations


//
// ----> RASCliSession (source)
//

/**

Default constructor.

Creates an instance of the RASCliSession class, setting its pointers to null.

*/
EXPORT_C RASCliSession::RASCliSession()
:	iPackage(NULL, 0, 0), iAlarmIdPointer(NULL, 0, 0)
	{
	}


/**

Connects the client process to the alarm server, starting the server if it 
is not already running.
@capability None
@return KErrNone if successful, otherwise one of the system-wide errors.

*/
EXPORT_C TInt RASCliSession::Connect()
	{
	TInt startupAttempts = KNumberOfServerStartupAttempts;
	for(;;)
		{
		TInt ret = CreateSession(ASCliDefinitions::ServerAndThreadName(), ASCliDefinitions::Version(), KAlarmServerAsynchronousSlotCount);

		if	(ret != KErrNotFound && ret != KErrServerTerminated)
			{
			return ret;
			}
		
		if	(startupAttempts-- == 0)
			{
			return ret;
			}
		
		ret = AlarmClientUtils::StartAlarmServer();
		if	(ret != KErrNone && ret != KErrAlreadyExists)
			{
			return ret;
			}
		}
	}


/** Provides the version number of the alarm server.
@capability None
@return The version number. */
EXPORT_C TVersion RASCliSession::Version() const
	{
	return ASCliDefinitions::Version();
	}


//
//
//


/** Validates an alarm object, allocates an unique identifier to it, and adds the 
object synchronously to the alarm server's queue.

@capability WriteUserData 
@param aAlarm The alarm object to add. On return, contains a unique identifier 
that the client can use to identify the alarm.
@return KErrNone if successful, otherwise one of the system-wide error codes. */
EXPORT_C TInt RASCliSession::AlarmAdd(TASShdAlarm& aAlarm) const
	{
	return DoAlarmAdd(aAlarm, KNullDesC8);
	}

/**
@capability WriteUserData 
*/
TInt RASCliSession::DoAlarmAdd(TASShdAlarm& aAlarm, const TDesC8& aData) const
	{
	TPckg<TASShdAlarm> package(aAlarm);
	TIpcArgs args(&package, aData.Length(), &aData);

	// Send message to server.
	return SendReceive(EASShdOpCodeAlarmAdd, args);
	}


/** Validates an alarm object, allocates an unique identifier to it, and adds the 
object with data synchronously to the alarm server's queue.

@capability WriteUserData 
@param aAlarm The alarm object to add. On return, contains a unique identifier 
that the client can use to identify the alarm.
@param aData Client-specific data to associate with the alarm. 
@return KErrNone if successful, otherwise one of the system-wide error codes.*/
EXPORT_C TInt RASCliSession::AlarmAdd(TASShdAlarm& aAlarm, const TDesC8& aData) const
	{
	return DoAlarmAdd(aAlarm, aData);
	}


/** Makes an asynchronous request to add an alarm object to the alarm server's 
queue.

It also allocates a unique identifier of type TAlarmId to the alarm object and 
adds the alarm object to the alarm server's queue.

When the alarm expires, or is cancelled, the alarm server notifies the client 
by completing the outstanding request.

@capability WriteUserData 
@param aStatus On completion of the request, this object holds the completion 
code.
@param aAlarm The alarm to add. On return, contains the unique identifier. KNullAlarmId indicates failure.*/
EXPORT_C void RASCliSession::AlarmAddWithNotification(TRequestStatus& aStatus, TASShdAlarm& aAlarm)
	{
	DoAlarmAddWithNotification(aStatus, aAlarm, KNullDesC8);
	}

/**
@capability WriteUserData
*/	
void RASCliSession::DoAlarmAddWithNotification(TRequestStatus& aStatus, TASShdAlarm& aAlarm, const TDesC8& aData)
	{
	// Set the session specific flag
	aAlarm.Characteristics().Set(EAlarmCharacteristicsSessionSpecific);

	// add the alarm and wait for notification. 
	TIpcArgs args(&iPackage, aData.Length(), &aData);
	//
	iPackage.Set((TUint8*) &aAlarm, sizeof(TASShdAlarm), sizeof(TASShdAlarm));

	// Unfortunately there is no way to send a request flag synchronously
	// The basic idea behind the asynchronous sendReceive is that we can attach a 
	// status flag to get informed whenever the server has accomplished the given task,
	// specifyed by the OpCode. However, in this case the flag  is used when an alarm
	// expires and not when the alarm server is ready registrating the alarm.
	// Therefore, we have to make a synchronous call afterwards to synchronise the client and server
	aAlarm.Id() = KNullAlarmId;
	SendReceive(EASShdOpCodeAlarmAddWithNotification, args, aStatus);

	// The synchronous call to synchronise the client and server.
	SendReceive(EASShdOpCodeFlushServer);
	}


/** 
Makes an asynchronous request to add an alarm object 
with the agenda entry information attached to the alarm server's 
queue.

It also allocates a unique identifier of type TAlarmId to the alarm object and 
adds the alarm object to the alarm server's queue.

When the alarm expires, or is cancelled, the alarm server notifies the client 
by completing the outstanding request.

@capability WriteUserData 
@param aStatus On completion of the request, this object holds the completion 
code.
@param aAlarm The alarm to add. On return, contains the unique identifier. KNullAlarmId indicates failure.
@param aData Client-specific data to associate with the alarm.
*/
EXPORT_C void RASCliSession::AlarmAddWithNotification(TRequestStatus& aStatus, TASShdAlarm& aAlarm, const TDesC8& aData)
	{
	DoAlarmAddWithNotification(aStatus, aAlarm, aData);
	}


/** Cancels an outstanding notification for an alarm and removes it from the queue.
@capability Note If the user does not have WriteDeviceData capability then we need to check
that the alarm belongs to the user SID (The SID is stored as part of the alarm)
@param aAlarmId The unique identifier of the alarm to be cancelled. */
EXPORT_C void RASCliSession::AlarmNotificationCancelAndDequeue(TAlarmId aAlarmId) const
	{
	TIpcArgs args(aAlarmId);
	// Backup/Restore locking the file or the Server dying are not programming
	// errors - any other error panics the client. 	
	const TInt error = SendReceive(EASShdOpCodeAlarmNotificationCancelAndDeQueue, args);
	__ASSERT_ALWAYS(error == KErrNone || error == KErrServerTerminated || error == KErrLocked, AlarmClientUtils::Panic(AlarmClientUtils::EAlarmClientPanicNotificationCancel));
	}


/** Retrieves information about a specified alarm.

@capability Note If the user does not have ReadUserData capability then we need to check 
that the alarm belongs to the user SID (The SID is stored as part of the alarm)
@param aAlarmId The unique identifier of the alarm under query.
@param aAlarm On return, contains details of the alarm sought.
@return KErrNone if successful, KErrCouldNotConnect if the time zone server is
not available, otherwise one of the other system-wide error codes. */
EXPORT_C TInt RASCliSession::GetAlarmDetails(TAlarmId aAlarmId, TASShdAlarm& aAlarm) const
	{
	TPckg<TASShdAlarm> package(aAlarm);
	TIpcArgs args(aAlarmId,&package);
	return SendReceive(EASShdOpCodeGetAlarmDetails, args);
	}



/** Deletes an alarm from the alarm server.
@capability Note If the user does not have WriteDeviceData capability then we need to check 
that the alarm belongs to the user SID (The SID is stored as part of the alarm)
@param aAlarmId The unique identifier of the alarm to be deleted.
@return KErrNone if successful, KErrNotFound if the specified alarm does not 
exist, KErrAccessDenied if the alarm has an outstanding notification, or else 
one of the system-wide error codes. */
EXPORT_C TInt RASCliSession::AlarmDelete(TAlarmId aAlarmId) const
	{
	TIpcArgs args(aAlarmId);
	return SendReceive(EASShdOpCodeAlarmDelete, args);
	}


/** Retrieves the category of an alarm. 

@capability None
@param aAlarmId The unique identifier of the alarm under query.
@param aCategory On return, contains the category of the alarm.
@return KErrNone if successful, KErrNotFound if the alarm does not exist, otherwise 
one of the system-wide error codes. */
EXPORT_C TInt RASCliSession::GetAlarmCategory(TAlarmId aAlarmId, TAlarmCategory& aCategory) const
	{
	TPckg<TAlarmCategory> package(aCategory);
	TIpcArgs args(aAlarmId, &package);
	//
	return SendReceive(EASShdOpCodeGetAlarmCategory, args);
	}


/** @deprecated 8.0

Retrieves the full name of the thread that owns the specified alarm.

@capability None
@param aAlarmId The unique identifier of the alarm under query.
@param aThreadName On return, contains the name of the owning session.
@return KErrNone if successful, KErrNotFound if the alarm has no originating 
session ID or if the session is no longer connected, otherwise one of the 
system-wide errors. */
EXPORT_C TInt RASCliSession::GetAlarmOwner(TAlarmId aAlarmId, TFullName& aThreadName) const
	{
//
// From 8.0 onwards, this function returns a NULL string.
// See CASSrvSession::MASSrvSessionFullName
//
	TIpcArgs args(aAlarmId, &aThreadName);
	return SendReceive(EASShdOpCodeGetAlarmOwner, args);
	}


/** Sets an alarm's status to either enabled or disabled.

@capability Note If the user does not have WriteDeviceData capability then we need to check 
that the alarm belongs to the user SID (The SID is stored as part of the alarm)
@param aAlarmId Unique identifier of the alarm.
@param aStatus Contains the status to be applied. 
@return KErrNone if successful, otherwise one of the system-wide error codes. */
EXPORT_C TInt RASCliSession::SetAlarmStatus(TAlarmId aAlarmId, TAlarmStatus aStatus) const
	{
	TIpcArgs args(aAlarmId, aStatus);
	return SendReceive(EASShdOpCodeSetAlarmStatus, args);
	}

/** Sets the alarm status for calendar file

@param aFileName Calendar file name 
@param aStatus Contains the status to be applied.  
@return KErrNone if successful, otherwise one of the system-wide error codes. */
EXPORT_C TInt RASCliSession::SetAlarmStatusForCalendarFile(const TDesC& aFileName, TAlarmStatus aStatus) const
    {
    TIpcArgs args(&aFileName, aStatus);
    return SendReceive(EASShdOpCodeSetAlarmStatusForCalendarFile, args);
    }

/** Retrieves the status of the alarm. 

@capability None
@param aAlarmId The unique identifier of an alarm under query.
@param aStatus On return, the alarm status.
@return KErrNone if successful, KErrNotFound if the alarm does not exist, otherwise 
one of the system-wide errors. */
EXPORT_C TInt RASCliSession::GetAlarmStatus(TAlarmId aAlarmId, TAlarmStatus& aStatus) const
	{
	TPckg<TAlarmStatus> package(aStatus);
	TIpcArgs args(aAlarmId, &package);
	//
	return SendReceive(EASShdOpCodeGetAlarmStatus, args);
	}


/** Specifies whether an alarm belongs to a timed or untimed event.

@capability Note If the user does not have WriteDeviceData capability then we need to check 
that the alarm belongs to the user SID (The SID is stored as part of the alarm)
@param aAlarmId Unique identifier of the alarm.
@param aDayOrTimed Whether an alarm is for a timed or untimed event. 
@return KErrNone if successful, otherwise one of the system-wide errors. */
EXPORT_C TInt RASCliSession::SetAlarmDayOrTimed(TAlarmId aAlarmId, TAlarmDayOrTimed aDayOrTimed) const
	{
	TIpcArgs args(aAlarmId, aDayOrTimed);
	return SendReceive(EASShdOpCodeSetAlarmDayOrTimed, args);
	}


/** Tests whether the specified alarm is for a timed or untimed event.

@capability None
@param aAlarmId Unique identifier of the alarm under query.
@param aDayOrTimed On return, whether an alarm is for a timed or untimed event.
@return KErrNone if successful, otherwise one of the system-wide errors. */
EXPORT_C TInt RASCliSession::GetAlarmDayOrTimed(TAlarmId aAlarmId, TAlarmDayOrTimed& aDayOrTimed) const
	{
	TPckg<TAlarmDayOrTimed> package(aDayOrTimed);
	TIpcArgs args(aAlarmId, &package);
	//
	return SendReceive(EASShdOpCodeGetAlarmDayOrTimed, args);
	}


/** Sets the characteristics of a specified alarm. 

If the session-specific flag is removed, the outstanding notification 
is completed.

@capability Note If the user does not have WriteDeviceData capability then we need to check 
that the alarm belongs to the user SID (The SID is stored as part of the alarm)
@param aAlarmId Unique identifier of the alarm. 
@param aCharacteristics Alarm characteristics to apply. 
@return KErrNone if successful, otherwise one of the system-wide errors. */
EXPORT_C TInt RASCliSession::SetAlarmCharacteristics(TAlarmId aAlarmId, TAlarmCharacteristicsFlags aCharacteristics) const
	{
	TPckgC<TAlarmCharacteristicsFlags> package(aCharacteristics);
	TIpcArgs args(aAlarmId, &package);
	//
	return SendReceive(EASShdOpCodeSetAlarmCharacteristics, args);
	}


/** Gets the characteristics of an alarm.

@capability None
@param aAlarmId Unique identifier of the alarm under query.
@param aCharacteristics On return, contains the charcteristics of the alarm 
specified.
@return KErrNone if successful, otherwise one of the system-wide errors. */
EXPORT_C TInt RASCliSession::GetAlarmCharacteristics(TAlarmId aAlarmId, TAlarmCharacteristicsFlags& aCharacteristics) const
	{
	TPckg<TAlarmCharacteristicsFlags> package(aCharacteristics);
	TIpcArgs args(aAlarmId, &package);
	//
	return SendReceive(EASShdOpCodeGetAlarmCharacteristics, args);
	}

/** Sets if the alarm is a wakeup alarm

@capability None
@param aAlarmId Unique identifier of the alarm under query.
@param aEnabled if the alarm is to be a wakeup alarm.
@return KErrNone if successful, otherwise one of the system-wide errors. */

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
EXPORT_C TInt RASCliSession::SetWakeup(TAlarmId aAlarmId, TBool aEnabled) const
	{
	TIpcArgs args(aAlarmId, aEnabled);
	return SendReceive(EASShdOpCodeSetWakeup, args);
	}
#endif

#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
/**
Sets which days of the week an alarm with a repeat definition of
EAlarmRepeatDefintionRepeatDailyOnGivenDays activates on.  By default, an alarm
with a repeat definition of EAlarmRepeatDefinitionRepeatDailyOnGivenDays will be
active on all days of the week.

@prototype

@capability
	WriteDeviceData
	Note: If the client does not have WriteDeviceData capability then it must
	own the alarm.  A client owns an alarm if the SID of the client matches the
	SID of the alarm.
	
@param aAlarmId
	Identifies the alarm whose alarm days are to be set.
	
@param aAlarmDays
	The days of the week that the alarm activates on.  Days are combined using
	the bitwise OR operator e.g. EAlarmDayMonday|EAlarmDayTuesday.

@return
	KErrNone if successful, KErrArgument if aAlarmDays is an invalid value
	otherwise one of the system-wide errors.
*/
EXPORT_C TInt RASCliSession::SetAlarmDays(TAlarmId aAlarmId, TUint8 aAlarmDays) const
	{
	TIpcArgs args(aAlarmId, aAlarmDays);
	return SendReceive(EASShdOpCodeSetAlarmDays, args);
	}

/**
Gets the days of the week that the given alarm activates on.  This value is only
applicable if the alarm’s repeat definition is
EAlarmRepeatDefinitionRepeatDailyOnGivenDays.  By default, an alarm with a
repeat definition of EAlarmRepeatDefinitionRepeatDailyOn-GivenDays will be
active on all days of the week.

@prototype

@param aAlarmId
	Identifies the alarm whose alarm days are to be returned.
	
@param aAlarmDays
	On return the days of the week that the alarm activates on.  The value will
	be a combination of the values defined in the TAlarmDays enumeration.

@return
	KErrNone if successful, otherwise one of the system-wide errors.
*/
EXPORT_C TInt RASCliSession::GetAlarmDays(TAlarmId aAlarmId, TUint8& aAlarmDays) const
	{
	TPckg<TUint8> package(aAlarmDays);
	TIpcArgs args(aAlarmId, &package);
	return SendReceive(EASShdOpCodeGetAlarmDays, args);
	}

/**
Sets the continuous state of the given alarm. 

@prototype

@capability
	WriteDeviceData
	Note: If the client does not have WriteDeviceData capability then it must
	own the alarm.  A client owns an alarm if the SID of the client matches the
	SID of the alarm.
	
@param aAlarmId
	Identifies the alarm whose continuous state is to be set.
	
@param aContinuous
	ETrue if the alarm is continuous, EFalse if the alarm is not continuous.

@return
	KErrNone if successful, otherwise one of the system-wide errors.
*/
EXPORT_C TInt RASCliSession::SetContinuous(TAlarmId aAlarmId, TBool aContinuous) const
	{
	TIpcArgs args(aAlarmId, aContinuous);
	return SendReceive(EASShdOpCodeSetContinuous, args);
	}

/**
Gets the continuous state of the given alarm.

@prototype

@param aAlarmId
	Identifies the alarm whose continuous state is to be returned.
	
@param aContinuous
	On return ETrue if the alarm is continuous, EFalse if the alarm is not
	continuous.

@return
	KErrNone if successful, otherwise one of the system-wide errors.
*/
EXPORT_C TInt RASCliSession::GetContinuous(TAlarmId aAlarmId, TBool& aContinuous) const
	{
	TPckg<TBool> package(aContinuous);
	TIpcArgs args(aAlarmId, &package);
	return SendReceive(EASShdOpCodeGetContinuous, args);
	}
#endif

/** Updates an alarm's client-specific data in the alarm server.

The TASShdAlarm class contains a number of client-specific fields. These fields 
are for use by clients of the alarm server to hold any data associated with 
an alarm. Although this data is not used in any way by the alarm server itself, 
clients can ensure that the alarm server keeps its own copy of the specified 
alarm up to date with that of the client.

@capability Note If the user does not have WriteDeviceData capability then we need to check 
that the alarm belongs to the user SID (The SID is stored as part of the alarm)
@param aAlarm The client-side alarm, whose corresponding server-side alarm 
is to be updated.
@return KErrNone if successful, otherwise one of the system-wide error codes. */
EXPORT_C TInt RASCliSession::SetClientData(const TASShdAlarm& aAlarm)
	{
	TPckgC<TASShdAlarm> package(aAlarm);
	TIpcArgs args(aAlarm.Id(), &package);
	//
	return SendReceive(EASShdOpCodeSetClientData, args);
	}


//
//
//


/** Associates the specified data with an alarm. 

This is arbitrary client-specific data, for which the alarm 
server simply acts as a router.

@capability Note If the user does not have WriteDeviceData capability then we need to check  
that the alarm belongs to the user SID (The SID is stored as part of the alarm) 
@param aAlarmId Unique identifier of the alarm.
@param aData Client-specific data to associate with the alarm.
@return KErrNone if successful, KErrInUse if the specified alarm already has 
data assigned to it, or one of the system-wide error codes. */
EXPORT_C TInt RASCliSession::AlarmDataAttachL(TAlarmId aAlarmId, const TDesC8& aData) const
	{
	TIpcArgs args(aAlarmId, aData.Length(), &aData);
	return SendReceive(EASShdOpCodeAlarmDataAttach, args);
	}


/** Removes any previously attached alarm data from the specified alarm. 

This releases any resources allocated by the alarm server for this alarm.

@capability Note If the user does not have WriteDeviceData capability then we need to check 
that the alarm belongs to the user SID (The SID is stored as part of the alarm) 
@param aAlarmId Unique identifier for the alarm.
@return KErrNone if successful, KErrNotFound if the specified alarm does not 
exist (or the specified alarm does not have any data), or another of the system-wide 
error codes. */
EXPORT_C TInt RASCliSession::AlarmDataDetach(TAlarmId aAlarmId) const
	{
	TIpcArgs args(aAlarmId);
	return SendReceive(EASShdOpCodeAlarmDataDetach, args);
	}


/** Returns the size in bytes of any data associated with the specified alarm.

@capability None
@param aAlarmId Unique identifier of the alarm under query.
@return The size in bytes of the alarm's data, or an error. KErrNotFound is 
returned if the specified alarm does not have any associated data. */
EXPORT_C TInt RASCliSession::AlarmDataSize(TAlarmId aAlarmId) const
	{
	TIpcArgs args(aAlarmId);
	return SendReceive(EASShdOpCodeAlarmDataSize, args);
	}


/** Retrieves the data attached to the specified alarm.

This is data previously attached using AlarmDataAttachL().

@capability Note If the user does not have ReadUserData capability then we need to check  
that the alarm belongs to the user SID (The SID is stored as part of the alarm) 
@param aAlarmId Unique identifier of the alarm under query.
@param aSink On return, contains the data associated with the alarm. This buffer 
must be large enough to contain all the data or a panic occurs. 
@return KErrNone if successful, KErrNotFound if the specified alarm does not 
have any associated data, or a system-wide error. */
EXPORT_C TInt RASCliSession::GetAlarmData(TAlarmId aAlarmId, TDes8& aSink) const
	{
	TIpcArgs args(aAlarmId, aSink.MaxLength(), &aSink);
	return SendReceive(EASShdOpCodeGetAlarmData, args);
	}


/** Retrieves the data attached to the specified alarm.

This is data previously attached using AlarmDataAttachL().

@capability Note If the user does not have ReadUserData capability then we need to check  
that the alarm belongs to the user SID (The SID is stored as part of the alarm) 
@param aAlarmId Unique identifier of the alarm under query.
@param aSink On return, contains the data associated with the alarm, or
NULL if the specified alarm does not have any associated data, or an error was returned.
@return KErrNone if successful, KErrNotFound if the specified alarm does not 
have any associated data, or a system-wide error. */
EXPORT_C TInt RASCliSession::GetAlarmData(TAlarmId aAlarmId, HBufC8*& aSink) const
	{
	aSink = NULL;
	TInt r = AlarmDataSize(aAlarmId);
	
	if (r > 0)
		{
		aSink = HBufC8::New(r);
		if (!aSink)
			{
			return KErrNoMemory;
			}
		
		TPtr8 data = aSink->Des();
		r = GetAlarmData(aAlarmId, data);
		
		if (r != KErrNone)
			{
			delete aSink;
			aSink = NULL;
			}
		}
	
	return r;
	}


//
//
//


/** Set the status of all alarms in the specified category.

@capability Note If the user does not have WriteDeviceData capability then we need to check  
that the alarm belongs to the user SID (The SID is stored as part of the alarm) 
@param aCategory An alarm category. 
@param aStatus An alarm status.
@return KErrNone if successful, otherwise one of the system-wide error codes. */
EXPORT_C TInt RASCliSession::SetAlarmStatusByCategory(TAlarmCategory aCategory, TAlarmStatus aStatus) const
	{
	TIpcArgs args(aCategory.iUid, aStatus);
	return SendReceive(EASShdOpCodeSetAlarmStatusByCategory, args);
	}


/** Returns the number of alarms in a specified category.

@capability None
@param aCategory Category of alarm under query.
@return Number of alarms in the specified category, or, if negative, one of 
the standard error codes.. */
EXPORT_C TInt RASCliSession::GetAlarmCountForCategory(TAlarmCategory aCategory) const
	{
	TPckgBuf<TInt> package;
	TIpcArgs args(aCategory.iUid, &package);
	//
	const TInt error = SendReceive(EASShdOpCodeGetAlarmCountForCategory, args);
	if	(error != KErrNone)
		return error;
	return package();
	}


/** Deletes all alarms in the queue corresponding to a specified category. 

You can also specify that only the orphaned alarms of that category be deleted.

@capability Note If the user does not have WriteDeviceData capability then we need to check 
that the alarm belongs to the user SID (The SID is stored as part of the alarm)
@param aCategory Category of alarms to be deleted.
@param aDeleteOnlyOrphanedAlarmsInCategory ETrue: delete only orphaned alarms 
within the category. EFalse: delete all alarms within the category.
@return KErrNone if successful, otherwise one of the system-wide error codes. */
EXPORT_C TInt RASCliSession::AlarmDeleteAllByCategory(TAlarmCategory aCategory, TBool aDeleteOnlyOrphanedAlarmsInCategory) const
	{
	TIpcArgs args(aCategory.iUid, aDeleteOnlyOrphanedAlarmsInCategory);
	return SendReceive(EASShdOpCodeAlarmDeleteAllByCategory, args);
	}


/** Deletes alarms in the queue corresponding to a specified category and type. 

@capability Note If the user does not have WriteDeviceData capability then we need to check  
that the alarm belongs to the user SID (The SID is stored as part of the alarm) 
@param aCategory Category of alarms to be deleted.
@param aWhatToDelete specify what type of alarms to delte
within the category.
@return KErrNone if successful, otherwise one of the system-wide error codes. */
EXPORT_C TInt RASCliSession::AlarmDeleteByCategory(TAlarmCategory aCategory, TDeleteType aWhatToDelete) const
	{
	TIpcArgs args(aCategory.iUid, aWhatToDelete);
	return SendReceive(EASShdOpCodeAlarmDeleteByCategory, args);
	}
 
/**
@internalComponent

Delete alarms based on their type and the Calendar filename to which alarms belong 

@capability Note If the user does not have WriteDeviceData capability then we need to check  
that the alarm belongs to the user SID (The SID is stored as part of the alarm) 
@param aWhatToDelete specify what type of alarms to delte
within the category.
@param aFileName specify the name of the Calendar file which alarms came from
@return KErrNone if successful, otherwise one of the system-wide error codes.
*/
EXPORT_C TInt RASCliSession::AlarmDeleteByCalendarFile(const TDesC& aFileName, TDeleteType aWhatToDelete) const
    {
    TIpcArgs args(&aFileName, aWhatToDelete);
    return SendReceive(EASShdOpCodeAlarmDeleteByCalendarFile, args);
    }
    

/**

Retrieves a list of all alarm categories in use within the alarm server.

@capability None
@param aCategories On return, contains the list of available categories.

*/
EXPORT_C void RASCliSession::GetAvailableCategoryListL(RArray<TAlarmCategory>& aCategories) const
	{
	CleanupClosePushL(aCategories);
	// First step is to build the transfer buffer in the server
	// and get the size (in bytes) so that we know
	// how big to make the client-side (corresponding) temporary one.
	TPckgBuf<TInt> sizeOfTransferBuffer;
	TIpcArgs args(&sizeOfTransferBuffer);
	User::LeaveIfError(SendReceive(EASShdOpCodeGetAvailableCategoryList, args));
	
	// Next step is to create a buffer of sufficient size in order to fetch the
	// buffer from the server
	CBufBase* buffer = FetchTransferBufferLC(sizeOfTransferBuffer());

	// The buffer just contains serialized TAlarmCategories so we need to
	// extract them and populate the array.
	TAlarmCategory category;
	aCategories.Reset();
	RBufReadStream stream(*buffer);
	CleanupClosePushL(stream); // Don't think this is necessary
	const TInt count = stream.ReadInt32L();
	for(TInt i=0; i<count; i++)
		{
		stream >> category;
		User::LeaveIfError(aCategories.Append(category));
		}
	//
	stream.Close();
	CleanupStack::PopAndDestroy(2, buffer);
  // relieve the ownership of the array to the caller
  CleanupStack::Pop(&aCategories);
	}


/**

Retrieves a list of all alarm identifiers for alarms within the server of a
specified category.

@capability None
@param aCategory Category of alarms to find.
@param aAlarmIds On return, contains the list of alarms.

*/

EXPORT_C void RASCliSession::GetAlarmIdListForCategoryL(TAlarmCategory aCategory, RArray<TAlarmId>& aAlarmIds) const
	{
	// First step is to build the transfer buffer in the server
	// and get the size (in bytes) so that we know
	// how big to make the client-side (corresponding) temporary one.
	TPckgBuf<TInt> sizeOfTransferBuffer;
	TIpcArgs args(aCategory.iUid, &sizeOfTransferBuffer);
	User::LeaveIfError(SendReceive(EASShdOpCodeGetAlarmIdListForCategory, args));

	// Fetch the array
	FetchAlarmIdsFromBufferL(aAlarmIds, sizeOfTransferBuffer());
	}


//
//
//


/**

Returns the number of alarms in a specified state.

@capability None
@param aState Alarm state of interest. 
@return Number of alarms in the specified state.	

*/
EXPORT_C TInt RASCliSession::AlarmCountByState(TAlarmState aState) const
	{
	TPckgBuf<TInt> package;
	TIpcArgs args(aState, &package);
	//
	const TInt error = SendReceive(EASShdOpCodeAlarmCountByState, args);
	if	(error != KErrNone)
		return error;
	return package();
	}


/**

Retrieves a list of the unique identifiers of all alarms in a specified state.

@capability None
@param aState Alarm state that you are interested in.
@param aAlarmIds On return, contains the unique identifiers of the alarms in
the specified state.

*/
EXPORT_C void RASCliSession::GetAlarmIdListByStateL(TAlarmState aState, RArray<TAlarmId>& aAlarmIds) const
	{
	// First step is to build the transfer buffer in the server
	// and get the size (in bytes) so that we know
	// how big to make the client-side (corresponding) temporary one.
	TPckgBuf<TInt> sizeOfTransferBuffer;
	TIpcArgs args(aState, &sizeOfTransferBuffer);
	User::LeaveIfError(SendReceive(EASShdOpCodeGetAlarmIdListByState, args));

	// Fetch the array
	FetchAlarmIdsFromBufferL(aAlarmIds, sizeOfTransferBuffer());
	}


/**

Retrieves a list of the unique identifiers of all the alarms in the alarm server.

@capability None
@param aAlarmIds On return, contains a list of unique identifiers.

*/
EXPORT_C void RASCliSession::GetAlarmIdListL(RArray<TAlarmId>& aAlarmIds) const
	{
	// First step is to build the transfer buffer in the server
	// and get the size (in bytes) so that we know
	// how big to make the client-side (corresponding) temporary one.
	TPckgBuf<TInt> sizeOfTransferBuffer;
	// We put this in slot 1 because of the way the server has been written.
	TIpcArgs args(TIpcArgs::ENothing, &sizeOfTransferBuffer);
	User::LeaveIfError(SendReceive(EASShdOpCodeGetAlarmIdList, args));

	// Fetch the array
	FetchAlarmIdsFromBufferL(aAlarmIds, sizeOfTransferBuffer());
	}


/**

Retrieves the unique identifier of the next alarm in the alarm server queue.

@capability None
@param aAlarmId On return, contains the unique identifier of the next alarm 
due.
@return KErrNone if successful, KNullAlarmId if there are no alarms in the 
queue, otherwise another one of the system-wide error codes.

*/
EXPORT_C TInt RASCliSession::GetNextDueAlarmId(TAlarmId& aAlarmId) const
	{
	TPckg<TAlarmId> package(aAlarmId);
	TIpcArgs args(&package);
	//
	return SendReceive(EASShdOpCodeGetNextDueAlarmId, args);
	}


/**

Returns the number of alarms that are currently active within the alarm queue.
Active alarms are:

Alarms that have not yet notified.

Alarms that are in the queued or snoozed state, and are enabled.


@capability None
@return Number of active alarms, or an error code.

*/
EXPORT_C TInt RASCliSession::NumberOfAlarmsActiveInQueue() const
	{
	TPckgBuf<TInt> package;
	TIpcArgs args(&package);
	//
	const TInt error = SendReceive(EASShdOpCodeNumberOfAlarmsActiveInQueue, args);
	if	(error != KErrNone)
		return error;
	return package();
	}


//
//
//


/**

Sets the alarm sound state to on or off.

@capability WriteDeviceData
@param aState The alarm sound state.
@return KErrNone if successful, otherwise one of the system-wide errors.	

*/
EXPORT_C TInt RASCliSession::SetAlarmSoundState(TAlarmGlobalSoundState aState) const
	{
	TIpcArgs args(aState);
	return SendReceive(EASShdOpCodeSetAlarmSoundState, args);
	}


/**

Retrieves the alarm sound state. The alarm sound can be on or off.

@capability None
@param aState On return, contains the alarm sound state.
@return KErrNone if successful, otherwise one of the system-wide error codes.	

*/
EXPORT_C TInt RASCliSession::GetAlarmSoundState(TAlarmGlobalSoundState& aState) const
	{
	TPckg<TAlarmGlobalSoundState> package(aState);
	TIpcArgs args(&package);
	//
	return SendReceive(EASShdOpCodeGetAlarmSoundState, args);
	}


/**

Disables alarm sounds until a specified time.

Alarms still expire and clients are notified during this period, but no sound
is played.

@capability WriteDeviceData
@param aLocalTime Time, in local time, when alarm sounds are to be played once
more.
@return KErrNone if successful, KErrArgument if the time is in the past,
otherwise another one of the system-wide error codes.

*/
EXPORT_C TInt RASCliSession::SetAlarmSoundsSilentUntil(const TTime& aLocalTime) const
	{
	TPckgC<TTime> package(aLocalTime);
	TIpcArgs args(&package);
	//
	return SendReceive(EASShdOpCodeSetAlarmSoundsSilentUntil, args);
	}


/**

Disables alarm sounds for a specified interval. 

Alarms still expire and clients are notified during this period, but no sound 
is played.

NOTE: If user or some application changes system time or UTC offset (for example by using  
User::SetUTCOffset(),  User::SetHomeTime(),  User::SetUTCTimeAndOffset() etc) 
the silent periond will be cancelled.

@capability WriteDeviceData
@param aTimeToRemainSilentFor Time interval in minutes, during which no sound 
is to be played.
@return KErrNone if successful, otherwise one of the system-wide error codes.	

*/
EXPORT_C TInt RASCliSession::SetAlarmSoundsSilentFor(TTimeIntervalMinutes aTimeToRemainSilentFor) const
	{
	TPckgC<TTimeIntervalMinutes> package(aTimeToRemainSilentFor);
	TIpcArgs args(&package);
	//
	return SendReceive(EASShdOpCodeSetAlarmSoundsSilentFor, args);
	}


/**

Retrieves the time at which all alarm sounds resume.

@capability None
@param aLocalTime On return, contains the time, in local time, when sounds resume.
@return KErrNone if successful, otherwise one of the system-wide error codes.

*/
EXPORT_C TInt RASCliSession::GetAlarmSoundsSilentUntil(TTime& aLocalTime) const
	{
	TPckg<TTime> package(aLocalTime);
	TIpcArgs args(&package);
	//
	return SendReceive(EASShdOpCodeGetAlarmSoundsSilentUntil, args);
	}


/**

Cancels the silent period, turning the alarm sounds on.

@capability WriteDeviceData
@return KErrNone if successful, otherwise one of the system-wide error codes.

*/
EXPORT_C TInt RASCliSession::CancelAlarmSilence() const
	{
	return SendReceive(EASShdOpCodeCancelAlarmSilence);
	}


/**

Tests whether the alarm server has temporarily disabled sounds. 

@capability None
@return ETrue: alarms are temporarily silent. EFalse: sounds are not silent.

*/
EXPORT_C TBool RASCliSession::AlarmSoundsTemporarilySilenced() const
	{
	TPckgBuf<TBool> package;
	TIpcArgs args(&package);
	//
	const TInt error = SendReceive(EASShdOpCodeAlarmSoundsTemporarilySilenced, args);
	if	(error != KErrNone)
		return EFalse;
	return package();
	}


/**

Sets the list of alarm intervals. 

Alarm intervals consist of a duration and an offset.

@capability WriteDeviceData
@param aIntervals Contains one or more alarm intervals. There must be one 
interval in the array with an offset of zero.
@leave KErrGeneral If the array is inappropriate, for example, if intervals 
overlap or has no elements. Also, from v9.2 onwards, if the array has only one
element and the Repeat Setting is set to RepeatLast.
@leave KErrArgument If no interval has an offset of zero.	
@leave KErrNotSupported The Alarm Play Intervals cannot be set because Alarm Play Intervals is disabled by resource file. (from v9.2 onwards)

*/
EXPORT_C void RASCliSession::SetAlarmPlayIntervalsL(const CArrayFix<TASCliSoundPlayDefinition>& aIntervals) const
	{
	// Create the buffer first off.
	const TInt count = aIntervals.Count();
	CBufBase* buffer = CBufFlat::NewL((count + 1) * sizeof(TASCliSoundPlayDefinition)); // approximation
	CleanupStack::PushL(buffer);

	// Stream out all the details
	RBufWriteStream stream(*buffer);
	CleanupClosePushL(stream); // don't think this is necessary
	stream.WriteInt32L(count);
	for(TInt i=0; i<count; i++)
		{
		const TASCliSoundPlayDefinition& interval = aIntervals[i];
		//
		stream.WriteInt32L(interval.Offset().Int());
		stream.WriteInt32L(interval.Duration().Int());
		}

	// Prepare our arguments
	TPtr8 ptr=buffer->Ptr(0);		//To stop a warning
	TIpcArgs args(&ptr, buffer->Size(), count);

	// Send to server, and tidy up afterwards
	const TInt error = SendReceive(EASShdOpCodeSetAlarmPlayIntervals, args);
	User::LeaveIfError(error);
	CleanupStack::PopAndDestroy(2, buffer);
	}


/**

Retrieves the list of alarm intervals. 

They are stored in ascending order of offset.

@capability None
@param aIntervals On return, contains the list of alarm intervals.

*/
EXPORT_C void RASCliSession::GetAlarmPlayIntervalsL(CArrayFix<TASCliSoundPlayDefinition>& aIntervals) const
	{
	// First step is to build the transfer buffer in the server
	// and get the size (in bytes) so that we know
	// how big to make the client-side (corresponding) temporary one.
	TPckgBuf<TInt> sizeOfTransferBuffer;
	TIpcArgs args(&sizeOfTransferBuffer);
	User::LeaveIfError(SendReceive(EASShdOpCodeGetAlarmPlayIntervals, args));

	// Get buffer
	CBufBase* buffer = FetchTransferBufferLC(sizeOfTransferBuffer());
	RBufReadStream stream(*buffer);
	CleanupClosePushL(stream);
	//
	aIntervals.Reset();
	const TInt count = stream.ReadInt32L();
	for(TInt i=0; i<count; i++)
		{
		const TTimeIntervalMinutes offset(stream.ReadUint32L());
		const TTimeIntervalSeconds duration(stream.ReadUint32L());
		//
		aIntervals.AppendL(TASCliSoundPlayDefinition(offset, duration));
		}
	//
	CleanupStack::PopAndDestroy(2, buffer);
	}


//
//
//


/**

Enables client notification when alarm settings change, and when the next alarm 
time is calculated.

A panic occurs if notification is already active.

@capability None
@param aStatus The request status object that is to be signalled. On return, 
contains one of the TAlarmChangeEvent enumeration values. If an error occurs, 
the TRequestStatus object contains one of the standard system-wide error codes.
@param aAlarmId An alarm identifier relating to the type of event that took 
place. If the type of event that occurred relates to a particular alarm, this 
object is populated with that alarm's identiifer. Otherwise it contains an 
undefined value.
@see TAlarmChangeEvent

*/
EXPORT_C void RASCliSession::NotifyChange(TRequestStatus& aStatus, TAlarmId& aAlarmId)
	{
	iAlarmIdPointer.Set((TUint8*) &aAlarmId, sizeof(TAlarmId), sizeof(TAlarmId));
	TIpcArgs args(&iAlarmIdPointer, EFalse);
	SendReceive(EASShdOpCodeNotifyChange, args, aStatus);
	}


/**

Cancels any previous change notification request.

@capability None
@panic Panics with AlarmClientUtils::EAlarmClientPanicNotificationCancel if the
notification fails to occur.

*/
EXPORT_C void RASCliSession::NotifyChangeCancel() const
	{
	const TInt error = SendReceive(EASShdOpCodeNotifyChangeCancel);
	__ASSERT_ALWAYS(error == KErrNone || error == KErrServerTerminated, AlarmClientUtils::Panic(AlarmClientUtils::EAlarmClientPanicNotificationCancel));
	}


//
//
//


/**
@internalComponent

In debug builds, this will cause the Alarm Server to terminate.

@capability None
*/
EXPORT_C void RASCliSession::__DbgShutDownServer() const
	{
#ifdef _DEBUG
	const TInt error = SendReceive(EASShdOpCodeDbgShutDownServer);
	__ASSERT_DEBUG(error == KErrNone || error == KErrServerTerminated, AlarmClientUtils::Fault(AlarmClientUtils::EAlarmClientFaultDebugFuncError));
	//the following line is to make sure that the funtion will not return until the server is closed down.
	while (NumberOfAlarmsActiveInQueue()!=KErrServerTerminated)
		{
		User::After(100000);
		}
#endif
	}


/**
@internalComponent

In debug builds, this will fail a memory allocation.

@capability None
@param aCount Set this to the number of allocations to fail, or 0
to reset the failure tool.

*/
EXPORT_C void RASCliSession::__DbgFailAlloc(TInt
#ifdef _DEBUG
												aCount
#endif
													) const
	{
#ifdef _DEBUG
	TIpcArgs args(aCount);
	const TInt error = SendReceive(EASShdOpCodeDbgFailAlloc, args);
	__ASSERT_DEBUG(error == KErrNone || error == KErrServerTerminated, AlarmClientUtils::Fault(AlarmClientUtils::EAlarmClientFaultDebugFuncError));
#endif
	}


/**
@internalComponent

In debug builds, this will prevent the Alarm Server from notifying the Alarm
Alert Server about the expiry of an alarm.

@capability None
@param aShouldStop ETrue: Alarm Alert Server is not notified on alarm expiry
EFalse: Alarm Alert Server is notified on alarm expiry

*/
EXPORT_C void RASCliSession::__DbgPreventUserNotify(TBool
#ifdef _DEBUG
														aShouldStop
#endif
																) const
	{
#ifdef _DEBUG
	TIpcArgs args(aShouldStop);
	const TInt error = SendReceive(EASShdOpCodeDbgPreventUserNotify, args);
	__ASSERT_DEBUG(error == KErrNone || error == KErrServerTerminated, AlarmClientUtils::Fault(AlarmClientUtils::EAlarmClientFaultDebugFuncError));
#endif
	}


/**

@internalComponent
 
In debug builds, this will cause the specified alarm to be snoozed.

@capability None
@param aAlarmId Unique identifier of the alarm.
@param aNewTime Time to reawaken alarm (as UTC time).
@return Returns whether the alarm could be snoozed.

 */
EXPORT_C TInt RASCliSession::__DbgSnoozeAlarm(TAlarmId
#ifdef _DEBUG
													aAlarmId
#endif
															, const TTime&
#ifdef _DEBUG
																		aNewTime
#endif
																				) const
	{
	TInt error = KErrNotSupported;
	//
#ifdef _DEBUG
	TPckgC<TTime> package(aNewTime);
	TIpcArgs args(aAlarmId, &package);
	error = SendReceive(EASShdOpCodeDbgSnoozeAlarm, args);
#endif
	//
	return error;
	}

/**

@internalComponent
 
In debug builds, this will switch handling of time/date changes by CASSrvEnvironmentChangeManager

@capability None
@param aFlag ETrue to enable and EFalse to disable handling 
 */

EXPORT_C TInt RASCliSession::__DbgSetEnvChgHandling(TBool
#ifdef _DEBUG
													aFlag
#endif
													) const
	{
	TInt error = KErrNotSupported;
	//
#ifdef _DEBUG
	TIpcArgs args(aFlag);
	error = SendReceive(EASShdOpCodeDbgSetEnvironmentChangesHandling, args);
#endif
    return error;
	}

//
//
//

/** 
@capability None
*/
void RASCliSession::FetchAlarmIdsFromBufferL(RArray<TAlarmId>& aAlarmIds, TInt aBufferSize) const
	{
	CleanupClosePushL(aAlarmIds);
	CBufBase* buffer = FetchTransferBufferLC(aBufferSize);

	// The buffer just contains serialized TAlarmId's so we need to
	// extract them and populate the array.
	aAlarmIds.Reset();
	RBufReadStream stream(*buffer);
	CleanupClosePushL(stream);
	//
	const TInt count = stream.ReadInt32L();
	for(TInt i=0; i<count; i++)
		{
		const TAlarmId id = stream.ReadInt32L();
		aAlarmIds.AppendL(id);
		}
	//
	CleanupStack::PopAndDestroy(2, buffer);
	// relieve the ownership of the array to the caller
	CleanupStack::Pop(&aAlarmIds); 
	}

/** 
Create a buffer of sufficient size in order to fetch the buffer from the server

@capability None
*/	
CBufBase* RASCliSession::FetchTransferBufferLC(TInt aBufferSize) const
	{
	// Create a buffer of sufficient size in order to fetch the
	// buffer from the server
	CBufBase* buffer = CBufFlat::NewL(aBufferSize);
	CleanupStack::PushL(buffer);
	buffer->ResizeL(aBufferSize);

	// Now fetch the transfer buffer from the server
	TPtr8 pBuffer(buffer->Ptr(0));
	//
	TIpcArgs args(&pBuffer);
	User::LeaveIfError(SendReceive(EASShdOpCodeFetchTransferBuffer, args));
	//
	return buffer;
	}








