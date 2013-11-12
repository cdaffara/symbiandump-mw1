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

// System includes
#include <asclisoundplay.h>

// Constants

// Type definitions
typedef CArrayFixFlat<TASCliSoundPlayDefinition> CPlayIntervalArray;

// This define appears in ASSrvSoundSettings.cpp (KAlarmCycleStartOffsetMinutes)
// update it if it changes there!
#define KDefaultAlarmCycleStartOffsetMinutes	{0,1,2,3,5,10,20,30,45,60,90,120,180,240,KErrNotFound};
#define KNewAlarmCycleStartOffsetMinutes		{0,10,20,30,50,100,200,300,450,600,900,1200,1800,2400, 3000, 4000, 5000, 10000,KErrNotFound};

static void testPrintAlarmL()
//
//	Print details about alarm
//
	{
	
	TheAlarmTest.Test().Next(_L("Test printing details about alarms"));
	TInt num;
	//
	num = TheAlarmTest.CountOrphanedAlarmsL();
	TheAlarmTest.Test().Printf(_L("\tThe number of orphan alarms is %d\n"), num);
	//
	num = TheAlarmTest.Session().AlarmCountByState(EAlarmStateNotified);
	TheAlarmTest.Test().Printf(_L("\tThe number of review alarms is %d\n"), num);
	//
	num = TheAlarmTest.Session().AlarmCountByState(EAlarmStateWaitingToNotify);
	TheAlarmTest.Test().Printf(_L("\tThe number of unacknowledged alarms is %d\n"),num);
	//
	num = TheAlarmTest.Session().NumberOfAlarmsActiveInQueue();
	TheAlarmTest.Test().Printf(_L("\tThe number of next alarms is %d\n"),num);
	//
	
	
	TheAlarmTest.Test().Next(_L("Clearing all alarms"));
	TheAlarmTest.TestClearStoreL();
	}

static void resetSoundArrayToDefaultValuesL()
	{
	// Initialise sound array with default values
	CPlayIntervalArray* array = new(ELeave) CPlayIntervalArray(20);
	CleanupStack::PushL(array);

	// Populate the new array to replace the default
	const TInt minuteCycle[]=KDefaultAlarmCycleStartOffsetMinutes;
	TInt i = 0;
	TInt offset = minuteCycle[i];
	while (offset != KErrNotFound)
		{
		const TASCliSoundPlayDefinition item(offset, KDefaultSoundPlayDurationInSeconds);
		array->AppendL(item);
		offset = minuteCycle[++i];
		}

	// Try setting the sound details
	TheAlarmTest.Session().SetAlarmPlayIntervalsL(*array);

	CleanupStack::PopAndDestroy(array);
	}


static void testDefaultSoundInfoL()
//
//	Get default sound info details
//
	{
	TheAlarmTest.Test().Next(_L("Test the default sound play intervals"));
	// When the server is started, the alarm sound intervals should currently be
	// set to their defaults defined in KDefaultAlarmCycleStartOffsetMinutes

	// Initialise sound array with default values
	CPlayIntervalArray* array = new(ELeave) CPlayIntervalArray(20);
	CleanupStack::PushL(array);

	// Get default sound intervals
	TheAlarmTest.Session().GetAlarmPlayIntervalsL(*array);

	// Check that the default values are correct
	const TInt minuteCycle[]=KDefaultAlarmCycleStartOffsetMinutes;
	TInt i = 0;
	TInt offset = minuteCycle[i];
	while (offset != KErrNotFound)
		{
		const TASCliSoundPlayDefinition& item = array->At(i);
		TheAlarmTest(item.Offset().Int() == offset, __LINE__);
		TheAlarmTest(item.Duration().Int() == KDefaultSoundPlayDurationInSeconds, __LINE__);
		offset = minuteCycle[++i];
		}
	CleanupStack::PopAndDestroy(array);
	}


