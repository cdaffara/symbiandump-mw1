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

#ifndef __TZDBENTITIES_H__
#define __TZDBENTITIES_H__

#include <badesca.h>
#include <e32std.h>

#include "tzpersisted.h"

class CReadOnlyTzDb;
class CTzDbRegion;
class CTzDbZone;
class CTzDbStdTimeAlignment;
class CTzDbRuleSet;
class CTzRules;
class CVTzActualisedRules;
class TVTzActualisedRule;

/******************************************************************************************/

/** CTzDbStringsTable
Encapsulates and provides access to the strings table contained in the database.

@internalComponent
@since 9.1
*/
class CTzDbStringsTable : public CBase
	{
public:
	static CTzDbStringsTable* NewL(const TTzStringsTable& aStringsTable);
	TPtrC8 GetStringL(TUint aReference);

private:
	CTzDbStringsTable(const TTzStringsTable& aStringsTable);

private:
	const TTzStringsTable& iPersistedEntity;
	};

/** CTzDbRegionsTable
Encapsulates and provides access to the regions table contained in the database.

@internalComponent
@since 9.1
*/
class CTzDbRegionsTable : public CBase
	{
public:
	static CTzDbRegionsTable* NewL(CReadOnlyTzDb& aReadOnlyTzDb, const TTzRegionsTable& aRegionsTable);

	CTzDbRegion* FindRegionL(const TDesC8& aRegionName);

private:
	CTzDbRegionsTable(CReadOnlyTzDb& aReadOnlyTzDb, const TTzRegionsTable& aRegionsTable);

private:
	CReadOnlyTzDb& iReadOnlyTzDb;
	const TTzRegionsTable& iPersistedEntity;
	};

/** CTzDbRegionalZonesTable
Encapsulates and provides access to the regional zones table contained in the database.

@internalComponent
@since 9.1
*/
class CTzDbRegionalZonesTable : public CBase
	{
public:
	static CTzDbRegionalZonesTable* NewL(CReadOnlyTzDb& aReadOnlyTzDb, const TTzRegionalZonesTable& aRegionalZonesTable);

	const TTzRegionalZoneIndex& GetTRegionalZoneIndex(TUint aReference);

private:
	CTzDbRegionalZonesTable(CReadOnlyTzDb& aReadOnlyTzDbr, const TTzRegionalZonesTable& aRegionalZonesTable);

private:
	CReadOnlyTzDb& iReadOnlyTzDb;
	const TTzRegionalZonesTable& iPersistedEntity;
	};

/** CTzDbZonesTable
Encapsulates and provides access to the zones table contained in the database.

@internalComponent
@since 9.1
*/
class CTzDbZonesTable : public CBase
	{
public:
	static CTzDbZonesTable* NewL(CReadOnlyTzDb& aReadOnlyTzDb, const TTzZonesTable& aZonesTable, TInt aZonesDataBaseAddress);

	CTzDbZone* GetZoneL(TUint aReference);
	TTzZone& GetTZone(TUint aReference);
	CTzDbZone* GetZoneByIdL(TUint aLocationId);

private:
	CTzDbZonesTable(CReadOnlyTzDb& aReadOnlyTzDb, const TTzZonesTable& aZonesTable, TInt aZonesDataBaseAddress);

private:
	CReadOnlyTzDb& iReadOnlyTzDb;
	const TTzZonesTable& iPersistedEntity;
	TInt iZonesDataBaseAddress;
	};

/** CTzDbLinksTable
Encapsulates and provides access to the links table contained in the database.

@internalComponent
@since 9.1
*/
class CTzDbLinksTable : public CBase
	{
public:
	static CTzDbLinksTable* NewL(CReadOnlyTzDb& aReadOnlyTzDb, const TTzLinksTable& aLinksTable);

	CTzDbZone* FindZoneL(const TDesC8& aLinkName);

private:
	CTzDbLinksTable(CReadOnlyTzDb& aReadOnlyTzDb, const TTzLinksTable& aLinksTable);

private:
	CReadOnlyTzDb& iReadOnlyTzDb;
	const TTzLinksTable& iPersistedEntity;
	};

