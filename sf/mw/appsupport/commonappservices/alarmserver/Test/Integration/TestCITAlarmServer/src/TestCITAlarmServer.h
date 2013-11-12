/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#include <asshdalarm.h>
#include <asclisession.h> 

#include <f32file.h> 
 
#ifndef TESTCITALARMSERVER_H
#define TESTCITALARMSERVER_H

#include <test/testexecutestepbase.h>
#include "TestCITAlarmServerSuiteStepBase.h"
#include "TestCITAlarmServerSuiteDefs.h"

/** literals */
_LIT(KTypeOfAlarm,	"typeofalarm");

/** constants */
const TInt secs = 119;
const TInt testnumber = 2;

/** CTestCITAlarmServer */
enum TTestNumber
		{
		ETest1 = 1,
		ETest2 = 2,
		ETest3 = 3,
		ETest4 = 4,
		ETest5 = 5,
		ETest6 = 6,
		ETest7 = 7,
		ETest8 = 8,
		ETest9 = 9,
		ETest10 = 10,
		ETest11 = 11,
		ETest12 = 12,
		ETest13 = 13,
		ETest14 = 14,
		};
		
class CTestCITAlarmServer : public CTestCITAlarmServerSuiteStepBase
	{
public:
	CTestCITAlarmServer();
	~CTestCITAlarmServer();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();	
	
private:
	TBool ExecuteTestL();
	void SetUTCOffSetL();
	void ChangeUTCOffSetL();
	void SetSystemTimeL(TBool aNewSystemTime);
	void AddAlarmToServerL();
	void CleanUpAlarmServer();
	void SetAlarmQuietPeriodL();
	TBool WaitAndCheckSoundPlayStartL();
	void SetAlarmSilentPeriodForL();
	void InfoForAlarmCount();
	void GetAlarmCharacL();
	TBool CheckForAlarmsInQueue();
	TBool WaitAndCheckExpiryL();
	TBool CheckIfAlarmDeleted();
	TBool CheckForAlarmDefintion();
	TAlarmId AlarmAddL(const TTime& aTime);
	TBool CheckNextExpiryTimeL();
	
private:
	TBuf<KMaxNumberOfCharac> iField;
	TInt iTestnumber;
	TPtrC iTypeOfAlarm;
	};

_LIT(KTestCITAlarmServer,"TestCITAlarmServer");

_LIT (KCITFS32, "Executing Integration Alarm Server Test (App-Services - FS 3.2)");

#endif
