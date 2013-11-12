// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "TestCITAlarmServer.h"
#include "TestCITAlarmServerSuiteDefs.h"

#include <tz.h>

typedef CArrayFixFlat<TASCliSoundPlayDefinition> CPlayIntervalArray;

CTestCITAlarmServer::~CTestCITAlarmServer()
/**
 * Destructor
 */
	{
	}

CTestCITAlarmServer::CTestCITAlarmServer()
/**
 * Constructor
 */
	{
	SetTestStepName(KTestCITAlarmServer);
	}

TVerdict CTestCITAlarmServer::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	INFO_PRINTF1(KCITFS32);
	SetTestStepResult(EPass);
	return TestStepResult();
	}

TVerdict CTestCITAlarmServer::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{   
    //Connects to Alarm Server
	ConnectToAlarmServer();
    CleanupClosePushL(iAlarmServerSession);
    
    TestClearStoreL();
    
    TBool success = EFalse;
	
	success = ExecuteTestL(); // Create alarm, info taken from text file
    
    CleanUpAlarmServer();
    CleanupStack::PopAndDestroy(); //Alarm Server Session
    
    if (success == EFalse)
    	{
    	SetTestStepResult(EFail);
    	}
    else
    	{	
    	SetTestStepResult(EPass);
    	}
    	
	return TestStepResult();
	}

TVerdict CTestCITAlarmServer::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	CloseAlarmServer();
	INFO_PRINTF1(_L("Completed Executing Integration Alarm Server Test (App-Services - FS 3.2)")); 
	return TestStepResult();
	}

