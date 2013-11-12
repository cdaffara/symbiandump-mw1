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

#ifndef __TEALARMTESTMANEVENTENTRY_H__
#define __TEALARMTESTMANEVENTENTRY_H__

#include <asshddefs.h>


const TInt KAllAlarms = -1;
const TInt KBogusAlarmIdx = 99;

enum TEventType
	{
	EEventSetAlarm,
	EEventSetState,
	EEventVisible,
	EEventSoundStart,
	EEventSoundStop,
	EEventDelete,
	EActionCheckState,
	EActionCheckStateOnly,
	EActionSnooze,
	EActionClear,
	EActionPauseSound,
	EActionSilence,
	EActionUserWait,
	EActionReconnectToAS,		
	EActionAddAlarm,
	EActionDeleteAlarm,
	EException,
	EUnknown
	};

_LIT(KEventSetAlarm,	"SetAlarm");
_LIT(KEventSetState,	"SetState");
_LIT(KEventVisible,		"Visible");
_LIT(KEventSoundStart,	"SoundStart");
_LIT(KEventSoundStop,	"SoundStop");
_LIT(KEventDelete,		"Delete");
_LIT(KActionCheckState,	"CheckState");
_LIT(KActionCheckStateOnly,	"CheckStateOnly");
_LIT(KActionSnooze,		"Snooze");
_LIT(KActionClear,		"Clear");
_LIT(KActionPauseSound,	"PauseSound");
_LIT(KActionSilence,	"Silence");
_LIT(KActionUserWait,	"UserWait");
_LIT(KActionReconnectToAS,	"ReconnectToAS");
_LIT(KActionAddAlarm,	"AddAlarm");
_LIT(KActionDeleteAlarm,"DeleteAlarm");
_LIT(KException,		"Exception");
_LIT(KUnknown,			"Unknown");

_LIT(KTrue,	"True");
_LIT(KFalse,"False");



// //
//
// TEventEntry
//
// //
class TEventEntry		// Event or Action
	{
public:
	TEventEntry(TEventType aEventType = EUnknown, TInt aError = KErrUnknown);
	TBool operator==(const TEventEntry& aEntry);
	inline TBool operator!=(TEventEntry& aEntry) { return !(*this == aEntry); }

	inline TBool IsEvent();
	inline TBool IsAction();

	void ToStr(TDes& aStr);	
	const TDesC& TypeToStr();

public:
	TEventType 	iType;
	TInt 		iSequenceNumber;
	TInt 		iAlarmIdx;	// KAllAlarms 	  = event/action for all alarms
							// KBogusAlarmIdx = non-existent alarm
	TAlarmId	iAlarmId;	
	TInt		iAltSFlags;	// applies to EEventSetState
	TAlarmState	iAlarmState;// applies to EActionCheckState
	TBool		iPlaying;	// applies to EActionCheckState only
	TBool		iVisible;	// applies to EActionCheckState only
	TInt		iPeriod;	// in seconds, applies to EActionSnooze, 
							//   EActionPauseSound and EActionUserWait
	TInt		iError;		// holds error code if EException type
	};



inline TBool TEventEntry::IsEvent() 
	{ 
	return !IsAction(); 
	}
	
inline TBool TEventEntry::IsAction()
	{ 
	return ( iType >= EActionCheckState && iType <= EActionDeleteAlarm ); 
	}

static inline const TDesC& BoolToStr(TBool aBool)
	{
	return aBool? (const TDesC&)KTrue: (const TDesC&)KFalse;
 	}


#endif // __TEALARMTESTMANEVENTENTRY_H__

