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

#include "TimeZoneOffsetCheck.h"
#include "DstStep.h"
#include "DstIntUtils.h"
#include <vtzrules.h>

CTimeZoneOffsetCheck::CTimeZoneOffsetCheck()
 	{
 	SetTestStepName(KTimeZoneOffsetCheckStep);
 	}
 	
CTimeZoneOffsetCheck::~CTimeZoneOffsetCheck()
	{
	iTimeZoneServer.Close();
	}
	
TVerdict CTimeZoneOffsetCheck::doTestStepPreambleL()
	{	
	_LIT(KLocationTag,"City");
	_LIT(KOldOffsetTag, "OldOffset");
	_LIT(KNewOffsetTag, "NewOffset");
	
	TPtrC ptr;
	
	// read the time zone by name 
	GetStringFromConfig(ConfigSection(), KLocationTag, ptr);
	iLocation.Copy(ptr);
	
	// read the expected offsets 
	TInt offsetInMins;
	GetIntFromConfig(ConfigSection(), KOldOffsetTag, offsetInMins);
	iOldOffset = offsetInMins;
	
	offsetInMins = 0;
	GetIntFromConfig(ConfigSection(), KNewOffsetTag, offsetInMins);
	iNewOffset = offsetInMins;
	
	return TestStepResult();
	}	
	
TVerdict CTimeZoneOffsetCheck::doTestStepL()
	{
	SetTestStepResult(EFail);
	// connect to the time zone server
	User::LeaveIfError(iTimeZoneServer.Connect());
	TInt err(KErrNone);
	// first we set the time zone where we want to test
	err = CDstIntUtils::SetHomeTimeZoneL(iLocation, iTimeZoneServer);
	if(err)
		{
		ERR_PRINTF2(_L("Setting time zone failed: error = %d"),err);
		return TestStepResult();
		}		
	
	TTzTimeReference timeRef = ETzUtcTimeReference;
	TTime minTime = TCalTime::MinTime();
	TTime maxTime = TCalTime::MaxTime();
	
	CTzId* zoneId =  CTzId::NewL(iLocation);
	CleanupStack::PushL(zoneId);
	CTzRules* tzRules = iTimeZoneServer.GetTimeZoneRulesL(*zoneId, minTime, maxTime, timeRef);
	CleanupStack::PopAndDestroy(zoneId);
	CleanupStack::PushL(tzRules);
	TInt count = tzRules->Count();
	SetTestStepResult(EPass);
	for(TInt i = 0;i<count;++i)
		{
		TTzRule& rule = (*tzRules)[i];
		if(rule.iOldLocalTimeOffset != iNewOffset && rule.iOldLocalTimeOffset != iOldOffset)
			{
			SetTestStepResult(EFail);
			INFO_PRINTF4(_L("Test Failed! Offset should have been %d or %d, found %d"),iNewOffset, iOldOffset, rule.iOldLocalTimeOffset);
			break;
			}
		if(rule.iNewLocalTimeOffset != iNewOffset && rule.iNewLocalTimeOffset != iOldOffset)
			{	
			SetTestStepResult(EFail);
			INFO_PRINTF4(_L("Test Failed! Offset should have been %d or %d, found %d"),iNewOffset, iOldOffset, rule.iNewLocalTimeOffset);
			break;
			}
		}
	CleanupStack::PopAndDestroy(tzRules);	
	return TestStepResult();
	}
	
TVerdict CTimeZoneOffsetCheck::doTestStepPostambleL()
	{
	iTimeZoneServer.Close();
	return TestStepResult();
	}
