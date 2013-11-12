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

#include "ASSrvStaticUtils.h"

// System includes

// User includes
#include "ASShdOpCodes.h"
//
#include "ASSrvAlarm.h"

// Type definitions

// Constants
const TInt KReturnFirstObjectLessThanSecond = -1;
const TInt KReturnFirstObjectGreaterThanSecond = 1;
const TInt KReturnObjectsAreEqual = 0;
const TInt KNumberOfSecondsInMinute = 60;

// Enumerations

// Classes referenced


//
// ----> ASSrvStaticUtils (source)
//

//*************************************************************************************
/**
 * Validate the alarm. Checks the characteristics applied to the alarm
 * are valid.
 */
TInt ASSrvStaticUtils::ValidateAlarm(TASSrvAlarm& aAlarm)
	{
	if	(aAlarm.Message().Length() < 0 || aAlarm.Message().Length() > KMaxAlarmMessageLength)
		return KErrArgument;
	else if (aAlarm.SoundName().Length() < 0 || aAlarm.SoundName().Length() > KMaxAlarmSoundNameLength)
		return KErrArgument;

	// Check repeat flags in conjunction with session alarms
	if	(aAlarm.Characteristics().IsSet(EAlarmCharacteristicsSessionSpecific) && aAlarm.RepeatDefinition() != EAlarmRepeatDefintionRepeatOnce)
		return KErrArgument;

	const TTimeIntervalSeconds KAllowNoWindow = 0;
	const TBool KAllowBlanketInThePastOnceOnlyAlarms = ETrue;

	// The only valid return types are KErrNone (everything is fine) or 
	// KErrLocked (repeat workdays alarm, but no workdays are defined).
	const TInt error = aAlarm.ValidateAndEnable(KAllowNoWindow, KAllowBlanketInThePastOnceOnlyAlarms);
	if	(!(error == KErrNone || error == KErrLocked))
		return error;
	
	// Even if the alarm is locked (because there aren't any workdays
	// defined), its still valid.
	return KErrNone;
	}


//*************************************************************************************
/**
 * Check that the sound play intervals are valid. Performs basic boundary 
 * checking to ensure that the start time + interval duration does not overlap
 * with a subsequent definition.
 *
 * @return  An error value indicating whether or not the array is valid. KErrNone
 *          for a valid array or KErrGeneral if the array is inappropriate.
 */
TInt ASSrvStaticUtils::ValidateSoundPlayIntervals(RArray<TASSrvAlarmSoundDetails>& aIntervals)
	{
	const TUint KOffsetZero = 0;

	// Sort array of alarm sound details by start time
	TLinearOrder<TASSrvAlarmSoundDetails> order(ASSrvStaticUtils::CompareSoundPlayIntervalStartTimes);
	aIntervals.Sort(order);

	// Compare entries - there must be at least one.
	const TInt count = aIntervals.Count();
	if	(!count)
		return KErrGeneral;

	// The first item in any array must always have an interval of 0 minutes before
	// the alarm will sound (i.e. the client will be audibly notified as soon as the
	// alarm fires).
	if	(aIntervals[0].OffsetInMinutes() != KOffsetZero)
		return KErrArgument;

	for(TInt ii=0; ii < count-1; ii++)
		{
		const TASSrvAlarmSoundDetails& interval1 = aIntervals[ii];
		const TASSrvAlarmSoundDetails& interval2 = aIntervals[ii + 1];
		//
		if	(SoundPlayIntervalsOverlap(interval1, interval2))
			return KErrGeneral;
		}

	return KErrNone;
	}


//*************************************************************************************
/**
 * Fault the server
 */
void ASSrvStaticUtils::Fault(TASSrvFault aFault)
	{
	_LIT(KAlarmServerFault, "ALARMSERVER-FAULT");
	User::Panic(KAlarmServerFault, aFault);
	}


//*************************************************************************************
/**
 * Panic the server
 */
void ASSrvStaticUtils::Panic(TASSrvPanic aPanic)
	{
	_LIT(KAlarmServerPanic, "ALARMSERVER-INTERNAL");
	User::Panic(KAlarmServerPanic, aPanic);
	}


