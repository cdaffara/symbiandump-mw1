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

#ifndef __TZSYSTEMDATA_H__
#define __TZSYSTEMDATA_H__

#include <tzlocalizationdatatypes.h>
#include <tz.h>
#include <barsc.h>
#include <barsread.h>
#include <f32file.h>

/**
Reads Localized time zone information and city information from resource files
@internalTechnology
*/
class CTzSystemDataDb : public CBase
	{
		//enums
/**
Enum to get to the various resources in the resource files. These can be added
to the offset returned by RResourceFile::Offset() to get the relevant resource
ID
*/
	enum TTzResourceOffset
		{
/**
Offset of the RSS Signature, this is always the first resource, so the enum
starts at 1.
*/
		ETzRssSignature = 1,
/**
Offset of the first localized resource. In the time zones resource file, this
is the default cached zones resource, in the groups resource files, this is the
first group.
*/
		ETzFirstLocalizedResource,
/**
Offset of the first localized time zone resource. This simply allows the
default cached zones resource to be skipped over in the time zones resource
file.
*/
		ETzFirstTimeZoneResource
		};

public:
	CTzLocalizedTimeZoneRecord* ReadTimeZoneL(TInt aTimeZoneId);
		void ReadCitiesL(RPointerArray<CTzLocalizedCityRecord>& aCities, TInt aTimeZoneId);
		CTzLocalizedCityRecord* ReadDefaultCityL(TInt aTimeZoneId);
		CTzLocalizedTimeZoneRecord* ReadFrequentlyUsedZoneL(TInt aFrequentlyUsedZone);
		TInt ReadFrequentlyUsedZoneIdL(TInt aFrequentlyUsedZone);
		static CTzSystemDataDb* NewLC();
		~CTzSystemDataDb();

	private:
		void ConstructL();
		TInt FindTimeZoneResourceIdL(TInt aTimeZoneId);
		TInt LocalizedResourceIdL(const RResourceFile& aResourceFile, const TTzResourceOffset aOffset);
		TInt FirstTimeZoneResourceIdL();
		TInt CachedTimeZoneResourceIdL();
		void BufferResourceL(const RResourceFile& aResourceFile, TInt aResourceId);
		void ResetResourceBuffer();
		CTzLocalizedTimeZoneRecord* DoReadTimeZoneL(TInt aResourceId);
		CTzLocalizedTimeZoneRecord* CreateTimeZoneRecordFromResourceL(TInt aResourceId);
		void DoReadCitiesL(RPointerArray<CTzLocalizedCityRecord>& aCities, TInt aResourceId);
		void AddCityArrayFromResourceL(RPointerArray<CTzLocalizedCityRecord>& aCities, TInt aResourceId);

	private:
		RFs iFs;
		RResourceFile iTimeZoneResourceFile;
		RResourceFile iGroupResourceFile;
		TBool iGroupResourceFileExists;
		TResourceReader iResourceReader;
		HBufC8* iResourceBuffer;
	};

#endif
