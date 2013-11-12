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

#include "consoleantestclient.h"
#include "TestMultipleAlarmsStep.h"
#include "TestCITAlarmServerBaseStep.h"
using namespace multialarms;

/**
@SYMTestCaseID PIM-APPSRV-ALARMSRV-CIT-1118-01
@SYMTestType CIT
@SYMTestPriority High
@SYMPREQ 1118
@SYMFssID AlarmServer 037 038
@SYMTestCaseDesc Notification of multiple alarms. 
@SYMTestActions The test uses three alarms, Alarm1, Alarm2 and Alarm3.
Add the three alarms with overlapping expiry times.
Wait for alarms to expire, check sound and states using the alarm server
notification and checking the console alert server output.
@SYMTestExpectedResults Notification from alarm server should should show
each alarm transitioning to the Notifying state with a new sound being 
started for each expiring alarm. Only one sound at a time is played. The
alarm server should notify that all alarms are in the Notifying state.
*/

CTestMultipleAlarmsStep::~CTestMultipleAlarmsStep()
/**
 * Destructor
 */
	{
	}

CTestMultipleAlarmsStep::CTestMultipleAlarmsStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KTestMultipleAlarms);
	}

TVerdict CTestMultipleAlarmsStep::doTestStepL()
	{
	SetTestStepResult(EPass);
		
	TRAPD(err, TestMultipleAlarmsL())
	if (err != KErrNone)
	    {
	    _LIT(KLeft,"Test Muliple Alarms Left = %d");
	    INFO_PRINTF2(KLeft,err);
	    SetTestStepResult(EFail);	
	    }

	return TestStepResult();
	}

void CTestMultipleAlarmsStep::TestMultipleAlarmsL()
	{	
	AddAlarmL(alarm1,iAlarmExpiry1);
	AddAlarmL(alarm2,iAlarmExpiry2);
	AddAlarmL(alarm3,iAlarmExpiry3);
	
	//wait for alarm1 to expire then check notifications
	//on alarmserver and alertserver side
	NotifyOnAlarmL(alarm1);
	WaitAndCheckExpiredAlarm(alarm1);
	WaitAndCheckSoundPlayStart(alarm1);
	NotifyOnSoundStartL(alarm1);
	CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateQueued);
	CheckAlarmStatesL(alarm3,EAlarmStateQueued);
	
	//wait for alarm2 to expire
	NotifyOnSoundStopL(alarm1);
	NotifyOnAlarmL(alarm2);
    WaitAndCheckExpiredAlarm(alarm2);
    WaitAndCheckSoundPlayStart(alarm2);
    NotifyOnSoundStartL(alarm2);
	CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
	CheckAlarmStatesL(alarm3,EAlarmStateQueued);

    //wait for alarm3 to expire
    NotifyOnSoundStopL(alarm2);
	NotifyOnAlarmL(alarm3);
    WaitAndCheckExpiredAlarm(alarm3);
	WaitAndCheckSoundPlayStart(alarm3);
	NotifyOnSoundStartL(alarm3);
	CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
	CheckAlarmStatesL(alarm3,EAlarmStateNotifying);
	
	CheckIdListByStateL(EAlarmStateNotifying);
    AcknowledgeAlarmL(alarm1);
    AcknowledgeAlarmL(alarm2);
    AcknowledgeAllL();
	WaitAndCheckSoundPlayStop(alarm3);
	CheckIdListByStateL(EAlarmStateNotified);
	}




/**
@SYMTestCaseID PIM-APPSRV-ALARMSRV-CIT-1118-02
@SYMTestType CIT
@SYMTestPriority High
@SYMPREQ 1118
@SYMFssID AlarmServer 037 and 038
@SYMTestCaseDesc Add multiple alarms with the same expiry times.
The test uses three alarms, Alarm1, Alarm2 and Alarm3.
@SYMTestActions Add and enable three alarms with the same expiry time.
Check the alarm notification of sound playing and check states. 
@SYMTestExpectedResults All three alarms are expected to be in the Notifying
state. Only one sound at a time is played.
*/
	
CTestSameTimeMultiAlarmsStep::~CTestSameTimeMultiAlarmsStep()
/**
 * Destructor
 */
	{
	}

CTestSameTimeMultiAlarmsStep::CTestSameTimeMultiAlarmsStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KTesSameTimeMultiAlarms);
	}

TVerdict CTestSameTimeMultiAlarmsStep::doTestStepL()
	{	
	SetTestStepResult(EPass);
	
	TRAPD(err, TestSameTimeMultipleAlarmsL())
	if (err != KErrNone)
	    {
	    _LIT(KLeft,"Test Same Time Muliple Alarms Left = %d");
	    INFO_PRINTF2(KLeft,err);
	    SetTestStepResult(EFail);	
	    }

	return TestStepResult();
	}
	