//*************************************************************************************
/**
 * Compare two alarm categories and return a value indicating which is less.
 */
TInt ASSrvStaticUtils::CompareCategories(const TAlarmCategory& aCategory1, const TAlarmCategory& aCategory2)
	{
	//
	// Semantics - returns:
	//
	// 1) zero, if the two objects are equal
	// 2) a negative value, if the first object is less than the second.
	// 3) a positive value, if the first object is greater than the second.
	//
	if	(aCategory1.iUid < aCategory2.iUid)
		return KReturnFirstObjectLessThanSecond;
	else if (aCategory1.iUid > aCategory2.iUid)
		return KReturnFirstObjectGreaterThanSecond;
	//
	return KReturnObjectsAreEqual;
	}


//*************************************************************************************
/**
 * Compare two alarm ids and return a value indicating which is less.
 */
TInt ASSrvStaticUtils::CompareAlarmIds(const TAlarmId& aId1, const TAlarmId& aId2)
	{
	if	(aId1 < aId2)
		return KReturnFirstObjectLessThanSecond;
	else if (aId1 > aId2)
		return KReturnFirstObjectGreaterThanSecond;
	//
	return KReturnObjectsAreEqual;
	}


//*************************************************************************************
/**
 * Compare two alarms and return a value indicating which is less.
 */
TInt ASSrvStaticUtils::CompareAlarms(const TASSrvAlarm& aAlarm1, const TASSrvAlarm& aAlarm2)
	{
	if	(aAlarm1.NextDueTime() < aAlarm2.NextDueTime())
		{
		return KReturnFirstObjectLessThanSecond;
		}
	else if (aAlarm1.NextDueTime() > aAlarm2.NextDueTime())
		{
		return KReturnFirstObjectGreaterThanSecond;
		}
	// If you're reading this comment in the year 2141 and you've been adding an 
	// alarm every second since 2005, you may begin to experience problems as we are
	// now making the assumption that the alarm id counter will never roll-over.
	return ASSrvStaticUtils::CompareAlarmIds(aAlarm1.Id(), aAlarm2.Id());
	}


//*************************************************************************************
/**
 * Compare two alarms and return a boolean if they are the same
 */
TBool ASSrvStaticUtils::CompareAlarmsExact(const TASSrvAlarm& aAlarm1, const TASSrvAlarm& aAlarm2)
	{
	return (CompareAlarmIds(aAlarm1.Id(), aAlarm2.Id()) == KReturnObjectsAreEqual);
	}


//*************************************************************************************
/**
 * Return the version of the Alarm Server
 */
TVersion ASSrvStaticUtils::ASVersion()
	{
	return TVersion(KAlarmServerMajorVN, KAlarmServerMinorVN, KAlarmServerBuildVN);
	}


//*************************************************************************************
/**
 * Return the name of the Alarm Server
 */
const TDesC& ASSrvStaticUtils::ASName()
	{
	return KAlarmServerName;
	}


//*************************************************************************************
/**
 * Rounds any time down to the minute
 */
void ASSrvStaticUtils::RoundTimeDownToTheMinute(TTime& aTime)
	{
	TDateTime time(aTime.DateTime());
	time.SetSecond(0);
	time.SetMicroSecond(0);
	aTime = time;
	}


//*************************************************************************************
/**
 * Panic the client
 */
void ASSrvStaticUtils::PanicClient(const RMessage2& aMessage, TAlarmServerInitiatedClientPanic aPanic)
	{
	_LIT(KServerIntiatedSessionPanic, "ALARMSERVER");
	aMessage.Panic(KServerIntiatedSessionPanic, aPanic);
	}


//*************************************************************************************
/**
 * Return the current utc time, to the nearest second
 */
TTime ASSrvStaticUtils::UtcTimeNow()
	{	
	TTime NowInUTC;	
	NowInUTC.UniversalTime();
	TDateTime dateTime(NowInUTC.DateTime());
	dateTime.SetMicroSecond(0); // Remove microsecond element.
	NowInUTC = dateTime;
	return NowInUTC;
	}

//*************************************************************************************
/**
 * Return the current local time, to the nearest second
 */
