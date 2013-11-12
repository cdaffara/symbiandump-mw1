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


#include <e32test.h>
#include <tz.h>
#include <vtzrules.h>
#include <e32math.h>
#include <tzconverter.h>
#include "testserver.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <tzusernames.h>
#include <tzuserdefineddata.h>
#endif
#include <tzlocalizedcityrecord.h>
#include <tzlocalizedtimezonerecord.h>
#include "tzidinternal.h"


_LIT(KTzUserDataDb, "c:\\private\\1020383e\\SQLite__tzuserdata.db");

const TInt KPrintFreq=500;

static RPIMTestServer Serv;
// Forward class references.	
class CTzUserDataTest;


_LIT(KTestName,"User-Defined TZ Data Test Suite");
RTest test(KTestName);


_LIT8(KTzLondon, "Europe/London");


typedef void (CTzUserDataTest::*OOMTestFunction)();


class CTzUserDataTest : public CBase
	{
public:
	enum TOOMTest  
		{
		EClientOOM,
		EServerOOM 
		};
	
	enum TTest
		{
		ETimeConversion,
		EDayLightSave,
		EDeleteCurrentUserRule,
		EUpdateCachedTz,
		EDeleteCachedTz,
		EPersistence
		};
	
    static CTzUserDataTest* NewLC();
	~CTzUserDataTest();
	
	// Basic test - client operations on user-defined time zones.
	void TestBasicUserDataL();
	void TestOutOfMemoryBehaviourL(TOOMTest aOOMTest);
	void TestTimeZoneRulesL(TTest aWhatToTest);	
	void TestServerResponseToChangesL();
	void TestReadIdsL(RTz& aRTz);
	void TestIdsL(RTz& aRTz);
	void TestTimeConversionL();
	void TestDaylightSaveL();
	void TestDeleteCurrentTimeZoneL(RTz& aTz);
	void TestCurrentTimeZonePersistenceL(RTz& aTz);
	void TestUpdateCachedUserTimeZoneL(RTz& aTz);
	void TestDeletedCachedUserTimeZoneL(RTz& aTz);
	void BoundaryTestL();
	void TestTzIdReusabilityL();
	void NullTestL();
	void TestShortTzId(RTz& aRTz);

private:
	CTzUserDataTest();

	// Basic operations.
	void TestCreateUserDefinedTzL();
	void TestUpdateUserDefinedTzL();
	void TestDeleteUserDefinedTzL();
	void ReadIdsAndTestL(RTz& aRTz, TBool aCheckId);

	// Create user-defined time zone.
	void CreateTzAndCheckL(TInt aOldOffset, TInt aNewOffset, RTz& aRTz, TTest aWhatToTest);	
	CTzId* CreateUserDefinedTzL(TInt16 aStandardOffset,TInt16 aDSTOffset, TBool aCheck);
	void CreateUserTzBasedOnLondonRuleL(RTz& aRTz, TTest aWhatToTest);
	void CreateUserTzBasedOnShanghaiRuleL(RTz& aRTz, TTest aWhatToTest);
	CTzUserNames* CreateUserDefinedTzNamesLC();
	CTzRules* CreateUserDefinedTzLC(TInt16 aStandardOffset,TInt16 aDSTOffset);
	
	// OOM tests.
	void CreateOOML();
	void ReadOOML();
	void GetIdsOOML();
	void UpdateOOML();
	void DeleteOOML();
	void OOMTestL(OOMTestFunction aFunction, TOOMTest  aOOMTest);

	// TZ Server response to changes.
	void DeleteCurrentTzL(RTz& aTz);
	void UpdateCurrentTzL(RTz& aTz,TInt aOldOffSet, TInt aNewOffSet);
	void DeleteCurrentUserTzL(RTz& aTz, TTest aWhatToTest);
	void UpdateCurrentUserTzL(RTz& aTz);
	void CreateUserTzAndSetCurrentTimeToL(RTz& aTz, TInt aStandardOffSet, TInt aDSTOffset);
	void CachedUserTimeZoneL(RTz& aRTz, TTest aWhatToTest);

	// Utility methods.
	void CreateString(TDes& aString);
	TBool CompareArray(const RPointerArray<CTzId>& aIds1, const RPointerArray<CTzId>& aIds2);
	TBool CompareNames(const CTzUserNames& aName1, const CTzUserNames& aNames);
	void TestDaylightSavingStateL(CTzId& aTzId, const RArray<TTime>& times, TBool aIsOn, RTz& aRTz);
	void TestUtcOffsetL(CTzId& aTzId,const CTzRules& rules, const RArray<TTime>& times, TInt aOffset, RTz& aRTz);
	void CheckTimeZoneL(const CTzId& aId, const CTzRules& aRules, const CTzUserNames& aNames);

	void ResetTzUserDataL();
	
private:
	RPointerArray<CTzId> 	iTzIds;	//Used for transfering time zone IDs in delete and update test.
	CTzUserData*			iUserData;
	TInt64					iSeed;
	};


CTzUserDataTest* CTzUserDataTest::NewLC()
	{
	CTzUserDataTest* self = new (ELeave) CTzUserDataTest();
	CleanupStack::PushL(self);
	return (self);
	}


CTzUserDataTest::CTzUserDataTest()
	:iSeed(100)
	{
	}	


CTzUserDataTest::~CTzUserDataTest()	
	{
	TRAP_IGNORE(ResetTzUserDataL());
	delete iUserData;
	}


LOCAL_D void ResetTimeZoneToLondonL()
	{
	RTz tz;
	User::LeaveIfError(tz.Connect());
	CleanupClosePushL(tz);
	
	CTzId* id = CTzId::NewL(KTzLondon);
	CleanupStack::PushL(id);
	tz.SetTimeZoneL(*id);
	CleanupStack::PopAndDestroy(2, &tz);
	}


LOCAL_D void ResetAndDestroyTzIdArray(TAny* aPtr)
	{
	RPointerArray<CTzId>* ids = static_cast<RPointerArray<CTzId>*>(aPtr);
	if (ids)
		{
		ids->ResetAndDestroy();
		}
	}

		
void CTzUserDataTest::CreateString(TDes& aString)
	{
	for (TInt i(0) ; i < 10 ; ++i)
		{
		// Add random upper and lower case letters
		aString.Append( 'A' + (Math::Rand(iSeed) % 26) );
		}
	}


TBool CTzUserDataTest::CompareArray(const RPointerArray<CTzId>& aIds1, const RPointerArray<CTzId>& aIds2)
	{
	TInt count1 = aIds1.Count();
	TInt count2 = aIds2.Count();
	test(count1 == count2);
	
	TBool isEqual = ETrue;
	TInt ii=0;
	
	while(ii<count1 && isEqual)
		{
		isEqual = EFalse;
		for (TInt jj=0; jj<count2; ++jj)
			{
			if(aIds1[ii]==aIds2[jj])
				{
				isEqual = ETrue;
				break;	
				}
			}
		++ii;
		}
	return isEqual;
	}


TBool CTzUserDataTest::CompareNames(const CTzUserNames& aNames1, const CTzUserNames& aNames2)
	{
	if (aNames1.StandardName() !=  aNames2.StandardName())
		return EFalse; 
	if (aNames1.ShortStandardName() != aNames2.ShortStandardName())
		return EFalse;
	if (aNames1.DaylightSaveName() != aNames2.DaylightSaveName())
		return EFalse;
	if (aNames1.ShortDaylightSaveName() != aNames2.ShortDaylightSaveName())
		return EFalse;
	if (aNames1.CityName() != aNames2.CityName())
		return EFalse;
	if (aNames1.RegionName() != aNames2.RegionName())
		return EFalse;
	
	return ETrue;
	}

	
