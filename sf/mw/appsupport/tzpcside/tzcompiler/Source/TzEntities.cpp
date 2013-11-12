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
#include "TzEntities.h"
#include "TzTables.h"
#include "TZNode.h"
#include "TZDocument.h"
#include "TzHelpers.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <algorithm>
using namespace std;
//============================================================================
// CTzCpString::CTzCpString
//============================================================================
CTzCpString::CTzCpString(CTZDocument& aDocument)
	:	CPersistedEntityWrapper(aDocument)
	{
	}
//============================================================================
// CTzCpString::ExternaliseL
//============================================================================
void CTzCpString::ExternaliseL(ofstream& aFilestream)
	{
	iOffset = (int)aFilestream.tellp() - iDocument.DbHeader()->iOffsetToStringTable;
	TUint8 stringLength = iString.length();
	aFilestream.write((char*)&stringLength,sizeof(stringLength));
	aFilestream << iString.c_str();
	}
//============================================================================
// CTzCpRegion::CTzCpRegion
//============================================================================
CTzCpRegion::CTzCpRegion(CTZDocument& aDocument)
	:	CPersistedEntityWrapper(aDocument)
	{
	iRegionalZonesIndex = new CTzCpRegionalZonesIndex(aDocument);
	}
//============================================================================
// CTzCpRegion::~CTzCpRegion
//============================================================================
CTzCpRegion::~CTzCpRegion()
	{
	for (int x = 0; x < iRegionalZonesIndex->iZoneIndex.size();++x)
		{
		iRegionalZonesIndex->iZoneIndex[x]->DecrRefCount();
		}
	
	delete iRegionalZonesIndex;
	}

//============================================================================
// CTzCpRegion::ExternaliseL
//============================================================================
void CTzCpRegion::ExternaliseL(ofstream& aFilestream)
	{
	iPersistedEntity.iOffsetToRegionName = iRegionNameRef->iOffset;
	iPersistedEntity.iOffsetToRegionalZoneIndex = iRegionalZonesIndex->iOffset;
	aFilestream.write((char*)&iPersistedEntity,sizeof(iPersistedEntity));
	}
//============================================================================
// CTzCpRuleUse::CTzCpRuleUse
// RuleUse constructor.
//============================================================================
CTzCpRuleUse::CTzCpRuleUse(CTZDocument& aDocument)
	:	CPersistedEntityWrapper(aDocument)
	{
	}
//============================================================================
// CTzCpRuleUse::~CTzCpRuleUse 
//============================================================================
CTzCpRuleUse::~CTzCpRuleUse()
	{
	iRuleLetterPtr->DecrRefCount();
	iRuleDefinitionPtr->DecrRefCount();
	}
//============================================================================
// CTzCpRuleUse::AssembleL
// Populates the CTzCpRuleUse from the current node.
// RuleUse 'To' field may contain 'only' in the olsen files
// This is to show the rule was in use for that year only.  In this case
// we change the to field to match the from field.
//============================================================================
void CTzCpRuleUse::AssembleL(CTZNode& aNode)
	{
	iPersistedEntity.iFromYear	= atoi(aNode.NodeList()[2]->iValue.c_str());
	if (aNode.NodeList()[3]->iValue.c_str()[0] == 'o')
		{
		iPersistedEntity.iUntilYear = iPersistedEntity.iFromYear;
		}
	else if (aNode.NodeList()[3]->iValue.c_str()[0] == 'm')
		{
		iPersistedEntity.iUntilYear = KMaxTUint16;
		}
	else 
		{
		iPersistedEntity.iUntilYear = atoi(aNode.NodeList()[3]->iValue.c_str());				
		}
	}
//============================================================================
// CTzCpLink::CTzCpLink
//============================================================================
CTzCpLink::CTzCpLink(CTZDocument& aDocument)
	:	CPersistedEntityWrapper(aDocument)
	{
	}
