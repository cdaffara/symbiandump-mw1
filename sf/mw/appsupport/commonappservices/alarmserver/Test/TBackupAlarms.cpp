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
#include "ASSrvDefs.h"

#include <f32file.h>
#include <s32file.h>
#include <asclisoundplay.h>

#include <connect/sbeclient.h>
#include <tz.h>
// Type definitions
typedef CArrayFixFlat<TASCliSoundPlayDefinition> CPlayIntervalArray;


// Literal constants
_LIT(KAlarmServerTestBackupFile, "C:\\AlarmServer.bak");
_LIT8(KSomeAlarmData, "This is some 8-bit data");

//Temp fix for wk37, main updates for wk40
_LIT(KAlarmServerBackupFileFullPath, "C:\\private\\101F5027\\AlarmServer.ini");

static const TTimeIntervalMicroSeconds32 KTenSeconds = 10000000;

#define KNewAlarmCycleStartOffsetMinutes		{0,10,20,30,50,100,200,300,450,600,900,1200,1800,2400, 3000, 4000, 5000, 10000,KErrNotFound};

static RFs FsSession;
static conn::CSBEClient* BackupClient;
static TDriveList DriveList;


//
/* Try to give a fairly thorough test of the Alarm Server
 * during Startup Internalize, backup and restore
 */


//
// building block test Helper functions
//

static void Helper_StartBackupL()
	{
	// Use the Secure Backup Engine API to "start" a backup.  Ultimate effect
	// is to cause CASSrvAlarmQueue::StartAlarmStoreOperation() to be called.
	// This happens indirectly through CASSrvAlarmStore::BackupBeginningL()/
	// BackupCompletedL()/RestoreBeginningL()/RestoreCompletedL() methods.
	// These methods are implementations for MBackupRestoreNotificatioObserver
	// which monitors changes in the P&S property conn::KUidBackupRestoreKey.
	// This P&S property is changed by calling the Secure Backup Engine API
	// below.
	BackupClient->SetBURModeL(DriveList, conn::EBURBackupFull, conn::EBackupBase);
	}


static void Helper_EndBackupL()
	{
	// Use the Secure Backup Engine API to "end" a backup.  Ultimate effect
	// is to cause CASSrvAlarmQueue::StartAlarmStoreOperation() to be called.
	// This happens indirectly through CASSrvAlarmStore::BackupBeginningL()/
	// BackupCompletedL()/RestoreBeginningL()/RestoreCompletedL() methods.
	// These methods are implementations for MBackupRestoreNotificatioObserver
	// which monitors changes in the P&S property conn::KUidBackupRestoreKey.
	// This P&S property is changed by calling the Secure Backup Engine API
	// below.
	BackupClient->SetBURModeL(DriveList, conn::EBURNormal, conn::ENoBackup);

	// allow Alarm Server observers to be notified
	User::After(1000);
	}


static void Helper_StartRestoreL()
	{
	// Use the Secure Backup Engine API to "start" a restore.  Ultimate effect
	// is to cause CASSrvAlarmQueue::StartAlarmStoreOperation() to be called.
	// This happens indirectly through CASSrvAlarmStore::BackupBeginningL()/
	// BackupCompletedL()/RestoreBeginningL()/RestoreCompletedL() methods.
	// These methods are implementations for MBackupRestoreNotificatioObserver
	// which monitors changes in the P&S property conn::KUidBackupRestoreKey.
	// This P&S property is changed by calling the Secure Backup Engine API
	// below.
	BackupClient->SetBURModeL(DriveList, conn::EBURRestoreFull, conn::EBackupBase);

	// allow Alarm Server observers to be notified
	User::After(100000);
	}


static void Helper_EndRestoreL(TBool aSuccess)
	{
	// Use the Secure Backup Engine API to "end" a restore.  Ultimate effect
	// is to cause CASSrvAlarmQueue::StartAlarmStoreOperation() to be called.
	// This happens indirectly through CASSrvAlarmStore::BackupBeginningL()/
	// BackupCompletedL()/RestoreBeginningL()/RestoreCompletedL() methods.
	// These methods are implementations for MBackupRestoreNotificatioObserver
	// which monitors changes in the P&S property conn::KUidBackupRestoreKey.
	// This P&S property is changed by calling the Secure Backup Engine API
	// below.
	if (aSuccess)
		{
		BackupClient->SetBURModeL(DriveList, conn::EBURNormal, conn::ENoBackup);
		}
	else
		{
		// starting a backup before a restore has completed.
		BackupClient->SetBURModeL(DriveList, conn::EBURBackupFull, conn::EBackupBase);
		}

	// allow Alarm Server observers to be notified
	User::After(1000000);
	}


static CDirectFileStore* Helper_StartRestoreAndOpenL()
	{
	Helper_StartRestoreL();

	TheAlarmTest.Test().Printf(_L("- Open and Lock Exclusive AlarmServer.Ini file\r\n"));
	// actually lock the file C:\System\Data\AlarmServer.ini -
	CDirectFileStore* store = NULL;
	for(TUint i=0; i < 10; i++)
		{
		TRAPD(err, store = CDirectFileStore::OpenL(FsSession,
			KAlarmServerBackupFileFullPath, EFileRead | EFileShareExclusive));
		// success?
		if (err == KErrNone)
			break;
		if (err != KErrInUse)
			User::Leave(err);
		// wait a little (10ms) & try again
		User::After(10000);
		}

	return store;
	}


static CDirectFileStore* Helper_StartBackupAndOpenL()
	{
	Helper_StartBackupL();

	TheAlarmTest.Test().Printf(_L("- Open and Lock Share Readers AlarmServer.Ini file\r\n"));
	// actually lock the file C:\System\Data\AlarmServer.ini -
	CDirectFileStore* store = NULL;
	for(TUint i=0; i < 10; i++)
		{
		TRAPD(err, store = CDirectFileStore::OpenL(FsSession,
			KAlarmServerBackupFileFullPath, EFileRead | EFileShareReadersOnly));
		// success?
		if (err == KErrNone)
			break;
		if (err != KErrInUse)
			User::Leave(err);
		// wait a little (10ms) & try again
		User::After(10000);
		}

	return store;
	}


//

//
// Backup & Restore support functions

/*
 * Makes a Backup of the Alarm Server ini file:
 * 1. Asks for Read Only lock for the AlarmServer.ini file
 * 2. Copies AlarmServer.ini to c:\AlarmServer.bak
 * 3. Hands back the file lock
 */
static void	BackupAlarmServerIniFileL()
	{
	TheAlarmTest.Test().Printf(_L("- Backup AlarmServer Ini file\r\n"));
	Helper_StartBackupL();	// make Backup

	CFileMan* fm=CFileMan::NewL(FsSession);
	FsSession.Delete(KAlarmServerTestBackupFile);		// no old file

	// keep on trying to copy AlarmServer.ini file, until it will be released by alarm server
	const TInt maxNumOfAttempts = 50;
	TInt numOfAttempts = 0;
	for (;numOfAttempts < maxNumOfAttempts; ++numOfAttempts)
		{
		if (KErrNone == fm->Copy(KAlarmServerBackupFileFullPath,KAlarmServerTestBackupFile))
			break;

		User::After(200000); //Wait for file to be released
		}
	TheAlarmTest(numOfAttempts < maxNumOfAttempts, __LINE__); //File was not locked after 50 attempts

	delete fm;
	Helper_EndBackupL();
	}


/*
 * Restores from a Backup of the Alarm Server ini file:
 * 1. Asks for full access file lock for the AlarmServer.ini file
 * 2. Copies c:\AlarmServer.bak to the AlarmServer.ini file
 * 3. Hands back the file lock
 */
static void RestoreAlarmServerIniFileL()
	{
	TheAlarmTest.Test().Printf(_L("- Restore AlarmServer Ini file\r\n"));

	// variables used to check Alarm Server notifications of Restore
	TAlarmId alarmId;
	TRequestStatus status;
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);

	Helper_StartRestoreL();
	TheAlarmTest.TestEventBuffer(EAlarmChangeEventRestoreStarted, status, alarmId);

	CFileMan* fm=CFileMan::NewL(FsSession);
	// didn't leave yet so:
	FsSession.Delete(KAlarmServerBackupFileFullPath);		// zap current file
	fm->Copy(KAlarmServerTestBackupFile,KAlarmServerBackupFileFullPath);
	delete fm;

	Helper_EndRestoreL(ETrue);
	}


/*
 * Pretends to start to make a Backup of the Alarm Server ini file:
 * 1. Asks for Read Only lock for the AlarmServer.ini file
 * Must delete store object and call SimulateBackupServerBackup_CompleteL()
 * after calling this!
 */
static CStreamStore* SimulateBackupServerBackup_StartL()
	{
	TheAlarmTest.Test().Printf(_L("- Claim AlarmServer Ini file for Backup\r\n"));

	// want to start a Backup and open the AlarmServer Ini file
	return Helper_StartBackupAndOpenL();
	}


/*
 * Ends pretend Backup of the Alarm Server ini file:
 * 1. Hands back the file lock
 * Must be preceded by a call to SimulateBackupServerBackup_StartL
 * and delete of the store object.
 */
static void SimulateBackupServerBackup_CompleteL()
	{
	TheAlarmTest.Test().Printf(_L("- ... Backup finished, return AlarmServer file lock\r\n"));

	Helper_EndBackupL();
	}

/*
 * Pretends to Restore the Alarm Server ini file:
 * 1. Asks for full access file lock for the AlarmServer.ini file
 * 2. Pauses briefly.
 * 3. Hands back the file lock
 * Alarm Server should re-Internalize existing alarms;
 * Session alarms should become orphaned.
 */
static void SimulateBackupServerRestorationL()
	{
	TheAlarmTest.Test().Printf(_L("- 'Restore' AlarmServer Ini file, (open and release)\r\n"));

	// want to start a Restore and open AlarmServer Ini file
	CDirectFileStore* store = Helper_StartRestoreAndOpenL();

	// take a little time for the Restore (.5s)
	User::After(500000);

	// now release file lock -
	TheAlarmTest.Test().Printf(_L("- Unlock AlarmServer.Ini file\r\n"));
	delete store;

	// restore success
	Helper_EndRestoreL(ETrue);
	}

/*
 * Pretends to start to Restore the Alarm Server ini file:
 * 1. Asks for full access file lock for the AlarmServer.ini file
 * Must be followed by a call to SimulateBackupServerRestore_Complete!
 */
