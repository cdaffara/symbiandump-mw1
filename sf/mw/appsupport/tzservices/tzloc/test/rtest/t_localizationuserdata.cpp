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


#include <e32std.h>
#include <e32des16.h>
#include <e32test.h>
#include <tzlocalizer.h> 
#include <tzlocalizationdatatypes.h>
#include <tz.h>
#include <vtzrules.h>
#include "testserver.h"
#include <e32math.h>
#include <tzlocalizedcityrecord.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <tzusernames.h>
#include <tzuserdefineddata.h>
#endif


class CTzLocalizationUserDataSuite : public CBase
	{
public:
    static CTzLocalizationUserDataSuite* NewLC();
	~CTzLocalizationUserDataSuite();
	void TestLocalizedTimeZoneL();
	void TestLocalizedTimeZoneWithLocalizeIDL();
	void TestLocalizedCityL();
	void TestDefaultCityL();
	void TestCityGroupL();
	void TestCityArrayL();
	void TestCurrentTimeZoneL();
	void TestFindTimeZoneL();
	void TestUserDataModificationL(CTzLocalizedTimeZone::TTzFrequentlyUsedZone aTimeZone);
	void TestUserDataDeletionL();
	void TestUserDataDeletionCityImpactL();
	void TestUserDefinedTimeZoneL();

private:
	CTzLocalizationUserDataSuite();
	void ConstructL();
	void ResetTzUserDataL();

	CTzId* CreateTimeZoneL();
	CTzUserNames* CreateUserRuleAndGetNamesLC(TInt& aTimeZoneId);
	void TestFindTimeZoneByNameL(TInt aTimeZoneId, const CTzUserNames& aNames,const TDesC& aNameToFind);
	
	CTzRules* CreateRulesLC();
	CTzUserNames* CreateNamesLC();

	TBool CompareLocalizedCity(TInt aTimeZoneId, const CTzUserNames& aNames, const CTzLocalizedCity& aCity);
	TBool CompareTimeZoneNames(TInt aTimezoneId, const CTzUserNames& aNames, const CTzLocalizedTimeZone& aLocalizedTimeZone);
	TBool CompareLocalizedCityGroup(const CTzUserNames& aNames, const CTzLocalizedCityGroup& aCityGroup);
	TBool FindCityInArrayL(const CTzLocalizedCityArray& aCityArray, TInt aTimeZoneId, const CTzUserNames& aNames);
	TBool CompareLocalizedCity(const CTzLocalizedCity& aCity, const CTzLocalizedCity& aCityToCompare);
	TBool CompareTimeZoneName(const CTzLocalizedTimeZone& aTimeZone, const CTzLocalizedTimeZone& aTimeZoneToCompare);
	
	void CreateString(TDes& aString);
	TBool TestTimeZoneShortStandardNameL(CTzLocalizer& aTzLocalizer, const TDesC& aShortStandardName);
	
private:
	CTzLocalizer* 	iLocalizer;
	RTz				iTzSession;
	CTzUserData*	iUserData;
	TInt64 			iSeed;
	};


_LIT(KTestName,"t_localizationuserdata");
_LIT(KPerFile1,"c:\\private\\100012a5\\DBS_2000836D_DBS_DSC.db");
_LIT(KPerFile2,"c:\\private\\100012a5\\DBS_10206A8B_TzLocalization.db");
	
_LIT(KUserAddedCity1Name, "UserAddedCity1");
_LIT(KUserAddedCity2Name, "UserAddedCity2");
_LIT(KUserAddedCity3Name, "UserAddedCity3");


const TInt KLondonTzId = 2592;
const TInt KParisTzId = 2656;


RTest test(KTestName);


LOCAL_D void ResetAndDestroyTzIdArray(TAny* aPtr)
	{
	RPointerArray<CTzId>* ids = static_cast<RPointerArray<CTzId>*>(aPtr);
	if (ids)
		{
		ids->ResetAndDestroy();
		}
	}


CTzLocalizationUserDataSuite* CTzLocalizationUserDataSuite::NewLC()
	{
	CTzLocalizationUserDataSuite* self = new (ELeave) CTzLocalizationUserDataSuite();
	CleanupStack::PushL(self);
	self->ConstructL();
	return (self);
	}


CTzLocalizationUserDataSuite::CTzLocalizationUserDataSuite():iSeed(100)
	{
	}


void CTzLocalizationUserDataSuite::ConstructL()
	{
	iLocalizer = CTzLocalizer::NewL();
	iLocalizer->SetDataSource(CTzLocalizer::ETzDataSourceSystem|CTzLocalizer::ETzDataSourceUser);
	iLocalizer->SetTimeZoneL(KLondonTzId);
	
	User::LeaveIfError(iTzSession.Connect());
	iUserData = CTzUserData::NewL(iTzSession);
	}


CTzLocalizationUserDataSuite::~CTzLocalizationUserDataSuite()	
	{
	TRAP_IGNORE(iLocalizer->SetTimeZoneL(KLondonTzId));
	delete iLocalizer;
	delete iUserData;
	iTzSession.Close();
	}


CTzUserNames* CTzLocalizationUserDataSuite::CreateUserRuleAndGetNamesLC(TInt& aTimeZoneId)
	{
	CTzId* tzid = CreateTimeZoneL();
	CleanupStack::PushL(tzid);
	CTzUserNames* names = iUserData->ReadNamesL(*tzid);
	aTimeZoneId = tzid->TimeZoneNumericID();
	CleanupStack::PopAndDestroy(tzid);
	CleanupStack::PushL(names);
	return names;
	}


