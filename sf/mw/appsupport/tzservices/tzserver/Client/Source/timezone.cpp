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
//

#include "timezoneserver.h"
#include <tzdefines.h>
#include <tz.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <tzusernames.h> //new file added for CTzUserNames which is publishedPartner
#endif
#include <tzlocalizedcityrecord.h>
#include <tzlocalizedtimezonerecord.h>
#include "tzrules.h"
#include <s32mem.h>
#include <vtzrules.h>
#include <tzupdate.h>
#include "tzruleholder.h"

const TInt KNumConnectRetries = 5;

// Define server name
_LIT(KServerCode,"TZSERVER");

// Return the client side version no.
TVersion RTz::Version() const
	{
	return (TVersion(KTimeZoneServerMajorVersion,
			KTimeZoneServerMinorVersion,
			KTimeZoneServerBuildVersion));
	}

// 
//Start the server process/thread which lives in an EPOCEXE object
//
TInt RTz::StartServer()
	{
	const TInt KIntServerType = 0x10004019;
	const TUidType serverUid(KNullUid,TUid::Uid(KIntServerType));
	
	TRequestStatus started(KRequestPending);
	TRendezvous rendezvous;
	rendezvous.iId = RThread().Id();		// id of this thread
	rendezvous.iStatus = &started;

	RProcess server;
	TInt err = server.Create(KServerCode,
						TPtrC(reinterpret_cast<TText*>(&rendezvous),
						sizeof(rendezvous)/sizeof(TText)),serverUid);

	if (err != KErrNone)
		{
		return err;
		}

	server.SetPriority(EPriorityHigh);

	TRequestStatus stat;
	server.Rendezvous(stat);
	if (stat != KRequestPending)
		{
		server.Kill(0);		// abort startup
		}
	else
		{
		server.Resume();	// logon OK - start the server
		}
	User::WaitForRequest(stat);		// wait for start or death
	server.Close();
	return stat.Int();
	}

/**
Retrieves the time zone rules for the current home time zone.

@param aStartTime The start date to generate the time zone rules from.
@param aEndTime The end date to generate the time zone rules to.
@param aTimeRef Whether to generate the rules relative to UTC or local time. Use 
ETzUtcTimeReference to generate from UTC, ETzWallTimeReference to generate from local time.
@return Pointer to the time zone rules generated.
*/
EXPORT_C CTzRules* RTz::GetTimeZoneRulesL(const TTime& aStartTime, const TTime& aEndTime, TTzTimeReference aTimeRef) const
	{
	TInt rulesSize = 0;
	TPckg<TTime> startTimeBuffer(aStartTime);
	TPckg<TTime> endTimeBuffer(aEndTime);
	TPckg<TTzTimeReference> timeRefBuffer(aTimeRef);
	TPckg<TInt> rulesSizeBuffer(rulesSize);
	TIpcArgs args(&startTimeBuffer, &endTimeBuffer, &timeRefBuffer, &rulesSizeBuffer);
	const TInt result = SendReceive(CTzServer::EGetLocalEncodedTimeZoneRulesSize, args);
	User::LeaveIfError(result);
	
	// prepare buffer for tz rules	
	CBufFlat* inBuffer = CBufFlat::NewL(rulesSize);
	CleanupStack::PushL(inBuffer);  // push #1
	inBuffer->ExpandL(0, rulesSize);
	TPtr8 inPtr(inBuffer->Ptr(0) );
	
	// get the rules
	TIpcArgs args2(&rulesSizeBuffer,&inPtr);
	const TInt result2 = SendReceive(CTzServer::EGetLocalEncodedTimeZoneRules, args2);
	User::LeaveIfError(result2);
	
	RBufReadStream readStream;
	readStream.Open(*inBuffer);
	
	CTzRules* rules = CTzRules::NewL(readStream);
	
	CleanupStack::PopAndDestroy(inBuffer); // pop #1
	return rules;
	}
	
/**
Retrieves the time zone rules for a specified time zone.

@param aZone The time zone to generate the rules for.
@param aStartTime The start date to generate the time zone rules from.
@param aEndTime The end date to generate the time zone rules to.
@param aTimeRef Whether to generate the rules relative to UTC or local time. Use 
ETzUtcTimeReference to generate from UTC, ETzWallTimeReference to generate from local time.
@return Pointer to the time zone rules generated.
*/
EXPORT_C CTzRules* RTz::GetTimeZoneRulesL(const CTzId& aZone, const TTime& aStartTime, const TTime& aEndTime, TTzTimeReference aTimeRef) const
	{
	TInt rulesSize = 0;
	TPckg<TTime> startTimeBuffer(aStartTime);
	TPckg<TTime> endTimeBuffer(aEndTime);
	TPckg<TInt> rulesSizeBuffer(rulesSize);
		
	// prepare out buffer for zoneid and timeref
	CBufFlat* outBuffer = CBufFlat::NewL(KMaxTimeZoneIdSize + sizeof(TTzTimeReference)); 
	CleanupStack::PushL(outBuffer); // push #1
	
	RBufWriteStream writeStream;
	writeStream.Open(*outBuffer);
	aZone.ExternalizeL(writeStream);
	writeStream.WriteInt8L(aTimeRef);
	TPtr8 outPtr(outBuffer->Ptr(0) );
	
	TIpcArgs args(&startTimeBuffer, &endTimeBuffer, &outPtr, &rulesSizeBuffer);
	const TInt result = SendReceive(CTzServer::EGetForeignEncodedTimeZoneRulesSize, args);
	User::LeaveIfError(result);
	
	CleanupStack::PopAndDestroy(outBuffer); // pop #1
	
	// prepare in rules buffer
	CBufFlat* inBuffer = CBufFlat::NewL(rulesSize);
	CleanupStack::PushL(inBuffer); // push #1
	inBuffer->ExpandL(0, rulesSize);
	TPtr8 inPtr(inBuffer->Ptr(0) );
	
	TIpcArgs args2(&rulesSizeBuffer, &inPtr);
	const TInt result2 = SendReceive(CTzServer::EGetForeignEncodedTimeZoneRules, args2);
	User::LeaveIfError(result2);
	
	RBufReadStream readStream;
	readStream.Open(*inBuffer);
	
	CTzRules* rules = CTzRules::NewL(readStream);
	
	CleanupStack::PopAndDestroy(inBuffer); // pop #1
	return rules;
	}
	