/** Executes Alarm Server Tests
@return True or False indicating the result of the alarm server tests
*/
TBool CTestCITAlarmServer::ExecuteTestL()
	{
	TBool passTest = EFalse;
	iField = KTest;
	GetIntFromConfig(ConfigSection(),iField,iTestnumber);// gets the test number
	
	iField.Zero();
	iField = KTypeOfAlarm;
	GetStringFromConfig(ConfigSection(),iField,iTypeOfAlarm);// gets the test number
	
	switch (iTestnumber)
		{
		case(ETest1):
			{
			SetUTCOffSetL();
			SetSystemTimeL(EFalse);
			SetAlarmQuietPeriodL(); //set out of range quiet period i.e. 2101
			AddAlarmToServerL(); // should not panic
			passTest = ETrue;
			break;
			}
			
		case(ETest2):
			{
			SetUTCOffSetL();
			SetSystemTimeL(EFalse);
			SetAlarmQuietPeriodL(); //set silent in past, and verify it returns KErrArgument
			passTest = ETrue;	
			break;
			}
			
		case(ETest3):
			{
			SetUTCOffSetL();
			SetSystemTimeL(EFalse);
			AddAlarmToServerL();
			SetAlarmSilentPeriodForL();
			ChangeUTCOffSetL(); //sets utc agian, so silent period should be cancelled
			User::After(KTimeToWait);//time delay
			passTest = CheckGlobalSoundState(EAlarmGlobalSoundStateOn); //check if state ON, as silent period cancelled
			break;
			}
			
		case(ETest4):
			{
			SetUTCOffSetL();
			SetSystemTimeL(EFalse);
			InfoForAlarmCount();  //should return 0 for all states as no alarms, and not panic
			AddAlarmToServerL();
			passTest = ETrue;
			break;
			}	
			
		case(ETest5):
			{
			SetUTCOffSetL();
			SetSystemTimeL(EFalse);
			AddAlarmToServerL();
			GetAlarmCharacL();
			passTest = ETrue;
			break;
			}
			
		case(ETest6):
			{
			SetUTCOffSetL();
			SetSystemTimeL(EFalse);
			AddAlarmToServerL(); //add alarm to expire 24 hours later
			passTest = CheckForAlarmsInQueue(); //check if alarm in queue
			break;
			}
		
		case(ETest7):
			{
			SetUTCOffSetL();
			SetSystemTimeL(EFalse);
			AddAlarmToServerL();
			SetSystemTimeL(ETrue);  //Change system time, so the new time is less than one day than the previously calculated time
			InfoForAlarmCount();  //should be in notifying state, as it should expire immediately
			if (WaitAndCheckExpiryL())
				{
				InfoForAlarmCount();
				passTest = WaitAndCheckSoundPlayStartL(); // should start playing sounds after expiring
				}
			else
				{
				passTest = EFalse;
				}
			break;
			}
				
		case(ETest8):
			{
			SetUTCOffSetL();
			SetSystemTimeL(EFalse);
			AddAlarmToServerL();
			SetSystemTimeL(ETrue);  //Change system time, so the new time is greter than one day, than the previously calculated expiry time
			InfoForAlarmCount(); 
			passTest = CheckIfAlarmDeleted(); 
			InfoForAlarmCount(); 
			break;
			}	
			
		case(ETest9):
			{
			SetUTCOffSetL();
			SetSystemTimeL(EFalse);
			AddAlarmToServerL(); // add 24 hour alarm, to expire 23hrs 59mins after system time
			passTest = CheckForAlarmsInQueue(); // alarm should be queued
			break;
			}
			
		case(ETest10):
			{
			SetUTCOffSetL();
			SetSystemTimeL(EFalse);
			AddAlarmToServerL(); // add 24 hour alarm
			SetSystemTimeL(ETrue); // change system time, so its before the calculated expiry time
			User::After(KTimeToWait); // time delay
			passTest = CheckForAlarmDefintion(); // repeat definition should change to repeat once
			break;
			}
			
		case(ETest11):
			{
			SetUTCOffSetL();
			SetSystemTimeL(EFalse);
			AddAlarmToServerL(); // add 24 hour alarm
			SetSystemTimeL(ETrue);  // Change system time, so new time is less than one day than the previuosly calculated expiry time
			InfoForAlarmCount();  //should be in notifying state, as alarm should expire immediately
			if (WaitAndCheckExpiryL())
				{
				passTest = WaitAndCheckSoundPlayStartL(); // sounds should start playing immediately 
				}
			else
				{
				passTest = EFalse;
				}
			break;
			}
			
		case(ETest12):
			{
			SetUTCOffSetL();
			SetSystemTimeL(EFalse);
			AddAlarmToServerL(); //add 24 hour alarm
			SetSystemTimeL(ETrue); //Change system time, so the new time is greater than one day than the previously calculated expiry time
			InfoForAlarmCount();
			passTest = CheckIfAlarmDeleted(); //Check if alarm deleted
			InfoForAlarmCount(); 
			break;
			}
				
		case(ETest13):
			{
			SetUTCOffSetL();
			SetSystemTimeL(EFalse);
			AddAlarmToServerL(); //Add Repeat Daily Alarm to server
			InfoForAlarmCount();
			if(WaitAndCheckExpiryL()) // wait to expire
				{
				InfoForAlarmCount();
				INFO_PRINTF2(_L("Wait for : %d secs"), secs);
				User::After(KTimeToWait * secs);
				INFO_PRINTF2(_L("Status of Alarm queue after waiting for : %d secs"), secs);
				InfoForAlarmCount();
				CheckNextExpiryTimeL(); // alarm should be scheduled for next day
				passTest = ETrue;
				}
			else
				{
				passTest = EFalse;
				}
			break;
			}
			
		case(ETest14):
			{	
			SetUTCOffSetL();
			SetSystemTimeL(EFalse);
			AddAlarmToServerL(); //Set Set Day/Timed alarm, add to server, get day/timed alarm and verify
			passTest = ETrue;
			break;
			}
			
		default:
			{
			SetUTCOffSetL();
			SetSystemTimeL(EFalse);
			SetAlarmQuietPeriodL();
			AddAlarmToServerL(); //adds alarm to server
			passTest = WaitAndCheckSoundPlayStartL(); //check if start playing is at the expected time
			break;
			}
		}
		
	return passTest;
	}
    
