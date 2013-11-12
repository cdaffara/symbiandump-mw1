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
#include <asaltdefs.h>
#include "consoleantestclient.h"

EXPORT_C RANTestClient::RANTestClient()
	: iAlarmPtr(0, 0, 0),
	iSoundStartIdPtr(0, 0, 0),
	iSoundStopIdPtr(0, 0, 0),
	iVisibleIdPtr(0, 0, 0),
	iVisibleBoolPtr(0, 0, 0),
	iStateIdPtr(0, 0, 0),
	iStateIntPtr(0, 0, 0),
	iDeleteIdPtr(0, 0, 0)
	{
	}

/**
Connect the the Console Alarm Alert Server. The Console Alarm Alert Server must be running
otherwise the method will fail.

@return KErrNone on succcess, a system wide error code otherwise.
*/

EXPORT_C TInt RANTestClient::Connect()
	{
	const TVersion KVersion(KASAltVersionMajor, KASAltVersionMinor, KASAltVersionBuild);
	return CreateSession(KAlarmAlertServerName, KVersion);
	}

/**
Acknowledges the latest notifying alarm. Do not use this method if Console Alarm Alert Server 
is in multiple alarm mode.
*/

EXPORT_C void RANTestClient::AcknowledgeAlarm()
	{
	SendReceive(EASAltTestOpCodeAcknowledgeAlarm);
	}

/**
Retrieves any data attached to the latest notifying alarm. Do not use this method if Console Alarm Alert Server 
is in multiple alarm mode.
@param buf buffer that will contain the data. This buffer must be big enough to contain all the data in the attachement.
*/

EXPORT_C void RANTestClient::GetAttachment(TDes8& buf)
	{
	TIpcArgs args(buf.MaxLength(),&buf);
	SendReceive(EASAltTestOpCodeGetAttachment, args);
	}

/**
Queries wether or not Console Alarm Alert Server is outputting to the console. Do not use this method if Console Alarm Alert Server 
is in multiple alarm mode.

@return ETrue if outputting, EFalse if not.
*/

EXPORT_C TBool RANTestClient::IsNotifying()
	{
	TPckgBuf<TBool> package;
	TIpcArgs args(&package);
	SendReceive(EASAltTestOpCodeGetIsNotifying, args);

	return package();
	}

/**
Switches the Console Alarm Alert Server into extended mode giving access to the new functionality.

@return KErrNone on success, a system wide error code otherwise.
*/

EXPORT_C TInt RANTestClient::SetExtendedMode() const
	{
	return Send(EASAltTestOpCodeSetExtendedMode);
	}

/**
Returns the Console Alarm Alert Server from extended mode.

@return KErrNone on success, a system wide error code otherwise.
*/
	
EXPORT_C TInt RANTestClient::UnsetExtendedMode() const
	{
	return Send(EASAltTestOpCodeUnsetExtendedMode);
	}

/**
Sets the time returned by the Console Alarm Alert Server when sent EASAltOpCodeGetUserTime.

@param aTime, new time to be set.
@return KErrNone on success, a system wide error code otherwise.
*/
	
EXPORT_C TInt RANTestClient::SetUserTime(const TTime& aTime) const
	{
	TPckgC<TTime> pTime(aTime);
	TIpcArgs args(&pTime);
	
	return SendReceive(EASAltTestOpCodeSetUserTime, args);
 	}

/**
Sets the value returned by the Console Alarm Alert Server when sent EASAltOpCodeGetMaxAlarms. 
If this is not set it defaults to 1. The Alarm Server will query this value when connecting to the Console Alarm Alert Server, if you 
want it set differently you need to call this method before the Alarm Server is started.

@param aMax, new max alarms value to be set.
@return KErrNone on success, a system wide error code otherwise.
*/
 	
EXPORT_C TInt RANTestClient::SetMaxAlarms(const TInt aMax) const
	{
	TPckgC<TInt> pMax(aMax);
	TIpcArgs args(&pMax);
	
	return SendReceive(EASAltTestOpCodeSetMaxAlarms, args);
	}

/**
Asynchronously notifies the client when a EASAltOpCodeSetAlarm is received by the Console Alarm Alert Server.

@param aAlarm, to be filled in with the data of the alarm attached to the message.
@param aFullname, the owner of the alarm.
@param aSink, any data attached to the alarm. This descriptor must be big enough to contain all data or no data will be returned.
@param aStatus, TRequestStatus that will be completed when a EASAltOpCodeSetAlarm is received.

@return KErrNone on success, a system wide error code otherwise.
*/
	
