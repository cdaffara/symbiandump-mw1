// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "tzdbentities.h"
#include "ReadOnlyTzDb.h"
#include <vtzrules.h>

/**
This const must match the last TTzRuleDay enumeration in tzdefines.h.
@internalComponent
*/
const TInt KValidatedLastDayRule = ETzDayInLastWeekOfMonth;

/**
This const must match the last TTzTimeReference enumeration in tzdefines.h.
@internalComponent
*/
const TInt KValidatedLastTimeReference = ETzWallTimeReference;

//=============================================================================

CTzDbStringsTable* CTzDbStringsTable::NewL(const TTzStringsTable& aRegionsTable)
	{
	CTzDbStringsTable* self = new(ELeave) CTzDbStringsTable(aRegionsTable);
	return self;
	}

CTzDbStringsTable::CTzDbStringsTable(const TTzStringsTable& aRegionsTable)
	: iPersistedEntity(aRegionsTable)
	{
	}

TPtrC8 CTzDbStringsTable::GetStringL(TUint aReferenceToString)
	{
	const TInt KStringsBaseAddress = (TInt)&iPersistedEntity;
	const TInt KLengthAddress = KStringsBaseAddress + aReferenceToString;
	const TInt KStringAddress = KLengthAddress + 1; // length occupies 1 byte
	TUint8 length = *(reinterpret_cast<TUint8*>(KLengthAddress));
	TPtrC8 theString(reinterpret_cast<TUint8*>(KStringAddress),length);
	return theString;
	}

//=============================================================================

CTzDbRegionsTable* CTzDbRegionsTable::NewL(CReadOnlyTzDb& aReadOnlyTzDb, const TTzRegionsTable& aRegionsTable)
	{
	CTzDbRegionsTable* self = new(ELeave) CTzDbRegionsTable(aReadOnlyTzDb, aRegionsTable);
	return self;
	}

CTzDbRegionsTable::CTzDbRegionsTable(CReadOnlyTzDb& aReadOnlyTzDb, const TTzRegionsTable& aRegionsTable)
:	iReadOnlyTzDb(aReadOnlyTzDb), 
	iPersistedEntity(aRegionsTable)
	{
	}

//
// Looks for a region given its name.
// Returns the region wrapped in a CTzDbRegion object.
// Returns NULL if region is not found
//
CTzDbRegion* CTzDbRegionsTable::FindRegionL(const TDesC8& aRegionName)
	{
	const TInt KRegionCount = iPersistedEntity.iNumberOfRegions;
	TTzRegion* region(NULL);
	TPtrC8 name;
	TInt regionAddress = reinterpret_cast<TInt>(&iPersistedEntity) + sizeof(iPersistedEntity.iNumberOfRegions);
	for (TInt i = 0; i < KRegionCount; i++, regionAddress += sizeof(TTzRegion))
		{
		region = reinterpret_cast<TTzRegion*>(regionAddress);
		name.Set(iReadOnlyTzDb.GetStringL(region->iOffsetToRegionName));

		// compare both strings
		if (aRegionName.Compare(name) == 0) // found our region
			{
			return CTzDbRegion::NewL(iReadOnlyTzDb,*region);
			}
		}

	// if it gets here, it means that the region has not been found
	return NULL;
	}

//=============================================================================

CTzDbRegionalZonesTable* CTzDbRegionalZonesTable::NewL(CReadOnlyTzDb& aReadOnlyTzDb, const TTzRegionalZonesTable& aRegionalZonesTable)
	{
	CTzDbRegionalZonesTable* self = new(ELeave) CTzDbRegionalZonesTable(aReadOnlyTzDb, aRegionalZonesTable);
	return self;
	}

CTzDbRegionalZonesTable::CTzDbRegionalZonesTable(CReadOnlyTzDb& aReadOnlyTzDb, const TTzRegionalZonesTable& aRegionalZonesTable)
:	iReadOnlyTzDb(aReadOnlyTzDb), 
	iPersistedEntity(aRegionalZonesTable)
	{
	}

const TTzRegionalZoneIndex& CTzDbRegionalZonesTable::GetTRegionalZoneIndex(TUint aReference)
	{
	return * reinterpret_cast<TTzRegionalZoneIndex*> (aReference + reinterpret_cast<TInt>(&iPersistedEntity));
	}

//=============================================================================

CTzDbZonesTable* CTzDbZonesTable::NewL(CReadOnlyTzDb& aReadOnlyTzDb, const TTzZonesTable& aZonesTable, TInt aZonesDataBaseAddress)
	{
	CTzDbZonesTable* self = new(ELeave) CTzDbZonesTable(aReadOnlyTzDb, aZonesTable, aZonesDataBaseAddress);
	return self;
	}

CTzDbZonesTable::CTzDbZonesTable(CReadOnlyTzDb& aReadOnlyTzDb, const TTzZonesTable& aZonesTable, TInt aZonesDataBaseAddress)
:	iReadOnlyTzDb(aReadOnlyTzDb), 
	iPersistedEntity(aZonesTable),
	iZonesDataBaseAddress(aZonesDataBaseAddress)
	{
	}

CTzDbZone* CTzDbZonesTable::GetZoneL(TUint aReference)
	{
	return CTzDbZone::NewL(iReadOnlyTzDb, GetTZone(aReference));
	}

TTzZone& CTzDbZonesTable::GetTZone(TUint aReference)
	{
	return *reinterpret_cast<TTzZone*>(aReference + iZonesDataBaseAddress);
	}

//
// Performs a binary search in the zones table, looking for the zone with the
// supplied numeric ID.
//
// IMPORTANT: Assumes zones are sorted by LocationId in zones table.
//
CTzDbZone* CTzDbZonesTable::GetZoneByIdL(TUint aNumericId)
	{
	TTzZone* tzone(NULL);
	const TInt KZoneCount = iPersistedEntity.iNumberOfZones;
	TInt start = 0;
	TInt end = KZoneCount - 1;
	TInt mid;
	TBool found = EFalse;
	while(start <= end)
		{
		mid = (end + start) / 2;
		tzone = &GetTZone(iPersistedEntity.iOffsetsToZones[mid]);
		if (aNumericId < tzone->iLocationId)
			{
			end = mid - 1;
			}
		else if (aNumericId == tzone->iLocationId)
			{ 
			// found our zone
			found = ETrue;
			break;
			}
		else
			{
			start = mid + 1;
			}
		}

	if (found)
		{
		return CTzDbZone::NewL(iReadOnlyTzDb, *tzone);
		}
	else
		{
		return NULL;	
		}
	}

