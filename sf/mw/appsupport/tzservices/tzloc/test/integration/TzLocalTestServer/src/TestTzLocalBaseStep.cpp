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

#include "TestTzLocalBaseStep.h" 
#include <test/testexecutelog.h>
#include <vtzrules.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <tzusernames.h>
#include <tzuserdefineddata.h>
#endif

using namespace tzconstants;

CTestTzLocalBaseStep::~CTestTzLocalBaseStep()
/**
 * Destructor
 */
	{
	}

CTestTzLocalBaseStep::CTestTzLocalBaseStep()
/**
 * Constructor
 */
	{		 
	}
	
TBool CTestTzLocalBaseStep::CheckReturnedTzInfo(const CTzLocalizedTimeZone& aTz)
    {
    
    TBool result;
    result = ETrue;
    TInt TzIdFromIni;
    TPtrC StandardNameFromIni;
    TPtrC DaylightNameFromIni;
    TPtrC ShortStandardNameFromIni;
    TPtrC ShortDaylightNameFromIni;
    TBuf<50> buf;
    
    if (!GetIntFromConfig(ConfigSection(),KTzIdTag, TzIdFromIni))
        {
        _LIT(KTzIDNotFound, "TzLocalBaseStep : TzID not found in ini");
        ERR_PRINTF1(KTzIDNotFound);	
        }
       
    if (!GetStringFromConfig(ConfigSection(),KStandardNameTag, StandardNameFromIni))
        {
        _LIT(KStNameNotFound,"TzLocalBaseStep : Standard name not found in ini");
        ERR_PRINTF1(KStNameNotFound);	
        }
    
        
    if (!GetStringFromConfig(ConfigSection(), KDaylightNameTag, DaylightNameFromIni))
        {
        _LIT(KDayNameNotFound,"TzLocalBaseStep : Daylight name not found in ini");
    	ERR_PRINTF1(KDayNameNotFound);
        }    
    
    if (!GetStringFromConfig(ConfigSection(),KShortStandardName, ShortStandardNameFromIni))
        {
        _LIT(KShortStNameNotFound,"TzLocalBaseStep : Short standard name not found in ini");
    	ERR_PRINTF1(KShortStNameNotFound);      	  	
        }
        
    if (!GetStringFromConfig(ConfigSection(),KShortDaylightName, ShortDaylightNameFromIni))
        {
        _LIT(KShortDayNameNotFound,"TzLocalBaseStep : Short daylight name not found in ini");
    	ERR_PRINTF1(KShortDayNameNotFound);
        }
      
    
    if(aTz.TimeZoneId() != TzIdFromIni )
        {
        _LIT(KIncorrectTzID,"Incorrect TzID = %d");
    	ERR_PRINTF2(KIncorrectTzID,aTz.TimeZoneId());
        result = EFalse;	
        }
    
    if (aTz.StandardName() != StandardNameFromIni)
        {
        buf.Copy(aTz.StandardName());
        _LIT(KIncorrectStName,"Incorrect Standard Name = %S");
    	ERR_PRINTF2(KIncorrectStName,&buf);
    	result = EFalse;
        }
    
    if (aTz.DaylightName() != DaylightNameFromIni) 
        {
        buf.Copy(aTz.DaylightName());
        _LIT(KIncorrectDayName,"Incorrect DaylightName");
       	ERR_PRINTF2(KIncorrectDayName,&buf);
    	result = EFalse;
        }
    
    if(aTz.ShortStandardName() != ShortStandardNameFromIni)
        {
        buf.Copy(aTz.ShortStandardName());
        _LIT(KIncorrectShortStame,"Incorrect ShortStandardName = %S");
        ERR_PRINTF2(KIncorrectShortStame,&buf);
    	result = EFalse;
        }
    
    if (aTz.ShortDaylightName() != ShortDaylightNameFromIni)
        {
        buf.Copy(aTz.ShortDaylightName());
        _LIT(KIncorrectShortDayName,"Incorrect ShortDaylightName = %S");
        ERR_PRINTF2(KIncorrectShortDayName,&buf);
    	result = EFalse;
        }
    
    if (result)
        {
        TBuf<40> buf;
        _LIT(KTzid,"Time Zone Id = %d");
        INFO_PRINTF2(KTzid, TzIdFromIni);
    
        buf.Copy(StandardNameFromIni);
        _LIT(KStandardName,"StandardName = %S");
        INFO_PRINTF2(KStandardName, &buf);
    
        buf.Copy(DaylightNameFromIni);
        _LIT(KDaylightName,"DaylightName = %S");
        INFO_PRINTF2(KDaylightName, &buf);
    
        buf.Copy(ShortStandardNameFromIni);
        _LIT(KShortStName,"ShortStandardName = %S");
        INFO_PRINTF2(KShortStName, &buf);
        
        buf.Copy(ShortDaylightNameFromIni);
        _LIT(KShortDayName,"ShortDaylightName = %S");
        INFO_PRINTF2(KShortDayName, &buf);
        }
    
    return result;
	
    }