EXPORT_C TInt RANTestClient::NotifyOnAlarm(TASShdAlarm& aAlarm, TFullName& aFullname, TDes8& aSink, TRequestStatus& aStatus)
	{
	TIpcArgs args(&iAlarmPtr, &aFullname, &aSink, 0);

	iAlarmPtr.Set(reinterpret_cast<TUint8*>(&aAlarm), sizeof(TASShdAlarm), sizeof(TASShdAlarm));
		
	SendReceive(EASAltTestOpCodeNotifyOnAlarm, args, aStatus);
	return KErrNone;
	}

/**
Asynchronously notifies the client when a EASAltOpCodeSetAlarm is received by the Console Alarm Alert Server.

@param aAlarm, to be filled in with the data of the alarm attached to the message.
@param aFullname, the owner of the alarm.
@param aStatus, TRequestStatus that will be completed when a EASAltOpCodeSetAlarm is received.

@return KErrNone on success, a system wide error code otherwise.
*/
	
EXPORT_C TInt RANTestClient::NotifyOnAlarm(TASShdAlarm& aAlarm, TFullName& aFullname, TRequestStatus& aStatus)
	{
	TIpcArgs args(&iAlarmPtr, &aFullname, 0, 0);

	iAlarmPtr.Set(reinterpret_cast<TUint8*>(&aAlarm), sizeof(TASShdAlarm), sizeof(TASShdAlarm));
	
	SendReceive(EASAltTestOpCodeNotifyOnAlarm, args, aStatus);
	return KErrNone;
	}

/**
Asynchronously notifies the client when a EASAltOpCodeSetAlarm is received by the Console Alarm Alert Server.

@param aAlarm, to be filled in with the data of the alarm attached to the message.
@param aStatus, TRequestStatus that will be completed when a EASAltOpCodeSetAlarm is received.

@return KErrNone on success, a system wide error code otherwise.
*/

EXPORT_C TInt RANTestClient::NotifyOnAlarm(TASShdAlarm& aAlarm, TRequestStatus& aStatus)
	{
	TIpcArgs args(&iAlarmPtr, 0, 0, 0);

	iAlarmPtr.Set(reinterpret_cast<TUint8*>(&aAlarm), sizeof(TASShdAlarm), sizeof(TASShdAlarm));
	
	SendReceive(EASAltTestOpCodeNotifyOnAlarm, args, aStatus);
	return KErrNone;
	}

/**
Asynchronously notifies the client when a EASAltOpCodeStartPlayingSound is received by the Console Alarm Alert Server.

@param aId, id of alarm attached to the message.
@param aFilename, filename of sound to play. This descriptor must be big enough to contain the entire filename or no filename will be returned.
@param aStatus, TRequestStatus that will be completed when a EASAltOpCodeStartPlayingSound is received.

@return KErrNone on success, a system wide error code otherwise.
*/

EXPORT_C TInt RANTestClient::NotifyOnSoundStart(TAlarmId& aId, TDes& aFilename, TRequestStatus& aStatus)
	{
	TIpcArgs args(&iSoundStartIdPtr, &aFilename);
	iSoundStartIdPtr.Set(reinterpret_cast<TUint8*>(&aId), sizeof(TAlarmId), sizeof(TAlarmId));
	
	SendReceive(EASAltTestOpCodeNotifyOnSoundStart, args, aStatus);
	return KErrNone;
	}

/**
Asynchronously notifies the client when a EASAltOpCodeStopPlayingSound is received by the Console Alarm Alert Server.

@param aId, id of alarm attached to the message.
@param aStatus, TRequestStatus that will be completed when a EASAltOpCodeStopPlayingSound is received.

@return KErrNone on success, a system wide error code otherwise.
*/

EXPORT_C TInt RANTestClient::NotifyOnSoundStop(TAlarmId& aId, TRequestStatus& aStatus)
	{
	TIpcArgs args(&iSoundStopIdPtr);
	iSoundStopIdPtr.Set(reinterpret_cast<TUint8*>(&aId), sizeof(TAlarmId), sizeof(TAlarmId));
	
	SendReceive(EASAltTestOpCodeNotifyOnSoundStop, args, aStatus);
	return KErrNone;
	}

/**
Asynchronously notifies the client when a EASAltOpCodeVisible is received by the Console Alarm Alert Server.

@param aId, id of alarm attached to the message.
@param aStatus, TRequestStatus that will be completed when a EASAltOpCodeVisible is received.

@return KErrNone on success, a system wide error code otherwise.
*/
		
EXPORT_C TInt RANTestClient::NotifyOnVisible(TAlarmId& aId, TBool& aVisible, TRequestStatus& aStatus)
	{
	TIpcArgs args(&iVisibleIdPtr, &iVisibleBoolPtr);
	iVisibleIdPtr.Set(reinterpret_cast<TUint8*>(&aId), sizeof(TAlarmId), sizeof(TAlarmId));
	iVisibleBoolPtr.Set(reinterpret_cast<TUint8*>(&aVisible), sizeof(TBool), sizeof(TBool));
	
	SendReceive(EASAltTestOpCodeNotifyOnVisible, args, aStatus);
	return KErrNone;
	}

