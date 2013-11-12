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

#include <string>
#include <sstream>
#include <iostream>

#include <algorithm>
#include <direct.h>
#include <iomanip>
#include <sys/stat.h>

#include "TZDocument.h"
#include "TZScanner.h"
#include "TzTables.h"

#include "TzHelpers.h"

using namespace std;

//============================================================================
// CTZDocument::CTZDocument
// Constructor.
//============================================================================
CTZDocument::CTZDocument(MScanner& aScanner)
:	iScanner(aScanner)
	{
	//Create the root element
	iRoot = new CTZElement();
	iRoot->iParent = NULL;
	iOpenedElement = iRoot;
	
	//Create the tables
	iRuleSetTable			= new CTzCpRuleSetsTable(*this);
	iRuleDefinitionTable	= new CTzCpRuleDefinitionsTable(*this);
	iStringTable			= new CTzCpStringTable(*this);
	iRegionsTable			= new CTzCpRegionsTable(*this);
	iRuleUseTable			= new CTzCpRuleUsesTable(*this);
	iZonesTable				= new CTzCpZonesTable(*this);
	iStdTimeAlignmentTable	= new CTzCpStdTimeAlignmentsTable(*this);
	iLinkTable				= new CTzCpLinksTable(*this);

	ReadConfigurationFile();
	}
//============================================================================
// CTZDocument::~CTZDocument
// Destructor
//============================================================================
CTZDocument::~CTZDocument()
	{
	WriteConfigurationFile();
	if (iRoot)
		delete iRoot;

	delete iRuleSetTable;
	delete iRuleUseTable;
	delete iRuleDefinitionTable;
	delete iStringTable;
	delete iRegionsTable;
	delete iZonesTable;
	delete iStdTimeAlignmentTable;
	delete iLinkTable;
	}
//============================================================================
// CTZDocument::Scan
// Delegates the actual file scanning to our CTzCpScanner object.
// returns KErrNone if succesful, or any TzErrorCode
//============================================================================
int CTZDocument::Scan(const char* aFileName)
	{
	return iScanner.Scan(aFileName);
	}
//============================================================================
// CTZDocument::AddRootChildElement
// Adds a new element as a child of the root node
//============================================================================
void CTZDocument::CreateRootChildElement()
	{
	iOpenedElement	= new CTZElement();
	iOpenedElement->iParent = iRoot;
	iRoot->NodeList().push_back(iOpenedElement);
	}
//============================================================================
// CTZDocument::AddChildElement
// Creates a new CTZElement as a child node of iOpenedElement.
// iOpenedElement is then assigned to the new child node
//============================================================================
void CTZDocument::CreateChildElement()
	{
	iOpenedElement = iOpenedElement->CreateChildElement();
	}

//============================================================================
// CTZDocument::AddAttribute
// Adds an attribute to the current node
//============================================================================
void CTZDocument::AddAttribute(const char* aValue)
	{
	iOpenedElement->AddAttribute(aValue);
	}
//============================================================================
// CTZDocument::CloseElement
// Back up to the parent node
//============================================================================
void CTZDocument::CloseElement()
	{
	if (iOpenedElement != iRoot)
		{
		iOpenedElement = iOpenedElement->iParent;
		}
	}

//============================================================================
// CTZDocument::Assemble
// Iterates through the nodes assembling each one into the relevant T-Classes
//============================================================================
void CTZDocument::AssembleL()
	{
	cout.width(30);
	CTzCpHelpers::PrintStep("Assembling nodes");
	
	CTZNode* tmpNode;	
	
	string currentRuleSetName;
	string currentZoneName;

	int size = iRoot->NodeList().size();
	for (int i = 0;i < size; i++)
		{
		tmpNode		= iRoot->NodeList()[i];
		if (tmpNode->CheckNode())
			{
			if (strcmpi(KOlsonRuleTag,tmpNode->NodeList()[0]->iValue.c_str()) == 0)
				AssembleRuleL(*tmpNode);
			else if (strcmpi(KOlsonLinkTag,tmpNode->NodeList()[0]->iValue.c_str()) == 0)
				iLinkTable->AssembleL(*tmpNode); // Assemble Links
			else if (strcmpi(KOlsonZoneTag,tmpNode->NodeList()[0]->iValue.c_str()) == 0)
				AssembleZoneL(*tmpNode);
			}
		}
	}