void CTestTzLocalBaseStep::CreateUserRuleLC(CTzUserData& aUserData)
	{
	//Create a user defined rule - CTzRules.
	TTime dston (TDateTime(2005, EMarch, 6, 0, 0, 0, 0)); 
	TTime dstoff (TDateTime(2005, ESeptember, 0, 0, 0, 0, 0));
	TMonth monthOn = dston.DateTime().Month();
	TInt dayOn = dston.DateTime().Day();
	TMonth monthOff = dstoff.DateTime().Month();
	TInt dayOff = dstoff.DateTime().Day();
	
	TTzRule dstOff(0, 9999, 180, 180, monthOn,  ETzFixedDate, dayOn,  0, ETzWallTimeReference, 0);
	TTzRule dstOn (0, 9999, 180, 180, monthOff, ETzFixedDate, dayOff, 0, ETzWallTimeReference, 0);

	CTzRules* rules = CTzRules::NewL(0, KMaxTUint);
	CleanupStack::PushL(rules);
	rules->AddRuleL(dstOff);
	rules->AddRuleL(dstOn);
	
	//Create a user defined rule names- CTzUserNames.
	_LIT(KStandardName, "User Standard Time");
	_LIT(KShortName, "UserTzSh");
	_LIT(KDaylightSaveName, "User DLS");
	_LIT(KShortDaylightSaveName, "UserShDLS");
	_LIT(KCityNameName, "User City");
	_LIT(KRegionName, "User Region");
	
	CTzUserNames* names = CTzUserNames::NewLC(KStandardName(), KShortName(), KDaylightSaveName(), KShortDaylightSaveName(), KCityNameName(), KRegionName());
	//Add the rule to the database
	CTzId* tzid = aUserData.CreateL(*rules, *names);
	CleanupStack::PopAndDestroy(2, rules);
	delete tzid;
	}

LOCAL_D void ResetAndDestroyTzIdArray(TAny* aPtr)
	{
	RPointerArray<CTzId>* ids = static_cast<RPointerArray<CTzId>*>(aPtr);
	if (ids)
		{
		ids->ResetAndDestroy();
		}
	}

void CTestTzLocalBaseStep::ResetTzUserDataL()
	{
	RTz tz;
    User::LeaveIfError(tz.Connect());
    CleanupClosePushL(tz);
    CTzUserData* userdata = CTzUserData::NewL(tz);
    CleanupStack::PushL(userdata);  	
	RPointerArray<CTzId> ids;
	CleanupStack::PushL(TCleanupItem(ResetAndDestroyTzIdArray, &ids));
	userdata->GetTzIdsL(ids);
	TInt count = ids.Count();
	for (TInt ii=0; ii<count; ++ii)
		{
		userdata->DeleteL(*ids[ii]);	
		}
	CleanupStack::PopAndDestroy(3, &tz);	
	}
