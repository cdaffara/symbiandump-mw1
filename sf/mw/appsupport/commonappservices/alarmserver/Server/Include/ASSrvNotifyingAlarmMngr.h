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
// Declares CASSrvNotifyingAlarmMngr
// 
//

/**
 @file
 @internalTechnology
*/

#ifndef __ASSRVNOTIFYINGALARMMNGR_H__
#define __ASSRVNOTIFYINGALARMMNGR_H__

#include "ASSrvTimerObserver.h"

class CASSrvNotificationCoordinator;
class CASSrvServerWideData;

/**
This case manage the list of notifying alarms in alarm server
*/
class CASSrvNotifyingAlarmMngr : public CBase, public MASSrvAlarmTimerObserver
	{
public:
	static CASSrvNotifyingAlarmMngr* NewL(CASSrvNotificationCoordinator& aParent, CASSrvServerWideData& aServerWideData);
	~CASSrvNotifyingAlarmMngr();
	
	void SetMaxNumberOfAlarms(TInt aMaxAlarms);
	TBool AmNotifyingAboutAlarm() const;
	TBool AlarmIsNotifying(TAlarmId aAlarmId) const;
	void FindAndExecuteNextAlarmAwaitingNotification(TBool aUpdate = ETrue);
	TBool RemoveNotifyingAlarm(TAlarmId aAlarmId);
	void RemoveAllNotifyingAlarms();
	TInt NumberOfNotifyingAlarm();
	void AcknowledgeAllNotifyingAlarms();

private:
	CASSrvNotifyingAlarmMngr(CASSrvNotificationCoordinator& aParent, CASSrvServerWideData& aServerWideData);
	void ConstructL();

	// from MASSrvAlarmTimerObserver
	void MATimerHandleAlarmExpired(TAlarmTimerEvent aEvent, TAlarmId aAlarmId);

	CASSrvServerWideData& ServerWideData() const;
	void SetAsNextNotifyingAlarm(TAlarmId aAlarmId);
	TBool AllowMoreNotifyingAlarms() const;
	TAlarmId YoungestNotifyingAlarmId() const;
	TBool SoleNotifyingAlarmHasSoundPaused() const;

private:	
	/** Pointer to notification coordinator */
	CASSrvNotificationCoordinator& iParent;

	/** Pointer to server wide data */
	CASSrvServerWideData& iServerWideData;
	
	/** The maximum number of alarms supported by Alert Server */
	TInt iMaxNumberOfAlarms;

	/** The list of notifying alarm ids, ordered by expiration time */
	RArray<TAlarmId> iCurrentlyNotifyingAlarmIds;
	};

#endif	//__ASSRVNOTIFYINGALARMMNGR_H__
