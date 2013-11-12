// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "..\inc\testtzutil.h"

// Remove whitespaces from a string
//@param	aLine	string in which whitespaces are to be removed
void TestTzUtil::RemoveInitialAndTrailingWhitespaces(string& aLine)
	{
	const char* ws = " \t\r";
	string tempLine = aLine;

	// Remove initial space
	string::size_type not_white;
	not_white = tempLine.find_first_not_of(ws);
	tempLine.erase(0, not_white);

	// Remove trailing space
	not_white = tempLine.find_last_not_of(ws);
	tempLine.erase(not_white+1);

	aLine = tempLine;	
	}

// Check if a token (a string) is present in another string
//@return	bool	return true	if aToken is present in aLine
//@param	aLine	string in which the token is to be found
//@param	aToken	string that is to be searches for in aLine
bool TestTzUtil::FindToken(string& aLine, const string& aToken)
	{
	bool rc = false;

	if (aLine.find(aToken.c_str()) != string::npos)
		{
		rc = true;
		}
	return rc;
	}


// Find the last position where aDelimiter is present in aLine. Store the string after aDelimiter in aValue.
//@return	bool		return true if a value is retrieved after aDelimiter match
//@param	aLine		modified string that contains the part of the string after aToken
//@param	aDelimiter	generally a single character string like ",", ".", etc.
//@param	aValue		string containing the value, i.e., the part of the string after aDelimiter.
bool TestTzUtil::FindValue(string& aLine, string& aDelimiter, string& aValue)
	{
	bool rc = false;
	string::size_type loc;

	loc = aLine.find_last_of(aDelimiter);

	if (loc != string::npos)
		{
		string tempString;
		tempString = aLine.substr(loc+1);

		// Trim the leading and trailing spaces
		RemoveInitialAndTrailingWhitespaces(tempString);
		aValue = tempString;
		rc = true;
		}
	
		return rc;
	}

// Check if a string(aToken) is present in another string(aLine). If present, then find the last position 
//where aDelimiter is present in aLine. Store the string after aDelimiter in aValue.
//@return	bool	return true if a value is retrieved after aToken match and aDelimiter match
//@param	aLine		modified string that contains the part of the string after aToken
//@param	aToken		string that needs to be searched for
//@param	aDelimiter	generally a single character string like ",", ".", etc.
//@param	aValue		string containing the value, i.e., the part of the string after aDelimiter.
bool TestTzUtil::FindValue(string& aLine, const string& aToken, string& aDelimiter, string& aValue)
	{
	bool rc = false;
	string::size_type loc;
	
	loc = aLine.find(aToken.c_str());
	if (loc != string::npos)
		{
		int tokenLength = aToken.length();

		aLine = aLine.substr(loc+tokenLength);

		rc = FindValue(aLine, aDelimiter, aValue);
		}
		return rc;
	}


// Find the first position where aDelimiter is present in aLine. Store the string before aDelimiter in aValue.
//@return	bool		return true if a value is retrieved after aDelimiter match
//@param	aLine		modified string that contains the part of the string after aToken
//@param	aDelimiter	generally a single character string like ",", ".", etc.
//@param	aValue		string containing the value, i.e., the part of the string before aDelimiter.
bool TestTzUtil::FindB4Value(string& aLine, string& aDelimiter, string& aValue)
	{
	bool rc = false;
	string::size_type loc;

	loc = aLine.find_first_of(aDelimiter);

	if (loc != string::npos)
		{
		string tempString;
		tempString = aLine.substr(0, loc);

		// Trim the leading and trailing spaces
		RemoveInitialAndTrailingWhitespaces(tempString);
		aValue = tempString;
		rc = true;
		}

	aLine = aLine.substr(loc+1);
	
	return rc;
	}

// Find the first position where aDelimiter is present in aLine. Store the string before aDelimiter in aValue.
//@return	bool		return true if a value is retrieved after aDelimiter match
//@param	aToken		string that needs to be searched for
//@param	aLine		modified string that contains the part of the string after aToken
//@param	aDelimiter	generally a single character string like ",", ".", etc.
//@param	aValue		string containing the value, i.e., the part of the string before aDelimiter.
bool TestTzUtil::FindB4Value(string& aLine, const string& aToken, string& aDelimiter, string& aValue)
	{
	bool rc = false;
	string::size_type loc;

	loc = aLine.find(aToken);
	if (loc != string::npos)
		{
		int tokenLength = aToken.length();
		aLine = aLine.substr(loc+tokenLength);

		rc = FindB4Value(aLine, aDelimiter, aValue);		
		}
		return rc;
	}