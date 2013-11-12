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

#include <connect/sbeclient.h>
#include <e32test.h>
#include "tzuserdata.h"
#include <tzlocalizer.h>
#include "testserver.h"
#include <tzlocalizedcityrecord.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <tzusernames.h>
#include <tzuserdefineddata.h>
#endif
#include "tzidinternal.h"

_LIT(KBackupTest, "Tz Backup");
RTest test(KBackupTest);

// Literal constants
_LIT(KTzTestBackupFile, "C:\\Backup__tzuserdata.db");
_LIT(KTzBackupFileFullPath, "C:\\private\\1020383e\\SQLite__tzuserdata.db");

_LIT(KTzTestBackupLocDb, "C:\\Backup__TzLocalization.db");
_LIT(KTzBackupLocDbFullPath, "C:\\private\\100012a5\\DBS_10206A8B_TzLocalization.db");

_LIT(KTimeZoneBackupRegistrationFullPath, "Z:\\private\\1020383E\\backup_registration.xml");

_LIT(KPerFile1,"c:\\private\\100012a5\\DBS_2000836D_DBS_DSC.db");
_LIT(KPerFile2,"c:\\private\\100012a5\\DBS_10206A8B_TzLocalization.db");

_LIT(KStandardName1, "UserTzName1");
_LIT(KShortStandardName1, "UserTzSh1");
_LIT(KDaylightSaveName1, "UserDLSaveName1");
_LIT(KShortDaylightSaveName1, "UserShDLS1");
_LIT(KCityNameName1, "UserCityName1");
_LIT(KRegionName1, "UserRegionName1");

_LIT(KStandardName2, "UserTzName2");
_LIT(KShortStandardName2, "UserTzSh2");
_LIT(KDaylightSaveName2, "UserDLSaveName2");
_LIT(KShortDaylightSaveName2, "UserShDLS2");
_LIT(KCityNameName2, "UserCityName2");
_LIT(KRegionName2, "UserRegionName2");

_LIT(KUserAddedCityName1, "UserAddedCity1");
_LIT(KUserAddedCityName2, "UserAddedCity2");

const TInt KLondonTzId = 2592;
const TInt KParisTzId = 2656;

static conn::CSBEClient* BackupClient;
static TDriveList DriveList;

class CTestUserDefinedBackupRestore : public CBase
	{
public:
	static CTestUserDefinedBackupRestore* NewL();
	~CTestUserDefinedBackupRestore();

enum TTest
	{
	EBackupTest,
	ERestoreTest
	};

	void RunTestsL();
	void TestBackupAndRestoreUserDbL(TTest aTest);
	void TestFrequentlyUsedZonesUpdateAfterRestoreDeleteL();
	void TestFrequentlyUsedZonesUpdateAfterRestoreUpdateL();
	void TestUserCitiesUpdateAfterRestoreL();
	void TestFrequentlyUsedCurrentZonesUpdateAfterRestoreUpdateL();
	void TestSystemRulesUpdateAfterRestoreL();
	void TestBackupAndRestoreLocDbL();
private:
	CTestUserDefinedBackupRestore();
	TBool CheckXMLRegistrationFileL();
	TBool SimulateTzBackup(const TDesC& aSrcfile, const TDesC& aDestfile);
	TBool SimulateTzRestore(const TDesC& aSrcfile, const TDesC& aDestfile);
	void ConstructL();
	void ResetTzUserDataL();
	TBool TestSystemRulesUpdateAfterRestoreL(TUint aExpectedTzId, TBool aRestoreEmptyFile);
	TBool LocDbWriteFunctionL(CTzLocalizer& aTzLocalizer, TInt aExpectedErr);

	TBool CompareFrequentlyUsedCity(const CTzLocalizedCity& aCity, const CTzLocalizedCity& aCityToCompare);
	TBool CompareFrequentlyUsedZone(const CTzLocalizedTimeZone& aZone, const CTzLocalizedTimeZone& aZoneToCompare);
	TBool CompareFrequentlyUsedCityAfterUpdate(TInt aId, const CTzLocalizedCity& aCity);
	TBool CompareFrequentlyUsedCityRestoredAfterUpdate(TInt aId, const CTzLocalizedCity& aCity);
	TBool CompareFrequentlyUsedZoneAfterUpdate(TInt aId, const CTzLocalizedTimeZone& aZone);
		
//Member
public:
	CTzUserData* iUserData;
	RFs iFs;
	RPIMTestServer iServ;
	RTz iTz;
	};

// building block test backup and restore functions

static void StartBackupL()
	{
	/* Use the Secure Backup Engine API to "start" a backup.  The backup
	 happens indirectly through CTzUserDataDb::BackupBeginningL()/
	 BackupCompletedL()/RestoreBeginningL()/RestoreCompletedL() methods.
	 These methods are implementations for MBackupRestoreNotificatioObserver
	 which monitors changes in the P&S property conn::KUidBackupRestoreKey.
	 This P&S property is changed by calling the Secure Backup Engine API
	 below.
	 */	
	test.Printf(_L("Starting Backup\n"));
	BackupClient->SetBURModeL(DriveList, conn::EBURBackupFull, conn::EBackupBase);
	User::After(3000000);// Wait a bit to allow the server to close the database
	}


static void EndBackupL()
	{
	/* Use the Secure Backup Engine API to "end" a backup. The backup ends
	 indirectly through CTzUserDataDb::BackupBeginningL()/
	 BackupCompletedL()/RestoreBeginningL()/RestoreCompletedL() methods.
	 These methods are implementations for MBackupRestoreNotificatioObserver
	 which monitors changes in the P&S property conn::KUidBackupRestoreKey.
	 This P&S property is changed by calling the Secure Backup Engine API
	 below.
	 */
	test.Printf(_L("Finishing Backup\n"));
	BackupClient->SetBURModeL(DriveList, conn::EBURNormal, conn::ENoBackup);
	User::After(3000000);// Wait a bit to allow the server to open the database
	}


