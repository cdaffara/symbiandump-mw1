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

#include <tz.h>
#include "ReadOnlyTzDb.h"
#include <f32file.h>
#include "dataprovider.h"
#include <vtzrules.h>

const TUint KDefaultTzNumericId = 0;

// TZ Database is stored in the timezone server's own private area
_LIT(KTzDbFileNameRom,"Z:\\private\\1020383E\\TZDB.DBZ");
_LIT(KTzDbFileNameFlash,"C:\\private\\1020383E\\TZDB.DBZ");
_LIT(KTzDbFileNameFlashCorrupt,"C:\\private\\1020383E\\TZDB.DB~");

CReadOnlyTzDb* CReadOnlyTzDb::NewL(RFs& aFs, CTzSwiObserver& aSwiObserver, MTzDataObserver& aTzDataObserver)
	{
	CReadOnlyTzDb* self = new(ELeave) CReadOnlyTzDb(aFs, aSwiObserver, aTzDataObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CReadOnlyTzDb::CReadOnlyTzDb(RFs& aFs, CTzSwiObserver& aSwiObserver, MTzDataObserver& aTzDataObserver)
	: iFs(aFs), iSwiObserver(aSwiObserver), iTzDataObserver(aTzDataObserver), iTzDbDataFromFlash(EFalse)
	{
	}

void CReadOnlyTzDb::ConstructL()
	{
	// Open the database, map the file into memory and build the entity tables
	InitialiseL();
	// Add observer to watch SWI completion
	iSwiObserver.AddObserverL(this);
	}

//
// Returns the memory address of the start of RAM copy of TZ Database file
//
TInt CReadOnlyTzDb::StartAddress() const 
	{ 
	return iStartAddress; 
	}

//
// Loads the TZ Database from given file into RAM.
//
TInt CReadOnlyTzDb::CopyDatabaseToRam(const TDesC& aTzDbFileName)
  {
	TInt error = KErrNone;
	RFile file;

	TUint fileMode = EFileShareReadersOnly | EFileRead | EFileStream;

	error = file.Open(iFs, aTzDbFileName, fileMode);
	if (error == KErrNone)
		{
		TInt size = 0;
		error = file.Size(size);
		if ( (error == KErrNone) && (size <= 0) )
			{
			error = KErrCorrupt;
			}
		if (error == KErrNone)
			{
			if (iTzDbData) 
				{
				iChunk.Close();
				iTzDbData = NULL;
				}
	
			error = iChunk.CreateLocal(size,size) ;
			if (error == KErrNone )
				{
				// Allocate memory inside the memory chunk
				iTzDbData = new (iChunk.Base()) TUint8 [size];
				// Set a TPtr to "point" at the created memory
				TPtr8 flashDataPtr(iTzDbData, size);

				error = file.Read(0,flashDataPtr,size);
				if (error == KErrNone)
					{
					iStartAddress = reinterpret_cast<TInt>(iTzDbData);
					}
				}
			}
		}
	file.Close();
	return error;
	}
  
TInt CReadOnlyTzDb::InvalidateFlashDatabaseL()
	{
	RFile file;
	CleanupClosePushL(file);	
	TUint fileMode = EFileShareExclusive | EFileWrite | EFileStream;
	TInt error = file.Open(iFs, KTzDbFileNameFlash, fileMode);
	if (error == KErrNone)
		{
		error = file.Rename(KTzDbFileNameFlashCorrupt);
		}
	CleanupStack::PopAndDestroy(&file);

	return error;
	}

void CReadOnlyTzDb::InitialiseL()
	{
	// deny access to Flash-based database 
	// if it is being restored in this precise moment
	// We must not allow access to the ROM-based DB either, 
	// because this might contain outdated data.
	if (iRestoreInProcess)
		{
		User::Leave(KErrNotReady);
		}
		
	//
	// try to copy TZ Database from file in Flash drive
	//
	TInt error = CopyDatabaseToRam(KTzDbFileNameFlash);

	//
	// if it wasn't found or couldn't be opened, try to copy from file in ROM
	//

	if (error != KErrNone)
		{
		User::LeaveIfError(CopyDatabaseToRam(KTzDbFileNameRom));
		iTzDbDataFromFlash = false;
		}
	else
		{
		iTzDbDataFromFlash = true;
		}
		
	//
	// read header and initialise table offsets
	//
	
	TTzHeader* header(reinterpret_cast<TTzHeader*>(iStartAddress) );

	// check that the db version is the expected
	if (header->iVersion != KTzDbVersion)
		{
		if (iTzDbData)
			{
			ReleaseData();
			if (iTzDbDataFromFlash)
			  {
				InvalidateFlashDatabaseL();
			  }
			}
		User::Leave(KErrCorrupt);
		}
	
	const TInt KStringsBaseAddress				= iStartAddress + header->iOffsetToStringTable;
	const TInt KRegionsBaseAddress				= iStartAddress + header->iOffsetToRegionsTable;
	const TInt KRegionalZonesBaseAddress		= iStartAddress + header->iOffsetToRegionalZonesTable;
	const TInt KZonesDataBaseAddress			= iStartAddress + header->iOffsetToZones;
	const TInt KZonesBaseAddress				= iStartAddress + header->iOffsetToZonesTable;
	const TInt KLinksBaseAddress				= iStartAddress + header->iOffsetToLinksTable;
	const TInt KStdTimeAlignmentsBaseAddress	= iStartAddress + header->iOffsetToStdTimeAlignmentsTable;
	const TInt KRuleSetsBaseAddress				= iStartAddress + header->iOffsetToRuleSetsTable;
	const TInt KRuleUsesBaseAddress				= iStartAddress + header->iOffsetToRuleUsesTable;
	const TInt KRuleDefinitionsBaseAddress		= iStartAddress + header->iOffsetToRuleDefinitionsTable;

	//
	// create tables
	//
	iStringsTable = CTzDbStringsTable::NewL(*(reinterpret_cast<TTzStringsTable*>(KStringsBaseAddress)));
	iRegionsTable = CTzDbRegionsTable::NewL(*this, *(reinterpret_cast<TTzRegionsTable*>(KRegionsBaseAddress)));
	iRegionalZonesTable = CTzDbRegionalZonesTable::NewL(*this, *(reinterpret_cast<TTzRegionalZonesTable*>(KRegionalZonesBaseAddress)));
	iZonesTable = CTzDbZonesTable::NewL(*this, *(reinterpret_cast<TTzZonesTable*>(KZonesBaseAddress)), KZonesDataBaseAddress);
	iLinksTable	= CTzDbLinksTable::NewL(*this, *(reinterpret_cast<TTzLinksTable*>(KLinksBaseAddress)));
	iStdTimeAlignmentsTable	= CTzDbStdTimeAlignmentsTable::NewL(*(reinterpret_cast<TTzStdTimeAlignmentsTable*>(KStdTimeAlignmentsBaseAddress)));
	iRuleSetsTable = CTzDbRuleSetsTable::NewL(*this, *(reinterpret_cast<TTzRuleSetsTable*>(KRuleSetsBaseAddress)));
	iRuleUsesTable = CTzDbRuleUsesTable::NewL(*(reinterpret_cast<TTzRuleUsesTable*>(KRuleUsesBaseAddress)));
	iRuleDefinitionsTable = CTzDbRuleDefinitionsTable::NewL(*(reinterpret_cast<TTzRuleDefinitionsTable*>(KRuleDefinitionsBaseAddress)));	
	}

//
// Releases all RAM used by the database
//
void CReadOnlyTzDb::ReleaseData()
	{
	// delete all entity tables that are created at startup
	delete iStringsTable;			iStringsTable = NULL;
	delete iRegionsTable;			iRegionsTable = NULL;
	delete iRegionalZonesTable;		iRegionalZonesTable = NULL;
	delete iZonesTable;				iZonesTable = NULL;
	delete iLinksTable;				iLinksTable = NULL;
	delete iStdTimeAlignmentsTable;	iStdTimeAlignmentsTable = NULL;
	delete iRuleSetsTable;			iRuleSetsTable = NULL;
	delete iRuleUsesTable;			iRuleUsesTable = NULL;
	delete iRuleDefinitionsTable;	iRuleDefinitionsTable = NULL;

	iStartAddress = 0;
	iChunk.Close(); iTzDbData = NULL;
	}

CReadOnlyTzDb::~CReadOnlyTzDb()
	{
	iSwiObserver.RemoveObserver(this);
	ReleaseData();
	}

//
// Returns the CTzId object that corresponds to the location set as default in the database
//
CTzId* CReadOnlyTzDb::GetDefaultTimeZoneIdL()
	{
	if (iZonesTable == NULL)
		{
		InitialiseL();
		}

	CTzId* defTZID = NULL;
	CTzDbZone* defZone = GetZoneL((reinterpret_cast<TTzHeader*>(iStartAddress))->iOffsetToDefaultZone);

	if (defZone)
		{
		CleanupStack::PushL(defZone); // push #1

		HBufC8* fullZoneName = defZone->GetFullZoneNameLC(); // push #2
		defTZID = CTzId::NewL(defZone->LocationId());
		CleanupStack::PushL(defTZID); // push #3
		defTZID->SetIdL(*fullZoneName);

		CleanupStack::Pop(defTZID);
		CleanupStack::PopAndDestroy(2, defZone);
		}

	return defTZID;
	}

void CReadOnlyTzDb::GetTzRulesL(CTzRules& aTzRules, CTzId& aTzId)
	{
	if (iZonesTable == NULL)
		{
		InitialiseL();
		}
		
	TRAPD(err, DoGetTzRulesL(aTzRules, aTzId));
	if ( (err == KErrCorrupt) && (iTzDbData) )
		{
		ReleaseData();
		// if the corruption occurred while accessing the Flash DB, we must flag it 
		// as corrupt before propagating the leave
		if (iTzDbDataFromFlash)
		  {
			InvalidateFlashDatabaseL();
			}
		}
		
	User::LeaveIfError(err);
	}
	
void CReadOnlyTzDb::DoGetTzRulesL(CTzRules& aTzRules, CTzId& aTzId)
	{
	CTzDbZone* zone = FindZoneL(aTzId,ETrue);
	if (zone)
		{
		CleanupStack::PushL(zone);
		zone->GetRulesL(aTzRules);
		CleanupStack::PopAndDestroy(zone);
		}
	}

TBool CReadOnlyTzDb::IsIdInDbL(TUint aTzId)
	{
	if (iZonesTable == NULL)
		{
		InitialiseL();
		}
	CTzDbZone* zone = FindZoneByNumericIdL(aTzId);
	TBool ret = (zone)?ETrue:EFalse;
	delete zone;
	return ret;
	}

//
// Returns the CTzDbZone object corresponding to the location received (as CTzId)
// The supplied CTzId has two different IDs: a numeric one and a name (string ID).
// If the numeric ID is set (>0), it is used to perform the database search. Otherwise,
// the name is used to do the search.
//
// If aFillInLocationIDs is ETrue, it also fills in the unset zone ID in the supplied CTzId
//
CTzDbZone* CReadOnlyTzDb::FindZoneL(CTzId& aLocation, TBool aFillInLocationIDs)
	{
	CTzDbZone* zone = NULL;
	if (aLocation.TimeZoneNumericID() > KDefaultTzNumericId)
		{
		zone = FindZoneByNumericIdL(aLocation.TimeZoneNumericID());
		if (zone && aFillInLocationIDs)
			{
			CleanupStack::PushL(zone); // push #1
			HBufC8* fullZoneName = zone->GetFullZoneNameLC(); // push #2
			aLocation.SetIdL(*fullZoneName);
			CleanupStack::PopAndDestroy(fullZoneName); // pop #2
			CleanupStack::Pop(zone); // pop #1
			}
		}
	else
		{
		zone = FindZoneByNameL(aLocation.TimeZoneNameID());
		if (zone)
			{
			CleanupStack::PushL(zone); // push #1
			aLocation.SetId(zone->LocationId());
			CleanupStack::Pop(zone); // pop #1
			}
		}

	return zone;
	}

//
//
// Looks for a Zone database entity given a location name
// Returns the Zone as a CTzDbZone* (NULL if it can't find
// a Zone in the database that matches the received location)
//
CTzDbZone* CReadOnlyTzDb::FindZoneByNameL(const TDesC8& aLocation)
	{
	// first of all, look in links table
	CTzDbZone* retZone = iLinksTable->FindZoneL(aLocation);

	if (retZone == NULL) 
		{ // received location was the name of an actual zone
		// break up Location in Region and City names: e.g. "Europe/London" = "Europe" + "London"
		TInt separatorOffset = aLocation.Locate('/');
		HBufC8* regionName;
		HBufC8* cityName;
		if (separatorOffset >= 0)
			{
			regionName = aLocation.Left(separatorOffset).AllocLC();  // push #1
			cityName = aLocation.Right(aLocation.Length() - (separatorOffset+1)).AllocLC(); // push #2
			}
		else
			{
			_LIT8(KNullStr,"");
			regionName = KNullStr().AllocLC(); // push #1
			cityName = aLocation.AllocLC(); // push #2
			}
		CTzDbRegion* region = iRegionsTable->FindRegionL(*regionName);
		if (region)
			{
			CleanupStack::PushL(region); // push #3
			retZone = region->FindZoneL(*cityName);
			CleanupStack::PopAndDestroy(region); // pop #3
			} // if (region)  
		
		CleanupStack::PopAndDestroy(2,regionName); // pop #2, #1 - cityName, regionName
		}

	return retZone;
	}

CTzDbZone* CReadOnlyTzDb::FindZoneByNumericIdL(TUint aLocationId)
	{
	return iZonesTable->GetZoneByIdL(aLocationId);
	}

//
//
// The following methods provide access to the entity tables
//

TPtrC8 CReadOnlyTzDb::GetStringL(TUint aOffset) const
	{
	return iStringsTable->GetStringL(aOffset);
	}

const TTzRuleDefinition& CReadOnlyTzDb::GetTRuleDefinition(TUint aOffset) const
	{
	return iRuleDefinitionsTable->GetRuleDefinition(aOffset);
	}

const TTzRegionalZoneIndex& CReadOnlyTzDb::GetTRegionalZoneIndex(TUint aReference) const
	{
	return iRegionalZonesTable->GetTRegionalZoneIndex(aReference);
	}

CTzDbZone* CReadOnlyTzDb::GetZoneL(TUint aReference) const
	{
	return iZonesTable->GetZoneL(aReference);
	}

const TTzZone& CReadOnlyTzDb::GetTZone(TUint aReference) const
	{
	return iZonesTable->GetTZone(aReference);
	}

const TTzStdTimeAlignment& CReadOnlyTzDb::GetTStdTimeAlignment(TUint aReference) const
	{
	return iStdTimeAlignmentsTable->GetTStdTimeAlignment(aReference);
	}

const TTzRuleUse& CReadOnlyTzDb::GetTRuleUse(TUint aReference) const
	{
	return iRuleUsesTable->GetTRuleUse(aReference);
	}

CTzDbRuleSet* CReadOnlyTzDb::GetRuleSetL(TUint aReference) const
	{
	return iRuleSetsTable->GetRuleSetL(aReference);
	}

//
// The following methods (inherited from MBackupRestoreNotificationObserver)
// provide BackupRestore support

void CReadOnlyTzDb::BackupBeginningL()
	{
	// No action needed
	}

void CReadOnlyTzDb::BackupCompletedL()
	{
	// No action needed
	}

void CReadOnlyTzDb::RestoreBeginningL()
	{
	// set flag indicating that restoring has begun
	SetRestoreInProcess();
	}

void CReadOnlyTzDb::RestoreCompletedL()
	{
	// reset flag
	ResetRestoreInProcess();	
	ReleaseData();	
	InitialiseL();
	}

void CReadOnlyTzDb::HandleDatabaseChangeL(RTz::TTzChanges aChange)
	{
	//notify the TZ Data Observer that a change has occurred
	iTzDataObserver.NotifyTZDataStatusChangeL(aChange);

	ReleaseData();
	InitialiseL();
	}

/*
Retrieves the UTC offset for a given numeric time zone id.

@param aTime time at which the UTC offset for given time zone is required.
@param aTzId a numeric time zone id,

@return	TInt offset from UTC (in minutes).
*/
TInt CReadOnlyTzDb::GetOffsetForTimeZoneIdL(const TTime& aTime, TInt aTzId)
	{
	if (iZonesTable == NULL)
		{
		InitialiseL();
		}
	
	TInt offset = 0;
	TDateTime nowDate(aTime.DateTime() );
	
	CTzDbZone* zone = FindZoneByNumericIdL(aTzId);;
	CTzDbStdTimeAlignment* timeAlignment;

	if (zone != NULL)
		{
		//
		// zone has been found
		//			
		CleanupStack::PushL(zone);
		timeAlignment = zone->FindStdTimeAlignmentL
			(
			nowDate.Year(),
			nowDate.Month(),
			nowDate.Day(),
			nowDate.Hour(),
			nowDate.Minute(),
			ETzUtcTimeReference
			);
			
		if(timeAlignment)
			{
			offset = timeAlignment->UtcOffset();
			delete timeAlignment;
			}
		CleanupStack::PopAndDestroy(zone);
		}
			
	return offset;
	}

//
// Called by CTzDbChangeNotifier to notify of a TZ Database change
// 
void CReadOnlyTzDb::NotifyTZDataStatusChangeL(RTz::TTzChanges aChange)
	{
	HandleDatabaseChangeL(aChange);
	}