void CTzUserDataTest::TestUtcOffsetL(CTzId& aTzId, const CTzRules& rules, const RArray<TTime>& times, TInt aOffset, RTz& aRTz )
	{
	aRTz.SetTimeZoneL(aTzId);

	TInt count = times.Count();
	
	for(TInt ii=0; ii<count; ++ii)
		{
		test (aOffset == rules.GetOffsetL(times[ii], ETzUtcTimeReference));	
		TTime utc = times[ii];
		aRTz.ConvertToUniversalTime(utc);
		TTimeIntervalMinutes minutes;
		times[ii].MinutesFrom(utc, minutes);
		test (minutes == TTimeIntervalMinutes(aOffset));	
		}
	}


void CTzUserDataTest::TestDaylightSavingStateL(CTzId& aTzId, const RArray<TTime>& times, TBool aIsOn, RTz& aRTz)
	{
	aRTz.SetTimeZoneL(aTzId);
	TInt count = times.Count();
	for(TInt ii=0; ii<count; ++ii)
		{
		test (aIsOn == aRTz.IsDaylightSavingOnL(aTzId, times[ii]));
		}
	}


void CTzUserDataTest::ResetTzUserDataL()
	{
	RTz tz;
	User::LeaveIfError(tz.Connect());
	CleanupClosePushL(tz);
	CTzUserData* userdata = CTzUserData::NewL(tz);
	CleanupStack::PushL(userdata);
	userdata->GetTzIdsL(iTzIds);
	TInt count = iTzIds.Count();
	for (TInt ii=0; ii<count; ++ii)
		{
		if(ii%KPrintFreq == 0)
			{
			test.Printf(_L("Deleted %d time zones in the database\n"), ii);
			}
		
		userdata->DeleteL(*iTzIds[ii]);	
		}
	CleanupStack::PopAndDestroy(2, &tz);
	
	iTzIds.ResetAndDestroy();
	}
	

CTzId* CTzUserDataTest::CreateUserDefinedTzL(TInt16 aStandardOffset,TInt16 aDSTOffset, TBool aCheck)
	{
	CTzRules* rules = CreateUserDefinedTzLC(aStandardOffset, aDSTOffset);
	CTzUserNames* names = CreateUserDefinedTzNamesLC();
	CTzId* id = iUserData->CreateL(*rules, *names);
	CleanupStack::PushL(id);
	if(aCheck)
		{
		CheckTimeZoneL(*id, *rules, *names);
		}
	CleanupStack::Pop(id);
	CleanupStack::PopAndDestroy(2, rules);
	return id;
	}

	
void CTzUserDataTest::CheckTimeZoneL(const CTzId& aId, const CTzRules& aRules, const CTzUserNames& aNames)
	{
	CTzRules* rulesGet = iUserData->ReadRulesL(aId);
	CleanupStack::PushL(rulesGet);
	test(aRules.IsEqualTo(*rulesGet));
	CleanupStack::PopAndDestroy(rulesGet);
		
	CTzUserNames* namesGet = iUserData->ReadNamesL(aId);
	CleanupStack::PushL(namesGet);
	test (CompareNames(aNames, *namesGet));
	CleanupStack::PopAndDestroy(namesGet);
	}

	
void CTzUserDataTest::CreateUserTzBasedOnLondonRuleL(RTz& aRTz, TTest aWhatToTest)
	{
	test.Next(_L("Test Creation user-defined time zone based on London rules"));
	//create a new rule which doesn't have DST saving.
	TTime start (TDateTime(2010, ESeptember, 0, 0, 0, 0, 0));
	TInt year = start.DateTime().Year();
	TMonth month = start.DateTime().Month();
	TInt day = start.DateTime().Day();
	TTzRule newrule(year, 9999, 0, 0, month,  ETzFixedDate, day,  0, ETzWallTimeReference, 120);

	//Get the existing london rule
	_LIT8(KTimeNone, "Europe/London");
	
	CTzId* londonId = CTzId::NewL(KTimeNone());
	CleanupStack::PushL(londonId);
	CTzRules* userrule = aRTz.GetTimeZoneRulesL(*londonId, 0, 9999, ETzUtcTimeReference);
	CleanupStack::PopAndDestroy(londonId); 

	//Use London rule to create a new user-defined rule
	TInt count = userrule->Count();

	//Since the last two rules (TTzRule objects one for summer rule one for winter rule) in existing database covers the years in which a new rule starts.
	//The client has to amend the ending years of those two rules before adding a new rule.
	//We set the ending year for last two rules to 2009 since the new rule starts on 2010

	TTzRule lastRule = (*userrule)[count-1];
	TDateTime dateTimeLast = lastRule.iTo.iTime.DateTime();
	dateTimeLast.SetYear(2009);
	lastRule.iTo.iTime = dateTimeLast;

	TTzRule lastSecondRule = (*userrule)[count-2];
	TDateTime dateTimeLastSecond = lastSecondRule.iTo.iTime.DateTime();
	dateTimeLastSecond.SetYear(2009);
	lastSecondRule.iTo.iTime = dateTimeLastSecond;
		
	CleanupStack::PushL(userrule);
	//Remove last two existing rules
	userrule->RemoveRule(count-1);
	userrule->RemoveRule(count-2);
	
	//Add two rules whoes ending years have been amended
	userrule->AddRuleL(lastRule);
	userrule->AddRuleL(lastSecondRule);
	
	//Add the new rule
	userrule->AddRuleL(newrule);
	//userrule->AddRuleL(newrule);//add the new rule which doesn't have DST
	
	CTzUserNames* newRuleNames = CreateUserDefinedTzNamesLC();
	CTzId* id = iUserData->CreateL(*userrule, *newRuleNames);
	CleanupStack::PushL(id);
	iTzIds.AppendL(id);
	CleanupStack::Pop(id);
	CheckTimeZoneL(*id, *userrule, *newRuleNames);
	CleanupStack::PopAndDestroy(newRuleNames);
		
	RArray<TTime> timesOn;
	CleanupClosePushL(timesOn);
	RArray<TTime> timesOff;
	CleanupClosePushL(timesOff);
	
	// Check UTC offset and DST.
	TTime check = TDateTime(1998, ESeptember, 0, 0, 0, 0, 0); 
	timesOn.AppendL(check);
	check = TDateTime(2005, ESeptember, 0, 0, 0, 0, 0);
	timesOn.AppendL(check);	
	check = TDateTime(2009, ESeptember, 0, 0, 0, 0, 0);
	timesOn.AppendL(check);

	check = TDateTime(2010, ESeptember, 0, 3, 0, 0, 0); 
	timesOff.AppendL(check);
	check = TDateTime(2015, ESeptember, 0, 3, 0, 0, 0);
	timesOff.AppendL(check);	
	check = TDateTime(2020, ESeptember, 0, 3, 0, 0, 0);
	timesOff.AppendL(check);

	if(aWhatToTest==ETimeConversion)
		{
		TestUtcOffsetL(*id,*userrule, timesOn, 60, aRTz);
		TestUtcOffsetL(*id,*userrule, timesOff, 0, aRTz);
		}
	else if(aWhatToTest==EDayLightSave)
		{
		TestDaylightSavingStateL(*id, timesOn, ETrue, aRTz);
		TestDaylightSavingStateL(*id, timesOff, EFalse, aRTz);
		}
	
	CleanupStack::PopAndDestroy(3, userrule);	
	}


