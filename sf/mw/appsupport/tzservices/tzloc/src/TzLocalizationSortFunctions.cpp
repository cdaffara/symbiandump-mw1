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

#include "TzLocalizationSortFunctions.h"	//NTzLocalSort

/**
Comparison function used by CTzLocalizedTimeZoneArray::Sort
Orders the array by UTC offset in ascending order
@param aTimeZone1 First time zone to compare
@param aTimeZone2 Second time zone to compare
@return <0 if aTimeZone1 < aTimeZone2, 0 if equal, >0 if aTimeZone1 > aTimeZone2
@internalTechnology
*/
TInt NTzLocalSortAndFind::SortUTCAscending(const CTzLocalizedTimeZone& aTimeZone1, const CTzLocalizedTimeZone& aTimeZone2)
	{
	if (aTimeZone1.UTCOffset() == aTimeZone2.UTCOffset())
		{
		return 0;
		}
	return (aTimeZone1.UTCOffset() < aTimeZone2.UTCOffset()) ? -1 : 1;
	}
/**
Comparison function used by CTzLocalizedTimeZoneArray::Sort
Orders the array by UTC offset in descending order
@param aTimeZone1 First time zone to compare
@param aTimeZone2 Second time zone to compare
@return <0 if aTimeZone2 < aTimeZone1, 0 if equal, >0 if aTimeZone2 > aTimeZone1
@internalTechnology
*/
TInt NTzLocalSortAndFind::SortUTCDescending(const CTzLocalizedTimeZone& aTimeZone1, const CTzLocalizedTimeZone& aTimeZone2)
	{
	if (aTimeZone1.UTCOffset() == aTimeZone2.UTCOffset())
		{
		return 0;
		}
	return (aTimeZone2.UTCOffset() < aTimeZone1.UTCOffset()) ? -1 : 1;
	}
/**
Comparison function used by CTzLocalizedTimeZoneArray::Sort
Orders the array by Standard Name in ascending order
@param aTimeZone1 First time zone to compare
@param aTimeZone2 Second time zone to compare
@return <0 if aTimeZone1 < aTimeZone2, 0 if equal, >0 if aTimeZone1 > aTimeZone2
@internalTechnology
*/
TInt NTzLocalSortAndFind::SortAlphaStdNameAscending(const CTzLocalizedTimeZone& aTimeZone1, const CTzLocalizedTimeZone& aTimeZone2)
	{
	return aTimeZone1.StandardName().CompareC(aTimeZone2.StandardName());
	}
/**
Comparison function used by CTzLocalizedTimeZoneArray::Sort
Orders the array by Standard Name in descending order
@param aTimeZone1 First time zone to compare
@param aTimeZone2 Second time zone to compare
@return <0 if aTimeZone2 < aTimeZone1, 0 if equal, >0 if aTimeZone2 > aTimeZone1
@internalTechnology
*/
TInt NTzLocalSortAndFind::SortAlphaStdNameDescending(const CTzLocalizedTimeZone& aTimeZone1, const CTzLocalizedTimeZone& aTimeZone2)
	{
	return aTimeZone2.StandardName().CompareC(aTimeZone1.StandardName());
	}
/**
Comparison function used by CTzLocalizedTimeZoneArray::Sort
Orders the array by Daylight savings name in ascending order
@param aTimeZone1 First time zone to compare
@param aTimeZone2 Second time zone to compare
@return <0 if aTimeZone1 < aTimeZone2, 0 if equal, >0 if aTimeZone1 > aTimeZone2
@internalTechnology
*/
TInt NTzLocalSortAndFind::SortAlphaDSTNameAscending(const CTzLocalizedTimeZone& aTimeZone1, const CTzLocalizedTimeZone& aTimeZone2)
	{
	return aTimeZone1.DaylightName().CompareC(aTimeZone2.DaylightName());
	}
/**
Comparison function used by CTzLocalizedTimeZoneArray::Sort
Orders the array by Daylight savings name in descending order
@param aTimeZone1 First time zone to compare
@param aTimeZone2 Second time zone to compare
@return <0 if aTimeZone2 < aTimeZone1, 0 if equal, >0 if aTimeZone2 > aTimeZone1
@internalTechnology
*/
TInt NTzLocalSortAndFind::SortAlphaDSTNameDescending(const CTzLocalizedTimeZone& aTimeZone1, const CTzLocalizedTimeZone& aTimeZone2)
	{
	return aTimeZone2.DaylightName().CompareC(aTimeZone1.DaylightName());
	}
