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

#include "TestUTCChangeUntil.h"
#include "TestCITAlarmServerSuiteDefs.h"

typedef CArrayFixFlat<TASCliSoundPlayDefinition> CPlayIntervalArray;

CTestUTCChangeUntil::~CTestUTCChangeUntil()
/**
 * Destructor
 */
	{
	}

CTestUTCChangeUntil::CTestUTCChangeUntil()
/**
 * Constructor
 */
	{
	SetTestStepName(KTestUTCChangeUntil);
	}

TVerdict CTestUTCChangeUntil::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	INFO_PRINTF1(_L("TEST 2: Test UTC Change for Non-Notifying and Notifying Alarms (UNTIL - GLOBAL SILENT)"));
	SetTestStepResult(EPass);
	return TestStepResult();
	}

TVerdict CTestUTCChangeUntil::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
    SetTestStepResult(EFail);

    //Connects to Alarm Server
	ConnectToAlarmServer();
    CleanupClosePushL(iAlarmServerSession);

    TestClearStoreL();

    TBool success = ExecuteTestL(); // Create alarm, info taken from text file

    if (success==EFalse)
    	{
    	return TestStepResult();
    	}

    CleanupStack::PopAndDestroy(); //Alarm Server Session

    SetTestStepResult(EPass);

	return TestStepResult();
	}

TVerdict CTestUTCChangeUntil::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	CloseAlarmServer();
	INFO_PRINTF1(_L("Completed TEST 2"));
	return TestStepResult();
	}

/** Executes Alarm Server Test
** Set Alarm to notify, Set alarm sounds silent until Change UTC offset before or after alarm expiry time.
** Sound start and stop should be as expected
@return True or False indicating the result of the alarm server test
*/
TBool CTestUTCChangeUntil::ExecuteTestL()
	{
	SetSystemTimeL();

	SetInitalUtcOffsetL();

	SetAlarmL();

	// Set Global silent peroid duration in minutes (SetAlarmSoundsSilentFor)
	SetSilentUntilL();

	// Determine wether to change utc before or after alarm expires
	GetWaitToNotify();

	//Get New Offset and set
	iUntilExpired = EFalse;
	iExpiryGreaterUntil = CheckExpGreaterUntilL(iInitialUtc);
	// wait for alarm to reach expiry time, then change utc offset, then wait for sound start and stop
	if (iWaitToNotify)
		{
		if (!WaitToNotifyBeforeChangeL())
			{
			return EFalse;
			}
		}

	ChangeUtcOffset();

	// Let the Alarm Server react to the UTC change
	User::After(KTimeToWait);

	iNow.HomeTime();

	// silent time should have not changed,after utc change
	if (iNow<=iSilentUntil) // If present utc time is greater then until time, then we dont check
		{                // silent period as it will crash (getuntil crashes, as its time has passed
		iNow.UniversalTime();
		if (iNow<=iUTCUntil)
			{
			if (!CheckSilentUntilL())
				{
				return EFalse;
				}
			}
		}

	// wait for alarm to expire after utc change and wait for sound start and stop
	if ((iWaitToNotify == EFalse) && (iMoveUtc != iInitialUtc))
		{
		if(!ExecuteNonNotifyingAlarmL(iMoveUtc))
			{
			return EFalse;
			}
		}

	if (iWaitToNotify && (iMoveUtc > 0))
		{
		if (!CheckStartAndStopL()) // Alarm state should be notifying.  Verifies if alarm sounds are resumed
			{
			return EFalse;
			}
		}

	if (iWaitToNotify && (iMoveUtc < 0))
		{
		if (!CheckAlarmGlobalSoundL()) // Alarm should be notifying.  Verify if alarms sound state is as expected
			{
			return EFalse;
			}
		}

	if (iWaitToNotify && (iMoveUtc == 0))
		{
		if(!CheckAlarmStateSoundStateUntilExpiredL()) // Alarm should be notifying.  Validate sound state.
			{
			return EFalse;
			}
		}

	return ETrue;
	}

