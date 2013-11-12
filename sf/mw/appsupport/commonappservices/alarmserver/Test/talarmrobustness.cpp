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
#include <s32mem.h>

// User includes
#include <asshdalarm.h>
#include <asclisession.h>
#include <consolealarmalertservermain.h>
#include "testserver.h"
#include "ASTstAlarmTest.h"

// Type definitions
typedef void (*ThreadTestL)(void);

// Constants
const TUid KServerUid2={0x1000008D};
const TUid KServerUid3={0x101F502A};
const TUidType serverUid(KNullUid,KServerUid2,KServerUid3);

// Literal constants
_LIT(KTestTitle, "TAlarmRobustness");
_LIT(KRobustTestCreateServer, "Creating alarm server");

#ifdef _DEBUG
#ifdef __WINS__
_LIT(KRobustTestCloseServer, "Close alarm server");
#endif // #ifdef __WINS__
#endif // #ifdef _DEBUG

#ifdef _DEBUG
#ifdef __WINS__
_LIT(KTestIniFile, "c:\\private\\101f5027\\AlarmServer.ini");
_LIT(KAlarmIniFileDir, "c:\\private\\101f5027\\");
#endif // #ifdef __WINS__
#endif // #ifdef _DEBUG

// Globals
//static RTest TheAlarmTest(KTestTitle);
static RASCliSession TheAlarmServerSession;

const TInt KAlarmObjectBuffer = 512;

#ifdef _DEBUG
#ifdef __WINS__

const TInt KAlarmTestWaitForFileWrite = 2000000;


static void TestClearStoreL()
	{
	// Delete all alarms
	RArray<TAlarmCategory> categories;
	TheAlarmServerSession.GetAvailableCategoryListL(categories);
	const TInt KAlarmCategoriesCount = categories.Count();
	for(TInt i=0; i<KAlarmCategoriesCount; i++)
		{
		const TAlarmCategory category = categories[i];
		TheAlarmTest(TheAlarmServerSession.AlarmDeleteAllByCategory(category, EFalse) == KErrNone, __LINE__);
		}
	categories.Close();
	}

static void DeleteIniFileL()
	{
   	RPIMTestServer serv;
    User::LeaveIfError(serv.Connect());
    CleanupClosePushL(serv);
    serv.DeleteFileL(KTestIniFile);
	CleanupStack::PopAndDestroy(&serv);
	}

#endif // #ifdef __WINS__
#endif // #ifdef _DEBUG

//*************************************************************************************
void InitializeAlarm(TASShdAlarm& theAlarm)
	{
	theAlarm.Category() = KASCliCategoryClock;
	theAlarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatDaily;
	_LIT(KAlarmTestInit, "This is an alarm message - alarm 1");
	theAlarm.Message() = KAlarmTestInit;
	theAlarm.NextDueTime().HomeTime();
	theAlarm.NextDueTime() += TTimeIntervalMinutes(1);
	_LIT(KAlarmTestSoundName, "Arf Arf Woop");
	theAlarm.SoundName() = KAlarmTestSoundName;
	}


#ifdef _DEBUG
#ifdef __WINS__
static void CreateSingleTestAlarmL(TAlarmId& testID)
	{
	_LIT(KAlarmTestAddDisabled, "Adding disabled alarm");
	TheAlarmTest.Next(KAlarmTestAddDisabled);
	//
	TASShdAlarm alarm;
	InitializeAlarm(alarm);

	//
	TheAlarmTest(TheAlarmServerSession.AlarmAdd(alarm) == KErrNone, __LINE__);
	const TAlarmId id = alarm.Id();
	testID = id;
	TheAlarmTest(id != KNullAlarmId, __LINE__);

	//
	_LIT(KAlarmTestGetDetails, "Get alarm details");
	TheAlarmTest.Next(KAlarmTestGetDetails);
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone, __LINE__);

	//
	_LIT(KAlarmTestEnableAlarm, "Enabling a disabled alarm");
	TheAlarmTest.Next(KAlarmTestEnableAlarm);
	TheAlarmTest(TheAlarmServerSession.SetAlarmStatus(id, EAlarmStatusEnabled) == KErrNone, __LINE__);
	}


