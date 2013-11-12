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

#include "TEAlarmTestMultipleAlarmsStep.h"
#include "TEAlarmTestMANEventObserver.h"


// Config Parameter names
_LIT(KAlarmNN,"Alarm%02d");
_LIT(KEventNN,"Event%02d");
_LIT(KActionNN,"Action%02d");

// Config subparameter values
_LIT(KStateInPreparation,	"InPreparation");
_LIT(KStateQueued,			"Queued");
_LIT(KStateNotifying,		"Notifying");
_LIT(KStateWaitingToNotify,	"WaitingToNotify");
_LIT(KStateSnoozed,			"Snoozed");
_LIT(KStateNotified,		"Notified");
_LIT(KVisible,				"Visible");
_LIT(KNotVisible,			"NotVisible");
_LIT(KSoundOn,				"Playing");
_LIT(KSoundOff,				"NotPlaying");

_LIT(KTimeFormatPattern, "%H:%T:%S.%*C3");
_LIT(KZeroTime,"20050601:130000.000000");

// Message strings
_LIT(KExpiresInDSeconds, " expires in %d seconds");
_LIT(KExpiresNow, " expires NOW");
_LIT(KObserverDying, "Observer is dying (%S)");
_LIT(KErrorDS, "ERROR (%d)     %S");
_LIT(KErrorUnexpectedEvent, "ERROR Unexpected event %d");
_LIT(KLitSS, "%S: %S");
_LIT(KLitSSIgnoringEvent, "%S: %S       Ignoring the event because of UserWait mode.");
_LIT(KLitSSExpectedS, "%S: %S       Expected %S");
_LIT(KLitAlarmStateDSSS, "A%02d   state=%S   visible=%S   sound playing=%S");

_LIT(KAlarmStateCheckPassedOK, "Alarm State check PASSED OK.");
_LIT(KAlarmStateOnlyCheckPassedOK, "Alarm StateOnly check PASSED OK.");
_LIT(KDoDelayedActionReconnectToCAASSuccess, \
				"::DoDelayedActionL(): Reconnected to ConsoleAAS successfully");		
_LIT(KDoDelayedActionReconnectToASSuccess, \
				"::DoDelayedActionL(): Reconnected to AlarmServer successfully");
_LIT(KDoDelayedActionCAASMaxAlarmsSetToD, \
				"::DoDelayedActionL(): ConsoleAAS.MAXALARMS SET TO %d");

_LIT(KUnexpectedAlarmState, "TEST FAILED: State of an Alarm is not as expected");
_LIT(KUnexpectedEvent, "TEST FAILED: SEQUENCE OF EVENTS IS NOT AS EXPECTED");
_LIT(KUnknownFailure, "TEST FAILED: Don't know what's actually happened");
_LIT(KAlarmStateSummaryStart, "* * * ALARM STATE SUMMARY * * *");
_LIT(KAlarmStateSummaryEnd, "* * * SUMMARY ENDS        * * *");
				
_LIT(KLoadedDAlarms, "GetAlarmsFromConfigL: Loaded %d Alarms");
_LIT(KLoadedDEvents, "GetEventsFromConfigL: Loaded %d Events");
				
// //
//
// CTestStep inherited methods
//
// //

CTEAlarmTestMultipleAlarmsStep::CTEAlarmTestMultipleAlarmsStep()
	{
	SetTestStepName(KTEAlarmTestMultipleAlarmsStep);
	iInUserWaitMode = EFalse;
	iStarted = Time::NullTTime();
	}


CTEAlarmTestMultipleAlarmsStep::~CTEAlarmTestMultipleAlarmsStep()
	{
	iCAASClient.CancelNotifications();
	iBunchOfObservers.ResetAndDestroy();	// RPointerArray<CMANEventObserver>
	iBunchOfDelayedHandlers.ResetAndDestroy(); // RPointerArray<CMANDelayedActionHandler>

	delete iAlarmListPtr;					// CArrayFixFlat<TAlarmEntry>*
	
	iEventList.Close();						// RArray<TEventEntry>
	
	CActiveScheduler* sched = CActiveScheduler::Current();
	delete sched;
	CActiveScheduler::Install(NULL);
	}


