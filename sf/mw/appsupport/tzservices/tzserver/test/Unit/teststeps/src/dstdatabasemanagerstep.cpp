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

#include "dstdatabasemanagerstep.h"
#include <utf.h>
#include "ReadOnlyTzDb.h"

_LIT(KDefaultZoneTag, "DefaultZone");
_LIT(KNumberOfTAChecks, "TimeAlignmentChecks");
_LIT(KTACheck, "TACheck%D");
_LIT(KLocationTag, "Location");
_LIT(KYearTag, "Year");
_LIT(KMonthTag, "Month");
_LIT(KDayTag, "Day");
_LIT(KHourTag, "Hour");
_LIT(KMinuteTag, "Minute");
_LIT(KUntilYearTag, "UntilYear");
_LIT(KUntilMonthTag, "UntilMonth");
_LIT(KUntilDayOfMonthTag, "UntilDayOfMonth");
_LIT(KUntilTimeInMinutesTag, "UntilTimeInMinutes");
_LIT(KUntilTimeReferenceTag, "UntilTimeReference");
_LIT(KNumberOfTimeZonesTag, "NumberOfTimeZones");
_LIT(KUtcOffsetTag, "UTCOffset");

_LIT(KDefZoneCheck, "\"Default Zone Check\"");
_LIT(KAllTAChecks, "\"Time Alignment Integrity Checks\"");
_LIT(KBackupRegistrationCheck, "\"Backup Registration Check\"");

_LIT(KStartYear, "<b><font color=\"blue\">DATABASE START YEAR: %D</font></b>");
_LIT(KStartYearWarning, "<b><font color=\"blue\">Tests with dates before the start year are likely to fail.</font></b>");
_LIT(KTimeZoneBackupRegistrationFullPath, "Z:\\private\\1020383E\\backup_registration.xml");

void CDstDatabaseManagerStep::DestroyTestUnit()
	{
	delete iReadOnlyTzDb;
	iReadOnlyTzDb = NULL;
	delete iTzSwiObserver;
	iTzSwiObserver = NULL;
	iFs.Close();
	}

CDstDatabaseManagerStep::~CDstDatabaseManagerStep()
	{
	}

CDstDatabaseManagerStep::CDstDatabaseManagerStep(CDstUnitServer &aServer) : CDstUnitStep(aServer)
	{
	SetTestStepName(KDstDatabaseManagerStep);
	}

TInt CDstDatabaseManagerStep::CreateAndRunTestUnitL()
	{
	iFs.Connect();
	iTzSwiObserver = CTzSwiObserver::NewL();
	TInt testFault = KErrUnknown;
	iReadOnlyTzDb = CReadOnlyTzDb::NewL(iFs, *iTzSwiObserver, *this);
	iStartAddress = iReadOnlyTzDb->StartAddress();
	
	//
	// Output Database configuration options
	//

	TInt dbStartYear = GetStartYear();

	INFO_PRINTF2(KStartYear,dbStartYear);
	INFO_PRINTF1(KStartYearWarning);
	
	//
	// Default Zone Check
	//
	TBool defZoneCheck = DoDefaultZoneCheckL();

	//
	// Time Alignment Integrity Checks
	//
	TBool taCheck = DoTimeAlignmentChecksL();

	//
	// Backup Registration File Check
	//
	TBool backupCheck = DoBackupRegistrationFileCheckL();
	
	if ((taCheck) && (backupCheck))
		{
		testFault = KErrNone;
		}
	else
		{
		if (!defZoneCheck)
			ERR_PRINTF2(_L("Failed %S"),&KDefZoneCheck);
		if (!taCheck)
			ERR_PRINTF2(_L("Failed %S"),&KAllTAChecks);
		if (!backupCheck)
			ERR_PRINTF2(_L("Failed %S"),&KBackupRegistrationCheck);
		}

	delete iReadOnlyTzDb;
	iReadOnlyTzDb = NULL;
	return testFault;
	}

//

TInt CDstDatabaseManagerStep::GetStartYear() const
	{
	TTzHeader* header = reinterpret_cast<TTzHeader*>(iStartAddress);
	return header->iStartYear;
	}

//

TBool CDstDatabaseManagerStep::DoDefaultZoneCheckL()
	{
	TBool result = EFalse;

	HBufC* expectedDefZoneName = HBufC::NewLC(50); // push
	TPtrC expectedDefZoneNamePtr(expectedDefZoneName->Des());
	GetStringFromConfig(ConfigSection(),KDefaultZoneTag,expectedDefZoneNamePtr);

	CTzDbZone* defZone = iReadOnlyTzDb->GetZoneL((reinterpret_cast<TTzHeader*>(iReadOnlyTzDb->StartAddress()))->iOffsetToDefaultZone);
	CleanupStack::PushL(defZone); // push
	
	if (defZone)
		{
		HBufC* defZoneName = HBufC::NewL(50);
		CleanupStack::PushL(defZoneName); // push
		HBufC8* defZoneName8 = defZone->GetFullZoneNameLC(); // push
		TPtr8 defZoneNamePtr8 = defZoneName8->Des();
		TPtr16 defZoneNamePtr = defZoneName->Des();
		CnvUtfConverter::ConvertToUnicodeFromUtf8(defZoneNamePtr, defZoneNamePtr8);

		if (!defZoneNamePtr.CompareC(expectedDefZoneNamePtr))
			{
			result = ETrue;
			}

		CleanupStack::PopAndDestroy(2,defZoneName); // pop 2
		}
	CleanupStack::PopAndDestroy(2,expectedDefZoneName);
	return result;
	}

