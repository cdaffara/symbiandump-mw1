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

#include "TzLocalizationDataTypes.h"
#include "TzLocalizer.h"

// Represents an invalid UTC offset of 25 hours and 1 minutes, in minutes
const TInt KTzInvalidUTCOffset = 1501;

//==================================================================
// TzLocalizedId Member Functions
//==================================================================

/**
Constructor.
@param aTzId The time zone ID to store.
@param aResourceId The resource ID corresponding to aTzId.
*/
TTzLocalizedId::TTzLocalizedId(const TUint16 aTzId, const TUint aResourceId)
:	iTzId(aTzId),
	iResourceId(aResourceId)
	{
	}

//==================================================================
// CTzLocalizedCity Member Functions
//==================================================================
/**
Two phase Symbian constructor.
@param aName The name of the city.
@param aTzLocalizedId Contains the time zone ID and resource ID for this city.
@param aGroupId The group that this city belongs to.  This defaults to zero.
@return A pointer to a new instance of a localized city object.
*/
CTzLocalizedCity* CTzLocalizedCity::NewL(const TDesC& aName, const TTzLocalizedId& aTzLocalizedId, const TUint8 aGroupId)
	{
	CTzLocalizedCity* self = CTzLocalizedCity::NewLC(aName, aTzLocalizedId, aGroupId);
	CleanupStack::Pop(self);
	return self;
	}

/**
Two phase Symbian constructor.
@param aName The name of the city.
@param aTzLocalizedId Contains the time zone ID and resource ID for this city.
@param aGroupId The group that this city belongs to.  This defaults to zero.
@return A pointer to a new instance of a localized city object that is left on
the cleanup stack.
*/
CTzLocalizedCity* CTzLocalizedCity::NewLC(const TDesC& aName, const TTzLocalizedId& aTzLocalizedId, const TUint8 aGroupId)
	{
	CTzLocalizedCity* self = new (ELeave) CTzLocalizedCity(aGroupId, aTzLocalizedId);
	CleanupStack::PushL(self);
	self->ConstructL(aName);
	return self;
	}

/**
Returns the name of the city.
@return The name of the city.
*/
EXPORT_C TPtrC CTzLocalizedCity::Name() const
	{
	TPtrC name;
	name.Set(*iName);
	return name;
	}

/**
Returns the city's time zone ID.
@return The time zone ID.
*/
EXPORT_C TUint16 CTzLocalizedCity::TimeZoneId() const
	{
	return iTzLocalizedId.TimeZoneId();
	}

/**
Returns the ID of the group to which the city belongs.
@return The ID of the group to which the city belongs.
*/
EXPORT_C TUint8 CTzLocalizedCity::GroupId() const
	{
	return iGroupId;
	}

/**
Returns the TTzLocalizedId of this city. Cannot be inline, as it is for internal use only.
@return The TTzLocalizedId of this city.
@internalTechnology
*/
TTzLocalizedId CTzLocalizedCity::TzLocalizedId() const
	{
	return iTzLocalizedId;
	}

/**
Second Phase constructor.
@param aName The city name.
*/
void CTzLocalizedCity::ConstructL(const TDesC& aName)
	{
	iName = aName.AllocL();
	}

/**
Constructor.
@param aGroupId The ID of the group that this city belongs to.
@param aTzLocalizedId Contains the timezone ID and resource ID for this city.
*/
CTzLocalizedCity::CTzLocalizedCity(const TUint8 aGroupId, const TTzLocalizedId& aTzLocalizedId)
:	iGroupId(aGroupId),
	iTzLocalizedId(aTzLocalizedId),
    iCityIndex(KErrNotFound),
    iUTCOffset(KTzInvalidUTCOffset)
	{
	}

/**
Destructor
*/
CTzLocalizedCity::~CTzLocalizedCity()
	{
	delete iName;
	}

/**
Returns the UTC offset for this city.
This is for internal sorting use only.  A more accurate offset can be
obtained from Tz by using the time zone id stored in iTzLocalizedId.
@return TInt representing UTC offset (in minutes).
@internalTechnology
*/
TInt CTzLocalizedCity::UTCOffset() const
	{
	return iUTCOffset;
	}

/**
Sets the UTC offset for this city (in minutes).
@param aUTCoffset UTC offset in minutes for this time zone.
@internalTechnology
*/
void CTzLocalizedCity::SetUTCOffset(TInt aUTCoffset)
	{
	iUTCOffset = aUTCoffset;
	}

void CTzLocalizedCity::SetCityIndex(TInt aIndex)
    {
    iCityIndex = aIndex;
    }

