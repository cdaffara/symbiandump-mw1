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

#include "TEAlarmTestMANEventEntry.h"

_LIT(KLitEvent, "EVENT");
_LIT(KLitAction, "ACTION");
_LIT(KLit02D, "%02d");
_LIT(KLitA02D, " A%02d");
_LIT(KLitColon, ":");
_LIT(KLitS, " %S");
_LIT(KLitFlags, " flags=0x%x");
_LIT(KLitDSec, " %dsec");
_LIT(KLitCompleted, " completed");
_LIT(KLitForDMinutes, " for %d minutes");



TEventEntry::TEventEntry(TEventType aEventType, TInt aError) :
			iType(aEventType), 
			iSequenceNumber(-1),
		 	iAlarmIdx(KBogusAlarmIdx),
		 	iAlarmId(KNullAlarmId),	
		 	iAltSFlags(0),
		 	iAlarmState(EAlarmStateInPreparation),
		 	iPlaying(EFalse),
		 	iVisible(EFalse),
		 	iPeriod(0),
		 	iError(KErrNone) 
	{
	if (iType==EException)  
		{
		iError = aError;
		}
	}
	

TBool TEventEntry::operator==(const TEventEntry& aEvent)
	{
	if ( iType!=aEvent.iType
			|| iAlarmIdx!=aEvent.iAlarmIdx
				|| (iType==EActionCheckState && iAlarmState!=aEvent.iAlarmState)
				|| (aEvent.iType==EEventVisible && iVisible!=aEvent.iVisible) )
					{
					return EFalse;
					}
	return ETrue;
	}

	
const TDesC& TEventEntry::TypeToStr()
	{
	switch (iType)
		{
		case EEventSetAlarm:
				return KEventSetAlarm;
		case EEventSetState:
				return KEventSetState;
		case EEventVisible:
				return KEventVisible;
		case EEventSoundStart:
				return KEventSoundStart;
		case EEventSoundStop:
				return KEventSoundStop;
		case EEventDelete:
				return KEventDelete;
		case EActionCheckState:	
				return KActionCheckState; 
		case EActionCheckStateOnly:
				return KActionCheckStateOnly; 
		case EActionSnooze:
				return KActionSnooze; 
		case EActionClear:
				return KActionClear;
		case EActionPauseSound:	
				return KActionPauseSound;
		case EActionSilence:	
				return KActionSilence;
		case EActionUserWait:
				return KActionUserWait;
		case EActionReconnectToAS:
				return KActionReconnectToAS;				
		case EActionAddAlarm:
				return KActionAddAlarm;
		case EActionDeleteAlarm:
				return KActionDeleteAlarm;
		case EException:
				return KException;
		case EUnknown:
		default: 	
				return KUnknown;
		}
	}


void TEventEntry::ToStr(TDes& aStr)
	{
	if (IsEvent())  							// Event or Action
		{
		aStr.Append(KLitEvent);
		}
	else
		{
		aStr.Append(KLitAction);
		}

	if (iSequenceNumber>=0)
		{
		aStr.AppendFormat(KLit02D, iSequenceNumber);		
		}

	aStr.Append(KLitColon);

	if (iType!=EActionUserWait)
		{
		aStr.AppendFormat(KLitA02D, iAlarmIdx);	// Alarm idx
		}
	
	aStr.AppendFormat(KLitS, &TypeToStr());	// Event name


	if (iType==EEventVisible)  			// Visibility
		{
		aStr.AppendFormat(KLitS, &BoolToStr(iVisible));
		}
	
	if (iType==EEventSetState) 			// State flags
		{
		aStr.AppendFormat(KLitFlags, iAltSFlags);
		}

	if (iType==EActionUserWait)
		{
		aStr.AppendFormat(KLitDSec, iPeriod);
		if (iAlarmIdx==KBogusAlarmIdx)  aStr.AppendFormat(KLitCompleted);
		}
		
	if (iType==EActionSnooze || iType==EActionPauseSound)
		{
		aStr.AppendFormat(KLitForDMinutes, iPeriod/60);
		}
	}

