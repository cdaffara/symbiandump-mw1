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
//

#include <s32buf.h>
#include <vtzrules.h>
#include <tz.h>

#include "tzrules.h"

const TInt KDaysInTheWeek = 7;
const TInt KTzRulesGranularity = 4;

//
// TVTzRule
//

/**
Constructor for a time zone rule.

@publishedAll
@released

@param aFrom The first date at which the rule applies.
@param aTo The last date at which the rule applies.
@param aOldOffset The UTC offset in minutes which applies before the DST change.
@param aNewOffset The UTC offset in minutes which applies after the DST change.
@param aMonth The month in which the DST change occurs.
@param aDayRule The rule defining on which day the DST change takes place.
@param aDayOfMonth The number of the day within the month, offset from zero.  Used in conjunction with 
       aDayRule to define the day when DST changes.
@param aDayOfWeek The number of the day within the week, the numerical equivalent of a TDay value.  
       Used in conjunction with aDayRule to define the day where DST changes.
@param aTimeReference Defines whether aTimeOfChange is a local (wall-clock) time or a UTC time.
@param aTimeOfChange The time of the DST change in minutes from midnight.
*/
EXPORT_C TTzRule::TTzRule(TTimeWithReference aFrom, TTimeWithReference aTo, TInt16 aOldOffset, TInt16 aNewOffset, TMonth aMonth, TTzRuleDay aDayRule,
				TUint8 aDayOfMonth, TUint8 aDayOfWeek, TTzTimeReference aTimeReference, TUint16 aTimeOfChange) :
 	iFrom(aFrom),
	iTo(aTo),
	iOldLocalTimeOffset(aOldOffset),
	iNewLocalTimeOffset(aNewOffset),
	iMonth(aMonth),
	iDayRule(aDayRule),
	iDayOfMonth(aDayOfMonth),
	iDayOfWeek(aDayOfWeek),
	iTimeReference(aTimeReference),
	iTimeOfChange(aTimeOfChange)
	{
	}

/**
Constructor for a time zone rule.

@publishedAll
@released

@param aFromYear The first year in which the rule applies.
@param aToYear The last year in which the rule applies.
@param aOldOffset The UTC offset in minutes which applies before the DST change.
@param aNewOffset The UTC offset in minutes which applies after the DST change.
@param aMonth The month in which the DST change occurs.
@param aDayRule The rule defining on which day the DST change takes place.
@param aDayOfMonth The number of the day within the month, offset from zero.  Used in conjunction with 
       aDayRule to define the day where DST changes.  
@param aDayOfWeek The number of the day within the week, the numerical equivalent of a TDay value.  
       Used in conjunction with aDayRule to define the day where DST changes.
@param aTimeReference Defines whether aTimeOfChange is a local (wall-clock) time or a UTC time.
@param aTimeOfChange The time of the DST change in minutes from midnight.
*/
EXPORT_C TTzRule::TTzRule(TInt16 aFromYear, TInt16 aToYear, TInt16 aOldOffset, TInt16 aNewOffset, TMonth aMonth, TTzRuleDay aDayRule,
				TUint8 aDayOfMonth, TUint8 aDayOfWeek, TTzTimeReference aTimeReference, TUint16 aTimeOfChange) :
 	iFrom(TTimeWithReference(TDateTime(aFromYear,EJanuary,0,0,0,0,0))),
	iTo(TTimeWithReference(TDateTime(aToYear,EDecember,30,23,59,59,0))),
	iOldLocalTimeOffset(aOldOffset),
	iNewLocalTimeOffset(aNewOffset),
	iMonth(aMonth),
	iDayRule(aDayRule),
	iDayOfMonth(aDayOfMonth),
	iDayOfWeek(aDayOfWeek),
	iTimeReference(aTimeReference),
	iTimeOfChange(aTimeOfChange)
	{
	}

/**
Default constructor for a time zone rule.

All member variables are set to zero values.

@publishedAll
@released
*/
EXPORT_C TTzRule::TTzRule() :
	iOldLocalTimeOffset(0),
	iNewLocalTimeOffset(0),
	iMonth(EJanuary),
	iDayRule(ETzFixedDate),
	iDayOfMonth(0),
	iDayOfWeek(0),
	iTimeReference(ETzWallTimeReference),
	iTimeOfChange(0)
	{
	}

/**
Copy constructor for a time zone rule.

@publishedAll
@released
*/
EXPORT_C TTzRule::TTzRule(const TTzRule& aRule) :
 	iFrom(aRule.iFrom),
	iTo(aRule.iTo),
	iOldLocalTimeOffset(aRule.iOldLocalTimeOffset),
	iNewLocalTimeOffset(aRule.iNewLocalTimeOffset),
	iMonth(aRule.iMonth),
	iDayRule(aRule.iDayRule),
	iDayOfMonth(aRule.iDayOfMonth),
	iDayOfWeek(aRule.iDayOfWeek),
	iTimeReference(aRule.iTimeReference),
	iTimeOfChange(aRule.iTimeOfChange)
	{
	}

/** 
Externalises a time zone rule to a write stream.

@param aStream Stream to which the object should be externalised. 
@internalComponent
@released
*/ 
void TTzRule::ExternalizeL(RWriteStream& aStream) const
	{
	aStream << iFrom.iTime.Int64();
	aStream << static_cast<TInt32>(iFrom.iTimeReference);
	aStream << iTo.iTime.Int64();
	aStream << static_cast<TInt32>(iTo.iTimeReference);
	aStream << iOldLocalTimeOffset;
	aStream << iNewLocalTimeOffset;
	aStream << static_cast<TInt32>(iMonth);
	aStream << static_cast<TInt32>(iDayRule);
	aStream << iDayOfMonth;
	aStream << iDayOfWeek;
	aStream << static_cast<TInt32>(iTimeReference);
	aStream << iTimeOfChange;
	}

/** 
Internalizes a time zone rule from a read stream.

@param aStream Stream from which the object should be internalised. 
@internalComponent
@released
*/ 
void TTzRule::InternalizeL(RReadStream& aStream)
	{
	TInt64 time;
	aStream >> time;	
	iFrom.iTime = TTime(time);
	iFrom.iTimeReference = static_cast<TTzTimeReference>(aStream.ReadInt32L());
	aStream >> time;	
	iTo.iTime = TTime(time);
	iTo.iTimeReference = static_cast<TTzTimeReference>(aStream.ReadInt32L());
	aStream >> iOldLocalTimeOffset;
	aStream >> iNewLocalTimeOffset;
	iMonth = static_cast<TMonth>(aStream.ReadInt32L());
	iDayRule = static_cast<TTzRuleDay>(aStream.ReadInt32L());
	aStream >> iDayOfMonth;
	aStream >> iDayOfWeek;
	iTimeReference = static_cast<TTzTimeReference>(aStream.ReadInt32L());
	aStream >> iTimeOfChange;
	}