//============================================================================
// CTzCpLink::AssembleL
//============================================================================
int CTzCpLink::AssembleL(CTZNode& aNode)
	{
	//Add the from value into the string table as is
	//aNode.iNodeList[ELinkFormatFromZone]->iValue;
	//Split the 'To' value into its region/zone parts
	//'Region' is first part of a zone identity. eg 'Europe' in 'Europe/London'
	//'Zone'  is second part of a zone identity. eg 'London' in 'Europe/London'
	string linkString	= aNode.NodeList()[ELinkFormatToZone]->iValue;
	string fromString	= aNode.NodeList()[ELinkFormatFromZone]->iValue;
	int slashChar		= linkString.find('/',0);
	iLinkString			= &iDocument.StringTable()->AddString(fromString);
	iLinkString->IncrRefCount();

	if (slashChar > 0)
		{
		iRegionString = &iDocument.StringTable()->AddString(linkString.substr(0,slashChar));
		}
	else
		{
		iRegionString = &iDocument.StringTable()->AddString("");
		}
	iRegionString->IncrRefCount();

	iZoneString			= &iDocument.StringTable()->AddString(linkString.substr(slashChar+1));
	iZoneString->IncrRefCount();
	return 0;
	}
//============================================================================
// CTzCpLink::ExternaliseL
//============================================================================
void CTzCpLink::ExternaliseL(ofstream& aFilestream)
	{
	iPersistedEntity.iOffsetToLinkName	= iLinkString->iOffset;
	iPersistedEntity.iOffsetToZone		= iLinkedZoneOffset->iOffset;
	aFilestream.write((char*)&iPersistedEntity,sizeof(iPersistedEntity));
	}
//============================================================================
// CTzCpRuleUse::ExternaliseL
//============================================================================
void CTzCpRuleUse::ExternaliseL(ofstream& aFilestream)
	{
	iOffset = (int)aFilestream.tellp() - iDocument.DbHeader()->iOffsetToRuleUsesTable;
	iPersistedEntity.iOffsetToRuleDefinition	= iRuleDefinitionPtr->iOffset;
	iPersistedEntity.iOffsetToRuleLetterString	= iRuleLetterPtr->iOffset;
	aFilestream.write((char*)&iPersistedEntity,sizeof(iPersistedEntity));
	}
//============================================================================
// CTzCpRuleDefinition::CTzCpRuleDefinition
//============================================================================
CTzCpRuleDefinition::CTzCpRuleDefinition(CTZDocument& aDocument)
	:	CPersistedEntityWrapper(aDocument)
	{
	}
//============================================================================
// CTzCpRuleDefinition::AssembleL
// Populates the CTzCpRuleDefinition from the current node
//============================================================================
int CTzCpRuleDefinition::AssembleL(CTZNode& aNode)
	{
	//Set the time offset
	//Assuming that the 'SAVE' field in a rule is of the format H:MM
	string tmpString	= aNode.NodeList()[ERuleFormatSave]->iValue;
	int breakAt			= tmpString.find_first_of(':');
	int hours			= atoi(tmpString.c_str());
	int minutes			= 0;
	if (tmpString.length() > 1)
		{
		minutes			= atoi(tmpString.substr(2).c_str());
		}
	iPersistedEntity.iStdTimeOffset		= (hours*60) + minutes;

	//Set the month ( 0 - 11)
	iPersistedEntity.iMonth = CTzCpHelpers::GetMonth(aNode.NodeList()[ERuleFormatIn]->iValue);
	
	//Set the day rule
	tmpString = aNode.NodeList()[ERuleFormatOn]->iValue;
	string searchDayAfterString  = ">=";
	string searchDayBeforeString = "<=";

	if (tmpString.c_str()[0] == 'l')
		{
		iPersistedEntity.iDayRule = ETzDayInLastWeekOfMonth;
		}
	else if (strstr(tmpString.c_str(),searchDayAfterString.c_str()) != NULL)
		{
		iPersistedEntity.iDayRule = ETzDayAfterDate;
		}
	else if (strstr(tmpString.c_str(),searchDayBeforeString.c_str()) != NULL)
		{
		iPersistedEntity.iDayRule = ETzDayBeforeDate;
		}
	else
		{
		iPersistedEntity.iDayRule = ETzFixedDate;
		}

	//Set the day of month.  This is only required if the day rule is 
	//ETzFixedDate, EDayBeforeDate or EDayAfterDate.  Defaults to 0.
	TInt tmpDay;
	switch (iPersistedEntity.iDayRule)
		{
		case ETzFixedDate:
			tmpDay = atoi(tmpString.c_str());
			tmpDay = tmpDay ? tmpDay-1 : 0; // OG: zero-based Day (as used in TDateTime in Symbian OS)
			iPersistedEntity.iDayOfMonth = tmpDay;
		break;
		case ETzDayBeforeDate:
		case ETzDayAfterDate:
			tmpDay = atoi(tmpString.substr(5).c_str());
			tmpDay = tmpDay ? tmpDay-1 : 0; // OG: zero-based Day (as used in TDateTime in Symbian OS)
			iPersistedEntity.iDayOfMonth = tmpDay;
		break;
		default:
			iPersistedEntity.iDayOfMonth = 0;
		break;
		}
	
	//Set the day of the week.  This only applies if day rule != ETzFixedDate
	iPersistedEntity.iDayOfWeek = CTzCpHelpers::GetDay(tmpString);
	
	//Set the time of change (in minutes)
	tmpString	= aNode.NodeList()[ERuleFormatAt]->iValue;
	iPersistedEntity.iTimeOfChange = CTzCpHelpers::GetTimeOfDayInMinutes(tmpString);

	//set the time reference
	iPersistedEntity.iTimeReference = CTzCpHelpers::GetTimeReference(tmpString);
	
	//Spare flags - these will probably be used for implementing write access 
	//if required
	//iPersistedEntity.iReferenceCount	= 0;	//Unused for now
	//iPersistedEntity.iStatus			= 0;	//Unused for now
	return 0;
	}
