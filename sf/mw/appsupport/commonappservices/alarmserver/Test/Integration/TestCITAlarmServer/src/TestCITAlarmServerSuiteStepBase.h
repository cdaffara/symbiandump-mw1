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



#ifndef __TESTCITALARMSERVER_STEP_BASE__
#define __TESTCITALARMSERVER_STEP_BASE__

#include <test/testexecutestepbase.h>
#include "TestCITAlarmServerSuiteDefs.h"

/****************************************************************************
* The reason to have a new step base is that it is very much possible
* that the all individual test steps have project related common variables 
* and members 
* and this is the place to define these common variable and members.
* 
****************************************************************************/
class CTestCITAlarmServerSuiteStepBase : public CTestStep
	{
public:
	virtual ~CTestCITAlarmServerSuiteStepBase();
	CTestCITAlarmServerSuiteStepBase();
	void ConnectToAlarmServer();
	void CloseAlarmServer();
	TTime FormatDateTime(const TDesC& aParamString);
	TBool CheckAlarmStatesL(TAlarmState aExpectedState);
	TTime CalculateUTCUntilL(TInt aInitialUTC);
	TBool CheckExpGreaterUntilL(TInt aInitialUTC);
	TBool CheckGlobalSoundState(TAlarmGlobalSoundState aGlobalSoundState);
	TTime CalculateSoundStopL(TTime iNow);
	void FormatLogMessage(const TDesC& aLogMessage, const TDesC& aDescriptor);
	TAlarmId AddAlarmL(const TTime& aTime);
	TBool CheckSilentForL(TTimeIntervalMinutes aSilentFor,TTime aNow);
	TTimeIntervalMinutes CalculateSilentForL();
	TBool CheckSilentUntilL();
	void TestClearStoreL();
	TBool WaitAndCheckExpiredAlarmL();
	TAlarmId WaitTimerExpiry();
	TBool WaitAndCheckSoundPlayStopL();
	TPtrC GetStringFromIni(TBuf<KMaxNumberOfCharac> aField);
	TBool WaitAndCheckSoundPlayStart1L(TTime aNow);
	TBool WaitAndCheckSoundPlayStop1L();
	TBool WaitToNotifyBeforeChangeL();
	void SetSystemTimeL();
	void SetInitalUtcOffsetL();
	void SetAlarmL();
	void SetSilentForL();
	void ChangeUtcOffset();
	void GetWaitToNotify();
	void SetSilentUntilL();
	void ChangeSystemTimeL();
	TBool VerifyAlarmSoundStateL(TAlarmGlobalSoundState aAlarmState, TBool aVerifySoundsSilenced);
		
public:
	RASCliSession iAlarmServerSession;
	TAlarmId iAlarmId;
	TTime iExpiryTime;
	TTime iNow;
	TTime iSilentUntil;
	TTimeIntervalMinutes iSilentFor;
	TTime iSoundStop;
	TBool iExpiryGreaterUntil;
	TTime iUTCUntil;
	TBool iUntilExpired;
	TInt iInitialUtc;
	TInt iMoveUtc;
	TBool iWaitToNotify;
	TTime iMoveSystemTime;
	TTime iSystemTime;

protected:
	};

#endif