// Registers to receive notification to time and zone changes in the
// Server
void RTz::RegisterTzChangeNotifier(TRequestStatus& aStatus) const
	{
	SendReceive(CTzServer::ERegisterTimeChangeNotifier, aStatus);
	}

// Cancels a previous request for notification of time zone changes
TInt RTz::CancelRequestForNotice() const
	{
	return (SendReceive(CTzServer::ECancelRequestforNotice));
	}

// Converts time between UTC and Local for the current selected system time zone
void RTz::doConvertL(TTime& aTime, TTzTimeReference aTimerRef) const
	{

	TPckg<TTime> timeBuffer(aTime);
	TPckg<TTime> timeInBuffer(aTime);
	TPckg<TTzTimeReference> timerRefBuffer(aTimerRef);

	TIpcArgs args(&timeBuffer, &timerRefBuffer, &timeInBuffer);
	const TInt result = SendReceive(CTzServer::EConvertLocalZoneTime, args);
	User::LeaveIfError(result);

	aTime = timeInBuffer();
	}

// Converts time between UTC and Local for a foreing time zone
void RTz::doConvertL(const CTzId& aZone,
								TTime& aTime,
								TTzTimeReference aTimerRef) const
	{
	// prepare out zoneid buffer
	CBufFlat* outBuffer = CBufFlat::NewL(KMaxTimeZoneIdSize);
	CleanupStack::PushL(outBuffer);

	RBufWriteStream writeStream;
	writeStream.Open(*outBuffer);
	aZone.ExternalizeL(writeStream);
	TPtr8 outPtr(outBuffer->Ptr(0) );

	TPckg<TTime> timeBuffer(aTime);
	TPckg<TTime> timeInBuffer(aTime);
	TPckg<TTzTimeReference> timerRefBuffer(aTimerRef);

	TIpcArgs args(&timeBuffer, &timerRefBuffer, &outPtr, &timeInBuffer);
	const TInt result = SendReceive(CTzServer::EConvertForeignZoneTime, args);
	CleanupStack::PopAndDestroy(outBuffer);

	User::LeaveIfError(result);
	aTime = timeInBuffer();
	}

/**
Constructor.
*/
EXPORT_C RTz::RTz() :
    iRulesHolder (NULL)
	{
	}

/**
Destructor. Calls Close().
*/
EXPORT_C RTz::~RTz()
	{
	Close();
	}

/** 
Connects to the time zone server, attempting to start it if necessary.
@return KErrNone if successful, otherwise a system error code.
@panic TzServer 1 The attempt to connect to the server failed with an error 
code other than KErrNotFound.
*/
EXPORT_C TInt RTz::Connect()
	{
	// The number of message slots in the RSessionBase::CreateSession() 
	// call are delibrately unspecified so the kernel will take messages 
	// from a global pool as and when required (the session is allowed
	// a maximum of 255 outstanding asynchronous messages)

	TInt retry = KNumConnectRetries;
	FOREVER
		{
		TInt err = CreateSession(KTimeZoneServerName,Version());
		if (err != KErrNotFound && err!=KErrServerTerminated)
			{
			return err;
			}
		// need to restart server
		if (--retry == 0)
			{
			return err;
			}
		err = StartServer();
		if (err != KErrNone && err != KErrAlreadyExists)
			{
			// Time Zone Server is configurable, so it is possible not to find it
			if (err != KErrNotFound)
				{
				return err;
				}
			Panic(EPanicServerNotFound);
			}
		}
	}

/**
Closes the connection to the time zone server.
*/
EXPORT_C void RTz::Close()
	{
	// Destroy rules before closing session.
    delete iRulesHolder;
    iRulesHolder = NULL;
    
	if (Handle())
		{
		
		RSessionBase::Close();
		}
	}

/** 
Converts a time from UTC to local (wall-clock) time for the current system 
time zone. If caching has been enabled by the CTzConverter class, then the 
conversion will be done using the cache instead of issuing a request to the 
server.

@return KErrNone if successful, otherwise another system error code.
@param aTime On entry, contains the UTC time to be converted,
and the converted value on exit. 
*/
EXPORT_C TInt RTz::ConvertToLocalTime(TTime& aTime) const
	{

    // Use cached method for conversion if caching enabled, otherwise request 
    // server to do the conversion.
    TInt result = KErrNone;
	if (iRulesHolder)
    	{
        TRAP( result, iRulesHolder->DoConvertL( aTime, ETzUtcTimeReference ) );
    	}
    else
        {
    	TRAP(result, doConvertL(aTime, ETzUtcTimeReference));
        }
	return result;
	}

/** 
Converts a time from UTC to the local (wall-clock) time 
for the specified time zone.
@return KErrNone if successful, otherwise another system error code.
@param aTime On entry, contains the UTC time to be converted,
and the converted value on exit.
@param aZone Time zone ID.
*/
EXPORT_C TInt RTz::ConvertToLocalTime(TTime& aTime, const CTzId& aZone) const
	{
	TRAPD(result, doConvertL(aZone, aTime, ETzUtcTimeReference));
	return result;
	}