void CTZDocument::AssembleRuleL(CTZNode& aNode)
	{
	//Conditions tested here:
	// 1: The 'To' year in a rule is >= the start year from the configuration file
	// 2: The 'To' year in a rule is 'max'.
	// 3: The 'To' year in a rule is 'only' but the from year is >= the start year from the configuration file
	//If none of these conditions are met, just dump this rule (do nothing)

	if ((atoi(aNode.NodeList()[ERuleFormatTo]->iValue.c_str()) >= TzGlobals::iStartYear) ||
		(aNode.NodeList()[ERuleFormatTo]->iValue == "max") ||
		(aNode.NodeList()[ERuleFormatTo]->iValue == "only" && atoi(aNode.NodeList()[ERuleFormatFrom]->iValue.c_str()) >= TzGlobals::iStartYear))
		{	
		//==========================Rule Set==========================
		//Get a RuleSet to add the RuleUse to
		CTzCpRuleSet& aRuleSet = iRuleSetTable->AssembleL(aNode);

		//=======================RuleDefinition=======================
		//Assemble a rule definition from the node
		CTzCpRuleDefinition& aRuleDefinition = iRuleDefinitionTable->AssembleL(aNode);
		
		//=========================RuleLetter=========================
		CTzCpString& aRuleLetter =iStringTable->AddString(aNode.NodeList()[ERuleFormatLetter]->iValue);

		//==========================RuleUse===========================
		CTzCpRuleUse& aRuleUse = iRuleUseTable->AssembleL(aNode);
		
		aRuleUse.iRuleDefinitionPtr = &aRuleDefinition;
		aRuleDefinition.IncrRefCount();

		aRuleUse.iRuleLetterPtr = &aRuleLetter; 
		aRuleLetter.IncrRefCount();

		//Add the RuleUse reference to the RuleSet				
		aRuleSet.AddRuleUse(aRuleUse);
		aRuleUse.IncrRefCount();
		}
	}

void CTZDocument::AssembleZoneL(CTZNode& aNode)
	{
	//Get indexes for the region and zone name strings in the string table
	string currentZoneName		= aNode.NodeList()[EZoneFormatName]->iValue;

	int slashChar		= currentZoneName.find('/',0);

	//'Region' is first part of a zone identity. eg 'Europe' in 'Europe/London'
	//'Zone'  is second part of a zone identity. eg 'London' in 'Europe/London'
	string regionName;
	if (slashChar > 0)
		{
		regionName = currentZoneName.substr(0,slashChar);
		}
	else
		{
		regionName = "";
		}
	CTzCpString& regionNameRef	= iStringTable->AddString(regionName);
	CTzCpString& zoneNameRef	= iStringTable->AddString(currentZoneName.substr(slashChar+1));

	//Start building the zone
	CTzCpZone* newZone	= iZonesTable->GetZone(zoneNameRef,regionNameRef,true);
	newZone->IncrRefCount();
	regionNameRef.IncrRefCount();
	zoneNameRef.IncrRefCount();

	//Get the region
	CTzCpRegion& aRegion = iRegionsTable->GetRegion(regionNameRef);
	//Add the zone to the regions zone index
	
	aRegion.iRegionalZonesIndex->AddZoneIndex(*newZone);
	newZone->IncrRefCount();

	//======================Time Alignments=======================
	//Process the zones time alignments
	//Check date range
	if ((EZoneFormatUntilYear >= aNode.NodeList().size()) ||
		(atoi(aNode.NodeList()[EZoneFormatUntilYear]->iValue.c_str()) >= TzGlobals::iStartYear))
		{
		CTzCpString& timeZoneFormatName = iStringTable->AddString(aNode.NodeList()[EZoneFormatFormat]->iValue);

		CTzCpStdTimeAlignment& aNewAlignment = iStdTimeAlignmentTable->AddTimeAlignment(timeZoneFormatName);
		timeZoneFormatName.IncrRefCount();

		aNewAlignment.AssembleL(aNode);
		aNewAlignment.iOffsetToTimeZoneFormatName = &timeZoneFormatName;
		//Offset to ruleset
		aNewAlignment.iRuleSet = iRuleSetTable->GetRuleSet(aNode.NodeList()[EZoneFormatRules]->iValue);
		aNewAlignment.iRuleSet->IncrRefCount();

		//Add the new time alignment to the zone
		newZone->iTimeAlignments.push_back(&aNewAlignment);
		aNewAlignment.IncrRefCount();
		}
	//Do the same for the zones other time alignments
	//A node value of "" indicates a child node
	int size = aNode.NodeList().size();
	for (int j = 0;j < size; j++)
		{
		CTZNode* tmpNode2 = aNode.NodeList()[j];
		if ( tmpNode2->CheckNode() && (tmpNode2->iValue == ""))
			{
			//Check date range
			if ((EZoneFormatUntilYear >= tmpNode2->NodeList().size()) ||
			   (atoi(tmpNode2->NodeList()[EZoneFormatUntilYear]->iValue.c_str()) >= TzGlobals::iStartYear))
				{
				CTzCpString& timeZoneFormatName = iStringTable->AddString(tmpNode2->NodeList()[EZoneFormatFormat]->iValue);
				CTzCpStdTimeAlignment& aNewAlignment = iStdTimeAlignmentTable->AddTimeAlignment(timeZoneFormatName);
				timeZoneFormatName.IncrRefCount();

				aNewAlignment.AssembleL(*tmpNode2);
				aNewAlignment.iOffsetToTimeZoneFormatName = &timeZoneFormatName;
				//Offset to ruleset
				string ruleSetName = aNode.NodeList()[EZoneFormatRules]->iValue;
				aNewAlignment.iRuleSet = iRuleSetTable->GetRuleSet(tmpNode2->NodeList()[EZoneFormatRules]->iValue);
				aNewAlignment.iRuleSet->IncrRefCount();

				//Add the new time alignment to the zone
				newZone->iTimeAlignments.push_back(&aNewAlignment);
				aNewAlignment.IncrRefCount();
				}
			}
		}
	}

