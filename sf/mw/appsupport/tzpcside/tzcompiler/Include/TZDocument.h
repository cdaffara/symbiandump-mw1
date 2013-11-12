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

#ifndef __TZDOCUMENT_H
#define __TZDOCUMENT_H

#include <vector>
#include <string>
#include "MCompiler.h"
#include "MDocument.h"
#include "TZNode.h"
#include "TZTables.h"
//============================================================================
// Forward declarations
//============================================================================
class MScanner;
class CTzCpStringTable;
class CTzCpStringTable;
class CTzCpRuleSetsTable;
class CTzCpRuleUsesTable;
class CTzCpRuleDefinitionsTable;
class CTzCpRegionsTable;
class CTzCpZonesTable;
class CTzCpLinksTable;
class CTzCpStdTimeAlignmentsTable;
//============================================================================
// CTZDocument class definition
//============================================================================
class CTZDocument : public MCompiler , public MDocument
	{
public:
	CTZDocument(MScanner& aScanner);
	~CTZDocument();

	//New methods
	void ExternaliseL();
	TUint16 GetDefaultZoneOffset() const;
	void AssignZoneIds();
	void VerifyZoneIds();
	void Exclude();								//Remove unwanted entities
	void LogScanningError(int aLineNum,int aColNum,int aType);

	//helper functions
	void CopyDatabaseFileToOutputDirectory() const;
	void DisplayData() const;

	//From MCompiler
	int Scan(const char* aFileName);
	void AssembleL();
	void Link();
	
	CTzCpStringTable* StringTable()			{ return iStringTable; }
	CTzCpRuleSetsTable* RuleSetTable()		{ return iRuleSetTable;}
	CTzCpRuleUsesTable* RuleUseTable()		{ return iRuleUseTable;}
	CTzCpRuleDefinitionsTable* RuleDefinitionTable() { return iRuleDefinitionTable;}
	CTzCpRegionsTable* RegionsTable()		{ return iRegionsTable;}
	CTzCpZonesTable* ZonesTable()			{ return iZonesTable; }
	CTzCpLinksTable* LinkTable()			{ return iLinkTable;	}
	CTzCpStdTimeAlignmentsTable* StdTimeAlignmentTable() { return iStdTimeAlignmentTable; }

	//Header
	TTzHeader* DbHeader()					{ return &iDbHeader; }

private:
	// helper functions
	std::string GetCommaSeparatedString(std::vector<std::string>& aStringVector);
	void DisplayNodeList() const;
	void DisplayRegionsTable() const;
	void DisplayZonesTable() const;
	void DisplayRuleSetTable() const;
	void DisplayRuleDefinitionTable() const;
	void DisplayStringTable() const;
	void DisplayTimeAlignmentTable() const;

	//From MDocument
	void CreateChildElement();
	void CreateRootChildElement();
	void CloseElement();
	void AddAttribute(const char* aValue);
	void HandleScanError(const char* aFileName,int aLine,int aCol,char aChar);

	void ReadConfigurationFile();				//Call on construction
	void WriteConfigurationFile();				//Call on destruction

	void AssembleRuleL(CTZNode& aNode);
	void AssembleZoneL(CTZNode& aNode);


public:
	//Scanning errors
	std::vector<std::string> iErrors;

private:
	//Data tables
	CTzCpStringTable* iStringTable;
	CTzCpRuleSetsTable* iRuleSetTable;
	CTzCpRuleUsesTable* iRuleUseTable;
	CTzCpRuleDefinitionsTable* iRuleDefinitionTable;
	CTzCpRegionsTable* iRegionsTable;
	CTzCpZonesTable* iZonesTable;
	CTzCpLinksTable* iLinkTable;
	CTzCpStdTimeAlignmentsTable* iStdTimeAlignmentTable;

	//Header
	TTzHeader iDbHeader;

	MScanner& iScanner;							//Handles the actual parsing
	CTZNode* iRoot;								//Root node
	CTZNode* iOpenedElement;					//Current node
	};

#endif //__TZDOCUMENT_H
//============================================================================
// End of file
//============================================================================