TVerdict CTEAlarmTestMultipleAlarmsStep::doTestStepPreambleL()
	{
	// Need to call base class doTestPreambleL() to start AS and CAAS
	TESTL(CTEAlarmTestStepBase::doTestStepPreambleL()==EPass);
	
	// Alarm list
	iAlarmListPtr = new (ELeave) CArrayFixFlat<TAlarmEntry>(KGranularity);

	// Active Scheduler
	CActiveScheduler* sched = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	
	// Read config from ini file	
	TESTL(GetAlarmsFromConfigL(ConfigSection(), *iAlarmListPtr));
	TESTL(GetEventsFromConfigL(ConfigSection(), iEventList));
	
	// Fine
	return TestStepResult();
	}

TVerdict CTEAlarmTestMultipleAlarmsStep::doTestStepL()
	{
	StartEventObserversL();
	User::After(500000);
	
	iStarted.UniversalTime();

	RegisterAlarmsL();

	// Process Sequence of Events
	CActiveScheduler::Start();

	// Finish
	return TestStepResult();			
	}
	
	 

// //
//
// CTEAlarmTestMultipleAlarmsStep own methods HANDLE EVENTS and DO ACTIONS
//
// //

void CTEAlarmTestMultipleAlarmsStep::HandleEventL(TEventEntry aCurrentEvent)
	{
	GetAlarmListIdxbyAlarmId(aCurrentEvent);
	
	LogEventL(aCurrentEvent);
	
	if (aCurrentEvent.iError!=KErrNone)
		{
		return;
		}

	if (aCurrentEvent.IsAction())
		{
		DoDelayedActionL(aCurrentEvent);
		if (iInUserWaitMode)
			{
			return;
			}
		}

	if (aCurrentEvent.IsEvent())
		{
		// an Event
		DoProcessEventL(aCurrentEvent);
		if (iInUserWaitMode)
			{
			return;
			}
		if (!DidWeExpectThisEvent(aCurrentEvent))
			{
			// Test Failed: Unexpected Event
			ReportFailureAndStopL(EUnexpectedEvent);
			return;
			}
		}

	// check what's next on the EventList
	TEventEntry nextEvent = aCurrentEvent;

	if (aCurrentEvent.IsEvent())
		{
		// if we've just processed an event, then get next event from EventList
		if (!GetNextEvent(nextEvent))
			{
			// if it's the last item on the list
			return;
			}
		}
	else
		{
		// if we've just did an action, then have
		// to deal with current item on the EventList
		nextEvent = iEventList[iCurrentEventIdx];
		}
		
	// as long as next item is action, do it immediately
	// if next item is an event, then break and wait for next event
	do	{
		if (nextEvent.IsEvent())  
			{
			// go wait for next event
			break;
			}
		LogEventL(nextEvent);
		if (!DoImmediateActionL(nextEvent))
			{
			// needs asynchronous handling (like UserWait or failure)
			if (iCurrentEventIdx < iEventList.Count()-1 )
				{
				GetNextEvent(nextEvent);
				}
			return; 
			}
	} while (GetNextEvent(nextEvent));

	}


TBool CTEAlarmTestMultipleAlarmsStep::GetNextEvent(TEventEntry& aEvent)
	{
	if ( iCurrentEventIdx >= iEventList.Count()-1 )
		{
		CActiveScheduler::Stop();
		return EFalse;
		}	
	aEvent = iEventList[++iCurrentEventIdx];
	return ETrue;
	}


TBool CTEAlarmTestMultipleAlarmsStep::DidWeExpectThisEvent(const TEventEntry& aEvent)
	{
	return (iEventList[iCurrentEventIdx]==aEvent);
	}


