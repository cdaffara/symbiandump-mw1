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

#ifndef __TZLOCALIZATION_SORT_FUNCTIONS_H__
#define __TZLOCALIZATION_SORT_FUNCTIONS_H__

#include <e32base.h> 					//CBase
#include "TzLocalizationDataTypes.h"	//CTzLocalizedTimeZone etc

/**
Defines sorting functions used by the CTzLocalizedArray sorting functions
These are passed in as function pointers to RPointerArray::Sort
*/
class  NTzLocalSortAndFind
	{
	//Time Zone sorting functions
	public:
	//Sort by UTC offset
	static TInt SortUTCAscending(const CTzLocalizedTimeZone& aTimeZone1, const CTzLocalizedTimeZone& aTimeZone2);
	static TInt SortUTCDescending(const CTzLocalizedTimeZone& aTimeZone1, const CTzLocalizedTimeZone& aTimeZone2);

	//Sort by Standard Name
	static TInt SortAlphaStdNameAscending(const CTzLocalizedTimeZone& aTimeZone1, const CTzLocalizedTimeZone& aTimeZone2);
	static TInt SortAlphaStdNameDescending(const CTzLocalizedTimeZone& aTimeZone1, const CTzLocalizedTimeZone& aTimeZone2);

	//Sort by DST Name
	static TInt SortAlphaDSTNameAscending(const CTzLocalizedTimeZone& aTimeZone1, const CTzLocalizedTimeZone& aTimeZone2);
	static TInt SortAlphaDSTNameDescending(const CTzLocalizedTimeZone& aTimeZone1, const CTzLocalizedTimeZone& aTimeZone2);

	//Sort by Short Standard Name
	static TInt SortAlphaShortStdNameAscending(const CTzLocalizedTimeZone& aTimeZone1, const CTzLocalizedTimeZone& aTimeZone2);
	static TInt SortAlphaShortStdNameDescending(const CTzLocalizedTimeZone& aTimeZone1, const CTzLocalizedTimeZone& aTimeZone2);

	//Sort by Short DST Name
	static TInt SortAlphaShortDSTNameAscending(const CTzLocalizedTimeZone& aTimeZone1, const CTzLocalizedTimeZone& aTimeZone2);
	static TInt SortAlphaShortDSTNameDescending(const CTzLocalizedTimeZone& aTimeZone1, const CTzLocalizedTimeZone& aTimeZone2);

	//City Sorting functions

	//Sort by UTC offset
	static TInt SortCityUTCAscending(const CTzLocalizedCity& aCity1, const CTzLocalizedCity& aCity2);
	static TInt SortCityUTCDescending(const CTzLocalizedCity& aCity1, const CTzLocalizedCity& aCity2);

	//Sort by City Name
	static TInt SortCityNameAscending(const CTzLocalizedCity& aCity1, const CTzLocalizedCity& aCity2);
	static TInt SortCityNameDescending(const CTzLocalizedCity& aCity1, const CTzLocalizedCity& aCity2);

	//City Group sorting functions
	//Sort by City Group Name
	static TInt SortCityGroupNameAscending(const CTzLocalizedCityGroup& aCityGroup1, const CTzLocalizedCityGroup& aCityGroup2);
	static TInt SortCityGroupNameDescending(const CTzLocalizedCityGroup& aCityGroup1, const CTzLocalizedCityGroup& aCityGroup2);

	//Find functions
	static TInt FindCityByName(const CTzLocalizedCity& aCity1, const CTzLocalizedCity& aCity2);
	static TInt FindTimeZoneByName(const CTzLocalizedTimeZone& aTimeZone1, const CTzLocalizedTimeZone& aTimeZone2);
	static TInt FindCityGroupByName(const CTzLocalizedCityGroup& aCityGroup1,const CTzLocalizedCityGroup& aCityGroup2);
	static TInt FindNames(const TDesC& aName1, const TDesC& aName2);
};


#endif //__TZLOCALIZATION_SORT_FUNCTIONS_H__
//============================================================================
// End of file
//============================================================================