/** 
Check if a time zone rule is applicable during a time range.

@return ETrue if the rule applies during the supplied time range, EFalse otherwise.
@param aStart Start of time range, inclusively.  ie. For a rule to be applicable, this time
       can be equal to or earlier than the rule's end time.  This time should use the same 
       time reference used when TTzRule is constructed.
@param aEnd End of time range, exclusively.  ie. In order for a rule to be applicable, this 
       time must be later than the rule's start time.  This time should use the same 
       time reference used when TTzRule is constructed.
@internalComponent
@released
*/ 
TBool TTzRule::RuleApplies(const TTime& aStart, const TTime& aEnd) const
 	{
	// If the end of the rule is in the time range or the beginning of the rule is in the time range
	return ( (iTo.iTime >= aStart) && (iFrom.iTime < aEnd) );
 	}

/**
Resolves the date rule to the precise date and time for the given year, and
returns it in a TVTzActualisedRule

For day rules ETzDayAfterDate & ETzDayBeforeDate, the reference date entered 
is inclusive in the calculation.  ie. If the reference date fits the criteria,
the reference date will be returned.   

eg. Actualise(Friday, ETzDayAfterDate, Fri Jun 22, 2007) returns Jun 22, 2007.

Deprecated.  Use TVTzActualisedRule TTzRule::ActualiseL instead.

@return The time zone rule with the precise date and time for the given year.
@param aYear Year to actualise time rules
@publishedAll
@deprecated 
@see TVTzActualisedRule TTzRule::ActualiseL(TInt aYear) const
*/
EXPORT_C TVTzActualisedRule TTzRule::Actualise(TInt aYear) const
    {
    TVTzActualisedRule rule;
    TRAP_IGNORE(rule = ActualiseL(aYear));        
    return rule;
    }

/**
Resolves the date rule to the precise date and time for the given year, and
returns it in a TVTzActualisedRule

For day rules ETzDayAfterDate & ETzDayBeforeDate, the reference date entered 
is inclusive in the calculation.  ie. If the reference date fits the criteria,
the reference date will be returned.   

eg. Actualise(Friday, ETzDayAfterDate, Fri Jun 22, 2007) returns Jun 22, 2007.

@return The time zone rule with the precise date and time for the given year.
@param aYear Year to actualise time rules
@leave KErrCorrupt if the current day rule is not one of the defined values in TTzRuleDay
@publishedAll
@released
*/
EXPORT_C TVTzActualisedRule TTzRule::ActualiseL(TInt aYear) const
	{
	TInt dayOfMonth = iDayOfMonth;
	TInt daysDifference = 0;
	TDay dayOfWeek(EMonday);

	TDateTime actualDateTime(aYear,
						(TMonth)iMonth,
						dayOfMonth,
						(iTimeOfChange / 60),
						(iTimeOfChange % 60),
						0,
						0);
	TTime actualTime(actualDateTime);
	switch(iDayRule)
		{
		case ETzFixedDate:
			// do nothing
			break;

		case ETzDayAfterDate:
			// e.g. "Sunday after the 15th"

			// find day in week for given date
			dayOfWeek = actualTime.DayNoInWeek();

			// find difference in days to the rule date
			daysDifference = iDayOfWeek - dayOfWeek;
			// make positive difference
			if (daysDifference < 0)
				{
				daysDifference += KDaysInTheWeek;	// sunday is last day of month
				}

			// set actual day
			dayOfMonth += daysDifference;
			actualDateTime.SetDay(dayOfMonth);
			break;

		case ETzDayBeforeDate:
			// e.g. "Sunday before the 15th"

			// find day in week for given date
			dayOfWeek = actualTime.DayNoInWeek();

			// find difference in days to the rule date
			daysDifference = dayOfWeek - iDayOfWeek;
			// make positive difference
			if (daysDifference < 0)
				{
				daysDifference += KDaysInTheWeek;	// sunday is last day of month
				}

			// set actual day
			dayOfMonth -= daysDifference;
			actualDateTime.SetDay(dayOfMonth);
			break;

		case ETzDayInLastWeekOfMonth:
			// e.g. "last Sunday in the month"

			// initialise the day to the last day in the month,
			dayOfMonth = actualTime.DaysInMonth() - 1; // days offset from 0
			actualDateTime.SetDay(dayOfMonth);

			// find day in week for given date
			actualTime = actualDateTime;
			dayOfWeek = actualTime.DayNoInWeek();

			// find difference in days to the rule date
			daysDifference = dayOfWeek - iDayOfWeek;
			// make positive difference
			if (daysDifference < 0)
				{
				daysDifference += KDaysInTheWeek;	// sunday is last day of month
				}

			// set actual day
			dayOfMonth -= daysDifference;
			actualDateTime.SetDay(dayOfMonth);
			break;

		default:
			User::Leave(KErrCorrupt);  // If data is corrupt method leaves
			break;
		}

	actualTime = actualDateTime;

	TVTzActualisedRule tActRule(actualTime, 
		iNewLocalTimeOffset, 
		static_cast<TTzTimeReference>(iTimeReference));
	return tActRule;
	}

//
// CTzRules
//

/**
Creates a new time zone rules object.
@return Pointer to the time zone rules.
@publishedAll
@released
*/
EXPORT_C CTzRules* CTzRules::NewL()
	{
	CTzRules* self = new(ELeave) CTzRules();
	return self;
	}

/**
Creates a new time zone rules object.
@param aStartYear The first year in which these time zone rules apply.
@param aEndYear The last year in which these time zone rules apply.
@return Pointer to the time zone rules.
@publishedAll
@released
*/
EXPORT_C CTzRules* CTzRules::NewL(TInt aStartYear, TInt aEndYear)
	{
	CTzRules* self = new(ELeave) CTzRules(aStartYear,aEndYear);
	return self;
	}

/**
Creates a new time zone rules object from a stream.
@return Pointer to the time zone rules.
@param aStream Stream with the time zone rules to be used to create a CTzRules object.
@publishedAll
@released
*/
EXPORT_C CTzRules* CTzRules::NewL(RReadStream& aStream)
	{
	CTzRules* self = new(ELeave) CTzRules();
	CleanupStack::PushL(self);
	self->InternalizeL(aStream);
	CleanupStack::Pop(self);
	return self;
	}

