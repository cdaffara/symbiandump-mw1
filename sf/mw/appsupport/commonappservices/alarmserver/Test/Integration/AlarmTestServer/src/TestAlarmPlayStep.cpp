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

#include "TestAlarmPlayStep.h"
#include <asclisoundplay.h>
#include <test/testexecutelog.h>
using namespace multialarms;
typedef CArrayFixFlat<TASCliSoundPlayDefinition> CPlayIntervalArray;


CTestPlayBaseStep::~CTestPlayBaseStep()
	{
	iIniOffsets.Close();
	}

CTestPlayBaseStep::CTestPlayBaseStep()
	{
	}
	
/**
The method times when an alarm play sound is started and compares
this against the expected values in the ini file. The method performs
the timing on the alarmserver side (component tests carry out the
timing on the console alarm alert server side).
*/
void CTestPlayBaseStep::TestPlayIntervalDiffL(TInt aAlarmNo, TInt aMaxOffsetIndex)
    {
    TInt currentOffsetIndex = 0;
    TTime alarmExpiredAt;
    TTime elapsedTime;
    TTimeIntervalMinutes diffMin;
	TAlarmId alarmId;
	TRequestStatus status;
	TAlarmId expectedAlarmId = iAlarmIds[aAlarmNo-1];
	
	for(;;)
		{
		ialarmSession.NotifyChange(status,alarmId);
		User::WaitForRequest(status);
		if	(status.Int() == EAlarmChangeEventSoundPlaying && alarmId == expectedAlarmId)
		    {
			if (currentOffsetIndex == 0)
			    {
			    alarmExpiredAt.UniversalTime();
				_LIT(KSoundStart,"Alarm Play Start For Alarm No %d At Offset Index 0");
			    INFO_PRINTF2(KSoundStart,aAlarmNo);
			    currentOffsetIndex++;
			    }
			else
			    {
			    TTimeIntervalMinutes diff = iIniOffsets[currentOffsetIndex];
			    elapsedTime.UniversalTime();
			    elapsedTime = elapsedTime - diff;		    
			    User::LeaveIfError(elapsedTime.MinutesFrom(alarmExpiredAt,diffMin));
			    
			    if (diffMin.Int() == 0)
			        {
			    	_LIT(Kstart,"Alarm Play Start at Index %d : Offset = %d");
			    	INFO_PRINTF3(Kstart,currentOffsetIndex,diff.Int());
			        }
			    else
			        {
			    	_LIT(Kdiff,"Unexpected Sound Play Start at Index %d : Diff = %d");
			    	INFO_PRINTF3(Kdiff,currentOffsetIndex,diffMin.Int());
			    	User::Leave(KErrGeneral);
			        }
			    			    
			    currentOffsetIndex++;
			    if (currentOffsetIndex == aMaxOffsetIndex)
			        {
			    	break;
			        }
			    }
			
			
		    }
		}
    }
    
/** 
The method provides a simple check after an alarm has expired as to whether
an alarm sound has started. The checking period is 5 seconds and is controlled by
the maxLoop constant.
*/
 void CTestPlayBaseStep::CheckForNoSoundL(TInt aAlarmNo)
    {
    TInt maxLoop = 5;
    TInt n=0;
	TAlarmId alarmId;
	TRequestStatus status;
	TAlarmId expectedAlarmId = iAlarmIds[aAlarmNo-1];  	
	
	for(;;)
		{
		ialarmSession.NotifyChange(status,alarmId);
		User::WaitForRequest(status);
		if	(status.Int() == EAlarmChangeEventSoundPlaying && alarmId == expectedAlarmId)
			{
			_LIT(KSoundStart,"Alarm Server Side Notification : Unexpected Sound Has Started To Play For Alarm No : %d");
			ERR_PRINTF2(KSoundStart,aAlarmNo);
			User::Leave(KErrGeneral);
			}
		else
		    {
		    if (n < maxLoop)
		        {
		    	User::After(1000000);
		    	n++;
		        }
		    else
		        {
		        _LIT(KNoSound,"Sound Has Not Restarted for Alarm No %d");
		        INFO_PRINTF2(KNoSound,aAlarmNo);
		    	break;
		        }
		    }
		}
    						    		    
    }   
 
