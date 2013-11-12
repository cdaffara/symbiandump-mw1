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

#ifndef __TZENTITIES_H_
#define __TZENTITIES_H_

#include "PersistedEntityWrapper.h"
#include "..\..\tz\server\include\tzpersisted.h"
#include "TzNode.h"
#include "TzEntities.h"
#include "TzDocument.h"
#include <vector>
#include <string>
using namespace std;

class CTzCpRuleSet;
//============================================================================
// CTzCpString Class Definition
// String wrapper class, binds a string to its offset in the string table.
//============================================================================
class CTzCpString : public CPersistedEntityWrapper
	{
public:
	CTzCpString(CTZDocument& aDocument);
	//From CPersistedEntityWrapper
	int AssembleL(CTZNode& aNode) {return 0;}
	void ExternaliseL(ofstream& aFilestream);
public:
	std::string iString;
	//int iOffset;
	};
//============================================================================
// CTzCpStdTimeAlignment Class Definition
//============================================================================
class CTzCpStdTimeAlignment : public CPersistedEntityWrapper
	{
public:
	CTzCpStdTimeAlignment(CTZDocument& aDocument);
	~CTzCpStdTimeAlignment();
	//From CPersistedEntityWrapper
	int AssembleL(CTZNode& aNode);
	void ExternaliseL(ofstream& aFilestream);

	//member data
	std::vector<CTzCpString*> iOffsetsToTimeZonesShortNames;
	CTzCpString* iOffsetToTimeZoneFormatName;
	CTzCpRuleSet* iRuleSet;
	TTzStdTimeAlignment iPersistedEntity;
	};
//============================================================================
// CTzCpZone Class Definition
//============================================================================
class CTzCpZone : public CPersistedEntityWrapper
	{
public:
	CTzCpZone(CTZDocument& aDocument);
	~CTzCpZone();
	//From CPersistedEntityWrapper
	void ExternaliseL(ofstream& aFilestream);

public: 
	//member data
	std::vector<CTzCpStdTimeAlignment*> iTimeAlignments;
	CTzCpString* iRegionNameRef;
	CTzCpString* iZoneNameRef;
	TUint16 iLocationId;

	TTzZone iPersistedEntity;

	//
	// three different methods to sort zones:
	//
	// alphabetically by zone name (for the regional zone indices):
	struct SZoneNameSort
		{
		TBool operator()(CTzCpZone* aZone1, CTzCpZone* aZone2)
			{
			return aZone1->iZoneNameRef->iString < aZone2->iZoneNameRef->iString;
			}
		};

	// by LocationId (for the Zones table)
	struct SLocationIdSort
		{
		TBool operator()(CTzCpZone* aZone1, CTzCpZone* aZone2)
			{
			return aZone1->iLocationId < aZone2->iLocationId;
			}
		};

	struct SZoneFullNameSort
		{
		TBool operator()(CTzCpZone* aZone1, CTzCpZone* aZone2)
			{
			if (aZone1->iRegionNameRef->iString < aZone2->iRegionNameRef->iString)
				{
				return true;
				}
			else if (aZone1->iRegionNameRef->iString ==  aZone2->iRegionNameRef->iString)
				{
				if (aZone1->iZoneNameRef->iString < aZone2->iZoneNameRef->iString)
					{
					return true;
					}
				else
					{
					return false;
					}
				}
			else
				{	
				return false;
				}			
			}
		};
	};
//============================================================================
// CTzCpRegionalZonesIndex Class Definition
//============================================================================
class CTzCpRegionalZonesIndex : public CPersistedEntityWrapper
	{
public:
	CTzCpRegionalZonesIndex(CTZDocument& aDocument);
	//From CPersistedEntityWrapper
	void ExternaliseL(ofstream& aFilestream);

	void AddZoneIndex(CTzCpZone& aZoneRef);
	//member data
	TTzRegionalZoneIndex iPersistedEntity;
	std::vector<CTzCpZone*> iZoneIndex;
	};
//============================================================================
// CTzCpRegion Class Definition
//============================================================================
class CTzCpRegion : public CPersistedEntityWrapper
	{
public:
	CTzCpRegion(CTZDocument& aDocument);
	~CTzCpRegion();
	//From CPersistedEntityWrapper
	void ExternaliseL(ofstream& aFilestream);

	//member data
	CTzCpRegionalZonesIndex* iRegionalZonesIndex;
	CTzCpString* iRegionNameRef;
	TTzRegion iPersistedEntity;

	};
//============================================================================
// CTzCpLink Class Definition
// Wrapper class for a TTzLink.
//============================================================================
class CTzCpLink : public CPersistedEntityWrapper
	{
public:
	CTzCpLink(CTZDocument& aDocument);
	//From CPersistedEntityWrapper
	int AssembleL(CTZNode& aNode);
	void ExternaliseL(ofstream& aFilestream);

	//member data
	TTzLink iPersistedEntity;
	//These objects are NOT owned by CTzCpLink
	CTzCpString* iLinkString;
	CTzCpString* iRegionString;
	CTzCpString* iZoneString;
	CTzCpZone* iLinkedZoneOffset;
	};
//============================================================================
// CTzCpRuleDefinition Class Definition
//============================================================================
class CTzCpRuleDefinition : public CPersistedEntityWrapper
	{
public:
	CTzCpRuleDefinition(CTZDocument& aDocument);
	//From CPersistedEntityWrapper
	int AssembleL(CTZNode& aNode);
	void ExternaliseL(ofstream& aFilestream);
	//member data
	TTzRuleDefinition iPersistedEntity;
	};
//============================================================================
// CTzCpRuleUse Class Definition
// Encapsulates TRuleUse.  Will be part of a collection of CTzCpRuleUse
// objects owned by a CTzCpRuleSet
//============================================================================
class CTzCpRuleUse : public CPersistedEntityWrapper
	{
public:
	CTzCpRuleUse(CTZDocument& aDocument);
	~CTzCpRuleUse();
	//From CPersistedEntityWrapper
	void AssembleL(CTZNode& aNode);
	void ExternaliseL(ofstream& aFilestream);

	//member data
	CTzCpString* iRuleLetterPtr;
	CTzCpRuleDefinition* iRuleDefinitionPtr;
	TTzRuleUse iPersistedEntity;
	};
//============================================================================
// CTzCPRuleSet Class Definition
// Encapsulates a TTzRuleSet, and is responsible for creating a collection of
// CTzCpRuleUse objects.
//============================================================================
class CTzCpRuleSet : public CPersistedEntityWrapper
	{
public:
	CTzCpRuleSet(CTZDocument& aDocument,std::string aName);
	~CTzCpRuleSet();
	//From CPersistedEntityWrapper
	void ExternaliseL(ofstream& aFilestream);

	void AddRuleUse(CTzCpRuleUse& aRuleUseRef);

	std::vector<CTzCpRuleUse*>& RuleUses() { return iVector; }
	std::string& Name() { return iName; }

private:
	//member data
	TTzRuleSet iPersistedEntity;
	std::string iName;	//Name string for the ruleset. DO NOT add to string table
	std::vector<CTzCpRuleUse*> iVector;
	};
#endif // __TZENTITIES_H_
//============================================================================
// End of file
//============================================================================