/** 
Converts a local (wall-clock) time for the current system time zone, to UTC.
If caching has been enabled by the CTzConverter class, then the conversion will
be done using the cache instead of issuing a request to the server.

@param aTime On entry, contains the local time to be converted, and 
the converted value on exit. 
@return KErrNone if successful, otherwise another system error code.
*/
EXPORT_C TInt RTz::ConvertToUniversalTime(TTime& aTime) const
	{
    // Use cached method for conversion if caching enabled, otherwise request 
    // server to do the conversion.
    TInt result = KErrNone;
	if (iRulesHolder)
    	{
        TRAP( result, iRulesHolder->DoConvertL( aTime, ETzWallTimeReference ) );
        }
    else
        {
    	TRAP(result, doConvertL(aTime, ETzWallTimeReference));
        }
	return result;
	}

/** 
Converts a local (wall-clock) time for the specified time zone, to UTC.
@param aTime On entry, contains the local time to be converted, and 
the converted value on exit. 

@param aZone The time zone ID of interest.
@return KErrNone if successful, otherwise another system error code.
*/
EXPORT_C TInt RTz::ConvertToUniversalTime(TTime& aTime, const CTzId& aZone) const
	{
	TRAPD(result, doConvertL(aZone, aTime, ETzWallTimeReference));
	return result;
	}

/**
Retrieves the UTC offset for an array of numeric time zone ids.
The offset is written back into aOffsets.
@param aTzNumericIds An array of numeric time zone IDs for which the current
UTC offset is required.
@param aOffsets An array that, on return, will contain the UTC offsets
corresponding to the the time zone IDs in aTzNumericIds.
*/
EXPORT_C void RTz::GetOffsetsForTimeZoneIdsL(const RArray<TInt>& aTzNumericIds, RArray<TInt>& aOffsets) const
	{
	TInt count 			= aTzNumericIds.Count();
	TInt tintSize 		= sizeof(TInt);
	TInt idBufLength	= count * tintSize;
	TInt pos 			= 0;
	idBufLength += tintSize;
	TInt x;
	//Write the number of elements in the array, followed by
	//the time zone id of each element into idBuf.
	CBufFlat* idBuf = CBufFlat::NewL(idBufLength);
	CleanupStack::PushL(idBuf);
	idBuf->ExpandL(pos,idBufLength);
		
	idBuf->Write(pos,&count,tintSize);
	pos += tintSize;
	
	for (x = 0; x < count; ++x)
		{
		TInt id = aTzNumericIds[x];
		idBuf->Write(pos,&id,tintSize);
		pos += tintSize ;
		}

	TInt size = idBuf->Size();
	TPtr8 outPtr(idBuf->Ptr(0) );
	TIpcArgs args(size,&outPtr);
	const TInt result = SendReceive(CTzServer::EGetOffsetsForTimeZoneIds, args);	
	User::LeaveIfError(result);
	
	// idBuf now contains the offsets INSTEAD of the timezone Ids
	pos = 0;
	count = 0;
	TInt offset = 0;
	idBuf->Read(pos,&count,tintSize);
	pos += tintSize;
	//Loop through aArray again, setting the offsets from idBuf
	for (x = 0; x < count; ++x)
		{
		idBuf->Read(pos,&offset,tintSize);
		aOffsets.AppendL(offset);
		pos += tintSize;
		}
	CleanupStack::PopAndDestroy(idBuf);
	}

/** 
Retrieves the time zone ID for the current system time zone.
@return Time zone ID. The client takes ownership of the object.
@leave KErrNoMemory or another system leave code.
@internalTechnology
*/
EXPORT_C CTzId* RTz::GetTimeZoneIdL() const
	{
	CBufFlat* inBuffer = CBufFlat::NewL(KMaxTimeZoneIdSize);
	CleanupStack::PushL(inBuffer);

	// reserve the full space
	inBuffer->ExpandL(0, KMaxTimeZoneIdSize);

	TPtr8 ptr(inBuffer->Ptr(0) );

	TIpcArgs args(&ptr);
	const TInt result = SendReceive(CTzServer::EGetLocalTimeZoneId, args);
	User::LeaveIfError(result);

	RBufReadStream readStream;
	readStream.Open(*inBuffer);
	CTzId* zone = CTzId::NewL(readStream);
	CleanupStack::PopAndDestroy(inBuffer);
	return zone;
	}

/** 
Sets the current system time zone information to that corresponding to the 
supplied time zone ID.
@param aZone The time zone ID.
@capability WRITE_DEVICE_DATA
@internalTechnology
*/
EXPORT_C void RTz::SetTimeZoneL(CTzId& aZone) const
	{
	CBufFlat* outBuffer = CBufFlat::NewL(KMaxTimeZoneIdSize);
	CleanupStack::PushL(outBuffer);

	RBufWriteStream writeStream;
	writeStream.Open(*outBuffer);

	aZone.ExternalizeL(writeStream);

	writeStream.CommitL();
	writeStream.Close();

	CBufFlat* inBuffer = CBufFlat::NewL(KMaxTimeZoneIdSize);
	CleanupStack::PushL(inBuffer);

	// reserve the full space
	inBuffer->ExpandL(0, KMaxTimeZoneIdSize);

	TPtr8 outPtr(outBuffer->Ptr(0) );
	TPtr8 inPtr(inBuffer->Ptr(0) );
	TIpcArgs args(&outPtr, &inPtr);
	const TInt result = SendReceive(CTzServer::ESetTimeZone, args);
	User::LeaveIfError(result);

	RBufReadStream readStream;
	readStream.Open(*inBuffer);
	aZone.InternalizeL(readStream);

	CleanupStack::PopAndDestroy(2, outBuffer);

    // The time zone changed. Update cache - it is no longer valid.
	if (iRulesHolder)
	    {
	    // This shouldn't leave, it's an implementation error if it does. 
	    TRAP_IGNORE( iRulesHolder->NotifyTimeZoneChangeL( 
	        ETZSystemTimeZoneChanged ) ); 
	    }
	}
	
