/**
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/



#ifndef __TESTCITALARMSERVERBASESTEP_H__
#define __TESTCITALARMSERVERBASESTEP_H__
#include <test/testexecutestepbase.h>
#include "consoleantestclient.h"
#include <asshdalarm.h>
#include <asclisession.h>


namespace multialarms
    {
    _LIT(KExpiry1,"Alarm1ExpiresInMinutes");
    _LIT(KExpiry2,"Alarm2ExpiresInMinutes");
    _LIT(KExpiry3,"Alarm3ExpiresInMinutes");
    _LIT(KExpiry4,"Alarm4ExpiresInMinutes");
    _LIT(KSnooze,"SnoozeTimeInMinutes");
    _LIT(KPauTime,"PaTimeInMinutes");
    _LIT(KMaxAlarms,"MaxNoOfAlarms");
    _LIT(KDefIniOffsetTag,"IniOffsetTag");
    _LIT(KDefIniPlayLength,"IniPlayLength");
    _LIT(KIniDuration,"Play Duration in Seconds");
    _LIT(KMaxOffIndex,"Offset Index To Stop Checking At");
    _LIT(KRestoreDefaults,"default");
    const TInt KMaxTagLength = 20;
    const TInt alarm1 = 1;
    const TInt alarm2 = 2;
    const TInt alarm3 = 3;
    }

/**
The class provides a base for most of the basic functions
required for adding alarms and checking what the alarm is
doing on the either the alarmserver side or the console alarm
alert server side.
*/   

class CTestCITAlarmServerBaseStep : public CTestStep
	{
public:

	~CTestCITAlarmServerBaseStep();
	CTestCITAlarmServerBaseStep();
    virtual TVerdict doTestStepPreambleL(); 
	virtual TVerdict doTestStepPostambleL();

protected:
    void ConnectToAlarmServerL();
    //AlarmServer side notifications
    void AddAlarmL(TInt aAlarmNo, TTimeIntervalMinutes aTimeInterval);
    void DeleteAllAlarmsL();
    void WaitAndCheckExpiredAlarm(TInt aAlarmNo);
    void WaitAndCheckSoundPlayStart(TInt alarmNo);
    void WaitAndCheckSoundPlayStop(TInt alarmNo);
    void CheckAlarmStatesL(TInt alarmNo, TAlarmState aExpectedState);
    void CheckIdListByStateL(TAlarmState aWantedState);
    //Console Alert Server side notifications and responses
    void NotifyOnAlarmL(TInt aAlarmNo);
    void NotifyOnSoundStartL(TInt aAlarmNo);
    void NotifyOnSoundStopL(TInt aAlarmNo);
    void SnoozeAlarmL(TInt aAlarmNo, TTimeIntervalMinutes aSnoozeTime);
    void AcknowledgeAlarmL(TInt aAlarmNo);
    void AcknowledgeAllL();
    
    RASCliSession ialarmSession;    //session with the alarm server
    RANTestClient iconsoleSession; //session with alert server
    RProcess iAlertServerProcess;   //console alert server process
    TTimeIntervalMinutes iAlarmExpiry1;             //Alarm 1 expiry time in seconds
    TTimeIntervalMinutes iAlarmExpiry2;             //Alarm 2 expiry time in seconds
    TTimeIntervalMinutes iAlarmExpiry3;             //Alarm 3 expiry time in seconds
    RArray<TAlarmId> iAlarmIds;     //list of alarm ID's 
    TInt iMaxAlarms;          
	};

#endif
