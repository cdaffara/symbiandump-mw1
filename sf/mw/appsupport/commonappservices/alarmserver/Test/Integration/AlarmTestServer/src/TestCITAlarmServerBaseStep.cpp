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

#include "TestCITAlarmServerBaseStep.h"
#include <test/testexecutelog.h>
using namespace multialarms;

TVerdict CTestCITAlarmServerBaseStep::doTestStepPreambleL()
/**
 * @return - TVerdict
 * Implementation of CTestStep base class virtual
 * It is used for doing all initialisation common to derived classes in here.
 * Make it being able to leave if there are any errors here as there's no point in
 * trying to run a test step if anything fails.
 * The leave will be picked up by the framework.
 */
	{
	TInt iniAlarmExpiry1;
	TInt iniAlarmExpiry2;
	TInt iniAlarmExpiry3;
	_LIT(KInfo,"Expiry Time Not in File");

	if (GetIntFromConfig(ConfigSection(),KExpiry1,iniAlarmExpiry1))
	    {
		iAlarmExpiry1 = iniAlarmExpiry1;
	    }
	else
	    {
		INFO_PRINTF1(KInfo);
	    }
	    
	if (GetIntFromConfig(ConfigSection(),KExpiry2,iniAlarmExpiry2))
	    {
		iAlarmExpiry2 = iniAlarmExpiry2;
	    }
	else
	    {
		INFO_PRINTF1(KInfo);
	    }
	    
	if (GetIntFromConfig(ConfigSection(),KExpiry3,iniAlarmExpiry3))
	    {
		iAlarmExpiry3 = iniAlarmExpiry3;
	    }
	else
	    {
		INFO_PRINTF1(KInfo);
	    }
	
	ConnectToAlarmServerL();
	//Perform initial clean up in case other tests havent cleaned up
	if (iMaxAlarms > 1)
	    {
		AcknowledgeAllL();
	    }
	else
	    {
		iconsoleSession.AcknowledgeAlarm(); 
	    }
	DeleteAllAlarmsL();
	return TestStepResult();
	}

TVerdict CTestCITAlarmServerBaseStep::doTestStepPostambleL()
/**
 * @return - TVerdict
 * Implementation of CTestStep base class virtual
 * It is used for doing all after test treatment common to derived classes in here.
 * Make it being able to leave
 * The leave will be picked up by the framework.
 */
	{
	_LIT(KComplete,"Test Complete: Now clean up");
	INFO_PRINTF1(KComplete);
	//Cleanup by Acking and Deleting all alarms in case a teststep
	//left leaving an alarm in the notify state, this is not part of
	//the actual testcase
	if (iMaxAlarms > 1)
	    {
		AcknowledgeAllL();
	    }
	else
	    {
		iconsoleSession.AcknowledgeAlarm(); 
	    }
	DeleteAllAlarmsL();
	return TestStepResult();
	}


CTestCITAlarmServerBaseStep::~CTestCITAlarmServerBaseStep()
	{
    _LIT(KDes,"Disconnect from Alarm and Alert Servers");
	INFO_PRINTF1(KDes);
	if (iMaxAlarms > 1)
	    {
	    iconsoleSession.CancelNotifications();	
	    }
	iAlarmIds.Close();
	ialarmSession.Close();
	iconsoleSession.Close();	
	INFO_PRINTF2(_L("AlarmTestStep[%X] : Terminating ConsoleAlarmAlertServer "), this);
	iAlertServerProcess.Terminate(0); 
	INFO_PRINTF1(_L("ConsoleAlarmAlertServer Terminated"));
	}

CTestCITAlarmServerBaseStep::CTestCITAlarmServerBaseStep()
	{
	}