//============================================================================
// CTzCpRuleDefinition::ExternaliseL
//============================================================================
void CTzCpRuleDefinition::ExternaliseL(ofstream& aFilestream)
	{
	iOffset = (int)aFilestream.tellp() - iDocument.DbHeader()->iOffsetToRuleDefinitionsTable;
	aFilestream.write((char*)&iPersistedEntity,sizeof(iPersistedEntity));
	}
//============================================================================
// CTzCpRuleSet::CTzCpRuleSet
//============================================================================
CTzCpRuleSet::CTzCpRuleSet(CTZDocument& aDocument,std::string aRuleSetName)
	:	CPersistedEntityWrapper(aDocument) , iName(aRuleSetName)
	{
	}
//============================================================================
// CTzCpRuleSet::~CTzCpRuleSet
//============================================================================
CTzCpRuleSet::~CTzCpRuleSet()
	{
	int size = RuleUses().size();
	for (int x = 0; x < size;x++)
		{
		iVector[x]->DecrRefCount();
		}
	}

//============================================================================
// CTzCpRuleSet::ExternaliseL
//============================================================================
void CTzCpRuleSet::ExternaliseL(ofstream& aFilestream)
	{
	iOffset = (int)aFilestream.tellp() - iDocument.DbHeader()->iOffsetToRuleSetsTable;
	iPersistedEntity.iNumberOfRuleUses = iVector.size();

	aFilestream.write((char*)&iPersistedEntity,sizeof(iPersistedEntity)-sizeof(iPersistedEntity.iOffsetsToRuleUses));
	TUint16 ruleUseOffset;
	for (int x = 0; x < iPersistedEntity.iNumberOfRuleUses;x++)
		{
		ruleUseOffset = iVector[x]->iOffset;
		aFilestream.write((char*)&ruleUseOffset,sizeof(ruleUseOffset));
		}
	}
//============================================================================
// CTzCpRuleSet::AddRuleUse
// Adds the index of the CTzCpRuleUse in the rule use table
// to the collection of rule uses
//============================================================================
void CTzCpRuleSet::AddRuleUse(CTzCpRuleUse& aRuleUse)
	{
	iVector.push_back(&aRuleUse); //The address of the ruleuse
	}
//============================================================================
// CTzCpRegionalZonesIndex::CTzCpRegionalZoneIndex
//============================================================================
CTzCpRegionalZonesIndex::CTzCpRegionalZonesIndex(CTZDocument& aDocument)
	:	CPersistedEntityWrapper(aDocument)
	{
	}

