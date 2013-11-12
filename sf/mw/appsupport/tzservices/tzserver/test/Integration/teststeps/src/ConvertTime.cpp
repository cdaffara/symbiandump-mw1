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
 @file ConvertTime.cpp
*/

#include "ConvertTime.h"
#include "DstIntUtils.h"
#include <tz.h>
#include <tzconverter.h>
#include <vtzrules.h>

CConvertTime::~CConvertTime()
	{
	iTimeZoneServer.Close();
	}

CConvertTime::CConvertTime()
	{
	SetTestStepName(KConvertTime);
	}

void CConvertTime::PrepareTestEnvironmentL()
	{
	// Tags for the script
	_LIT(KTagTimeUTC, "TimeUTC");
	_LIT(KTagTimeLoc, "TimeLoc");
	_LIT(KTagConvert, "Convert");
	_LIT(KTagSystemTimeLoc, "SystemTimeLoc");

	// Values for the conversion parameter
	_LIT(KTagConvertUTC,   "UTC");
	_LIT(KTagConvertLocal, "Local");
	_LIT(KTagConvertBoth,  "Both");

	// Read parameters and store
	TPtrC strPtr;

	GetCityParametersL();
	GetStringFromConfig(ConfigSection(), KTagTimeUTC, strPtr);
	iUTCTime = CDstStep::ReadTimeParamStringL(strPtr);
	GetStringFromConfig(ConfigSection(), KTagTimeLoc, strPtr);
	iLocalTime = CDstStep::ReadTimeParamStringL(strPtr);
	if (GetStringFromConfig(ConfigSection(), KTagSystemTimeLoc, strPtr))
		{
		iSystemLocalTime = CDstStep::ReadTimeParamStringL(strPtr);
		}
	else
		{
		iSystemLocalTime = Time::NullTTime();
		}
	
	GetStringFromConfig(ConfigSection(), KTagConvert, strPtr);

	iConversion = EConvertUTC;
	if(strPtr == KTagConvertUTC)
		iConversion = EConvertUTC;
	if(strPtr == KTagConvertLocal)
		iConversion = EConvertLocal;
	if(strPtr == KTagConvertBoth)
		iConversion = EConvertBoth;
	}

TInt CConvertTime::CreateAndRunTestUnitL()
	{
	TTime t;
	TInt converterErrUTC = KErrNone;
	TInt rulesErrUTC = KErrNone;
	TInt converterErrLocal = KErrNone;
	TInt rulesErrLocal = KErrNone;
	
	User::LeaveIfError(iTimeZoneServer.Connect());

	// Set home location
	TInt err = CDstIntUtils::SetHomeTimeZoneL(iCityName, iTimeZoneServer);
	
	if (iSystemLocalTime != Time::NullTTime())
		{
		iTimeZoneServer.SetHomeTime(iSystemLocalTime);
		}
	
	// Instantiate Converter
	CTzConverter *converter = CTzConverter::NewL(iTimeZoneServer);
	CleanupStack::PushL(converter);
	
	CTzId* tzId = CTzId::NewL(iCityName);
	CleanupStack::PushL(tzId);
	CTzRules* rules = iTimeZoneServer.GetTimeZoneRulesL(*tzId, TDateTime(0, EJanuary, 0, 0, 0, 0, 0), TDateTime(9999, EJanuary, 0, 0, 0, 0, 0), ETzWallTimeReference);
	CleanupStack::PopAndDestroy(tzId);
	CleanupStack::PushL(rules);
	
	if(err != KErrNone)
		{
		TBuf8<64> txt;
		txt.Append(_L8("Zone not set: "));
		txt.Append(iCityName);
		TBuf<64> txtmsg;
		txtmsg.Copy(txt);
		LogCheck(txtmsg, err);
		}
	else
		{
		// Handle UTC to local conversion
		if((iConversion == EConvertUTC) || (iConversion == EConvertBoth))
			{
			t = iUTCTime;
			converterErrUTC = converter->ConvertToLocalTime(t); 
			if(converterErrUTC == KErrNone)
				{
				converterErrUTC = CompareTimesL(t, EConvertUTC);
				}
			
			t = iUTCTime;
			TRAP(rulesErrUTC, rules->ConvertToLocalL(t)); 
			if(rulesErrUTC == KErrNone)
				{
				rulesErrUTC = CompareTimesL(t, EConvertUTC);
				}
			}

		// Handle local to UTC conversion
		if((iConversion == EConvertLocal) || (iConversion == EConvertBoth))
			{
			t = iLocalTime;
			converterErrLocal = converter->ConvertToUniversalTime(t); 
			if(converterErrLocal == KErrNone)
				{
				converterErrLocal = CompareTimesL(t, EConvertLocal);
				}
			
			t = iLocalTime;
			TRAP(rulesErrLocal, rules->ConvertToUtcL(t)); 
			if(rulesErrLocal == KErrNone)
				{
				rulesErrLocal = CompareTimesL(t, EConvertLocal);
				}
			}

		if(converterErrUTC != KErrNone)
			err = converterErrUTC;
		if(converterErrLocal != KErrNone)
			err = converterErrLocal;
		if(rulesErrUTC != KErrNone)
			err = rulesErrUTC;
		if(rulesErrLocal != KErrNone)
			err = rulesErrLocal;
		}

	CleanupStack::PopAndDestroy(rules);
	CleanupStack::PopAndDestroy(converter);

	return err;
	}