CTzRules::CTzRules(TInt aStartYear, TInt aEndYear) :
	iStartYear((TInt16)aStartYear),
	iEndYear((TInt16)aEndYear),
	iRules(KTzRulesGranularity)
	{
	}

CTzRules::CTzRules() :
	iRules(KTzRulesGranularity)
	{
	}

/**
Destructor.
@publishedAll
@released
*/
EXPORT_C CTzRules::~CTzRules()
	{
	delete iActualisedRulesCache;
	iRules.Reset();
	}

/**
Get the object size when it is internalize and externalised.

@return the size of thisobject 
@internalComponent
@released
*/
EXPORT_C TInt CTzRules::SizeOfObject() const
	{
	return iRules.Count() * sizeof (TTzRule) + 4*sizeof (TInt16);
	//As to 4 size of TInt16, they are for iStartYear, iEndYear, iInitialStdTimeOffset and the count of array of TTzRule
	}
/**
Internalizes time zone rules from a read stream.

@param aStream Stream from which the object should be internalised. 
@publishedAll
@released
*/
EXPORT_C void CTzRules::InternalizeL(RReadStream& aStream)
	{
	iRules.Reset();

	// read start and end year covered by rules
	iStartYear = aStream.ReadInt16L();
	iEndYear = aStream.ReadInt16L();

	// read initial std time offset
	iInitialStdTimeOffset = aStream.ReadInt16L();
	
	// read number of rules
	const TInt16 KCount = aStream.ReadInt16L();

	// read rules
	TTzRule rule;
	for (TInt i = 0; i < KCount; ++i)
		{
		rule.InternalizeL(aStream);
		User::LeaveIfError(iRules.Append(rule));
		}
	
	// The cache has been invalidated so clear it
	delete iActualisedRulesCache;
	iActualisedRulesCache = NULL;
	}

/** 
Externalises time zone rules to a write stream.

@param aStream Stream to which the object should be externalised. 
@leave KErrArgument if the output stream size is invalid
@publishedAll
@released
*/
EXPORT_C void CTzRules::ExternalizeL(RWriteStream& aStream) const
	{
	// Ensure the size of the stream is valid.
	TInt size = aStream.Sink()->SizeL();
	const TInt KMaxSize = KMaxTInt / 2;
	if (size < 0 || size >= KMaxSize)
	    {
	    User::Leave(KErrArgument);
	    }

	// write range of years covered by rules
	aStream.WriteInt16L(iStartYear);
	aStream.WriteInt16L(iEndYear);
	
	// write initial std time offset
	aStream.WriteInt16L(iInitialStdTimeOffset);
	
	// write number of rules
	const TInt16 KCount = (TInt16)iRules.Count();
	aStream.WriteInt16L(KCount);

	// write rules
	for (TInt i = 0; i < KCount; i++)
		{
		iRules[i].ExternalizeL(aStream);
		}
	}

/**
Gets the first year in which the time zone rules apply.
@return The year.
@publishedAll
@released
*/
EXPORT_C TInt CTzRules::StartYear() const
	{
	return iStartYear;
	}

/**
Gets the last year in which the time zone rules apply.
@return The year.
@publishedAll
@released
*/
EXPORT_C TInt CTzRules::EndYear() const
	{
	return iEndYear;
	}

/**
Sets the first year in which the time zone rules apply.
@param aYear The year.
@publishedAll
@released
*/
EXPORT_C void CTzRules::SetStartYear(TInt aYear)
	{
	iStartYear = (TInt16)aYear;
	}

/**
Sets the last year in which the time zone rules apply.
@param aYear The year.
@publishedAll
@released
*/
EXPORT_C void CTzRules::SetEndYear(TInt aYear)
	{
	iEndYear = (TInt16)aYear;
	}

/**
Gets the number of time zone rules (TTzRules) in this set.
@return The number of rules.
@publishedAll
@released
*/
EXPORT_C TInt CTzRules::Count() const
	{
	return iRules.Count();
	}

/**
Gets the initial UTC offset for this set of time zone rules.
@return The offset in minutes.
@publishedAll
@released
*/
EXPORT_C TInt CTzRules::InitialStdTimeOffset() const
	{
	return iInitialStdTimeOffset;
	}
	
/**
Sets the initial UTC offset for this set of time zone rules.
@param aOffset The offset in minutes.
@publishedAll
@released
*/
EXPORT_C void CTzRules::SetInitialStdTimeOffset(TInt aOffset)
	{
	// The cache has been invalidated so clear it
	delete iActualisedRulesCache;
	iActualisedRulesCache = NULL;
	
	iInitialStdTimeOffset = aOffset;
	}
	
/**
Adds a time zone rule to this set.
@param aTRule The rule to be added.
@publishedAll
@released
*/
EXPORT_C void CTzRules::AddRuleL(TTzRule aTRule)
	{
	// the cache has been invalidated so clear it
	delete iActualisedRulesCache;
	iActualisedRulesCache = NULL;
	
	TInt result = iRules.Append(aTRule);
	User::LeaveIfError(result);
	}

/**
Removes a time zone rule from this set.
@param aIndex The index of the rule to be removed.
@publishedAll
@released
*/
EXPORT_C void CTzRules::RemoveRule(TInt aIndex)
	{
	// the cache has been invalidated so clear it
	delete iActualisedRulesCache;
	iActualisedRulesCache = NULL;
		
	iRules.Remove(aIndex);
	}

/**
Gets a time zone rule from this set.
@param aIndex The index of the rule to be fetched.
@return Reference to the time zone rule.
@publishedAll
@released
*/
EXPORT_C TTzRule& CTzRules::operator[](TInt aIndex)
	{
	// prevent array bounds error
	__ASSERT_ALWAYS( (aIndex < iRules.Count() && aIndex >= 0), RTz::Panic(RTz::EPanicRulesIndexOutofRange));
	
	return iRules[aIndex];
	}

/**
Queries the time zone rule set to see if they apply to a specified time.
@param aTime The time to be checked, using the same time reference used when constructing 
       CTzRules.
@return ETrue if the time zone rules apply to the specified time. EFalse if not.
@publishedAll
@released
*/
 EXPORT_C TBool CTzRules::RulesApply(const TTime& aTime) const
 	{
	const TTime KStart(TDateTime(iStartYear, EJanuary, 0, 0, 0, 0, 0));
	const TTime KEnd(TDateTime(iEndYear + 1, EJanuary, 0, 0, 0, 0, 0));
	return ((aTime >= KStart) && (aTime < KEnd));
 	}


