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

#include "tzpassivetimezoneteststep.h"

_LIT(KIniTimeZoneIdTag,"time_zone_id");

CTzPassiveTimeZoneTestStep::CTzPassiveTimeZoneTestStep()
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTzPassiveTimeZoneTestStep);
	}

CTzPassiveTimeZoneTestStep::~CTzPassiveTimeZoneTestStep()
	{
	}

TVerdict CTzPassiveTimeZoneTestStep::doTestStepL()
	{
	_LIT(KInTestStep,"In test step");
	INFO_PRINTF1(KInTestStep);

	if(!GetTimeZoneFromTimeZoneIdTest())
		{
		SetTestStepResult(EFail);
		}

	if(!GetTimeZoneFromCityTest())
		{
		SetTestStepResult(EFail);
		}

	return TestStepResult();
	}

/**
Perform test of CTzLocalizer::GetLocalizedTimeZoneL(TInt aTimeZoneId)

@return ETrue if test passes, EFalse if it fails

@internalTechnology
*/
TBool CTzPassiveTimeZoneTestStep::GetTimeZoneFromTimeZoneIdTest()
	{
	_LIT(KGetTimeZoneFromTimeZoneIdTest,"Get a Time Zone from a Time Zone ID.");
	INFO_PRINTF1(KGetTimeZoneFromTimeZoneIdTest);
	
	// Get timezone id from ini file
	TInt iniTimeZoneId;
	GetIntFromConfig(ConfigSection(),KIniTimeZoneIdTag,iniTimeZoneId);
	_LIT(KTimeZoneId, "Timezone Id: %d");
	INFO_PRINTF2(KTimeZoneId, iniTimeZoneId);

	TBool result = EFalse;
	CTzLocalizedTimeZone* timeZone = 0;
	TRAPD(err, timeZone = iLocalizer->GetLocalizedTimeZoneL(iniTimeZoneId));
	if (err == KErrNone)
		{
		result = CompareTimeZoneWithIniFileSection(*timeZone,ConfigSection());
		}
	else
		{
		_LIT(KLeaveCode, "GetLocalizedTimeZoneL left with %d");
		INFO_PRINTF2(KLeaveCode, err);
		if ((iniTimeZoneId == 666) && (err == KErrNotFound))
			{
			// This test checks that KErrNotFound is returned for an inexistent time zone
			result = true;
			}
		}
	delete timeZone;

	PrintResult(result);
	return result;
	}

/**
Perform test of CTzLocalizer::GetLocalizedTimeZoneL(CTzLocalizedCity* aCity)

@return ETrue if test passes, EFalse if it fails

@internalTechnology
*/
TBool CTzPassiveTimeZoneTestStep::GetTimeZoneFromCityTest()
	{
	_LIT(KGetTimeZoneFromCityTest,"Get a Time Zone from a CTzLocalizedCity ID.");
	INFO_PRINTF1(KGetTimeZoneFromCityTest);
	// Get timezone id from ini file
	TInt iniTimeZoneId;
	GetIntFromConfig(ConfigSection(),KIniTimeZoneIdTag,iniTimeZoneId);
	_LIT(KTimeZoneId, "Timezone Id: %d");
	INFO_PRINTF2(KTimeZoneId, iniTimeZoneId);

	TBool result = EFalse;
	CTzLocalizedCity* defaultCity = 0;
	TRAPD(err, defaultCity = iLocalizer->GetDefaultCityL(iniTimeZoneId));
	if (err == KErrNone)
		{
		CTzLocalizedTimeZone* timeZone = 0;
		TRAP(err, timeZone = iLocalizer->GetLocalizedTimeZoneL(*defaultCity));
		if (err == KErrNone)
			{
			result = CompareTimeZoneWithIniFileSection(*timeZone,ConfigSection());
			}
		else
			{
			_LIT(KLeaveCode, "GetLocalizedTimeZoneL left with %d");
			INFO_PRINTF2(KLeaveCode, err);
			}
		delete timeZone;
		delete defaultCity;
		}
	else
		{
		_LIT(KLeaveCode, "GetDefaultCityL left with %d");
		INFO_PRINTF2(KLeaveCode, err);
		if ((iniTimeZoneId == 666) && (err == KErrNotFound))
			{
			// This test checks that KErrNotFound is returned for an inexistent time zone
			result = true;
			}
		}
	
	PrintResult(result);
	return result;
	}
