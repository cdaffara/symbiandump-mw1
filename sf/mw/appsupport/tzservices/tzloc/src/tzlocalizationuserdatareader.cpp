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

#include "tzlocalizationuserdatareader.h"
#include "TzLocalizationDataTypes.h"
#include "TzLocalizer.h"
#include <tz.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <tzusernames.h>
#include <tzuserdefineddata.h>
#endif

LOCAL_D void ResetAndDestroyTzIdArray(TAny* aPtr)
	{
	RPointerArray<CTzId>* ids = static_cast<RPointerArray<CTzId>*>(aPtr);
	if (ids)
		{
		ids->ResetAndDestroy();
		ids->Close();
		}
	}

CTzLocalizationUserDataReader::CTzLocalizationUserDataReader(RTz& aTzSession)
	:iTzSession(aTzSession)
	{
	}

void CTzLocalizationUserDataReader::ConstructL()
	{
	iUserData = CTzUserData::NewL(iTzSession);
	}
/**
Allocates and constructs a new CTzLocalizationUserDataReader
@return the newly constructed CTzLocalizationUserDataReader
@internalTechnology
*/
CTzLocalizationUserDataReader* CTzLocalizationUserDataReader::NewL(RTz& aTzSession)
	{
	CTzLocalizationUserDataReader* self = CTzLocalizationUserDataReader::NewLC(aTzSession);
	CleanupStack::Pop(self);
	return self;
	}

