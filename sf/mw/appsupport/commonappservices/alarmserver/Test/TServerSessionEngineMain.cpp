// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

// System includes
#include <e32base.h>
#include <e32test.h>
#include <f32file.h>

// User includes
#include "TServerSessionEngine.h"

// Literal constants
_LIT(KTest,		"server session engine test");

// Constants
const TAlarmCategory KASCliCategoryTest		= { 0x101F5031 };

// Globals
static RTest TheTest(KTest);


void Test1L(CTestServerSessionEngine* aSessionEngine)
	{
	TheTest.Next(_L("Test 1"));
	//******************************************************************************
	// Test1 tests
	//
	// AlarmAddL
	// AlarmDeleteL
	// AlarmCountByCategory
	// AlarmCountByState
	// AlarmIdListByStateLC
	//******************************************************************************
	aSessionEngine->ClearAlarmStoreL();

	RArray<TAlarmId> alarmIds;
	CleanupClosePushL(alarmIds);

	//add 10 alarms (category KASCliCategoryClock)
	for(TInt ii=1; ii<=10; ii++)
		{
		TInt secondsFromNow = (ii)*2;
		TAlarmId id = aSessionEngine->AddAlarmWithSpecificTimeL(secondsFromNow);
		alarmIds.AppendL(id);
		
		TInt numberOfAlarms = aSessionEngine->Engine().AlarmCountByCategory(KASCliCategoryClock);
		TheTest(numberOfAlarms == ii, __LINE__);
		}

	//check states of alarms
	TInt alarmCount=0;
	//states should be:
	// 10 x EAlarmStateQueued
	alarmCount = aSessionEngine->Engine().AlarmCountByState(EAlarmStateQueued);
	TheTest(alarmCount == 10, __LINE__);

	//wait for alarm to expire
	aSessionEngine->AwaitNotificationL();
	CActiveScheduler::Start();
	//states should be:
	// 1 x EAlarmStateNotifying
	// 9 x EAlarmStateQueued
	alarmCount = aSessionEngine->Engine().AlarmCountByState(EAlarmStateNotifying);
	TheTest(alarmCount == 1, __LINE__);
	alarmCount = aSessionEngine->Engine().AlarmCountByState(EAlarmStateQueued);
	TheTest(alarmCount == 9, __LINE__);

	//wait for another alarm to expire
	CActiveScheduler::Start();
	//states should be:
	// 1 x EAlarmStateNotifying
	// 1 x EAlarmStateWaitingToNotify
	// 8 x EAlarmStateQueued
	alarmCount = aSessionEngine->Engine().AlarmCountByState(EAlarmStateNotifying);
	TheTest(alarmCount == 1, __LINE__);
	alarmCount = aSessionEngine->Engine().AlarmCountByState(EAlarmStateWaitingToNotify);
	TheTest(alarmCount == 1, __LINE__);
	alarmCount = aSessionEngine->Engine().AlarmCountByState(EAlarmStateQueued);
	TheTest(alarmCount == 8, __LINE__);

	//last 8 alarms in our array should correspond to array of alarms with state queued
	RArray<TAlarmId>* alarmsWithStateQueued = aSessionEngine->Engine().AlarmIdListByStateLC(EAlarmStateQueued);
		for(TInt jj=0; jj<8; jj++)
		{
		TheTest(alarmIds[jj+2] == (*alarmsWithStateQueued)[jj], __LINE__);
		}
	alarmsWithStateQueued->Close();
	CleanupStack::PopAndDestroy(alarmsWithStateQueued);

	//delete all alarms individually
	for(TInt count=alarmIds.Count(); count>0; count--)
		{
		aSessionEngine->Engine().AlarmDeleteL(alarmIds[count-1]);
		}
	CleanupStack::PopAndDestroy();//alarmIds
	}