void CTEAlarmTestMultipleAlarmsStep::DoProcessEventL(const TEventEntry& aEvent)
	{
	switch (aEvent.iType)
		{
	case EEventSetAlarm:
		AlarmAt(aEvent.iAlarmIdx).iNotifying = ETrue;
		break;
	case EEventVisible:
		AlarmAt(aEvent.iAlarmIdx).iVisible = aEvent.iVisible;
		break;
	case EEventSoundStart:
		AlarmAt(aEvent.iAlarmIdx).iSoundPlaying = ETrue;
		break;
	case EEventSoundStop:
		AlarmAt(aEvent.iAlarmIdx).iSoundPlaying = EFalse;
		break;
	case EEventDelete:
		if (aEvent.iAlarmIdx != KBogusAlarmIdx)
			{
			AlarmAt(aEvent.iAlarmIdx).iNotifying = EFalse;
			}
		else // this is a DeleteAll command
			{
			for (TInt i = iAlarmListPtr->Count()-1; i >= 0; i--)
				{
				AlarmAt(i).iNotifying = EFalse;
				}
			}
		break;
				
	case EEventSetState:
		// Do nothing
		break;
	default:	
		ERR_PRINTF2(KErrorUnexpectedEvent, aEvent.iType);
		User::Leave(KErrArgument);
		break;
		}
	}


TBool CTEAlarmTestMultipleAlarmsStep::DoImmediateActionL(const TEventEntry& aEvent)
	{

	switch (aEvent.iType)
		{
		case EActionCheckState:
			UpdateAlarmDetails(aEvent.iAlarmIdx);
			LogAlarmStateL(aEvent.iAlarmIdx);
			if ( !CheckAlarmState(aEvent) )
				{
				// Test Failed: Alarm is in an unexpected state
				ReportFailureAndStopL(EUnexpectedAlarmState);
				return EFalse;
				}
			else
				{
				INFO_PRINTF1(KAlarmStateCheckPassedOK);
				}
			break;

		case EActionCheckStateOnly:
			UpdateAlarmDetails(aEvent.iAlarmIdx);
			LogAlarmStateL(aEvent.iAlarmIdx);
			if ( !CheckAlarmStateOnly(aEvent) )
				{
				// Test Failed: Alarm is in an unexpected state
				ReportFailureAndStopL(EUnexpectedAlarmState);
				return EFalse;
				}
			else
				{
				INFO_PRINTF1(KAlarmStateOnlyCheckPassedOK);
				}
			break;

		case EActionSnooze:
			{
			TASShdAlarm& shdAlarm = ShdAlarmAt(aEvent.iAlarmIdx);				
			TTime snoozeTime;
			snoozeTime.UniversalTime();
			snoozeTime += TTimeIntervalSeconds(aEvent.iPeriod);
			iCAASClient.SnoozeAlarm(shdAlarm.Id(), snoozeTime);
			}
			break;

		case EActionPauseSound:
			{
			TASShdAlarm& shdAlarm = ShdAlarmAt(aEvent.iAlarmIdx);				
			TTime pauseTime;
			pauseTime.UniversalTime();
			pauseTime += TTimeIntervalSeconds(aEvent.iPeriod);
			iCAASClient.PauseSoundForAlarm(shdAlarm.Id(), pauseTime);
			}
			break;

		case EActionSilence:
			{
			TAlarmId alarmId = ShdAlarmAt(aEvent.iAlarmIdx).Id();
			iCAASClient.SilenceAlarm(alarmId);
			}
			break;

		case EActionClear:
			if (aEvent.iAlarmIdx==KAllAlarms)
				{
				iCAASClient.AcknowledgeAll();
				}
			else 
				{
				TAlarmId alarmId = ShdAlarmAt(aEvent.iAlarmIdx).Id();
				iCAASClient.AcknowledgeAlarm(alarmId);
				}
			break;

		case EActionUserWait:
			if (!iInUserWaitMode)
				{
				UserWaitL(aEvent.iPeriod);
				return EFalse;
				}
			break;

		case EActionReconnectToAS:
			if (!iInUserWaitMode)
				{
				ReconnectToASL();
				return EFalse;
				}
			break;

		case EActionAddAlarm:
			AddAlarm(aEvent.iAlarmIdx);
			break;

		case EActionDeleteAlarm:
			{
			TAlarmId alarmId = ShdAlarmAt(aEvent.iAlarmIdx).Id();
			iSession.AlarmDelete(alarmId);
			}
			break;
	
		case EException:	// ...fall through
		case EUnknown:		// ...fall through
		default:
				{
				ReportFailureAndStopL(EUnexpectedAlarmState);
				return EFalse;
				}
		}
	return ETrue;
	}


