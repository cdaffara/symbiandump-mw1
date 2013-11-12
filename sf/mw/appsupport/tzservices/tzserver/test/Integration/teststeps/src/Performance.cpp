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
 @file Performance.cpp
*/

#include "Performance.h"
#include "DstIntUtils.h"
#include <tzconverter.h>


// These are the two zones we use to alternate between
_LIT8(KLondon, "Europe/London");
_LIT8(KSydney, "Australia/Sydney");
_LIT8(KMoscow, "Europe/Moscow");

// Default number of repetitions
const TInt KDefaultRepetitions = 1000;

// The time to convert
_LIT(KTime2004, "20041026:090000");

const TTime KZeroTime(0);

CPerformance::~CPerformance()
	{
	iTimeZoneServer.Close();
	}

CPerformance::CPerformance()
	{
	SetTestStepName(KPerformance);
	}

void CPerformance::PrepareTestEnvironmentL()
	{
	// Tags for the script
	_LIT(KTagRepetitions, "Repetitions");

	// Read parameters and store
	iIterations = KDefaultRepetitions;
	GetIntFromConfig(ConfigSection(), KTagRepetitions, iIterations);
	}

TInt CPerformance::CreateAndRunTestUnitL()
	{
	TTime total;
	TTime timeStart, timeEnd;
	const TTime time2004 = ReadTimeParamStringL(KTime2004);
	TTime t;
	TInt  i;

	// Instantiate Converter
	User::LeaveIfError(iTimeZoneServer.Connect());
	total = KZeroTime;
	timeStart.UniversalTime();
	CTzConverter *converter = CTzConverter::NewL(iTimeZoneServer);
	timeEnd.UniversalTime();
	CleanupStack::PushL(converter);
	total += timeEnd.MicroSecondsFrom(timeStart);
	LogAverageL(_L("CTimeZoneConverter::NewL()"), total, 1);

	// Create a timezones
	CTzId *tzSydney = CTzId::NewL(KSydney);
	CleanupStack::PushL(tzSydney);
	CTzId *tzMoscow = CTzId::NewL(KMoscow);
	CleanupStack::PushL(tzMoscow);
	CTzId *pZone = tzSydney;

	// Set current zone to London
	CDstIntUtils::SetHomeTimeZoneL(KLondon, iTimeZoneServer);

	// Scenario 1 - convert between UTC and local time for a new zone and time
	total = KZeroTime;
	for(i = 0; i < iIterations; i++)
		{
		pZone = pZone == tzMoscow ? tzSydney : tzMoscow;
		t = time2004;
		timeStart.UniversalTime();
		converter->ConvertToLocalTime(t, *pZone);
		timeEnd.UniversalTime();
		total += timeEnd.MicroSecondsFrom(timeStart);
		}
	LogAverageL(_L("Scenario 1 - New zone"), total, iIterations);

	// Scenario 2 - convert between UTC and local time for the current time zone
	total = KZeroTime;
	for(i = 0; i < iIterations; i++)
		{
		t = time2004;
		timeStart.UniversalTime();
		converter->ConvertToLocalTime(t);
		timeEnd.UniversalTime();
		total += timeEnd.MicroSecondsFrom(timeStart);
		}
	LogAverageL(_L("Scenario 2 - Same zone, same time"), total, iIterations);

	// Scenario 3 - convert between UTC and local time for the current time zone but different years
	// As CTimeZoneConverter caches rules, we will use the RTimeZone class directly here, so the
	// results taken do not take the caching effect into account.  We don't need to swap years either,
	// as without a cache any conversion is akin to a 'new' conversion
	RTz rTz;
	User::LeaveIfError(rTz.Connect());
	CleanupClosePushL(rTz);
	total = KZeroTime;
	for(i = 0; i < iIterations; i++)
		{
		t = time2004;
		timeStart.UniversalTime();
		rTz.ConvertToLocalTime(t);
		timeEnd.UniversalTime();
		total += timeEnd.MicroSecondsFrom(timeStart);
		}
	CleanupStack::PopAndDestroy();	// invokes rTz.Close()
	LogAverageL(_L("Scenario 3 - Using RTimeZone::ConvertToLocalTime()"), total, iIterations);

	CleanupStack::PopAndDestroy(3);	// tzMoscow, tzSydney, converter
	return KErrNone;
	}


void CPerformance::DestroyTestUnit()
	{
	iTimeZoneServer.Close();
	}

void CPerformance::LogAverageL(const TDesC& aTxt, TTime aTotal, TInt aRepetitions)
	{
	// Calculate average time
	TTimeIntervalMicroSeconds microSeconds = aTotal.MicroSecondsFrom(KZeroTime);
	microSeconds = microSeconds.Int64() / aRepetitions;
	aTotal = KZeroTime;
	aTotal += microSeconds;

	// Log
	TBuf<30> dateString;
	_LIT(KDateFormat,"%S.%*C6s per call");
	aTotal.FormatL(dateString, KDateFormat);
	TBuf<128> txt;
	txt.Append(dateString);
	txt.Append(_L(" ("));
	txt.AppendNum(aRepetitions);
	txt.Append(_L(" iterations) : "));
	txt.Append(aTxt);
	INFO_PRINTF1(txt);
	}