void CTestSameTimeMultiAlarmsStep::TestSameTimeMultipleAlarmsL()
	{ 
	AddAlarmL(alarm1,iAlarmExpiry1);
	AddAlarmL(alarm2,iAlarmExpiry2);
	AddAlarmL(alarm3,iAlarmExpiry3); 
	
    User::After(5 * 1000000);
	
	//simple check that alarms with the same expiry
	//time transition to notify state as do the alarms
	//with overlapping times
	CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
	CheckAlarmStatesL(alarm3,EAlarmStateNotifying);
	
	AcknowledgeAlarmL(alarm1);
	AcknowledgeAlarmL(alarm2);
	AcknowledgeAlarmL(alarm3);
	
	CheckAlarmStatesL(alarm1,EAlarmStateNotified);
    CheckAlarmStatesL(alarm2,EAlarmStateNotified);
	CheckAlarmStatesL(alarm3,EAlarmStateNotified);
	CheckIdListByStateL(EAlarmStateNotified);
	}
	

	

/**
@SYMTestCaseID PIM-APPSRV-ALARMSRV-CIT-1118-03
@SYMTestType CIT
@SYMTestPriority High
@SYMPREQ 1118
@SYMFssID AlarmServer 040
@SYMTestCaseDesc Notification of multiple expired alarms with snooze
@SYMTestActions Add three alarms, wait for first two alarms to expire and check
the states and alarm playing notification. Then snooze alarm2, wait for alarm3
to expire.
@SYMTestExpectedResults After snoozing alarm2, alarm 1 should start playing. Alarm2
will be interrupted by alarm3 expirying ang playing. After snooze period finishes
alarm2 will move into Notifying state and sound starts playing.
*/	

CTestMultipleAlarmsSnoozeStep::~CTestMultipleAlarmsSnoozeStep()
/**
 * Destructor
 */
	{
	}

CTestMultipleAlarmsSnoozeStep::CTestMultipleAlarmsSnoozeStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KTestMultiWithSnooze);
	}

TVerdict CTestMultipleAlarmsSnoozeStep::doTestStepL()
	{	
	SetTestStepResult(EPass);
	TInt iniSnoozeTime;	
	if (GetIntFromConfig(ConfigSection(),KSnooze,iniSnoozeTime))
	    {
		iSnoozeTime = iniSnoozeTime;
	    }
	else
	    {
	    _LIT(KErr,"Snooze Time Parameter Not Found");
		ERR_PRINTF1(KErr);
		SetTestStepResult(EFail);
		return TestStepResult();
	    }
		
	TRAPD(err,TestMultipleAlarmsSnoozeL())
	if (err != KErrNone)
	    {
	    _LIT(KLeft,"Test Muliple Alarms Snooze Left = %d");
	    INFO_PRINTF2(KLeft,err);
	    SetTestStepResult(EFail);	
	    }

	return TestStepResult();
	}
	
void CTestMultipleAlarmsSnoozeStep::TestMultipleAlarmsSnoozeL()
	{
	AddAlarmL(alarm1,iAlarmExpiry1);
	AddAlarmL(alarm2,iAlarmExpiry2);
	AddAlarmL(alarm3,iAlarmExpiry3);
	
	NotifyOnAlarmL(alarm1);
	WaitAndCheckExpiredAlarm(alarm1);
	WaitAndCheckSoundPlayStart(alarm1);
	NotifyOnSoundStartL(alarm1);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateQueued);
    CheckAlarmStatesL(alarm3,EAlarmStateQueued);	  
                                                    
    NotifyOnAlarmL(alarm2);   
    WaitAndCheckExpiredAlarm(alarm2);
    WaitAndCheckSoundPlayStart(alarm2);
    NotifyOnSoundStartL(alarm2);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm3,EAlarmStateQueued);
    
    //snooze alarm2 via console
    SnoozeAlarmL(alarm2,iSnoozeTime);
    NotifyOnSoundStartL(alarm1);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateSnoozed);
    CheckAlarmStatesL(alarm3,EAlarmStateQueued);
    
    NotifyOnAlarmL(alarm3); 
    WaitAndCheckExpiredAlarm(alarm3);
    NotifyOnSoundStartL(alarm3);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateSnoozed);                                                                                                                                                                                                                                                                                         
    CheckAlarmStatesL(alarm3,EAlarmStateNotifying);
	
    //After A2 snooze period expires	
    NotifyOnSoundStartL(alarm2);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm3,EAlarmStateNotifying);
    	
	CheckIdListByStateL(EAlarmStateNotifying);	
	AcknowledgeAllL();
	WaitAndCheckSoundPlayStop(alarm2);
	CheckIdListByStateL(EAlarmStateNotified);
	}
	



