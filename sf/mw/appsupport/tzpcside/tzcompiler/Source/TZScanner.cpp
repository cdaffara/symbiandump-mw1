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
#include <vector>
#include "TZDocument.h"
#include "TZScanner.h"
#include "TZNode.h"

#include <ctype.h>

using namespace std;
//============================================================================
// CTzCpScanner::CTzCpScanner
// Parser construction
//============================================================================
CTzCpScanner::CTzCpScanner()
	{
	//Add valid characters
	iValidChars = "+-_<>=:/%";
	}
//============================================================================
// CTzCpScanner::~CTzCpScanner
// Parser destructor
//============================================================================
CTzCpScanner::~CTzCpScanner()
	{
	}
//============================================================================
// CTzCpScanner::Scan
// From MScanner
// Returns KErrNone if succesful or any other TzErrorCode
//============================================================================
int CTzCpScanner::Scan(const char* aFileName)
	{
	iFileName = aFileName;
	dbFile.open(aFileName);

	if (dbFile.is_open())
		{
		dbFile.clear();		//This resets the eof flag and character position
		scanState = EWaitFirstChar;
		iLineNo = 0;
		iColumnNo = 0;
		ControlScanning();
		dbFile.close();
		return TzGlobals::ETzNone;
		}
	else 
	//File was not opened
		{
		cout << "Cannot open " << aFileName << endl;
		throw TzGlobals::ETzAbortScannerFileIOError;
		}
	}
//============================================================================
// CTzCpScanner::ControlScanning
// Reads the data file line by line.  Each line is converted to a number of 
// CTzNodes and ownership of the node is passed to the document.
// The general structure of a node is split into elements and attributes
// An 'element' node encapsulates the entire line as read from the data file
// by owning 'attribute' nodes, the actual fields on the line.
// The decisions made here are purely based on the first character of each
// line in the file to be parsed.  The parser is unaware of the content or
// meaning of the file being parsed.
// Current rules used are:
//	1:	We are only interested in lines that start with 'R','Z','L' or '\t'
//	2:	The end of a token is a newline, tab, space or '#'
//	3:	If a line starts with a tab the element is created as a child of the 
//		previous node
//	4:	All other elements are created as chilren of the document root node
//	5:	There is no limit on the number of attributes owned by an element
//============================================================================
void CTzCpScanner::ControlScanning()

	{
	char tmpChar;
	char firstChar;
	string tmpString;

	do
		{
		if (dbFile.eof())
			tmpChar = EOF;			// define something out of the range of normal characters
		else
			{
			dbFile.get(tmpChar);	// get next char
			++iColumnNo;
			}

		switch (scanState)
			{
			case EWaitFirstChar:
				{
				++iLineNo;			// we have a new line
				iColumnNo = 0;		// reset the column count
				firstChar = tmpChar;
				tmpString.erase();	// is this the right call to empty the string ?
				if ((tmpChar == 'R') || (tmpChar == 'Z') || (tmpChar == 'L'))
					{
					tmpString += tmpChar;
					iDocument->CreateRootChildElement();
					scanState = EWaitTokenEnd;
					}
				else if (tmpChar == '\t')
					{
					iDocument->CreateChildElement();
					tmpString += tmpChar;
					// this creates an element whose name is '/t'
					// We add two '/t' to achieve the same number of fields in the Stdtimealignment and Zone lines
					iDocument->AddAttribute(tmpString.c_str());	
					iDocument->AddAttribute(tmpString.c_str());
					tmpString.erase();	// clear the string for the next state
					scanState = EWaitTokenStart;
					}
				else if (tmpChar == '\n')
					{
					// do nothing
					}
				else if ((tmpChar == KCharOlsonStartOfComment) || isspace(tmpChar))
					{
					// ignore the whole line
					scanState = EWaitLineFeed;
					}
				else if (tmpChar != EOF)					
					{
					iDocument->HandleScanError(iFileName.c_str(),iLineNo,iColumnNo,tmpChar);
					}
				}
			break;
			
			case EWaitTokenEnd:
				{
				// can we assume that there will always be a whitespace before a comment ('#')?
				// how about 'CR'; the model assumes there will be a 'CRLF', perhaps that's not right
				if ((tmpChar == ' ') || (tmpChar == '\t') || (tmpChar == '\n') || (tmpChar == EOF))
					{
					iDocument->AddAttribute(tmpString.c_str());
					tmpString.erase();		// clear the string for the next state
					if (tmpChar == '\n')
						{
						scanState = EWaitFirstChar;
						if (firstChar != 'Z')
							{
							iDocument->CloseElement();
							}
						}
					else
						{
						scanState = EWaitTokenStart;
						}
					}
				else
					{
					std::string strValidChars = iValidChars;
					int pos = strValidChars.find(tmpChar);
					if ((!isalpha(tmpChar)) && (!isdigit(tmpChar)) && (pos == string::npos))
						{
						iDocument->HandleScanError(iFileName.c_str(),iLineNo,iColumnNo,tmpChar);
						}
					
					tmpString += tmpChar;
					}
				}
			break;
			
			case EWaitTokenStart:
				{
				if (tmpChar == '#')
					{
					// ignore the whole line
					scanState = EWaitLineFeed;
					//If a comment appears on a zone line we don't want to close the element
					if (firstChar != 'Z')
						{
						iDocument->CloseElement();
						}
					}
				else if ((tmpChar != ' ') && (tmpChar != '\t') && (tmpChar != '\n')) // valid char
					{
					tmpString += tmpChar;
					scanState = EWaitTokenEnd;
					}
				}
			break;
			
			case EWaitLineFeed:
				{
				// ignore every thing till the end of line
				if (tmpChar == '\n')
					{
					scanState = EWaitFirstChar;
					}
				}
			break;

			default:
				{
				cout << "State Error!" << endl;
				}
			break;
			}
		}
		while (tmpChar != EOF);
		iDocument->CloseElement(); 
	}

//============================================================================
// End of file
//============================================================================