/**
This method is called by the preamble to connect to the alarm and
alert servers. The max number of alarms in the ini is used to decide
if the connection should invoke the extended mode or the old single
alarm mode.
*/
void CTestCITAlarmServerBaseStep::ConnectToAlarmServerL()
    {
    _LIT(KCon,"Connect to Alarm and Alert Server");
    INFO_PRINTF1(KCon);
        
    const TUid KServerUid2={0x1000008D};
	const TUid KServerUid3={0x101F502A};
	const TUidType serverUid(KNullUid,KServerUid2,KServerUid3);
	_LIT(KConsoleAlarmAlertServerImg,"ConsoleAlarmAlertServer");
	
	if (!GetIntFromConfig(ConfigSection(),KMaxAlarms,iMaxAlarms))
	    {
		_LIT(KMax,"Max Number of Alarms Not Found");
		INFO_PRINTF1(KMax);
		User::Leave(KErrNotFound);
	    }
        
    //assume multiple alarms handling if iMaxAlarms>1
    if (iMaxAlarms>1)
        {
        iAlertServerProcess.Create(KConsoleAlarmAlertServerImg,KNullDesC,serverUid);        
	    iAlertServerProcess.Resume();		 
		User::After(500000);    
    	TESTL(iconsoleSession.Connect() == KErrNone);        
        TESTL(iconsoleSession.SetExtendedMode() == KErrNone); 
        TESTL(iconsoleSession.SetMaxAlarms(iMaxAlarms) == KErrNone);	       
        TESTL(ialarmSession.Connect() == KErrNone);    
        }
    //assume old single alarm mode if iMaxAlarms=1
    else
        {
        TBuf<256> cmdline;
        cmdline.Append(EKeyEscape);
     	cmdline.Append(EKeyEscape);
    	cmdline.Append(EKeyEscape);
    	
        iAlertServerProcess.Create(KConsoleAlarmAlertServerImg,cmdline,serverUid);
	    iAlertServerProcess.Resume();		 
		User::After(500000);
        TESTL(ialarmSession.Connect() == KErrNone);       
    	TESTL(iconsoleSession.Connect() == KErrNone);
        }	
    }

/**
This method adds a bsic clock alarm to expire as defined by the passed
in time interval value. The alarm ID is store in an array for easy
access by other methods. This and other methods use an aAlarmNo parameter
for easy referencing during the test and also for use when reading the test
specification.
*/
void CTestCITAlarmServerBaseStep::AddAlarmL(TInt aAlarmNo, TTimeIntervalMinutes aTimeInterval)
    {
    _LIT(KAlarmMessage,"This is Alarm Number ");
    _LIT(KTest," In Test");
    TBuf<KMaxAlarmMessageLength> messageBuff;
    messageBuff = KAlarmMessage;
    messageBuff.AppendNum(aAlarmNo);  
    messageBuff.Append(KTest);
    messageBuff.Append(TestStepName());     
    TASShdAlarm alarm;
    alarm.Message() = messageBuff;
	alarm.Category() = KASCliCategoryClock;
	TTime time;
	time.UniversalTime();
    time += aTimeInterval;
	alarm.SetUtcNextDueTime(time);	
	TInt alarmRet = ialarmSession.AlarmAdd(alarm);
	if (alarmRet != KErrNone)
	    {
	    _LIT(KAddAlarm,"Method AlarmAdd() for Alarm %d returned error code = %d");
	    ERR_PRINTF3(KAddAlarm,aAlarmNo,alarmRet);
		User::Leave(alarmRet);
	    }
    
	TAlarmId id = alarm.Id();
	if (iAlarmIds.Count() != (aAlarmNo-1))
	    {
	    _LIT(KParam,"Add Alarm Numbers in Sequence");
	    INFO_PRINTF1(KParam);
		User::Leave(KErrArgument);
	    }
	TInt insertRet = iAlarmIds.Insert(id,(aAlarmNo-1));
	if (insertRet != KErrNone)
	    {
		_LIT(KInsert,"Insert() action returned error code = %d");
		ERR_PRINTF2(KInsert,insertRet);
		User::Leave(insertRet);
	    }
    }
    
/**
This method delete all alarms that are found to have the clock category.
Only alarms with the clock category are used in the tests.
*/
void CTestCITAlarmServerBaseStep::DeleteAllAlarmsL()
    {
    //only use one category throught the tests
    RArray<TAlarmId> idList;
    iAlarmIds.Reset();
	TInt res = ialarmSession.AlarmDeleteAllByCategory(KASCliCategoryClock,EFalse);
	if (res != KErrNone)
	    {
	    _LIT(KDel,"AlarmDeleteAllByCategory() returnd error code = %d");
	    ERR_PRINTF2(KDel,res);
		User::Leave(res);
	    }
	else
	    {
		_LIT(KDel,"Requested Delete All Alarms");
		INFO_PRINTF1(KDel);
	    }
	    
	//just in case delete everything
	_LIT(KDeleteAll,"Delete All Alarm IDs");
	INFO_PRINTF1(KDeleteAll);
	ialarmSession.GetAlarmIdListL(idList);
	for (TInt n = 0; n < idList.Count(); n++)
	    {
		TESTL(ialarmSession.AlarmDelete(idList[n]) == KErrNone);
	    }
	
	idList.Close();
	}

