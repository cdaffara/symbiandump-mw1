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

#include "asaltrequestsetalarm.h"
#include "ASAltClientSession.h"
#include "ASSrvAlarmQueue.h"
#include "ASSrvSessionCollection.h"

CASAltRequestSetAlarm::CASAltRequestSetAlarm(RASAltClientSession& aSession, CASSrvNotificationCoordinator& aParent, CASSrvAlarmQueue& aQueue, MASSrvSessionCollection& aSessionCollection)
:	CActive(CActive::EPriorityHigh), iSession(aSession), iParent(aParent), iQueue( aQueue ), iSessionCollection( aSessionCollection )
	{
	CActiveScheduler::Add(this);
	}


CASAltRequestSetAlarm::~CASAltRequestSetAlarm()
	{
	Cancel();
	delete iAlarmCopy;
	delete iAlarmDataCopy;
	iRequestedAlarmIds.Reset();
	}

void CASAltRequestSetAlarm::RequestSetAlarm(TAlarmId aId)
	{
	if (!IsActive())
		{
		RequestNextAlarm(aId);
		}
	else
		{
		// Non-leaving version of Append intentionaly used here
		// Space for 8 elements has been reserved in ConstructL
		iRequestedAlarmIds.Append(aId);
		}
	}

void CASAltRequestSetAlarm::RequestNextAlarm(TAlarmId aId)
    {
    ASSERT(!IsActive());
    
	// Get the alarm details from the queue and prepare any associated data
    const TASSrvAlarm* alarm = iQueue.QueueAlarmById(aId);
	if  (alarm)
	    {
    	// Notify the alarm alert server about the alarm and its associated
    	// data (if it has any)
    	TPtrC8 pAlarmData(KNullDesC8);
    	if (alarm->HasAssociatedData())
    		{
    		pAlarmData.Set(alarm->Data());
    		}

        // Get alarm owner
    	TFullName owner(KNullDesC);
    	iSessionCollection.MASSessionCollectionAlarmOwner(aId, owner);

        // Get alarm data
    	delete iAlarmDataCopy;
    	iAlarmDataCopy = NULL;
    	TRAPD(err, iAlarmDataCopy = pAlarmData.AllocL());
     	
     	// Queue alarm with alert server
     	*iAlarmCopy = *alarm;
        
     	if  (iAlarmDataCopy && err == KErrNone)
     		{
     		iSession.SetAlarm(iStatus, *iAlarmCopy, owner, *iAlarmDataCopy);
     		}
     	else
     		{
      		iSession.SetAlarm(iStatus, *iAlarmCopy, owner, KNullDesC8());
    		}
    	
    	SetActive();
	    }
    }
    
void CASAltRequestSetAlarm::ConstructL()
	{
	iAlarmCopy = new(ELeave) TASShdAlarm;
	iRequestedAlarmIds.ReserveL(8);
	}

CASAltRequestSetAlarm* CASAltRequestSetAlarm::NewL(RASAltClientSession& aSession, CASSrvNotificationCoordinator& aParent, CASSrvAlarmQueue& aQueue, MASSrvSessionCollection& aSessionCollection)
	{
	CASAltRequestSetAlarm* self = new(ELeave) CASAltRequestSetAlarm(aSession, aParent, aQueue, aSessionCollection);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CASAltRequestSetAlarm::RequestSetAlarmCancel(TAlarmId aId)
    {
    if  (IsActive() && (iAlarmCopy->Id() == aId))
        {
        iCancelledCurrentRequest = ETrue;
        }
    else
        {
		const TInt KPosition(iRequestedAlarmIds.Find(aId));
		
		if (KPosition != KErrNotFound)
			{
			iRequestedAlarmIds.Remove(KPosition);
			}
        }
    }
    

void CASAltRequestSetAlarm::RunL()
	{
	if (!iCancelledCurrentRequest)
	    {
    	iParent.CompleteUpdateAlarmAlertServer(iAlarmCopy);
	    }
	    
	// Do we have more items to request?
	if (iRequestedAlarmIds.Count() > 0)
		{
		// Request next item
		const TAlarmId KHeadItem = iRequestedAlarmIds[0];
		iRequestedAlarmIds.Remove(0);
		RequestNextAlarm(KHeadItem);
		}
		
	iCancelledCurrentRequest = EFalse;
	}
	
void CASAltRequestSetAlarm::DoCancel()
	{
	}