//============================================================================
// CTZDocument::Link
// Combines the format string from the stdtimealignment with the ruleuse 
// letter.  The resulting string is added to the string table and the offset 
// to the string is stored back in the stdtimealignment.
// We can expect four different formatting cases eg
//	1: GMT/BST	: Seperate the two strings and add to string table
//	2: C%sT		: Replace %s with letter string from rule use
//	3: GMT		: Add to string table if not already added
//	4: -		: Ignore (remove %s)
//============================================================================
void CTZDocument::Link()
	{
	CTzCpHelpers::PrintStep("Linking");
	string formatString;
	
	//Iterate through all the STDTimeAlignments
	int size = iStdTimeAlignmentTable->TimeAlignments().size();
	for (int x = 0; x < size;x++)
		{
		CTzCpStdTimeAlignment* aTimeAlignment = iStdTimeAlignmentTable->TimeAlignments()[x];
		string alignmentString = aTimeAlignment->iOffsetToTimeZoneFormatName->iString;
		//Get the ruleset
		if (aTimeAlignment->iPersistedEntity.iOffsetToRuleSet != KMaxTUint16)
			{
			CTzCpRuleSet* aRuleSet = aTimeAlignment->iRuleSet;
			if (aRuleSet->RuleUses().size() == 0)
				{
				//Use the alignment string - there are no rule uses here
				CTzCpString& stringRef = iStringTable->AddString(alignmentString);
				aTimeAlignment->iOffsetsToTimeZonesShortNames.push_back(&stringRef);
				stringRef.IncrRefCount();
				}
			else
				{
				int size2 = aRuleSet->RuleUses().size();
				for (int z = 0; z < size2; z++)
					{
					
					CTzCpRuleUse* aRuleUse = iRuleUseTable->RuleUses()[z];
					
					//Test for the following condition:
					// Rule Use 'To' field is >= Time alignment 'From' field - AND
					// Rule Use 'From' field is <= Time Alignment 'To' field
					//We assume that the time alignment start field is the end year of the 
					//previous time alignment unless x == 0
					int timeAlignmentStartYear = (x == 0) ? 0 : iStdTimeAlignmentTable->TimeAlignments()[x-1]->iPersistedEntity.iUntilYear;
					
					if ((aRuleUse->iPersistedEntity.iUntilYear >= timeAlignmentStartYear) &&
						(aRuleUse->iPersistedEntity.iFromYear <= aTimeAlignment->iPersistedEntity.iUntilYear)) 
						{						
						string letterString = aRuleUse->iRuleLetterPtr->iString;
						int slashPos  = alignmentString.find_first_of('/');
						int insertPos = alignmentString.find_first_of('%');
						if (slashPos != -1)
							{
							formatString = alignmentString.substr(0,slashPos);
							CTzCpString& formatStringRef = iStringTable->AddString(formatString);
							aTimeAlignment->iOffsetsToTimeZonesShortNames.push_back(&formatStringRef);
							formatStringRef.IncrRefCount();

							formatString= alignmentString.substr(slashPos+1);
							CTzCpString& stringRef = iStringTable->AddString(formatString);
							aTimeAlignment->iOffsetsToTimeZonesShortNames.push_back(&stringRef);
							stringRef.IncrRefCount();
							}
						
						else if (insertPos != -1)
							{
							formatString = alignmentString.substr(0,insertPos);
							
							CTzCpString& formatStringRef = iStringTable->AddString(formatString);
							aTimeAlignment->iOffsetsToTimeZonesShortNames.push_back(&formatStringRef);
							formatStringRef.IncrRefCount();
							
							if (letterString[0] != '-')
								{
								formatString += letterString;
								}
							formatString += alignmentString.substr(insertPos+2);
							CTzCpString& stringRef = iStringTable->AddString(formatString);
							aTimeAlignment->iOffsetsToTimeZonesShortNames.push_back(&stringRef);
							stringRef.IncrRefCount();
							}
						}
					}
				}
			}
		}
		
	//Iterate through the links - discarding invalid ones
	std::vector<CTzCpLink*>::iterator iter = iLinkTable->Links().begin();
	while(iter != iLinkTable->Links().end())
		{
		CTzCpZone* zone = iLinkTable->FindZone((*iter)->iLinkString->iString);
		if (zone == NULL)
			{
			//Cannot find a valid zone for this link - discard
			iter = iLinkTable->Links().erase(iter);
			}
		else
			{
			(*iter)->iLinkedZoneOffset = zone;
			++iter;
			}
		}
	}