TBool CTzLocalizationUserDataSuite::CompareTimeZoneNames(TInt aTimeZoneId, const CTzUserNames& aNames, const CTzLocalizedTimeZone& aLocalizedTimeZone)
	{
	if (aTimeZoneId != aLocalizedTimeZone.TimeZoneId())
		{
		return EFalse; 
		}
	if (aNames.StandardName() !=  aLocalizedTimeZone.StandardName())
		{
		return EFalse; 
		}
	if (aNames.ShortStandardName() != aLocalizedTimeZone.ShortStandardName())
		{
		return EFalse;
		}
	if (aNames.DaylightSaveName() != aLocalizedTimeZone.DaylightName())
		{
		return EFalse;
		}
	if (aNames.ShortDaylightSaveName() != aLocalizedTimeZone.ShortDaylightName())
		{
		return EFalse;
		}
	
	return ETrue;
	}


TBool CTzLocalizationUserDataSuite::CompareLocalizedCity(TInt aTimeZoneId, const CTzUserNames& aNames, const CTzLocalizedCity& aCity)
	{	
	if (aNames.CityName() != aCity.Name())
		{
		return EFalse;
		}
	if (aTimeZoneId != aCity.TimeZoneId())
		{
		return EFalse;
		}
	if (aCity.GroupId()!=0)
		{
		return EFalse;
		}
		
	return ETrue;
	}


TBool CTzLocalizationUserDataSuite::CompareLocalizedCityGroup(const CTzUserNames& aNames, const CTzLocalizedCityGroup& aCityGroup)
	{	
	if (aNames.RegionName() != aCityGroup.Name())
		{
		return EFalse;
		}
	if (aCityGroup.Id()!=0)
		{
		return EFalse;
		}
		
	return ETrue;
	}

	
TBool CTzLocalizationUserDataSuite::CompareLocalizedCity(const CTzLocalizedCity& aCity, const CTzLocalizedCity& aCityToCompare)
	{
	if(aCity.TimeZoneId() != aCityToCompare.TimeZoneId() )
		{
		return EFalse;
		}
	if(aCity.GroupId() != aCityToCompare.GroupId())
		{
		return EFalse;
		}
	if(aCity.Name() != aCityToCompare.Name())
		{
		return EFalse;
		}
	return ETrue;
	}


TBool CTzLocalizationUserDataSuite::CompareTimeZoneName(const CTzLocalizedTimeZone& aTimeZone, const CTzLocalizedTimeZone& aTimeZoneToCompare)
	{
	if(aTimeZone.StandardName() != aTimeZoneToCompare.StandardName())
		{
		return EFalse;
		}
	if(aTimeZone.ShortStandardName() != aTimeZoneToCompare.ShortStandardName())
		{
		return EFalse;
		}
	if(aTimeZone.DaylightName() != aTimeZoneToCompare.DaylightName())
		{
		return EFalse;
		}
	if(aTimeZone.ShortDaylightName() != aTimeZoneToCompare.ShortDaylightName())
		{
		return EFalse;
		}
	return ETrue;	
	}
	
	
CTzId* CTzLocalizationUserDataSuite::CreateTimeZoneL()
	{	
	//Create a user defined rule - CTzRules.
	TTime dston (TDateTime(2005, EMarch, 0, 0, 0, 0, 0)); 
	TTime dstoff (TDateTime(2005, ESeptember, 0, 0, 0, 0, 0));
	TMonth monthOn = dston.DateTime().Month();
	TInt dayOn = dston.DateTime().Day();
	TMonth monthOff = dstoff.DateTime().Month();
	TInt dayOff = dstoff.DateTime().Day();
	
	TTzRule dstOff(0, 9999, 0, 0, monthOn,  ETzFixedDate, dayOn,  0, ETzWallTimeReference, 0);
	TTzRule dstOn (0, 9999, 0, 0, monthOff, ETzFixedDate, dayOff, 0, ETzWallTimeReference, 0);

	CTzRules* rules = CTzRules::NewL(0, KMaxTUint);
	CleanupStack::PushL(rules);
	rules->AddRuleL(dstOff);
	rules->AddRuleL(dstOn);
	
	//Create a user defined rule names- CTzUserNames.
	_LIT(KStandardName, "UserTzName");
	_LIT(KShortName, "UserTzSh");
	_LIT(KDaylightSaveName, "UserDLSaveName");
	_LIT(KShortDaylightSaveName, "UserShDLS");
	_LIT(KCityNameName, "UserCityName");
	_LIT(KRegionName, "UserRegionName");
	
	CTzUserNames* names = CTzUserNames::NewLC(KStandardName(), KShortName(), KDaylightSaveName(), KShortDaylightSaveName(), KCityNameName(), KRegionName());
	//Add the rule to the database
	CTzId* tzid = iUserData->CreateL(*rules, *names);
	CleanupStack::PopAndDestroy(2, rules);
	return tzid;
	}


void CTzLocalizationUserDataSuite::ResetTzUserDataL()
	{
	RPointerArray<CTzId> ids;
	CleanupStack::PushL(TCleanupItem(ResetAndDestroyTzIdArray, &ids));
	iUserData->GetTzIdsL(ids);
	TInt count = ids.Count();
	for (TInt ii=0; ii<count; ++ii)
		{
		iUserData->DeleteL(*ids[ii]);	
		}
	CleanupStack::PopAndDestroy(&ids);	
	}