/**

Compares two times with time references (TTzTimeReference).
TTzTimeReference is either UTC, or STD, or ETzWallTimeReference.

Comparison is done at some point in time when aStdOffset applies to item(s) with ETzStdTimeReference, 
and aWallOffset applies to item(s) with ETzWallTimeReference.

If time references for both aTimeA and aTimeB are the same, then straight comparison is done to 
aTimeA and aTimeB. Otherwise, STD or wall-clock time is converted to UTC using either aStdOffset, 
or aWallOffset correspondingly.

Thus, aStdOffset parameter is never used if none of aTimeA and aTimeB is ETzStdTimeReference;
and, similarly, aWallOffset is never used if none of aTimeA and aTimeB is ETzWallTimeReference.

@param	aTimeA		- first time with reference;
@param	aTimeB		- second time with reference, to compare to aTimeA;
@param	aStdOffset	- an offset (in minutes) to be used for a time with ETzStdTimeReference;
@param	aWallOffset	- an offset (in minutes) to be used for a time with ETzWallTimeReference;
@param	aMinutesDifference	-on return, the time difference between aTimeA and aTimeB in minutes;

@return TInt		-1 if aTimeA is < aTimeB;
					 0 if aTimeA is equal to aTimeB;
					 1 if aTimeA is > aTimeB.	 
Example: 
	compare 20050403T020200 local with 20050403T020100Z UTC in PST time zone (-0800):
			CompareTimesWithRef(20050403T020200, wall, 20050403T020100, utc, 0(not used), -8(PST))  gives -1;
	while 	CompareTimesWithRef(20050403T020200, wall, 20050403T020100, utc, 0(not used), 0(GMT))  gives 1.

@internalComponent
@released
 */ 
TInt CTzRules::CompareTimesWithRef(TTime aTimeA, TTzTimeReference aTimeARef,
								   TTime aTimeB, TTzTimeReference aTimeBRef,
								   TInt aStdOffset, TInt aWallOffset, TTimeIntervalMinutes* aMinutesDifference) const
	{
	TTime timeA(aTimeA), timeB(aTimeB);
	if (aTimeARef != aTimeBRef)
		{
		if (aTimeARef == ETzStdTimeReference)
			{
			timeA -= TTimeIntervalMinutes(aStdOffset);	
			}
		if (aTimeARef == ETzWallTimeReference)
			{
			timeA -= TTimeIntervalMinutes(aWallOffset);	
			}
		if (aTimeBRef == ETzStdTimeReference)
			{
			timeB -= TTimeIntervalMinutes(aStdOffset);	
			}
		if (aTimeBRef == ETzWallTimeReference)
			{
			timeB -= TTimeIntervalMinutes(aWallOffset);	
			}
		}

	if (aMinutesDifference)
		{
		timeA.MinutesFrom(timeB, *aMinutesDifference);
		}

	return (timeA<timeB) ? -1 : ((timeA>timeB) ? 1 : 0);
	}

/**
Based on the rules in the object calculates actualised rules for particular year and adds them
to the specified array of actualised rules (CVTzActualisedRules).

@param	aActRules	- an array of actualised rules where new actualised rules are added;
@param	aYear		- a year for which actualised rules are added.

@internalComponent
@released
*/
void CTzRules::AddActualisedRulesL(CVTzActualisedRules& aActRules, TInt aYear) const
   	{
   	const TInt count=iRules.Count();
   	const TTzRule* trule = NULL;

	const TTime KYearBegin(TDateTime(aYear, EJanuary, 0, 0, 0, 0, 0));
	const TTime KYearEnd(TDateTime(aYear + 1, EJanuary, 0, 0, 0, 0, 0));

   	for (TInt i = 0; i < count; i++)
   		{
   		trule = &(iRules[i]);
   		if ( trule->RuleApplies(KYearBegin, KYearEnd))
   			{
   			TVTzActualisedRule tactRule = trule->Actualise(aYear);
   
   			// ETzStdTimeReference is not useful for VTIMEZONE or for conversions
   			if(trule->iTimeReference == ETzStdTimeReference)
   				{
				//# Time: Time for the DST switch. Important: sometimes the time is followed by a letter. The meaning of this letter is:
				//no letter or w: wall clock time, actual local time.
				//s: local standard time (winter time)
				//u or g or z: UTC time.
				//So, if a 'DST on' time is given as '0:00s', this means a switch on 0:00 local time.
   				//If a 'DST off' time is given as '0:00s', this means a switch on 1:00 local time (assuming a DST offset of 1 hour).
   				
   				//DST off time is always the lowest among the two.
				//We do not want to use the oldlocaltimeoffset for DST Off conditions.
   				TInt standardOffset = trule->iOldLocalTimeOffset;
   				if(trule->iNewLocalTimeOffset < trule->iOldLocalTimeOffset)
   					{
   					standardOffset = trule->iNewLocalTimeOffset;
   					}
   				tactRule.iTimeOfChange -= TTimeIntervalMinutes(standardOffset);
   				tactRule.iTimeReference = ETzUtcTimeReference;
   				}
   				
  			if ( 0 >= CompareTimesWithRef(tactRule.iTimeOfChange, tactRule.iTimeReference,
  											trule->iTo.iTime, trule->iTo.iTimeReference,
  											0, trule->iOldLocalTimeOffset, NULL) )
  				{
  				// tactRule.iTimeOfChange w/ref is less or equal to trule->iTo

  				/*Fix for INC117764:
				Before adding make sure that rule does not occur before the rules which have already been added.
				For Ex-"Europe\Tirane" 
				1) Rule Starts at 1/4/1984, Rule Ends at 30/6/1984, Old offset = 60, New Offset = 120 (Summer rule)
				2) Rule Starts at 1/7/1984, Rule Ends at 1/7/1984, Old offset = 60, New Offset = 120 (Summer rule contd.)
				3) Rule Starts at 1/9/1984, Rule Ends at 31/12/1995, Old offset = 60, New Offset = 60(winter rule) 
				4) Rule Starts at 1/3/1984, Rule Ends at 31/12/9998, Old offset = 60, New Offset = 120 (Summer Rule)
				The 4th rule should follow from 7/7/1984 onwards, otherwise if 4th rule is actualised for year 1984
				it would look like --Rule Starts at 1/3/1984, NewOffset = 120, this will cause issues when converting time in March since summer
				time won't start until April 1984.
				*/
  				TBool ruleAdded = ETrue;
				for(TInt loop = aActRules.Count()-1; loop >=0; --loop)
					{
					TVTzActualisedRule prevActRule = aActRules[loop];
					TDateTime prevRdt = prevActRule.iTimeOfChange.DateTime();
					TDateTime actRdt = tactRule.iTimeOfChange.DateTime();	
					if(actRdt.Year() > prevRdt.Year())
						{
						//no covering rule for this year in previously added rules
						//no need of looking into other year rules
						break;						
						}
					if(actRdt.Year() == prevRdt.Year() &&
					   actRdt.Month() < prevRdt.Month() &&
					   tactRule.iNewOffset == prevActRule.iNewOffset)
						{
						//covering rule already exists so dont add this one
						ruleAdded = EFalse;
						break;
						}	
					}
				if(ruleAdded)
					{
					aActRules.AddRuleL(tactRule);	
					}
  				}
			}
   		}
   	}