/**
This method reads the offset values from the ini files, stores them
in the array for use when setting or comparing against real offsets and also
prints the offset values for information.
*/	
void CTestPlayBaseStep::StoreIniPlayOffsetsL()
    {
    TInt iniDefLength;
    TInt offsetValue;
    TBuf <KMaxTagLength> iniTag;
    
	GetIntFromConfig(ConfigSection(),KDefIniPlayLength,iniDefLength);	
	for(TInt i = 0; i < iniDefLength; i++)
	    {
	    iniTag = KDefIniOffsetTag;
	    iniTag.AppendNum(i);
	    GetIntFromConfig(ConfigSection(),iniTag,offsetValue);
		TESTL(iIniOffsets.Insert(offsetValue,i) == KErrNone);

	    }
	
	for (TInt i=0; i <iniDefLength; i++)
	    {
		TInt offsetValue = iIniOffsets[i];
		_LIT(KVal,"Offset Value From INI File at Index %d = %d");
		INFO_PRINTF3(KVal,i,offsetValue);
	    }
    }
 

/**
The method reads the previously stored alarm offsets then sets
the values in the alarm server.
*/
void CTestPlayBaseStep::SetPlayIntervalsL()
    {
	CPlayIntervalArray* playDefs = new(ELeave) CPlayIntervalArray(20);
	CleanupStack::PushL(playDefs);
	
	for (TInt i=0; i < iIniOffsets.Count(); i++)
	    {
		const TASCliSoundPlayDefinition defs(iIniOffsets.operator[](i),iIniDuration);
		playDefs->AppendL(defs);
	    }
	
    ialarmSession.SetAlarmPlayIntervalsL(*playDefs);
	CleanupStack::PopAndDestroy(playDefs);
    }
    
    
/**
The method gets an array of TASCliSoundPlayDefinition object and
compare the offset and duration values against the previously
stored offsets and durations.
*/
void CTestPlayBaseStep::CheckPlayIntervalsL()   
    {
	CPlayIntervalArray* playDefs = new	(ELeave) CPlayIntervalArray(20);
   	CleanupStack::PushL(playDefs);
   	ialarmSession.GetAlarmPlayIntervalsL(*playDefs);
    TInt cnt = iIniOffsets.Count();
    
    for (TInt i=0;i<cnt; i++)
        {
       	TInt offset = playDefs->At(i).Offset().Int();
       	TInt duration = playDefs->At(i).Duration().Int();
       	INFO_PRINTF4(_L("Offset[%d] = %d, Duraton = %d"),i,offset,duration);
      	TESTL(iIniOffsets[i] == offset);
      	TESTL(iIniDuration == duration);
      	}
      	
    CleanupStack::PopAndDestroy(playDefs);    
    }




/**
@SYMTestCaseID PIM-APPSRV-ALARMSRV-CIT-1118-11
@SYMTestType CIT
@SYMTestPriority High
@SYMPREQ 1118
@SYMFssID AlarmServer 033
@SYMTestCaseDesc Alarm Play Interval Default Configuration
@SYMTestActions Add an alarm, check default play offset and durations, wait for
alarm to expire and checksound timings
@SYMTestExpectedResults Sound timing should correspond to default in resource file
*/

CTestDefaultAlarmPlayStep::~CTestDefaultAlarmPlayStep()
/**
 * Destructor
 */
	{
	}

CTestDefaultAlarmPlayStep::CTestDefaultAlarmPlayStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KTestDefPlay);
	}

