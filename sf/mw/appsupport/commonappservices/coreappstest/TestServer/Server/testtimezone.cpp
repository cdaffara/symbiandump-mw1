// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "testtz.h"
#include <s32mem.h>
#include <e32debug.h>



const TInt KNumConnectRetries = 5;
const TInt KCapabilityNone = 0;
const TInt KCapabilityWriteDeviceData = 1000;
const TUint KTimeZoneServerMajorVersion = 0;
const TUint KTimeZoneServerMinorVersion = 1;
const TUint KTimeZoneServerBuildVersion = 1;
const TUint KMaxTimeZoneIdSize = 64;

enum TFunctionCode
	{
	EGetLocalTimeZoneId = KCapabilityNone,
	EConvertLocalZoneTime,
	EConvertForeignZoneTime,
	EGetLocalEncodedTimeZoneRulesSize,
	EGetLocalOlsenEncodedTimeZoneRulesSize,
	EGetForeignEncodedTimeZoneRulesSize,
	EGetLocalEncodedTimeZoneRules,
	EGetLocalOlsenEncodedTimeZoneRules,
	EGetForeignEncodedTimeZoneRules,
	EGetTimeZoneFormat,
	ERegisterTimeChangeNotifier,
	ECancelRequestforNotice,
	ESrvOpcodeResourceCount,
	ESrvOpcodeSetHeapFailure,
	EGetOffsetsForTimeZoneIds,
	EIsDaylightSavingOn,
	EAutoUpdate,
	EGetUserTimeZoneNamesSize,
	EGetUserTimeZoneNames,
	EGetUserTimeZoneIds,
	EGetUserTimeZoneIdsSize,
	ESrvOpcodeResetHeap,
	ESetTimeZone = KCapabilityWriteDeviceData,
	EEnableAutoUpdate,  // Also has WriteDeviceData capability.
	ENotifyHomeTimeZoneChanged,
	ESetHomeTime,
	ESetUnknownZoneTime,
	ECreateUserTimeZone,
    EUpdateUserTimeZone,
    EDeleteUserTimeZone,
	ENotSupported
	};

// Structures
struct TRendezvous
	{
	TThreadId iId;
	TRequestStatus* iStatus;
	};
	
// Define server name
_LIT(KServerCode,"TZSERVER");
_LIT(KTimeZoneServerName,"!TzServer");

// Return the client side version no.
TVersion RTestTz::Version() const
	{
	return (TVersion(KTimeZoneServerMajorVersion,
			KTimeZoneServerMinorVersion,
			KTimeZoneServerBuildVersion));
	}

