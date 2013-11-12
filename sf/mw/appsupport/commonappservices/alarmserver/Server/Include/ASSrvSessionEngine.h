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

#ifndef __ASSRVSESSIONENGINE_H__
#define __ASSRVSESSIONENGINE_H__

// System includes
#include <e32base.h>

// User includes
#include "ASSrvDefs.h"
#include "ASSrvAlarm.h"
#include "ASSrvAnyEventObserver.h"
#include "ASSrvSessionCollection.h"

// Classes referenced
class RASSrvIteratorBase;
class CASSrvServerWideData;

/**
Copy of System-Wide Unique Agenda Entry Reference defined in agmalarm.h
*/
struct TAgnAlarmInfo
    {
    TUid iAlarmCategory;
    TUint32 iEntryId;
    TTime iInstanceTime;
    TUint32 iTimeMode;
    TFileName iFileName;
    TBuf8<255> iGlobalIdentifier;
    TTime iRecurrenceId;
    TUint32 iAgnUniqueId;  
    }; 


//
// ----> CASSrvSessionEngine (header)
//
/**
 * The underlying engine used by each session. Manipulates the server-side objects.
 */
class CASSrvSessionEngine : public CBase, public MASSrvAnyEventObserver, public MASSrvSession
	{
//
public:										// STATIC CONSTRUCT / DESTRUCT
//
	static CASSrvSessionEngine*				NewL(CASSrvServerWideData& aServerWideData, MASSrvAnyEventObserver& aChangeObserver, MASSrvSession& aSession);
	~CASSrvSessionEngine();

//
private:									// INTERNAL CONSTRUCT
//
	CASSrvSessionEngine(CASSrvServerWideData& aServerWideData, MASSrvAnyEventObserver& aChangeObserver, MASSrvSession& aSession);
	void									ConstructL();

//
public:										// FROM MASSrvAnyEventObserver
//

	void									MASSrvAnyEventHandleChange(TAlarmChangeEvent aEvent, TAlarmId aAlarmId);

//
public:										// FROM MASSrvSession
//

	TASSrvSessionId							MASSrvSessionId() const;

	void									MASSrvSessionFullName(TDes& aDes) const;

//
public:										// ENGINE API
//

	void									AlarmAddL(TASSrvAlarm& aAlarm, TAlarmId aSpecificAlarmId = KNullAlarmId);

	void									AlarmDetailsL(TAlarmId aAlarmId, TASSrvAlarm& aAlarm) const;

	void									AlarmDeleteL(TAlarmId aAlarmId);

	TAlarmCategory							AlarmCategoryL(TAlarmId aAlarmId) const;

	void									SetAlarmStatusL(TAlarmId aAlarmId, TAlarmStatus aStatus);
	
	void                                    SetAlarmStatusForCalendarFileL(const TDesC& aCalendarFileName, TAlarmStatus aStatus);

	TAlarmStatus							AlarmStatusL(TAlarmId aAlarmId) const;

	void									SetAlarmDayOrTimedL(TAlarmId aAlarmId, TAlarmDayOrTimed aDayOrTimed);

	TAlarmDayOrTimed						AlarmDayOrTimedL(TAlarmId aAlarmId) const;

	void									SetAlarmCharacteristicsL(TAlarmId aAlarmId, TAlarmCharacteristicsFlags aCharacteristics);

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	void									SetWakeupL(TAlarmId aAlarmId, TBool aEnable);
#endif
	
#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
	void									SetAlarmDaysL(TAlarmId aAlarmId, TUint8 aAlarmDays);
	TUint8									AlarmDaysL(TAlarmId aAlarmId) const;
	void									SetContinuousL(TAlarmId aAlarmId, TBool aContinuous);
	TBool									ContinuousL(TAlarmId aAlarmId) const;
#endif
	
	void									SetAlarmOrphanedL(TAlarmId aAlarmId);

	TAlarmCharacteristicsFlags				AlarmCharacteristicsL(TAlarmId aAlarmId) const;

	void									SetAlarmStatusByCategoryL(TAlarmCategory aCategory, TAlarmStatus aStatus);

	TInt									AlarmCountByCategory(TAlarmCategory aCategory) const;

	TInt									AlarmCountByState(TAlarmState aState) const;

	void									DeleteAllAlarmsByCategoryL(TAlarmCategory aCategory, TBool aRestrictToOrphanedAlarms, TDeleteType aWhatToDelete=EAllAlarms);
 
	void                                    DeleteAllAlarmsByCalendarFileL(const TDesC& aCalendarFileName, TDeleteType aWhatToDelete);
	
	RArray<TAlarmCategory>*					AlarmCategoryListLC() const;

	RArray<TAlarmId>*						AlarmIdListByCategoryLC(TAlarmCategory aCategory) const;

	RArray<TAlarmId>*						AlarmIdListByStateLC(TAlarmState aState) const;

	RArray<TAlarmId>*						AlarmIdListLC() const;

	TInt									NumberOfActiveAlarmsInQueue() const;

	void									AlarmDataAttachL(TAlarmId aAlarmId, HBufC8* aData);

	void									AlarmDataDetachL(TAlarmId aAlarmId);


//
private:									// INTERNAL METHODS
//

	/**
	 * Access the server wide data
	 */
	inline CASSrvServerWideData&			ServerData() const { return iServerWideData; }

	RArray<TAlarmId>*						AlarmIdListFromIteratorLC(RASSrvIteratorBase& aIterator) const;

//
private:									// MEMBER DATA
//

	/**
	 * The server wide data
	 */
	CASSrvServerWideData&					iServerWideData;

	/**
	 * The handle to the real any event observer
	 */
	MASSrvAnyEventObserver&					iChangeObserver;

	/**
	 * The handle to the real session
	 */
	MASSrvSession&							iSession;

	/**
	 * The session id assocaited with this object
	 */
	TASSrvSessionId							iSessionId;
	};

#endif