/**
This method is used on the alarm server side to listen for alarm server
notificatios that an alarm has expired. The method checks the alarm ID
received against the alarm ID that was stored when the alarm was added.
*/	
void CTestCITAlarmServerBaseStep::WaitAndCheckExpiredAlarm(TInt aAlarmNo)
    {
	TAlarmId alarmId;
	TRequestStatus status;
	TAlarmId expectedAlarmId = iAlarmIds[aAlarmNo-1];
	for(;;)
		{
		ialarmSession.NotifyChange(status,alarmId);
		User::WaitForRequest(status);
				    
		if (status.Int() == EAlarmChangeEventTimerExpired && alarmId == expectedAlarmId)
		    { 		              		
			 _LIT(KAlarmExpired,"Alarm Server Side Notification : Alarm No %d Has Expired");
			 INFO_PRINTF2(KAlarmExpired,aAlarmNo);
			 break;
			}
		}	
    }
    
/**
This method is used on the alarm server side to listen for alarm server
notificatios that an alarm sound has started to play. The method checks the
alarm ID received against the alarm ID that was stored when the alarm was added.
*/
void CTestCITAlarmServerBaseStep::WaitAndCheckSoundPlayStart(TInt aAlarmNo)
    {
	TAlarmId alarmId;
	TRequestStatus status;
	TAlarmId expectedAlarmId = iAlarmIds[aAlarmNo-1];
	for(;;)
		{
		ialarmSession.NotifyChange(status,alarmId);
		User::WaitForRequest(status);
		if	(status.Int() == EAlarmChangeEventSoundPlaying && alarmId == expectedAlarmId)
			{
			_LIT(KSoundStart,"Alarm Server Side Notification : Sound Has Started To Play For Alarm No : %d");
			INFO_PRINTF2(KSoundStart,aAlarmNo);
			break;
			}
		}
    }

/**
This method is used on the alarm server side to listen for alarm server
notificatios that an alarm sound has stopped playing. The method checks the
alarm ID received against the alarm ID that was stored when the alarm was added.
*/  
void CTestCITAlarmServerBaseStep::WaitAndCheckSoundPlayStop(TInt aAlarmNo)
    {
	TAlarmId alarmId;
	TRequestStatus status;
	TAlarmId expectedAlarmId = iAlarmIds[aAlarmNo-1];
	for(;;)
		{
		ialarmSession.NotifyChange(status,alarmId);
		User::WaitForRequest(status);
		if	(status.Int() == EAlarmChangeEventSoundStopped && alarmId == expectedAlarmId)
			{
			_LIT(KSoundStop,"Alarm Server Side Notification : Sound Has Stopped Playing For Alarm No : %d");
			INFO_PRINTF2(KSoundStop,aAlarmNo);
			break;
			}
		}
    }
    
/**
This method requests the alarm server to provide alarm state details for a
particular alarm.
*/
void CTestCITAlarmServerBaseStep::CheckAlarmStatesL(TInt aAlarmNo, TAlarmState aExpectedState)
    {
    TASShdAlarm retAlarm;
    TAlarmId wantedAlarmId = iAlarmIds[aAlarmNo-1];
    TInt retValue = ialarmSession.GetAlarmDetails(wantedAlarmId,retAlarm);
    if (retValue != KErrNone)
        {
        _LIT(KLeft,"GetAlarmDetails() in CheckAlarmStatesL() Left, err = %d");
        INFO_PRINTF2(KLeft,retValue);
    	User::Leave(retValue);
        }    
	TAlarmState state = retAlarm.State();
	if (state != aExpectedState)
	    {
		_LIT(KWrongState,"Alarm Server Side Notification: Alarm No %d Found To Be In An Unexpected State : %d");
		ERR_PRINTF3(KWrongState,aAlarmNo,state);
		User::Leave(KErrGeneral); 
	    }
	else
	    {
		_LIT(KStateOK,"Alarm Server Side Notification: Alarm No %d Found To Be In The Correct State : %d");
		INFO_PRINTF3(KStateOK,aAlarmNo,state);
	    }
	
    }
    
