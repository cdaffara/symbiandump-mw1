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


#include <e32base.h>
#include <e32std.h>
#include <e32des16.h>
#include <e32test.h>
#include <tzlocalizer.h> 
#include <tzlocalizedcityrecord.h>
#include "testserver.h"
#include <f32file.h>

_LIT(KDefaultLocale, "elocl.loc");
#ifdef SYMBIAN_DISTINCT_LOCALE_MODEL
_LIT(KFrenchLocale, "elocl_lan.002");
#else
_LIT(KFrenchLocale, "elocl.02");
#endif
const TInt KDefaultTzId = 2592;
const TInt KDefaultCityGroupId = 1;
const TInt KParisTzId = 2656;

_LIT(KSrcTzLocRscFile1, "z:\\timezonelocalization\\timezonegroups.r01");
_LIT(KSrcTzLocRscFile2, "z:\\timezonelocalization\\timezonegroups.r02");
_LIT(KSrcTzLocRscFile3, "z:\\timezonelocalization\\timezonegroups.rsc");
_LIT(KSrcTzLocRscFile4, "z:\\timezonelocalization\\timezones.r01");
_LIT(KSrcTzLocRscFile5, "z:\\timezonelocalization\\timezones.r02");
_LIT(KSrcTzLocRscFile6, "z:\\timezonelocalization\\timezones.rsc");

_LIT(KDestTzLocRscDir, "c:\\resource\\timezonelocalization\\");
_LIT(KDestTzLocRscFile1, "c:\\resource\\timezonelocalization\\timezonegroups.r01");
_LIT(KDestTzLocRscFile2, "c:\\resource\\timezonelocalization\\timezonegroups.r02");
_LIT(KDestTzLocRscFile3, "c:\\resource\\timezonelocalization\\timezonegroups.rsc");
_LIT(KDestTzLocRscFile4, "c:\\resource\\timezonelocalization\\timezones.r01");
_LIT(KDestTzLocRscFile5, "c:\\resource\\timezonelocalization\\timezones.r02");
_LIT(KDestTzLocRscFile6, "c:\\resource\\timezonelocalization\\timezones.rsc");

_LIT(KTzLocTemporaryFile,"c:\\private\\100012a5\\DBS_2000836D_DBS_DSC.db");
_LIT(KTzLocDb,"c:\\private\\100012a5\\DBS_10206A8B_TzLocalization.db");

_LIT(KSrcTzLocModifiedDir, "z:\\timezonelocalization\\modified");

_LIT(KSrcTzLocLocaleModifiedDir, "z:\\timezonelocalization\\locale\\");

_LIT(KAgendaServer,"agsvexe*");
_LIT(KTzServer,"Tzserver*");

RTest test(_L("TZ Localization Resource Files Update Test Suite"));


LOCAL_C void CopyFilesL(RPIMTestServer& aTestServer)
	{
	aTestServer.CopyFileL(KSrcTzLocRscFile1,KDestTzLocRscFile1);
	aTestServer.CopyFileL(KSrcTzLocRscFile2,KDestTzLocRscFile2);
	aTestServer.CopyFileL(KSrcTzLocRscFile3,KDestTzLocRscFile3);
	aTestServer.CopyFileL(KSrcTzLocRscFile4,KDestTzLocRscFile4);
	aTestServer.CopyFileL(KSrcTzLocRscFile5,KDestTzLocRscFile5);
	aTestServer.CopyFileL(KSrcTzLocRscFile6,KDestTzLocRscFile6);
	}


LOCAL_C void DeleteFilesL(RPIMTestServer& aTestServer)
	{
	aTestServer.DeleteFileL(KDestTzLocRscFile1);
	aTestServer.DeleteFileL(KDestTzLocRscFile2);
	aTestServer.DeleteFileL(KDestTzLocRscFile3);
	aTestServer.DeleteFileL(KDestTzLocRscFile4);
	aTestServer.DeleteFileL(KDestTzLocRscFile5);
	aTestServer.DeleteFileL(KDestTzLocRscFile6);
	aTestServer.DeleteDirL(KDestTzLocRscDir);
	}