/**
Get actualised rules for time zone rules.
@param aActRules Actualised rules for time zone rules.
@publishedAll
@released
*/
EXPORT_C void CTzRules::GetActualisedRulesL(CVTzActualisedRules& aActRules) const
	{
	// Always add the initial offset because there may not have been
	// any rules to actualise before the actualised rules range specified
	TVTzActualisedRule tDefRule(TDateTime(iStartYear, EJanuary, 0, 0, 0, 0, 0), iInitialStdTimeOffset, ETzWallTimeReference);
	aActRules.AddRuleL(tDefRule);
	
	if (iRules.Count() == 0)
		{
		// do nothing
		}
	if (iRules.Count() == 1)
		{
		// There is only one rule, perhaps this is an unknown timezone
		// or a timezone that has never had DST, so just add that one rule
		
		TVTzActualisedRule tDefRule(TDateTime(aActRules.StartYear(), EJanuary, 0, 0, 0, 0, 0), iRules[0].iNewLocalTimeOffset, ETzWallTimeReference);
		aActRules.AddRuleL(tDefRule);
		}
	else
		{
		// Add actualised rules for the year range spcified in aActRules
		// also make sure that a year before this range is actualised
		// so that the initial offset for the actualised rules range is correct
		
		TInt32 year(0);
		for (year = aActRules.EndYear() ; year >= aActRules.StartYear() ; --year) 
			{
			AddActualisedRulesL(aActRules, year);
			}
		
		// try to add an actualised rule for the previous year
		// so that the start of the range will be correct
		const TInt KActRulesCount(aActRules.Count());
		AddActualisedRulesL(aActRules, year);
		
		if (aActRules.Count() == KActRulesCount)
			{
			// No rules were added for the year before the range being actualised
			// so find the first rule that applies before this year
			
			TTime timeSoFar(TDateTime(iStartYear, EJanuary, 0, 0, 0, 0, 0));	
			TTime endTime(TDateTime(year, EJanuary, 0, 0, 0, 0, 0));
			
			TBool found(EFalse);
			
			for (TInt i(iRules.Count() - 1) ; i >= 0 ; --i)
				{
				if (iRules[i].iTo.iTime > timeSoFar && iRules[i].iTo.iTime < endTime)
					{
					timeSoFar = iRules[i].iTo.iTime;
					found = ETrue;
					}
				}
			
			if (found)
				{
				AddActualisedRulesL(aActRules, timeSoFar.DateTime().Year());
				}
			}
		}
	}

/**
Converts the received local time to UTC time.
@param aTime The time to convert. On return, this contains the converted time.
@publishedAll
@released
*/ 
EXPORT_C void CTzRules::ConvertToUtcL(TTime& aTime) const
	{
	// This check would not be necessary if we can ensure this function
	// is never called with a null TTime
	if (aTime == Time::NullTTime())
		{
		return;
		}
	
	TInt offset = GetOffsetL(aTime, ETzWallTimeReference);
	aTime -= (TTimeIntervalMinutes)(offset);	
	}

/**
Converts the received UTC time to local time.
@param aTime The time to convert. On return, this contains the converted time.
@publishedAll
@released
*/
EXPORT_C void CTzRules::ConvertToLocalL(TTime& aTime) const
	{
	// This check would not be necessary if we can ensure this function
	// is never called with a null TTime	
	if (aTime == Time::NullTTime())
		{
		return;
		}
	
	TInt offset = GetOffsetL(aTime, ETzUtcTimeReference);
	aTime += (TTimeIntervalMinutes)(offset);	
	}
	
/**
Calculate the local time offset at the supplied time.

@return local time offset
@internalComponent
@released
*/
EXPORT_C TInt CTzRules::GetOffsetL(const TTime& aTime, TTzTimeReference aTimeRef) const
	{
	if (iActualisedRulesCache)
		{
		TTime startTime(TDateTime(iActualisedRulesCache->StartYear(), EJanuary, 0, 0, 0, 0, 0));	
		TTime endTime(TDateTime(iActualisedRulesCache->EndYear() + 1, EJanuary, 0, 0, 0, 0, 0));
		
		if (aTime >= startTime && aTime <= endTime)
			{
			return iActualisedRulesCache->GetOffsetFromRuleL(aTime, aTimeRef);
			}
		}

	TInt year(aTime.DateTime().Year());
			
	// actualise rules
	delete iActualisedRulesCache;
	iActualisedRulesCache = NULL;
	iActualisedRulesCache = CVTzActualisedRules::NewL(year, year);
	TRAPD(leaveCode,GetActualisedRulesL(*iActualisedRulesCache));
	if(leaveCode != KErrNone)
		{
		delete iActualisedRulesCache;
		iActualisedRulesCache = NULL;
		User::LeaveIfError(leaveCode);		
		}
	
	return iActualisedRulesCache->GetOffsetFromRuleL(aTime, aTimeRef);;
	}

/**
Creates a copy of these timezone rules.

@capability None
@return A pointer to the CTzRules copy.
@publishedAll
@released
*/
EXPORT_C CTzRules* CTzRules::CloneL() const
	{
	CTzRules* newRules = CTzRules::NewL();
	CleanupStack::PushL(newRules);
	
	newRules->SetStartYear(iStartYear);
	newRules->SetEndYear(iEndYear);
	newRules->SetInitialStdTimeOffset(iInitialStdTimeOffset);
	
	TInt numRules = Count();
	for (TInt i = 0; i < numRules; i++)
		{
		newRules->AddRuleL(iRules[i]);
		}
	
	CleanupStack::Pop(newRules);
	return newRules;
	}