static CStreamStore* SimulateBackupServerRestore_StartL()
	{
	CStreamStore* store = NULL;
	TInt err = KErrNone;

	TheAlarmTest.Test().Printf(_L("- Access AlarmServer Ini file, as if  Restoring\r\n"));

	// Use this Secure Backup Engine API to "start" a restore.  Ultimate effect
	// is to cause CASSrvAlarmQueue::StartAlarmStoreOperation() to be called.
	// This happens indirectly through CASSrvAlarmStore::BackupBeginningL()/
	// BackupCompletedL()/RestoreBeginningL()/RestoreCompletedL() methods.
	// These methods are implementations for MBackupRestoreNotificatioObserver
	// which monitors changes in the P&S property conn::KUidBackupRestoreKey.
	// This P&S property is changed by calling the Secure Backup Engine API
	// below.
	BackupClient->SetBURModeL(DriveList, conn::EBURRestoreFull, conn::EBackupBase);

	// actually lock the file C:\System\Data\AlarmServer.ini
	const TInt maxNumOfAttempts = 50;
	TInt numOfAttempts = 0;
	for (;numOfAttempts < maxNumOfAttempts; ++numOfAttempts)
		{
		TRAP(err,store = CDirectFileStore::OpenL(FsSession, KAlarmServerBackupFileFullPath,
												 EFileRead | EFileShareExclusive));
		RDebug::Print(_L("CDirectFileStore::OpenL %S, err: %d"), &KAlarmServerBackupFileFullPath, err);
		if (err == KErrNone)
			break;

		User::After(200000); //Wait for file to be released
		}
	TheAlarmTest(numOfAttempts < maxNumOfAttempts, __LINE__); //File was not locked after 50 attempts

	return store;
	}

/*
 * Ends pretend Restore of the Alarm Server ini file:
 * 1. Hands back the file lock
 * Alarm Server should re-Initernalize existing alarms;
 * Session alarms should become orphaned.
 * Must be preceded by acall to SimulateBackupServerRestore_StartLC
 */
static void SimulateBackupServerRestore_CompleteL()
	{
	TheAlarmTest.Test().Printf(_L("- Restore finished, release AlarmServer Ini file\r\n"));

	Helper_EndRestoreL(ETrue);
	}

/*
 * Ends pretend Restore of the Alarm Server ini file:
 * 1. Hands back the file lock
 * Alarm Server should re-Initernalize existing alarms;
 * Session alarms should become orphaned.
 * Must be preceded by a call to SimulateBackupServerRestore_StartL
 * and delete of the store object.
 */
//
// Shutdown & Restart of Alarm Server only possibly in Windows Udeb builds

#if defined(_DEBUG) && defined (__WINSCW__)
static void closeServers()
	{
	TInt r=TheAlarmTest.Session().Connect();
	TheAlarmTest(r==KErrNone, __LINE__);
	TheAlarmTest.Test().Printf(_L("Server shut down\r\n"));
	TheAlarmTest.Test().Printf(_L("now closing server...\r\n"));
	TheAlarmTest.Session().__DbgShutDownServer();
	TheAlarmTest.Session().Close();
	}

static void shutDownAndStartUpServers()
	{
	closeServers();
	User::After(5000000);
	TheAlarmTest.Test().Printf(_L("now restarting server...\r\n"));
	TheAlarmTest.TestStartServers();
	}
#endif

static TAlarmId addUtcAlarm(TInt aMinutesFromNow, TAlarmRepeatDefinition aRepeatDefinition = EAlarmRepeatDefintionRepeatOnce)
	{
	TBuf<200> testTitle;
	testTitle.Format(_L("- Adding alarm set with UTC time to expire in %d minutes"), aMinutesFromNow);
	TheAlarmTest.Test().Next(testTitle);
	//
	TASShdAlarm alarm;
	alarm.Category()=KASCliCategoryClock;
	alarm.RepeatDefinition()=aRepeatDefinition;
	TTime time;
	time.UniversalTime();
	time += TTimeIntervalMinutes(aMinutesFromNow);
	alarm.SetUtcNextDueTime(time);
	//
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
	const TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);
	// allow servers to catch up
	User::After(1000000);
	return id;
	}

static TAlarmId addAlarm(TInt aMinutesFromNow, TAlarmRepeatDefinition aRepeatDefinition = EAlarmRepeatDefintionRepeatOnce)
	{
	TBuf<200> testTitle;
	testTitle.Format(_L("- Adding alarm set with local wall-clock time to expire in %d minutes"), aMinutesFromNow);
	TheAlarmTest.Test().Next(testTitle);
	//
	TASShdAlarm alarm;
	alarm.Category() = KASCliCategoryClock;
	alarm.RepeatDefinition() = aRepeatDefinition;
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalMinutes(aMinutesFromNow);

	//
	TDateTime date=alarm.NextDueTime().DateTime();
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alarm) == KErrNone, __LINE__);
	const TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);
	TheAlarmTest.Test().Printf(_L("alarm %d due time is : hours: %d, minutes: %d, seconds: %d.\r\n"),
		     id,date.Hour(), date.Minute(), date.Second());
	// allow servers to catch up
	User::After(1000000);
	return id;
	}

//
//
//


/*****************************************************************************
 */

static void doTestAlarmQueueOrderShutdownStartupL()
	{
	TheAlarmTest.Test().Next(_L("* Test alarms set using local wall-clock time order stable over server shutdown/startup"));

#if !defined(_DEBUG) || !defined(__WINSCW__)
	// Can't shutdown Alarm Server on UREL or target builds
	TheAlarmTest.Test().Printf(_L("THIS TEST IS A WINS - DEBUG TEST ONLY\r\n"));
#else
	TheAlarmTest.TestClearStoreL();

	//add alarms
	TheAlarmTest.Test().Next(_L("add 5 alarms"));
	RArray<TAlarmId> alarmIds;
	for	(TInt ii=2; ii<=6; ii++)
		{
		TAlarmId id = addAlarm(ii);
		alarmIds.Append(id);
		}

	TheAlarmTest.Test().Next(_L("shutdown then restart Alarm Server"));
	shutDownAndStartUpServers();

	//get server alarms
	RArray<TAlarmId> serverAlarmIds;
	TheAlarmTest.Session().GetAlarmIdListForCategoryL(KASCliCategoryClock, serverAlarmIds);
	alarmIds.Sort();
	serverAlarmIds.Sort();
	TInt count = alarmIds.Count();
	TheAlarmTest(count == serverAlarmIds.Count(), __LINE__);
	for	(TInt jj=0; jj<count; jj++)
		{
		TheAlarmTest(alarmIds[jj] == serverAlarmIds[jj], __LINE__);
		}
	serverAlarmIds.Close();
	alarmIds.Close();
#endif
	}

/*****************************************************************************
 */

static void doTestAlarmQueueOrderBackupRestoreL()
	{
	TheAlarmTest.Test().Next(_L("* Test alarm order across Backup and Restore"));

	TheAlarmTest.TestClearStoreL();

	//add alarms
	TheAlarmTest.Test().Next(_L("add 5 alarms"));
	RArray<TAlarmId> alarmIds;
	for	(TInt ii=2; ii<=6; ii++)
		{
		TAlarmId id = addAlarm(ii);
		alarmIds.AppendL(id);
		}

	// brief pause before backup (.5s)
	User::After(500000);

	TheAlarmTest.Test().Next(_L("make real backup of alarmserver ini file"));
	TRAPD(err, BackupAlarmServerIniFileL());
	TheAlarmTest(err==KErrNone, __LINE__);

	// delete alarms
	TheAlarmTest.Test().Next(_L("delete alarms"));
	TInt jj;
	for (jj=0; jj<5; jj++)
		{
		TheAlarmTest(TheAlarmTest.Session().AlarmDelete(alarmIds[jj]) == KErrNone, __LINE__);
		}

	// pause before restore (5s)
	User::After(5000000);
	TheAlarmTest.Test().Next(_L("restore alarmserver ini file"));
	TRAP(err, RestoreAlarmServerIniFileL());
	TheAlarmTest(err==KErrNone, __LINE__);

	TheAlarmTest.Test().Next(_L("check restored alarms"));
	//get server alarms
	RArray<TAlarmId> serverAlarmIds;
	TheAlarmTest.Session().GetAlarmIdListForCategoryL(KASCliCategoryClock, serverAlarmIds);
	alarmIds.Sort();
	serverAlarmIds.Sort();
	TInt count = alarmIds.Count();
	TInt serverCount = serverAlarmIds.Count();
	TheAlarmTest.Test().Printf(_L("[Added Alarms count = %i, server Count = %i]\r\n"), count, serverCount);
	TheAlarmTest(count == serverCount, __LINE__);
	for	(jj=0; jj<count; jj++)
		{
		TheAlarmTest(alarmIds[jj] == serverAlarmIds[jj], __LINE__);
		}
	serverAlarmIds.Close();
	alarmIds.Close();
	}

/*****************************************************************************
 */

static void doTestAlarmQueueBackupRestoreL()
	{
	TheAlarmTest.Test().Next(_L("* Test correct alarms in queue after backup and restore"));

	TheAlarmTest.TestClearStoreL();

	// add an alarm to expire in 2 minutes
	TAlarmId id2 = addAlarm(2);
	// add an alarm to expire in 3 minutes
	TAlarmId id3 = addAlarm(3);

	// brief pause before backup (.5s)
	User::After(500000);

	TheAlarmTest.Test().Next(_L("make real backup of alarmserver ini file"));
	TRAPD(err, BackupAlarmServerIniFileL());
	TheAlarmTest(err==KErrNone, __LINE__);

	// delete alarms
	User::After(1000000);
	TheAlarmTest.Test().Next(_L("delete alarms"));
	TheAlarmTest(TheAlarmTest.Session().AlarmDelete(id2) == KErrNone, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().AlarmDelete(id3) == KErrNone, __LINE__);
	User::After(1000000);

	// add alarms
	TheAlarmTest.Test().Next(_L("add alarms"));
	// add alarms to expire now
	TAlarmId id0 = addAlarm(0);
	TAlarmId id1 = addAlarm(1);
	TASShdAlarm alarm;
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id0, alarm) == KErrNone, __LINE__);

	// first alarm (id0) should be notifying
	TheAlarmTest(alarm.State() == EAlarmStateNotifying, __LINE__);



	// pause briefly before restore (2s)
	User::After(2000000);
	TheAlarmTest.Test().Next(_L("restore alarmserver ini file"));
	TRAP(err, RestoreAlarmServerIniFileL());
	TheAlarmTest(err==KErrNone, __LINE__);

	TheAlarmTest.Test().Next(_L("check correct alarms after restore"));

	// alarms should now be gone
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id0, alarm) == KErrNotFound, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id1, alarm) == KErrNotFound, __LINE__);

	// alarms should be restored
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id2, alarm) == KErrNone, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id3, alarm) == KErrNone, __LINE__);
	}