/** Changes UTC Offset in seconds
*/
void CTestCITAlarmServer::ChangeUTCOffSetL()
	{
	TInt utcOffset(0);

	iField = KCHANGEUTC;
	TPtrC ptrUTC = GetStringFromIni(iField);	
		
	if(ptrUTC == KNullDesC)
		{
		ERR_PRINTF1(_L("UTC offset not provided in test data"));
		SetTestStepResult(EFail);
		User::Leave(KErrNotFound);
		}
		
	TLex lex(ptrUTC); 	
	User::LeaveIfError(lex.Val(utcOffset));

	//Sets utc offset
	User::SetUTCOffset(utcOffset);
			
	FormatLogMessage(_L("Change UTC Offset (in seconds) to:-"), ptrUTC);
	}

/** Sets UTC Offset in seconds
*/
void CTestCITAlarmServer::SetUTCOffSetL()
	{
	TInt utcOffset(0);

	iField = KUTC;
	TPtrC ptrUTC = GetStringFromIni(iField);
		
	if(ptrUTC == KNullDesC)
		{
		ERR_PRINTF1(_L("UTC offset not provided in test data"));
		SetTestStepResult(EFail);
		User::Leave(KErrNotFound);
		}
		
	TLex lex(ptrUTC); 	
	User::LeaveIfError(lex.Val(utcOffset));

	//Sets utc offset
	RTz tz;
	tz.Connect();
	TTime now;
	now.UniversalTime();
	
	tz.SetUnknownZoneTimeL(now, utcOffset);
	tz.Close();
			
	FormatLogMessage(_L("UTC Offset (in seconds) Set to:-"), ptrUTC);
	}
	
/** Sets System Time
@param Sets the system time to a value passed from the ini
*/
void CTestCITAlarmServer::SetSystemTimeL(TBool aNewSystemTime)
	{
	if (aNewSystemTime)
		{
		iField = KMoveSystemTime;
		}
	else
		{
		iField = KSystemTime;
		}
	
	TPtrC ptrSystemTime = GetStringFromIni(iField);
	
	if(ptrSystemTime == KNullDesC)
		{
		ERR_PRINTF1(_L("System Time not provided in test data"));
		SetTestStepResult(EFail);
		User::Leave(KErrNotFound);
		}
		
	TTime systemTime;
	systemTime = FormatDateTime(ptrSystemTime);
	TInt err = User::SetUTCTime(systemTime);
	
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("User::SetUTCTime Error Code %d"), err);
		SetTestStepResult(EFail);
		SetTestStepError(err);
		}
	else
		{
		TBuf<MAX_TEXT_MESSAGE> messageLog(0);
		
		TDateTime systemDateTime(systemTime.DateTime());
		messageLog.Format(KDateTime, systemDateTime.Year(),TInt(systemDateTime.Month()),systemDateTime.Day(),
			systemDateTime.Hour(),systemDateTime.Minute(),systemDateTime.Second(),systemDateTime.MicroSecond());
		FormatLogMessage(_L("System Time Set to:- "), messageLog);
		}
	}
	
/** Sets expiry time (from ini test data file) and adds alarm to the alarm server session
*/
void CTestCITAlarmServer::AddAlarmToServerL()
	{
	TInt alarmExpires(0);

	//Sets expiry Time
	iField = KAlarmExpires;
	TPtrC ptrAlarmExpires = GetStringFromIni(iField);
	
	if(ptrAlarmExpires == KNullDesC)
		{
		ERR_PRINTF1(_L("Expiry Time not provided in test data"));
		SetTestStepResult(EFail);
		User::Leave(KErrNotFound);
		}
		
	TLex lex1(ptrAlarmExpires); 	
	User::LeaveIfError(lex1.Val(alarmExpires));
					
	iExpiryTime.UniversalTime();
	iExpiryTime += TTimeIntervalMinutes(alarmExpires);
		
	iAlarmId = AlarmAddL(iExpiryTime); 
		
	if (iAlarmId == KNullAlarmId)
		{
		TBuf<MAX_TEXT_MESSAGE> messageLog(0);
		messageLog.Format(KAddAlarmError);
		FormatLogMessage(_L("Alarm "),  messageLog);
		SetTestStepResult(EFail);
		}
	}