/** Copy a time zone rule to this time zone rule.

@capability None
@param aTzRule A rule to copy from.
@internalAll
@released
*/
EXPORT_C void CTzRules::CopyL(const CTzRules& aTzRule)
	{
	SetStartYear(aTzRule.iStartYear);
	SetEndYear(aTzRule.iEndYear);
	SetInitialStdTimeOffset(aTzRule.iInitialStdTimeOffset);
	iRules.Reset();	
	const TInt numRules = aTzRule.Count();
	for (TInt i = 0; i < numRules; i++)
		{
		AddRuleL(aTzRule.iRules[i]);
		}
	}
/**
Compares two sets of timezone rules.

@capability None
@param aRules The timezone rules to compare with.
@return ETrue if the rules are identical. EFalse if not.
@publishedAll
@released
*/
EXPORT_C TBool CTzRules::IsEqualTo(const CTzRules& aRules) const
	{
	if (iStartYear != aRules.StartYear() ||
		iEndYear != aRules.EndYear() ||
		iInitialStdTimeOffset != aRules.InitialStdTimeOffset() ||
		Count() != aRules.Count())
		{
		return EFalse;
		}

	const TInt KRuleCount(Count());
	for (TInt i = 0; i < KRuleCount; i++)
		{
		TTzRule rule1 = const_cast<CTzRules&>(*this)[i];
		TTzRule rule2 = const_cast<CTzRules&>(aRules)[i];
		
		if (rule1.iFrom != rule2.iFrom ||
			rule1.iTo != rule2.iTo ||
			rule1.iOldLocalTimeOffset != rule2.iOldLocalTimeOffset ||
			rule1.iNewLocalTimeOffset != rule2.iNewLocalTimeOffset ||
			rule1.iMonth != rule2.iMonth ||
			rule1.iDayRule != rule2.iDayRule ||
			rule1.iDayOfMonth != rule2.iDayOfMonth ||
			rule1.iDayOfWeek != rule2.iDayOfWeek ||
			rule1.iTimeReference != rule2.iTimeReference ||
			rule1.iTimeOfChange != rule2.iTimeOfChange )
			{
			return EFalse;
			}
		}
	return ETrue;
	}
	
/**
Converts between local (wall-clock) and UTC times.

@return KErrNone if successful, otherwise KErrNotSupported or KErrNotFound
@param aRules The actualised rules to use.
@param aTime The time to convert, specified by aTimerRef if the time is in UTC or local time. On return, 
       it will contain the converted time from UTC to local time or vice versa, depending on aTimerRef's value. 
@param aTimerRef What aTime is expressed in.
@publishedAll
@released
*/
EXPORT_C TInt CTzRules::ConvertTime(CVTzActualisedRules& aRules, TTime& aTime, TTzTimeReference aTimeRef) const
	{
	TInt results = KErrNone;
	TInt offset = 0;

#ifdef _DEBUG
	TDateTime aDateTime = aTime.DateTime();
#endif
	
	TRAP( results, offset = aRules.GetOffsetFromRuleL(aTime, aTimeRef));
	
	if (results == KErrNone)
		{
		switch (aTimeRef)
			{
			case ETzUtcTimeReference:
				aTime += TTimeIntervalMinutes(offset);
				break;
			case ETzWallTimeReference:
				aTime -= TTimeIntervalMinutes(offset);
				break;
			default:
				results = KErrNotSupported;
				break;
			}
		}

	return (results);
	}

//
// TVTzActualisedRule
//

/** 
Constructor for an actualised rule (local time change).

@param aTimeOfChange Time of local time change
@param aNewOffset New UTC offset in minutes 
@param aTimeReference Time reference
@publishedAll
@released
*/
EXPORT_C TVTzActualisedRule::TVTzActualisedRule(TTime aTimeOfChange, TInt aNewOffset, TTzTimeReference aTimeReference) :
	iTimeOfChange(aTimeOfChange),
	iNewOffset(aNewOffset),
	iTimeReference(aTimeReference)
	{
	}


/** 
Default constructor for an actualised rule (local time change).
@publishedAll
@released
*/
EXPORT_C TVTzActualisedRule::TVTzActualisedRule() :
	iTimeOfChange(0),
	iNewOffset(0),
	iTimeReference(ETzUtcTimeReference)
	{
	}

/** 
Assignment operator for an actualised rule.
@param aRule Actualised rule
@publishedAll
@released
*/
EXPORT_C void TVTzActualisedRule::operator=(TVTzActualisedRule aRule)
	{
	iTimeOfChange = aRule.iTimeOfChange;
	iNewOffset = aRule.iNewOffset;
	iTimeReference = aRule.iTimeReference;
	}

/**
Order actualised rules by time of local time change.
@internalComponent
@released
*/
TInt TVTzActualisedRule::Order(const TVTzActualisedRule& aLeft, const TVTzActualisedRule& aRight)
	{
	if (aLeft.iTimeOfChange < aRight.iTimeOfChange)
		{
		return -1;
		}
	if (aLeft.iTimeOfChange == aRight.iTimeOfChange)
		{
		return 0;
		}
	return 1;
	}


//
// CVTzActualisedRules
//

/**
Factory method for CVTzActualisedRules objects.
Creates a new instance of CVTzActualizedRules - an array of actualised rules (TVTzActualisedRule).

The array of rules can be populated later by invoking CTzRules::GetActualisedRulesL() method.

Rules in the array are sorted ascending by rule's start time (TVTzActualisedRule.iTimeOfChange);
The very first rule in the array, if present, defines "default standard rule" for the time zone 
indicating the STD time offset from UTC, in effect from the start of the first year (aStartYear).

@param aStartYear, aEndYear	- specify range of years for to which actualised rules apply.
@return 	A pointer to the newly created rules.
@panic RTz 10 if aStartYear is larger than aEndYear.
@publishedAll
@released
*/
EXPORT_C CVTzActualisedRules* CVTzActualisedRules::NewL(TInt aStartYear, TInt aEndYear)
	{
	__ASSERT_ALWAYS(aStartYear <= aEndYear, RTz::Panic(RTz::EPanicInvalidArgument));  

	CVTzActualisedRules* self = new (ELeave) CVTzActualisedRules(aStartYear, aEndYear);
	return self;
	}

/**
Constructor.
@internalComponent
@released
*/
CVTzActualisedRules::CVTzActualisedRules(TInt aStartYear, TInt aEndYear) :
	iStartYear(aStartYear),
	iEndYear(aEndYear),
	iRules(KVTzRulesGranularity)
	{
	}

