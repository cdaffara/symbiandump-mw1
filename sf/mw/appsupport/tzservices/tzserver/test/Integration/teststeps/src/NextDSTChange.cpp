// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "NextDSTChange.h"
#include "DstStep.h"
#include "DstIntUtils.h"
#include <tz.h>
#include <tzconverter.h>
#include <tzupdate.h>
#include "ConvertTime.h"
#include <e32property.h>

_LIT(KExpectedNewOffset, "expected_new_offset");
_LIT(KExpectedSystemTimeUTC, "expected_system_time_utc");
_LIT(KExpectedPreviousOffset, "expected_previous_offset");
_LIT(KExpectedPreviousDSTUTC, "expected_previous_dst_utc");
_LIT(KTimeZone, "timezone");
_LIT(KHomeTime, "time");
_LIT(KDSTChangeExpected, "dst_change_expected");
_LIT(KAutoUpdatesOn, "auto_updates_on");

const TInt KNextDSTChangePropertyKey(0x10285B32);

CNextDSTChange* CNextDSTChange::NewL()
	{
	CNextDSTChange* self = new CNextDSTChange();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CNextDSTChange::CNextDSTChange()
	{
	SetTestStepName(KNextDSTChange);
	}

void CNextDSTChange::ConstructL()
	{
	User::LeaveIfError(iTz.Connect());
	}

CNextDSTChange::~CNextDSTChange()
	{
	iTz.Close();
	}

TVerdict CNextDSTChange::doTestStepPreambleL()
	{
	TBool autoUpdatesOn;
	TESTL(GetBoolFromConfig(ConfigSection(), KAutoUpdatesOn, autoUpdatesOn));
	
	if (autoUpdatesOn)
		{
		iTz.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOn);
		}
	else
		{
		iTz.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOff);
		}
	
	// Set the time zone
	TPtrC timeZoneString;
	TESTL(GetStringFromConfig(ConfigSection(), KTimeZone, timeZoneString));
	TBuf8<25> timeZone;
	timeZone.Copy(timeZoneString);
	CTzId* tzId = CTzId::NewL(timeZone);
	CleanupStack::PushL(tzId);
	iTz.SetTimeZoneL(*tzId);
	CleanupStack::PopAndDestroy(tzId);
	
	// Set the system home time
	TPtrC homeTimeString;
	TESTL(GetStringFromConfig(ConfigSection(), KHomeTime, homeTimeString));
	TTime homeTime(homeTimeString);
	User::LeaveIfError(iTz.SetHomeTime(homeTime));
	
	return TestStepResult();
	}

/**
Base class pure virtual.
@return		EPass or EFail indicating the result of the test step.
@test
*/
TVerdict CNextDSTChange::doTestStepL()
	{
	// Fetch the next DST change value using the Publish and subscribe property
	TPckgBuf<NTzUpdate::TDSTChangeInfo> fetchedNextDSTChangeBuf;
	User::LeaveIfError(RProperty::Get(KUidSystemCategory, KNextDSTChangePropertyKey, fetchedNextDSTChangeBuf));
	NTzUpdate::TDSTChangeInfo fetchedNextDSTChange = fetchedNextDSTChangeBuf();
	
	TBuf<50> fetchedTimeString;
	CDstIntUtils::AppendTimeParamStringL(fetchedNextDSTChange.iNextDSTChangeUTC, fetchedTimeString);
	INFO_PRINTF2(_L("Fetched next DST change time UTC = %S"), &fetchedTimeString);
	INFO_PRINTF2(_L("Fetched new offset = %d"), fetchedNextDSTChange.iNextUTCOffset);
	TBuf<50> fetchedPrevTimeString;
	CDstIntUtils::AppendTimeParamStringL(fetchedNextDSTChange.iPreviousDSTChangeUTC, fetchedPrevTimeString);
	INFO_PRINTF2(_L("Fetched previous DST change time UTC = %S"), &fetchedPrevTimeString);
	INFO_PRINTF2(_L("Fetched previous offset = %d"), fetchedNextDSTChange.iPreviousUTCOffset);
	
	TBool dstChangeExpected;
	TESTL(GetBoolFromConfig(ConfigSection(), KDSTChangeExpected, dstChangeExpected));	
	
	if (dstChangeExpected)
		{
		INFO_PRINTF1(_L("There is an expected DST change so test that the published values are correct"));
		
		// Fetch the expected values
		TPtrC timeStr;
		TESTL(GetStringFromConfig(ConfigSection(), KExpectedSystemTimeUTC, timeStr));
		TTime time(timeStr);
		TInt newOffset;
		TESTL(GetIntFromConfig(ConfigSection(), KExpectedNewOffset, newOffset)); 
		TPtrC prevTimeStr;
		TESTL(GetStringFromConfig(ConfigSection(), KExpectedPreviousDSTUTC, prevTimeStr));
		TTime prevTime(prevTimeStr);
		TInt prevOffset;
		TESTL(GetIntFromConfig(ConfigSection(), KExpectedPreviousOffset, prevOffset)); 
		
		// Print out the expected and fetched values
		TBuf<50> expectedTimeString;
		CDstIntUtils::AppendTimeParamStringL(time, expectedTimeString);
		INFO_PRINTF2(_L("Expected next DST change time UTC = %S"), &expectedTimeString);
		INFO_PRINTF2(_L("Expected new offset = %d"), newOffset);
		TBuf<50> expectedPrevTimeString;
		CDstIntUtils::AppendTimeParamStringL(prevTime, expectedPrevTimeString);
		INFO_PRINTF2(_L("Expected previous DST change time UTC = %S"), &expectedPrevTimeString);
		INFO_PRINTF2(_L("Expected previous offset = %d"), prevOffset);
		
		// Do the test
		TESTL(fetchedNextDSTChange.iVersion == 2);
		TESTL(fetchedNextDSTChange.iNextDSTChangeUTC == time);
		TESTL(fetchedNextDSTChange.iNextUTCOffset == newOffset);
		TESTL(fetchedNextDSTChange.iPreviousDSTChangeUTC == prevTime);
		TESTL(fetchedNextDSTChange.iPreviousUTCOffset == prevOffset);
		}
	else
		{
		INFO_PRINTF1(_L("There are either no DST changes, or automatic updates are off, so test that a null value is published"));
		
		TESTL(fetchedNextDSTChange.iVersion == 2);
		TESTL(fetchedNextDSTChange.iNextDSTChangeUTC == Time::NullTTime());
		TESTL(fetchedNextDSTChange.iNextUTCOffset == 0);
		TESTL(fetchedNextDSTChange.iPreviousDSTChangeUTC == Time::NullTTime());
		TESTL(fetchedNextDSTChange.iPreviousUTCOffset == 0);
		}
	
	return TestStepResult();
	}

TVerdict CNextDSTChange::doTestStepPostambleL()
	{
	// make sure that auto updates are returned to their default state of on
	iTz.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOn);
	
	return TestStepResult();
	}


