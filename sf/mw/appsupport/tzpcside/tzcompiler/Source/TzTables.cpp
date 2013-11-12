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

#include "TzGlobals.h"
#include "TzTables.h"
#include "TZNode.h"
#include "TZDocument.h"
#include "TzHelpers.h"
#include <algorithm> // sort
#include <iostream>
#include <fstream>
#include <cstdio>
using namespace std;
//============================================================================
// CTzCpLinksTable::CTzCpLinksTable
//============================================================================
CTzCpLinksTable::CTzCpLinksTable(CTZDocument& aDocument)
	:	CPersistedEntityWrapper(aDocument)
	{
	}
//============================================================================
// CTzCpLinksTable::AssembleL
// Adds a link node to the link table.  At this stage a CTzCpLink consists of
// two CTzCpString pointers. One is to the string for the link, the other is 
// the name of the linked zone.  The actual zone to be linked will be resolved 
// during the linker stage of the compilation process.  When the link table is 
// externalised the persisted class (TTzLink) will be populated with the 
// offsets to the string and linkedzone objects in the database 
//============================================================================
CTzCpLink& CTzCpLinksTable::AssembleL(CTZNode& aNode)
	{
	CTzCpLink* aNewLink = new CTzCpLink(iDocument);
	aNewLink->AssembleL(aNode);
	iVector.push_back(aNewLink);
	return *aNewLink;
	}
//============================================================================
// CTzCpLinksTable::ExternaliseL
// Write the links table to a file
//============================================================================
void CTzCpLinksTable::ExternaliseL(ofstream& aFilestream)
	{
	iDocument.DbHeader()->iOffsetToLinksTable = aFilestream.tellp();		
	// write out the number of links
	TUint16 numLinks = iVector.size();
	aFilestream.write((char*)&numLinks,sizeof(numLinks));
		
	for (int x = 0; x < numLinks;x++)
		{
		iVector[x]->ExternaliseL(aFilestream);
		}	
	}

//============================================================================
// CTzCpLinksTable::FindZone
// Given a link name, returns the "real" database zone it links to
// (allowing for "cascade linking": link_1 -> link_2 -> ... -> link_n -> Zone)
//============================================================================
CTzCpZone* CTzCpLinksTable::FindZone(std::string& aLinkName)
	{
	CTzCpZone* zone = NULL;
	std::string name = aLinkName;

	// iterate through the collection of links
	std::vector<CTzCpLink*>::iterator iter = iVector.begin();
	CTzCpLink** lastElementPtr = iVector.end();
	while(iter != lastElementPtr)
		{
		if ((*iter)->iLinkString->iString == name)
			{
			// name appears as the "old name" in one of the links
			// recursively search for the new name
			name = (*iter)->iRegionString->iString;
			if (name.length() > 0) 
				{// new names are not guaranteed to be in the form Region/Zone
				name += '/';
				}
			name += (*iter)->iZoneString->iString;
			return FindZone(name);
			}
		iter++;
		}

	// aLinkName was not found as the old name in any of the links
	// this means that aLinkName is not a link but the name of a real zone
	return iDocument.ZonesTable()->GetZone(name);
	}

//============================================================================
// CTzCpRuleSetsTable::CTzCpRuleSetsTable
//============================================================================
CTzCpRuleSetsTable::CTzCpRuleSetsTable(CTZDocument& aDocument)
	:	CPersistedEntityWrapper(aDocument)
	{
	}
//============================================================================
// CTzCpRuleSetsTable::AssembleL
//============================================================================
CTzCpRuleSet& CTzCpRuleSetsTable::AssembleL(CTZNode& aNode)
	{
	//Get the current Ruleset name
	string currentRuleSetName = aNode.NodeList()[ERuleFormatName]->iValue;
	return *GetRuleSet(currentRuleSetName);
	}
//============================================================================
// CTzCpRuleSetsTable::ExternaliseL
// Write the rule set table to file
//============================================================================
void CTzCpRuleSetsTable::ExternaliseL(ofstream& aFilestream)
	{
	iDocument.DbHeader()->iOffsetToRuleSetsTable = aFilestream.tellp();
	//Write the number of rulesets
	TUint16 numRuleSets = iVector.size();
	aFilestream.write((char*)&numRuleSets,sizeof(numRuleSets));
	
	TUint16 ruleUseOffset = 0;
	for (int x = 0; x < numRuleSets;x++)
		{
		iVector[x]->ExternaliseL(aFilestream);
		}
	}