/*****************************************************************************
 */

static void doTestAlarmSoundIntervalsBackupRestoreL()
	{
	TheAlarmTest.Test().Next(_L("* Test correct Sound Intervals after backup and restore"));

	TheAlarmTest.TestClearStoreL();

	// Fetch the Alarm Sound Intervals
	CPlayIntervalArray* originalSoundArray = new(ELeave) CPlayIntervalArray(20);
	CleanupStack::PushL(originalSoundArray);
	TheAlarmTest.Session().GetAlarmPlayIntervalsL(*originalSoundArray);

	TheAlarmTest.Test().Next(_L("set non-default sound intervals"));

	// Initialise sound array with default values
	CPlayIntervalArray* newSoundArray = new(ELeave) CPlayIntervalArray(20);
	CleanupStack::PushL(newSoundArray);

	// Populate the new array to replace the default
	const TInt minuteCycle[]=KNewAlarmCycleStartOffsetMinutes;
	TInt i = 0;
	TInt offset = minuteCycle[i];
	while (offset != KErrNotFound)
		{
		const TASCliSoundPlayDefinition item(offset, KDefaultSoundPlayDurationInSeconds);
		newSoundArray->AppendL(item);
		offset = minuteCycle[++i];
		}

	// Try setting the sound details
	TheAlarmTest.Session().SetAlarmPlayIntervalsL(*newSoundArray);

	// brief pause before backup (.5s)
	User::After(500000);

	TheAlarmTest.Test().Next(_L("make real backup of alarmserver ini file"));
	TRAPD(err, BackupAlarmServerIniFileL());
	TheAlarmTest(err==KErrNone, __LINE__);

	TheAlarmTest.Test().Next(_L("reset default sound intervals"));
	TheAlarmTest.Session().SetAlarmPlayIntervalsL(*originalSoundArray);

	// pause briefly before restore (2s)
	User::After(2000000);
	TheAlarmTest.Test().Next(_L("restore alarmserver ini file"));
	TRAP(err, RestoreAlarmServerIniFileL());
	TheAlarmTest(err==KErrNone, __LINE__);

	TheAlarmTest.Test().Next(_L("check correct sound intervals after restore"));

	// Fetch the details back again.
	CPlayIntervalArray* restoreSoundArray = new(ELeave) CPlayIntervalArray(20);
	CleanupStack::PushL(restoreSoundArray);
	TheAlarmTest.Session().GetAlarmPlayIntervalsL(*restoreSoundArray);

	// Test that there are the same number of entries
	TheAlarmTest(newSoundArray->Count() == restoreSoundArray->Count(), __LINE__);

	// Test entries are the same
	TInt count = newSoundArray->Count();
	for(TInt j=0; j<count; j++)
		{
		const TASCliSoundPlayDefinition& item = newSoundArray->At(j);
		const TASCliSoundPlayDefinition& item2 = restoreSoundArray->At(j);
		TheAlarmTest(item.Offset().Int() == item2.Offset().Int(), __LINE__);
		TheAlarmTest(item.Duration().Int() == item2.Duration().Int(), __LINE__);
		}

	TheAlarmTest.Test().Next(_L("reset default sound intervals"));
	TheAlarmTest.Session().SetAlarmPlayIntervalsL(*originalSoundArray);

	CleanupStack::PopAndDestroy(3);// originalSoundArray, newSoundArray, restoreSoundArray
	}

/*****************************************************************************
 */

static void doTestAlarmAddDuringBackupL()
	{
	TheAlarmTest.Test().Next(_L("* Test alarm changes during backup"));

	TheAlarmTest.TestClearStoreL();

	// Attempt to add an alarm while the backup server is backing up the alarms
	CStreamStore* store = NULL;
	TRAPD(err, store = SimulateBackupServerBackup_StartL());
	TheAlarmTest(err==KErrNone, __LINE__);

	CleanupStack::PushL(store);

	TheAlarmTest.Test().Next(_L("adding alarm should work"));

	TASShdAlarm alm;
	TheAlarmTest(addAlarm(0, alm.RepeatDefinition()) != KNullAlarmId, __LINE__);

	TheAlarmTest.Test().Next(_L("changing time forces Alarm Store class to queue Externalize"));
	// Locale change (time) x2 (INC038240 regression)
	// Exercise CASSrvAlarmStore queue an Externalize
	TTime time;
	time.UniversalTime();
	time += TTimeIntervalSeconds(10);
	User::SetUTCTime(time);
	User::After(1000000);
	time -= TTimeIntervalSeconds(10);
	User::SetUTCTime(time);
	User::After(1000000);

	// release file lock -
	CleanupStack::PopAndDestroy(store); // store
	// backup complete
	TRAP(err, SimulateBackupServerBackup_CompleteL());
	TheAlarmTest(err==KErrNone, __LINE__);

	// wait for AlarmServer to catch-up
	User::After(1000000);

	TheAlarmTest.Test().Next(_L("check we can now add alarm"));

	// re-try alarm add after backup complete - should be successful
	TAlarmId id = addAlarm(0);
	TheAlarmTest(id != KNullAlarmId, __LINE__);

	// wait for AlarmServer to catch-up
	User::After(1000000);

	// make Alarm Server re-Internalize
	TheAlarmTest.Test().Next(_L("now simulating restore..."));
	TRAP(err, SimulateBackupServerRestorationL());
	TheAlarmTest(err==KErrNone, __LINE__);

	// check absence of alarm
	TheAlarmTest.Test().Next(_L("check alarm has gone"));
	TASShdAlarm alarm;
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id, alarm) == KErrNotFound, __LINE__);
	}

/*****************************************************************************
 */

static void doTestAlarmAddDuringRestoreL()
	{
	// variables used to check Alarm Server notifications of Restore
	TAlarmId alarmId;
	TRequestStatus status;

	TheAlarmTest.Test().Next(_L("* Test alarm changes during restore"));

	TheAlarmTest.TestClearStoreL();

	// Attempt to add an alarm while the backup server is restoring up the alarms
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	CStreamStore* store = NULL;
	TRAPD(err, store = SimulateBackupServerRestore_StartL());
	CleanupStack::PushL(store);
	TheAlarmTest(err==KErrNone, __LINE__);

	// check Restore Start notification received
	TheAlarmTest.Test().Next(_L("check for Alarm Restore Start notification"));
	User::After(1000000);
	TheAlarmTest(status == EAlarmChangeEventRestoreStarted, __LINE__);
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);

	TheAlarmTest(alarmId==KNullAlarmId, __LINE__);

	TheAlarmTest.Test().Next(_L("adding alarm should fail"));

	TASShdAlarm alm;
	alm.NextDueTime().HomeTime();
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alm) == KErrLocked, __LINE__);

	// allow servers to catch up
	User::After(1000000);

	TheAlarmTest.Test().Next(_L("changing time forces Alarm Store class to queue Externalize"));
	// Locale change (time) x2 (INC038240 regression)
	// Exercise CASSrvAlarmStore

	TTime time;
	time.UniversalTime();
	time += TTimeIntervalSeconds(10);
	User::SetUTCTime(time);
	User::After(1000000);
	time -= TTimeIntervalSeconds(10);
	User::SetUTCTime(time);
	User::After(1000000);

	CleanupStack::PopAndDestroy(store);

	// restore complete
	TRAP(err, SimulateBackupServerRestore_CompleteL());
	TheAlarmTest(err==KErrNone, __LINE__);

	// check Restore Complete notification received
	TheAlarmTest.Test().Next(_L("check for Alarm Restore Complete notification"));
	TheAlarmTest.TestEventBuffer(EAlarmChangeEventRestoreCompleted, status, alarmId);
	TheAlarmTest(alarmId==KNullAlarmId, __LINE__);


	// wait for AlarmServer to catch-up
	User::After(1000000);

	TheAlarmTest.Test().Next(_L("check we can now add alarm"));

	// re-try alarm add after restore complete - should be successful
	TAlarmId id = addAlarm(0);
	TheAlarmTest(id != KNullAlarmId, __LINE__);

	// wait for Alarm Server to externalize
	User::After(3000000);
	}

/*****************************************************************************
 */




/*****************************************************************************
 */

static void doTestNotifyingAlarmShutdownStartupL()
	{
	TheAlarmTest.Test().Next(_L("* Test Notifying alarm across shutdown / startup"));

	TheAlarmTest.TestClearStoreL();

	TheAlarmTest.Test().Next(_L("add an alarm"));
	// add an alarm to expire in 15 minutes
	TAlarmId id = addAlarm(15);

	// get alarm expiry time
	TASShdAlarm alarm;
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id, alarm) == KErrNone, __LINE__);
	TTime time = alarm.NextDueTime();

#if !defined(_DEBUG) || !defined(__WINSCW__)
	// Can't shutdown Alarm Server on UREL or target builds
	TheAlarmTest.Test().Printf(_L("THIS TEST IS A WINS - DEBUG TEST ONLY\r\n"));

	TheAlarmTest.Test().Printf(_L("Waiting for Alarm to expire\r\n"));
	// target Alternative: wait until after alarm due time
	User::SetHomeTime(time - TTimeIntervalSeconds(3));
	User::After(KTenSeconds);
#else
	// UDEB:
	// shut down the server
	closeServers();

	// re-start server after alarm expires
	User::SetHomeTime(time - TTimeIntervalSeconds(3));
	User::After(KTenSeconds);
	TheAlarmTest.Test().Next(_L("now restarting server...\r\n"));
	TheAlarmTest.TestStartServers();
	User::After(1000000);
#endif

	// Internalise on startup has a 59s window for recent alarms
	TheAlarmTest.Test().Next(_L("check that alarm is now set and notifying"));
	//check status of alarm
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id, alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.State() == EAlarmStateNotifying, __LINE__);

	TheAlarmTest(TheAlarmTest.Session().AlarmDelete(id) == KErrNone, __LINE__);
	}

/*****************************************************************************
 */

