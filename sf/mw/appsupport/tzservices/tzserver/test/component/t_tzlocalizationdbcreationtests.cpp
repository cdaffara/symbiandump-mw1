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
#include <e32test.h>
#include <d32dbms.h>
#include "testserver.h"
#include <tzlocalizationdatatypes.h>
#include <tzlocalizer.h>

// These literals are copied from the product code as we don't want to export them
_LIT(KTzLocalizationDbName, "c:TzLocalization.db");
_LIT(KTzLocalizationDbSecurityPolicy,"secure[10206A8B]");
_LIT(KlocDbStart_test_source, "z:\\testresourcefiles\\locDbStart_test.db");
_LIT(KlocDbStart_test_destination, "c:\\private\\100012a5\\DBS_10206A8B_TzLocalization.db");
_LIT(KTimeStamp, "c:\\private\\1020383e\\timestamps");

/**
This is a series of tests that check that the frequently used timezones table and
the user-defined cities table stored in the c:TzLocalization.db DBMS database are
created correctly when they don't exist, are corrupted or need to be upgraded from
an old format to a new format.
*/
class CTzLocalizationDbCreationTests : public CBase
	{
public:
	static CTzLocalizationDbCreationTests* NewLC(RTest& aTest);
	~CTzLocalizationDbCreationTests();
	
private:
	CTzLocalizationDbCreationTests(RTest& aTest);
	void ConstructL();
	
public:
	void RunL();
	
private:
	void RunTzLocalizationDbSchemaUpgradeTestL();
	void RunTzLocalizationDbStartUpdateTestL();
	
private:
	void CheckTzServerNotRunningL();
	void EnsureTzServerNotRunning();
	void EnsureAgendaServerNotRunning();
	void DeleteTzLocalizationDbL();
	void CreateTzLocalizationDbUsingOldSchemaL();
	void CreateFrequentlyUsedTimeZonesTableUsingOldSchemaL(RDbNamedDatabase& aDatabase);
	void CreateUserAddedCitiesTableL(RDbNamedDatabase& aDatabase);
	
private:
	RTest& iTest;
	RPIMTestServer iPIMTestServer;
	RDbs iDbsSession;
	};