//============================================================================
// CTzCpRuleSetsTable::GetRuleSet
// Searches for a match with aRuleSetName in the iStrings vector.  If a match
// is found the ruleset pointer from the iRuleSets vector with the matching
// index is returned.  If no match is found, a new ruleset is created and 
// added to the rulesets, the name is added to the strings and a pointer to
// the new ruleset is returned.
//============================================================================
CTzCpRuleSet* CTzCpRuleSetsTable::GetRuleSet(string aRuleSetName)
	{
	int numRuleSets = iVector.size();
	for (int index = 0; index < numRuleSets; index++)
		{
		if (iVector[index]->Name() == aRuleSetName)
			{
			return iVector[index];
			}
		}
	//New RuleSet required
	CTzCpRuleSet* newRuleSet = new CTzCpRuleSet(iDocument,aRuleSetName);
	iVector.push_back(newRuleSet);
	return newRuleSet;
	}
//============================================================================
// CTzCpRuleSetsTable::GetRuleSetIndex
// Gets the index of the ruleset from the ruleset name
//============================================================================
int CTzCpRuleSetsTable::GetRuleSetIndex(std::string aRuleSetName)
	{
	int size = iVector.size();
	for (int index = 0; index < size; index++)
		{
		if (iVector[index]->Name() == aRuleSetName)
			{
			return index;
			}
		}
	GetRuleSet(aRuleSetName);
	return GetRuleSetIndex(aRuleSetName);
	}

//============================================================================
// CTzCpRuleSetsTable::RemoveUnreferencedEntities
// Iterate through the vector of entities, removing all entities with a reference count of 0
//============================================================================
void CTzCpRuleSetsTable::RemoveUnreferencedEntities()
	{
	iVector.erase(std::remove_if(iVector.begin(),iVector.end(),SEntityCheck()),iVector.end());
	}

//============================================================================
// CTzCpRuleDefinitionsTable::CTzCpRuleDefinitionsTable
//============================================================================
CTzCpRuleDefinitionsTable::CTzCpRuleDefinitionsTable(CTZDocument& aDocument)
	:	CPersistedEntityWrapper(aDocument)
	{
	}
//============================================================================
// CTzCpRuleDefinitionsTable::AddRuleDefinition
// Adds a CTzCpRuleDefinition to the collection of rule definitions
// iRuleDefinitions takes ownership of aRuleDefinition.  Checks that the
// Rule definition does not already exist before adding it.  If it does exist,
// a reference to the duplicate is returned.  If it does not exist then it is
// added and the new reference is returned.
//============================================================================
CTzCpRuleDefinition& CTzCpRuleDefinitionsTable::AddRuleDefinition(CTzCpRuleDefinition* aRuleDefinition)
	{
	//Checks for a match - quick and dirty - consider revising
	int size = iVector.size();
	for (int x = 0; x < size; x++)
		{
		TTzRuleDefinition tmpRuleDef = iVector[x]->iPersistedEntity;
		if ((tmpRuleDef.iStdTimeOffset	== aRuleDefinition->iPersistedEntity.iStdTimeOffset) &&
			(tmpRuleDef.iMonth			== aRuleDefinition->iPersistedEntity.iMonth) &&
			(tmpRuleDef.iDayRule		== aRuleDefinition->iPersistedEntity.iDayRule) &&
			(tmpRuleDef.iDayOfMonth		== aRuleDefinition->iPersistedEntity.iDayOfMonth) &&
			(tmpRuleDef.iDayOfWeek		== aRuleDefinition->iPersistedEntity.iStdTimeOffset) && 
			(tmpRuleDef.iTimeReference	== aRuleDefinition->iPersistedEntity.iTimeReference) &&
			(tmpRuleDef.iTimeOfChange	== aRuleDefinition->iPersistedEntity.iTimeOfChange))
			{

			//If we don't add the ruledefinition to the vector we 
			//are responsible for deleting it, as clients will 
			//assume they have passed ownership to the table.			
			delete aRuleDefinition;
			return *iVector[x];
			}
		}
	iVector.push_back(aRuleDefinition);
	return *iVector[iVector.size() -1];
	}
//============================================================================
// CTzCpRuleDefinitionsTable::AssembleL
//============================================================================
CTzCpRuleDefinition&  CTzCpRuleDefinitionsTable::AssembleL(CTZNode& aNode)
	{
	CTzCpRuleDefinition* aRuleDefinition = new CTzCpRuleDefinition(iDocument);
	aRuleDefinition->AssembleL(aNode);
	return AddRuleDefinition(aRuleDefinition);
	}