TInt CTzLocalizedCity::CityIndex() const
    {
    return iCityIndex;
    }

//==================================================================
// CTzLocalizedCityGroup Member Functions
//==================================================================
/**
Two phase Symbian constructor.
@param aName The group's name.
@param aGroupId The unique group ID.
@return A pointer to a new instance of a localized city group object.
*/
CTzLocalizedCityGroup* CTzLocalizedCityGroup::NewL(const TDesC& aName, const TUint8 aGroupId)
	{
	CTzLocalizedCityGroup* self = CTzLocalizedCityGroup::NewLC(aName, aGroupId);
	CleanupStack::Pop(self);
	return self;
	}

/**
Two phase Symbian constructor.
@param aName The group's name.
@param aGroupId The unique group ID.
@return A pointer to a new instance of a localized city group object that is
left on the cleanup stack.
*/
CTzLocalizedCityGroup* CTzLocalizedCityGroup::NewLC(const TDesC& aName, const TUint8 aGroupId)
	{
	CTzLocalizedCityGroup* self = new (ELeave) CTzLocalizedCityGroup(aGroupId);
	CleanupStack::PushL(self);
	self->ConstructL(aName);
	return self;
	}

/**
Destructor
*/
CTzLocalizedCityGroup::~CTzLocalizedCityGroup()
	{
	delete iName;
	}

/**
Returns the name of the localized city group.
@return The name the of localized city group.
*/
EXPORT_C TPtrC CTzLocalizedCityGroup::Name() const
	{
	TPtrC name;
	name.Set(*iName);
	return name;
	}

/**
Returns the unique group ID.
@return The unique group ID.
*/
EXPORT_C TUint8 CTzLocalizedCityGroup::Id() const
	{
	return iGroupId;
	}

/**
Second phase construtor
@param aName The city group name.
*/
void CTzLocalizedCityGroup::ConstructL(const TDesC& aName)
	{
	iName = aName.AllocL();
	}

/**
First phase constructor.
@param aGroupId The city group ID.
*/
CTzLocalizedCityGroup::CTzLocalizedCityGroup(const TUint8 aGroupId)
:	iGroupId(aGroupId)
	{
	}

//==================================================================
// CTzLocalizedTimeZone Member Functions
//==================================================================
/**
Two phase Symbian constructor.
@param aTzLocalizedId Contains the time zone ID and resource ID for
this time zone.
@param aStandardName The standard name of the time zone, for instance "Greenwich Mean
Time", "Pacific Standard Time".
@param aDaylightName The daylight saving time name, for instance "British Summer Time",
"Pacific Daylight Time" of this time zone.
@param aShortStandardName The short standard name, for instance "GMT", "PST" of this
time zone.
@param aShortDaylightName The short daylight saving time name for instance "BST",
"PDT" of this time zone.
@return A pointer to a new instance of a localized time zone object.
*/
CTzLocalizedTimeZone* CTzLocalizedTimeZone::NewL(
		const TTzLocalizedId& aTzLocalizedId,
		const TDesC& aStandardName,
		const TDesC& aDaylightName,
		const TDesC& aShortStandardName,
		const TDesC& aShortDaylightName)
	{
	CTzLocalizedTimeZone* self = CTzLocalizedTimeZone::NewLC(
		aTzLocalizedId,aStandardName,aDaylightName,aShortStandardName,aShortDaylightName);
	CleanupStack::Pop(self);
	return self;
	}

/**
Two phase Symbian constructor.

@param aTzLocalizedId Contains the time zone ID and resource ID for
this time zone.
@param aStandardName The standard name of the time zone, for instance "Greenwich Mean
Time", "Pacific Standard Time".
@param aDaylightName The daylight saving time name, for instance "British Summer Time",
"Pacific Daylight Time" of this time zone.
@param aShortStandardName The short standard name, for instance "GMT", "PST" of this
time zone.
@param aShortDaylightName The short daylight saving time name for instance "BST",
"PDT" of this time zone.
@return A pointer to a new instance of a localized time zone object that is 
left on the cleanup stack.
*/
CTzLocalizedTimeZone* CTzLocalizedTimeZone::NewLC(
		const TTzLocalizedId& aTzLocalizedId,
		const TDesC& aStandardName,
		const TDesC& aDaylightName,
		const TDesC& aShortStandardName,
		const TDesC& aShortDaylightName)
	{
	CTzLocalizedTimeZone* self = new (ELeave) CTzLocalizedTimeZone(aTzLocalizedId);
	CleanupStack::PushL(self);
	self->ConstructL
		(aStandardName,aDaylightName,aShortStandardName,aShortDaylightName);
	return self;
	}

