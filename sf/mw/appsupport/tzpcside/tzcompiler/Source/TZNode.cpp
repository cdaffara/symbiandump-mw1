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

#include "TZNode.h"
#include <iostream>

//============================================================================
// CTZElement::~CTZElement
//============================================================================
CTZElement::~CTZElement()
	{
	for (int i = 0; i < NodeList().size();++i)
		{
		delete NodeList()[i];
		}
	}
//============================================================================
// CTZElement::CheckNode
// Checks to see that the node actually contains information and is valid for
// assembly.  Invalid nodes may be generated if a file contains a comment
// on a line beginning with empty tabs.  If this happens, a node with empty
// tab elements will be generated.
//============================================================================
bool CTZElement::CheckNode() const
	{
	char startChar;
	int size = iNodeList.size();
	for (int x = 0; x < size;x++)
		{
		startChar = iNodeList[x]->iValue[0]; //First character of attribute
		if ((startChar != '\t') && (startChar != KCharOlsonStartOfComment) && (startChar != ' '))
			{
			return true;
			}
		}
	return false;
	}

bool CTZAttribute::CheckNode() const
	{
	char startChar = iValue[0]; //First character of attribute
	if ((startChar != '\t') && (startChar != KCharOlsonStartOfComment) && (startChar != ' '))
		{
		return true;
		}
	return false;
	}

//============================================================================
// CTZElement::AddAttribute
// Adds an attribute to the current node
//============================================================================
void CTZElement::AddAttribute(const char* aValue)
	{
	CTZNode* aAttribute		= new CTZAttribute();
	aAttribute->iParent		= this;
	aAttribute->iValue		= aValue;
	iNodeList.push_back(aAttribute);
	}

//============================================================================
// CTZElement::AddChildElement
// Creates a new CTZElement as a child node of the current element.
//============================================================================
CTZNode* CTZElement::CreateChildElement()
	{
	CTZElement* childElement = new CTZElement();
	childElement->iParent = this;
	iNodeList.push_back(childElement);
	return childElement;
	}
//============================================================================
// End of file
//============================================================================
