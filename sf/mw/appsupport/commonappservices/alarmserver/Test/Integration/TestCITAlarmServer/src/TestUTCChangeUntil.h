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



#include <asshdalarm.h>
#include <asclisession.h> 
#include <f32file.h> 
 
#ifndef __TESTUTCCHANGEUNTIL_H__
#define __TESTUTCCHANGEUNTIL_H__
#include <test/testexecutestepbase.h>
#include "TestCITAlarmServerSuiteStepBase.h"
#include "TestCITAlarmServerSuiteDefs.h"

/** CTestUTCChangeUntil */
enum TAlarmSoundStatus
		{
		EAlarmSilenedFuture = -1,
		EAlarmExpired = 0,
		EAlarmWaitStartStop = 1,
		};
		
class CTestUTCChangeUntil : public CTestCITAlarmServerSuiteStepBase
	{
public:
	CTestUTCChangeUntil();
	~CTestUTCChangeUntil();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();
	
private:
	TBool ExecuteTestL();
	TBool WaitAndCheckSoundPlayStartL();
	TBool CheckStartAndStopL();
	TBool ExecuteNonNotifyingAlarmL(TInt aMoveUTC);
	TBool CheckAlarmStateStartStopOffOnL();
	TBool CheckAlarmGlobalSoundL();
	TBool CheckAlarmStateSoundStateUntilExpiredL();

private:	
	};

_LIT(KTestUTCChangeUntil,"TestUTCChangeUntil");

#endif