/**
@SYMTestCaseID PIM-APPSRV-ALARMSRV-CIT-1118-04
@SYMTestType CIT
@SYMTestPriority High
@SYMPREQ 1118
@SYMFssID AlarmServer 040
@SYMTestCaseDesc Notification of multiple expired alarms with invalid snooze
@SYMTestActions Add three alarms, wait for first two alarms to expire and check
the states and alarm playing notification. Then snooze alarm1,then snooze alarm2
with an invalid alarm id. Check alarm state and sound and wait for alarm3 to
expire
@SYMTestExpectedResults Snoozing with an invalid alarm id should not have any effect
on the correctly snoozed alarm1 or the alarm2
*/	

CTestMultiAlarmsInvalidSnoozeStep::~CTestMultiAlarmsInvalidSnoozeStep()
/**
 * Destructor
 */
	{
	}

CTestMultiAlarmsInvalidSnoozeStep::CTestMultiAlarmsInvalidSnoozeStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KTestMultiWithInvalidSnooze);
	}

TVerdict CTestMultiAlarmsInvalidSnoozeStep::doTestStepL()
	{	
	SetTestStepResult(EPass);
	TInt iniSnoozeTime;
	if (GetIntFromConfig(ConfigSection(),KSnooze,iniSnoozeTime))
	    {
		iSnoozeTime = iniSnoozeTime;
	    }
	else
	    {
		_LIT(KErr,"Snooze Time Parameter Not Found");
		ERR_PRINTF1(KErr);
		SetTestStepResult(EFail);
		return TestStepResult();
	    }
	
	TRAPD(err, TestMultipleAlarmsInvalidSnoozeL())
	if (err != KErrNone)
	    {
	    _LIT(KLeft,"Test Muliple Invalid Snooze Left = %d");
	    INFO_PRINTF2(KLeft,err);
	    SetTestStepResult(EFail);	
	    }

	return TestStepResult();
	}
	
void CTestMultiAlarmsInvalidSnoozeStep::TestMultipleAlarmsInvalidSnoozeL()
	{
	AddAlarmL(alarm1,iAlarmExpiry1);
	AddAlarmL(alarm2,iAlarmExpiry2);
	AddAlarmL(alarm3,iAlarmExpiry3);
	
	NotifyOnAlarmL(alarm1);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateQueued);
    CheckAlarmStatesL(alarm3,EAlarmStateQueued);	
    
    NotifyOnAlarmL(alarm2);   
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm3,EAlarmStateQueued);
    
    SnoozeAlarmL(alarm1,iSnoozeTime);
    CheckAlarmStatesL(alarm1,EAlarmStateSnoozed);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm3,EAlarmStateQueued);
    
    //snooze with wrong alarm ID to console alert server
    //this should not affect the other alarms
    SnoozeAlarmL(alarm3,iSnoozeTime);    
    CheckAlarmStatesL(alarm1,EAlarmStateSnoozed);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm3,EAlarmStateQueued);
    
    NotifyOnAlarmL(alarm3);
    CheckAlarmStatesL(alarm1,EAlarmStateSnoozed);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm3,EAlarmStateNotifying);
	
    //After A1 snooze period expires check console server
    //and alarmserver side for sound notifications
    NotifyOnSoundStartL(alarm1);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm3,EAlarmStateNotifying);
    	
	CheckIdListByStateL(EAlarmStateNotifying);
	AcknowledgeAllL();
	WaitAndCheckSoundPlayStop(alarm1);
	CheckIdListByStateL(EAlarmStateNotified);	
	}
	



/**
@SYMTestCaseID PIM-APPSRV-ALARMSRV-CIT-1118-05
@SYMTestType CIT
@SYMTestPriority High
@SYMPREQ 1118
@SYMFssID AlarmServer 
@SYMTestCaseDesc  Notification of Multiple Alarms, Pause Sound Playing
@SYMTestActions   Add an alarm, then add two further alarms with the same expiry time.
Wait for first alarm to expire then pause the sound playing. Wait for second and third
alarms to expire and check states.
@SYMTestExpectedResults Alarm which is paused is moved to the snoozed state due to a number
of alarms in the notifying state.
*/	

CTestMultiAlarmsPauseSoundStep::~CTestMultiAlarmsPauseSoundStep()
/**
 * Destructor
 */
	{
	}

CTestMultiAlarmsPauseSoundStep::CTestMultiAlarmsPauseSoundStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KTestPauseSound);
	}

TVerdict CTestMultiAlarmsPauseSoundStep::doTestStepL()
	{
	SetTestStepResult(EPass);
	TInt iniPauseTime;
	if (GetIntFromConfig(ConfigSection(),KPauTime,iniPauseTime))
	    {
		iPauseTime = iniPauseTime;
	    }
	else
	    {
		_LIT(KErr,"Pause Time Not Found : Use A Default");
		INFO_PRINTF1(KErr);
		iPauseTime = 5;
	    }
			
	TRAPD(err, TestMultiAlarmsPauseSoundL())
	if (err != KErrNone)
	    {
	    _LIT(KLeft,"Test Muliple Alarms With Pause Left = %d");
	    INFO_PRINTF2(KLeft,err);
	    SetTestStepResult(EFail);
	    return TestStepResult();	
	    }
	    
	TRAPD(err1, TestMultiAlarmsPauseSound2L())
	if (err1 != KErrNone)
	    {
	    _LIT(KLeft,"Test Muliple Alarms With Pause Left = %d");
	    INFO_PRINTF2(KLeft,err1);
	    SetTestStepResult(EFail);	
	    }

	return TestStepResult(); 
	}