//============================================================================
// CTzCpRuleDefinitionsTable::ExternaliseL
// Write the rule definition table to file
//============================================================================
void CTzCpRuleDefinitionsTable::ExternaliseL(ofstream& aFilestream)
	{
	iDocument.DbHeader()->iOffsetToRuleDefinitionsTable = aFilestream.tellp();
	//Write number of rule definitions
	TUint16 numRuleDefs = iVector.size();
	aFilestream.write((char*)&numRuleDefs,sizeof(numRuleDefs));

	for (int x = 0; x < numRuleDefs;x++)
		{
		iVector[x]->ExternaliseL(aFilestream);		
		}
	}

//============================================================================
// CTzCpRuleDefinitionsTable::RemoveUnreferencedEntities
// Iterate through the vector of entities, removing all entities with a reference count of 0
//============================================================================
void CTzCpRuleDefinitionsTable::RemoveUnreferencedEntities()
	{
	iVector.erase(std::remove_if(iVector.begin(),iVector.end(),SEntityCheck()),iVector.end());
	}

//============================================================================
// CTzCpStringTable::CTzCpStringTable
//============================================================================
CTzCpStringTable::CTzCpStringTable(CTZDocument& aDocument)
	:	CPersistedEntityWrapper(aDocument)
	{
	}

//============================================================================
// CTzCpStringTable::ExternaliseL
// Write the string table to file
//============================================================================
void CTzCpStringTable::ExternaliseL(ofstream& aFilestream)
	{
	iDocument.DbHeader()->iOffsetToStringTable = aFilestream.tellp();
	//Write number of strings
	TUint16 numStrings = iVector.size();
	aFilestream.write((char*)&numStrings,sizeof(numStrings));
	
	for (int x = 0; x < numStrings;x++)
		{
		iVector[x]->ExternaliseL(aFilestream);
		}
	}
//============================================================================
// CTzCpStringTable::AddString
// Returns a reference to the added string, or if the string already exists 
// a reference to the existing string
//============================================================================
CTzCpString& CTzCpStringTable::AddString(std::string aString)
	{
	int size = iVector.size();
	for (int i = 0; i < size; i++)
		{
		if (aString == iVector[i]->iString)
			{
  			return *iVector[i];
			}
		}
	CTzCpString* aNewString = new CTzCpString(iDocument);
	aNewString->iString = aString;
	iVector.push_back(aNewString);
	return *iVector[iVector.size() -1];
	}

//============================================================================
// CTzCpStringsTable::RemoveUnreferencedEntities
// Iterate through the vector of entities, removing all entities with a reference count of 0
//============================================================================
void CTzCpStringTable::RemoveUnreferencedEntities()
	{
	iVector.erase(std::remove_if(iVector.begin(),iVector.end(),SEntityCheck()),iVector.end());
	}
//============================================================================
// CTzCpRegionsTable::CTzCpRegionsTable
//============================================================================
CTzCpRegionsTable::CTzCpRegionsTable(CTZDocument& aDocument)
	:	CPersistedEntityWrapper(aDocument)
	{
	}

//============================================================================
// CTzCpRegionsTable::ExternaliseL
//============================================================================
void CTzCpRegionsTable::ExternaliseL(ofstream& aFilestream)
	{
	iDocument.DbHeader()->iOffsetToRegionalZonesTable = aFilestream.tellp();	
	//Loop through all the regions, externalising the regional zone index
	TUint16 numRegions = iVector.size();
	aFilestream.write((char*)&numRegions,sizeof(numRegions));

	TUint16 numberOfZones = 0;
	TUint16 zoneOffset = 0;

	for (int x = 0; x < numRegions;x++)
		{
		iVector[x]->iRegionalZonesIndex->ExternaliseL(aFilestream);
		}
	
	//Externalise the regions
	iDocument.DbHeader()->iOffsetToRegionsTable = aFilestream.tellp();
	aFilestream.write((char*)&numRegions,sizeof(numRegions));
	for (x = 0; x < numRegions;x++)
		{
		iVector[x]->ExternaliseL(aFilestream);
		}
	}