TTime ASSrvStaticUtils::LocalTimeNow()
	{	
  	TTime now;
  	now.HomeTime();
  	TDateTime dateTime(now.DateTime());
  	dateTime.SetMicroSecond(0);
  	return dateTime;
	}
	
//*************************************************************************************
/**
 * Return (via reference) today's date at the specified time.
 */
void ASSrvStaticUtils::TodayAtTheSpecifiedTime(const TTime& aTime, TTime& aReturn)
	{
	// Reset the cycle and sound state ready to start playing
	const TDateTime todaysDate = ASSrvStaticUtils::UtcTimeNow().DateTime();
	TDateTime baselineDateTime = aTime.DateTime();
	//
	baselineDateTime.SetYear(todaysDate.Year());
	baselineDateTime.SetMonth(todaysDate.Month());
	baselineDateTime.SetDay(todaysDate.Day());
	baselineDateTime.SetMicroSecond(0);
	aReturn = baselineDateTime;
	}


//*************************************************************************************
/**
 * Internalize the sound play intervals from the specified stream and return
 * them on the cleanup stack.
 */
RArray<TASSrvAlarmSoundDetails>* ASSrvStaticUtils::InternalizeSoundPlayIntervalsLC(RReadStream& aStream)
	{
	const TInt count = aStream.ReadInt32L();
	if	(!count)
		User::Leave(KErrArgument);
	//
	RArray<TASSrvAlarmSoundDetails>* newIntervals = new(ELeave) RArray<TASSrvAlarmSoundDetails>(count);
	CleanupStack::PushL(TCleanupItem(ASSrvStaticUtils::CleanupCloseDeleteSoundDetailsArray, newIntervals));
	//
	for(TInt i=0; i<count; i++)
		{
		const TUint offset = aStream.ReadUint32L();
		const TUint duration = aStream.ReadUint32L();
		//
		User::LeaveIfError(newIntervals->Append(TASSrvAlarmSoundDetails(offset, duration)));
		}
	//
	return newIntervals;
	}
//*************************************************************************************
/**
This method will return a descriptor created in such a way that
it holds the path to the directory containing the Alarm Servers private data.
*/
void ASSrvStaticUtils::GetPrivateDirL(RFs& aFsSession, TDes& aDirName)
	{
	//Sets the session path to the process's private directory 
	User::LeaveIfError(aFsSession.PrivatePath(aDirName));
	//Alarmserver.ini is supposed to be on drive C:
	TDriveName driveName(TDriveUnit(EDriveC).Name());
	aDirName.Insert(0,driveName); //drive name goes before the path
	}

//*************************************************************************************
/**
This method will return a descriptor created in such a way that
it holds the whole search path to the Alarmserver.ini file.
*/
void ASSrvStaticUtils::GetServerPathL(RFs& aFsSession, TDes& aFileName)
	{
	//Sets the session path to the process's private directory on C:
	GetPrivateDirL(aFsSession, aFileName);
	aFileName.Append(KAlarmServerBackupFile); //filename goes after the path
	}

//*************************************************************************************
/**
 * Calculate days until the next active alarm day.
 */
void ASSrvStaticUtils::DaysUntilNextActiveAlarmDay(
	TTimeIntervalDays& aDaysFromNow,
	TTimeIntervalDays& aRollOverDays,
	TDay aCurrentDayInWeek,
	TUint aActiveDays)
	{
	TInt daysToAddFromNow = 7;
	TInt daysForRollOver = 0;

	for(TInt i=6; i>=0; --i)
		{
		daysForRollOver = daysToAddFromNow;

		const TInt day = (aCurrentDayInWeek + i) % 7;
		const TInt dayMask = (1 << day);

		if (dayMask & aActiveDays)
			{
			daysToAddFromNow = i;
			}
		}

	aDaysFromNow = daysToAddFromNow;
	aRollOverDays = daysForRollOver;
	}


//*************************************************************************************
/**
 * Update the specified alarm for next 24 hours repeat. Depending on the current
 * time the alarm may repeat within the next day, or it may repeat tomorrow.
 */