void CTestMultiAlarmsPauseSoundStep::TestMultiAlarmsPauseSoundL()
    {	
    AddAlarmL(alarm1,iAlarmExpiry1);
	AddAlarmL(alarm2,iAlarmExpiry2);
		
	//check console alert server and alarm server
	//side for sound notifications
	NotifyOnAlarmL(alarm1);
	WaitAndCheckExpiredAlarm(alarm1);
    WaitAndCheckSoundPlayStart(alarm1);
    NotifyOnSoundStartL(alarm1);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateQueued);
    
    //pause sound playing for alarm1 via console alert server
    TTime pauseUntill;
    pauseUntill.UniversalTime();
    pauseUntill += TTimeIntervalMinutes(iPauseTime);
	TAlarmId alarmId = iAlarmIds.operator[](alarm1-1);
	TInt err = iconsoleSession.PauseSoundForAlarm(alarmId,pauseUntill);
	if (err != KErrNone)
	    {
	    _LIT(KErr,"PauseSound ForAlarm No %d : Error = %d");
	    ERR_PRINTF3(KErr,alarm1,err);
		User::Leave(err);
	    }
	else
	    {
		_LIT(KPau,"Request Pause Alarm No %d");
		INFO_PRINTF2(KPau,alarm1);
	    }
       
    NotifyOnAlarmL(alarm2);
    WaitAndCheckExpiredAlarm(alarm2);
    WaitAndCheckSoundPlayStart(alarm2);
    NotifyOnSoundStartL(alarm2);
    CheckAlarmStatesL(alarm1,EAlarmStateSnoozed);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying); 
    
    //after pause period expires
    NotifyOnSoundStartL(alarm1);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    CheckIdListByStateL(EAlarmStateNotifying);
    
    AcknowledgeAlarmL(alarm2);
    AcknowledgeAlarmL(alarm1);
	WaitAndCheckSoundPlayStop(alarm1);
	CheckIdListByStateL(EAlarmStateNotified);
	DeleteAllAlarmsL();
    }

void CTestMultiAlarmsPauseSoundStep::TestMultiAlarmsPauseSound2L()
    {
    //force alarm3 into snooze state
	AddAlarmL(alarm1,iAlarmExpiry1);
	AddAlarmL(alarm2,iAlarmExpiry2);
	AddAlarmL(alarm3,iAlarmExpiry3);
	
	NotifyOnSoundStartL(alarm1);
	
	CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateQueued);
    CheckAlarmStatesL(alarm3,EAlarmStateQueued);
    
    NotifyOnSoundStartL(alarm2);
	
	CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm3,EAlarmStateQueued);
    
    NotifyOnSoundStartL(alarm3);
    
    //pause sound playing for alarm3 via console alert server
    TTime pauseUntill;
    pauseUntill.UniversalTime();
    pauseUntill += TTimeIntervalMinutes(iPauseTime);
	TAlarmId alarmId = iAlarmIds.operator[](alarm3-1);
	TInt err = iconsoleSession.PauseSoundForAlarm(alarmId,pauseUntill);
	if (err != KErrNone)
	    {
	    _LIT(KErr,"PauseSound ForAlarm No %d : Error = %d");
	    ERR_PRINTF3(KErr,alarm3,err);
		User::Leave(err);
	    }
	else
	    {
		_LIT(KPau,"Request Pause Alarm No %d");
		INFO_PRINTF2(KPau,alarm3);
	    }
 
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm3,EAlarmStateSnoozed);
    
    NotifyOnSoundStartL(alarm2);
    
    //wait for pause to finish
    NotifyOnSoundStartL(alarm3);
    AcknowledgeAllL();    
    }

/**
@SYMTestCaseID PIM-APPSRV-ALARMSRV-CIT-1118-06
@SYMTestType CIT
@SYMTestPriority High
@SYMPREQ 1118
@SYMFssID AlarmServer 041
@SYMTestCaseDesc Notification of Multiple Expired Alarm with Clear
@SYMTestActions  Add three alarms withe overlapping expiry time. Wait for alarm1
to expire, check all states and sound play for alarm1. Wait for alarm2 to expire, 
check all alarm states and sound play for alarm2. Clear alarm2 only, wait for 
alarm3 to expire. 
@SYMTestExpectedResults Clearing alarm2 moves the state to notified. Alarm1 is still
in the notifying state and sound play should start for this alarm. After alarm3
expires the alarm moves to notifying and starts sound playing.
*/	