static void RunNoConfigFileTestL()
	{
	_LIT(KRobustTestNoConfig, "Run No Config File Test");
	TheAlarmTest.Next(KRobustTestNoConfig);

	// 1) Delete AlarmServer.ini file
	DeleteIniFileL();

	// 2) Connect to the alarm server.
	TheAlarmTest.Next(KRobustTestCreateServer);
	User::LeaveIfError(TheAlarmServerSession.Connect());

	// 3) Add alarm.
	TAlarmId testID = KNullAlarmId;
	CleanupClosePushL(TheAlarmServerSession);
	CreateSingleTestAlarmL(testID);

	// 4) Shutdown alarm server.
	TheAlarmTest.Next(KRobustTestCloseServer);
	TheAlarmServerSession.__DbgShutDownServer();
	CleanupStack::PopAndDestroy(&TheAlarmServerSession);

	// 5) Restart alarm server.
	TheAlarmTest.Next(KRobustTestCreateServer);
	User::LeaveIfError(TheAlarmServerSession.Connect());

	// 6) Check alarm has persisted.
	TASShdAlarm testAlarm;
	TInt error = TheAlarmServerSession.GetAlarmDetails(testID, testAlarm);
	TheAlarmTest(error == KErrNone, __LINE__);
	}


static void RunZeroLengthConfigFileTestL()
	{
	_LIT(KRobustTestZeroLengthConfigFile, "Run Zero Length Config File Test");
	TheAlarmTest.Next(KRobustTestZeroLengthConfigFile);

	// 1) Create an alarm to make sure there is a config file.
	TAlarmId testID = KNullAlarmId;
	CreateSingleTestAlarmL(testID);
	User::After(KAlarmTestWaitForFileWrite);

	// 2) Zero length the AlarmServer.ini file
   	TestClearStoreL();

	// 3) Connect to the alarm server.
	TheAlarmTest.Next(KRobustTestCreateServer);
	User::LeaveIfError(TheAlarmServerSession.Connect());

	// 4) Add alarm.
	CleanupClosePushL(TheAlarmServerSession);
	CreateSingleTestAlarmL(testID);

	// 5) Shutdown alarm server.
	TheAlarmTest.Next(KRobustTestCloseServer);
	TheAlarmServerSession.__DbgShutDownServer();
	CleanupStack::PopAndDestroy(&TheAlarmServerSession);

	// 6) Restart alarm server.
	TheAlarmTest.Next(KRobustTestCreateServer);
	User::LeaveIfError(TheAlarmServerSession.Connect());

	// 7) Check alarm has persisted.
	TASShdAlarm testAlarm;
	TInt error = TheAlarmServerSession.GetAlarmDetails(testID, testAlarm);
	TheAlarmTest(error == KErrNone, __LINE__);
	}


static void CorruptErrorConditionTestL()
	{
	_LIT(KRobustTestCorruptErrorCondition, "Corrupt Error Condition Test");
	TheAlarmTest.Next(KRobustTestCorruptErrorCondition);

	// 1) Create file session to call SetErrorCondition.
	TAlarmId testID = KNullAlarmId;
 	TInt error  = KErrCorrupt;
	RFs testFile;
	User::LeaveIfError(testFile.Connect());
	CleanupClosePushL(testFile);

	// 2) Create a few alarms to make sure we have populated config file.
	TheAlarmTest.Next(KRobustTestCreateServer);
	User::LeaveIfError(TheAlarmServerSession.Connect());
	CleanupClosePushL(TheAlarmServerSession);
	const TInt KNumberOfTestAlarms = 3;
	for( TInt i=0;i<KNumberOfTestAlarms;i++)
		{
		CreateSingleTestAlarmL(testID);
		}
	User::After(KAlarmTestWaitForFileWrite);

	// 3) Shut down server so we can re-connect and do internalization.
	TheAlarmServerSession.__DbgShutDownServer();
	CleanupStack::PopAndDestroy(&TheAlarmServerSession);

	// 4) Test loop - This will cycle through file session internalization
	//	process, checking to see that it leaves each time, instead of panicking.
	for (TInt failureCounter = 1 ; error == KErrCorrupt ; ++failureCounter)
		{
		testFile.SetErrorCondition(KErrCorrupt , failureCounter );
		error = TheAlarmServerSession.Connect();
		testFile.SetErrorCondition(KErrNone );
		}

	// 5) Close file session.
	CleanupStack::PopAndDestroy(&testFile);

	TheAlarmTest(error == KErrNone, __LINE__);
	}