//=============================================================================

CTzDbLinksTable* CTzDbLinksTable::NewL(CReadOnlyTzDb& aReadOnlyTzDb, const TTzLinksTable& aLinksTable)
	{
	CTzDbLinksTable* self = new(ELeave) CTzDbLinksTable(aReadOnlyTzDb, aLinksTable);
	return self;
	}

CTzDbLinksTable::CTzDbLinksTable(CReadOnlyTzDb& aReadOnlyTzDb, const TTzLinksTable& aLinksTable)
:	iReadOnlyTzDb(aReadOnlyTzDb), 
	iPersistedEntity(aLinksTable)
	{
	}

CTzDbZone* CTzDbLinksTable::FindZoneL(const TDesC8& aLinkName)
	{
	const TInt KLinkCount = iPersistedEntity.iNumberOfLinks;
	TPtrC8 name;
	TTzLink* link(NULL);
	TInt linkAddress = reinterpret_cast<TInt>(&iPersistedEntity) + sizeof(iPersistedEntity.iNumberOfLinks);
	for (TInt i = 0; i < KLinkCount; i++, linkAddress += sizeof(TTzLink))
		{
		link = reinterpret_cast<TTzLink*>(linkAddress);
		name.Set(iReadOnlyTzDb.GetStringL(link->iOffsetToLinkName));
		if (!name.CompareC(aLinkName))
			{ // we found our link
			return iReadOnlyTzDb.GetZoneL(link->iOffsetToZone);
			}
		}

	// if it gets here, it means that the zone has not been found
	return NULL;
	}

//=============================================================================

CTzDbStdTimeAlignmentsTable* CTzDbStdTimeAlignmentsTable::NewL(const TTzStdTimeAlignmentsTable& aStdTimeAlignmentsTable)
	{
	CTzDbStdTimeAlignmentsTable* self = new(ELeave) CTzDbStdTimeAlignmentsTable(aStdTimeAlignmentsTable);
	return self;
	}

CTzDbStdTimeAlignmentsTable::CTzDbStdTimeAlignmentsTable(const TTzStdTimeAlignmentsTable& aStdTimeAlignmentsTable)
	: iPersistedEntity(aStdTimeAlignmentsTable)
	{
	}

TTzStdTimeAlignment& CTzDbStdTimeAlignmentsTable::GetTStdTimeAlignment(TUint aReference)
	{
	return *reinterpret_cast<TTzStdTimeAlignment*>(aReference + (TInt)&iPersistedEntity);
	}

//=============================================================================

CTzDbRuleSetsTable* CTzDbRuleSetsTable::NewL(CReadOnlyTzDb& aReadOnlyTzDb, const TTzRuleSetsTable& aRuleSetsTable)
	{
	CTzDbRuleSetsTable* self = new(ELeave) CTzDbRuleSetsTable(aReadOnlyTzDb, aRuleSetsTable);
	return self;
	}

CTzDbRuleSetsTable::CTzDbRuleSetsTable(CReadOnlyTzDb& aReadOnlyTzDb, const TTzRuleSetsTable& aRuleSetsTable)
	: iReadOnlyTzDb(aReadOnlyTzDb), iPersistedEntity(aRuleSetsTable)
	{
	}

CTzDbRuleSet* CTzDbRuleSetsTable::GetRuleSetL(TUint aReference)
	{
	return CTzDbRuleSet::NewL(iReadOnlyTzDb, GetTRuleSet(aReference));
	}

TTzRuleSet& CTzDbRuleSetsTable::GetTRuleSet(TUint aReference)
	{
	return * reinterpret_cast<TTzRuleSet*>(aReference + (TInt)&iPersistedEntity);
	}

//=============================================================================

CTzDbRuleUsesTable* CTzDbRuleUsesTable::NewL(const TTzRuleUsesTable& aRuleUsesTable)
	{
	CTzDbRuleUsesTable* self = new(ELeave) CTzDbRuleUsesTable(aRuleUsesTable);
	return self;
	}

CTzDbRuleUsesTable::CTzDbRuleUsesTable(const TTzRuleUsesTable& aRuleUsesTable)
	: iPersistedEntity(aRuleUsesTable)
	{
	}

TTzRuleUse& CTzDbRuleUsesTable::GetTRuleUse(TUint aReference)
	{
	return * reinterpret_cast<TTzRuleUse*> (aReference + (TInt)&iPersistedEntity);
	}

//=============================================================================

CTzDbRuleDefinitionsTable* CTzDbRuleDefinitionsTable::NewL(const TTzRuleDefinitionsTable& aRuleDefinitionsTable)
	{
	CTzDbRuleDefinitionsTable* self = new(ELeave) CTzDbRuleDefinitionsTable(aRuleDefinitionsTable);
	return self;
	}

CTzDbRuleDefinitionsTable::CTzDbRuleDefinitionsTable(const TTzRuleDefinitionsTable& aRuleDefinitionsTable)
	: iPersistedEntity(aRuleDefinitionsTable)
	{
	}

const TTzRuleDefinition& CTzDbRuleDefinitionsTable::GetRuleDefinition(TUint aReference)
	{
	return * reinterpret_cast<TTzRuleDefinition*>(aReference + (TInt)&iPersistedEntity);
	}

//=============================================================================

CTzDbRegion* CTzDbRegion::NewL(CReadOnlyTzDb& aReadOnlyTzDb, const TTzRegion& aRegion)
	{
	CTzDbRegion* self = new(ELeave) CTzDbRegion(aReadOnlyTzDb,aRegion);
	return self;
	}

CTzDbRegion::CTzDbRegion(CReadOnlyTzDb& aReadOnlyTzDb, const TTzRegion& aRegion)
:	iReadOnlyTzDb(aReadOnlyTzDb),
	iPersistedEntity(aRegion)
	{
	}

CTzDbZone* CTzDbRegion::FindZoneL(TUint aCityNameReference)
	{
	TTzRegionalZoneIndex* zoneIndex = const_cast<TTzRegionalZoneIndex*>(&iReadOnlyTzDb.GetTRegionalZoneIndex(iPersistedEntity.iOffsetToRegionalZoneIndex));
	const TInt KZoneCount = zoneIndex->iNumberOfZones;
	TTzZone* tzone(NULL);
	for (TInt i = 0; i < KZoneCount; i++)
		{
		tzone = const_cast<TTzZone*>(&iReadOnlyTzDb.GetTZone(zoneIndex->iOffsetsToZones[i]));
		if (tzone->iOffsetToZoneName == aCityNameReference)
			{ // found our zone
			return CTzDbZone::NewL(iReadOnlyTzDb,*tzone);
			}
		}

	// if it gets here, it means that the zone has not been found
	return NULL;
	}

