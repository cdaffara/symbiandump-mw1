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

#include "TestSystemChangeUntil.h"
#include "TestCITAlarmServerSuiteDefs.h"

typedef CArrayFixFlat<TASCliSoundPlayDefinition> CPlayIntervalArray;

CTestSystemChangeUntil::~CTestSystemChangeUntil()
/**
 * Destructor
 */
	{
	}

CTestSystemChangeUntil::CTestSystemChangeUntil()
/**
 * Constructor
 */
	{
	SetTestStepName(KTestSystemChangeUntil);
	}

TVerdict CTestSystemChangeUntil::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	INFO_PRINTF1(_L("TEST 4: Test System Change for Non-Notifying and Notifying Alarms (UNTIL - GLOBAL SILENT)"));
	SetTestStepResult(EPass);
	return TestStepResult();
	}

TVerdict CTestSystemChangeUntil::doTestStepL()
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

TVerdict CTestSystemChangeUntil::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	CloseAlarmServer();
	INFO_PRINTF1(_L("Completed TEST 4"));
	INFO_PRINTF1(KCITFS34FINISH);
	return TestStepResult();
	}

/** Executes Alarm Server Test
** Set Alarm to notify, Set alarm sounds silent until. Change System before or after alarm expiry time.
** Sound start and stop should be as expected
@return True or False indicating the result of the alarm server test
*/
TBool CTestSystemChangeUntil::ExecuteTestL()
	{
	SetSystemTimeL();

	SetInitalUtcOffsetL();

	SetAlarmL();

	// Set Global silent peroid duration in minutes (SetAlarmSoundsSilentFor)
	SetSilentUntilL();

	iUTCUntil = CalculateUTCUntilL(iInitialUtc);

	// Determine wether to change utc before or after alarm expires
	GetWaitToNotify();

	//Get New Offset and set
	iNewSysGreaterExp = EFalse;
	iUntilExpired = EFalse;
	iExpiryGreaterUntil = CheckExpGreaterUntilL(iInitialUtc);
	// wait for alarm to reach expiry time, then change system time, and then wait for sound start and stop
	if (iWaitToNotify)
		{
		if (!WaitToNotifyBeforeChangeL())
			{
			return EFalse;
			}
		}

	// Get new system time and set
	ChangeSystemTimeL();

	// Let the Alarm Server react to the System Time Change change
	User::After(KTimeToWait);

	iNow.UniversalTime();

	// silent time should have not changed,after system time change
	if (iNow <= iUTCUntil) // If present utc time is greater then until time, then we dont check
		{                // silent period as it will crash (getuntil crashes, as its time has passed
		if (!iWaitToNotify && !iExpiryGreaterUntil) //if silent period expired this will crash
			{
			if (!CheckSilentUntilL())
				{
				return EFalse;
				}
			}
		}

	if (iWaitToNotify) // If alarm has expired, alarm should be in Notifying state, then validate for sound start and stop
		{
		INFO_PRINTF1(_L("After System Time Change, checking state....."));
		if(!CheckAlarmStatesL(EAlarmStateNotifying))
			{
			return EFalse;
			}
		}

	TTimeIntervalMinutes minutesDifference;
	User::LeaveIfError(iSystemTime.MinutesFrom(iExpiryTime, minutesDifference));

	TInt difference = minutesDifference.Int();

	// wait for alarm to expire after utc change then wait for sound start and stop
	// new system time less than expiry time, so we wait for alarm to expire
	if (!iWaitToNotify && difference <= -1 && iMoveSystemTime < iExpiryTime)
		{
		if (!WaitToNotifyAfterChangeL())
			{
			return EFalse;
			}
		}

	// new system time is equal or greater than expiry time
	// state should be notifying,as alarm should expire immediately, so we dont wait for alarm to expire
	if (!iWaitToNotify && difference >= -1 && iMoveSystemTime >= iExpiryTime)
		{
		INFO_PRINTF1(_L(" New System Time equal to Expiry Time or Greater, checking state.... "));
		if(!CheckAlarmStatesL(EAlarmStateNotifying))
			{
			return EFalse;
			}

		WaitTimerExpiry();
		INFO_PRINTF1(_L("This expiry notification is due to the timer invalidating due to System Time Change "));
		iNewSysGreaterExp = ETrue;
		}

	if (iInitialUtc < 0)
		{
		INFO_PRINTF1(_L("More than an hour to go to play sounds "));
		}
	else
		{
		if (!CheckStartStopTimingsL()) // checks if start and stop timings are as expected
			{
			return EFalse;
			}
		}

	return ETrue;
	}

/** Waits for alarm to start playing sounds
@return True or False indicating if the sounds started playing at the correct time.
*/
TBool CTestSystemChangeUntil::WaitAndCheckSoundPlayStartL(TBool aUntilExpired)
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

			INFO_PRINTF3(KPlayStart,returnedAlarmId,iAlarmId);

			TDateTime aStartTime(iNow.DateTime());
			messageLog.Format(KDateTime, aStartTime.Year(),TInt(aStartTime.Month()),aStartTime.Day(),
					aStartTime.Hour(),aStartTime.Minute(),aStartTime.Second(),aStartTime.MicroSecond());
			FormatLogMessage(_L("Sound started (UTC time) to play at:- "), messageLog);

			TTimeIntervalMinutes minutesDifference;
			User::LeaveIfError(iNow.MinutesFrom(iSilentUntil, minutesDifference));
			TInt difference = minutesDifference.Int();

			if (aUntilExpired)
				{
				INFO_PRINTF3(KPlayStart,returnedAlarmId,iAlarmId);
				INFO_PRINTF1(_L("Sound Started At Expected Time"));
				iSoundStop = CalculateSoundStopL(iNow);
				return ETrue;
				}
			else
				{
				if (iNewSysGreaterExp)
					{
					INFO_PRINTF3(KPlayStart,returnedAlarmId,iAlarmId);
					INFO_PRINTF1(_L("Sound Started At Expected Time"));
					iSoundStop = CalculateSoundStopL(iNow);
					return ETrue;
					}
				else
					{
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
		}
    }

/** Waits for alarm to expire, validates the global sound and teporarily silenced state
@return True or False indicating that the alarm expired at correct times and the global sound and temprarily silenced state is as expected.
*/
TBool CTestSystemChangeUntil::WaitToNotifyAfterChangeL()
	{
	if (!WaitAndCheckExpiredAlarmL())
		{
		return EFalse;
		}

	if(!CheckAlarmStatesL(EAlarmStateNotifying))
		{
		return EFalse;
		}

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
		iUntilExpired = EFalse;
		}

	return ETrue;
	}

/** Waits for start/stop sound and validates the times, as well as the global sound state being ON
@return True or False indicating that the alarm start/stop sounds are as expected
*/
TBool CTestSystemChangeUntil::CheckStartStopTimingsL()
	{
	if(!WaitAndCheckSoundPlayStartL(iUntilExpired))
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


