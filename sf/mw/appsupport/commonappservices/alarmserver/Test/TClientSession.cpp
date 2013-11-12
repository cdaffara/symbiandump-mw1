// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

// Globals
static TRequestStatus TheStatus;

// Constants
const TAlarmCategory KASCliCategoryTest		= { 0x101F5031 };


TAlarmId AddAlarm(TInt aMinutesFromNow, TAlarmCategory aAlarmCategory = KASCliCategoryClock)
	{
	TBuf<200> testTitle;
	testTitle.Format(_L("\nAdding alarm to expire in %d minutes\n"), aMinutesFromNow);
	TheAlarmTest.Test().Printf(testTitle);
	//
	TASShdAlarm alarm;
	alarm.Category() = aAlarmCategory;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalMinutes(aMinutesFromNow);
	//
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
	const TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);
	TAlarmCategory category;
	TheAlarmTest.Session().GetAlarmCategory(id, category);
	TheAlarmTest(category == aAlarmCategory, __LINE__);

	return id;
	}

TAlarmId AddAlarm(const TTime& aTime, TAlarmCategory aAlarmCategory = KASCliCategoryClock)
	{
	TheAlarmTest.Test().Printf(_L("\nAdding alarm\n"));
	//
	TASShdAlarm alarm;
	alarm.Category() = aAlarmCategory;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	alarm.NextDueTime()=aTime;
	//
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
	const TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);

	return id;
	}

TAlarmId AddUtcAlarm(TInt aMinutesFromNow, TAlarmCategory aAlarmCategory = KASCliCategoryClock)
	{
	TBuf<200> testTitle;
	testTitle.Format(_L("\nAdding UTC alarm to expire in %d minutes\n"), aMinutesFromNow);
	TheAlarmTest.Test().Printf(testTitle);
	//
	TASShdAlarm alarm;
	alarm.Category()=aAlarmCategory;
	alarm.RepeatDefinition()=(EAlarmRepeatDefintionRepeatOnce);
	TTime time;
	time.UniversalTime();
	time += TTimeIntervalMinutes(aMinutesFromNow);
	alarm.SetUtcNextDueTime(time);
	//
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
	const TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);
	TAlarmCategory category;
	TheAlarmTest.Session().GetAlarmCategory(id, category);
	TheAlarmTest(category == aAlarmCategory, __LINE__);

	return id;
	}

TAlarmId AddUtcAlarm(const TTime& aTime, TAlarmCategory aAlarmCategory = KASCliCategoryClock)
	{
	TheAlarmTest.Test().Printf(_L("\nAdding UTC alarm\n"));
	//
	TASShdAlarm alarm;
	alarm.Category()=aAlarmCategory;
	alarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatOnce;
	alarm.SetUtcNextDueTime(aTime);
	//
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
	const TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);

	return id;
	}


void WaitForAlarmToExpire()
	{
#if defined(_DEBUG) || defined(__WINSCW__)
	TheAlarmTest.Test().Printf(_L("\nWaiting for alarm to expire...\n"));

	TAlarmId alarmId;
	TRequestStatus status;
	FOREVER
		{
		TheAlarmTest.Session().NotifyChange(status, alarmId);
		User::WaitForRequest(status);
		if	(status.Int() == EAlarmChangeEventTimerExpired)
			break;
		}
#endif
	}


void VerifyStates(TInt aQueued, TInt aNotifying=0, TInt aWaitingToNotify=0, TInt aNotified=0, TInt aSnoozed=0)
	{
#if defined(_DEBUG) || defined(__WINSCW__)
	TInt queued = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
	TInt notifying = TheAlarmTest.Session().AlarmCountByState(EAlarmStateNotifying);
	TInt waitingToNotify = TheAlarmTest.Session().AlarmCountByState(EAlarmStateWaitingToNotify);
	TInt notified = TheAlarmTest.Session().AlarmCountByState(EAlarmStateNotified);
	TInt snoozed = TheAlarmTest.Session().AlarmCountByState(EAlarmStateSnoozed);
	//
	TheAlarmTest(queued == aQueued, __LINE__);
	// When TheAlarmTest.Session().__DbgPreventUserNotify() is set with ETrue (i.e. when
	// on target platform), all notifying alarms immediately become notified.
#if defined(__WINSCW__)
	TheAlarmTest(notifying == aNotifying, __LINE__);
	TheAlarmTest(waitingToNotify == aWaitingToNotify, __LINE__);
	TheAlarmTest(notified == aNotified, __LINE__);
#else   // __WINSCW__
	TheAlarmTest(notifying == 0, __LINE__);
	TheAlarmTest(waitingToNotify == 0, __LINE__);
	TheAlarmTest(notified == aNotified + aWaitingToNotify + aNotifying, __LINE__);
#endif  // __WINSCW__
	TheAlarmTest(snoozed == aSnoozed, __LINE__);
#endif
	}