CTestMultipleAlarmsClearStep::~CTestMultipleAlarmsClearStep()
/**
 * Destructor
 */
	{
	}

CTestMultipleAlarmsClearStep::CTestMultipleAlarmsClearStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KTestClear);
	}

TVerdict CTestMultipleAlarmsClearStep::doTestStepL()
	{	
	SetTestStepResult(EPass);
	TRAPD(err, TestMultipleAlarmsClearL())
	if (err != KErrNone)
	    {
	    _LIT(KLeft,"Test Muliple Alarms With Clear Left = %d");
	    INFO_PRINTF2(KLeft,err);
	    SetTestStepResult(EFail);	
	    }

	return TestStepResult();
	}

void CTestMultipleAlarmsClearStep::TestMultipleAlarmsClearL()
    {
    AddAlarmL(alarm1,iAlarmExpiry1);
	AddAlarmL(alarm2,iAlarmExpiry2);
	AddAlarmL(alarm3,iAlarmExpiry3);
	
	NotifyOnAlarmL(alarm1);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateQueued);
    CheckAlarmStatesL(alarm3,EAlarmStateQueued);
    
    NotifyOnAlarmL(alarm2);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm3,EAlarmStateQueued);
    
    AcknowledgeAlarmL(alarm2);
    
    WaitAndCheckSoundPlayStart(alarm1);
    WaitAndCheckSoundPlayStop(alarm2);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateNotified);
    CheckAlarmStatesL(alarm3,EAlarmStateQueued);
    
    NotifyOnAlarmL(alarm3);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateNotified);
    CheckAlarmStatesL(alarm3,EAlarmStateNotifying);
    
    AcknowledgeAllL();
    CheckIdListByStateL(EAlarmStateNotified);
    }
    
    


/**
@SYMTestCaseID PIM-APPSRV-ALARMSRV-CIT-1118-07
@SYMTestType CIT
@SYMTestPriority High
@SYMPREQ 1118
@SYMFssID AlarmServer 041
@SYMTestCaseDesc Notification of Multiple Expired Alarm with Invalid Clear
@SYMTestActions  Add three alarms withe overlapping expiry time. Wait for alarm1
to expire, check all states and sound play for alarm1. Wait for alarm2 to expire, 
check all alarm states and sound play for alarm2. Attempt to clear alarm with an
invalid alarm ID. Wait for alarm3 to expire. 
@SYMTestExpectedResults Attempting to clear using and invalid alarm ID should not
affect any of the expired alarms, all alarms should finally be in the notifying state.
*/	

CTestMultiAlarmsInvalidClearStep::~CTestMultiAlarmsInvalidClearStep()
/**
 * Destructor
 */
	{
	}

CTestMultiAlarmsInvalidClearStep::CTestMultiAlarmsInvalidClearStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KTestInvalidClear);
	}

TVerdict CTestMultiAlarmsInvalidClearStep::doTestStepL()
	{	
	SetTestStepResult(EPass);
	TRAPD(err, TestMultipleAlarmsInvalidClearL())
	if (err != KErrNone)
	    {
	    _LIT(KLeft,"Test Muliple Alarms Invalid Clear = %d");
	    INFO_PRINTF2(KLeft,err);
	    SetTestStepResult(EFail);	
	    }

	return TestStepResult();
	}

void CTestMultiAlarmsInvalidClearStep::TestMultipleAlarmsInvalidClearL()
    {	
    AddAlarmL(alarm1,iAlarmExpiry1);
	AddAlarmL(alarm2,iAlarmExpiry2);
	AddAlarmL(alarm3,iAlarmExpiry3);
	
	NotifyOnAlarmL(alarm1);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateQueued);
    CheckAlarmStatesL(alarm3,EAlarmStateQueued);
    
    NotifyOnAlarmL(alarm2);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm3,EAlarmStateQueued);
    
    //attempt to clear an alarm in an incorrect state
    //and verify no effect on notifying alarm
    AcknowledgeAlarmL(alarm3);
    WaitAndCheckSoundPlayStart(alarm2);
     
    NotifyOnAlarmL(alarm3);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm3,EAlarmStateNotifying);
        
    AcknowledgeAllL();
    WaitAndCheckSoundPlayStop(alarm3);
    CheckIdListByStateL(EAlarmStateNotified);
    }




