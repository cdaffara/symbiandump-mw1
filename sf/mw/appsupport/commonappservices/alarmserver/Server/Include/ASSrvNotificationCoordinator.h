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

#ifndef __ASSRVNOTIFICATIONCOORDINATOR_H__
#define __ASSRVNOTIFICATIONCOORDINATOR_H__

// System includes
#include <e32base.h>

// User includes
#include "ASAltClientSession.h"
#include "ASAltAlarmAlertObserver.h"
//
#include "ASSrvAlarmObserver.h"
#include "ASSrvAlarmQueueObserver.h"
#include "ASSrvSoundControllerObserver.h"

// Classes referenced
class CASSrvServerWideData;
class CASAltEventHandler;
class CASAltServerDeathObserver;
class CASAltRequestSetAlarm;
class TASShdAlarm;

class CASSrvNotifyingAlarmMngr;

//
// ----> CASSrvNotificationCoordinator (header)
//
/**
 * Observes the Alarm Alert Server and handles its requests. Acts as a co-ordinator
 * between alarm display (Alarm Alert Server functionality), and sound control
 * (Alarm Server) functionality.
 */
class CASSrvNotificationCoordinator : public CActive, public MASAltAlarmAlertObserver, public MASSrvSoundControllerObserver, public MASSrvAlarmObserver, public MASSrvAlarmQueueObserver
	{
//
public:										// STATIC CONSTRUCT / DESTRUCT
//
	static CASSrvNotificationCoordinator*	NewL(CASSrvServerWideData& aServerWideData);
	~CASSrvNotificationCoordinator();
	
	void									CompleteUpdateAlarmAlertServer(const TASShdAlarm* aAlarm, TBool aInternalizing = EFalse);

//
private:									// INTERNAL CONSTRUCT
//
	CASSrvNotificationCoordinator(CASSrvServerWideData& aServerWideData);
	void									ConstructL();


//
private:									// FROM MASAltAlarmAlertObserver
//


	void									ASAltQuietPeriodUntil(const TTime& aTimeWhenSoundsShouldResume);

	void									ASAltSilenceCurrentAlarm(TAlarmId aAlarmId);

	void									ASAltAcknowledgeAlarm(TAcknowledgeType aType, TAlarmId aAlarmId);

	TInt									ASAltSnoozeAlarmUntil(const TTime& aTimeWhenAlarmToReAwaken, TAlarmId aAlarmId);

	void									ASAltPauseSoundUntil(const TTime& aTimeWhenSoundSoundResume, TAlarmId aAlarmId);

//
private:									// FROM MASSrvSoundControllerObserver
//

	void									MASHandleSoundControllerEvent(TSoundControllerEvent aEvent, TAlarmId aAlarmId);

//
private:									// FROM MASSrvAlarmObserver
//

	void									MASSrvAlarmObsHandleEvent(TObserverEvent aEvent, const TASSrvAlarm& aAlarm, TInt aEventSpecificData);

//
private:									// FROM MASSrvAlarmQueueObserver
//

	void									MAlarmQueueObserverHandleEvent(TASSrvAlarmQueueEvent aEvent, TAlarmId aAlarmId);

//
public:										// FROM CActive
//

	void									RunL();

	void									DoCancel();

//
public:										// MISC METHODS
//
#ifdef _DEBUG
	void									__DbgPreventUserNotify(TBool aAllowNotification);
#endif

	TBool									SetAlarmInProgress();

	void									UpdateAlarmAlertServer(TAlarmId aAlarmId);

#ifdef _DEBUG
	TBool									PreventUserNotification();
#endif // _DEBUG


//
private:									// INTERNAL
//

	/**
	 * Access the server wide data
	 */
	inline CASSrvServerWideData&			ServerWideData() const { return iServerWideData; }

	/**
	 * Access the Alarm Alert Server client session
	 */
	inline const RASAltClientSession&		AlarmAlert() const { return iAlarmAlertSession; }


	TInt									FinalizeConstruction();

	void									DeleteNotifyingAlarm(TAlarmId aAlarmId, TBool aUpdateAlertServer=ETrue);

	void									DeleteAllNotifyingAlarms();

//
private:									// INTERNAL EVENT HANDLERS
//

	void									HandleAlarmStateChange(const TASSrvAlarm& aAlarm, TAlarmState aOldState);

	void									HandleAlarmStatusChange(const TASSrvAlarm& aAlarm, TAlarmStatus aOldStatus);

//
private:									// MEMBER DATA
//

	/**
	 * A handle to the server wide data.
	 */
	CASSrvServerWideData&					iServerWideData;

	/**
	 * The connection to the Alarm Alert Server
	 */
	RASAltClientSession						iAlarmAlertSession;

	/**
	 * The object which receives and interprets events from the 
	 * Alarm Alert Server
	 */
	CASAltEventHandler*						iAlarmAlertEventHandler;
	
	/**
	 * Allows asynchronous SetAlarm request 
	 */
	CASAltRequestSetAlarm*					iRequestSetAlarm;

	/**
	 * The object which watches to see if the Alarm Alert Server dies
	 */
	CASAltServerDeathObserver*				iAlarmAlertDeathObserver;


	/**
	 * Notifying alarm manager
	 */
	CASSrvNotifyingAlarmMngr*				iNotifyingAlarmMngr;

#ifdef _DEBUG
	/**
	 * Debug only - prevent the Alarm Alert Server from being notified
	 * when an alarm expires
	 */
	TBool									iPreventUserNotification;
#endif
	};

#endif