static void doTestNotifyingAlarmBackupRestoreL()
	{
	TheAlarmTest.Test().Next(_L("* Test Notifying alarm removed by backup / restore"));

	TheAlarmTest.TestClearStoreL();

	TheAlarmTest.Test().Next(_L("add an alarm"));
	// add an alarm to expire in 15 minutes
	TAlarmId id = addAlarm(15);

	TheAlarmTest.Test().Next(_L("make real backup of alarmserver ini file"));
	TRAPD(err, BackupAlarmServerIniFileL());
	TheAlarmTest(err==KErrNone, __LINE__);

	// get alarm expiry time
	TASShdAlarm alarm;
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id, alarm) == KErrNone, __LINE__);
	TTime time = alarm.NextDueTime();

	TheAlarmTest.Test().Printf(_L("Waiting for Alarm to expire\r\n"));

	// wait until after alarm due time
	User::SetHomeTime(time - TTimeIntervalSeconds(3));
	User::After(KTenSeconds);

	// Internalise on startup has a 59s window for recent alarms
	TheAlarmTest.Test().Next(_L("check that alarm is now set and notifying"));
	//check status of alarm
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id, alarm) == KErrNone, __LINE__);

	TheAlarmTest(alarm.State() == EAlarmStateNotifying, __LINE__);


	TheAlarmTest.Test().Next(_L("restore alarmserver ini file"));
	TRAP(err, RestoreAlarmServerIniFileL());
	TheAlarmTest(err==KErrNone, __LINE__);

	// allow Alarm server to notice file lock has been released
	User::After(1000000);

	// Internalise after Restore has a 0s window for recent alarms
	// check absence of alarm
	TheAlarmTest.Test().Next(_L("check alarm has gone\r\n"));
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id, alarm) == KErrNotFound, __LINE__);
	}

/*****************************************************************************
 */

static void doTestSessionAlarmShutdownRestartL()
	{
	TheAlarmTest.Test().Next(_L("* Test that Session alarm is removed by shutdown"));

#if !defined(_DEBUG) || !defined(__WINSCW__)
	// Can't shutdown Alarm Server on UREL or target builds
	TheAlarmTest.Test().Printf(_L("THIS TEST IS A WINS - DEBUG TEST ONLY\r\n"));
#else
	TheAlarmTest.TestClearStoreL();

	// Set the session alarm
	TheAlarmTest.Test().Next(_L("set session alarm in 2 minutes"));
	TASShdAlarm alarm;
	alarm.Category() = KASCliCategoryClock;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalMinutes(2);
	// session specific alarm (has a TRequestStatus to complete)
	alarm.Characteristics().Set(EAlarmCharacteristicsSessionSpecific);
	TRequestStatus status;
	TheAlarmTest.Session().AlarmAddWithNotification(status, alarm);
	const TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);

	// status should be pending
	TheAlarmTest.Test().Next(_L("checking request is pending"));
	TheAlarmTest(status == KRequestPending, __LINE__);

	TheAlarmTest.Test().Next(_L("Shutdown Alarm Server"));
	closeServers();

	User::After(10000000);

	// check absence of alarm
	TheAlarmTest.Test().Next(_L("check alarm notification has been aborted"));

	TheAlarmTest(status != KRequestPending, __LINE__);

	// Get the abort
	// (Probably KErrDied though KErrServerTerminated should be okay too)
	User::WaitForRequest(status);
	TInt stat = status.Int();
	TheAlarmTest((stat == KErrDied) || (stat == KErrServerTerminated), __LINE__);

	// now restart Alarm Server
	TheAlarmTest.Test().Next(_L("restart Alarm Server"));
	TheAlarmTest.TestStartServers();

	// check presence of alarm
	TheAlarmTest.Test().Next(_L("check shutdown deleted alarm"));
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id, alarm) == KErrNotFound, __LINE__);
#endif
	}

/*****************************************************************************
 */

static void doTestSessionAlarmBackupRestoreL()
	{
	TheAlarmTest.Test().Next(_L("* Test that Session alarm is orphaned by restore"));

	TheAlarmTest.TestClearStoreL();

	// Set the session alarm
	TheAlarmTest.Test().Next(_L("set session alarm in 20 minutes"));
	TASShdAlarm alarmWrite;
	alarmWrite.Category() = KASCliCategoryClock;
	alarmWrite.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	alarmWrite.NextDueTime().HomeTime();
	alarmWrite.NextDueTime() += TTimeIntervalMinutes(20);
	// session specific alarm (has a TRequestStatus to complete)
	alarmWrite.Characteristics().Set(EAlarmCharacteristicsSessionSpecific);
	TRequestStatus status;
	TheAlarmTest.Session().AlarmAddWithNotification(status, alarmWrite);
	// check that alarm was set okay
	const TAlarmId id = alarmWrite.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);

	// status should be pending
	TheAlarmTest.Test().Next(_L("checking request is pending"));
	TheAlarmTest(status == KRequestPending, __LINE__);

	// remember the alarm time
	TTime alarmTime = alarmWrite.NextDueTime();

	// make the Alarm server re-internlize ....
	TheAlarmTest.Test().Next(_L("now simulating restore..."));
	TRAPD(err, SimulateBackupServerRestorationL());
	TheAlarmTest(err==KErrNone, __LINE__);


	// allow Alarm server to Internalize...
	User::After(1000000);	// 1s


	// check old alarm queue has been cleared
	TheAlarmTest.Test().Next(_L("check alarm notification has been aborted"));
	TheAlarmTest(status != KRequestPending, __LINE__);

	// Get the abort result
	User::WaitForRequest(status);
	TheAlarmTest(status.Int() == KErrCancel, __LINE__);

	// check presence of alarm
	TASShdAlarm alarmRead;
	TheAlarmTest.Test().Next(_L("check alarm is no longer a session alarm"));
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id, alarmRead) == KErrNone, __LINE__);

	TheAlarmTest(!alarmRead.HasOwningSession(), __LINE__);
	TheAlarmTest(alarmRead.HasBecomeOrphaned(), __LINE__);

	// advance clock, wait for alarm
	User::SetHomeTime(alarmWrite.NextDueTime() - TTimeIntervalSeconds(3));
	TheAlarmTest.Test().Next(_L("waiting for alarm to notify"));
	// wait until after alarm due time
	User::After(KTenSeconds);

	// alarm should be notifying
	// if Alarm Server server tries to access the TRequestStatus
	// there will be an Access Violation
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id, alarmRead) == KErrNone, __LINE__);

	//TheAlarmTest(alarm.State() == EAlarmStateNotifying, __LINE__);

	const TAlarmState alarmState = alarmRead.State();
	TheAlarmTest(alarmState == EAlarmStateNotifying, __LINE__);
	}


/*****************************************************************************
 * Test adapted from TAlarmData, checks alarm data across Backup & Restore.
 * (Original tested across Alarm Server shutdown & re-start.)
 */

static void doTestRestoreValidAlarmDataL()
	{
	TheAlarmTest.Test().Next(_L("Data API test"));
	TheAlarmTest.TestClearStoreL();
	TInt r;
	TAlarmId alarmId;
	TASShdAlarm alarm;
	TRequestStatus status;
	//
	alarm.Message() = _L("Alarm with data");
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalMinutes(2);
	//
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r==KErrNone, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmChangeEventAlarmData, __LINE__);
	TheAlarmTest(alarmId = alarm.Id(), __LINE__);
	User::WaitForRequest(status);
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrInUse, __LINE__);
	TheAlarmTest(status == KRequestPending, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KSomeAlarmData().Size());
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDelete(alarm.Id());
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest.TestEventBuffer(EAlarmChangeEventAlarmData, status, alarmId);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalMinutes(2);
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r==KErrNone, __LINE__);
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmChangeEventAlarmData, __LINE__);
	TheAlarmTest(alarmId = alarm.Id(), __LINE__);
	User::WaitForRequest(status);
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest.TestEventBuffer(EAlarmChangeEventAlarmData, status, alarmId);
	//
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmChangeEventAlarmData, __LINE__);
	TheAlarmTest(alarmId = alarm.Id(), __LINE__);
	User::WaitForRequest(status);
	//
	HBufC8* buf = HBufC8::NewLC(KSomeAlarmData().Size());
	TPtr8 pBuf(buf->Des());
	r = TheAlarmTest.Session().GetAlarmData(alarm.Id(), pBuf);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(*buf == KSomeAlarmData, __LINE__);
	//
	TheAlarmTest.Test().Next(_L("Waiting for alarm to expire"));
	// advance clock to 3s before alarm due
	User::SetHomeTime(alarm.NextDueTime() - TTimeIntervalSeconds(3));
	TheAlarmTest.WaitForEvent(EAlarmChangeEventTimerExpired, status, alarmId);
	TheAlarmTest(alarmId == alarm.Id(), __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrInUse, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KSomeAlarmData().Size(), __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDelete(alarm.Id());
	TheAlarmTest(r == KErrNone, __LINE__);
	//

	TheAlarmTest.Session().AlarmAddWithNotification(status, alarm, KSomeAlarmData);
	TheAlarmTest(alarmId = alarm.Id(), __LINE__);
	HBufC8* alarmdata = HBufC8::NewLC(KSomeAlarmData().Size());
	TPtr8 pAlarmdata(alarmdata->Des());
	r = TheAlarmTest.Session().GetAlarmData(alarm.Id(), pAlarmdata);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(*alarmdata == KSomeAlarmData, __LINE__);
	TheAlarmTest.Session().AlarmNotificationCancelAndDequeue(alarmId);
	CleanupStack::PopAndDestroy(alarmdata);
	//
	alarm.Reset();
	alarm.Message() = _L("Alarm with data");
	alarm.NextDueTime().HomeTime();
	alarm.NextDueTime() += TTimeIntervalMinutes(2);
	//
	r =	TheAlarmTest.Session().AlarmAdd(alarm, KSomeAlarmData);
	TheAlarmTest(alarmId = alarm.Id(), __LINE__);
	HBufC8* alarmdata1 = HBufC8::NewLC(KSomeAlarmData().Size());
	TPtr8 pAlarmdata1(alarmdata1->Des());
	r = TheAlarmTest.Session().GetAlarmData(alarm.Id(), pAlarmdata1);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(*alarmdata1 == KSomeAlarmData, __LINE__);
	CleanupStack::PopAndDestroy(alarmdata1);

	TheAlarmTest.Test().Next(_L("make real backup of alarmserver ini file"));
	User::After(1000000); // 1s
	BackupAlarmServerIniFileL();
	User::After(1000000); // 1s

	TheAlarmTest.Test().Next(_L("Delete Alarm"));
	TheAlarmTest(TheAlarmTest.Session().AlarmDelete(alarmId) == KErrNone, __LINE__);

	User::After(1000000); // 1s

	TheAlarmTest.Test().Next(_L("restore alarmserver ini file"));
	RestoreAlarmServerIniFileL();

	User::After(1000000); // 1s

	//
	TheAlarmTest.Test().Next(_L("testing data restoration"));
	//
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrInUse, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KSomeAlarmData().Size(), __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDelete(alarm.Id());
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r==KErrNone, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);

	CleanupStack::PopAndDestroy(buf);
	}