static void ServerStartUpWithNoIniFileL()
	{
	_LIT(KRobustTestNoConfigDir, "Server Startup With No Config File Test");
	_LIT(KRobustTestDeleteIniFileDir, "Delete INI file and Dir");
	_LIT(KRobustTestDeleteIniFile, "Delete INI file");
	TheAlarmTest.Next(KRobustTestNoConfigDir);
	RPIMTestServer serv;
	User::LeaveIfError(serv.Connect());
	CleanupClosePushL(serv);

	// 1) Close Alarm Server
	TheAlarmTest.Start(KRobustTestCloseServer);
	TheAlarmServerSession.__DbgShutDownServer();

	// 2) Delete AlarmServer.ini file and it's dir
	TheAlarmTest.Next(KRobustTestDeleteIniFileDir);
	serv.DeleteFileL(KTestIniFile);
	serv.DeleteDirL(KAlarmIniFileDir);
	AlarmTest(serv.CheckForFile(KTestIniFile)==KErrPathNotFound);	//Check directory deleted

	// 3) Connect to the alarm server and check it created INI file directory
	TheAlarmTest.Next(KRobustTestCreateServer);
	User::LeaveIfError(TheAlarmServerSession.Connect());
	AlarmTest(serv.CheckForFile(KTestIniFile)==KErrNotFound);	//Check directory created but file still absent

	// 4) Add alarm and check INI file created
	TAlarmId testID = KNullAlarmId;
	TRequestStatus serverIniStatus;
	const TTimeIntervalMicroSeconds32 KTimeOutPeriod(1000000);
	RTimer timeoutTimer;
	CleanupClosePushL(timeoutTimer);
	TRequestStatus timerStatus;
	User::LeaveIfError(timeoutTimer.CreateLocal());
	timeoutTimer.After(timerStatus, KTimeOutPeriod);
	serv.FileServerNotifyChange(ENotifyFile, serverIniStatus, KTestIniFile);

	CreateSingleTestAlarmL(testID);
	User::WaitForRequest(timerStatus, serverIniStatus);

	if (timerStatus.Int() != KRequestPending)
		{
		// The file creation did not time out
		// so cancel the time out timer
		timeoutTimer.Cancel();
		User::WaitForRequest(timerStatus);
		}
	else
		{
		// The file creation timed out
		// so cancel the FileServerNotifyChange
		serv.FileServerNotifyChangeCancel();
		User::WaitForRequest(serverIniStatus);
		}

	AlarmTest(serv.CheckForFile(KTestIniFile)==KErrNone);	//Check file present

	// 6) Close Alarm Server Again
	TheAlarmTest.Next(KRobustTestCloseServer);
	TheAlarmServerSession.__DbgShutDownServer();

	// 7) Delete AlarmServer.ini file only
	TheAlarmTest.Next(KRobustTestDeleteIniFile);
	serv.DeleteFileL(KTestIniFile);
	AlarmTest(serv.CheckForFile(KTestIniFile)==KErrNotFound);	//Check file deleted but directory still there

	// 8) Connect to the alarm server
	TheAlarmTest.Next(KRobustTestCreateServer);
	User::LeaveIfError(TheAlarmServerSession.Connect());
	AlarmTest(serv.CheckForFile(KTestIniFile)==KErrNotFound);	//Check directory there but file still absent

	// 9) Add alarm and check INI file created
	testID = KNullAlarmId;
	serv.FileServerNotifyChange(ENotifyFile, serverIniStatus, KTestIniFile);
	timeoutTimer.After(timerStatus, KTimeOutPeriod);

	CreateSingleTestAlarmL(testID);
	User::WaitForRequest(timerStatus, serverIniStatus);

	if (timerStatus.Int() != KRequestPending)
		{
		// The file creation did not time out
		// so cancel the time out timer
		timeoutTimer.Cancel();
		User::WaitForRequest(timerStatus);
		}
	else
		{
		// The file creation timed out
		// so cancel the FileServerNotifyChange
		serv.FileServerNotifyChangeCancel();
		User::WaitForRequest(serverIniStatus);
		}

	AlarmTest(serv.CheckForFile(KTestIniFile)==KErrNone);	//Check file present

	TheAlarmTest.UpLevel();
	CleanupStack::PopAndDestroy(&timeoutTimer);
	CleanupStack::PopAndDestroy(&serv);
	serv.Close();
	}


