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

#include "tzlocalizedcityrecord.h"
#include "tzid.h"
#include "tzidinternal.h"

EXPORT_C CTzLocalizedCityRecord* CTzLocalizedCityRecord::NewL(const TDesC& aName, TUint8 aGroupId, TInt aIndex,
		TInt aTzId, TUint aTzResourceId)
	{
	CTzLocalizedCityRecord* self = CTzLocalizedCityRecord::NewLC(aName, aGroupId, aIndex, aTzId, aTzResourceId);
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CTzLocalizedCityRecord* CTzLocalizedCityRecord::NewLC(const TDesC& aName, TUint8 aGroupId, TInt aIndex,
		TInt aTzId, TUint aTzResourceId)
	{
	CTzLocalizedCityRecord* self = new(ELeave) CTzLocalizedCityRecord(aGroupId, aIndex, aTzResourceId);
	CleanupStack::PushL(self);
	self->ConstructL(aName, aTzId);
	return self;
	}

EXPORT_C CTzLocalizedCityRecord* CTzLocalizedCityRecord::NewL(RReadStream& aStream)
	{
	CTzLocalizedCityRecord* self = CTzLocalizedCityRecord::NewLC(aStream);
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CTzLocalizedCityRecord* CTzLocalizedCityRecord::NewLC(RReadStream& aStream)
	{
	CTzLocalizedCityRecord* self = new(ELeave) CTzLocalizedCityRecord();
	CleanupStack::PushL(self);
	self->ConstructL(aStream);
	return self;
	}

EXPORT_C CTzLocalizedCityRecord::~CTzLocalizedCityRecord()
	{
	delete iName;
	}

EXPORT_C void CTzLocalizedCityRecord::CleanupArray(TAny* aArray)
	{
	RPointerArray<CTzLocalizedCityRecord>* array = static_cast<RPointerArray<CTzLocalizedCityRecord>* >(aArray);
	if (array)
		{
		array->ResetAndDestroy();
		array->Close();
		}
	}

CTzLocalizedCityRecord::CTzLocalizedCityRecord()
	{
	}

CTzLocalizedCityRecord::CTzLocalizedCityRecord(TUint8 aGroupId, TInt aIndex, TUint aResourceId)
	: iGroupId(aGroupId), iIndex(aIndex), iTzResourceId(aResourceId)
	{
	}

void CTzLocalizedCityRecord::ConstructL(const TDesC& aName, TInt aTzId)
	{
	if (aTzId > KTzIdMax)
		{
		// This is a mess, although most of the API allows for 32 bit tz ids some of them
		// only allow for 16 bit tz ids so we accept a TInt but do check that it can fit in 
		// 16 bits
		User::Leave(KErrArgument);
		}
	iTzId = aTzId;
	iName = aName.AllocL();
	}

void CTzLocalizedCityRecord::ConstructL(RReadStream& aStream)
	{
	TInt nameLength = aStream.ReadInt32L();
	iName = HBufC::NewL(nameLength);
	TPtr16 namePtr = iName->Des();
	aStream.ReadL(namePtr, nameLength);
	iGroupId = aStream.ReadUint8L();
	iIndex = aStream.ReadInt32L();
	iTzId = aStream.ReadUint16L();
	iTzResourceId = aStream.ReadUint32L();
	}

EXPORT_C TInt CTzLocalizedCityRecord::ExternalizeSize() const
	{
	return sizeof(TInt32) + iName->Size() + sizeof(TUint8) + sizeof(TInt32) + sizeof(TUint16) + sizeof(TUint32);
	}

EXPORT_C void CTzLocalizedCityRecord::ExternalizeL(RWriteStream& aStream) const
	{
	aStream.WriteInt32L(iName->Length());
	aStream.WriteL(*iName);
	aStream.WriteUint8L(iGroupId);
	aStream.WriteInt32L(iIndex);
	aStream.WriteUint16L(iTzId);
	aStream.WriteUint32L(iTzResourceId);
	}

EXPORT_C TInt CTzLocalizedCityRecord::ExternalizeSize(const RPointerArray<CTzLocalizedCityRecord>& aCities)
	{
	TInt bufferSize = sizeof(TInt32);
	TInt i = 0;
	TInt end = aCities.Count();
	while (i < end)
		{
		bufferSize += aCities[i]->ExternalizeSize();
		++i;
		}
	return bufferSize;
	}

EXPORT_C void CTzLocalizedCityRecord::ExternalizeL(const RPointerArray<CTzLocalizedCityRecord>& aCities, 
	RWriteStream& aStream)
	{
	TInt i = 0;
	TInt end = aCities.Count();
	aStream.WriteInt32L(end);
	while (i < end)
		{
		aCities[i]->ExternalizeL(aStream);
		++i;
		}
	}

EXPORT_C void CTzLocalizedCityRecord::InternalizeL(RReadStream& aStream, 
		RPointerArray<CTzLocalizedCityRecord>& aCities)
	{
	TInt count = aStream.ReadInt32L();
	TInt i = 0;
	aCities.ReserveL(aCities.Count() + count);
	while (i < count)
		{
		CTzLocalizedCityRecord* newRecord = CTzLocalizedCityRecord::NewL(aStream);
		aCities.Append(newRecord);
		++i;
		}
	}