/** Deletes the previously set alarm
*/	
void CTestCITAlarmServer::CleanUpAlarmServer()
	{
	INFO_PRINTF1(_L("Alarm deleting....."));
	TInt err = iAlarmServerSession.AlarmDelete(iAlarmId);
	if (err != KErrNone)
		{
		TBuf<MAX_TEXT_MESSAGE> messageLog(0);
		messageLog.Format(KAlarmDeleteError);
		FormatLogMessage(_L("Alarm Delete "),  messageLog);
		SetTestStepResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("Alarm has been deleted"));
		}
	}

/** Sets quiet period of all alarms using api - SetAlarmSoundsSilentUntil
*/	
void CTestCITAlarmServer::SetAlarmQuietPeriodL()
	{
	iField = KQuietPeriod;
	TPtrC ptrQuietPeriod = GetStringFromIni(iField);
	
	if(ptrQuietPeriod == KNullDesC)
		{
		ERR_PRINTF1(_L("Quiet Period not provided in test data"));
		SetTestStepResult(EFail);
		User::Leave(KErrNotFound);
		}
		
	TTime quietPeriod;
	quietPeriod = FormatDateTime(ptrQuietPeriod);
		
	TInt err = iAlarmServerSession.SetAlarmSoundsSilentUntil(quietPeriod);
	
	if (iTestnumber == testnumber)
		{
		TESTL(err == KErrArgument);
		}
	else
		{
		if (err != KErrNone)
			{
			ERR_PRINTF2(_L("SetAlarmSoundsSilentUntil Error Code %d"), err);
			SetTestStepResult(EFail);
			SetTestStepError(err);
			}
		else
			{
			TBuf<MAX_TEXT_MESSAGE> messageLog(0);
		
			TDateTime quietPeriodDateTime(quietPeriod.DateTime());
			messageLog.Format(KDateTime, quietPeriodDateTime.Year(),TInt(quietPeriodDateTime.Month()),quietPeriodDateTime.Day(),
				quietPeriodDateTime.Hour(),quietPeriodDateTime.Minute(),quietPeriodDateTime.Second(),quietPeriodDateTime.MicroSecond());
			FormatLogMessage(_L("Sound Silent Until:- "), messageLog);
			}
		}
	}

/** Waits for the alarm to start playing sounds, and verifies the time
@return ETrue if alarm sounds start at expected time otherwise EFalse
*/	
TBool CTestCITAlarmServer::WaitAndCheckSoundPlayStartL()
	{
	TTime now;

	iField = KSoundStartTime;
	TPtrC ptrSoundStart = GetStringFromIni(iField);
	
	if(ptrSoundStart == KNullDesC)
		{
		ERR_PRINTF1(_L("Expected Sound Start not provided in test data"));
		SetTestStepResult(EFail);
		User::Leave(KErrNotFound);
		}
		
	TTime silentUntil;
	silentUntil = FormatDateTime(ptrSoundStart);
	TBuf<MAX_TEXT_MESSAGE> messageLog(0);
	TRequestStatus status;
	TAlarmId returnedAlarmId;
	FOREVER
		{
		iAlarmServerSession.NotifyChange(status,returnedAlarmId);
		User::WaitForRequest(status);
		if	(status.Int() == EAlarmChangeEventSoundPlaying)
			{
			now.UniversalTime();
						
			TDateTime startTime(now.DateTime());
			messageLog.Format(KDateTime, startTime.Year(),TInt(startTime.Month()),startTime.Day(),
					startTime.Hour(),startTime.Minute(),startTime.Second(),startTime.MicroSecond());
			FormatLogMessage(_L("Sound started (UTC time) to play at:- "), messageLog);
			
			TDateTime nowTime(now.DateTime());
	
			TTimeIntervalMicroSeconds microsecond = nowTime.MicroSecond();
			now = now - microsecond;
	
			TTimeIntervalSeconds secondsDifference;
			TInt difference;
			
			TDateTime silentUntil1(silentUntil.DateTime());
			messageLog.Format(KDateTime, silentUntil1.Year(),TInt(silentUntil1.Month()),silentUntil1.Day(),
					silentUntil1.Hour(),silentUntil1.Minute(),silentUntil1.Second(),silentUntil1.MicroSecond());
			FormatLogMessage(_L("Sound playing time expected:- "), messageLog);
			
			
			TDateTime now2(now.DateTime());
			messageLog.Format(KDateTime, now2.Year(),TInt(now2.Month()),now2.Day(),
					now2.Hour(),now2.Minute(),now2.Second(),now2.MicroSecond());
			FormatLogMessage(_L("Sound playing time actual:- "), messageLog);
			
			User::LeaveIfError(silentUntil.SecondsFrom(now, secondsDifference));
			difference = secondsDifference.Int();
			INFO_PRINTF2(_L("Difference between actual sound play and expected sound play in seconds: %d"), difference);
			if (difference < 5 && difference > -5)
				{
				INFO_PRINTF3(KPlayStart,returnedAlarmId,iAlarmId);
				INFO_PRINTF1(_L("Sound Started At Expected Time"));
				return ETrue;
				}
			else
				{
				INFO_PRINTF1(KAlarmSoundStartError);
				return EFalse;
				}
			}
		}
	}