void Test2L(CTestServerSessionEngine* aSessionEngine)
	{
	TheTest.Next(_L("Test 2"));
	//******************************************************************************
	// Test2 tests
	//
	// AlarmDetailsL
	// AlarmCategoryL
	// SetAlarmStatusByCategoryL
	// AlarmCountByCategory
	// DeleteAllAlarmsByCategoryL
	// AlarmCategoryListLC
	// AlarmIdListByCategoryLC
	// SetAlarmStatusL
	// AlarmStatus
	//******************************************************************************
	aSessionEngine->ClearAlarmStoreL();

	//add 5 alarms with category KASCliCategoryClock
	for(TInt ii=0; ii<5; ii++)
		{
		aSessionEngine->AddAlarmWithSpecificCategoryL(KASCliCategoryClock);
		}

	//get list of categories - should contain only KASCliCategoryClock
	RArray<TAlarmCategory>* alarmCategories = aSessionEngine->Engine().AlarmCategoryListLC();
		
	TheTest(alarmCategories->Count() == 1 && (*alarmCategories)[0] == KASCliCategoryClock, __LINE__);
	alarmCategories->Close();
	CleanupStack::PopAndDestroy(alarmCategories);
	
	//add 5 alarms with category KASCliCategoryTest
	for(TInt jj=0; jj<5; jj++)
		{
		aSessionEngine->AddAlarmWithSpecificCategoryL(KASCliCategoryTest);
		}
	
	RArray<TAlarmCategory>* alarmCategories2 = aSessionEngine->Engine().AlarmCategoryListLC();
	TheTest(alarmCategories2->Count() == 2, __LINE__);
	alarmCategories2->Close();
	CleanupStack::PopAndDestroy(alarmCategories2);

	//add another 10 alarms with category KASCliCategoryClock
	for(TInt kk=0; kk<10; kk++)
		{
		aSessionEngine->AddAlarmWithSpecificCategoryL(KASCliCategoryClock);
		}

	//get alarms with category KASCliCategoryClock
	RArray<TAlarmId>* alarms = aSessionEngine->Engine().AlarmIdListByCategoryLC(KASCliCategoryClock);
	TInt count = alarms->Count();
	TheTest(count == 15, __LINE__);
	//check each alarm has category KASCliCategoryClock
	for(TInt mm=0; mm<15; mm++)
		{
		//directly
		TheTest(aSessionEngine->Engine().AlarmCategoryL((*alarms)[mm]) == KASCliCategoryClock, __LINE__);
		//indirectly
		TASSrvAlarm alarm(aSessionEngine->ServerWideData());
		aSessionEngine->Engine().AlarmDetailsL((*alarms)[mm], alarm);
		TheTest(alarm.Category() == KASCliCategoryClock, __LINE__);
		}
	alarms->Close();
	CleanupStack::PopAndDestroy(alarms);

	//should now have 15 alarms with category KASCliCategoryClock and 5 with category KASCliCategoryTest
	TheTest(aSessionEngine->Engine().AlarmCountByCategory(KASCliCategoryClock)==15 &&
		aSessionEngine->Engine().AlarmCountByCategory(KASCliCategoryTest)==5, __LINE__);
	//disable all KASCliCategoryClock alarms
	aSessionEngine->Engine().SetAlarmStatusByCategoryL(KASCliCategoryClock, EAlarmStatusDisabled);
	TheTest(aSessionEngine->Engine().NumberOfActiveAlarmsInQueue()==5, __LINE__);
	//try to delete all KASCliCategoryTest orphaned alarms (should be none)
	aSessionEngine->Engine().DeleteAllAlarmsByCategoryL(KASCliCategoryTest, ETrue);
	TheTest(aSessionEngine->Engine().NumberOfActiveAlarmsInQueue()==5, __LINE__);
	//delete all KASCliCategoryTest alarms
	aSessionEngine->Engine().DeleteAllAlarmsByCategoryL(KASCliCategoryTest, EFalse);
	TheTest(aSessionEngine->Engine().NumberOfActiveAlarmsInQueue()==0, __LINE__);
	//re-enable the KASCliCategoryClock alarms
	aSessionEngine->Engine().SetAlarmStatusByCategoryL(KASCliCategoryClock, EAlarmStatusEnabled);
	TheTest(aSessionEngine->Engine().NumberOfActiveAlarmsInQueue()==15, __LINE__);
	//create a new enabled alarm
	TAlarmId id = aSessionEngine->AddAlarmWithSpecificCategoryL(KASCliCategoryClock);
	TheTest(aSessionEngine->Engine().NumberOfActiveAlarmsInQueue()==16, __LINE__);
	//check status
	TheTest(aSessionEngine->Engine().AlarmStatusL(id) == EAlarmStatusEnabled, __LINE__);
	//disable it
	aSessionEngine->Engine().SetAlarmStatusL(id, EAlarmStatusDisabled);
	TheTest(aSessionEngine->Engine().NumberOfActiveAlarmsInQueue()==15, __LINE__);
	TheTest(aSessionEngine->Engine().AlarmStatusL(id) == EAlarmStatusDisabled, __LINE__);
	
	//delete all remaining alarms
	aSessionEngine->Engine().DeleteAllAlarmsByCategoryL(KASCliCategoryClock, EFalse);
	TheTest(aSessionEngine->Engine().NumberOfActiveAlarmsInQueue()==0, __LINE__);
	}


static void doTestsL()
	{
	// this is a WINS test only, because it tests the functionality of the alarm server
	// engine directly.  The tests are repeated in TClientSession which exercises the same
	// functionality through a client session
	
#ifdef __WINS__
	CTestServerSessionEngine* sessionEngine = CTestServerSessionEngine::NewLC();
	//
	TheTest.Next(_L("Start tests"));
	Test1L(sessionEngine);
	Test2L(sessionEngine);
	
	CleanupStack::PopAndDestroy(sessionEngine);
#else
	TheTest.Printf(_L("\nThis is a WINS test only\n"));
#endif
	}

/**
@SYMTestCaseID PIM-TSERVERSESSIONENGINEMAIN-0001
*/	
TInt E32Main()
	{
	__UHEAP_MARK;
	//
	TheTest.Title();
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if	(!cleanup)
		return KErrNoMemory;
	//
	TheTest.Start(_L("@SYMTestCaseID PIM-TSERVERSESSIONENGINEMAIN-0001"));
	//
	TRAPD(err, doTestsL());
	TheTest(err == KErrNone, __LINE__);

	delete cleanup;
	TheTest.End();
	TheTest.Close();

	__UHEAP_MARKEND;
	return KErrNone;
	}
