// Copyright (c) 1997-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include <s32mem.h>
#include <vtzrules.h>
#include <tzupdate.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <tzusernames.h>//new file added for CTzUserNames which is publishedPartner
#include <tzuserdefineddata.h> //new file added for CTzUserData which is publishedPartner
#endif
#include <tzlocalizedcityrecord.h>
#include <tzlocalizedtimezonerecord.h>
#include "timezonesession.h"
#include "timezoneserver.h"

CTzServerSession* CTzServerSession::NewL()
	{
	CTzServerSession* self = new (ELeave) CTzServerSession();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

CTzServerSession::~CTzServerSession()
	{
	if (iPendingRequest)
		{
		iClientMessage.Complete(KErrCancel);
		iPendingRequest = EFalse;
		}

	TzServer()->SessionClosed();
	
	delete iTzUserDataCache;
	
	iCitiesResultCache.ResetAndDestroy();
	iCitiesResultCache.Close();
	delete iTimeZoneResultCache;
	delete iCityResultCache;
	}

CTzServerSession::CTzServerSession()
	{
	}

void CTzServerSession::ConstructL()
	{
	iTzUserDataCache = CTzUserDataCache::NewL();
	}

// completes a pending client notifier
void CTzServerSession::NotifyTZStatusChange(RTz::TTzChanges aChange, const TAny* aRequester)
	{
	if (static_cast<const CTzServerSession*>(aRequester) == this)
		{
		// no need informing the client making the change
		// he should look after him self
		return;
		}

	if (iPendingRequest)
		{
		iClientMessage.Complete(aChange);
		iPendingRequest = EFalse;
		}
	}

//
// Registers to receive notification to time and zone changes in the
// Server
TInt CTzServerSession::doRegisterTzChangeNotifier(const RMessage2& aMessage)
	{
	if (!iPendingRequest)
		{
		iClientMessage = aMessage;
		iPendingRequest = ETrue;
		}
	else
		{
		// the same client already requested such notice
		// Panic the client
		iPendingRequest = EFalse;
		aMessage.Panic(KTimeZoneServerName, RTz::EPanicNotificationRequestPending);
		
		return (KErrCancel);
		}

	return KRequestPending;
	}

//
// Cancels request to receive notification to time and zone changes in the
// Server
TInt CTzServerSession::doCancelRequestForTzChangeNoticeL(const RMessage2& /* aMessage */)
	{
	if (iPendingRequest)
		{
		iPendingRequest = EFalse;
		iClientMessage.Complete(KErrCancel);
		}
	else
		{
		User::Leave(KErrNotFound);
		}

	return KErrNone;
	}


TInt CTzServerSession::doSetTimeZoneL(const RMessage2& aMessage)
	{
	CBufFlat* buffer = CBufFlat::NewL(KMaxTimeZoneIdSize);
	CleanupStack::PushL(buffer);

	// Read from client message buffer
	// reserve the full space
	buffer->ExpandL(0, KMaxTimeZoneIdSize);
	TPtr8 bufferPtr(buffer->Ptr(0) );
	aMessage.ReadL(0, bufferPtr);

	RBufReadStream readStream;
	readStream.Open(*buffer);
	
	CTzId* zoneId = NULL;
	TRAPD(err, zoneId = CTzId::NewL(readStream));

	// Panic the client if it sent invalid data that would cause the server to panic.
	if (err == KErrArgument)
	    {
    	CleanupStack::PopAndDestroy(buffer);
        aMessage.Panic(KTimeZoneServerName, RTz::EPanicInvalidArgument);
        return KRequestPending;
	    }
	else
	    {
	    User::LeaveIfError(err);
	    }
	
	CleanupStack::PushL(zoneId);
	const CTzId& KInZoneId = TzServer()->TimeZoneManager().SetTimeZoneL(*zoneId, this, ETrue);
	CleanupStack::PopAndDestroy(zoneId);

	// return the new setting
	buffer->Reset();
	RBufWriteStream writeStream;
	writeStream.Open(*buffer);
	KInZoneId.ExternalizeL(writeStream);
	writeStream.CommitL();
	writeStream.Close();

	// Write to client message buffer
	aMessage.WriteL(1, buffer->Ptr(0));

	CleanupStack::PopAndDestroy(buffer);

	return (KErrNone);
	}
	
TInt CTzServerSession::doIsDaylightSavingOnL(const RMessage2& aMessage)
{
	CBufFlat* buffer = CBufFlat::NewL(KMaxTimeZoneIdSize);
	CleanupStack::PushL(buffer);
	// Read from client message buffer
	// reserve the full space
	buffer->ExpandL(0, KMaxTimeZoneIdSize);
	TPtr8 bufferPtr(buffer->Ptr(0) );
	aMessage.ReadL(0, bufferPtr);
	
	TPckgBuf<TTime> timeBuffer;
	aMessage.ReadL(1, timeBuffer);
	
	TTime time(timeBuffer());

	RBufReadStream readStream;
	readStream.Open(*buffer);

    CTzId* zoneId = NULL;
	TRAPD(err, zoneId = CTzId::NewL(readStream));
	
	// Panic the client if it sent invalid data that would cause the server to panic.
	if (err == KErrArgument)
	    {
    	CleanupStack::PopAndDestroy(buffer);
        aMessage.Panic( KTimeZoneServerName, RTz::EPanicInvalidArgument );
        return KRequestPending;
	    }
	else
	    {
	    User::LeaveIfError(err);
	    }
	
	CleanupStack::PushL(zoneId);
	
	if( time == Time::NullTTime())
		{
		time.UniversalTime();		
		}

	TInt isDaylightSavingOn = TzServer()->TimeZoneManager().IsDaylightSavingOnL(*zoneId, time);
	CleanupStack::PopAndDestroy(zoneId);
	// Write to client message buffer
	TPckg<TInt> response(isDaylightSavingOn);
	aMessage.WriteL(2, response);
	CleanupStack::PopAndDestroy(buffer);
	return (KErrNone);
}

TInt CTzServerSession::doGetLocalTimeZoneIdL(const RMessage2& aMessage)
	{
	const CTzId& KZoneId = TzServer()->TimeZoneManager().GetTimeZoneIdL();

	CBufFlat* buffer = CBufFlat::NewL(KMaxTimeZoneIdSize);
	CleanupStack::PushL(buffer);

	RBufWriteStream writeStream;
	writeStream.Open(*buffer);

	KZoneId.ExternalizeL(writeStream);

	writeStream.CommitL();
	writeStream.Close();

	// Write to client message buffer
	aMessage.WriteL(0, buffer->Ptr(0));

	CleanupStack::PopAndDestroy(buffer);
	return (KErrNone);

	}

TInt CTzServerSession::doGetLocalEncodedTimeZoneRulesL(const RMessage2& aMessage)
	{
	TPckgBuf<TInt> rulesSizeBuffer;
	aMessage.ReadL(0, rulesSizeBuffer);

	// Check if rules size buffer is valid. Panic the client if it sent 
	// invalid data that would cause the server to panic.
    TInt size = rulesSizeBuffer();
    const TInt KMaxSize = KMaxTInt / 2;
    if (size <= 0 || size >= KMaxSize)
        {
        aMessage.Panic( KTimeZoneServerName, RTz::EPanicInvalidArgument );
        return KRequestPending;
        }

	CBufFlat* buffer = CBufFlat::NewL(rulesSizeBuffer());
	CleanupStack::PushL(buffer);
	
	const CTzRules& KRules = TzServer()->TimeZoneManager().GetEncodedTimeZoneRulesL();		

	// Externalise rules	
	RBufWriteStream writeStream;
	writeStream.Open(*buffer);

	TRAPD(err, KRules.ExternalizeL(writeStream));
	// If the stream contains invalid data that would cause the server to panic 
	// then panic the client.
	if (err == KErrArgument)
	    {
	    CleanupStack::PopAndDestroy(buffer);
        aMessage.Panic( KTimeZoneServerName, RTz::EPanicInvalidArgument );
        return KRequestPending;
	    }
	else
	    {
	    User::LeaveIfError(err);
	    }
	    
	writeStream.CommitL();
	writeStream.Close();
	
	// Write to client message buffer
	aMessage.WriteL(1, buffer->Ptr(0));
	
	CleanupStack::PopAndDestroy(buffer);
	return KErrNone;
	}

TInt CTzServerSession::doGetLocalEncodedTimeZoneRulesSizeL(const RMessage2& aMessage)
	{
	TPckgBuf<TTime> startTimeBuffer;
	aMessage.ReadL(0, startTimeBuffer);
	
	TPckgBuf<TTime> endTimeBuffer;
	aMessage.ReadL(1, endTimeBuffer);
	
	TPckgBuf<TTzTimeReference> timeRefBuffer;
	aMessage.ReadL(2, timeRefBuffer);

	TInt rulesSize = TzServer()->TimeZoneManager().GetEncodedTimeZoneRulesSizeL(startTimeBuffer(), endTimeBuffer(),
		timeRefBuffer());
	
	// Write to client message buffer
	TPckgBuf<TInt> rulesSizeBuffer(rulesSize);
	aMessage.WriteL(3, rulesSizeBuffer);

	return KErrNone;
	}

TInt CTzServerSession::doGetForeignEncodedTimeZoneRulesL(const RMessage2& aMessage)
	{
	TPckgBuf<TInt> rulesSizeBuffer;
	aMessage.ReadL(0, rulesSizeBuffer);
   
	// Check if rules size buffer is invalid. Panic the client if it sent 
	// invalid data that would cause the server to panic.
    TInt size = rulesSizeBuffer();
    const TInt KMaxSize = KMaxTInt/2;
    if (size <= 0 || size >= KMaxSize)
        {
        aMessage.Panic( KTimeZoneServerName, RTz::EPanicInvalidArgument );
        return KRequestPending;
        }

	CBufFlat* buffer = CBufFlat::NewL(rulesSizeBuffer());
	CleanupStack::PushL(buffer);
	
	const CTzRules& KRules = TzServer()->TimeZoneManager().GetForeignEncodedTimeZoneRulesL();		

	// Externalise rules	
	RBufWriteStream writeStream;
	writeStream.Open(*buffer);
	// If the stream contains invalid data that would cause the server to panic 
	// then panic the client.
	TRAPD(err, KRules.ExternalizeL(writeStream));
	if (err == KErrArgument)
	    {
	    CleanupStack::PopAndDestroy(buffer);
        aMessage.Panic( KTimeZoneServerName, RTz::EPanicInvalidArgument );
        return KRequestPending;
	    }
	else
	    {
	    User::LeaveIfError(err);
	    }
	
	writeStream.CommitL();
	writeStream.Close();
	
	// Write to client message buffer
	aMessage.WriteL(1, buffer->Ptr(0));
	
	CleanupStack::PopAndDestroy(buffer);
	return KErrNone;
	}
	
TInt CTzServerSession::doGetForeignEncodedTimeZoneRulesSizeL(const RMessage2& aMessage)
	{
	TPckgBuf<TTime> startTimeBuffer;
	aMessage.ReadL(0, startTimeBuffer);

	TPckgBuf<TTime> endTimeBuffer;
	aMessage.ReadL(1, endTimeBuffer);

	// Read from client message buffer
	// reserve the full space
	CBufFlat* buffer = CBufFlat::NewL(KMaxTimeZoneIdSize + sizeof(TTzTimeReference));
	CleanupStack::PushL(buffer);

	buffer->ExpandL(0, KMaxTimeZoneIdSize + sizeof(TTzTimeReference));
	TPtr8 bufferPtr(buffer->Ptr(0) );
	aMessage.ReadL(2, bufferPtr);

	RBufReadStream readStream;
	readStream.Open(*buffer);
	
	CTzId* zoneId = NULL;
	TRAPD(err, zoneId = CTzId::NewL(readStream));
	// If the stream contains invalid data that would cause the server to panic 
	// then panic the client.
	if (err == KErrArgument)
	    {
	    CleanupStack::PopAndDestroy(buffer);
        aMessage.Panic( KTimeZoneServerName, RTz::EPanicInvalidArgument );
        return KRequestPending;
	    }
	else
	    {
	    User::LeaveIfError(err);
	    }

	CleanupStack::PushL(zoneId);

	TTzTimeReference timeRef(static_cast<TTzTimeReference>(readStream.ReadInt8L() ) );
	TPckgBuf<TTzTimeReference> timeRefBuffer(timeRef);
	
	TInt rulesSize = TzServer()->TimeZoneManager().GetForeignEncodedTimeZoneRulesSizeL(*zoneId, startTimeBuffer(),
		endTimeBuffer(), timeRefBuffer());


	// Write to client message buffer
	TPckgBuf<TInt> rulesSizeBuffer(rulesSize);
	aMessage.WriteL(3, rulesSizeBuffer);

	CleanupStack::PopAndDestroy(2,buffer);
	return KErrNone;
	}

TInt CTzServerSession::doConvertLocalZoneTimeL(const RMessage2& aMessage)
	{
	TPckgBuf<TTime> timeBuffer;
	aMessage.ReadL(0, timeBuffer);

	TPckgBuf<TTzTimeReference> timerRefBuffer;
	aMessage.ReadL(1, timerRefBuffer);

	TTime time(timeBuffer());
	TzServer()->TimeZoneManager().ConvertL(time, timerRefBuffer());

	// Write to client message buffer
	TPckg<TTime> timeInBuffer(time);
	aMessage.WriteL(2, timeInBuffer);

	return (KErrNone);
	}


TInt CTzServerSession::doConvertForeignZoneTimeL(const RMessage2& aMessage)
	{
	TPckgBuf<TTime> timeBuffer;
	aMessage.ReadL(0, timeBuffer);

	TPckgBuf<TTzTimeReference> timerRefBuffer;
	aMessage.ReadL(1, timerRefBuffer);

	// Read from client message buffer
	// reserve the full space
	CBufFlat* buffer = CBufFlat::NewL(KMaxTimeZoneIdSize);
	CleanupStack::PushL(buffer);

	buffer->ExpandL(0, KMaxTimeZoneIdSize);
	TPtr8 bufferPtr(buffer->Ptr(0) );
	aMessage.ReadL(2, bufferPtr);

	RBufReadStream readStream;
	readStream.Open(*buffer);

    CTzId* zoneId = NULL;
	TRAPD(err, zoneId = CTzId::NewL(readStream));
	// If the stream contains invalid data that would cause the server to panic 
	// then panic the client.
	if (err == KErrArgument)
	    {
	    CleanupStack::PopAndDestroy(buffer);
        aMessage.Panic( KTimeZoneServerName, RTz::EPanicInvalidArgument );
        return KRequestPending;
	    }
	else
	    {
	    User::LeaveIfError(err);
	    }
	
	CleanupStack::PushL(zoneId);

	TTime time(timeBuffer() );
	TzServer()->TimeZoneManager().ConvertL(*zoneId, time, timerRefBuffer());

	// Write to client message buffer
	TPckg<TTime> timeInBuffer(time);
	aMessage.WriteL(3, timeInBuffer);

	CleanupStack::PopAndDestroy(2);
	return (KErrNone);
	}
	
/**
Retrieves UTC offset for a number of time zone ids
*/
TInt CTzServerSession::doGetOffsetsForTimeZoneIdsL(const RMessage2& aMessage) const
	{
	TInt bufferSize = aMessage.Int0();				
    
    // If buffer size invalid then panic client.
    const TInt KMaxSize = KMaxTInt / 4;
    if (bufferSize <= 0 || bufferSize > KMaxSize)
        {
        aMessage.Panic( KTimeZoneServerName, RTz::EPanicInvalidArgument );
        return KRequestPending;
        }
	
	CBufFlat* idBuf = CBufFlat::NewL(bufferSize);
	CleanupStack::PushL(idBuf);
	idBuf->ExpandL(0,bufferSize);
	TPtr8 bufferPtr = idBuf->Ptr(0);
	aMessage.ReadL(1, bufferPtr);

	CTzConfigAgent& configAgent = TzServer()->TimeZoneManager();
	configAgent.GetOffsetsForTimeZoneIdsL(*idBuf);

	// Write to client message buffer
	aMessage.WriteL(1, idBuf->Ptr(0));

	CleanupStack::PopAndDestroy(idBuf);
		
	return KErrNone;
	}

/**
Sets the configuration of the UTC Offset auto-update functionality.

@param aMessage The client-server message parameter.
@return An error code.
@internalComponent
@capability ECapabilityWriteDeviceData Write Device Data capability is needed
to be able to set the UTC Offset auto-update configuration. This prevents
non-system applications from altering the settings.
*/
TInt CTzServerSession::doSetAutoUpdateBehaviorL(const RMessage2& aMessage)
    {
    // Configuration value is stored in message slot zero.
	TzServer()->TimeZoneManager().SetAutoUpdateBehaviorL(aMessage.Int0());
	return KErrNone;
    }
/**
Retrieves the auto update value
*/
TInt CTzServerSession::doAutoUpdateSettingL(const RMessage2& aMessage)
    {
   	TInt autoUpdate = TzServer()->TimeZoneManager().AutoUpdateSetting();
	// Write to client message buffer
	TPckg<TInt> response(autoUpdate);
	aMessage.WriteL(0, response);
	return (KErrNone);
	}

TInt CTzServerSession::doNotifyHomeTimeZoneChanged(const RMessage2& aMessage)
	{
	NTzUpdate::TTimeZoneChange change;
	change.iNewTimeZoneId = aMessage.Int0();
	change.iOldTimeZoneId = aMessage.Int1();
		
	TPckgBuf<NTzUpdate::TTimeZoneChange> changeBuf(change);
	return RProperty::Set(NTzUpdate::KPropertyCategory, NTzUpdate::EHomeTimeZoneId, changeBuf);
	}

TInt CTzServerSession::doSetHomeTimeL(const RMessage2& aMessage)
	{
	TPckgBuf<TTime> homeTimeBuffer;
	aMessage.ReadL(0, homeTimeBuffer);
	return TzServer()->TimeZoneManager().SetHomeTimeL(homeTimeBuffer());
	}

TInt CTzServerSession::doSetUnknownZoneTimeL(const RMessage2& aMessage)
	{
	TPckgBuf<TTime> utcTimeBuffer;
	TPckgBuf<TInt>  utcOffset;
	TPckgBuf<TBool>  persist;
	aMessage.ReadL(0, utcTimeBuffer);
	aMessage.ReadL(1, utcOffset);
	aMessage.ReadL(2, persist);

	return TzServer()->TimeZoneManager().SetUnknownTimeZoneTimeL(utcTimeBuffer(),utcOffset(),persist());
	}
	
/**
 * Creates user defined time zone
 * Arguments: 0 : Input - Buffer Size
 *			  1 : Input - Data Buffer contains time zone info 
 *			  2 : Output - ID of the time zone created 
 */	
TInt CTzServerSession::doCreateUserTimeZoneL(const RMessage2& aMessage)
	{
	const TInt buffersize = aMessage.Int0();
	
	CBufFlat* buffer = CBufFlat::NewL(buffersize);
	CleanupStack::PushL(buffer);

	buffer->ExpandL(0, buffersize);
	TPtr8 bufferPtr(buffer->Ptr(0) );
	aMessage.ReadL(1, bufferPtr);

	RBufReadStream readStream;
	CleanupClosePushL(readStream);
	readStream.Open(*buffer);
	
	CTzRules* rules = NULL;
	TRAPD(err, 	rules = CTzRules::NewL(readStream));

	// Panic the client if it sent invalid data that would cause the server to panic.
	if (err == KErrArgument)
	    {
    	CleanupStack::PopAndDestroy(2, buffer);
        aMessage.Panic(KTimeZoneServerName, RTz::EPanicInvalidArgument);
        return KRequestPending;
	    }
	else
	    {
	    User::LeaveIfError(err);
	    }
	    
	CleanupStack::PushL(rules);

	CTzUserNames* names = CTzUserNames::NewL(readStream);
	CleanupStack::PushL(names);
	TInt id = TzServer()->UserTimeZoneDb().CreateTzL(*rules, *names);		
	CleanupStack::PopAndDestroy(4, buffer);
	TPckgBuf<TInt> idBuffer(id);
	aMessage.WriteL(2, idBuffer);
	return KErrNone;
	}

/**
 * Update a user defined time zone
 * Arguments: 0 : Input - Size of data buffer 
 *			  1 : Input - Data buffer contained time zone info
 *			  2 : Input - ID of thetTime zone to be updated
 */	
TInt CTzServerSession::doUpdateUserTimeZoneL(const RMessage2& aMessage)
	{
	const TInt buffersize = aMessage.Int0();
	
	CBufFlat* buffer = CBufFlat::NewL(buffersize);
	CleanupStack::PushL(buffer);

	buffer->ExpandL(0, buffersize);
	TPtr8 bufferPtr(buffer->Ptr(0) );
	aMessage.ReadL(1, bufferPtr);

	RBufReadStream readStream;
	CleanupClosePushL(readStream);
	readStream.Open(*buffer);
	
	CTzRules* rules = NULL;
	TRAPD(err, 	rules = CTzRules::NewL(readStream));

	// Panic the client if it sent invalid data that would cause the server to panic.
	if (err == KErrArgument)
	    {
    	CleanupStack::PopAndDestroy(2, buffer);
        aMessage.Panic(KTimeZoneServerName, RTz::EPanicInvalidArgument);
        return KRequestPending;
	    }
	else
	    {
	    User::LeaveIfError(err);
	    }
	    
	CleanupStack::PushL(rules);

	CTzUserNames* names = CTzUserNames::NewL(readStream);;
	CleanupStack::PushL(names);
	
	const TInt id = aMessage.Int2();
	
    TzServer()->UserTimeZoneDb().UpdateTzL(id, *rules, *names);
	CleanupStack::PopAndDestroy(4, buffer);
	return KErrNone;
	}
	
/**
 * Delete a user defined time zone
 * Arguments: 0 : Input - Time Zone ID
 */		
TInt CTzServerSession::doDeleteUserTimeZoneL(const RMessage2& aMessage)
	{
	const TInt id = aMessage.Int0();
	TzServer()->UserTimeZoneDb().DeleteTzL(id);		
	return KErrNone;
	}
	
/**
 * Gets the size of user defined time zone names
 * Arguments: 0 : Input - Time Zone ID
 *			  1 : Output - size of time zone names
 */		
TInt CTzServerSession::doGetUserTimeZoneNamesSizeL(const RMessage2& aMessage)
	{
	const TInt id = aMessage.Int0();
	TInt sizeOfNames = 0;
	//ReadTzNamesL leaves if there is not a names found
	CTzUserNames* names = TzServer()->UserTimeZoneDb().ReadTzNamesL(id);		
	iTzUserDataCache->SetNames(names);
	sizeOfNames = iTzUserDataCache->SizeOfNames();
	// Write to client message buffer
	TPckgBuf<TInt> nameSizeBuffer(sizeOfNames);
	aMessage.WriteL(1, nameSizeBuffer);
	
	return KErrNone;
	}
		
/**
 * Gets user defined time zone names
 * Arguments: 0 : Output - Data buffer contained time zone name 
*/	
 TInt CTzServerSession::doGetUserTimeZoneNamesL(const RMessage2& aMessage)
	{
	TInt size = iTzUserDataCache->SizeOfNames();
	if ( size > 0 )
		{
		CBufFlat* buffer = CBufFlat::NewL(size);
		CleanupStack::PushL(buffer);
		buffer->ExpandL(0,size);
		
		RBufWriteStream writeStream;
		CleanupClosePushL(writeStream);
		writeStream.Open(*buffer);
		const CTzUserNames& names = iTzUserDataCache->GetNames();
		writeStream << names;
		writeStream.CommitL();
		aMessage.WriteL(0, buffer->Ptr(0));
		CleanupStack::PopAndDestroy(2, buffer);
		return KErrNone;
		}
	return KErrArgument;
	}
	
/**
 * Gets the size of user defined time zone Ids
 * Arguments: 0 : Output - Size of time zone IDs
 */		
TInt CTzServerSession::doGetUserTimeZoneIdsSizeL(const RMessage2& aMessage)
	{
	RArray<TUint32> ids;
	CleanupClosePushL(ids);
	TzServer()->UserTimeZoneDb().ReadTzIdsL(ids);
	iTzUserDataCache->SetIdsL(ids);
	CleanupStack::PopAndDestroy(&ids);
		
	TInt idsSize = iTzUserDataCache->SizeOfIds();
	// Write to client message buffer
	TPckgBuf<TInt> idsSizeBuffer(idsSize);
	aMessage.WriteL(0, idsSizeBuffer);
	return KErrNone;
	}
	
/**
 * Gets the user defined time zone Ids
 * Arguments: 0 : Output - Data buffer contained time zone IDs
 */	
TInt CTzServerSession::doGetUserTimeZoneIdsL(const RMessage2& aMessage)
	{
	CBufFlat* buffer = CBufFlat::NewL(iTzUserDataCache->SizeOfIds());
	CleanupStack::PushL(buffer);
	buffer->ExpandL(0,iTzUserDataCache->SizeOfIds());
	
	RBufWriteStream writeStream;
	CleanupClosePushL(writeStream);
	writeStream.Open(*buffer);
	TArray<TUint32> ids = iTzUserDataCache->GetIds();
	TInt count = ids.Count();
	writeStream.WriteInt16L(count);
	for (TInt ii=0; ii<count; ++ii)
		{
		writeStream.WriteUint32L((ids)[ii]);	
		}
	writeStream.CommitL();
	aMessage.WriteL(0, buffer->Ptr(0));
	CleanupStack::PopAndDestroy(2, buffer);
	return KErrNone;	
	}

TInt CTzServerSession::doGetHeapSizeL(const RMessage2& aMessage)
	{
	TInt reply = User::Heap().Count();
	TInt sizeInBytes;
	reply = User::AllocSize(sizeInBytes);
	TPckg<TInt> heapCellsBuffer(reply);
	TPckg<TInt> heapsizeBuffer(sizeInBytes);
	aMessage.WriteL(0, heapCellsBuffer);
	aMessage.WriteL(1, heapsizeBuffer);	
	return reply;
	}

TInt CTzServerSession::doLocalizationReadCitiesSizeL(const RMessage2& aMessage)
	{
	iCitiesResultCache.ResetAndDestroy();
	TzServer()->LocalizationDb().ReadCitiesL(iCitiesResultCache);
	TInt resultSize = CTzLocalizedCityRecord::ExternalizeSize(iCitiesResultCache);
		
	// Write to client message buffer
	TPckgBuf<TInt> resultSizeBuffer(resultSize);
	aMessage.WriteL(0, resultSizeBuffer);

	return KErrNone;
	}

TInt CTzServerSession::doLocalizationReadCitiesTzIdSizeL(const RMessage2& aMessage)
	{
	iCitiesResultCache.ResetAndDestroy();
	TzServer()->LocalizationDb().ReadCitiesL(iCitiesResultCache, aMessage.Int1());
	TInt resultSize = CTzLocalizedCityRecord::ExternalizeSize(iCitiesResultCache);
		
	// Write to client message buffer
	TPckgBuf<TInt> resultSizeBuffer(resultSize);
	aMessage.WriteL(0, resultSizeBuffer);

	return KErrNone;
	}

TInt CTzServerSession::doLocalizationReadCitiesInGroupSizeL(const RMessage2& aMessage)
	{
	iCitiesResultCache.ResetAndDestroy();
	TzServer()->LocalizationDb().ReadCitiesInGroupL(iCitiesResultCache, aMessage.Int1());
	TInt resultSize = CTzLocalizedCityRecord::ExternalizeSize(iCitiesResultCache);
		
	// Write to client message buffer
	TPckgBuf<TInt> resultSizeBuffer(resultSize);
	aMessage.WriteL(0, resultSizeBuffer);

	return KErrNone;
	}

TInt CTzServerSession::doLocalizationReadCitiesL(const RMessage2& aMessage)
	{
	TInt maxSize = aMessage.GetDesLengthL(0);
	CBufFlat* buffer = CBufFlat::NewL(maxSize);
	CleanupStack::PushL(buffer);
	
	RBufWriteStream writeStream;
	writeStream.Open(*buffer);
	CleanupClosePushL(writeStream);
	CTzLocalizedCityRecord::ExternalizeL(iCitiesResultCache, writeStream);
	writeStream.CommitL();
	iCitiesResultCache.ResetAndDestroy();
	CleanupStack::PopAndDestroy(&writeStream);
	
	// Write to client message buffer
	aMessage.WriteL(0, buffer->Ptr(0));
	
	CleanupStack::PopAndDestroy(buffer);
	
	return KErrNone;
	}

TInt CTzServerSession::doLocalizationReadFrequentlyUsedZoneSizeL(const RMessage2& aMessage)
	{
	delete iTimeZoneResultCache;
	iTimeZoneResultCache = 0;
	iTimeZoneResultCache = TzServer()->LocalizationDb().ReadFrequentlyUsedZoneL(aMessage.Int1());
	TInt resultSize = iTimeZoneResultCache->ExternalizeSize();
	
	// Write to client message buffer
	TPckgBuf<TInt> resultSizeBuffer(resultSize);
	aMessage.WriteL(0, resultSizeBuffer);

	return KErrNone;
	}

TInt CTzServerSession::doLocalizationReadFrequentlyUsedZoneL(const RMessage2& aMessage)
	{
	TInt maxSize = aMessage.GetDesLengthL(0);
	CBufFlat* buffer = CBufFlat::NewL(maxSize);
	CleanupStack::PushL(buffer);
	
	RBufWriteStream writeStream;
	writeStream.Open(*buffer);
	CleanupClosePushL(writeStream);
	iTimeZoneResultCache->ExternalizeL(writeStream);
	writeStream.CommitL();
	delete iTimeZoneResultCache;
	iTimeZoneResultCache = 0;
	CleanupStack::PopAndDestroy(&writeStream);
		
	// Write to client message buffer
	aMessage.WriteL(0, buffer->Ptr(0));
	
	CleanupStack::PopAndDestroy(buffer);
	
	return KErrNone;
	}

TInt CTzServerSession::doLocalizationReadCachedTimeZoneCitySizeL(const RMessage2& aMessage)
	{
	delete iCityResultCache;
	iCityResultCache = 0;
	iCityResultCache = TzServer()->LocalizationDb().ReadCachedTimeZoneCityL(aMessage.Int1());
	TInt resultSize = iCityResultCache->ExternalizeSize();
	
	// Write to client message buffer
	TPckgBuf<TInt> resultSizeBuffer(resultSize);
	aMessage.WriteL(0, resultSizeBuffer);

	return KErrNone;
	}

TInt CTzServerSession::doLocalizationReadCachedTimeZoneCityL(const RMessage2& aMessage)
	{
	TInt maxSize = aMessage.GetDesLengthL(0);
	CBufFlat* buffer = CBufFlat::NewL(maxSize);
	CleanupStack::PushL(buffer);
	
	RBufWriteStream writeStream;
	writeStream.Open(*buffer);
	CleanupClosePushL(writeStream);
	iCityResultCache->ExternalizeL(writeStream);
	writeStream.CommitL();
	delete iCityResultCache;
	iCityResultCache = 0;
	CleanupStack::PopAndDestroy(&writeStream);
	
	// Write to client message buffer
	aMessage.WriteL(0, buffer->Ptr(0));
	
	CleanupStack::PopAndDestroy(buffer);
	
	return KErrNone;
	}

TInt CTzServerSession::doLocalizationOpenDbL()
	{
	TzServer()->LocalizationDb().OpenDbL();
	return KErrNone;
	}

TInt CTzServerSession::doLocalizationCloseDb()
	{
	TzServer()->LocalizationDb().CloseDb();
	return KErrNone;
	}

TInt CTzServerSession::doLocalizationWriteCityL(const RMessage2& aMessage)
	{
	TInt length=aMessage.GetDesLengthL(0);
	HBufC* const cityName=HBufC::NewLC(length);
	TPtr cityNamePtr(cityName->Des());
	aMessage.ReadL(0, cityNamePtr);

	TzServer()->LocalizationDb().WriteCityL(*cityName, (TUint16)aMessage.Int1(), aMessage.Int2(), aMessage.Int3());
	
	CleanupStack::PopAndDestroy(cityName);
	
	return KErrNone;
	}

TInt CTzServerSession::doLocalizationDeleteCityL(const RMessage2& aMessage)
	{
	TInt length=aMessage.GetDesLengthL(0);
	HBufC* const cityName=HBufC::NewLC(length);
	TPtr cityNamePtr(cityName->Des());
	aMessage.ReadL(0, cityNamePtr);

	TzServer()->LocalizationDb().DeleteCityL(*cityName, (TUint16)aMessage.Int1());
	
	CleanupStack::PopAndDestroy(cityName);
	
	return KErrNone;
	}

TInt CTzServerSession::doLocalizationWriteFrequentlyUsedZoneL(const RMessage2& aMessage)
	{
	TInt length=aMessage.GetDesLengthL(0);
	CBufFlat* buffer = CBufFlat::NewL(length);
	CleanupStack::PushL(buffer);
	buffer->ExpandL(0,length);
	TPtr8 des(buffer->Ptr(0));
	aMessage.ReadL(0,des);
	RBufReadStream readStream;
	readStream.Open(*buffer);
	CleanupClosePushL(readStream);
	CTzLocalizedTimeZoneRecord* timeZone = CTzLocalizedTimeZoneRecord::NewLC(readStream);
	CTzLocalizedCityRecord* city = CTzLocalizedCityRecord::NewLC(readStream);
	TInt frequentlyUsedZone = readStream.ReadInt32L();
	
	TzServer()->LocalizationDb().WriteFrequentlyUsedZoneL(*timeZone, *city, frequentlyUsedZone);

	CleanupStack::PopAndDestroy(city);
	CleanupStack::PopAndDestroy(timeZone);
	CleanupStack::PopAndDestroy(&readStream);
	CleanupStack::PopAndDestroy(buffer);
	
	return KErrNone;
	}

TInt CTzServerSession::doLocalizationWriteAllFrequentlyUsedZonesL(const RMessage2& aMessage)
	{
	TInt length=aMessage.GetDesLengthL(0);
	CBufFlat* buffer = CBufFlat::NewL(length);
	CleanupStack::PushL(buffer);
	buffer->ExpandL(0,length);
	TPtr8 des(buffer->Ptr(0));
	aMessage.ReadL(0,des);
	RBufReadStream readStream;
	readStream.Open(*buffer);
	CleanupClosePushL(readStream);
	RPointerArray<CTzLocalizedTimeZoneRecord> timeZones;
	CleanupStack::PushL(TCleanupItem(CleanupTimeZonePointerArray,&timeZones));
	CTzLocalizedTimeZoneRecord::InternalizeL(readStream, timeZones);
	RPointerArray<CTzLocalizedCityRecord> cities;
	CleanupStack::PushL(TCleanupItem(CleanupCityPointerArray,&cities));
	CTzLocalizedCityRecord::InternalizeL(readStream, cities);
	
	TzServer()->LocalizationDb().WriteAllFrequentlyUsedZonesL(timeZones, cities);
	
	CleanupStack::PopAndDestroy();	// cities
	CleanupStack::PopAndDestroy();	// timeZones
	CleanupStack::PopAndDestroy(&readStream);
	CleanupStack::PopAndDestroy(buffer);
	
	return KErrNone;
	}

TInt CTzServerSession::doSwiObsBeginL(const RMessage2& /*aMessage*/)
	{
	TzServer()->SwiObserver().BeginL();
	return KErrNone;
	}

TInt CTzServerSession::doSwiObsFileChangedL(const RMessage2& aMessage)
	{
	TzServer()->SwiObserver().FileChangedL((RTz::TSWIObserverFilterIndex)aMessage.Int0());
	return KErrNone;
	}

TInt CTzServerSession::doSwiObsEndL(const RMessage2& /*aMessage*/)
	{
	TzServer()->SwiObserver().EndL();
	return KErrNone;
	}

void CTzServerSession::ServiceL(const RMessage2& aMessage)
	{
	TInt reply = KErrNone;
	switch(aMessage.Function())
		{
#if defined(_DEBUG)
	case CTzServer::ESrvOpcodeResourceCount:
		reply = doGetHeapSizeL(aMessage);
		break;
	case CTzServer::ESrvOpcodeSetHeapFailure:
		User::__DbgSetAllocFail(RHeap::EUser,(RAllocator::TAllocFail)aMessage.Int0(),aMessage.Int1());
		break;
	case CTzServer::ESrvOpcodeResetHeap:
		User::__DbgSetAllocFail(FALSE,RAllocator::ENone,1);
		iTzUserDataCache->Reset();
		TzServer()->TimeZoneManager().ResetForeignTimeZoneRulesCache();
		break;	
#endif
	case CTzServer::EConvertLocalZoneTime:
		reply = doConvertLocalZoneTimeL(aMessage);
		break;
	case CTzServer::EConvertForeignZoneTime:
		reply = doConvertForeignZoneTimeL(aMessage);
		break;
	case CTzServer::ESetTimeZone:
		reply = doSetTimeZoneL(aMessage);
		break;
	case CTzServer::ENotifyHomeTimeZoneChanged:
		reply = doNotifyHomeTimeZoneChanged(aMessage);
		break;
	case CTzServer::EGetLocalTimeZoneId:
		reply = doGetLocalTimeZoneIdL(aMessage);
		break;
	case CTzServer::EGetLocalEncodedTimeZoneRules:
		reply = doGetLocalEncodedTimeZoneRulesL(aMessage);
		break;
	case CTzServer::EGetLocalOlsenEncodedTimeZoneRules:	
		reply = KErrNotSupported;
		break;
	case CTzServer::EGetLocalEncodedTimeZoneRulesSize:
		reply = doGetLocalEncodedTimeZoneRulesSizeL(aMessage);
		break;		
	case CTzServer::EGetLocalOlsenEncodedTimeZoneRulesSize:
		reply = KErrNotSupported;
		break;		
	case CTzServer::EGetForeignEncodedTimeZoneRules:
		reply = doGetForeignEncodedTimeZoneRulesL(aMessage);
		break;
	case CTzServer::EGetForeignEncodedTimeZoneRulesSize:
		reply = doGetForeignEncodedTimeZoneRulesSizeL(aMessage);
		break;				
	case CTzServer::ERegisterTimeChangeNotifier:
		reply = doRegisterTzChangeNotifier(aMessage);
		break;
	case CTzServer::ECancelRequestforNotice:
		reply = doCancelRequestForTzChangeNoticeL(aMessage);
		break;
	case CTzServer::EGetOffsetsForTimeZoneIds:
		reply = doGetOffsetsForTimeZoneIdsL(aMessage);
		break;
	case CTzServer::EEnableAutoUpdate:
	    reply = doSetAutoUpdateBehaviorL(aMessage);
	    break;
	case CTzServer::EIsDaylightSavingOn:
	    reply = doIsDaylightSavingOnL(aMessage);
	    break;
	case CTzServer::EAutoUpdate:
	    reply = doAutoUpdateSettingL(aMessage);
	    break;
	case CTzServer::ESetHomeTime:
		reply = doSetHomeTimeL(aMessage);
		break;
	case CTzServer::ESetUnknownZoneTime:
		reply = doSetUnknownZoneTimeL(aMessage);
		break;
	case CTzServer::EGetUserTimeZoneNames:
		reply = doGetUserTimeZoneNamesL(aMessage);
		break;
	case CTzServer::EGetUserTimeZoneNamesSize:
		reply = doGetUserTimeZoneNamesSizeL(aMessage);
		break;
	case CTzServer::EGetUserTimeZoneIds:
		reply = doGetUserTimeZoneIdsL(aMessage);
		break;
	case CTzServer::EGetUserTimeZoneIdsSize:
		reply = doGetUserTimeZoneIdsSizeL(aMessage);
		break;
	case CTzServer::ECreateUserTimeZone:
		reply = doCreateUserTimeZoneL(aMessage);
		break;
	case CTzServer::EUpdateUserTimeZone:
		reply = doUpdateUserTimeZoneL(aMessage);
		break;
	case CTzServer::EDeleteUserTimeZone:
		reply = doDeleteUserTimeZoneL(aMessage);
		break;
	case CTzServer::ELocalizationReadCitiesSize:
		reply = doLocalizationReadCitiesSizeL(aMessage);
		break;
	case CTzServer::ELocalizationReadCitiesTzIdSize:
		reply = doLocalizationReadCitiesTzIdSizeL(aMessage);
		break;
	case CTzServer::ELocalizationReadCitiesInGroupSize:
		reply = doLocalizationReadCitiesInGroupSizeL(aMessage);
		break;
	case CTzServer::ELocalizationReadCities:
		reply = doLocalizationReadCitiesL(aMessage);
		break;
	case CTzServer::ELocalizationReadFrequentlyUsedZoneSize:
		reply = doLocalizationReadFrequentlyUsedZoneSizeL(aMessage);
		break;
	case CTzServer::ELocalizationReadFrequentlyUsedZone:
		reply = doLocalizationReadFrequentlyUsedZoneL(aMessage);
		break;
	case CTzServer::ELocalizationReadCachedTimeZoneCitySize:
		reply = doLocalizationReadCachedTimeZoneCitySizeL(aMessage);
		break;
	case CTzServer::ELocalizationReadCachedTimeZoneCity:
		reply = doLocalizationReadCachedTimeZoneCityL(aMessage);
		break;
	case CTzServer::ELocalizationOpenDb:
		reply = doLocalizationOpenDbL();
		break;
	case CTzServer::ELocalizationCloseDb:
		reply = doLocalizationCloseDb();
		break;
	case CTzServer::ELocalizationWriteCity:
		reply = doLocalizationWriteCityL(aMessage);
		break;
	case CTzServer::ELocalizationDeleteCity:
		reply = doLocalizationDeleteCityL(aMessage);
		break;
	case CTzServer::ELocalizationWriteFrequentlyUsedZone:
		reply = doLocalizationWriteFrequentlyUsedZoneL(aMessage);
		break;
	case CTzServer::ELocalizationWriteAllFrequentlyUsedZones:
		reply = doLocalizationWriteAllFrequentlyUsedZonesL(aMessage);
		break;
	case CTzServer::ESwiObsBegin:
		reply = doSwiObsBeginL(aMessage);
		break;
	case CTzServer::ESwiObsFileChanged:
		reply = doSwiObsFileChangedL(aMessage);
		break;
	case CTzServer::ESwiObsEnd:
		reply = doSwiObsEndL(aMessage);
		break;
	default:
		reply = KErrNotFound;
		break;
		}

	if (reply != KRequestPending)
		{
		aMessage.Complete(reply);
		}

#if defined(_DEBUG)
	User::Heap().Check();
#endif
	}

const CTzServer* CTzServerSession::TzServer() const
	{
	return static_cast<const CTzServer*>(Server());
	}

void CTzServerSession::CleanupTimeZonePointerArray(TAny* aArray)
	{
	RPointerArray<CTzLocalizedTimeZoneRecord>* array = static_cast<RPointerArray<CTzLocalizedTimeZoneRecord>* >(aArray);
	if (array)
		{
		array->ResetAndDestroy();
		array->Close();
		}
	}

void CTzServerSession::CleanupCityPointerArray(TAny* aArray)
	{
	RPointerArray<CTzLocalizedCityRecord>* array = static_cast<RPointerArray<CTzLocalizedCityRecord>* >(aArray);
	if (array)
		{
		array->ResetAndDestroy();
		array->Close();
		}
	}