/**
Tells if daylight savings are applied at the specified zone at the current time

@param aZone The time zone ID.
@return ETrue if the Daylight Saving is on, i.e. if it is summertime.
*/	
EXPORT_C TBool RTz::IsDaylightSavingOnL(CTzId& aZone) const
	{
	// prepare out zoneid buffer
	CBufFlat* outBuffer = CBufFlat::NewL(KMaxTimeZoneIdSize);
	CleanupStack::PushL(outBuffer);

	RBufWriteStream writeStream;
	writeStream.Open(*outBuffer);
	aZone.ExternalizeL(writeStream);
	
	TPckg<TTime> timeBuffer(Time::NullTTime());
	
	TPtr8 outPtr(outBuffer->Ptr(0) );
	TPckgBuf<TInt> package;
	TIpcArgs args(&outPtr, &timeBuffer, &package);
	const TInt result = SendReceive(CTzServer::EIsDaylightSavingOn, args);
	User::LeaveIfError(result);
	CleanupStack::PopAndDestroy(outBuffer);
	return package();		
	}

/**
Tells if daylight savings are applied at the specified zone at a specified time

@param aZone The time zone ID.
@param aUTCTime The UTC time at which DST settings are required.
@return ETrue if the Daylight Saving is on, i.e. if it is summertime.
*/	
EXPORT_C TBool RTz::IsDaylightSavingOnL(CTzId& aZone, const TTime& aUTCTime) const
	{
	// prepare out zoneid buffer
	CBufFlat* outBuffer = CBufFlat::NewL(KMaxTimeZoneIdSize);
	CleanupStack::PushL(outBuffer);

	RBufWriteStream writeStream;
	writeStream.Open(*outBuffer);
	aZone.ExternalizeL(writeStream);
		
	TPckg<TTime> timeBuffer(aUTCTime);	
	
	TPtr8 outPtr(outBuffer->Ptr(0) );
 	TPckgBuf<TInt> package;
	TIpcArgs args(&outPtr, &timeBuffer, &package);
 	const TInt result = SendReceive(CTzServer::EIsDaylightSavingOn, args);
 	User::LeaveIfError(result);
 	CleanupStack::PopAndDestroy(outBuffer);
 	return package();		
 	}

/** 
Panics the TzClient or TzServer code.

@internalComponent
*/
EXPORT_C void RTz::Panic(TPanic aPanic)
	{
	_LIT(KTzServer,"TzServer");
	User::Panic(KTzServer, aPanic);
	}

/**
Sets the configuration of the UTC Offset auto-update functionality.

@param aUpdateEnabled If set to RTz::ETZAutoDSTUpdateOn then the UTC Offset is automatically
updated for changes to Daylight Savings Time. If set to RTz::ETZAutoDSTUpdateOff then auto-update
is disabled. The RTz::ETZAutoDSTNotificationOnly - Means that the client app needs to confirm that the 
time should be updated whenever a DST event occurs.

*/
EXPORT_C void RTz::SetAutoUpdateBehaviorL(TTzAutoDSTUpdateModes aUpdateEnabled)
    {
    // Create the message arguments, setting slot zero to the configuration value.
	TIpcArgs args(aUpdateEnabled);
	
	// Send the message.
	User::LeaveIfError(SendReceive(CTzServer::EEnableAutoUpdate, args));
    }

/**
Retrieves the daylight saving auto-update functionality.
@return An error code. KErrNone is expected unless there is an error while trying to retrieve the auto-update setting.
*/
EXPORT_C TInt RTz::AutoUpdateSettingL()
    {
    TPckgBuf<TInt> package;
	TIpcArgs args(&package);
	const TInt result = SendReceive(CTzServer::EAutoUpdate, args);
	User::LeaveIfError(result);
	return package();	
    }
        
/**
Sets the system time to the given local time. Note that the standard Tz
algorithm is used - in the case of an ambiguous time the first occurance of the
given local wall-clock time is chosen.

An example of an ambiguous time: 01:30 wall-clock time on 31/10/2004 in the UK
could map to either 01:30 BST (00:30 GMT) or 01:30 GMT, as the clocks would go
back at 02:00 BST (01:00 GMT))

@capability WriteDeviceData
@param aLocalTime The time to set in wall-clock time.

@return An error code. KErrNone is expected unless there is an error in
converting the given local time to UTC.

*/
EXPORT_C TInt RTz::SetHomeTime(const TTime& aLocalTime) const
    {
	TPckg<TTime>   timeBuffer(aLocalTime);

	TIpcArgs args(&timeBuffer);
	return SendReceive(CTzServer::ESetHomeTime, args);
    }

/**
Publish notification that the home time zone (as defined in CTzLocalizer) has been changed.
This function should only be called from TimeZoneLocalization.

@param aChange Time zone change information.
*/
EXPORT_C void RTz::NotifyHomeTimeZoneChangedL(const NTzUpdate::TTimeZoneChange& aChange) const
	{
	TIpcArgs args(aChange.iNewTimeZoneId, aChange.iOldTimeZoneId);
	
	// Send the message.
	User::LeaveIfError(SendReceive(CTzServer::ENotifyHomeTimeZoneChanged, args));
	}