/**
Destructor
*/
CTzLocalizedTimeZone::~CTzLocalizedTimeZone()
	{
	delete iStandardName;
	delete iDaylightName;
	delete iShortStandardName;
	delete iShortDaylightName;
	}

/**
Returns the time zone ID.
@return The time zone ID.
@internalTechnology
*/
EXPORT_C TUint16 CTzLocalizedTimeZone::TimeZoneId() const
	{
	return iTzLocalizedId.TimeZoneId();
	}

/**
Returns this time zone's standard name, for instance "Greenwich Mean Time", 
"Pacific Standard Time".
@return The standard name of this time zone.
*/
EXPORT_C TPtrC CTzLocalizedTimeZone::StandardName() const
	{
	TPtrC standardName;
	standardName.Set(*iStandardName);
	return standardName;
	}

/**
Returns this time zone's daylight saving time name, for instance  "British Summer Time", 
"Pacific Daylight Time".
@return The daylight saving time name of this time zone.
*/
EXPORT_C TPtrC CTzLocalizedTimeZone::DaylightName() const
	{
	TPtrC daylightName;
	daylightName.Set(*iDaylightName);
	return daylightName;
	}

/**
Returns this time zone's short standard name, for instance "GMT", "PST".
@return The short standard name of this time zone.
*/
EXPORT_C TPtrC CTzLocalizedTimeZone::ShortStandardName() const
	{
	TPtrC shortStandardName;
	shortStandardName.Set(*iShortStandardName);
	return shortStandardName;
	}

/**
Returns this time zone's short daylight saving time name, for instance "BST", "PDT".
@return The short daylight saving time name of this time zone.
*/
EXPORT_C TPtrC CTzLocalizedTimeZone::ShortDaylightName() const
	{
	TPtrC shortDaylightName;
	shortDaylightName.Set(*iShortDaylightName);
	return shortDaylightName;
	}

/**
Returns this time zone's TTzLocalizedId. Cannot be inline, as it is for
internal use only.
@return The TTzLocalizedId of this time zone.
@internalTechnology
*/
TTzLocalizedId CTzLocalizedTimeZone::TzLocalizedId() const
	{
	return iTzLocalizedId;
	}

/**
Returns the UTC offset for this time zone.
This is for internal sorting use only.  A more accurate offset can be
obtained from Tz by using the time zone id stored in iTzLocalizedId
@return TInt representing UTC offset (in minutes)
@internalTechnology
*/
TInt CTzLocalizedTimeZone::UTCOffset() const
	{
	return iUTCOffset;
	}

/**
Sets the UTC offset for this time zone (in minutes)
@param aUTCoffset UTC offset in minutes for this time zone.
@internalTechnology
*/
void CTzLocalizedTimeZone::SetUTCOffset(TInt aUTCoffset)
	{
	iUTCOffset = aUTCoffset;
	}

/**
Second phase construtor.
@param aStandardName The standard name of the time zone, for instance "Greenwich Mean
Time", "Pacific Standard Time".
@param aDaylightName The daylight saving time name, for instance "British Summer Time",
"Pacific Daylight Time" of this time zone.
@param aShortStandardName The short standard name, for instance "GMT", "PST" of this
time zone.
@param aShortDaylightName The short daylight saving time name, for instance "BST",
"PDT" of this time zone.
*/
void CTzLocalizedTimeZone::ConstructL(
		const TDesC& aStandardName,
		const TDesC& aDaylightName,
		const TDesC& aShortStandardName,
		const TDesC& aShortDaylightName)
	{
	iStandardName = aStandardName.AllocL();
	iDaylightName = aDaylightName.AllocL();
	iShortStandardName = aShortStandardName.AllocL();
	iShortDaylightName = aShortDaylightName.AllocL();
	}

/**
First phase construtor
*/
CTzLocalizedTimeZone::CTzLocalizedTimeZone(const TTzLocalizedId& aTzLocalizedId)
	: iTzLocalizedId(aTzLocalizedId), iUTCOffset(KTzInvalidUTCOffset)
	{
	}

//=============================================================================
// CTzLocalizedCityArray Member Functions
//=============================================================================
/**
Allocates and constructs a new CTzLocalizedCityArray object.
@return The newly created CTzLocalizedCityArray object.
*/
EXPORT_C CTzLocalizedCityArray* CTzLocalizedCityArray::NewL()
	{
	CTzLocalizedCityArray* self = CTzLocalizedCityArray::NewLC();
	CleanupStack::Pop(self);
	return self;
	}