CTzDbZone* CTzDbRegion::FindZoneL(const TDesC8& aCityName)
	{
	TTzZone* tzone(NULL);
	TTzRegionalZoneIndex* zoneIndex = const_cast<TTzRegionalZoneIndex*>(&iReadOnlyTzDb.GetTRegionalZoneIndex(iPersistedEntity.iOffsetToRegionalZoneIndex));
	const TInt KZoneCount = zoneIndex->iNumberOfZones;
	TPtrC8 name;

	TBool found = EFalse;
	TInt start = 0;
	TInt end = KZoneCount - 1;
	TInt mid;

	// perform a binary search for aCityName among the zones listed in the zone index. This assumes
	// that zones are sorted alphabetically in the zone index.
	while (start <= end)
		{
		mid = (end + start) / 2;
		tzone = const_cast<TTzZone*>(&iReadOnlyTzDb.GetTZone(zoneIndex->iOffsetsToZones[mid]));
		
		// compare this zone's name with the name we've been given as a parameter
		// use strict comparison method Compare, as CompareC may cause the search to fail:
		// for example, Stanley will come BEFORE St_Helens using the standard collation method in Symbian OS,
		// but it will have been placed AFTER St_Helens by std::sort in he TZ Compiler. This would result
		// in this binary search to fail.
		name.Set(iReadOnlyTzDb.GetStringL(tzone->iOffsetToZoneName));
		if (aCityName.Compare(name) < 0)
			{
			end = mid - 1;
			}
		else if (aCityName.Compare(name) == 0) 
			{ // found our zone
			found = ETrue;
			break;
			}
		else if (aCityName.Compare(name) > 0)
			{
			start = mid + 1;
			}
		}

	if (found)
		{
		return CTzDbZone::NewL(iReadOnlyTzDb,*tzone);
		}
	else
		{
		return NULL;
		}
	}

//=============================================================================

CTzDbZone* CTzDbZone::NewL(CReadOnlyTzDb& aReadOnlyTzDb, const TTzZone& aZone)
	{
	CTzDbZone* self = new(ELeave) CTzDbZone(aReadOnlyTzDb,aZone);
	return self;
	}

CTzDbZone::CTzDbZone(CReadOnlyTzDb& aReadOnlyTzDb, const TTzZone& aZone)
:	iReadOnlyTzDb(aReadOnlyTzDb),
	iPersistedEntity(aZone)
	{
	}

void CTzDbZone::GetRulesL(CTzRules& aRules)
	{	
	// start and end years of interest
	TInt firstYearOfInterest = aRules.StartYear();
	TInt lastYearOfInterest = aRules.EndYear();

	// record last instant of end year, in order to see if we need to get another time alignment
	TDateTime endDateTimeOfInterest(lastYearOfInterest,EDecember,30,23,59,59,0);
	TTime endTimeOfInterest(endDateTimeOfInterest);

	// initialise start time to beginning of startYear (UTC)
	TDateTime startDateTime(firstYearOfInterest,EJanuary,0,0,0,0,0);
	TTime startTime(startDateTime);
	TTime staEndTime;
	TDateTime staEndDateTime;
	TTzTimeReference staTimeReference(ETzUtcTimeReference);

	// the following variables are used to iterate through the list of standard time alignments for this location
	TInt taCount = iPersistedEntity.iNumberOfStdTimeAlignments;
	TInt tmpOffset;
	TTzStdTimeAlignment* ta(NULL);
	TTzStdTimeAlignment* prevTa(NULL);
	CTzDbStdTimeAlignment* sta = NULL;

	// the standard time offset at the beginning of the period of interest has to be set in aTzRules
	TBool initialOffsetHasBeenSet = EFalse;
	
	// Iterate through std time alignments until we find the one that contains our date of interest.
	// Get the rules for this time alignment, up to the end of the time alignment or the end of our 
	// period of interest (whichever comes first).
	// If the period of interest is covered by only one time alignment, finish and return.
	// If the period of interest is covered by more than one time alignment, repeat procedure with 
	// next time alignment(s) until we reach the end of the period of interest
	for (TInt i = 0; (i < taCount) && (startTime < endTimeOfInterest ); i++)
		{
		// get a standard time alignment

		prevTa = ta; // this will be needed to eventually wrap the time alignment in a CTzDbStdTimeAlignment object
		tmpOffset = iPersistedEntity.iOffsetsToTimeAlignments[i];
		ta = const_cast<TTzStdTimeAlignment*>(&iReadOnlyTzDb.GetTStdTimeAlignment(tmpOffset));

		// check if our time of interest is within this time alignment
		if (IsTimeInStdTimeAlignment(*ta,	startDateTime.Year(),
											startDateTime.Month(),
											startDateTime.Day(),
											startDateTime.Hour(),
											startDateTime.Minute(),
											staTimeReference) )
			{
			// wrap the time alignment in a CTzDbStdTimeAlignment object
			sta = CTzDbStdTimeAlignment::NewL(iReadOnlyTzDb,*ta,prevTa);
			if (sta)
				{
				CleanupStack::PushL(sta); // push #1 - sta
				
				if (initialOffsetHasBeenSet == EFalse)
					{
					aRules.SetInitialStdTimeOffset(ta->iUtcOffset);
					initialOffsetHasBeenSet = ETrue;
					}
					
				// staStartDateTime, staEndTime are passed by reference and their values
				// are updated in GetRulesL
				sta->GetRulesL(aRules,startDateTime,staEndDateTime);

				// The time reference to calculate the start of the next time alignment is the time reference
				// in which the end time of this time alignment is expressed 
				staTimeReference = static_cast<TTzTimeReference>(sta->UntilTimeReference());
				CleanupStack::PopAndDestroy(sta); // pop #1 - sta

				staEndTime = staEndDateTime;
				if (staEndTime == Time::MaxTTime())
					{
					startTime = staEndTime; // this will cause loop to finish
					}
				else
					{
					// move just after end of time alignment
					startTime = staEndTime + static_cast<TTimeIntervalSeconds>(1);
					startDateTime = startTime.DateTime();
					}
				}
			} // if IsTimeInStdTimeAlignment(...)
		} // for
			
	}
	