//============================================================================
// CTZDocument:Exclude
// Removes unwanted entities from the database. Unwanted regions are specified 
// in the compiler configuration file.  Removing a region may cause certain 
// database entities to become unreferenced.  Each entity has a reference 
// count, the entities will only be persisted if their reference count > 0.
//============================================================================
void CTZDocument::Exclude()
	{
	if (! TzGlobals::iIncludeAllRegions)
		{
		bool keep = false;
		int size = iRegionsTable->Regions().size();
		for (int x = 0; x < size; ++x)
			{
			CTzCpRegion& region = *iRegionsTable->Regions()[x];
			TzGlobals::iAvailableRegions.push_back(region.iRegionNameRef->iString);

			std::vector<string>::iterator it = 
				find(TzGlobals::iIncludedRegions.begin(),TzGlobals::iIncludedRegions.end(),region.iRegionNameRef->iString);

			if (it == TzGlobals::iIncludedRegions.end())
				{
				//Need to remove this region from the region table,
				//add it to the list of excluded regions and
				//decrement all its zones reference counts by 1

				TzGlobals::iExcludedRegions.push_back(region.iRegionNameRef->iString);
				//Don't decrement the regions reference count - it will be 0 by default
				region.iRegionNameRef->DecrRefCount();
				region.iRegionalZonesIndex->DecrRefCount();
				for (int y = 0; y < region.iRegionalZonesIndex->iZoneIndex.size();++y)
					{
					region.iRegionalZonesIndex->iZoneIndex[y]->DecrRefCount();
					}
				}
			else
				{
				//Nothing really references a region, but we give it a reference count because we want to keep it
				region.IncrRefCount();
				}
			}
		}
	else
		{
		//Using all the regions.  We should still remove unreferenced database entries though
		int size = iRegionsTable->Regions().size();
		for (int x = 0; x < size; ++x)
			{
			CTzCpRegion& region = *iRegionsTable->Regions()[x];
			TzGlobals::iAvailableRegions.push_back(region.iRegionNameRef->iString);
			region.IncrRefCount();
			}
		}
		
	iRegionsTable->RemoveUnreferencedEntities();
	iZonesTable->RemoveUnreferencedEntities();
	iStdTimeAlignmentTable->RemoveUnreferencedEntities();
	iRuleSetTable->RemoveUnreferencedEntities();
	iRuleUseTable->RemoveUnreferencedEntities();
	iRuleDefinitionTable->RemoveUnreferencedEntities();
	iStringTable->RemoveUnreferencedEntities();
		
	}

//============================================================================
// CTZDocument::HandleScanError
//============================================================================
void CTZDocument::HandleScanError(const char* aFileName,int aLine,int aCol,char aChar)
	{
	stringstream os;
	os << "Error in file " << aFileName << " line:\t" << aLine << "\tColumn:\t" << aCol << "\tUnexpected character:\t" << aChar;
	iErrors.push_back(os.str());
	}
//============================================================================
// CTZDocument::ExternaliseL
// Persist the data model to a file to be included in rom
//============================================================================
void CTZDocument::ExternaliseL()
	{
	ofstream aFile(TzGlobals::iOutputFilePath.c_str(),ios::out | ios::binary);
	if (aFile.bad())
		{
		//Can't open file for writing - Abort
		throw TzGlobals::ETzAbortCreateDatabaseFile;
		}
	//Write header space at start of file
	aFile.write((char*)&iDbHeader,sizeof(iDbHeader));

	//Strings
	iStringTable->ExternaliseL(aFile);

	// add extra characters to make the file size so far a multiple of 4 
	// and avoid potential byte alignment problems
	int filePos = aFile.tellp();
	if (filePos % 4) 
		{
		char padChar = 0;
		for (int i = 4; i > filePos % 4; i--)
			{
			aFile.write(&padChar,sizeof(char));
			}
		}

	//RuleDefinitions
	iRuleDefinitionTable->ExternaliseL(aFile);
	//RuleUses
	iRuleUseTable->ExternaliseL(aFile);
	//RuleSets
	iRuleSetTable->ExternaliseL(aFile);
	//StdTimeAlignments
	iStdTimeAlignmentTable->ExternaliseL(aFile);
	//Zones
	iZonesTable->ExternaliseL(aFile);
	//Regions
	iRegionsTable->ExternaliseL(aFile);
	//Links (must be externalised after zones)
	iLinkTable->ExternaliseL(aFile);



	//After externalising all the tables, the header should now be complete.
	//Reset to the beginning of the file and rewrite the header.

	iDbHeader.iReserved1		= 0;
	iDbHeader.iReserved2		= 0;
	iDbHeader.iReserved3		= 0;
	iDbHeader.iVersion			= KTzDbVersion;
	iDbHeader.iStartYear		= TzGlobals::iStartYear;
	iDbHeader.iReserved4		= 0;
	iDbHeader.iOffsetToDefaultZone = GetDefaultZoneOffset();
	aFile.seekp(0);
	aFile.write((char*)&iDbHeader,sizeof(iDbHeader));
	aFile.close();
	}
