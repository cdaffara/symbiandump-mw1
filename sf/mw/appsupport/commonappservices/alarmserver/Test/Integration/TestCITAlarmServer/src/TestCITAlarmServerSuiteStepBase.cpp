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

#include "TestCITAlarmServerSuiteStepBase.h"
#include "TestCITAlarmServerSuiteDefs.h"

typedef CArrayFixFlat<TASCliSoundPlayDefinition> CPlayIntervalArray;

CTestCITAlarmServerSuiteStepBase::~CTestCITAlarmServerSuiteStepBase()
	{
	}

CTestCITAlarmServerSuiteStepBase::CTestCITAlarmServerSuiteStepBase()
	{
	}

/** Connects to the Alarm Server
*/
void CTestCITAlarmServerSuiteStepBase::ConnectToAlarmServer()
	{
	//Connects to Alarm Server
	INFO_PRINTF1(_L("Connecting to Alarm Server....."));
    TInt err = iAlarmServerSession.Connect();

    if (err == KErrNone)
    	{
    	INFO_PRINTF1(_L("Error code == KErrNone"));
    	INFO_PRINTF1(_L("Connected to Alarm Server"));
    	}
    else
    	{
    	INFO_PRINTF1(_L("Error code != KErrNone"));
    	INFO_PRINTF1(_L("Not Connected to Alarm Server"));
    	}
	}

/** Closes connection to the alarm server
*/
void CTestCITAlarmServerSuiteStepBase::CloseAlarmServer()
	{
	INFO_PRINTF1(_L("Closing Alarm Server Session....."));
	iAlarmServerSession.Close();
	}

/** Formats the string consisting of a date into a TTime
@return TTime resulting from formatting the string
@param The string to convert into a TTime
*/
TTime CTestCITAlarmServerSuiteStepBase::FormatDateTime(const TDesC& aParamString)
	{
	// Format of buffer to construct a TTime is YYYYMMDD:HHMMSS (15 characters).
	// TTime uses zero-based values for month and day - which is confusing for scripting.
	// In our script, we use actual month and day numbers to make things simpler so we
	// must modify the string here to take account of this.
	TBuf<32> buf;
	TInt m, d;

	buf.Zero();
	buf.Copy(aParamString.Left(4));		// The year
	TLex lexMonth = aParamString.Mid(4, 2);
	lexMonth.Val(m);
	TLex lexDay = aParamString.Mid(6, 2);
	lexDay.Val(d);
	buf.AppendFormat(_L("%02d%02d"), m - 1, d - 1);	// The month and day
	buf.Append(aParamString.Right(7));

	return TTime(buf);
	}

/** Validates current alarm state
@param The excepted alarm state of the alarm
@return ETrue if alarm has the expected state, otherwise EFalse
*/
TBool CTestCITAlarmServerSuiteStepBase::CheckAlarmStatesL(TAlarmState aExpectedState)
	{
	TBuf<12> stateBuf(0);

	if (aExpectedState == EAlarmStateNotifying)
		{
		stateBuf= KNotifying;
		}
	else
		{
		stateBuf= KQueued;
		}

	TBuf<MAX_TEXT_MESSAGE> messageLog(0);
	TASShdAlarm alarm;
	iAlarmServerSession.GetAlarmDetails(iAlarmId, alarm);
	TAlarmState state = alarm.State();
	if (state != aExpectedState) // alarm state should be as expected
	    {
	    messageLog.Format(KAlarmStateMessage);
		FormatLogMessage(_L("Alarm State"),  messageLog);
		return EFalse;
	    }
	else
		{
	    FormatLogMessage(_L("Alarm found to be in correct state:-"),  stateBuf);
		}

	messageLog.Zero();
	return ETrue;
	}

/** Converts local until time to universal time according to utc offset
@param The utc offset to be used when coverting
@return The converted until utc time
*/
TTime CTestCITAlarmServerSuiteStepBase::CalculateUTCUntilL(TInt aInitialUTC)
	{
	TInt hours;
	TTime getSilentUntil;
	User::LeaveIfError(iAlarmServerSession.GetAlarmSoundsSilentUntil(getSilentUntil));

	// converts local get until time to universal time, according to UTC offset
	hours = aInitialUTC / KSecondsInHour;
	return(getSilentUntil -= TTimeIntervalHours(hours));
	}

