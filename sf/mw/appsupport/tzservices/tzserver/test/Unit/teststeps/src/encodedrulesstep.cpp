// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 

#include "encodedrulesstep.h"
#include <utf.h>
#include <tz.h>
#include <vtzrules.h>

const TInt KMaxTagSize = 50;

/*
If you set "CompareRules=1", you must include
rules in encodedrulesstep.ini for the start of each
standard time alignment, in addition to DST rules.

For compatibility with existing tests, if "CompareRules=0", 
"NumberOfRules" should be set to the number of DST rules only,
excluding the rules for the start of each standard time
alignment.
*/

_LIT(KStartYearTag, "StartYear");
_LIT(KEndYearTag, "EndYear");
_LIT(KLocationTag, "Location");
_LIT(KNumberOfRulesTag, "NumberOfRules");
_LIT(KCompareRulesTag, "CompareRules");
_LIT(KRuleTag, "Rule%D");

_LIT(KRuleFromYearTag, 			"FromYear");
_LIT(KRuleToYearTag, 			"ToYear");
_LIT(KRuleOldStdTimeOffsetTag,	"OldStdTimeOffset");
_LIT(KRuleNewStdTimeOffsetTag, 	"NewStdTimeOffset");
_LIT(KRuleMonthTag, 			"Month");
_LIT(KRuleDayRuleTag, 			"DayRule");
_LIT(KRuleDayOfMonthTag, 		"DayOfMonth");
_LIT(KRuleDayOfWeekTag, 		"DayOfWeek");
_LIT(KRuleTimeReferenceTag, 	"TimeReference");
_LIT(KRuleTimeOfChangeTag, 		"TimeOfChange");


TInt CEncodedRulesStep::CreateAndRunTestUnitL()
	{
	TInt result = KErrNotFound;
	
	// read test values from configuration file
	
	TPtrC location;
	TInt startYear;
	TInt endYear;
	TInt numberOfRules;
	TInt compareRules;
	ReadTestData(location,startYear,endYear,numberOfRules,compareRules);
	
	TDateTime startDateTime(startYear,EJanuary,0,0,0,0,0);
	TDateTime endDateTime(endYear,EDecember,30,23,59,59,0);

	// read from the config file the rules that should be obtained from the tz server
	CTzRules* expectedRules = CTzRules::NewL(startYear,endYear);
	CleanupStack::PushL(expectedRules); // PUSH #1
	
	ReadExpectedRulesL(*expectedRules,numberOfRules);	
	TBool areEqual = EFalse;
	
	// connect to tz server
	
	RTz tz;
	CleanupClosePushL(tz);	 // PUSH #2
	User::LeaveIfError(tz.Connect());
	
	// set time zone in tz server
	
	TPtrC8 locationPtr8(CnvUtfConverter::ConvertFromUnicodeToUtf8L(location)->Des());	
	CTzId* zoneId = CTzId::NewL(locationPtr8);
	CleanupStack::PushL(zoneId);	// PUSH #3
	tz.SetTimeZoneL(*zoneId);
	
	
	// get the encoded tz rules for this location from the tz server
	
	CTzRules* dbRules = tz.GetTimeZoneRulesL(startDateTime,endDateTime,ETzUtcTimeReference);
	if (dbRules)
		{
		CleanupStack::PushL(dbRules); // PUSH #4
		
		TInt regularRuleCount = 0;
		for (TInt i = 0; i < dbRules->Count(); i++)
			{
			if((*dbRules)[i].iFrom != (*dbRules)[i].iTo)
				{
				regularRuleCount++;
				}
			}


		if (compareRules != 0)
			{
			// compare the rules from the tz server with the rules from the config file
			areEqual = CompareRuleCollections(*dbRules,*expectedRules);
			}
		else
			{
			// check number of rules returned
			areEqual = (numberOfRules == regularRuleCount);
			}
			
		CleanupStack::PopAndDestroy(dbRules); // POP #4
		
		if (areEqual)
			result = KErrNone;
		else
			result = KErrGeneral;
		}

	// repeat now using the "foreign time zone" method

	CTzRules* dbRules2 = tz.GetTimeZoneRulesL(*zoneId,startDateTime,endDateTime,ETzUtcTimeReference);
	if (dbRules)
		{
		CleanupStack::PushL(dbRules2); // PUSH #4

		TInt regularRuleCount = 0;
		for (TInt i = 0; i < dbRules->Count(); i++)
			{
			if((*dbRules)[i].iFrom != (*dbRules)[i].iTo)
				{
				regularRuleCount++;
				}
			}

		if (compareRules != 0)
			{	
			// compare the rules from the tz server with the rules from the config file
			areEqual = CompareRuleCollections(*dbRules2,*expectedRules);
			}
		else
			{
			// just check number of rules returned is same as expected
			areEqual = (numberOfRules == regularRuleCount);
			}
			
		CleanupStack::PopAndDestroy(dbRules2); // POP #4
		
		if (areEqual)
			result = KErrNone;
		else
			result = KErrGeneral;
		}
	
	CleanupStack::PopAndDestroy(3, expectedRules); // zoneId, tz, expectedRules
	return result;
	}

CEncodedRulesStep::CEncodedRulesStep(CDstUnitServer& aServer) : CDstUnitStep(aServer)
	{
	SetTestStepName(KEncodedRulesStep);
	}
	