TBool CTzDbZone::IsTimeInStdTimeAlignment(const TTzStdTimeAlignment& aStdTa, TInt aYear, TInt aMonth, TInt aDay, TInt aHour, TInt aMinute, TTzTimeReference aTimeRef)
	{
	TInt found = EFalse;

	if (aYear > (TInt)KMaxTUint16)
		{
		aYear = KMaxTUint16; // KMaxTUint16 (0xFFFF) is the value given to year when the end of a time alignment is unknown
		}

	if (aStdTa.iUntilYear > aYear)
		{
		found = ETrue;
		}
	else if (aStdTa.iUntilYear == aYear)
		{
		if ( (aStdTa.iUntilMonth > aMonth) || (aStdTa.iUntilMonth == KMaxTUint8) )
			{
			found = ETrue;
			}
		else if (aStdTa.iUntilMonth == aMonth)
			{
			// evaluate the day and time the time alignment ends 
			// and see if the received time falls in or out of the t.a.
			TTime ourTime;

			if (aYear >= (TInt)KMaxTUint16)
				{
				ourTime = Time::MaxTTime();
				}
			else
				{
				TDateTime ourDateTime(aYear,
									(TMonth)aMonth,
									aDay,
									aHour,
									aMinute,
									0,
									0);

				ourTime = ourDateTime;

				// convert ourTime to UTC
				if (aTimeRef == ETzStdTimeReference)
					{
					ourTime -= static_cast<TTimeIntervalMinutes>(aStdTa.iUtcOffset);
					}
				}

			TTime endTime;
			if (aStdTa.iUntilYear == KMaxTUint16)
				{
				endTime = Time::MaxTTime();
				}
			else
				{
				TDateTime endDateTime(aStdTa.iUntilYear,
									(TMonth)aStdTa.iUntilMonth,
									aStdTa.iUntilDayOfMonth,
									(aStdTa.iUntilTimeInMinutes / 60),
									(aStdTa.iUntilTimeInMinutes % 60),
									0,
									0);

				endTime = endDateTime;

				// convert endTime to UTC
				if (aTimeRef == ETzStdTimeReference)
					{
					endTime -= static_cast<TTimeIntervalMinutes>(aStdTa.iUtcOffset);
					}
				}
			
			// and finally compare both times
			if (endTime >= ourTime)
				{
				found = ETrue;
				}
			}
		} // else if (aStdTa->iUntilYear == aYear)

	return found;
	}
//
// Traverses the collection of Std Time Alignments for the current zone, looking for the one that 
// matches the given time.
// Returns a CTzDbStdTimeAlignment object.
//
CTzDbStdTimeAlignment* CTzDbZone::FindStdTimeAlignmentL(TInt aYear, TInt aMonth, TInt aDay, TInt aHour, TInt aMinute, const TTzTimeReference aTimeRef)
	{
	TTzStdTimeAlignment* ta(NULL);
	TTzStdTimeAlignment* prevTa(NULL);
	TInt taCount = iPersistedEntity.iNumberOfStdTimeAlignments;
	TBool found = EFalse;
	TInt tmpOffset = 0;
	for (TInt i = 0; (i < taCount) && (!found); i++)
		{
		prevTa = ta;
		tmpOffset = iPersistedEntity.iOffsetsToTimeAlignments[i];
		ta = const_cast<TTzStdTimeAlignment*>(&iReadOnlyTzDb.GetTStdTimeAlignment(tmpOffset));
		
		found = IsTimeInStdTimeAlignment(*ta,aYear,aMonth,aDay,aHour,aMinute,aTimeRef);
		}
		
	if (found)
		{
		return CTzDbStdTimeAlignment::NewL(iReadOnlyTzDb, *ta, prevTa);
		}
	else
		{
		return NULL;
		}
	}

HBufC8* CTzDbZone::GetFullZoneNameLC()
	{
	HBufC8* fullName = HBufC8::NewL(KMaxTimeZoneIdSize);
	CleanupStack::PushL(fullName);
	TPtr8 fullNamePtr(fullName->Des() );
	fullNamePtr.Append(iReadOnlyTzDb.GetStringL(iPersistedEntity.iOffsetToRegionName));
	if (fullNamePtr.Length() > 0)
		{
		// if the region name is empty (e.g. zone "CET"), don't add the slash
		fullNamePtr.Append('/');
		}
	fullNamePtr.Append(iReadOnlyTzDb.GetStringL(iPersistedEntity.iOffsetToZoneName));

	return fullName;
	}

//========================================================================

CTzDbStdTimeAlignment* CTzDbStdTimeAlignment::NewL(CReadOnlyTzDb& aReadOnlyTzDb, const TTzStdTimeAlignment& aTimeAlignment, TTzStdTimeAlignment* aPrevTimeAlignment)
	{
	CTzDbStdTimeAlignment* self = new(ELeave) CTzDbStdTimeAlignment(aReadOnlyTzDb,aTimeAlignment,aPrevTimeAlignment);
	return self;		
	}

CTzDbStdTimeAlignment::CTzDbStdTimeAlignment(CReadOnlyTzDb& aReadOnlyTzDb, 
							const TTzStdTimeAlignment& aTimeAlignment, TTzStdTimeAlignment* aPrevTimeAlignment)
	: iReadOnlyTzDb(aReadOnlyTzDb)
	{
	iPersistedEntity = aTimeAlignment;
	iPrevTimeAlignment = aPrevTimeAlignment;
	}

CTzDbStdTimeAlignment::~CTzDbStdTimeAlignment()
	{
	}

void CTzDbStdTimeAlignment::GetRulesL(CTzRules& aRules, const TDateTime& aStartDateTime, TDateTime& aEndDateTime)
	{
	// get the ruleSet for this time alignment
	CTzDbRuleSet* ruleSet = iReadOnlyTzDb.GetRuleSetL(iPersistedEntity.iOffsetToRuleSet);
	if (ruleSet)
		{
		CleanupStack::PushL(ruleSet); // PUSH #1 - RULESET

		// get end time of time alignment
		TTime endTime;
		CalculateEndTime(endTime);
		if (endTime == Time::MaxTTime())
			{
			aEndDateTime.SetYear(KMaxTUint16);
			}
		else
			{
			aEndDateTime = endTime.DateTime();
			}
	
		TInt firstYearOfInterest = (aRules.StartYear() >= aStartDateTime.Year()) ? aRules.StartYear() : aStartDateTime.Year();
		TInt lastYearOfInterest  = (aRules.EndYear() <= aEndDateTime.Year()) ? aRules.EndYear() : aEndDateTime.Year();
		CTzRules* newRules = CTzRules::NewL(firstYearOfInterest, lastYearOfInterest);
		CleanupStack::PushL(newRules); // PUSH #2 - NEWRULES
		
		ruleSet->GetRulesL(*newRules,iPersistedEntity.iUtcOffset,aStartDateTime,aEndDateTime);
		
		// the new rules obtained for the current std time alignment must be merged into 
		// the global rule collection
		AddRulesToCollectionL(aRules,*newRules);
	
		CleanupStack::PopAndDestroy(2, ruleSet); // POP #2,#1 - NEWRULES, RULESET
		}
	}