/** Determines wether expiry time or until time will be reached first
@param The utc offset to be used when coverting times
@return ETrue, if expiry time is greater than until time, otherwise EFalse
*/
TBool CTestCITAlarmServerSuiteStepBase::CheckExpGreaterUntilL(TInt aInitialUTC)
	{
	TInt hours;
	TBuf<MAX_TEXT_MESSAGE> messageLog(0);
	TTime getSilentUntil;
	User::LeaveIfError(iAlarmServerSession.GetAlarmSoundsSilentUntil(getSilentUntil));

	// converts local get until time to universal time, according to UTC offset
	if (aInitialUTC < 0)
		{
		aInitialUTC = aInitialUTC * (-1);
		hours = aInitialUTC / KSecondsInHour;
		getSilentUntil += TTimeIntervalHours(hours);
		}
	else
		{
		hours = aInitialUTC / KSecondsInHour;
		getSilentUntil -= TTimeIntervalHours(hours);
		}

	return(iExpiryTime >= getSilentUntil);
	}

/** Validates global sound state
@param The excepted global sound state
@return ETrue if the global sound state has the expected global sound state, otherwise EFalse
*/
TBool CTestCITAlarmServerSuiteStepBase::CheckGlobalSoundState(TAlarmGlobalSoundState aGlobalSoundState)
	{
	TBuf<MAX_TEXT_MESSAGE> messageLog(0);
	if (aGlobalSoundState == EAlarmGlobalSoundStateOff)
		{
		messageLog.Format(KGlobalSoundState, &KGlobalSoundStateOff);
		}
	else
		{
		messageLog.Format(KGlobalSoundState, &KGlobalSoundStateOn);
		}

	TAlarmGlobalSoundState soundState;

	TInt err = iAlarmServerSession.GetAlarmSoundState(soundState);	 // get sound state

	// check if sound state matches expected state
	if (err == KErrNone && soundState == aGlobalSoundState)
		{
		FormatLogMessage(_L("Alarm found to be in correct global sound state:-"),  messageLog);
		return ETrue;
		}
	else
		{
		FormatLogMessage(_L("ERROR : Alarm found NOT to be in correct global sound state:-"),  messageLog);
		return EFalse;
		}
	}

/** Calculates the expected time of the alarm sounds to stop playing
@param The time when the sound started to play
@return TTime indicating the time that the sounds should stop playing
*/
TTime CTestCITAlarmServerSuiteStepBase::CalculateSoundStopL(TTime iNow)
	{
	CPlayIntervalArray* array = new(ELeave) CPlayIntervalArray(20);
	CleanupStack::PushL(array);

	// Get default sound intervals
	iAlarmServerSession.GetAlarmPlayIntervalsL(*array);

	TInt i = 0;
	const TASCliSoundPlayDefinition& item = array -> At(i);

	TTimeIntervalSeconds duration = item.Duration();
	TInt x = duration.Int();
	CleanupStack::PopAndDestroy(array);

	TDateTime aNow(iNow.DateTime());

	TTimeIntervalMicroSeconds microsecond = aNow.MicroSecond();
	TTimeIntervalSeconds second = aNow.Second();
	iNow = iNow - microsecond; // remove microsecond as this is a delay in the test code

	iNow = iNow + duration;

	return iNow;
	}

/** Formats 2 strings, and prints it out
@param the string to print out
@param the 2nd string to print out
*/
void CTestCITAlarmServerSuiteStepBase::FormatLogMessage(const TDesC& aLogMessage, const TDesC& aDescriptor)
	{
	TBuf<MAX_TEXT_MESSAGE> message(0);
	message.Format(_L("%S %S"), &aLogMessage, &aDescriptor);
	INFO_PRINTF1(message);
	}

