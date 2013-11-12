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

#include "TEAlarmTestSilentUntilStep.h"

#include <asclisession.h>

_LIT(KSilentUntil, "SilentUntil");
_LIT(KInitialTime, "InitialTime");
_LIT(KInvalidates, "Invalidates");

CTEAlarmTestSilentUntilStep::~CTEAlarmTestSilentUntilStep()
	{
	}
	
CTEAlarmTestSilentUntilStep::CTEAlarmTestSilentUntilStep()
	{
	// MANDATORY Call to base class method to set up the human readable name for logging.
	SetTestStepName(KTEAlarmTestSilentUntilStep);
	}
	
TVerdict CTEAlarmTestSilentUntilStep::doTestStepPreambleL()
	{
	SetTestStepResult(EFail);
	
	TVerdict verdict = CTEAlarmTestSilentStep::doTestStepPreambleL();
	
	if(verdict != EPass)
		{
		SetTestStepResult(verdict);
		return TestStepResult();
		}
		
	if(!GetTimeFromConfig(ConfigSection(), KSilentUntil, iSilentUntil)) 
		{
		SetTestStepResult(ETestSuiteError);
		return TestStepResult();
		}
		
	if(!GetTimeFromConfig(ConfigSection(), KInitialTime, iInitialTime))
		{
		SetTestStepResult(ETestSuiteError);
		return TestStepResult();
		}
	
	if(!GetBoolFromConfig(ConfigSection(), KInvalidates, iInvalidated))
		{
		SetTestStepResult(ETestSuiteError);
		return TestStepResult();
		}
	
	SetTestStepResult(EPass);
	return TestStepResult();
	}

TVerdict CTEAlarmTestSilentUntilStep::doTestStepL()
	{
	if(TestStepResult() != EPass)
		{
		return TestStepResult();
		}
	
	SetTestStepResult(EFail);
	
	INFO_PRINTF2(_L("Initial UTC Offset: %d s"), iInitialUTC.Int());
		
	User::SetUTCOffset(iInitialUTC);
	
	TBuf<10> buf;
	
	iInitialTime.FormatL(buf, _L("%H:%T:%S"));
	INFO_PRINTF2(_L("Initial UTC Time: %s"), buf.PtrZ());
	
	TTime oldTime;
	oldTime.UniversalTime();
	
	User::SetUTCTime(iInitialTime);
		
	RASCliSession session;
	
	User::LeaveIfError(session.Connect());
	CleanupClosePushL(session);
	
	iSilentUntil.FormatL(buf, _L("%H:%T:%S"));
	INFO_PRINTF2(_L("Setting Alarm Sounds Silent Until: %s"), buf.PtrZ());

	User::LeaveIfError(session.SetAlarmSoundsSilentUntil(iSilentUntil));
	
	if (!CheckSilentUntilL(session)) 
		{
		goto ret;
		}

	INFO_PRINTF2(_L("Updating UTC Offset: %d s"), iMoveToUTC.Int());	
	User::SetUTCOffset(iMoveToUTC);
	
	// Let the Alarm Server react to the UTC change
	User::After(1000000);
	
	if (!CheckSilentUntilL(session)) 
		{
		goto ret;
		}
		
	SetTestStepResult(EPass);
	
	ret:
	User::SetUTCTime(oldTime);
	CleanupStack::PopAndDestroy();
	return TestStepResult();			
	}
	
TVerdict CTEAlarmTestSilentUntilStep::doTestStepPostambleL()
	{
	SetTestStepResult(CTEAlarmTestSilentStep::doTestStepPostambleL());
	return TestStepResult();
	}

TBool CTEAlarmTestSilentUntilStep::CheckSilentUntilL(const RASCliSession &aSession)
	{
	TTime getSilentUntil;
	TInt ret = aSession.GetAlarmSoundsSilentUntil(getSilentUntil);
	
	TTime now;
	now.HomeTime();
	
	if ((ret == KErrGeneral) &&
		iInvalidated) // UTC offset change invalidates silent period.
		{
		return ETrue;
		}
	
	User::LeaveIfError(ret);
	
	TBuf<10> buf;
	iSilentUntil.FormatL(buf, _L("%H:%T:%S"));	
	
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