void CTzUserDataTest::CreateUserTzBasedOnShanghaiRuleL(RTz& aRTz, TTest aWhatToTest)
	{
	_LIT8(KTimeNone, "Asia/Shanghai");
	
	CTzId* shanghaiId = CTzId::NewL(KTimeNone());
	CleanupStack::PushL(shanghaiId);

	CTzRules* shanghaiRule = aRTz.GetTimeZoneRulesL(*shanghaiId, 0, 9999, ETzUtcTimeReference);
	CleanupStack::PopAndDestroy(shanghaiId); 
	
	// Last rule: 00:00:000 Date:1/01/1987  Time: 23:59:59:000 Date:31/12/1991  Old=480 New=540 Ch=0  
	TTime dston (TDateTime(2010, EMarch, 0, 0, 0, 0, 0)); 
	TTime dstoff (TDateTime(2010, ESeptember, 0, 0, 0, 0, 0));
	TMonth monthOn = dston.DateTime().Month();
	TInt dayOn = dston.DateTime().Day();
	TMonth monthOff = dstoff.DateTime().Month();
	TInt dayOff = dstoff.DateTime().Day();
	 
	test.Next(_L("Test Creation user-defined time zone based on Shanghai rules"));
	
	TTzRule dstOn(2010, 9999, 480, 540, monthOn,  ETzFixedDate, dayOn,  0, ETzWallTimeReference, 120);
	TTzRule dstOff (2010, 9999, 540, 480, monthOff, ETzFixedDate, dayOff, 0, ETzWallTimeReference, 120);
	
	CleanupStack::PushL(shanghaiRule);
	shanghaiRule->AddRuleL(dstOff);//add the new rule which have DST
	shanghaiRule->AddRuleL(dstOn);
	CTzUserNames* newRuleNames = CreateUserDefinedTzNamesLC();

	CTzId* id = iUserData->CreateL(*shanghaiRule, *newRuleNames);
	CleanupStack::PushL(id);
	iTzIds.AppendL(id);
	CleanupStack::Pop(id);
	
	CheckTimeZoneL(*id, *shanghaiRule, *newRuleNames);
	
	// Check UTC offset and DST.
	RArray<TTime> timesOn;
	CleanupClosePushL(timesOn);
	RArray<TTime> timesOff;
	CleanupClosePushL(timesOff);

	TTime check = TDateTime(1998, ESeptember, 0, 3, 0, 0, 0); 
	timesOff.AppendL(check);
	check = TDateTime(2005, ESeptember, 0, 3, 0, 0, 0);
	timesOff.AppendL(check);	
	check = TDateTime(2020, ESeptember, 0, 3, 0, 0, 0);
	timesOff.AppendL(check);

	check = TDateTime(2011, EAugust, 0, 0, 0, 0, 0); 
	timesOn.AppendL(check);
	check = TDateTime(2015, EAugust, 0, 0, 0, 0, 0);
	timesOn.AppendL(check);	
	check = TDateTime(2020, EAugust, 0, 0, 0, 0, 0);
	timesOn.AppendL(check);

	if(aWhatToTest==ETimeConversion)
		{
		TestUtcOffsetL(*id,*shanghaiRule, timesOn, 540, aRTz);
		TestUtcOffsetL(*id,*shanghaiRule, timesOff, 480, aRTz);
		}
	else if(aWhatToTest==EDayLightSave)
		{
		TestDaylightSavingStateL(*id, timesOn, ETrue, aRTz);
		TestDaylightSavingStateL(*id, timesOff, EFalse, aRTz);
		}
	
	CleanupStack::PopAndDestroy(4, shanghaiRule);
	}	


CTzRules* CTzUserDataTest::CreateUserDefinedTzLC(TInt16 aStandardOffset,TInt16 aDSTOffset)
	{
	TTime dston (TDateTime(2005, EMarch, 0, 0, 0, 0, 0)); 
	TTime dstoff (TDateTime(2005, ESeptember, 0, 0, 0, 0, 0));
	TMonth monthOn = dston.DateTime().Month();
	TInt dayOn = dston.DateTime().Day();
	TMonth monthOff = dstoff.DateTime().Month();
	TInt dayOff = dstoff.DateTime().Day();

	TTzRule dstOff(0, 9999, aStandardOffset, aDSTOffset, monthOn,  ETzFixedDate, dayOn,  0, ETzWallTimeReference, 120);
	TTzRule dstOn (0, 9999, aDSTOffset, aStandardOffset, monthOff, ETzFixedDate, dayOff, 0, ETzWallTimeReference, 120);

	CTzRules* rules = CTzRules::NewL(0, 9999);
	CleanupStack::PushL(rules);
	rules->AddRuleL(dstOff);
	rules->AddRuleL(dstOn);
	return rules;
	}


CTzUserNames* CTzUserDataTest::CreateUserDefinedTzNamesLC()
	{
	TBuf<10> standardName;
	CreateString(standardName);
	
	TBuf<10> shortStandardName;
	CreateString(shortStandardName);
	
	TBuf<10> dayLightSaveName;
	CreateString(dayLightSaveName);
	
	TBuf<10> shortDayLightSaveName;
	CreateString(shortDayLightSaveName);
		
	TBuf<10> cityName;
	CreateString(cityName);
	
	TBuf<10> regionName;
	CreateString(regionName);
	
	return CTzUserNames::NewLC(standardName, shortStandardName, dayLightSaveName, shortDayLightSaveName, cityName, regionName);
	}

	
void CTzUserDataTest::CreateTzAndCheckL(TInt aOldOffset, TInt aNewOffset, RTz& aRTz, TTest aWhatToTest)	
	{
/* On the day:
 
	TTime dston (TDateTime(2005, EMarch, 0, 0, 0, 0, 0)); 
	TTime dstoff (TDateTime(2005, ESeptember, 0, 0, 0, 0, 0));

After two hours DST changes which should be repeated every year
*/
	RArray<TTime> timesOn;
	CleanupClosePushL(timesOn);
	RArray<TTime> timesOff;
	CleanupClosePushL(timesOff);
	
	TTime check = TDateTime(1920, EMarch, 0, 0, 0, 0, 0); 
	timesOff.AppendL(check);
	check = TDateTime(2005, EMarch, 0, 0, 0, 0, 0);
	timesOff.AppendL(check);	
	check = TDateTime(2020, EMarch, 0, 0, 0, 0, 0);
	timesOff.AppendL(check);
	
	check = TDateTime(1920, EMarch, 0, 3, 0, 0, 0); 
	timesOn.AppendL(check);
	check = TDateTime(2005, EMarch, 0, 3, 0, 0, 0);
	timesOn.AppendL(check);	
	check = TDateTime(2020, EMarch, 0, 3, 0, 0, 0);
	timesOn.AppendL(check);
	
	CTzId* id = CreateUserDefinedTzL(aOldOffset, aNewOffset, ETrue);
	CleanupStack::PushL(id);
	CTzRules* rules = iUserData->ReadRulesL(*id);
	CleanupStack::PushL(rules);
	
	if (aWhatToTest==ETimeConversion)
		{
		TestUtcOffsetL(*id,*rules, timesOn, aNewOffset, aRTz);
		TestUtcOffsetL(*id,*rules, timesOff, aOldOffset, aRTz);
		}
	else if (aWhatToTest==EDayLightSave)
		{
		TBool isDSTon = aOldOffset != aNewOffset;
		TestDaylightSavingStateL(*id, timesOn, isDSTon, aRTz);
		TestDaylightSavingStateL(*id, timesOff, EFalse, aRTz);
		}

	CleanupStack::PopAndDestroy(4, &timesOn);
	}
		

void CTzUserDataTest::CreateOOML()
	{
	CTzId* id = CreateUserDefinedTzL(60, 60, EFalse);
	delete id;
	}


void CTzUserDataTest::ReadOOML()
	{
	CTzRules* rule = iUserData->ReadRulesL(*iTzIds[0]);
	CleanupStack::PushL(rule);
	CTzUserNames* newNames = iUserData->ReadNamesL(*iTzIds[0]);
	CleanupStack::PopAndDestroy(rule);
	delete newNames;
	}

	
void CTzUserDataTest::GetIdsOOML()
	{
	RPointerArray<CTzId> ids;
	CleanupStack::PushL(TCleanupItem(ResetAndDestroyTzIdArray, &ids));
	iUserData->GetTzIdsL(ids);
	CleanupStack::PopAndDestroy(&ids);	
	}


void CTzUserDataTest::UpdateOOML()
	{
	CTzRules* rules = CreateUserDefinedTzLC(0, 0);
	CTzUserNames* newNames = CreateUserDefinedTzNamesLC();
	iUserData->UpdateL(*(iTzIds[0]), *rules, *newNames);
	CleanupStack::PopAndDestroy(2, rules);
	}	


