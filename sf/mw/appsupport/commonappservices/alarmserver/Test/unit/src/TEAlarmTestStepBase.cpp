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

#include "TEAlarmTestStepBase.h"
#include <asclisoundplay.h>
#include <asshdalarm.h>
#include "AlarmServer.hrh"


_LIT(KAlarm, "Alarm");
_LIT(KAlarmTimeout, "Timeout");
_LIT(KAlarmMessage, "Message");
_LIT(KIntervalRepeat, "Repeat");
_LIT(KIntervalCount, "IntervalCount");
_LIT(KTimerContingency, "TimerContingency");
_LIT(KMaxAlarms, "MaxAlarms");
_LIT(KDoTestStepPreambleLCAASMaxAlarmsSetToD, \
				"::doTestStepPreambleL(): ConsoleAAS.MAXALARMS SET TO %d");

CTEAlarmTestStepBase::~CTEAlarmTestStepBase()
	{

	iSession.Close();
#ifdef __WINS__
	iCAASClient.Close();
	StopConsoleAlarmAlertServer();
#endif //  __WINS__	

	}

TVerdict CTEAlarmTestStepBase::doTestStepPreambleL()
	{

#ifdef __WINS__ 
	TEST(StartConsoleAlarmAlertServerL());

	User::After(500000);
	TESTL(iCAASClient.Connect()==KErrNone);
#endif //  __WINS__ 		

	// read MaxAlarms setting from config and,
	// if found, set CAAS to Extended mode and SetMaxAlarms 
	if (GetIntFromConfig(ConfigSection(), KMaxAlarms, iMaxAlarms))
		{
		iCAASClient.SetExtendedMode();
		iCAASClient.SetMaxAlarms(iMaxAlarms);
		INFO_PRINTF2(KDoTestStepPreambleLCAASMaxAlarmsSetToD, iMaxAlarms);		
		} 

	// now can connect to Alarm Server
	TESTL(iSession.Connect()==KErrNone);
	
	return TestStepResult();
	
	}

/*
Starts the ConsoleAlarmAlertServer, which is an integral component in the
AlarmServer testing environment.

@internalComponent
*/
TBool CTEAlarmTestStepBase::StartConsoleAlarmAlertServerL()
	{
	INFO_PRINTF2(_L("AlarmTestStep[%X]: Starting ConsoleAlarmAlertServer"), this);

	const TUid KServerUid2={0x1000008D};
	const TUid KServerUid3={0x101F502A};
	const TUidType serverUid(KNullUid,KServerUid2,KServerUid3);
		
	_LIT(KConsoleAlarmAlertServerImg,"ConsoleAlarmAlertServer");
	TESTL((iAlertServerProcess.Create(KConsoleAlarmAlertServerImg,KNullDesC,serverUid))==KErrNone);

	iAlertServerProcess.Resume();	// logon OK - start the server
	
	return ETrue;
	}

/*
Stops the ConsoleAlarmAlertServer.

@internalComponent
*/
void CTEAlarmTestStepBase::StopConsoleAlarmAlertServer()
	{
	INFO_PRINTF2(_L("AlarmTestStep[%X]: Terminating ConsoleAlarmAlertServer "), this);

	iAlertServerProcess.Terminate(0);
	INFO_PRINTF2(_L("AlarmTestStep[%X]: Terminated ConsoleAlarmAlertServer"), this);
	}


/*
Gets a specified integer from config, and converts to TTimeIntervalMinutes.

@internalComponent
@return ETrue when Section/Key was found and aResult was set, EFalse otherwise.
@param aSectName The config section name.
@param aKeyName  The key's name, ie the left side of the equation 'key = value'
@param aResult The value is returned in this referenced variable
*/
TBool CTEAlarmTestStepBase::GetOffsetFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TTimeIntervalMinutes& aResult)
	{
	TInt interval;	
		
	if(!GetIntFromConfig(aSectName, aKeyName, interval)) 
		{
		return EFalse;
		}
		
	aResult = interval;
	return ETrue;	
	}