/** Waits for alarm to start playing sounds
@return True or False indicating if the sounds started playing at the correct time.
*/
TBool CTestUTCChangeUntil::WaitAndCheckSoundPlayStartL()
    {
    TBuf<MAX_TEXT_MESSAGE> messageLog(0);
	TRequestStatus status;
	TAlarmId returnedAlarmId;
	FOREVER
		{
		iAlarmServerSession.NotifyChange(status,returnedAlarmId);
		User::WaitForRequest(status);
		if	(status.Int() == EAlarmChangeEventSoundPlaying)
			{
			iNow.UniversalTime();

			TDateTime aStartTime(iNow.DateTime());
			messageLog.Format(KDateTime, aStartTime.Year(),TInt(aStartTime.Month()),aStartTime.Day(),
					aStartTime.Hour(),aStartTime.Minute(),aStartTime.Second(),aStartTime.MicroSecond());
			FormatLogMessage(_L("Sound started (UTC time) to play at:- "), messageLog);

			iNow.HomeTime();

			TDateTime now(iNow.DateTime());

			TTimeIntervalMicroSeconds microsecond = now.MicroSecond();
			iNow = iNow - microsecond;

			iUntilExpired = iNow>=iSilentUntil;
			TTimeIntervalSeconds secondsDifference;
			TInt difference;

			if (iNow <= iSilentUntil)
				{
				TTimeIntervalSeconds secondsDifference;
				User::LeaveIfError(iNow.SecondsFrom(iSilentUntil, secondsDifference));
				difference = secondsDifference.Int();
				if (difference < KDelay6sec && difference > (KDelay6sec * -1))
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
			else
				{
				iNow.UniversalTime();
				User::LeaveIfError(iNow.SecondsFrom(iExpiryTime, secondsDifference));
				difference = secondsDifference.Int();
				if (difference < KDelay6sec && difference > (KDelay6sec * -1))
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
    }

/** Waits for alarm to start/stopping playing sounds
@return True or False indicating if alarm playing/stopping sounds at the correct times.
*/
TBool CTestUTCChangeUntil::CheckStartAndStopL()
	{
	if(!WaitAndCheckSoundPlayStartL())
		{
		return EFalse;
		}

	if (!VerifyAlarmSoundStateL(EAlarmGlobalSoundStateOn, EFalse))
		{
		return EFalse;
		}

	if(!WaitAndCheckSoundPlayStopL())
		{
		return EFalse;
		}
	return ETrue;
	}

/** Waits for alarm to start/stopping playing sounds
@return True or False indicating if alarm playing/stopping sounds at the correct times.
*/
TBool CTestUTCChangeUntil::ExecuteNonNotifyingAlarmL(TInt aMoveUTC)
	{
	if (!WaitAndCheckExpiredAlarmL())
		{
		return EFalse;
		}

	if(aMoveUTC<0)
		{
		aMoveUTC = -1;
		}
	else
		{
		if (aMoveUTC > 0)
			{
			aMoveUTC = +1;
			}
		}

	switch(aMoveUTC)
		{
		case (EAlarmSilenedFuture):

			if (!VerifyAlarmSoundStateL(EAlarmGlobalSoundStateOff, ETrue)) // 1 hour or more for sounds to play. Need to check if sounds silenced
				{
				return EFalse;
				}

			break;

		case (EAlarmWaitStartStop):

			if (!CheckStartAndStopL()) // Verify start and stop alarm sounds times
				{
				return EFalse;
				}

			break;

		case (EAlarmExpired):

			if (!CheckAlarmStateStartStopOffOnL()) // Alarm state should be notifying. Alarm sounds should be temporarily silenced
				{
				return EFalse;
				}

			break;

		default:

			break;
		}

	return ETrue;
	}

/** Validates if alarm is notifying, and if alarms are temporarily silenced, and if not, validate the start/stop sounds timings
@return True or False indicating if the sounds are temporarily silenced
*/
TBool CTestUTCChangeUntil::CheckAlarmStateStartStopOffOnL()
	{
	iNow.HomeTime();

	TTimeIntervalMinutes minutesDifference;
	User::LeaveIfError(iSilentUntil.MinutesFrom(iNow, minutesDifference));
	TInt difference = minutesDifference.Int();

	if (difference < KDelay6sec)
		{
		if(!CheckAlarmStatesL(EAlarmStateNotifying))
			{
			return EFalse;
			}

		if (!CheckStartAndStopL())
			{
			return EFalse;
			}
		}
	else
		{
		if (!VerifyAlarmSoundStateL(EAlarmGlobalSoundStateOff, ETrue))
			{
			return EFalse;
			}
		}

	return ETrue;
	}

/** Validates if alarm is notifying, and validates the global sound state
@return True or False indicating if the alarm is notifying and the global sound state as expected
*/
TBool CTestUTCChangeUntil::CheckAlarmGlobalSoundL()
	{
	if (iExpiryGreaterUntil)
		{
		if (!VerifyAlarmSoundStateL(EAlarmGlobalSoundStateOn, EFalse))
			{
			return EFalse;
			}
		iUntilExpired = ETrue;
		}
	else
		{
		if (!VerifyAlarmSoundStateL(EAlarmGlobalSoundStateOff, ETrue))
			{
			return EFalse;
			}
		}

	return ETrue;
	}

/** Validates if alarm is notifying, and validates the global sound state.  Also validates the start/stop sound timings
@return True or False indicating if the alarm is notifying and the global sound state as expected
		Also indicating that the sounds start/stop are as expected
*/
TBool CTestUTCChangeUntil::CheckAlarmStateSoundStateUntilExpiredL()
	{
	if(!CheckAlarmStatesL(EAlarmStateNotifying))
		{
		return EFalse;
		}

	iNow.HomeTime();

	TTimeIntervalMinutes minutesDifference;
	User::LeaveIfError(iSilentUntil.MinutesFrom(iNow, minutesDifference));
	TInt difference = minutesDifference.Int();

	if (difference < KDelay6sec)
		{
		if (!CheckStartAndStopL())
			{
			return EFalse;
			}
		}
	else
		{
		if (iUntilExpired)
			{
			if (!CheckStartAndStopL())
				{
				return EFalse;
				}
			}
		else
			{
			if (!VerifyAlarmSoundStateL(EAlarmGlobalSoundStateOff, ETrue))
				{
				return EFalse;
				}
			}
		}

	return ETrue;
	}