//============================================================================
// CTzCpRegionalZonesIndex::ExternaliseL
//============================================================================
void CTzCpRegionalZonesIndex::ExternaliseL(ofstream& aFilestream)
	{
	iOffset = (int)aFilestream.tellp() - iDocument.DbHeader()->iOffsetToRegionalZonesTable;

	// sort zone index alphabetically before externalising it
	sort(iZoneIndex.begin(),iZoneIndex.end(),CTzCpZone::SZoneNameSort());

	iPersistedEntity.iNumberOfZones = iZoneIndex.size();
	aFilestream.write((char*)&iPersistedEntity,sizeof(iPersistedEntity) - sizeof(iPersistedEntity.iOffsetsToZones));
	//Write out the zone offsets
	for (int x = 0; x < iPersistedEntity.iNumberOfZones;x++)
		{
		TUint16 zoneOffset = iZoneIndex[x]->iOffset;
		aFilestream.write((char*)&zoneOffset,sizeof(zoneOffset));
		}
	}
//============================================================================
// CTzCpRegionalZonesIndex::AddZoneIndex
// We add the address of the reference to avoid a double deletion - 
// this zone will already 
//============================================================================
void CTzCpRegionalZonesIndex::AddZoneIndex(CTzCpZone& aZoneRef)
	{
	iZoneIndex.push_back(&aZoneRef);
	}
//============================================================================
// CTzCpZone::CTzCpZone
//============================================================================
CTzCpZone::CTzCpZone(CTZDocument& aDocument)
	:	CPersistedEntityWrapper(aDocument)
	{
	}
//============================================================================
// CTzCpZone::~CTzCpZone
// When we destroy a zone, we need to decrement all its referenced entity 
// reference counts by 1.
//============================================================================
CTzCpZone::~CTzCpZone()
	{
	iRegionNameRef->DecrRefCount();
	iZoneNameRef->DecrRefCount();

	for (int x = 0; x < iTimeAlignments.size();++x)
		{
		iTimeAlignments[x]->DecrRefCount();
		}
	}

//============================================================================
// CTzCpZone::ExternaliseL
//============================================================================
void CTzCpZone::ExternaliseL(ofstream& aFilestream)
	{
	iOffset = (int)aFilestream.tellp() - iDocument.DbHeader()->iOffsetToZones;
	iPersistedEntity.iLocationId				= iLocationId;
	iPersistedEntity.iOffsetToZoneName			= iZoneNameRef->iOffset;
	iPersistedEntity.iOffsetToRegionName		= iRegionNameRef->iOffset;
	iPersistedEntity.iNumberOfStdTimeAlignments	= iTimeAlignments.size();

	aFilestream.write((char*)&iPersistedEntity,sizeof(iPersistedEntity)-sizeof(iPersistedEntity.iOffsetsToTimeAlignments));
	
	//Externalise the time alignment offsets
	for (int x = 0; x < iPersistedEntity.iNumberOfStdTimeAlignments; x++)
		{
		TUint16 stdoffset = iTimeAlignments[x]->iOffset;
		aFilestream.write((char*)&stdoffset,sizeof(stdoffset));
		}
	}
//============================================================================
// CTzCpStdTimeAlignment::CTzCpStdTimeAlignment
//============================================================================
CTzCpStdTimeAlignment::CTzCpStdTimeAlignment(CTZDocument& aDocument)
	:	CPersistedEntityWrapper(aDocument)
	{
	}
//============================================================================
// CTzCpStdTimeAlignment::~CTzCpStdTimeAlignment
// When we destroy a CTzCpStdTimeAlignment we need to decrement all its 
// referenced entity reference counts by 1.
//============================================================================
CTzCpStdTimeAlignment::~CTzCpStdTimeAlignment()
	{
	iOffsetToTimeZoneFormatName->DecrRefCount();

	for (int x = 0; x < iOffsetsToTimeZonesShortNames.size();++x)
		{
		iOffsetsToTimeZonesShortNames[x]->DecrRefCount();
		}
		iRuleSet->DecrRefCount();
	}
