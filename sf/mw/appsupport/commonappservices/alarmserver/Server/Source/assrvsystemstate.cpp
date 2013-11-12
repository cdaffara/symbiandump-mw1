// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "assrvsystemstate.h"

#include <startupdomaindefs.h>
#include <ssm/ssmsubstates.hrh>

// Some constants defined for ssm workaround
const TUid KCategoryWorkAroundUID = {0x101f5027};
const TInt KKeyWorkAroundStateChange = 301;
const TInt KKeyWorkAroundAck = 302;
	
CASSrvSystemState* CASSrvSystemState::NewL()
	{
	CASSrvSystemState* self = new(ELeave) CASSrvSystemState();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CASSrvSystemState::~CASSrvSystemState()
	{
	Cancel();
	iStateAwareSession.Close();
	iObservers.Close();
	
	// close the workaround property
	iWorkAroundProperty.Close();
	}

CASSrvSystemState::CASSrvSystemState()
	:CActive(CActive::EPriorityStandard)
	{
	CActiveScheduler::Add(this);
	}

void CASSrvSystemState::ConstructL()
	{
	iWorkAround = (iWorkAroundProperty.Attach(KCategoryWorkAroundUID, KKeyWorkAroundStateChange) == KErrNone);
	
	if (iWorkAround)
		{
		// the workwround property has been defined so we must be in test code
		// subscribe to changes to this property
		iWorkAroundProperty.Subscribe(iStatus);
		SetActive();
		}
	else
		{
		// This should be all that is in this function when we remove the workaround
		TInt connectError = iStateAwareSession.Connect(KSM2AppServicesDomain3);
		if (connectError == KErrNone)
			{
			iStateAwareSession.RequestStateNotification(iStatus);
			SetActive();
			}
		}
	}

void CASSrvSystemState::RunL()
	{
	// If there is an error fail silently, there isn't anything useful we can do
	// and it would be excessive to panic the server because we can't register a device wakeup
	
	if (iStatus == KErrNone)
		{
		MASSrvSystemStateObserver::TState state;
		if (GetState(state))
			{
			// notify the observers
			const TInt KObserverCount(iObservers.Count()); 
			for (TInt i(0) ; i < KObserverCount ; ++i)
				{
				iObservers[i]->MHandleSystemStateChange(state);
				}
			}
		
		AcknowledgeAndRequestStateNotification();
		}
	}

void CASSrvSystemState::DoCancel()
	{
	if (iWorkAround)
		{
		iWorkAroundProperty.Cancel();
		}
	else
		{
		iStateAwareSession.RequestStateNotificationCancel();
		}
	}

void CASSrvSystemState::RequestNotificationL(MASSrvSystemStateObserver& aObserver)
	{
	User::LeaveIfError(iObservers.InsertInAddressOrder(&aObserver));
	}

void CASSrvSystemState::RequestNotificationCancel(MASSrvSystemStateObserver& aObserver)
	{
	TInt index = KErrNotFound;
	const TInt error = iObservers.FindInAddressOrder(&aObserver, index);
	
	if	(error != KErrNotFound)
		{
		iObservers.Remove(index);
		}
	}

TBool CASSrvSystemState::GetState(MASSrvSystemStateObserver::TState& aState)
	{
	TBool interestedInChange(EFalse);
	TSsmState ssmState;
	
	if (iWorkAround)
		{
		TInt value;
		iWorkAroundProperty.Get(KCategoryWorkAroundUID, KKeyWorkAroundStateChange, value);
		ssmState.Set(value, 0x10);
		}
	else
		{
		ssmState = iStateAwareSession.State();
		}
		
	if (ssmState.MainState() == ESsmShutdown && ssmState.SubState() == ESsmShutdownSubStateCritical)
		{
		aState = MASSrvSystemStateObserver::EShutdown;
		interestedInChange = ETrue;
		}
	else if (ssmState.MainState() == ESsmNormal && ssmState.SubState() == ESsmNormalSubState)
		{
		aState = MASSrvSystemStateObserver::ENormal;
		interestedInChange = ETrue;
		}
	
	return interestedInChange;
	}

void CASSrvSystemState::AcknowledgeAndRequestStateNotification()
	{
	if (iWorkAround)
		{
		// Send an acknoledgement back to the test code
		// that we have finished actioning the state notification
		RProperty::Set(KCategoryWorkAroundUID, KKeyWorkAroundAck, 3);
		iWorkAroundProperty.Subscribe(iStatus);
		}
	else
		{
		// Acknowledge this state change and request notification
		// for the next state change
		iStateAwareSession.AcknowledgeAndRequestStateNotification(KErrNone, iStatus);
		}
	
	SetActive();
	}

