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

#ifndef __ASSRVSESSION_H__
#define __ASSRVSESSION_H__

// System includes
#include <e32base.h>
#include <babitflags.h>

// User includes
#include "ASSrvAlarm.h"
#include "ASSrvAnyEventObserver.h"
#include "ASSrvSessionCollection.h"
#include "ASSrvAlarmQueueObserver.h"

// Constants
const TInt KAlarmServerMaxNumberOfChangeEventsToBuffer = 10;

// Classes referenced
class CASSrvServerWideData;
class CASSrvSessionEngine;

// Enumerations


//
// ----> CASSrvSession (header)
//
/**
 * This class acts as a simple wrapper around the IPC client-server framework,
 * and drives a real underlying engine, which is actually responsible for all the
 * server-wide object manipulation. This allows the server functionality to be
 * tested in isolation from the client API, providing better test coverage.
 */
class CASSrvSession : public CSession2, public MASSrvAnyEventObserver, public MASSrvSession
	{
//
public:										// STATIC CONSTRUCT / DESTRUCT
//
	static CASSrvSession*					NewL(CASSrvServerWideData& aServerWideData);
	~CASSrvSession();

//
private:									// INTERNAL CONSTRUCT
//
	CASSrvSession(CASSrvServerWideData& aServerWideData);
	void									ConstructL();

//
public:										// CALLED FROM CASSrvServer dtor
//

	void									HandleServerDestruction();

//
private:									// FROM MASSrvAnyEventObserver
//

	void									MASSrvAnyEventHandleChange(TAlarmChangeEvent aEvent, TAlarmId aAlarmId);

//
public:										// FROM MASSrvSession
//

	TASSrvSessionId							MASSrvSessionId() const;

	void									MASSrvSessionFullName(TDes& aDes) const;

//
private:									// FROM CSession2
//
	void									ServiceL(const RMessage2& aMessage);

//
private:									// INTERNAL
//
	TBool									DoServiceL();

//
private:									// CMD - ALARM SPECIFIC FUNCTIONALITY
//
	TBool									CmdAlarmAddL();
	TBool									CmdAlarmAddWithNotificationL();
	TBool									CmdAlarmNotificationCancelAndDeQueueL();
	TBool									CmdGetAlarmDetailsL();
	TBool									CmdAlarmDeleteL();
	TBool									CmdGetAlarmCategoryL();
	TBool									CmdGetAlarmOwnerL();
	TBool									CmdSetAlarmStatusL();
	TBool                                   CmdSetAlarmStatusForCalendarFileL();
	TBool									CmdGetAlarmStatusL();
	TBool									CmdSetAlarmDayOrTimedL();
	TBool									CmdGetAlarmDayOrTimedL();
	TBool									CmdGetAlarmCharacteristicsL();
	TBool									CmdSetAlarmCharacteristicsL();
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	TBool									CmdSetWakeupL();
#endif
#ifdef SYMBIAN_ALARM_REPEAT_EXTENSIONS
	TBool									CmdGetAlarmDaysL();
	TBool									CmdSetAlarmDaysL();	
	TBool 									CmdSetContinuousL();
	TBool									CmdContinuousL();
#endif
	TBool									CmdSetClientDataL();
	TBool									CmdOrphanAlarmL();


//
private:									// CMD - ALARM DATA FUNCTIONALITY
//
	TBool									CmdAlarmDataAttachL();
	TBool									CmdAlarmDataDetachL();
	TBool									CmdAlarmDataSizeL();
	TBool									CmdGetAlarmDataL();

//
private:									// CMD - CATEGORY-SPECIFIC FUNCTIONALITY
//
	TBool									CmdSetAlarmStatusByCategoryL();
	TBool									CmdGetAlarmCountForCategoryL();
	TBool									CmdAlarmDeleteAllByCategoryL();
	TBool                                   CmdAlarmDeleteByCalendarFileL();
	TBool									CmdAlarmDeleteByCategoryL();
 	TBool									CmdGetAvailableCategoryListL();
	TBool									CmdGetAlarmIdListForCategoryL();

//
private:									// CMD - MISC FUNCTIONALITY
//
	TBool									CmdAlarmCountByStateL();
	TBool									CmdGetAlarmIdListByStateL();
	TBool									CmdGetAlarmIdListL();
	TBool									CmdGetNextDueAlarmIdL();
	TBool									CmdNumberOfActiveAlarmsInQueueL();

//
private:									// CMD - SOUND CONTROL
//
	TBool									CmdSetAlarmSoundStateL();
	TBool									CmdGetAlarmSoundStateL();
	TBool									CmdSetAlarmSoundsSilentUntilL();
	TBool									CmdSetAlarmSoundsSilentForL();
	TBool									CmdGetAlarmSoundsSilentUntilL();
	TBool									CmdCancelAlarmSilenceL();
	TBool									CmdGetAlarmSoundsTemporarilySilencedL();
	TBool									CmdSetAlarmPlayIntervalsL();
	TBool									CmdGetAlarmPlayIntervalsL();

//
private:									// CMD - CHANGE NOTIFICATION
//
	TBool									CmdNotifyChangeL();
	TBool									CmdNotifyChangeCancelL();

//
private:									// CMD - DEBUG ONLY
//
	TBool									CmdDbgShutDownServerL();
	TBool									CmdDbgFailAllocL();
	TBool									CmdDbgPreventUserNotifyL();
	TBool									CmdDbgSnoozeAlarmL();
	TBool 									CmdDbgSetEnvChgHandling();

//
private:									// CMD - FROM MASShdAlarmInfoProvider
//
	TBool									CmdInfoAlarmCountL();
	TBool									CmdInfoAlarmByIndexL();

//
private:									// CMD - MISC
//
	TBool									CmdFetchTransferBufferL();

//
private:									// MISCELLANEOUS
//

	void									RequestExpiryNotificationL(TASSrvAlarm& aAlarm);
	void									AlarmDataAttachL(TASSrvAlarm& aAlarm);


//
private:									// INTERNAL FLAGS
//

	/**
	 * Flags for this session
	 */
	enum TAlarmSessionFlags
		{
		/**
		 * Indicates that the client has an outstanding notification request
		 */
		EAlarmSessionFlagsNotifyPending		= 0
		};

//
private:									// INTERNAL METHODS
//

	/**
	 * Access the server wide data
	 */
	inline CASSrvServerWideData&			ServerData() const { return iServerWideData; }

	/**
	 * Access the transfer buffer
	 */
	inline CBufBase&						TransferBuffer() const { return *iTransferBuffer; }

	void									StreamAlarmIdsToTransferBufferL(RArray<TAlarmId>& aArray);

	void									CompleteChangeNotificationMessage(TInt aCompletionCode, TAlarmId aAlarmId);

	inline const RMessage2&					Message() const { return *iMessage; }
	
	void 									ConvertFromUtcToLocal(TASShdAlarm& aAlarm) const;
	void 									ConvertFromLocalToUtc(TASShdAlarm& aAlarm) const;

//
private:									// INTERNAL CLASSES
//

	//
	// ----> TASSrvBufferredEvent (header)
	//
	/**
	 * A collection class which associates an Alarm Id with a specific event.
	 * This class is used to group notification events.
	 */
	class TASSrvBufferredEvent
		{
	//
	public:									// CONSTRUCT
	//
		inline TASSrvBufferredEvent(TInt aCode, TAlarmId aId) : iCode(aCode), iAlarmId(aId) { }
	
	//
	public:									// ACCESS
	//
		inline TInt							Code() const { return iCode; }
		inline TAlarmId						AlarmId() const { return iAlarmId; }

	//
	private:								// MEMBER DATA
	//
		TInt								iCode;
		TAlarmId							iAlarmId;
		};

//
private:									// MEMBER DATA
//

	/**
	 * The server wide data handle
	 */
	CASSrvServerWideData&					iServerWideData;

	/**
	 * General flags used by this class
	 */
	TBitFlags								iFlags;

	/**
	 * For outstanding change notification events. This is NOT used for
	 * specific alarm expiry notifications.
	 */
	RMessage2								iAlarmQueueChangeNotificationMessage;

	/**
	 * A change event buffer - so that clients don't miss events. 
	 */
	RArray<TASSrvBufferredEvent>			iChangeEventBuffer;

	/**
	 * A temporary heap-based alarm object, which is written to by the client-side
	 * when data needs to be passed from client to server (or vice-versa). Saves
	 * creating lots of temporary TASSrvAlarm objects on the stack.
	 */
	TASSrvAlarm							iAlarmSink;

	/**
	 * Used to transfer collections of objects between server and client.
	 */
	CBufBase*								iTransferBuffer;

	/**
	 * Manipulates the actual alarm server objects
	 */
	CASSrvSessionEngine*					iSessionEngine;

	const RMessage2*						iMessage;
	};

#endif
