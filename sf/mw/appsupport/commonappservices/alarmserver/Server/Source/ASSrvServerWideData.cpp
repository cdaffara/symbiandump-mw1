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

#include "ASSrvServerWideData.h"

// User Includes
#include "ASSrvTimer.h"
#include "ASSrvSession.h"
#include "ASSrvDataPool.h"
#include "ASSrvAlarmQueue.h"
#include "ASSrvAlarmStore.h"
#include "ASSrvStaticUtils.h"
#include "ASSrvSessionEngine.h"
#include "ASSrvSoundSettings.h"
#include "ASSrvSoundController.h"
#include "ASSrvAnyEventObserver.h"
#include "ASSrvNotificationCoordinator.h"
#include "ASSrvEnvironmentChangeManager.h"

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
    #include "assrvsystemstate.h"
#endif

CASSrvServerWideData::CASSrvServerWideData() : iCachedUtcOffset(0)
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
, iServerIsReadOnly(EFalse)
#endif
	{
	}

CASSrvServerWideData::~CASSrvServerWideData()
	{
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	delete iASSrvSystemState;
#endif
	delete iStore;
	delete iSoundCountroller;
	delete iNotificationCoordinator;
	delete iDataPool;
	delete iSoundSettings;
	delete iTimer;
	delete iQueue;
	delete iEnvironmentChangeManager;
	
	iAnyEventObservers.Close();
	iActiveSessions.Close();
	
	iFsSession.Close();
	}

void CASSrvServerWideData::ConstructL()
	{
	User::LeaveIfError(iFsSession.Connect());

	// NOTE: The construction order ***IS IMPORTANT***
	iEnvironmentChangeManager = CASSrvEnvironmentChangeManager::NewL(*this);
	iQueue = CASSrvAlarmQueue::NewL(*this);
	iTimer = CASSrvAlarmTimer::NewL(*this);
	iSoundSettings = CASSrvSoundSettings::NewL(*this);
	iDataPool = CASSrvDataPool::NewL(*this);
	iNotificationCoordinator = CASSrvNotificationCoordinator::NewL(*this);
	iSoundCountroller = CASSrvSoundController::NewL(*this, *iNotificationCoordinator);
	iStore = CASSrvAlarmStore::NewL(*this);
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	iASSrvSystemState = CASSrvSystemState::NewL();
	iASSrvSystemState->RequestNotificationL(*iStore);
	iASSrvSystemState->RequestNotificationL(*this);
#endif
	}