/** CTzDbStdTimeAlignmentsTable
Encapsulates and provides access to the std time alignments table contained in the database.

@internalComponent
@since 9.1
*/
class CTzDbStdTimeAlignmentsTable : public CBase
	{
public:
	static CTzDbStdTimeAlignmentsTable* NewL(const TTzStdTimeAlignmentsTable& aStdTimeAlignmentsTable);
	TTzStdTimeAlignment& GetTStdTimeAlignment(TUint aReference);

private:
	CTzDbStdTimeAlignmentsTable(const TTzStdTimeAlignmentsTable& aStdTimeAlignmentsTable);

private:
	const TTzStdTimeAlignmentsTable& iPersistedEntity;
	};

/** CTzDbRuleSetsTable
Encapsulates and provides access to the rulesets table contained in the database.

@internalComponent
@since 9.1
*/
class CTzDbRuleSetsTable : public CBase
	{
public:
	static CTzDbRuleSetsTable* NewL(CReadOnlyTzDb& aReadOnlyTzDb, const TTzRuleSetsTable& aRuleSetsTable);
	CTzDbRuleSet* GetRuleSetL(TUint aReference);

private:
	CTzDbRuleSetsTable(CReadOnlyTzDb& aReadOnlyTzDb, const TTzRuleSetsTable& aRuleSetsTable);
	TTzRuleSet& GetTRuleSet(TUint aReference);

private:
	CReadOnlyTzDb& iReadOnlyTzDb;
	const TTzRuleSetsTable& iPersistedEntity;
	};

/** CTzDbRuleUsesTable
Encapsulates and provides access to the rule uses table contained in the database.

@internalComponent
@since 9.1
*/
class CTzDbRuleUsesTable : public CBase
	{
public:
	static CTzDbRuleUsesTable* NewL(const TTzRuleUsesTable& aRuleUsesTable);
	TTzRuleUse& GetTRuleUse(TUint aReference);

private:
	CTzDbRuleUsesTable(const TTzRuleUsesTable& aRuleUsesTable);

private:
	const TTzRuleUsesTable& iPersistedEntity;
	};

/** CTzDbRuleDefinitionsTable
Encapsulates and provides access to the rule definitions table contained in the database.

@internalComponent
@since 9.1
*/
class CTzDbRuleDefinitionsTable : public CBase
	{
public:
	static CTzDbRuleDefinitionsTable* NewL(const TTzRuleDefinitionsTable& aRuleDefinitionsTable);
	const TTzRuleDefinition& GetRuleDefinition(TUint aReference);

private:
	CTzDbRuleDefinitionsTable(const TTzRuleDefinitionsTable& aRuleDefinitionsTable);

private:
	const TTzRuleDefinitionsTable& iPersistedEntity;
	};


/** CTzDbRegion
Encapsulates and provides access to the information that the database holds about a region

@internalComponent
@since 9.1
*/
class CTzDbRegion : public CBase
	{
public:
	static CTzDbRegion* NewL(CReadOnlyTzDb& aReadOnlyTzDb, const TTzRegion& aRegion);
	CTzDbZone* FindZoneL(const TDesC8& aCityName);
	CTzDbZone* FindZoneL(TUint aCityNameReference);

private:
	CTzDbRegion(CReadOnlyTzDb& aReadOnlyTzDb, const TTzRegion& aRegion);

private:
	CReadOnlyTzDb& iReadOnlyTzDb;
	const TTzRegion& iPersistedEntity;
	};

/** CTzDbZone
Encapsulates and provides access to the information that the database holds about a zone

@internalComponent
@since 9.1
*/
class CTzDbZone : public CBase
	{
public:
	static CTzDbZone* NewL(CReadOnlyTzDb& aReadOnlyTzDb, const TTzZone& aZone);
	void GetRulesL(CTzRules& aRules);
	CTzDbStdTimeAlignment* FindStdTimeAlignmentL(TInt aYear, TInt aMonth, TInt aDay, TInt aHour, TInt aMinute, TTzTimeReference aTimeRef);
	inline TUint LocationId() const;
	HBufC8* GetFullZoneNameLC();

private:
	CTzDbZone(CReadOnlyTzDb& aReadOnlyTzDb, const TTzZone& aZone);
	TBool IsTimeInStdTimeAlignment(const TTzStdTimeAlignment& aStdTa, TInt aYear, TInt aMonth, TInt aDay, TInt aHour, TInt aMinute, TTzTimeReference aTimeRef);

private:
	CReadOnlyTzDb& iReadOnlyTzDb;
	const TTzZone& iPersistedEntity;
	};