/**
Sets the UTC time and the wall-clock offset for a zone not defined in the
Timezone Database.

This is useful when the client wants to specify the UTC time and the wall-clock offset explicitly
rather than using time zones. This function should be used instead of User::SetUTCOffset() and 
User::SetUTCTimeAndOffset().

If this function has been called the time zone server will use the value returned by User::UTCOffset()
as the offset value when doing time conversions. Calling User::SetUTCOffset() may therefore cause unexpected
results.

The fact that the time zone has been set to the unknown time zone is not persisted across time zone server 
reboots. So it may be necessary to call this function each time the time zone 
server is started. If persistence across reboots is desired the overload 
RTz::SetUnknownZoneTimeL(const TTime& aUTCTime, const TInt aUTCOffset, TBool aPersistInCenRep) 
should be used.

@param aUTCTime Universal Time to set the device time to.
@param aUTCOffset Offset (in minutes) in use in the time zone.
*/
EXPORT_C void RTz::SetUnknownZoneTimeL(const TTime& aUTCTime, const TInt aUTCOffset)
	{
	SetUnknownZoneTimeL(aUTCTime, aUTCOffset, EFalse);
	}

/**
Sets the UTC time and the wall-clock offset for a zone not defined in the
Timezone Database.

This is useful when the client wants to specify the UTC time and the wall-clock offset explicitly
rather than using time zones. This function should be used instead of User::SetUTCOffset() and 
User::SetUTCTimeAndOffset().

If this function has been called the time zone server will use the value returned by User::UTCOffset()
as the offset value when doing time conversions. Calling User::SetUTCOffset() may therefore cause unexpected
results.

The fact that the time zone has been set to the unknown time zone is persisted across time zone server 
reboots only if the aPersistInCenRep argument is set to ETrue. The desired offset however is never persisted 
(the value returned by User::UTCOffset() is used). So it may be necessary to call this function each time 
the time zone server is started.

@param aUTCTime Universal Time to set the device time to.
@param aUTCOffset Offset (in minutes) in use in the time zone.
@param aPersistInCenRep If ETrue the unknown time zone is persisted across reboots else it is not.
*/
EXPORT_C void RTz::SetUnknownZoneTimeL(const TTime& aUTCTime, const TInt aUTCOffset, TBool aPersistInCenRep)
	{
	TPckg<TTime> utcTime(aUTCTime);   
	TPckg<TInt>  utcOffset(aUTCOffset);
	TPckg<TBool> persist(aPersistInCenRep);

	TIpcArgs args(&utcTime, &utcOffset, &persist);
	User::LeaveIfError(SendReceive(CTzServer::ESetUnknownZoneTime, args));
	}

/**
Reads all the user defined cities stored in the
database. If the database is empty the
returned array will contain no elements.
@param aCities Array of cities to add the user defined to.
@internalTechnology
*/
EXPORT_C void RTz::LocalizationReadCitiesL(RPointerArray<CTzLocalizedCityRecord>& aCities)
	{
	TInt resultSize = 0;
	TPckg<TInt> resultSizeBuffer(resultSize);
	TIpcArgs args(&resultSizeBuffer);
	User::LeaveIfError(SendReceive(CTzServer::ELocalizationReadCitiesSize, args));
		
	CBufFlat* inBuffer = CBufFlat::NewL(resultSize);
	CleanupStack::PushL(inBuffer);
	inBuffer->ExpandL(0, resultSize);
	TPtr8 inPtr(inBuffer->Ptr(0));
	
	TIpcArgs args2(&inPtr);
	User::LeaveIfError(SendReceive(CTzServer::ELocalizationReadCities, args2));
	
	RBufReadStream readStream;
	readStream.Open(*inBuffer);
	CleanupClosePushL(readStream);
	CTzLocalizedCityRecord::InternalizeL(readStream, aCities);
	CleanupStack::PopAndDestroy(&readStream);
	
	CleanupStack::PopAndDestroy(inBuffer);
	}

/**
Reads all the user defined cities stored in the database that are members of
the time zone referenced by aTimeZoneid.
If the database is empty the returned array will contain no elements.
@param aCities Array of cities to add the user defined cities to.
@param aTimeZoneId A time zone id identifying a time zone
@internalTechnology
*/
EXPORT_C void RTz::LocalizationReadCitiesL(RPointerArray<CTzLocalizedCityRecord>& aCities, TInt aTimeZoneId)
	{
	TInt resultSize = 0;
	TPckg<TInt> resultSizeBuffer(resultSize);
	TIpcArgs args(&resultSizeBuffer, aTimeZoneId);
	User::LeaveIfError(SendReceive(CTzServer::ELocalizationReadCitiesTzIdSize, args));
		
	CBufFlat* inBuffer = CBufFlat::NewL(resultSize);
	CleanupStack::PushL(inBuffer);
	inBuffer->ExpandL(0, resultSize);
	TPtr8 inPtr(inBuffer->Ptr(0));
	
	TIpcArgs args2(&inPtr, aTimeZoneId);
	User::LeaveIfError(SendReceive(CTzServer::ELocalizationReadCities, args2));
		
	RBufReadStream readStream;
	readStream.Open(*inBuffer);
	CleanupClosePushL(readStream);
	CTzLocalizedCityRecord::InternalizeL(readStream, aCities);
	CleanupStack::PopAndDestroy(&readStream);
	
	CleanupStack::PopAndDestroy(inBuffer);	
	}

/**
Reads all the user defined cities stored in the database with a matching group id.
If the database is empty the returned array will contain no elements.
@param aCities Array of cities to add the user defined cities to.
@param aGroupId A city group id
@internalTechnology
*/
EXPORT_C void RTz::LocalizationReadCitiesInGroupL(RPointerArray<CTzLocalizedCityRecord>& aCities, TUint8 aGroupId)
	{
	TInt resultSize = 0;
	TPckg<TInt> resultSizeBuffer(resultSize);
	TIpcArgs args(&resultSizeBuffer, aGroupId);
	User::LeaveIfError(SendReceive(CTzServer::ELocalizationReadCitiesInGroupSize, args));
		
	CBufFlat* inBuffer = CBufFlat::NewL(resultSize);
	CleanupStack::PushL(inBuffer);
	inBuffer->ExpandL(0, resultSize);
	TPtr8 inPtr(inBuffer->Ptr(0));
	
	TIpcArgs args2(&inPtr, aGroupId);
	User::LeaveIfError(SendReceive(CTzServer::ELocalizationReadCities, args2));
		
	RBufReadStream readStream;
	readStream.Open(*inBuffer);
	CleanupClosePushL(readStream);
	CTzLocalizedCityRecord::InternalizeL(readStream, aCities);
	CleanupStack::PopAndDestroy(&readStream);
	
	CleanupStack::PopAndDestroy(inBuffer);
	}