/**
Allocates and constructs a new CTzLocalizationUserDataReader
The pointer to the new object is left on the cleanup stack
@return the newly constructed CTzLocalizationUserDataReader
@internalTechnology
*/
CTzLocalizationUserDataReader* CTzLocalizationUserDataReader::NewLC(RTz& aTzSession)
	{
	CTzLocalizationUserDataReader* self = new (ELeave) CTzLocalizationUserDataReader(aTzSession);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

/**
Destructor - frees resources
@internalTechnology
*/
CTzLocalizationUserDataReader::~CTzLocalizationUserDataReader()
	{
	delete iUserData;
	}

/** Read the time zone with the matching time zone identifier
@param aTimeZoneId - time zone id
@return CTzLocalizedTimeZone* the localized time zone with matching id
@internalTechnology
*/
CTzLocalizedTimeZone* CTzLocalizationUserDataReader::ReadTimeZoneL(const TInt aTimeZoneId)
	{
	return GetLocalizedTimeZoneL(aTimeZoneId);
	}

/** Read the time zone with the matching TTzLocalizedId
@param aTzLocalizedId - time zone id
@return CTzLocalizedTimeZone* the localized time zone with matching id
@internalTechnology
*/
CTzLocalizedTimeZone* CTzLocalizationUserDataReader::ReadTimeZoneL(const TTzLocalizedId& aTzLocalizedId)
	{
	return ReadTimeZoneL(aTzLocalizedId.TimeZoneId());
	}

/** Read all time zones stored in user database
@internalTechnology
@param aTimeZones array of localized time zones.
*/
void CTzLocalizationUserDataReader::ReadAllTimeZonesL(CTzLocalizedTimeZoneArray& aTimeZones)
	{
	RPointerArray<CTzId> tzids;
	CleanupStack::PushL(TCleanupItem(ResetAndDestroyTzIdArray, &tzids));
	iUserData->GetTzIdsL(tzids);
	const TInt count = tzids.Count();
	aTimeZones.ReserveL(count + aTimeZones.Count());
	for(TInt ii=0; ii<count; ++ii)
		{
		CTzLocalizedTimeZone* localizedTz = GetLocalizedTimeZoneL(tzids[ii]->TimeZoneNumericID());
		aTimeZones.AppendL(localizedTz);
		}
	CleanupStack::PopAndDestroy(&tzids);
	}

/** Reads all the user defined cities stored in the
database into the specified CTzLocalizedCityArray. If the database is empty the
returned array will contain no elements.
@param aCities Array of cities to add the user defined to.
@internalTechnology
*/
void CTzLocalizationUserDataReader::ReadCitiesL(CTzLocalizedCityArray& aCities)
	{
	RPointerArray<CTzId> tzids;
	CleanupStack::PushL(TCleanupItem(ResetAndDestroyTzIdArray, &tzids));
	iUserData->GetTzIdsL(tzids);
	const TInt count = tzids.Count();
	aCities.ReserveL(count + aCities.Count());
	for(TInt ii=0; ii<count; ++ii)
		{
		CTzLocalizedCity* loccity = GetLocalizedCityL(tzids[ii]->TimeZoneNumericID());
		aCities.AppendL(loccity);
		}
	CleanupStack::PopAndDestroy(&tzids);
	}

/** Reads the city with the matching aTimeZoneid into the specifed CTzLocalizedCityArray.
Note that there only one city in a given user defined time zone.
If the database is empty the returned array will contain no elements.
@param aCities Array of citeis to add the user defined cities to.
@param aTimeZoneId A time zone id identifying a time zone
@internalTechnology
*/
void CTzLocalizationUserDataReader::ReadCitiesL(CTzLocalizedCityArray& aCities, const TInt aTimeZoneId)
	{
	CTzLocalizedCity* loccity = GetLocalizedCityL(aTimeZoneId);
	CleanupStack::PushL(loccity);
	aCities.AppendL(loccity);
	CleanupStack::Pop(loccity);
	}

/** Reads all cities in user defined time zone database into the specified CTzLocalizedCityArray. 
Note that this method should be called only if aGroupId is KTzCityNotInGroupId since all of cities are groupless.
If the database is empty the returned array will contain no elements.
@param aCities Array of cities to add the user defined cities to.
@param aGroupId A city group id
@internalTechnology
*/
void CTzLocalizationUserDataReader::ReadCitiesInGroupL(CTzLocalizedCityArray& aCities, const TUint8 aGroupId)
	{
	__ASSERT_ALWAYS(aGroupId == KTzCityNotInGroupId, User::Leave(KErrNotFound));
	ReadCitiesL(aCities); 
	}

/** Reads all groups in user defined time zone database into the specified CTzLocalizedCityGroupArray. 
Note that each time zone is formed as an individule CTzLocalizedCityGroup object since they do not belong to the same group.
If the database is empty the returned array will contain no elements.
@param aCities Array of groups.
@internalTechnology
*/
void CTzLocalizationUserDataReader::ReadAllGroupsL(CTzLocalizedCityGroupArray& aCities)
	{
	RPointerArray<CTzId> tzids;
	CleanupStack::PushL(TCleanupItem(ResetAndDestroyTzIdArray, &tzids));
	iUserData->GetTzIdsL(tzids);
	TInt count = tzids.Count();
	aCities.ReserveL(count + aCities.Count());
	for(TInt ii=0; ii<count; ++ii)
		{
		CTzLocalizedCityGroup* group = GetLocalizedCityGroupL(tzids[ii]->TimeZoneNumericID());
		aCities.AppendL(group);
		}
	CleanupStack::PopAndDestroy(&tzids);
	}

/** Reads the default city with the matching time zone ID.
Note that there is only one city in a given user defined time zone.
@param aTimeZoneId
@internalTechnology
*/
CTzLocalizedCity* CTzLocalizationUserDataReader::ReadDefaultCityL(const TInt aTimeZoneId)
	{
	return GetLocalizedCityL(aTimeZoneId);
	}

/*************************************** Private Methods*****************************/

/* Get user names for a given time zone identifier
@param aTimeZoneId A time zone identifier
@param aUserData A reference of CTzUserData
@return A pointer to CTzUserNames
@internalComponent
*/
CTzUserNames* CTzLocalizationUserDataReader::GetTzUserNamesLC(TInt aTimeZoneId)
	{
	CTzId* tzid = CTzId::NewL(aTimeZoneId);
	CleanupStack::PushL(tzid);
	CTzUserNames* names = iUserData->ReadNamesL(*tzid);
	CleanupStack::PopAndDestroy(tzid);
	CleanupStack::PushL(names);
	return names;
	}

CTzLocalizedTimeZone* CTzLocalizationUserDataReader::GetLocalizedTimeZoneL(TInt aTimeZoneId)
	{
	CTzUserNames* names = GetTzUserNamesLC(aTimeZoneId);
	TTzLocalizedId id(aTimeZoneId, 0);
	CTzLocalizedTimeZone* locTimeZone = CTzLocalizedTimeZone::NewL(id, names->StandardName(), names->DaylightSaveName(), names->ShortStandardName(), names->ShortDaylightSaveName());	
	CleanupStack::PopAndDestroy(names);
	return locTimeZone;
	}

CTzLocalizedCity* CTzLocalizationUserDataReader::GetLocalizedCityL(TInt aTimeZoneId)
	{
	CTzLocalizedCity* localisedCity = NULL;
	CTzUserNames* names = GetTzUserNamesLC(aTimeZoneId);
	TTzLocalizedId id(aTimeZoneId, 0);
	localisedCity = CTzLocalizedCity::NewL(names->CityName(), id);	
	localisedCity->SetCityIndex(0);		
	CleanupStack::PopAndDestroy(names);
	return localisedCity;
	}

CTzLocalizedCityGroup* CTzLocalizationUserDataReader::GetLocalizedCityGroupL(TInt aTimeZoneId)
	{
	CTzLocalizedCityGroup* localisedGroup = NULL;
	CTzUserNames* names = GetTzUserNamesLC(aTimeZoneId);
	localisedGroup = CTzLocalizedCityGroup::NewL(names->RegionName(), KTzCityNotInGroupId);	
	CleanupStack::PopAndDestroy(names);
	return localisedGroup;
	}
