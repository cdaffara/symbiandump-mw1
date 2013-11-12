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

#include "tzsystemdata.h"
#include <bautils.h>
#include <tzlocalizedcityrecord.h>
#include <tzlocalizedtimezonerecord.h>

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
CTzSystemDataDb* CTzSystemDataDb::NewLC()
	{
	CTzSystemDataDb* self = new(ELeave) CTzSystemDataDb();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

/**
Destructor
@internalTechnology
*/
CTzSystemDataDb::~CTzSystemDataDb()
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
Second phase contructor
@internalTechnology
*/
void CTzSystemDataDb::ConstructL()
	{
	User::LeaveIfError(iFs.Connect());
	
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
	iTimeZoneResourceFile.OpenL(iFs,*timeZoneResourceFileName);
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
Searches the time zone resource file for the given time zone ID. This function will
leave if the specified ID is not found.
@param aTimeZoneId time zone to search for.
@return The resource ID or KErrNotFound.
@internalTechnology
*/
TInt CTzSystemDataDb::FindTimeZoneResourceIdL(TInt aTimeZoneId)
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
TInt CTzSystemDataDb::LocalizedResourceIdL(const RResourceFile& aResourceFile, const TTzResourceOffset aOffset)
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
TInt CTzSystemDataDb::FirstTimeZoneResourceIdL()
	{
	return LocalizedResourceIdL(iTimeZoneResourceFile,ETzFirstTimeZoneResource);
	}

/**
Returns the resource Id of the resource containing the default cached zone
information. This leaves if this resource is not found in the resource file.
@return The resource Id of the resource containing the default cached zone
information.
@internalTechnology
*/
TInt CTzSystemDataDb::CachedTimeZoneResourceIdL()
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
void CTzSystemDataDb::BufferResourceL(const RResourceFile& aResourceFile, TInt aResourceId)
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
void CTzSystemDataDb::ResetResourceBuffer()
	{
	delete iResourceBuffer;
	iResourceBuffer = NULL;
	}

CTzLocalizedTimeZoneRecord* CTzSystemDataDb::ReadTimeZoneL(TInt aTimeZoneId)
	{
	TInt resourceId = FindTimeZoneResourceIdL(aTimeZoneId);
	return DoReadTimeZoneL(resourceId);
	}

/**
Reads in and returns a localized time zone found at the specified resource id.
@param aResourceId The resource ID of the time zone in the time zone resource
file
@return The localized time zone found at the specified resource ID.
@internalTechnology
*/
CTzLocalizedTimeZoneRecord* CTzSystemDataDb::DoReadTimeZoneL(TInt aResourceId)
	{
	BufferResourceL(iTimeZoneResourceFile,aResourceId);
	CTzLocalizedTimeZoneRecord* timeZone = CreateTimeZoneRecordFromResourceL(aResourceId);
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
CTzLocalizedTimeZoneRecord* CTzSystemDataDb::CreateTimeZoneRecordFromResourceL(TInt aResourceId)
	{
	// Read localized time zone data form the resource file in the correct order
	TInt16 timeZoneId = (TInt16)iResourceReader.ReadInt16();
	TPtrC standardName(iResourceReader.ReadTPtrC());
	TPtrC daylightName(iResourceReader.ReadTPtrC());
	TPtrC shortStandardName(iResourceReader.ReadTPtrC());
	TPtrC shortDaylightName(iResourceReader.ReadTPtrC());

	CTzLocalizedTimeZoneRecord* timeZone = CTzLocalizedTimeZoneRecord::NewL(timeZoneId, standardName,
		daylightName, shortStandardName, shortDaylightName, aResourceId);

	return timeZone;
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
void CTzSystemDataDb::ReadCitiesL(RPointerArray<CTzLocalizedCityRecord>& aCities, TInt aTimeZoneId)
	{
	TInt resourceId = FindTimeZoneResourceIdL(aTimeZoneId);
	DoReadCitiesL(aCities, resourceId);
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
void CTzSystemDataDb::DoReadCitiesL(RPointerArray<CTzLocalizedCityRecord>& aCities, TInt aResourceId)
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
void CTzSystemDataDb::AddCityArrayFromResourceL(RPointerArray<CTzLocalizedCityRecord>& aCities, TInt aResourceId)
	{
	// Eat up the localised time zone info from the resource
	CTzLocalizedTimeZoneRecord* timeZone = CreateTimeZoneRecordFromResourceL(aResourceId);
	TUint tzid = timeZone->Id();
	delete timeZone;
	timeZone = NULL;

	// Get the number of cities (which automatically precedes the array as a WORD in the STRUCT)
	TInt numCities = iResourceReader.ReadInt16();

	// Leave if no cities are found
	if(numCities < 1)
		{
		User::Leave(KErrNotFound);
		}

	aCities.ReserveL(numCities + aCities.Count());
	// Loop through all the cities
	for(TInt i = 0; i < numCities; ++i)
		{
		TUint8 groupId = iResourceReader.ReadUint8();
		TPtrC cityName;
		cityName.Set(iResourceReader.ReadTPtrC());
		CTzLocalizedCityRecord* city = CTzLocalizedCityRecord::NewLC(cityName,groupId,i,tzid,aResourceId);
        aCities.Append(city);
		CleanupStack::Pop(city);
		}
	}

CTzLocalizedCityRecord* CTzSystemDataDb::ReadDefaultCityL(TInt aTimeZoneId)
	{
	// Read the cities from the specified resource
	RPointerArray<CTzLocalizedCityRecord> cities;
	CleanupStack::PushL(TCleanupItem(CTzLocalizedCityRecord::CleanupArray, &cities));
	ReadCitiesL(cities,aTimeZoneId);

	// Move first element (which is the default city) out of cities
	// to stop it being deleted when cities is
	CTzLocalizedCityRecord* defaultCity = cities[0];
	cities.Remove(0);

	CleanupStack::PopAndDestroy();	// cities
	
	return defaultCity;
	}

TInt CTzSystemDataDb::ReadFrequentlyUsedZoneIdL(TInt aFrequentlyUsedZone)
	{
	// You cannot pass ECachedTimeZones in as the argument, because it is only
	// used to keep count of how many cached zones there are.
	__ASSERT_ALWAYS(aFrequentlyUsedZone != CTzLocalizedTimeZone::ECachedTimeZones, User::Leave(KErrArgument));

	TInt offset = CachedTimeZoneResourceIdL();

	BufferResourceL(iTimeZoneResourceFile,offset);
	
	TInt tzid = 0;
	TInt defaultHomeZoneId = iResourceReader.ReadInt16();
	TInt defaultInterestZoneId = iResourceReader.ReadInt16();
	TInt defaultRecentZone1Id = iResourceReader.ReadInt16();
	TInt defaultRecentZone2Id = iResourceReader.ReadInt16();
	switch(aFrequentlyUsedZone)
			{
			case CTzLocalizedTimeZone::EHomeZone:
				tzid = defaultHomeZoneId;
				break;
			case CTzLocalizedTimeZone::EInterestZone:
				tzid = defaultInterestZoneId;
				break;
			case CTzLocalizedTimeZone::ERecentZone1:
				tzid = defaultRecentZone1Id;
				break;
			case CTzLocalizedTimeZone::ERecentZone2:
				tzid = defaultRecentZone2Id;
				break;
			case CTzLocalizedTimeZone::ECurrentZone:	// FALL THROUGH to default - current zone not supported
			default:
				User::Leave(KErrArgument);
			}
	ResetResourceBuffer();
	return tzid;
	}

CTzLocalizedTimeZoneRecord* CTzSystemDataDb::ReadFrequentlyUsedZoneL(TInt aFrequentlyUsedZone)
	{
	TInt tzid = ReadFrequentlyUsedZoneIdL(aFrequentlyUsedZone);
	return ReadTimeZoneL(tzid);
	}
