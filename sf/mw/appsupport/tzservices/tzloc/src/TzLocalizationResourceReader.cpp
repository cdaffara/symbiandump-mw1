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

#include "TzLocalizationResourceReader.h"	//CTzLocalizationResourceReader
#include "TzLocalizationDataTypes.h"		//CTzLocalizedCity etc
#include "TzLocalizer.h"					//TTzLocalizerPanics

#include <bautils.h>						//BaflUtils::NearestLanguageFile

// Resource File locations
_LIT(KTzLocalizationTimeZoneResourceFileName,"\\Resource\\TimeZoneLocalization\\timezones.rSC");
_LIT(KTzLocalizationGroupResourceFileName,"\\Resource\\TimeZoneLocalization\\timezonegroups.rSC");
_LIT(KFlashPath,"c:\\Resource\\TimeZoneLocalization\\");
_LIT(KFlashDrive, "c:");
_LIT(KRom, "z:");

/**
Allocates and constructs a new CTzLocalizationResourceReader object
@return the newly constructed CTzLocalizationResourceReader
@internalTechnology
*/
CTzLocalizationResourceReader* CTzLocalizationResourceReader::NewL()
	{
	CTzLocalizationResourceReader* self = CTzLocalizationResourceReader::NewLC();
	CleanupStack::Pop(self);
	return self;
	}

