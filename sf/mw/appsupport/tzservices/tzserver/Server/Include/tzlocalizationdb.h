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

#ifndef __TZLOCALIZATIONDB_H__
#define __TZLOCALIZATIONDB_H__

#include <e32base.h>
#include <d32dbms.h>
#include <tzlocalizationdatatypes.h>
#include <tz.h>
#include "tzsystemdata.h"
#include "tzuserdata.h"
#include "mtimezonedataaccess.h"

class CTzServer;

/**
This class provides access to a database containing the localization information.

The database contains two tables : one called "FrequentlyUsedZones" containing time zones 
and one called "UserCities" containing cities.

The "FrequentlyUsedZones" table contains one entry (row) for each of the frequently used zones 
as defined by the CTzLocalizedTimeZone::TTzFrequentlyUsedZone enum (i.e. 5 rows at the time of 
writing). The table always contains this number of rows. When the database is created all the 
rows are initialized to a "null" value.

The table contains 9 columns containing the following information:
Column 1: The time zone id (note this column is a TUint16 while CTzId uses a TUint32)
Column 2: The standard name of the time zone
Column 3: The DST name of the time zone
Column 4: The short standard name of the time zone
Column 5: The short DST name of the time zone
Column 6: The name of the city that is representative of this time zone
Column 7: The group id of the city that is representative of this time zone
Column 8: The resource id associated with the time zone id.
Column 9: The index of the city in the resource file (note this is not the resource id but the 
position of this city in the list of cities in the resource file)

In other words columns 1 and 8 store the values of CTzLocalizedTimeZone::iTzLocalizedId columns 2 to 5 
store the other members of CTzLocalizedTimeZone.

As we mentioned above this table always has the same number of rows but they may have been initialized to
a "null". The code in CTzLocalizer::PrepareFrequentlyUsedZonesL() checks if this is the case and replaces 
the "null" value with the correct data.

Column 9 holds an index that is used when changing language. This is used in the 
CTzLocalizer::UpdateFrequentlyUsedZonesL() function. When changing language the UpdateFrequentlyUsedZonesL()
code first tries to find a city with the same name in the new resource file. If it can't find a city with
the same name it will use the city that has the same index. The index is simply the position of the city in
the resource file so this code assumes that cities are in the same order in the different resource files. See
CTzLocalizationResourceReader::AddCityArrayFromResourceL() for the code that initializes the index.

Note that this table used to have only 8 columns. Column 9 was added as part of a defect fix in 
October 2006. The CTzLocalizer class will check whether the database has 8 or 9 columns on construction
and upgrade the database format if needed. However because of this the CTzLocalizationDb code must cope
with the absence of the 9th column.
	      
The "UserCities" table contains the list of cities that have been added by the user using the
@see CTzLocalizer::AddCityL() function. The table contains 4 columns containing the following 
information:
Column 1: The id of the time zone in which the city is
Column 2: The name of the city
Column 3: The id of the group this city is part of or 0 if the city doesn't belong to a group
Column 4: The resource id associated with the time zone id.

In other words columns 1 and 2 store the values of CTzLocalizedCity::iTzLocalizedId, column 2 stores
the value of CTzLocalizedCity::iName and column stores the value of CTzLocalizedCity::iGroupId.

The columns can be addressed using the TTzCityColumn enum.

@internalComponent
*/
class CTzLocalizationDb : public CBase, public MTzUserDataChangeObserver, public MTzDataObserver
	{
public:
	static CTzLocalizationDb* NewL(CTzServer& aServer);
	~CTzLocalizationDb();
	
private:
	CTzLocalizationDb(CTzServer& aServer);
	void ConstructL();
	
public:
	void OpenDbL();
	void CloseDb();
	
private:
	void CreateBlankDatabaseL();
	void CreateBlankFrequentlyUsedZoneTableL();
	void CreateBlankUserCityTableL();
	TBool IsInvalidL();
	void RecreateBlankDatabaseL();
	TBool NeedsUpdatingL();
	
public: // from MTzUserDataChangeObserver
	void NotifyUserTzRulesChange(TTzUserDataChange aChange);
	void NotifyUserTzNamesChange(TTzUserDataChange aChange);
	
	// from MTzDataObserver
	void NotifyTZDataStatusChangeL(RTz::TTzChanges);
				
public:
	void ReadCitiesL(RPointerArray<CTzLocalizedCityRecord>& aCities);
	void ReadCitiesL(RPointerArray<CTzLocalizedCityRecord>& aCities, TInt aTimeZoneId);
	void ReadCitiesInGroupL(RPointerArray<CTzLocalizedCityRecord>& aCities, TUint8 aGroupId);
	CTzLocalizedTimeZoneRecord* ReadFrequentlyUsedZoneL(TInt aFrequentlyUsedZone);
	CTzLocalizedCityRecord* ReadCachedTimeZoneCityL(TInt aFrequentlyUsedZone);
	void WriteCityL(const TDesC& aCityName, TUint16 aCityTzId, TUint8 aCityGroupId, TUint aCityTzResourceId);
	void DeleteCityL(const TDesC& aCityName, TUint16 aCityTzId);
	void WriteFrequentlyUsedZoneL(const CTzLocalizedTimeZoneRecord& aTimeZone, 
		const CTzLocalizedCityRecord& aCity, TInt aFrequentlyUsedZone);
	void WriteAllFrequentlyUsedZonesL(const RPointerArray<CTzLocalizedTimeZoneRecord>& aTimeZones, 
		const RPointerArray<CTzLocalizedCityRecord>& aCities);
	
	// methods to handle backup-restore
	void BackupBeginningL();
	void BackupCompletedL();
	void RestoreBeginningL();
	void RestoreCompletedL();
	
private:
	void PrepareCityViewL(RDbView& aCityView, const TDesC& aSqlQuery);
	void PrepareZoneViewL(RDbView& aZoneView);
	HBufC* GetCityQueryLC(const TDesC& aCityName, TInt aCityTimeZoneId);
	HBufC* GetCityQueryLC(TInt aCityTimeZoneId);
	void DoReadCitiesL(RPointerArray<CTzLocalizedCityRecord>& aCities, const TDesC& aSqlString);
	void DoWriteFrequentlyUsedZoneL(RDbView& aZoneView, const CTzLocalizedTimeZoneRecord& aTimeZone,
		const CTzLocalizedCityRecord& aCity);
	TInt GetTimeZoneIdFromTzServerL(CTzSystemDataDb& aSystemDataDb);
	void NotifyUserTzNamesChangeL(TTzUserDataChange aChange);
	TBool ExistsL(TUint aTzId);
	static void ReleaseMutex(TAny* iTarget);
	void DeleteCitiesL(TTzUserDataChange aChange);
	void UpdateTimeZoneAndCityRecordL(RPointerArray<CTzLocalizedTimeZoneRecord>& aFrequentlyUsedZones, 
		RPointerArray<CTzLocalizedCityRecord>& aCachedCities, TInt aTzId);
	static void CleanupTimeZonePointerArray(TAny* aArray);
    static void CleanupCityPointerArray(TAny* aArray);
	TBool ExistsInSystemDbL(TUint aTzId, CTzSystemDataDb& aSystemDataDb);
	void UpdateTimeZoneAndCityRecordForSystemDataL(RPointerArray<CTzLocalizedTimeZoneRecord>& aFrequentlyUsedZones, 
		RPointerArray<CTzLocalizedCityRecord>& aCachedCities, TInt aTzId, CTzSystemDataDb& aSystemDataDb);
	void RestoreDbL();
	TUint32 GetFrequentlyUsedDefaultZoneCenRepKeyL(CTzLocalizedTimeZone::TTzFrequentlyUsedZone aFreqUsedZone);
	CTzLocalizedTimeZoneRecord* DoReadFrequentlyUsedZoneL(TInt aFrequentlyUsedZone);
	void DoWriteAllFrequentlyUsedZonesL(const RPointerArray<CTzLocalizedTimeZoneRecord>& aTimeZones, 
		const RPointerArray<CTzLocalizedCityRecord>& aCities);
	TInt GetFrequentlyUsedDefaultZoneIdL(CTzLocalizedTimeZone::TTzFrequentlyUsedZone aFreqUsedZone,	CTzSystemDataDb& aSystemDataDb);

private:
	enum  TTzCityColumn
		{
		// Enum will used as a DB column index, so it must start at 1
		ETzCityId =1,
		ETzCityCity,
		ETzCityGroup,
		ETzCityResourceId,
		};

	// enum that specifies the maximum length of the strings corresponding
	// to time zone IDs and group IDs
	enum TTzStringLength
		{
		// Group ID is an unsigned 8-bit integer
		EMaxGroupIdStringLength = 3,
		// Time Zone ID is an unsigned 16-bit integer
		EMaxTimeZoneIdStringLength = 5
		};
	
private:
	CTzServer& iServer;
	RDbs iDbsSession;
	RDbNamedDatabase iLocalizedTimeZoneDb;
	/**
	This is used to forbid clients to access the database when a backup/restore 
	is taking place. The database can also be locked permanently if there has been
	an unrecoverable error during an update, in which case only a server reboot
	will unlock it. 
	*/
	TBool iLocked;
	// Protecting iZoneView changes
	RMutex iZoneMutex;
	};

#endif