void CTzUserDataTest::DeleteOOML()
	{
	for(TInt ii=iTzIds.Count()-1; ii>=0; --ii)
		{
		//Delete the user-defined time zone as the last element in the array.
		iUserData->DeleteL(*(iTzIds[ii]));
		}
	}


void CTzUserDataTest::OOMTestL(OOMTestFunction aFunction, TOOMTest  aOOMTest)
	{
	TInt tryCount = 1;
	
	RTz tz;
	User::LeaveIfError(tz.Connect());
	CleanupClosePushL(tz);
	iUserData = CTzUserData::NewL(tz);

	TInt cellsBefore = 0;
			
	for (;;)
		{
		test.Printf(_L("Heap memory allocation set to fail at allocation #%d.\n"),
			tryCount);

		if(aOOMTest == EClientOOM)
			{
			__UHEAP_SETFAIL(RAllocator::EDeterministic, tryCount);
			__UHEAP_MARK;
			}
		else
			{
			TInt heapSize = 0;
			tz.__dbgSetHeapFailL(RAllocator::EDeterministic, tryCount);
			cellsBefore = tz.__dbgRequestAllocatedCellsL(heapSize);
			}
		
		TRAPD(err, (this->*aFunction)());

		if(err == KErrNone)
   			{
   			test.Printf(_L("OOM test complete.\n"));
   			break;
   			}
		
		test(err == KErrNoMemory);
		
		if(aOOMTest == EClientOOM)
			{
			__UHEAP_MARKEND;	
			}
		else
			{
			tz.__dbgResetHeapL();
			TInt heapSize = 0;
			TInt cellsAfter = tz.__dbgRequestAllocatedCellsL(heapSize);
			if(cellsBefore != cellsAfter)
				{
				test.Printf(_L("ERROR: Memory leak.  Heap cells: %d before, %d after.\n"),
					cellsBefore, cellsAfter);
				test(EFalse);
				}
			}
  	
		tryCount++;
		}
	
	__UHEAP_RESET;
	tz.__dbgResetHeapL();
	
	delete iUserData;
	iUserData = NULL;
	
	CleanupStack::PopAndDestroy(&tz);	
	}

	
/**
Create and read user-defined time zones.

@SYMTestCaseID 	PIM-APPSERV-TZS-CO-0001

@SYMTestCaseDesc
	The purpose of this test is to verify that user-defined time zone rules and
	names can be created and read back.

@SYMTestActions    
	1.	Create a few user-defined time zone with different offset.
	2.	Read the rules and names for the user-defined time zone created in action
		1. 

@SYMTestExpectedResults
	The following information is the same as the information used to created
	the user-defined time zone in action 1:
	
	- Time zone rules
	- Standard time name
	- Daylight saving time name 
	- Short standard time name
	- Short daylight saving time name
	- City name
	- Region name

@SYMTestType
	CT

@SYMTestPriority
	1
*/


void CTzUserDataTest::TestCreateUserDefinedTzL()
	{
	test.Next(_L("@SYMTestCaseID PIM-APPSERV-TZS-CO-0001 Create user-defined Time Zone"));

	// Offset 60 There is no DST 
	test.Printf(_L("Offset 60 There is no DST\n"));

	CTzId* id = CreateUserDefinedTzL(60, 60, ETrue);
	CleanupStack::PushL(id);
	iTzIds.AppendL(id);
	CleanupStack::Pop(id);

	// Offset - 60 There is no DST
	test.Printf(_L("Offset -60 There is no DST\n"));
	id = CreateUserDefinedTzL(-60, -60, ETrue);
	CleanupStack::PushL(id);
	iTzIds.AppendL(id);
	CleanupStack::Pop(id);
	
	// Offset 60 There is DST of 60
	id = CreateUserDefinedTzL(60, 120, ETrue);
	CleanupStack::PushL(id);
	iTzIds.AppendL(id);
	CleanupStack::Pop(id);

	// Offset - 60 There is DST of 60
	test.Printf(_L("Offset - 60 There is DST of 60\n"));
	id = CreateUserDefinedTzL(-60, 0, ETrue);
	CleanupStack::PushL(id);
	iTzIds.AppendL(id);
	CleanupStack::Pop(id);
	}


/**
Update user-defined time zone.

@SYMTestCaseID 	PIM-APPSERV-TZS-CO-0002

@SYMTestCaseDesc
	The purpose of this test is to verify that user-defined time zone time rules
	and names can be updated.

@SYMTestActions    
	1.	Create a user-defined time zone (rules and names).
	2.	Get all existing user-defined time zones and update them with the rules
		and names created in action 1.
	3.	Read the new user-defined time zone rules and names updated in action 2. 

@SYMTestExpectedResults
	The following information is the same as the information used to update the
	user-defined time zone in action 3:
	
	- Time zone rules
	- Standard time name
	- Daylight saving time name 
	- Short standard time name
	- Short daylight saving time name
	- City name
	- Region name

@SYMTestType
	CT
	
@SYMTestPriority
	1
*/
void CTzUserDataTest::TestUpdateUserDefinedTzL()
	{
	test.Next(_L("@SYMTestCaseID PIM-APPSERV-TZS-CO-0002 "));
	CTzRules* newRule = CreateUserDefinedTzLC(0,0);
	CTzUserNames* newNames = CreateUserDefinedTzNamesLC();
	
	for(TInt ii=0; ii<iTzIds.Count(); ++ii)
		{
		iUserData->UpdateL(*(iTzIds[ii]), *newRule, *newNames);
	
		//Get and Compare
		CTzRules* userRuleGet = iUserData->ReadRulesL(*(iTzIds[ii]));
		test (userRuleGet->IsEqualTo(*newRule));
		delete userRuleGet;

		CTzUserNames* userNamesGet = iUserData->ReadNamesL(*(iTzIds[ii]));
		test(CompareNames(*userNamesGet, *newNames));
		delete userNamesGet;
		}
	
	CleanupStack::PopAndDestroy(2, newRule);
	}


/**
Delete user-defined time zones.

@SYMTestCaseID 	PIM-APPSERV-TZS-CO-0003

@SYMTestCaseDesc
	The purpose of this test is to verify that user-defined time zones can be
	deleted.

@SYMTestActions  
	1.	Get all existing user-defined time zones.
	2.	Delete the user-defined time zone as the last element in the array one
		by one.
	3.	Try to read the user-defined time zone deleted in action 2.

@SYMTestExpectedResults
	The user-defined time zone does not exist.

@SYMTestType
	CT

@SYMTestPriority
	1
*/
void CTzUserDataTest::TestDeleteUserDefinedTzL()	
	{
	test.Next(_L("@SYMTestCaseID  PIM-APPSERV-TZS-CO-0003"));
	RPointerArray<CTzId> ids;
	CleanupStack::PushL(TCleanupItem(ResetAndDestroyTzIdArray, &ids));

	for(TInt ii=iTzIds.Count()-1; ii>=0; --ii)
		{
		iUserData->GetTzIdsL(ids);
		
		// The array obtained should be same as expected who elements is deleted
		// one by one in each loop.
		CompareArray(ids,iTzIds);
		ids.ResetAndDestroy();
		
		// Delete the user-defined time zone as the last element in the array.
		iUserData->DeleteL(*(iTzIds[ii]));
		
		// The rule and names of the last element in the array should not be
		// found since it has been deleted.
		TRAPD(err, iUserData->ReadRulesL(*(iTzIds[ii])));
		test(err == KErrNotFound);
		TRAP(err, iUserData->ReadNamesL(*(iTzIds[ii])));
		test(err == KErrNotFound);

		delete iTzIds[ii];
		iTzIds.Remove(ii);
		}

	CleanupStack::PopAndDestroy(&ids);
	}


/**
All user-defined time zone identifiers.

@SYMTestCaseID PIM-APPSERV-TZS-CO-0004 

@SYMTestCaseDesc
	The purpose of this test is to verify that time zone identifiers for user-
	defined time zones can be queried.

@SYMTestActions    
	1.	Get all existing user-defined time zones.

@SYMTestExpectedResults
	The time zone identifiers obtained should be same as those existing ones.

@SYMTestType
	CT

@SYMTestPriority
	1
*/	