//============================================================================
// CTzCpRegionsTable::GetRegion
// Uses the region name index (from the strings table) to determine if we 
// already have an existing CTzCpRegion for this region.  If we do, we return
// this.  If we don't, we create a new pointer, add to iRegions and return
//============================================================================
CTzCpRegion& CTzCpRegionsTable::GetRegion(CTzCpString& aRegionNameRef)
	{
	int size = iVector.size();
	for (int x = 0; x < size; x++)
		{
		if (iVector[x]->iRegionNameRef->iString == aRegionNameRef.iString)
			{
			return *iVector[x];
			}
		}
	CTzCpRegion* aNewRegion = new CTzCpRegion(iDocument);
	aNewRegion->iRegionNameRef = &aRegionNameRef;
	iVector.push_back(aNewRegion);
	return *aNewRegion;
	}

//============================================================================
// CTzCpRegionsTable::RemoveUnreferencedEntities
// Iterate through the vector of entities, removing all entities with a reference count of 0
//============================================================================
void CTzCpRegionsTable::RemoveUnreferencedEntities()
	{
	iVector.erase(std::remove_if(iVector.begin(),iVector.end(),SEntityCheck()),iVector.end());
	}

//============================================================================
// CTzCpRuleUsesTable::CTzCpRuleUsesTable
//============================================================================
CTzCpRuleUsesTable::CTzCpRuleUsesTable(CTZDocument& aDocument)
	:	CPersistedEntityWrapper(aDocument)
	{
	}
//============================================================================
// CTzCpRuleUsesTable::AssembleL
//============================================================================
CTzCpRuleUse& CTzCpRuleUsesTable::AssembleL(CTZNode& aNode)
	{
	//Create the RuleUse
	CTzCpRuleUse* aRuleUse = new CTzCpRuleUse(iDocument);
	aRuleUse->AssembleL(aNode);
	//Add the Rule Use to the ruleuse collection
	return AddRuleUse(aRuleUse);
	}
//============================================================================
// CTzCpRuleUsesTable::AddRuleUse
// Adds a rule use to the rule use table and returns a reference to it
//============================================================================
CTzCpRuleUse& CTzCpRuleUsesTable::AddRuleUse(CTzCpRuleUse* aRuleUse)
	{
	iVector.push_back(aRuleUse);
	return *iVector[iVector.size() - 1];
	}
//============================================================================
// CTzCpRuleUsesTable::ExternaliseL
// Write the rule use table to a file
//============================================================================
void CTzCpRuleUsesTable::ExternaliseL(ofstream& aFilestream)
	{
	iDocument.DbHeader()->iOffsetToRuleUsesTable = aFilestream.tellp();
	//Write number of rule uses
	TUint16 numRuleUses = iVector.size();
	aFilestream.write((char*)&numRuleUses,sizeof(numRuleUses));
	
	for (int x = 0; x < numRuleUses; x++)
		{
		iVector[x]->ExternaliseL(aFilestream);
		}
	}

//============================================================================
// CTzCpRuleUsesTable::RemoveUnreferencedEntities
// Iterate through the vector of entities, removing all entities with a reference count of 0
//============================================================================
void CTzCpRuleUsesTable::RemoveUnreferencedEntities()
	{
	iVector.erase(std::remove_if(iVector.begin(),iVector.end(),SEntityCheck()),iVector.end());
	}

//============================================================================
// CTzCpZonesTable::CTzCpZonesTable
//============================================================================
CTzCpZonesTable::CTzCpZonesTable(CTZDocument& aDocument)
	:	CPersistedEntityWrapper(aDocument)
	{
	}

//============================================================================
// CTzCpZonesTable::ExternaliseL
//============================================================================
void CTzCpZonesTable::ExternaliseL(ofstream& aFilestream)
	{ 
	// sort zones by location ID
	sort(iVector.begin(), iVector.end(), CTzCpZone::SLocationIdSort());	
	
	TUint16 numZones = iVector.size();
	// write out zones data to database
	iDocument.DbHeader()->iOffsetToZones = aFilestream.tellp();
	for (int x = 0; x < numZones; x++)
		{
		iVector[x]->ExternaliseL(aFilestream);
		}

	// write out the zone table to database
	iDocument.DbHeader()->iOffsetToZonesTable = aFilestream.tellp();
	aFilestream.write((char*)&numZones,sizeof(numZones));
	TUint16 zoneOffset;
	for (x = 0; x < numZones; x++)
		{
		zoneOffset = iVector[x]->iOffset;
		aFilestream.write((char*)&zoneOffset,sizeof(zoneOffset));
		}
	}