TBool CTEAlarmTestMultipleAlarmsStep::DoDelayedActionL(const TEventEntry& aEvent)
	{
	switch (aEvent.iType)
		{
		case EActionUserWait:
					// UserWait period completed
					if (iInUserWaitMode)
						{
						iInUserWaitMode = EFalse;
						}
					else
						{
						TESTL(EFalse);
						}
					break;

		case EActionReconnectToAS:
					// time to reconnect to Alarm Server
					if (iInUserWaitMode)
						{
						iInUserWaitMode = EFalse;
						// connect to Alarm Server
						TESTL(iSession.Connect()==KErrNone);
						
						// connect to ConsoleAlarmAlertServer
						TESTL(iCAASClient.Connect()==KErrNone);
						INFO_PRINTF1(KDoDelayedActionReconnectToCAASSuccess);
						if (iMaxAlarms>0)
							{
							iCAASClient.SetExtendedMode();
							iCAASClient.SetMaxAlarms(iMaxAlarms);
							INFO_PRINTF2(KDoDelayedActionCAASMaxAlarmsSetToD, iMaxAlarms);		
							} 
						// connect to Alarm Server
						TESTL(iSession.Connect()==KErrNone);
						INFO_PRINTF1(KDoDelayedActionReconnectToASSuccess);
						// restore observers
						StartEventObserversL();
						}
					else
						{
						TESTL(EFalse);
						}
					break;

		case EActionAddAlarm:
					AddAlarm(aEvent.iAlarmIdx);
					return EFalse;
					
		case EActionCheckState:		// ...fall through
		case EActionCheckStateOnly:	// ...fall through
		case EActionSnooze:			// ...fall through
		case EActionClear:			// ...fall through
		case EActionPauseSound:		// ...fall through
		case EActionSilence:		// ...fall through
		case EActionDeleteAlarm:	// ...fall through
		case EException:			// ...fall through
		case EUnknown:				// ...fall through
		default:	
					TESTL(EFalse);
		}
	return ETrue;
	}



// //
//
// CTEAlarmTestMultipleAlarmsStep -> ALARMS and OBSERVERS
//
// //


TInt CTEAlarmTestMultipleAlarmsStep::GetAlarmListIdxbyAlarmId(TEventEntry& aEvent)
	{
	if (aEvent.iAlarmId==KNullAlarmId) 
		{
		return KBogusAlarmIdx;
		}
	
	TInt nofAlarms = iAlarmListPtr->Count();
	for (TInt idx=0; idx<nofAlarms; idx++)
		{
		if ( ShdAlarmAt(idx).Id() == aEvent.iAlarmId )
			{
			return aEvent.iAlarmIdx = idx;
			}
		}
		
	return KBogusAlarmIdx;
	}


void CTEAlarmTestMultipleAlarmsStep::AddObserverL(const TEventEntry& aEvent)
	{
	CMANEventObserver* pObserver = CMANEventObserver::NewL(aEvent, iCAASClient, *this);
	CleanupStack::PushL(pObserver);
	iBunchOfObservers.AppendL(pObserver);
	CleanupStack::Pop();
	}


void CTEAlarmTestMultipleAlarmsStep::AddDelayedHandlerL(const TEventEntry& anAction)
	{
	CMANDelayedActionHandler* pHandler = 
					CMANDelayedActionHandler::NewL(anAction, *this);
	CleanupStack::PushL(pHandler);
	iBunchOfDelayedHandlers.AppendL(pHandler);
	CleanupStack::Pop();
	}


void CTEAlarmTestMultipleAlarmsStep::StartEventObserversL()
	{
	for (TEventType eventType=EEventSetAlarm; 
				eventType<=EEventDelete; 
					eventType=TEventType(eventType+1))
		{
		TEventEntry event(eventType);
		AddObserverL(event);
		}
	}
	
	
void CTEAlarmTestMultipleAlarmsStep::RegisterAlarmsL()
	{
	TEventEntry	event(EActionAddAlarm);
	TInt nofAlarms = iAlarmListPtr->Count();
	for (TInt idx=0; idx<nofAlarms; idx++)
		{
		event.iAlarmIdx = idx;
		event.iPeriod = AlarmAt(idx).iDelay;
		AddDelayedHandlerL(event);
		}
	}
	