/*
Gets a specified integer from config, and converts to TTimeIntervalSeconds.

@internalComponent
@return ETrue when Section/Key was found and aResult was set, EFalse otherwise.
@param aSectName The config section name.
@param aKeyName  The key's name, ie the left side of the equation 'key = value'
@param aResult The value is returned in this referenced variable
*/
TBool CTEAlarmTestStepBase::GetOffsetFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TTimeIntervalSeconds& aResult)
	{
	TInt interval;	
		
	if(!GetIntFromConfig(aSectName, aKeyName, interval)) 
		{
		return EFalse;
		}
		
	aResult = interval * 60;
	return ETrue;	
	}

/*
Gets a specified TTime value from config, from the form '<aKeyName> = HH:MM'

@internalComponent
@return ETrue when Section/Key was found and aResult was set, EFalse otherwise.
@param aSectName The config section name.
@param aKeyName  The key's name, ie the left side of the equation 'key = value'
@param aResult The value is returned in this referenced variable
*/
TBool CTEAlarmTestStepBase::GetTimeFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TTime& aResult)
	{
	TPtrC ptrBuf;

	if (!GetStringFromConfig(aSectName, aKeyName, ptrBuf)) 
		{
		return EFalse;
		}

	TLex lex(ptrBuf);

	TInt hour;
	TInt min;

	// HH:MM

	if (lex.Val(hour) != KErrNone) 
		{
		return EFalse;
		}

	lex.Inc();
		 
	if(lex.Val(min) != KErrNone)
		{
		return EFalse;
		}
		
	TDateTime dt;
	dt.SetHour(hour);
	dt.SetMinute(min);

	aResult = dt;
	return ETrue;
	}

/*
Gets Sound Play Intervals from config, with the syntax:
IntervalCount = <TInt>
IntervalNN = <Offset from timer expiry(min)> <Duration of Sound (sec)>'

@internalComponent
@return ETrue when Section was found and aResult was set, EFalse otherwise.
@param aSectName The config section name.
@param aResult The value is returned in this referenced variable
@leave KErrNoMemory under low memory conditions.
*/
TBool CTEAlarmTestStepBase::GetIntervalsFromConfigL(const TDesC& aSectName, CArrayFixFlat<TASCliSoundPlayDefinition>& aResult)
	{
	// Get Interval info from Config
	TInt intervalCount = 0;
	if(!GetIntervalCountFromConfig(aSectName,intervalCount))
		{
		SetTestStepResult(ETestSuiteError);
		return EFalse;
		}
	INFO_PRINTF2(_L("AlarmPlayConfig: NumIntervals from Config=%d"), intervalCount);
	if (intervalCount < 0 || intervalCount > 99)
		{
		return EFalse;
		}

	TInt intervalIterator;
	for (intervalIterator = 0; intervalIterator < intervalCount; intervalIterator++)
		{
		TTimeIntervalMinutes offset;
		TTimeIntervalSeconds duration;
		if (!GetIntervalFromConfig(aSectName, intervalIterator, offset, duration))
			{
			INFO_PRINTF2(_L("AlarmPlayConfig: Failed to load Interval #%d"), intervalIterator);
			SetTestStepResult(ETestSuiteError);
			return EFalse;
			}
		TASCliSoundPlayDefinition item(offset, duration);
		aResult.AppendL(item);
		}

	return ETrue;
	}