TBool CTzLocalizationUserDataSuite::FindCityInArrayL(const CTzLocalizedCityArray& aCityArray, TInt aTimeZoneId, const CTzUserNames& aNames)
	{
	TBool found = EFalse;
	const TInt count = aCityArray.Count();
	for(TInt ii=0; ii<count; ++ii)
		{
		if(CompareLocalizedCity(aTimeZoneId, aNames, aCityArray.At(ii)))
			{
			found = ETrue;
			break;
			}
		}
	return found;
	}


void CTzLocalizationUserDataSuite::TestFindTimeZoneByNameL(TInt aTimeZoneId, const CTzUserNames& aNames,const TDesC& aNameToFind)
	{
	CTzLocalizedTimeZone* timeZone = iLocalizer->FindTimeZoneByNameL(aNameToFind);
	CleanupStack::PushL(timeZone);
	test(CompareTimeZoneNames(aTimeZoneId, aNames, *timeZone));
	CleanupStack::PopAndDestroy(timeZone);
	}


/**
@SYMTestCaseID	PIM-APPSERV-TZL-TZN-0001
	
@SYMTestCaseDesc
	The purpose of this test is to verify that the localized time zone names for
	a user defined time zone is correct.
	
@SYMTestActions     
	1.	Create a user defined zone and store the returned time zone identifier.
	2.	Get the localized time zone information for the user defined time zone
		identifier obtained in action 1. 

@SYMTestExpectedResults
	The following information is the same as that used to create the user
	defined time zone in action 1:

	- Standard time zone name
 	- Short standard time zone name
 	- Standard daylight saving time name
 	- Short daylight saving time name

@SYMTestType: 	CT
	
@SYMTestPriority: 1
*/



void CTzLocalizationUserDataSuite::TestLocalizedTimeZoneL()
	{
	test.Next(_L("@SYMTestCaseID PIM-APPSERV-TZL-TZN-0001 Test Localized Time Zone"));
	TInt tzid = 0;
	CTzUserNames* names = CreateUserRuleAndGetNamesLC(tzid);
	CTzLocalizedTimeZone* locTimezone = iLocalizer->GetLocalizedTimeZoneL(tzid);
	CleanupStack::PushL(locTimezone);
	test(CompareTimeZoneNames(tzid, *names, *locTimezone));
	CleanupStack::PopAndDestroy(2, names);
	ResetTzUserDataL();
	}


	
void CTzLocalizationUserDataSuite::TestLocalizedTimeZoneWithLocalizeIDL()
	{
	test.Next(_L("Test Localized Time Zone"));
	TInt tzid = 0;
	CTzUserNames* names = CreateUserRuleAndGetNamesLC(tzid);
	CTzLocalizedCity* city = iLocalizer->GetDefaultCityL(tzid);
	CleanupStack::PushL(city);
	CTzLocalizedTimeZone* locTimezone = iLocalizer->GetLocalizedTimeZoneL(*city);
	CleanupStack::PopAndDestroy(city);
	CleanupStack::PushL(locTimezone);
	test(CompareTimeZoneNames(tzid, *names, *locTimezone));
	CleanupStack::PopAndDestroy(2, names);
	ResetTzUserDataL();
	}


/**
@SYMTestCaseID PIM-APPSERV-TZL-TZN-0002
	
@SYMTestCaseDesc
	The purpose of this test is to verify that the localized city information
	for a user defined time zone is correct.

@SYMTestActions     
	1.	Create a user defined zone and store the returned time zone identifier.
	2.	Get the localized city information for the user defined zone identifier
		obtained in action 1. 

@SYMTestExpectedResults
	The following information is the same as that used to create the user
	defined time zone in action 1:

	- City name
 	- Time zone identifier
 	- Group identifier (0 for all user defined time zones)
 
@SYMTestType
	CT
	
@SYMTestPriority
	1
*/


void CTzLocalizationUserDataSuite::TestLocalizedCityL()
	{
	test.Next(_L("@SYMTestCaseID PIM-APPSERV-TZL-TZN-0002 Test Localized City"));
	TInt tzid = 0;
	CTzUserNames* names = CreateUserRuleAndGetNamesLC(tzid);
	CTzLocalizedCityArray* locCityArray = iLocalizer->GetCitiesL(tzid);
	CleanupStack::PushL(locCityArray);
	test(CompareLocalizedCity(tzid, *names, locCityArray->At(0)));
	CleanupStack::PopAndDestroy(2, names);
	ResetTzUserDataL();
	}


/**
@SYMTestCaseID PIM-APPSERV-TZL-TZN-0003
	
@SYMTestCaseDesc
	The purpose of this test is to verify that the default localized city
	information for a user defined time zone is correct.
	
@SYMTestActions     
	1.	Create a user defined zone and store the returned time zone identifier.
	2.	Get the default localized city information for the user defined zone
		identifier obtained in action 1. 

@SYMTestExpectedResults
	The following information is the same as that used to create the user
	defined time zone in action 1:

	 - City name
 	- Time zone identifier
 	- Group identifier (0 for all user defined time zones)
 
@SYMTestType
	CT

@SYMTestPriority
	1
*/


void CTzLocalizationUserDataSuite::TestDefaultCityL()
	{
	test.Next(_L("@SYMTestCaseID PIM-APPSERV-TZL-TZN-0003  Test Default City"));
	TInt tzid = 0;
	CTzUserNames* names = CreateUserRuleAndGetNamesLC(tzid);
	CTzLocalizedCity*  defaultCity = iLocalizer->GetDefaultCityL(tzid);
	CleanupStack::PushL(defaultCity);
	test(CompareLocalizedCity(tzid, *names, *defaultCity));
	CleanupStack::PopAndDestroy(2, names);
	ResetTzUserDataL();
	}