void CTzDbStdTimeAlignment::AddRulesToCollectionL(CTzRules& aRuleCollection, CTzRules& aNewRules)
	{
	TTzRule* tRule(NULL);
	TVTzActualisedRule tActRule;
	TInt count = aNewRules.Count();
	for (TInt i = 0; i < count; i++)
		{
		TBool ruleAdded = EFalse;
		tRule = &(aNewRules[i]);
		TInt basicRuleFromYear = tRule->iFrom.iTime.DateTime().Year();
		TInt basicRuleToYear = tRule->iTo.iTime.DateTime().Year();
		
		if ( (iPrevTimeAlignment != NULL) 
			&& (basicRuleFromYear <= iPrevTimeAlignment->iUntilYear)
			&& (basicRuleToYear >= iPrevTimeAlignment->iUntilYear) )
			{ // the rule is in effect during the first year of the time alignment
			// actualise rule for this year and remove it if it falls outside the time alignment
			tActRule = tRule->Actualise(iPrevTimeAlignment->iUntilYear);
			
			// work out start time of time alignment (i.e. end time of the prev. alignment)
				
			TDateTime staStartDateTime(	iPrevTimeAlignment->iUntilYear,
										(TMonth)iPrevTimeAlignment->iUntilMonth,
										iPrevTimeAlignment->iUntilDayOfMonth,
										iPrevTimeAlignment->iUntilTimeInMinutes/60, // hour
										iPrevTimeAlignment->iUntilTimeInMinutes%60, // minute
										0,0); // second, ms
			
			// make sure times are expressed in the same reference (UTC or Local)
			if (tActRule.iTimeReference != iPrevTimeAlignment->iUntilTimeReference)
				{			
				if (iPrevTimeAlignment->iUntilTimeReference == ETzWallTimeReference)
					{ // convert sta start time to UTC
					TTime staTime(staStartDateTime);
					staTime -= (TTimeIntervalMinutes)(iPrevTimeAlignment->iUtcOffset);
					staStartDateTime = staTime.DateTime();
					}
				else if (iPrevTimeAlignment->iUntilTimeReference == ETzUtcTimeReference)
					{ // convert time of change to UTC
					tActRule.iTimeOfChange -= (TTimeIntervalMinutes)tRule->iOldLocalTimeOffset;
					}
				}
			
			// add rule to collection only if its time of change falls within the time alignment
			if (tActRule.iTimeOfChange >= staStartDateTime)
				{
				aRuleCollection.AddRuleL(*tRule);
				ruleAdded = ETrue;
				}
			else 
				{
				TDateTime tActRuleDt = tActRule.iTimeOfChange.DateTime();
				/*
				Fix for INC117764:
				Update the rule's month and daytime information only if the rule starts and ends in the same year
				otherwise just add them to the collection.
				For example:
				Iqaluit rules:
				-6:00	Canada	C%sT	2000 Oct 29 2:00
				-5:00	Canada	E%sT

				Rule	Canada	1974	2006	-	Oct	lastSun	2:00	0	S
				Rule	Canada	1987	2006	-	Apr	Sun>=1	2:00	1:00	D
				Rule	Canada	2007	max	-	Mar	Sun>=8	2:00	1:00	D
				Rule	Canada	2007	max	-	Nov	Sun>=1	2:00	0	S
				Before this fix, In such a case for all the rules between 2000 and 2006 would be overwritten with start month as 10 whereas they
				should be alternating between October and April till 2006.
				*/
				if ((tActRuleDt.Year() == staStartDateTime.Year()) && (basicRuleFromYear == basicRuleToYear) )
					{
					tRule->iMonth = staStartDateTime.Month();
					tRule->iDayOfMonth = staStartDateTime.Day();
					tRule->iDayRule = ETzFixedDate;
					aRuleCollection.AddRuleL(*tRule);
					ruleAdded = ETrue;
					}
				else if (tActRuleDt.Year() == staStartDateTime.Year())
					{
					aRuleCollection.AddRuleL(*tRule);
					ruleAdded = ETrue;	
					}
				}
			}
		else if (  
					(tRule->iFrom.iTime.DateTime().Year() <= iPersistedEntity.iUntilYear)
				 	&&(tRule->iTo.iTime.DateTime().Year() >= iPersistedEntity.iUntilYear) 
				 )
			{ 
			// the rule is in effect during the last year of the time alignment
			tActRule = tRule->Actualise(iPersistedEntity.iUntilYear);
			
			TDateTime staEndDateTime(iPersistedEntity.iUntilYear, (TMonth)iPersistedEntity.iUntilMonth, 
									 iPersistedEntity.iUntilDayOfMonth, 
									 iPersistedEntity.iUntilTimeInMinutes / 60, // hour
									 iPersistedEntity.iUntilTimeInMinutes % 60, // minute
									 0,0); // second, ms
									 
			if (tActRule.iTimeReference != iPersistedEntity.iUntilTimeReference)
				{
				if (iPersistedEntity.iUntilTimeReference == ETzWallTimeReference)
					{
					TTime staTime(staEndDateTime);
					staTime -= (TTimeIntervalMinutes)iPersistedEntity.iUtcOffset;
					staEndDateTime = staTime.DateTime();
					}
				else if (iPersistedEntity.iUntilTimeReference == ETzUtcTimeReference)
					{
					tActRule.iTimeOfChange -= (TTimeIntervalMinutes)tRule->iOldLocalTimeOffset;
					}
				}
				
			// add rule to collection only if its time of change falls withing the time alignment
			if (tActRule.iTimeOfChange < staEndDateTime)
				{
				aRuleCollection.AddRuleL(*tRule);
				ruleAdded = ETrue;
				}
			}
		else
			{
			// no doubt that the rule falls within the time alignment
			aRuleCollection.AddRuleL(*tRule);
			ruleAdded = ETrue;
			}
		/*
		Fix for INC117764:
		Updating the default rule for a year if any new rule overlaps with the default rule.(Default rules start and end at same time)
		The offsets are updated if default rule and newly added rule start during same time and newly added rule month occurs before
		the default rule month.
		*/
		if( ruleAdded )
			{
			if(basicRuleFromYear != basicRuleToYear)
				{
				TInt ruleCount = aRuleCollection.Count() - 2;								
				for(TInt loop = ruleCount;loop >= 0; --loop)
					{
					TTzRule prevAddedRule = aRuleCollection[loop];
					//check that we do not update a winter rule using a summer rule and vice versa					
					//Summer rules start before July and winter rules after
					if(tRule->iMonth < EJuly && prevAddedRule.iMonth > EJuly)
						{
						continue;	
						}
					if(tRule->iMonth > EJuly && prevAddedRule.iMonth < EJuly)
						{
						continue;	
						}
					if(basicRuleFromYear > prevAddedRule.iFrom.iTime.DateTime().Year())
						{
						//no need to check beyond the newly added rules start year.
						break;	
						}					
					if((tRule->iFrom == prevAddedRule.iFrom) &&
					 (prevAddedRule.iFrom == prevAddedRule.iTo) && 
					 (tRule->iMonth < prevAddedRule.iMonth))
						{
						prevAddedRule.iOldLocalTimeOffset = tRule->iOldLocalTimeOffset;
						prevAddedRule.iNewLocalTimeOffset = tRule->iNewLocalTimeOffset;
						aRuleCollection.RemoveRule(loop);
						aRuleCollection.AddRuleL(prevAddedRule);
						}
					}	
				}		
			}
		} // for
	}