CTzLocalizationDbCreationTests* CTzLocalizationDbCreationTests::NewLC(RTest& aTest)
	{
	CTzLocalizationDbCreationTests* self = new(ELeave) CTzLocalizationDbCreationTests(aTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CTzLocalizationDbCreationTests::~CTzLocalizationDbCreationTests()
	{
	iDbsSession.Close();
	iPIMTestServer.Close();
	}

CTzLocalizationDbCreationTests::CTzLocalizationDbCreationTests(RTest& aTest)
	: iTest(aTest)
	{
	}

void CTzLocalizationDbCreationTests::ConstructL()
	{
	User::LeaveIfError(iPIMTestServer.Connect());
	iPIMTestServer.CloseTzSession();
	User::LeaveIfError(iDbsSession.Connect());
	}

void CTzLocalizationDbCreationTests::RunL()
	{
	_LIT(KTestName, "@SYMTestCaseID:PIM-APPSERV-tzlocalizationdbcreationtests-0001 TzLocalizationDbSchemaUpgradeTest");
	
	iTest.Start(KTestName);
	TRAPD(err, RunTzLocalizationDbSchemaUpgradeTestL())
	if (err != KErrNone)
		{
		_LIT(KErrorMsg, "RunTzLocalizationDbSchemaUpgradeTestL with error code: %d\n");
		iTest.Printf(KErrorMsg, err);
		User::Leave(err);
		}

	_LIT(KTestName1, "RunTzLocalizationDbStartUpdateTestL");
	
	iTest.Next(KTestName1);
	TRAP(err, RunTzLocalizationDbStartUpdateTestL());
	if (err != KErrNone)
		{
		_LIT(KErrorMsg1, "RunTzLocalizationDbStartUpdateTestL with error code: %d\n");
		iTest.Printf(KErrorMsg1, err);
		User::Leave(err);
		}
	iTest.End();
	}

void CTzLocalizationDbCreationTests::RunTzLocalizationDbSchemaUpgradeTestL()
	{
	// Delete the current database and replace it with an old version
	DeleteTzLocalizationDbL();
	CreateTzLocalizationDbUsingOldSchemaL();
	
	// Create a CTzLocalizer object and check that the database
	// is upgraded correctly
	CTzLocalizer* localizer = 0;
	TRAPD(err, localizer = CTzLocalizer::NewL());
	if (err != KErrNone)
		{
		_LIT(KErrorMsg, "CTzLocalizer creation failed with error code: %d\n");
		iTest.Printf(KErrorMsg, err);
		User::Leave(err);
		}
	delete localizer;
	
	// Creation of the CTzLocalizer should have caused a database upgrade
	RDbNamedDatabase tzLocalizationDb;
	User::LeaveIfError(tzLocalizationDb.Open(iDbsSession,KTzLocalizationDbName,KTzLocalizationDbSecurityPolicy));
	CleanupClosePushL(tzLocalizationDb);
	RDbView zoneView;
	CleanupClosePushL(zoneView);
	_LIT(KReadZoneView,"SELECT * FROM FrequentlyUsedZones");
	User::LeaveIfError(zoneView.Prepare(tzLocalizationDb, TDbQuery(KReadZoneView), zoneView.EUpdatable));
	User::LeaveIfError(zoneView.EvaluateAll());
	zoneView.Reset();
	const TInt KExpectedNumberOfColumns = 9;
	_LIT(KMsg1, "Expected number of columns: %d\n");
	iTest.Printf(KMsg1, KExpectedNumberOfColumns);
	const TInt colCount = zoneView.ColCount();
	_LIT(KMsg2, "Actual number of columns: %d\n");
	iTest.Printf(KMsg2, colCount);
	if (colCount != KExpectedNumberOfColumns)
		{
		iTest(EFalse);
		}
	CleanupStack::PopAndDestroy(&zoneView);
	CleanupStack::PopAndDestroy(&tzLocalizationDb);
	
	// Cleanup database file
	DeleteTzLocalizationDbL();
	}
/* Localization Database start up
 * Verify that the localization database start-up copes well when it contains some
 * time zone infomation whose Tz ID no longer exists in the TZ database.
 */
void CTzLocalizationDbCreationTests::RunTzLocalizationDbStartUpdateTestL()
	{
	//Make sure the Tz server is not running.
	EnsureTzServerNotRunning();
	//Copy a localization DB contained few frequently used Tz zones which do not exist in the current Tz database.
	iTest.Printf(_L("Copy test file DBS_10206A8B_TzLocalization.db"));
	iPIMTestServer.CopyFileL(KlocDbStart_test_source ,KlocDbStart_test_destination);
	iPIMTestServer.DeleteFileL(KTimeStamp);
	
	//Connect to the Tz server which result in the localization DB updating its tables.
	iTest.Printf(_L("Connect to the Tz server"));
	RTz tz;
	CleanupClosePushL(tz);
	User::LeaveIfError(tz.Connect());
	CTzId* tzid = tz.GetTimeZoneIdL();
	TUint id = tzid->TimeZoneNumericID();
	delete tzid;
	
	//Carry some basic operations which access to the localization DB.
	iTest.Printf(_L("Access to the localization DB"));
	CTzLocalizer* localizer = CTzLocalizer::NewL();
	CleanupStack::PushL(localizer);
	
	CTzLocalizedTimeZone* locTzId = localizer->GetFrequentlyUsedZoneL(CTzLocalizedTimeZone::ECurrentZone);
	iTest (locTzId->TimeZoneId() == id);
	delete locTzId;
	
	CTzLocalizedCity* locCity = localizer->GetFrequentlyUsedZoneCityL(CTzLocalizedTimeZone::ECurrentZone);
	iTest (locCity->TimeZoneId() == id);
	delete locCity;

	//Disconnect to the Tz server and delete the localization DB
	CleanupStack::PopAndDestroy(2, &tz);
	iPIMTestServer.DeleteFileL(KlocDbStart_test_destination);
	}

void CTzLocalizationDbCreationTests::CheckTzServerNotRunningL()
	{
	_LIT(KTzServerProcessPattern, "tzserver.exe*");
	TFindProcess finder(KTzServerProcessPattern);
	TFileName foundName;
	while (finder.Next(foundName) == KErrNone)
		{
		RProcess proc;
		TInt err = proc.Open(foundName);
		if (err == KErrNone)
			{
			if (proc.ExitType() == EExitPending)
				{
				_LIT(KErrorMsg, "TzServer must not be running\n");
				iTest.Printf(KErrorMsg);
				User::Leave(KErrNotReady);
				}
			}
		}
	}


void CTzLocalizationDbCreationTests::EnsureAgendaServerNotRunning()
	{
	_LIT(KAgendaServerProcessPattern, "agsvexe.exe*");
	TFindProcess finder(KAgendaServerProcessPattern);
	TFileName foundName;
	while (finder.Next(foundName) == KErrNone)
		{
		RProcess proc;
		TInt err = proc.Open(foundName);
		if (err == KErrNone)
			{
			if (proc.ExitType() == EExitPending)
				{
				_LIT(KInfoMsg, "INFO: Agenda server must not be running - killing it now.\n");
				iTest.Printf(KInfoMsg);
				TRAP_IGNORE(iPIMTestServer.KillProcessL(KAgendaServerProcessPattern));
				}
			}
		}
	}

void CTzLocalizationDbCreationTests::EnsureTzServerNotRunning()
	{
	_LIT(KTzServerProcessPattern, "tzserver.exe*");
	TFindProcess finder(KTzServerProcessPattern);
	TFileName foundName;
	while (finder.Next(foundName) == KErrNone)
		{
		RProcess proc;
		TInt err = proc.Open(foundName);
		if (err == KErrNone)
			{
			if (proc.ExitType() == EExitPending)
				{
				_LIT(KInfoMsg, "INFO: TzServer must not be running - killing it now.\n");
				iTest.Printf(KInfoMsg);
				TRAP_IGNORE(iPIMTestServer.KillProcessL(KTzServerProcessPattern));
				}
			}
		}
	}

void CTzLocalizationDbCreationTests::DeleteTzLocalizationDbL()
	{
	EnsureAgendaServerNotRunning();
	EnsureTzServerNotRunning();
	
	_LIT(KTzLocalizationDbFilePath, "c:\\private\\100012a5\\DBS_10206A8B_TzLocalization.db");
	TRAPD(err, iPIMTestServer.DeleteFileL(KTzLocalizationDbFilePath));
	if (err != KErrNone)
		{
		_LIT(KErrorMsg, "Database deletion failed with error code: %d\n");
		iTest.Printf(KErrorMsg, err);
		User::Leave(err);
		}
	}

void CTzLocalizationDbCreationTests::CreateTzLocalizationDbUsingOldSchemaL()
	{
	EnsureAgendaServerNotRunning();
	EnsureTzServerNotRunning();
	
	RDbNamedDatabase tzLocalizationDb;
	CleanupClosePushL(tzLocalizationDb);
	TInt err = tzLocalizationDb.Create(iDbsSession, KTzLocalizationDbName, KTzLocalizationDbSecurityPolicy);
	if (err != KErrNone)
		{
		_LIT(KErrorMsg, "Database creation failed with error code: %d\n");
		iTest.Printf(KErrorMsg, err);
		User::Leave(err);
		}
	CreateFrequentlyUsedTimeZonesTableUsingOldSchemaL(tzLocalizationDb);
	CreateUserAddedCitiesTableL(tzLocalizationDb);
	CleanupStack::PopAndDestroy(&tzLocalizationDb);
	}

/**
This is mostly a copy/paste of CTzLocalizationDb::CreateFrequentlyUsedTimeZonesTableL() but 
modified so that it creates a database without the city index column.
*/
void CTzLocalizationDbCreationTests::CreateFrequentlyUsedTimeZonesTableUsingOldSchemaL(RDbNamedDatabase& aDatabase)
	{
	//Table and Column names for Cached Time Zone Table
	//These text strings are never visible and do not need localizing
	_LIT(KCZTableName, "FrequentlyUsedZones");
	_LIT(KCZTableTzIdCol, "TzId");
	_LIT(KCZTableStdCol, "StandardName");
	_LIT(KCZTableDSTCol, "DSTName");
	_LIT(KCZTableShortStdCol, "ShortStandardName");
	_LIT(KCZTableShortDSTCol, "ShortDSTName");
	_LIT(KCZTableCityCol, "City");
	_LIT(KCZTableCityGroupCol, "GroupId");
	_LIT(KCZTableResourceIdCol, "ResourceId");
	
	// Create the columns for the cached zones table
	RArray<TDbCol> cachedTableCols;
	CleanupClosePushL(cachedTableCols);
	cachedTableCols.AppendL(TDbCol(KCZTableTzIdCol,			EDbColUint16));
	cachedTableCols.AppendL(TDbCol(KCZTableStdCol, 			EDbColText));
	cachedTableCols.AppendL(TDbCol(KCZTableDSTCol, 			EDbColText));
	cachedTableCols.AppendL(TDbCol(KCZTableShortStdCol, 	EDbColText));
	cachedTableCols.AppendL(TDbCol(KCZTableShortDSTCol, 	EDbColText));
	cachedTableCols.AppendL(TDbCol(KCZTableCityCol, 		EDbColText));
	cachedTableCols.AppendL(TDbCol(KCZTableCityGroupCol, 	EDbColUint8));
	cachedTableCols.AppendL(TDbCol(KCZTableResourceIdCol, 	EDbColUint32));
	// We do not add the city index column as we want to create a database using
	// the old schema
    
	// Create the columnset - add the columns
	// Columns MUST be added in the same order they appear in TTzZoneColumn
	CDbColSet* frequentlyUsedTimeZonesColSet = CDbColSet::NewLC();
	TInt numCols = cachedTableCols.Count();
	for(TInt i = 0; i < numCols; ++i)
		{
		frequentlyUsedTimeZonesColSet->AddL(cachedTableCols[i]);
		}

	// Create the Cached Time Zone table
	User::LeaveIfError(aDatabase.CreateTable(KCZTableName, *frequentlyUsedTimeZonesColSet));

	//Open the newly created table
	RDbView zoneView;
	CleanupClosePushL(zoneView);
	_LIT(KReadZoneView,"SELECT * FROM FrequentlyUsedZones");
	User::LeaveIfError(zoneView.Prepare(aDatabase, TDbQuery(KReadZoneView), zoneView.EUpdatable));
	User::LeaveIfError(zoneView.EvaluateAll());
	zoneView.Reset();
	//Populate with initial (blank) data.

	_LIT(KEmptyString," ");

	enum TTzOldSchemaZoneColumn
		{
		// Enum will used as a DB column index, so it must start at 1
		ETzZoneId = 1,
		ETzZoneStdName,
		ETzZoneDSTName,
		ETzZoneShortStdName,
		ETzZoneShortDSTName,
		ETzZoneCity,
		ETzZoneGroupId,
		ETzZoneResourceId,
		};
	
	for (TInt x = 0; x < CTzLocalizedTimeZone::ECachedTimeZones; ++x)
		{
		// Insert empty row
		zoneView.InsertL();
		// Fill the table with blank data
		zoneView.SetColL(ETzZoneId,			0);
		zoneView.SetColL(ETzZoneStdName,		KEmptyString);
		zoneView.SetColL(ETzZoneDSTName,		KEmptyString);
		zoneView.SetColL(ETzZoneShortStdName,	KEmptyString);
		zoneView.SetColL(ETzZoneShortDSTName,	KEmptyString);
		zoneView.SetColL(ETzZoneCity,			KEmptyString);
		zoneView.SetColL(ETzZoneGroupId,		0);
		zoneView.SetColL(ETzZoneResourceId,	0);
		zoneView.PutL(); // Complete insertion
		}
	
	CleanupStack::PopAndDestroy(&zoneView);
	CleanupStack::PopAndDestroy(frequentlyUsedTimeZonesColSet);
	CleanupStack::PopAndDestroy(&cachedTableCols);
	}

/**
This is a copy/paste of CTzLocalizationDb::CreateUserAddedCitiesTableL() that 
we can't use as it is not exported.
*/
void CTzLocalizationDbCreationTests::CreateUserAddedCitiesTableL(RDbNamedDatabase& aDatabase)
	{
	//Table and Column names for User Added Cities Table
	//These text strings are never visible and do not need localizing
	_LIT(KUCTableName, "UserCities");
	_LIT(KUCTableTzId, "TzId");
	_LIT(KUCTableCity, "City");
	_LIT(KUCTableGroup, "GroupId");
	_LIT(KUCTableResourceId, "ResourceId");
	
	//Create the columns for the user aded cities table
	RArray<TDbCol> cityTableCols;
	CleanupClosePushL(cityTableCols);
	cityTableCols.AppendL(TDbCol(KUCTableTzId,			EDbColUint16));
	cityTableCols.AppendL(TDbCol(KUCTableCity,			EDbColText));
	cityTableCols.AppendL(TDbCol(KUCTableGroup,			EDbColUint8));
	cityTableCols.AppendL(TDbCol(KUCTableResourceId,	EDbColUint32));

	// Create the columnset - add the columns
	// Columns MUST be added in the same order they appear in TTzCityColumn
	CDbColSet* userAddedCitiesColSet = CDbColSet::NewLC();
	TInt numCols = cityTableCols.Count();
	for(TInt i = 0; i < numCols; ++i)
		{
		userAddedCitiesColSet->AddL(cityTableCols[i]);
		}

	// Create the User City table
	User::LeaveIfError(aDatabase.CreateTable(KUCTableName, *userAddedCitiesColSet));
	
	CleanupStack::PopAndDestroy(userAddedCitiesColSet);
	CleanupStack::PopAndDestroy(&cityTableCols);
	}

static void DoTestsL(RTest& aTest)
	{
	CTzLocalizationDbCreationTests* tests = CTzLocalizationDbCreationTests::NewLC(aTest);
	tests->RunL();
	CleanupStack::PopAndDestroy(tests);
	}

/**

@SYMTestCaseID	PIM-APPSERV-tzlocalizationdbcreationtests-0001

*/

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

	_LIT(KTestName, "t_tzlocalizationdbcreationtests");
	RTest test(KTestName);
	TRAPD(ret, DoTestsL(test));
	test.Printf(_L("Trapped return value from DoTestsL(): %d\n"), ret);
	test(ret == KErrNone);
	test.Close();
	
	delete scheduler;
	delete trapCleanup;	

	__UHEAP_MARKEND;

	return (KErrNone);
    }