/**
Allocates and constructs a new CTzLocalizedCityArray object.
The pointer to the new object is left on the cleanup stack.
@return The newly created CTzLocalizedCityArray object.
*/
EXPORT_C CTzLocalizedCityArray* CTzLocalizedCityArray::NewLC()
	{
	CTzLocalizedCityArray* self = new (ELeave) CTzLocalizedCityArray();
	CleanupStack::PushL(self);
	return self;
	}

/**
Returns the city at the specified index.
@param aIndex Index of the element to get.
@return Reference to the city at aIndex.
*/
EXPORT_C CTzLocalizedCity& CTzLocalizedCityArray::At(TInt aIndex) const
	{
	return CTzLocalizedArray<CTzLocalizedCity>::At(aIndex);
	}
	
/**
Returns the number of elements in the city array.
@return The number of elements in the array.
*/
EXPORT_C TInt CTzLocalizedCityArray::Count() const
	{
	return CTzLocalizedArray<CTzLocalizedCity>::Count();
	}
	
/**
Appends a city to this array. Ownership of the pointer is transferred
to the array.
@param aCity Pointer to the city to add.
*/
EXPORT_C void CTzLocalizedCityArray::AppendL(CTzLocalizedCity* aCity)
	{
	CTzLocalizedArray<CTzLocalizedCity>::AppendL(aCity);
	}
	
/**
Removes the city at the specified index.
@param aIndex The index of the city to remove.
*/
EXPORT_C void CTzLocalizedCityArray::Remove(TInt aIndex)
	{
	CTzLocalizedArray<CTzLocalizedCity>::Remove(aIndex);
	}

/**
Sorts this array.
@param aOrder The sorting function to use.
*/	
EXPORT_C void CTzLocalizedCityArray::Sort(TLinearOrder<CTzLocalizedCity> aOrder)
	{
	CTzLocalizedArray<CTzLocalizedCity>::Sort(aOrder);
	}
	
/**
Searches for a city in the array.
@param aCity The city to search for.
@param aIdentityRelation The search function to use.
@return The index of the first matching object in the array, 
or KErrNotFound if no match is found.
*/
EXPORT_C TInt CTzLocalizedCityArray::Find(CTzLocalizedCity* aCity,TIdentityRelation<CTzLocalizedCity> aIdentityRelation)
	{
	return CTzLocalizedArray<CTzLocalizedCity>::Find(aCity, aIdentityRelation);
	}

void CTzLocalizedCityArray::ReserveL(TInt aCount)
	{
	CTzLocalizedArray<CTzLocalizedCity>::ReserveL(aCount);
	}

//=============================================================================
// CTzLocalizedCityGroupArray Member Functions
//=============================================================================
/**
Allocates and constructs a new CTzLocalizedCityGroupArray object.
@return The newly created CTzLocalizedCityGroupArray object.
*/
EXPORT_C CTzLocalizedCityGroupArray* CTzLocalizedCityGroupArray::NewL()
	{
	CTzLocalizedCityGroupArray* self = CTzLocalizedCityGroupArray::NewLC();
	CleanupStack::Pop(self);
	return self;
	}

/**
Allocates and constructs a new CTzLocalizedCityGroupArray object.
The pointer to the new object is left on the cleanup stack.
@return The newly created CTzLocalizedCityGroupArray object.
*/
EXPORT_C CTzLocalizedCityGroupArray* CTzLocalizedCityGroupArray::NewLC()
	{
	CTzLocalizedCityGroupArray* self = new (ELeave) CTzLocalizedCityGroupArray();
	CleanupStack::PushL(self);
	return self;
	}

/**
Returns the group at the specified index.
@param aIndex Index of the element to get.
@return Reference to the group at aIndex.
*/
EXPORT_C CTzLocalizedCityGroup& CTzLocalizedCityGroupArray::At(TInt aIndex) const
	{
	return CTzLocalizedArray<CTzLocalizedCityGroup>::At(aIndex);
	}
	
/**
Returns the number of elements in the array.
@return The number of elements in the array.
*/
EXPORT_C TInt CTzLocalizedCityGroupArray::Count() const
	{
	return CTzLocalizedArray<CTzLocalizedCityGroup>::Count();
	}
	
/**
Appends a group to this array. Ownership of the pointer is transferred 
to the array.
@param aGroup A pointer to the group to add.
*/
EXPORT_C void CTzLocalizedCityGroupArray::AppendL(CTzLocalizedCityGroup* aGroup)
	{
	CTzLocalizedArray<CTzLocalizedCityGroup>::AppendL(aGroup);
	}
	
/**
Removes the group at the specified index.
@param aIndex Index of the group to remove.
*/
EXPORT_C void CTzLocalizedCityGroupArray::Remove(TInt aIndex)
	{
	CTzLocalizedArray<CTzLocalizedCityGroup>::Remove(aIndex);
	}
	
