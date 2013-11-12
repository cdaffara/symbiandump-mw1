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

#include "TestSystemChangeFor.h"
#include "TestCITAlarmServerSuiteDefs.h"

typedef CArrayFixFlat<TASCliSoundPlayDefinition> CPlayIntervalArray;

CTestSystemChangeFor::~CTestSystemChangeFor()
/**
 * Destructor
 */
	{
	}

CTestSystemChangeFor::CTestSystemChangeFor()
/**
 * Constructor
 */
	{
	SetTestStepName(KTestSystemChangeFor);
	}

TVerdict CTestSystemChangeFor::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	INFO_PRINTF1(_L("TEST 3: Test System Change for Non-Notifying and Notifying Alarms (FOR - GLOBAL SILENT)"));
	SetTestStepResult(EPass);
	return TestStepResult();
	}

TVerdict CTestSystemChangeFor::doTestStepL()
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

TVerdict CTestSystemChangeFor::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	CloseAlarmServer();
	INFO_PRINTF1(_L("Completed TEST 3"));
	return TestStepResult();
	}

/** Executes Alarm Server Test
** Set Alarm to notify, Set alarm sounds silent for in minutes. Change System before or after alarm expiry time.
** Sound start and stop should be as expected
@return True or False indicating the result of the alarm server test
*/
TBool CTestSystemChangeFor::ExecuteTestL()
	{
	SetSystemTimeL();

	SetInitalUtcOffsetL();

	SetAlarmL();

	// Set Global silent peroid duration in minutes (SetAlarmSoundsSilentFor)
	SetSilentForL();

	// Determine wether to change system before or after alarm expires
	GetWaitToNotify();

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

	VerifySilentUntil();

	if (iWaitToNotify)  // If alarm notified then alarm state should be Notifying
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

	// wait for alarm to expire after system time change then wait for sound start and stop
	// new system time less than expiry time, so we wait for alarm to expire
	if (!iWaitToNotify && difference <= -1 && iMoveSystemTime < iExpiryTime)
		{
		if(!WaitToNotifyAfterChangeL())
			{
			return EFalse;
			}
		}

	// new system time is equal or greater than expiry time
	// state should be notifying, so we don't wait for alarm to expire
	// Just need to check if alarm is in Notifying state as well as start.stop sounds timings
	if (!iWaitToNotify && difference >= -1 && iMoveSystemTime >= iExpiryTime)
		{
		INFO_PRINTF1(_L(" New System Time equal to Expiry Time or Greater, checking state.... "));

		if(!CheckAlarmNotifyingL())
			{
			return EFalse;
			}
		}

	// checks if sound start and stop timings are as expected
	if(!CheckStartStopSoundsL())
		{
		return EFalse;
		}

	return ETrue;
	}

/** Waits for alarm to start playing sounds
@return True or False indicating if the sounds started playing at the correct time.
*/
TBool CTestSystemChangeFor::WaitAndCheckSoundPlayStartL()
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

			// no checking done as sounds are played straight away after system time change
			INFO_PRINTF3(KPlayStart,returnedAlarmId,iAlarmId);
			INFO_PRINTF1(_L("Sound Started At Expected Time"));
			iSoundStop = CalculateSoundStopL(iNow);
			return ETrue;
			}
		}
    }

/** Waits for alarm to notify, validates alarm state (Notifying) and global sound state (State ON)
@return True or False indicating that the alarm and global sound state are as expected
*/
TBool CTestSystemChangeFor::WaitToNotifyAfterChangeL()
	{
	if (!WaitAndCheckExpiredAlarmL())
		{
		return EFalse;
		}

	if (!VerifyAlarmSoundStateL(EAlarmGlobalSoundStateOn, EFalse))
		{
		return EFalse;
		}

	return ETrue;
	}

/** Checks if alarm is in Notifying state, as well as the global sound state being ON
@return True or False indicating that the alarm and global sound state are as expected
*/
TBool CTestSystemChangeFor::CheckAlarmNotifyingL()
	{
	if(!CheckAlarmStatesL(EAlarmStateNotifying))
		{
		return EFalse;
		}

	WaitTimerExpiry();
	INFO_PRINTF1(_L("This expiry notification is due to the timer invalidating due to System Time Change"));

	if (!VerifyAlarmSoundStateL(EAlarmGlobalSoundStateOn, EFalse))
		{
		return EFalse;
		}

	return ETrue;
	}

/** Waits for start/stop sound and validates the times, as well as the global sound state being ON
@return True or False indicating that the alarm start/stop sounds are as expected
*/
TBool CTestSystemChangeFor::CheckStartStopSoundsL()
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

/** Verifies that GetAlarmSoundsSilentUntil API does not return KErrNone
*/
void CTestSystemChangeFor::VerifySilentUntil()
	{
	TTime getSilentUntil;
	if(iAlarmServerSession.GetAlarmSoundsSilentUntil(getSilentUntil)!=KErrNone)
		{
		INFO_PRINTF1(_L("GetAlarmSoundsSilentUntil() != KErrNone"));
		}
	else
		{
		ERR_PRINTF1(_L("GetAlarmSoundsSilentUntil() == KErrNone"));
		}
	}