/** Sets quiet period of all alarms using api - SetAlarmSoundsSilentFor
*/	
void CTestCITAlarmServer::SetAlarmSilentPeriodForL()
	{
	TInt silentFor(0);

	// set silent period in minutes
	iField = KSilentFor;
	TPtrC ptrSilentFor = GetStringFromIni(iField);
		
	TLex lex(ptrSilentFor); 	
	User::LeaveIfError(lex.Val(silentFor));
		
	INFO_PRINTF2(_L("Setting Alarm Sounds Silent For: %d min"), silentFor);
	TInt err = iAlarmServerSession.SetAlarmSoundsSilentFor(TTimeIntervalMinutes(silentFor));
	
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("SetAlarmSoundsSilentFor Error Code %d"), err);
		SetTestStepResult(EFail);
		SetTestStepError(err);
		User::Leave(err);
		}
	else
		{
		INFO_PRINTF2(_L("Alarm Sounds Silent For Has been Set For: %d min"), silentFor);
		}
	}

/** Prints information of states of all alarms in the alarm server
*/	
void CTestCITAlarmServer::InfoForAlarmCount()
	{
	TInt countByState = iAlarmServerSession.AlarmCountByState(EAlarmStateInPreparation);
	INFO_PRINTF2(_L("No of Alarms in state EAlarmStateInPreparation: %d"), countByState); 
	 
	countByState = iAlarmServerSession.AlarmCountByState(EAlarmStateQueued );
	INFO_PRINTF2(_L("No of Alarms in state EAlarmStateQueued: %d"), countByState); 
	 
	countByState = iAlarmServerSession.AlarmCountByState(EAlarmStateSnoozed );
	INFO_PRINTF2(_L("No of Alarms in state EAlarmStateSnoozed: %d"), countByState); 
	
	countByState = iAlarmServerSession.AlarmCountByState(EAlarmStateWaitingToNotify);
	INFO_PRINTF2(_L("No of Alarms in state EAlarmStateWaitingToNotify: %d"), countByState); 
	
	countByState = iAlarmServerSession.AlarmCountByState(EAlarmStateNotifying);
	INFO_PRINTF2(_L("No of Alarms in state EAlarmStateNotifying: %d"), countByState); 
	
	countByState = iAlarmServerSession.AlarmCountByState(EAlarmStateNotified);
	INFO_PRINTF2(_L("No of Alarms in state EAlarmStateNotified: %d"), countByState); 
	}