/**
Sorts the array.
@param aOrder Sorting function to use.
*/
EXPORT_C void CTzLocalizedCityGroupArray::Sort(TLinearOrder<CTzLocalizedCityGroup> aOrder)
	{
	CTzLocalizedArray<CTzLocalizedCityGroup>::Sort(aOrder);
	}
	
/**
Searches for a group in this array.
@param aGroup The group to search for.
@param aIdentityRelation The search function to use.
@return The index of the first matching object in the array, 
or KErrNotFound if no match is found.
*/
EXPORT_C TInt CTzLocalizedCityGroupArray::Find(CTzLocalizedCityGroup* aGroup,TIdentityRelation<CTzLocalizedCityGroup> aIdentityRelation)
	{
	return CTzLocalizedArray<CTzLocalizedCityGroup>::Find(aGroup, aIdentityRelation);
	}

void CTzLocalizedCityGroupArray::ReserveL(TInt aCount)
	{
	CTzLocalizedArray<CTzLocalizedCityGroup>::ReserveL(aCount);
	}
//=============================================================================
// CTzLocalizedTimeZoneArray Member Functions
//=============================================================================
/**
Allocates and constructs a new CTzLocalizedTimeZoneArray object.
@return The newly created CTzLocalizedTimeZoneArray object.
*/
EXPORT_C CTzLocalizedTimeZoneArray* CTzLocalizedTimeZoneArray::NewL()
	{
	CTzLocalizedTimeZoneArray* self = CTzLocalizedTimeZoneArray::NewLC();
	CleanupStack::Pop(self);
	return self;
	}

/**
Allocates and constructs a new CTzLocalizedTimeZoneArray object.
The pointer to the new object is left on the cleanup stack.
@return The newly created CTzLocalizedTimeZoneArray object.
*/
EXPORT_C CTzLocalizedTimeZoneArray* CTzLocalizedTimeZoneArray::NewLC()
	{
	CTzLocalizedTimeZoneArray* self = new (ELeave) CTzLocalizedTimeZoneArray();
	CleanupStack::PushL(self);
	return self;
	}

/**
Returns the time zone at the specified index.
@param aIndex The index of the element.
@return A reference to the time zone at aIndex.
*/
EXPORT_C CTzLocalizedTimeZone& CTzLocalizedTimeZoneArray::At(TInt aIndex) const
	{
	return CTzLocalizedArray<CTzLocalizedTimeZone>::At(aIndex);
	}
	
/**
Returns the number of elements in the array.
@return The number of elements in the array.
*/
EXPORT_C TInt CTzLocalizedTimeZoneArray::Count() const
	{
	return CTzLocalizedArray<CTzLocalizedTimeZone>::Count();
	}
	
/**
Appends a time zone to this array. Ownership of the pointer is transferred
to the array.
@param aTimeZone A pointer to the time zone to add.
*/
EXPORT_C void CTzLocalizedTimeZoneArray::AppendL(CTzLocalizedTimeZone* aTimeZone)
	{
	CTzLocalizedArray<CTzLocalizedTimeZone>::AppendL(aTimeZone);
	}
	
/**
Removes the time zone at the specified index.
@param aIndex The index of the time zone to remove.
*/
EXPORT_C void CTzLocalizedTimeZoneArray::Remove(TInt aIndex)
	{
	CTzLocalizedArray<CTzLocalizedTimeZone>::Remove(aIndex);
	}
	
/**
Sorts the array.
@param aOrder The sorting function to use.
*/	
EXPORT_C void CTzLocalizedTimeZoneArray::Sort(TLinearOrder<CTzLocalizedTimeZone> aOrder)
	{
	CTzLocalizedArray<CTzLocalizedTimeZone>::Sort(aOrder);
	}
	
/**
Searches for a time zone in this array.
@param aTimeZone The time zone to search for.
@param aIdentityRelation The search function to use.
@return The index of the first matching object in the array, 
or KErrNotFound if no match is found.
*/
EXPORT_C TInt CTzLocalizedTimeZoneArray::Find(CTzLocalizedTimeZone* aTimeZone,TIdentityRelation<CTzLocalizedTimeZone> aIdentityRelation)
	{
	return CTzLocalizedArray<CTzLocalizedTimeZone>::Find(aTimeZone, aIdentityRelation);
	}

void CTzLocalizedTimeZoneArray::ReserveL(TInt aCount)
	{
	CTzLocalizedArray<CTzLocalizedTimeZone>::ReserveL(aCount);
	}