//============================================================================
// CTZDocument::GetDefaultZoneOffset
// Finds the default zone specified in the configuration file, and returns the
// offset to this zone in the zone table
//============================================================================
TUint16 CTZDocument::GetDefaultZoneOffset() const
	{
	
	string tmpString;
	if (strcmpi("Not Set",TzGlobals::iDefaultZoneName) == 0)
		{
		tmpString = iZonesTable->Zones()[0]->iRegionNameRef->iString;
		tmpString += "/";
		tmpString += iZonesTable->Zones()[0]->iZoneNameRef->iString;
		cout << "Default zone not set.  Using " << tmpString << " instead." <<endl;
		}
	else
		{
		tmpString = TzGlobals::iDefaultZoneName;
		cout <<"Setting Default zone to " << TzGlobals::iDefaultZoneName << endl;
		}
	
	//Get default zone
	int slashChar		= tmpString.find('/',0);

	string regStr;
	string zoneStr;
	if (slashChar > 0)
		{
		regStr = tmpString.substr(0,slashChar);
		zoneStr = tmpString.substr(slashChar+1);
		}
	else
		{
		regStr = "";
		zoneStr = tmpString;
		}

	CTzCpString& regionName	= iStringTable->AddString(tmpString.substr(0,slashChar));
	CTzCpString& zoneName	= iStringTable->AddString(tmpString.substr(slashChar+1));
	
	CTzCpZone* defaultZone	= iZonesTable->GetZone(zoneName,regionName,false);

	if (defaultZone != NULL)
		{
		cout << "Found matching zone\t";
		cout << defaultZone->iRegionNameRef->iString << " ";
		cout << defaultZone->iZoneNameRef->iString << endl;
		}
	else
		{
		CTzCpString& firstZone		= iStringTable->AddString(iZonesTable->Zones()[0]->iZoneNameRef->iString);
		CTzCpString& firstRegion	= iStringTable->AddString(iZonesTable->Zones()[0]->iRegionNameRef->iString);
		
		defaultZone = iZonesTable->GetZone(firstZone,firstRegion,false);
		cout << TzGlobals::iDefaultZoneName << " not found.  Using ";
		cout << defaultZone->iRegionNameRef->iString;
		cout << "/";
		cout << defaultZone->iZoneNameRef->iString << " instead" << endl;
		}
	

	return defaultZone->iOffset;
	}
//============================================================================
// CTZDocument::DisplayNodeList
// Prints out the node list to console
//============================================================================
void CTZDocument::DisplayNodeList() const
	{
	cout <<"Nodelist: "<< endl;
	CTZNode* tmpNode;	
	CTZNode* tmpNode2;	
	
	int size = iRoot->NodeList().size();
	for (int i = 0;i < size;i++)
		{
		tmpNode = iRoot->NodeList()[i];
		int size2 = tmpNode->NodeList().size();
		for (int j = 0; j < size2;j++)
			{
			tmpNode2 = tmpNode->NodeList()[j];

			if (tmpNode2->iValue == "")
				{
				cout <<  endl << ".";
				int size3 = tmpNode2->NodeList().size();
				for (int k = 0; k < size3;k++)
					{
					cout << tmpNode2->NodeList()[k]->iValue << " ";
					}
				}
			else
				{
				cout <<  tmpNode2->iValue << " ";
				}
			}
		cout << endl;
		}
	}
//============================================================================
// CTZDocument::GetCommaSeperatedString
// Creates a comma seperated string from a string vector
//============================================================================
string CTZDocument::GetCommaSeparatedString(vector<string>& aStringVector)
	{
	string tmpString;
	int size = aStringVector.size();
	for (int j = 0; j < size;j++)
		{
		tmpString.append(aStringVector[j]);
		if (j < size-1)
			{
			tmpString.append(",");
			}
		}
	return tmpString;
	}
//============================================================================
// CTZDocument::DisplayRegionsTable
// Prints out the regions table to console
//============================================================================
void CTZDocument::DisplayRegionsTable() const
	{
	int size = iRegionsTable->Regions().size();
	for (int j = 0; j < size;j++)
		{
		cout << "Region " << j << ": " << iRegionsTable->Regions()[j]->iRegionNameRef->iString << endl;
		}
	}