#endif // #ifdef _DEBUG
#endif // #ifdef __WINS__


static void AlarmInternalizationTestL()
	{
	_LIT(KRobustTestAlarmInternalization, "Alarm Internalization Test");
	TheAlarmTest.Next(KRobustTestAlarmInternalization);

	// 1) Create an alarm object.
	TASShdAlarm alarm;
	TASShdAlarm alarm2;
	InitializeAlarm(alarm);

	// 2) Create a memory buffer to store alarm object created above.
	CBufFlat* buffer = CBufFlat::NewL(KAlarmObjectBuffer); // approximation
	CleanupStack::PushL(buffer);
	RBufWriteStream stream(*buffer);

	// 3) Externalize alarm object into memory buffer.
	alarm.ExternalizeL(stream);
	stream.CommitL();

	// 4) Create second memor`y buffer to manipulate the one above.
	CBufFlat* buffer2 = CBufFlat::NewL(KAlarmObjectBuffer);
	CleanupStack::PushL(buffer2);

	// 5) Test loop - Copy the contents of buffer to buffer2, starting at
	// 				the end of buffer.
	//    We just want to test that our methods leave normally under
	//    the error condition and not induce a panic. So we don't need
	//    to handle the Trap in this test loop.
	TInt err = KErrCorrupt;
	for(TInt i = buffer->Size()-1; i>-1; i--)
		{
		TPtr8 ptr=buffer->Ptr(i);
		buffer2->Reset();
		buffer2->InsertL(0, ptr);

		RBufReadStream rStream(*buffer2);
		TRAP(err, alarm2.InternalizeL(rStream))

		}

	TheAlarmTest.Test().Printf(_L("AlarmInternalizationTestL error = %d\n"), err);

	TheAlarmTest(err == KErrNone, __LINE__);

	CleanupStack::PopAndDestroy(buffer2);
	CleanupStack::PopAndDestroy(buffer);
	}