/*
Gets Sound Play Interval from config, from the form
 'IntervalNN = <Offset from timer expiry(min)> <Duration of Sound (sec)>'

@internalComponent
@return EFalse if aIntervalNumber not [0 .. 99], or the Section/Key is not found.
@param aSectName The config section name.
@param aIntervalNumber The numbered interval to get.
@param aOffset The offset in minutes is returned in this referenced variable.
@param aDuration The duration in seconds is returned in this reference variable.
*/
TBool CTEAlarmTestStepBase::GetIntervalFromConfig(const TDesC& aSectName, const TInt aIntervalNumber, TTimeIntervalMinutes& aOffset, TTimeIntervalSeconds& aDuration)
	{
	TBuf<10> intervalNN;
	intervalNN.Format(_L("Interval%02d"),aIntervalNumber);
	TPtrC intervalLine;
	if(!GetStringFromConfig(aSectName, intervalNN, intervalLine))
		{
		return EFalse;
		}

	TInt intValue;

	TLex intervalLex(intervalLine);
	TPtrC offsetPtr = intervalLex.NextToken();
	TLex lex(offsetPtr);
	lex.Val(intValue);
	aOffset = intValue;

	TPtrC durationPtr = intervalLex.NextToken();
	lex.Assign(durationPtr);
	lex.Val(intValue);
	aDuration = intValue;

	return ETrue;
	}

/*
Gets Sound Play Interval Count from config, from the form
 'IntervalCount = <TInt>'

@internalComponent
@return ETrue when Section/Key was found and aResult was set, EFalse otherwise.
@param aSectName The config section name.
@param aDuration The interval count is returned in this reference variable.
*/
TBool CTEAlarmTestStepBase::GetIntervalCountFromConfig(const TDesC& aSectName, TInt& aResult)
	{
	return GetIntFromConfig(aSectName,KIntervalCount,aResult);
	}

/*
Appends TASShdAlarm's to the supplied array. It does not add the alarm.
Alarms are specific in their own section, and are referenced from
the testcase's section, following the following syntax:
[<TestCase Section Name>]
Alarm = 1 5

[Alarm01]
Message = This is the message for Alarm #1
Timeout = <Offset from 'UTC::now' in seconds>

[Alarm05]
Message = And this is the message for Alarm #5
Timeout = <Offset from 'UTC::now' in seconds>


@internalComponent
@return ETrue when Section/Key was found and all indexed alarms were found, EFalse otherwise.
@param aSectName The config section name.
@param aAlarmArray The alarms are appended to this referenced array.
@leave KErrNoMemory under low memory conditions.
*/
TBool CTEAlarmTestStepBase::GetAlarmsFromConfigL(const TDesC& aSectName, CArrayFixFlat<TASShdAlarm>& aAlarmArray)
	{
	TPtrC alarmLine;

	if(!GetStringFromConfig(aSectName, KAlarm, alarmLine))
		{
		return EFalse;
		}

	TLex alarmLineLex(alarmLine);
	TPtrC currentAlarmPtr = alarmLineLex.NextToken();
	while(currentAlarmPtr.Length() > 0)
		{
		TLex alarmLex(currentAlarmPtr);
		TInt alarmNum;
		alarmLex.Val(alarmNum);
		TASShdAlarm alarm;
		if(!GetAlarmFromConfig(alarmNum, alarm))
			{
			return EFalse;
			}
		aAlarmArray.AppendL(alarm);
		currentAlarmPtr.Set(alarmLineLex.NextToken());
		}

	return ETrue;
	}

/*
Gets Alarm info from config, and set's the alarms internal data. It does not add the alarm.

@internalComponent
@return ETrue when Section/Keys were found and aAlarm was set, EFalse otherwise.
@param aSectName Used to build the config section name 'AlarmNN'. It must be [0..99].
@param aAlarm The interval count is returned in this reference variable. The next due time
      is set in UTC time.
@see CTEAlarmTestStepBase::GetAlarmsFromConfigL
*/
TBool CTEAlarmTestStepBase::GetAlarmFromConfig(const TInt aAlarmNumber, TASShdAlarm& aAlarm)
	{
	if (aAlarmNumber < 0 || aAlarmNumber > 99)
		{
		return EFalse;
		}

	// Compose section heading
	TBuf<7> alarmNN;
	alarmNN.Format(_L("Alarm%02d"), aAlarmNumber);

	// Get the Message
	TPtrC message;
	if (!GetStringFromConfig(alarmNN, KAlarmMessage, message))
		{
		return EFalse;
		}

	// Get the Timeout
	TInt interval;
	if (!GetIntFromConfig(alarmNN, KAlarmTimeout, interval))
		{
		return EFalse;
		}
	aAlarm.Message() = message;

	TTime uni;
	uni.UniversalTime();
	uni += TTimeIntervalSeconds(interval);
	aAlarm.SetUtcNextDueTime(uni);
	return ETrue;
	}