/**
Asynchronously notifies the client when a EASAltOpCodeSetState is received by the Console Alarm Alert Server.

@param aId, id of alarm attached to the message.
@param aState, new state of alarm.
@param aStatus, TRequestStatus that will be completed when a EASAltOpCodeSetState is received.

@return KErrNone on success, a system wide error code otherwise.
*/
	
EXPORT_C TInt RANTestClient::NotifyOnState(TAlarmId& aId, TInt& aState, TRequestStatus& aStatus)
	{
	TIpcArgs args(&iStateIdPtr, &iStateIntPtr);
	iStateIdPtr.Set(reinterpret_cast<TUint8*>(&aId), sizeof(TAlarmId), sizeof(TAlarmId));
	iStateIntPtr.Set(reinterpret_cast<TUint8*>(&aState), sizeof(TInt), sizeof(TInt));
	
	SendReceive(EASAltTestOpCodeNotifyOnState, args, aStatus);
	return KErrNone;
	}

/**
Asynchronously notifies the client when a EASAltOpCodeSetState is received by the Console Alarm Alert Server.

@param aId, id of alarm attached to the message.
@param aStatus, TRequestStatus that will be completed when a EASAltOpCodeSetState is received.

@return KErrNone on success, a system wide error code otherwise.
*/

EXPORT_C TInt RANTestClient::NotifyOnDelete(TAlarmId& aId, TRequestStatus& aStatus)
	{
	TIpcArgs args(&iDeleteIdPtr);
	iDeleteIdPtr.Set(reinterpret_cast<TUint8*>(&aId), sizeof(TAlarmId), sizeof(TAlarmId));
	
	SendReceive(EASAltTestOpCodeNotifyOnDelete, args, aStatus);
	return KErrNone;
	}

/**
Instructs the Console Alarm Alert Server to snooze the alarm for the specified amount of time.

@param aId, id of alarm to snooze. This doesn't have to be a valid id.
@param aTime, time to snooze to in UTC.

@return KErrNone on success, a system wide error code otherwise.
*/

EXPORT_C TInt RANTestClient::SnoozeAlarm(const TAlarmId& aId, const TTime& aTime) const
	{
	TPckgC<TTime> pTime(aTime);
	TIpcArgs args(aId, &pTime);
	return SendReceive(EASAltTestOpCodeResponseSnoozeAlarm, args);
	}

/**
Instructs the Console Alarm Alert Server to silence the alarm.

@param aId, id of alarm to silence. This doesn't have to be a valid id.

@return KErrNone on success, a system wide error code otherwise.
*/

EXPORT_C TInt RANTestClient::SilenceAlarm(const TAlarmId& aId) const
	{
	TIpcArgs args(aId);
	return SendReceive(EASAltTestOpCodeResponseSilenceAlarm, args);
	}

/**
Instructs the Console Alarm Alert Server to acknowledge the alarm.

@param aId, id of alarm to acknowledge. This doesn't have to be a valid id.

@return KErrNone on success, a system wide error code otherwise.
*/
	
EXPORT_C TInt RANTestClient::AcknowledgeAlarm(const TAlarmId& aId) const
	{
	TIpcArgs args(aId);
	return SendReceive(EASAltTestOpCodeResponseAcknowledgeAlarm, args);
	}

/**
Instructs the Console Alarm Alert Server to acknowledge all currently notifying alarms.

@return KErrNone on success, a system wide error code otherwise.
*/

EXPORT_C TInt RANTestClient::AcknowledgeAll() const
	{
	return SendReceive(EASAltTestOpCodeResponseAcknowledgeAll);
	}

/**
Instructs the Console Alarm Alert Server to silence the alarm.

@param aId, id of alarm to silence. This doesn't have to be a valid id.

@return KErrNone on success, a system wide error code otherwise.
*/
	
EXPORT_C TInt RANTestClient::PauseSoundForAlarm(const TAlarmId& aId, const TTime& aTime) const
	{
	TPckgC<TTime> pTime(aTime);
	TIpcArgs args(aId, &pTime);
	
	return SendReceive(EASAltTestOpCodeResponsePauseSound, args);
	}

/**
Cancels all outstanding notifications for this session.

@return KErrNone on success, a system wide error code otherwise.
*/

EXPORT_C TInt RANTestClient::CancelNotifications()
	{
	return Send(EASAltTestOpCodeCancelNotifications);
	}


#ifndef EKA2
GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	{
	return KErrNone;
	}
#endif