static void AlarmFormatInternalizationTestL()
	{
	_LIT(KRobustTestAlarmFormatInternalization, "Alarm Format Internalization Test");
	TheAlarmTest.Next(KRobustTestAlarmFormatInternalization);

	// 1) Create an alarm object.
	TASShdAlarm alarm;
	TASShdAlarm alarm2;
	InitializeAlarm(alarm);

	// 2) Create a memory buffer to store alarm object created above.
	CBufFlat* buffer = CBufFlat::NewL(KAlarmObjectBuffer); // approximation
	CleanupStack::PushL(buffer);
	RBufWriteStream stream(*buffer);

	// 3) Externalize alarm object into memory buffer.
	alarm.ExternalizeL(stream);
	stream.CommitL();

	// 4) Create second memory buffer to manipulate the one above.
	TPtr8 initPtr = buffer->Ptr(0); // Ptr to beginning of buffer.
	CBufFlat* buffer2 = CBufFlat::NewL(KAlarmObjectBuffer);
	CleanupStack::PushL(buffer2);

	// 5) Test loop - Write a character to every position of the
	//					alarm object.
	//    We just want to test that our methods leave normally under
	//    the error condition and not induce a panic. So we don't need
	//    to handle the Trap in this test loop.
	TInt err = KErrCorrupt;
	_LIT(KRobustTestCharactter, "a");
	const TInt KBufferSize = buffer->Size()-1;
	for(TInt i = 0; i<KBufferSize; i++)
		{
		alarm2.Reset();
		alarm2 = alarm;
		buffer2->Reset();
		buffer2->InsertL(0, initPtr);

		TPtr8 ptr = buffer2->Ptr(i);
		ptr.Copy(KRobustTestCharactter);

		RBufReadStream rStream(*buffer2);
		TRAP(err, alarm2.InternalizeL(rStream))

		}

	TheAlarmTest(err == KErrNone, __LINE__);

	CleanupStack::PopAndDestroy(buffer2);
	CleanupStack::PopAndDestroy(buffer);
	}

//*************************************************************************************
void InitializeUtcAlarm(TASShdAlarm& theAlarm)
	{
	theAlarm.Category()=KASCliCategoryClock;
	theAlarm.RepeatDefinition()=EAlarmRepeatDefintionRepeatDaily;
	_LIT(KAlarmTestInit, "This is a UTC alarm message - alarm 1");
	TBuf<KMaxAlarmMessageLength> buf(KAlarmTestInit);
	theAlarm.Message()=buf;
	TTime time;
	time.UniversalTime();
	time += TTimeIntervalMinutes(1);
	theAlarm.SetUtcNextDueTime(time);
	_LIT(KAlarmTestSoundName, "Arf Arf Woop");
	buf = KAlarmTestSoundName;
	theAlarm.SoundName()=buf;
	}


#ifdef _DEBUG
#ifdef __WINS__
static void CreateSingleTestUtcAlarmL(TAlarmId& testID)
	{
	_LIT(KAlarmTestAddDisabled, "Adding disabled UTC alarm");
	TheAlarmTest.Next(KAlarmTestAddDisabled);
	//
	TASShdAlarm alarm;
	InitializeUtcAlarm(alarm);

	//
	TheAlarmTest(TheAlarmServerSession.AlarmAdd(alarm) == KErrNone, __LINE__);
	const TAlarmId id = alarm.Id();
	testID = id;
	TheAlarmTest(id != KNullAlarmId, __LINE__);

	//
	_LIT(KAlarmTestGetDetails, "Get UTC alarm details");
	TheAlarmTest.Next(KAlarmTestGetDetails);
	TheAlarmTest(TheAlarmServerSession.GetAlarmDetails(id, alarm) == KErrNone, __LINE__);

	//
	_LIT(KAlarmTestEnableAlarm, "Enabling a disabled UTC alarm");
	TheAlarmTest.Next(KAlarmTestEnableAlarm);
	TheAlarmTest(TheAlarmServerSession.SetAlarmStatus(id, EAlarmStatusEnabled) == KErrNone, __LINE__);
	}