/** Validates the default alarm characteristics
*/
void CTestCITAlarmServer::GetAlarmCharacL()
	{
	TAlarmCharacteristicsFlags characteristics;
	
	TInt err = iAlarmServerSession.GetAlarmCharacteristics(iAlarmId, characteristics);
	
	TESTL(err == KErrNone);
	
	TESTL(characteristics.iFlags == 4);
	}

/** Verifies the number of alarms active in the alarm server queue
@return ETrue if number of active alarms in server queue as expected otherwise EFalse
*/
TBool CTestCITAlarmServer::CheckForAlarmsInQueue()
	{
	iField = KAlarmInQueue;
	TInt expecNumOfAlarm;
	
	GetIntFromConfig(ConfigSection(), iField, expecNumOfAlarm);

	TInt numOfAlarmInQueue = iAlarmServerSession.NumberOfAlarmsActiveInQueue();
	
	return (expecNumOfAlarm == numOfAlarmInQueue);
	}

/** Waits for the alarm to expire, and verifies the time
@return ETrue if alarm expires at expected time otherwise EFalse
*/
TBool CTestCITAlarmServer::WaitAndCheckExpiryL()
	{
	TTime now;

	iField = KExpectedExpiryTime;
	TPtrC ptrExpiryTime = GetStringFromIni(iField);
	
	if(ptrExpiryTime == KNullDesC)
		{
		ERR_PRINTF1(_L("Expected Expiry Time not provided in test data"));
		SetTestStepResult(EFail);
		User::Leave(KErrNotFound);
		}
		
	TTime expiryTime;
	expiryTime = FormatDateTime(ptrExpiryTime);
	TBuf<MAX_TEXT_MESSAGE> messageLog(0);
	TRequestStatus status;
	TAlarmId returnedAlarmId;
	FOREVER
		{
		iAlarmServerSession.NotifyChange(status,returnedAlarmId);
		User::WaitForRequest(status);
		if	(status.Int() == EAlarmChangeEventTimerExpired)
			{
			now.UniversalTime();
						
			TDateTime expiryTime1(now.DateTime());
			messageLog.Format(KDateTime, expiryTime1.Year(),TInt(expiryTime1.Month()),expiryTime1.Day(),
					expiryTime1.Hour(),expiryTime1.Minute(),expiryTime1.Second(),expiryTime1.MicroSecond());
			FormatLogMessage(_L("Alarm Expired at:- "), messageLog);
			
			TDateTime nowTime(now.DateTime());
	
			TTimeIntervalMicroSeconds microsecond = nowTime.MicroSecond();
			now = now - microsecond;
	
			TTimeIntervalSeconds secondsDifference;
			TInt difference;
			
			User::LeaveIfError(expiryTime.SecondsFrom(now, secondsDifference));
			difference = secondsDifference.Int();
			INFO_PRINTF2(_L("Difference between actual expiry time and expected expiry time in seconds: %d"), difference);
			if (difference < 5 && difference > -5)
				{
				INFO_PRINTF3(KAlarmExpired,returnedAlarmId,iAlarmId);
				INFO_PRINTF1(_L("Alarm Expired At Expected Time"));
				return ETrue;
				}
			else
				{
				INFO_PRINTF1(KAlarmExpireError);
				return EFalse;
				}
			}
		}
	}

/** Verifies that the alarm has been deleted from the alarm server 
@return ETrue if alarm has been deleted, other EFalse
*/
TBool CTestCITAlarmServer::CheckIfAlarmDeleted()
	{
	TTime now;
		
	TBuf<MAX_TEXT_MESSAGE> messageLog(0);
	TRequestStatus status;
	TAlarmId returnedAlarmId;
	FOREVER
		{
		iAlarmServerSession.NotifyChange(status,returnedAlarmId);
		User::WaitForRequest(status);
		if	(status.Int() == EAlarmChangeEventAlarmDeletion)
			{
			now.UniversalTime();
						
			TDateTime deleteTime(now.DateTime());
			messageLog.Format(KDateTime, deleteTime.Year(),TInt(deleteTime.Month()),deleteTime.Day(),
				deleteTime.Hour(),deleteTime.Minute(),deleteTime.Second(),deleteTime.MicroSecond());
			FormatLogMessage(_L("Alarm deleted at:- "), messageLog);
			
			return ETrue;
			}
		}
	}
		
