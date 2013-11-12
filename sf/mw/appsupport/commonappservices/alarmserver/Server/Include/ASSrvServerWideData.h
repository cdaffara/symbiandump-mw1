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

#ifndef __ASSRVSERVERWIDEDATA_H__
#define __ASSRVSERVERWIDEDATA_H__

// System Includes
#include <f32file.h>

// User Includes
#include "ASSrvAnyEventManager.h"
#include "ASSrvSessionCollection.h"

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
    #include "assrvsystemstateobserver.h"
#endif

// Classes Referenced
class CASSrvDataPool;
class CASSrvAlarmQueue;
class CASSrvAlarmStore;
class CASSrvAlarmTimer;
class CASSrvSoundSettings;
class CASSrvSoundController;
class MASSrvAnyEventManager;
class MASSrvSessionCollection;
class CASSrvNotificationCoordinator;
class CASSrvEnvironmentChangeManager;

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
    class CASSrvSystemState;
#endif


/**
 * The Alarm-Server wide data object which owns most of the worker classes. Split out
 * from the server so that each of these objects can be tested in isolation from
 * the server (i.e. the Alarm Server can be tested in a single EXE configuration).
 */
class CASSrvServerWideData : public CBase, public MASSrvAnyEventManager, public MASSrvSessionCollection
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
, public MASSrvSystemStateObserver
#endif
	{
public:										
	static CASSrvServerWideData* NewL();
	~CASSrvServerWideData();

private:									
	CASSrvServerWideData();
	void ConstructL();
	TTimeIntervalSeconds iCachedUtcOffset;
	
public:										
    // Access
	RFs& FsSession() const;
	CASSrvAlarmQueue& Queue() const;
	CASSrvSoundController& SoundController() const;
	CASSrvSoundSettings& SoundSettings() const;
	CASSrvDataPool& DataPool() const;
	CASSrvAlarmTimer& Timer() const;
	MASSrvAnyEventManager& AnyEventManager() const;
	CASSrvEnvironmentChangeManager& EnvironmentChangeManager() const;
	CASSrvNotificationCoordinator& NotificationCoordinator() const;
	MASSrvSessionCollection& SessionCollection() const;
	/**
	 * The previous UTC offset for checking whether it has changed while the Alarm
	 * Server was down.
	 */
	inline TTimeIntervalSeconds& CachedUtcOffset() { return iCachedUtcOffset; }
	
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	TBool ServerIsReadOnly();
#endif

private:									
    // from MASSrvAnyEventManager
	void MASAnyEventManagerObserverAddL(MASSrvAnyEventObserver& aObserver);
	void MASAnyEventManagerObserverRemove(const MASSrvAnyEventObserver& aObserver);
	void MASAnyEventManagerObserverNotifyChanges(TAlarmChangeEvent aEvent, TAlarmId aAlarmId);

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
private: 
    // from MASSrvSystemStateObserver
	void MHandleSystemStateChange(TState aState);
#endif
	
private:									
    // from MASSrvSessionCollection
	TASSrvSessionId MASSessionCollectionAttachL(const MASSrvSession& aSession);
	void MASSessionCollectionDetach(const MASSrvSession& aSession);
	TInt MASSessionCollectionAlarmOwner(TAlarmId aAlarmId, TDes& aFullName) const;

private:

	/**
	 * The file server session
	 */
	RFs iFsSession;
	
	/**
	 * The sound settings
	 */
	CASSrvSoundSettings* iSoundSettings;
	
	/**
	 * The alarm queue
	 */
	CASSrvAlarmQueue* iQueue;

	/**
	 * The alarm timer
	 */
	CASSrvAlarmTimer* iTimer;

	/**
	 * The sound controller
	 */
	CASSrvSoundController* iSoundCountroller;

	/**
	 * The data pool
	 */
	CASSrvDataPool* iDataPool;

	/**
	 * The environment change manager
	 */
	CASSrvEnvironmentChangeManager* iEnvironmentChangeManager;

	/**
	 * The notification co-ordinator
	 */
	CASSrvNotificationCoordinator* iNotificationCoordinator;

	/**
	 * The alarm store
	 */
	CASSrvAlarmStore* iStore;

	/**
	 * The collection of "any event observers"
	 */
	RPointerArray<MASSrvAnyEventObserver> iAnyEventObservers;

	/**
	 * The collection of sessions
	 */
	RPointerArray<const MASSrvSession> iActiveSessions;

	/**
	 * The next free session identifier
	 */
	TASSrvSessionId iNextFreeSessionId;
	
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	/**
	 * Listens to changes in system state such as shutdown.
	 **/
	CASSrvSystemState* iASSrvSystemState;
	
	/**
	 * A flag to state if the alarm server is in a read only state.
	 */
	TBool iServerIsReadOnly;
#endif
	};

#endif