void AlarmDeleteAllByCategoryIndirectlyL(TAlarmCategory aCategory)
	{
	if (TheAlarmTest.Session().GetAlarmCountForCategory(aCategory) == 0)
		return;

	//get ids of alarms with this category
	RArray<TAlarmId> alarmIds;
	CleanupClosePushL(alarmIds);

	TheAlarmTest.Session().GetAlarmIdListForCategoryL(aCategory, alarmIds);

	//delete alarms
	for (TInt ii = alarmIds.Count() - 1; ii >= 0; ii--)
		{
		TheAlarmTest.Session().AlarmDelete(alarmIds[ii]);
		TheAlarmTest(TheAlarmTest.Session().GetAlarmCountForCategory(aCategory) == ii, __LINE__);
		}

	CleanupStack::PopAndDestroy(&alarmIds); // Close array.
	}

//
//
//
//
//


static void TestCategorySpecificFunctionalityL()
	{
	TheAlarmTest.Test().Next(_L("test category specific functionality"));

	RArray<TAlarmCategory> categoryList;
	CleanupClosePushL(categoryList);

	TheAlarmTest.Session().GetAvailableCategoryListL(categoryList);
	TheAlarmTest(categoryList.Count() == 0, __LINE__);
	categoryList.Reset();

	//add 5 alarms with category KASCliCategoryClock 1,3,5,7,9 minutes from now
	TAlarmId id = KNullAlarmId;
	for (TInt ii=1; ii<=5; ++ii)
		{
		const TAlarmId allocatedId = AddAlarm(2*ii - 1);
		if (id == KNullAlarmId)
			{
			id = allocatedId;
			}
		TheAlarmTest(TheAlarmTest.Session().GetAlarmCountForCategory(KASCliCategoryClock) == ii, __LINE__);
		}
	TheAlarmTest.Session().GetAvailableCategoryListL(categoryList);
	TheAlarmTest(categoryList.Count() == 1 && categoryList[0] == KASCliCategoryClock, __LINE__);
	categoryList.Reset();

	//add 5 alarms with category KASCliCategoryTest 2,4,6,8,10 minutes from now
	for (TInt jj=1; jj<=5; ++jj)
		{
		AddAlarm(2*jj, KASCliCategoryTest);
		TheAlarmTest(TheAlarmTest.Session().GetAlarmCountForCategory(KASCliCategoryTest) == jj, __LINE__);
		}
	TheAlarmTest.Session().GetAvailableCategoryListL(categoryList);
	TheAlarmTest(categoryList.Count() == 2, __LINE__);
	categoryList.Reset();

	VerifyStates(10);
	WaitForAlarmToExpire();
	VerifyStates(9,1);
	WaitForAlarmToExpire();
	VerifyStates(8,1,1);

	//delete all KASCliCategoryTest alarms
	AlarmDeleteAllByCategoryIndirectlyL(KASCliCategoryTest);
	TheAlarmTest.Session().GetAvailableCategoryListL(categoryList);
	TheAlarmTest(categoryList.Count() == 1 && categoryList[0] == KASCliCategoryClock, __LINE__);
	categoryList.Reset();

	//delete all KASCliCategoryClock alarms
	TheAlarmTest.Session().AlarmDeleteAllByCategory(KASCliCategoryClock, EFalse);
	TheAlarmTest.Session().GetAvailableCategoryListL(categoryList);
	TheAlarmTest(categoryList.Count() == 0, __LINE__);

    TheAlarmTest.TestClearStoreL();

	CleanupStack::PopAndDestroy(&categoryList); // Close array.
	}


void TestOverlappingAlarmsL()
	{
	TheAlarmTest.Test().Next(_L("test overlapping alarms"));

	//schedule 3 alarms for the same time
	TTime time;
	time.HomeTime();
	time += TTimeIntervalSeconds(10);
	for(TInt ii=0; ii<3; ii++)
		{
		AddAlarm(time);
		}

	VerifyStates(3);
	WaitForAlarmToExpire();
	User::After(1000000);
	VerifyStates(0,1,2);

	TheAlarmTest.TestClearStoreL();
    TRequestStatus status;
    TAlarmId alarmId;
    TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);

	time.HomeTime();
	time += TTimeIntervalSeconds(10);
	TAlarmId id1 = AddAlarm(time);
	time += TTimeIntervalSeconds(10);
	TAlarmId id2 = AddAlarm(time);

	TASShdAlarm alarm1;
	TASShdAlarm alarm2;

#if defined(_DEBUG)
	WaitForAlarmToExpire();
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id1, alarm1) == KErrNone, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id2, alarm2) == KErrNone, __LINE__);
#if defined(__WINSCW__)
	TheAlarmTest(alarm1.State() == EAlarmStateNotifying, __LINE__);
	TheAlarmTest(alarm2.State() == EAlarmStateQueued, __LINE__);
#else
	TheAlarmTest(alarm1.State() == EAlarmStateNotified, __LINE__);
	TheAlarmTest(alarm2.State() == EAlarmStateQueued, __LINE__);
#endif

	WaitForAlarmToExpire();
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id1, alarm1) == KErrNone, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id2, alarm2) == KErrNone, __LINE__);
#if defined(__WINSCW__)
	TheAlarmTest(alarm1.State() == EAlarmStateNotifying, __LINE__);
	TheAlarmTest(alarm2.State() == EAlarmStateWaitingToNotify, __LINE__);
