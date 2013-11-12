// Copyright (c) 2004-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include "TzLocalizationDbAccessor.h"
#include "TzLocalizationDataTypes.h"

//==================================================================
// Database location and access policy
//==================================================================
_LIT(KTzLocalizationDbName,"c:TzLocalization.db");
_LIT(KTzLocalizationDbSecurityPolicy,"secure[10206A8B]");
//==================================================================

//==================================================================
//Table and Column names for Cached Time Zone Table
//These text strings are never visible and do not need localizing
//==================================================================
_LIT(KCZTableName, 			"FrequentlyUsedZones");
_LIT(KCZTableTzIdCol, 		"TzId");
_LIT(KCZTableStdCol, 		"StandardName");
_LIT(KCZTableDSTCol, 		"DSTName");
_LIT(KCZTableShortStdCol,	"ShortStandardName");
_LIT(KCZTableShortDSTCol, 	"ShortDSTName");
_LIT(KCZTableCityCol, 		"City");
_LIT(KCZTableCityGroupCol, 	"GroupId");
_LIT(KCZTableResourceIdCol,	"ResourceId");
_LIT(KCZTableCityIndexCol,	"CityIndex");

//==================================================================
//Table and Column names for User Added Cities Table
//These text strings are never visible and do not need localizing
//==================================================================
_LIT(KUCTableName, 			"UserCities");
_LIT(KUCTableTzId, 			"TzId");
_LIT(KUCTableCity, 			"City");
_LIT(KUCTableGroup, 		"GroupId");
_LIT(KUCTableResourceId,	"ResourceId");

/**
Allocates and constructs a new CTzLocalizationDbAccessor
@return the newly constructed CTzLocalizationDbAccessor
@internalTechnology
*/
CTzLocalizationDbAccessor* CTzLocalizationDbAccessor::NewL()
	{
	CTzLocalizationDbAccessor* self = CTzLocalizationDbAccessor::NewLC();
	CleanupStack::Pop(self);
	return self;
	}