LOCAL_C void DeleteEnglishFilesL(RPIMTestServer& aTestServer)
	{
	aTestServer.DeleteFileL(KDestTzLocRscFile1);
	aTestServer.DeleteFileL(KDestTzLocRscFile3);
	aTestServer.DeleteFileL(KDestTzLocRscFile4);
	aTestServer.DeleteFileL(KDestTzLocRscFile6);
	aTestServer.DeleteDirL(KDestTzLocRscDir);
	}

/**
@SYMTestCaseID	PIM-APPSERV-LOC-TZL-ECL-0001

@SYMTestCaseDesc
	The purpose of this test is to verify the use of eclipsed system TZ
	localization resources.
	
@SYMTestActions
	1.	Use the CTzLocalizer API to obtain a copy of the cities
		(preUpdateCities) for a given city group identifier.

	2.	Copy system TZ localization resources that adds a city to the city group
		used in action 1 to the appropriate location on the C: drive.

	3.	Use the CTzLocalizer API to obtain a copy of the cities
		(postUpdateCities) for the same city group identifier used in action 1.

@SYMTestExpectedResults
	The preUpdateCities object should not contain the added city and the
	postUpdateCities object should contain the added city.

@SYMREQ
	REQ9952

@SYMTestType
	CT

@SYMTestPriority
	1
*/
LOCAL_C void TestUpdatedLocalizationFilesL()
	{
	RPIMTestServer serv;	
	CleanupClosePushL(serv);
	User::LeaveIfError(serv.Connect());
	
	serv.CloseTzSession();
	TRAP_IGNORE(serv.KillProcessL(KAgendaServer));
	TRAP_IGNORE(serv.KillProcessL(KTzServer));
	
	DeleteFilesL(serv);
	serv.DeleteDirL(KDestTzLocRscDir);
	serv.DeleteFileL(KTzLocDb);
	
	CTzLocalizer* localizer = CTzLocalizer::NewLC();
	// Set timezone to be London.
	localizer->SetTimeZoneL(KDefaultTzId);		
	
	CTzLocalizedCityGroup* group = localizer->GetCityGroupL(KDefaultCityGroupId);
	CleanupStack::PushL(group);
	CTzLocalizedCityArray* cities = localizer->GetCitiesInGroupL(*group);
	CleanupStack::PushL(cities);

	TInt countDefault = cities->Count();
	test.Printf(_L("The number of cities found in a city group in the default files are %d\n"), countDefault);

	CleanupStack::PopAndDestroy(cities);
	CleanupStack::PopAndDestroy(group);
	CleanupStack::PopAndDestroy(localizer);	
	
	CopyFilesL(serv);
	
	CTzLocalizer* localizerFlash = CTzLocalizer::NewLC();
	localizerFlash->SetTimeZoneL(KDefaultTzId);
	
	CTzLocalizedCityGroup* groupFlash = localizerFlash->GetCityGroupL(KDefaultCityGroupId);
	CleanupStack::PushL(groupFlash);
	CTzLocalizedCityArray* citiesFlash = localizerFlash->GetCitiesInGroupL(*groupFlash);
	CleanupStack::PushL(citiesFlash);
	TInt countFlash = citiesFlash->Count();

	test.Printf(_L("The number of cities found in a city group in the files under system drive are %d\n"), countFlash);
	__ASSERT_ALWAYS(countDefault < countFlash, User::Panic(_L("Number of cities found in the file on ROM and flash is the same"), 1));
	
	CleanupStack::PopAndDestroy(citiesFlash);
	CleanupStack::PopAndDestroy(groupFlash);
	CleanupStack::PopAndDestroy(localizerFlash);

	DeleteFilesL(serv);
	serv.DeleteDirL(KDestTzLocRscDir);
	
	CleanupStack::PopAndDestroy(&serv);
	}