#else
	TheAlarmTest(alarm1.State() == EAlarmStateNotified, __LINE__);
	TheAlarmTest(alarm2.State() == EAlarmStateNotified, __LINE__);
#endif
#endif

	TheAlarmTest.TestClearStoreL();
	}


void  TestAlarmDetails1L()
	{
	TheAlarmTest.Test().Next(_L("test alarm details - 1"));

	TheAlarmTest.TestClearStoreL();

	TInt id = AddAlarm(1);

	TAlarmStatus status;
	TheAlarmTest(TheAlarmTest.Session().GetAlarmStatus(id, status) == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmStatusEnabled, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().SetAlarmStatus(id, EAlarmStatusDisabled) == KErrNone, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().GetAlarmStatus(id, status) == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmStatusDisabled, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().SetAlarmStatusByCategory(KASCliCategoryClock, EAlarmStatusEnabled) == KErrNone, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().GetAlarmStatus(id, status) == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmStatusEnabled, __LINE__);

	WaitForAlarmToExpire();

	TheAlarmTest.Session().AlarmDelete(id);
	}
	
void TestAlarmDetails2L()
	{
	TheAlarmTest.Test().Next(_L("test alarm details - 2"));
	//******************************************************************************
	// tests
	//
	// AlarmAddL
	// AlarmDeleteL
	// AlarmCountByCategory
	// AlarmCountByState
	// AlarmIdListByStateLC
	//******************************************************************************
	TheAlarmTest.TestClearStoreL();

	RArray<TAlarmId> alarmIds;
	CleanupClosePushL(alarmIds);

	//add 10 alarms (category KASCliCategoryClock)
	for(TInt ii=1; ii<=10; ii++)
		{
		TInt minutesFromNow = (ii);
		TAlarmId id = AddAlarm(minutesFromNow);
		alarmIds.AppendL(id);

		TInt numberOfAlarms = TheAlarmTest.Session().GetAlarmCountForCategory(KASCliCategoryClock);
		TheAlarmTest(numberOfAlarms == ii, __LINE__);
		}

	//check states of alarms
	TInt alarmCount=0;
	//states should be:
	// 10 x EAlarmStateQueued
	alarmCount = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
	TheAlarmTest(alarmCount == 10, __LINE__);

#if defined(_DEBUG)
#if defined(__WINSCW__)
	//wait for alarm to expire
	WaitForAlarmToExpire();
	//states should be:
	// 1 x EAlarmStateNotifying
	// 9 x EAlarmStateQueued
	alarmCount = TheAlarmTest.Session().AlarmCountByState(EAlarmStateNotifying);
	TheAlarmTest(alarmCount == 1, __LINE__);
	alarmCount = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
	TheAlarmTest(alarmCount == 9, __LINE__);

	//wait for another alarm to expire
	WaitForAlarmToExpire();
	//states should be:
	// 1 x EAlarmStateNotifying
	// 1 x EAlarmStateWaitingToNotify
	// 8 x EAlarmStateQueued
	alarmCount = TheAlarmTest.Session().AlarmCountByState(EAlarmStateNotifying);
	TheAlarmTest(alarmCount == 1, __LINE__);
	alarmCount = TheAlarmTest.Session().AlarmCountByState(EAlarmStateWaitingToNotify);
	TheAlarmTest(alarmCount == 1, __LINE__);
	alarmCount = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
	TheAlarmTest(alarmCount == 8, __LINE__);

	//last 8 alarms in our array should correspond to array of alarms with state queued
	RArray<TAlarmId> alarmsWithStateQueued;
	CleanupClosePushL(alarmsWithStateQueued);

	TheAlarmTest.Session().GetAlarmIdListByStateL(EAlarmStateQueued, alarmsWithStateQueued);
		for(TInt jj=0; jj<8; jj++)
		{
		TheAlarmTest(alarmIds[jj+2] == alarmsWithStateQueued[jj], __LINE__);
		}

	CleanupStack::PopAndDestroy(&alarmsWithStateQueued);    // Close array.
#else
	//wait for alarm to expire
	WaitForAlarmToExpire();
	//states should be:
	// 1 x EAlarmStateNotified
	// 9 x EAlarmStateQueued
	alarmCount = TheAlarmTest.Session().AlarmCountByState(EAlarmStateNotified);
	TheAlarmTest(alarmCount == 1, __LINE__);
	alarmCount = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
	TheAlarmTest(alarmCount == 9, __LINE__);

	//wait for another alarm to expire
	WaitForAlarmToExpire();
	//states should be:
	// 2 x EAlarmStateNotified
	// 8 x EAlarmStateQueued
	alarmCount = TheAlarmTest.Session().AlarmCountByState(EAlarmStateNotified);
	TheAlarmTest(alarmCount == 2, __LINE__);
	alarmCount = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
	TheAlarmTest(alarmCount == 8, __LINE__);

	//last 8 alarms in our array should correspond to array of alarms with state queued
	RArray<TAlarmId> alarmsWithStateQueued;
	CleanupClosePushL(alarmsWithStateQueued);

	TheAlarmTest.Session().GetAlarmIdListByStateL(EAlarmStateQueued, alarmsWithStateQueued);
		for(TInt jj=0; jj<8; jj++)
		{
		TheAlarmTest(alarmIds[jj+2] == alarmsWithStateQueued[jj], __LINE__);
		}

	CleanupStack::PopAndDestroy(&alarmsWithStateQueued);    // Close array.
#endif
#endif

	//delete all alarms individually
	for (TInt count = alarmIds.Count() - 1; count >= 0; count--)
		{
		TheAlarmTest.Session().AlarmDelete(alarmIds[count]);
		}

	CleanupStack::PopAndDestroy(&alarmIds);    // Close array.
	}