void ASSrvStaticUtils::CalculateNext24HoursRepeat(TASShdAlarm& aAlarm, TTimeIntervalSeconds aAllowableWindow)
	{
	__ASSERT_DEBUG(aAlarm.RepeatDefinition() == EAlarmRepeatDefintionRepeatNext24Hours, ASSrvStaticUtils::Panic(ASSrvStaticUtils::EASSrvPanicInvalidAlarmRepeat));
	
	// Get the current due time.
	const TDateTime alarmTimeAsDateTime = aAlarm.NextDueTime().DateTime();
	
	// Get the current time.
	TTime timeNow(UtcTimeNow());
	
	// Set the new due time.
	TDateTime nextDueTimeAsDateTime = timeNow.DateTime();   // Take the current date.
	nextDueTimeAsDateTime.SetHour(alarmTimeAsDateTime.Hour());
	nextDueTimeAsDateTime.SetMinute(alarmTimeAsDateTime.Minute());
	nextDueTimeAsDateTime.SetSecond(0);
	nextDueTimeAsDateTime.SetMicroSecond(0);
	
    TTime nextDueTime(nextDueTimeAsDateTime);
	
	// Allow for the startup window. This ensures that any alarm which causes the
	// device to be brought out of standby isn't silently ignored.
	timeNow -= aAllowableWindow;
	
	// If the due time has passed then set it for the next day.
	if (nextDueTime < timeNow)
	    {
	    nextDueTime += TTimeIntervalDays(1);
	    }
	//
	aAlarm.NextDueTime()=nextDueTime;
	}


//*************************************************************************************
/**
 * Cleanup an RArray<TAlarmId>*
 */
void ASSrvStaticUtils::CleanupCloseDeleteAlarmIdArray(TAny* aArray)
	{
	RArray<TAlarmId>* array = reinterpret_cast<RArray<TAlarmId>*>(aArray);
	array->Close();
	delete array;
	}


//*************************************************************************************
/**
 * Cleanup an RArray<TAlarmCategory>*
 */
void ASSrvStaticUtils::CleanupCloseDeleteCategoryArray(TAny* aArray)
	{
	RArray<TAlarmCategory>* array = reinterpret_cast<RArray<TAlarmCategory>*>(aArray);
	array->Close();
	delete array;
	}


//*************************************************************************************
/**
 * Cleanup an RArray<TASSrvAlarmSoundDetails>*
 */
void ASSrvStaticUtils::CleanupCloseDeleteSoundDetailsArray(TAny* aArray)
	{
	RArray<TASSrvAlarmSoundDetails>* array = reinterpret_cast<RArray<TASSrvAlarmSoundDetails>*>(aArray);
	array->Close();
	delete array;
	}


//
//
//


//*************************************************************************************
/**
 * Compare two alarm sound play interval start times and return a value indicating which is less.
 */
TInt ASSrvStaticUtils::CompareSoundPlayIntervalStartTimes(const TASSrvAlarmSoundDetails& aAlarmSound1, const TASSrvAlarmSoundDetails& aAlarmSound2)
	{
	if	(aAlarmSound1.OffsetInMinutes() < aAlarmSound2.OffsetInMinutes())
		return KReturnFirstObjectLessThanSecond;
	else if (aAlarmSound1.OffsetInMinutes() > aAlarmSound2.OffsetInMinutes())
		return KReturnFirstObjectGreaterThanSecond;
	//
	return KReturnObjectsAreEqual;
	}


//*************************************************************************************
/**
 * Compare two alarm sound play intervals return ETrue if they overlap, else EFalse.
 */
TBool ASSrvStaticUtils::SoundPlayIntervalsOverlap(const TASSrvAlarmSoundDetails& aDetails1, const TASSrvAlarmSoundDetails& aDetails2)
	{
	TInt offsetInSeconds = aDetails1.OffsetInMinutes() * KNumberOfSecondsInMinute;
	offsetInSeconds += aDetails1.DurationInSeconds();
	//
	const TInt offsetOfOtherItemInSeconds = aDetails2.OffsetInMinutes() * KNumberOfSecondsInMinute;
	//
	return (offsetInSeconds > offsetOfOtherItemInSeconds);
	}