void CConvertTime::DestroyTestUnit()
	{
	iTimeZoneServer.Close();
	}

TInt CConvertTime::CompareTimesL(TTime t, TConversion aConversion)
	{
	TInt err = KErrNone;
	TBuf<128> txt;
	txt.Zero();

	switch(aConversion)
		{
	case EConvertUTC:
		if(t != iLocalTime)
			{
			err = KErrGeneral;
			CDstIntUtils::AppendTimeParamStringL(iUTCTime, txt);
			txt.Append(_L(" UTC to "));
			CDstIntUtils::AppendTimeParamStringL(iLocalTime, txt);		
			txt.Append(_L(" local, received "));
			CDstIntUtils::AppendTimeParamStringL(t, txt);
			LogCheck(txt, err);
			}
		break;

	case EConvertLocal:
		if(t != iUTCTime)
			{
			err = KErrGeneral;
			CDstIntUtils::AppendTimeParamStringL(iLocalTime, txt);
			txt.Append(_L(" local to "));
			CDstIntUtils::AppendTimeParamStringL(iUTCTime, txt);		
			txt.Append(_L(" UTC, received "));
			CDstIntUtils::AppendTimeParamStringL(t, txt);
			LogCheck(txt, err);			}
		break;

	default:
		err = KErrNotFound;
		txt.Append(_L("Unknown conversion"));
		LogCheck(txt, err);
		break;
		}

	return err;
	}

//
// Introduced for defect: INC085691

CConvertTimeViaServer::~CConvertTimeViaServer()
	{
	iTimeZoneServer.Close();
	}

CConvertTimeViaServer::CConvertTimeViaServer()
	{
	SetTestStepName(KConvertTimeViaServer);
	}

void CConvertTimeViaServer::PrepareTestEnvironmentL()
	{
	// Tags for the script
	_LIT(KTagTimeUTC, "TimeUTC");
	_LIT(KTagTimeLoc, "TimeLoc");
	_LIT(KTagConvert, "Convert");
	_LIT(KTagSystemTimeLoc, "SystemTimeLoc");

	// Values for the conversion parameter
	_LIT(KTagConvertUTC,   "UTC");
	_LIT(KTagConvertLocal, "Local");
	_LIT(KTagConvertBoth,  "Both");

	// Read parameters and store
	TPtrC strPtr;

	GetCityParametersL();
	GetStringFromConfig(ConfigSection(), KTagTimeUTC, strPtr);
	iUTCTime = CDstStep::ReadTimeParamStringL(strPtr);
	GetStringFromConfig(ConfigSection(), KTagTimeLoc, strPtr);
	iLocalTime = CDstStep::ReadTimeParamStringL(strPtr);
	if (GetStringFromConfig(ConfigSection(), KTagSystemTimeLoc, strPtr))
		{
		iSystemLocalTime = CDstStep::ReadTimeParamStringL(strPtr);
		}
	else
		{
		iSystemLocalTime = Time::NullTTime();
		}
	
	GetStringFromConfig(ConfigSection(), KTagConvert, strPtr);

	iConversion = EConvertUTC;
	if(strPtr == KTagConvertUTC)
		iConversion = EConvertUTC;
	if(strPtr == KTagConvertLocal)
		iConversion = EConvertLocal;
	if(strPtr == KTagConvertBoth)
		iConversion = EConvertBoth;
	}

TInt CConvertTimeViaServer::CreateAndRunTestUnitL()
	{
	TTime t;
	TInt errUTC = KErrNone;
	TInt errLocal = KErrNone;

	// Instantiate Converter
	User::LeaveIfError(iTimeZoneServer.Connect());

	// Set home location
	TInt err = CDstIntUtils::SetHomeTimeZoneL(iCityName, iTimeZoneServer);
	
	if (iSystemLocalTime != Time::NullTTime())
		{
		iTimeZoneServer.SetHomeTime(iSystemLocalTime);
		}
	
	if(err != KErrNone)
		{
		TBuf8<64> txt;
		txt.Append(_L8("Zone not set: "));
		txt.Append(iCityName);
		TBuf<64> txtmsg;
		txtmsg.Copy(txt);
		LogCheck(txtmsg, err);
		}
	else
		{
		// Handle UTC to local conversion
		if((iConversion == EConvertUTC) || (iConversion == EConvertBoth))
			{
			t = iUTCTime;
			errUTC = iTimeZoneServer.ConvertToLocalTime(t);
			if(errUTC == KErrNone)
				{
				errUTC = CompareTimesL(t, EConvertUTC);
				}
			}

		// Handle local to UTC conversion
		if((iConversion == EConvertLocal) || (iConversion == EConvertBoth))
			{
			t = iLocalTime;
			errLocal = iTimeZoneServer.ConvertToUniversalTime(t); 
			if(errLocal == KErrNone)
				{
				errLocal = CompareTimesL(t, EConvertLocal);
				}
			}

		if(errUTC != KErrNone)
			err = errUTC;
		if(errLocal != KErrNone)
			err = errLocal;
		}

	return err;
	}