void CTEAlarmTestMultipleAlarmsStep::UserWaitL(TInt aSeconds)
	{
	if (aSeconds > 0) 
		{
		TEventEntry	event(EActionUserWait);
		event.iPeriod = aSeconds;
		AddDelayedHandlerL(event);	
		iInUserWaitMode = ETrue;
		}
	}
	
	
void CTEAlarmTestMultipleAlarmsStep::ReconnectToASL()
	{
	if (KReconnectTimeout > 0) 
		{
		// remove observers and disconnect from ConsoleAlarmAlertServer
		iCAASClient.CancelNotifications();
		iBunchOfObservers.ResetAndDestroy();	// RPointerArray<CMANEventObserver>		
		iCAASClient.Close();

		// disconnect from Alarm Server
		iSession.Close();
		
		// set delayed action handler for reconnect after a timeout
		TEventEntry	event(EActionReconnectToAS);
		event.iPeriod = KReconnectTimeout;
		AddDelayedHandlerL(event);	
		iInUserWaitMode = ETrue;
		}
	}	
	
	
void CTEAlarmTestMultipleAlarmsStep::AddAlarm(TInt aAlarmIdx)
	{
	TAlarmEntry& alarmEntry = AlarmAt(aAlarmIdx);	
	TASShdAlarm& shdAlarm = alarmEntry.iASShdAlarm;
	
	alarmEntry.iNotifying = EFalse;
	alarmEntry.iVisible = EFalse;
	alarmEntry.iSoundPlaying = EFalse;

	// assumed that this method is called after the delay
	TInt timeShift = alarmEntry.iOffset - alarmEntry.iDelay;

	TTime now, then;
	now.UniversalTime();
	then = now + TTimeIntervalSeconds(timeShift);
	shdAlarm.SetUtcNextDueTime(then);
	shdAlarm.RepeatDefinition() = EAlarmRepeatDefintionRepeatOnce;

	iSession.AlarmAdd(shdAlarm);
	}


void CTEAlarmTestMultipleAlarmsStep::RemoveAlarms()
	{
	TInt nofAlarms = iAlarmListPtr->Count();
	for (TInt idx=0; idx<nofAlarms; idx++)	
		{
		TAlarmId alarmId = ShdAlarmAt(idx).Id();
		if (alarmId!=KNullAlarmId)
			{
			iSession.AlarmDelete(alarmId);
			}
		}
	}


void CTEAlarmTestMultipleAlarmsStep::UpdateAlarmDetails(TInt aAlarmIdx)
	{
	TASShdAlarm& shdAlarm = ShdAlarmAt(aAlarmIdx);
	iSession.GetAlarmDetails(shdAlarm.Id(), shdAlarm);
	}


TBool CTEAlarmTestMultipleAlarmsStep::CheckAlarmState(const TEventEntry& aEvent)
	{
	TAlarmEntry& alarmEntry = AlarmAt(aEvent.iAlarmIdx);

	if ( alarmEntry.iASShdAlarm.State()!=aEvent.iAlarmState ||
					alarmEntry.iVisible!=aEvent.iVisible ||
						alarmEntry.iSoundPlaying!=aEvent.iPlaying )
		{
		return EFalse;
		}
	
	return ETrue;	
	}
	
	
TBool CTEAlarmTestMultipleAlarmsStep::CheckAlarmStateOnly(const TEventEntry& aEvent)
	{
	TAlarmEntry& alarmEntry = AlarmAt(aEvent.iAlarmIdx);

	if ( alarmEntry.iASShdAlarm.State()!=aEvent.iAlarmState )
		{
		return EFalse;
		}
	
	return ETrue;	
	}


// //
//
// CTEAlarmTestMultipleAlarmsStep -> EVENT LOGGING and stuff
//
// //