//

TBool CDstDatabaseManagerStep::DoTimeAlignmentChecksL()
	{
	TInt taChecks;
	GetIntFromConfig(ConfigSection(), KNumberOfTAChecks, taChecks);
	if (taChecks == 0) 
		return ETrue;
	
	HBufC* taSection = NULL;
	TPtrC taSectionPtr;
	TBuf<50> taCheckTag;

	TPtrC location;
	TInt year;
	TInt month;
	TInt day;
	TInt hour;
	TInt minute;

	TInt expUntilYear;
	TInt expUntilMonth;
	TInt expUntilDay;
	TInt expUntilTimeInMinutes;
	TInt expUntilTimeReference;
	TInt expTimeZonesCount;
	TInt expUtcOffset;

	TTzHeader* header = (TTzHeader*)iStartAddress;
	TBool isOk = ETrue;
	for (TInt i = 0; (i < taChecks) && isOk; i++)
		{
		isOk = EFalse;

		taCheckTag.Format(KTACheck,i+1);
		taSection = HBufC::NewLC(50); // push #1
		taSectionPtr.Set(taSection->Des());
		GetStringFromConfig(ConfigSection(),taCheckTag,taSectionPtr);

		GetStringFromConfig(taSectionPtr, KLocationTag, location);
		GetIntFromConfig(taSectionPtr, KYearTag, year);
		GetIntFromConfig(taSectionPtr, KMonthTag, month);
		GetIntFromConfig(taSectionPtr, KDayTag, day);
		GetIntFromConfig(taSectionPtr, KHourTag, hour);
		GetIntFromConfig(taSectionPtr, KMinuteTag, minute);

		GetIntFromConfig(taSectionPtr, KUntilYearTag, expUntilYear);
		GetIntFromConfig(taSectionPtr, KUntilMonthTag, expUntilMonth);
		GetIntFromConfig(taSectionPtr, KUntilDayOfMonthTag, expUntilDay);
		GetIntFromConfig(taSectionPtr, KUntilTimeInMinutesTag, expUntilTimeInMinutes);
		GetIntFromConfig(taSectionPtr, KUntilTimeReferenceTag, expUntilTimeReference);
		GetIntFromConfig(taSectionPtr, KNumberOfTimeZonesTag, expTimeZonesCount);
		GetIntFromConfig(taSectionPtr, KUtcOffsetTag, expUtcOffset);


		TPtrC8 location8(CnvUtfConverter::ConvertFromUnicodeToUtf8L(location)->Des());


		TInt separatorOffset = location8.Locate('/');
		HBufC8* regionName = HBufC8::NewLC(separatorOffset); // push #2
		*regionName = location8.Left(separatorOffset); 
		HBufC8* cityName = HBufC8::NewLC(location8.Length() - (separatorOffset+1)); // push #3
		*cityName = location8.Right(location8.Length() - (separatorOffset+1));
		CTzDbRegionsTable* regionsTable = CTzDbRegionsTable::NewL(*iReadOnlyTzDb,*(TTzRegionsTable*)(iStartAddress + header->iOffsetToRegionsTable));
		CleanupStack::PushL(regionsTable); // push #4
		CTzDbRegion* region = regionsTable->FindRegionL(*regionName);
		if (region)
			{
			CleanupStack::PushL(region); // push #5
			CTzDbZone* zone = region->FindZoneL(*cityName);
			CleanupStack::PopAndDestroy(region); // pop #5

			if (zone)
				{
				CleanupStack::PushL(zone); // push #5
				CTzDbStdTimeAlignment* sta = zone->FindStdTimeAlignmentL(year,month,day,hour,minute,ETzUtcTimeReference);
				CleanupStack::PopAndDestroy(zone); // pop #5
				if (sta)
					{
					CleanupStack::PushL(sta); // push #5

					if	(	(sta->UntilYear() == expUntilYear) &&	(sta->UntilMonth() == expUntilMonth)
						&&	(sta->UntilDay() == expUntilDay) && (sta->UntilTimeInMinutes() == expUntilTimeInMinutes)
						&&	(sta->UntilTimeReference() == expUntilTimeReference) && (sta->UtcOffset() == expUtcOffset)
						)
						{
						isOk = ETrue;
						}

					CleanupStack::PopAndDestroy(sta); // pop #5
					} // if (sta)
				} // if (zone)
			} // if (region)  
		CleanupStack::PopAndDestroy(4, taSection);
		} // for

	return isOk;
	}

//

//Checking for the presence of backup_registration.xml in the private directory on Z drive
TBool CDstDatabaseManagerStep::DoBackupRegistrationFileCheckL()
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