/**
Retrieves a cached zone from the database.
The calling function takes ownership of the returned zone
@param aFrequentlyUsedZone - The cached zone to return
@return a cached zone from the database
@internalTechnology
*/
EXPORT_C CTzLocalizedTimeZoneRecord* RTz::LocalizationReadFrequentlyUsedZoneL(TInt aFrequentlyUsedZone)
	{
	TInt resultSize = 0;
	TPckg<TInt> resultSizeBuffer(resultSize);
	TIpcArgs args(&resultSizeBuffer, aFrequentlyUsedZone);
	User::LeaveIfError(SendReceive(CTzServer::ELocalizationReadFrequentlyUsedZoneSize, args));
		
	CBufFlat* inBuffer = CBufFlat::NewL(resultSize);
	CleanupStack::PushL(inBuffer);
	inBuffer->ExpandL(0, resultSize);
	TPtr8 inPtr(inBuffer->Ptr(0));
	
	TIpcArgs args2(&inPtr, aFrequentlyUsedZone);
	User::LeaveIfError(SendReceive(CTzServer::ELocalizationReadFrequentlyUsedZone, args2));
		
	RBufReadStream readStream;
	readStream.Open(*inBuffer);
	CleanupClosePushL(readStream);
	CTzLocalizedTimeZoneRecord* result = CTzLocalizedTimeZoneRecord::NewL(readStream);
	CleanupStack::PopAndDestroy(&readStream);
	
	CleanupStack::PopAndDestroy(inBuffer);	
	
	return result;
	}

/**
Retrieves the city used to select this time zone if set.  If the time zone was not originally
set using a city then the default city for the time zone will be returned instead.
@param aFrequentlyUsedZone - The cached zone to find the city for
@return	 selected city for the cached zone
@internalTechnology
*/
EXPORT_C CTzLocalizedCityRecord* RTz::LocalizationReadCachedTimeZoneCityL(TInt aFrequentlyUsedZone)
	{
	TInt resultSize = 0;
	TPckg<TInt> resultSizeBuffer(resultSize);
	TIpcArgs args(&resultSizeBuffer, aFrequentlyUsedZone);
	User::LeaveIfError(SendReceive(CTzServer::ELocalizationReadCachedTimeZoneCitySize, args));
		
	CBufFlat* inBuffer = CBufFlat::NewL(resultSize);
	CleanupStack::PushL(inBuffer);
	inBuffer->ExpandL(0, resultSize);
	TPtr8 inPtr(inBuffer->Ptr(0));
	
	TIpcArgs args2(&inPtr, aFrequentlyUsedZone);
	User::LeaveIfError(SendReceive(CTzServer::ELocalizationReadCachedTimeZoneCity, args2));
		
	RBufReadStream readStream;
	readStream.Open(*inBuffer);
	CleanupClosePushL(readStream);
	CTzLocalizedCityRecord* result = CTzLocalizedCityRecord::NewL(readStream);
	CleanupStack::PopAndDestroy(&readStream);
	
	CleanupStack::PopAndDestroy(inBuffer);	
	
	return result;
	}

EXPORT_C void RTz::LocalizationCloseDbL()
	{
	User::LeaveIfError(SendReceive(CTzServer::ELocalizationCloseDb));
	}

EXPORT_C void RTz::LocalizationOpenDbL()
	{
	User::LeaveIfError(SendReceive(CTzServer::ELocalizationOpenDb));
	}

/**
Adds aCity into the user added cities database table. The city is checked first to prevent
two cities with identical names and time zones existing at the same time. Name checking is
case sensitive. Cities in different time zones can have the same name.

@param aCityName The name of the city to add to the database
@param aCityTzId The time zone of the city
@param aCityGroupId The group id
@param aCityResourceId The resource id

@internalTechnology
*/
EXPORT_C void RTz::LocalizationWriteCityL(const TDesC& aCityName, TInt aCityTzId, TUint8 aCityGroupId, TUint aCityResourceId)
	{
	if (aCityTzId > 0xFFFF)
		{
		// This is a mess, although most of the API allows for 32 bit tz ids some of them
		// only allow for 16 bit tz ids so we accept a TInt but do check that it can fit in 
		// 16 bits
		User::Leave(KErrArgument);
		}
	TIpcArgs args(&aCityName, aCityTzId, aCityGroupId, aCityResourceId);
	User::LeaveIfError(SendReceive(CTzServer::ELocalizationWriteCity, args));
	}

/**
Deletes aCity from the user added cities database table.
The city is checked first to ensure it exists.
Name checking is case sensitive.

@param aCityName The name of the city to delete from the database.
@param aCityTzId The numeric identifier of the city to delete from the database.

@internalTechnology
*/
EXPORT_C void RTz::LocalizationDeleteCityL(const TDesC& aCityName, TInt aCityTzId)
	{
	if (aCityTzId > 0xFFFF)
		{
		// This is a mess, although most of the API allows for 32 bit tz ids some of them
		// only allow for 16 bit tz ids so we accept a TInt but do check that it can fit in 
		// 16 bits
		User::Leave(KErrArgument);
		}
	TIpcArgs args(&aCityName, aCityTzId);
	User::LeaveIfError(SendReceive(CTzServer::ELocalizationDeleteCity, args));
	}