/*
Gets Sound Play Repeat Setting from config, from the form
 'Repeat = Loop', 'Repeat = RepeatLast', or 'Repeat = Stop'

@internalComponent
@return ETrue when Section/Key was found and aResult was set, EFalse otherwise.
@param aSectName The config section name.
@param aResult The repeat setting is returned in this reference variable.
*/
TBool CTEAlarmTestStepBase::GetIntervalRepeatFromConfig(const TDesC& aSectName, TAlarmSoundRepeatSetting& aResult)
	{
	TPtrC setting;
	if(!GetStringFromConfig(aSectName, KIntervalRepeat, setting))
		return EFalse;
	if ( setting == _L("Loop") )
		{
		aResult = EAlarmSoundRepeatSettingLoop;
		}
	else if ( setting == _L("RepeatLast") )
		{
		aResult = EAlarmSoundRepeatSettingRepeatLast;
		}
	else if ( setting == _L("Stop") )
		{
		aResult = EAlarmSoundRepeatSettingStop;
		}
	else
		{
		return EFalse;
		}
	INFO_PRINTF2(_L("GetIntervalRepeatFromConfig: RepeatSetting = %d"), aResult);
	return ETrue;
	}

/*
Gets Sound Play Contingency from config, from the form
 'TimerContingency = <TInt>'

Suggested contingency is 3 seconds, as this allows for some delay as the
ConsoleAlarmAlertServer has to forward the events to the test steps, but
still bounds sound events sufficiently to distinguish between occurences.

@internalComponent
@return ETrue when Section/Key was found and aResult was set, EFalse otherwise.
@param aSectName The config section name.
@param aResult The contingency is returned in this reference variable.
*/
TBool CTEAlarmTestStepBase::GetTimerContingencyFromConfig(const TDesC& aSectName, TTimeIntervalSeconds& aResult)
	{
	TInt contingency;
	if(!GetIntFromConfig(aSectName, KTimerContingency, contingency))
		return EFalse;
	aResult = contingency;
	INFO_PRINTF2(_L("GetTimerContingencyFromConfig: Contingency = %d"), contingency);

	return ETrue;
	}

/*
Compares the two supplied arrays of Sound Play Definitions (Sound Intervals).

@internalComponent
@return aFirst==aSecond, including count and ordered entries.
@param aFirst The first set of Sound Intervals
@param aSecond The second set of Sound Intervals
*/
TBool CTEAlarmTestStepBase::CompareAlarmPlayIntervals(CArrayFixFlat<TASCliSoundPlayDefinition>& aFirst, CArrayFixFlat<TASCliSoundPlayDefinition>& aSecond)
	{
	if ( aFirst.Count() != aSecond.Count() )
		{
		INFO_PRINTF3(_L("SoundPlay array sizes differ: %d != %d"), aFirst.Count(), aSecond.Count());
		return EFalse;
		}

	TInt count = aFirst.Count();
	for(TInt i=0; i < count; i++)
		{
		if ( (aFirst.At(i).Offset().Int() != aSecond.At(i).Offset().Int()) ||
		     (aFirst.At(i).Duration().Int() != aSecond.At(i).Duration().Int()) )
			{
			INFO_PRINTF6(_L("SoundPlay Element[%d] differ, (%d,%d) != (%d,%d)"),
					count, aSecond.At(i).Offset().Int(), aSecond.At(i).Duration().Int(),
					aFirst.At(i).Offset().Int(), aFirst.At(i).Duration().Int());
			return EFalse;
			}
		}
	return ETrue;
	}