/**
@SYMTestCaseID PIM-APPSERV-TZL-TZN-0004
	
@SYMTestCaseDesc
	The purpose of this test is to verify that the localized city group
	information for a user defined time zone fails as expected.

@SYMTestActions     
	1.	Create a user defined zone and store the returned time zone identifier.
	2.	Verify that the localized city group information for a user defined time
		zone fails as expected. 

@SYMTestExpectedResults
	The localized city group information cannot be obtained (the call leaves
	with KErrArgument).
 
@SYMTestType
	CT
	
@SYMTestPriority
	1
*/


void CTzLocalizationUserDataSuite::TestCityGroupL()
	{
	test.Next(_L("@SYMTestCaseID PIM-APPSERV-TZL-TZN-0004 Test City Group"));
	TInt tzid = 0;
	CTzUserNames* names = CreateUserRuleAndGetNamesLC(tzid);
	CTzLocalizedCityArray* locCityArray = iLocalizer->GetCitiesL(tzid);
	CleanupStack::PushL(locCityArray);
	TRAPD(err, iLocalizer->GetCityGroupL(locCityArray->At(0).GroupId())); 
	test(err == KErrArgument);
	CleanupStack::PopAndDestroy(2, names);
	ResetTzUserDataL();
	}


/**
@SYMTestCaseID 	PIM-APPSERV-TZL-TZN-0005
	
@SYMTestCaseDesc
	The purpose of this test is to verify that the localized city array
	information for user defined time zones is correct.
	
@SYMTestActions     
	1.	Create two user defined time zones.
	2.	Get the localized city array information for the user defined time zone
		group identifier (always 0).

@SYMTestExpectedResults
	The localized city array contains the two city names used to create the user
	defined time zones in action 1.
 
@SYMTestType
	CT

@SYMTestPriority
	1
*/


void CTzLocalizationUserDataSuite::TestCityArrayL()
	{
	test.Next(_L("@SYMTestCaseID PIM-APPSERV-TZL-TZN-0005 Test City Array"));

	TInt grouplessId = 0;
	CTzLocalizedCityArray* cities = iLocalizer->GetCitiesInGroupL(grouplessId);
	const TInt originalCount = cities->Count();
	delete cities;

	TInt tzid1 = 0;
	CTzUserNames* names1 = CreateUserRuleAndGetNamesLC(tzid1);

	TInt tzid2 = 0;
	CTzUserNames* names2 = CreateUserRuleAndGetNamesLC(tzid2);
	
	cities = iLocalizer->GetCitiesInGroupL(grouplessId);
	CleanupStack::PushL(cities);

	test(cities->Count() == (2 + originalCount));
	test(FindCityInArrayL(*cities, tzid1, *names1));
	test(FindCityInArrayL(*cities, tzid1, *names2));
	
	CleanupStack::PopAndDestroy(3);
	}


/**
@SYMTestCaseID PIM-APPSERV-TZL-TZN-0006
	
@SYMTestCaseDesc
	Verify that the current time zone can be set to a user defined time zone.

@SYMTestActions     
	1.	Create a user defined time zone and store the returned time zone
		identifier.
	2.	Set the current time using the user defined time zone identifier stored
		in action 1.
	3.	Get the time zone identifier for the current time zone from the TZ
		Server.

@SYMTestExpectedResults
	The time zone identifier obtained in action 1 is the same as the time zone
	identifier obtained in action 3.
 
@SYMTestType
	CT

@SYMTestPriority
	1
*/


void CTzLocalizationUserDataSuite::TestCurrentTimeZoneL()
	{
	test.Next(_L("@SYMTestCaseID PIM-APPSERV-TZL-TZN-0006 Test Current Time Zone"));
	TInt tzid = 0;
	CTzUserNames* names = CreateUserRuleAndGetNamesLC(tzid);
	iLocalizer->SetTimeZoneL(tzid);
	CTzLocalizedTimeZone* currentTz = iLocalizer->GetFrequentlyUsedZoneL(CTzLocalizedTimeZone::ECurrentZone);
	CleanupStack::PushL(currentTz);
	test(CompareTimeZoneNames(tzid, *names, *currentTz));
	CleanupStack::PopAndDestroy(2, names);
	ResetTzUserDataL();		
	}


/**
@SYMTestCaseID PIM-APPSERV-TZL-TZN-0010
	
@SYMTestCaseDesc
	The purpose of this test is to verify that time zone name, city name and
	city group name for a user defined time zone can be found.
	
@SYMTestActions     
	1.	Create a user defined time zone and store the returned time zone
		identifier.
	2.	Find the user defined time zone by long/short standard time name and
		long short daylight saving time names. [1]
	3.	Find the user defined time zone by city name, searching both in all time
		zones and the specific time zone identified by the time zone identifier
		obtained in action 1. [2]
	4.	Find the user defined time zone by city group name. [3]

@SYMTestExpectedResults
	[1], [2], [3] User defined time zone is found
 
@SYMTestType
	CT
	
@SYMTestPriority
	1
*/