//============================================================================
// CTzCpStdTimeAlignment::AssembleL
// Populates the CTzCpStdTimeAlignment from the current node
//============================================================================
int CTzCpStdTimeAlignment::AssembleL(CTZNode& aNode)
	{
	string tmpString;
	//Until Year
	if (EZoneFormatUntilYear >= aNode.NodeList().size())
		{
		iPersistedEntity.iUntilYear	= KMaxYear;
		}
	else
		{
		iPersistedEntity.iUntilYear	= atoi(aNode.NodeList()[EZoneFormatUntilYear]->iValue.c_str());
		}
	//Until Month
	if (EZoneFormatUntilMonth >= aNode.NodeList().size())
		{
		iPersistedEntity.iUntilMonth	= 0;
		}
	else
		{
		tmpString = aNode.NodeList()[EZoneFormatUntilMonth]->iValue;
		if (tmpString.empty())
			iPersistedEntity.iUntilMonth = 0;
		else
			iPersistedEntity.iUntilMonth = CTzCpHelpers::GetMonth(tmpString);
		}
	//Until DayOfMonth
	if (EZoneFormatUntilDay >= aNode.NodeList().size())
		{
		iPersistedEntity.iUntilDayOfMonth	= 0;
		}
	else
		{
		tmpString = aNode.NodeList()[EZoneFormatUntilDay]->iValue;
		TInt tmpDay = atoi(aNode.NodeList()[EZoneFormatUntilDay]->iValue.c_str());
		tmpDay = tmpDay ? tmpDay-1 : 0; // zero-based Day (as used in TDateTime in Symbian OS)
		iPersistedEntity.iUntilDayOfMonth	= tmpDay;
		}
	//Until minutes
	//Until Time Reference
	if (EZoneFormatUntilMinutes >= aNode.NodeList().size())
		{
		iPersistedEntity.iUntilTimeInMinutes = 0;
		iPersistedEntity.iUntilTimeReference = 0;
		}
	else
		{
		tmpString = aNode.NodeList()[EZoneFormatUntilMinutes]->iValue;
		iPersistedEntity.iUntilTimeInMinutes = CTzCpHelpers::GetTimeOfDayInMinutes(tmpString);
		iPersistedEntity.iUntilTimeReference = CTzCpHelpers::GetTimeReference(tmpString);
		}
	//GMTOffset in minutes
	tmpString = aNode.NodeList()[EZoneFormatGMTOffset]->iValue;
	iPersistedEntity.iUtcOffset = CTzCpHelpers::GetTimeOfDayInMinutes(tmpString);

	//Set these in the document after completion.
	//Offset to ruleset
	iPersistedEntity.iOffsetToRuleSet = 0;
	//offset to timezoneformat	
	iPersistedEntity.iOffsetToTimeZoneFormatName = 0;
	return 0;
	}
//============================================================================
// CTzCpStdTimeAlignment::ExternaliseL
//============================================================================
void CTzCpStdTimeAlignment::ExternaliseL(ofstream& aFilestream)
	{
	iOffset = (int)aFilestream.tellp() - iDocument.DbHeader()->iOffsetToStdTimeAlignmentsTable;
	//Retrieve the ruleset offset
	iPersistedEntity.iOffsetToRuleSet = iRuleSet->iOffset;
	 //Retrieve the time zone format name offset
	iPersistedEntity.iOffsetToTimeZoneFormatName = iDocument.StringTable()->Strings()[iPersistedEntity.iOffsetToTimeZoneFormatName]->iOffset;
	//Set the number of time zones
	iPersistedEntity.iNumberOfTimeZones = iOffsetsToTimeZonesShortNames.size();

	aFilestream.write((char*)&iPersistedEntity,sizeof(iPersistedEntity)-sizeof(iPersistedEntity.iOffsetsToTimeZones));
	//Write the time zone offsets
	int size = iOffsetsToTimeZonesShortNames.size();
	for (int x = 0; x < size; x++)
		{
		TUint16 offsetToTimeZone = iOffsetsToTimeZonesShortNames[x]->iOffset;
		aFilestream.write((char*)&offsetToTimeZone,sizeof(offsetToTimeZone));
		}
	}
//============================================================================
// End of file
//============================================================================