/**
Stores the localized time zone in the database for easy retrieval.
The database keeps track of the two most recently used zones, these are updated every time
a new zone is set.  The zone to be replaced becomes recently used zone 1, and recently used zone 1 becomes
recently used zone 2.  The old recently used zone 2 is discarded.

@param aTimeZone Time zone names information to store in the given frequently
used time zone.
@param aCity City associated with the time zone names to store in the given
frequently used time zone.
@param aFrequentlyUsedZone Which frequently used time zone to overwrite in the
database.

@internalTechnology
*/
EXPORT_C void RTz::LocalizationWriteFrequentlyUsedZoneL(const CTzLocalizedTimeZoneRecord& aTimeZone,
	const CTzLocalizedCityRecord& aCity, TInt aFrequentlyUsedZone)
	{
	TInt bufferSize = aTimeZone.ExternalizeSize() + aCity.ExternalizeSize() + sizeof(TInt32);
	CBufFlat* buffer = CBufFlat::NewL(bufferSize);
	CleanupStack::PushL(buffer);
	RBufWriteStream bufStream;
	bufStream.Open(*buffer);
	CleanupClosePushL(bufStream);
	aTimeZone.ExternalizeL(bufStream);
	aCity.ExternalizeL(bufStream);
	bufStream.WriteInt32L(aFrequentlyUsedZone);
	bufStream.CommitL();
	CleanupStack::PopAndDestroy(&bufStream);
	TPtr8 bufferPtr = buffer->Ptr(0); 
	TIpcArgs args(&bufferPtr);
	User::LeaveIfError(SendReceive(CTzServer::ELocalizationWriteFrequentlyUsedZone, args));
	CleanupStack::PopAndDestroy(buffer);
	}

/**
@internalTechnology
*/
EXPORT_C void RTz::LocalizationWriteAllFrequentlyUsedZonesL(const RPointerArray<CTzLocalizedTimeZoneRecord>& aTimeZones,
		const RPointerArray<CTzLocalizedCityRecord>& aCities)
	{
	TInt bufferSize = CTzLocalizedTimeZoneRecord::ExternalizeSize(aTimeZones);
	bufferSize += CTzLocalizedCityRecord::ExternalizeSize(aCities);
	CBufFlat* buffer = CBufFlat::NewL(bufferSize);
	CleanupStack::PushL(buffer);
	RBufWriteStream bufStream;
	bufStream.Open(*buffer);
	CleanupClosePushL(bufStream);
	CTzLocalizedTimeZoneRecord::ExternalizeL(aTimeZones, bufStream);
	CTzLocalizedCityRecord::ExternalizeL(aCities, bufStream);
	bufStream.CommitL();
	CleanupStack::PopAndDestroy(&bufStream);
	TPtr8 bufferPtr = buffer->Ptr(0); 
	TIpcArgs args(&bufferPtr);
	User::LeaveIfError(SendReceive(CTzServer::ELocalizationWriteAllFrequentlyUsedZones, args));
	CleanupStack::PopAndDestroy(buffer);
	}

/**
Called by the installation observer plugin to signal the start of an (un)install log.

@internalTechnology
*/
EXPORT_C void RTz::SwiObsBeginL()
	{
	User::LeaveIfError(SendReceive(CTzServer::ESwiObsBegin));
	}

/**
Called by the installation observer plugin to signal that the rules database or
the resource files have been changed.

@param aType The type of file that has changed (resource or rules database).

@internalTechnology
*/
EXPORT_C void RTz::SwiObsFileChangedL(TSWIObserverFilterIndex aType)
	{
	TIpcArgs args(aType);
	User::LeaveIfError(SendReceive(CTzServer::ESwiObsFileChanged, args));
	}

/**
Called by the installation observer plugin to signal the end of an (un)install log.
 
@internalTechnology
*/
EXPORT_C void RTz::SwiObsEndL()
	{
	User::LeaveIfError(SendReceive(CTzServer::ESwiObsEnd));
	}