void CTzUserDataTest::TestReadIdsL(RTz& aRTz)
	{
	test.Next(_L("@SYMTestCaseID PIM-APPSERV-TZS-CO-0004  Read time zone IDs"));
	ReadIdsAndTestL(aRTz, EFalse);
	}


/**
Check time zone identifier is a user-defined time zone identifier.
	
@SYMTestCaseID 	PIM-APPSERV-TZS-CO-0005

@SYMTestCaseDesc
	The purpose of this test is to verify that a time zone identifier is or is
	not a user-defined time zone identifier.

@SYMTestActions 
	1.	Get all existing user-defined time zones.
	2.	Obtain a time zone identifier for a non-user-defined time zone

@SYMTestExpectedResults     
	  - Check that the time zone identifier is a user-defined time zone
		identifier.
	  - Check that the time zone identifier is not a user-defined time zone 

@SYMTestType
	CT

@SYMTestPriority
	1
*/	


void CTzUserDataTest::TestIdsL(RTz& aRTz)
	{
	test.Next(_L("@SYMTestCaseID PIM-APPSERV-TZS-CO-0005 Check time zone IDs"));
	ReadIdsAndTestL(aRTz, ETrue);
	}

	
void CTzUserDataTest::TestShortTzId(RTz& aRTz)
	{
	test.Next(_L("test short TzId behaviour"));
	TInt KGreaterThanExpectedVal = 65536; //a value greater than 0xffff i.e. 16 bits
	TUint8 groupId = 8; //make up some random value
	TUint resourceId = 123; //make up some random value
	TRAPD(err, aRTz.LocalizationWriteCityL(_L("myCity"), KGreaterThanExpectedVal, groupId, resourceId));
	test(err == KErrArgument);
	TRAPD(err1, aRTz.LocalizationDeleteCityL(_L("myCity"), KGreaterThanExpectedVal));
	test(err1 == KErrArgument);
	
	TRAPD(err2, aRTz.LocalizationDeleteCityL(_L("myCity"), KGreaterThanExpectedVal - 1));
	test(err2 == KErrNotFound);
	
	TRAPD(err3, CTzLocalizedTimeZoneRecord::NewLC(KGreaterThanExpectedVal, _L("standardName"), _L("daylightName"), _L("shortStandardName"), _L("shortDaylighName"), 0));
	test(err3 == KErrArgument);
	
	TRAPD(err4, CTzLocalizedCityRecord::NewLC(_L("aCity"), 0, 0, KGreaterThanExpectedVal, 0));
	test(err4 == KErrArgument);
	}

	
	
void CTzUserDataTest::ReadIdsAndTestL(RTz& aRTz, TBool aCheckId)
	{
	test.Next(_L("Read time zone IDs"));
	RPointerArray<CTzId> ids;
	CleanupStack::PushL(TCleanupItem(ResetAndDestroyTzIdArray, &ids));
	iUserData->GetTzIdsL(ids);
	test(iTzIds.Count() == ids.Count());
	if(aCheckId)
		{
		TInt count = ids.Count();
		for(TInt ii = 0; ii<count; ++ii)
			{
			test(ids[ii]->IsUserTzId());	
			}
		//Current time zone is default time zone Europe/London which is not the user-defined time zone.
		CTzId* currentId = aRTz.GetTimeZoneIdL();
		test (!currentId->IsUserTzId());
		delete currentId;
		}
	
	CleanupStack::PopAndDestroy(&ids);
	}

	
void CTzUserDataTest::TestBasicUserDataL()
	{
	ResetTzUserDataL();
	RTz tz;
	User::LeaveIfError(tz.Connect());
	CleanupClosePushL(tz);
	iUserData = CTzUserData::NewL(tz);
	TestCreateUserDefinedTzL();
	TestReadIdsL(tz);
	TestIdsL(tz);
	TestUpdateUserDefinedTzL();
	TestDeleteUserDefinedTzL();	
	TestShortTzId(tz);
	delete iUserData;
	iUserData = NULL;
	CleanupStack::PopAndDestroy(&tz);
	}

	
/**
Out of memory behaviour.

@SYMTestCaseID PIM-APPSERV-TZS-CO-0006

@SYMTestCaseDesc
	The purpose of this test is to verify that user-defined time zone operations
	are robust and result in no memory leaks under OOM conditions.
	
@SYMTestActions
	Create, Read, Update and Delete a user-defined time zone rule under OOM
	conditions.

@SYMTestExpectedResults
	Each operation leaves with KErrNoMemory until eventually the operation
	completes successfully.  Once the operation completes successfully the
	client process (for client-side OOM testing) or TZ Server process (for
	server-side OOM testing) is terminated and does not report a memory leak.

@SYMTestType
	CT

@SYMTestPriority
	1
*/
void CTzUserDataTest::TestOutOfMemoryBehaviourL(TOOMTest aOOMTest)
	{
	ResetTzUserDataL();

	
	test.Start(_L("@SYMTestCaseID PIM-APPSERV-TZS-CO-0006 Test Create User-Defined Time Zone OOM"));
	OOMTestL(&CreateOOML,aOOMTest);
	
	// Ensure that a tz ID is saved in iTzIds which is used to fetch a time zone
	// in ReadOOML().
	RTz tz;
	User::LeaveIfError(tz.Connect());
	CleanupClosePushL(tz);
	iUserData = CTzUserData::NewL(tz);
	iUserData->GetTzIdsL(iTzIds);
	delete iUserData;
	iUserData = NULL;
	CleanupStack::PopAndDestroy(&tz);
	
	test.Next(_L("Test Read User-Defined Time Zone OOM"));
	OOMTestL(&ReadOOML,aOOMTest);

	test.Next(_L("Test Get User-Defined Time Zone IDs OOM"));
	OOMTestL(&GetIdsOOML,aOOMTest);

	test.Next(_L("Test Update User-Defined Time Zone OOM"));
	OOMTestL(&UpdateOOML,aOOMTest);

	test.Next(_L("Test Delete User-Defined Time Zone OOM"));
	OOMTestL(&DeleteOOML,aOOMTest);
	
	test.End();

	ResetTzUserDataL();
	}
 
 
/**
Boundary tests.
 
@SYMTestCaseID PIM-APPSERV-TZUSERDATA-0013
 	
@SYMTestCaseDesc
	The purpose of this test is to test behaviour under various boundary
	conditions.

@SYMTestActions     
	1.  Get TZ Ids of  user-defined time zone when the database is empty.
	2.  Get the user-defined rule for a valid TZ Id when the database is empty.
	3.  Get the user-defined rule names when the database is empty.
	4.  Create a user time zone with all named KNullDesC
	5.  Get rules, names and try to update and delete using a non-user-defined
		TZID.

@SYMTestExpectedResults
	Array obtained in action 1 has 0 count.  Leaving code KErrNotFound is
	expected in action 2 and 3.  A user time zone with all named KNullDesC
	should be allowed.

@SYMTestType
	CT

@SYMTestPriority
	1
*/	