// 
//Start the server process/thread which lives in an EPOCEXE object
//
TInt RTestTz::StartServer()
	{
	const TInt KIntServerType = 0x10004019;
	const TUidType serverUid(KNullUid,TUid::Uid(KIntServerType));
	
	TRequestStatus started(KRequestPending);
	TRendezvous rendezvous;
	rendezvous.iId=RThread().Id();		// id of this thread
	rendezvous.iStatus=&started;

	RProcess server;
	TInt err=server.Create(KServerCode,
						TPtrC(reinterpret_cast<TText*>(&rendezvous),
						sizeof(rendezvous)/sizeof(TText)),serverUid);

	if (err!=KErrNone)
		{
		return err;
		}

	server.SetPriority(EPriorityHigh);

	TRequestStatus stat;
	server.Rendezvous(stat);
	if (stat!=KRequestPending)
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

// Converts time between UTC and Local for the current selected system time zone
void RTestTz::doConvertL(TTime& aTime, TTzTimeReference aTimerRef) const
	{

	TPckg<TTime> timeBuffer(aTime);
	TPckg<TTime> timeInBuffer(aTime);
	TPckg<TTzTimeReference> timerRefBuffer(aTimerRef);

	TIpcArgs args(&timeBuffer, &timerRefBuffer, &timeInBuffer);
	const TInt result = SendReceive(EConvertLocalZoneTime, args);
	User::LeaveIfError(result);

	aTime = timeInBuffer();
	}

/**
Constructor.
*/
 RTestTz::RTestTz()
	{
	}

/**
Destructor. Calls Close().
*/
 RTestTz::~RTestTz()
	{
	Close();
	}

/** 
Connects to the time zone server, attempting to start it if necessary.

@return KErrNone if successful, otherwise a system error code.
@panic TzServer 1 The attempt to connect to the server 
failed with an error code other than KErrNotFound.
*/
 TInt RTestTz::Connect()
	{
	// The number of message slots in the RSessionBase::CreateSession() 
	// call are delibrately unspecified so the kernel will take messages 
	// from a global pool as and when required (the session is allowed
	// a maximum of 255 outstanding asynchronous messages)

	TInt retry=KNumConnectRetries;
	FOREVER
		{
		TInt err=CreateSession(KTimeZoneServerName,Version());
		if (err!=KErrNotFound && err!=KErrServerTerminated)
			{
			return err;
			}
		// need to restart server
		if (--retry==0)
			{
			return err;
			}
		err=StartServer();
		if (err!=KErrNone && err!=KErrAlreadyExists)
			{
			// Time Zone Server is configurable, so it is possible not to find it
			if (err == KErrNotFound)
				{
				return err;
				}
			User::Panic(_L("Test TZ Server"), 1);
			}
		}
	}

/**
Closes the connection to the time zone server.
*/
 void RTestTz::Close()
	{
	if (Handle())
		{
		
		RSessionBase::Close();
		}
	}

/** 
Converts a local (wall-clock) time for the current system time zone, to UTC.
@param aTime On entry, contains the local time to be converted, and 
the converted value on exit.
@return KErrNone if successful, otherwise another system error code.
*/
 TInt RTestTz::ConvertToUniversalTime(TTime& aTime) const
	{

	TRAPD(result, doConvertL(aTime, ETzWallTimeReference));
	return result;
	}

/** 
Retrieves the time zone ID for the current system time zone.
@return Time zone ID. The client takes ownership of the object.
@leave KErrNoMemory or another system leave code.
@internalTechnology
*/
 CTestTzId* RTestTz::GetTimeZoneIdL() const
	{
	CBufFlat* inBuffer = CBufFlat::NewL(KMaxTimeZoneIdSize);
	CleanupStack::PushL(inBuffer);

	// reserve the full space
	inBuffer->ExpandL(0, KMaxTimeZoneIdSize);

	TPtr8 ptr = inBuffer->Ptr(0);

	TIpcArgs args(&ptr);
	const TInt result = SendReceive(EGetLocalTimeZoneId, args);
	User::LeaveIfError(result);

	RBufReadStream readStream;
	readStream.Open(*inBuffer);
	CTestTzId* zone = CTestTzId::NewL(readStream);

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
 void RTestTz::SetTimeZoneL(CTestTzId& aZone) const
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
	TPtr8 outPtr = outBuffer->Ptr(0);
	TPtr8 inPtr = inBuffer->Ptr(0);
	TIpcArgs args(&outPtr, &inPtr);
	
	const TInt result = SendReceive(ESetTimeZone, args);
	User::LeaveIfError(result);
	
	RBufReadStream readStream;
	readStream.Open(*inBuffer);
	aZone.InternalizeL(readStream);
	
	CleanupStack::PopAndDestroy(2, outBuffer);
	}
    
/**
Sets the system time to the given local time. Note that the standard Tz
algorithm is used - in the case of an ambiguous time the first occurance of the
given local wall-clock time is chosen.

An example of an ambiguous time: 01:30 wall-clock time on 31/10/2004 in the UK
could map to either 01:30 BST (00:30 GMT) or 01:30 GMT, as the clocks would go
back at 02:00 BST (01:00 GMT))

@param aLocalTime The time to set in wall-clock time.

@return An error code. KErrNone is expected unless there is an error in
converting the given local time to UTC.

*/
TInt RTestTz::SetHomeTime(const TTime& aLocalTime) const
    {
    // Convert aLocalTime to UTC.
    TTime utcTime(aLocalTime);
    TInt err = ConvertToUniversalTime(utcTime);
    
    if (err)
        {
        return err;
        }
    	
    // Set the system UTC time and UTC offset.
    // Note that the UTC offset will be recalculated when the
    // EnvironmentChangeCallback is called, so to pass anything
    // other than the previous UTC offset here could prevent
    // UTC offset change notification.
   	
   	TTimeIntervalSeconds utcOffset = 0;
    aLocalTime.SecondsFrom(utcTime,utcOffset);    
    return User::SetUTCTimeAndOffset(utcTime, utcOffset);
    }


