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

#include <s32mem.h>
#include "testtz.h"

CTestTzId::CTestTzId()
	{
	}


/**
Time zone ID data externaliser.
@param aStream A stream that will contain the serialised time zone ID.
@internalComponent
*/
void CTestTzId::ExternalizeL(RWriteStream& aStream) const
	{
	aStream << iReferenceId;
	TInt32 size	= 0;
	if (iZoneId)
		{
		size = iZoneId->Length();
		aStream << size;
		if (size)
			{
			aStream.WriteL(iZoneId->Des(), size);
			}
		}
	else
		{
		aStream << size;
		}
	}

/**
Serialised CTestTzId data Internaliser
@param aStream a stream containing the serialised time zone Id
@leave KErrNoMemory or some other system error code
@internalComponent
*/
void CTestTzId::InternalizeL(RReadStream& aStream)
	{

	aStream >> iReferenceId;

	delete iZoneId;
	iZoneId = NULL;

	TInt32 size;
	aStream >> size;
	if (size>0)
		{
		iZoneId = HBufC8::NewL(size);
		TPtr8 bufferPtr = iZoneId->Des();
		aStream.ReadL(bufferPtr, size);
		}
	}


/**
Destructor.
*/
CTestTzId::~CTestTzId()
	{
	delete iZoneId;
	}


/**
Time zone ID factory method.
@param aStream a stream containing the serialised time zone Id
@leave KErrNoMemory or some other system error code
@return Pointer to CTestTzId; clients own the object.
@internalComponent
*/
CTestTzId* CTestTzId::NewL(RReadStream& aStream)
	{

	CTestTzId* self = new(ELeave) CTestTzId();
	CleanupStack::PushL(self);

	self->InternalizeL(aStream);

	CleanupStack::Pop(self);
	return self;
	}


/**
Time zone ID factory method.
@param aNameIdentity A name recognised by the time zone database.
@return A pointer to the time zone ID. Clients take ownership.
*/
CTestTzId* CTestTzId::NewL(const TDesC8& aNameIdentity)
	{
	CTestTzId* self = new(ELeave) CTestTzId();
	CleanupStack::PushL(self);

	self->iZoneId = aNameIdentity.AllocL();

	CleanupStack::Pop(self);
	return self;
	}

/**
Retrieves the time zone's name.
This can be used after calling RTz::GetTimeZoneIdL().

@return The time zone's name, if one has been set. A NULL descriptor if not.
*/
const TDesC8& CTestTzId::TimeZoneNameID() const
	{
	if (NULL == iZoneId)
		{
		return KNullDesC8();
		}

	return *iZoneId;
	}