/** Adds an alarm to the alarm server session
@param the expiry time of alarm to set
@return the alarm id of the newly created alarm
*/
TAlarmId CTestCITAlarmServerSuiteStepBase::AddAlarmL(const TTime& aTime)
	{
	TBuf<MAX_TEXT_MESSAGE> messageLog(0);
	TDateTime time(aTime.DateTime());
	messageLog.Format(KDateTime, time.Year(),TInt(time.Month()),time.Day(),
	time.Hour(),time.Minute(),time.Second(),time.MicroSecond());
	FormatLogMessage(_L("Expiry Time (UTC time) Initially Set to:- "), messageLog);

	TASShdAlarm alarm;

	INFO_PRINTF1(_L("Adding fixed alarm to alarm server....."));

	alarm.Category() = KASCliCategoryClock;
	alarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;
	alarm.SoundName() = _L("Arf Arf Woop");
	alarm.SetUtcNextDueTime(aTime);

	TInt err = iAlarmServerSession.AlarmAdd(alarm);
	INFO_PRINTF2(_L("err = %d - after alarm add....."), err);

	if (err!=KErrNone)
		{
		return KNullAlarmId;
		}

	const TAlarmId id = alarm.Id();

	if (id==KNullAlarmId)
		{
		return KNullAlarmId;
		}

	TAlarmStatus status1;
	err = iAlarmServerSession.GetAlarmStatus(id, status1);
	INFO_PRINTF2(_L("err = %d - after alarm status....."), err);
	if (status1 == EAlarmStatusDisabled ) // default value should be EAlarmStatusEnabled
		{
		return KNullAlarmId;
		}

	INFO_PRINTF1(_L("Checking if alarm is a fixed alarm....."));
	TBool isFloating = alarm.IsFloating();
	if (isFloating)
		{
		INFO_PRINTF1(KAlarmCreatedError);
		return KNullAlarmId;
		}
	else
		{
		INFO_PRINTF1(_L("Fixed alarm added"));
		}

	//Sets type of alarm (timed or day)
	TBuf<KMaxNumberOfCharac> field(0);
	TAlarmDayOrTimed dayOrTimed;

	field = KAlarmType;

	TPtrC ptrAlarmType = GetStringFromIni(field);

	if (ptrAlarmType != KNullDesC)
		{
		if (ptrAlarmType == _L("timed"))
			{
			TRAPD(err, iAlarmServerSession.SetAlarmDayOrTimed(KNullAlarmId, EASShdAlarmTypeTimed));
			TESTL(err == KErrNone);
			TRAP(err, iAlarmServerSession.GetAlarmDayOrTimed(id, dayOrTimed));
			TESTL(err == KErrNone && dayOrTimed == EASShdAlarmTypeTimed);
			}
		else
			{
			TRAPD(err1, iAlarmServerSession.SetAlarmDayOrTimed(KNullAlarmId, EASShdAlarmTypeDay));
			TESTL(err1 == KErrNone);
			TRAP(err1, iAlarmServerSession.GetAlarmDayOrTimed(id, dayOrTimed));
			TESTL(err1 == KErrNone && dayOrTimed == EASShdAlarmTypeDay);
			}
		}

	TRAPD(err1, iAlarmServerSession.GetAlarmDayOrTimed(id, dayOrTimed));

	TESTL(err1 == KErrNone && dayOrTimed == EASShdAlarmTypeTimed);

	TTime aTime1 = alarm.NextDueTime();

	TDateTime aExpiryTime(aTime1.DateTime());
	messageLog.Format(KDateTime, aExpiryTime.Year(),TInt(aExpiryTime.Month()),aExpiryTime.Day(),
	aExpiryTime.Hour(),aExpiryTime.Minute(),aExpiryTime.Second(),aExpiryTime.MicroSecond());
	FormatLogMessage(_L("Expiry Time (UTC time) Initially Set to:- "), messageLog);

	messageLog.Zero();

	return id;
	}

/** Verifies if SetAlarmSoundsSilentFor and GetAlarmSoundsSilentUntil matches
@param silent duration in minutes
@param present time
@return ETrue if SetAlarmSoundsSilentFor and GetAlarmSoundsSilentUntil matches, otherwise EFalse
*/
TBool CTestCITAlarmServerSuiteStepBase::CheckSilentForL(TTimeIntervalMinutes aSilentFor,TTime aNow)
	{
	TTime getSilentUntil;
	User::LeaveIfError(iAlarmServerSession.GetAlarmSoundsSilentUntil(getSilentUntil));
 	TBuf<MAX_TEXT_MESSAGE> messageLog(0);

	TTimeIntervalMinutes silentMinutes;
	User::LeaveIfError(getSilentUntil.MinutesFrom(aNow, silentMinutes));

	TBuf<32> buf;
	getSilentUntil.FormatL(buf, _L("%H:%T:%S"));

	if (aSilentFor != silentMinutes)
		{
		INFO_PRINTF1(_L("SetAlarmSoundsSilentFor and GetAlarmSoundsSilentUntil doesn't match!"));
		INFO_PRINTF2(_L("SetAlarmsSoundsSilentFor: %d min"), aSilentFor.Int());
		INFO_PRINTF3(_L("GetAlarmsSoundsSilentUntil: %d min (%s local)"), silentMinutes.Int(), buf.PtrZ());

		return EFalse;
		}

	INFO_PRINTF1(_L("SetAlarmSoundsSilentFor and GetAlarmSoundsSilentUntil matches."));
	INFO_PRINTF2(_L("SetAlarmsSoundsSilentFor: %d min"), aSilentFor.Int());
	INFO_PRINTF3(_L("GetAlarmsSoundsSilentUntil: %d min (%s local)"), silentMinutes.Int(), buf.PtrZ());

	return ETrue;
	}

