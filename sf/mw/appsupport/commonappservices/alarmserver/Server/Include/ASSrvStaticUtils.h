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

#ifndef __ASSRVSTATICUTILS_H__
#define __ASSRVSTATICUTILS_H__

// System includes
#include <e32base.h>
#include <s32strm.h>
#include <f32file.h>

// User includes
#include <asshddefs.h>
#include "ASSrvAlarmSoundDetails.h"

// Classes referenced
class TASSrvAlarm;
class TASShdAlarm;

//
// ----> ASSrvStaticUtils (header)
//
/**
 * Global utility functions
 */
class ASSrvStaticUtils
	{
//
public:										// PANIC/FAULT ENUMERATIONS
//

	/**
	 * For DEBUG-ONLY panics
	 */
	enum TASSrvFault
		{
		/**
		 * A default section of a switch statement relating to alarm
		 * states has been hit
		 */
		EASSrvFaultAlarmStateNotHandled		= 0,

		/**
		 * Timer event not handled in switch statement
		 */
		EASSrvFaultTimerEventNotHandled		= 1,

		/**
		 * The alarm data for the specified alarm couldn't be
		 * found, when it should be available
		 */
		EASSrvFaultAlarmDataEntryNotFound	= 2,

		/**
		 * There wasn't any alarm data when there should have been
		 */
		EASSrvFaultNoData					= 3,

		/**
		 * Some kind of timing error took place which was unexpected
		 */
		EASSrvFaultPhantomErrorReported		= 4,

		/**
		 * An alarm status wasn't handled in a switch statement
		 */
		EASSrvFaultAlarmStatusNotHandled	= 5,

		/**
		 * An iterator has attempted to attach itself to itself
		 */
		EASSrvFaultCyclicIteratorAttach		= 6,

		/**
		 * The alarm wasn't found when it should have been
		 */
		EASSrvFaultAlarmNotFound			= 7,

		/**
		 * The index specified within the sound intervals
		 * list isn't valid
		 */
		EASSrvFaultBadSoundIntervalIndex	= 8,

		/**
		 * A facade interface wasn't reimplemented
		 */
		EASSrvFaultFacadeInterfaceError		= 9,

		/**
		 * An invalid alarm alert server response was received
		 */
		EASSrvFaultInvalidAlarmAlertServerResponse	= 10,

		/**
		 * The alarm alert server response handler already
		 * has an outstanding request with the alarm alert server.
		 */
		EASSrvFaultAlarmAlertServerResponseReaderAlreadyActive	= 11,

		/**
		 * A fault condition occurred within the alarm alert
		 * server client code
		 */
		EASSrvFaultAlarmAlertServerFault	= 12,

		/**
		 * The flags passed to/from the alarm alert server
		 * are invalid
		 */
		EASSrvFaultAlarmAlertSetFlagsWithInappropriateValue = 13,

		/**
		 * Alarm Queue StartAlarmStoreOperation called with invalid store operation
		 */
		EASSrvFaultStartInvalidAlarmStoreOperation = 14,

		/**
		 * Alarm Queue EndAlarmStoreOperation called with invalid store operation
		 */
		EASSrvFaultEndInvalidAlarmStoreOperation = 15,

		/**
		 * An alarm has sound paused while there is another alarm in notifying state.
		 */
		EASSrvFaultInvalidSoundPauseAlarm = 16,

		/**
		 * Alarm server tries to notify more than max number of allowed alarm.
		 */
		EASSrvFaultNotifyAlarmExceedMax = 17,

		/**
		 * Alarm server has no notifying alarms.
		 */
		EASSrvFaultNoNotifyingAlarms = 18,


		/**
		 * Marker
		 */
		EASSrvFaultLast
		};

	/**
	 * For ASSERT-ALWAYS panics
	 */
	enum TASSrvPanic
		{
		/**
		 * A next alarm for a given iterator
		 * was expected but not available
		 */
		EASSrvPanicNoNextAlarmForIterator	= 0,

		/**
		 * The alarm specified by the given id
		 * doesn't exist
		 */
		EASSrvPanicNoAlarmForId				= 1,

		/**
		 * The specified queue observer could not be
		 * located
		 */
		EASSrvPanicNoMatchingObserver		= 2,

		/**
		 * A iterator chaining function was executed
		 * but no attached iterator was found
		 */
		EASSrvPanicNoAttachedIterator		= 3,

		/**
		 * It wasn't possible to update the status
		 * of the given alarm
		 */
		EASSrvPanicCannotSetAlarmStatus		= 4,

		/**
		 * An invalid or unexpected repeat definition
		 * was encountered
		 */
		EASSrvPanicInvalidAlarmRepeat		= 5,

		/**
		 * The specified alarm wasn't removed from
		 * the alarm server queue
		 */
		EASSrvPanicAlarmNotDeQueued			= 6,

		/**
		 * It wasn't possible to start the alarm
		 * server's active scheduler
		 */
		EASSrvPanicSchedulerNotStarted		= 7,

		/**
		 * An invalid backup operation was initiated
		 */
		EASSrvPanicInvalidBackupOperation	= 8,

		/**
		 * A request to start notifications about
		 * a given alarm do not match that which is
		 * expected.
		 */
		EASSrvPanicNotifyingAboutWrongAlarm	= 9,

		/**
		 * An alarm iterator function has
		 * encountered a null alarm id
		 */
		EASSrvPanicIteratorAlarmIdNull		= 10,

		/**
		 * An alarm has attempted to notify itself
		 * when it is already in the notifying state.
		 */
		EASSrvPanicAlarmRepeatedlyNotifying	= 11,

		/**
		 * The wrong alarm id has been promoted to the
		 * notifying state
		 */
		EASSrvPanicNotifyingAlarmIsNotAlertingAlarm = 12,

		/**
		 * An invalid alarm state was encountered
		 */
		EASSrvPanicInvalidAlarmState		= 13,

		/**
		 * It wasn't possible to complete the two-phase idle
		 * construction of the notification co-ordinator
		 */
//		EASSrvPanicNotificationCoordinatorConstructionIncomplete = 14, // not used any more

		/**
		 * An invalid observer event was encountered
		 */
		EASSrvPanicInvalidTObserverEvent	= 15,

		/**
		 * An unexpected null alarm id was encountered
		 */
		EASSrvPanicQueueAlarmIdNull			= 16,

		/**
		 * The current alarm id managed by the notification
		 * co-ordinator does not match that of the currently
		 * notifying alarm
		 */
		EASSrvPanicCurrentAlarmNotNotifyingAlarm = 17,

		/**
		 * Two or more alarms are trying to notify at
		 * the same time
		 */
		EASSrvPanicMoreThanOneAlarmNotifying	= 18,

		/**
		 * An unexpected or invalid TSoundSettings event
		 * was encountered
		 */
		EASSrvPanicInvalidTSoundSettingsEvent	= 19,

		/**
		 * A request has been made to delete an alarm
		 * but its id doesn't match the expected value
		 */
		EASSrvPanicAttemptingToDequeWrongAlarm	= 20,

		/**
		 * An Internalize() request has been made without 
		 * correctly setting the type of request.
		 */
		EASSrvPanicInternalizeTypeInvalid = 21,

		/**
		 * OutOfMemory
		 */
		EASSrvPanicOutOfMemory= 22,

		/**
		 * Alarm server sound controller received an unexpected event while disabled.
		 */
		EASSrvPanicUnexpectedEventSoundControllerDisabled = 23,


		/**
		 * Marker
		 */
		EASSrvPanicLast
		};

//
public:										// STATIC UTILITY FUNCTIONS
//

	static TInt								ValidateAlarm(TASSrvAlarm& aAlarm);

	static TInt								ValidateSoundPlayIntervals(RArray<TASSrvAlarmSoundDetails>& aIntervals);

	static void								Fault(TASSrvFault aFault);

	static void								Panic(TASSrvPanic aPanic);

	static TInt								CompareCategories(const TAlarmCategory& aCategory1, const TAlarmCategory& aCategory2);

	static TInt								CompareAlarmIds(const TAlarmId& aId1, const TAlarmId& aId2);

	static TInt								CompareAlarms(const TASSrvAlarm& aAlarm1, const TASSrvAlarm& aAlarm2);

	static TBool							CompareAlarmsExact(const TASSrvAlarm& aAlarm1, const TASSrvAlarm& aAlarm2);

	static TVersion							ASVersion();
   
	static const TDesC&						ASName();

	static void								RoundTimeDownToTheMinute(TTime& aTime);

	static void								PanicClient(const RMessage2& aMessage, TAlarmServerInitiatedClientPanic aPanic);

	static TTime							UtcTimeNow();

	static TTime							LocalTimeNow();

	static void								TodayAtTheSpecifiedTime(const TTime& aTime, TTime& aReturn);

	static RArray<TASSrvAlarmSoundDetails>*	InternalizeSoundPlayIntervalsLC(RReadStream& aStream);

	static void								GetPrivateDirL(RFs& aFsSession, TDes& aDirName);

	static void								GetServerPathL(RFs& aFsSession, TDes& aFileName);

	static void								DaysUntilNextActiveAlarmDay(TTimeIntervalDays& aDaysFromNow, TTimeIntervalDays& aRollOverDays, TDay aCurrentDayInWeek, TUint aActiveDays);

	static void								CalculateNext24HoursRepeat(TASShdAlarm& aAlarm, TTimeIntervalSeconds aAllowableWindow = 0);

	static void								CleanupCloseDeleteAlarmIdArray(TAny* aArray);

	static void								CleanupCloseDeleteCategoryArray(TAny* aArray);
	
	static void								CleanupCloseDeleteSoundDetailsArray(TAny* aArray);

//
private:									// INTERNAL FUNCTIONS
//

	static TInt								CompareSoundPlayIntervalStartTimes(const TASSrvAlarmSoundDetails& aAlarmSound1, const TASSrvAlarmSoundDetails& aAlarmSound2);

	static TBool							SoundPlayIntervalsOverlap(const TASSrvAlarmSoundDetails& aDetails1, const TASSrvAlarmSoundDetails& aDetails2);

	};
	
/**
   * Template classes to call ResetAndDestroy on array classes
   * automatically, when ::PopAndDestroy'ing from the cleanup stack.
   */
  template <class T>
  class CleanupResetAndDestroy
  	{
  public:
  	inline static void PushL(T& aRef);
  private:
  	static void ResetAndDestroy(TAny *aPtr);
  	};
  template <class T>
  inline void CleanupResetAndDestroyL(T& aRef);
  
  // Template class CleanupClose
  template <class T>
  inline void CleanupResetAndDestroy<T>::PushL(T& aRef)
  	{CleanupStack::PushL(TCleanupItem(&ResetAndDestroy, &aRef));}
  template <class T>
  void CleanupResetAndDestroy<T>::ResetAndDestroy(TAny *aPtr)
  	{static_cast<T*>(aPtr)->ResetAndDestroy();}
  template <class T>
  inline void CleanupResetAndDestroyPushL(T& aRef)
  	{CleanupResetAndDestroy<T>::PushL(aRef);}


#endif