/**
Comparison function used by CTzLocalizedTimeZoneArray::Sort
Orders the array by Short Standard Name in ascending order
@param aTimeZone1 First time zone to compare
@param aTimeZone2 Second time zone to compare
@return <0 if aTimeZone1 < aTimeZone2, 0 if equal, >0 if aTimeZone1 > aTimeZone2
@internalTechnology
*/
TInt NTzLocalSortAndFind::SortAlphaShortStdNameAscending(const CTzLocalizedTimeZone& aTimeZone1, const CTzLocalizedTimeZone& aTimeZone2)
	{
	return aTimeZone1.ShortStandardName().CompareC(aTimeZone2.ShortStandardName());
	}
/**
Comparison function used by CTzLocalizedTimeZoneArray::Sort
Orders the array by Short Standard Name in descending order
@param aTimeZone1 First time zone to compare
@param aTimeZone2 Second time zone to compare
@return <0 if aTimeZone2 < aTimeZone1, 0 if equal, >0 if aTimeZone2 > aTimeZone1
@internalTechnology
*/
TInt NTzLocalSortAndFind::SortAlphaShortStdNameDescending(const CTzLocalizedTimeZone& aTimeZone1, const CTzLocalizedTimeZone& aTimeZone2)
	{
	return aTimeZone2.ShortStandardName().CompareC(aTimeZone1.ShortStandardName());
	}
/**
Comparison function used by CTzLocalizedTimeZoneArray::Sort
Orders the array by Short Daylight savings name in ascending order
@param aTimeZone1 First time zone to compare
@param aTimeZone2 Second time zone to compare
@return <0 if aTimeZone1 < aTimeZone2, 0 if equal, >0 if aTimeZone1 > aTimeZone2
@internalTechnology
*/
TInt NTzLocalSortAndFind::SortAlphaShortDSTNameAscending(const CTzLocalizedTimeZone& aTimeZone1, const CTzLocalizedTimeZone& aTimeZone2)
	{
	return aTimeZone1.ShortDaylightName().CompareC(aTimeZone2.ShortDaylightName());
	}
/**
Comparison function used by CTzLocalizedTimeZoneArray::Sort
Orders the array by Short Daylight savings name in descending order
@param aTimeZone1 First time zone to compare
@param aTimeZone2 Second time zone to compare
@return <0 if aTimeZone2 < aTimeZone1, 0 if equal, >0 if aTimeZone2 > aTimeZone1
@internalTechnology
*/
TInt NTzLocalSortAndFind::SortAlphaShortDSTNameDescending(const CTzLocalizedTimeZone& aTimeZone1, const CTzLocalizedTimeZone& aTimeZone2)
	{
	return aTimeZone2.ShortDaylightName().CompareC(aTimeZone1.ShortDaylightName());
	}
/**
Comparison function used by CTzLocalizedCityArray::Sort
Orders the array by UTC in ascending order
@param aCity1 First city to compare
@param aCity2 Second city to compare
@return <0 if aCity1 < aCity2, 0 if equal, >0 if aCity1 > aCity2
@internalTechnology
*/
TInt NTzLocalSortAndFind::SortCityUTCAscending(const CTzLocalizedCity& aCity1, const CTzLocalizedCity& aCity2)
	{
	if (aCity1.UTCOffset() == aCity2.UTCOffset())
		{
		return SortCityNameAscending(aCity1,aCity2);
		}
	return (aCity1.UTCOffset() < aCity2.UTCOffset()) ? -1 : 1;
	}
/**
Comparison function used by CTzLocalizedCityArray::Sort
Orders the array by UTC offset in descending order
@param aCity1 First city to compare
@param aCity2 Second city to compare
@return <0 if aCity2 < aCity1, 0 if equal, >0 if aCity2 > aCity1
@internalTechnology
*/
TInt NTzLocalSortAndFind::SortCityUTCDescending(const CTzLocalizedCity& aCity1, const CTzLocalizedCity& aCity2)
	{
	if (aCity1.UTCOffset() == aCity2.UTCOffset())
		{
		return SortCityNameDescending(aCity1,aCity2);
		}
	return (aCity2.UTCOffset() < aCity1.UTCOffset()) ? -1 : 1;
	}
/**
Comparison function used by CTzLocalizedCityArray::Sort
Orders the array by city name in ascending order
@param aCity1 First city to compare
@param aCity2 Second city to compare
@return <0 if aCity1 < aCity2, 0 if equal, >0 if aCity1 > aCity2
@internalTechnology
*/
TInt NTzLocalSortAndFind::SortCityNameAscending(const CTzLocalizedCity& aCity1, const CTzLocalizedCity& aCity2)
	{
	return aCity1.Name().CompareC(aCity2.Name());
	}