/** Calculates the silent duration from until time
@return TTime indicating the silent duration in minutes
*/
TTimeIntervalMinutes CTestCITAlarmServerSuiteStepBase::CalculateSilentForL()
	{
	TTime getSilentUntil;
	User::LeaveIfError(iAlarmServerSession.GetAlarmSoundsSilentUntil(getSilentUntil));
 	TBuf<MAX_TEXT_MESSAGE> messageLog(0);

	TTimeIntervalMinutes silentMinutes;
	User::LeaveIfError(getSilentUntil.MinutesFrom(iNow, silentMinutes));

	return silentMinutes;
	}

/** Verifies the until time is constant regardless of utc change
@return ETrue, if until time is constant, otherwise EFalse
*/
TBool CTestCITAlarmServerSuiteStepBase::CheckSilentUntilL()
	{
	TTime getSilentUntil;
	TInt ret = iAlarmServerSession.GetAlarmSoundsSilentUntil(getSilentUntil);

	User::LeaveIfError(ret);

	TBuf<10> buf;
	iSilentUntil.FormatL(buf, _L("%H:%T:%S"));
	// until time should not change regardless of system or UTC offset change
	if (iSilentUntil != getSilentUntil)
		{
		INFO_PRINTF1(_L("SetAlarmSoundsSilentUntil and GetAlarmSoundsSilentUntil doesn't match!"));
		INFO_PRINTF2(_L("SetAlarmsSilentUntil: %s"), buf.PtrZ());

		getSilentUntil.FormatL(buf, _L("%H:%T:%S"));
		INFO_PRINTF2(_L("GetAlarmsSilentUntil: %s"), buf.PtrZ());

		return EFalse;
		}

	INFO_PRINTF1(_L("SetAlarmSoundsSilentUntil and GetAlarmSoundsSilentUntil matches"));
	INFO_PRINTF2(_L("SetAlarmsSilentUntil: %s"), buf.PtrZ());

	getSilentUntil.FormatL(buf, _L("%H:%T:%S"));
	INFO_PRINTF2(_L("GetAlarmsSilentUntil: %s"), buf.PtrZ());

	return ETrue;
	}

/** Clear all alarms in alarm server session
*/
void CTestCITAlarmServerSuiteStepBase::TestClearStoreL()
	{
	// Delete all alarms
	RArray<TAlarmCategory> categories;
	iAlarmServerSession.GetAvailableCategoryListL(categories);

	TInt count = categories.Count();
	for(TInt i = 0; i < count; i++)
		{
		const TAlarmCategory category = categories[i];
		iAlarmServerSession.AlarmDeleteAllByCategory(category, EFalse);
		}
	categories.Close();

	//restore alarm sound
	iAlarmServerSession.SetAlarmSoundState(EAlarmGlobalSoundStateOn);
	}

/** Waits for alarm to expire
@return alarm id of the alarm expiring
*/
TAlarmId CTestCITAlarmServerSuiteStepBase::WaitTimerExpiry()
    {
    TBuf<MAX_TEXT_MESSAGE> messageLog(0);
	TRequestStatus status;
	TAlarmId returnedAlarmId(-1);
	FOREVER
		{
		iAlarmServerSession.NotifyChange(status,returnedAlarmId);
		User::WaitForRequest(status); // waits for change of status

		if ( status.Int() == EAlarmChangeEventTimerExpired && returnedAlarmId==iAlarmId )
		    iNow.UniversalTime();

			TDateTime systemDateTime(iNow.DateTime());
			messageLog.Format(KDateTime, systemDateTime.Year(),TInt(systemDateTime.Month()),systemDateTime.Day(),
					systemDateTime.Hour(),systemDateTime.Minute(),systemDateTime.Second(),systemDateTime.MicroSecond());
			FormatLogMessage(_L("Alarm change event (UTC time) at:- "), messageLog);

			return returnedAlarmId;
		}
    }

