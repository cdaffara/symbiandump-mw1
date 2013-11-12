/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __UTCALARM_H_
#define __UTCALARM_H_

#include <tz.h>
#include <test/testexecutestepbase.h>
#include <asclisession.h>
#include <asshdalarm.h>

// String for the server create test step code
_LIT(KUTCAlarm, "UTCAlarm");

const TInt KTzNameLength = 64;

enum TUTCAlarmTestType
	{
	EUnnkown,
	ETimeZoneChange,
	EDSTEvent	
	};

class CUTCAlarm : public CTestStep
	{
public:
	CUTCAlarm();
	~CUTCAlarm();
		
private:
	// from CTestStep
	TVerdict doTestStepL();
	TVerdict doTestStepPreambleL();
	TVerdict doTestStepPostamble();
	
	void SetTestType();
	void InitialiseTestStepL();
	void RunTestL();
	void CheckDueTimeL(const TAlarmId aAlarmId, TASShdAlarm& aAlarm);
private:
	CActiveScheduler* iActiveScheduler;
	RTz iTzServer;
	RASCliSession iAlarmServer;

	TASShdAlarm iShdAlarm;
	
	// these data are retreived from utcalarm.ini file.	
	TUTCAlarmTestType iTestType;
	TTime iInitialTime;
	TTime iDueTime;
	TBuf8<KTzNameLength> iInitialLocation;
	TBuf8<KTzNameLength> iChangedLocation;
	TBool iIsUtcAlarm;
	};
	
#endif //__UTCALARM_H_
