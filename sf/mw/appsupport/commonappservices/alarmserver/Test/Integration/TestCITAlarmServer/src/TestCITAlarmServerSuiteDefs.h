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



#ifndef __TESTCITALARMSERVER_SUITEDEFS_H__
#define __TESTCITALARMSERVER_SUITEDEFS_H__

#define MAX_TEXT_MESSAGE 256

#include <e32test.h>
#include <s32file.h>
#include <e32std.h> 
#include <f32file.h> 
#include <e32cmn.h>

_LIT(KColon, ":");

//Date Time
_LIT(KDateTime,"%d/%d/%d  %d:%d:%d.%d");

/** literals */
_LIT(KTest,	"test");

/** constants */
const TInt KMaxNumberOfCharac = 90;
const TInt KSecondsInHour = 3600;
const TInt KDelay10sec = 10;
const TInt KDelay6sec = 6;
const TInt KDelay25sec = 25;

#include <asshdalarm.h>
#include <asclisession.h>
#include <consolealarmalertservermain.h>
#include <asclisoundplay.h>
#include "ASAltClientSession.h"
#include <test/testexecutelog.h>

/** Comparision Error CTestCITAlarmServersuite*/
_LIT (KUTCOffsetError, "ERROR -- Comparision error:- UTC Offset not match");
_LIT (KSetUTCOffsetError, "ERROR -- UTC Offset not set");
_LIT (KAddAlarmError, "ERROR -- Alarm has not been added to Alarm Server Session");
_LIT (KAlarmDeleteError, "ERROR -- Alarm could not be deleted");
_LIT (KAlarmNotifyingError, "ERROR: Alarm not notifying after System Change");
_LIT (KAlarmSilencedError, "ERROR : Alarm sounds has not been Temporarily Silenced");
_LIT (KAlarmSilencedError1, "ERROR : Alarm sounds has been Temporarily Silenced");
_LIT (KAlarmCreatedError, "ERROR: Floating alarm created");
_LIT (KAlarmExpireError, "ERROR: Alarm Did Not Expire At Expected Time");
_LIT (KAlarmSoundStartError, "ERROR: Sound Did Not Start At Expected Time");
_LIT (KAlarmSoundStopError, "ERROR: Sound Did Not Stop At Expected Time");

_LIT (KQueued, "QUEUED");
_LIT (KNotified, "NOTIFIED");
_LIT (KNotifying, "NOTIFYING");

// For test step panics
_LIT(KTestCITAlarmServerSuitePanic,"TestCITAlarmServerSuite");

_LIT(KGlobalSoundState,"%S");
_LIT(KGlobalSoundStateOff, "EAlarmGlobalSoundStateOff");
_LIT(KGlobalSoundStateOn, "EAlarmGlobalSoundStateOn");
_LIT(KStartSound,"Console Alert Server Has Received StartPlayingSound for Alarm");
_LIT(KAlarmExpired,"Alarm No %d Has Expired  Expected Alarm No %d");
_LIT(KAlarmStateMessage, " Alarm does not have the state being checked for.");
_LIT(KPlayStart,"Sound Has Started To Play For Alarm No %d Expected Alarm No %d");
_LIT(KPlayStop,"Sound Has Stopped Playing For Alarm No %d Expected Alarm No %d");
_LIT(KAlarmNumber,"ALARM");
_LIT(KSystemTime, "SystemTime");
_LIT(KInitialUTC, "InitialUTC");
_LIT(KAlarmExpires, "AlarmExpires");
_LIT(KSilentFor, "SilentFor");
_LIT(KWaitToNotify, "WaitToNotifyBeforeChange");
_LIT(KMoveUTC, "MoveUTC");
_LIT(KSilentUntil, "SilentUntil");
_LIT(KUTC, "UTC");
_LIT(KCHANGEUTC, "CHANGEUTC");
_LIT(KMoveSystemTime, "MoveSystemTime");
_LIT(KQuietPeriod, "QuietPeriod");
_LIT(KSoundStartTime, "SoundStartTime");
_LIT(KAlarmInQueue, "alarminqueue");
_LIT(KExpectedExpiryTime, "expectedExpiryTime");
_LIT(KExpectedNextExpiryTime, "nextCalculatedexpiryTime");
_LIT(KNextDueTime," For Alarm No %d Expected Alarm No %d");
_LIT (KNextDueTimeError, "ERROR: Next Due Time not as expected");
_LIT(KAlarmType, "type");

const TInt KTimeToWait = 1000000;

#endif