const TDesC& CTEAlarmTestMultipleAlarmsStep::AlarmStateToStrL(TAlarmState aState)
	{
	switch (aState)
		{
		case EAlarmStateInPreparation:
				return KStateInPreparation;
		case EAlarmStateQueued:
				return KStateQueued;	
		case EAlarmStateSnoozed:
				return KStateSnoozed;
		case EAlarmStateWaitingToNotify:
				return KStateWaitingToNotify;
		case EAlarmStateNotifying:
				return KStateNotifying;
		case EAlarmStateNotified:
		  		return KStateNotified;
		default: // only those alarm states above from ASShdDefs.h are valid
				TESTL(EFalse);
		}
	return KStateInPreparation;
	}


void CTEAlarmTestMultipleAlarmsStep::LogEventL(TEventEntry aEvent)
	{
	// calc relative time since start
	TTime zeroTime(KZeroTime);
	
	TTime relTime;
	TBuf<24> strRelTime;
	relTime.UniversalTime();
	relTime -= TTimeIntervalMicroSeconds(iStarted.Int64());
	relTime += TTimeIntervalMicroSeconds(zeroTime.Int64());
	relTime.FormatL(strRelTime, KTimeFormatPattern);
	
	TBuf<64> strThisEvent, strExpectedEvent;
	aEvent.ToStr(strThisEvent);
	
	if (aEvent.iType==EActionAddAlarm)
		{
		TAlarmEntry& alarmEntry = AlarmAt(aEvent.iAlarmIdx);
		TInt timeShift = alarmEntry.iOffset - alarmEntry.iDelay;
		if (timeShift!=0)
			{
			strThisEvent.AppendFormat(KExpiresInDSeconds, timeShift);
			}
		else
			{
			strThisEvent.Append(KExpiresNow);
			}
		}
	
	if (aEvent.iError!=KErrNone)
		{
		if (aEvent.iError==KErrDied)
			{
			INFO_PRINTF2(KObserverDying, &strThisEvent);
			}
		else
			{
			ERR_PRINTF3(KErrorDS, aEvent.iError, &strThisEvent);
			}
		return;	
		}

	if (!aEvent.IsEvent())
		{
		INFO_PRINTF3(KLitSS, &strRelTime, &strThisEvent);
		} 
	else if (iInUserWaitMode)
		{
		INFO_PRINTF3(KLitSSIgnoringEvent, &strRelTime, &strThisEvent);
		}
	else 
		{
		iEventList[iCurrentEventIdx].ToStr(strExpectedEvent);
		INFO_PRINTF4(KLitSSExpectedS, &strRelTime, 
				&strThisEvent, &strExpectedEvent);
		} 
	}

	
void CTEAlarmTestMultipleAlarmsStep::LogAlarmStateL(TInt aAlarmIdx)
	{
	TAlarmEntry& alarmEntry = AlarmAt(aAlarmIdx);

	INFO_PRINTF5(KLitAlarmStateDSSS,
			aAlarmIdx, &AlarmStateToStrL(alarmEntry.iASShdAlarm.State()), 
			&BoolToStr(alarmEntry.iVisible), &BoolToStr(alarmEntry.iSoundPlaying));
	}


void CTEAlarmTestMultipleAlarmsStep::ReportFailureAndStopL(TTestFailureCode aCode)
	{
	switch(aCode)
		{
		case EUnexpectedAlarmState:
				ERR_PRINTF1(KUnexpectedAlarmState);
				break;
		case EUnexpectedEvent:
				ERR_PRINTF1(KUnexpectedEvent);
				break;
		default: 
				ERR_PRINTF1(KUnknownFailure);
				break;
		}

	// Log state of alarms	
	INFO_PRINTF1(KAlarmStateSummaryStart);
	TInt nofAlarms = iAlarmListPtr->Count();
	for (TInt idx=0; idx<nofAlarms; idx++)
		{
		UpdateAlarmDetails(idx);
		LogAlarmStateL(idx);
		}
	INFO_PRINTF1(KAlarmStateSummaryEnd);

	// Set test result to FAIL and stop scheduler	
	SetTestStepResult(EFail);
	CActiveScheduler::Stop();
	}


// //
//
// CTEAlarmTestMultipleAlarmsStep Utility methods for reading from config
//
// //