static void RunNoConfigFileTestUtcL()
	{
	_LIT(KRobustTestNoConfig, "Run No Config File Test");
	TheAlarmTest.Next(KRobustTestNoConfig);

	// 1) Delete AlarmServer.ini file
   	DeleteIniFileL();

	// 2) Connect to the alarm server.
	TheAlarmTest.Next(KRobustTestCreateServer);
	User::LeaveIfError(TheAlarmServerSession.Connect());

	// 3) Add alarm.
	TAlarmId testID = KNullAlarmId;
	CleanupClosePushL(TheAlarmServerSession);
	CreateSingleTestUtcAlarmL(testID);

	// 4) Shutdown alarm server.
	TheAlarmTest.Next(KRobustTestCloseServer);
	TheAlarmServerSession.__DbgShutDownServer();
	CleanupStack::PopAndDestroy(&TheAlarmServerSession);

	// 5) Restart alarm server.
	TheAlarmTest.Next(KRobustTestCreateServer);
	User::LeaveIfError(TheAlarmServerSession.Connect());

	// 6) Check alarm has persisted.
	TASShdAlarm testAlarm;
	TInt error = TheAlarmServerSession.GetAlarmDetails(testID, testAlarm);
	TheAlarmTest(error == KErrNone, __LINE__);
	}


static void RunZeroLengthConfigFileTestUtcL()
	{
	_LIT(KRobustTestZeroLengthConfigFile, "Run Zero Length Config File Test");
	TheAlarmTest.Next(KRobustTestZeroLengthConfigFile);

	// 1) Create an alarm to make sure there is a config file.
	TAlarmId testID = KNullAlarmId;
	CreateSingleTestUtcAlarmL(testID);
	User::After(KAlarmTestWaitForFileWrite);

	// 2) Zero length the AlarmServer.ini file
   	TestClearStoreL();

	// 3) Connect to the alarm server.
	TheAlarmTest.Next(KRobustTestCreateServer);
	User::LeaveIfError(TheAlarmServerSession.Connect());

	// 4) Add alarm.
	CleanupClosePushL(TheAlarmServerSession);
	CreateSingleTestUtcAlarmL(testID);

	// 5) Shutdown alarm server.
	TheAlarmTest.Next(KRobustTestCloseServer);
	TheAlarmServerSession.__DbgShutDownServer();
	CleanupStack::PopAndDestroy(&TheAlarmServerSession);

	// 6) Restart alarm server.
	TheAlarmTest.Next(KRobustTestCreateServer);
	User::LeaveIfError(TheAlarmServerSession.Connect());

	// 7) Check alarm has persisted.
	TASShdAlarm testAlarm;
	TInt error = TheAlarmServerSession.GetAlarmDetails(testID, testAlarm);
	TheAlarmTest(error == KErrNone, __LINE__);
	}


static void CorruptErrorConditionTestUtcL()
	{
	_LIT(KRobustTestCorruptErrorCondition, "Corrupt Error Condition Test");
	TheAlarmTest.Next(KRobustTestCorruptErrorCondition);

	// 1) Create file session to call SetErrorCondition.
	TAlarmId testID = KNullAlarmId;
 	TInt error  = KErrCorrupt;
	RFs testFile;
	CleanupClosePushL(testFile);
	testFile.Connect();

	// 2) Create a few alarms to make sure we have populated config file.
	CleanupClosePushL(TheAlarmServerSession);
	const TInt KNumberOfTestAlarms = 3;
	for( TInt i=0;i<KNumberOfTestAlarms;i++)
		{
		CreateSingleTestUtcAlarmL(testID);
		}
	User::After(KAlarmTestWaitForFileWrite);


	// 3) Shut down server so we can re-connect and do internalization.
	TheAlarmServerSession.__DbgShutDownServer();
	CleanupStack::PopAndDestroy(&TheAlarmServerSession);

	// 4) Test loop - This will cycle through file session internalization
	//	process, checking to see that it leaves each time, instead of panicking.
	for (TInt failureCounter = 1 ; error == KErrCorrupt ; ++failureCounter)
		{
		testFile.SetErrorCondition(KErrCorrupt , failureCounter );
		error = TheAlarmServerSession.Connect();
		testFile.SetErrorCondition(KErrNone );
		TheAlarmServerSession.Close();
		}

	// 5) Close file session.
	CleanupStack::PopAndDestroy(&testFile);
	testFile.Close();
	TheAlarmTest(error == KErrNone, __LINE__);
	}
#endif // #ifdef _DEBUG
#endif // #ifdef __WINS__