void CTzLocalizationUserDataSuite::TestFindTimeZoneL()
	{
	test.Next(_L("@SYMTestCaseID PIM-APPSERV-TZL-TZN-0010 Test Find Time Zone"));
	TInt tzid = 0;
	CTzUserNames* names = CreateUserRuleAndGetNamesLC(tzid);
	TestFindTimeZoneByNameL(tzid,*names,names->StandardName());
	TestFindTimeZoneByNameL(tzid,*names,names->ShortStandardName());
	TestFindTimeZoneByNameL(tzid,*names,names->DaylightSaveName());
	TestFindTimeZoneByNameL(tzid,*names,names->ShortDaylightSaveName());
	CTzLocalizedCity* city = iLocalizer->FindCityByNameL(names->CityName());
	CleanupStack::PushL(city);
	test(CompareLocalizedCity(tzid, *names, *city));
	CleanupStack::PopAndDestroy(city);
	
	city = iLocalizer->FindCityByNameL(names->CityName(), tzid);
	CleanupStack::PushL(city);
	test(CompareLocalizedCity(tzid, *names, *city));
	CleanupStack::PopAndDestroy(city);
	
	CTzLocalizedCityGroup* group = iLocalizer->FindCityGroupByNameL(names->RegionName());
	CleanupStack::PushL(group);
	test(CompareLocalizedCityGroup(*names, *group));
	CleanupStack::PopAndDestroy(group);
	
	CleanupStack::PopAndDestroy(names);
	}


CTzRules* CTzLocalizationUserDataSuite::CreateRulesLC()
	{
	TTime dston (TDateTime(2005, EMarch, 0, 0, 0, 0, 0)); 
	TTime dstoff (TDateTime(2005, ESeptember, 0, 0, 0, 0, 0));
	TMonth monthOn = dston.DateTime().Month();
	TInt dayOn = dston.DateTime().Day();
	TMonth monthOff = dstoff.DateTime().Month();
	TInt dayOff = dstoff.DateTime().Day();
	
	TTzRule dstOff(0, 9999, 0, 0, monthOn,  ETzFixedDate, dayOn,  0, ETzWallTimeReference, 0);
	TTzRule dstOn (0, 9999, 0, 0, monthOff, ETzFixedDate, dayOff, 0, ETzWallTimeReference, 0);

	CTzRules* rules = CTzRules::NewL(0, KMaxTUint);
	CleanupStack::PushL(rules);
	rules->AddRuleL(dstOff);
	rules->AddRuleL(dstOn);
	
	return rules;
	}


void CTzLocalizationUserDataSuite::CreateString(TDes& aString)
	{
	for (TInt i(0) ; i < 10 ; ++i)
		{
		// Add random upper and lower case letters
		aString.Append( 'A' + (Math::Rand(iSeed) % 26) );
		}
	}

	
CTzUserNames* CTzLocalizationUserDataSuite::CreateNamesLC()
	{	
	TBuf<10> stdName;
	CreateString(stdName);
	TBuf<10> shortStdName;
	CreateString(shortStdName);
	TBuf<10> dstName;
	CreateString(dstName);
	TBuf<10> shortDstName;
	CreateString(shortDstName);
	TBuf<10> cityName;
	CreateString(cityName);
	TBuf<10> regionName;
	CreateString(regionName);

	return CTzUserNames::NewLC(stdName, shortStdName, dstName, shortDstName, cityName, regionName);
	}


/**
@SYMTestCaseID  PIM-APPSERV-TLOCALIZATIONUSERDATA-0002
		
@SYMTestCaseDesc
	The purpose of this test is to verify that an update to user-defined rules
	is reflected in the frequently used zone database.
	
@SYMTestActions		
	1.  Create a user-defined time zone and set it as the frequently used zone.
	2.  Update the time zone rule.
	3.  Read the frequently used city and zone and test against the ones
		expected.

@SYMTestExpectedResults
	The frequently used zone read from the database should be the one expected.

@SYMTestType
	CT

@SYMTestPriority
	1
*/


void CTzLocalizationUserDataSuite::TestUserDataModificationL(CTzLocalizedTimeZone::TTzFrequentlyUsedZone aFrequentlyUsedZone)
	{
	test.Next(_L("@SYMTestCaseID PIM-APPSERV-TLOCALIZATIONUSERDATA-0002 Test frequently used zone database after user data modification"));

	//Create user-defined time zone
	CTzRules* rules = CreateRulesLC();
	CTzUserNames* names = CreateNamesLC();
	CTzId* tzId = iUserData->CreateL(*rules, *names);
	TInt tzid = tzId->TimeZoneNumericID();
	delete tzId;
	CleanupStack::PopAndDestroy(names);

	// Associate the user-defined zone with the given frequently used zone.
	iLocalizer->SetFrequentlyUsedZoneL(tzid, aFrequentlyUsedZone);
		
	//Create a new user-defined name
	CTzUserNames* names2 = CreateNamesLC();
	
	//Update the time zone
	CTzId* id = CTzId::NewL(tzid);
	CleanupStack::PushL(id);
	iUserData->UpdateL(*id, *rules, *names2);
	
	switch(aFrequentlyUsedZone)
		{
	case CTzLocalizedTimeZone::EHomeZone:
	case CTzLocalizedTimeZone::EInterestZone:
		{
		CTzLocalizedCity* city = iLocalizer->GetFrequentlyUsedZoneCityL(aFrequentlyUsedZone);
		test(CompareLocalizedCity(tzid, *names2, *city));
		delete city;
		
		CTzLocalizedTimeZone* timeZone = iLocalizer->GetFrequentlyUsedZoneL(aFrequentlyUsedZone);
		test(CompareTimeZoneNames(tzid, *names2, *timeZone));
		delete timeZone;
		break;	
		}
		
	case CTzLocalizedTimeZone::ECurrentZone:
		{
		//Extract zone information, same zone should be returned by GetFrequentlyUsedZoneL below.
		CTzLocalizedTimeZone* zoneToCompareWith = iLocalizer->GetLocalizedTimeZoneL(KParisTzId);
		CleanupStack::PushL(zoneToCompareWith);
		//Extract zone information, same city should be returned by GetFrequentlyUsedZoneCityL below.
		CTzLocalizedCity* cityToCompareWith = iLocalizer->GetDefaultCityL(KParisTzId);
		CleanupStack::PushL(cityToCompareWith);
		
		iLocalizer->SetTimeZoneL(KParisTzId);
		CTzLocalizedCity* freqUsedCity = iLocalizer->GetFrequentlyUsedZoneCityL(aFrequentlyUsedZone);
		test(CompareLocalizedCity(*freqUsedCity, *cityToCompareWith));
		delete freqUsedCity;
		
		CTzLocalizedTimeZone* freqUsedZone = iLocalizer->GetFrequentlyUsedZoneL(aFrequentlyUsedZone);
		test(CompareTimeZoneName(*freqUsedZone, *zoneToCompareWith));
		delete freqUsedZone;
		CleanupStack::PopAndDestroy(2,zoneToCompareWith);
		break;
		}
	default:
		{
		User::Invariant();
		break;
		}
		
		}
		
	CleanupStack::PopAndDestroy(3, rules);
	ResetTzUserDataL();
	}