void TestAlarmDetails3L()
	{
	TheAlarmTest.Test().Next(_L("test alarm details - 3"));
	//******************************************************************************
	// tests
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
	TheAlarmTest.TestClearStoreL();

	//add 5 alarms with category KASCliCategoryClock
	for(TInt ii=1; ii<=5; ii++)
		{
		AddAlarm(ii, KASCliCategoryClock);
		}

	//get list of categories - should contain only KASCliCategoryClock
	RArray<TAlarmCategory> alarmCategories;
	CleanupClosePushL(alarmCategories);

	TheAlarmTest.Session().GetAvailableCategoryListL(alarmCategories);
	TheAlarmTest(alarmCategories.Count() == 1 && alarmCategories[0] == KASCliCategoryClock, __LINE__);

	//add 5 alarms with category KASCliCategoryTest
	for(TInt jj=1; jj<=5; jj++)
		{
		AddAlarm(jj, KASCliCategoryTest);
		}

	alarmCategories.Reset();
	TheAlarmTest.Session().GetAvailableCategoryListL(alarmCategories);
	TheAlarmTest(alarmCategories.Count() == 2, __LINE__);

	CleanupStack::PopAndDestroy(&alarmCategories);    // Close array.

	//add another 10 alarms with category KASCliCategoryClock
	for(TInt kk=1; kk<=10; kk++)
		{
		AddAlarm(kk, KASCliCategoryClock);
		}

	//get alarms with category KASCliCategoryClock
	RArray<TAlarmId> alarms;
	CleanupClosePushL(alarms);

	TheAlarmTest.Session().GetAlarmIdListForCategoryL(KASCliCategoryClock, alarms);
	TInt count = alarms.Count();
	TheAlarmTest(count == 15, __LINE__);
	//check each alarm has category KASCliCategoryClock
	for(TInt mm=0; mm<15; mm++)
		{
		//directly
		TAlarmCategory category;
		TheAlarmTest(TheAlarmTest.Session().GetAlarmCategory(alarms[mm], category) == KErrNone, __LINE__);
		TheAlarmTest(category == KASCliCategoryClock, __LINE__);
		//indirectly
		TASShdAlarm alarm;
		TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(alarms[mm], alarm) == KErrNone, __LINE__);
		TheAlarmTest(alarm.Category() == KASCliCategoryClock, __LINE__);
		}

	CleanupStack::PopAndDestroy(&alarms);    // Close array.

	//should now have 15 alarms with category KASCliCategoryClock and 5 with category KASCliCategoryTest
	TheAlarmTest(TheAlarmTest.Session().GetAlarmCountForCategory(KASCliCategoryClock)==15 &&
		TheAlarmTest.Session().GetAlarmCountForCategory(KASCliCategoryTest)==5, __LINE__);
	//disable all KASCliCategoryClock alarms
	TheAlarmTest.Session().SetAlarmStatusByCategory(KASCliCategoryClock, EAlarmStatusDisabled);
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue()==5, __LINE__);
	//try to delete all KASCliCategoryTest orphaned alarms (should be none)
	TheAlarmTest.Session().AlarmDeleteAllByCategory(KASCliCategoryTest, ETrue);
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue()==5, __LINE__);
	//delete all KASCliCategoryTest alarms
	TheAlarmTest.Session().AlarmDeleteAllByCategory(KASCliCategoryTest, EFalse);
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue()==0, __LINE__);
	//re-enable the KASCliCategoryClock alarms
	TheAlarmTest.Session().SetAlarmStatusByCategory(KASCliCategoryClock, EAlarmStatusEnabled);
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue()==15, __LINE__);
	//create a new enabled alarm
	TAlarmId id = AddAlarm(1, KASCliCategoryClock);
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue()==16, __LINE__);
	//check status
	TAlarmStatus status;
	TheAlarmTest(TheAlarmTest.Session().GetAlarmStatus(id, status) == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmStatusEnabled, __LINE__);
	//disable it
	TheAlarmTest.Session().SetAlarmStatus(id, EAlarmStatusDisabled);
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue()==15, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().GetAlarmStatus(id, status) == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmStatusDisabled, __LINE__);

	//delete all remaining alarms
	TheAlarmTest.Session().AlarmDeleteAllByCategory(KASCliCategoryClock, EFalse);
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue()==0, __LINE__);
	}