/** Waits for alarm to expire
@return ETrue, if alarm expires at the correct time
*/
TBool CTestCITAlarmServerSuiteStepBase::WaitAndCheckExpiredAlarmL()
    {
    TBuf<MAX_TEXT_MESSAGE> messageLog(0);
	TRequestStatus status;
	TAlarmId returnedAlarmId;
	FOREVER
		{
		iAlarmServerSession.NotifyChange(status,returnedAlarmId);
		User::WaitForRequest(status); // waits for change of status

		if (status.Int() == EAlarmChangeEventTimerExpired && returnedAlarmId==iAlarmId)
			{
			iNow.UniversalTime();

			TDateTime systemDateTime(iNow.DateTime());
			messageLog.Format(KDateTime, systemDateTime.Year(),TInt(systemDateTime.Month()),systemDateTime.Day(),
					systemDateTime.Hour(),systemDateTime.Minute(),systemDateTime.Second(),systemDateTime.MicroSecond());
			FormatLogMessage(_L("Alarm change event (UTC time) at:- "), messageLog);

			// The event could be due to timer aborting, so check the state of the alarm .
			TBool alarmQueued = CheckAlarmStatesL(EAlarmStateQueued);
			if (alarmQueued)
				{
				continue;
				}
			else
				{
				TTimeIntervalSeconds secondsDifference;
				User::LeaveIfError(iNow.SecondsFrom(iExpiryTime, secondsDifference));

				TInt difference = secondsDifference.Int();
				// time delay of 10 seconds
				if (difference < KDelay10sec && difference > (KDelay10sec * -1))
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
    }

/** Waits for alarm to stop playing sounds
@return ETrue, if alarm stops playing sounds at the expected time, otherwise EFalse
*/
TBool CTestCITAlarmServerSuiteStepBase::WaitAndCheckSoundPlayStopL()
    {
    TBuf<MAX_TEXT_MESSAGE> messageLog(0);
	TRequestStatus status;
	TAlarmId returnedAlarmId;
	FOREVER
		{
		iAlarmServerSession.NotifyChange(status,returnedAlarmId);
		User::WaitForRequest(status);
		if	(status.Int() == EAlarmChangeEventSoundStopped && returnedAlarmId==iAlarmId)
			{
			iNow.UniversalTime();

			TDateTime systemDateTime(iNow.DateTime());
			messageLog.Format(KDateTime, systemDateTime.Year(),TInt(systemDateTime.Month()),systemDateTime.Day(),
					systemDateTime.Hour(),systemDateTime.Minute(),systemDateTime.Second(),systemDateTime.MicroSecond());
			FormatLogMessage(_L("Sound stopped (UTC time) at:- "), messageLog);


			TDateTime now(iNow.DateTime());
			TTimeIntervalMicroSeconds microsecond = now.MicroSecond();
			iNow = iNow - microsecond;

			TTimeIntervalSeconds secondsDifference;
			User::LeaveIfError(iNow.SecondsFrom(iSoundStop, secondsDifference));
			TInt difference = secondsDifference.Int();
			// time delay of 6 seconds
			if ((difference < KDelay6sec && difference > (KDelay6sec * -1) || difference == (KSecondsInHour * -1)))
				{
				INFO_PRINTF3(KPlayStop,returnedAlarmId,iAlarmId);
				INFO_PRINTF1(_L("Sound Stopped At Expected Time"));
				return ETrue;
				}
			else
		// alarm will stop and then immediately restart as a result of UTC offset change.
				{
				if (difference > KDelay25sec || difference < (KDelay25sec * -1))
					{
					if(!WaitAndCheckSoundPlayStart1L(iNow)) // checks if sounds are restarted mmediately
						{
						return EFalse;
						}

					if(WaitAndCheckSoundPlayStop1L()) // checks if sounds are stopped atfer 30 seconds from
						{							// the point when sounds are first started
						return ETrue;
						}
					else
						{
						return EFalse;
						}
					}
				else  //END if statement
					{
					INFO_PRINTF1(KAlarmSoundStopError);
					return EFalse;
					}
				}
			}
		}
    }

/** Retracts data from ini file
@return The string that contains the data
*/
TPtrC CTestCITAlarmServerSuiteStepBase::GetStringFromIni(TBuf<KMaxNumberOfCharac> aField)
	{
	TPtrC returnedString;

	GetStringFromConfig(ConfigSection(),aField,returnedString);

	return returnedString;
	}

/** Waits for alarm to start playing sounds
@param the current system time
@return ETrue, if alarm starts playing sounds at the expected time, otherwise EFalse
*/
TBool CTestCITAlarmServerSuiteStepBase::WaitAndCheckSoundPlayStart1L(TTime aNow)
	{
	TBuf<MAX_TEXT_MESSAGE> messageLog(0);
	TRequestStatus status;
	TAlarmId returnedAlarmId;
	FOREVER
		{
		iAlarmServerSession.NotifyChange(status,returnedAlarmId);
		User::WaitForRequest(status);
		if	(status.Int() == EAlarmChangeEventSoundPlaying && returnedAlarmId==iAlarmId)
			{
			iNow.UniversalTime();

			TDateTime systemDateTime(iNow.DateTime());
			messageLog.Format(KDateTime, systemDateTime.Year(),TInt(systemDateTime.Month()),systemDateTime.Day(),
					systemDateTime.Hour(),systemDateTime.Minute(),systemDateTime.Second(),systemDateTime.MicroSecond());
			FormatLogMessage(_L("Sound started (UTC time) at:- "), messageLog);

			TDateTime now(iNow.DateTime());
			TTimeIntervalMicroSeconds microsecond = now.MicroSecond();
			iNow = iNow - microsecond;

			TTimeIntervalSeconds secondsDifference;

			User::LeaveIfError(iNow.SecondsFrom(aNow, secondsDifference));
			TInt difference = secondsDifference.Int();
			// sounds are played immediately
			if (difference == 0)
				{
				INFO_PRINTF3(KPlayStart,returnedAlarmId,iAlarmId);
				INFO_PRINTF1(_L("Sound Started At Expected Time"));
				iSoundStop = CalculateSoundStopL(iNow);
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

/** Waits for alarm to stop playing sounds
@return ETrue, if alarm stops playing sounds at the expected time, otherwise EFalse
*/
TBool CTestCITAlarmServerSuiteStepBase::WaitAndCheckSoundPlayStop1L()
    {
    TBuf<MAX_TEXT_MESSAGE> messageLog(0);
	TRequestStatus status;
	TAlarmId returnedAlarmId;
	FOREVER
		{
		iAlarmServerSession.NotifyChange(status,returnedAlarmId);
		User::WaitForRequest(status);
		if	(status.Int() == EAlarmChangeEventSoundStopped && returnedAlarmId==iAlarmId)
			{
			iNow.UniversalTime();

			TDateTime systemDateTime(iNow.DateTime());
			messageLog.Format(KDateTime, systemDateTime.Year(),TInt(systemDateTime.Month()),systemDateTime.Day(),
					systemDateTime.Hour(),systemDateTime.Minute(),systemDateTime.Second(),systemDateTime.MicroSecond());
			FormatLogMessage(_L("Sound stopped (UTC time) at:- "), messageLog);

			TDateTime now(iNow.DateTime());
			TTimeIntervalMicroSeconds microsecond = now.MicroSecond();
			iNow = iNow - microsecond;

			TTimeIntervalSeconds secondsDifference;

			User::LeaveIfError(iNow.SecondsFrom(iSoundStop, secondsDifference));
			TInt difference = secondsDifference.Int();
			// time delay of 6 seconds
			if (difference < KDelay6sec && difference > (KDelay6sec * -1))
				{
				INFO_PRINTF3(KPlayStop,returnedAlarmId,iAlarmId);
				INFO_PRINTF1(_L("Sound Stopped At Expected Time"));
				return ETrue;
				}
			else
				{
				INFO_PRINTF1(KAlarmSoundStopError);
				return EFalse;
				}
			}
		}
    }

/** Waits for alarm to notify, and validates alarm sound state
@return ETrue, if alarm expired at the expected time, otherwise EFalse
*/
TBool CTestCITAlarmServerSuiteStepBase::WaitToNotifyBeforeChangeL()
	{
	if(!WaitAndCheckExpiredAlarmL())
		{
		return EFalse;
		}

	if (iExpiryGreaterUntil) //Sound state should be on, as until time is reached
		{

		if (!VerifyAlarmSoundStateL(EAlarmGlobalSoundStateOn, EFalse))
			{
			return EFalse;
			}

		iUntilExpired = ETrue;
		}
	else          // sound state should be off, as yet not reached until time
		{
		if (!VerifyAlarmSoundStateL(EAlarmGlobalSoundStateOff, ETrue))
			{
			return EFalse;
			}
		}

	return ETrue;
	}

/** Sets system time
*/
void CTestCITAlarmServerSuiteStepBase::SetSystemTimeL()
	{
	TBuf<MAX_TEXT_MESSAGE> messageLog(0);
	TBuf<KMaxNumberOfCharac> field(0);

	// Get the system time and set
	field = KSystemTime;
	TPtrC ptrSystemTime = GetStringFromIni(field);
	if(ptrSystemTime == KNullDesC)
		{
		ERR_PRINTF1(_L("System Time Not Provided"));
		User::Leave(KErrNotFound);
		}

	TTime systemTime;
	systemTime = FormatDateTime(ptrSystemTime);
	iSystemTime = systemTime;
	TInt err1 = User::SetUTCTime(systemTime);

	if (err1 != KErrNone)
		{
		ERR_PRINTF2(_L("System Time Not Set Error Code %d"), err1);
		User::Leave(err1);
		}

	TDateTime systemDateTime(systemTime.DateTime());
	messageLog.Format(KDateTime, systemDateTime.Year(),TInt(systemDateTime.Month()),systemDateTime.Day(),
			systemDateTime.Hour(),systemDateTime.Minute(),systemDateTime.Second(),systemDateTime.MicroSecond());
	FormatLogMessage(_L("System Time Set to:- "), messageLog);
	}

/** Sets utc offset
*/
void CTestCITAlarmServerSuiteStepBase::SetInitalUtcOffsetL()
	{
	TBuf<MAX_TEXT_MESSAGE> messageLog(0);
	TBuf<KMaxNumberOfCharac> field(0);

	// Get inital utc offset and set
	field = KInitialUTC;
	TPtrC ptrInitialUTC = GetStringFromIni(field);

	TLex lex(ptrInitialUTC);
	lex.Val(iInitialUtc);
	//Set utc offset
	User::SetUTCOffset(iInitialUtc);

	iNow.HomeTime();
	iNow = iNow - TTimeIntervalSeconds(1);

	// Get utc offset
	TTimeIntervalSeconds offset = User::UTCOffset();
	if (offset != TTimeIntervalSeconds(iInitialUtc))
		{
		messageLog.Format(KUTCOffsetError);
		FormatLogMessage(_L("UTC Offset "),  messageLog);
		User::Leave(KErrGeneral);
		}

	FormatLogMessage(_L("Initial UTC Offset (in seconds) Set to:-"),ptrInitialUTC);
	}

/** Adds an alarm to the alarm server session
*/
void CTestCITAlarmServerSuiteStepBase::SetAlarmL()
	{
	TBuf<MAX_TEXT_MESSAGE> messageLog(0);
	TBuf<KMaxNumberOfCharac> field(0);

	//Get and Set expiry Time
	field = KAlarmExpires;
	TPtrC ptrAlarmExpires = GetStringFromIni(field);

	iExpiryTime = FormatDateTime(ptrAlarmExpires);
	iAlarmId = AddAlarmL(iExpiryTime);

	if (iAlarmId == KNullAlarmId)
		{
		messageLog.Format(KAddAlarmError);
		FormatLogMessage(_L("Alarm "),  messageLog);
		}
	}

/** Sets silent period for in minutes
*/
void CTestCITAlarmServerSuiteStepBase::SetSilentForL()
	{
	TBuf<KMaxNumberOfCharac> field(0);
	TInt silentFor(0);

	// set silent period in minutes
	field = KSilentFor;
	TPtrC ptrSilentFor = GetStringFromIni(field);

	TLex lex2(ptrSilentFor);
	lex2.Val(silentFor);

	iSilentFor = TTimeIntervalMinutes(silentFor);

	INFO_PRINTF2(_L("Setting Alarm Sounds Silent For: %d min"), silentFor);
	User::LeaveIfError(iAlarmServerSession.SetAlarmSoundsSilentFor(TTimeIntervalMinutes(silentFor)));
	iAlarmServerSession.GetAlarmSoundsSilentUntil(iSilentUntil);

	if (!CheckSilentForL(iSilentFor,iNow))
		{
		ERR_PRINTF1(_L("SetAlarmSoundsSilentFor and GetAlarmSoundsSilentUntil doesn't match!"));
		}
	}

/** Changes utc offset
*/
void CTestCITAlarmServerSuiteStepBase::ChangeUtcOffset()
	{
	TBuf<MAX_TEXT_MESSAGE> messageLog(0);
	TBuf<KMaxNumberOfCharac> field(0);

	field = KMoveUTC;
	TPtrC ptrMoveUTC = GetStringFromIni(field);

	TLex lex3(ptrMoveUTC);
	lex3.Val(iMoveUtc);
	FormatLogMessage(_L("Updating UTC Offset (in seconds) to:-"),ptrMoveUTC);
	//change utc offset
	User::SetUTCOffset(iMoveUtc);
	}

/** Determines wether to wait for alarm to notify before or after utc change
*/
void CTestCITAlarmServerSuiteStepBase::GetWaitToNotify()
	{
	TBuf<KMaxNumberOfCharac> field(0);

	field = KWaitToNotify;
	TPtrC ptrWaitNotify = GetStringFromIni(field);

	if(ptrWaitNotify != KNullDesC)
		{
		iWaitToNotify = ETrue;
		}
	}

/** Sets silent period until
*/
void CTestCITAlarmServerSuiteStepBase::SetSilentUntilL()
	{
	TBuf<MAX_TEXT_MESSAGE> messageLog(0);
	TBuf<KMaxNumberOfCharac> field(0);

	// set silent period in minutes
	field = KSilentUntil;
	TPtrC ptrSilentUntil = GetStringFromIni(field);
	iSilentUntil = FormatDateTime(ptrSilentUntil);

	TDateTime aUntilDateTime(iSilentUntil.DateTime());
	messageLog.Format(KDateTime, aUntilDateTime.Year(),TInt(aUntilDateTime.Month()),aUntilDateTime.Day(),
		aUntilDateTime.Hour(),aUntilDateTime.Minute(),aUntilDateTime.Second(),aUntilDateTime.MicroSecond());
	FormatLogMessage(_L("Setting Alarm Sounds Silent Until:- "), messageLog);

	User::LeaveIfError(iAlarmServerSession.SetAlarmSoundsSilentUntil(iSilentUntil));
	iUTCUntil = CalculateUTCUntilL(iInitialUtc);
	if (!CheckSilentUntilL())
		{
		ERR_PRINTF1(_L("SetAlarmSoundsSilentUntil return Time not as expected"));
		}
	}

/** Changes system time
*/
void CTestCITAlarmServerSuiteStepBase::ChangeSystemTimeL()
	{
	TBuf<MAX_TEXT_MESSAGE> messageLog(0);
	TBuf<KMaxNumberOfCharac> field(0);
	field = KMoveSystemTime;
	TPtrC ptrMoveSystemTime = GetStringFromIni(field);

	iMoveSystemTime = FormatDateTime(ptrMoveSystemTime);

	TDateTime systemDateTime1(iMoveSystemTime.DateTime());
	messageLog.Format(KDateTime, systemDateTime1.Year(),TInt(systemDateTime1.Month()),systemDateTime1.Day(),
			systemDateTime1.Hour(),systemDateTime1.Minute(),systemDateTime1.Second(),systemDateTime1.MicroSecond());
	FormatLogMessage(_L("Updating System Time to:- "), messageLog);

	TInt err2 = User::SetUTCTime(iMoveSystemTime);

	if (err2 != KErrNone)
		{
		ERR_PRINTF2(_L("User::SetUTCTime Error Code %d"), err2);
		User::Leave(err2);
		}
	}

/** Validates alarm state, global sound state and temporarily silenced status
@param aAlarmState the expected alarm state
@param aSoundsSilenced indicates wether alarm sounds has been temporarily silenced
@return ETrue, if all alarm states are as expected, otherwise EFalse
*/
TBool CTestCITAlarmServerSuiteStepBase::VerifyAlarmSoundStateL(TAlarmGlobalSoundState aAlarmState, TBool aSoundsSilenced)
	{
	if(!CheckAlarmStatesL(EAlarmStateNotifying))
		{
		return EFalse;
		}

	if (!CheckGlobalSoundState(aAlarmState))
		{
		return EFalse;
		}

	if (aSoundsSilenced)
		{
		if(!iAlarmServerSession.AlarmSoundsTemporarilySilenced())
			{
			INFO_PRINTF1(KAlarmSilencedError);
			return EFalse;
			}
		else
			{
			INFO_PRINTF1(_L("Alarm sounds has been Temporarily Silenced"));
			INFO_PRINTF1(_L("Need to wait until sounds are allowed"));
			}
		}
	else
		{
		if(!iAlarmServerSession.AlarmSoundsTemporarilySilenced())
			{
			INFO_PRINTF1(_L("Alarm sounds has not been Temporarily Silenced"));
			}
		else
			{
			INFO_PRINTF1(KAlarmSilencedError1);
			return EFalse;
			}
		}

	return ETrue;
	}

