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
#ifndef __TIMEZONE_USERNAMES_H__
#define __TIMEZONE_USERNAMES_H__

#include <e32base.h>

class RWriteStream; 
/**
The CTzUserNames class is used to encapsulate the names for a user-defined
time zone.

When creating an instance of this class the client can specify the following
names:

- Long standard time name, for example "Mountain Standard Time"
- Short standard time name, for example "MST"
- Long daylight saving time name, for example "Mountain Daylight Time"
- Short daylight saving time name, for example "MDT"
- City name, for example "Shiprock"
- Region name, for example "America"

The client can also read these names.  

@publishedPartner
@released
*/ 
class CTzUserNames : public CBase
    {
public:
    IMPORT_C static CTzUserNames* NewL(const TDesC& aStandardName,
    	const TDesC& aShortStandardName, const TDesC& aDaylightSaveName,
    	const TDesC& aShortDaylightSaveName, const TDesC& aCityName,
    	const TDesC& aRegionName);
    IMPORT_C static CTzUserNames* NewLC(const TDesC& aStandardName,
    	const TDesC& aShortStandardName, const TDesC& aDayLightSaveName,
    	const TDesC& aShortDaylightSaveName, const TDesC& aCityName,
    	const TDesC& aRegionName);
	IMPORT_C ~CTzUserNames();
	
	IMPORT_C const TDesC& StandardName() const;
	IMPORT_C const TDesC& ShortStandardName() const;
	IMPORT_C const TDesC& DaylightSaveName() const;
	IMPORT_C const TDesC& ShortDaylightSaveName() const;
	IMPORT_C const TDesC& CityName() const;
	IMPORT_C const TDesC& RegionName() const;
	
	IMPORT_C static CTzUserNames* NewL(RReadStream& aStream);
	IMPORT_C void ExternalizeL(RWriteStream& aStream) const;
	IMPORT_C void InternalizeL(RReadStream& aStream);
	IMPORT_C TInt SizeOfObject() const;

private:
    CTzUserNames();
	void ConstructL(const TDesC& aStandardName, const TDesC& aShortStandardName,
		const TDesC& aDayLightSaveName, const TDesC& aShortDayLightSaveName,
		const TDesC& aCityName, const TDesC& aRegionName);
	
private:
    HBufC* iStandardName;
    HBufC* iShortStandardName;
    HBufC* iDaylightName;
    HBufC* iShortDaylightName;	
    HBufC* iCityName;
    HBufC* iRegionName;
    };

#endif //__TIMEZONE_USERNAMES_H__