//============================================================================
// CTZDocument::DisplayTimeAlignmentTable
// Prints out the Time Alignment table to console
//============================================================================
void CTZDocument::DisplayTimeAlignmentTable() const
	{
	int size = iStdTimeAlignmentTable->TimeAlignments().size();
	for (int j = 0; j < size;j++)
		{
		cout << "RuleSets " << j << ": " << iStdTimeAlignmentTable->TimeAlignments()[j]->iOffsetToTimeZoneFormatName->iString << endl;
		}
	}
//============================================================================
// CTZDocument::DisplayZonesTable
// Prints out the zones table to console
//============================================================================
void CTZDocument::DisplayZonesTable() const
	{
	int size = iZonesTable->Zones().size();
	for (int j = 0; j < size; j++)
		{
		cout << "Region " << ": " << iZonesTable->Zones()[j]->iRegionNameRef->iString;
		cout << "\tZone: " << iZonesTable->Zones()[j]->iZoneNameRef->iString << endl;
		}
	}
//============================================================================
// CTZDocument::DisplayRuleSetTable
// Prints out the ruleset table to console
//============================================================================
void CTZDocument::DisplayRuleSetTable() const
	{
	int size = iRuleSetTable->RuleSets().size();
	for (int j = 0; j < size;j++)
		{
		cout << "RuleSet " << iRuleSetTable->RuleSets()[j]->Name() << ": " << iRuleSetTable->RuleSets()[j]->RuleUses().size() << " rules\tRef:\t" << iRuleSetTable->RuleSets()[j]->ReferenceCount()<< endl;
		int size2 = iRuleSetTable->RuleSets()[j]->RuleUses().size();
		for (int k = 0; k < size2;k++)
			{
			cout << "RuleUse:\tFrom: " << iRuleUseTable->RuleUses()[k]->iPersistedEntity.iFromYear;
			cout << "\tTo: " << iRuleUseTable->RuleUses()[k]->iPersistedEntity.iFromYear << endl;
			}
		}
	}
//============================================================================
// CTZDocument::DisplayRuleDefinitionTable
// Prints out the rule definition table to console
//============================================================================
void CTZDocument::DisplayRuleDefinitionTable() const
	{
	int size = iRuleDefinitionTable->RuleDefinitions().size();
	cout << size << " RULES DEFINED" << endl;
	for (int z = 0; z < size; z++)
		{
		TTzRuleDefinition tmpDef = iRuleDefinitionTable->RuleDefinitions()[z]->iPersistedEntity;
		cout << "Rule Definition: Saving: ";
		cout << (int)(tmpDef.iStdTimeOffset);
		cout << "\tTime: " << (int)(tmpDef.iTimeOfChange) << endl;
		}
	}
//============================================================================
// CTZDocument::DisplayStringTable
// Prints out the string table to console
//============================================================================
void CTZDocument::DisplayStringTable() const
	{
	int size = iStringTable->Strings().size();
	for (int x = 0; x < size;x++)
		{
		cout << iStringTable->Strings()[x]->iString << endl;
		}
	}
//============================================================================
// CTZDocument::AssignZoneIds
// After all the zones have been assembled, we sort them alphabetically
// then assign each zone an id.  The id will be read from the configuration
// file if the zone already exists.  If this is a new zone, then the zone id
// will be equal to the next available zone id from the configuration file,
// and the next available zone id will be incremented.
//============================================================================
void CTZDocument::AssignZoneIds()
	{
	const int KMaxTzId = 0x3FFF; //Maximum numeric id that can be assigned to a zone
	sort(iZonesTable->Zones().begin(),iZonesTable->Zones().end(),CTzCpZone::SZoneFullNameSort());
	string fullZoneName;
	int size = iZonesTable->Zones().size();
	for (int i = 0; i < size;++i)
		{
		fullZoneName = iZonesTable->Zones()[i]->iRegionNameRef->iString;
		if (fullZoneName.length() > 0)
			{
			fullZoneName += '/';
			}
		fullZoneName += iZonesTable->Zones()[i]->iZoneNameRef->iString;
		int id = GetPrivateProfileInt("ZoneIdentities",fullZoneName.c_str(), KMaxTUint16, TzGlobals::iZoneIdIniFilePath.c_str());
		if (id != KMaxTUint16)
			{
			iZonesTable->Zones()[i]->iLocationId = id; 
			}
		else
			{
			iZonesTable->Zones()[i]->iLocationId = TzGlobals::iNextNumericZoneId; 

			if(TzGlobals::iNextNumericZoneId < KMaxTzId)
				{
				TzGlobals::iNextNumericZoneId			+= 8; 
				}
			else
				{
				cout << "ERROR: All available numeric ids have been used" << endl;
				throw TzGlobals::ETzAbortCreateDatabaseFile;
				}
			}
		}
	}