TVerdict CTestDefaultAlarmPlayStep::doTestStepL()
	{
	SetTestStepResult(EPass);	
		
	if(!GetIntFromConfig(ConfigSection(),KIniDuration,iIniDuration))
	    {
		_LIT(KErr,"Duration Value Not Found in INI");
		INFO_PRINTF1(KErr);
		SetTestStepResult(EFail);
		return TestStepResult();
	    }
	    
	if (!GetIntFromConfig(ConfigSection(),KMaxOffIndex,iIniMaxOffsetIndex))
	    {
		_LIT(KErr,"Max OffsetIndex Value Not Found in INI");
		INFO_PRINTF1(KErr);
		SetTestStepResult(EFail);
		return TestStepResult();
	    }
	
	TRAPD(err, TestDefaultAlarmPlayStepL())
	if (err != KErrNone)
	    {
	    _LIT(KLeft,"Test Default Alarm Play Left = %d");
	    INFO_PRINTF2(KLeft,err);
	    SetTestStepResult(EFail);	
	    }

	return TestStepResult();
	}
	
void CTestDefaultAlarmPlayStep::TestDefaultAlarmPlayStepL()
    { 
	StoreIniPlayOffsetsL(); 
	CheckPlayIntervalsL();
	AddAlarmL(alarm1,iAlarmExpiry1);
	NotifyOnAlarmL(alarm1);
	CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
	TestPlayIntervalDiffL(alarm1,iIniMaxOffsetIndex);
	AcknowledgeAlarmL(alarm1);
	WaitAndCheckSoundPlayStop(alarm1);
	CheckAlarmStatesL(alarm1,EAlarmStateNotified);
	CheckIdListByStateL(EAlarmStateNotified);
    }



/*
@SYMTestCaseID PIM-APPSRV-ALARMSRV-CIT-1118-12
@SYMTestType CIT
@SYMTestPriority High
@SYMPREQ 1118
@SYMFssID AlarmServer 033
@SYMTestCaseDesc Change Alarm Play Interval
@SYMTestActions Change the offset settings with the run-time
options, add an alarm and test the timing
@SYMTestExpectedResults The run-time option should overwrite the resource settings
*/
CTestChangePlayIntervalStep::~CTestChangePlayIntervalStep()
/**
 * Destructor
 */
	{
	}

CTestChangePlayIntervalStep::CTestChangePlayIntervalStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KTestPlayChange);
	}

TVerdict CTestChangePlayIntervalStep::doTestStepL()
	{	
	SetTestStepResult(EPass);
	
    if(!GetIntFromConfig(ConfigSection(),KIniDuration,iIniDuration))
	    {
		_LIT(KErr,"Duration Value Not Found in INI");
		INFO_PRINTF1(KErr);
		SetTestStepResult(EFail);
		return TestStepResult();
	    }
	    
	if (!GetIntFromConfig(ConfigSection(),KMaxOffIndex,iIniMaxOffsetIndex))
	    {
		_LIT(KErr,"Max OffsetIndex Value Not Found in INI");
		INFO_PRINTF1(KErr);
		SetTestStepResult(EFail);
		return TestStepResult();
	    }		
	
	TRAPD(err, TestChangeIntervalstepL())
	if (err != KErrNone)
	    {
	    _LIT(KLeft,"Test Change Alarm Play Left = %d");
	    INFO_PRINTF2(KLeft,err);
	    SetTestStepResult(EFail);	
	    }

	return TestStepResult();
	}
	
void CTestChangePlayIntervalStep::TestChangeIntervalstepL()
    { 
	StoreIniPlayOffsetsL();  
	SetPlayIntervalsL();
	CheckPlayIntervalsL();
	AddAlarmL(alarm1,iAlarmExpiry1);
	NotifyOnAlarmL(alarm1);
	TestPlayIntervalDiffL(alarm1,iIniMaxOffsetIndex);
	AcknowledgeAlarmL(alarm1);
	WaitAndCheckSoundPlayStop(alarm1);
	CheckAlarmStatesL(alarm1,EAlarmStateNotified);
	CheckIdListByStateL(EAlarmStateNotified);
    }
    
    
 /*
@SYMTestCaseID PIM-APPSRV-ALARMSRV-CIT-1118-13
@SYMTestType CIT
@SYMTestPriority High
@SYMPREQ 1118
@SYMFssID AlarmServer 033 036
@SYMTestCaseDesc Invalid Attempt to Disable Alarm Server Control of Alarm Play Interval
@SYMTestActions Attempt to set an empty play interval sequence, add alarm and check
the timings.
@SYMTestExpectedResults Empty sequence should be ignored and should not affect
alarm server contol of play intervals. Alarm should play the default settings defined
the resource file. This test step should be run after test -011.
*/   
 CTestInvalidIntervalStep::~CTestInvalidIntervalStep()