void TestDeleteSnoozedAlarmL()
	{
	TheAlarmTest.Test().Next(_L("test delete snoozed agenda alarm"));
	//******************************************************************************
	// tests
	//
	// AlarmDeleteByCategory
	//******************************************************************************
	TheAlarmTest.TestClearStoreL();

	//add 1 alarm with category KUidAgendaModelAlarmCategory

	const TUid KUidAgendaModelAlarmCategory = { 0x101F4A70 };
	TAlarmId id=AddAlarm(1, KUidAgendaModelAlarmCategory);
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue()==1, __LINE__);
	User::After(1000000 * 61);
	TTime now;
	now.UniversalTime();
	TTime timeToSnoozeUntil = now+ TTimeIntervalMinutes(1);
	TheAlarmTest(TheAlarmTest.Session().__DbgSnoozeAlarm(id, timeToSnoozeUntil) == KErrNone);

	TDeleteType whatToDelete=0;
	whatToDelete=EFuture|EExpired;
	User::LeaveIfError(TheAlarmTest.Session().AlarmDeleteByCategory(KUidAgendaModelAlarmCategory, whatToDelete));
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue()==1, __LINE__);

	//delete all alarms
	TheAlarmTest.Session().AlarmDeleteAllByCategory(KUidAgendaModelAlarmCategory, EFalse);
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue()==0, __LINE__);
	}

	
static void TestCategorySpecificFunctionalityUtcL()
	{
	TheAlarmTest.Test().Next(_L("test category specific functionality for UTC alarms"));

	RArray<TAlarmCategory> categoryList;
	CleanupClosePushL(categoryList);

	TheAlarmTest.Session().GetAvailableCategoryListL(categoryList);
	TheAlarmTest(categoryList.Count() == 0, __LINE__);
	categoryList.Reset();

	//add 5 alarms with category KASCliCategoryClock 1,3,5,7,9 minutes from now
	TAlarmId id = KNullAlarmId;
	for (TInt ii=1; ii<=5; ++ii)
		{
		const TAlarmId allocatedId = AddUtcAlarm(2*ii - 1);
		if (id == KNullAlarmId)
			{
			id = allocatedId;
			}
		TheAlarmTest(TheAlarmTest.Session().GetAlarmCountForCategory(KASCliCategoryClock) == ii, __LINE__);
		}
	TheAlarmTest.Session().GetAvailableCategoryListL(categoryList);
	TheAlarmTest(categoryList.Count() == 1 && categoryList[0] == KASCliCategoryClock, __LINE__);
	categoryList.Reset();

	//add 5 alarms with category KASCliCategoryTest 2,4,6,8,10 minutes from now
	for (TInt jj=1; jj<=5; ++jj)
		{
		AddUtcAlarm(2*jj, KASCliCategoryTest);
		TheAlarmTest(TheAlarmTest.Session().GetAlarmCountForCategory(KASCliCategoryTest) == jj, __LINE__);
		}
	TheAlarmTest.Session().GetAvailableCategoryListL(categoryList);
	TheAlarmTest(categoryList.Count() == 2, __LINE__);
	categoryList.Reset();

	VerifyStates(10);
	WaitForAlarmToExpire();
	VerifyStates(9,1);
	WaitForAlarmToExpire();
	VerifyStates(8,1,1);

	//delete all KASCliCategoryTest alarms
	AlarmDeleteAllByCategoryIndirectlyL(KASCliCategoryTest);
	TheAlarmTest.Session().GetAvailableCategoryListL(categoryList);
	TheAlarmTest(categoryList.Count() == 1 && categoryList[0] == KASCliCategoryClock, __LINE__);
	categoryList.Reset();

	//delete all KASCliCategoryClock alarms
	TheAlarmTest.Session().AlarmDeleteAllByCategory(KASCliCategoryClock, EFalse);
	TheAlarmTest.Session().GetAvailableCategoryListL(categoryList);
	TheAlarmTest(categoryList.Count() == 0, __LINE__);

	CleanupStack::PopAndDestroy(&categoryList);    // Close array.
	}


void TestOverlappingUtcAlarmsL()
	{
	TheAlarmTest.Test().Next(_L("test overlapping alarms"));

	//schedule 3 alarms for the same time
	TTime time;
	// TASShdAlarm::SetUtcNextDueTime(TTime aUtcTime), therefore can't use time.HomeTime()
	time.UniversalTime();
	time += TTimeIntervalSeconds(10);
	for(TInt ii=0; ii<3; ii++)
		{
		AddUtcAlarm(time);
		}

	VerifyStates(3);
	WaitForAlarmToExpire();
	VerifyStates(0,1,2);

	TheAlarmTest.TestClearStoreL();
	TRequestStatus status;
	TAlarmId alarmId;
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);

	// TASShdAlarm::SetUtcNextDueTime(TTime aUtcTime), therefore can't use time.HomeTime()
	time.UniversalTime();
	time += TTimeIntervalSeconds(10);
	TAlarmId id1 = AddUtcAlarm(time);
	time += TTimeIntervalSeconds(10);
	TAlarmId id2 = AddUtcAlarm(time);

	TASShdAlarm alarm1;
	TASShdAlarm alarm2;

