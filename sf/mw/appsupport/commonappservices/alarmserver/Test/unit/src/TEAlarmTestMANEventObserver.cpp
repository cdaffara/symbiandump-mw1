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

#include "TEAlarmTestMANEventObserver.h"
#include "consoleantestclient.h"


// //
//
// CMANEventObserver : public CActive
//
// //

CMANEventObserver::CMANEventObserver(const TEventEntry& aEventEntry, RANTestClient& aANTestClient,
								MMANEventHandler& aEventHandler) : CActive(EPriorityStandard),
		iCAASClient(aANTestClient), iEventHandler(aEventHandler), iObserveEvent(aEventEntry)
	{
	CActiveScheduler::Add(this);	
	}


CMANEventObserver::~CMANEventObserver()
	{
	Deque();	
	}


CMANEventObserver* CMANEventObserver::NewL(const TEventEntry& aEventEntry, 
		RANTestClient& aANTestClient, MMANEventHandler& aEventHandler)
	{
	__ASSERT_ALWAYS( aEventEntry.iType<=EEventDelete, \
						User::Leave(KErrArgument));
	CMANEventObserver* self = 
			new (ELeave) CMANEventObserver(aEventEntry, aANTestClient, aEventHandler);

	self->RegisterForNextEventL();
	return self;
	}


void CMANEventObserver::RegisterForNextEventL()
	{
	switch (iObserveEvent.iType)
		{
		case EEventSetAlarm:
				iCAASClient.NotifyOnAlarm(iASShdAlarm, iStatus);
				break;
		case EEventSetState: 
				iCAASClient.NotifyOnState(iObserveEvent.iAlarmId, 
								iObserveEvent.iAltSFlags, iStatus);
				break;
		case EEventVisible:
				iCAASClient.NotifyOnVisible(iObserveEvent.iAlarmId,
								iObserveEvent.iVisible, iStatus);
				break;
		case EEventSoundStart:
				iCAASClient.NotifyOnSoundStart(iObserveEvent.iAlarmId,
								iSoundFileName,	iStatus);
				break;
		case EEventSoundStop:
				iCAASClient.NotifyOnSoundStop(iObserveEvent.iAlarmId, iStatus);
				break;
		case EEventDelete:
				iCAASClient.NotifyOnDelete(iObserveEvent.iAlarmId, iStatus);
				break;
		default: 
				User::Leave(KErrArgument);
		}
	SetActive();
	}


void CMANEventObserver::RunL()
	{
	if (iStatus!=KErrNone)
		{
		TEventEntry errorEvent(EException, iStatus.Int());
		iEventHandler.HandleEventL(errorEvent);
		return;
		}

	RegisterForNextEventL();

	if (iObserveEvent.iType==EEventSetAlarm)
		{
		iObserveEvent.iAlarmId = iASShdAlarm.Id();
		}

	iEventHandler.HandleEventL(iObserveEvent);
	}
	
	

// //
//
// CMANDelayedActionHandler : public CTimer
//
// //

CMANDelayedActionHandler::CMANDelayedActionHandler(const TEventEntry& anAction,
					MMANEventHandler& aEventHandler) : CTimer(EPriorityStandard),
					iEventHandler(aEventHandler), iDelayedAction(anAction)
	{
	CActiveScheduler::Add(this);
	}


CMANDelayedActionHandler::~CMANDelayedActionHandler()
	{
	Deque();	
	}


CMANDelayedActionHandler* CMANDelayedActionHandler::NewL(const TEventEntry& anAction, 
								MMANEventHandler& aEventHandler)
	{
	__ASSERT_ALWAYS( (anAction.iType==EActionAddAlarm \
							|| anAction.iType==EActionUserWait \
								|| anAction.iType==EActionReconnectToAS), \
						User::Leave(KErrArgument));
	CMANDelayedActionHandler* self = new (ELeave) CMANDelayedActionHandler(anAction, aEventHandler);
	CleanupStack::PushL(self);
	self->ConstructL();						// CTimer::ConstructL();
	CleanupStack::Pop();
	
	self->After(1000000 * anAction.iPeriod);
	
	return self;
	}


void CMANDelayedActionHandler::RunL()
	{
	if (iStatus==KErrNone)
		{
		iEventHandler.HandleEventL(iDelayedAction);
		}
	else
		{
		TEventEntry errorEvent(EException, iStatus.Int());
		iEventHandler.HandleEventL(errorEvent);
		}
	}