/**
Default constructor.
@internalComponent
@released
*/
CVTzActualisedRules::CVTzActualisedRules() :
	iRules(KVTzRulesGranularity)
	{
	}

/**
Destructor.
@publishedAll
@released
*/
EXPORT_C CVTzActualisedRules::~CVTzActualisedRules()
	{
	iRules.Reset();
	}

/**
Returns specified TVTzActualisedRule.

@param aIndex Index of a rule
@return A reference to TVTzActualised rule at index aIndex.
@panic RTz 4 if aIndex is out of bounds.
@publishedAll
@released
*/
EXPORT_C TVTzActualisedRule& CVTzActualisedRules::operator[](TInt aIndex) const
	{
	// prevent array bounds error
	__ASSERT_ALWAYS( (aIndex < iRules.Count() && aIndex >= 0), RTz::Panic(RTz::EPanicRulesIndexOutofRange));
	
	return const_cast<TVTzActualisedRule&>(iRules[aIndex]);
	}

/** 
Add an actualised rule to the collection.

@param aRule An actualised rule to be added into the collection.
@publishedAll
@released
*/
EXPORT_C void CVTzActualisedRules::AddRuleL(const TVTzActualisedRule& aRule)
	{
	TLinearOrder<TVTzActualisedRule> order(TVTzActualisedRule::Order);
	
	User::LeaveIfError(iRules.InsertInOrderAllowRepeats(aRule, order));
	}

/**
Returns number of rules currently held in array of rules.

@return Number of rules.
@publishedAll
@released
*/
EXPORT_C TInt CVTzActualisedRules::Count() const
	{
	return iRules.Count();
	}

/**
Returns min year of the year range the CVTzActualisedRules object describes the rules for.
It is guaranteed that the array of rules does not contain any rules for years before that min year.

@return 	Start year (e.g. 1998).
@publishedAll
@released
*/
EXPORT_C TInt CVTzActualisedRules::StartYear() const
	{
	return iStartYear;
	}

/**
Returns max year of the year range the CVTzActualisedRules object describes the rules for.

@return 	End year (e.g. 2005).
@publishedAll
@released
*/
EXPORT_C TInt CVTzActualisedRules::EndYear() const
	{
	return iEndYear;
	}

/**
Tells if Daylight Savings Applies for the current time zone at the current time
@return EFalse if Daylight Savings does not apply (winter time) and ETrue if Daylight Savings does apply
@param aTime The time of interest given in UTC.
@panic RTz 5 if aTime is not covered by the current set of time zone rules.
@panic RTz 7 if one of the time zone rules uses standard time reference.
@publishedAll
@released
*/
EXPORT_C TBool CVTzActualisedRules::IsDaylightSavingOn(TTime& aTime) const
	{
	const TInt count = iRules.Count();

	// ensure that there are rules
	__ASSERT_ALWAYS((count > 0), RTz::Panic(RTz::EPanicTimeNotCoveredByRules));

	// initialisation separated from declaration to avoid arm4 'unused variable warning'
	TDateTime dateTime;
	dateTime = aTime.DateTime();

	// ensure that there are rules defined for the time in question
	__ASSERT_ALWAYS(dateTime.Year() >= StartYear() && dateTime.Year() <= EndYear(), 
					RTz::Panic(RTz::EPanicTimeNotCoveredByRules));
	
	// the illustration below is for a location in the northern hemisphere
	// the opposite applies to southern hemisphere locations
	//
	//          --------|--------|---------
	// season:  winter    summer    winter
	// rule:           [1]      [2]   
	//
	// the rules are sorted by iTimeOfChange.
	// we start searching for the applicable rule with time 0.
	//
	// general dst example from southern hemisphere:
	//	America/Sao_Paulo
	//	UTC offset (dst off): -180min (-3h)	
	//	UTC offset (dst on):  -120min (-2h)		
	//	starting dst (Nov) 4/11/2007 at 00:00:00
	//	utc time:  local time:   expected dst status:
	//	02:00       23:00        off (-180)
	//	02:58       23:58        off (-180)
	//	02:59       23:59        off (-180)
	//	n/a         (00:00)      n/a
	//	03:00       01:00        on (-120)
	//	03:01       01:01        on (-120)
	//	04:00       02:00        on (-120)
	//	05:00       03:00        on (-120)	
	//
	//	ending dst (Feb) 24/02/2008: 00:00:00
	//	utc time:  local time:   expected dst status:
	//	01:00       23:00        on (-120)
	//	01:58       23:58        on (-120)
	//	01:59       23:59        on (-120)
	//	n/a	        (00:00)      n/a
	//	02:00       23:00        off (-180)
	//	02:58       23:58        off (-180)
	//	02:59       23:59        off (-180)
	//	03:00       00:00        off (-180)
	//	04:00       01:00        off (-180)
	//	05:00       02:00        off (-180)
	//   
	TBool dstOn(EFalse);
	TTime lastTimeOfChange(0);
    TInt lowestOffset = 0;
    TBool firstMatchFound = EFalse;
    
    // Retrieve lowest applicable time offset.
	// This is used to determine if DST is on.
	for (TInt i = 0; i < count; ++i)
		{
		const TVTzActualisedRule& rule = iRules[i];
		
		// Only check for the same year as requested (aTime)
		if (rule.iTimeOfChange.DateTime().Year() == dateTime.Year())
   		    {
   		    if (!firstMatchFound)
	   		    {
	   		    lowestOffset = rule.iNewOffset;
	   		    firstMatchFound = ETrue;	
	   		    }
   		    
			if (rule.iNewOffset < lowestOffset)
				{
				lowestOffset = rule.iNewOffset;
				}	
			}	
		}
	
	if (firstMatchFound) // lowest offset has been found in year of interest
	    {
	    // there are only a few actualised rules
	    // so simple sequential search will suffice
	    // Times may be expressed in utc, or local
	    // so we need to compare like for like
		for (TInt i = 0; i < count; ++i)
			{
			const TVTzActualisedRule& rule = iRules[i];
			TInt oldOffset = (i>0) ?  iRules[i-1].iNewOffset : rule.iNewOffset;
 			
			__ASSERT_ALWAYS(rule.iTimeReference!=ETzStdTimeReference, RTz::Panic(RTz::EPanicUnsupportedTimeReference));
		
			TTime timeOfChange(rule.iTimeOfChange);
			
			// convert to utc
		    if (rule.iTimeReference == ETzWallTimeReference)
				{
				timeOfChange -= (TTimeIntervalMinutes)oldOffset;				
				}

		#if defined(_DEBUG)
			// initialisation separated from declaration to avoid arm4 'unused variable warning'
			TDateTime dateTimeofChange;
			dateTimeofChange = timeOfChange.DateTime();
		#endif

			if (aTime >= timeOfChange) // continue until finding the rule which applies to the current time
				{
				if (timeOfChange > lastTimeOfChange) // a double check that rules are in order
					{
					// Check to see if the lowest offset is greater than the current
					// offset. If it is, then this implies DST is on.
					if (rule.iNewOffset > lowestOffset)
						{
						dstOn = ETrue;  // daylight savings is on
						}
					else
						{
						dstOn = EFalse; // daylight savings is off
						}
					
					lastTimeOfChange = timeOfChange;
					}
				}
			else
				{
				// found the matching rule on previous run through loop
				break;
				}
			}
	    }
	return(dstOn);
	}
	