/**
@SYMTestCaseID PIM-APPSRV-ALARMSRV-CIT-1118-08
@SYMTestType CIT
@SYMTestPriority High
@SYMPREQ 1118
@SYMFssID AlarmServer 038
@SYMTestCaseDesc Alarm Server Queues Multiple Alarms that Cannot be Notified
@SYMTestActions  Ensure that Console Alert Server can handle max two alarms.
Add three alarms and enable. Check alarm states for all alarms after alarm1 and
alarm2 have expired. Wait for alarm3 to expire and check states.Clear alarm2 then
check alarms states and check alarm sound play for alarm3.
For the second part of the test add three alarms, wait for alarm1 to expire and
play sound then pause it. Wait for alarm2 and alarm3 to expire, check states of
all alarms. Wait for pause period on alarm1 to expire, check all alarm states.
For the third part of the test add three alarms with overlapping times then check
states, then acknowledge all alarms. Check the states again.
@SYMTestExpectedResults For the first part of the test the first two alarms should 
go into the notifying state. After alarm3 expires it gets queued in 
waiting to notify state. After clearing alarm2 then alarm3 should move to the notifying
state and sound play will start for this alarm.
For the second part of the test after alarm1 is paused and the other two alarms expire
then alarm1 should move into the snoozed state while the other two will move
to notify. After the pause timer on alarm1 expires it should move to state waiting to
notify.
For the third part of the test, acknowledging all alarms causes alarms that are in
the notifying AND waiting to notify states to transition to the notified states.
*/	

CTestMultipleAlarmsQueueStep::~CTestMultipleAlarmsQueueStep()
/**
 * Destructor
 */
	{
	}

CTestMultipleAlarmsQueueStep::CTestMultipleAlarmsQueueStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KTestQueue);
	}

TVerdict CTestMultipleAlarmsQueueStep::doTestStepL()
	{	
	SetTestStepResult(EPass);
	TInt iniPauseTime;
	if (GetIntFromConfig(ConfigSection(),KPauTime,iniPauseTime))
	    {
		iPauseTime = iniPauseTime;
	    }
	else
	    {
		_LIT(KErr,"Pause Time Not Found : Use A Default");
		INFO_PRINTF1(KErr);
		iPauseTime = 4;
	    }
	
	TRAPD(err,TestMultipleAlarmsQueueL())
	if (err != KErrNone)
	    {
	    _LIT(KLeft,"Test Muliple Alarms Queue Left = %d");
	    INFO_PRINTF2(KLeft,err);	 
	    SetTestStepResult(EFail);
	    return TestStepResult();	
	    }
	    	
	TRAPD(err1,TestMultAlarmsQFromSnoozeL())
	if (err1 != KErrNone)
	    {
	    _LIT(KLeft,"Test Mult Alarms Q From Snooze Has Left = %d");
	    ERR_PRINTF2(KLeft,err1);
		SetTestStepResult(EFail);
	    }

	return TestStepResult();
	}

void CTestMultipleAlarmsQueueStep::TestMultipleAlarmsQueueL()
    {
    AddAlarmL(alarm1,iAlarmExpiry1);
	AddAlarmL(alarm2,iAlarmExpiry2);
	AddAlarmL(alarm3,iAlarmExpiry3);
	
	NotifyOnAlarmL(alarm1);
	WaitAndCheckExpiredAlarm(alarm1);
	WaitAndCheckSoundPlayStart(alarm1);
	NotifyOnSoundStartL(alarm1);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateQueued);
    CheckAlarmStatesL(alarm3,EAlarmStateQueued);
	
	NotifyOnAlarmL(alarm2);
    WaitAndCheckExpiredAlarm(alarm2);
    WaitAndCheckSoundPlayStart(alarm2);
    NotifyOnSoundStartL(alarm2);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm3,EAlarmStateQueued);
    
    WaitAndCheckExpiredAlarm(alarm3);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm3,EAlarmStateWaitingToNotify);
    
    AcknowledgeAlarmL(alarm2);
    
    NotifyOnSoundStartL(alarm3);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateNotified);
    CheckAlarmStatesL(alarm3,EAlarmStateNotifying);   
    
    AcknowledgeAlarmL(alarm1);
    AcknowledgeAlarmL(alarm3);
    
    WaitAndCheckSoundPlayStop(alarm3);
    CheckIdListByStateL(EAlarmStateNotified);
    DeleteAllAlarmsL();
    }
    
void CTestMultipleAlarmsQueueStep::TestMultAlarmsQFromSnoozeL()
    {    		
	AddAlarmL(alarm1,iAlarmExpiry1);
	AddAlarmL(alarm2,iAlarmExpiry2);
    AddAlarmL(alarm3,iAlarmExpiry3);
	
	WaitAndCheckExpiredAlarm(alarm1);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateQueued);
    CheckAlarmStatesL(alarm3,EAlarmStateQueued);
    
    User::After(3 * 1000000);
    
    TTime pauseUntill;
    pauseUntill.UniversalTime();
    pauseUntill += TTimeIntervalMinutes(iPauseTime);
	TAlarmId alarmId = iAlarmIds.operator[](alarm1-1);
	TInt err = iconsoleSession.PauseSoundForAlarm(alarmId,pauseUntill);
	if (err != KErrNone)
	    {
	    _LIT(KErr,"PauseSound ForAlarm No %d : Error = %d");
	    ERR_PRINTF3(KErr,alarm1,err);
		User::Leave(err);
	    }
	else
	    {
		_LIT(KPau,"Request Pause Alarm No %d");
		INFO_PRINTF2(KPau,alarm1);
	    }
    
    NotifyOnAlarmL(alarm2);
    CheckAlarmStatesL(alarm1,EAlarmStateSnoozed);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm3,EAlarmStateQueued);
    
    WaitAndCheckExpiredAlarm(alarm3);
    CheckAlarmStatesL(alarm1,EAlarmStateSnoozed);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm3,EAlarmStateNotifying);
    
    User::After(30 * 1000000);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm3,EAlarmStateWaitingToNotify);
    
    AcknowledgeAllL();
	CheckIdListByStateL(EAlarmStateNotified);
    DeleteAllAlarmsL();
    }

    



