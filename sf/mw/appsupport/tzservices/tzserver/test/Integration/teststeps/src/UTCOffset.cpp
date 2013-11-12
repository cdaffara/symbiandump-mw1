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
 @file UTCOffset.cpp
*/
 
#include "UTCOffset.h"
#include "DstStep.h"
#include "DstIntUtils.h"
#include <tz.h>

const TInt KSecondsPerMinute = 60;
CUTCOffset::CUTCOffset()
 	{
 	SetTestStepName(KUTCOffset);
 	}
 	
CUTCOffset::~CUTCOffset()
	{
	iTimeZoneServer.Close();
	}
	
TVerdict CUTCOffset::doTestStepPreambleL()
	{	
	_LIT(KDstLocationTag,"City");
	_LIT(KDstOffsetTag, "UTCOffset");
	_LIT(KDstTimeTag, "Time");
	
	TPtrC ptr;
	
	// read the time zone by name 
	GetStringFromConfig(ConfigSection(), KDstLocationTag, ptr);
	iLocation.Copy(ptr);
	
	// read the time to be set
	GetStringFromConfig(ConfigSection(), KDstTimeTag, ptr);
	iTime = CDstStep::ReadTimeParamStringL(ptr);
	
	// read the expected utc offset 
	TInt offsetInMins;
	GetIntFromConfig(ConfigSection(), KDstOffsetTag, offsetInMins);
	// convert it to seconds
	iUTCOffset = offsetInMins * KSecondsPerMinute;
	
	return TestStepResult();
	}	
	
TVerdict CUTCOffset::doTestStepL()
	{
	SetTestStepResult(EFail);
	// connect to the time zone server
	User::LeaveIfError(iTimeZoneServer.Connect());
	// enable auto update function - it's enabled by default but to make it sure.
	iTimeZoneServer.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOn);

	TInt err(KErrNone);
	// first we set the time zone where we want to test
	err = CDstIntUtils::SetHomeTimeZoneL(iLocation, iTimeZoneServer);
	if(err)
		{
		ERR_PRINTF2(_L("Setting time zone failed: error = %d"),err);
		return TestStepResult();
		}		

	// set local time
	err = iTimeZoneServer.SetHomeTime(iTime);
	if(err)
  		{
  		ERR_PRINTF2(_L("Setting home time failed: error = %d"),err);
 		return TestStepResult();
 		}
		
	// get utc offset from the system
	TTimeIntervalSeconds utcOffset = User::UTCOffset();
	
	// compare the offset to the expected value.
	if(iUTCOffset == utcOffset)
		{
		SetTestStepResult(EPass);
		}
	else
		{
		TTime time;
		time.HomeTime();
		TBuf<128> timeStr;
		CDstIntUtils::AppendTimeParamStringL(time, timeStr);
		INFO_PRINTF2(_L("Current local time: %S"), &timeStr );
		
		TTime utc;
		utc.UniversalTime();
		TBuf<128> utcStr;
		CDstIntUtils::AppendTimeParamStringL(utc, utcStr);
		INFO_PRINTF2(_L("Current UTC time: %S"), &utcStr );
		
		INFO_PRINTF3(_L(" received UTC Offset: %d, expected UTC offset: %d"), utcOffset.Int()/60, iUTCOffset.Int()/60);
		}
	
	return TestStepResult();
	}
	
TVerdict CUTCOffset::doTestStepPostambleL()
	{
	iTimeZoneServer.Close();
	return TestStepResult();
	}
