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



#ifndef __TESTUTCCHANGEFOR_H__
#define __TESTUTCCHANGEFOR_H__

#include <asshdalarm.h>
#include <asclisession.h> 
#include <f32file.h> 
#include <test/testexecutestepbase.h>
#include "TestCITAlarmServerSuiteStepBase.h"
#include "TestCITAlarmServerSuiteDefs.h"

class CTestUTCChangeFor : public CTestCITAlarmServerSuiteStepBase
	{
public:
	CTestUTCChangeFor();
	~CTestUTCChangeFor();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();
	
private:
	TBool ExecuteTestL();
	TBool WaitAndCheckSoundPlayStartL();
	TBool WaitToNotifyAfterChangeL(TBool aChangedUTC);
	TBool CheckStartAndStopL();
	TBool VerifyGetSilentUntil();
	
public:
	TBool changedUTC;
	};

_LIT(KTestUTCChangeFor,"TestUTCChangeFor");

_LIT (KCITFS34, "Executing Integration Tests Alarm Server FS 34");

#endif