TBool CTEAlarmTestMultipleAlarmsStep::GetAlarmsFromConfigL(const TDesC& aSectName, CArrayFixFlat<TAlarmEntry>& aResult)
	{
	for(TInt i=0; i <= 99; i++)
		{
		TAlarmEntry entry;
		if(!GetAlarmFromConfig(aSectName, i, entry))
			{
			break;
			}
		aResult.AppendL(entry);
		}

	INFO_PRINTF2(KLoadedDAlarms, aResult.Count());
	if (aResult.Count() <= 0)
		{
		return EFalse;
		}

	return ETrue;
	}

TBool CTEAlarmTestMultipleAlarmsStep::GetAlarmFromConfig(const TDesC& aSectName, TInt aAlarmIdx, TAlarmEntry& aResult)
	{
	TBuf<10> alarmNN;
	alarmNN.Format(KAlarmNN, aAlarmIdx);
	TPtrC alarmLine;
	if(!GetStringFromConfig(aSectName, alarmNN, alarmLine))
		{
		return EFalse;
		}

	TInt offset;
	TLex alarmLex(alarmLine);
	TLex lex(alarmLex.NextToken());
	lex.Val(offset);

	TInt delay;
	lex.Assign(alarmLex.NextToken());
	lex.Val(delay);

	aResult.iAlarmIdx = aAlarmIdx;
	aResult.iOffset = offset;
	aResult.iDelay = delay;
	aResult.iSoundPlaying = EFalse;
	aResult.iVisible = EFalse;

	return ETrue;
	}

TBool CTEAlarmTestMultipleAlarmsStep::GetEventsFromConfigL(const TDesC& aSectName, RArray<TEventEntry>& aResult)
	{
	for(TInt i=0; i <= 99; i++)
		{
		TEventEntry entry;
		if(!GetEventFromConfigL(aSectName, i, entry))
			{
			break;
			}
		aResult.AppendL(entry);
		}

	INFO_PRINTF2(KLoadedDEvents, aResult.Count());
	if (aResult.Count() <= 0)
		{
		return EFalse;
		}

	return ETrue;
	}

TBool CTEAlarmTestMultipleAlarmsStep::GetEventFromConfigL(const TDesC& aSectName, TInt aEventIdx, TEventEntry& aResult)
	{
	TBuf<10> eventNN;
	eventNN.Format(KEventNN, aEventIdx);

	TBuf<10> actionNN;
	actionNN.Format(KActionNN, aEventIdx);

	TPtrC line;
	if( (!GetStringFromConfig(aSectName, eventNN, line) ) &&
		(!GetStringFromConfig(aSectName, actionNN, line)) )
		{
		return EFalse;
		}

	aResult.iSequenceNumber = aEventIdx;

	TLex lineLex(line);
	TLex lex(lineLex.NextToken());
	lex.Val(aResult.iAlarmIdx);
	TESTL(aResult.iAlarmIdx >= KAllAlarms && aResult.iAlarmIdx <= KBogusAlarmIdx);

	TPtrC typeName = lineLex.NextToken();
	aResult.iType = GetEventTypeFromString(typeName);
	TESTL(aResult.iType != EException);


	switch ( aResult.iType )
		{
	case EEventSetAlarm:		// Fall through...
	case EEventSoundStart:		// Fall through...
	case EEventSoundStop:		// Fall through...
	case EEventDelete:			// Fall through...
	case EActionSilence:		// Fall through...
	case EActionClear:			// Fall through...
	case EActionReconnectToAS:	// Fall through...
	case EActionAddAlarm:		// Fall through...
	case EActionDeleteAlarm:	// Fall through...
	case EException:			// Fall through...
	case EUnknown:
		// We've read all the values for these events.
		break;
	case EEventSetState:
		{
		TLex lex(lineLex.NextToken());
		TInt64 value;
		lex.Val(value, EHex);
		aResult.iAltSFlags = value;
		}
		break;
	case EEventVisible:
		// We still have the visible bool to read:
		{
		TPtrC visibility = lineLex.NextToken();
		aResult.iVisible = GetBoolFromStringL(visibility);
		}
		break;
	case EActionCheckState:
		// We still have values to read:
		{
		// <expected state>
		TPtrC state = lineLex.NextToken();
		aResult.iAlarmState  = GetAlarmStateFromStringL(state);

		// <sound playing>
		TPtrC playing = lineLex.NextToken();
		aResult.iPlaying = GetSoundPlayingFromStringL(playing);

		// <visible>
		TPtrC visibility = lineLex.NextToken();
		aResult.iVisible = GetVisibilityFromStringL(visibility);
		}
		break;
	case EActionCheckStateOnly:
		// We still have values to read:
		{
		// <expected state>
		TPtrC state = lineLex.NextToken();
		aResult.iAlarmState  = GetAlarmStateFromStringL(state);
		}
		break;

	case EActionSnooze:		// Fall through...
	case EActionPauseSound:	// Fall through...
		{
		// <snooze period> or <pausesound period>
		TLex lex(lineLex.NextToken());
		lex.Val(aResult.iPeriod);	// value specified in minutes
		aResult.iPeriod *= 60;	
		}
		break;

	case EActionUserWait:
		{
		// <wait period>
		TLex lex(lineLex.NextToken());
		lex.Val(aResult.iPeriod);	// value specified in seconds
		}
		break;
		}
	
	return ETrue;
	}