#if defined(_DEBUG)
	WaitForAlarmToExpire();
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id1, alarm1) == KErrNone, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id2, alarm2) == KErrNone, __LINE__);
#if defined(__WINSCW__)
	TheAlarmTest(alarm1.State() == EAlarmStateNotifying, __LINE__);
	TheAlarmTest(alarm2.State() == EAlarmStateQueued, __LINE__);
#else
	TheAlarmTest(alarm1.State() == EAlarmStateNotified, __LINE__);
	TheAlarmTest(alarm2.State() == EAlarmStateQueued, __LINE__);
#endif

	WaitForAlarmToExpire();
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id1, alarm1) == KErrNone, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id2, alarm2) == KErrNone, __LINE__);
#if defined(__WINSCW__)
	TheAlarmTest(alarm1.State() == EAlarmStateNotifying, __LINE__);
	TheAlarmTest(alarm2.State() == EAlarmStateWaitingToNotify, __LINE__);
#else
	TheAlarmTest(alarm1.State() == EAlarmStateNotified, __LINE__);
	TheAlarmTest(alarm2.State() == EAlarmStateNotified, __LINE__);
#endif
#endif

	TheAlarmTest.TestClearStoreL();
	}


void TestUtcAlarmDetails1L()
	{
	TheAlarmTest.Test().Next(_L("test alarm details - 1"));

	TheAlarmTest.TestClearStoreL();

	TInt id = AddUtcAlarm(1);

	TAlarmStatus status;
	TheAlarmTest(TheAlarmTest.Session().GetAlarmStatus(id, status) == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmStatusEnabled, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().SetAlarmStatus(id, EAlarmStatusDisabled) == KErrNone, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().GetAlarmStatus(id, status) == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmStatusDisabled, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().SetAlarmStatusByCategory(KASCliCategoryClock, EAlarmStatusEnabled) == KErrNone, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().GetAlarmStatus(id, status) == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmStatusEnabled, __LINE__);

	WaitForAlarmToExpire();

	TheAlarmTest.Session().AlarmDelete(id);
	}


void TestUtcAlarmDetails2L()
	{
	TheAlarmTest.Test().Next(_L("test alarm details - 2"));
	//******************************************************************************
	// tests
	//
	// AlarmAddL
	// AlarmDeleteL
	// AlarmCountByCategory
	// AlarmCountByState
	// AlarmIdListByStateLC
	//******************************************************************************
	TheAlarmTest.TestClearStoreL();

	RArray<TAlarmId> alarmIds;
	CleanupClosePushL(alarmIds);

	//add 10 alarms (category KASCliCategoryClock)
	for(TInt ii=1; ii<=10; ii++)
		{
		TInt minutesFromNow = (ii);
		TAlarmId id = AddUtcAlarm(minutesFromNow);
		alarmIds.AppendL(id);

		TInt numberOfAlarms = TheAlarmTest.Session().GetAlarmCountForCategory(KASCliCategoryClock);
		TheAlarmTest(numberOfAlarms == ii, __LINE__);
		}

	//check states of alarms
	TInt alarmCount=0;
	//states should be:
	// 10 x EAlarmStateQueued
	alarmCount = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
	TheAlarmTest(alarmCount == 10, __LINE__);

#if defined(_DEBUG)
#if defined(__WINSCW__)
	//wait for alarm to expire
	WaitForAlarmToExpire();
	//states should be:
	// 1 x EAlarmStateNotifying
	// 9 x EAlarmStateQueued
	alarmCount = TheAlarmTest.Session().AlarmCountByState(EAlarmStateNotifying);
	TheAlarmTest(alarmCount == 1, __LINE__);
	alarmCount = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
	TheAlarmTest(alarmCount == 9, __LINE__);

	//wait for another alarm to expire
	WaitForAlarmToExpire();
	//states should be:
	// 1 x EAlarmStateNotifying
	// 1 x EAlarmStateWaitingToNotify
	// 8 x EAlarmStateQueued
	alarmCount = TheAlarmTest.Session().AlarmCountByState(EAlarmStateNotifying);
	TheAlarmTest(alarmCount == 1, __LINE__);
	alarmCount = TheAlarmTest.Session().AlarmCountByState(EAlarmStateWaitingToNotify);
	TheAlarmTest(alarmCount == 1, __LINE__);
	alarmCount = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
	TheAlarmTest(alarmCount == 8, __LINE__);

	//last 8 alarms in our array should correspond to array of alarms with state queued
	RArray<TAlarmId> alarmsWithStateQueued;
	CleanupClosePushL(alarmsWithStateQueued);

	TheAlarmTest.Session().GetAlarmIdListByStateL(EAlarmStateQueued, alarmsWithStateQueued);
		for(TInt jj=0; jj<8; jj++)
		{
		TheAlarmTest(alarmIds[jj+2] == alarmsWithStateQueued[jj], __LINE__);
		}

	CleanupStack::PopAndDestroy(&alarmsWithStateQueued);    // Close array.
#else
	//wait for alarm to expire
	WaitForAlarmToExpire();
	//states should be:
	// 1 x EAlarmStateNotified
	// 9 x EAlarmStateQueued
	alarmCount = TheAlarmTest.Session().AlarmCountByState(EAlarmStateNotified);
	TheAlarmTest(alarmCount == 1, __LINE__);
	alarmCount = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
	TheAlarmTest(alarmCount == 9, __LINE__);

	//wait for another alarm to expire
	WaitForAlarmToExpire();
	//states should be:
	// 2 x EAlarmStateNotified
	// 8 x EAlarmStateQueued
	alarmCount = TheAlarmTest.Session().AlarmCountByState(EAlarmStateNotified);
	TheAlarmTest(alarmCount == 2, __LINE__);
	alarmCount = TheAlarmTest.Session().AlarmCountByState(EAlarmStateQueued);
	TheAlarmTest(alarmCount == 8, __LINE__);

	//last 8 alarms in our array should correspond to array of alarms with state queued
	RArray<TAlarmId> alarmsWithStateQueued;
	CleanupClosePushL(alarmsWithStateQueued);

	TheAlarmTest.Session().GetAlarmIdListByStateL(EAlarmStateQueued, alarmsWithStateQueued);
		for(TInt jj=0; jj<8; jj++)
		{
		TheAlarmTest(alarmIds[jj+2] == alarmsWithStateQueued[jj], __LINE__);
		}

	CleanupStack::PopAndDestroy(&alarmsWithStateQueued);    // Close array.
#endif
#endif

	//delete all alarms individually
	for (TInt count = alarmIds.Count() - 1; count >= 0; count--)
		{
		TheAlarmTest.Session().AlarmDelete(alarmIds[count]);
		}

	CleanupStack::PopAndDestroy(&alarmIds);    // Close array.
	}