/** Validates the definition of alarms
@return ETrue if alarm definition has been set to default values otherwise EFalse
*/
TBool CTestCITAlarmServer::CheckForAlarmDefintion()
	{
	TASShdAlarm alarmDetails;
	
 	TInt err = iAlarmServerSession.GetAlarmDetails(iAlarmId, alarmDetails);
 	
	if(err == KErrNone)
		{
		if (alarmDetails.RepeatDefinition() == EAlarmRepeatDefintionRepeatOnce)
			{
			INFO_PRINTF1(_L("Repeat Definition is as expected : EAlarmRepeatDefintionRepeatOnce"));
			return ETrue;
			}
		else
			{
			ERR_PRINTF1(_L("Repeat definition is not as expected"));
			SetTestStepResult(EFail);
			SetTestStepError(err);
			return EFalse;
			}
		}
	else
		{
		ERR_PRINTF2(_L("GetAlarmDetails Error Code %d"), err);
		SetTestStepResult(EFail);
		SetTestStepError(err);
		return EFalse;
		}
	}
	
/** Adds alarm to the alarm server
@return id of alarm stored in the alarm server
@param the time to set the alarm to expire
*/
TAlarmId CTestCITAlarmServer::AlarmAddL(const TTime& aTime)
	{
	TBuf<MAX_TEXT_MESSAGE> messageLog(0);
	TDateTime time(aTime.DateTime());
	messageLog.Format(KDateTime, time.Year(),TInt(time.Month()),time.Day(),
	time.Hour(),time.Minute(),time.Second(),time.MicroSecond());
	FormatLogMessage(_L("Expiry Time (UTC time) Initially Set to:- "), messageLog);
	
	TASShdAlarm alarm;
	
	INFO_PRINTF1(_L("Adding fixed alarm to alarm server....."));
	
	alarm.Category() = KASCliCategoryClock;
	
	if (iTypeOfAlarm == _L("EAlarmRepeatDefintionRepeatOnce"))
		{
		alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;	
		}
	else
		{
		if (iTypeOfAlarm == _L("EAlarmRepeatDefintionRepeatNext24Hours"))	
			{
			alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatNext24Hours;	
			}
		else
			{
			if (iTypeOfAlarm == _L("EAlarmRepeatDefintionRepeatDaily"))	
				{
				alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatDaily;	
				}
			}
		}
	
	alarm.SoundName() = _L("Arf Arf Woop");
	alarm.SetUtcNextDueTime(aTime);

	TInt err = iAlarmServerSession.AlarmAdd(alarm);
	INFO_PRINTF2(_L("err = %d - after alarm add....."), err);

	TAlarmId id;

	if (err!=KErrNone)
		{
		id = KNullAlarmId;
		}
	else
		{
		id = alarm.Id();
	
		if (id==KNullAlarmId)
			{
			id = KNullAlarmId;
			}	
		else
			{
			TAlarmStatus status1;
			err = iAlarmServerSession.GetAlarmStatus(id, status1);
			INFO_PRINTF2(_L("err = %d - after alarm status....."), err);
			if (status1 == EAlarmStatusDisabled ) // default value should be EAlarmStatusEnabled
				{
				id = KNullAlarmId;
				}
			else
				{
				INFO_PRINTF1(_L("Checking if alarm is a fixed alarm....."));
				TBool isFloating = alarm.IsFloating();
				if (isFloating)
					{
					INFO_PRINTF1(KAlarmCreatedError);
					id = KNullAlarmId;
					}
				else
					{
					INFO_PRINTF1(_L("Fixed alarm added"));
					
					//Sets type of alarm (timed or day)
					TBuf<90> field(0);
					TAlarmDayOrTimed dayOrTimed;
	
					field = KAlarmType;
		
					TPtrC ptrAlarmType = GetStringFromIni(field);
	
					if (ptrAlarmType != KNullDesC)
						{
						if (ptrAlarmType == _L("timed"))
							{
							TInt err = iAlarmServerSession.SetAlarmDayOrTimed(id, EASShdAlarmTypeTimed);
							TESTL(err == KErrNone);
							err =  iAlarmServerSession.GetAlarmDayOrTimed(id, dayOrTimed);
							TESTL(err == KErrNone && dayOrTimed == EASShdAlarmTypeTimed);
							}
						else
							{
							TInt err = iAlarmServerSession.SetAlarmDayOrTimed(id, EASShdAlarmTypeDay);
							TESTL(err == KErrNone);
							err = iAlarmServerSession.GetAlarmDayOrTimed(id, dayOrTimed);
							TESTL(err == KErrNone && dayOrTimed == EASShdAlarmTypeDay);
							}
						}
	
					TInt err1 = iAlarmServerSession.GetAlarmDayOrTimed(id, dayOrTimed);	
	
					TESTL(err1 == KErrNone && dayOrTimed == EASShdAlarmTypeTimed);
	
					TTime aTime1 = alarm.NextDueTime();
		
					TDateTime aExpiryTime(aTime1.DateTime());
					messageLog.Format(KDateTime, aExpiryTime.Year(),TInt(aExpiryTime.Month()),aExpiryTime.Day(),
					aExpiryTime.Hour(),aExpiryTime.Minute(),aExpiryTime.Second(),aExpiryTime.MicroSecond());
					FormatLogMessage(_L("Expiry Time (UTC time) Initially Set to:- "), messageLog);
			
					messageLog.Zero();
					}
				}
			}
		}
		
	return id;
	}
	