TInt CTzDbStdTimeAlignment::UtcOffset()
	{
	return iPersistedEntity.iUtcOffset;	
	}

TUint CTzDbStdTimeAlignment::RuleSetReference()	
	{
	return iPersistedEntity.iOffsetToRuleSet;	
	}

TInt CTzDbStdTimeAlignment::UntilYear()			
	{
	return iPersistedEntity.iUntilYear;		
	}

TInt CTzDbStdTimeAlignment::UntilMonth()			
	{
	return iPersistedEntity.iUntilMonth;		
	}

TInt CTzDbStdTimeAlignment::UntilDay()	
	{
	return iPersistedEntity.iUntilDayOfMonth;		
	}

TInt CTzDbStdTimeAlignment::UntilTimeInMinutes()	
	{
	return iPersistedEntity.iUntilTimeInMinutes;
	}

TInt CTzDbStdTimeAlignment::UntilTimeReference()
	{
	return iPersistedEntity.iUntilTimeReference;
	}	
	
//
// Calculate End Time of TimeAlignment (UTC)
//
void CTzDbStdTimeAlignment::CalculateEndTime(TTime& aEndTime)
	{
	TInt endYear;
	TInt endMonth;
	TInt endDay;
	TInt endHour;
	TInt endMinute;
	TInt endSecond = 0;
	TInt endMicrosecond = 0;
	
	if (iPersistedEntity.iUntilYear == KMaxTUint16)
		{
		aEndTime = Time::MaxTTime();
		}
	else
		{
		
		endYear = iPersistedEntity.iUntilYear;
		endMonth = iPersistedEntity.iUntilMonth; if (endMonth > EDecember) endMonth = EJanuary;
		endDay = iPersistedEntity.iUntilDayOfMonth; if (endDay > 31) endDay = 0;
		if (iPersistedEntity.iUntilTimeInMinutes == KMaxTUint16)
			{
			endHour = endMinute = 0;
			}
		else
			{
			endHour = iPersistedEntity.iUntilTimeInMinutes / 60;
			endMinute = iPersistedEntity.iUntilTimeInMinutes % 60;
			}

		TDateTime endDateTime(endYear,(TMonth)endMonth,endDay,endHour,endMinute,endSecond,endMicrosecond);
		aEndTime = endDateTime;

		// convert time to UTC
		TTzTimeReference taTimeReference(static_cast<TTzTimeReference>(iPersistedEntity.iUntilTimeReference) );
		if (taTimeReference == ETzStdTimeReference)
			{
			aEndTime -= static_cast<TTimeIntervalMinutes>(iPersistedEntity.iUtcOffset);
			}		

		// aEndTime is, at this point, the beginning of the the next Std Time Alignment
		aEndTime -= static_cast<TTimeIntervalSeconds>(1);
		}	
	}

//
// Calculate Start Time of TimeAlignment (UTC)
//
void CTzDbStdTimeAlignment::CalculateStartTime(TTime& aStartTime)
  	{
  	TInt startYear;
  	TInt startMonth;
  	TInt startDay;
  	TInt startHour;
  	TInt startMinute;
  	TInt startSecond = 0;
  	TInt startMicrosecond = 0;
  	
  	if (iPrevTimeAlignment == NULL)
  		{
  		aStartTime = 0;
  		}
  	else
  		{
  		startYear = iPrevTimeAlignment->iUntilYear;
  		startMonth = iPrevTimeAlignment->iUntilMonth; if (startMonth == (TInt)KMaxTUint8) startMonth = EJanuary;
  		startDay = iPrevTimeAlignment->iUntilDayOfMonth; if (startDay == (TInt)KMaxTUint8) startDay = 0;
  		if (iPrevTimeAlignment->iUntilTimeInMinutes == KMaxTUint16)
  			{
  			startHour = startMinute = 0;
  			}
  		else
  			{
  			startHour = iPrevTimeAlignment->iUntilTimeInMinutes / 60;
  			startMinute = iPrevTimeAlignment->iUntilTimeInMinutes % 60;
  			}
  
  		TDateTime startDateTime(startYear,(TMonth)startMonth,startDay,startHour,startMinute,startSecond,startMicrosecond);
  		aStartTime = startDateTime;
		// convert time to UTC
		TTzTimeReference taTimeReference(static_cast<TTzTimeReference>(iPersistedEntity.iUntilTimeReference) );
		if (taTimeReference == ETzStdTimeReference)
			{
			aStartTime -= static_cast<TTimeIntervalMinutes>(iPersistedEntity.iUtcOffset);
			}		
  		}		
  	}
	
//========================================================================

CTzDbRuleSet* CTzDbRuleSet::NewL(CReadOnlyTzDb& aReadOnlyTzDb, const TTzRuleSet& aRuleSet)
	{
	CTzDbRuleSet* self = new(ELeave) CTzDbRuleSet(aReadOnlyTzDb,aRuleSet);
	return self;	
	}


