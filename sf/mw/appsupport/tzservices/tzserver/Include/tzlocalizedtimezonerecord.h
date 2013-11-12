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

#ifndef __TZLOCALIZEDTIMEZONERECORD_H__
#define __TZLOCALIZEDTIMEZONERECORD_H__

#include <e32base.h>
#include <s32std.h>

/**
Class used instead of CTzLocalizedTimeZone which we can't use in TZ. 

@internalTechnology
*/
class CTzLocalizedTimeZoneRecord : public CBase
	{
public:
	IMPORT_C static CTzLocalizedTimeZoneRecord* NewL(TInt aTzId, const TDesC& aStandardName, 
		const TDesC& aDaylightName, const TDesC& aShortStandardName, const TDesC& aShortDaylightName,
		TUint aResourceId);
	IMPORT_C static CTzLocalizedTimeZoneRecord* NewLC(TInt aTzId, const TDesC& aStandardName, 
		const TDesC& aDaylightName, const TDesC& aShortStandardName, const TDesC& aShortDaylightName,
		TUint aResourceId);
	IMPORT_C static CTzLocalizedTimeZoneRecord* NewL(RReadStream& aStream);
	IMPORT_C static CTzLocalizedTimeZoneRecord* NewLC(RReadStream& aStream);
	IMPORT_C ~CTzLocalizedTimeZoneRecord();
	IMPORT_C static void CleanupArray(TAny* aArray);
	
private:
	CTzLocalizedTimeZoneRecord();
	CTzLocalizedTimeZoneRecord(TUint aResourceId);
	void ConstructL(TInt aTzId, const TDesC& aStandardName, const TDesC& aDaylightName,
		const TDesC& aShortStandardName, const TDesC& aShortDaylightName);
	void ConstructL(RReadStream& aStream);
	HBufC* ConstructNameL(RReadStream& aStream);
	
public:
	inline TUint16 Id() const;
	inline const TDesC& StandardName() const;
	inline const TDesC& DaylightName() const;
	inline const TDesC& ShortStandardName() const;
	inline const TDesC& ShortDaylightName() const;
	inline TUint ResourceId() const;
	
public:
	IMPORT_C TInt ExternalizeSize() const;
	IMPORT_C void ExternalizeL(RWriteStream& aStream) const;
	IMPORT_C static TInt ExternalizeSize(const RPointerArray<CTzLocalizedTimeZoneRecord>& aTimeZones);
	IMPORT_C static void ExternalizeL(const RPointerArray<CTzLocalizedTimeZoneRecord>& aTimeZones, 
		RWriteStream& aStream);
	IMPORT_C static void InternalizeL(RReadStream& aStream, RPointerArray<CTzLocalizedTimeZoneRecord>& aTimeZones);
	
private:
	TUint16 iId;
	HBufC* iStandardName;
	HBufC* iDaylightName;
	HBufC* iShortStandardName;
	HBufC* iShortDaylightName;
	TUint iResourceId;
	};
	
#include "tzlocalizedtimezonerecord.inl"
	
#endif