/**
 * Destructor
 */
	{
	}

CTestInvalidIntervalStep::CTestInvalidIntervalStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KTestPlayInvalid);
	}

TVerdict CTestInvalidIntervalStep::doTestStepL()
	{	
	SetTestStepResult(EPass);
	
	if(!GetIntFromConfig(ConfigSection(),KIniDuration,iIniDuration))
	    {
		_LIT(KErr,"Duration Value Not Found in INI");
		INFO_PRINTF1(KErr);
		SetTestStepResult(EFail);
		return TestStepResult();
	    }
	    
	if (!GetIntFromConfig(ConfigSection(),KMaxOffIndex,iIniMaxOffsetIndex))
	    {
		_LIT(KErr,"Max OffsetIndex Value Not Found in INI");
		INFO_PRINTF1(KErr);
		SetTestStepResult(EFail);
		return TestStepResult();
	    }
	    
	TRAPD(err, TestInvalidIntervalStepL())
	if (err != KErrNone)
	    {
	    _LIT(KLeft,"Test Invalid Interval Left = %d");
	    INFO_PRINTF2(KLeft,err);
	    SetTestStepResult(EFail);	
	    }

	return TestStepResult();
	}
	
void CTestInvalidIntervalStep::TestInvalidIntervalStepL()
    {  
	StoreIniPlayOffsetsL();
	SetEmptyPlayIntervalsL();
	CheckPlayIntervalsL();
	AddAlarmL(alarm1,iAlarmExpiry1);
	NotifyOnAlarmL(alarm1);
	TestPlayIntervalDiffL(alarm1,iIniMaxOffsetIndex);
	AcknowledgeAlarmL(alarm1);
	WaitAndCheckSoundPlayStop(alarm1);
	CheckAlarmStatesL(alarm1,EAlarmStateNotified);
	CheckIdListByStateL(EAlarmStateNotified);
    }
    
void CTestInvalidIntervalStep::SetEmptyPlayIntervalsL()
    {
	CPlayIntervalArray* playDefs = new(ELeave) CPlayIntervalArray(20);
	CleanupStack::PushL(playDefs);
	
	TRAPD(err,ialarmSession.SetAlarmPlayIntervalsL(*playDefs));
	if (err != KErrArgument)
	    {
		_LIT(KWrongCode,"Unexpected Error Code %d returned, Expecting KErrArgument(-6)");
		INFO_PRINTF2(KWrongCode,err);
		User::Leave(KErrGeneral);
	    }
	
	CleanupStack::PopAndDestroy(playDefs);
    }
    
    
    
    
/*
@SYMTestCaseID PIM-APPSRV-ALARMSRV-CIT-1118-14
@SYMTestType CIT
@SYMTestPriority High
@SYMPREQ 1118
@SYMFssID AlarmServer 034
@SYMTestCaseDesc Alarm Notification Using Repeating Last Play Interval
@SYMTestActions Resource file has last play interval set. Add and alarm,
wait for it to expire, check play interval timing.
@SYMTestExpectedResults The las play interval is continually repeated.
*/
 CTestRepeatLastIntervalStep::~CTestRepeatLastIntervalStep()
/**
 * Destructor
 */
	{
	}

CTestRepeatLastIntervalStep::CTestRepeatLastIntervalStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KTestLastPlay);
	}