//
//
//


static void doTestUtcAlarmQueueOrderShutdownStartupL()
	{
	TheAlarmTest.Test().Next(_L("* Test alarms set with UTC time order stable over server shutdown/startup"));

#if !defined(_DEBUG) || !defined(__WINSCW__)
	// Can't shutdown Alarm Server on UREL or target builds
	TheAlarmTest.Test().Printf(_L("THIS TEST IS A WINS - DEBUG TEST ONLY\r\n"));
#else
	TheAlarmTest.TestClearStoreL();

	//add alarms
	TheAlarmTest.Test().Next(_L("add 5 alarms"));
	RArray<TAlarmId> alarmIds;
	for	(TInt ii=2; ii<=6; ii++)
		{
		TAlarmId id = addUtcAlarm(ii);
		alarmIds.Append(id);
		}

	TheAlarmTest.Test().Next(_L("shutdown then restart Alarm Server"));
	shutDownAndStartUpServers();

	//get server alarms
	RArray<TAlarmId> serverAlarmIds;
	TheAlarmTest.Session().GetAlarmIdListForCategoryL(KASCliCategoryClock, serverAlarmIds);
	alarmIds.Sort();
	serverAlarmIds.Sort();
	TInt count = alarmIds.Count();
	TheAlarmTest(count == serverAlarmIds.Count(), __LINE__);
	for	(TInt jj=0; jj<count; jj++)
		{
		TheAlarmTest(alarmIds[jj] == serverAlarmIds[jj], __LINE__);
		}
	serverAlarmIds.Close();
	alarmIds.Close();
#endif
	}

static void doTestUtcAlarmDSTShutdownStartupL()
	{
	TheAlarmTest.Test().Next(_L("* Test alarms set stable over server shutdown in DST off/startup in DST on"));
#if !defined(_DEBUG) || !defined(__WINSCW__)
	// Can't shutdown Alarm Server on UREL or target builds
	TheAlarmTest.Test().Printf(_L("THIS TEST IS A WINS - DEBUG TEST ONLY\r\n"));
#else
	_LIT(KDSTON,"DST is on\r\n");
	_LIT(KDSTOFF,"DST is off\r\n");
	_LIT8(KTzEuropeLondon, "Europe/London");

	TheAlarmTest.TestClearStoreL();

	//Set time zone and system time at first
	const TInt currYear = 2007;

 	TTime oldTime;
 	oldTime.HomeTime();

 	RTz tz;
 	User::LeaveIfError(tz.Connect());
	CleanupClosePushL(tz);
	CTzId* oldTz=tz.GetTimeZoneIdL();
	CleanupStack::PushL(oldTz);
    CTzId* zoneId = CTzId::NewL(KTzEuropeLondon);
	CleanupStack::PushL(zoneId);
	tz.SetTimeZoneL(*zoneId);
	tz.SetHomeTime(TDateTime(currYear, EMarch, 24, 00, 58, 0, 0));
	tz.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOn);

    //add alarms
	TheAlarmTest.Test().Next(_L("add 2 alarms"));
	RArray<TAlarmId> alarmIds;
	//Create an alarm at 5.00 am
	TAlarmId id = addAlarm(242);
	alarmIds.Append(id);
	//Create an alarm at 6.00 am
	id = addAlarm(302);
	alarmIds.Append(id);

	//Get the due time of the returned back alarms
	RArray<TTime> dueTimes;
	TASShdAlarm orginalAlarm;
	TheAlarmTest.Session().GetAlarmDetails(alarmIds[0], orginalAlarm);
	dueTimes.Append(orginalAlarm.NextDueTime());
	TheAlarmTest.Session().GetAlarmDetails(alarmIds[1], orginalAlarm);
	dueTimes.Append(orginalAlarm.NextDueTime());

	TTime curTime;
	curTime.HomeTime();
	TDateTime date=curTime.DateTime();
	TBool onDST=tz.IsDaylightSavingOnL(*zoneId);

	TheAlarmTest.Test().Printf(_L("system time is now: hours: %d, minutes: %d, seconds: %d.\r\n"), date.Hour(), date.Minute(), date.Second());
	if (onDST)
		{
		TheAlarmTest.Test().Printf(KDSTON);
		}
	else
		{
		TheAlarmTest.Test().Printf(KDSTOFF);
		}

	TheAlarmTest.Test().Next(_L("shutdown then restart Alarm Server"));
	closeServers();
	User::After(1000000*60*3);  //wait three minutes
	TheAlarmTest.Test().Printf(_L("now restarting server...\r\n"));
	TheAlarmTest.TestStartServers();

	curTime.HomeTime();
	date=curTime.DateTime();
	onDST=tz.IsDaylightSavingOnL(*zoneId);
	CleanupStack::PopAndDestroy(zoneId);

	TheAlarmTest.Test().Printf(_L("system time is now: hours: %d, minutes: %d, seconds: %d.\r\n"), date.Hour(), date.Minute(), date.Second());
	if (onDST)
		{
		TheAlarmTest.Test().Printf(KDSTON);
		}
	else
		{
		TheAlarmTest.Test().Printf(KDSTOFF);
		}

	//get server alarms
	RArray<TAlarmId> serverAlarmIds;
	TheAlarmTest.Session().GetAlarmIdListForCategoryL(KASCliCategoryClock, serverAlarmIds);
	alarmIds.Sort();
	serverAlarmIds.Sort();
	TInt count = alarmIds.Count();
	TheAlarmTest(count == serverAlarmIds.Count(), __LINE__);
	TASShdAlarm alarm;
	for	(TInt jj=0; jj<count; jj++)
		{
		TheAlarmTest(alarmIds[jj] == serverAlarmIds[jj], __LINE__);
		TheAlarmTest.Session().GetAlarmDetails(alarmIds[jj], alarm);
		TheAlarmTest(alarm.NextDueTime() == dueTimes[jj], __LINE__);
		date = alarm.NextDueTime().DateTime();
		TheAlarmTest.Test().Printf(_L("alarm %d due time is : hours: %d, minutes: %d, seconds: %d.\r\n"),
		     alarmIds[jj],date.Hour(), date.Minute(), date.Second());
		}


	serverAlarmIds.Close();
	alarmIds.Close();
	dueTimes.Close();

	tz.SetTimeZoneL(*oldTz);
	CleanupStack::PopAndDestroy(oldTz);
  	tz.SetHomeTime(oldTime);
 	CleanupStack::PopAndDestroy(&tz);
#endif
	}

static void doTestFloatingAlarmDSTShutdownStartupHelsinkiL()
	{
	TheAlarmTest.Test().Next(_L("* Test alarms set stable over server shutdown in DST on/startup in DST off"));
	
#if !defined(_DEBUG) || !defined(__WINSCW__)
	// Can't shutdown Alarm Server on UREL or target builds
	TheAlarmTest.Test().Printf(_L("THIS TEST IS A WINS - DEBUG TEST ONLY\r\n"));
#else
	_LIT8(KTzEuropeHelsinki, "Europe/Helsinki");
	_LIT(KDSTON,"DST is on\r\n");
	_LIT(KDSTOFF,"DST is off\r\n");

	TheAlarmTest.TestClearStoreL();
	
	//Set time zone and system time at first
	const TInt currYear = 2007;
 
 	TTime oldTime; 
 	oldTime.HomeTime();
 
 	RTz tz;
 	User::LeaveIfError(tz.Connect());
	CleanupClosePushL(tz);
	CTzId* oldTz=tz.GetTimeZoneIdL();
	CleanupStack::PushL(oldTz);
    CTzId* zoneId = CTzId::NewL(KTzEuropeHelsinki);
	CleanupStack::PushL(zoneId);
	tz.SetTimeZoneL(*zoneId);
	tz.SetHomeTime(TDateTime(currYear, EOctober, 27, 03, 58, 0, 0));
	tz.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOn);	

    //add alarms
	TheAlarmTest.Test().Next(_L("add alarms"));
	RArray<TAlarmId> alarmIds;
	//Create an alarm at 4.02 am
	TAlarmId id = addAlarm(4);
	alarmIds.Append(id);
		
	id = addAlarm(7);
	alarmIds.Append(id);
	
	id = addAlarm(10);
	alarmIds.Append(id);
	
	//Get the due time of the returned back alarms
	RArray<TTime> dueTimes;
	TASShdAlarm floatingAlarm;
	TheAlarmTest.Session().GetAlarmDetails(alarmIds[0], floatingAlarm);
	dueTimes.Append(floatingAlarm.NextDueTime());
	
	TheAlarmTest.Session().GetAlarmDetails(alarmIds[1], floatingAlarm);
	dueTimes.Append(floatingAlarm.NextDueTime());
	
	TheAlarmTest.Session().GetAlarmDetails(alarmIds[2], floatingAlarm);
	dueTimes.Append(floatingAlarm.NextDueTime());
	
	TTime curTime;
	curTime.HomeTime();
	TDateTime date=curTime.DateTime();
	TBool onDST=tz.IsDaylightSavingOnL(*zoneId);
	
	TheAlarmTest.Test().Printf(_L("system time is now: hours: %d, minutes: %d, seconds: %d.\r\n"), date.Hour(), date.Minute(), date.Second());
	if (onDST)
		{
		TheAlarmTest.Test().Printf(KDSTON);
		}
	else
		{
		TheAlarmTest.Test().Printf(KDSTOFF);
		}
		
	TheAlarmTest.Test().Next(_L("shutdown then restart Alarm Server"));
	closeServers();
	User::After(1000000*60*4);  //wait for 4 mins
	
	tz.SetHomeTime(TDateTime(currYear, EOctober, 27, 04, 02, 0, 0));
	TheAlarmTest.Test().Printf(_L("now restarting server...\r\n"));
	TheAlarmTest.TestStartServers();

	curTime.HomeTime();
	date=curTime.DateTime();
	onDST=tz.IsDaylightSavingOnL(*zoneId);
	CleanupStack::PopAndDestroy(zoneId);

	TheAlarmTest.Test().Printf(_L("system time is now: hours: %d, minutes: %d, seconds: %d.\r\n"), date.Hour(), date.Minute(), date.Second());
	if (onDST)
		{
		TheAlarmTest.Test().Printf(KDSTON);
		}
	else
		{
		TheAlarmTest.Test().Printf(KDSTOFF);
		}
	
	//get server alarms
	RArray<TAlarmId> serverAlarmIds;
	TheAlarmTest.Session().GetAlarmIdListForCategoryL(KASCliCategoryClock, serverAlarmIds);
	alarmIds.Sort();
	serverAlarmIds.Sort();
	TInt count = alarmIds.Count();
	if(count != serverAlarmIds.Count())
		{
		TheAlarmTest.Test().Printf(_L("No alarms found after DST change \r\n"));
		}
	else
		{
		TASShdAlarm alarm;
		for	(TInt jj=0; jj<count; jj++)
			{
			TheAlarmTest(alarmIds[jj] == serverAlarmIds[jj], __LINE__);
			TheAlarmTest.Session().GetAlarmDetails(alarmIds[jj], alarm);
			TheAlarmTest(alarm.NextDueTime() == dueTimes[jj], __LINE__);
			date = alarm.NextDueTime().DateTime();
			TheAlarmTest.Test().Printf(_L("alarm %d due time is : hours: %d, minutes: %d, seconds: %d.\r\n"), 
			     alarmIds[jj],date.Hour(), date.Minute(), date.Second());
			}
			
		}
	serverAlarmIds.Close();
	alarmIds.Close();
	dueTimes.Close();
	
	tz.SetTimeZoneL(*oldTz);
	CleanupStack::PopAndDestroy(oldTz);
  	tz.SetHomeTime(oldTime);
 	CleanupStack::PopAndDestroy(&tz);