static void UtcAlarmInternalizationTestL()
	{
	_LIT(KRobustTestAlarmInternalization, "UTC Alarm Internalization Test");
	TheAlarmTest.Next(KRobustTestAlarmInternalization);

	// 1) Create an alarm object.
	TASShdAlarm alarm;
	TASShdAlarm alarm2;
	InitializeUtcAlarm(alarm);

	// 2) Create a memory buffer to store alarm object created above.
	CBufFlat* buffer = CBufFlat::NewL(KAlarmObjectBuffer); // approximation
	CleanupStack::PushL(buffer);
	RBufWriteStream stream(*buffer);

	// 3) Externalize alarm object into memory buffer.
	alarm.ExternalizeL(stream);
	stream.CommitL();

	// 4) Create second memor`y buffer to manipulate the one above.
	CBufFlat* buffer2 = CBufFlat::NewL(KAlarmObjectBuffer);
	CleanupStack::PushL(buffer2);

	// 5) Test loop - Copy the contents of buffer to buffer2, starting at
	// 				the end of buffer.
	//    We just want to test that our methods leave normally under
	//    the error condition and not induce a panic. So we don't need
	//    to handle the Trap in this test loop.
	TInt err = KErrCorrupt;
	for(TInt i = buffer->Size()-1; i>-1; i--)
		{
		TPtr8 ptr=buffer->Ptr(i);
		buffer2->Reset();
		buffer2->InsertL(0, ptr);

		RBufReadStream rStream(*buffer2);
		TRAP(err, alarm2.InternalizeL(rStream))

		}

	TheAlarmTest.Test().Printf(_L("UtcAlarmInternalizationTestL error = %d\n"), err);

	TheAlarmTest(err == KErrNone, __LINE__);

	CleanupStack::PopAndDestroy(buffer2);
	CleanupStack::PopAndDestroy(buffer);
	}


static void UtcAlarmFormatInternalizationTestL()
	{
	_LIT(KRobustTestAlarmFormatInternalization, "UTC Alarm Format Internalization Test");
	TheAlarmTest.Next(KRobustTestAlarmFormatInternalization);

	// 1) Create an alarm object.
	TASShdAlarm alarm;
	TASShdAlarm alarm2;
	InitializeUtcAlarm(alarm);

	// 2) Create a memory buffer to store alarm object created above.
	CBufFlat* buffer = CBufFlat::NewL(KAlarmObjectBuffer); // approximation
	CleanupStack::PushL(buffer);
	RBufWriteStream stream(*buffer);

	// 3) Externalize alarm object into memory buffer.
	alarm.ExternalizeL(stream);
	stream.CommitL();

	// 4) Create second memory buffer to manipulate the one above.
	TPtr8 initPtr = buffer->Ptr(0); // Ptr to beginning of buffer.
	CBufFlat* buffer2 = CBufFlat::NewL(KAlarmObjectBuffer);
	CleanupStack::PushL(buffer2);

	// 5) Test loop - Write a character to every position of the
	//					alarm object.
	//    We just want to test that our methods leave normally under
	//    the error condition and not induce a panic. So we don't need
	//    to handle the Trap in this test loop.
	TInt err = KErrCorrupt;
	_LIT(KRobustTestCharactter, "a");
	const TInt KBufferSize = buffer->Size()-1;
	for(TInt i = 0; i<KBufferSize; i++)
		{
		alarm2.Reset();
		alarm2 = alarm;
		buffer2->Reset();
		buffer2->InsertL(0, initPtr);

		TPtr8 ptr = buffer2->Ptr(i);
		ptr.Copy(KRobustTestCharactter);

		RBufReadStream rStream(*buffer2);
		TRAP(err, alarm2.InternalizeL(rStream))

		}

	TheAlarmTest(err == KErrNone, __LINE__);

	CleanupStack::PopAndDestroy(buffer2);
	CleanupStack::PopAndDestroy(buffer);
	}


