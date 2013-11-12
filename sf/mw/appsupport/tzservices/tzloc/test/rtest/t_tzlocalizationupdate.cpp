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
#include "testserver.h"
#include <f32file.h>
#include "ctzswiplugintester.h"


const TInt KDefaultTzId = 2592;
const TInt KDefaultCityGroupId = 1;
_LIT(KFlashLocalizationUpgradeSis, "c:\\signed_timezone_loc_upgrade.sis");
_LIT(KRomLocalizationUpgradeSis, "z:\\timezonelocalization\\signed_timezone_loc_upgrade.sis");
_LIT(KTimeZonePackageUid, "200020B0");
_LIT(KTimeZonePackageName, "TimeZone-Database-locpatch");
_LIT(KTimeZoneVendorName, "Symbian Software Ltd.");

_LIT(Ktimezonegroups_r01, "c:\\resource\\timezonelocalization\\timezonegroups.r01");
_LIT(Ktimezonegroups_r02, "c:\\resource\\timezonelocalization\\timezonegroups.r02");
_LIT(Ktimezonegroups_rsc, "c:\\resource\\timezonelocalization\\timezonegroups.rsc");
_LIT(Ktimezones_r01, "c:\\resource\\timezonelocalization\\timezones.r01");
_LIT(Ktimezones_r02, "c:\\resource\\timezonelocalization\\timezones.r02");
_LIT(Ktimezones_rsc, "c:\\resource\\timezonelocalization\\timezones.rsc");

RTest test (_L("T_LocalizationUpdate"));

LOCAL_C void DeleteFilesL()
	{
	RPIMTestServer serv;	
	CleanupClosePushL(serv);
	User::LeaveIfError(serv.Connect());
	serv.DeleteFileL(Ktimezonegroups_r01());	
	serv.DeleteFileL(Ktimezonegroups_r02());
	serv.DeleteFileL(Ktimezonegroups_rsc());
	serv.DeleteFileL(Ktimezones_r01());	
	serv.DeleteFileL(Ktimezones_r02());
	serv.DeleteFileL(Ktimezones_rsc());
	CleanupStack::PopAndDestroy(&serv);
	}
LOCAL_C void CopyFileL(const TDesC& aCopyFromFile, const TDesC& aCopyToFile)
	{
	RPIMTestServer serv;	
	CleanupClosePushL(serv);
	User::LeaveIfError(serv.Connect());
	serv.CopyFileL(aCopyFromFile, aCopyToFile);
	CleanupStack::PopAndDestroy(&serv);
	}

LOCAL_C TInt GetNumCitiesFromGroupL(const TInt aGroupId)
	{
	CTzLocalizer* localizer = CTzLocalizer::NewLC();
	//set timezone to be London
	localizer->SetTimeZoneL(KDefaultTzId);		
	CTzLocalizedCityGroup* group = localizer->GetCityGroupL(aGroupId);
	CleanupStack::PushL(group);
	CTzLocalizedCityArray* cities = localizer->GetCitiesInGroupL(*group);
	CleanupStack::PushL(cities);
	TInt count = cities->Count();
	CleanupStack::PopAndDestroy(cities);
	CleanupStack::PopAndDestroy(group);
	CleanupStack::PopAndDestroy(localizer);	
	return count;
	}


/**
@SYMTestCaseID      PIM-APPSERV-TZLOC-TZL-ECL-0001
@SYMTestCaseDesc    The purpose of this test is to verify the use of eclipsed system
					TZ localization resources.  
@SYMTestActions     1.	Use the CTzLocalizer API to obtain a copy of the cities 
						(preUpdateCities) for a given city group identifier.
					2.	Copy system TZ localization resources that adds a city 
						to the city group used in action 1 to the appropriate location 
						on the C: drive.
					3.	Use the CTzLocalizer API to obtain a copy of the cities 
						(postUpdateCities) for the same city group identifier used in action 1.

@SYMTestExpectedResults     The preUpdateCities object should not contain the added city and the
							postUpdateCities object should contain the added city.
@SYMREQ                     REQ9952
@SYMTestType                CT
@SYMTestPriority            1
*/

LOCAL_C void TestUpdatedLocalizationFilesL()
	{
	TInt originalNumCities = GetNumCitiesFromGroupL(KDefaultCityGroupId);
    test.Printf(_L(" @SYMTestCaseID  PIM-APPSERV-TZLOC-TZL-ECL-0001 "));
	test.Printf(_L("The number of cities found in a city group in the default files are %d\n"), originalNumCities);
	//Install a patch with new localization files on the c: drive
	TRequestStatus reqStatus;
	CCTzSwiPluginTester* sisInstaller = CCTzSwiPluginTester::NewLC();
	sisInstaller->InstallSIS(KFlashLocalizationUpgradeSis(), CCTzSwiPluginTester::EPropertyNames, reqStatus);
	CActiveScheduler::Start();
	//Get the new number of cities from the group, this should come from the eclipsed files
	TInt flashNumCities = GetNumCitiesFromGroupL(KDefaultCityGroupId);
	test.Printf(_L("The number of cities found in a city group in the new installed files are %d\n"), flashNumCities);
	test(originalNumCities < flashNumCities);
	test (sisInstaller->UninstallSIS(KTimeZonePackageUid(), KTimeZonePackageName(), KTimeZoneVendorName()) == KErrNone);
	//Need this to allow the notification plug-in code to run
	User::After(1000000);
	CleanupStack::PopAndDestroy(sisInstaller);
	TInt romNumCities = GetNumCitiesFromGroupL(KDefaultCityGroupId);
	test.Printf(_L("The number of cities found in a city group in the rom files are %d\n"), romNumCities);
	test(originalNumCities == romNumCities);
	}

LOCAL_C void DoStartL()
	{
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);
	DeleteFilesL();
	CopyFileL(KRomLocalizationUpgradeSis(), KFlashLocalizationUpgradeSis());
	TestUpdatedLocalizationFilesL();
	// Delete active scheduler
	CleanupStack::PopAndDestroy(scheduler);	
	}

//  Global Functions


GLDEF_C TInt E32Main()
	{
	// Create cleanup stack
	__UHEAP_MARK;
	test.Start(_L("Starting Test t_tzlocalizationupdate"));
	CTrapCleanup* cleanup = CTrapCleanup::New();

	// Run application code inside TRAP harness.
	TRAPD(mainError, DoStartL());
	
	test(mainError == KErrNone);
	
	delete cleanup;
	test.End();
	test.Close();
	__UHEAP_MARKEND;
	return KErrNone;
	}



