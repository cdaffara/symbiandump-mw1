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

#include "ASTstAlarmTest.h"
#include "ASSrvDefs.h"
#include <tz.h>
#include <vtzrules.h>
#include <e32math.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <tzusernames.h>
#include <tzuserdefineddata.h>
#endif

//
class CUserDefinedFloatingAlarmTest:public CBase
	{
public:
	static CUserDefinedFloatingAlarmTest* NewLC();
	~CUserDefinedFloatingAlarmTest();	
	void TestFloatingAlarmWithUserDefinedRulesL();
	
private:
	CUserDefinedFloatingAlarmTest();
	void DeleteAllAlarmsL();
	void TestTimesEqual(const TTime& aTime1, const TTime& aTime2);
	void ResetDataL();
	CTzRules* CreateRulesL(TInt aOffset);
	CTzUserNames* CreateNamesL();
	void CreateString(TDes& aString);

private:
	TInt64 iSeed;	
	};

CUserDefinedFloatingAlarmTest::CUserDefinedFloatingAlarmTest():iSeed(100)
	{	
	}
	
CUserDefinedFloatingAlarmTest::~CUserDefinedFloatingAlarmTest()
	{
	TRAP_IGNORE(ResetDataL());	
	}

CUserDefinedFloatingAlarmTest* CUserDefinedFloatingAlarmTest::NewLC()
	{
	CUserDefinedFloatingAlarmTest* self = new(ELeave) CUserDefinedFloatingAlarmTest();
	CleanupStack::PushL(self);
	return self;
	}

void CUserDefinedFloatingAlarmTest::TestTimesEqual(const TTime& aTime1, const TTime& aTime2)
	{
	// determine the difference, in minutes
	TTimeIntervalMinutes	timeDiffMinutes;
	const TInt error = aTime2.MinutesFrom(aTime1, timeDiffMinutes);
	TheAlarmTest(error == KErrNone);
	// check the times are within 1 minute
	const TTimeIntervalMinutes	maxDiff(1);
	const TTimeIntervalMinutes	minDiff(-1);
	TheAlarmTest(((timeDiffMinutes <= maxDiff) && (timeDiffMinutes >= minDiff)));
	}

void CUserDefinedFloatingAlarmTest::DeleteAllAlarmsL()
	{
	// Delete all alarms
	RArray<TAlarmCategory> categories;
	TheAlarmTest.Session().GetAvailableCategoryListL(categories);
	TInt count = categories.Count();
	for(TInt i=0; i<count; i++)
		{
		const TAlarmCategory category = categories[i];
		User::LeaveIfError(TheAlarmTest.Session().AlarmDeleteAllByCategory(category, EFalse));
		}
	categories.Close();
	}

LOCAL_D void ResetAndDestroyTzIdArray(TAny* aPtr)
	{
	RPointerArray<CTzId>* ids = static_cast<RPointerArray<CTzId>*>(aPtr);
	if (ids)
		{
		ids->ResetAndDestroy();
		}
	}
	
void CUserDefinedFloatingAlarmTest::ResetDataL()
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

CTzRules* CUserDefinedFloatingAlarmTest::CreateRulesL(TInt aOffset)
	{
	//Create time zone rules
	TTzRule rule1(2007, 2009, aOffset, 0, EOctober,  ETzFixedDate, 0,  0, ETzWallTimeReference, 120);
	TTzRule rule2(2007,2009,0,aOffset,EMarch,ETzFixedDate,0,0,ETzWallTimeReference,0);

	CTzRules* rules = CTzRules::NewL(0, 9999);
	CleanupStack::PushL(rules);
	rules->AddRuleL(rule1);
	rules->AddRuleL(rule2);
	
	return rules;	
	}

void CUserDefinedFloatingAlarmTest::CreateString(TDes& aString)
	{
	for (TInt i(0) ; i < 10 ; ++i)
		{
		// Add random upper and lower case letters
		aString.Append( 'A' + (Math::Rand(iSeed) % 26) );
		}
	}
	
CTzUserNames* CUserDefinedFloatingAlarmTest::CreateNamesL()
	{	
	TBuf<10> stdName;
	CreateString(stdName);
	TBuf<10> shortStdName;
	CreateString(shortStdName);
	TBuf<10> dstName;
	CreateString(dstName);
	TBuf<10> shortDstName;
	CreateString(shortDstName);
	TBuf<10> cityName;
	CreateString(cityName);
	TBuf<10> regionName;
	CreateString(regionName);

	return CTzUserNames::NewLC(stdName, shortStdName, dstName, shortDstName, cityName, regionName);
	}