/** Validates the next scheduled alarm time
@return ETrue if next scheduled expiry alarm time is as expected otherwise EFalse
*/
TBool CTestCITAlarmServer::CheckNextExpiryTimeL()
	{
	iField = KExpectedNextExpiryTime;
	TPtrC ptrNextExpiryTime = GetStringFromIni(iField);
	
	if(ptrNextExpiryTime == KNullDesC)
		{
		ERR_PRINTF1(_L("Expected Next Expiry Time not provided in test data"));
		SetTestStepResult(EFail);
		}
		
	TTime nextexpectedExpiryTime;
	nextexpectedExpiryTime = FormatDateTime(ptrNextExpiryTime);

	TASShdAlarm alarmDetails;
	TRAPD(err, iAlarmServerSession.GetAlarmDetails(iAlarmId, alarmDetails));
	if (err == KErrNone)
		{
		TTimeIntervalSeconds secondsDifference;
		TInt difference;
			
		TTime nextDueTime = alarmDetails.NextDueTime();
		
		TBuf<MAX_TEXT_MESSAGE> messageLog(0);

		TDateTime nextduedatetime(nextDueTime.DateTime());
		messageLog.Format(KDateTime, nextduedatetime.Year(),TInt(nextduedatetime.Month()),nextduedatetime.Day(),
			nextduedatetime.Hour(),nextduedatetime.Minute(),nextduedatetime.Second(),nextduedatetime.MicroSecond());
		FormatLogMessage(_L("Next Expiry Time (UTC time) Set to:- "), messageLog);

		User::LeaveIfError(nextexpectedExpiryTime.SecondsFrom(nextDueTime, secondsDifference));
		difference = secondsDifference.Int();
		if (difference < 5 && difference > -5)
			{
			INFO_PRINTF1(_L("Next Due Time At Expected Time"));
			return ETrue;
			}
		else
			{
			INFO_PRINTF1(KNextDueTimeError);
			return EFalse;
			}
		}
	else
		{
		ERR_PRINTF2(_L("GetAlarmDetails Error Code %d"), err);
		SetTestStepResult(EFail);
		SetTestStepError(err);
		return EFalse;
		}
	}
	