CTzDbRuleSet::CTzDbRuleSet(CReadOnlyTzDb& aReadOnlyTzDb, const TTzRuleSet& aRuleSet)
	: iReadOnlyTzDb(aReadOnlyTzDb),
	iPersistedEntity(aRuleSet)
	{
	}

//
// Gets the encoded rules for the current standard time alignment, between aStartDateTime and aEndDateTime
//
void CTzDbRuleSet::GetRulesL(CTzRules& aTzRules, TInt aUtcOffset, const TDateTime& aStartDateTime, const TDateTime& aEndDateTime) const
	{	
	TInt startYear = aStartDateTime.Year();
	// the last year we are interested in is the earliest of the following:
	// 		1) the last year of the CTzRules
	//		2) the year of aEndDateTime (the end of the invoking std time alignment)
	
	TInt endYear = (aEndDateTime.Year() < aTzRules.EndYear()) 
		? aEndDateTime.Year() : aTzRules.EndYear();
	const TDateTime rulesEndDateTime(aTzRules.EndYear(), EDecember, 30, 0,0,0,0);
	const TDateTime& endDateTime = (aEndDateTime.Year() <= aTzRules.EndYear())
		? aEndDateTime : rulesEndDateTime;
	
	RArray<TTzRuleDefinition*> ruleDefs;
	CleanupClosePushL(ruleDefs); // PUSH #1
	RArray<TTzRuleUse*> ruleUses;
	CleanupClosePushL(ruleUses); // PUSH #2
	
	FetchRuleDefinitionsL(ruleDefs,ruleUses,startYear,endYear);

	// fetch rules for previous year (these will be needed to work out the "Old Offset" field of 
	// the first rule in aStartYear	
	TInt initialLocalTimeOffset = GetLocalTimeOffsetAtEndOfYearL(startYear-1,aUtcOffset);
		
	// convert rule definitions (together with rule uses) to TTzRules and add them to aTzRules
	
	CompleteRulesAndAddToCollectionL(aTzRules,ruleDefs,ruleUses,aUtcOffset,initialLocalTimeOffset,aStartDateTime,endDateTime);
	
	CleanupStack::PopAndDestroy(2,&ruleDefs); // POP #2,#1 - ruleUses, ruleDefs
	}

TInt CTzDbRuleSet::GetLocalTimeOffsetAtEndOfYearL(TInt aYear, TInt aUtcOffset) const
	{
	RArray<TTzRuleDefinition*> ruleDefs;
	CleanupClosePushL(ruleDefs);
	RArray<TTzRuleUse*> ruleUses;
	CleanupClosePushL(ruleUses);
	
	TDateTime startDateTime(aYear,EJanuary,0,0,0,0,0);
	TDateTime endDateTime(aYear,EDecember,30,23,59,59,0);
	
	FetchRuleDefinitionsL(ruleDefs,ruleUses,aYear,aYear);
	
	CVTzActualisedRules* actRules = CVTzActualisedRules::NewL(aYear,aYear);
	CleanupStack::PushL(actRules);
	TVTzActualisedRule tDefaultRule(startDateTime,aUtcOffset,ETzUtcTimeReference);
	ActualiseRuleDefinitionsL(*actRules,ruleDefs,ruleUses,aUtcOffset,startDateTime,endDateTime,tDefaultRule);

	TInt count = actRules->Count();
	TInt finalOffset = (*actRules)[count-1].iNewOffset;
		
	CleanupStack::PopAndDestroy(3,&ruleDefs);
	return finalOffset;
	}


void CTzDbRuleSet::CompleteRulesAndAddToCollectionL(
					CTzRules& aTzRules, 
					const RArray<TTzRuleDefinition*>& aRuleDefs, 
					const RArray<TTzRuleUse*>& aRuleUses, 
					TInt aUtcOffset, TInt aInitialLocalTimeOffset,
					TDateTime aStart, TDateTime aEnd) const
	{
	TInt count = aRuleDefs.Count();
	if (count != aRuleUses.Count())
		{
		User::Leave(KErrCorrupt);
		}
	
	TTimeWithReference startOfAlignment(aStart,ETzUtcTimeReference);
	TTimeWithReference endOfAlignment(aEnd,ETzUtcTimeReference);

	//Keep track of changes to UTC offset
	// even if there are no DST rules.
	TTzRule trule(	
				startOfAlignment, startOfAlignment,
				(TUint16)aInitialLocalTimeOffset, (TUint16)aInitialLocalTimeOffset, EJanuary, ETzFixedDate,
				0, 0, 
				ETzUtcTimeReference, 
				0);
	aTzRules.AddRuleL(trule);		
	//Calculate the new and old offsets for each rule.
	TInt prevOffset = 0;
	TInt maxOffset = 0;
	for (TInt i = 0; i < count; i++)
		{
		TInt oldOffset = aInitialLocalTimeOffset;
		TInt dstOffset = aRuleDefs[i]->iStdTimeOffset;
		TInt newOffset = aUtcOffset + dstOffset;
		//newoffset and oldoffset should always have a difference of dstoffset.
		if (newOffset <= oldOffset )
			{
			//there are cases when dstoffset in the set of rules is same for few continuous rules
			//in such cases, newoffset and oldoffset would have a difference of maxoffset.
			if(prevOffset == dstOffset)
				{
				oldOffset =	aUtcOffset + maxOffset;		
				}
			else
				{
				oldOffset =	aUtcOffset + prevOffset;		
				}				
			}
		else
			//newoffset is more than oldoffset so maintain the difference with dstoffset.
			{
			oldOffset =	newOffset - dstOffset;							
			}
		prevOffset = dstOffset;		
		//store maximum offset, to handle cases in which dstoffset in the set of rules
		//is same for few continuous rules
		if(dstOffset > maxOffset)
			{
			maxOffset = dstOffset;
			}
			
		TUint16 timeOfChange = aRuleDefs[i]->iTimeOfChange;
		TTzTimeReference timeReference = 
			static_cast<TTzTimeReference>(aRuleDefs[i]->iTimeReference);

		//

		// Find the intersection of the date ranges:
		//  use latest start year, earliest end year
		TDateTime start = aStart;
		TDateTime end = aEnd;
		if(aRuleUses[i]->iFromYear > start.Year())
			{
			start = TDateTime(aRuleUses[i]->iFromYear,EJanuary,0,0,0,0,0);
			}
			
		if(aEnd.Year() > aRuleUses[i]->iUntilYear)
			{
			end = TDateTime(aRuleUses[i]->iUntilYear,EDecember,30,23,59,59,0);
			}
			
		TTimeWithReference startOfRule(start,ETzUtcTimeReference);
		TTimeWithReference endOfRule(end,ETzUtcTimeReference);
		if(endOfRule.iTime >= startOfRule.iTime)
			{
			TTzRule trule(	
						startOfRule, endOfRule,
						(TUint16)oldOffset, (TUint16)newOffset, (TMonth)aRuleDefs[i]->iMonth, (TTzRuleDay)aRuleDefs[i]->iDayRule,
						aRuleDefs[i]->iDayOfMonth, aRuleDefs[i]->iDayOfWeek, 
						timeReference, timeOfChange);				 

			aTzRules.AddRuleL(trule);
	
			}
		}	
	}