/**
@SYMTestCaseID PIM-APPSRV-ALARMSRV-CIT-1118-09
@SYMTestType CIT
@SYMTestPriority High
@SYMPREQ 1118
@SYMFssID AlarmServer 037
@SYMTestCaseDesc Silence Playing Alarm in the Notifying State
@SYMTestActions Add two alarms and ebale, wait for alarm1 to expire and
check alarm state and sound play. Wait for Alarm2 to expire and check
alarm state and sound play start. Silence alarm2 via the console alert
server then check alarm sound stop and alarm states. Wait for play inteval
for alarm2 to start again and check sound play and state.
@SYMTestExpectedResults After silencing alarm2 the state should stii be
Notifying, when alarm play interval starts again the state should stay
as Notifying and a sound play start should be received.
*/

CTestSilenceAlarmStep::~CTestSilenceAlarmStep()
/**
 * Destructor
 */
	{
	}

CTestSilenceAlarmStep::CTestSilenceAlarmStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KTestSilenceStep);
	}

TVerdict CTestSilenceAlarmStep::doTestStepL()
	{	
	SetTestStepResult(EPass);
	
	TRAPD(err,TestSilenceAlarmL());
	if (err != KErrNone)
	    {
	    _LIT(KComplete,"Test Silence Alarm Left = %d");
	    INFO_PRINTF2(KComplete,err);
		SetTestStepResult(EFail);
	    }
	    
	return TestStepResult();
	}
	
void CTestSilenceAlarmStep::TestSilenceAlarmL()
    {
	AddAlarmL(alarm1,iAlarmExpiry1);
	AddAlarmL(alarm2,iAlarmExpiry2);
	
	NotifyOnAlarmL(alarm1);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateQueued);
    
    NotifyOnAlarmL(alarm2);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    
    //Let alarm play for 3 seconds then silence via console server
    User::After(3 * 1000000);
    TAlarmId alarmId = iAlarmIds.operator[](alarm2-1);
	TInt err = iconsoleSession.SilenceAlarm(alarmId);
	if (err != KErrNone)
	    {
	    _LIT(KErr,"SilenceAlarm For No %d : Error = %d");
	    ERR_PRINTF3(KErr,alarm2,err);
		User::Leave(err);
	    }
	else
	    {
		_LIT(KSilence,"Request Silence Alarm No %d");
		INFO_PRINTF2(KSilence,alarm2);
	    }
	    
    WaitAndCheckSoundPlayStop(alarm2);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    
    //Alarm will resume playing at the next interval
    WaitAndCheckSoundPlayStart(alarm2);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    
    AcknowledgeAllL();
    WaitAndCheckSoundPlayStop(alarm2);
    CheckIdListByStateL(EAlarmStateNotified);
    }




/**
@SYMTestCaseID PIM-APPSRV-ALARMSRV-CIT-1118-10
@SYMTestType CIT
@SYMTestPriority High
@SYMPREQ 1118
@SYMFssID AlarmServer 039
@SYMTestCaseDesc Backwards Compatibility for Single Alarm Notification
@SYMTestActions  Set the cosole alert server to handle single expired alarms.
Add three alarms and enable. Wait for first alarm to notify, check state and 
alarm sound play. Wait for second and third alarm to expire and check states.
After all alarms have expired acknowledge first alarm and check states, continue
to do the same for the other two alarms. Second part of test adds three alarms,
waits for first alarm to notify and check alarm states using the old API. Us the 
old API to clear notifying alarms
@SYMTestExpectedResults After the three alarms have expired only the first alarm
should be in state notifying, the others should be queued, alarm sound should be
playing.After clearing the first alarm the second alarm should move to notifying, 
and again after acking the second the third alarm should move to notifying. This
is the old behaviour. In the second part of the test alarms notifying will move
to notified when calling the old ack API.
*/	

CTestBackwardsCompatibilityStep::~CTestBackwardsCompatibilityStep()
/**
 * Destructor
 */
	{
	}

CTestBackwardsCompatibilityStep::CTestBackwardsCompatibilityStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KBackwardsComp);
	}

