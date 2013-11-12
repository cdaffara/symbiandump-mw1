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

#include "TestUTCChangeFor.h"
#include "TestCITAlarmServerSuiteDefs.h"

typedef CArrayFixFlat<TASCliSoundPlayDefinition> CPlayIntervalArray;

CTestUTCChangeFor::~CTestUTCChangeFor()
/**
 * Destructor
 */
	{
	}

CTestUTCChangeFor::CTestUTCChangeFor()
/**
 * Constructor
 */
	{
	SetTestStepName(KTestUTCChangeFor);
	}

TVerdict CTestUTCChangeFor::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	INFO_PRINTF1(KCITFS34);
	INFO_PRINTF1(_L("TEST 1: Test UTC Change for Non-Notifying and Notifying Alarms (FOR - GLOBAL SILENT)"));
	SetTestStepResult(EPass);
	return TestStepResult();
	}

TVerdict CTestUTCChangeFor::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
    SetTestStepResult(EFail);
	
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

TVerdict CTestUTCChangeFor::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	CloseAlarmServer();
	INFO_PRINTF1(_L("Completed TEST 1")); 
	return TestStepResult();
	}

/** Executes Alarm Server Test 
** Set Alarm to notify, Set alarm sounds silent for in minutes. Change UTC offset before or after alarm expiry time.
** Sound start and stop should be as expected
@return True or False indicating the result of the alarm server test
*/	
TBool CTestUTCChangeFor::ExecuteTestL()
	{
	SetSystemTimeL();
	
	SetInitalUtcOffsetL();
	
	SetAlarmL();
	
	// Set Global silent peroid duration in minutes (SetAlarmSoundsSilentFor)
	SetSilentForL();
	
	// Determine wether to change utc before or after alarm expires
	GetWaitToNotify();
			
	iUntilExpired = EFalse;
	changedUTC = EFalse;
	iExpiryGreaterUntil = CheckExpGreaterUntilL(iInitialUtc);
	
	// wait for alarm to reach expiry time before changing utc offset (if wait to notify then change utc test)
	if (iWaitToNotify)
		{
		if(!WaitToNotifyBeforeChangeL())
			{
			return EFalse;
			}
		}
	
	ChangeUtcOffset();	
	
	// Let the Alarm Server react to the UTC change
	User::After(KTimeToWait);
	
	if(!VerifyGetSilentUntil())
		{
		return EFalse;
		}
				
	// wait for alarm to expire after utc change then wait for sound start and stop
	if ((iWaitToNotify == EFalse) && (iMoveUtc != iInitialUtc))	
		{
		if(!WaitToNotifyAfterChangeL(changedUTC))
			{
			return EFalse;
			}
		}
	else // If alarm already expired then check for sound start and stop
		{
		if(!CheckStartAndStopL())
			{
			return EFalse;
			}
		}
		
	return ETrue;
	}

/** Waits for alarm to start playing sounds
@return True or False indicating if the sounds started playing at the correct time.
*/
TBool CTestUTCChangeFor::WaitAndCheckSoundPlayStartL()
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
			
			if (!changedUTC && !iUntilExpired)
				{
				TTimeIntervalSeconds secondsDifference;
				User::LeaveIfError(iNow.SecondsFrom(iSilentUntil, secondsDifference));
				TInt difference = secondsDifference.Int();
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
				TTimeIntervalSeconds secondsDifference;
				User::LeaveIfError(iNow.SecondsFrom(iExpiryTime, secondsDifference));
				TInt difference = secondsDifference.Int();
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

/** Waits for alarm to expire after changing UTC offset.
@return True or False indicating if alarm expired and playing/stopping sounds at the correct times.
*/
TBool CTestUTCChangeFor::WaitToNotifyAfterChangeL(TBool aChangedUTC)
	{
	if (!WaitAndCheckExpiredAlarmL())
		{
		return EFalse;
		}
		
	if (!aChangedUTC && iExpiryGreaterUntil)		
		{
		iUntilExpired = ETrue;
		}
		
	if(!CheckStartAndStopL())
		{
		return EFalse;
		}	
	return ETrue;
	}

/** Waits for alarm to start/stopping playing sounds
@return True or False indicating if alarm playing/stopping sounds at the correct times.
*/
TBool CTestUTCChangeFor::CheckStartAndStopL()
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
	
/** Verifies the get alarm until time
@return True or False indicating if the alarm until time is correct
*/
TBool CTestUTCChangeFor::VerifyGetSilentUntil()
	{
	TTime getSilentUntil;
	if(iAlarmServerSession.GetAlarmSoundsSilentUntil(getSilentUntil)!=KErrNone)
		{
		if (iMoveUtc == iInitialUtc)
			{
			INFO_PRINTF1(_L("ERROR: GetAlarmSoundsSilentUntil()!=KErrNone"));
			return EFalse;
			}
		else
			{
			changedUTC = ETrue;
			INFO_PRINTF1(_L("GetAlarmSoundsSilentUntil()!=KErrNone"));
			}
		}
	else
		{
		if (iMoveUtc == iInitialUtc)
			{
			INFO_PRINTF1(_L("GetAlarmSoundsSilentUntil()==KErrNone"));
			}
		else
			{
			INFO_PRINTF1(_L("GetAlarmSoundsSilentUntil()==KErrNone"));
			return EFalse;
			}
		}
		
	return ETrue;
	}





	

	

	