void CTzUserDataTest::BoundaryTestL()
	{
	test.Next(_L("@SYMTestCaseID PIM-APPSERV-TZUSERDATA-0013 Boundary Test"));
	ResetTzUserDataL();
	
	RTz tz;
	User::LeaveIfError(tz.Connect());
	CleanupClosePushL(tz);
	iUserData = CTzUserData::NewL(tz); 

	// Empty user time zone database.
	iUserData->GetTzIdsL(iTzIds);
	test (iTzIds.Count() == 0);
	
	CTzId * id = CTzId::NewL(KUserTzIdMin);
	CleanupStack::PushL(id);
	CTzRules* rulesGet = NULL;
	TRAPD(err, rulesGet = iUserData->ReadRulesL(*id));
	test (!rulesGet);
	test (err==KErrNotFound);

	CTzUserNames* namesGet = NULL;
	TRAP(err, namesGet = iUserData->ReadNamesL(*id));
	test (!namesGet);
	test (err==KErrNotFound);
	iUserData->DeleteL(*id);
	CleanupStack::PopAndDestroy(id);

	// Create a user time zone with all named KNullDesC.
	CTzRules*  rules = CreateUserDefinedTzLC(0, 0);
	CTzUserNames* names = CTzUserNames::NewLC(KNullDesC , KNullDesC, KNullDesC, KNullDesC, KNullDesC, KNullDesC);
	id = iUserData->CreateL(*rules, *names);
	CleanupStack::PushL(id);
	CheckTimeZoneL(*id, *rules, *names);

	// Create a non-user-defined TZID object and reuse it in the following
	// tests.
	CTzId* nonUserDefinedTzId = CTzId::NewL(KUserTzIdMin - 1);
	CleanupStack::PushL(nonUserDefinedTzId);

	// Attempt to update user-defined TZ data using a TZID which is not a user-
	// defined TZID.
	TRAP(err, iUserData->UpdateL(*nonUserDefinedTzId, *rules, *names));
	test(err == KErrArgument);

	// Attempt to read rules from user-defined TZ data using a TZID which is
	// not a user-defined TZID.
	TRAP(err, rulesGet = iUserData->ReadRulesL(*nonUserDefinedTzId));
	test(err == KErrArgument);
		
	// Attempt to read names from user-defined TZ data using a TZID which is
	// not a user-defined TZID.
	TRAP(err, namesGet = iUserData->ReadNamesL(*nonUserDefinedTzId));
	test(err == KErrArgument);
	
	// Attempt to delete from user-defined TZ data using a TZID which is not
	// a user-defined TZID.
	TRAP(err, iUserData->DeleteL(*nonUserDefinedTzId));
	test(err == KErrArgument);

	CleanupStack::PopAndDestroy(4, rules);

	delete iUserData;
	CleanupStack::PopAndDestroy(&tz);
	ResetTzUserDataL();
	}


/**
Convert time using user-defined time zone.

@SYMTestCaseID PIM-APPSERV-TZS-TZR-0001

@SYMTestCaseDesc
	The purpose of this test is to verify the behaviour of time conversion using
	a user-defined time zone.

@SYMTestActions 
	1.	Create a few user-defined time zone with different UTC offsets and store
		them in the database.
	2.	Get the UTC offset of each time zone created.
	3.	Set the current time zone to each user-defined time zone created in
		action 1.
	4.	Convert the system time from UTC to local.
	5. 	Verify the UTC offset for each user-defined time zone.

@SYMTestExpectedResults
	The UTC offset for the given time zone created is correct.

@SYMTestType
	CT

@SYMTestPriority
	1
*/
void CTzUserDataTest::TestTimeConversionL()
	{
	TestTimeZoneRulesL(ETimeConversion);
	}


/**
Daylight saving observation using user-defined time zone.

@SYMTestCaseID	PIM-APPSERV-TZS-TZR-0002

@SYMTestCaseDesc
	The purpose of this test is to verify the behaviour of daylight saving time
	zone rules for a user-defined time zone.

@SYMTestActions 
	1.	Create a user-defined time zone.
	2.	Set the current time zone to the user-defined time zone created in
		action 1.
	3.	Check if the daylight saving is on to a time that is inside the time
		interval during which DST is observed. [1]
	4.	Check if the daylight saving is on to a time that is outside the time
		interval during which DST is not observed. [2]

@SYMTestExpectedResults     
	[1]	The system indicates that DST is currently being observed.
	[2]	The system indicates that DST is not currently being observed.

@SYMTestType
	CT
	
@SYMTestPriority
	1
*/
void CTzUserDataTest::TestDaylightSaveL()
	{
	TestTimeZoneRulesL(EDayLightSave);
	}


/**
Check that CTzUserData can work with creating a CTzUserData with KUserTzIdMin
id and with a KNullDesC8 name as expected.
*/
void CTzUserDataTest::NullTestL()
	{
	CTzId* minId = CTzId::NewL(KUserTzIdMin);
	test (minId->IsUserTzId());
	delete minId;
	
	CTzId* nullId = CTzId::NewL(KNullDesC8());
	test (nullId->IsUserTzId() == EFalse);
	delete nullId;
	}

void CTzUserDataTest::TestTimeZoneRulesL(TTest aWhatToTest)
	{
	ResetTzUserDataL();
	
	test.Next(_L("Time Zone Rules"));
	RTz tz;
	User::LeaveIfError(tz.Connect());
	CleanupClosePushL(tz);
	tz.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOn);
	iUserData = CTzUserData::NewL(tz);

	test.Next(_L("Create user-defined Time Zone"));

	// Offset 60 There is no DST 
	test.Printf(_L("Offset 60 There is no DST\n"));
	CreateTzAndCheckL(60, 60, tz, aWhatToTest);

	// Offset - 60 There is no DST
	test.Printf(_L("Offset -60 There is no DST\n"));
	CreateTzAndCheckL(-60, -60, tz, aWhatToTest);
	
	// Offset 60 There is DST of 60
	CreateTzAndCheckL(60, 120, tz, aWhatToTest);

	// Offset - 60 There is DST of 60
	test.Printf(_L("Offset - 60 There is DST of 60\n"));
	CreateTzAndCheckL(-60, 0, tz, aWhatToTest);

	// Create user time zone based on London TZ rules 
	test.Printf(_L("Create User Tz Based On London Rule\n"));
	CreateUserTzBasedOnLondonRuleL(tz, aWhatToTest);
	
	// Create user time zone based on Shanghai TZ rules
	test.Printf(_L("Create User Tz Based On Shanghai Rule\n"));
	CreateUserTzBasedOnShanghaiRuleL(tz, aWhatToTest);
	
	delete iUserData;
	iUserData= NULL;
	CleanupStack::PopAndDestroy(&tz);
	ResetTzUserDataL();
	}


void CTzUserDataTest::UpdateCurrentTzL(RTz& aTz,TInt aOldOffSet, TInt aNewOffSet )
	{
	CTzRules*  newrules = CreateUserDefinedTzLC(aOldOffSet, aNewOffSet);
	CTzUserNames* newnames = CreateUserDefinedTzNamesLC();
	CTzId* currentTzid = aTz.GetTimeZoneIdL();
	CleanupStack::PushL(currentTzid);
	iUserData->UpdateL(*currentTzid,*newrules, *newnames);
	CleanupStack::PopAndDestroy(currentTzid);
	CTzRules* currentRules = aTz.GetTimeZoneRulesL(0, 9999,ETzUtcTimeReference );
	CleanupStack::PushL(currentRules);
	test(newrules->IsEqualTo(*currentRules));
	CleanupStack::PopAndDestroy(3, newrules);
	}


void CTzUserDataTest::DeleteCurrentTzL(RTz& aTz)
	{
	CTzId* currentTzid = aTz.GetTimeZoneIdL();
	CleanupStack::PushL(currentTzid);
	iUserData->DeleteL(*currentTzid);
	CleanupStack::PopAndDestroy(currentTzid);
	CTzId* tzGot = aTz.GetTimeZoneIdL();
	test(KTzLondon()==tzGot->TimeZoneNameID());
	delete tzGot;
	}


/**
User-defined time zone is current time zone.

@SYMTestCaseID	PIM-APPSERV-TZS-RC-0001  

@SYMTestCaseDesc
	The purpose of this test is to verify that the current time zone can be set
	to a user-defined time zone.

@SYMTestActions 
	1.	Create a user-defined time zone and store the returned time zone
		identifier.
	2.	Set the current time zone using the time zone identifier stored in
		action 1.
	3.	Get the time zone identifier for the current time zone.

@SYMTestExpectedResults
	The time zone identifier obtained in action 1 is the same as the time zone
	identifier obtained in action 3.

@SYMTestType
	CT

@SYMTestPriority
	1
*/