TVerdict CTestRepeatLastIntervalStep::doTestStepL()
	{	
	SetTestStepResult(EPass);
	
	if(!GetIntFromConfig(ConfigSection(),KIniDuration,iIniDuration))
	    {
		_LIT(KErr,"Duration Value Not Found in INI");
		INFO_PRINTF1(KErr);
		SetTestStepResult(EFail);
		return TestStepResult();
	    }
	    
	if (!GetIntFromConfig(ConfigSection(),KMaxOffIndex,iIniMaxOffsetIndex))
	    {
		_LIT(KErr,"Max OffsetIndex Value Not Found in INI");
		INFO_PRINTF1(KErr);
		SetTestStepResult(EFail);
		return TestStepResult();
	    }
	    
	TRAPD(err, TestRepeatLastIntervalL())
	if (err != KErrNone)
	    {
	    _LIT(KLeft,"Test Play Last Interval Left = %d");
	    INFO_PRINTF2(KLeft,err);
	    SetTestStepResult(EFail);	
	    }

	return TestStepResult();
	}
	
void CTestRepeatLastIntervalStep::TestRepeatLastIntervalL()
    {
	SetSimpleSequence();
	SetPlayIntervalsL();
	CheckPlayIntervalsL();
	StoreIniPlayOffsetsL();
	AddAlarmL(alarm1,iAlarmExpiry1);
    WaitAndCheckExpiredAlarm(alarm1);
	TestPlayIntervalDiffL(alarm1,iIniMaxOffsetIndex);
	AcknowledgeAlarmL(alarm1);
	WaitAndCheckSoundPlayStop(alarm1);
	CheckAlarmStatesL(alarm1,EAlarmStateNotified);
	CheckIdListByStateL(EAlarmStateNotified);
    }
    
void CTestRepeatLastIntervalStep::SetSimpleSequence()
    {
    const TInt tempLength = 3;
	TInt tempOffset[tempLength] = {0, 1, 2};
	for (TInt i=0; i < tempLength; i++)
	    {
		iIniOffsets.Insert(tempOffset[i],i);
	    }
    }
    
    
/*
@SYMTestCaseID PIM-APPSRV-ALARMSRV-CIT-1118-15
@SYMTestType CIT
@SYMTestPriority High
@SYMPREQ 1118
@SYMFssID AlarmServer 035
@SYMTestCaseDesc Stop Alarm Playing When End of Play Interval Reached
@SYMTestActions Resource file should be updated. Add alarm, wiat for it
to expire and check play interval timings.
@SYMTestExpectedResults Alarm does not repeat when end of play interval reached.
*/
CTestStopPlayStep::~CTestStopPlayStep()
/**
 * Destructor
 */
	{
	}

CTestStopPlayStep::CTestStopPlayStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KTestStopPlay);
	}

TVerdict CTestStopPlayStep::doTestStepL()
	{
	SetTestStepResult(EPass);
	
	if(!GetIntFromConfig(ConfigSection(),KIniDuration,iIniDuration))
	    {
		_LIT(KErr,"Duration Value Not Found in INI");
		INFO_PRINTF1(KErr);
		SetTestStepResult(EFail);
		return TestStepResult();
	    }
	    
	if (!GetIntFromConfig(ConfigSection(),KMaxOffIndex,iIniMaxOffsetIndex))
	    {
		_LIT(KErr,"Max OffsetIndex Value Not Found in INI");
		INFO_PRINTF1(KErr);
		SetTestStepResult(EFail);
		return TestStepResult();
	    }
	
	TRAPD(err, TestStopPlayL())
	if (err != KErrNone)
	    {
	    _LIT(KLeft,"Test Stop Play Interval Left = %d");
	    INFO_PRINTF2(KLeft,err);
	    SetTestStepResult(EFail);	
	    }

	return TestStepResult();
	}
	
void CTestStopPlayStep::TestStopPlayL()
    {
	StoreIniPlayOffsetsL();
	SetPlayIntervalsL();
	CheckPlayIntervalsL();
	AddAlarmL(alarm1,iAlarmExpiry1);
	NotifyOnAlarmL(alarm1);
	TestPlayIntervalDiffL(alarm1,iIniMaxOffsetIndex);
	AcknowledgeAlarmL(alarm1);
	CheckAlarmStatesL(alarm1,EAlarmStateNotified);
	CheckIdListByStateL(EAlarmStateNotified);
    }
    
    
    