TVerdict CTestBackwardsCompatibilityStep::doTestStepL()
	{	
	SetTestStepResult(EPass);
		
	TRAPD(err,TestBackwardsCompL());
	if (err != KErrNone)
	    {
	    _LIT(KLeft,"Test Backwards Compatibility Left = %d");
	    INFO_PRINTF2(KLeft,err);
		SetTestStepResult(EFail);
		return TestStepResult();
	    }
	    
	TRAPD(err1,TestBackwardsComp2L())
	if (err1 != KErrNone)
	    {
		_LIT(KLeft,"Test Backwards Compatibility2 Left = %d");
	    INFO_PRINTF2(KLeft,err1);
		SetTestStepResult(EFail);
	    }

	return TestStepResult();
	}
	
void CTestBackwardsCompatibilityStep::TestBackwardsCompL()
    {		
	AddAlarmL(alarm1,iAlarmExpiry1);
	AddAlarmL(alarm2,iAlarmExpiry2);
	AddAlarmL(alarm3,iAlarmExpiry3);
	
	WaitAndCheckExpiredAlarm(alarm1);
    WaitAndCheckSoundPlayStart(alarm1);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateQueued);
    CheckAlarmStatesL(alarm3,EAlarmStateQueued);
    
    WaitAndCheckExpiredAlarm(alarm2);
    CheckAlarmStatesL(alarm1,EAlarmStateNotified);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm3,EAlarmStateQueued);
    
    WaitAndCheckExpiredAlarm(alarm3);
    CheckAlarmStatesL(alarm3,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm1,EAlarmStateNotified);
    CheckAlarmStatesL(alarm2,EAlarmStateNotified);
     
    User::After(5* 1000000);    
    CheckIdListByStateL(EAlarmStateNotified);
    DeleteAllAlarmsL();	    
    }
    
void CTestBackwardsCompatibilityStep::TestBackwardsComp2L()
    {
	AddAlarmL(alarm1,iAlarmExpiry1);
	AddAlarmL(alarm2,iAlarmExpiry2);
	AddAlarmL(alarm3,iAlarmExpiry3);
	
	WaitAndCheckExpiredAlarm(alarm1);
    WaitAndCheckSoundPlayStart(alarm1);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateQueued);
    CheckAlarmStatesL(alarm3,EAlarmStateQueued);    
    //Use the old API to check that alarm1 is in the
    //correct notifying state
    IsAlarmNotifying();
    
    WaitAndCheckExpiredAlarm(alarm2);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateWaitingToNotify);
    CheckAlarmStatesL(alarm3,EAlarmStateQueued);    
    //Check alarm1 is in notifying state
    IsAlarmNotifying();
    
    WaitAndCheckExpiredAlarm(alarm3);
    CheckAlarmStatesL(alarm1,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm2,EAlarmStateWaitingToNotify);
    CheckAlarmStatesL(alarm3,EAlarmStateWaitingToNotify);    
    //Check alarm1 is in notifying state
    IsAlarmNotifying();
    
    //Use the old API to clear the notifying alarm1
    AcknowledgeAlarm();
    User::After(5 * 1000000);     
    CheckAlarmStatesL(alarm1,EAlarmStateNotified);
    CheckAlarmStatesL(alarm2,EAlarmStateNotifying);
    CheckAlarmStatesL(alarm3,EAlarmStateWaitingToNotify);
    WaitAndCheckSoundPlayStart(alarm2);
    
    //Use the old API to clear the notifying alarm2
    AcknowledgeAlarm();    
    User::After(5 * 1000000); 
    CheckAlarmStatesL(alarm1,EAlarmStateNotified);
    CheckAlarmStatesL(alarm2,EAlarmStateNotified);
    CheckAlarmStatesL(alarm3,EAlarmStateNotifying);
    WaitAndCheckSoundPlayStart(alarm3);
    
    //Use the old API to clear the notifying alarm3
    AcknowledgeAlarm(); 
    User::After(5 * 1000000);   
    CheckAlarmStatesL(alarm1,EAlarmStateNotified);
    CheckAlarmStatesL(alarm2,EAlarmStateNotified);
    CheckAlarmStatesL(alarm3,EAlarmStateNotified);
    }

void CTestBackwardsCompatibilityStep::IsAlarmNotifying()
    {
	TBool isNotifyingState = iconsoleSession.IsNotifying();
    if (isNotifyingState)
        {
    	_LIT(KNotify,"Alarm is in the Notifying State");
    	INFO_PRINTF1(KNotify);
        }
    else
        {
    	_LIT(KNoNotify,"Alarm is NOT in the Notifying State");
    	INFO_PRINTF1(KNoNotify);
        }
    }
   
void CTestBackwardsCompatibilityStep::AcknowledgeAlarm()
    {
	_LIT(Kack,"Requesting to Clear Notifying Alarm Using Old API");
    INFO_PRINTF1(Kack);
    iconsoleSession.AcknowledgeAlarm();
    }

    
    
    
    
    