void TestUtcAlarmDetails3L()
	{
	TheAlarmTest.Test().Next(_L("test alarm details - 3"));
	//******************************************************************************
	// tests
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
	TheAlarmTest.TestClearStoreL();

	//add 5 alarms with category KASCliCategoryClock
	for(TInt ii=1; ii<=5; ii++)
		{
		AddUtcAlarm(ii, KASCliCategoryClock);
		}

	//get list of categories - should contain only KASCliCategoryClock
	RArray<TAlarmCategory> alarmCategories;
	CleanupClosePushL(alarmCategories);

	TheAlarmTest.Session().GetAvailableCategoryListL(alarmCategories);
	TheAlarmTest(alarmCategories.Count() == 1 && alarmCategories[0] == KASCliCategoryClock, __LINE__);

	//add 5 alarms with category KASCliCategoryTest
	for(TInt jj=1; jj<=5; jj++)
		{
		AddUtcAlarm(jj, KASCliCategoryTest);
		}

	alarmCategories.Reset();
	TheAlarmTest.Session().GetAvailableCategoryListL(alarmCategories);
	TheAlarmTest(alarmCategories.Count() == 2, __LINE__);

	CleanupStack::PopAndDestroy(&alarmCategories);    // Close array.

	//add another 10 alarms with category KASCliCategoryClock
	for(TInt kk=1; kk<=10; kk++)
		{
		AddUtcAlarm(kk, KASCliCategoryClock);
		}

	//get alarms with category KASCliCategoryClock
	RArray<TAlarmId> alarms;
	CleanupClosePushL(alarms);

	TheAlarmTest.Session().GetAlarmIdListForCategoryL(KASCliCategoryClock, alarms);
	TInt count = alarms.Count();
	TheAlarmTest(count == 15, __LINE__);
	//check each alarm has category KASCliCategoryClock
	for(TInt mm=0; mm<15; mm++)
		{
		//directly
		TAlarmCategory category;
		TheAlarmTest(TheAlarmTest.Session().GetAlarmCategory(alarms[mm], category) == KErrNone, __LINE__);
		TheAlarmTest(category == KASCliCategoryClock, __LINE__);
		//indirectly
		TASShdAlarm alarm;
		TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(alarms[mm], alarm) == KErrNone, __LINE__);
		TheAlarmTest(alarm.Category() == KASCliCategoryClock, __LINE__);
		}

	CleanupStack::PopAndDestroy(&alarms);    // Close array.

	//should now have 15 alarms with category KASCliCategoryClock and 5 with category KASCliCategoryTest
	TheAlarmTest(TheAlarmTest.Session().GetAlarmCountForCategory(KASCliCategoryClock)==15 &&
		TheAlarmTest.Session().GetAlarmCountForCategory(KASCliCategoryTest)==5, __LINE__);
	//disable all KASCliCategoryClock alarms
	TheAlarmTest.Session().SetAlarmStatusByCategory(KASCliCategoryClock, EAlarmStatusDisabled);
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue()==5, __LINE__);
	//try to delete all KASCliCategoryTest orphaned alarms (should be none)
	TheAlarmTest.Session().AlarmDeleteAllByCategory(KASCliCategoryTest, ETrue);
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue()==5, __LINE__);
	//delete all KASCliCategoryTest alarms
	TheAlarmTest.Session().AlarmDeleteAllByCategory(KASCliCategoryTest, EFalse);
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue()==0, __LINE__);
	//re-enable the KASCliCategoryClock alarms
	TheAlarmTest.Session().SetAlarmStatusByCategory(KASCliCategoryClock, EAlarmStatusEnabled);
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue()==15, __LINE__);
	//create a new enabled alarm
	TAlarmId id = AddUtcAlarm(1, KASCliCategoryClock);
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue()==16, __LINE__);
	//check status
	TAlarmStatus status;
	TheAlarmTest(TheAlarmTest.Session().GetAlarmStatus(id, status) == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmStatusEnabled, __LINE__);
	//disable it
	TheAlarmTest.Session().SetAlarmStatus(id, EAlarmStatusDisabled);
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue()==15, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().GetAlarmStatus(id, status) == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmStatusDisabled, __LINE__);

	//delete all remaining alarms
	TheAlarmTest.Session().AlarmDeleteAllByCategory(KASCliCategoryClock, EFalse);
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue()==0, __LINE__);
	}