/**
@SYMTestCaseID PIM-localizationupdate-0001
     
@SYMTestCaseDesc 
   	The purpose of this test is to verify the use of eclipsed system
	TZ localization resources.  
	
@SYMTestActions 
    1. 	The current locale of the system is changed to French

	2. 	Fetch the number of cities in the group

	3. 	Files are copied to system drive

	4. 	Fetch the number of cities in the group

	5.	Delete the files on system drive

	6. 	Fetch the number of cities in the group
	
@SYMTestExpectedResults
    Number of cities in Step 2 and 6 is 2
     
	Number of cities in Step 4 is 52
	
@SYMREQ 
	REQ9952
@SYMTestType  
    CT
    
@SYMTestPriority            
	1
*/
LOCAL_C void TestEclipsingOfRscFileL()
	{
	_LIT(KLocaleChangeStart, "Changing locale to French\n");
	test.Printf(KLocaleChangeStart);

	TExtendedLocale extendedLocale;
	extendedLocale.LoadSystemSettings();
	TBuf<100> localeDllLocationString ;
	localeDllLocationString.Copy(KFrenchLocale);
	extendedLocale.LoadLocaleAspect(ELocaleLanguageSettings, localeDllLocationString) ;	
	extendedLocale.SaveSystemSettings();
	
	RPIMTestServer serv;
	CleanupClosePushL(serv);
	User::LeaveIfError(serv.Connect());
	
	serv.CloseTzSession();
	TRAP_IGNORE(serv.KillProcessL(KAgendaServer));
	TRAP_IGNORE(serv.KillProcessL(KTzServer));
	
	DeleteFilesL(serv);
	serv.DeleteDirL(KDestTzLocRscDir);
	serv.DeleteFileL(KTzLocDb);
	
	CTzLocalizer* localizer = CTzLocalizer::NewLC();
	// Set timezone to be London
	localizer->SetTimeZoneL(KDefaultTzId);	
		
	// Get the default city and cities within the group
	CTzLocalizedCityGroup* group = localizer->GetCityGroupL(KDefaultCityGroupId);
	CleanupStack::PushL(group);
	CTzLocalizedCityArray* cities = localizer->GetCitiesInGroupL(*group);
	CleanupStack::PushL(cities);
	
	// Get the number of cities
	TInt countDefault = cities->Count();
	test.Printf(_L("The number of cities found in a city group in the default files are %d\n"), countDefault);
	
	CleanupStack::PopAndDestroy(cities);
	CleanupStack::PopAndDestroy(group);
	CleanupStack::PopAndDestroy(localizer);	
	
	// Copy files on to system drive
	serv.CopyFileL(KSrcTzLocLocaleModifiedDir, KDestTzLocRscDir);
	
	// Get cities and city group 
	CTzLocalizer* localizerFlash = CTzLocalizer::NewLC();
	localizerFlash->SetTimeZoneL(KDefaultTzId);
	
	// Fetch cities
	CTzLocalizedCityGroup* groupFlash = localizerFlash->GetCityGroupL(KDefaultCityGroupId);
	CleanupStack::PushL(groupFlash);
	CTzLocalizedCityArray* citiesFlash = localizerFlash->GetCitiesInGroupL(*groupFlash);
	CleanupStack::PushL(citiesFlash);
	
	// Get the number of cities in the group
	TInt countFlash = citiesFlash->Count();
	test.Printf(_L("The number of cities found in a city group in the files under system drive are %d\n"), countFlash);
	
	// Test whether number of cities from system drive is more than ones on ROM
	test(countDefault < countFlash);
	
	// Cleanup
	CleanupStack::PopAndDestroy(citiesFlash);
	CleanupStack::PopAndDestroy(groupFlash);
	CleanupStack::PopAndDestroy(localizerFlash);
	
	// Delete the resource files
	DeleteEnglishFilesL(serv);
	serv.DeleteFileL(KTzLocDb);
	
	// Get cities and city group 
	CTzLocalizer* newLocalizer = CTzLocalizer::NewLC();
	CTzLocalizedCityGroup* newGroup = newLocalizer->GetCityGroupL(KDefaultCityGroupId);
	CleanupStack::PushL(newGroup);
	CTzLocalizedCityArray* newCities = newLocalizer->GetCitiesInGroupL(*newGroup);
	CleanupStack::PushL(newCities);
	
	// Get the count of cities
	TInt newCount = newCities->Count();
	test.Printf(_L("The number of cities found in a city group in the files under ROM are %d\n"), newCount);
	test(newCount < countFlash);
	
	// Cleanup
	CleanupStack::PopAndDestroy(newCities);
	CleanupStack::PopAndDestroy(newGroup);
	CleanupStack::PopAndDestroy(newLocalizer);
	CleanupStack::PopAndDestroy(&serv);
	
	// Revert the system locale to English
	localeDllLocationString.Copy(KDefaultLocale);
	TInt ret = extendedLocale.LoadLocaleAspect(ELocaleLanguageSettings, localeDllLocationString) ;
	if(ret != KErrNone)	
		{
		test.Printf(_L("Could not revert to default locale. Error code is %d"), ret);
		}	
	extendedLocale.SaveSystemSettings();
	}