/**
@SYMTestCaseID  PIM-APPSERV-TLOCALIZATIONUSERDATA-0003
		
@SYMTestCaseDesc
	The purpose of this test is to verify that a deletion to the user-defined 
	rules is reflected in the frequently used zone database.
	
@SYMTestActions
	1.  Create a user-defined time zone and set it as the frequently used zone of
		interest.
	2.  Create another user-defined time zone and set it as the frequently used
		home zone.
	3.  Delete the time zone created in Step 2.
	4.  Read the frequently used city and zone and test whether against the ones
		expected.

@SYMTestExpectedResults
	The frequently used city and zone read from the database should be the ones
	expected.

@SYMTestType
	CT

@SYMTestPriority
	1
*/


void CTzLocalizationUserDataSuite::TestUserDataDeletionL()
	{
	test.Next(_L("@SYMTestCaseID PIM-APPSERV-TLOCALIZATIONUSERDATA-0003 Test frequently used zone database after user data deletion"));
	// Add a user-defined time zone as a frequently used zone of interest	
	CTzRules* rulesInterest = CreateRulesLC();
	CTzUserNames* namesInterest = CreateNamesLC();
	CTzId* tzIdInterest = iUserData->CreateL(*rulesInterest, *namesInterest);
	TInt tzidInterest = tzIdInterest->TimeZoneNumericID();
	delete tzIdInterest;
	
	//Extract zone information, same city should be returned by GetFrequentlyUsedZoneCityL below.	
	CTzLocalizedCity* cityToCompareWith = iLocalizer->GetDefaultCityL(KLondonTzId);
	CleanupStack::PushL(cityToCompareWith);
	
	//Extract zone information, same zone should be returned by GetFrequentlyUsedZoneL below.
	CTzLocalizedTimeZone* zoneToCompareWith = iLocalizer->GetLocalizedTimeZoneL(KLondonTzId);
	CleanupStack::PushL(zoneToCompareWith);
	
	iLocalizer->SetFrequentlyUsedZoneL(tzidInterest, CTzLocalizedTimeZone::EInterestZone);
	
	CTzRules* rulesHome = CreateRulesLC();
	CTzUserNames* namesHome = CreateNamesLC();
	CTzId* tzId = iUserData->CreateL(*rulesHome, *namesHome);
	TInt tzidHome = tzId->TimeZoneNumericID();
	delete tzId;

	// Add rule to frequently used zones
	iLocalizer->SetFrequentlyUsedZoneL(tzidHome, CTzLocalizedTimeZone::EHomeZone);
		
	CTzId* id = CTzId::NewL(tzidHome);
	CleanupStack::PushL(id);
	iUserData->DeleteL(*id);
	CleanupStack::PopAndDestroy(id);
		
	CTzLocalizedCity* city = iLocalizer->GetFrequentlyUsedZoneCityL(CTzLocalizedTimeZone::EHomeZone);
	//Check if the frequently used city read from the database is same as the one expected
	test(CompareLocalizedCity(*city, *cityToCompareWith));
	delete city;
	
	CTzLocalizedCity* cityInterest = iLocalizer->GetFrequentlyUsedZoneCityL(CTzLocalizedTimeZone::EInterestZone);
	//Check if the frequently used city of interest read from the database is same as the one expected
	test(CompareLocalizedCity(tzidInterest, *namesInterest, *cityInterest));
	delete cityInterest;
		
	CTzLocalizedTimeZone* timeZone = iLocalizer->GetFrequentlyUsedZoneL(CTzLocalizedTimeZone::EHomeZone);
	//Check if the frequently used zone read from the database is same as the one expected
	test(CompareTimeZoneName(*timeZone, *zoneToCompareWith));
	delete timeZone;
	
	CTzLocalizedTimeZone* timeZoneInterest = iLocalizer->GetFrequentlyUsedZoneL(CTzLocalizedTimeZone::EInterestZone);
	//Check if the frequently used zone of interest read from the database is same as the one expected
	test(CompareTimeZoneNames(tzidInterest, *namesInterest, *timeZoneInterest));
	
	delete timeZoneInterest;
	ResetTzUserDataL();
	CleanupStack::PopAndDestroy(6, rulesInterest);
	}


