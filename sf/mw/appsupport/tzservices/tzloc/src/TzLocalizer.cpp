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

#include "TzLocalizer.h"

#include <tzconverter.h>					// For CTzConverter.
#include <centralrepository.h>				// For CRepository.

#include "TzLocalizationSortFunctions.h"
#include "TzLocalizationDbAccessor.h"
#include "TzLocalizationResourceReader.h"
#include "tzlocalizationuserdatareader.h"
#include <tzlocalizedtimezonerecord.h>
#include <tzlocalizedcityrecord.h>
/**
Allocates and constructs a new CTzLocalizer object.
@return The newly created CTzLocalizer object.
*/
EXPORT_C CTzLocalizer* CTzLocalizer::NewL()
	{
	CTzLocalizer* self = CTzLocalizer::NewLC();
	CleanupStack::Pop(self);
	return self;
	}

/**
Allocates and constructs a new CTzLocalizer object.
The pointer to the new object is left on the cleanup stack.
@return The newly created CTzLocalizer object.
*/
EXPORT_C CTzLocalizer* CTzLocalizer::NewLC()
	{
	CTzLocalizer* self = new (ELeave) CTzLocalizer();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

EXPORT_C CTzLocalizer::~CTzLocalizer()
	{
	delete iStaticDataReader;
	delete iUserDataReader;
	iTzSession.Close();
	}

CTzLocalizer::CTzLocalizer()
	: iLanguage(User::Language())
	{
	iDataSource = ETzDataSourceSystem;
	}

void CTzLocalizer::ConstructL()
	{
	User::LeaveIfError(iTzSession.Connect());

	iStaticDataReader = CTzLocalizationResourceReader::NewL();
   	iUserDataReader	= CTzLocalizationUserDataReader::NewL(iTzSession);

	if (DbNeedsUpdatingL())
		{
        TRAP_IGNORE(UpgradeDbVersionL());
		}

	SetDataSource(ETzDataSourceSystem | ETzDataSourceUser);
	if(!PrepareFrequentlyUsedZonesL())
		{
		// The cached zones DB was not updated using the defaults, so update it
		// using the time zone IDs that are already in the DB.  This ensures
		// that the language of the strings in the cached zone DB is correct on
		// construction.
		UpdateFrequentlyUsedZonesL();
		}
	SetDataSource(ETzDataSourceSystem);
	}

// Check if the DB needs upgrading
TBool CTzLocalizer::DbNeedsUpdatingL() const
    {
    CTzLocalizationDbAccessor* dbAccessor = CTzLocalizationDbAccessor::NewLC();
    TBool dbNeedsUpdating = dbAccessor->DbNeedsUpdatingL();
    CleanupStack::PopAndDestroy(dbAccessor);
    return dbNeedsUpdating;
    }

/**
Allows the client to specify the data source which should be used for
CTzLocalizer methods which can return a combination of system time zone
localization information and user-defined time zone names.

The following CTzLocalizer methods are affected by the use of this method:

- GetAllTimeZonesL()
- GetCitiesL()
- GetCitiesInGroupL()
- GetAllCityGroupsL()

@param aDataSource The data source to use.  The permissible values for this
argument are defined by the TTzLocalizerDataSource type.

@released
*/
EXPORT_C  void CTzLocalizer::SetDataSource(TUint aDataSource)
	{
	iDataSource = aDataSource;
	}

void CTzLocalizer::UpgradeDbVersionL()
    {
    // Fetch the cached zones from the old database
    CTzLocalizedTimeZoneArray* timeZones = CTzLocalizedTimeZoneArray::NewLC();
    CTzLocalizedCityArray* cities = CTzLocalizedCityArray::NewLC();

    FetchCityToUpgradeL(*timeZones, *cities, CTzLocalizedTimeZone::ECurrentZone);
    FetchCityToUpgradeL(*timeZones, *cities, CTzLocalizedTimeZone::EHomeZone);
    FetchCityToUpgradeL(*timeZones, *cities, CTzLocalizedTimeZone::EInterestZone);

    // delete the database and re-create it in the latest format
    RecreateDbL();

    // rewrite the cached zones
    for (TInt i = CTzLocalizedTimeZone::ECurrentZone; i <= CTzLocalizedTimeZone::EInterestZone; ++i)
        {
        SetFrequentlyUsedZoneL(timeZones->At(i), cities->At(i), CTzLocalizedTimeZone::TTzFrequentlyUsedZone(i));
        }

    CleanupStack::PopAndDestroy(cities);
    CleanupStack::PopAndDestroy(timeZones);
    }

// This function fetches a cached city and time zone as part of the DB upgrade
void CTzLocalizer::FetchCityToUpgradeL(CTzLocalizedTimeZoneArray& aTimeZoneArray, CTzLocalizedCityArray& aCityArray, CTzLocalizedTimeZone::TTzFrequentlyUsedZone aCachedZone)
    {
    CTzLocalizedTimeZone* zone = GetFrequentlyUsedZoneL(aCachedZone);
    CleanupStack::PushL(zone);
    aTimeZoneArray.AppendL(zone);
    CleanupStack::Pop(zone);

    CTzLocalizedCity* city = GetFrequentlyUsedZoneCityL(aCachedZone);
    CleanupStack::PushL(city);
    aCityArray.AppendL(city);
    CleanupStack::Pop(city);

    //  Find the city from resource file with matched name defined in given "city" and set its city index to "city"
    TBool hasFound = FindCityAndSetCityIndexL(*city, ETzDataSourceSystem);
    //  If not found and user defined data source is considered, find the city from user defined database with matched name defined in given "city" and set its city index to "city"
    if (!hasFound && iDataSource&ETzDataSourceUser)
	   {
	   FindCityAndSetCityIndexL(*city, ETzDataSourceUser);
	   }
   }

// Close and delete old DB, and create a new one (in latest version)
void CTzLocalizer::RecreateDbL()
    {
    CTzLocalizationDbAccessor* dbAccessor = CTzLocalizationDbAccessor::NewLC();
    dbAccessor->RecreateDbL(iTzSession);
    CleanupStack::PopAndDestroy(dbAccessor);
    }

/**
Populates the cached zones data in the persisted data writer with the default
values from the time zone server (for the current zone) or the static data
reader for the other zones.
@return ETrue if the cached Zone DB table had to be prepared using the defaults,
		EFalse if not.
*/
TBool CTzLocalizer::PrepareFrequentlyUsedZonesL()
	{
	CTzLocalizedTimeZoneArray* frequentlyUsedZones = CTzLocalizedTimeZoneArray::NewLC();
	CTzLocalizedCityArray* cachedCities = CTzLocalizedCityArray::NewLC();
	TBool frequentlyUsedZonesNeedUpdating = EFalse;

	// Loop to fill up the frequentlyUsedZones and cachedCities arrays to send to
	// WriteAllZonesL().  It also keeps track of whether the update is actually necessary
	for(TInt i = CTzLocalizedTimeZone::ECurrentZone; i < CTzLocalizedTimeZone::ECachedTimeZones; ++i)
		{
		CTzLocalizedTimeZone::TTzFrequentlyUsedZone freqUsedZone = (CTzLocalizedTimeZone::TTzFrequentlyUsedZone)i;
		CTzLocalizedTimeZoneRecord* timeZoneRecord = iTzSession.LocalizationReadFrequentlyUsedZoneL(freqUsedZone);
		CleanupStack::PushL(timeZoneRecord);
		CTzLocalizedTimeZone* timeZone = CreateTimeZoneL(*timeZoneRecord);
		CleanupStack::PopAndDestroy(timeZoneRecord);
		CleanupStack::PushL(timeZone);

		CTzLocalizedCityRecord* cityRecord = iTzSession.LocalizationReadCachedTimeZoneCityL(freqUsedZone);
		CleanupStack::PushL(cityRecord);
		CTzLocalizedCity* city = CreateCityL(*cityRecord);
		CleanupStack::PopAndDestroy(cityRecord);
		CleanupStack::PushL(city);

		// Check if the persisted cached zone is not set yet.
		if (timeZone->TimeZoneId() == 0 || city->TimeZoneId() == 0)
			{
			CleanupStack::PopAndDestroy(city);
			city = NULL;
			CleanupStack::PopAndDestroy(timeZone);
			timeZone = NULL;

			if(freqUsedZone == CTzLocalizedTimeZone::ECurrentZone)
				{
				TInt id = GetTimeZoneIdFromTzServerL();
				timeZone = GetLocalizedTimeZoneL(id, EFalse);
				}
			else
				{
				timeZone = GetFrequentlyUsedDefaultZoneL(freqUsedZone);
				}
			CleanupStack::PushL(timeZone);
			city = ReadDefaultCityL(timeZone->TimeZoneId(), EFalse);
			CleanupStack::PushL(city);
			frequentlyUsedZonesNeedUpdating = ETrue;
			}

		cachedCities->AppendL(city);
		// ownership is transferred into the array
		CleanupStack::Pop(city);
		frequentlyUsedZones->AppendL(timeZone);
		// ownership is transferred into the array
		CleanupStack::Pop(timeZone);
		}

	if (frequentlyUsedZonesNeedUpdating)
		{
		WriteAllFrequentlyUsedZonesL(*frequentlyUsedZones, *cachedCities);
		}

	CleanupStack::PopAndDestroy(cachedCities);
	CleanupStack::PopAndDestroy(frequentlyUsedZones);

	return frequentlyUsedZonesNeedUpdating;
	}

/**
Updates the cached zone information in the persisted data writer with the
strings that are currently in static data.
*/
void CTzLocalizer::UpdateFrequentlyUsedZonesL()
	{
	CTzLocalizedTimeZoneArray* frequentlyUsedZones = CTzLocalizedTimeZoneArray::NewLC();
	CTzLocalizedCityArray* cachedCities = CTzLocalizedCityArray::NewLC();

	for(TInt i = CTzLocalizedTimeZone::ECurrentZone; i < CTzLocalizedTimeZone::ECachedTimeZones; ++i)
		{
		CTzLocalizedTimeZone::TTzFrequentlyUsedZone freqUsedZone = (CTzLocalizedTimeZone::TTzFrequentlyUsedZone)i;

		// Find out what the time zone Id of the cached zone is
		CTzLocalizedTimeZoneRecord* timeZoneRecord = iTzSession.LocalizationReadFrequentlyUsedZoneL(freqUsedZone);
		TInt timeZoneId = timeZoneRecord->Id();
		delete timeZoneRecord;

		// Update the time zone
		CTzLocalizedTimeZone* timeZone = GetLocalizedTimeZoneL(timeZoneId, EFalse);
		CleanupStack::PushL(timeZone);


		// Does the cached city still exist in this time zone?

		// Get all the cities in this cached zone
		CTzLocalizedCityArray* citiesInTimeZone = GetCitiesL(*timeZone);
		CleanupStack::PushL(citiesInTimeZone);

		TIdentityRelation<CTzLocalizedCity> identity(NTzLocalSortAndFind::FindCityByName);

		// search for the city that was used to set this cached zone
		// in the previous language (it might be a user defined city or
		// it might have the same name in the new language)
		CTzLocalizedCityRecord* cityRecord = iTzSession.LocalizationReadCachedTimeZoneCityL(freqUsedZone);
		CleanupStack::PushL(cityRecord);
		CTzLocalizedCity* city = CreateCityL(*cityRecord);
		CleanupStack::PopAndDestroy(cityRecord);
		CleanupStack::PushL(city);
		TInt index = citiesInTimeZone->Find(city,identity);
        if (index == KErrNotFound)
            {
            // if city with this name can't be found, use the city with the same index in this time zone group
            if (city->CityIndex() >= 0 && city->CityIndex() < citiesInTimeZone->Count())
                {
                index = city->CityIndex();
                }
            }
		CleanupStack::PopAndDestroy(city);
		city = NULL;

		if(index != KErrNotFound)
		// city still exists in the new language
			{
			// Point city to array member.
			// This ensures all city info is valid
			city = &(citiesInTimeZone->At(index));
			// we're taking ownership of this element, so remove it
			// from the array
			citiesInTimeZone->Remove(index);
			}
		else
		// city not found - use the default city for the zone instead
			{
			city = ReadDefaultCityL(timeZone->TimeZoneId(), EFalse);
			}

		CleanupStack::PopAndDestroy(citiesInTimeZone);

		CleanupStack::PushL(city);
		cachedCities->AppendL(city);
		CleanupStack::Pop(city);
		frequentlyUsedZones->AppendL(timeZone);
		CleanupStack::Pop(timeZone);
		}

	WriteAllFrequentlyUsedZonesL(*frequentlyUsedZones,*cachedCities);
	CleanupStack::PopAndDestroy(cachedCities);
	CleanupStack::PopAndDestroy(frequentlyUsedZones);
	}

/**
Get the localized default time zone for the given frequently used time zone.
The default time zone for each of the possible frequently used time zones may be
specified in the TZ Server repository or in the TZ Localization resource file.
Check the possible sources in this order.

@param aFreqUsedZone The frequently used time zone for which the localized
default time zone is required.

@return The localized default time zone for the given frequently used time zone.
*/
CTzLocalizedTimeZone* CTzLocalizer::GetFrequentlyUsedDefaultZoneL(CTzLocalizedTimeZone::TTzFrequentlyUsedZone aFreqUsedZone)
	{
	// Assume that we will not find the key in the repository or that we do find
	// the key but it has no value.  If either of these scenarios is true then
	// we will use the time zone identifier recorded in the resource file for
	// the default time zone.
	TBool readFreqUsedZoneFromRscFile = ETrue;
	
	TUint32 defaultTimeZoneKey = GetFrequentlyUsedDefaultZoneCenRepKeyL(aFreqUsedZone);
	TInt defaultTimeZoneId = 0;

	CRepository* tzRepository = CRepository::NewL(NTzUpdate::KPropertyCategory);
	CleanupStack::PushL(tzRepository);
	TInt err = tzRepository->Get(defaultTimeZoneKey, defaultTimeZoneId);
	CleanupStack::PopAndDestroy(tzRepository);
	
	CTzLocalizedTimeZone* defaultTimeZone = 0;
	
	// The key may not exist in the repository so this is not an error
	// condition.
	if(err != KErrNotFound)
		{
		User::LeaveIfError(err);

		// The key exists - check that it has a value.
		if(defaultTimeZoneId != 0)
			{
			readFreqUsedZoneFromRscFile = EFalse;
			defaultTimeZone = iStaticDataReader->ReadTimeZoneL(defaultTimeZoneId);
			}
		}

	if(readFreqUsedZoneFromRscFile)
		{
		defaultTimeZone = iStaticDataReader->ReadFrequentlyUsedZoneL(aFreqUsedZone);
		}

	return defaultTimeZone;
	}

/**
Get the TZ Server's CenRep repository key for the given frequently used zone.

@param aFreqUsedZone The frequently used time zone for which the TZ Server's
CenRep repository key is required.

@return The TZ Server's CenRep repository key for the given frequently used
zone.
*/
TUint32 CTzLocalizer::GetFrequentlyUsedDefaultZoneCenRepKeyL(CTzLocalizedTimeZone::TTzFrequentlyUsedZone aFreqUsedZone)
	{
	// These key values are copied from those defined in tzconfigagent.h.  We
	// want to keep the key values private - this is one way to do it.
	const TUint32 KDefaultHomeTimeZoneKey = 0x3UL;
	const TUint32 KDefaultInterestTimeZoneKey = 0x4UL;
	const TUint32 KDefaultRecent1TimeZoneKey = 0x5UL;
	const TUint32 KDefaultRecent2TimeZoneKey = 0x6UL;

	TUint32 key = 0;

	switch(aFreqUsedZone)
	    {
	    case CTzLocalizedTimeZone::EHomeZone:
	    	{
	    	key = KDefaultHomeTimeZoneKey;
	        }
	    break;

	    case CTzLocalizedTimeZone::EInterestZone:
	    	{
	    	key = KDefaultInterestTimeZoneKey;
	        }
	    break;

	    case CTzLocalizedTimeZone::ERecentZone1:
	    	{
	    	key = KDefaultRecent1TimeZoneKey;
	        }
	    break;

	    case CTzLocalizedTimeZone::ERecentZone2:
	    	{
	    	key = KDefaultRecent2TimeZoneKey;
	        }
	    break;
	    
	    default:
	    	{
	    	User::Leave(KErrArgument);
	    	}
	    }

	return key;
    }

/**
Gets the localized time zone that matches the time zone ID specified.
The calling function takes ownership of the returned localized time zone.
@param aTimeZoneId A positive time zone ID of the localized time zone to return.
@return The localized time zone.
*/
EXPORT_C CTzLocalizedTimeZone* CTzLocalizer::GetLocalizedTimeZoneL(TInt aTimeZoneId) const
	{
	return GetLocalizedTimeZoneL(aTimeZoneId, ETrue);
	}

/**
Gets the localized time zone that aCity is a member of.
The calling function takes ownership of the returned localized time zone.
@param aCity Any city in the time zone to return.
@return The localized time zone.
*/
EXPORT_C CTzLocalizedTimeZone* CTzLocalizer::GetLocalizedTimeZoneL(const CTzLocalizedCity& aCity) const
	{
	if(IsOperateOnUserDbL(aCity.TzLocalizedId().TimeZoneId(), ETrue))
		{
		return iUserDataReader->ReadTimeZoneL(aCity.TzLocalizedId());
		}
	else
		{
		return iStaticDataReader->ReadTimeZoneL(aCity.TzLocalizedId());
		}
	}

/**
Gets an array of all available localizable time zones, sorted according to aSortOrder.
The calling function takes ownership of the returned array.
@param aSortOrder Specifies the sort order of the returned array.
@return Array of all available localizable time zones.
*/
EXPORT_C CTzLocalizedTimeZoneArray* CTzLocalizer::GetAllTimeZonesL(const TTzSortOrder aSortOrder)
	{
	CTzLocalizedTimeZoneArray* allTimeZones = CTzLocalizedTimeZoneArray::NewLC();
	if(iDataSource&ETzDataSourceSystem)
		{
		iStaticDataReader->ReadAllTimeZonesL(*allTimeZones);
		}
	//User time zones will be appended
	if (iDataSource&ETzDataSourceUser)
		{
		iUserDataReader->ReadAllTimeZonesL(*allTimeZones);
		}

	if ((aSortOrder == ETzUTCAscending) || (aSortOrder == ETzUTCDescending))
		{
		PrepareForUTCSortL(*allTimeZones);
		}

	if (aSortOrder != ETzUnsorted)
		{
		TLinearOrder<CTzLocalizedTimeZone> sortOrder(TimeZoneSortOrderL(aSortOrder));
		allTimeZones->Sort(sortOrder);
		}

	CleanupStack::Pop(allTimeZones);

	return allTimeZones;
	}

/**
Sets the time zone in the time zone server to be that of the
supplied time zone id.  The current cached zone is also updated.
@param aTimeZoneId A positive time zone ID of the new time zone.
*/
EXPORT_C void CTzLocalizer::SetTimeZoneL(TInt aTimeZoneId)
	{
	SetFrequentlyUsedZoneL(aTimeZoneId, CTzLocalizedTimeZone::ECurrentZone);
	}

/**
Gets the default city for the specified time zone.
The calling function takes ownership of the returned city.
@param aTimeZoneId A positive time zone ID of the default city to return.
@return The default city for the specified time zone.
*/
EXPORT_C CTzLocalizedCity* CTzLocalizer::GetDefaultCityL(TInt aTimeZoneId) const
	{
	return ReadDefaultCityL(aTimeZoneId, ETrue);
	}

/**
Gets the default city for the time zone the specified city is a member of.
The calling function takes ownership of the returned city.
@param aCity Any city in the required time zone.
@return The default city.
*/
EXPORT_C CTzLocalizedCity* CTzLocalizer::GetDefaultCityL(const CTzLocalizedCity& aCity) const
	{
	return ReadDefaultCityL(aCity.TzLocalizedId().TimeZoneId(), ETrue);
	}

/**
Gets the default city for the specified time zone.
The calling function takes ownership of the returned city.
@param aTimeZone A time zone.
@return The default city for the specified time zone.
*/
EXPORT_C CTzLocalizedCity* CTzLocalizer::GetDefaultCityL(const CTzLocalizedTimeZone& aTimeZone) const
	{
	return ReadDefaultCityL(aTimeZone.TzLocalizedId().TimeZoneId(), ETrue);
	}

/**
Returns all cities defined in the static and persisted data.
The calling function takes ownership of the returned array.
@param aSortOrder Specifies the order in which the returned array will be sorted.
It can have one of the following values: ETzUnsorted, ETzUTCAscending, ETzUTCDescending,
ETzAlphaNameAscending, or ETzAlphaNameDescending.
@return Pointer to the array of cities.
*/
EXPORT_C CTzLocalizedCityArray* CTzLocalizer::GetCitiesL(const TTzSortOrder aSortOrder)
	{
	//Create an empty city array and retrieve all cities
	CTzLocalizedCityArray* cities = CTzLocalizedCityArray::NewLC();
	if(iDataSource&ETzDataSourceSystem)
		{
		iStaticDataReader->ReadCitiesL(*cities);
		}
	if(iDataSource&ETzDataSourceUser)
		{//Cities stored in the user tz database will be appended.
		iUserDataReader->ReadCitiesL(*cities);
		}
	RPointerArray<CTzLocalizedCityRecord> cityRecords;
	CleanupStack::PushL(TCleanupItem(CleanupCityPointerArray, &cityRecords));
	iTzSession.LocalizationReadCitiesL(cityRecords);
	PopulateCityArrayL(cityRecords, *cities);
	CleanupStack::PopAndDestroy();	// cityRecords

	if ((aSortOrder == ETzUTCAscending) || (aSortOrder == ETzUTCDescending))
		{
		//get utc offset for each city if required
		PrepareForUTCSortL(*cities);
		}

	if (aSortOrder != ETzUnsorted)
		{
		TLinearOrder<CTzLocalizedCity> sortOrder(CitySortOrderL(aSortOrder));
		cities->Sort(sortOrder);
		}

	CleanupStack::Pop(cities);
	return cities;
	}

/**
Gets all the cities defined in the static and persisted data that belong
to the time zone identified by aTimeZoneId.
The calling function takes ownership of the returned array.
@param aTimeZoneId A positive time zone ID that identifies the time zone of
the cities to return.
@param aSortOrder Defines the order in which the returned array will be sorted.
It can have one of the following values: ETzUnsorted, ETzAlphaNameAscending,
ETzAlphaNameDescending.
@leave KErrArgument An invalid sort order is specified.
@return A pointer to the array of cities.
*/
EXPORT_C CTzLocalizedCityArray* CTzLocalizer::GetCitiesL(TInt aTimeZoneId, const TTzSortOrder aSortOrder)
	{
	CTzLocalizedCityArray* cities = CTzLocalizedCityArray::NewLC();
	ReadCitiesL(*cities, aTimeZoneId, ETrue);

	RPointerArray<CTzLocalizedCityRecord> cityRecords;
	CleanupStack::PushL(TCleanupItem(CleanupCityPointerArray, &cityRecords));
	iTzSession.LocalizationReadCitiesL(cityRecords, aTimeZoneId);
	PopulateCityArrayL(cityRecords, *cities);
	CleanupStack::PopAndDestroy();	// cityRecords

	//Only sort alphabetically if requested.
	//We have all cities in a time zone, so there is no need to sort by UTC offset
	if ((aSortOrder == ETzAlphaNameAscending) || (aSortOrder == ETzAlphaNameDescending))
		{
		TLinearOrder<CTzLocalizedCity> sortOrder(CitySortOrderL(aSortOrder));
		cities->Sort(sortOrder);
		}
	else if(aSortOrder != ETzUnsorted)
		{
		User::Leave(KErrArgument);
		}

	CleanupStack::Pop(cities);

	return cities;
	}

/**
Gets all the cities defined in the static and persisted data that belong to
the same time zone as aCity.
The calling function takes ownership of the returned array.
@param aCity Any city in the desired time zone.
@param aSortOrder Specifies the order in which the returned array will be sorted.
@return A pointer to the array of cities.
*/
EXPORT_C CTzLocalizedCityArray* CTzLocalizer::GetCitiesL(const CTzLocalizedCity& aCity,const TTzSortOrder aSortOrder)
	{
	CTzLocalizedCityArray* cities = CTzLocalizedCityArray::NewLC();
	GetCitiesL(*cities,aCity.TzLocalizedId(),ETrue,aSortOrder);
	CleanupStack::Pop(cities);
	return cities;
	}

/**
Gets all the cities defined in the static and persisted data that belong to
aTimeZone.
The calling function takes ownership of the array.
@param aTimeZone The required time zone
@param aSortOrder Specifies the order in which the returned array will be sorted
@return A pointer to the array of cities.
*/
EXPORT_C CTzLocalizedCityArray* CTzLocalizer::GetCitiesL(const CTzLocalizedTimeZone& aTimeZone, const TTzSortOrder aSortOrder)
	{
	CTzLocalizedCityArray* cities = CTzLocalizedCityArray::NewLC();
	GetCitiesL(*cities,aTimeZone.TzLocalizedId(),ETrue,aSortOrder);
	CleanupStack::Pop(cities);
	return cities;
	}

/**
Adds to the specified city array all cities defined in the static and persisted
data that are members of the time zone referenced by aLocalizedId.
@param The array of cities to add the cities to.
@param aLocalizedId A TzLocalizedId identifying a time zone
@param aSortOrder Specifies the order in which the returned array will be
sorted. CTzLocalizedCityArray can take the following sort orders:
ETzUnsorted, ETzAlphaNameAscending,	ETzAlphaNameDescending.
@leave KErrArgument An invalid sort order is asked for.
*/
void CTzLocalizer::GetCitiesL(CTzLocalizedCityArray& aCities, const TTzLocalizedId& aLocalizedId,
		TBool aUseDataSource, const TTzSortOrder aSortOrder)
	{
	ReadCitiesL(aCities, aLocalizedId.TimeZoneId(), aUseDataSource);

	RPointerArray<CTzLocalizedCityRecord> cityRecords;
	CleanupStack::PushL(TCleanupItem(CleanupCityPointerArray, &cityRecords));
	iTzSession.LocalizationReadCitiesL(cityRecords, aLocalizedId.TimeZoneId());
	PopulateCityArrayL(cityRecords, aCities);
	CleanupStack::PopAndDestroy();	// cityRecords

	//Only sort alphabetically if requested.
	//We have all cities in a time zone, so there is no need to sort by UTC offset
	if ((aSortOrder == ETzAlphaNameAscending) || (aSortOrder == ETzAlphaNameDescending))
		{
		TLinearOrder<CTzLocalizedCity> sortOrder(CitySortOrderL(aSortOrder));
		aCities.Sort(sortOrder);
		}
	else if(aSortOrder != ETzUnsorted)
		{
		User::Leave(KErrArgument);
		}
	}

/**
Creates a user defined city, specifying the time zone and
optionally the group to which it belongs.
@param aTimeZoneId A positive time zone ID of the city to add.
@param aCityName The name of the city to add.
@param aGroupId The ID of the city group to add the city to. Zero if the city doesn't
belong to a group. Currently supports up to 255 groups
@return The newly created city. The calling function takes ownership.
@leave KErrAlreadyExists The specified city already exists in the time zone.
@leave KErrNotFound aTimeZoneId or aGroupId is not found.
@leave KErrArgument aGroupId is invalid
*/
EXPORT_C CTzLocalizedCity* CTzLocalizer::AddCityL(TInt aTimeZoneId,const TDesC& aCityName, TInt aGroupId)
	{
	//Check that aGroupId is a valid TUint8 and leave if not.
	//Group number > 255 is not supported
	__ASSERT_ALWAYS(aGroupId >= 0 && aGroupId <256 , User::Leave(KErrArgument));

	// Check that the group exists - this code will leave with
	// KErrNotFound if the group does not exist.
	// Do not need to check when the gtoup ID is zero as this specifies a groupless city
	if(aGroupId != 0)
		{
		CTzLocalizedCityGroup* group = GetCityGroupL(aGroupId);
		delete group;
		}

	//Check that the city doesn't exist in this time zone
	CTzLocalizedCityArray* staticCityArray = CTzLocalizedCityArray::NewLC();
	ReadCitiesL(*staticCityArray, aTimeZoneId, EFalse);

	TInt count = staticCityArray->Count();
	for (TInt x = 0; x < count; ++x)
		{
		CTzLocalizedCity& city = staticCityArray->At(x);
		if (aCityName == city.Name())
			{
			User::Leave(KErrAlreadyExists);
			}
		}
	// Copy the TzLocalizedID (timezoneID/resourceId pair) for these cities to
	// put in the new city
	TTzLocalizedId localizedId(staticCityArray->At(0).TzLocalizedId());
	CleanupStack::PopAndDestroy(staticCityArray);

	//City doesn't exist - we can try and add it
	CTzLocalizedCity* newCity = CTzLocalizedCity::NewLC(aCityName,localizedId,(TUint8)aGroupId);

	//WriteCityL checks to see if a city with the same Time Zone ID
	//is already in the persisted cities, while adding the city.
	iTzSession.LocalizationWriteCityL(newCity->Name(), (TInt)newCity->TimeZoneId(), newCity->GroupId(),
			newCity->TzLocalizedId().ResourceId());
	CleanupStack::Pop(newCity);
	return newCity;
	}

/**
Removes a city from the user defined city collection.
The calling function loses ownership of aCity.
@param aCity The city to remove.
*/
EXPORT_C void CTzLocalizer::RemoveCityL(CTzLocalizedCity* aCity)
	{
	if(aCity)
		{
		CleanupStack::PushL(aCity);
		iTzSession.LocalizationDeleteCityL(aCity->Name(), aCity->TimeZoneId());
		CleanupStack::PopAndDestroy(aCity);
		}
	else
		{
		User::Leave(KErrArgument);
		}
	}

/**
Gets a list of all the existing city groups.
The calling function takes ownership of the returned array.
@param aSortOrder Specifies the order in which the returned array will be
sorted. Possible values are ETzAlphaNameAscending or ETzAlphaNameDescending.
@return Array of all existing city groups.
*/
EXPORT_C CTzLocalizedCityGroupArray* CTzLocalizer::GetAllCityGroupsL(const TTzSortOrder aSortOrder) const
	{
	CTzLocalizedCityGroupArray* cityGroups = CTzLocalizedCityGroupArray::NewLC();
	if(iDataSource&ETzDataSourceSystem)
		{
		iStaticDataReader->ReadAllGroupsL(*cityGroups);
		}
	if(iDataSource&ETzDataSourceUser)
		{
		iUserDataReader->ReadAllGroupsL(*cityGroups);
		}

	if (aSortOrder == ETzAlphaNameAscending)
		{
		TLinearOrder<CTzLocalizedCityGroup> order(NTzLocalSortAndFind::SortCityGroupNameAscending);
		cityGroups->Sort(order);
		}
	else if (aSortOrder == ETzAlphaNameDescending)
		{
		TLinearOrder<CTzLocalizedCityGroup> order(NTzLocalSortAndFind::SortCityGroupNameDescending);
		cityGroups->Sort(order);
		}
	CleanupStack::Pop(cityGroups);
	return cityGroups;
	}

/**
Gets the city group with the specified ID.
The calling function takes ownership of the returned CTzLocalizedCityGroup.
@param aGroupId The ID of the city group to get.
@return The matching city group.
@leave KErrArgument aGroupId is invalid
*/
EXPORT_C CTzLocalizedCityGroup* CTzLocalizer::GetCityGroupL(TInt aGroupId) const
	{
	//Check that aGroupId is a valid TUint8 and leave if not.
	//Group number > 255 is not supported
	__ASSERT_ALWAYS(aGroupId >= 0 && aGroupId <256 , User::Leave(KErrArgument));
	return iStaticDataReader->ReadGroupL((TUint8)aGroupId);
	}

/**
Gets the city group to which the specified city belongs.
The calling function takes ownership of the returned CTzLocalizedCityGroup.
@param aCity Any city in the required city group.
@return The city group.
*/
EXPORT_C CTzLocalizedCityGroup* CTzLocalizer::GetCityGroupL(const CTzLocalizedCity& aCity) const
	{
	return iStaticDataReader->ReadGroupL(aCity.GroupId());
	}

/**
Returns all cities defined in the static and persisted data that are members
of the group with the specified group ID.
The calling function takes ownership of the array.
@param aGroupId The group ID.
@param aSortOrder Specifies the order in which the returned array will be sorted.
The following sort orders are supported: ETzUnsorted, ETzUTCAscending, ETzUTCDescending,
ETzAlphaNameAscending, ETzAlphaNameDescending.
@return Pointer to the array of cities.
@leave KErrArgument aGroupId is invalid
*/
EXPORT_C CTzLocalizedCityArray* CTzLocalizer::GetCitiesInGroupL(TInt aGroupId,const TTzSortOrder aSortOrder)
	{
	//Check that aGroupId is a valid TUint8 and leave if not.
	//Group number > 255 is not supported
	__ASSERT_ALWAYS(aGroupId >= 0 && aGroupId <256 , User::Leave(KErrArgument));

	CTzLocalizedCityArray* cities = CTzLocalizedCityArray::NewLC();

	if(iDataSource&ETzDataSourceSystem)
		{
		iStaticDataReader->ReadCitiesInGroupL(*cities,(TUint8)aGroupId);
		}
	if(aGroupId == KTzCityNotInGroupId && iDataSource&ETzDataSourceUser)
		{//All user time zone are groupless
		iUserDataReader->ReadCitiesInGroupL(*cities, KTzCityNotInGroupId);
		}

	RPointerArray<CTzLocalizedCityRecord> cityRecords;
	CleanupStack::PushL(TCleanupItem(CleanupCityPointerArray, &cityRecords));
	iTzSession.LocalizationReadCitiesInGroupL(cityRecords, (TUint8)aGroupId);
	PopulateCityArrayL(cityRecords, *cities);
	CleanupStack::PopAndDestroy();	// cityRecords

	if ((aSortOrder == ETzUTCAscending) || (aSortOrder == ETzUTCDescending))
		{
		PrepareForUTCSortL(*cities);
		}

	if (aSortOrder != ETzUnsorted)
		{
		TLinearOrder<CTzLocalizedCity> sortOrder(CitySortOrderL(aSortOrder));
		cities->Sort(sortOrder);
		}
	CleanupStack::Pop(cities);
	return cities;
	}

/**
Returns all cities defined in the static and persisted data that are members
of the same group as the specified city.
The calling function takes ownership of the array.
@param aCity Any city in the desired city group.
@param aSortOrder Specifies the order in which the returned array will be sorted.
The following sort orders are supported: ETzUnsorted, ETzUTCAscending, ETzUTCDescending,
ETzAlphaNameAscending, ETzAlphaNameDescending.
@return Pointer to an array of cities.
*/
EXPORT_C CTzLocalizedCityArray* CTzLocalizer::GetCitiesInGroupL(const CTzLocalizedCity& aCity,const TTzSortOrder aSortOrder)
	{
	return GetCitiesInGroupL(aCity.GroupId(),aSortOrder);
	}

/**
Returns all cities defined in the static and persisted data that are members
of the specified city group.
The calling function takes ownership of the array.
@param aCityGroup The city group of interest.
@param aSortOrder Specifies the order in which the returned array will be sorted.
The following sort orders are supported: ETzUnsorted, ETzUTCAscending,
ETzUTCDescending, ETzAlphaNameAscending, ETzAlphaNameDescending.
@return Pointer to an array of cities.
*/
EXPORT_C CTzLocalizedCityArray* CTzLocalizer::GetCitiesInGroupL(const CTzLocalizedCityGroup& aCityGroup,const TTzSortOrder aSortOrder)
	{
	return GetCitiesInGroupL(aCityGroup.Id(),aSortOrder);
	}

/**
Gets the cached time zone identified by aFrequentlyUsedZone.
The calling function takes ownership of the returned zone.
When retrieving the current zone (CTzLocalizedTimeZone::ECurrentZone), the time zone
server is queried to make sure that the system's current time zone has not been updated
since the current zone was cached.
If the system's time zone has been updated, the new localised zone is stored in the database.
@param aFrequentlyUsedZone Identifies the cached zone to return.
@return The cached zone.
*/
EXPORT_C CTzLocalizedTimeZone* CTzLocalizer::GetFrequentlyUsedZoneL(const CTzLocalizedTimeZone::TTzFrequentlyUsedZone aFrequentlyUsedZone)
	{
	CTzLocalizedTimeZone* timeZone;

	if (aFrequentlyUsedZone == CTzLocalizedTimeZone::ECurrentZone)
		{
		//Get cached current zone
		CTzLocalizedTimeZoneRecord* frequentlyUsedZoneRecord = iTzSession.LocalizationReadFrequentlyUsedZoneL(aFrequentlyUsedZone);
		CleanupStack::PushL(frequentlyUsedZoneRecord);
		CTzLocalizedTimeZone* frequentlyUsedZone = CreateTimeZoneL(*frequentlyUsedZoneRecord);
		CleanupStack::PopAndDestroy(frequentlyUsedZoneRecord);
		CleanupStack::PushL(frequentlyUsedZone);

		TInt currentTimeZoneId = GetTimeZoneIdFromTzServerL();
		if (frequentlyUsedZone->TimeZoneId() != currentTimeZoneId)
			{
			//Current timezone has changed.  Set the current zone in the cache and return it
			CTzLocalizedTimeZone* currentZone = GetLocalizedTimeZoneL(currentTimeZoneId, EFalse);
			CleanupStack::PushL(currentZone);
			timeZone = currentZone;

			CTzLocalizedCity* city = ReadDefaultCityL(timeZone->TimeZoneId(), EFalse);
			CleanupStack::PushL(city);
			SetFrequentlyUsedZoneL(*timeZone, *city, CTzLocalizedTimeZone::ECurrentZone);
	        CleanupStack::PopAndDestroy(city);

			CleanupStack::Pop(currentZone);
			CleanupStack::PopAndDestroy(frequentlyUsedZone);
			frequentlyUsedZone = NULL;
			}
		else
			{
			//Return the cached zone.
			timeZone = frequentlyUsedZone;
			CleanupStack::Pop(frequentlyUsedZone);
			frequentlyUsedZone = NULL;
			}
		}

	else
		{
		CTzLocalizedTimeZoneRecord* timeZoneRecord = iTzSession.LocalizationReadFrequentlyUsedZoneL(aFrequentlyUsedZone);
		CleanupStack::PushL(timeZoneRecord);
		timeZone = CreateTimeZoneL(*timeZoneRecord);
		CleanupStack::PopAndDestroy(timeZoneRecord);
		}

	return timeZone;
	}

/**
Retrieves the integer time zone ID of the current time zone from the time zone
server (app-services/Tz).
@return The integer time zone ID according to the time zone server.
*/
TInt CTzLocalizer::GetTimeZoneIdFromTzServerL()
	{
	// Get current time zone using the current CTzId from the time zone server
	CTzId* currentCTzId = iTzSession.GetTimeZoneIdL();
	CleanupStack::PushL(currentCTzId);

	TUint timeZoneIdInt = currentCTzId->TimeZoneNumericID();

	CleanupStack::PopAndDestroy(currentCTzId);

	// Is the time zone ID the unknown/invalid ID?
	// This is temporary measure that is required until PREQ 234' TzServer
	// changes are introduced
	const TUint32 KUnknownTZId = 0x0ffffff0;
	if((TUint32)timeZoneIdInt == KUnknownTZId)
		{
		// Return the ID of the DEFAULT home zone instead
		CTzLocalizedTimeZone* homeZone = iStaticDataReader->ReadFrequentlyUsedZoneL(CTzLocalizedTimeZone::EHomeZone);
		CleanupStack::PushL(homeZone);
		timeZoneIdInt = homeZone->TimeZoneId();
		CleanupStack::PopAndDestroy(homeZone);
		}

	return timeZoneIdInt;
	}

/**
Gets the city that was used to set the specified cached time zone.
The calling function takes ownership of the returned city.
If a city was not used to set the cached zone, the default
city for the zone is returned.
@param aFrequentlyUsedZone Indicates one of the five cached zones.
@return The city used to set the cached zone.
*/
EXPORT_C CTzLocalizedCity* CTzLocalizer::GetFrequentlyUsedZoneCityL(const CTzLocalizedTimeZone::TTzFrequentlyUsedZone aFrequentlyUsedZone)
	{
	CTzLocalizedCityRecord* cityRecord = iTzSession.LocalizationReadCachedTimeZoneCityL(aFrequentlyUsedZone);
	CleanupStack::PushL(cityRecord);
	CTzLocalizedCity* city = CreateCityL(*cityRecord);
	CleanupStack::PopAndDestroy(cityRecord);

	// check that the city contains valid information
	if(city->TimeZoneId() == 0)
	// if not, get of the default cached zone's default city from the static reader
		{
		delete city;
		city = NULL;

		if(aFrequentlyUsedZone != CTzLocalizedTimeZone::ECurrentZone)
			{
			city = iStaticDataReader->ReadCachedTimeZoneCityL(aFrequentlyUsedZone);
			}
		else
			{
			// use GetFrequentlyUsedZoneL(), because it deals with the current zone correctly
			CTzLocalizedTimeZone* defaultFrequentlyUsedZone = GetFrequentlyUsedZoneL(aFrequentlyUsedZone);
			CleanupStack::PushL(defaultFrequentlyUsedZone);
			// then get the city
			city = ReadDefaultCityL(defaultFrequentlyUsedZone->TimeZoneId(), EFalse);
			CleanupStack::PopAndDestroy(defaultFrequentlyUsedZone);
			}
		}
	else
		{
		if(aFrequentlyUsedZone == CTzLocalizedTimeZone::ECurrentZone)
			{
			CleanupStack::PushL(city);
			CTzLocalizedTimeZone* timeZone = GetLocalizedTimeZoneL(*city, EFalse);
			TUint16 tzId = timeZone->TimeZoneId();
			delete timeZone;
			CTzLocalizedTimeZone* currentTimeZone = GetFrequentlyUsedZoneL(CTzLocalizedTimeZone::ECurrentZone);
			TUint16 currentTzId = currentTimeZone->TimeZoneId();
			delete currentTimeZone;
			if (tzId != currentTzId)
				{
				CleanupStack::PopAndDestroy(city);
				city = NULL;
				city = ReadDefaultCityL(currentTzId, EFalse);
				}
			else
				{
				CleanupStack::Pop(city);
				}
			}

		}

	return city;
	}

/**
Sets the ID of the specified cached time zone.
@param aTimeZoneId A positive time zone ID to assign to aFrequentlyUsedZone. Must not
be zero or the function leaves with KErrArgument.
@param aFrequentlyUsedZone Identifies the cached zone whose ID will be set. This must be
either CTzLocalizedTimeZone::ECurrentZone, CTzLocalizedTimeZone::EHomeZone or
CTzLocalizedTimeZone::EInterestZone, or the function leaves with KErrArgument.
N.B. When the current zone is set, the time zone of the device is altered.
*/
EXPORT_C void CTzLocalizer::SetFrequentlyUsedZoneL(TInt aTimeZoneId,const CTzLocalizedTimeZone::TTzFrequentlyUsedZone aFrequentlyUsedZone)
	{
	// The time zone ID must be a non-zero number to be valid
	__ASSERT_ALWAYS(aTimeZoneId > 0, User::Leave(KErrArgument));
	CTzLocalizedTimeZone* aTimeZone = GetLocalizedTimeZoneL(aTimeZoneId, EFalse);
	CleanupStack::PushL(aTimeZone);
	SetFrequentlyUsedZoneL(*aTimeZone,aFrequentlyUsedZone);
	CleanupStack::PopAndDestroy(aTimeZone);
	}

/**
Sets the value of the specified cached time zone.
@param aTimeZone The value to assign to aFrequentlyUsedZone. It must have an ID greater
than zero or the function leaves with KErrArgument.
@param aFrequentlyUsedZone Identifies the cached zone whose ID will be set. This must be
either CTzLocalizedTimeZone::ECurrentZone, CTzLocalizedTimeZone::EHomeZone or
CTzLocalizedTimeZone::EInterestZone, or the function leaves with KErrArgument.
N.B. When the current zone is set, the time zone of the device is altered.
*/
EXPORT_C void CTzLocalizer::SetFrequentlyUsedZoneL(const CTzLocalizedTimeZone& aTimeZone, const CTzLocalizedTimeZone::TTzFrequentlyUsedZone aFrequentlyUsedZone)
	{
	// The time zone ID must be a non-zero number to be valid
	__ASSERT_ALWAYS(aTimeZone.TimeZoneId() > 0, User::Leave(KErrArgument));
	CTzLocalizedCity* city = ReadDefaultCityL(aTimeZone.TimeZoneId(), EFalse);
	CleanupStack::PushL(city);
	// Update the time zone server, if the current zone is being updated
	if(aFrequentlyUsedZone == CTzLocalizedTimeZone::ECurrentZone)
		{
		DoSetTimeZoneL(aTimeZone.TimeZoneId());
		}
	else if(aFrequentlyUsedZone == CTzLocalizedTimeZone::EHomeZone)
		{
		ChangeHomeTimeZoneL(aTimeZone.TimeZoneId());
		}
	SetFrequentlyUsedZoneL(aTimeZone, *city, aFrequentlyUsedZone);
    CleanupStack::PopAndDestroy(city);
	}

/**
Sets the value of the specified cached zone to be the zone which aCity
is located in.
Setting a cached zone by city also stores the city used to select the time zone.
@param aCity Any city in the time zone to set. It must have a time zone ID greater
than zero or the function leaves with KErrArgument.
@param aFrequentlyUsedZone Indicates which of the cached zones to set. This must be
either CTzLocalizedTimeZone::ECurrentZone, CTzLocalizedTimeZone::EHomeZone or
CTzLocalizedTimeZone::EInterestZone, or the function leaves with KErrArgument.
N.B. When the current zone is set, the time zone of the device is altered.
*/
EXPORT_C void CTzLocalizer::SetFrequentlyUsedZoneL(const CTzLocalizedCity& aCity,const CTzLocalizedTimeZone::TTzFrequentlyUsedZone aFrequentlyUsedZone)
	{
	// The time zone ID must be a non-zero number to be valid
	__ASSERT_ALWAYS(aCity.TimeZoneId() > 0, User::Leave(KErrArgument));
	CTzLocalizedTimeZone* timeZone = GetLocalizedTimeZoneL(aCity, EFalse);
	CleanupStack::PushL(timeZone);
	// Update the time zone server, if the current zone is being updated
	if(aFrequentlyUsedZone == CTzLocalizedTimeZone::ECurrentZone)
		{
		DoSetTimeZoneL(aCity.TimeZoneId());
		}
	else if(aFrequentlyUsedZone == CTzLocalizedTimeZone::EHomeZone)
		{
		ChangeHomeTimeZoneL(timeZone->TimeZoneId());
		}
	SetFrequentlyUsedZoneL(*timeZone, aCity, aFrequentlyUsedZone);
	CleanupStack::PopAndDestroy(timeZone);
	}

void CTzLocalizer::ChangeHomeTimeZoneL(TInt aNewId)
	{
	NTzUpdate::TTimeZoneChange change;
	change.iNewTimeZoneId = aNewId;
	CTzLocalizedTimeZoneRecord* oldTz = iTzSession.LocalizationReadFrequentlyUsedZoneL(CTzLocalizedTimeZone::EHomeZone);
	change.iOldTimeZoneId = oldTz->Id();
	delete oldTz; // don't need to put on cleanup stack because previous line can't leave

	RTz tz;
	User::LeaveIfError(tz.Connect());
	CleanupClosePushL(tz);
	tz.NotifyHomeTimeZoneChangedL(change);
	CleanupStack::PopAndDestroy(&tz);
	}

/**
Set the time zone in the time zone server to be that of the
supplied time zone id
@param aTimeZoneId The time zone id of the new time zone.
@internalTechnology
*/
void CTzLocalizer::DoSetTimeZoneL(TInt aTimeZoneId)
	{
	// Create a new CTzId with the new time zone ID in it
	CTzId* newCTzId = CTzId::NewL(aTimeZoneId);
	CleanupStack::PushL(newCTzId);

	//Connect to time zone server
	RTz tz;
	User::LeaveIfError(tz.Connect());
	CleanupClosePushL(tz);

	tz.SetTimeZoneL(*newCTzId);

	CleanupStack::PopAndDestroy(); //tz
	CleanupStack::PopAndDestroy(newCTzId);
	}

/**
Prepares a CTzLocalizedTimeZoneArray or CTzLocalizedCityArray for sorting by UTC offset.
All array elements which have not already had the UTC offset set will retrieve the
correct offset from App-Services/Tz.
@param aArray
@internalTechnology
*/
template <class T>
void CTzLocalizer::PrepareForUTCSortL(T& aArray)
	{
	//Prepare an array of timezone IDs to give to the time zone server
	RArray<TInt> timeZoneIds;
	CleanupClosePushL(timeZoneIds);
	TInt i;
	TInt count = aArray.Count();
	for(i=0; i<count; ++i)
		{
		timeZoneIds.AppendL(aArray.At(i).TimeZoneId());
		}

	//Prepare an array of UTC Offsets for the timezone server to fill
	RArray<TInt> utcOffsets;
	CleanupClosePushL(utcOffsets);

	//Connect to Time Zone Server
	RTz tz;
	User::LeaveIfError(tz.Connect());
	CleanupClosePushL(tz);

	// Pass the arrays to tz - the time zone server will populate the array
	// of UTC Offsets based using the array of Time Zone IDs
	tz.GetOffsetsForTimeZoneIdsL(timeZoneIds, utcOffsets);

	//Finished with Time Zone Server
	CleanupStack::PopAndDestroy(); //tz

	// copy the utc offsets from one array to the other
	for(i=0; i<count; ++i)
		{
		aArray.At(i).SetUTCOffset(utcOffsets[i]);
		}

	//Finished with arrays, so close them
	CleanupStack::PopAndDestroy(2);	// utcOffsets, timeZoneIds
	}

/**
Searches for a city with the specified name. If a time Zone ID is also specified, then
the search only covers cities in that time zone. Otherwise, all time zones are searched.
The first matching city is returned.
@param aCityName Name of the city to search for.
@param aTimeZoneId (Optional) A positive time zone ID of the city to search for.
Specify zero to omit.
@return If successful, a pointer to a CTzLocalizedCity. The calling function
takes ownership of the returned pointer. If the search string is not found,
NULL is returned.
@leave KErrNotFound The specified time zone ID does not exist.
*/
EXPORT_C CTzLocalizedCity* CTzLocalizer::FindCityByNameL(const TDesC& aCityName, const TInt aTimeZoneId)
	{
	CTzLocalizedCity* foundCity;
	//Create a dummy city for the search
	CTzLocalizedCity* aCity = CTzLocalizedCity::NewLC(aCityName,TTzLocalizedId((TUint16)aTimeZoneId,0),0);

	CTzLocalizedCityArray* citiesToSearch = NULL;

	if(aTimeZoneId > 0)
	// A non-zero Time Zone ID has been supplied, so search through the cities within that Time Zone.
		{
		citiesToSearch = GetCitiesL(aTimeZoneId);
		}
	else
	// Search through all cities
		{
		citiesToSearch= GetCitiesL();
		}

	TIdentityRelation<CTzLocalizedCity> identity(NTzLocalSortAndFind::FindCityByName);

	TInt index = citiesToSearch->Find(aCity,identity);

	if (index != KErrNotFound)
		{
		foundCity = &(citiesToSearch->At(index));
		// ownership is transferred to calling function
		// we don't want this pointer deleted when the array is
		citiesToSearch->Remove(index);
		}
	else
		{
		foundCity = NULL;
		}

	CleanupStack::PopAndDestroy(aCity);

	delete citiesToSearch;
	citiesToSearch = NULL;

	return foundCity;
	}
/**
Searches for a time zone with the specified name.
All text fields of the time zone are searched.
@param aTimeZoneName the name of the time zone to search for.
@return If successful, a pointer to a CTzLocalizedTimeZone.
The calling function takes ownership of the returned pointer.
If the search string is not found, NULL is returned.
*/
EXPORT_C CTzLocalizedTimeZone* CTzLocalizer::FindTimeZoneByNameL(const TDesC& aTimeZoneName)
	{
	CTzLocalizedTimeZone* foundTimeZone;
	//Create a dummy time zone for the search
	TTzLocalizedId id(0, 0);
	CTzLocalizedTimeZone* timeZone = CTzLocalizedTimeZone::NewLC(id,aTimeZoneName,aTimeZoneName,aTimeZoneName,aTimeZoneName);
	//Get all the time zones
	CTzLocalizedTimeZoneArray* allTimeZones = GetAllTimeZonesL();

	TIdentityRelation<CTzLocalizedTimeZone> identity(NTzLocalSortAndFind::FindTimeZoneByName);

	TInt index = allTimeZones->Find(timeZone,identity);

	if (index != KErrNotFound)
		{
		foundTimeZone = &(allTimeZones->At(index));
		allTimeZones->Remove(index);
		}
	else
		{
		foundTimeZone = NULL;
		}

	CleanupStack::PopAndDestroy(timeZone);

	delete allTimeZones;
	allTimeZones = NULL;

	return foundTimeZone;
	}

/**
Searches for a city group with the specified name.
@param aCityGroupName The name of the city group to search for.
@return If successful, a pointer to a CTzLocalizedCityGroup.
The calling function takes ownership of the returned pointer.
If the search string is not found, NULL is returned.
*/
EXPORT_C CTzLocalizedCityGroup* CTzLocalizer::FindCityGroupByNameL(const TDesC& aCityGroupName)
	{
	CTzLocalizedCityGroup* foundCityGroup;
	//Create a dummy city group for the search
	CTzLocalizedCityGroup* aCityGroup = CTzLocalizedCityGroup::NewLC(aCityGroupName,0);
	//Get all the city groups
	CTzLocalizedCityGroupArray* allCityGroups = GetAllCityGroupsL();

	TIdentityRelation<CTzLocalizedCityGroup> identity(NTzLocalSortAndFind::FindCityGroupByName);

	TInt index = allCityGroups->Find(aCityGroup,identity);

	if (index != KErrNotFound)
		{
		foundCityGroup = &(allCityGroups->At(index));
		allCityGroups->Remove(index);
		}
	else
		{
		foundCityGroup = NULL;
		}

	CleanupStack::PopAndDestroy(aCityGroup);

	delete allCityGroups;
	allCityGroups = NULL;

	return foundCityGroup;
	}

/**
Tests whether the language of the loaded static data is still correct. If it
is not, the static data is reloaded.
Method could leave while creating a data reader.
@return ETrue if the system language (as returned by User::Language()) has changed,
EFalse if it has not.
@see User::Language()
*/
EXPORT_C TBool CTzLocalizer::CheckLanguage()
	{
	TBool languageChange = (iLanguage != User::Language());

	if(languageChange)
	// the language has changed - reload the static data and repopulate the
	// cached zones database
		{
		iLanguage = User::Language();
		delete iStaticDataReader;
		iStaticDataReader = 0;
		iStaticDataReader = CTzLocalizationResourceReader::NewL(); // Method could leave while creating a data reader
		UpdateFrequentlyUsedZonesL();
		}

	return languageChange;
	}

/**
Returns all cities defined in the static and persisted data that have
the specified UTC offset.
The calling function takes ownership of the array.
@param aUTCOffsetInMinutes The offset in minutes.
@param aSortOrder Specifies the order in which the returned array will be sorted.
Only ETzUnsorted, ETzAlphaNameAscending and ETzAlphaNameDescending will have any effect,
as all elements in the returned array will have the same UTC offset.
@return Pointer to the array of cities.
*/
EXPORT_C CTzLocalizedCityArray* CTzLocalizer::GetCitiesWithUTCOffsetL(TInt aUTCOffsetInMinutes, const TTzSortOrder aSortOrder)
	{
	//Fetch all the cities sorted by UTC offset
	CTzLocalizedCityArray* allCities = GetCitiesL(ETzUTCAscending);
	TInt count = allCities->Count();

	CTzLocalizedCity* city;
	for (TInt x = count-1; x >= 0; --x)
		{
		city = &(allCities->At(x));
		if (city->UTCOffset() != aUTCOffsetInMinutes)
			{
			allCities->Remove(x);
			delete city;
			}
		}

	//Only sort the city array alphabetically if requested
	if ((aSortOrder == ETzAlphaNameAscending) | (aSortOrder == ETzAlphaNameDescending))
		{
		TLinearOrder<CTzLocalizedCity> sortOrder(CitySortOrderL(aSortOrder));
		allCities->Sort(sortOrder);
		}

	return allCities;
	}
/**
Returns all the time zones defined in the static data that have
the specified UTC offset.
The calling function takes ownership of the array.
@param aUTCOffsetInMinutes The UTC offset in minutes.
@param aSortOrder Specifies the order in which the returned array will be sorted.
Sort orders ETzUTCAscending and ETzUTCDescending will not have any effect
as all elements in the returned array will have the same UTC offset.
@return Pointer to the array of time zones.
*/
EXPORT_C CTzLocalizedTimeZoneArray* CTzLocalizer::GetTimeZonesWithUTCOffsetL(TInt aUTCOffsetInMinutes, const TTzSortOrder aSortOrder)
	{
	//Fetch all the time zones sorted by UTC offset
	CTzLocalizedTimeZoneArray* allTimeZones = GetAllTimeZonesL(ETzUTCAscending);
	TInt count = allTimeZones->Count();

	CTzLocalizedTimeZone* timeZone;
	for (TInt x = count-1; x >= 0; --x)
		{
		timeZone = &(allTimeZones->At(x));
		if (timeZone->UTCOffset() != aUTCOffsetInMinutes)
			{
			allTimeZones->Remove(x);
			delete timeZone;
			}
		}

	//Only sort the time zone array alphabetically if requested
	if ((aSortOrder != ETzAlphaNameAscending) && (aSortOrder > ETzUTCDescending))
		{
		TLinearOrder<CTzLocalizedTimeZone> sortOrder(TimeZoneSortOrderL(aSortOrder));
		allTimeZones->Sort(sortOrder);
		}
	return allTimeZones;
	}
/**
Gets the correct TLinearOrder to sort by.
@param aSortOrder
@return the TLinearOrder to sort by
@internalTechnology
*/
TLinearOrder<CTzLocalizedCity> CTzLocalizer::CitySortOrderL(const TTzSortOrder aSortOrder)
	{

	TLinearOrder<CTzLocalizedCity> sortOrder(NTzLocalSortAndFind::SortCityUTCAscending);

	switch (aSortOrder)
		{
		case ETzUTCAscending:
			{
			//Sort order already set
			}
		break;

		case ETzUTCDescending:
			{
			sortOrder = TLinearOrder<CTzLocalizedCity>(NTzLocalSortAndFind::SortCityUTCDescending);
			}
		break;

		case ETzAlphaNameAscending:
			{
			sortOrder = TLinearOrder<CTzLocalizedCity>(NTzLocalSortAndFind::SortCityNameAscending);
			}
		break;

		case ETzAlphaNameDescending:
			{
			sortOrder = TLinearOrder<CTzLocalizedCity>(NTzLocalSortAndFind::SortCityNameDescending);
			}
		break;

		case ETzAlphaStandardNameAscending:		//FALL THROUGH - these aren't supported
		case ETzAlphaDaylightNameAscending:
		case ETzAlphaShortStandardNameAscending:
		case ETzAlphaShortDaylightNameAscending:
		case ETzAlphaStandardNameDescending:
		case ETzAlphaDaylightNameDescending:
		case ETzAlphaShortStandardNameDescending:
		case ETzAlphaShortDaylightNameDescending:
		default:
			{
			User::Leave(KErrArgument);
			}
		}
	return sortOrder;
	}
/**
Gets the correct TLinearOrder to sort by
@param aSortOrder
@return the TLinearOrder to sort by
@internalTechnology
*/
TLinearOrder<CTzLocalizedTimeZone> CTzLocalizer::TimeZoneSortOrderL(const TTzSortOrder aSortOrder)
	{
	TLinearOrder<CTzLocalizedTimeZone> sortOrder(NTzLocalSortAndFind::SortUTCAscending);

	switch (aSortOrder)
		{
		case ETzUTCAscending:
			{
			// Sort order already set
			}
		break;

		case ETzUTCDescending:
			{
			sortOrder = TLinearOrder<CTzLocalizedTimeZone>(NTzLocalSortAndFind::SortUTCDescending);
			}
		break;

		case ETzAlphaNameAscending:			//Standard Name
		case ETzAlphaStandardNameAscending:
			{
			sortOrder = TLinearOrder<CTzLocalizedTimeZone>(NTzLocalSortAndFind::SortAlphaStdNameAscending);
			}
		break;

		case ETzAlphaDaylightNameAscending:
			{
			sortOrder = TLinearOrder<CTzLocalizedTimeZone>(NTzLocalSortAndFind::SortAlphaDSTNameAscending);
			}
		break;

		case ETzAlphaShortStandardNameAscending:
			{
			sortOrder = TLinearOrder<CTzLocalizedTimeZone>(NTzLocalSortAndFind::SortAlphaShortStdNameAscending);
			}
		break;

		case ETzAlphaShortDaylightNameAscending:
			{
			sortOrder = TLinearOrder<CTzLocalizedTimeZone>(NTzLocalSortAndFind::SortAlphaShortDSTNameAscending);
			}
		break;

		case ETzAlphaNameDescending:		//Standard Name
		case ETzAlphaStandardNameDescending:
			{
			sortOrder = TLinearOrder<CTzLocalizedTimeZone>(NTzLocalSortAndFind::SortAlphaStdNameDescending);
			}
		break;

		case ETzAlphaDaylightNameDescending:
			{
			sortOrder = TLinearOrder<CTzLocalizedTimeZone>(NTzLocalSortAndFind::SortAlphaDSTNameDescending);
			}
		break;

		case ETzAlphaShortStandardNameDescending:
			{
			sortOrder = TLinearOrder<CTzLocalizedTimeZone>(NTzLocalSortAndFind::SortAlphaShortStdNameDescending);
			}
		break;

		case ETzAlphaShortDaylightNameDescending:
			{
			sortOrder = TLinearOrder<CTzLocalizedTimeZone>(NTzLocalSortAndFind::SortAlphaShortDSTNameDescending);
			}
		break;

		default:
			{
			User::Leave(KErrArgument);
			}
		}
	return sortOrder;
	}

/* Find the city matching the name defined in aLocalizedCity and set its city index to aLocalizedCity
@param aLocalizedCity A CTzLocalizedCity object
@param aDataSource A data source flag
@return Return ETrue if the city has been found, otherwise EFalse.
 */
TBool CTzLocalizer::FindCityAndSetCityIndexL(CTzLocalizedCity& aLocalizedCity, TTzLocalizerDataSource aDataSource)
	{
	TBool hasFound = EFalse;
    CTzLocalizedCityArray* citiesInDb = CTzLocalizedCityArray::NewLC();
    if(aDataSource == ETzDataSourceSystem)
    	{
    	iStaticDataReader->ReadCitiesL(*citiesInDb);
    	}
    else
    	{
    	iUserDataReader->ReadCitiesL(*citiesInDb);
    	}

    TInt KCityCount = citiesInDb->Count();
    for (TInt i = 0; i < KCityCount; ++i)
        {
        if (citiesInDb->At(i).Name() == aLocalizedCity.Name())
            {
            aLocalizedCity.SetCityIndex(citiesInDb->At(i).CityIndex());
            hasFound = ETrue;
            break;
            }
        }
    CleanupStack::PopAndDestroy(citiesInDb);
    return hasFound;
	}

TBool CTzLocalizer::IsOperateOnUserDbL(TInt aTimeZoneId, TBool aUseDataSource) const
	{
	TBool userDbOperation = EFalse;
	if (iDataSource&ETzDataSourceUser || !aUseDataSource)
		{
		CTzId* tzid = CTzId::NewL(aTimeZoneId);
		userDbOperation = tzid->IsUserTzId();
		delete tzid;
		}
	return userDbOperation;
	}

void CTzLocalizer::ReadCitiesL(CTzLocalizedCityArray& aCityArray, TInt aTimeZoneId, TBool aUseDataSource) const
	{
	if (IsOperateOnUserDbL(aTimeZoneId, aUseDataSource))
		{
		iUserDataReader->ReadCitiesL(aCityArray, aTimeZoneId);
		}
	else if (iDataSource&ETzDataSourceSystem)
		{
		iStaticDataReader->ReadCitiesL(aCityArray, aTimeZoneId);
		}
	}

CTzLocalizedCity* CTzLocalizer::ReadDefaultCityL(TInt aTimeZoneId, TBool aUseDataSource) const
	{
	if (IsOperateOnUserDbL(aTimeZoneId, aUseDataSource))
		{
		return iUserDataReader->ReadDefaultCityL(aTimeZoneId);
		}
	else if(iDataSource&ETzDataSourceSystem)
		{
		return iStaticDataReader->ReadDefaultCityL(aTimeZoneId);
		}
	return NULL;
	}

/**
Sets all the cached zones and their associated cities to be those that are
supplied.
@param aTimeZones Array of time zones to be written.
@param aCities Array of cities to be written.
@internalTechnology
*/
void CTzLocalizer::WriteAllFrequentlyUsedZonesL(const CTzLocalizedTimeZoneArray& aTimeZones, const CTzLocalizedCityArray& aCities)
	{
	// Ensure that the numbers of items in the arrays are the expected amount
	__ASSERT_ALWAYS(aTimeZones.Count() == CTzLocalizedTimeZone::ECachedTimeZones &&
					aCities.Count() == CTzLocalizedTimeZone::ECachedTimeZones, User::Leave(KErrArgument));

	RPointerArray<CTzLocalizedTimeZoneRecord> timeZones;
	CleanupStack::PushL(TCleanupItem(CleanupTimeZonePointerArray,&timeZones));
	TInt i = 0;
	TInt end = aTimeZones.Count();
	timeZones.ReserveL(end);
	while (i < end)
		{
		CTzLocalizedTimeZoneRecord* newTimeZone = CreateTimeZoneRecordL(aTimeZones.At(i));
		timeZones.Append(newTimeZone);
		++i;
		}

	RPointerArray<CTzLocalizedCityRecord> cities;
	CleanupStack::PushL(TCleanupItem(CleanupCityPointerArray,&cities));
	i = 0;
	end = aCities.Count();
	cities.ReserveL(end);
	while (i < end)
		{
		CTzLocalizedCityRecord* newCity = CreateCityRecordL(aCities.At(i));
		cities.Append(newCity);
		++i;
		}

	iTzSession.LocalizationWriteAllFrequentlyUsedZonesL(timeZones, cities);

	CleanupStack::PopAndDestroy();	// cities
	CleanupStack::PopAndDestroy();	// timeZones
	}

void CTzLocalizer::SetFrequentlyUsedZoneL(const CTzLocalizedTimeZone& aTimeZone, const CTzLocalizedCity& aCity,
		const CTzLocalizedTimeZone::TTzFrequentlyUsedZone aFrequentlyUsedZone)
	{
	CTzLocalizedTimeZoneRecord* timeZoneRecord = CreateTimeZoneRecordLC(aTimeZone);
    CTzLocalizedCityRecord* cityRecord = CreateCityRecordLC(aCity);
    iTzSession.LocalizationWriteFrequentlyUsedZoneL(*timeZoneRecord, *cityRecord, aFrequentlyUsedZone);
    CleanupStack::PopAndDestroy(cityRecord);
    CleanupStack::PopAndDestroy(timeZoneRecord);
	}

CTzLocalizedTimeZone* CTzLocalizer::GetLocalizedTimeZoneL(TInt aTimeZoneId, TBool aUseDataSource) const
	{
	if (IsOperateOnUserDbL(aTimeZoneId, aUseDataSource))
		{
		return iUserDataReader->ReadTimeZoneL(aTimeZoneId);
		}
	else
		{
		return iStaticDataReader->ReadTimeZoneL(aTimeZoneId);
		}
	}

CTzLocalizedTimeZone* CTzLocalizer::GetLocalizedTimeZoneL(const CTzLocalizedCity& aCity, TBool aUseDataSource) const
	{
	if(IsOperateOnUserDbL(aCity.TzLocalizedId().TimeZoneId(), aUseDataSource))
		{
		return iUserDataReader->ReadTimeZoneL(aCity.TzLocalizedId());
		}
	else
		{
		return iStaticDataReader->ReadTimeZoneL(aCity.TzLocalizedId());
		}
	}

void CTzLocalizer::PopulateCityArrayL(const RPointerArray<CTzLocalizedCityRecord>& aCityRecords,
		CTzLocalizedCityArray& aCities)
	{
	TInt i = 0;
	TInt end = aCityRecords.Count();
	aCities.ReserveL(aCities.Count() + end);
	while (i < end)
		{
		CTzLocalizedCityRecord* record = aCityRecords[i];
		TTzLocalizedId id(record->TzId(), record->TzResourceId());
		CTzLocalizedCity* city = CTzLocalizedCity::NewL(record->Name(), id, record->GroupId());
		city->SetCityIndex(record->Index());
		aCities.AppendL(city);	// Won't leave since we have reserved the necessary space
		++i;
		}
	}

CTzLocalizedTimeZoneRecord* CTzLocalizer::CreateTimeZoneRecordL(const CTzLocalizedTimeZone& aTimeZone)
	{
	return CTzLocalizedTimeZoneRecord::NewL(aTimeZone.TimeZoneId(), aTimeZone.StandardName(),
		aTimeZone.DaylightName(), aTimeZone.ShortStandardName(), aTimeZone.ShortDaylightName(),
		aTimeZone.TzLocalizedId().ResourceId());
	}

CTzLocalizedTimeZoneRecord* CTzLocalizer::CreateTimeZoneRecordLC(const CTzLocalizedTimeZone& aTimeZone)
	{
	return CTzLocalizedTimeZoneRecord::NewLC(aTimeZone.TimeZoneId(), aTimeZone.StandardName(),
		aTimeZone.DaylightName(), aTimeZone.ShortStandardName(), aTimeZone.ShortDaylightName(),
		aTimeZone.TzLocalizedId().ResourceId());
	}

CTzLocalizedCityRecord* CTzLocalizer::CreateCityRecordL(const CTzLocalizedCity& aCity)
	{
	return CTzLocalizedCityRecord::NewL(aCity.Name(), aCity.GroupId(), aCity.CityIndex(),
		aCity.TimeZoneId(), aCity.TzLocalizedId().ResourceId());
	}

CTzLocalizedCityRecord* CTzLocalizer::CreateCityRecordLC(const CTzLocalizedCity& aCity)
	{
	return CTzLocalizedCityRecord::NewLC(aCity.Name(), aCity.GroupId(), aCity.CityIndex(),
		aCity.TimeZoneId(), aCity.TzLocalizedId().ResourceId());
	}

CTzLocalizedTimeZone* CTzLocalizer::CreateTimeZoneL(const CTzLocalizedTimeZoneRecord& aTimeZoneRecord)
	{
	TTzLocalizedId id(aTimeZoneRecord.Id(), aTimeZoneRecord.ResourceId());
	return CTzLocalizedTimeZone::NewL(id, aTimeZoneRecord.StandardName(),
		aTimeZoneRecord.DaylightName(),	aTimeZoneRecord.ShortStandardName(),
		aTimeZoneRecord.ShortDaylightName());
	}

CTzLocalizedCity* CTzLocalizer::CreateCityL(const CTzLocalizedCityRecord& aCityRecord)
	{
	TTzLocalizedId tzId(aCityRecord.TzId(), aCityRecord.TzResourceId());
	CTzLocalizedCity* city = CTzLocalizedCity::NewL(aCityRecord.Name(), tzId, aCityRecord.GroupId());
	city->SetCityIndex(aCityRecord.Index());
	return city;
	}

void CTzLocalizer::CleanupTimeZonePointerArray(TAny* aArray)
	{
	RPointerArray<CTzLocalizedTimeZoneRecord>* array = static_cast<RPointerArray<CTzLocalizedTimeZoneRecord>* >(aArray);
	if (array)
		{
		array->ResetAndDestroy();
		array->Close();
		}
	}

void CTzLocalizer::CleanupCityPointerArray(TAny* aArray)
	{
	RPointerArray<CTzLocalizedCityRecord>* array = static_cast<RPointerArray<CTzLocalizedCityRecord>* >(aArray);
	if (array)
		{
		array->ResetAndDestroy();
		array->Close();
		}
	}