CEncodedRulesStep::~CEncodedRulesStep()
	{
	// nothing to destroy
	}
	
void CEncodedRulesStep::DestroyTestUnit()
	{
	// nothing to destroy
	}

void CEncodedRulesStep::ReadTestData(TPtrC& aLocation, TInt& aStartYear, TInt& aEndYear, TInt& aNumberOfRules, TInt& aCompareRules)
	{
	GetStringFromConfig(ConfigSection(),KLocationTag,aLocation);
	GetIntFromConfig(ConfigSection(),KStartYearTag,aStartYear);
	GetIntFromConfig(ConfigSection(),KEndYearTag,aEndYear);
	GetIntFromConfig(ConfigSection(),KNumberOfRulesTag,aNumberOfRules);
	GetIntFromConfig(ConfigSection(),KCompareRulesTag,aCompareRules);
	}
	
TBool CEncodedRulesStep::CompareRuleCollections(CTzRules& aRules1, CTzRules& aRules2)
	{
	TBool areEqual = EFalse;
	
	if (aRules1.Count() != aRules2.Count())
		{
		return EFalse;
		}
		
	// compare rules one by one
	TInt count = aRules1.Count();
	for (TInt i = 0; i < count && areEqual; i++)
		{
		areEqual = CompareRules(aRules1[i], aRules2[i]);
		}
		
	return areEqual;
	}
	
TBool CEncodedRulesStep::CompareRules(const TTzRule& aRule1, const TTzRule& aRule2)
	{	
	//if (aRule1.iFromYear != aRule2.iFromYear)
	if (aRule1.iFrom != aRule2.iFrom)
		return EFalse;
	
	//if (aRule1.iToYear != aRule2.iToYear)
	if (aRule1.iTo != aRule2.iTo)
		return EFalse;
	
	if (aRule1.iOldLocalTimeOffset != aRule2.iOldLocalTimeOffset)
		return EFalse;

	if (aRule1.iNewLocalTimeOffset != aRule2.iNewLocalTimeOffset)
		return EFalse;

	if (aRule1.iMonth != aRule2.iMonth)
		return EFalse;
	
	if (aRule1.iDayRule != aRule2.iDayRule)
		return EFalse;
	
	if (aRule1.iDayOfMonth != aRule2.iDayOfMonth)
		return EFalse;
	
	if (aRule1.iDayOfWeek != aRule2.iDayOfWeek)
		return EFalse;
	
	if (aRule1.iTimeReference != aRule2.iTimeReference)
		return EFalse;

	if (aRule1.iTimeOfChange != aRule2.iTimeOfChange)
		return EFalse;	
	
	return ETrue;	
	}
	
void CEncodedRulesStep::ReadExpectedRulesL(CTzRules& aExpectedRules, TInt aExpectedNumberOfRules)
	{
	HBufC* ruleSection;
	TBuf<KMaxTagSize> ruleTag;
	TPtrC ruleSectionPtr;
	
	for (TInt i = 0; i < aExpectedNumberOfRules; i++)
		{
		ruleTag.Format(KRuleTag,i+1);
		ruleSection = HBufC::NewLC(KMaxTagSize);
		ruleSectionPtr.Set(ruleSection->Des());
		GetStringFromConfig(ConfigSection(),ruleTag,ruleSectionPtr);
		TTzRule* trule = ReadExpectedRuleL(ruleSectionPtr);
		CleanupStack::PushL(trule);
		aExpectedRules.AddRuleL(*trule);
		CleanupStack::PopAndDestroy(2,ruleSection);
		}
	}

TTzRule* CEncodedRulesStep::ReadExpectedRuleL(const TDesC& aRuleSection)
	{
	
	TInt fromYear;
	TInt toYear;
	TInt oldStdTimeOffset;
	TInt newStdTimeOffset;
	TInt month;
	TInt dayRule;
	TInt dayOfMonth;
	TInt dayOfWeek;
	TInt timeReference;
	TInt timeOfChange;
	
	GetIntFromConfig(aRuleSection, KRuleFromYearTag,		fromYear);
	GetIntFromConfig(aRuleSection, KRuleToYearTag,			toYear);
	GetIntFromConfig(aRuleSection, KRuleOldStdTimeOffsetTag,oldStdTimeOffset);
	GetIntFromConfig(aRuleSection, KRuleNewStdTimeOffsetTag,newStdTimeOffset);
	GetIntFromConfig(aRuleSection, KRuleMonthTag,			month);
	GetIntFromConfig(aRuleSection, KRuleDayRuleTag,			dayRule);
	GetIntFromConfig(aRuleSection, KRuleDayOfMonthTag,		dayOfMonth);
	GetIntFromConfig(aRuleSection, KRuleDayOfWeekTag,		dayOfWeek);
	GetIntFromConfig(aRuleSection, KRuleTimeReferenceTag,	timeReference);
	GetIntFromConfig(aRuleSection, KRuleTimeOfChangeTag,	timeOfChange);
		
	
	return new (ELeave) TTzRule((TInt16)fromYear,(TInt16)toYear,(TUint16)oldStdTimeOffset,(TUint16)newStdTimeOffset,
								(TMonth)(month?month-1:0),(TTzRuleDay)dayRule,(TUint8)(dayOfMonth?dayOfMonth-1:0),
								(TUint8)dayOfWeek,(TTzTimeReference)timeReference,(TUint16)timeOfChange);
	}