/**
@SYMTestCaseID PIM-APPSERV-TLOCALIZATIONUSERDATA-0004
	
@SYMTestCaseDesc
	The purpose of this test is to verify that a deletion to the user-defined 
	rules is reflected in the city database		

@SYMTestActions
	1.  Create a user-defined time zone.
	2.  Add three cities to the time zone.
	3.  Read the number of cities.
	4.  Delete one of the cities.
	5.  Read the number of cities after deletion.

@SYMTestExpectedResults
	The number of cities in Step 3 should be 3 and in Step 5 should be 2.
	
@SYMTestType
	CT
	
@SYMTestPriority
	1
*/


void CTzLocalizationUserDataSuite::TestUserDataDeletionCityImpactL()
	{
	test.Next(_L("@SYMTestCaseID PIM-APPSERV-TLOCALIZATIONUSERDATA-0004 Test city database after user data deletion\n"));
	CTzRules* rules = CreateRulesLC();
	CTzUserNames* names = CreateNamesLC();
	CTzId* tzId = iUserData->CreateL(*rules, *names);
	CleanupStack::PopAndDestroy(names);
	CleanupStack::PopAndDestroy(rules);
	TInt tzid = tzId->TimeZoneNumericID();
	delete tzId;
	//Add 3 user-defined cities to the city database	
	test.Printf(_L("Add 3 cities to the city database\n"));
	CTzLocalizedCity* userCity = iLocalizer->AddCityL(tzid, KUserAddedCity1Name);
	delete userCity;
	CTzRules* rules2 = CreateRulesLC();
	CTzUserNames* names2 = CreateNamesLC();
	CTzId* tzId1 = iUserData->CreateL(*rules2, *names2);
	CleanupStack::PopAndDestroy(2,rules2);
	tzid = tzId1->TimeZoneNumericID();
	delete tzId1;
	CTzLocalizedCity* userCity1 = iLocalizer->AddCityL(tzid, KUserAddedCity2Name);
	delete userCity1;

	CTzRules* rules3 = CreateRulesLC();	
	CTzUserNames* names3 = CreateNamesLC();
	CTzId* tzId2 = iUserData->CreateL(*rules3, *names3);
	CleanupStack::PopAndDestroy(2,rules3);
	tzid = tzId2->TimeZoneNumericID();
	delete tzId2;

	CTzLocalizedCity* userCity2 = iLocalizer->AddCityL(tzid, KUserAddedCity3Name);
	delete userCity2;
	//Read the number of cities
	RPointerArray<CTzLocalizedCityRecord> cityRecords;
	iTzSession.LocalizationReadCitiesL(cityRecords);
	test.Printf(_L("Check if the number of cities read is correct\n"));
	test(cityRecords.Count() == 3);
	cityRecords.ResetAndDestroy();
	cityRecords.Close();
	//Delete a city from the city database
	CTzId* id = CTzId::NewL(tzid);
	CleanupStack::PushL(id);
	test.Printf(_L("Delete a city from the database\n"));
	iUserData->DeleteL(*id);
	CleanupStack::PopAndDestroy(id);
	//Read the number of cities again
	RPointerArray<CTzLocalizedCityRecord> cityRecords2;
	iTzSession.LocalizationReadCitiesL(cityRecords2);
	test.Printf(_L("Again, check if the number of cities read is correct\n"));
	test(cityRecords2.Count() == 2);
	cityRecords2.ResetAndDestroy();
	cityRecords2.Close();
		
	ResetTzUserDataL();
	}


TBool CTzLocalizationUserDataSuite::TestTimeZoneShortStandardNameL(CTzLocalizer& aTzLocalizer, const TDesC& aShortStandardName)
	{
	CTzLocalizedTimeZone* localizedTimeZone = aTzLocalizer.GetFrequentlyUsedZoneL(CTzLocalizedTimeZone::ECurrentZone);
	CleanupStack::PushL(localizedTimeZone);
	HBufC* shortStandardName(localizedTimeZone->ShortStandardName().AllocLC());
	test.Printf(_L("The short standard name is %S\n"), shortStandardName);
	TBool compare = (shortStandardName->Des() == aShortStandardName);
	CleanupStack::PopAndDestroy(shortStandardName);
	CleanupStack::PopAndDestroy(localizedTimeZone);
	return compare;
	}