CASSrvServerWideData* CASSrvServerWideData::NewL()
	{
	CASSrvServerWideData* self = new(ELeave) CASSrvServerWideData();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
void CASSrvServerWideData::MHandleSystemStateChange(TState aState)
	{
	switch (aState)
		{
		case EShutdown:
		    {
            iServerIsReadOnly = ETrue;
            break;
		    }
		case ENormal:
		    {
            iServerIsReadOnly = EFalse;
            break;
		    }
		default:
			break;
		}
	}

TBool CASSrvServerWideData::ServerIsReadOnly()
	{
	return iServerIsReadOnly;
	}
#endif

/**
 * Access the file server session
 */
RFs& CASSrvServerWideData::FsSession() const
	{
	return const_cast<CASSrvServerWideData*>(this)->iFsSession;
	}

/**
 * Access the sound settings
 */
CASSrvSoundSettings& CASSrvServerWideData::SoundSettings() const
	{
	return *iSoundSettings;
	}

/**
 * Access the alarm queue
 */
CASSrvAlarmQueue& CASSrvServerWideData::Queue() const
	{
	return *iQueue;
	}

/**
 * Access the sound controller
 */
CASSrvSoundController& CASSrvServerWideData::SoundController() const
	{
	return *iSoundCountroller;
	}

/**
 * Access the data pool
 */
CASSrvDataPool& CASSrvServerWideData::DataPool() const
	{
	return *iDataPool;
	}

/**
 * Access the timer
 */
CASSrvAlarmTimer& CASSrvServerWideData::Timer() const
	{
	return *iTimer;
	}

/**
 * Access the "any event manager"
 */
MASSrvAnyEventManager& CASSrvServerWideData::AnyEventManager() const
	{
	return *const_cast<CASSrvServerWideData*>(this);
	}

/**
 * Access the environment change manager
 */
CASSrvEnvironmentChangeManager& CASSrvServerWideData::EnvironmentChangeManager() const
	{
	return *iEnvironmentChangeManager;
	}

/**
 * Access the notification co-ordinator
 */
CASSrvNotificationCoordinator& CASSrvServerWideData::NotificationCoordinator() const
	{
	return *iNotificationCoordinator;
	}

/**
 * Access the session collection
 */
MASSrvSessionCollection& CASSrvServerWideData::SessionCollection() const
	{
	return *const_cast<CASSrvServerWideData*>(this);
	}

/**
 * @see MASSrvAnyEventManager
 */
void CASSrvServerWideData::MASAnyEventManagerObserverAddL(MASSrvAnyEventObserver& aObserver)
	{
	User::LeaveIfError(iAnyEventObservers.InsertInAddressOrder(&aObserver));
	}

/**
 * @see MASSrvAnyEventManager
 */
void CASSrvServerWideData::MASAnyEventManagerObserverRemove(const MASSrvAnyEventObserver& aObserver)
	{
	TInt index = KErrNotFound;
	const TInt error = iAnyEventObservers.FindInAddressOrder(&aObserver, index);
	if	(error != KErrNotFound)
	    {
	    iAnyEventObservers.Remove(index);
	    }
	}

/**
 * @see MASSrvAnyEventManager
 */
void CASSrvServerWideData::MASAnyEventManagerObserverNotifyChanges(TAlarmChangeEvent aEvent, TAlarmId aAlarmId)
	{
	const TInt count = iAnyEventObservers.Count();
	for(TInt i=0; i<count; i++)
		{
		// Notify observers
		iAnyEventObservers[i]->MASSrvAnyEventHandleChange(aEvent, aAlarmId);
		}
	}

/**
 * @see MASSrvSessionCollection
 */
TASSrvSessionId CASSrvServerWideData::MASSessionCollectionAttachL(const MASSrvSession& aSession)
	{
	// Add to collection
	User::LeaveIfError(iActiveSessions.InsertInAddressOrder(&aSession));

	// Generate Id
	if	(++iNextFreeSessionId == KMaxTInt)
	    {
	    iNextFreeSessionId = 0;
	    }
		
	return iNextFreeSessionId;
	}

/**
 * @see MASSrvSessionCollection
 */
void CASSrvServerWideData::MASSessionCollectionDetach(const MASSrvSession& aSession)
	{
	TInt index = KErrNotFound;
	const TInt error = iActiveSessions.FindInAddressOrder(&aSession, index);
	if	(error != KErrNotFound)
	    {
	    iActiveSessions.Remove(index);
	    }

	// Remove any session alarms for this session
	Queue().RemoveAllSessionAlarmsBySessionId(aSession.MASSrvSessionId());
	}

/**
 * @see MASSrvSessionCollection
 */
TInt CASSrvServerWideData::MASSessionCollectionAlarmOwner(TAlarmId aAlarmId, TDes& aFullName) const
	{
	// If the alarm doesn't have an originating session id then we can't return the owning session name.
	const TASSrvAlarm* alarm = Queue().QueueAlarmById(aAlarmId);
	if (!alarm)
		{
		// If the alarm doesn't exist then we can't return the owning session
		return KErrNotFound;
		}
	if	(alarm->OriginatingSessionId() == KNullSessionId)
	    {
	    return KErrNotFound;
	    }
		
	// If the originating session is no longer connected then we can't return the session name.
	const TInt count = iActiveSessions.Count();
	for(TInt i=0; i<count; i++)
		{
		// Find the session which matches the originating session id
		const MASSrvSession& session = *iActiveSessions[i];
		if	(session.MASSrvSessionId() == alarm->OriginatingSessionId())
			{
			// Get the session name
			session.MASSrvSessionFullName(aFullName);
			return KErrNone;
			}
		}
	return KErrNotFound;
	}