static void StartRestoreL()
	{
	/* Use the Secure Backup Engine API to "start" a restore.  
	 This happens indirectly through CTzUserDataDb::BackupBeginningL()/
	 BackupCompletedL()/RestoreBeginningL()/RestoreCompletedL() methods.
	 These methods are implementations for MBackupRestoreNotificatioObserver
	 which monitors changes in the P&S property conn::KUidBackupRestoreKey.
	 This P&S property is changed by calling the Secure Backup Engine API
	 below.
	 */
	test.Printf(_L("Starting restore\n"));
	BackupClient->SetBURModeL(DriveList, conn::EBURRestoreFull, conn::EBackupBase);
	User::After(3000000);// Wait a bit to allow the server to close the database
	}


static void EndRestoreL()
	{
	/* Use the Secure Backup Engine API to "end" a restore.
	 This happens indirectly through CTzUserDataDb::BackupBeginningL()/
	 BackupCompletedL()/RestoreBeginningL()/RestoreCompletedL() methods.
	 These methods are implementations for MBackupRestoreNotificatioObserver
	 which monitors changes in the P&S property conn::KUidBackupRestoreKey.
	 This P&S property is changed by calling the Secure Backup Engine API
	 below.
	 */
	test.Printf(_L("Finishing restore\n"));
	BackupClient->SetBURModeL(DriveList, conn::EBURNormal, conn::ENoBackup);
	User::After(3000000);// Wait a bit to allow the server to open and update the database
	}

LOCAL_D void ResetAndDestroyTzIdArray(TAny* aPtr)
	{
	RPointerArray<CTzId>* ids = static_cast<RPointerArray<CTzId>*>(aPtr);
	if (ids)
		{
		ids->ResetAndDestroy();
		}
	}

LOCAL_D void CleanupCityPointerArray(TAny* aPtr)
	{
	RPointerArray<CTzLocalizedCityRecord>* ids = static_cast<RPointerArray<CTzLocalizedCityRecord>*>(aPtr);
	if (ids)
		{
		ids->ResetAndDestroy();
		}
	}
	
/** Access to user defined time zone data during backup or restore operation 
@SYMTestCaseID      PIM-TTZBACKUP-TZS-BR-0001
@SYMTestCaseDesc    The purpose of this test is to verify the behaviour of the 
					client APIs that access user defined time zones during a backup or restore operation.
@SYMTestActions    
1.	Create a user defined time zone.
2.	Simulate start of backup or restore operation.
3.	Access (read, update, delete) the user defined time zone created in action 1. 1
4.	Create a user defined time zone. 2
5.	Simulate end of backup or restore operation.
6.	Access (read, update, delete) the user defined time zone created in action 1. 3
7.	Create a user defined time zone. 4

@SYMTestExpectedResults In step 3 and 4, operation leaves with KErrLocked
						In step 6 and 7, operation completes without error	

@SYMTestType                CT
@SYMTestPriority            1
*/

CTestUserDefinedBackupRestore::CTestUserDefinedBackupRestore()
	{
	
	}
	
