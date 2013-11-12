// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <tzuserdefineddata.h>
#endif
#include "dataprovider.h"
#include "ReadOnlyTzDb.h"
#include "vtzrules.h"
#include "timezoneserver.h"

CTzDataProvider* CTzDataProvider::NewL(RFs& aFs, CTzSwiObserver& aSwiObserver, 
		CTzUserDataDb& aTzUserDataDb, MTzDataObserver& aTzDataObserver)
	{
	CTzDataProvider* self = new(ELeave) CTzDataProvider(aFs, aTzUserDataDb, aTzDataObserver);
	CleanupStack::PushL(self);
	self->ConstructL(aSwiObserver);
	CleanupStack::Pop(self);
	return self;
	}

CTzDataProvider::CTzDataProvider(RFs& aFs, CTzUserDataDb& aTzUerDataDb, MTzDataObserver& aTzDataObserver)
:	iFs(aFs),
	iTzDataObserver(aTzDataObserver),
	iUserDataTzDb(aTzUerDataDb)
	{
	}

void CTzDataProvider::ConstructL(CTzSwiObserver& aSwiObserver)
	{
	iReadOnlyTzDb = CReadOnlyTzDb::NewL(iFs, aSwiObserver, iTzDataObserver);
	}

CTzDataProvider::~CTzDataProvider()
	{
	delete iReadOnlyTzDb;
	}

void CTzDataProvider::ReleaseTzRules()
	{
	iReadOnlyTzDb->ReleaseData();
	}

TBool CTzDataProvider::IsIdInDbL(TUint aTzId)
	{
	if(CTzId::IsUserTzId(aTzId))
		{
		return iUserDataTzDb.IsIdInDbL(aTzId);
		}
	else
		{
		return iReadOnlyTzDb->IsIdInDbL(aTzId);	
		}
	}

void CTzDataProvider::GetTzRulesL(CTzRules& aTzRules, CTzId& aTzId) const
	{
	if(aTzId.IsUserTzId())
		{
		iUserDataTzDb.ReadTzRulesL(aTzRules, aTzId.TimeZoneNumericID());
		}
	else
		{
		iReadOnlyTzDb->GetTzRulesL(aTzRules, aTzId);	
		}
	}

CTzId* CTzDataProvider::GetDefaultTimeZoneIdL() const
	{
	return iReadOnlyTzDb->GetDefaultTimeZoneIdL();
	}

/**
Retrieves the UTC offset for an array of numeric time zone identifiers.  The
offset is written back into aTimeZoneIdArray at the position that the numeric
identifier was read from, overwriting the original identifier.

@param aTimeZoneIdArray Flat buffer consisting of a TInt for the number of
identifiers followed by the numeric time zone identifiers (TInt's) for which the
current UTC offset is required.
*/
void CTzDataProvider::GetOffsetsForTimeZoneIdsL(CBufFlat& aTimeZoneIdArray) const
	{
	const TInt KElementSize = sizeof(TInt);
	
	// Get number of time zone identifiers in array.
	TInt arrayOffset = 0;
	TInt numIds = 0;
	aTimeZoneIdArray.Read(arrayOffset, &numIds, KElementSize);

	// Advance array offset to first time zone identifier.
	arrayOffset += KElementSize;

	// To obtain the current UTC offset we need to know the current UTC time.
	TTime now;
	now.UniversalTime();

	while(arrayOffset <= numIds*KElementSize)
		{
		TInt id = 0;
		
		// Get the time zone identifier at the current array position.
		aTimeZoneIdArray.Read(arrayOffset, &id, KElementSize);
		
		TInt utcOffset = 0;
		
		if(CTzId::IsUserTzId(id))
			{
			CTzRules* rules = CTzRules::NewL(0, KMaxYear);
			CleanupStack::PushL(rules);
			TRAPD(err, iUserDataTzDb.ReadTzRulesL(*rules, id));

			// Assume the rules are not found and therefore that the UTC offset
			// shall be 0 (this follows the behaviour of CReadOnlyTzDb::-
			// GetOffsetForTimeZoneIdL()).
			utcOffset = 0;

			if(err != KErrNotFound)
				{
				User::LeaveIfError(err);
				utcOffset = rules->GetOffsetL(now, ETzUtcTimeReference);
				}

			CleanupStack::PopAndDestroy(rules);
			}
		else
			{
			utcOffset = iReadOnlyTzDb->GetOffsetForTimeZoneIdL(now, id);	
			}

		// Replace the time zone identifier with the UTC offset.
		aTimeZoneIdArray.Write(arrayOffset, &utcOffset, KElementSize);

		// Advance array offset to next time zone identifier.
		arrayOffset += KElementSize;
		}
	}

void CTzDataProvider::BackupBeginningL()
	{
	iReadOnlyTzDb->BackupBeginningL();
	iUserDataTzDb.BackupBeginningL();
	}

void CTzDataProvider::BackupCompletedL()
	{
	iReadOnlyTzDb->BackupCompletedL();
	iUserDataTzDb.BackupCompletedL();
	}
void CTzDataProvider::RestoreBeginningL()
	{
	iReadOnlyTzDb->RestoreBeginningL();
	iUserDataTzDb.RestoreBeginningL();
	}

void CTzDataProvider::RestoreCompletedL()
	{
	iReadOnlyTzDb->RestoreCompletedL();
	iUserDataTzDb.RestoreCompletedL();
	}