void CTzUserDataTest::CreateUserTzAndSetCurrentTimeToL(RTz& aTz, TInt aStandardOffSet, TInt aDSTOffset)
	{
	test.Next(_L("@SYMTestCaseID PIM-APPSERV-TZS-RC-0001 Test user-defined time zone is current time zone"));
	CTzId* usertzid = CreateUserDefinedTzL(aStandardOffSet, aDSTOffset, ETrue);
	CleanupStack::PushL(usertzid);
	aTz.SetTimeZoneL(*usertzid);
	CTzId* tzGot = aTz.GetTimeZoneIdL();
	test (*tzGot == *usertzid);
	delete tzGot;
	CleanupStack::PopAndDestroy(usertzid);
	}


/**
Update user-defined time zone whose rules are being cached by TZ Server in the
current time zone rules cache.

@SYMTestCaseID	PIM-APPSERV-TZS-RC-0002 

@SYMTestCaseDesc
	The purpose of this test is to verify that expected behaviour when a time
	zone rule for a user-defined time zone, which is also the current time zone,
	is updated.

@SYMTestActions 
	1.	Create a user-defined time zone.
	2.	Set the current time zone to the user-defined time zone created in
		action 1.
	3.	Update the first time zone rule for the user-defined zone created in
		action 1.
	4.	Get the first time zone rule for the current time zone.

@SYMTestExpectedResults
	The rule used to update the user-defined time zone in action 2 is the same
	as the rule obtained in action 4, implying that the current time zone rules
	cache in the TZ Server has been updated.

@SYMTestType
	CT

@SYMTestPriority
	1
*/


void CTzUserDataTest::UpdateCurrentUserTzL(RTz& aTz)
	{
	test.Next(_L("@SYMTestCaseID PIM-APPSERV-TZS-RC-0002 Update Current TZ rules - Server Response to Changes"));
	TInt standardOffset = 60;
	TInt dstOffset = 60;
	CreateUserTzAndSetCurrentTimeToL(aTz,standardOffset,dstOffset); 
	standardOffset = 120;
	dstOffset = 120;
	UpdateCurrentTzL(aTz,standardOffset, dstOffset);
	}


/**
Delete user-defined time zone whose rules are being cached by TZ Server in the
current time zone rules cache.

@SYMTestCaseID	PIM-APPSERV-TZS-RC-0003

@SYMTestCaseDesc
	The purpose of this test is to verify the expected behaviour when a user-
	defined time zone, which is also the current time zone, is deleted.

@SYMTestActions 
	1.	Set the current time zone to the default time zone for the system TZ
		database.
	2.	Get the time zone identifier for the current time zone.
	3.	Create a user-defined time zone.
	4.	Set the current time zone to the user-defined time zone created in
		action 3.
	5.	Delete the user-defined time zone created in action 3.
	6.	Get the time zone identifier for the current time zone.

@SYMTestExpectedResults
	The time zone identifier obtained in action 2 is the same as the time zone
	identifier obtained in action 6, implying that the current time zone has
	reverted to the default time zone for the system TZ database.

@SYMTestType
	CT

@SYMTestPriority
	1
*/
void CTzUserDataTest::TestDeleteCurrentTimeZoneL(RTz& aTz)
	{
	DeleteCurrentUserTzL(aTz, EDeleteCurrentUserRule);
	}


void CTzUserDataTest::DeleteCurrentUserTzL(RTz& aTz, TTest aWhatToTest)
	{
	test.Next(_L("Delete Current TZ rules - Server Response to Changes"));

	// Set system to default time zone
	ResetTimeZoneToLondonL();
	CTzId* defaulteTzid = aTz.GetTimeZoneIdL();
	test(KTzLondon()==defaulteTzid->TimeZoneNameID());
	delete defaulteTzid;
	
	// Set system to a newly created user time zone
	const TInt KStandardOffset = 60;
	const TInt KDSTOffset = 120;
	CreateUserTzAndSetCurrentTimeToL(aTz,KStandardOffset,KDSTOffset); 
	
	//Delete the user time zone
	DeleteCurrentTzL(aTz);
	
	if(aWhatToTest == EPersistence)
		{
		aTz.Close();	
		User::After(1000000);
		User::LeaveIfError(aTz.Connect());
		}
	CTzId* tzGot = aTz.GetTimeZoneIdL();
	test(KTzLondon()==tzGot->TimeZoneNameID());
	delete tzGot;
	}


/**
Current time zone persistence.

@SYMTestCaseID	PIM-APPSERV-TZS-RC-0006

@SYMTestCaseDesc
	The purpose of this test is to ensure the home time zone identifier in the
	Central Repository is not defined when it is deleted.

@SYMTestActions 
	1.	Set the current time zone to the default time zone for the system TZ
		database.
	2.	Get the time zone identifier for the current time zone.
	3.	Create a user-defined time zone.
	4.	Set the current time zone to the user-defined time zone created in
		action 3.
	5.	Delete the user-defined time zone created in action 3.
	6.	Get the time zone identifier for the current time zone.
	7.	Close session with TZ Server so that TZ Server process is terminated.
	8.	Get the time zone identifier for the current time zone.
	9.	The expected result is same as 8.

@SYMTestExpectedResults
	The time zone identifier obtained in action 2 is the same as the time zone
	identifier obtained in action 8, implying that the current time zone has
	been established by the TZ Server when the server is started using the
	current time zone value persisted to the Central Repository.

@SYMTestType
	CT

@SYMTestPriority
	1
*/
void CTzUserDataTest::TestCurrentTimeZonePersistenceL(RTz& aTz)
	{
	DeleteCurrentUserTzL(aTz, EPersistence);
	}


/**
Update user-defined time zone whose rules are being cached by TZ Server in the
foreign time zone rules cache.

@SYMTestCaseID	PIM-APPSERV-TZS-RC-0004 

@SYMTestCaseDesc
	The purpose of this test is to verify that time conversion to/from a foreign
	(i.e. not current) time zone behaves correctly when the time zone rules
	cached by the TZ Server for the foreign time zone are time zone rules from a
	user-defined time zone that is subsequently updated.

@SYMTestActions 
	1.	Create a user-defined time zone.
	2.	Convert a given time from UTC to local time using the user-defined time
		zone. 
	3.	Update the user-defined time zone created in action 1.
	4.	Convert a given time from UTC to local time using an existing time zone. 

@SYMTestExpectedResults
	The local time is correctly converted, implying that the foreign time zone
	rules cache in the TZ Server is invalidated and recreated.

@SYMTestType
	CT

@SYMTestPriority
	1
*/
void CTzUserDataTest::TestUpdateCachedUserTimeZoneL(RTz& aRTz)
	{
	CachedUserTimeZoneL(aRTz, EUpdateCachedTz);
	}


/**
Delete user-defined time zone whose rules are being cached by TZ Server in the
foreign time zone rules cache.

@SYMTestCaseID	PIM-APPSERV-TZS-RC-0005

@SYMTestCaseDesc
	The purpose of this test is to verify that time conversion to/from a foreign
	(i.e. not current) time zone behaves correctly when the time zone rules
	cached by the TZ Server for the foreign time zone are time zone rules from a
	user-defined time zone that is subsequently deleted.

@SYMTestActions 
	1.	Create a user-defined time zone.
	2.	Convert a given time from UTC to local time using the user-defined time
		zone.
	3.	Delete the user-defined time zone created in action 1.
	4.	Convert a given time from UTC to local time using an existing time zone.

@SYMTestExpectedResults
	The local time is correctly converted, implying that the foreign time zone
	rules cache in the TZ Server is invalidated and recreated. 

@SYMTestType
	CT

@SYMTestPriority
	1
*/
void CTzUserDataTest::TestDeletedCachedUserTimeZoneL(RTz& aRTz)
	{
	CachedUserTimeZoneL(aRTz, EDeleteCachedTz);
	}


