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
// DST TZ Database Compiler 
// 
//

#ifndef __TZTABLES_H_
#define __TZTABLES_H_

#include "PersistedEntityWrapper.h"
#include "..\..\tz\server\include\tzpersisted.h"
#include "TzNode.h"
#include "TzEntities.h"
#include <vector>
#include <string>
#include <fstream>
//============================================================================
// Forward Declarations
//============================================================================
class CTZDocument;
class CTzCpStdTimeAlignment;
class CTzCpLink;
class CTzCpString;
class CTzCpZone;
class CTzCpRegion;
class CTzCpRuleUse;
class CTzCpRuleSet;
class CTzCpRuleDefinition;

using namespace std;
//============================================================================
// Enumerations
//============================================================================
//Location of rule fields in olsen files
enum TzRuleFormat
	{
	ERuleFormatRule = 0,
	ERuleFormatName,
	ERuleFormatFrom,
	ERuleFormatTo,
	ERuleFormatType,
	ERuleFormatIn,
	ERuleFormatOn,
	ERuleFormatAt,
	ERuleFormatSave,
	ERuleFormatLetter
	};
//Location of zone fields in olsen files
enum TzZoneFormat
	{
	EZoneFormatZone = 0,
	EZoneFormatName,
	EZoneFormatGMTOffset,
	EZoneFormatRules,
	EZoneFormatFormat,
	EZoneFormatUntilYear,
	EZoneFormatUntilMonth,
	EZoneFormatUntilDay,
	EZoneFormatUntilMinutes
	};
//Location of link fields in olsen files
enum TzLinkFormat
	{
	ELinkFormatLink = 0,
	ELinkFormatToZone,
	ELinkFormatFromZone
	};

enum TzMonths
	{
	EJan = 0,
	EFeb,
	EMar,
	EApr,
	EMay,
	EJun,
	EJul,
	EAug,
	ESep,
	EOct,
	ENov,
	EDec
	};

enum TzDays
	{
	EMon = 0,
	ETue,
	EWed,
	EThu,
	EFri,
	ESat,	
	ESun
	};

//============================================================================
// CTzCpStdTimeAlignmentsTable Class Definition
// Responsible for managing, creating and externalising the collection of 
// STDTimeAlignments.
//============================================================================
class CTzCpStdTimeAlignmentsTable : public CPersistedEntityWrapper
	{
public:
	CTzCpStdTimeAlignmentsTable(CTZDocument& aDocument);
	void ExternaliseL(ofstream& aFilestream);
	CTzCpStdTimeAlignment& AddTimeAlignment(CTzCpString& aTimeZoneFormatName);

	std::vector<CTzCpStdTimeAlignment*>& TimeAlignments() { return iVector; } 
	void RemoveUnreferencedEntities();

private:
	//member data
	std::vector<CTzCpStdTimeAlignment*> iVector;
	};
//============================================================================
// CTzCpLinksTable Class Definition
//============================================================================
class CTzCpLinksTable : public CPersistedEntityWrapper
	{
public:
	CTzCpLinksTable(CTZDocument& aDocument);
	//From CPersistedEntityWrapper
	CTzCpLink& AssembleL(CTZNode& aNode);
	void ExternaliseL(ofstream& aFilestream);
	CTzCpZone* FindZone(std::string& aLinkName);
	std::vector<CTzCpLink*>& Links() { return iVector; } 

private:
	//member data
	TTzLinksTable iPersistedEntity;
	std::vector<CTzCpLink*> iVector;
	};
//============================================================================
// CTzCpStringTable Class Definition
// Responsible for managing, creating and externalising the collection of
// Time Zone Strings.  Strings must not be duplicated to conserve space.  This 
// is enforced by this class by returning an existing TZ String object when 
// required to create one for a value already in use.
//============================================================================
class CTzCpStringTable : public CPersistedEntityWrapper
	{
public:
	CTzCpStringTable(CTZDocument& aDocument);
	//From CPersistedEntityWrapper
	void ExternaliseL(ofstream& aFilestream);

	CTzCpString& AddString(std::string aString);
	std::vector<CTzCpString*>& Strings() { return iVector; }
	void RemoveUnreferencedEntities();

private:
	//member data
	std::vector<CTzCpString*> iVector;
	};