/**
Allocates and constructs a new CTzLocalizationDbAccessor
The pointer to the new object is left on the cleanup stack
@return the newly constructed CTzLocalizationDbAccessor
@internalTechnology
*/
CTzLocalizationDbAccessor* CTzLocalizationDbAccessor::NewLC()
	{
	CTzLocalizationDbAccessor* self = new (ELeave) CTzLocalizationDbAccessor();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CTzLocalizationDbAccessor::CTzLocalizationDbAccessor()
	{
	}

/**
Second phase construction
Connects to a database session and opens the database.  If the database doesn't exist it is created
and populated with blank time zone data
@internalTechnology
*/
void CTzLocalizationDbAccessor::ConstructL()
	{
    OpenDbL();
    }

void CTzLocalizationDbAccessor::OpenDbL()
	{
	if (iZoneMutex.OpenGlobal(KTzMutexName) != KErrNone)
		{
		User::LeaveIfError(iZoneMutex.CreateGlobal(KTzMutexName)) ;
		}        
	//Connect to database server session
	User::LeaveIfError(iDbsSession.Connect());
	//Attempt to open the database
	TInt error = iLocalizedTimeZoneDb.Open(iDbsSession,KTzLocalizationDbName,KTzLocalizationDbSecurityPolicy);
	if (error == KErrNotFound)
		{
		//Database file doesn't exist.  Attempt to create a new one.
		error = iLocalizedTimeZoneDb.Create(iDbsSession,KTzLocalizationDbName,KTzLocalizationDbSecurityPolicy);
	 	User::LeaveIfError(error); 
 	 
 		//Check whether tables exist and create them if they do not	 
 		if (!IsTableCreatedL(KCZTableName)) 
 			{ 
			User::LeaveIfError(CreateFrequentlyUsedZoneTableL()); 
 			} 
  		if (!IsTableCreatedL(KUCTableName)) 
 			{ 
 			User::LeaveIfError(CreateUserCityTableL()); 
 			} 
  		}
	}

	
/**
Destructor
Closes the database tables, the database and the database session
@internalTechnology
*/
CTzLocalizationDbAccessor::~CTzLocalizationDbAccessor()
	{
    CloseDb();
    iDbsSession.Close();
    }

void CTzLocalizationDbAccessor::CloseDb()
    {
	iCityView.Close();
	iZoneView.Close();
	iLocalizedTimeZoneDb.Close();
	if (iZoneMutex.IsHeld())    // it is a leave, so release mutex
		{
		iZoneMutex.Signal();
		}       
	iZoneMutex.Close();
	}

// Close and delete the DB and create a new one in the latest format
void CTzLocalizationDbAccessor::RecreateDbL(RTz& aTzSession)
    {
    CloseDb();
    aTzSession.LocalizationCloseDbL();
    const TUid KTzLocalizationDbSecurityPolicyUid = {0x10206A8B};
    User::LeaveIfError(iDbsSession.DeleteDatabase(KTzLocalizationDbName(),KTzLocalizationDbSecurityPolicyUid));
    aTzSession.LocalizationOpenDbL();
    OpenDbL();
    }

TBool CTzLocalizationDbAccessor::DbNeedsUpdatingL()
    {
    // Check the version of the database
    // There are 9 columns as of Oct 2006
    PrepareZoneViewL();
    if (iZoneView.ColCount() == ETzZoneCityIndex)
        {
        return EFalse;
        }
    return ETrue;
     }
/**
Releases the mutex if leave occurs between wait and signal.
@internalTechnology
*/
void CTzLocalizationDbAccessor::ReleaseMutex(TAny *target)
{
	RMutex *mutex = static_cast<RMutex*> (target) ;	
	mutex->Signal() ;	
}

/**
Creates the Cached Zone Table if it doesn't already exist.
@return KErrNone if succesful, or one of the DBMS Leave codes
@internalTechnology
*/
TInt CTzLocalizationDbAccessor::CreateFrequentlyUsedZoneTableL()
	{
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
    cachedTableCols.AppendL(TDbCol(KCZTableCityIndexCol,	EDbColInt32));

	// Create the columnset - add the columns
	// Columns MUST be added in the same order they appear in TTzZoneColumn
	CDbColSet* FrequentlyUsedZoneColSet = CDbColSet::NewLC();
	TInt numCols = cachedTableCols.Count();
	for(TInt i = 0; i < numCols; ++i)
		{
		FrequentlyUsedZoneColSet->AddL(cachedTableCols[i]);
		}

	// Create the Cached Time Zone table
	TInt error = iLocalizedTimeZoneDb.CreateTable(KCZTableName,*FrequentlyUsedZoneColSet);
	User::LeaveIfError(error);

	//Open the newly created table
	PrepareZoneViewL();
	iZoneView.Reset();
	//Populate with initial (blank) data.

	_LIT(KEmptyString," ");

	for (TInt x = 0; x < CTzLocalizedTimeZone::ECachedTimeZones; ++x)
		{
		iZoneMutex.Wait() ;
		CleanupStack::PushL(TCleanupItem(ReleaseMutex,&iZoneMutex));
		// Insert empty row
		iZoneView.InsertL();
		// Fill the table with blank data
		iZoneView.SetColL(ETzZoneId,			0);
		iZoneView.SetColL(ETzZoneStdName,		KEmptyString);
		iZoneView.SetColL(ETzZoneDSTName,		KEmptyString);
		iZoneView.SetColL(ETzZoneShortStdName,	KEmptyString);
		iZoneView.SetColL(ETzZoneShortDSTName,	KEmptyString);
		iZoneView.SetColL(ETzZoneCity,			KEmptyString);
		iZoneView.SetColL(ETzZoneGroupId,		0);
		iZoneView.SetColL(ETzZoneResourceId,	0);
        iZoneView.SetColL(ETzZoneCityIndex,		0);
		iZoneView.PutL(); // Complete insertion
		CleanupStack::Pop() ;
		iZoneMutex.Signal() ;
		}
	
	iZoneView.Close();

	CleanupStack::PopAndDestroy(FrequentlyUsedZoneColSet);
	CleanupStack::PopAndDestroy(&cachedTableCols); //cachedTableCols
	return error;
	}

/**
Creates the user added city database table if it doesn't exist
@return KErrNone if succesful, or one of the DBMS Leave codes
@internalTechnology
*/
TInt CTzLocalizationDbAccessor::CreateUserCityTableL()
	{
	//Create the columns for the user aded cities table
	RArray<TDbCol> cityTableCols;
	CleanupClosePushL(cityTableCols);
	cityTableCols.AppendL(TDbCol(KUCTableTzId,			EDbColUint16));
	cityTableCols.AppendL(TDbCol(KUCTableCity,			EDbColText));
	cityTableCols.AppendL(TDbCol(KUCTableGroup,			EDbColUint8));
	cityTableCols.AppendL(TDbCol(KUCTableResourceId,	EDbColUint32));

	// Create the columnset - add the columns
	// Columns MUST be added in the same order they appear in TTzCityColumn
	CDbColSet* userCityColSet = CDbColSet::NewLC();
	TInt numCols = cityTableCols.Count();
	for(TInt i = 0; i < numCols; ++i)
		{
		userCityColSet->AddL(cityTableCols[i]);
		}

	// Create the User City table
	TInt error = iLocalizedTimeZoneDb.CreateTable(KUCTableName,*userCityColSet);
	
	CleanupStack::PopAndDestroy(userCityColSet);
	CleanupStack::PopAndDestroy(); //cityTableCols

	return error;
	}


/**
Opens the a view on the city table and returns a reference to it.
The view should be released after use with a call to CloseCityView()
@param aSql A sql string to prepare the view with
@return reference to opened iCityTable
@internalTechnology
*/
void CTzLocalizationDbAccessor::PrepareCityViewL(const TDesC& aSqlQuery)
	{
	User::LeaveIfError(
		iCityView.Prepare(iLocalizedTimeZoneDb,
		TDbQuery(aSqlQuery),
		iCityView.EUpdatable)
		);
	User::LeaveIfError(iCityView.EvaluateAll());
	}


/**
Opens the zone table and returns a reference
@return reference to the opened iZoneTable
@internalTechnology
*/
void CTzLocalizationDbAccessor::PrepareZoneViewL()
	{
	_LIT(KReadZoneView,"SELECT * FROM FrequentlyUsedZones");
	User::LeaveIfError(
		iZoneView.Prepare(iLocalizedTimeZoneDb,
		TDbQuery(KReadZoneView),
		iZoneView.EUpdatable)
		);

	User::LeaveIfError(iZoneView.EvaluateAll());
	}
TBool CTzLocalizationDbAccessor::IsTableCreatedL(const TDesC& aTableName) const 
  	{ 
   	TBool result = EFalse; 
  	CDbTableNames* tableNames = iLocalizedTimeZoneDb.TableNamesL(); 
	CleanupStack::PushL(tableNames); 
	if(tableNames) 
		{ 
		const TInt count = tableNames->Count(); 
  		for(TInt i = 0; i < count; i++) 
			{ 		 
			if((*tableNames)[i] == aTableName) 
				{ 
				result = ETrue; 
				break; 
				} 
			} 
		} 
		CleanupStack::PopAndDestroy(tableNames); 
 	return result;	 
 	} 