LOCAL_C void TestImpactOfTimeZoneDeleteOnCityL()
	{
	RPIMTestServer serv;
	CleanupClosePushL(serv);
	User::LeaveIfError(serv.Connect());
	
	serv.CloseTzSession();

	TRAP_IGNORE(serv.KillProcessL(KAgendaServer));
	TRAP_IGNORE(serv.KillProcessL(KTzServer));
	
	DeleteFilesL(serv);
	serv.DeleteDirL(KDestTzLocRscDir);
	serv.DeleteFileL(KTzLocDb);
	
	RTz tz;
	CleanupClosePushL(tz);
	User::LeaveIfError(tz.Connect());	
	
	CTzLocalizer* localizer = CTzLocalizer::NewLC();
	localizer->SetTimeZoneL(KDefaultTzId);

	_LIT(KUserCity1, "UserCity1");
	CTzLocalizedCity* city1 = localizer->AddCityL(KDefaultTzId, KUserCity1);
	delete city1;

	_LIT(KUserCity2, "UserCity2");
	CTzLocalizedCity* city2 = localizer->AddCityL(KDefaultTzId, KUserCity2);
	delete city2;

	_LIT(KUserCity3, "UserCity3");
	CTzLocalizedCity* city3 = localizer->AddCityL(KParisTzId, KUserCity3);
	delete city3;

	//Read the number of cities
	RPointerArray<CTzLocalizedCityRecord> cityRecords;
	tz.LocalizationReadCitiesL(cityRecords);
	test.Printf(_L("Check if the number of cities read is correct\n"));
	test(cityRecords.Count() == 3);
	cityRecords.ResetAndDestroy();
	cityRecords.Close();
	CleanupStack::PopAndDestroy(localizer);
	
	// Copy the files that do not contain information on Paris	
	TRAPD(err, serv.CopyFileL(KSrcTzLocModifiedDir, KDestTzLocRscDir));
	if (err != KErrNone)
		{
		_LIT(KErrCopyFiles, "Couldn't copy database file\n");
		test.Printf(KErrCopyFiles);
		User::Leave(err);
		}
	tz.SwiObsBeginL();
	tz.SwiObsFileChangedL(RTz::EFilterResourceTimezonelocalization);
	tz.SwiObsEndL();	
	
	CTzLocalizer* newLocalizer = CTzLocalizer::NewLC();
	newLocalizer->SetTimeZoneL(KDefaultTzId);
	//Read the number of cities again
	RPointerArray<CTzLocalizedCityRecord> cityRecords2;
	tz.LocalizationReadCitiesL(cityRecords2);
	test.Printf(_L("Again, check if the number of cities read is correct\n"));
	test(cityRecords2.Count() == 2);
	cityRecords2.ResetAndDestroy();
	cityRecords2.Close();	
	CleanupStack::PopAndDestroy(newLocalizer);
	CleanupStack::PopAndDestroy(&tz);
	
	DeleteFilesL(serv);
	serv.DeleteDirL(KDestTzLocRscDir);
	serv.DeleteFileL(KTzLocDb);

	CleanupStack::PopAndDestroy(&serv);
	}
	