#endif
	}


static void doTestUtcAlarmQueueOrderBackupRestoreL()
	{
	TheAlarmTest.Test().Next(_L("* Test alarm set with UTC time order across Backup and Restore"));

	TheAlarmTest.TestClearStoreL();

	//add alarms
	TheAlarmTest.Test().Next(_L("add 5 alarms"));
	RArray<TAlarmId> alarmIds;
	for	(TInt ii=2; ii<=6; ii++)
		{
		TAlarmId id = addUtcAlarm(ii);
		alarmIds.AppendL(id);
		}

	// brief pause before backup (1s)
	User::After(1000000);

	TheAlarmTest.Test().Next(_L("make real backup of alarmserver ini file"));
	TRAPD(err, BackupAlarmServerIniFileL());
	TheAlarmTest(err==KErrNone, __LINE__);

	// delete alarms
	TheAlarmTest.Test().Next(_L("delete alarms"));
	TInt jj;
	for (jj=0; jj<5; jj++)
		{
		TheAlarmTest(TheAlarmTest.Session().AlarmDelete(alarmIds[jj]) == KErrNone, __LINE__);
		}

	// pause before restore (10s)
	User::After(10000000);
	TheAlarmTest.Test().Next(_L("restore alarmserver ini file"));
	TRAP(err, RestoreAlarmServerIniFileL());
	TheAlarmTest(err==KErrNone, __LINE__);

	TheAlarmTest.Test().Next(_L("check restored alarms"));
	//get server alarms
	RArray<TAlarmId> serverAlarmIds;
	TheAlarmTest.Session().GetAlarmIdListForCategoryL(KASCliCategoryClock, serverAlarmIds);
	alarmIds.Sort();
	serverAlarmIds.Sort();
	TInt count = alarmIds.Count();
	TInt serverCount = serverAlarmIds.Count();
	TheAlarmTest.Test().Printf(_L("[Added Alarms count = %i, server Count = %i]\r\n"), count, serverCount);
	TheAlarmTest(count == serverCount, __LINE__);
	for	(jj=0; jj<count; jj++)
		{
		TheAlarmTest(alarmIds[jj] == serverAlarmIds[jj], __LINE__);
		}
	serverAlarmIds.Close();
	alarmIds.Close();
	}


static void doTestUtcAlarmQueueBackupRestoreL()
	{
	TheAlarmTest.Test().Next(_L("* Test correct alarms set with UTC time in queue after backup and restore"));

	TheAlarmTest.TestClearStoreL();

	// add an alarm to expire in 2 minutes
	TAlarmId id2 = addUtcAlarm(2);
	// add an alarm to expire in 3 minutes
	TAlarmId id3 = addUtcAlarm(3);

	// brief pause before backup (.5s)
	User::After(500000);

	TheAlarmTest.Test().Next(_L("make real backup of alarmserver ini file"));
	TRAPD(err, BackupAlarmServerIniFileL());
	TheAlarmTest(err==KErrNone, __LINE__);

	// delete alarms
	TheAlarmTest.Test().Next(_L("delete alarms"));
	TheAlarmTest(TheAlarmTest.Session().AlarmDelete(id2) == KErrNone, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().AlarmDelete(id3) == KErrNone, __LINE__);

	// add alarms
	TheAlarmTest.Test().Next(_L("add alarms"));
	// add alarms to expire now
	TAlarmId id0 = addUtcAlarm(0);
	TAlarmId id1 = addUtcAlarm(1);

	// first alarm (id0) should be notifying
	TASShdAlarm alarm;
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id0, alarm) == KErrNone, __LINE__);
	TheAlarmTest(alarm.State() == EAlarmStateNotifying, __LINE__);


	// pause briefly before restore (2s)
	User::After(2000000);
	TheAlarmTest.Test().Next(_L("restore alarmserver ini file"));
	TRAP(err, RestoreAlarmServerIniFileL());
	TheAlarmTest(err==KErrNone, __LINE__);

	TheAlarmTest.Test().Next(_L("check correct alarms after restore"));

	// alarms should now be gone
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id0, alarm) == KErrNotFound, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id1, alarm) == KErrNotFound, __LINE__);

	// alarms should be restored
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id2, alarm) == KErrNone, __LINE__);
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id3, alarm) == KErrNone, __LINE__);
	}


static void doTestUtcAlarmAddDuringBackupL()
	{
	TheAlarmTest.Test().Next(_L("* Test alarm changes during backup"));

	TheAlarmTest.TestClearStoreL();

	// Attempt to add an alarm while the backup server is backing up the alarms
	CStreamStore* store = NULL;
	TRAPD(err, store = SimulateBackupServerBackup_StartL());
	TheAlarmTest(err==KErrNone, __LINE__);

	CleanupStack::PushL(store);

	TheAlarmTest.Test().Next(_L("adding alarm should work"));

	TASShdAlarm alm;
	TTime time;
	time.UniversalTime();
	alm.SetUtcNextDueTime(time);
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alm) == KErrNone, __LINE__);

	TheAlarmTest.Test().Next(_L("changing time forces Alarm Store class to queue Externalize"));
	// Locale change (time) x2 (INC038240 regression)
	// Exercise CASSrvAlarmStore queue an Externalize
	time.UniversalTime();
	time += TTimeIntervalSeconds(10);
	User::SetUTCTime(time);
	User::After(1000000);
	time -= TTimeIntervalSeconds(10);
	User::SetUTCTime(time);
	User::After(1000000);

	// release file lock -
	CleanupStack::PopAndDestroy(store); // store
	// backup complete
	TRAP(err, SimulateBackupServerBackup_CompleteL());
	TheAlarmTest(err==KErrNone, __LINE__);

	// wait for AlarmServer to catch-up
	User::After(1000000);


	TheAlarmTest.Test().Next(_L("check we can now add alarm"));

	// re-try alarm add after backup complete - should be successful
	TAlarmId id = addUtcAlarm(0);
	TheAlarmTest(id != KNullAlarmId, __LINE__);

	// make Alarm Server re-Internalize
	User::After(1000000);
	TheAlarmTest.Test().Next(_L("now simulating restore..."));
	TRAP(err, SimulateBackupServerRestorationL());
	TheAlarmTest(err==KErrNone, __LINE__);
	TASShdAlarm alarm;


	// check absence of alarm
	TheAlarmTest.Test().Next(_L("check alarm has gone"));
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id, alarm) == KErrNotFound, __LINE__);

	}


static void doTestUtcAlarmAddDuringRestoreL()
	{
	// variables used to check Alarm Server notifications of Restore
	TAlarmId alarmId;
	TRequestStatus status;

	TheAlarmTest.Test().Next(_L("* Test alarm changes during restore"));

	TheAlarmTest.TestClearStoreL();

	// Attempt to add an alarm while the backup server is restoring up the alarms
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	CStreamStore* store = NULL;
	TRAPD(err, store = SimulateBackupServerRestore_StartL());
	CleanupStack::PushL(store);
	TheAlarmTest(err==KErrNone, __LINE__);

	// check Restore Start notification received
	TheAlarmTest.Test().Next(_L("check for Alarm Restore Start notification"));
	User::After(1000000);
	TheAlarmTest(status == EAlarmChangeEventRestoreStarted, __LINE__);
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);

	TheAlarmTest(alarmId==KNullAlarmId, __LINE__);

	TheAlarmTest.Test().Next(_L("adding alarm should fail"));

	TASShdAlarm alm;
	TTime time;
	time.UniversalTime();
	alm.SetUtcNextDueTime(time);
	TheAlarmTest(TheAlarmTest.Session().AlarmAdd(alm) == KErrLocked, __LINE__);

	TheAlarmTest.Test().Next(_L("changing time forces Alarm Store class to queue Externalize"));
	// Locale change (time) x2 (INC038240 regression)
	// Exercise CASSrvAlarmStore

	time.UniversalTime();
	time += TTimeIntervalSeconds(10);
	User::SetUTCTime(time);
	User::After(1000000);
	time -= TTimeIntervalSeconds(10);
	User::SetUTCTime(time);
	User::After(1000000);

	CleanupStack::PopAndDestroy(store);
	// restore complete
	TRAP(err, SimulateBackupServerRestore_CompleteL());
	TheAlarmTest(err==KErrNone, __LINE__);

	// check Restore Complete notification received
	TheAlarmTest.Test().Next(_L("check for Alarm Restore Complete notification"));
	TheAlarmTest.TestEventBuffer(EAlarmChangeEventRestoreCompleted, status, alarmId);
	TheAlarmTest(alarmId==KNullAlarmId, __LINE__);


	// wait for AlarmServer to catch-up
	User::After(1000000);

	TheAlarmTest.Test().Next(_L("check we can now add alarm"));

	// re-try alarm add after restore complete - should be successful
	TAlarmId id = addUtcAlarm(0);
	TheAlarmTest(id != KNullAlarmId, __LINE__);

	// wait for Alarm Server to externalize
	User::After(3000000);
	}





static void doTestNotifyingUtcAlarmShutdownStartupL()
	{
	TheAlarmTest.Test().Next(_L("* Test Notifying alarm across shutdown / startup"));

	TheAlarmTest.TestClearStoreL();

	TheAlarmTest.Test().Next(_L("add an alarm"));
	// add an alarm to expire in 15 minutes
	TAlarmId id = addUtcAlarm(15);

	// get alarm expiry time
	TASShdAlarm alarm;
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id, alarm) == KErrNone, __LINE__);
	TTime time = alarm.NextDueTime();

