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
// Description: The header contains class declaration for alarm queue class used
//              for holding all the alarms in the alarm server.
//

#ifndef __ASSRVALARMQUEUE_H__
#define __ASSRVALARMQUEUE_H__

// User Includes
#include "ASSrvDefs.h"
#include "ASSrvAlarm.h"
#include "ASSrvStaticUtils.h"
#include "ASSrvTimerObserver.h"
#include "ASSrvAlarmObserver.h"
#include "ASSrvAlarmQueueObserver.h"
#include "ASSrvEnvironmentChangeObserver.h"

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
    #include <ssm/ssmadaptationcli.h>
#endif

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
    #include <ASShdAlarmCal.h>
#endif

// Classes Referenced
class MASSrvAlarmObserver;
class CASSrvServerWideData;
class CASSrvDSTChange;


/**
 * The overall container class for all alarms maintained within the Alarm Server.
 */
class CASSrvAlarmQueue :  public CBase, public MASSrvEnvironmentChangeObserver
	{
public:
	static CASSrvAlarmQueue* NewL(CASSrvServerWideData& aServerWideData);
	~CASSrvAlarmQueue();

private:									
	CASSrvAlarmQueue(CASSrvServerWideData& aServerWideData);
	void ConstructL();

private:									
    // from MASSrvEnvironmentChangeObserver
	void MEnvChangeHandleEvent(TInt aChanges, TUint aWorkdays, TBool aWorkdaysChanged);

public:
    // from MASSrvAlarmInfoProvider
	TInt ASSrvAlarmInfoCount() const;
	void ASSrvAlarmInfoAt(TInt aIndex, TASSrvAlarm& aAlarm) const;

public:										
    // Notification Management
	void NotificationPoolChangeL(MASSrvAlarmQueueObserver& aObserver);
	void NotificationPoolChangeCancel(MASSrvAlarmQueueObserver& aObserver);

public:										
    // Access - Alarm Observation
	void RequestAlarmObservationEventsL(MASSrvAlarmObserver& aObserver);
	void RequestAlarmObservationEventsCancel(MASSrvAlarmObserver& aObserver);
	void RequestAlarmObservationEventsEnabled(MASSrvAlarmObserver& aObserver, TBool aEnabled);

public:										
    // Queue Access
	TInt QueueAlarmCount() const;
	TASSrvAlarm& QueueAlarmAt(TInt aIndex);
	const TASSrvAlarm& QueueAlarmAt(TInt aIndex) const;
	TInt QueueAlarmById(TAlarmId aId, TASSrvAlarm& aAlarm) const;
	TASSrvAlarm* QueueAlarmById(TAlarmId aId);
	const TASSrvAlarm* QueueAlarmById(TAlarmId aId) const;
	TASSrvAlarm& QueueAlarmByIdL(TAlarmId aId);
	const TASSrvAlarm& QueueAlarmByIdL(TAlarmId aId) const;

public:										
    // Access
	void QueueAlarmAndAllocateIdL(TASSrvAlarm& aAlarm, TAlarmId aSpecificAlarmId = KNullAlarmId);
	void DeQueueAlarm(const TASSrvAlarm& aAlarm);
	const TASSrvAlarm* HeadAlarm() const;

#ifdef SYMBIAN_SKIPPED_CALENDAR_ALARMS	
	TAlarmId HeadCalendarAlarmId() const;
#endif
	
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	TAlarmId HeadWakeupAlarmId() const;
#endif

	TAlarmId HeadAlarmId() const;
	void RemoveAllSessionAlarmsBySessionId(TASSrvSessionId aSessionId);
	TAlarmId NextFreeAlarmId();
	TBool HaveAdditionalAlarmsToNotify();
	TInt NumberOfAlarmsPendingNotification();
	TASSrvAlarm& NextAlarmWaitingForNotification();

public:										
    // Store Framework
	void InternalizeL(RReadStream& aStream);
	void ExternalizeL(RWriteStream& aStream) const;
	
#ifdef SYMBIAN_SKIPPED_CALENDAR_ALARMS	
	void InternalizeLastAlarmedInstanceParamsL(RReadStream& aStream);
	void ExternalizeLastAlarmedInstanceParamsL(RWriteStream& aStream) const;
#endif	
	
public:										
    // Support for store framework

	/**
	 * The next/current Alarm Store operation
	 */
	enum TStoreOperation
		{
		/**
		 * No-op state
		 */
		EStoreIdle = 0,

		/**
		 * Attempt to Internalize after system start
		 */
		EStoreInternalizeStartup,

		/**
		 * Attempt to Internalize after a backup restore
		 */
		EStoreInternalizeRestore,

		/**
		 * Externalize (e.g. Alarms have changed, Alarm Sound Settings have changed
		 * or Alarm Server shutdown)
		 */
		EStoreExternalize,

		/**
		 * Backup - Read access to AlarmServer.ini file requested for backup
		 * (by backup/synchronisation app.)
		 */
		EStoreBackup,

		/**
		 * Restore - Exclusive Write access to AlarmServer.ini file requested for Restore
		 * (by backup/synchronisation app.)
		 */
		EStoreRestore
		};

	TInt StartAlarmStoreOperation(TStoreOperation aStoreOperation);
	void EndAlarmStoreOperation(TInt aError);

	/**
	 * Check that Alarm and Alarm Data is writable.
	 */
	void CheckAlarmQueueWritableL();

public:										
    // Event Handlers - called by TASSrvAlarm
	void HandleAlarmStatusChanged(TAlarmId aAlarmThatChangedStatus, TAlarmStatus aOldStatus);
	void HandleAlarmStateChanged(TAlarmId aAlarmThatChangedState, TAlarmState aOldState);
	void HandleAlarmCharacteristicsChanged(TAlarmId aAlarmThatChangedCharacteristics, TAlarmCharacteristicsFlags aOldCharacteristics);

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	void HandleWakeupChanged(TAlarmId aAlarmThatChangedWakeup);
#endif

#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
	void HandleAlarmDaysChanged(TAlarmId aAlarmThatChangedDays);
#endif

	void HandleAlarmDataChanged(TAlarmId aAlarmThatChangedData);

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
public:

	void HandleNextDSTChangeEventL();
	void UpdateRTC();
#endif
	
	
private:									
    // Internal
	inline CASSrvServerWideData& ServerData() const { return iServerWideData; }
	TBool AlarmIdIsInUse(TAlarmId aAlarmId) const;
	void RemoveDeadAlarms();
	void NotifyAlarmObserverEvent(MASSrvAlarmObserver::TObserverEvent aEvent, const TASSrvAlarm& aAlarm, TInt aEventSpecificData);
	void NotifyEvent(MASSrvAlarmQueueObserver::TASSrvAlarmQueueEvent aEvent, TAlarmId aAlarmId);
	void ReplaceQueueWithInternalizedQueue();
	void ApplyInternalizedData(TBool aUseNewData);
	void UpdateFloatingDueTimes();
	void PublishSkippedAlarm(TBool aSkippedCalAlarm);
	
#ifdef SYMBIAN_SKIPPED_CALENDAR_ALARMS	
	void PublishAlarmedInstanceParams(const TTime& aFirstSkippedAlarmLocalTime, TBool aSystemTimeChange);
	void PublishAlarmedInstanceParams(const TASShdAlarmedInstanceParams& aAlarmedInstanceParams);
#endif
	
private:									
    // Internal Classes
	/**
	 * A mapping between alarm observers and enabled states. Allows an observer
	 * to temporarily disable notification events without being removed from
	 * the observer array. This prevents OOM problems since observers don't
	 * have to run the risk of a leave (due to no memory) when re-requesting
	 * observations.
	 */
	class TASSrvAlarmObserverMapplet
		{
	public:
		inline TASSrvAlarmObserverMapplet(MASSrvAlarmObserver& aObserver) : iEnabled(ETrue), iObserver(&aObserver) { }
		/**
		 * Access the observer.
		 */
		
		inline MASSrvAlarmObserver& Observer() { return *iObserver; }
		/**
		 * Does this observer want to receive notification events?
		 */
		
		inline TBool IsEnabled() const { return iEnabled; }
		/**
		 * Change the enabled status of the observer.
		 */
		
		inline void SetEnabled(TBool aEnabled) { iEnabled = aEnabled; }

	private:
		/**
		 * Wehther or not the observer wants to receive notifications.
		 */
		TBool iEnabled;

		/**
		 * The observer itself.
		 */
		MASSrvAlarmObserver* iObserver;
		};

private:									
	/**
	 * The server wide data.
	 */
	CASSrvServerWideData& iServerWideData;
	/**
	 * The next free alarm Id.
	 */
	TAlarmId iNextFreeAlarmId;
	/**
	 * The array of alarm observers.
	 */
	RArray<TASSrvAlarmObserverMapplet> iAlarmObservers;
	/**
	 * The list of all alarms within the server.
	 */
	RPointerArray<TASSrvAlarm> iAlarms;
	/**
	 * A list of observes who are interested in queue change notifications.
	 */
	RPointerArray<MASSrvAlarmQueueObserver>	iNotificationList;

	/**
	 * Type of Alarm Store operation being performed
	 */
	TStoreOperation iStoreOperation;

	/**
	 * Internalize buffers for data. used/discarded by ApplyInternalizedData() method
	 */
	TAlarmId iInternalizeNextFreeAlarmId;
	RPointerArray<TASSrvAlarm> iInternalizeAlarmQueue;
	
	/**
	 * The previous UTC offset for checking whether it has changed while the Alarm
	 * Server was down.
	 */
	TTimeIntervalSeconds iPreviousUtcOffset;
	
	/**
	 * Details about the next DST change.
	 */
	CASSrvDSTChange* iASSrvDSTChange;

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	/**
	 * Handle to the  real-time-clock so that we can set wakeup alarms.
	 */
	RSsmRtcAdaptation iSsmRtcAdaptation;
#endif
	
#ifdef SYMBIAN_SKIPPED_CALENDAR_ALARMS	
	/**
	 * Stores the last published value of the skipped alarm data.
	 */
	TASShdAlarmedInstanceParams iLastAlarmedInstanceParams;
#endif	
	};


#endif
