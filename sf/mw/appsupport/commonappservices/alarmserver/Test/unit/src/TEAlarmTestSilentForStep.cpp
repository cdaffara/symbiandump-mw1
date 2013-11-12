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

#include "TEAlarmTestSilentForStep.h"

#include <asclisession.h>

_LIT(KSilentFor, "SilentFor");
_LIT(KTestUTCoffset, "TestUTCoffset");
_LIT(KTestSysTime, "TestSysTime");
_LIT(KSysTimeOffset, "SysTimeOffset");


CTEAlarmTestSilentForStep::~CTEAlarmTestSilentForStep()
	{
	}
	
CTEAlarmTestSilentForStep::CTEAlarmTestSilentForStep()
	{
	// MANDATORY Call to base class method to set up the human readable name for logging.
	SetTestStepName(KTEAlarmTestSilentForStep);
	}
	
TVerdict CTEAlarmTestSilentForStep::doTestStepPreambleL()
	{

	
	TESTL(CTEAlarmTestSilentStep::doTestStepPreambleL()==EPass);
		
	GetIntFromConfig(ConfigSection(), KTestUTCoffset, iTestUTCoffset);
	GetIntFromConfig(ConfigSection(), KTestSysTime, iTestSysTime);

	TESTL(GetOffsetFromConfig(ConfigSection(), KSilentFor, iSilentFor));

		
	if(iTestSysTime)
		{
		TESTL(GetIntFromConfig(ConfigSection(), KSysTimeOffset, iSysTimeOffset));
		}	
	
	return TestStepResult();
	}

TVerdict CTEAlarmTestSilentForStep::doTestStepL()
	{

	
	INFO_PRINTF2(_L("Initial UTC Offset: %d s"), iInitialUTC.Int());
	
	User::SetUTCOffset(iInitialUTC);
		
	TESTL(iSession.Connect()==KErrNone);
	
	INFO_PRINTF2(_L("Setting Alarm Sounds Silent For: %d min"), iSilentFor.Int());
	TESTL(iSession.SetAlarmSoundsSilentFor(iSilentFor)==KErrNone);
	
	TESTL(CheckSilentForL(iSession, EFalse)) ;
	

	TTime now;
	now.UniversalTime();
	TTimeIntervalMinutes offset(iSysTimeOffset);
	now = now + offset;

	if (iTestUTCoffset && iTestSysTime)
		{
		User::SetUTCTimeAndOffset(now,iMoveToUTC);
		}
	else if (iTestUTCoffset)
		{
		INFO_PRINTF2(_L("Updating UTC Offset: %d s"), iMoveToUTC.Int());	
		User::SetUTCOffset(iMoveToUTC);
		}
	else if (iTestSysTime)
		{

		User::SetUTCTime(now);
		}
	else
		{
		INFO_PRINTF1(_L("At least one flag should be set."));
		TESTL(EFalse);
		}
	
	
	// Let the Alarm Server react to environment change
	User::After(1000000);
	
	TBool checkIfCancelled = ETrue;
	if (iTestUTCoffset && !iTestSysTime && (iMoveToUTC==iInitialUTC))
		{
		checkIfCancelled = EFalse;  //silent period should not be cancelled if we set the same UTC as before
		}
	TESTL(CheckSilentForL(iSession, checkIfCancelled)); 


	return TestStepResult();
	}
	
TVerdict CTEAlarmTestSilentForStep::doTestStepPostambleL()
	{
	SetTestStepResult(CTEAlarmTestSilentStep::doTestStepPostambleL());
	return TestStepResult();
	}
	
TBool CTEAlarmTestSilentForStep::CheckSilentForL(const RASCliSession &aSession, TBool aSecondPass)
	{
	TTime getSilentUntil;
	TInt err=aSession.GetAlarmSoundsSilentUntil(getSilentUntil);
	if (aSecondPass)
		{
		TAlarmGlobalSoundState soundSstate;
		TESTL(err == KErrGeneral);
		TESTL(aSession.GetAlarmSoundState(soundSstate) == KErrNone);
		TESTL(EAlarmGlobalSoundStateOn == soundSstate);
		return ETrue;
		}
	TTime now;
	now.HomeTime();
	
	TTimeIntervalSeconds silentSeconds;
	TESTL(getSilentUntil.SecondsFrom(now, silentSeconds)==KErrNone);

	now -= TTimeIntervalSeconds(silentSeconds.Int() % 60);
	TTimeIntervalMinutes silentMinutes;
	TESTL(getSilentUntil.MinutesFrom(now, silentMinutes)==KErrNone);

	TBuf<32> buf;
	getSilentUntil.FormatL(buf, _L("%H:%T:%S"));

	if (iSilentFor != silentMinutes)
		{
		INFO_PRINTF1(_L("SetAlarmSoundsSilentFor and GetAlarmSoundsSilentUntil doesn't match!"));
		INFO_PRINTF2(_L("SetAlarmsSoundsSilentFor: %d min"), iSilentFor.Int());
		INFO_PRINTF3(_L("GetAlarmsSoundsSilentUntil: %d min (%s UTC)"), silentMinutes.Int(), buf.PtrZ());
		
		return EFalse;
		}
	
	INFO_PRINTF1(_L("SetAlarmSoundsSilentFor and GetAlarmSoundsSilentUntil matches."));	
	INFO_PRINTF2(_L("SetAlarmsSoundsSilentFor: %d min"), iSilentFor.Int());
	INFO_PRINTF3(_L("GetAlarmsSoundsSilentUntil: %d min (%s UTC)"), silentMinutes.Int(), buf.PtrZ());
	
	return ETrue;	
	}