#if !defined(_DEBUG) || !defined(__WINSCW__)
	// Can't shutdown Alarm Server on UREL or target builds
	TheAlarmTest.Test().Printf(_L("THIS TEST IS A WINS - DEBUG TEST ONLY\r\n"));

	TheAlarmTest.Test().Printf(_L("Waiting for Alarm to expire\r\n"));
#if defined(_DEBUG)
	// UREL Alternative: wait until after alarm due time
	User::SetUTCTime(time - TTimeIntervalSeconds(3));
	User::After(KTenSeconds);
#endif
#else
	// UDEB:
	// shut down the server
	closeServers();

	// re-start server after alarm expires
	User::SetUTCTime(time - TTimeIntervalSeconds(3));
	User::After(KTenSeconds);
	TheAlarmTest.Test().Next(_L("now restarting server...\r\n"));
	TheAlarmTest.TestStartServers();
	User::After(1000000);
#endif
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id, alarm) == KErrNone, __LINE__);

#if	defined(_DEBUG)
	//check status of alarm
	// Internalise on startup has a 59s window for recent alarms
	TheAlarmTest.Test().Next(_L("check that alarm is now set and notifying"));
	TheAlarmTest(alarm.State() == EAlarmStateNotifying, __LINE__);
#else
	// Test only possible if in _DEBUG build.
	TheAlarmTest.Test().Printf(_L("THIS TEST IS A DEBUG TEST ONLY\r\n"));
#endif
	TheAlarmTest(TheAlarmTest.Session().AlarmDelete(id) == KErrNone, __LINE__);
	}


static void doTestNotifyingUtcAlarmBackupRestoreL()
	{
	TheAlarmTest.Test().Next(_L("* Test Notifying alarm removed by backup / restore"));

	TheAlarmTest.TestClearStoreL();

	TheAlarmTest.Test().Next(_L("add an alarm"));
	// add an alarm to expire in 15 minutes
	TAlarmId id = addUtcAlarm(15);

	TheAlarmTest.Test().Next(_L("make real backup of alarmserver ini file"));
	TRAPD(err, BackupAlarmServerIniFileL());
	TheAlarmTest(err==KErrNone, __LINE__);

	// get alarm expiry time
	TASShdAlarm alarm;
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id, alarm) == KErrNone, __LINE__);
	TTime time = alarm.NextDueTime();
	TheAlarmTest.Test().Printf(_L("Waiting for Alarm to expire\r\n"));
	TheAlarmTest.Test().Printf(_L("Alarm set to expire at: hours: %d, minutes: %d, seconds: %d.\r\n"), time.DateTime().Hour(), time.DateTime().Minute(), time.DateTime().Second());


	// wait until after alarm due time
	User::SetUTCTime(time - TTimeIntervalSeconds(3));
	TTime now;
	now.UniversalTime();
	TheAlarmTest.Test().Printf(_L("System UTC time is now: hours: %d, minutes: %d, seconds: %d.\r\n"), now.DateTime().Hour(), now.DateTime().Minute(), now.DateTime().Second());
	TheAlarmTest.Test().Printf(_L("Wait for approximately 10 seconds for alarm to expire...\r\n"));
	User::After(KTenSeconds);
	now.UniversalTime();
	TheAlarmTest.Test().Printf(_L("System UTC time is now: hours: %d, minutes: %d, seconds: %d.\r\n"), now.DateTime().Hour(), now.DateTime().Minute(), now.DateTime().Second());

	// Internalise on startup has a 59s window for recent alarms
	TheAlarmTest.Test().Next(_L("check that alarm is now set and notifying"));

	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id, alarm) == KErrNone, __LINE__);
	TheAlarmTest.Test().Printf(_L("Alarm state: %d.\r\n"), alarm.State());
	TheAlarmTest(alarm.State() == EAlarmStateNotifying, __LINE__);

	TheAlarmTest.Test().Next(_L("restore alarmserver ini file"));
	TRAP(err, RestoreAlarmServerIniFileL());
	TheAlarmTest(err==KErrNone, __LINE__);

	// allow Alarm server to notice file lock has been released
	User::After(1000000);

	// Internalise after Restore has a 0s window for recent alarms
	// check absence of alarm
	TheAlarmTest.Test().Next(_L("check alarm has gone\r\n"));
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id, alarm) == KErrNotFound, __LINE__);
	}


static void doTestSessionUtcAlarmShutdownRestartL()
	{
	TheAlarmTest.Test().Next(_L("* Test that Session alarm is removed by shutdown"));

#if !defined(_DEBUG) || !defined(__WINSCW__)
	// Can't shutdown Alarm Server on UREL or target builds
	TheAlarmTest.Test().Printf(_L("THIS TEST IS A WINS - DEBUG TEST ONLY\r\n"));
#else
	TheAlarmTest.TestClearStoreL();

	// Set the session alarm
	TheAlarmTest.Test().Next(_L("set session alarm in 2 minutes"));
	TASShdAlarm alarm;
	alarm.Category()=(KASCliCategoryClock);
	alarm.RepeatDefinition()=(EAlarmRepeatDefintionRepeatOnce);
	TTime time;
	time.UniversalTime();
	time += TTimeIntervalMinutes(2);
	alarm.SetUtcNextDueTime(time);
	// session specific alarm (has a TRequestStatus to complete)
	alarm.Characteristics().Set(EAlarmCharacteristicsSessionSpecific);
	TRequestStatus status;
	TheAlarmTest.Session().AlarmAddWithNotification(status, alarm);
	const TAlarmId id = alarm.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);

	// status should be pending
	TheAlarmTest.Test().Next(_L("checking request is pending"));
	TheAlarmTest(status == KRequestPending, __LINE__);

	TheAlarmTest.Test().Next(_L("Shutdown Alarm Server"));
	closeServers();

	User::After(10000000);

	// check absence of alarm
	TheAlarmTest.Test().Next(_L("check alarm notification has been aborted"));

	TheAlarmTest(status != KRequestPending, __LINE__);

	// Get the abort
	// (Probably KErrDied though KErrServerTerminated should be okay too)
	User::WaitForRequest(status);
	TInt stat = status.Int();
	TheAlarmTest((stat == KErrDied) || (stat == KErrServerTerminated), __LINE__);

	// now restart Alarm Server
	TheAlarmTest.Test().Next(_L("restart Alarm Server"));
	TheAlarmTest.TestStartServers();

	// check presence of alarm
	TheAlarmTest.Test().Next(_L("check shutdown deleted alarm"));
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id, alarm) == KErrNotFound, __LINE__);
#endif
	}


static void doTestSessionUtcAlarmBackupRestoreL()
	{
	TheAlarmTest.Test().Next(_L("* Test that Session alarm is orphaned by restore"));

	TheAlarmTest.TestClearStoreL();

	// Set the session alarm
	TheAlarmTest.Test().Next(_L("set session alarm in 20 minutes"));
	TASShdAlarm alarmWrite;
	alarmWrite.Category()=(KASCliCategoryClock);
	alarmWrite.RepeatDefinition()=EAlarmRepeatDefintionRepeatOnce;
	TTime time;
	time.UniversalTime();
	time += TTimeIntervalMinutes(20);
	alarmWrite.SetUtcNextDueTime(time);
	// session specific alarm (has a TRequestStatus to complete)
	alarmWrite.Characteristics().Set(EAlarmCharacteristicsSessionSpecific);
	TRequestStatus status;
	TheAlarmTest.Session().AlarmAddWithNotification(status, alarmWrite);
	// check that alarm was set okay
	const TAlarmId id = alarmWrite.Id();
	TheAlarmTest(id != KNullAlarmId, __LINE__);

	// status should be pending
	TheAlarmTest.Test().Next(_L("checking request is pending"));
	TheAlarmTest(status == KRequestPending, __LINE__);

	// remember the alarm time
	TTime alarmTime = alarmWrite.NextDueTime();

	// make the Alarm server re-internlize ....
	TheAlarmTest.Test().Next(_L("now simulating restore..."));
	TRAPD(err, SimulateBackupServerRestorationL());
	TheAlarmTest(err==KErrNone, __LINE__);


	// allow Alarm server to Internalize...
	User::After(1000000);	// 1s


	// check old alarm queue has been cleared
	TheAlarmTest.Test().Next(_L("check alarm notification has been aborted"));
	TheAlarmTest(status != KRequestPending, __LINE__);

	// Get the abort result
	User::WaitForRequest(status);
	TheAlarmTest(status.Int() == KErrCancel, __LINE__);

	// check presence of alarm
	TASShdAlarm alarmRead;
	TheAlarmTest.Test().Next(_L("check alarm is no longer a session alarm"));
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id, alarmRead) == KErrNone, __LINE__);

	TheAlarmTest(!alarmRead.HasOwningSession(), __LINE__);
	TheAlarmTest(alarmRead.HasBecomeOrphaned(), __LINE__);

	// advance clock, wait for alarm
	User::SetUTCTime(alarmWrite.NextDueTime() - TTimeIntervalSeconds(3));
	TheAlarmTest.Test().Next(_L("waiting for alarm to notify"));
	// wait until after alarm due time
	User::After(KTenSeconds);

	// alarm should be notifying
	// if Alarm Server server tries to access the TRequestStatus
	// there will be an Access Violation
	TheAlarmTest(TheAlarmTest.Session().GetAlarmDetails(id, alarmRead) == KErrNone, __LINE__);

	//TheAlarmTest(alarm.State() == EAlarmStateNotifying, __LINE__);
	const TAlarmState alarmState = alarmRead.State();
	TheAlarmTest(alarmState == EAlarmStateNotifying, __LINE__);
	}