/**
Comparison function used by CTzLocalizedCityArray::Sort
Orders the array by city name in descending order
@param aCity1 First city to compare
@param aCity2 Second city to compare
@return <0 if aCity2 < aCity1, 0 if equal, >0 if aCity2 > aCity1
@internalTechnology
*/
TInt NTzLocalSortAndFind::SortCityNameDescending(const CTzLocalizedCity& aCity1, const CTzLocalizedCity& aCity2)
	{
	return aCity2.Name().CompareC(aCity1.Name());
	}
/**
Comparison function used by CTzLocalizedCityGroupArray::Sort
Orders the array by city group name in ascending order
@param aCity1 First city group to compare
@param aCity2 Second city groupto compare
@return <0 if aCityGroup1 < aCityGroup2, 0 if equal, >0 if aCityGroup1 > aCityGroup2
@internalTechnology
*/
TInt NTzLocalSortAndFind::SortCityGroupNameAscending(const CTzLocalizedCityGroup& aCityGroup1, const CTzLocalizedCityGroup& aCityGroup2)
	{
	return aCityGroup1.Name().CompareC(aCityGroup2.Name());
	}
/**
Comparison function used by CTzLocalizedCityGroupArray::Sort
Orders the array by city group name in descending order
@param aCity1 First city group to compare
@param aCity2 Second city groupto compare
@return <0 if aCityGroup2 < aCityGroup1, 0 if equal, >0 if aCityGroup2 > aCityGroup1
@internalTechnology
*/
TInt NTzLocalSortAndFind::SortCityGroupNameDescending(const CTzLocalizedCityGroup& aCityGroup1, const CTzLocalizedCityGroup& aCityGroup2)
	{
	return aCityGroup2.Name().CompareC(aCityGroup1.Name());
	}

/**
Find function used by CTzLocalizedCityArray::Find
@param aCity1 First city to check
@param aCity2 Second city to check
@return ETrue if aCity1.Name() matches aCity2.Name(), EFalse if the names do not match
@internalTechnology
*/
TInt NTzLocalSortAndFind::FindCityByName(const CTzLocalizedCity& aCity1, const CTzLocalizedCity& aCity2)
	{
	return aCity1.Name().FindC(aCity2.Name()) != KErrNotFound;
	}
/**
Find function used by CTzLocalizedTimeZoneArray::Find
@param aCity1 First citygroup to check
@param aCity2 Second citygroup to check
@return ETrue if aCityGroup1.Name() matches aCityGroup2.Name(), EFalse if the names do not match
@internalTechnology
*/
TInt NTzLocalSortAndFind::FindTimeZoneByName(const CTzLocalizedTimeZone& aTimeZone1, const CTzLocalizedTimeZone& aTimeZone2)
	{
	return 
			(FindNames(aTimeZone1.StandardName(), aTimeZone2.StandardName()) ||
			 FindNames(aTimeZone1.DaylightName(), aTimeZone2.DaylightName()) ||
			 FindNames(aTimeZone1.ShortDaylightName(),aTimeZone2.ShortDaylightName()) ||
			 FindNames(aTimeZone1.ShortStandardName(),aTimeZone2.ShortStandardName()));
	}


/**
Find function called by FindTimeZoneByName
@param aName1 First text to check
@param aName2 Second text to check
@return ETrue if aName1 and aName2 are identified, otherwise return EFalse.
@internalComponent
*/
TInt NTzLocalSortAndFind::FindNames(const TDesC& aName1, const TDesC& aName2)
	{
	if(aName1==KNullDesC && aName2==KNullDesC)
		{
		return ETrue;
		}
	
	if(aName1 == KNullDesC || aName2 == KNullDesC)
		{//One of them is KNullDesC and the other is not
		return EFalse;
		}
	
	return (aName1.FindC(aName2) != KErrNotFound); 
	}

/**
Find function used by CTzLocalizedCityGroupArray::Find
@param aCity1 First citygroup to check
@param aCity2 Second citygroup to check
@return ETrue if aCityGroup1.Name() matches aCityGroup2.Name(), EFalse if the names do not match
@internalTechnology
*/
TInt NTzLocalSortAndFind::FindCityGroupByName(const CTzLocalizedCityGroup& aCityGroup1,const CTzLocalizedCityGroup& aCityGroup2)
	{
	return (aCityGroup2.Name().CompareC(aCityGroup1.Name()) == 0);
	}

//============================================================================
// End of file
//============================================================================