//============================================================================
// CTZDocument::VerifyZoneIds
// Checks that each zone id from TzIdentities.ini is present in the compiled 
// database.  If a zone is missing, a warning message is displayed.  It is not 
// an error for a zone to not be included in the database.  Possible causes
// include a change in zone name in the Olsen data sources, or compilation
// of a custom database using a limited region set.
//============================================================================
void CTZDocument::VerifyZoneIds()
	{
	//Get the filesize of TzIdentities.ini
    struct stat results;
    int iniFileSizeBytes;
    if (stat(TzGlobals::iZoneIdIniFilePath.c_str(), &results) == 0)
		{
		iniFileSizeBytes = results.st_size;
		}
    else
		{
		//It is very unlikely that the above call will fail, as we have already processed
		//the TzIdentities.ini file by this point
		cout << "WARNING: Could not retrieve TzIdentities.ini file size.  Missing zone warnings will NOT be generated" << endl;
		return;
		}

	//Create a char[] the same size as TzIdentities.ini to store the zone names
	char* zoneBuffer = NULL;   
	zoneBuffer = new char[iniFileSizeBytes];
	for (int i=0; i<iniFileSizeBytes; i++) 
		{
		zoneBuffer[i] = 0;    // Initialize all elements to zero.
		}

	//Retrieve all the zone names from TzIdentities.ini
	GetPrivateProfileString("ZoneIdentities",0,"",zoneBuffer,iniFileSizeBytes,TzGlobals::iZoneIdIniFilePath.c_str());

	//zoneBuffer now contains all the zone names, seperated by null char
	//Split zoneBuffer into seperate zone names
	string tmpString;
	int numZones = iZonesTable->Zones().size();
	int slashChar;
	string regStr;
	string zoneStr;

	for (int x = 0; x < iniFileSizeBytes; ++x)
		{
		if (zoneBuffer[x] != 0)
			{
			tmpString += zoneBuffer[x];
			}
		else
			{
			//tmpString contains a full zone name. See if it is in the database

			slashChar = tmpString.find('/',0);
			//Split into Region and Zone
			if (slashChar > 0)
				{
				regStr = tmpString.substr(0,slashChar);
				zoneStr = tmpString.substr(slashChar+1);
				}
			else
				{
				regStr = "";
				zoneStr = tmpString;
				}

			CTzCpString& regionName	= iStringTable->AddString(regStr);
			CTzCpString& zoneName	= iStringTable->AddString(zoneStr);
	
			if (iZonesTable->GetZone(zoneName,regionName,false) == NULL)
				{
				cout << "WARNING: Zone " << tmpString.c_str() << " not included in dataset" << endl;
				}

			tmpString.erase();

			//The last key is followed by 2 null chars.
			//If we find this we reached the last zone and can stop the search
			if (zoneBuffer[x+1] == 0)
				{
				break;
				}
			}
		}

	delete [] zoneBuffer;  // Free memory zoneBuffer char[]
	zoneBuffer = NULL;
	}

//============================================================================
// CTzDocument::CopyDatabaseFileToOutputDirectory
// Copies the created database file to the location specified in the
// configuration file.  If there is no location specified, does nothing
//============================================================================
void CTZDocument::CopyDatabaseFileToOutputDirectory() const
	{
	char outputDir[KMaxPathLength];
	GetPrivateProfileString("TzCompilerConfiguration","OutputDirectory","",outputDir,KMaxPathLength,TzGlobals::iIniFilePath.c_str());

	if (strcmpi("",outputDir))
		{
		//Check the output directory exists - make it if necessary
		string mkdir = "mkdir ";
		mkdir += outputDir;
		system(mkdir.c_str());	

		string copyString;

		copyString = "copy ";
		copyString += TzGlobals::iOutputFilePath;
		copyString += " ";
		copyString += outputDir;

		cout << copyString.c_str() << endl;
		system(copyString.c_str());

		}
	}