/** CTzDbStdTimeAlignment
Encapsulates and provides access to the information that the database holds about a std time alignment

@internalComponent
@since 9.1
*/
class CTzDbStdTimeAlignment : public CBase
	{
public:
	static CTzDbStdTimeAlignment* NewL(CReadOnlyTzDb& aReadOnlyTzDb, const TTzStdTimeAlignment& aTimeAlignment, 
		TTzStdTimeAlignment* aPrevTimeAlignment);

	~CTzDbStdTimeAlignment();
	void GetRulesL(CTzRules& aRules, const TDateTime& aStartDateTime, TDateTime& aEndDateTime);
	
    TInt UtcOffset();
	TUint RuleSetReference();
	TInt UntilYear();
	TInt UntilMonth();
	TInt UntilDay();
	TInt UntilTimeInMinutes();
	TInt UntilTimeReference();

private:
	CTzDbStdTimeAlignment(CReadOnlyTzDb& aReadOnlyTzDb, const TTzStdTimeAlignment& aTimeAlignment, 
		TTzStdTimeAlignment* aPrevTimeAlignment);

	void AddRulesToCollectionL(CTzRules& aRuleCollection, CTzRules& aNewRules);
	
	void CalculateEndTime(TTime& aEndTime);
	void CalculateStartTime(TTime& aStartTime);
	
private:
	CReadOnlyTzDb& iReadOnlyTzDb;
    TTzStdTimeAlignment	  iPersistedEntity;
	TTzStdTimeAlignment*  iPrevTimeAlignment;
	};



/** CTzDbRuleSet
Encapsulates and provides access to the information that the database holds about a rule set

@internalComponent
@since 9.1
*/
class CTzDbRuleSet : public CBase
	{
public:
	static CTzDbRuleSet* NewL(CReadOnlyTzDb& aReadOnlyTzDb, const TTzRuleSet& aRuleSet);
	void GetRulesL(CTzRules& aTzRules, TInt aUtcOffset, const TDateTime& aStartDateTime, const TDateTime& aEndDateTime) const;
	void FetchRuleDefinitionsL(RArray<TTzRuleDefinition*>& aTzRuleDefinitions, RArray<TTzRuleUse*>& aTzRuleUses, TInt aStartYear, TInt aEndYear) const;
	void ActualiseRuleDefinitionsL(CVTzActualisedRules& aActualisedRules, const RArray<TTzRuleDefinition*>& aTzRuleDefinitions, const RArray<TTzRuleUse*>& aTzRuleUses, TInt aUtcOffset, const TDateTime& aStartDateTime, const TDateTime& aEndDateTime, const TVTzActualisedRule& aDefaultRule) const;
	
private:
	CTzDbRuleSet(CReadOnlyTzDb& aReadOnlyTzDb, const TTzRuleSet& aRuleSet);
	void CompleteRulesAndAddToCollectionL(CTzRules& aTzRules, 
							const RArray<TTzRuleDefinition*>& aRuleDefs, const RArray<TTzRuleUse*>& aRuleUses, 
							TInt aUtcOffset, TInt aInitialLocalTimeOffset, 
							TDateTime aStartYear, TDateTime aEndYear) const;

	TInt GetLocalTimeOffsetAtEndOfYearL(TInt aYear, TInt aUtcOffset) const;

private:
	CReadOnlyTzDb& iReadOnlyTzDb;
	const TTzRuleSet& iPersistedEntity;
	};

class CTzHelpers
	{
public:
	static TInt Validate(const TTzRuleDefinition& aRuleDefinition);
	};

inline TUint CTzDbZone::LocationId() const
	{
	return iPersistedEntity.iLocationId;
	}


#endif // __TZDBENTITIES_H__