/*
@SYMTestCaseID PIM-APPSRV-ALARMSRV-CIT-1118-16
@SYMTestType CIT
@SYMTestPriority High
@SYMPREQ 1118
@SYMFssID AlarmServer 036
@SYMTestCaseDesc Disable Alarm Server Control of Play Interval with Resource File 
@SYMTestActions Resource File has an empty play interval defined. Add alarm and
wait for alarm to expire, check for alarm play start/no sound.
@SYMTestExpectedResults Alarm Server does not issue an StartPlayingSound.
*/    
CTestDisablePlayStep::~CTestDisablePlayStep()
/**
 * Destructor
 */
	{
	}

CTestDisablePlayStep::CTestDisablePlayStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KTestDisable);
	}

TVerdict CTestDisablePlayStep::doTestStepL()
	{	
	SetTestStepResult(EPass);
	TRAPD(err, TestDisablePlayL())
	if (err != KErrNone)
	    {
	    _LIT(KLeft,"Test Disable Alarm Server Control Left = %d");
	    INFO_PRINTF2(KLeft,err);
	    SetTestStepResult(EFail);	
	    }

	return TestStepResult();
	}
	
void CTestDisablePlayStep::TestDisablePlayL()
    {
	AddAlarmL(alarm1,iAlarmExpiry1);
	CheckForNoSoundL(alarm1);
	CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
	AcknowledgeAlarmL(alarm1);
	CheckAlarmStatesL(alarm1,EAlarmStateNotified);
	CheckIdListByStateL(EAlarmStateNotified);
    }
    
    
 /*
@SYMTestCaseID PIM-APPSRV-ALARMSRV-CIT-1118-17
@SYMTestType CIT
@SYMTestPriority High
@SYMPREQ 1118
@SYMFssID AlarmServer 036
@SYMTestCaseDesc Attempt to Override Resource File Control of Play Intervals
Where Alarm Server Control of Play Intervals Has Been Disabled
@SYMTestActions Resource File should have an empty play interval defined.
Attempt to change alarm play interval with the run-time option and specifying
a valid play sequence. Add and enable the alarm and check output when alarm expires.
@SYMTestExpectedResults Alarm Server does not issue an AlarmPlayStart to Alert Server.
*/   
 CTestOverrideResourceStep::~CTestOverrideResourceStep()
/**
 * Destructor
 */
	{
	}

CTestOverrideResourceStep::CTestOverrideResourceStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KTestOverride);
	}

TVerdict CTestOverrideResourceStep::doTestStepL()
	{	
	SetTestStepResult(EPass);
	
	TRAPD(err,TestOverrideResourceL())
	if (err != KErrNone)
	    {
	    _LIT(KLeft,"Test Override Resource File Left = %d");
	    INFO_PRINTF2(KLeft,err);
	    SetTestStepResult(EFail);	
	    }

	return TestStepResult();
	}
	
void CTestOverrideResourceStep::TestOverrideResourceL()
    {   
	StoreIniPlayOffsetsL();
	TRAPD(error,SetPlayIntervalsL());
	
	if (error == KErrNotSupported)
	    {
		_LIT(Kerr,"SetPlayIntervalsL Left For Alarm No %d With Correct Error Code = %d");
		INFO_PRINTF3(Kerr,alarm1,error);
	    }
	else
	    {
		_LIT(Kerr,"SetPlayIntervalsL Left For Alarm No %d With An Unexpected Error Code = %d");
		ERR_PRINTF3(Kerr,alarm1,error);
		User::Leave(KErrGeneral);
	    }
	
	AddAlarmL(alarm1,iAlarmExpiry1);
	CheckForNoSoundL(alarm1);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
	AcknowledgeAlarmL(alarm1);
	CheckAlarmStatesL(alarm1,EAlarmStateNotified);
	CheckIdListByStateL(EAlarmStateNotified);
    }