//============================================================================
// CTZDocument::ReadConfigurationFile
// Sets global variables from the configuration file.
//============================================================================
void CTZDocument::ReadConfigurationFile()
	{
	//Read the ini file to set up default parameters
	char buffer[KMaxPathLength];
	getcwd(buffer,KMaxPathLength);
	
	cout.fill('.');	
	cout << setiosflags(ios::left);
	std::string dataPath(buffer);
	TzGlobals::iZoneIdIniFilePath = dataPath + "\\TZIdentities.ini";

	dataPath += "\\Data";
	
	char includeRegions[KMaxFiles];
	char inputDir[KMaxPathLength];
	char excludedFiles[KMaxFiles];


	TzGlobals::iStartYear = GetPrivateProfileInt("TZCompilerConfiguration","EarliestDateOfInterest",KDefaultStartYear,TzGlobals::iIniFilePath.c_str());
	if (TzGlobals::iStartYear < KMinStartYear || TzGlobals::iStartYear > KMaxStartYear)
	{
		TzGlobals::iStartYear = KDefaultStartYear; //invalid year was provided, set it to default value
	}
	int len = GetPrivateProfileString("TZCompilerConfiguration","DefaultZone","Not Set",TzGlobals::iDefaultZoneName,KMaxZoneNameLength,TzGlobals::iIniFilePath.c_str());
	if (0 == len) //no DefaultZone was provided, set it to "Not Set"
	{
		strcpy(TzGlobals::iDefaultZoneName, "Not Set");
	}
	len = GetPrivateProfileString("TZCompilerConfiguration","IncludedRegions","All",includeRegions,KMaxFiles,TzGlobals::iIniFilePath.c_str());
	if (0 == len) //no includeRegions was provided, set it to "All" - default value
	{
		strcpy(includeRegions, "All");
	}
	TzGlobals::iNextNumericZoneId = GetPrivateProfileInt("NextNumericZoneId","NextNumericZoneId",KFirstZoneNumericId,TzGlobals::iZoneIdIniFilePath.c_str());
	GetPrivateProfileString("TzCompilerConfiguration","InputDirectory","",inputDir,KMaxPathLength,TzGlobals::iIniFilePath.c_str());
	GetPrivateProfileString("Files","ExcludedFiles","",excludedFiles,KMaxFiles,TzGlobals::iIniFilePath.c_str());

	//Set files to exclude
	char* token;
	token = strtok(excludedFiles,",");

	while (token != 0)
		{
		TzGlobals::iExcludedFiles.push_back(token);
		token = strtok(NULL,",");
		}

	if (strcmp(includeRegions, "All") != 0) //do not include all the regions
	{
		TzGlobals::iIncludeAllRegions = false;
		token = strtok(includeRegions,",");

		while (token != 0)
			{
			TzGlobals::iIncludedRegions.push_back(token);
			token = strtok(NULL,",");
			}
	}

	TzGlobals::iInputFilePath = inputDir;

	if (TzGlobals::iInputFilePath.empty())
		{	
		//Use current directory
		TzGlobals::iInputFilePath += dataPath;
		}

	// Add a trailing backslash if it doesn't have one
	int pathsize = TzGlobals::iInputFilePath.size();
	if ((TzGlobals::iInputFilePath).find_last_of("\\") < pathsize-1)
		{
		TzGlobals::iInputFilePath.append("\\");
		}

	TzGlobals::iOutputFilePath = buffer;
	TzGlobals::iOutputFilePath += "\\tzdb.dbz";

	// region mask is not used at the moment
	TzGlobals::iRegionMask = 0;

	CTzCpHelpers::PrintStep("Using settings from ");
	cout << TzGlobals::iIniFilePath.c_str() << endl;
	CTzCpHelpers::PrintStep("Data directory is ");
	cout << TzGlobals::iInputFilePath.c_str() << endl;
	CTzCpHelpers::PrintStep("Start Year:");
	cout << TzGlobals::iStartYear << endl;
	}
//============================================================================
// CTZDocument::WriteConfigurationFile
// Update the configuration file to reflect the new next available id
// and display the time zone identity list
//============================================================================
void CTZDocument::WriteConfigurationFile()
	{
	//Update the next available numeric zone id
	char tmp[256] = "";
	sprintf(tmp,"%d",TzGlobals::iNextNumericZoneId);
	WritePrivateProfileString("NextNumericZoneId","NextNumericZoneId",tmp,TzGlobals::iZoneIdIniFilePath.c_str());
	WritePrivateProfileString("Regions","AvailableRegions",GetCommaSeparatedString(TzGlobals::iAvailableRegions).c_str(),TzGlobals::iIniFilePath.c_str());
	WritePrivateProfileString("Regions","ExcludedRegions",GetCommaSeparatedString(TzGlobals::iExcludedRegions).c_str(),TzGlobals::iIniFilePath.c_str());
	WritePrivateProfileString("Files","AvailableFiles",GetCommaSeparatedString(TzGlobals::iAvailableFiles).c_str(),TzGlobals::iIniFilePath.c_str());
	// it is assumed that zones have been sorted by location ID before calling this
	int numZones = iZonesTable->Zones().size();
	string zoneString;
	for (int i = 0; i < numZones; i++)
		{
		char id[256] = "";
		sprintf(id,"%d",iZonesTable->Zones()[i]->iLocationId);
		zoneString = iZonesTable->Zones()[i]->iRegionNameRef->iString;
		if (zoneString.length() > 0)
			{
			zoneString += '/';
			}
		zoneString += iZonesTable->Zones()[i]->iZoneNameRef->iString;
		WritePrivateProfileString("ZoneIdentities",zoneString.c_str(),id,TzGlobals::iZoneIdIniFilePath.c_str());
		}
	}

//============================================================================
// CTZDocument::DisplayData
// Display the data that have been parsed. Very useful for diagnostic purposes.
//============================================================================
void CTZDocument::DisplayData() const
	{
	//Have a look whats going on
	DisplayNodeList();
	DisplayRuleDefinitionTable();
	DisplayRuleSetTable();
	DisplayStringTable();
	DisplayZonesTable();
	DisplayTimeAlignmentTable();
	}

//============================================================================
// End of file
//============================================================================
