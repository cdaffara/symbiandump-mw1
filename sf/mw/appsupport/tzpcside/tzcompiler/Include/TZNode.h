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

#ifndef __TZNODE_H
#define __TZNODE_H

#include <string>
#include <vector>
#include "tzglobals.h"

//============================================================================
// CTZNode class definition
//============================================================================
class CTZNode
	{
public:
	virtual ~CTZNode() {};
	virtual std::vector<CTZNode*>& NodeList() = 0;
	virtual bool CheckNode() const = 0;
	virtual CTZNode* CreateChildElement() = 0;
	virtual void AddAttribute(const char* aValue) = 0;
	//From model
	std::string iValue;					//The actual value of the token
	CTZNode* iParent;					//This nodes parent node	
	};

//============================================================================
// CTZElement class Definition
// An element is a CTZNode which can contain other CTZNodes (CTZAttributes)
// Encapsulates a line of TZ rule specification, such as RULE, LINK, ZONE etc
//============================================================================
class CTZElement : public CTZNode
	{
private:
	~CTZElement();
	void AddAttribute(const char* aValue);
	CTZNode* CreateChildElement();
	std::vector<CTZNode*>& NodeList() { return iNodeList; }
	bool CheckNode() const;

private:
	std::vector<CTZNode*> iNodeList;	//Vector of CTZNode objects
	};
//============================================================================
// CTZAttribute class definition
// An attribute represents a field from a line of the TZ database
//============================================================================
class CTZAttribute : public CTZNode
	{
private:
	CTZNode* CreateChildElement()			{ throw TzGlobals::ETzAbortInvalidCompilerState; }
	void AddAttribute(const char* aValue)	{ throw TzGlobals::ETzAbortInvalidCompilerState;  };
	std::vector<CTZNode*>& NodeList()		{ throw TzGlobals::ETzAbortInvalidCompilerState; }
	bool CheckNode() const;
	};

#endif //__TZNODE_H
//============================================================================
// End of file
//============================================================================