//============================================================================
// CTzCpZonesTable::GetZone
// Uses the id of the zone name in the string table to determine if we already
// have created a CTzCpZone object for this zone.  If it exists we return this
// otherwise we create a new zone, add it to iZones and return
// aIndex contains the index of the zone to use in the vector
// If addZone is false we do not add a new zone, just set aIndex -1
//============================================================================
CTzCpZone* CTzCpZonesTable::GetZone(CTzCpString& aZoneRef, CTzCpString& aRegionRef,bool addZone)
	{
	int size = iVector.size();
	for (int x = 0; x < size; x++)
		{
		string zoneTestString	= iVector[x]->iZoneNameRef->iString;
		string regionTestString	= iVector[x]->iRegionNameRef->iString;
		if ((iVector[x]->iZoneNameRef->iString == aZoneRef.iString) && (iVector[x]->iRegionNameRef->iString == aRegionRef.iString))
			{
			CTzCpZone* zone = iVector[x];
			return zone;
			}
		}
	if (addZone)
		{
		CTzCpZone* aNewZone = new CTzCpZone(iDocument);
		aNewZone->iZoneNameRef		= &aZoneRef;
		aNewZone->iRegionNameRef	= &aRegionRef;
		iVector.push_back(aNewZone);
		return aNewZone;
		}
	return NULL;

	}

CTzCpZone* CTzCpZonesTable::GetZone(std::string& aZoneName)
	{
	int slashPos = aZoneName.find('/');
	int length = aZoneName.length();
	CTzCpString* regString = new CTzCpString(iDocument);
	CTzCpString* zoneString = new CTzCpString(iDocument);
	if (slashPos > 0)
		{
		regString->iString = aZoneName.substr(0,slashPos);
		zoneString->iString = aZoneName.substr(slashPos+1,length-1);
		}
	else
		{
		regString->iString = "";
		zoneString->iString = aZoneName;
		}

	CTzCpZone* zone = GetZone(*zoneString,*regString,false);

	return zone;
	}

//============================================================================
// CTzCpZonesTable::RemoveUnreferencedEntities
// Iterate through the vector of entities, removing all entities with a reference count of 0
//============================================================================
void CTzCpZonesTable::RemoveUnreferencedEntities()
	{
	iVector.erase(std::remove_if(iVector.begin(),iVector.end(),SEntityCheck()),iVector.end());
	}

//============================================================================
// CTzCpStdTimeAlignmentsTable::CTzCpStdTimeAlignmentsTable
//============================================================================
CTzCpStdTimeAlignmentsTable::CTzCpStdTimeAlignmentsTable(CTZDocument& aDocument)
	:	CPersistedEntityWrapper(aDocument)
	{
	}
//============================================================================
// CTzCpStdTimeAlignmentsTable::AddTimeAlignment
//============================================================================
CTzCpStdTimeAlignment& CTzCpStdTimeAlignmentsTable::AddTimeAlignment(CTzCpString& aTimeZoneFormatName)
	{
	CTzCpStdTimeAlignment* aNewTimeAlignment = new CTzCpStdTimeAlignment(iDocument);
	iVector.push_back(aNewTimeAlignment);
	return *aNewTimeAlignment;
	}

//============================================================================
// CTzCpStdTimeAlignmentsTable::ExternaliseL
// Write the time alignments to file
//============================================================================
void CTzCpStdTimeAlignmentsTable::ExternaliseL(ofstream& aFilestream)
	{
	iDocument.DbHeader()->iOffsetToStdTimeAlignmentsTable = aFilestream.tellp();	
	//Write number of time alignments
	TUint16 numTimeAlignments = iVector.size();
	aFilestream.write((char*)&numTimeAlignments,sizeof(numTimeAlignments));
	
	for (int x = 0; x < numTimeAlignments;x++)
		{
		iVector[x]->ExternaliseL(aFilestream);
		}
	}

//============================================================================
// CTzCpStdTimeAlignmentsTable::RemoveUnreferencedEntities
// Iterate through the vector of entities, removing all entities with a reference count of 0
//============================================================================
void CTzCpStdTimeAlignmentsTable::RemoveUnreferencedEntities()
	{
	iVector.erase(std::remove_if(iVector.begin(),iVector.end(),SEntityCheck()),iVector.end());
	}
//============================================================================
// End of file
//============================================================================