//*************************************************************************************
static void RunTestsL()
	{

	// Have to start the alarm alert server otherwise the alarm
	// server cannot start.
	_LIT(KRobustTestCreateAlarmAlert, "Creating alarm alert server");
	TheAlarmTest.Next(KRobustTestCreateAlarmAlert);
	const TInt KGmtOffset = 3600;
	_LIT(KTime, "20040730:050000.000000"); //Summertime
	TInt err = User::SetUTCTimeAndOffset(TTime(KTime), KGmtOffset);
	TheAlarmTest(err == KErrNone, __LINE__);

#ifdef __WINS__
	_LIT(KConsoleAlarmAlertServerImg,"ConsoleAlarmAlertServer");
    TBuf<256> cmdline;
	cmdline.Append('p');
	cmdline.Append('p');
	cmdline.Append(EKeyEscape);
	RProcess server;
	server.Create(KConsoleAlarmAlertServerImg, cmdline, serverUid);
	TRequestStatus stat;
	server.Rendezvous(stat);
	if (stat != KRequestPending)
		{
		server.Kill(0);		// abort startup
		}
	else
		{
		server.Resume();	// logon OK - start the server
		}
		
	User::WaitForRequest(stat);
	TInt result = stat.Int();
	if(result != KErrNone && result != KErrAlreadyExists)
		{
		User::LeaveIfError(result);	
		}		
#endif //  __WINS__

 	// Connect() checks if the server is already running before connecting.
 	// Here, we call connect to make sure it is running so the tests below can be
 	// executed in any order.

	// Connect to the alarm server.
	TheAlarmTest.Next(KRobustTestCreateServer);
	User::LeaveIfError(TheAlarmServerSession.Connect());

#ifdef _DEBUG
#ifdef __WINS__
	TestClearStoreL();
	DeleteIniFileL();
	User::After(KAlarmTestWaitForFileWrite);
#endif // #ifdef __WINS__
#endif // #ifdef _DEBUG

	// Call Test functions.
	_LIT(KRobustTestCallTests, "Calling Test functions");
	TheAlarmTest.Next(KRobustTestCallTests);

	TheAlarmTest.Test().Printf(_L("Start local alarm tests.\n"));

#ifdef _DEBUG
#ifdef __WINS__
	RunNoConfigFileTestL();
	RunZeroLengthConfigFileTestL();
	CorruptErrorConditionTestL();
	ServerStartUpWithNoIniFileL();
#endif // #ifdef __WINS__
#endif // #ifdef _DEBUG

	AlarmInternalizationTestL();
	AlarmFormatInternalizationTestL();

	TheAlarmTest.Test().Printf(_L("Start UTC alarm tests.\n"));

#ifdef _DEBUG
#ifdef __WINS__
	RunNoConfigFileTestUtcL();
	RunZeroLengthConfigFileTestUtcL();
	CorruptErrorConditionTestUtcL();
#endif // #ifdef __WINS__
#endif // #ifdef _DEBUG

	UtcAlarmInternalizationTestL();
	UtcAlarmFormatInternalizationTestL();

	}




//
// -----> Global Exports
//

//*************************************************************************************
TInt E32Main()
	{
	__UHEAP_MARK;
	TInt ret = KErrNone;
	//
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if	(!cleanup)
		return KErrNoMemory;
	//
	TheAlarmTest.Title();
	TheAlarmTest.Start(KTestTitle);
	// @SYMTestCaseID PIM-TALARMROBUSTNESS-0001
    TheAlarmTest.Next(_L(" @SYMTestCaseID PIM-TALARMROBUSTNESS-0001 "));
	//
	TRAPD(err, RunTestsL());
	TheAlarmTest(err == KErrNone, __LINE__);

	delete cleanup;
    TRAP(ret,TheAlarmTest.EndL());
	TheAlarmTest.Test().Close();
	ASTstAlarmTest::Close();

	__UHEAP_MARKEND;
	return ret;
	}
