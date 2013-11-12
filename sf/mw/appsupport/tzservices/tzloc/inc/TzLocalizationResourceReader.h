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

#ifndef __TZLOCAL_RESOURCE_READER_H__
#define __TZLOCAL_RESOURCE_READER_H__

#include "TzLocalizationDataTypes.h"
#include <barsc.h>		// RResourceFile
#include <barsread.h>	// TResourceReader
#include <f32file.h>	// RFs

/**
Concrete implementation of MTzLocalizationReader
Reads Localized time zone information and city information from resource files
@internalTechnology
*/
class CTzLocalizationResourceReader : public CBase
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
		CTzLocalizedTimeZone* ReadTimeZoneL(const TInt aTimeZoneId);
		CTzLocalizedTimeZone* ReadTimeZoneL(const TTzLocalizedId& aTzLocalizedId);
		void ReadAllTimeZonesL(CTzLocalizedTimeZoneArray& aTimeZones);
		void ReadCitiesL(CTzLocalizedCityArray& aCities);
		void ReadCitiesL(CTzLocalizedCityArray& aCities, const TInt aTimeZoneId);
		void ReadCitiesL(CTzLocalizedCityArray& aCities, const TTzLocalizedId& aTzLocalizedId);
		void ReadCitiesInGroupL(CTzLocalizedCityArray& aCities, const TUint8 aGroupId);
		void ReadAllGroupsL(CTzLocalizedCityGroupArray& aGroups);
		CTzLocalizedCityGroup* ReadGroupL(const TUint8 aGroupId);
		CTzLocalizedCity* ReadDefaultCityL(const TInt aTimeZoneId);
		CTzLocalizedCity* ReadDefaultCityL(const TTzLocalizedId& aTzLocalizedId);
		CTzLocalizedTimeZone* ReadFrequentlyUsedZoneL(const CTzLocalizedTimeZone::TTzFrequentlyUsedZone aFrequentlyUsedZone);
		CTzLocalizedCity* ReadCachedTimeZoneCityL(const CTzLocalizedTimeZone::TTzFrequentlyUsedZone aFrequentlyUsedZone);

		//New Functions
		static CTzLocalizationResourceReader* NewL();
		static CTzLocalizationResourceReader* NewLC();
		~CTzLocalizationResourceReader();

	private:
		void ConstructL();
		TInt FindTimeZoneResourceIdL(const TInt aTimeZoneId);
		TInt LocalizedResourceIdL(const RResourceFile& aResourceFile, const TTzResourceOffset aOffset);
		TInt FirstTimeZoneResourceIdL();
		TInt FirstGroupResourceIdL();
		TInt CachedTimeZoneResourceIdL();
		void BufferResourceL(const RResourceFile& aResourceFile, const TInt aResourceId);
		void ResetResourceBuffer();
		CTzLocalizedTimeZone* DoReadTimeZoneL(const TInt aResourceId);
		CTzLocalizedTimeZone* CreateTimeZoneFromResourceL(const TInt aResourceId);
		void DoReadCitiesL(CTzLocalizedCityArray& aCities, const TInt aResourceId);
		void AddCityArrayFromResourceL(CTzLocalizedCityArray& aCities, const TInt aResourceId);
		CTzLocalizedCityGroup* DoReadGroupL(const TInt aResourceId);
		CTzLocalizedCityGroup* CreateGroupFromResourceL();

	private:
		RFs iFs;
		RResourceFile iTimeZoneResourceFile;
		RResourceFile iGroupResourceFile;
		TBool iGroupResourceFileExists;
		TResourceReader iResourceReader;
		HBufC8* iResourceBuffer;
	};

#endif //__TZLOCAL_RESOURCE_READER_H__