//============================================================================
// CTzCpRuleSetsTable Class Definition
// Responsible for managing, creating and externalising the collection of
// RuleSets. 
//============================================================================
class CTzCpRuleSetsTable : public CPersistedEntityWrapper
	{
public:
	CTzCpRuleSetsTable(CTZDocument& aDocument);
	//From CPersistedEntityWrapper
	CTzCpRuleSet&  AssembleL(CTZNode& aNode);
	void ExternaliseL(ofstream& aFilestream);
	
	CTzCpRuleSet* GetRuleSet(std::string aRuleSetName);
	int GetRuleSetIndex(std::string aRuleSetName);
	std::vector<CTzCpRuleSet*>& RuleSets() { return iVector; }
	void RemoveUnreferencedEntities();

private:
	//member data
	std::vector<CTzCpRuleSet*> iVector;
	};
//============================================================================
// CTzCpRuleUsesTable Class Definition
// Stores a list of ruleuses
//============================================================================
class CTzCpRuleUsesTable : public CPersistedEntityWrapper
	{
public:
	CTzCpRuleUsesTable(CTZDocument& aDocument);
	//From CPersistedEntityWrapper
	CTzCpRuleUse& AssembleL(CTZNode& aNode);
	void ExternaliseL(ofstream& aFilestream);

	CTzCpRuleUse& AddRuleUse(CTzCpRuleUse* aRuleUse);
	std::vector<CTzCpRuleUse*>& RuleUses() { return iVector; }
	void RemoveUnreferencedEntities();

private:
	//member data
	std::vector<CTzCpRuleUse*> iVector;
	};
//============================================================================
// CTzCpRuleDefinitionsTable Class Definition
// Responsible for managing, creating and externalising the collection of
// Rule Definitions. Rule Definitions must not be duplicated to conserve space
// This is enforced by this class returning an existing definition object
// when required to create one for a value already defined.
//============================================================================
class CTzCpRuleDefinitionsTable : public CPersistedEntityWrapper
	{
public:
	CTzCpRuleDefinitionsTable(CTZDocument& aDocument);
	//From CPersistedEntityWrapper
	CTzCpRuleDefinition& AssembleL(CTZNode& aNode);
	void ExternaliseL(ofstream& aFilestream);
	CTzCpRuleDefinition& AddRuleDefinition(CTzCpRuleDefinition* aRuleDefinition);
	std::vector<CTzCpRuleDefinition*>& RuleDefinitions() { return iVector; }
	void RemoveUnreferencedEntities();

private:
	//member data
	std::vector<CTzCpRuleDefinition*> iVector;
	};
//============================================================================
// CTzCpRegionsTable Class Definition
// Responsible for managing, creating and externalising the collection of
// Regions.  Each Region must have a unique name.  This is enforced by this 
// class returning an existing region when required to create a region for a 
// name already taken.
//============================================================================
class CTzCpRegionsTable : public CPersistedEntityWrapper
	{
public:
	CTzCpRegionsTable(CTZDocument& aDocument);
	void ExternaliseL(ofstream& aFilestream);
	
	CTzCpRegion& GetRegion(CTzCpString& aRegionRef);
	TTzRegionsTable iPersistedEntity;
	void RemoveUnreferencedEntities();
	std::vector<CTzCpRegion*>& Regions() { return iVector; }	

private:
	//member data
	std::vector<CTzCpRegion*> iVector;
	};
//============================================================================
// CTZCpZonesTable Class Definition
// Responsible for managing, creating and externalising the collection of
// Zones.  This is enforced by this class returning an existing Zone when 
// required to create a Zone for a name already taken.
//============================================================================
class CTzCpZonesTable : public CPersistedEntityWrapper
	{
public:
	CTzCpZonesTable(CTZDocument& aDocument);
	CTzCpZone* GetZone(CTzCpString& aZoneRef, CTzCpString& aRegionRef,bool addNew);
	CTzCpZone* CTzCpZonesTable::GetZone(std::string& aZoneName);
	void ExternaliseL(ofstream& aFilestream); // //From CPersistedEntityWrapper
	std::vector<CTzCpZone*>& Zones() { return iVector; }
	void RemoveUnreferencedEntities();

private:
	std::vector<CTzCpZone*> iVector;
	TTzZonesTable iPersistedEntity;
	int iNumberOfZones;
	};
#endif //__TZTABLES_H
//============================================================================
// End of file
//============================================================================