static void doTestRestoreValidUtcAlarmDataL()
	{
	TheAlarmTest.Test().Next(_L("Data API test"));
	TheAlarmTest.TestClearStoreL();
	TInt r;
	TAlarmId alarmId;
	TASShdAlarm alarm;
	TRequestStatus status;
	//
	alarm.Message()=(_L("Alarm with data"));
	TTime time;
	time.UniversalTime();
	time += TTimeIntervalMinutes(2);
	alarm.SetUtcNextDueTime(time);
	//
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r==KErrNone, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmChangeEventAlarmData, __LINE__);
	TheAlarmTest(alarmId = alarm.Id(), __LINE__);
	User::WaitForRequest(status);
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrInUse, __LINE__);
	TheAlarmTest(status == KRequestPending, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KSomeAlarmData().Size());
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDelete(alarm.Id());
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest.TestEventBuffer(EAlarmChangeEventAlarmData, status, alarmId);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	time.UniversalTime();
	time += TTimeIntervalMinutes(2);
	alarm.SetUtcNextDueTime(time);
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r==KErrNone, __LINE__);
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmChangeEventAlarmData, __LINE__);
	TheAlarmTest(alarmId = alarm.Id(), __LINE__);
	User::WaitForRequest(status);
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest.TestEventBuffer(EAlarmChangeEventAlarmData, status, alarmId);
	//
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	TheAlarmTest.WaitForNotificationBufferToBeEmptied(status, alarmId);
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(status == EAlarmChangeEventAlarmData, __LINE__);
	TheAlarmTest(alarmId = alarm.Id(), __LINE__);
	User::WaitForRequest(status);
	//
	HBufC8* buf = HBufC8::NewLC(KSomeAlarmData().Size());
	TPtr8 pBuf(buf->Des());
	r = TheAlarmTest.Session().GetAlarmData(alarm.Id(), pBuf);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(*buf == KSomeAlarmData, __LINE__);
	//
	TheAlarmTest.Test().Next(_L("Waiting for alarm to expire"));
	// advance clock to 3s before alarm due
	User::SetUTCTime(alarm.NextDueTime() - TTimeIntervalSeconds(3));
	TheAlarmTest.WaitForEvent(EAlarmChangeEventTimerExpired, status, alarmId);
	TheAlarmTest(alarmId == alarm.Id(), __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrInUse, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KSomeAlarmData().Size(), __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDelete(alarm.Id());
	TheAlarmTest(r == KErrNone, __LINE__);
	//

	TheAlarmTest.Session().AlarmAddWithNotification(status, alarm, KSomeAlarmData);
	TheAlarmTest(alarmId = alarm.Id(), __LINE__);
	HBufC8* alarmdata = HBufC8::NewLC(KSomeAlarmData().Size());
	TPtr8 pAlarmdata(alarmdata->Des());
	r = TheAlarmTest.Session().GetAlarmData(alarm.Id(), pAlarmdata);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(*alarmdata == KSomeAlarmData, __LINE__);
	TheAlarmTest.Session().AlarmNotificationCancelAndDequeue(alarmId);
	CleanupStack::PopAndDestroy(alarmdata);
	//
	alarm.Reset();
	alarm.Message()=(_L("Alarm with data"));
	time.UniversalTime();
	time += TTimeIntervalMinutes(2);
	alarm.SetUtcNextDueTime(time);
	//
	r =	TheAlarmTest.Session().AlarmAdd(alarm, KSomeAlarmData);
	TheAlarmTest(alarmId = alarm.Id(), __LINE__);
	HBufC8* alarmdata1 = HBufC8::NewLC(KSomeAlarmData().Size());
	TPtr8 pAlarmdata1(alarmdata1->Des());
	r = TheAlarmTest.Session().GetAlarmData(alarm.Id(), pAlarmdata1);
	TheAlarmTest(r == KErrNone, __LINE__);
	TheAlarmTest(*alarmdata1 == KSomeAlarmData, __LINE__);
	CleanupStack::PopAndDestroy(alarmdata1);

	TheAlarmTest.Test().Next(_L("make real backup of alarmserver ini file"));
	User::After(1000000); // 1s
	BackupAlarmServerIniFileL();
	User::After(1000000); // 1s

	TheAlarmTest.Test().Next(_L("Delete Alarm"));
	TheAlarmTest(TheAlarmTest.Session().AlarmDelete(alarmId) == KErrNone, __LINE__);

	User::After(1000000); // 1s

	TheAlarmTest.Test().Next(_L("restore alarmserver ini file"));
	RestoreAlarmServerIniFileL();

	User::After(1000000); // 1s

	//
	TheAlarmTest.Test().Next(_L("testing data restoration"));
	//
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrInUse, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KSomeAlarmData().Size(), __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDelete(alarm.Id());
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmAdd(alarm);
	TheAlarmTest(r==KErrNone, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataAttachL(alarm.Id(), KSomeAlarmData);
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataDetach(alarm.Id());
	TheAlarmTest(r == KErrNone, __LINE__);
	//
	r = TheAlarmTest.Session().AlarmDataSize(alarm.Id());
	TheAlarmTest(r == KErrNotFound, __LINE__);

	CleanupStack::PopAndDestroy(buf);
	}


//

static void doTestsL()
	{
	// Initialise the drive list to empty and then get drive list data from
	// File Server.
	DriveList.FillZ();
	User::LeaveIfError(FsSession.DriveList(DriveList));

	BackupClient = conn::CSBEClient::NewL();
	CleanupStack::PushL(BackupClient);

	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	TheAlarmTest.TestStartServers();
	TheAlarmTest(TheAlarmTest.Session().Connect() == KErrNone, __LINE__);
	TheAlarmTest.TestClearStoreL();

	User::After(100000);

	// delete AlarmServer.ini file
	FsSession.Delete(KAlarmServerBackupFileFullPath);
	User::After(1000000);


	// 1.
	// Baseline for Internalize behaviour
	// [add alarms, shutdown, re-start alarm server, check alarms still queued]
	doTestAlarmQueueOrderShutdownStartupL();

	// 2.
	// Backup, Restore Internalize behaviour
	// [add alarms, pretend backup/restore, check alarms queued again]
	doTestAlarmQueueOrderBackupRestoreL();


	// 3.
	// Correct Alarms are Backed up and Restored
	// [add alarms, backup, delete alarms, add different alarms, restore, check queued alarms]
	doTestAlarmQueueBackupRestoreL();

	// 4.
	// Alarm Sound Intervals are Backed up and Restored.
	doTestAlarmSoundIntervalsBackupRestoreL();

	/* 5.
	 * Should be able to add alarm or change Locale during Backup
	 */
	doTestAlarmAddDuringBackupL();

	/* 6.
	 * Testing Restore behaviour
	 *    a) Should not be able to add alarm during Restore
	 *    b) Should get Notifications at Start & End of Restore
	 *    c) Locale (time) changes during Restore should be processed,
	 *       (but Externalize happens when Restore ends).
	 */
	doTestAlarmAddDuringRestoreL();

	/* 7.
	 * Testing Failed Restore behaviour
	 *    a) Should get Notifications at Start & End of Restore
	 *    b) Alarm Queue should not change
	 *    c) Alarm Server should perform a new Externalize
	 */
	/* 8, 9.
	 * Handling of Notifying alarm across Shutdown/Startup and Backup/Restore
	 */
	doTestNotifyingAlarmShutdownStartupL();
	doTestNotifyingAlarmBackupRestoreL();

	/* 10, 11.
	 * Handling of Session alarm
	 */
	doTestSessionAlarmShutdownRestartL();
	doTestSessionAlarmBackupRestoreL();

	/* 12.
	 * Test restore of alarm with attached data
	 * [create alarm with data, backup, delete alarm, restore, check alarm & data]
	 * (Based on test case in TAlarmData.)
	 */
	doTestRestoreValidAlarmDataL();

	// UTC Tests

	// 13.
	// Baseline for Internalize behaviour
	// [add alarms, shutdown, re-start alarm server, check alarms still queued]
	doTestUtcAlarmQueueOrderShutdownStartupL();

	// 14.
	// Backup, Restore Internalize behaviour
	// [add utc alarms, pretend backup/restore, check alarms queued again]
	doTestUtcAlarmQueueOrderBackupRestoreL();


	// 15.
	// Correct UTC Alarms are Backed up and Restored
	// [add utc alarms, backup, delete alarms, add different alarms, restore, check queued alarms]
	doTestUtcAlarmQueueBackupRestoreL();

	/* 16.
	 * Should be able to add utc alarm or change Locale during Backup
	 */
	doTestUtcAlarmAddDuringBackupL();

	/* 17.
	 * Testing Restore behaviour
	 *    a) Should not be able to add utc alarm during Restore
	 *    b) Should get Notifications at Start & End of Restore
	 *    c) Locale (time) changes during Restore should be processed,
	 *       (but Externalize happens when Restore ends).
	 */
	doTestUtcAlarmAddDuringRestoreL();

	/* 18.
	 * Testing Failed Restore behaviour
	 *    a) Should get Notifications at Start & End of Restore
	 *    b) Alarm Queue should not change
	 *    c) Alarm Server should perform a new Externalize
	 */
	/* 19, 20.
	 * Handling of Notifying UTC alarm across Shutdown/Startup and Backup/Restore
	 */
	doTestNotifyingUtcAlarmShutdownStartupL();
	doTestNotifyingUtcAlarmBackupRestoreL();

	/* 21, 22.
	 * Handling of Session UTC alarm
	 */
	doTestSessionUtcAlarmShutdownRestartL();
	doTestSessionUtcAlarmBackupRestoreL();

	/* 23.
	 * Test restore of alarm with attached data
	 * [create utc alarm with data, backup, delete alarm, restore, check alarm & data]
	 * (Based on test case in TAlarmData.)
	 */
	doTestRestoreValidUtcAlarmDataL();

	/* 24.
	 * Baseline for Internalize behaviour
	 * [add alarms, shutdown, re-start alarm server, check local due time is unchanged]
	 */
	doTestUtcAlarmDSTShutdownStartupL();
	doTestFloatingAlarmDSTShutdownStartupHelsinkiL();

	// cleanup
	CleanupStack::PopAndDestroy(scheduler);
	CleanupStack::PopAndDestroy(BackupClient);
	TheAlarmTest.TestClearStoreL();
	}

//

/**

@SYMTestCaseID PIM-TBACKUPALARMS-0001

*/

GLDEF_C TInt E32Main()
    {
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if	(!cleanup)
		return KErrNoMemory;

	TheAlarmTest.Test().Title();
	TheAlarmTest.Test().Start(_L("@SYMTestCaseID:PIM-TBACKUPALARMS-0001 Backup Alarms"));
	// initialise globals
	FsSession.Connect();

	TheAlarmTest.Test().Next(_L("SECURE_DATA mode"));

	// actually do tests
	TRAPD(error, doTestsL());

	// Cleanup files
	// delete test file
	FsSession.Delete(KAlarmServerTestBackupFile);
	// delete AlarmServer.ini file
	FsSession.Delete(KAlarmServerBackupFileFullPath);
	FsSession.Close();

	// did any test Leave?
	TheAlarmTest(error == KErrNone, __LINE__);

	//
	TheAlarmTest.Test().End();
	TheAlarmTest.Test().Close();
	ASTstAlarmTest::Close();
	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
    }