LOCAL_C void TestImpactOfTimeZoneDeleteOnZoneL()
	{
	RPIMTestServer serv;
	CleanupClosePushL(serv);
	User::LeaveIfError(serv.Connect());
	
	serv.CloseTzSession();
	TRAP_IGNORE(serv.KillProcessL(KAgendaServer));
	TRAP_IGNORE(serv.KillProcessL(KTzServer));
	
	DeleteFilesL(serv);
	serv.DeleteDirL(KDestTzLocRscDir);
	serv.DeleteFileL(KTzLocDb);
	
	RTz tz;
	CleanupClosePushL(tz);
	User::LeaveIfError(tz.Connect());	
	
	CTzLocalizer* localizer = CTzLocalizer::NewLC();
	localizer->SetFrequentlyUsedZoneL(KParisTzId, CTzLocalizedTimeZone::EHomeZone);
	CleanupStack::PopAndDestroy(localizer);	
	
	// Copy the resource files that do not have Paris in them
	test.Printf(_L("Copy resource files to flash drive\n"));
	TRAPD(err, serv.CopyFileL(KSrcTzLocModifiedDir, KDestTzLocRscDir));
	if (err != KErrNone)
		{
		_LIT(KErrCopyFiles, "Couldn't copy database file\n");
		test.Printf(KErrCopyFiles);
		User::Leave(err);
		}
	tz.SwiObsBeginL();
	tz.SwiObsFileChangedL(RTz::EFilterResourceTimezonelocalization);
	tz.SwiObsEndL();	
	
	CTzLocalizer* newLocalizer = CTzLocalizer::NewLC();
	//Extract zone information, same city should be returned by GetFrequentlyUsedZoneCityL below.	
	CTzLocalizedCity* cityToCompareWith = newLocalizer->GetDefaultCityL(KDefaultTzId);
	CleanupStack::PushL(cityToCompareWith);
	
	//Extract zone information, same zone should be returned by GetFrequentlyUsedZoneL below.
	CTzLocalizedTimeZone* zoneToCompareWith = newLocalizer->GetLocalizedTimeZoneL(KDefaultTzId);
	CleanupStack::PushL(zoneToCompareWith);
	
	CTzLocalizedCity* city = newLocalizer->GetFrequentlyUsedZoneCityL(CTzLocalizedTimeZone::EHomeZone);
	test.Printf(_L("Check if the frequently used city read from the database is same as the one expected\n"));
	test(city->TimeZoneId() == cityToCompareWith->TimeZoneId());
	test(city->GroupId() == cityToCompareWith->GroupId());
	test(city->Name() == cityToCompareWith->Name());
	delete city;
	
	CTzLocalizedTimeZone* timeZone = newLocalizer->GetFrequentlyUsedZoneL(CTzLocalizedTimeZone::EHomeZone);
	test.Printf(_L("Check if the frequently used zone read from the database is same as the one expected\n"));
	test(timeZone->TimeZoneId() == zoneToCompareWith->TimeZoneId());
	test(timeZone->StandardName() == zoneToCompareWith->StandardName());
	test(timeZone->ShortStandardName() == zoneToCompareWith->ShortStandardName());	
	test(timeZone->DaylightName() == zoneToCompareWith->DaylightName());
	test(timeZone->ShortDaylightName() == zoneToCompareWith->ShortDaylightName());
	delete timeZone;
	
	CleanupStack::PopAndDestroy(2, cityToCompareWith);
	CleanupStack::PopAndDestroy(newLocalizer);	
	CleanupStack::PopAndDestroy(&tz);
	
	DeleteFilesL(serv);
	serv.DeleteDirL(KDestTzLocRscDir);
	serv.DeleteFileL(KTzLocDb);

	CleanupStack::PopAndDestroy(&serv);
	}