CTestUserDefinedBackupRestore* CTestUserDefinedBackupRestore::NewL()
	{
	CTestUserDefinedBackupRestore* self = new(ELeave) CTestUserDefinedBackupRestore();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CTestUserDefinedBackupRestore::ConstructL()
	{
	User::LeaveIfError(iFs.Connect());
	User::LeaveIfError(iServ.Connect());
	iServ.CloseTzSession();
	}

CTestUserDefinedBackupRestore::~CTestUserDefinedBackupRestore()
	{
	delete iUserData;
	iFs.Delete(KTzTestBackupFile);
	// delete database file
	iFs.Delete(KTzBackupFileFullPath);
	iFs.Close();
	iServ.Close();
	}

void CTestUserDefinedBackupRestore::ResetTzUserDataL()
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

TBool CTestUserDefinedBackupRestore::CheckXMLRegistrationFileL()
	{
	TEntry entry;

	TInt error = iFs.Entry(KTimeZoneBackupRegistrationFullPath, entry);

	if (error == KErrNotFound)
		{
		return EFalse;
		}

	User::LeaveIfError(error);

	if(entry.iSize == 0)
		{
		// file is present but is empty
		User::Leave(KErrCorrupt);
		}

	// found the registration file
	return ETrue;
	}

TBool CTestUserDefinedBackupRestore::SimulateTzBackup(const TDesC& aSrcfile, const TDesC& aDestfile)
	{
    TRAPD(error, iServ.CopyFileL(aSrcfile, aDestfile));
 
	if (error != KErrNone)
		{
		test.Printf(_L("Copy failed with error %i"), error);
		return EFalse;
		}
	return ETrue;
	}
	
TBool CTestUserDefinedBackupRestore::SimulateTzRestore(const TDesC& aSrcfile, const TDesC& aDestfile)
	{
    // Backup tz user-defined database file.
    TRAPD(error, iServ.CopyFileL(aSrcfile ,aDestfile));
	
	if (error != KErrNone)
		{
		test.Printf(_L("Copy failed with error %i"), error);
		return EFalse;
		}
	return ETrue;
	}

TBool CTestUserDefinedBackupRestore::CompareFrequentlyUsedCity(const CTzLocalizedCity& aCity, const CTzLocalizedCity& aCityToCompare)
	{
	if(aCity.TimeZoneId() != aCityToCompare.TimeZoneId())
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
	
TBool CTestUserDefinedBackupRestore::CompareFrequentlyUsedZone(const CTzLocalizedTimeZone& aZone, const CTzLocalizedTimeZone& aZoneToCompare)
	{
	if(aZone.TimeZoneId() != aZoneToCompare.TimeZoneId())
		{
		return EFalse;
		}
	if(aZone.StandardName() != aZoneToCompare.StandardName())
		{
		return EFalse;
		}
	if(aZone.ShortStandardName() != aZoneToCompare.ShortStandardName())
		{
		return EFalse;
		}
	if(aZone.DaylightName() != aZoneToCompare.DaylightName())
		{
		return EFalse;
		}
	if(aZone.ShortDaylightName() != aZoneToCompare.ShortDaylightName())
		{
		return EFalse;
		}
	return ETrue;
	}

TBool CTestUserDefinedBackupRestore::CompareFrequentlyUsedCityAfterUpdate(TInt aId, const CTzLocalizedCity& aCity)
	{
	if(aCity.TimeZoneId() != aId)
		{
		return EFalse;
		}
	if(aCity.GroupId() != 0)
		{
		return EFalse;
		}
	if(aCity.Name() != KCityNameName2)
		{
		return EFalse;
		}
	return ETrue;
	}
	
TBool CTestUserDefinedBackupRestore::CompareFrequentlyUsedCityRestoredAfterUpdate(TInt aId, const CTzLocalizedCity& aCity)
	{
	if(aCity.TimeZoneId() != aId)
		{
		return EFalse;
		}
	if(aCity.GroupId() != 0)
		{
		return EFalse;
		}
	if(aCity.Name() != KCityNameName1)
		{
		return EFalse;
		}
	return ETrue;
	}
	
TBool CTestUserDefinedBackupRestore::CompareFrequentlyUsedZoneAfterUpdate(TInt aId, const CTzLocalizedTimeZone& aZone)
	{
	if(aZone.TimeZoneId() != aId)
		{
		return EFalse;
		}
	if(aZone.StandardName() != KStandardName1)
		{
		return EFalse;
		}
	if(aZone.ShortStandardName() != KShortStandardName1)
		{
		return EFalse;
		}
	if(aZone.DaylightName() != KDaylightSaveName1)
		{
		return EFalse;
		}
	if(aZone.ShortDaylightName() != KShortDaylightSaveName1)
		{
		return EFalse;
		}
	return ETrue;
	}
	
void CTestUserDefinedBackupRestore::TestBackupAndRestoreUserDbL(TTest aTest)
	{
	TTzRule rule1(0, KMaxTUint16, 60, 0, EAugust,  ETzFixedDate, 0,  0, ETzWallTimeReference, 120);
	TTzRule rule2(0,KMaxTUint16,0,60,EJanuary,ETzFixedDate,0,0,ETzWallTimeReference,120);

	CTzRules* rules = CTzRules::NewL(0, KMaxTUint);
	CleanupStack::PushL(rules);
	rules->AddRuleL(rule1);
	rules->AddRuleL(rule2);
	
	TBuf<20> stdName;
	stdName.Format(_L("StdName1"));	
	TBuf<20> shortStdName;
	shortStdName.Format(_L("ShortStd1"));	
	TBuf<20> dstName;
	dstName.Format(_L("DstName1"));	
	TBuf<20> shortDstName;
	shortDstName.Format(_L("ShortDst1"));	
	TBuf<20> cityName;
	cityName.Format(_L("CityName1"));	
	TBuf<20> regionName;
	regionName.Format(_L("RegionName1"));
	
	CTzUserNames* names = CTzUserNames::NewLC(stdName, shortStdName, dstName, shortDstName, cityName, regionName);
	
	CTzId* id = NULL;
	id = iUserData->CreateL(*rules, *names);
	
	CleanupStack::PopAndDestroy(2, rules);
	CleanupStack::PushL(id);
	
	TBool file = CheckXMLRegistrationFileL();
	test(file);
	
	if(aTest == EBackupTest)
		{
		TRAPD(err, StartBackupL());
		test(err==KErrNone);
		TBool result = SimulateTzBackup(KTzBackupFileFullPath, KTzTestBackupFile);
		test(result);
		}
	else
		{
		TRAPD(err, StartRestoreL());
		test(err == KErrNone);
		
		TBool result = SimulateTzRestore(KTzTestBackupFile, KTzBackupFileFullPath) ;
		test(result);
		
		CleanupStack::PopAndDestroy(id);
		// Setting back to initial id as the database
		// copied during restore will not have all the changes
		// that happened afterwards
		id = CTzId::NewL(KUserTzIdMin);
		CleanupStack::PushL(id);
		}
	
	test.Printf(_L("Now try reading the names of the created zone\n"));

	CTzUserNames* readNames = NULL;
	TRAPD(err, readNames = iUserData->ReadNamesL(*id));
	delete readNames;
	
	test(err == KErrLocked);
	test.Printf(_L("Could not read names as backup/restore is in progress\n"));
			
	test.Printf(_L("Now try reading the rules of the created zone\n"));

	CTzRules* readRules = NULL;
	TRAP(err, readRules = iUserData->ReadRulesL(*id));
	delete readRules;
	
	test(err == KErrLocked);
	test.Printf(_L("Could not read rules as backup/restore is in progress\n"));
		
	test.Printf(_L(" @SYMTestCaseID PIM-TTZBACKUP-TZS-BR-0001  Now try reading the TZIDs of all user-defined time zone data\n"));
	
	RPointerArray<CTzId> tzIds;
	TRAP(err, iUserData->GetTzIdsL(tzIds));
	tzIds.ResetAndDestroy();
	tzIds.Close();

	test(err == KErrLocked);
	test.Printf(_L("Could not read TZIDs as backup/restore is in progress\n"));

	TTzRule rule3(1981,1996,0,60,EMarch,ETzDayInLastWeekOfMonth,0,ESunday,ETzUtcTimeReference,60);
	TTzRule rule4(1981,1996,60,0,EOctober,ETzDayInLastWeekOfMonth,0,ESunday,ETzUtcTimeReference,60);
	
	CTzRules* rulesAdd = CTzRules::NewL(0, KMaxTUint);
	CleanupStack::PushL(rulesAdd);
	rulesAdd->AddRuleL(rule3);
	rulesAdd->AddRuleL(rule4);

	stdName.Format(_L("StdName2"));	
	shortStdName.Format(_L("ShortStd2"));	
	dstName.Format(_L("DstName2"));	
	shortDstName.Format(_L("ShortDst2"));	
	cityName.Format(_L("CityName2"));	
	regionName.Format(_L("RegionName2"));
	
	CTzUserNames* namesAdd = CTzUserNames::NewLC(stdName, shortStdName, dstName, shortDstName, cityName, regionName);
	CTzId* tzId = NULL;
	TRAP(err, tzId  = iUserData->CreateL(*rulesAdd, *namesAdd));
	delete tzId;
	test(err == KErrLocked);
	
	TRAP(err, iUserData->UpdateL(*id, *rulesAdd, *namesAdd));
	test(err == KErrLocked);
	test.Printf(_L("Could not update user-defined rules as backup/restore is in progress\n"));

	TRAP(err, iUserData->DeleteL((*id)));
	test(err == KErrLocked);
	test.Printf(_L("Could not delete user-defined rules as backup/restore is in progress\n"));
	
	CleanupStack::PopAndDestroy(namesAdd);
	CleanupStack::PopAndDestroy(rulesAdd);
	
	if(aTest == EBackupTest)
		{
		// backup complete
		TRAP(err, EndBackupL());
		test(err == KErrNone);
		}
	else
		{
		// restore complete
		TRAP(err, EndRestoreL());
		test(err == KErrNone);
		}
	
	test.Printf(_L("Check if we can perform CRUD operations on user-defined rules\n"));
	CTzUserNames* readNamesAgain = NULL;

	TRAPD(ret, readNamesAgain = iUserData->ReadNamesL(*(id)));
	test(ret == KErrNone);
	test.Printf(_L("Successfully read user-defined rules after backup/restore has completed\n"));
				
	delete readNamesAgain;
	CleanupStack::PopAndDestroy();
	
	CTzRules* newRules = CTzRules::NewL(0, KMaxTUint);
	CleanupStack::PushL(newRules);
	newRules->AddRuleL(rule3);
	newRules->AddRuleL(rule4);

	stdName.Format(_L("StdName3"));	
	shortStdName.Format(_L("ShortStd3"));	
	dstName.Format(_L("DstName3"));	
	shortDstName.Format(_L("ShortDst3"));	
	cityName.Format(_L("CityName3"));	
	regionName.Format(_L("RegionName3"));
	
	CTzUserNames* newNames = CTzUserNames::NewLC(stdName, shortStdName, dstName, shortDstName, cityName, regionName);
	CTzId* newId = NULL;
	TRAP(err, newId  = iUserData->CreateL(*newRules, *newNames));
	test(err == KErrNone);
	test.Printf(_L("Successfully created user-defined rules after backup/restore has completed\n"));
	
	TRAP(err, iUserData->UpdateL(*newId, *newRules, *newNames));
	test(err == KErrNone);
	test.Printf(_L("Successfully updated user-defined rules after backup/restore has completed\n"));
		
	TRAP(err, iUserData->DeleteL(*newId));
	test(err == KErrNone);
	test.Printf(_L("Successfully deleted user-defined rules after backup/restore has completed\n"));
	
	CleanupStack::PopAndDestroy(newNames);
	CleanupStack::PopAndDestroy(newRules);	
	delete newId;
	}

void CTestUserDefinedBackupRestore::TestBackupAndRestoreLocDbL()
	{
	test.Printf(_L("Start to backup or restore localization DB which should lock the file\n"));
	CTzLocalizer* localizer = NULL;
	//Make sure the localization DB exists
	localizer = CTzLocalizer::NewL();
	delete localizer;
	localizer = NULL;
	
	//Verify that the client can not create an object of CTzLocalizer when backup or restroe starts
	StartBackupL();
	test(SimulateTzBackup(KTzBackupLocDbFullPath, KTzTestBackupLocDb));
	TRAPD(err, localizer = CTzLocalizer::NewL());
	test (err == KErrLocked);
	test (!localizer);
	EndBackupL();
	StartRestoreL();
	test(SimulateTzRestore(KTzTestBackupLocDb, KTzBackupLocDbFullPath) );
	TRAP(err, localizer = CTzLocalizer::NewL());
	test(err == KErrLocked);
	test (!localizer);
	EndRestoreL();
	
	localizer = CTzLocalizer::NewL();
	CleanupStack::PushL(localizer);
	//Verify that functions which requires writing to the localization file are locked when backup or restore starts
	StartBackupL();
	test(SimulateTzBackup(KTzBackupLocDbFullPath, KTzTestBackupLocDb));
	test(LocDbWriteFunctionL(*localizer, KErrLocked));
	EndBackupL();
	test(LocDbWriteFunctionL(*localizer, KErrNone));
	StartRestoreL();
	test(SimulateTzRestore(KTzTestBackupLocDb, KTzBackupLocDbFullPath) );
	test(LocDbWriteFunctionL(*localizer, KErrLocked));
	EndRestoreL();
	test(LocDbWriteFunctionL(*localizer, KErrNone));
	CleanupStack::PopAndDestroy(localizer);
	}

TBool CTestUserDefinedBackupRestore::LocDbWriteFunctionL(CTzLocalizer& aTzLocalizer, TInt aExpectedErr)
	{
	TRAPD(err, aTzLocalizer.SetFrequentlyUsedZoneL(KLondonTzId,CTzLocalizedTimeZone::EHomeZone));
	if(aExpectedErr != err)
		{
		return EFalse;
		}
	return ETrue;
	}

void CTestUserDefinedBackupRestore::TestFrequentlyUsedZonesUpdateAfterRestoreDeleteL()
	{
	// Backup the file before we added the user defined time zones
	test.Printf(_L("Backup the file without any user-defined zones\n"));
	_LIT8(KLondon, "Europe/London");
	CTzId* tzid  = CTzId::NewL(KLondon);
	iTz.SetTimeZoneL(*tzid);
	delete tzid;
	
	TRAPD(err, StartBackupL());
	test(err==KErrNone);
	TBool result = SimulateTzBackup(KTzBackupFileFullPath, KTzTestBackupFile);
	test(result);
	TRAP(err, EndBackupL());
	test(err == KErrNone);
	
	// Add a user defined time zone
	test.Printf(_L("Add a user-defined zone\n"));
	TTzRule rule1(0, KMaxTUint16, 60, 0, EAugust,  ETzFixedDate, 0,  0, ETzWallTimeReference, 120);
	TTzRule rule2(0,KMaxTUint16,0,60,EJanuary,ETzFixedDate,0,0,ETzWallTimeReference,120);
	CTzRules* rules = CTzRules::NewL(0, KMaxTUint);
	CleanupStack::PushL(rules);
	rules->AddRuleL(rule1);
	rules->AddRuleL(rule2);
	
	CTzUserNames* names = CTzUserNames::NewLC(KStandardName1(), KShortStandardName1(), KDaylightSaveName1(),
	KShortDaylightSaveName1(), KCityNameName1(), KRegionName1());
	CTzId* id = iUserData->CreateL(*rules, *names);
	CleanupStack::PopAndDestroy(2, rules);
	CleanupStack::PushL(id);
	
	// Add this time zone in the frequently used zone table
	CTzLocalizer* localizer = CTzLocalizer::NewL();
	test.Printf(_L("Set the user-defined zone as the frequently used zone\n"));
	localizer->SetFrequentlyUsedZoneL(id->TimeZoneNumericID(), CTzLocalizedTimeZone::EHomeZone);
	CleanupStack::PopAndDestroy(id);
	
	// Restore the file
	test.Printf(_L("Restore the backed up file\n"));
	TRAP(err, StartRestoreL());
	test(err == KErrNone);
	result = SimulateTzRestore(KTzTestBackupFile, KTzBackupFileFullPath) ;
	test(result);
	TRAP(err, EndRestoreL());
	test(err == KErrNone);
	
	CTzLocalizedCity* cityToCompareWith = localizer->GetDefaultCityL(KLondonTzId);
	CleanupStack::PushL(cityToCompareWith);
	
	test.Printf(_L("Fetch the frequently used city and check if it is the expected one\n"));
	CTzLocalizedCity* city = localizer->GetFrequentlyUsedZoneCityL(CTzLocalizedTimeZone::EHomeZone);
	test(CompareFrequentlyUsedCity(*city, *cityToCompareWith));
	delete city;
	
	CTzLocalizedTimeZone* zoneToCompareWith = localizer->GetLocalizedTimeZoneL(KLondonTzId);
	CleanupStack::PushL(zoneToCompareWith);
	
	CTzLocalizedTimeZone* timeZone = localizer->GetFrequentlyUsedZoneL(CTzLocalizedTimeZone::EHomeZone);
	test.Printf(_L("Fetch the frequently used zone and check if it is the expected one\n"));
	test(CompareFrequentlyUsedZone(*timeZone, *zoneToCompareWith));
	delete timeZone;
	
	delete localizer;
	CleanupStack::PopAndDestroy(2, cityToCompareWith);
	}

void CTestUserDefinedBackupRestore::TestFrequentlyUsedZonesUpdateAfterRestoreUpdateL()
	{
	// Add a user defined time zone
	test.Printf(_L("Create a user-defined zone\n"));
	TTzRule rule1(0, KMaxTUint16, 60, 0, EAugust,  ETzFixedDate, 0,  0, ETzWallTimeReference, 120);
	TTzRule rule2(0,KMaxTUint16,0,60,EJanuary,ETzFixedDate,0,0,ETzWallTimeReference,120);
	
	CTzRules* rules = CTzRules::NewL(0, KMaxTUint);
	CleanupStack::PushL(rules);
	rules->AddRuleL(rule1);
	rules->AddRuleL(rule2);
	
	CTzUserNames* names = CTzUserNames::NewLC(KStandardName1(), KShortStandardName1(), KDaylightSaveName1(),
	KShortDaylightSaveName1(), KCityNameName1(), KRegionName1());
	CTzId* id = iUserData->CreateL(*rules, *names);
	CleanupStack::PopAndDestroy(2, rules);
	
	CleanupStack::PushL(id);
	
	// Backup the file before we added the user defined time zones
	test.Printf(_L("Backup the user-defined database\n"));
	TRAPD(err, StartBackupL());
	test(err==KErrNone);
	TBool result = SimulateTzBackup(KTzBackupFileFullPath, KTzTestBackupFile);
	test(result);
	TRAP(err, EndBackupL());
	test(err == KErrNone);
	
	// Update the user defined time zone
	test.Printf(_L("Update the user-defined rule\n"));
	CTzRules* newRules = CTzRules::NewL(0, KMaxTUint);
	CleanupStack::PushL(newRules);
	newRules->AddRuleL(rule1);
	newRules->AddRuleL(rule2);
	
	//Create a user defined rule names for update
	CTzUserNames* newNames = CTzUserNames::NewLC(KStandardName2(), KShortStandardName2(),
	KDaylightSaveName2(), KShortDaylightSaveName2(), KCityNameName2(), KRegionName2());
	iUserData->UpdateL(*id, *newRules, *newNames);
	CleanupStack::PopAndDestroy(2, newRules);
	
	// Add this time zone in the frequently used zone table
	CTzLocalizer* localizer = CTzLocalizer::NewL();
	test.Printf(_L("Set the new zone as the frequently used zone\n"));
	localizer->SetFrequentlyUsedZoneL(id->TimeZoneNumericID(), CTzLocalizedTimeZone::EHomeZone);
	test.Printf(_L("Fetch the frequently used city and check if it is the expected one\n"));
	CTzLocalizedCity* city = localizer->GetFrequentlyUsedZoneCityL(CTzLocalizedTimeZone::EHomeZone);
	test(CompareFrequentlyUsedCityAfterUpdate(id->TimeZoneNumericID(), *city));
	delete city;
	
	// Restore the file
	test.Printf(_L("Restore the backed up file\n"));
	TRAP(err, StartRestoreL());
	test(err == KErrNone);
	result = SimulateTzRestore(KTzTestBackupFile, KTzBackupFileFullPath) ;
	test(result);
	TRAP(err, EndRestoreL());
	test(err == KErrNone);
			
	test.Printf(_L("Get the frequently used city and check if it is the expected one\n"));
	CTzLocalizedCity* updatedCity = localizer->GetFrequentlyUsedZoneCityL(CTzLocalizedTimeZone::EHomeZone);
	test(CompareFrequentlyUsedCityRestoredAfterUpdate(id->TimeZoneNumericID(), *updatedCity));
	delete updatedCity;
	test.Printf(_L("Get the frequently used zone and check if it is the expected one\n"));
	CTzLocalizedTimeZone* timeZone = localizer->GetFrequentlyUsedZoneL(CTzLocalizedTimeZone::EHomeZone);
	test(CompareFrequentlyUsedZoneAfterUpdate(id->TimeZoneNumericID(), *timeZone));
	delete timeZone;
	
	CleanupStack::PopAndDestroy(id);
	delete localizer;
	}

void CTestUserDefinedBackupRestore::TestFrequentlyUsedCurrentZonesUpdateAfterRestoreUpdateL()
	{
	// Add a user defined time zone
	test.Printf(_L("Create a user-defined zone\n"));
	TTzRule rule1(0, KMaxTUint16, 60, 0, EAugust,  ETzFixedDate, 0,  0, ETzWallTimeReference, 120);
	TTzRule rule2(0,KMaxTUint16,0,60,EJanuary,ETzFixedDate,0,0,ETzWallTimeReference,120);
	
	CTzRules* rules = CTzRules::NewL(0, KMaxTUint);
	CleanupStack::PushL(rules);
	rules->AddRuleL(rule1);
	rules->AddRuleL(rule2);

	CTzUserNames* names = CTzUserNames::NewLC(KStandardName1(), KShortStandardName1(), KDaylightSaveName1(),
	KShortDaylightSaveName1(), KCityNameName1(), KRegionName1());
	CTzId* id = iUserData->CreateL(*rules, *names);
	TInt tzid = id->TimeZoneNumericID();
	delete id;
	CleanupStack::PopAndDestroy(2, rules);
		
	// Backup the file before we added the user defined time zones
	test.Printf(_L("Start the database backup\n"));
	TRAPD(err, StartBackupL());
	test(err==KErrNone);
	TBool result = SimulateTzBackup(KTzBackupFileFullPath, KTzTestBackupFile);
	test(result);
	TRAP(err, EndBackupL());
	test(err == KErrNone);
	
	// Add user-defined rules
	test.Printf(_L("Update the rule in the database\n"));
	CTzRules* newRules = CTzRules::NewL(0, KMaxTUint);
	CleanupStack::PushL(newRules);
	newRules->AddRuleL(rule1);
	newRules->AddRuleL(rule2);
	
	//Create a user defined rule names- CTzUserNames.
	CTzUserNames* names2 = CTzUserNames::NewLC(KStandardName2(), KShortStandardName2(),
	KDaylightSaveName2(), KShortDaylightSaveName2(), KCityNameName2(), KRegionName2());
	CTzId* tzId = CTzId::NewL(tzid);
	CleanupStack::PushL(tzId);
	iUserData->UpdateL(*tzId, *rules, *names2);
	CleanupStack::PopAndDestroy(tzId);
	CleanupStack::PopAndDestroy(2, newRules);
	
	// Set the time zone to Paris
	test.Printf(_L("Set the time zone to Paris\n"));
	CTzLocalizer* localizer = CTzLocalizer::NewL();
	localizer->SetTimeZoneL(KParisTzId);
	
	CTzLocalizedCity* cityToCompareWith = localizer->GetDefaultCityL(KParisTzId);
	CleanupStack::PushL(cityToCompareWith);
	
	// Read the frequently used city and compare with the expected one
	test.Printf(_L("Get the frequently used city and check if it is the expected one\n"));
	CTzLocalizedCity* city = localizer->GetFrequentlyUsedZoneCityL(CTzLocalizedTimeZone::ECurrentZone);
	test(CompareFrequentlyUsedCity(*city, *cityToCompareWith));
	delete city;
	
	CTzLocalizedTimeZone* zoneToCompareWith = localizer->GetLocalizedTimeZoneL(KParisTzId);
	CleanupStack::PushL(zoneToCompareWith);
	
	test.Printf(_L("Get the frequently used zone and check if it is the expected one\n"));
	CTzLocalizedTimeZone* timeZone = localizer->GetFrequentlyUsedZoneL(CTzLocalizedTimeZone::ECurrentZone);
	test(CompareFrequentlyUsedZone(*timeZone, *zoneToCompareWith));
	delete timeZone;
	
	// Restore the file
	test.Printf(_L("Restore the backed up file\n"));
	TRAP(err, StartRestoreL());
	test(err == KErrNone);
	result = SimulateTzRestore(KTzTestBackupFile, KTzBackupFileFullPath) ;
	test(result);
	TRAP(err, EndRestoreL());
	test(err == KErrNone);
	
	test.Printf(_L("Again, get the frequently used city and check if it is the expected one\n"));
	CTzLocalizedCity* updatedCity = localizer->GetFrequentlyUsedZoneCityL(CTzLocalizedTimeZone::ECurrentZone);
	test(CompareFrequentlyUsedCity(*updatedCity, *cityToCompareWith));
	delete updatedCity;
	
	test.Printf(_L("Again, get the frequently used zone and check if it is the expected one\n"));
	CTzLocalizedTimeZone* updatedTimeZone = localizer->GetFrequentlyUsedZoneL(CTzLocalizedTimeZone::ECurrentZone);
	test(CompareFrequentlyUsedZone(*updatedTimeZone, *zoneToCompareWith));
	
	delete updatedTimeZone;
	delete localizer;
	CleanupStack::PopAndDestroy(2, cityToCompareWith);
	}

void CTestUserDefinedBackupRestore::TestUserCitiesUpdateAfterRestoreL()
	{
	// Backup the file before we add the user defined time zones
	test.Printf(_L("Start the backup of the user-defined database\n"));
	TRAPD(err, StartBackupL());
	test(err==KErrNone);
	TBool result = SimulateTzBackup(KTzBackupFileFullPath, KTzTestBackupFile);
	test(result);
	TRAP(err, EndBackupL());
	test(err == KErrNone);
	
	// Add a user defined time zone
	test.Printf(_L("Create a user-defined zone\n"));
	TTzRule rule1(0, KMaxTUint16, 60, 0, EAugust,  ETzFixedDate, 0,  0, ETzWallTimeReference, 120);
	TTzRule rule2(0,KMaxTUint16,0,60,EJanuary,ETzFixedDate,0,0,ETzWallTimeReference,120);
	CTzRules* rules = CTzRules::NewL(0, KMaxTUint);
	CleanupStack::PushL(rules);
	rules->AddRuleL(rule1);
	rules->AddRuleL(rule2);

	CTzUserNames* names = CTzUserNames::NewLC(KStandardName1(), KShortStandardName1(), KDaylightSaveName1(), 
	KShortDaylightSaveName1(), KCityNameName1(), KRegionName1());
	CTzId* id = iUserData->CreateL(*rules, *names);
	TInt tzid = id->TimeZoneNumericID();
	delete id;
	CleanupStack::PopAndDestroy(2, rules);
		
	CTzLocalizer* localizer = CTzLocalizer::NewL();
	// Add a city that uses this time zone
	test.Printf(_L("Add a user-defined city to the zone\n"));
	CTzLocalizedCity* userCity = localizer->AddCityL(tzid, KUserAddedCityName1);
	delete userCity;
	
	// Add a user defined time zone
	test.Printf(_L("Add another rule to the database\n"));
	CTzRules* newRules = CTzRules::NewL(0, KMaxTUint);
	CleanupStack::PushL(newRules);
	newRules->AddRuleL(rule1);
	newRules->AddRuleL(rule2);
	
	CTzUserNames* newNames = CTzUserNames::NewLC(KStandardName2(), KShortStandardName2(), KDaylightSaveName2(), 
	KShortDaylightSaveName2(), KCityNameName2(), KRegionName2());
	CTzId* newId = iUserData->CreateL(*newRules, *newNames);
	tzid = newId->TimeZoneNumericID();
	delete newId;
	CleanupStack::PopAndDestroy(2, newRules);
	// Add a city to the new zone
	test.Printf(_L("Add a city to the zone\n"));
	CTzLocalizedCity*	city = localizer->AddCityL(tzid, KUserAddedCityName2);
	delete city;
	delete localizer;
	// Fetch the cities and check the count
	test.Printf(_L("Compare the cities retrieved from the table against the number expected\n"));
	RPointerArray<CTzLocalizedCityRecord> cityRecords;
	CleanupStack::PushL(TCleanupItem(CleanupCityPointerArray, &cityRecords));
	iTz.LocalizationReadCitiesL(cityRecords);
	test(cityRecords.Count() == 2);
	CleanupStack::PopAndDestroy(&cityRecords);
	
	// Restore the file
	test.Printf(_L("Restore the backed up file\n"));
	TRAP(err, StartRestoreL());
	test(err == KErrNone);
	result = SimulateTzRestore(KTzTestBackupFile, KTzBackupFileFullPath) ;
	test(result);
	TRAP(err, EndRestoreL());
	test(err == KErrNone);
	// Fetch the cities from the backed up file and check the count
	test.Printf(_L("Again, compare the cities retrieved from the table against the number expected\n"));
	RPointerArray<CTzLocalizedCityRecord> cityRecords2;
	CleanupStack::PushL(TCleanupItem(CleanupCityPointerArray, &cityRecords2));
	iTz.LocalizationReadCitiesL(cityRecords2);
	test(cityRecords2.Count() == 0);
	CleanupStack::PopAndDestroy(&cityRecords2);
	}

/* Test the update of the cached system time zone rules in Tz server after restore the user defined tz DB 
 * There are three scenarios are to be tested:
 * case 1. Set the Shanghai, which is in the existing database, as the current time zone  
 *    The current time zone should be not be affected after restore the user defined tz DB.
 * case 2. Set the current time zone to a user defined time zone.
 * 	  The current time zone should be reset to the same time zone after restore the user defined tz DB which have the current time zone rules. 
 * case 3. Set the current time zone to a user defined time zone.
 * 	  The current time zone should be reset to the default time zone London after restore the user defined tz DB which doesn't have the current time zone rules. 	
 */
void CTestUserDefinedBackupRestore::TestSystemRulesUpdateAfterRestoreL()
	{
	CTzId* tzid = NULL;
	TUint tzNumId = 0;
	
	_LIT8(KTzShangHai, "Asia/Shanghai");
	tzid = CTzId::NewL(KTzShangHai);
	iTz.SetTimeZoneL(*tzid);
	tzNumId = tzid->TimeZoneNumericID();
	delete tzid;
	
	//case 1
	test(TestSystemRulesUpdateAfterRestoreL(tzNumId, EFalse));
	
	test.Printf(_L("Add a user-defined zone\n"));
	TTzRule rule1(0, KMaxTUint16, 60, 0, EAugust,  ETzFixedDate, 0,  0, ETzWallTimeReference, 120);
	TTzRule rule2(0,KMaxTUint16,0,60,EJanuary,ETzFixedDate,0,0,ETzWallTimeReference,120);
	CTzRules* rules = CTzRules::NewL(0, KMaxTUint);
	CleanupStack::PushL(rules);
	rules->AddRuleL(rule1);
	rules->AddRuleL(rule2);
	CTzUserNames* names = CTzUserNames::NewLC(KStandardName1(), KShortStandardName1(), KDaylightSaveName1(),
	KShortDaylightSaveName1(), KCityNameName1(), KRegionName1());
	tzid = iUserData->CreateL(*rules, *names);
	CleanupStack::PopAndDestroy(2, rules);
	CleanupStack::PushL(tzid);
	iTz.SetTimeZoneL(*tzid);
	tzNumId = tzid->TimeZoneNumericID();
	CleanupStack::PopAndDestroy(tzid);
	
	//case 2
	test(TestSystemRulesUpdateAfterRestoreL(tzNumId, EFalse));
	
	//case 3
	test(TestSystemRulesUpdateAfterRestoreL(KLondonTzId, ETrue));
	}


TBool CTestUserDefinedBackupRestore::TestSystemRulesUpdateAfterRestoreL(TUint aExpectedTzId, TBool aRestoreEmptyFile)
	{
	test.Next(_L("Test that the system cached rules updated correctly after a restore"));
	
	// Backup the user defined data file
	test.Printf(_L("Backup the user defined data file\n"));
	TRAPD(err, StartBackupL());
	test(err==KErrNone);
	TBool result = SimulateTzBackup(KTzBackupFileFullPath, KTzTestBackupFile);
	test(result);
	TRAP(err, EndBackupL());
	test(err == KErrNone);
	
	// Restore the file
	test.Printf(_L("Restore the backed up file\n"));
	TRAP(err, StartRestoreL());
	test(err == KErrNone);
	if(aRestoreEmptyFile)
		{
		//Copy a empty user database to be restored
		_LIT(KTzUserDataEmptyFile, "z:\\testresourcefiles\\tz_userdata_empty.db");
		iServ.CopyFileL(KTzUserDataEmptyFile, KTzTestBackupFile);
		}
	result = SimulateTzRestore(KTzTestBackupFile, KTzBackupFileFullPath) ;
	test(result);
	TRAP(err, EndRestoreL());
	test(err == KErrNone);
	test.Printf(_L("Get the current time zone and check if it is the expected one\n"));
	CTzId* tzid = iTz.GetTimeZoneIdL();
	TUint numId = tzid->TimeZoneNumericID();
	delete tzid;
	return numId == aExpectedTzId;
	}


void CTestUserDefinedBackupRestore::RunTestsL()
	{
	_LIT(KTzServer,"Tzserver*");
	TRAP_IGNORE(iServ.KillProcessL(KTzServer));
	iServ.DeleteFileL(KTzBackupFileFullPath);
	User::LeaveIfError(iTz.Connect());
	iUserData = CTzUserData::NewL(iTz);
	test.Title();
	
	test.Start(_L("Test Access To User Defined Data During Backup"));
	ResetTzUserDataL();
	TestBackupAndRestoreUserDbL(EBackupTest);

	test.Next(_L("Test Access To User Defined Data During Restore"));
	TestBackupAndRestoreUserDbL(ERestoreTest);	

	test.Next(_L("Test Frequently Used Zones Update After Restore With Delete"));
	TestFrequentlyUsedZonesUpdateAfterRestoreDeleteL();
	
	test.Next(_L("Test Frequently Used Zones Update After Restore With Update"));
	TestFrequentlyUsedZonesUpdateAfterRestoreUpdateL();

	test.Next(_L("Test Frequently Used Current Zones Update After Restore With Update"));
	TestFrequentlyUsedCurrentZonesUpdateAfterRestoreUpdateL();

	test.Next(_L("Test User Cities Update After Restore"));
	TestUserCitiesUpdateAfterRestoreL();
	
	test.Next(_L("Test Update system Cached rules After Restore"));
	TestSystemRulesUpdateAfterRestoreL();
	
	test.Next(_L("Test Access To Localization  DB During Backup and Restore"));
	TestBackupAndRestoreLocDbL();	

	ResetTzUserDataL();	
	test.End();

	iTz.Close();	
	TRAP_IGNORE(iServ.KillProcessL(KTzServer));
	iServ.DeleteFileL(KTzBackupFileFullPath);
	iServ.DeleteFileL(KPerFile1);
	iServ.DeleteFileL(KPerFile2);
	}

	
static void DoTestsL()
	{
	BackupClient = conn::CSBEClient::NewL();
	CleanupStack::PushL(BackupClient);

	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);
	
	CTestUserDefinedBackupRestore* backupRestore = CTestUserDefinedBackupRestore::NewL();
	CleanupStack::PushL(backupRestore);

	backupRestore->RunTestsL();

	// cleanup
	CleanupStack::PopAndDestroy(backupRestore);
	CleanupStack::PopAndDestroy(scheduler);
	CleanupStack::PopAndDestroy(BackupClient);
	}


GLDEF_C TInt E32Main()
    {
	__UHEAP_MARK;

	CTrapCleanup* cleanup = CTrapCleanup::New();
	if	(!cleanup)
		{
		return KErrNoMemory;
		}

	TRAPD(ret, DoTestsL());
	test.Printf(_L("Trapped return value from DoTestsL(): %d\n"), ret);		
	test(ret == KErrNone);
	
	test.Close();
	
	delete cleanup;

	__UHEAP_MARKEND;

	return KErrNone;
    }
