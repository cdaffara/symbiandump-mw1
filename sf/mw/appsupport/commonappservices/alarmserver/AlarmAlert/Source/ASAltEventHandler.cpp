// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ASAltEventHandler.h"

// User includes
#include <asaltdefs.h>
#include "ASAltClientSession.h"
#include "ASAltAlarmAlertObserver.h"
#include "ASSrvStaticUtils.h"
#include "ASAltRequestQuietPeriodEnd.h"

//
// ----> CASAltEventHandler (source)
//

//*************************************************************************************
CASAltEventHandler::CASAltEventHandler(RASAltClientSession& aSession, MASAltAlarmAlertObserver& aObserver)
:	CActive(EPriorityUserInput), iSession(aSession), iObserver(aObserver)
	{
	CActiveScheduler::Add(this);
	}


//*************************************************************************************
CASAltEventHandler::~CASAltEventHandler()
	{
	Cancel();
	delete iRequestQuietPeriodEnd;
	}


//*************************************************************************************
void CASAltEventHandler::ConstructL()
	{
	iRequestQuietPeriodEnd=CASAltRequestQuietPeriodEnd::NewL(iSession,iObserver);
	RequestEvents();
	}


//*************************************************************************************
CASAltEventHandler* CASAltEventHandler::NewL(RASAltClientSession& aSession, MASAltAlarmAlertObserver& aObserver)
	{
	CASAltEventHandler* self = new(ELeave) CASAltEventHandler(aSession, aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}


//
//
//


//*************************************************************************************
/**
 * Request events from the Alarm Alert Server
 */
void CASAltEventHandler::RequestEvents()
	{
	__ASSERT_DEBUG(!IsActive(), ASSrvStaticUtils::Fault(ASSrvStaticUtils::EASSrvFaultAlarmAlertServerResponseReaderAlreadyActive));

	Session().NotifyOnResponse(iStatus, iTimeValueFromAlarmAlertServer, iAlarmId);
	SetActive();
	}


//
//
//


//*************************************************************************************
void CASAltEventHandler::RunL()
	{
	const TInt response = iStatus.Int();
	if	( response == KErrCancel
		|| response == KErrServerTerminated)	// if Alarm Alert Server dies, don't ask
												// for any further information from it
		return;

	// Request any further events from the server. Do this now so that
	// we don't miss anything whilst processing this event.
	RequestEvents();

	switch(response)
		{
	// The Alarm Alert Server requested that alarm of the returned alarm id be silenced
	case EASAltAlertServerResponseSilence:
		Observer().ASAltSilenceCurrentAlarm(iAlarmId);
		break;

	// Clear the current alarm and setup ready for the next pending alarm
	case EASAltAlertServerResponseClear:
		Observer().ASAltAcknowledgeAlarm(MASAltAlarmAlertObserver::EAcknowledgeCurrentAlarm, iAlarmId);
		break;

	// Clears all alarms in the due que
	case EASAltAlertServerResponseClearAll:
		Observer().ASAltAcknowledgeAlarm(MASAltAlarmAlertObserver::EAcknowledgeAllAlarms, iAlarmId);
		break;

	// The Alarm Alert Server requested that all alarms are quiet until the specified time
	case EASAltAlertServerResponseQuietPeriod:
		{
		iRequestQuietPeriodEnd->RequestQuietPeriodEnd();
		}
		break;

	// The Alarm Alert Server requested that sound playing is paused until 
	// the specified time. When requested, we also need to reset the cycle
	// count back to zero (since the user requested this snooze, rather 
	// than it being an automatic one).
	case EASAltAlertServerResponsePauseSound:
		Observer().ASAltPauseSoundUntil(iTimeValueFromAlarmAlertServer, iAlarmId);
		break;

	// The Alarm Alert Server requested that the alarm is snoozed until the specified time
	case EASAltAlertServerResponseSnooze:
		Observer().ASAltSnoozeAlarmUntil(iTimeValueFromAlarmAlertServer, iAlarmId);
		break;

	case KErrServerBusy:
		// Simply try again
		break;

	default:
		__ASSERT_DEBUG(EFalse, ASSrvStaticUtils::Fault(ASSrvStaticUtils::EASSrvFaultInvalidAlarmAlertServerResponse));
		break;
		}
	}

//*************************************************************************************
void CASAltEventHandler::DoCancel()
	{
	if	(Session().Handle() == KNullHandle)
		{
		TRequestStatus* status = &iStatus;
		User::RequestComplete(status, KErrCancel);
		}
	else
		{
		Session().NotifyOnResponseCancel();
		}
	}


//
//
//

