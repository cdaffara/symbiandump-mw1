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

#ifndef __TZLOCALIZATION_DB_ACCESSOR_H__
#define __TZLOCALIZATION_DB_ACCESSOR_H__

#include <e32base.h>
#include <d32dbms.h>
#include "TzLocalizationDataTypes.h"
#include <tz.h>

_LIT(KTzMutexName, 	"TZ_GlobalMutex");

/**
Provides access to the time zone localization database
through a RDbs session.
The database is stored in the DBMS private directory
@internalTechnology
*/
class CTzLocalizationDbAccessor : public CBase
	{
	public:
		//Enumerations used for column ordering in the database.
		//The database columns MUST be created in the same order they appear in this enumeration
		enum  TTzCityColumn
			{
			// Enum will used as a DB column index, so it must start at 1
			ETzCityId =1,
			ETzCityCity,
			ETzCityGroup,
			ETzCityResourceId,
			};

		enum TTzZoneColumn
			{
			// Enum will used as a DB column index, so it must start at 1
			ETzZoneId =1,
			ETzZoneStdName,
			ETzZoneDSTName,
			ETzZoneShortStdName,
			ETzZoneShortDSTName,
			ETzZoneCity,
			ETzZoneGroupId,
			ETzZoneResourceId,
            ETzZoneCityIndex,
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

	public:
		//Construction / Destruction
		static CTzLocalizationDbAccessor* NewL();
		static CTzLocalizationDbAccessor* NewLC();
		~CTzLocalizationDbAccessor();

		void PrepareCityViewL(const TDesC& aSqlQuery);
		void PrepareZoneViewL();

        TBool DbNeedsUpdatingL();
        void RecreateDbL(RTz& aTzSession);

	protected:
		CTzLocalizationDbAccessor();
		//Second phase construction
		void ConstructL();
		static void ReleaseMutex(TAny *target) ;

	private:
		TInt CreateFrequentlyUsedZoneTableL();
		TInt CreateUserCityTableL();
		TBool IsTableCreatedL(const TDesC& aTableName) const;
        void CloseDb();
        void OpenDbL();
		

	protected:
		//Member data
		RDbs iDbsSession;						//DBMS server session
		RDbNamedDatabase iLocalizedTimeZoneDb;	//Database
		RDbView iCityView;						//Rowset
		RDbView iZoneView;						//Rowset
		RMutex	iZoneMutex ;                // protecting iZoneView changes
	};

#endif //__TZLOCALIZATION_DB_ACCESSOR_H__