void CTzUserDataTest::CachedUserTimeZoneL(RTz& aRTz, TTest aWhatToTest)
	{
	test.Next(_L("Conver times - Server Response to Changes"));
	TInt offset = 60;
	TTime utctime (TDateTime(2000, EMay, 0, 0, 0, 0, 0));
	
	//Convert a given time from UTC to local time using the user-defined time zone
	TTime localtime = utctime ;
	CreateUserTzAndSetCurrentTimeToL(aRTz, offset, offset);
	aRTz.ConvertToLocalTime(localtime);
	TTimeIntervalMinutes minutes;
	localtime.MinutesFrom(utctime, minutes);
	test (minutes == TTimeIntervalMinutes(offset));
	
	localtime = utctime;

	//Convert a given time from UTC to local time when the corrent tz rule is updated 
	if(aWhatToTest==EUpdateCachedTz)
		{
		offset = 120;
		UpdateCurrentTzL(aRTz, offset, offset);
		}
	else if(aWhatToTest==EDeleteCachedTz)
		{
		//Delete the current tz rule which result in system time reverting to the default time zone. 
		DeleteCurrentTzL(aRTz);
		//Convert a given time from UTC to local time using an existing time zone which is the defaut time zone.
		CTzRules* currentRules = aRTz.GetTimeZoneRulesL(0, 9999,ETzUtcTimeReference );
		CleanupStack::PushL(currentRules);
		offset = currentRules->GetOffsetL(utctime, ETzUtcTimeReference);
		CleanupStack::PopAndDestroy(currentRules);
		}
	
	aRTz.ConvertToLocalTime(localtime);
	localtime.MinutesFrom(utctime, minutes);
	test (minutes == TTimeIntervalMinutes(offset));
	}


/**
The purpose of the following test cases is to verify the behaviour of the TZ
Server component’s response to changes in user-defined time zones, specifically
the rules associated with user-defined time zones.

The TZ Server maintains a cache of certain time zone rules data and persists the
time zone identifier for the current time zone.

If either this cache or the persisted setting relates to user-defined time zones
and those time zones are modified then the TZ Server must respond to the change
appropriately.
*/

// @SYMTestCaseID PIM-APPSERV-TZS-RC-0003-TO-0006

void CTzUserDataTest::TestServerResponseToChangesL()
	{
	ResetTzUserDataL();

	test.Next(_L("@SYMTestCaseID PIM-APPSERV-TZS-RC-0003-TO-0006 Server Response To Changes"));

	RTz tz;
	User::LeaveIfError(tz.Connect());
	CleanupClosePushL(tz);

	iUserData = CTzUserData::NewL(tz);

	const TInt KStandardOffset = 60;
	const TInt KDSTOffset = 60;
	CreateUserTzAndSetCurrentTimeToL(tz,KStandardOffset,KDSTOffset); 

	UpdateCurrentUserTzL(tz);

	TestUpdateCachedUserTimeZoneL(tz);
	TestDeletedCachedUserTimeZoneL(tz);
	TestDeleteCurrentTimeZoneL(tz);
	TestCurrentTimeZonePersistenceL(tz);
	
	delete iUserData;
	iUserData= NULL;

	CleanupStack::PopAndDestroy(&tz);
	}

	
/**
The purpose of the follwing test case is to verify the behaviour of 
reuse of Tz Ids in case all the 4095 ids have been used.
*/	
void CTzUserDataTest::TestTzIdReusabilityL()
	{
	ResetTzUserDataL();
	RTz tz;
	CleanupClosePushL(tz);
	User::LeaveIfError(tz.Connect());
	iUserData = CTzUserData::NewL(tz);
	const TInt maxTzIds = 4095;
	for(TInt i=0; i<= maxTzIds; i++)
		{
		CTzId* tzid = NULL;
		TRAPD(err, tzid = CreateUserDefinedTzL(0,0, EFalse));
		if(i%KPrintFreq == 0 || i == maxTzIds)
			{
			test.Printf(_L("Added %d rules to the database\n"), i);
			}
			
		if(err != KErrNone)
			{
			test.Printf(_L("Error while creating rules. Error is %d\n"), err);	
			}
		delete tzid;
		}
	const TUint KFirstReusedTzId = 16400;
	CTzId* tzId = CTzId::NewL(KFirstReusedTzId);
	CleanupStack::PushL(tzId);
	iUserData->DeleteL(*tzId);
	CleanupStack::PopAndDestroy(tzId);
	
	const TUint KSecondReusedTzId = 16500;
	CTzId* tzId1 = CTzId::NewL(KSecondReusedTzId);
	CleanupStack::PushL(tzId1);
	iUserData->DeleteL(*tzId1);
	CleanupStack::PopAndDestroy(tzId1);	
		
	CTzId* reusedId = CreateUserDefinedTzL(0, 0, EFalse);
	test(reusedId->TimeZoneNumericID() == KFirstReusedTzId);
	test.Printf(_L("The first re-used id is %d\n"), reusedId->TimeZoneNumericID());
	delete reusedId;
	
	CTzId* secondReusedId = CreateUserDefinedTzL(0, 0, EFalse);
	test(secondReusedId->TimeZoneNumericID() == KSecondReusedTzId);
	test.Printf(_L("The second re-used id is %d\n"), secondReusedId->TimeZoneNumericID());
	delete secondReusedId;
	
	TRAPD(err, CreateUserDefinedTzL(0, 0, EFalse));
	test(err == KErrOverflow);
	
	delete iUserData;
	iUserData = NULL;
	CleanupStack::PopAndDestroy(&tz);
	}
	
static void DoTestsL()
	{
	User::LeaveIfError(Serv.Connect());
		
	test.Title();

	ResetTimeZoneToLondonL();

	CTzUserDataTest* userDataTest = CTzUserDataTest::NewLC();

	test.Start(_L("Test Basic User Data"));
	userDataTest->TestBasicUserDataL();

	test.Next(_L("Test Out Of Memory Behaviour (Client)"));
	userDataTest->TestOutOfMemoryBehaviourL(CTzUserDataTest::EClientOOM);

	test.Next(_L("Test Out Of Memory Behaviour (Server)"));
	userDataTest->TestOutOfMemoryBehaviourL(CTzUserDataTest::EServerOOM );
	
	test.Next(_L("@SYMTestCaseID PIM-APPSERV-TZS-TZR-0001 Test Time Conversion"));
	userDataTest->TestTimeConversionL();

	test.Next(_L("@SYMTestCaseID PIM-APPSERV-TZS-TZR-0002 Test Daylight Save"));
	userDataTest->TestDaylightSaveL();

	test.Next(_L("Test Boundary Test"));
	userDataTest->BoundaryTestL();

	test.Next(_L("Test Server Response To Changes"));
	userDataTest->TestServerResponseToChangesL();
	
	// Delete the user-defined TZ database before the next test case.
	Serv.CloseTzSession();
	Serv.DeleteFileL(KTzUserDataDb);

	test.Next(_L("Test Reusability Of TZ IDs"));
	userDataTest->TestTzIdReusabilityL();

	// Delete the user defined TZ database.
	Serv.CloseTzSession();
	Serv.DeleteFileL(KTzUserDataDb);	

	test.Next(_L("Null Test"));
	userDataTest->NullTestL();

	ResetTimeZoneToLondonL();

	CleanupStack::PopAndDestroy(userDataTest);

	test.End();

	Serv.Close();
	}


TInt E32Main()
    {
	__UHEAP_MARK;

	CTrapCleanup* trapCleanup = CTrapCleanup::New();
	if(!trapCleanup)
		{
		return KErrNoMemory;
		}

	CActiveScheduler* scheduler = new CActiveScheduler;
	if(!scheduler)
		{
		return KErrNoMemory;
		}
	CActiveScheduler::Install(scheduler);	
	
	TRAPD(ret, DoTestsL());
	test.Printf(_L("Trapped return value from DoTestsL(): %d\n"), ret);
	test(ret == KErrNone);

	test.Close();
		
	delete scheduler;
	delete trapCleanup;	

	__UHEAP_MARKEND;

	return (KErrNone);
    }