void CConvertTimeViaServer::DestroyTestUnit()
	{
	iTimeZoneServer.Close();
	}

TInt CConvertTimeViaServer::CompareTimesL(TTime t, TConversion aConversion)
	{
	TInt err = KErrNone;
	TBuf<128> txt;
	txt.Zero();

	switch(aConversion)
		{
	case EConvertUTC:
		if(t != iLocalTime)
			{
			err = KErrGeneral;
			CDstIntUtils::AppendTimeParamStringL(iUTCTime, txt);
			txt.Append(_L(" UTC to "));
			CDstIntUtils::AppendTimeParamStringL(iLocalTime, txt);		
			txt.Append(_L(" local, received "));
			CDstIntUtils::AppendTimeParamStringL(t, txt);
			LogCheck(txt, err);
			}
		break;

	case EConvertLocal:
		if(t != iUTCTime)
			{
			err = KErrGeneral;
			CDstIntUtils::AppendTimeParamStringL(iLocalTime, txt);
			txt.Append(_L(" local to "));
			CDstIntUtils::AppendTimeParamStringL(iUTCTime, txt);		
			txt.Append(_L(" UTC, received "));
			CDstIntUtils::AppendTimeParamStringL(t, txt);
			LogCheck(txt, err);			}
		break;

	default:
		err = KErrNotFound;
		txt.Append(_L("Unknown conversion"));
		LogCheck(txt, err);
		break;
		}

	return err;
	}

//



CUnknownZoneTest::~CUnknownZoneTest()
	{
	iTimeZoneServer.Close();
	}

CUnknownZoneTest::CUnknownZoneTest()
	{
	SetTestStepName(KUnknownZone);
	}

void CUnknownZoneTest::PrepareTestEnvironmentL()
	{
	}

TInt CUnknownZoneTest::CreateAndRunTestUnitL()
	{
	User::LeaveIfError(iTimeZoneServer.Connect());

	//   
	// Initially set home zone to London
	//
	CTzId* tzId = CTzId::NewL(_L8("Europe/London"));
	CleanupStack::PushL(tzId);
	iTimeZoneServer.SetTimeZoneL(*tzId);

	//
	// Call SetUnknownZoneTime. Set the time to a winter time in Europe/London
	// to check that the previous time zone settings are overridden by the new
	// unknown zone settings.
	//
	TDateTime winterDateTime(2006,EJanuary,10,10,0,0,0); // 11-Jan-2006 10:00
	TTime localTime(winterDateTime);
	localTime.HomeTime();
	TInt offset = 120; // UTC+2
	iTimeZoneServer.SetUnknownZoneTimeL(localTime,offset);

	TTime utcTime(localTime);
	iTimeZoneServer.ConvertToUniversalTime(utcTime);

	TTimeIntervalMinutes diff;
	localTime.MinutesFrom(utcTime,diff);

	if (diff.Int() != offset)
		{
		ERR_PRINTF3(_L("Expected offset = %D, Retrieved offset = %D"),&offset,&diff);
		return KErrGeneral;
		}

	// convert back to local time and we should get the initial time again
	iTimeZoneServer.ConvertToLocalTime(utcTime);
	if (utcTime!=localTime)
		{
		ERR_PRINTF1(_L("ConvertToLocalTime failed"));
		return KErrGeneral;       
		}

	//
	// Repeat again with a London summer time
	//

	// reset time zone to London
	iTimeZoneServer.SetTimeZoneL(*tzId);

	TDateTime summerDateTime(2006,EJune,10,10,0,0,0); // 11-June-2006 10:00

	localTime = summerDateTime;
	localTime.HomeTime();
	iTimeZoneServer.SetUnknownZoneTimeL(localTime,offset);

	utcTime = localTime;
	iTimeZoneServer.ConvertToUniversalTime(utcTime);

	localTime.MinutesFrom(utcTime,diff);

	if (diff.Int() != offset)
		{
		ERR_PRINTF3(_L("Expected offset = %D, Retrieved offset = %D"),&offset,&diff);
		return KErrGeneral;
		}

	// convert back to local time and we should get the initial time again
	iTimeZoneServer.ConvertToLocalTime(utcTime);
	if (utcTime!=localTime)
		{
		ERR_PRINTF1(_L("ConvertToLocalTime failed"));
		return KErrGeneral;       
		}

	CleanupStack::PopAndDestroy(tzId);
	return KErrNone;
	}

void CUnknownZoneTest::DestroyTestUnit()
	{
	iTimeZoneServer.Close();
	}