/** Floating local time alarm using user defined time zone

@SYMTestCaseID				PIM-APPSRV-ALS-TZC-0001
@SYMTestCaseDesc			The purpose of this test is to verify that
							the expiry time of a floating local time 
							alarm is correct when the time zone rules
							data for the current time zone is changed.
@SYMTestActions				1.	Create a user defined time zone with a UTC
								offset of 2 hours.
							2.	Set the current time zone to the user defined
								time zone.
							3.	Create a floating local time alarm using the
								Alarm Server that is due to expire 2 hours +
								t minutes from the current local time.
							4.	Modify the user defined time zone to have a UTC
								offset of 0 hours.
@SYMTestExpectedResults		The alarm expires no longer than t minutes from the
							current local time.
@SYMTestType 				CIT
@SYMTestPriority  			1

*/
void CUserDefinedFloatingAlarmTest::TestFloatingAlarmWithUserDefinedRulesL()
	{
	//Clear the alarmserver's queue
	DeleteAllAlarmsL();
	//Clear the user-defined database
	ResetDataL();
	RTz tz;
	CleanupClosePushL(tz);
	User::LeaveIfError(tz.Connect());
	
	CTzUserData* userData = CTzUserData::NewL(tz);
	CleanupStack::PushL(userData);
	TheAlarmTest.Test().Printf(_L("@SYMTestCaseID PIM-APPSRV-ALS-TZC-0001  Add a user-defined time zone rule to the database\n"));
		
	//aOffset = 60
	CTzRules* rules = CreateRulesL(60);
		
	CTzUserNames* names = CreateNamesL();
		
	CTzId* id = NULL;
	id = userData->CreateL(*rules, *names); //Create user-defined rules
		
	CleanupStack::PopAndDestroy(2, rules);
	
	//Set the current time zone to the user-defined zone
	tz.SetTimeZoneL(*id);
	tz.SetHomeTime(TDateTime(2008, EJuly, 11, 7, 0, 0, 0));
	
	TTime now;
	now.HomeTime();
	TDateTime dt = now.DateTime();
	TheAlarmTest.Test().Printf(_L("The current home time is hours: %d, minutes: %d, seconds:%d \r\n"),
	dt.Hour(), dt.Minute(), dt.Second());
	
	TASShdAlarm alarm;
	alarm.Category() = KASCliCategoryClock;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	
	TDateTime date;
	//Set the time for the floating alarm
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalMinutes(61);	
	//Add the floating alarm to the server's queue
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
	const TAlarmId alarmId = alarm.Id();
	TheAlarmTest(alarmId != KNullAlarmId, __LINE__);
	
	date=alarm.NextDueTime().DateTime();
	TheAlarmTest.Test().Printf(_L("Floating alarm with id %d has due time hours: %d,\
	minutes: %d, seconds: %d.\r\n"), alarmId,date.Hour(), date.Minute(), date.Second());
		
	CTzRules* newRule = CreateRulesL(120);
	
	CTzUserNames* newNames = CreateNamesL();
	
	//Update the rule
	userData->UpdateL(*id, *newRule, *newNames);
	
	now.HomeTime();
	dt = now.DateTime();
	TheAlarmTest.Test().Printf(_L("The new local time after update is hours: %d, minutes: %d, seconds:%d \r\n"),
	dt.Hour(), dt.Minute(), dt.Second());
	
	date = alarm.NextDueTime().DateTime();
	TheAlarmTest.Test().Printf(_L("Floating alarm has a new due time : hours: %d, minutes:\
	%d, seconds: %d.\r\n"),date.Hour(), date.Minute(), date.Second());
		
	now.HomeTime();
	TestTimesEqual(alarm.NextDueTime(), now);	
		
	CleanupStack::PopAndDestroy(2, newRule);
	delete id;
	CleanupStack::PopAndDestroy(userData);
	CleanupStack::PopAndDestroy(&tz);
	// Clear all the alarms from the queue
	DeleteAllAlarmsL();	
	ResetDataL();
	}
	
//

static void doTestsL()
	{
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	TheAlarmTest.TestStartServers();
	TheAlarmTest(TheAlarmTest.Session().Connect() == KErrNone, __LINE__);

	CUserDefinedFloatingAlarmTest* testFloatingAlarm = CUserDefinedFloatingAlarmTest::NewLC();

	testFloatingAlarm->TestFloatingAlarmWithUserDefinedRulesL();
	
	// cleanup
	CleanupStack::PopAndDestroy(testFloatingAlarm);
	CleanupStack::PopAndDestroy(scheduler);
	}

//


GLDEF_C TInt E32Main()
    {
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if	(!cleanup)
		return KErrNoMemory;

	TheAlarmTest.Test().Title();
	TheAlarmTest.Test().Start(_L("Alarms with user-defined rules"));

	
	TRAPD(error, doTestsL());

	// did any test leave?
	TheAlarmTest(error == KErrNone, __LINE__);

	TheAlarmTest.Test().End();
	TheAlarmTest.Test().Close();
	ASTstAlarmTest::Close();
	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
    }