// Enables client side caching of time zone conversions for the local time zone
// if caching is not already enabled. Must be connected to the server before 
// calling.
// Returns ETrue if caching was enabled, EFalse if caching was already enabled.
TBool RTz::StartCachingL()
    {
    if (!iRulesHolder)
        {
        iRulesHolder = CTzRuleHolder::NewL( *this );
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }
    
// Gets the time zone id associated with the cache. If caching is
// disabled then this returns 0.
// Returns the timezone id.
//
TUint16 RTz::CurrentCachedTzId()
	{
	if (iRulesHolder) 
	    {
	    return iRulesHolder->CurrentTzId();
	    }
	else
		{
        return 0;
        }
	} 



/*********************************User Defined Time Zone Handling *****************/

CTzId* RTz::CreateUserTimeZoneL(const CTzRules& aTzUserRules, const CTzUserNames& aTzUserNames)
	{
	TInt size = aTzUserNames.SizeOfObject() + aTzUserRules.SizeOfObject();

	CBufFlat* inpBuffer = CBufFlat::NewL(size);
	CleanupStack::PushL(inpBuffer);
	inpBuffer->ExpandL(0, size);

	RBufWriteStream writeStream;
	CleanupClosePushL(writeStream);
	writeStream.Open(*inpBuffer);
	aTzUserRules.ExternalizeL(writeStream);
	aTzUserNames.ExternalizeL(writeStream);
	writeStream.CommitL();
	
	TPtr8 ptrInp(inpBuffer->Ptr(0));
	TPckgBuf<TUint32> idBuffer;
	TIpcArgs args(size, &ptrInp, &idBuffer);
	User::LeaveIfError(SendReceive(CTzServer::ECreateUserTimeZone, args));
	CleanupStack::PopAndDestroy(2, inpBuffer);
	return CTzId::NewL(idBuffer());
	}

CTzUserNames* RTz::GetUserTimeZoneNamesL(const CTzId& aTzId) const
	{
	TInt nameSize = 0;
	TPckg<TInt> nameSizeBuffer(nameSize);

	TIpcArgs args(aTzId.TimeZoneNumericID(),&nameSizeBuffer);
	User::LeaveIfError(SendReceive(CTzServer::EGetUserTimeZoneNamesSize, args));

	CBufFlat* retBuffer = CBufFlat::NewL(nameSize);
	CleanupStack::PushL(retBuffer);
	retBuffer->ExpandL(0, nameSize);
	TPtr8 outPtr(retBuffer->Ptr(0) );
	
	TIpcArgs args2(&outPtr);
	User::LeaveIfError(SendReceive(CTzServer::EGetUserTimeZoneNames, args2));
	
	RBufReadStream readStream;
	CleanupClosePushL(readStream);
	readStream.Open(*retBuffer);
	
	CTzUserNames* names = CTzUserNames::NewL(readStream);
	CleanupStack::PopAndDestroy(2, retBuffer); 
	return names;
	}
	
void RTz::UpdateUserTimeZoneL(const CTzId& aTzId, const CTzRules& aTzUserRules, const CTzUserNames& aTzUserNames)
	{
	TInt size = aTzUserNames.SizeOfObject() + aTzUserRules.SizeOfObject();
	
	CBufFlat* inpBuffer = CBufFlat::NewL(size);
	CleanupStack::PushL(inpBuffer);
	inpBuffer->ExpandL(0, size);

	RBufWriteStream writeStream;
	CleanupClosePushL(writeStream);
	writeStream.Open(*inpBuffer);
	aTzUserRules.ExternalizeL(writeStream);
	aTzUserNames.ExternalizeL(writeStream);
	writeStream.CommitL();
	
	TPtr8 ptrInp(inpBuffer->Ptr(0) );
	TIpcArgs args(size, &ptrInp, aTzId.TimeZoneNumericID());
	TInt reply = SendReceive(CTzServer::EUpdateUserTimeZone, args);
	User::LeaveIfError(reply);
	CleanupStack::PopAndDestroy(2, inpBuffer);
	}
	
void RTz::DeleteUserTimeZoneL(const CTzId& aTzId)
	{
	TIpcArgs args(aTzId.TimeZoneNumericID());
	User::LeaveIfError(SendReceive(CTzServer::EDeleteUserTimeZone, args));
	}
	
void RTz::GetUserTimeZoneIdsL(RPointerArray<CTzId>& aTzIds) const
	{
	TInt idsSize = 0;
	TPckg<TInt> idsSizeBuffer(idsSize);

	TIpcArgs args(&idsSizeBuffer);
	User::LeaveIfError(SendReceive(CTzServer::EGetUserTimeZoneIdsSize, args));

	CBufFlat* retBuffer = CBufFlat::NewL(idsSize);
	CleanupStack::PushL(retBuffer);
	retBuffer->ExpandL(0, idsSize);
	TPtr8 outPtr(retBuffer->Ptr(0) );
	
	TIpcArgs args2(&outPtr);
	User::LeaveIfError(SendReceive(CTzServer::EGetUserTimeZoneIds, args2));
	
	RBufReadStream readStream;
	CleanupClosePushL(readStream);
	readStream.Open(*retBuffer);
	
	TInt count = readStream.ReadInt16L();
	aTzIds.ReserveL(aTzIds.Count()+ count);
	for (TInt ii=0; ii<count; ++ii)
		{
		TUint32 id = readStream.ReadUint32L();
		CTzId* zone = CTzId::NewL(id);
		aTzIds.AppendL(zone);
		}
	CleanupStack::PopAndDestroy(2, retBuffer);
	}


#if defined(_DEBUG)


/**
Clears timezone information cached at client side.

@param aRestartCaching Set to ETrue if caching needs to be restarted after
clearing current cache.

@internalTechnology
*/
EXPORT_C void RTz::__dbgClearCacheL(TBool aRestartCaching)
	{
	delete iRulesHolder;
	iRulesHolder = NULL;
	
	if(aRestartCaching)
		{
		StartCachingL();
		}
	}


EXPORT_C TInt RTz::__dbgRequestAllocatedCellsL(TInt aHeapSizeInBytes)
	{
	TPckg<TInt> numCells(0);
	TPckg<TInt> sizeOfHeap(aHeapSizeInBytes);
	TIpcArgs args(&numCells, &sizeOfHeap);
 	User::LeaveIfError(SendReceive(CTzServer::ESrvOpcodeResourceCount, args));
 	return numCells();
	}


/**
Set heap allocation failure according the arguments, debug only.

@param aType An enumeration which indicates how to simulate heap allocation
failure. 

@param aRate The rate of failure.  When aType is RAllocator::EDeterministic
heap allocation fails every aRate attempt.

@internalTechnology
*/	
EXPORT_C void RTz::__dbgSetHeapFailL(RAllocator::TAllocFail aType, TInt aRate)
	{
	TIpcArgs args(aType, aRate);
	User::LeaveIfError(SendReceive(CTzServer::ESrvOpcodeSetHeapFailure, args));
	}

	
EXPORT_C void RTz::__dbgResetHeapL()
	{
	User::LeaveIfError(SendReceive(CTzServer::ESrvOpcodeResetHeap));
	}

	
#else


EXPORT_C void RTz::__dbgClearCacheL(TBool /*aRestartCaching*/)
	{
	// Do nothing.
	}


EXPORT_C TInt RTz::__dbgRequestAllocatedCellsL(TInt /*aHeapSizeInBytes*/)
	{
	return 0;
	}


EXPORT_C void RTz::__dbgSetHeapFailL(RAllocator::TAllocFail /*aType*/,
	TInt /*aRate*/)
	{
	// Do nothing.
	}


EXPORT_C void RTz::__dbgResetHeapL()
	{
	// Do nothing.
	}


#endif