void TestDeleteSnoozedUtcAlarmL()
	{
	TheAlarmTest.Test().Next(_L("test delete snoozed agenda alarm"));
	//******************************************************************************
	// tests
	//
	// AlarmDeleteByCategory
	//******************************************************************************
	TheAlarmTest.TestClearStoreL();

	//add 1 alarm with category KUidAgendaModelAlarmCategory

	const TUid KUidAgendaModelAlarmCategory = { 0x101F4A70 };
	TAlarmId id=AddUtcAlarm(1, KUidAgendaModelAlarmCategory);
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue()==1, __LINE__);
	User::After(1000000 * 61);
	TTime now;
	now.UniversalTime();
	TTime timeToSnoozeUntil = now+ TTimeIntervalMinutes(1);
	TheAlarmTest(TheAlarmTest.Session().__DbgSnoozeAlarm(id, timeToSnoozeUntil) == KErrNone);

	TDeleteType whatToDelete=0;
	whatToDelete=EFuture|EExpired;
	User::LeaveIfError(TheAlarmTest.Session().AlarmDeleteByCategory(KUidAgendaModelAlarmCategory, whatToDelete));
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue()==1, __LINE__);

	//delete all alarms
	TheAlarmTest.Session().AlarmDeleteAllByCategory(KUidAgendaModelAlarmCategory, EFalse);
	TheAlarmTest(TheAlarmTest.Session().NumberOfAlarmsActiveInQueue()==0, __LINE__);
	}

/**
@SYMTestCaseID PIM-TCLIENTSESSION-0001
*/
	
static void DoTestsL()
	{
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	TheAlarmTest.Test().Start(_L("@SYMTestCaseID PIM-TCLIENTSESSION-0001 Connecting to server"));
	TInt r = TheAlarmTest.Session().Connect();
	TheAlarmTest(r == KErrNone, __LINE__);

#if defined(_DEBUG)
// The alarm should only be allowed to display notification in WINS builds.
#if defined(__WINSCW__)
	TheAlarmTest.Session().__DbgPreventUserNotify(EFalse);
#else
	TheAlarmTest.Session().__DbgPreventUserNotify(ETrue);
#endif
#endif

	TheAlarmTest.TestClearStoreL();

	TestCategorySpecificFunctionalityL();
	TestOverlappingAlarmsL();
	TestAlarmDetails1L();
	TestAlarmDetails2L();
	TestAlarmDetails3L();

#if defined(_DEBUG) && defined(__WINS__)
	TestDeleteSnoozedAlarmL();
#endif

	TheAlarmTest.TestClearStoreL();

	TestCategorySpecificFunctionalityUtcL();
	TestOverlappingUtcAlarmsL();
	TestUtcAlarmDetails1L();
	TestUtcAlarmDetails2L();
	TestUtcAlarmDetails3L();

#if defined(_DEBUG) && defined(__WINS__)
	TestDeleteSnoozedUtcAlarmL();
#endif

	TheAlarmTest.TestClearStoreL();

	TheAlarmTest.Session().Close();

	CleanupStack::PopAndDestroy(scheduler);
	}

GLDEF_C TInt E32Main()
//
// Test the alarm server.
//
	{
	__UHEAP_MARK;
	TInt ret = KErrNone;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if (!cleanup)
	    {
	    return KErrNoMemory;
	    }

	TheAlarmTest.Title();
	TheAlarmTest.TestStartServers();
	//
	TRAPD(error, DoTestsL());
	TheAlarmTest(error == KErrNone, __LINE__);
	//
    TRAP(ret,TheAlarmTest.EndL());
    TheAlarmTest.Test().Close();
	ASTstAlarmTest::Close();

	delete cleanup;
	__UHEAP_MARKEND;
	return ret;
	}