static void testSettingSoundInfoL()
//
//	Get and set the sound info details
//
	{
	TheAlarmTest.Test().Next(_L("Test setting the sound play intervals"));
	// When the server is started, the alarm sound intervals should currently be
	// set to their defaults defined in KDefaultAlarmCycleStartOffsetMinutes

	// Initialise sound array with default values
	CPlayIntervalArray* array = new(ELeave) CPlayIntervalArray(20);
	CleanupStack::PushL(array);

	// Populate the new array to replace the default
	const TInt minuteCycle[]=KNewAlarmCycleStartOffsetMinutes;
	TInt i = 0;
	TInt offset = minuteCycle[i];
	while (offset != KErrNotFound)
		{
		const TASCliSoundPlayDefinition item(offset, KDefaultSoundPlayDurationInSeconds);
		array->AppendL(item);
		offset = minuteCycle[++i];
		}

	// Try setting the sound details
	TheAlarmTest.Session().SetAlarmPlayIntervalsL(*array);

	// Fetch the details back again.
	CPlayIntervalArray* array2 = new(ELeave) CPlayIntervalArray(20);
	CleanupStack::PushL(array2);
	TheAlarmTest.Session().GetAlarmPlayIntervalsL(*array2);

	// Test that there are the same number of entries
	TheAlarmTest(array->Count() == array2->Count(), __LINE__);

	// Test entries are the same
	TInt count = array->Count();
	for(TInt j=0; j<count; j++)
		{
		const TASCliSoundPlayDefinition& item = array->At(j);
		const TASCliSoundPlayDefinition& item2 = array2->At(j);
		TheAlarmTest(item.Offset().Int() == item2.Offset().Int(), __LINE__);
		TheAlarmTest(item.Duration().Int() == item2.Duration().Int(), __LINE__);
		}

	CleanupStack::PopAndDestroy(2);
	}


static void testSettingSoundInfoBadValuesL()
//
//	Attempt to set the sound interval array to invalid values.
//
	{
	TheAlarmTest.Test().Next(_L("Test setting sound play intervals to invalid values"));

	// Initialise sound array with default values
	CPlayIntervalArray* array = new(ELeave) CPlayIntervalArray(20);
	CleanupStack::PushL(array);

	// Try setting the sound details to an empty array
	TRAPD(err, TheAlarmTest.Session().SetAlarmPlayIntervalsL(*array));
	TheAlarmTest(err == KErrArgument, __LINE__);

	// Try setting the sound intervals to an array which doesn't contain an entry
	// with an offset of zero
	const TASCliSoundPlayDefinition item(1, KDefaultSoundPlayDurationInSeconds); // offset of 1 min
	array->AppendL(item);
	TRAP(err, TheAlarmTest.Session().SetAlarmPlayIntervalsL(*array));
	TheAlarmTest(err == KErrArgument, __LINE__);
	CleanupStack::PopAndDestroy(array);
	}
	
/**
@SYMTestCaseID PIM-TALARMINFO-0001
*/
static void doTestsL()
	{

	TheAlarmTest.Start(_L("@SYMTestCaseID PIM-TALARMINFO-0001 Connecting to server"));

	const TInt KGmtOffset = 3600;
	_LIT(KTime, "20040730:050000.000000"); //Summertime
	TInt err = User::SetUTCTimeAndOffset(TTime(KTime), KGmtOffset);
	TheAlarmTest(err == KErrNone, __LINE__);

	const TInt r = TheAlarmTest.Session().Connect();
	TheAlarmTest(r == KErrNone, __LINE__);

	testPrintAlarmL();
	testDefaultSoundInfoL();
	testSettingSoundInfoL();
	testSettingSoundInfoBadValuesL();

	resetSoundArrayToDefaultValuesL();
	TheAlarmTest.TestClearStoreL();

	TheAlarmTest.EndL();
	}


GLDEF_C TInt E32Main()
//
// Test the alarm server.
//
    {
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if	(!cleanup)
		return KErrNoMemory;

	TheAlarmTest.Title();
	TheAlarmTest.TestStartServers();
	//
	TRAPD(error, doTestsL());
	TheAlarmTest(error == KErrNone, __LINE__);
	TheAlarmTest.Session().Close();
	TheAlarmTest.Test().Close();
	ASTstAlarmTest::Close();

	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
    }