TEventType CTEAlarmTestMultipleAlarmsStep::GetEventTypeFromString(const TDesC& aEventString)
	{
	if(aEventString == KEventSetAlarm) return EEventSetAlarm;
	if(aEventString == KEventSetState) return EEventSetState;
	if(aEventString == KEventVisible) return EEventVisible;
	if(aEventString == KEventSoundStart) return EEventSoundStart;
	if(aEventString == KEventSoundStop) return EEventSoundStop;
	if(aEventString == KEventDelete) return EEventDelete;
	if(aEventString == KActionCheckState) return EActionCheckState;
	if(aEventString == KActionCheckStateOnly) return EActionCheckStateOnly;	
	if(aEventString == KActionSnooze) return EActionSnooze;
	if(aEventString == KActionClear) return EActionClear;
	if(aEventString == KActionPauseSound) return EActionPauseSound;	
	if(aEventString == KActionSilence) return EActionSilence;
	if(aEventString == KActionDeleteAlarm) return EActionDeleteAlarm;		
	if(aEventString == KActionUserWait) return EActionUserWait;
	if(aEventString == KActionReconnectToAS) return EActionReconnectToAS;	
	return EException;
	}

TAlarmState CTEAlarmTestMultipleAlarmsStep::GetAlarmStateFromStringL(TPtrC& aStateString)
	{
	if(aStateString == KStateQueued) return EAlarmStateQueued;
	if(aStateString == KStateNotifying) return EAlarmStateNotifying;
	if(aStateString == KStateWaitingToNotify) return EAlarmStateWaitingToNotify;
	if(aStateString == KStateSnoozed) return EAlarmStateSnoozed;
	if(aStateString == KStateNotified) return EAlarmStateNotified;
	if(aStateString == KStateInPreparation) return EAlarmStateInPreparation;
	TESTL(EFalse); // Only the above states are valid.
	return EAlarmStateInPreparation;
	}

TBool CTEAlarmTestMultipleAlarmsStep::GetVisibilityFromStringL(TPtrC& aVisibilityString)
	{
	if(aVisibilityString == KVisible) return ETrue;
	if(aVisibilityString == KNotVisible) return EFalse;
	TESTL(EFalse); // Only the above 2 strings are valid.
	return EFalse;
	}

TBool CTEAlarmTestMultipleAlarmsStep::GetBoolFromStringL(TPtrC& aBoolString)
	{
	if(aBoolString == KTrue) return ETrue;
	if(aBoolString == KFalse) return EFalse;
	TESTL(EFalse); // Only the above 2 strings are valid.
	return EFalse;
	}
	
TBool CTEAlarmTestMultipleAlarmsStep::GetSoundPlayingFromStringL(TPtrC& aPlayingString)
	{
	if(aPlayingString == KSoundOn) return ETrue;
	if(aPlayingString == KSoundOff) return EFalse;
	TESTL(EFalse); // Only the above 2 strings are valid.
	return EFalse;
	}

