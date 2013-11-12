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

/**
 @file DSTUpdate.cpp
*/
 
#include "DstIntUtils.h"
#include "DSTUpdate.h"
#include "DstStep.h"
#include <tz.h>

// time format string for logs
_LIT(KTimeFormat, "%F%Y/%M/%D %H%:2%T%:2%S%");

CDSTUpdate::CDSTUpdate()
	{
	SetTestStepName(KDSTUpdate);
	}
	
CDSTUpdate::~CDSTUpdate()
	{
	iTimeZoneServer.Close();
	}	
	
TVerdict CDSTUpdate::doTestStepL()
	{
	TVerdict result1 = TestAutoUpdateL(RTz::ETZAutoDSTUpdateOn);
	TVerdict result2 = TestAutoUpdateL(RTz::ETZAutoDSTUpdateOff);
	TVerdict result3 = TestAutoUpdateL(RTz::ETZAutoDSTNotificationOnly);
	if ( (result1 == EPass) && (result2 == EPass) && (result3 == EPass) )
		{
		return EPass;
		}
	return EFail;
	}

TVerdict CDSTUpdate::TestAutoUpdateL(RTz::TTzAutoDSTUpdateModes aAutoUpdateMode)
	{
	SetTestStepResult(EFail);
	// connect to the time zone server
	User::LeaveIfError(iTimeZoneServer.Connect());
	// enable the auto update functionality - it's enabled by default but to make it sure
	iTimeZoneServer.SetAutoUpdateBehaviorL(aAutoUpdateMode);
	
	TInt err(KErrNone);
	// set the time zone where we want to test
	err = CDstIntUtils::SetHomeTimeZoneL(iLocation, iTimeZoneServer);
	if(err)
		{	
		ERR_PRINTF1(_L("Setting location failed"));
		return TestStepResult();
		}
		
	TBuf<128> systimestring;
	iInitialTime.FormatL(systimestring,KTimeFormat);
	INFO_PRINTF2(_L("System Time to be set at: %S"), &systimestring );

	// set local time in the time zone where we've already changed to
	// if the time to be set is in UTC, then set system time in UTC.
	// This is to avoid the ambiguous UTC time when changing summer time to winter time.
	// e.g. local time 1.30am 30/10/2005 Europe/London - we don't know if this is before 
	// the DST change (summer time) or after (winter time) 
	// So we need to set it in UTC time.
	if (iIsUtc)
		{
		err = User::SetUTCTime(iInitialTime);
		}
	else
		{
		err = iTimeZoneServer.SetHomeTime(iInitialTime);
		}
		
	if (err)
		{
		ERR_PRINTF1(_L("Setting home time failed"));
		return TestStepResult();
		}
	
	// get current system time and offset for log
	TTime before;
	TBuf<128> beforeStr;
	if(iIsUtc)
		before.UniversalTime();
	else
		before.HomeTime();
	
	before.FormatL(beforeStr,KTimeFormat);
	TTimeIntervalSeconds offset = User::UTCOffset();
	INFO_PRINTF3(_L("Time before auto update: %S and UTC offset is: %d"), &beforeStr,offset.Int()/60);
	if(beforeStr.Compare(systimestring))
		{
		ERR_PRINTF1(_L("Initial time is not set correctly"));
		return TestStepResult();
		}
	
	//wait for 1 second for DST change to occur
	User::After(1000000);
	
	// get the current local time.
	TTime local;
	local.HomeTime();
	
	// compare the curent local time to the expected one.
	TBuf<128> time;
	CDstIntUtils::AppendTimeParamStringL(local, time);
	TBuf<128> exptime;
   
	if (aAutoUpdateMode == RTz::ETZAutoDSTUpdateOn)
		{
		// with auto update on, the time should have been updated
		CDstIntUtils::AppendTimeParamStringL(iExpectedTime, exptime);
		}
	else if ( (aAutoUpdateMode == RTz::ETZAutoDSTUpdateOff) || (aAutoUpdateMode == RTz::ETZAutoDSTNotificationOnly) )
		{
		// with auto update turned off, the time shouldn't be the initial time plus 1 second
		before += (TTimeIntervalSeconds)1;
		if (iIsUtc)
			{
			// we have to compare local with local
			before.HomeTime();
			}
		CDstIntUtils::AppendTimeParamStringL(before , exptime);
		}
		
	if(!time.Compare(exptime))
		{
		SetTestStepResult(EPass);
		}
	else 
		{
		ERR_PRINTF3(_L("local: %S: expected: %S"), &time, &exptime);	
		}
	
	return TestStepResult();
	}

TVerdict CDSTUpdate::doTestStepPreambleL()
	{
	_LIT(KDstLocationTag,			"City");
	_LIT(KDstPresetTimeTag,			"PreSetTime");
	_LIT(KDstResultTimeTag,			"ResultTime");
	_LIT(KDstUtcTag,				"UTC");
	
	TPtrC strPtr;
	// read time zone by name(e.g. Europe/Tirane)
	GetStringFromConfig(ConfigSection(), KDstLocationTag, strPtr);
	iLocation.Copy(strPtr);
	
	//read the time 1 second before the DST change
	GetStringFromConfig(ConfigSection(), KDstPresetTimeTag, strPtr);
	iInitialTime = CDstStep::ReadTimeParamStringL(strPtr);
	
	// read the expected time after the DST change
	GetStringFromConfig(ConfigSection(), KDstResultTimeTag, strPtr);
	iExpectedTime = CDstStep::ReadTimeParamStringL(strPtr);
	
	GetBoolFromConfig(ConfigSection(), KDstUtcTag, iIsUtc);
	
	return TestStepResult();
	}

TVerdict CDSTUpdate::doTestStepPostambleL()
	{
	iTimeZoneServer.Close();
	return TestStepResult();
	}