LOCAL_C void TestImpactOfTimeZoneUpdateL()
	{
	RPIMTestServer serv;
	CleanupClosePushL(serv);
	User::LeaveIfError(serv.Connect());
	
	serv.CloseTzSession();
	TRAP_IGNORE(serv.KillProcessL(KAgendaServer));
	TRAP_IGNORE(serv.KillProcessL(KTzServer));
	
	DeleteFilesL(serv);
	serv.DeleteDirL(KDestTzLocRscDir);
	serv.DeleteFileL(KTzLocDb);
	
	RTz tz;
	CleanupClosePushL(tz);
	User::LeaveIfError(tz.Connect());	
	
	CTzLocalizer* localizer = CTzLocalizer::NewLC();
	localizer->SetFrequentlyUsedZoneL(KDefaultTzId, CTzLocalizedTimeZone::EHomeZone);
	CleanupStack::PopAndDestroy(localizer);
	// Copy the resource files that have updated information for London
	TRAPD(err, serv.CopyFileL(KSrcTzLocModifiedDir, KDestTzLocRscDir));
	if (err != KErrNone)
		{
		_LIT(KErrCopyFiles, "Couldn't copy database file\n");
		test.Printf(KErrCopyFiles);
		User::Leave(err);
		}
	tz.SwiObsBeginL();
	tz.SwiObsFileChangedL(RTz::EFilterResourceTimezonelocalization);
	tz.SwiObsEndL();	
	
	CTzLocalizer* newLocalizer = CTzLocalizer::NewLC();
	//Extract zone information, same city should be returned by GetFrequentlyUsedZoneCityL below.	
	CTzLocalizedCity* cityToCompareWith = newLocalizer->GetDefaultCityL(KDefaultTzId);
	CleanupStack::PushL(cityToCompareWith);
	
	//Extract zone information, same zone should be returned by GetFrequentlyUsedZoneL below.
	CTzLocalizedTimeZone* zoneToCompareWith = newLocalizer->GetLocalizedTimeZoneL(KDefaultTzId);
	CleanupStack::PushL(zoneToCompareWith);
		
	CTzLocalizedCity* city = newLocalizer->GetFrequentlyUsedZoneCityL(CTzLocalizedTimeZone::EHomeZone);
	test.Printf(_L("Check if the frequently used city read from the database is same as the one expected\n"));
	test(city->TimeZoneId() == cityToCompareWith->TimeZoneId());
	test(city->GroupId() == cityToCompareWith->GroupId());
	test(city->Name() == cityToCompareWith->Name());
	delete city;
	
	CTzLocalizedTimeZone* timeZone = newLocalizer->GetFrequentlyUsedZoneL(CTzLocalizedTimeZone::EHomeZone);
	test.Printf(_L("Check if the frequently used zone read from the database is same as the one expected\n"));
	test(timeZone->TimeZoneId() == zoneToCompareWith->TimeZoneId());
	test(timeZone->StandardName() == zoneToCompareWith->StandardName());
	test(timeZone->ShortStandardName() == zoneToCompareWith->ShortStandardName());	
	test(timeZone->DaylightName() == zoneToCompareWith->DaylightName());
	test(timeZone->ShortDaylightName() == zoneToCompareWith->ShortDaylightName());
	delete timeZone;
	CleanupStack::PopAndDestroy(2, cityToCompareWith);
	CleanupStack::PopAndDestroy(newLocalizer);	
	CleanupStack::PopAndDestroy(&tz);
	
	DeleteFilesL(serv);
	serv.DeleteDirL(KDestTzLocRscDir);
	serv.DeleteFileL(KTzLocDb);
	serv.DeleteFileL(KTzLocTemporaryFile);
	
	CleanupStack::PopAndDestroy(&serv);
	}

LOCAL_C void DoTestsL()
	{
	test.Title();

	
	test.Start(_L(" @SYMTestCaseID	PIM-APPSERV-LOC-TZL-ECL-0001  Test Updated Localization Files"));
	TestUpdatedLocalizationFilesL();
	
	test.Next(_L(" @SYMTestCaseID PIM-localizationupdate-0001 Test Eclipsing of Files with locale changes"));
	TestEclipsingOfRscFileL();
	
	
	test.Next(_L("Test the impact of deleting time zone id on user cities table"));
	TestImpactOfTimeZoneDeleteOnCityL();

	test.Next(_L("Test the impact of deleting time zone id on frequently used zone table"));
	TestImpactOfTimeZoneDeleteOnZoneL();
	
	test.Next(_L("Test the impact of updating time zone id on frequently used zone table"));
	TestImpactOfTimeZoneUpdateL();
	
	test.End();
	}


GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;

	CTrapCleanup* cleanup = CTrapCleanup::New();

	TRAPD(ret, DoTestsL());
	test.Printf(_L("Trapped return value from DoTestsL(): %d\n"), ret);	
	test(ret == KErrNone);
	
	delete cleanup;

	test.Close();

	__UHEAP_MARKEND;

	return KErrNone;
	}