/**
This method requests that the alarm server returns a list of alarm ID's for
a particular alarm state that the test passes in. The alarm ID's are compared
agains the list of ID's that have been built up by adding alarms.
*/
void CTestCITAlarmServerBaseStep::CheckIdListByStateL(TAlarmState aWantedState)
    {
	RArray<TAlarmId> idList;
	ialarmSession.GetAlarmIdListByStateL(aWantedState,idList);
	if (iAlarmIds.Count() != idList.Count())
	    {
		_LIT(KWrongNo,"Get ID List By State : Unexpected Number of Alarms in State %d");
		_LIT(KWrongNumb,"Expected Number = %d : Received Number = %d");
		ERR_PRINTF2(KWrongNo,aWantedState);
		ERR_PRINTF3(KWrongNumb,iAlarmIds.Count(),idList.Count());
		
		_LIT(KPrintWrongNumb,"Unexpected ID = %d");
		for(TInt n = 0; n < idList.Count(); n++)
		    {
			ERR_PRINTF2(KPrintWrongNumb,idList[n]);
		    }
		idList.Close();
		User::Leave(KErrGeneral);
	    }
        
    TInt count = idList.Count();
    for (TInt i = 0; i < count; i++)
        {
        for (TInt n = 0; n < count; n++)
            {
            if (iAlarmIds.operator[](i) == idList.operator[](n))
        	    {
            	 _LIT(KStateOK,"GetIDListByState : Alarm No %d In CorrectState: Received ID = %d");
        		 INFO_PRINTF3(KStateOK, i+1,idList.operator[](n));
            	 break;
                }
            else if (n == (count-1))
                {
                _LIT(KStateNotOK,"GetIDListByState : Alarm No %d In Incorrect State: Received ID = %d");
        		ERR_PRINTF3(KStateNotOK, i+1,idList[n]);
        		idList.Close();
        	    User::Leave(KErrGeneral);
                }
            else
                {
            	//default
                }
 
            }    	         	
        }    
        
    idList.Close();
    }
    
/**
This method is used on the console alarm alert server side to listen for
notificatios that the console alert server has received the SetAlarm command.
The method checks the alarm ID derived from the received alarm object
against the alarm ID that was stored when the alarm was added.
*/
void CTestCITAlarmServerBaseStep::NotifyOnAlarmL(TInt aAlarmNo)
    {
	TASShdAlarm notifyingAlarm;
	TRequestStatus status;
	TAlarmId alarmId;	
	TAlarmId expectedAlarmId = iAlarmIds[aAlarmNo-1];	
	for(;;)
		{
		TESTL(iconsoleSession.NotifyOnAlarm(notifyingAlarm,status) == KErrNone);
		User::WaitForRequest(status);
		alarmId = notifyingAlarm.Id();
		if	(alarmId == expectedAlarmId)
			{
			if (status.Int() == KErrNone)
			    {
				_LIT(KNotify,"Console Alert Server : Received SetAlarm For Alarm No : %d");
			    INFO_PRINTF2(KNotify,aAlarmNo);
			    break;
			    }
			else
			    {
				_LIT(KNotify,"Console Alert Server : Error code Received in NotifyOnAlarmL : status = : %d");
			    ERR_PRINTF2(KNotify,status.Int());
			    User::Leave(status.Int());
			    }
			}
		}
    }

/**
This method is used on the console alarm alert server side to listen for
notificatios that the console alert server has received the StartPlayingSound
command. The method checks the alarm ID against the alarm ID that was stored 
when the alarm was added.
*/  
void CTestCITAlarmServerBaseStep::NotifyOnSoundStartL(TInt aAlarmNo)
    {
    TBuf<256> soundFile;
	TRequestStatus status;
	TAlarmId alarmId;	
	TAlarmId expectedAlarmId = iAlarmIds[aAlarmNo-1];
	for(;;)
	    {
		TESTL(iconsoleSession.NotifyOnSoundStart(alarmId,soundFile,status) == KErrNone);
		User::WaitForRequest(status);
		if (alarmId == expectedAlarmId)
		    {
			if (status.Int() == KErrNone)
                {
        	    _LIT(KStartSound,"Console Alert Server : Received StartPlayingSound for Alarm No %d");
        	    INFO_PRINTF2(KStartSound,aAlarmNo);
        	    break;
                }
	        else
			    {
			    _LIT(KStartSound,"Console Alert Server :Error Received in StartPlayingSoundL : Status = %d, Alarm %d");
			    ERR_PRINTF3(KStartSound,status.Int(),aAlarmNo);
			    User::Leave(status.Int());	
			    }
		    }
		}
    }