/**
Allocates and constructs a new CTzLocalizationResourceReader object
The pointer to the new object is left on the cleanup stack
@return the newly constructed CTzLocalizationResourceReader
@internalTechnology
*/
CTzLocalizationResourceReader* CTzLocalizationResourceReader::NewLC()
	{
	CTzLocalizationResourceReader* self = new (ELeave) CTzLocalizationResourceReader();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

/**
Second phase contructor
@internalTechnology
*/
void CTzLocalizationResourceReader::ConstructL()
	{
	User::LeaveIfError(iFs.Connect());

	// Assign Panic category, in case it's needed
	_LIT(KTzLocalizationCategory,"TZLOCALIZATION");
	
	// If resource files are found on flash drive,
	// these will be used exclusively otherwise the 
	// default ones on ROM will be used.	
	CDir* dirList;
	// Get a list of files on flash drive
	TInt ret = iFs.GetDir(KFlashPath, KEntryAttMaskSupported, ESortNone, dirList);
	CleanupStack::PushL(dirList);
	TFileName* timeZoneResourceFileName = new(ELeave) TFileName;
	CleanupStack::PushL(timeZoneResourceFileName);
	TFileName* groupResourceFileName = new(ELeave) TFileName;
	CleanupStack::PushL(groupResourceFileName);
	TInt count = 0;
	if(dirList != NULL)
		{
		count = dirList->Count();		
		}
	
	// Use the files on ROM
	if(ret == KErrPathNotFound || count == 0)
		{
		timeZoneResourceFileName->Format(KRom);
		timeZoneResourceFileName->Append(KTzLocalizationTimeZoneResourceFileName);

		groupResourceFileName->Format(KRom);
		groupResourceFileName->Append(KTzLocalizationGroupResourceFileName);		
		}
	// Use the files on system drive
	else if(ret == KErrNone)
		{		
		if(count > 0)
			{
			timeZoneResourceFileName->Format(KFlashDrive);
			timeZoneResourceFileName->Append(KTzLocalizationTimeZoneResourceFileName);
			
			groupResourceFileName->Format(KFlashDrive);
			groupResourceFileName->Append(KTzLocalizationGroupResourceFileName);
			}
		}
	else
		{
		User::Leave(ret);
		}
	// Form the time zone resource filename using BaflUtils::NearestLanguageFile
	BaflUtils::NearestLanguageFile(iFs,*timeZoneResourceFileName);

	// Form the time zone group resource filename using BaflUtils::NearestLanguageFile
	BaflUtils::NearestLanguageFile(iFs,*groupResourceFileName);
	
	// Open resource files - this can be done now, as it is opened in EFileShareReadersOnly mode		
	TRAPD(timeZoneResErr,iTimeZoneResourceFile.OpenL(iFs,*timeZoneResourceFileName));
	if(timeZoneResErr == KErrNotFound || timeZoneResErr == KErrPathNotFound)
		{
		User::Panic(KTzLocalizationCategory,CTzLocalizer::ETzLocalizerPanicResourceFileNotFound);
		}
	User::LeaveIfError(timeZoneResErr);		
	iTimeZoneResourceFile.ConfirmSignatureL();
		
	TRAPD(groupResErr,iGroupResourceFile.OpenL(iFs,*groupResourceFileName));
	
	// Set iGroupResourceFileExists to so that it can be checked before 
	// iGroupResourceFile is used
	iGroupResourceFileExists = (groupResErr == KErrNone);
	if(iGroupResourceFileExists)
		{
		iGroupResourceFile.ConfirmSignatureL();
		iGroupResourceFileExists = ETrue;
		}
	CleanupStack::PopAndDestroy(3, dirList); // groupResourceFileName, timeZoneResourceFileName, dirList
	}

/**
Destructor
@internalTechnology
*/
CTzLocalizationResourceReader::~CTzLocalizationResourceReader()
	{
	iTimeZoneResourceFile.Close();
	if(iGroupResourceFileExists)
		{
		iGroupResourceFile.Close();
		}
	delete iResourceBuffer;
	iFs.Close();
	}

/**
Searches the time zone resource file for the given time zone ID. This function will
leave if the specified ID is not found.
@param aTimeZoneId time zone to search for.
@return The resource ID or KErrNotFound.
@internalTechnology
*/
TInt CTzLocalizationResourceReader::FindTimeZoneResourceIdL(const TInt aTimeZoneId)
	{
	TInt initialOffset = FirstTimeZoneResourceIdL();

	TInt idToCheck;

	// Search through all resources
	TInt i = 0;
	while(iTimeZoneResourceFile.OwnsResourceIdL(initialOffset + i))
		{
		// Read current resource contents
		BufferResourceL(iTimeZoneResourceFile,initialOffset + i);

		// get the first int16 (WORD in resource STRUCT)
		idToCheck = iResourceReader.ReadInt16();

		// get rid of the buffer, it's no longer needed
		ResetResourceBuffer();

		// perform check
		if(idToCheck == aTimeZoneId)
			{
			// return the current resource ID
			return initialOffset + i;
			}
		++i;
		}

	// If it's got to here the aId hasn't been found, so leave
	User::Leave(KErrNotFound);

	return KErrNotFound;	// To satisfy compiler
	}

/**
Returns the resource Id of the specified resource. This
leaves if this resource is not found in the resource file.
@param aResourceFile The resource file that the offset belongs to.
@param aOffset The offset to return.
@return The resource Id of the first resource containing localised data.
@internalTechnology
*/
TInt CTzLocalizationResourceReader::LocalizedResourceIdL(const RResourceFile& aResourceFile, const TTzResourceOffset aOffset)
	{
	TInt resourceIdToReturn = aResourceFile.Offset();

	// Skip on the relevant number of resources, using the Offset enum
	resourceIdToReturn += aOffset;

	if(!aResourceFile.OwnsResourceIdL(resourceIdToReturn))
		{
		User::Leave(KErrNotFound);
		}

	return resourceIdToReturn;
	}

/**
Returns the resource Id of the first time zone resource. This
leaves if this resource is not found in the resource file.
@return The resource Id of the first time zone resource.
@internalTechnology
*/
TInt CTzLocalizationResourceReader::FirstTimeZoneResourceIdL()
	{
	return LocalizedResourceIdL(iTimeZoneResourceFile,ETzFirstTimeZoneResource);
	}

/**
Returns the resource Id of the first group resource. This
leaves if this resource is not found in the resource file.
This should only be called if iGroupResourceFile != NULL
@return The resource Id of the first group resource.
@internalTechnology
*/
TInt CTzLocalizationResourceReader::FirstGroupResourceIdL()
	{
	return LocalizedResourceIdL(iGroupResourceFile,ETzFirstLocalizedResource);
	}

/**
Returns the resource Id of the resource containing the default cached zone
information. This leaves if this resource is not found in the resource file.
@return The resource Id of the resource containing the default cached zone
information.
@internalTechnology
*/
TInt CTzLocalizationResourceReader::CachedTimeZoneResourceIdL()
	{
	return LocalizedResourceIdL(iTimeZoneResourceFile,ETzFirstLocalizedResource);
	}

/**
Fills iResourceBuffer with the resource specified by aResourceId and sets up
iResourceReader to read from this resource. This should always be followed by a
call to ResetResourceBuffer.  This function will leave if the requested resource
is not found in the specified resource file.
@param aResourceFile The resourcefile to read from.
@param aResourceId The resource to read from.
@internalTechnology
*/
void CTzLocalizationResourceReader::BufferResourceL(const RResourceFile& aResourceFile, const TInt aResourceId)
	{
	if(!aResourceFile.OwnsResourceIdL(aResourceId))
		{
		User::Leave(KErrNotFound);
		}

	// Assign buffer for holding resource
	iResourceBuffer = aResourceFile.AllocReadL(aResourceId);

	// TResourceReader for getting data out of the buffer
	iResourceReader.SetBuffer(iResourceBuffer);
	}

/**
Resets the iResourceBuffer that contains the binary data of a resource.
@internalTechnology
*/
void CTzLocalizationResourceReader::ResetResourceBuffer()
	{
	delete iResourceBuffer;
	iResourceBuffer = NULL;
	}

/**
From MTzLocalizationReader
Reads the specified time zone from the resource file.
@param aTimeZoneId The ID of the time zone to return
@return The specfied localized time zone object.
@leave KErrNotFound Leaves if the requested Time Zone is not found.
@internalTechnology
*/
CTzLocalizedTimeZone* CTzLocalizationResourceReader::ReadTimeZoneL(const TInt aTimeZoneId)
	{
	TInt resourceId = FindTimeZoneResourceIdL(aTimeZoneId);

	return DoReadTimeZoneL(resourceId);
	}

/**
From MTzLocalizationReader
Reads the time zone specified in aTzLocalizedId time
zone from the resource file. If the resource ID is invalid, the time zone is
read using the time zone ID.
@param aTzLocalizedId The TTzLocalizedId specifying the resource ID and Time
zone Id of the Time zone to return.
@return The specfied localized time zone object.
@leave KErrNotFound Leaves if the requested Time Zone is not found.
@internalTechnology
*/
CTzLocalizedTimeZone* CTzLocalizationResourceReader::ReadTimeZoneL(const TTzLocalizedId& aTzLocalizedId)
	{
	CTzLocalizedTimeZone* timeZoneToReturn;

	// Check if the resource is in the time zone resource file
	if(iTimeZoneResourceFile.OwnsResourceIdL(aTzLocalizedId.ResourceId()))
		{
		timeZoneToReturn = DoReadTimeZoneL(aTzLocalizedId.ResourceId());
		}
	// If the resource ID of aTzLocalizedId isn't found in the reource file,
	// attempt to search for the timezone using the timezone id.
	else
		{
		timeZoneToReturn = ReadTimeZoneL(aTzLocalizedId.TimeZoneId());
		}

	return timeZoneToReturn;
	}

/**
From MTzLocalizationReader
Reads all time zones from the resource file and appends them to the supplied array.
@param aTimeZones An array that at the end of this function contains all time zones.
@leave KErrNotFound Leaves if the no time zones are found in the resource file.
@internalTechnology
*/
void CTzLocalizationResourceReader::ReadAllTimeZonesL(CTzLocalizedTimeZoneArray& aTimeZones)
	{
	TInt initialOffset = FirstTimeZoneResourceIdL();

	// Variables for us in loop
	CTzLocalizedTimeZone* timeZone;

	// Search through all resources
	TInt i = 0;
	while(iTimeZoneResourceFile.OwnsResourceIdL(initialOffset + i))
		{
		// Read new time zone
		timeZone = DoReadTimeZoneL(initialOffset + i);
		CleanupStack::PushL(timeZone);

		// Add new time zone to the array (ownership is transferred into array)
		aTimeZones.AppendL(timeZone);
		CleanupStack::Pop(timeZone);
		timeZone = NULL;

		++i;
		}

	// If no owned resources were found - Leave
	if(i == 0)
		{
		User::Leave(KErrNotFound);
		}
	}

/**
Reads in and returns a localized time zone found at the specified resource id.
@param aResourceId The resource ID of the time zone in the time zone resource
file
@return The localized time zone found at the specified resource ID.
@internalTechnology
*/
CTzLocalizedTimeZone* CTzLocalizationResourceReader::DoReadTimeZoneL(const TInt aResourceId)
	{
	BufferResourceL(iTimeZoneResourceFile,aResourceId);

	CTzLocalizedTimeZone* timeZone = CreateTimeZoneFromResourceL(aResourceId);

	ResetResourceBuffer();

	return timeZone;
	}

/**
This does the actual reading of a localized time zone from the resource file.
The iResourceReader MUST already have been set up with a call to
BufferResourceL(iTimeZoneResourceFile,...), BEFORE this function is called.

@return A CTzLocalizedTimeZone containing the localized time zone information
that is currently found in iResourceReader.

@internalTechnology
*/
CTzLocalizedTimeZone* CTzLocalizationResourceReader::CreateTimeZoneFromResourceL(const TInt aResourceId)
	{
	// Read localized time zone data form the resource file in the correct order
	TInt16 timeZoneId = (TInt16)iResourceReader.ReadInt16();
	TPtrC standardName(iResourceReader.ReadTPtrC());
	TPtrC daylightName(iResourceReader.ReadTPtrC());
	TPtrC shortStandardName(iResourceReader.ReadTPtrC());
	TPtrC shortDaylightName(iResourceReader.ReadTPtrC());

	TTzLocalizedId tzlocalId(timeZoneId,aResourceId);
	CTzLocalizedTimeZone* timeZone = CTzLocalizedTimeZone::NewL
		(
		tzlocalId,
		standardName,
		daylightName,
		shortStandardName,
		shortDaylightName
		);

	return timeZone;
	}

/**
From MTzLocalizationReader Reads all cities from the resource file and appends
them to the supplied array.
@param aCities An array that at the end of this function will contain all
cities contained in the resource file.

@leave KErrNotFound Leaves if no cities are found.
@internalTechnology
*/
void CTzLocalizationResourceReader::ReadCitiesL(CTzLocalizedCityArray& aCities)
	{
	TInt initialOffset = FirstTimeZoneResourceIdL();

	// Search through all resources
	TInt i = 0;

	while(iTimeZoneResourceFile.OwnsResourceIdL(initialOffset + i))
		{
		// Read new city into the city array
		DoReadCitiesL(aCities, initialOffset + i);
		++i;
		}

	// If no owned resources were found - Leave
	if(i == 0)
		{
		User::Leave(KErrNotFound);
		}
	}

/**
From MTzLocalizationReader Read the cities that are in the specified time zone
into the specified city array.
@param aCities An array of cities that will contain the cities from the
specified time zone.
@param aTimeZoneId The ID of the time zone containing the cities to return,.
@leave KErrNotFound Leaves if the specified time zone is not found or it
contains no cities.
@internalTechnology
*/
void CTzLocalizationResourceReader::ReadCitiesL(CTzLocalizedCityArray& aCities, const TInt aTimeZoneId)
	{
	TInt resourceId = FindTimeZoneResourceIdL(aTimeZoneId);

	DoReadCitiesL(aCities, resourceId);
	}

/**
From MTzLocalizationReader Read the cities that are in the specified time zone
into the specifed city array. If the resource ID is invalid, the correct
resource ID is found in the resource file.
@param An array of cities that will contain the cities from the specified time
zone.
@param aTzLocalizedId The TTzLocalizedId specifying of the time zone ID and
resource ID of the time zone containing the cities to return,.
@leave KErrNotFound Leaves if the specified time zone is not found or it
contains no cities.
@internalTechnology
*/
void CTzLocalizationResourceReader::ReadCitiesL(CTzLocalizedCityArray& aCities, const TTzLocalizedId& aTzLocalizedId)
	{
	// Check if the resource is in the time zone resource file
	if(iTimeZoneResourceFile.OwnsResourceIdL(aTzLocalizedId.ResourceId()))
		{
		DoReadCitiesL(aCities, aTzLocalizedId.ResourceId());
		}
	// search for aTzLocalizedId's timezone ID instead
	else
		{
		ReadCitiesL(aCities, aTzLocalizedId.TimeZoneId());
		}
	}

/**
Reads in and adds the array of cities contained in the specified resource ID to
the supplied city array
@param The array of cities to which the the cities found in the time zone at
the specified resource ID will be appended to.
@param aResourceId The resource ID of the time zone that contains the cities to
be read.
@internalTechnology
*/
void CTzLocalizationResourceReader::DoReadCitiesL(CTzLocalizedCityArray& aCities, const TInt aResourceId)
	{
	BufferResourceL(iTimeZoneResourceFile,aResourceId);

	AddCityArrayFromResourceL(aCities,aResourceId);

	ResetResourceBuffer();
	}

/**
This does the actual reading of the localized cities from the resource file.
The iResourceReader MUST already have been set up with a call to
BufferResourceL(iTimeZoneResourceFile,...) BEFORE this function is called.
@param aCities The array of cities to add the resource cities to.
@param aResourceId The resource ID of the time zone containing the cities to
read.
@return An array of CTzLocalizedCity objects containing the cities that are
currently found in iResourceReader.
@internalTechnology
*/
void CTzLocalizationResourceReader::AddCityArrayFromResourceL(CTzLocalizedCityArray& aCities, const TInt aResourceId)
	{
	// Eat up the localised time zone info from the resource
	CTzLocalizedTimeZone* timeZone = CreateTimeZoneFromResourceL(aResourceId);

	// Create a TTZlocalizedId to give to all the cities
	TTzLocalizedId tzLocalizedId(timeZone->TimeZoneId(),aResourceId);

	// Delete timeZone as it is no longer needed
	delete timeZone;
	timeZone = NULL;

	// Get the number of cities (which automatically precedes the array as a WORD in the STRUCT)
	TInt numCities = iResourceReader.ReadInt16();

	// Leave if no cities are found
	if(numCities < 1)
		{
		User::Leave(KErrNotFound);
		}

	// variables for use in the loop
	TUint8 groupId;
	TPtrC cityName;
	CTzLocalizedCity* city;

	// Loop through all the cities
	for(TInt i = 0; i < numCities; ++i)
		{
		groupId = iResourceReader.ReadUint8();
		cityName.Set(iResourceReader.ReadTPtrC());

		city = CTzLocalizedCity::NewLC(cityName,tzLocalizedId,groupId);
        city->SetCityIndex(i);

		aCities.AppendL(city);

		CleanupStack::Pop(city);
		city = NULL;
		}
	}

/**
From MTzLocalizationReader
Reads the cities from the specified city group and adds them to the specified city array.
@param aCities The array to add the cities from the specified group to.
@param aGroupId The ID of the group containing the cities to return.
@leave KErrNotFound Leaves if no groups are found in the resource file, or if
the group numbering in the resource file is incorrect.
@internalTechnology
*/
void CTzLocalizationResourceReader::ReadCitiesInGroupL(CTzLocalizedCityArray& aCities, const TUint8 aGroupId)
	{
	if(iGroupResourceFileExists)
	// Only do this if a groups resource file exists - otherwise no groups have been specified
		{
		TInt initialOffset = FirstTimeZoneResourceIdL();

		// Variables for use in loop
		TUint8 groupIdToCheck;
		CTzLocalizedCityArray* citiesInTimeZone = CTzLocalizedCityArray::NewLC();
		CTzLocalizedCity* currentCity;

		// Loop through all time zones looking for cities in the requested group
		TInt i = 0;
		while(iTimeZoneResourceFile.OwnsResourceIdL(initialOffset + i))
			{
			DoReadCitiesL(*citiesInTimeZone, initialOffset + i);

			// Check cities in this time zone to see if they are in the specified group
			// in reverse order, because we remove as we go.
			TInt numCitiesInZone = citiesInTimeZone->Count();
			for(TInt j = numCitiesInZone - 1; j>=0; --j)
				{
				groupIdToCheck = citiesInTimeZone->At(j).GroupId();
				currentCity = &(citiesInTimeZone->At(j));
				
				// Remove currentCity from citiesInTimeZone now, so that we 
				// don't have to PopAndDestroy citiesInGroup and reallocate 
				// once per while loop
				citiesInTimeZone->Remove(j);

				// If we are adding this city, move it from citiesInTimeZone to the aCities
				if(groupIdToCheck == aGroupId)
					{
					aCities.AppendL(currentCity);
					}
				// otherwise get rid of it while we still have a pointer to it.
				else
					{
					delete currentCity;
					}
				currentCity = NULL;
				}
			++i;
			}
		CleanupStack::PopAndDestroy(citiesInTimeZone);
		citiesInTimeZone = NULL;
		}
	}

/**
From MTzLocalizationReader
Reads all groups from the resource file and adds them to the specified group array.
No groups are added to the array if none are found.
@param aGroups The array to add the city groups to.
@internalTechnology
*/
void CTzLocalizationResourceReader::ReadAllGroupsL(CTzLocalizedCityGroupArray& aGroups)
	{
	if(iGroupResourceFileExists)
		{
		TInt initialOffset = FirstGroupResourceIdL();

		// Variables for use in the loop
		CTzLocalizedCityGroup* groupToAdd;

		TInt i = 0;
		while(iGroupResourceFile.OwnsResourceIdL(initialOffset + i))
			{
			groupToAdd = DoReadGroupL(initialOffset + i);

			CleanupStack::PushL(groupToAdd);
			aGroups.AppendL(groupToAdd);
			// we are transferring ownership into the array			
			CleanupStack::Pop(groupToAdd);
			groupToAdd = NULL;

			++i;
			}
		}
	}

/**
From MTzLocalizationReader Reads the specified group and returns it.
@param aGroupId The ID of the group to return.
@return The specified city group object or NULL if the groups resource file
does not exist.
@leave KErrNotFound Leaves if the specified group ID is not found.
@internalTechnology
*/
CTzLocalizedCityGroup* CTzLocalizationResourceReader::ReadGroupL(const TUint8 aGroupId)
	{
	if(aGroupId < 1)
		{
		User::Leave(KErrArgument);
		}

	CTzLocalizedCityGroup* group = NULL;
	
	if(iGroupResourceFileExists)
		{
		TInt initialOffset = FirstGroupResourceIdL();

		// The groups are numbered 1 to N, where N is the number of resources
		TInt resourceToRead = initialOffset + aGroupId - 1;

		// Assign a group to return
		group = DoReadGroupL(resourceToRead);
		}
	else
	// The group resource file doesn't exist - leave with KErrNotFound
	// This means that it is transparent to the user, whether the groups 
	// resource file exists or not, because this function already leaves
	// with KErrNotFound if the specified Group ID is not found.
		{
		User::Leave(KErrNotFound);
		}

	return group;
	}


/**
Reads in the group at the specified resource ID from the group resource file.
Should only be called if iGroupResourceFile != NULL
@param aResourceId The resource ID to get the group from.
@return The group specifed by aResourceId.
@leave KErrNotFound Leaves if the specified resouce ID is not found.
@internalTechnology
*/
CTzLocalizedCityGroup* CTzLocalizationResourceReader::DoReadGroupL(const TInt aResourceId)
	{
	BufferResourceL(iGroupResourceFile,aResourceId);

	CTzLocalizedCityGroup* group = CreateGroupFromResourceL();

	ResetResourceBuffer();

	return group;
	}

/**
This does the actual reading of a city group from the group resource file.
The iResourceReader MUST already have been set up with a call to
BufferResourceL(iGroupResourceFile,...), BEFORE this function is called.
@param aResourceId The resource ID of the group to read.
@return The group at the specifed resource ID in the the group resource file.
@leave KErrNotFound Leaves if the specified resouce ID is not found.
@internalTechnology
*/
CTzLocalizedCityGroup* CTzLocalizationResourceReader::CreateGroupFromResourceL()
	{
	TUint8 groupId = iResourceReader.ReadUint8();
	TPtrC groupName(iResourceReader.ReadTPtrC());

	CTzLocalizedCityGroup* group = CTzLocalizedCityGroup::NewL(groupName,groupId);

	return group;
	}

/**
From MtzLocalizationReader
Read and return the default city from the specified time zone.
@param aTimeZoneId The ID of the time zone of the default city to return
@return The defulat city of the specified time zone.
@internalTechnology
*/
CTzLocalizedCity* CTzLocalizationResourceReader::ReadDefaultCityL(const TInt aTimeZoneId)
	{
	// Read the cities from the specified resource
	CTzLocalizedCityArray* cities = CTzLocalizedCityArray::NewLC();
	ReadCitiesL(*cities,aTimeZoneId);

	// Move first element (which is the default city) out of cities
	// to stop it being deleted when cities is
	CTzLocalizedCity* defaultCity = &(cities->At(0));
	cities->Remove(0);

	CleanupStack::PopAndDestroy(cities);

	return defaultCity;
	}

/**
From MtzLocalizationReader
Read and return the default city from the specified time zone.
@param aTzLocalizedId The TTzLocalizedId of the time zone of the default city to return
@return The defulat city of the specified time zone.
@internalTechnology
*/
CTzLocalizedCity* CTzLocalizationResourceReader::ReadDefaultCityL(const TTzLocalizedId& aTzLocalizedId)
	{
	// Read the cities from the specified resource
	CTzLocalizedCityArray* cities = CTzLocalizedCityArray::NewLC();
	ReadCitiesL(*cities,aTzLocalizedId);

	// Move first element (which is the default city) out of cities
	// to stop it being deleted when cities is
	CTzLocalizedCity* defaultCity = &(cities->At(0));
	cities->Remove(0);

	CleanupStack::PopAndDestroy(cities);

	return defaultCity;
	}

/**
From MTzLocalizationReader
Read and return the specified cached zone.
@param aFrequentlyUsedZone the cached zone to return
@return The specified cached zone.
@internalTechnology
*/
CTzLocalizedTimeZone* CTzLocalizationResourceReader::ReadFrequentlyUsedZoneL(const CTzLocalizedTimeZone::TTzFrequentlyUsedZone aFrequentlyUsedZone)
	{
	// You cannot pass ECachedTimeZones in as the argument, because it is only
	// used to keep count of how many cached zones there are.
	__ASSERT_ALWAYS(aFrequentlyUsedZone != CTzLocalizedTimeZone::ECachedTimeZones, User::Leave(KErrArgument));

	TInt offset = CachedTimeZoneResourceIdL();

	BufferResourceL(iTimeZoneResourceFile,offset);

	TInt defaultHomeZoneId = iResourceReader.ReadInt16();
	TInt defaultInterestZoneId = iResourceReader.ReadInt16();
	TInt defaultRecentZone1Id = iResourceReader.ReadInt16();
	TInt defaultRecentZone2Id = iResourceReader.ReadInt16();

	ResetResourceBuffer();

	CTzLocalizedTimeZone* cachedTimeZone = NULL;

	switch(aFrequentlyUsedZone)
		{
		case CTzLocalizedTimeZone::EHomeZone:
			cachedTimeZone = ReadTimeZoneL(defaultHomeZoneId);
		break;
		case CTzLocalizedTimeZone::EInterestZone:
			cachedTimeZone = ReadTimeZoneL(defaultInterestZoneId);
		break;
		case CTzLocalizedTimeZone::ERecentZone1:
			cachedTimeZone = ReadTimeZoneL(defaultRecentZone1Id);
		break;
		case CTzLocalizedTimeZone::ERecentZone2:
			cachedTimeZone = ReadTimeZoneL(defaultRecentZone2Id);
		break;
		case CTzLocalizedTimeZone::ECurrentZone:	// FALL THROUGH to default - current zone not supported
		default:
			User::Leave(KErrArgument);
		}

	return cachedTimeZone;
	}

/**
From MTzLocalizationReader
Read and return the city used to set thespecified cached zone.
@param aFrequentlyUsedZone the cached zone specifying which city to return.
@return The city used to set thespecified cached zone.
@internalTechnology
*/
CTzLocalizedCity* CTzLocalizationResourceReader::ReadCachedTimeZoneCityL(const CTzLocalizedTimeZone::TTzFrequentlyUsedZone aFrequentlyUsedZone)
	{
	// You cannot pass ECachedTimeZones in as the argument, because it is only
	// used to keep count of how many cached zones there are.
	__ASSERT_ALWAYS(aFrequentlyUsedZone != CTzLocalizedTimeZone::ECachedTimeZones, User::Leave(KErrArgument));

	TInt offset = CachedTimeZoneResourceIdL();

	BufferResourceL(iTimeZoneResourceFile,offset);

	TInt defaultHomeZoneId = iResourceReader.ReadInt16();
	TInt defaultInterestZoneId = iResourceReader.ReadInt16();
	TInt defaultRecentZone1Id = iResourceReader.ReadInt16();
	TInt defaultRecentZone2Id = iResourceReader.ReadInt16();

	ResetResourceBuffer();

	CTzLocalizedCity* cachedCity = NULL;

	switch(aFrequentlyUsedZone)
		{
		case CTzLocalizedTimeZone::EHomeZone:
			cachedCity = ReadDefaultCityL(defaultHomeZoneId);
		break;
		case CTzLocalizedTimeZone::EInterestZone:
			cachedCity = ReadDefaultCityL(defaultInterestZoneId);
		break;
		case CTzLocalizedTimeZone::ERecentZone1:
			cachedCity = ReadDefaultCityL(defaultRecentZone1Id);
		break;
		case CTzLocalizedTimeZone::ERecentZone2:
			cachedCity = ReadDefaultCityL(defaultRecentZone2Id);
		break;
		case CTzLocalizedTimeZone::ECurrentZone:	// FALL THROUGH to default - current zone not supported
		default:
			User::Leave(KErrArgument);
		}

	return cachedCity;
	}
//============================================================================
// End of file
//============================================================================


