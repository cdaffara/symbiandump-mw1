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
 
#ifndef __TESTSYSTEMCHANGEUNTIL_H__
#define __TESTSYSTEMCHANGEUNTIL_H__
#include <test/testexecutestepbase.h>
#include "TestCITAlarmServerSuiteStepBase.h"
#include "TestCITAlarmServerSuiteDefs.h"

class CTestSystemChangeUntil : public CTestCITAlarmServerSuiteStepBase
	{
public:
	CTestSystemChangeUntil();
	~CTestSystemChangeUntil();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();
	
private:
	TBool ExecuteTestL();
	TBool WaitAndCheckSoundPlayStartL(TBool aSystemGreaterExpiry);
	TBool WaitToNotifyAfterChangeL();
	TBool CheckStartStopTimingsL();
	
private:	
	TBool iNewSysGreaterExp;
	};

_LIT(KTestSystemChangeUntil,"TestSystemChangeUntil");

_LIT(KCITFS34FINISH, "Finished Executing Integration Tests Alarm Server FS 34");

#endif