void CTzDbRuleSet::FetchRuleDefinitionsL(RArray<TTzRuleDefinition*>& aTzRuleDefinitions, RArray<TTzRuleUse*>& aTzRuleUses, TInt aStartYear, TInt aEndYear) const
	{	
	TInt ruleUseCount = iPersistedEntity.iNumberOfRuleUses;
	TTzRuleUse* truleUse(NULL);
	TTzRuleDefinition* truleDef(NULL);	
	for (TInt i = 0; i < ruleUseCount; i++)
		{
		truleUse = const_cast<TTzRuleUse*>(&iReadOnlyTzDb.GetTRuleUse(iPersistedEntity.iOffsetsToRuleUses[i]));
		
		// intersect rule use with (startYear,endYear) period
		if ( (truleUse->iFromYear <= aEndYear) && (truleUse->iUntilYear >= aStartYear) )
			{
			// Found one rule use valid during the years of interest - fetch rule definition.
			// For every year, check if it occurs within (aStartTime,aEndTime)
			truleDef = const_cast<TTzRuleDefinition*>(&iReadOnlyTzDb.GetTRuleDefinition(truleUse->iOffsetToRuleDefinition));
			// check that the rule definition has valid data. leave if the data is not valid, as it would mean that the database is corrupt
			User::LeaveIfError(CTzHelpers::Validate((const TTzRuleDefinition&)*truleDef));
			
			// Add rule definition ptr to array
			aTzRuleDefinitions.Append(truleDef);
			// Add rule use ptr to array
			aTzRuleUses.Append(truleUse);
			}
		}
	}

void CTzDbRuleSet::ActualiseRuleDefinitionsL(CVTzActualisedRules& aActualisedRules, const RArray<TTzRuleDefinition*>& aTzRuleDefinitions, const RArray<TTzRuleUse*>& aTzRuleUses, TInt aUtcOffset, const TDateTime& aStartDateTime, const TDateTime& aEndDateTime, const TVTzActualisedRule& aDefaultRule) const
	{
	TInt startYear = aStartDateTime.Year();
	TInt endYear = (aActualisedRules.EndYear() < (TUint)aEndDateTime.Year()) ? aActualisedRules.EndYear() : aEndDateTime.Year();	
	TInt rulesAddedSoFar = 0;
	TInt yearOfFirstRule = endYear;	
	TInt count = aTzRuleDefinitions.Count();
	
	TInt oldOffset = 0; // Arbitrarily set to zero. Required to create TTzRule.
	
	for (TInt i = 0; i < count; i++)
		{
		for (TInt year = (startYear > aTzRuleUses[i]->iFromYear) ? startYear : aTzRuleUses[i]->iFromYear ; (year <= endYear) && (year <= aTzRuleUses[i]->iUntilYear); year++)
			{
			TTzRule trule(	
					static_cast<TUint16>(startYear), static_cast<TUint16>(endYear),
					static_cast<TUint16>(oldOffset), static_cast<TUint16>(aUtcOffset + aTzRuleDefinitions[i]->iStdTimeOffset),static_cast<TMonth>(aTzRuleDefinitions[i]->iMonth), static_cast<TTzRuleDay>(aTzRuleDefinitions[i]->iDayRule),
					static_cast<TUint8>(aTzRuleDefinitions[i]->iDayOfMonth), static_cast<TUint8>(aTzRuleDefinitions[i]->iDayOfWeek), 
					static_cast<TTzTimeReference>(aTzRuleDefinitions[i]->iTimeReference), static_cast<TTzTimeReference>(aTzRuleDefinitions[i]->iTimeOfChange));
			
			TVTzActualisedRule tActRule = trule.Actualise(year);
			if ( (tActRule.iTimeOfChange < aEndDateTime) && (tActRule.iTimeOfChange >= aStartDateTime) )
				{
				aActualisedRules.AddRuleL(tActRule);
				// record the year of the first rule added
				if (rulesAddedSoFar == 0)
					{
					yearOfFirstRule = year;
					}
				rulesAddedSoFar++;			
				}
			}
		}
		
	// In some cases we need to add a "default rule" to aRules. ("Default rule" 
	// means a rule with zero as DST offset and the start-time of the time-alignment 
	// as time-of-change). This default rule will be added if no rule exists for the
	// first year of the time alignment.
	if ( (rulesAddedSoFar == 0) || (yearOfFirstRule > startYear) )
		{
		aActualisedRules.AddRuleL(aDefaultRule);
		}			
	}
	
//============================================================================================

TInt CTzHelpers::Validate(const TTzRuleDefinition& aRuleDefinition)
	{
	if (aRuleDefinition.iStdTimeOffset > KMaxDstOffset)
		{
		return KErrCorrupt;
		}

	if (aRuleDefinition.iMonth > EDecember)
		{
		return KErrCorrupt;
		}

	if (aRuleDefinition.iDayRule > KValidatedLastDayRule) // last element of enum TTzRuleDay
		{
		return KErrCorrupt;
		}

	if (aRuleDefinition.iDayOfMonth > KMaxDayOfMonth)
		{
		return KErrCorrupt;
		}

	if (aRuleDefinition.iDayOfWeek > ESunday)
		{
		return KErrCorrupt;
		}

	if (aRuleDefinition.iTimeReference > KValidatedLastTimeReference) // last element of enum TTzTimeReference
		{
		return KErrCorrupt;
		}

	if (aRuleDefinition.iTimeOfChange >= KMinutesInOneDay)
		{
		return KErrCorrupt;
		}

	return KErrNone;
	}
