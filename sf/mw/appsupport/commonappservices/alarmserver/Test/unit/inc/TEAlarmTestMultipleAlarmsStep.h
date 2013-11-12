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

#ifndef __TEALARMTESTMULTIPLEALARMSSTEP_H__
#define __TEALARMTESTMULTIPLEALARMSSTEP_H__


#include <asshdalarm.h>

#include "TEAlarmTestMANEventEntry.h"
#include "TEAlarmTestMANEventObserver.h"
#include "TEAlarmTestStepBase.h"


_LIT(KTEAlarmTestMultipleAlarmsStep,"TestMultipleAlarmsStep");


const TInt KGranularity = 3;
const TInt KReconnectTimeout = 1;	// in seconds

class CTEAlarmTestMultipleAlarmsStep : public CTEAlarmTestStepBase, public MMANEventHandler
	{
private:
	enum TTestFailureCode {
		EUnexpectedEvent,
		EUnexpectedAlarmState
		};

	struct TAlarmEntry
		{
		TInt		iAlarmIdx;		// alarm's index from .ini file
		TASShdAlarm iASShdAlarm;	// shared alarm object itself
		TInt		iOffset;		// in seconds
		TInt		iDelay;			// in seconds
		TBool		iNotifying;
		TBool		iVisible;
		TBool		iSoundPlaying;
		};

public:
	CTEAlarmTestMultipleAlarmsStep();
	~CTEAlarmTestMultipleAlarmsStep();
	
	TVerdict doTestStepPreambleL();
	TVerdict doTestStepL();

public:
	void HandleEventL(TEventEntry aEvent);

	// Utility methods for reading from Config
	TBool GetAlarmsFromConfigL(const TDesC& aSectName, CArrayFixFlat<TAlarmEntry>& aResult);
	TBool GetEventsFromConfigL(const TDesC& aSectName, RArray<TEventEntry>& aResult);

private:
	// Event observers
	void AddObserverL(const TEventEntry& aEvent);
	void AddDelayedHandlerL(const TEventEntry& anAction);	
	void StartEventObserversL();	
	void RegisterAlarmsL();	
	void UserWaitL(TInt aSeconds);
	void ReconnectToASL();	

	// Actions and Events
	void DoProcessEventL(const TEventEntry& aEvent);
	TBool DoImmediateActionL(const TEventEntry& aEvent);
	TBool DoDelayedActionL(const TEventEntry& aEvent);
	TBool GetNextEvent(TEventEntry& aEvent);
	TBool DidWeExpectThisEvent(const TEventEntry& aEvent);

	// Logging
	const TDesC& AlarmStateToStrL(TAlarmState aState);
	void LogEventL(TEventEntry aEvent);
	void LogAlarmStateL(TInt aAlarmIdx);
	void ReportFailureAndStopL(TTestFailureCode aCode);

	// Alarms and Alarms list
	TInt GetAlarmListIdxbyAlarmId(TEventEntry& aEvent);
	inline TAlarmEntry& AlarmAt(TInt aIdx) { return (*iAlarmListPtr)[aIdx]; } 
	inline TASShdAlarm& ShdAlarmAt(TInt aIdx) { return (*iAlarmListPtr)[aIdx].iASShdAlarm; } 
	void UpdateAlarmDetails(TInt aAlarmIdx);
	TBool CheckAlarmState(const TEventEntry& aEvent);
	TBool CheckAlarmStateOnly(const TEventEntry& aEvent);

	void AddAlarm(TInt aAlarmIdx);
	void RemoveAlarms();
	
	// Private Utility methods for reading from config
	TBool GetAlarmFromConfig(const TDesC& aSectName, TInt aAlarmIdx, TAlarmEntry& aResult);
	TBool GetEventFromConfigL(const TDesC& aSectName, TInt aEventIdx, TEventEntry& aResult);
	TEventType GetEventTypeFromString(const TDesC& aEventString);
	TAlarmState GetAlarmStateFromStringL(TPtrC& aStateString);
	TBool GetVisibilityFromStringL(TPtrC& aVisibilityString);
	TBool GetBoolFromStringL(TPtrC& aBoolString);
	TBool GetSoundPlayingFromStringL(TPtrC& aPlayingString);
	
private:
	RPointerArray<CMANEventObserver> iBunchOfObservers;
	RPointerArray<CMANDelayedActionHandler> iBunchOfDelayedHandlers;
	CArrayFixFlat<TAlarmEntry>	*iAlarmListPtr;
	RArray<TEventEntry>	iEventList;
	TInt	iCurrentEventIdx;
	TBool	iInUserWaitMode;
	TTime	iStarted;
	};


#endif // __TEALARMTESTMULTIPLEALARMSSTEP_H__
