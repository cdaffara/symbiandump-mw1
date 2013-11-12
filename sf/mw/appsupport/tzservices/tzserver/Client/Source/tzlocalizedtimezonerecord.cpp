// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "tzlocalizedtimezonerecord.h"
#include "tzid.h"
#include "tzidinternal.h"


EXPORT_C CTzLocalizedTimeZoneRecord* CTzLocalizedTimeZoneRecord::NewL(TInt aTimeZoneId, const TDesC& aStandardName,
		const TDesC& aDaylightName, const TDesC& aShortStandardName, const TDesC& aShortDaylightName,
		TUint aResourceId)
	{
	CTzLocalizedTimeZoneRecord* self = CTzLocalizedTimeZoneRecord::NewLC(aTimeZoneId, aStandardName,
		aDaylightName, aShortStandardName, aShortDaylightName, aResourceId);
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CTzLocalizedTimeZoneRecord* CTzLocalizedTimeZoneRecord::NewLC(TInt aTimeZoneId, const TDesC& aStandardName,
		const TDesC& aDaylightName, const TDesC& aShortStandardName, const TDesC& aShortDaylightName,
		TUint aResourceId)
	{
	CTzLocalizedTimeZoneRecord* self = new(ELeave) CTzLocalizedTimeZoneRecord(aResourceId);
	CleanupStack::PushL(self);
	self->ConstructL(aTimeZoneId, aStandardName, aDaylightName, aShortStandardName, aShortDaylightName);
	return self;
	}

EXPORT_C CTzLocalizedTimeZoneRecord* CTzLocalizedTimeZoneRecord::NewL(RReadStream& aStream)
	{
	CTzLocalizedTimeZoneRecord* self = CTzLocalizedTimeZoneRecord::NewLC(aStream);
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CTzLocalizedTimeZoneRecord* CTzLocalizedTimeZoneRecord::NewLC(RReadStream& aStream)
	{
	CTzLocalizedTimeZoneRecord* self = new(ELeave) CTzLocalizedTimeZoneRecord();
	CleanupStack::PushL(self);
	self->ConstructL(aStream);
	return self;
	}

EXPORT_C CTzLocalizedTimeZoneRecord::~CTzLocalizedTimeZoneRecord()
	{
	delete iStandardName;
	delete iDaylightName;
	delete iShortStandardName;
	delete iShortDaylightName;
	}

EXPORT_C void CTzLocalizedTimeZoneRecord::CleanupArray(TAny* aArray)
	{
	RPointerArray<CTzLocalizedTimeZoneRecord>* array = static_cast<RPointerArray<CTzLocalizedTimeZoneRecord>* >(aArray);
	if (array)
		{
		array->ResetAndDestroy();
		array->Close();
		}
	}

CTzLocalizedTimeZoneRecord::CTzLocalizedTimeZoneRecord()
	{
	}

CTzLocalizedTimeZoneRecord::CTzLocalizedTimeZoneRecord(TUint aResourceId)
	: iResourceId(aResourceId)
	{
	}

void CTzLocalizedTimeZoneRecord::ConstructL(TInt aTzId, const TDesC& aStandardName, 
		const TDesC& aDaylightName,	const TDesC& aShortStandardName, const TDesC& aShortDaylightName)
	{
	if (aTzId > KTzIdMax)
		{
		// This is a mess, although most of the API allows for 32 bit tz ids some of them
		// only allow for 16 bit tz ids so we accept a TInt but do check that it can fit in 
		// 16 bits
		User::Leave(KErrArgument);
		}
	iId = aTzId;
	iStandardName = aStandardName.AllocL();
	iDaylightName = aDaylightName.AllocL();
	iShortStandardName = aShortStandardName.AllocL();
	iShortDaylightName = aShortDaylightName.AllocL();
	}

void CTzLocalizedTimeZoneRecord::ConstructL(RReadStream& aStream)
	{
	iId = aStream.ReadUint16L();
	iStandardName = ConstructNameL(aStream);
	iDaylightName = ConstructNameL(aStream);
	iShortStandardName = ConstructNameL(aStream);
	iShortDaylightName = ConstructNameL(aStream);
	iResourceId = aStream.ReadUint32L();
	}

HBufC* CTzLocalizedTimeZoneRecord::ConstructNameL(RReadStream& aStream)
	{
	TInt nameLength = aStream.ReadInt32L();
	HBufC* name = HBufC::NewLC(nameLength);
	TPtr16 namePtr = name->Des();
	aStream.ReadL(namePtr, nameLength);
	CleanupStack::Pop(name);
	return name;
	}

EXPORT_C TInt CTzLocalizedTimeZoneRecord::ExternalizeSize() const
	{
	return sizeof(TUint16) + sizeof(TInt32) + iStandardName->Size() + 
		sizeof(TInt32) + iDaylightName->Size() + sizeof(TInt32) + iShortStandardName->Size() +
		sizeof(TInt32) + iShortDaylightName->Size() + sizeof(TUint32);
	}

EXPORT_C void CTzLocalizedTimeZoneRecord::ExternalizeL(RWriteStream& aStream) const
	{
	aStream.WriteUint16L(iId);
	aStream.WriteInt32L(iStandardName->Length());
	aStream.WriteL(*iStandardName);
	aStream.WriteInt32L(iDaylightName->Length());
	aStream.WriteL(*iDaylightName);
	aStream.WriteInt32L(iShortStandardName->Length());
	aStream.WriteL(*iShortStandardName);
	aStream.WriteInt32L(iShortDaylightName->Length());
	aStream.WriteL(*iShortDaylightName);
	aStream.WriteUint32L(iResourceId);
	}

EXPORT_C TInt CTzLocalizedTimeZoneRecord::ExternalizeSize(const RPointerArray<CTzLocalizedTimeZoneRecord>& aTimeZones)
	{
	TInt bufferSize = sizeof(TInt32);
	TInt i = 0;
	TInt end = aTimeZones.Count();
	while (i < end)
		{
		bufferSize += aTimeZones[i]->ExternalizeSize();
		++i;
		}
	return bufferSize;
	}

EXPORT_C void CTzLocalizedTimeZoneRecord::ExternalizeL(const RPointerArray<CTzLocalizedTimeZoneRecord>& aTimeZones, 
		RWriteStream& aStream) 
	{
	TInt i = 0;
	TInt end = aTimeZones.Count();
	aStream.WriteInt32L(end);
	while (i < end)
		{
		aTimeZones[i]->ExternalizeL(aStream);
		++i;
		}
	}

EXPORT_C void CTzLocalizedTimeZoneRecord::InternalizeL(RReadStream& aStream, 
		RPointerArray<CTzLocalizedTimeZoneRecord>& aTimeZones)
	{
	TInt count = aStream.ReadInt32L();
	TInt i = 0;
	aTimeZones.ReserveL(aTimeZones.Count() + count);
	while (i < count)
		{
		CTzLocalizedTimeZoneRecord* newRecord = CTzLocalizedTimeZoneRecord::NewL(aStream);
		aTimeZones.Append(newRecord);
		++i;
		}
	}
