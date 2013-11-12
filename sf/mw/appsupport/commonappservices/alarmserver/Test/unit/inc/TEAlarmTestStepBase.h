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

#ifndef __TEALARMTESTSTEPBASE_H__
#define __TEALARMTESTSTEPBASE_H__

#include <test/testexecutestepbase.h>
#include <asclisession.h>
#include "consoleantestclient.h"
#include <asclisoundplay.h>
#include <asshdalarm.h>
#include "AlarmServer.hrh"


class CTEAlarmTestStepBase : public CTestStep
	{
public:
	// Destructor
	~CTEAlarmTestStepBase();

	// CTestStep virtual functions
	virtual TVerdict doTestStepPreambleL();

	// Unility methods for reading parameters from config.
	TBool GetTimeFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TTime& aResult);
	TBool GetOffsetFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TTimeIntervalMinutes& aResult);
	TBool GetOffsetFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TTimeIntervalSeconds& aResult);
	TBool GetIntervalsFromConfigL(const TDesC& aSectName, CArrayFixFlat<TASCliSoundPlayDefinition>& aResult);
	TBool GetAlarmsFromConfigL(const TDesC& aSectName, CArrayFixFlat<TASShdAlarm>& aAlarmArray);
	TBool GetAlarmFromConfig( TInt aAlarmNumber, TASShdAlarm& aAlarm);
	TBool GetIntervalRepeatFromConfig(const TDesC& aSectName, TAlarmSoundRepeatSetting& aResult);
	TBool GetTimerContingencyFromConfig(const TDesC& aSectName, TTimeIntervalSeconds& aResult);
protected:
	TBool CompareAlarmPlayIntervals(CArrayFixFlat<TASCliSoundPlayDefinition>& origArray, CArrayFixFlat<TASCliSoundPlayDefinition>& otherArray);
protected:
	// Methods to control Console Alarm Alert Server
	TBool StartConsoleAlarmAlertServerL();
	void StopConsoleAlarmAlertServer();
private:
	TBool GetIntervalFromConfig(const TDesC& aSectName, const TInt aIntervalNumber, TTimeIntervalMinutes& aOffset, TTimeIntervalSeconds& aDuration);
	TBool GetIntervalCountFromConfig(const TDesC& aSectName, TInt& aResult);
protected:
	RASCliSession iSession;			// Alarm Server
	RProcess iAlertServerProcess;	// ConsoleAlarmAlertServer
	RANTestClient iCAASClient;		//
	TInt iMaxAlarms;				//
	};

#endif // __TEALARMTESTSTEPBASE_H__