void CTzLocalizationUserDataSuite::TestUserDefinedTimeZoneL()
	{
	test.Next(_L("Test user defined zone\n"));
	_LIT(KUserStandardName, "Southark Standard Time");
	_LIT(KUserShortStandardName, "SST");
	_LIT(KUserDaylightName, "WorkingDay");
	_LIT(KUserShortDaylightName, "WD");
	_LIT(KRegionName, "Sarf");
	_LIT(KCityName, "MobiCity");
	_LIT(KCity2Name, "East end");
	CTzRules* rules = CreateRulesLC();
	CTzUserNames* names = CTzUserNames::NewLC(KUserStandardName(), KUserShortStandardName(), KUserDaylightName(), KUserShortDaylightName(), KCityName(), KRegionName() );
	CTzId* tzId = iUserData->CreateL(*rules, *names);
	CleanupStack::PopAndDestroy(names);
	CleanupStack::PopAndDestroy(rules);
	TInt tzid = tzId->TimeZoneNumericID();

	RTz tz;
	CleanupClosePushL(tz);
	User::LeaveIfError(tz.Connect());
	//Create a localizer and get frequently used zone
	CTzLocalizer* localizer = CTzLocalizer::NewLC();
	localizer->SetTimeZoneL(tzid);

    //use the utility test function here for the short standard name
	test (TestTimeZoneShortStandardNameL(*localizer, KUserShortStandardName()));

	CTzLocalizedCity* localizedCity = localizer->GetFrequentlyUsedZoneCityL(CTzLocalizedTimeZone::ECurrentZone);
	CleanupStack::PushL(localizedCity);
	test(localizedCity->Name() == KCityName());
	test(localizedCity->TimeZoneId() ==  (TUint16) tzid);
	CleanupStack::PopAndDestroy(localizedCity);

	//
	// Test the AddCityL() and RemoveCityL() APIs.
	//

	// Try adding the same city.
	TRAPD(errSame, localizer->AddCityL(tzid, KCityName, 0));
	test(errSame == KErrAlreadyExists);

	// Try adding and removing a different city.
	CTzLocalizedCity* differentCity = NULL;
	TRAPD(errDifferent, differentCity = localizer->AddCityL(tzid, KCity2Name, 0));
	test(errDifferent == KErrNone);
	TRAPD(errDelete, localizer->RemoveCityL(differentCity));
	test(errDelete == KErrNone);

    //Get the CTzLocalizedCity and keep it to use it later on.
	CTzLocalizedCity* localizedGetCity = localizer->GetFrequentlyUsedZoneCityL(CTzLocalizedTimeZone::ECurrentZone);
	CleanupStack::PushL(localizedGetCity);
	test(localizedGetCity->Name() == KCityName());

    //Set the time zone to London using a tzId.
	localizer->SetFrequentlyUsedZoneL(KLondonTzId, CTzLocalizedTimeZone::ECurrentZone);
	//Test the short standard name for compliance
    test (TestTimeZoneShortStandardNameL(*localizer, _L("GMT")));

    // Set the frequently used zone using the previously retrieved CTzLocalizedCity of Southwark.
	localizer->SetFrequentlyUsedZoneL(*localizedGetCity, CTzLocalizedTimeZone::ECurrentZone);
	CleanupStack::PopAndDestroy(localizedGetCity);

    //Get the frequently used zone and check that short standard name is as expected.
	test (TestTimeZoneShortStandardNameL(*localizer, KUserShortStandardName()));

    //Set the frequently used time zone back to london, this time using the CTzLocalizedTimeZone API
	CTzLocalizedTimeZone* londonLocalizedTimeZone = localizer->GetLocalizedTimeZoneL(KLondonTzId);
	CleanupStack::PushL(londonLocalizedTimeZone);
	localizer->SetFrequentlyUsedZoneL(*londonLocalizedTimeZone, CTzLocalizedTimeZone::ECurrentZone);
    //Test that the short standard name is again GMT
	test (TestTimeZoneShortStandardNameL(*localizer, _L("GMT")));
	CleanupStack::PopAndDestroy(londonLocalizedTimeZone);

	CleanupStack::PopAndDestroy(localizer);
	CleanupStack::PopAndDestroy(&tz);
	delete tzId;
	}

/**
@SYMTestCaseID PIM-APPSERV-TLOCALIZATIONUSERDATA-0001
*/	
static void DoTestsL()
	{
	test.Title();

	test.Start(_L("@SYMTestCaseID PIM-APPSERV-TLOCALIZATIONUSERDATA-0001 User defined time zone test suite"));

	RPIMTestServer serv;	
	CleanupClosePushL(serv);
	User::LeaveIfError(serv.Connect());
	serv.CloseTzSession();
	_LIT(KAgendaServer,"agsvexe*");
	TRAP_IGNORE(serv.KillProcessL(KAgendaServer));
	_LIT(KTzServer,"Tzserver*");
	TRAP_IGNORE(serv.KillProcessL(KTzServer));
	serv.DeleteFileL(KPerFile1);
	serv.DeleteFileL(KPerFile2);

	CTzLocalizationUserDataSuite* loctest = CTzLocalizationUserDataSuite::NewLC();
	loctest->TestUserDefinedTimeZoneL();
	loctest->TestLocalizedTimeZoneL();
	loctest->TestLocalizedTimeZoneWithLocalizeIDL();
	loctest->TestLocalizedCityL();
	loctest->TestDefaultCityL();
	loctest->TestCityGroupL();
	loctest->TestCityArrayL();
	loctest->TestCurrentTimeZoneL();
	loctest->TestFindTimeZoneL();
	loctest->TestUserDataModificationL(CTzLocalizedTimeZone::EHomeZone);
	loctest->TestUserDataModificationL(CTzLocalizedTimeZone::ECurrentZone);
	loctest->TestUserDataModificationL(CTzLocalizedTimeZone::EInterestZone);
	loctest->TestUserDataDeletionL();
	loctest->TestUserDataDeletionCityImpactL();
	CleanupStack::PopAndDestroy(loctest);
	
	TRAP_IGNORE(serv.KillProcessL(KTzServer));

	//delete persistent file
	serv.DeleteFileL(KPerFile1);
	serv.DeleteFileL(KPerFile2);
	
	CleanupStack::PopAndDestroy(&serv);

	test.End();
	}


TInt E32Main()
    {
	__UHEAP_MARK;

	CTrapCleanup* trapCleanup = CTrapCleanup::New();
	if(!trapCleanup)
		{
		return KErrNoMemory;
		}

	CActiveScheduler* scheduler = new CActiveScheduler;
	if(!scheduler)
		{
		return KErrNoMemory;
		}
	CActiveScheduler::Install(scheduler);	

	TRAPD(ret, DoTestsL());
	test.Printf(_L("Trapped return value from DoTestsL(): %d\n"), ret);
	test(ret == KErrNone);
	
	test.Close();

	delete scheduler;
	delete trapCleanup;	

	__UHEAP_MARKEND;

	return (KErrNone);
    }