/**
Receives a time. Finds out which of these rules applies at the received time
and returns the offset in effect at the specified time.

Leaves with KErrNotFound, if it doesn't find the rule 
(i.e. aTime is earlier than the very first time in CVTzActualisedRules).

@param	aUserTime		- time of interest
@param	aUserTimeRef	- time reference for the aUserTime (UTC or wall-time)
					note: ETzStdTimeReference for aTimeRef is not supported, will 
					panic with RTz::EPanicUnsupportedTimeReference.

@return	TInt		  - offset from UTC (in minutes).
 */
EXPORT_C TInt CVTzActualisedRules::GetOffsetFromRuleL(const TTime& aUserTime, TTzTimeReference aUserTimeRef) const
	{
	// The first rule with iTimeOfChange <= aUserTime that is found going backwards in the array is the one that
	// applies at the requested aUserTime.
	__ASSERT_ALWAYS(aUserTimeRef!=ETzStdTimeReference, RTz::Panic(RTz::EPanicUnsupportedTimeReference));

	// we traverse the array of rules, starting from the last one, until we find the
	// rule that has a start time just before aTime
	TBool ruleFound = EFalse;
	TInt resultOffset = 0;
	
	TLinearOrder<TVTzActualisedRule> linearOrder(TVTzActualisedRule::Order);
	TVTzActualisedRule findActualisedRule(aUserTime, 0, aUserTimeRef);
	TInt position;
	TInt error = iRules.FindInOrder(findActualisedRule, position, linearOrder);
	
	for (TInt i(Min(position, iRules.Count()-1)); i >= 0; i--)
		{
		const TVTzActualisedRule& actRule = iRules[i];
		TInt oldOffset = (i>0) ? iRules[i-1].iNewOffset : actRule.iNewOffset;

		TTime rolloverTime = actRule.iTimeOfChange;
		TTzTimeReference rolloverTimeRef = actRule.iTimeReference;

    #if defined(_DEBUG)
		TDateTime dtRolloverTime = rolloverTime.DateTime();
		TDateTime dtUserTime     = aUserTime.DateTime();
    #endif
		
		TTimeIntervalMinutes diffMinutes;
		// Find out if rolloverTime is earlier than aUserTime
		if (CompareTimesWithRef(rolloverTime, rolloverTimeRef, aUserTime, aUserTimeRef, oldOffset, diffMinutes) <= 0)
			{// rolloverTime is now earlier than aUserTime, so we are done iterating backwards
			//  and aUserTime is now between this iteration and the last iterated rule.
			//  This means we need to use the current iNewOffset for the correct offset to give aUserTime on return.
			resultOffset = actRule.iNewOffset;			
			
            // If diffMinutes is less than difference between iNewOffset and oldOffset,              
            // then we are potentially in the "missing" hour, which should return the  
            // oldOffset instead of iNewOffset. 			
    		if (aUserTimeRef == ETzWallTimeReference &&
    		    oldOffset < actRule.iNewOffset &&
    		    diffMinutes.Int() < (actRule.iNewOffset-oldOffset))
				{
				// the missing hour - use oldOffset instead.
				// see general dst example in CVTzActualisedRules::IsDaylightSavingOn
				// why this is necessary.
				resultOffset = oldOffset;
				}
			
			ruleFound = ETrue;
			break;
			}
		}

	if (!ruleFound)
		{
		User::Leave(KErrNotFound);
		}
		
	return resultOffset;
	}
	
/**	

Compares two times with time references (TTzTimeReference).
TTzTimeReference is either ETzUtcTimeReference or ETzWallTimeReference.

If time references for both aRolloverTime and aUserTime are the same, 
then straight comparison is done. Otherwise aRolloverTime is converted to match
the time reference of aUserTime.

@param	aRolloverTime	 - the time of change from/to Daylight Savings Time (DST)
@param  aTimeRefRollover - the time reference used in aRolloverTime
@param	aUserTime		 - the time provided by the user to compare with 
@param  aTimeRefUser     - the time reference used for the user time
@param  aOldWallOffset   - the offset that is active before aRolloverTime happens

@return TInt		-1 if aRolloverTime is earlier than aUserTime;
					 0 if aRolloverTime is equal to aUserTime;
					 1 if aRolloverTime is later than aUserTime.	 
 */ 
TInt CVTzActualisedRules::CompareTimesWithRef(	
		TTime aRolloverTime, TTzTimeReference aTimeRefRollover, 
		TTime aUserTime,     TTzTimeReference aTimeRefUser,
		TInt aOldWallOffset, TTimeIntervalMinutes& aDiffMinutes) const
	{
	TInt result = 0;		

	// Always convert the Rollover time if it has not 
	// the same reference type as user time
	if ( aTimeRefRollover != aTimeRefUser)
		{
		// As they are different; if RolloverTime is of
		// wall time type, we need to convert it to Utc
		// and vice versa, to match aTimeRefUser
		if ( aTimeRefRollover == ETzWallTimeReference)
			{
			// Convert to utc time reference
			aRolloverTime -= TTimeIntervalMinutes (aOldWallOffset);
			}
		else // aTimeRefRollover is ETzUtcTimeReference
			{
			// Convert to wall time reference
			aRolloverTime += TTimeIntervalMinutes (aOldWallOffset);
 			}
		}

	#if defined(_DEBUG)
		TDateTime dtRollover = aRolloverTime.DateTime ();
		TDateTime dtUser     = aUserTime.DateTime ();
	#endif
		
	TInt err = aUserTime.MinutesFrom(aRolloverTime, aDiffMinutes);
	
	if (aRolloverTime < aUserTime)
		{
		result = -1;
 		}
	else if (aRolloverTime > aUserTime)
		{
		result = 1;
		}

	return result;
	}


	