/**
This method is used on the console alarm alert server side to listen for
notificatios that the console alert server has received the StopPlayingSound
command. The method checks the alarm ID against the alarm ID that was stored 
when the alarm was added.
*/         
void CTestCITAlarmServerBaseStep::NotifyOnSoundStopL(TInt aAlarmNo)
    {
	TRequestStatus status;
	TAlarmId alarmId;	
	TAlarmId expectedAlarmId = iAlarmIds[aAlarmNo-1];
	for(;;)
	    {
		TESTL(iconsoleSession.NotifyOnSoundStop(alarmId,status) == KErrNone);
		User::WaitForRequest(status);
        if (alarmId == expectedAlarmId)
            {
            if (status.Int() == KErrNone)
                {
                _LIT(KStopSound,"Console Alert Server : Received StopPlayingSound for Alarm No %d");
        	    INFO_PRINTF2(KStopSound,aAlarmNo);
        	    break;	
                }
            else
                {
            	_LIT(KStopSound,"Console Alert Server : Error Received in StopPlayingSound : status = %d");
        	    ERR_PRINTF2(KStopSound,status.Int());
        	    User::Leave(status.Int());
                }
            }
	    }	
    }
        
/**
This method is used to request the console alarm alert server side to
snooze an alarm for a specific time period. The alarm number passed in
is converted to the alarm ID for the alarm to be snoozed.
*/
void CTestCITAlarmServerBaseStep::SnoozeAlarmL(TInt aAlarmNo, TTimeIntervalMinutes aSnoozeTime)
    {
    TTime snoozeUntill;
    snoozeUntill.UniversalTime();
    snoozeUntill += aSnoozeTime;
    
	TAlarmId alarmId = iAlarmIds[aAlarmNo-1];
	TInt err = iconsoleSession.SnoozeAlarm(alarmId,snoozeUntill);
	if (err != KErrNone)
	    {
	    _LIT(KErr,"SnoozeAlarm For Alarm No %d : Error = %d");
	    ERR_PRINTF3(KErr,aAlarmNo,err);
		User::Leave(err);
	    }
	else
	    {
		_LIT(KSnoo,"Request Snooze For Alarm No %d");
		INFO_PRINTF2(KSnoo,aAlarmNo);
	    }
    }

/**
This method is used to request the console alarm alert server side to
clear/ack a specific alarm. The alarm is expected to be in the notifying 
state for the request to have any effect. The alarm number passed in is 
converted to the alarm ID for the alarm to be acknowledged.
*/        
void CTestCITAlarmServerBaseStep::AcknowledgeAlarmL(TInt aAlarmNo)
    {
	TAlarmId alarmId = iAlarmIds[aAlarmNo-1];
	TInt err = iconsoleSession.AcknowledgeAlarm(alarmId);
	if (err != KErrNone)
	    {
	    _LIT(KErr,"AcknowledgeAlarm For Alarm %d : Error = %d");
	    ERR_PRINTF3(KErr,aAlarmNo,err);
		User::Leave(err);
	    }
	else
	    {
		_LIT(KAck,"Request Acknowledge Alarm No %d");
		INFO_PRINTF2(KAck,aAlarmNo);
	    }
    }    
 
       
/**
This method is used to request the console alarm alert server side to
clear/ack all alarms that are found to be in the notifying state.   
*/
void CTestCITAlarmServerBaseStep::AcknowledgeAllL()
    {
    TInt err = iconsoleSession.AcknowledgeAll();
    if (err != KErrNone)
        {
        _LIT(KErr,"AcknowledgeAll : Error = %d");
	    ERR_PRINTF2(KErr,err);
        User::Leave(err);	
        }
    else
        {
    	_LIT(KAck,"Request Acknowledge For All Alarms");
		INFO_PRINTF1(KAck);
        }
    }
    
