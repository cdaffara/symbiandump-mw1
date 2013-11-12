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

#ifndef __TESTTZUTIL__H
#define __TESTTZUTIL__H

#include "testtzdata.h"

// Globals used by the TestTzCompiler
const string	IniDefaultZone="DefaultZone";
const string	IniEarliestDateOfInterest="EarliestDateOfInterest";
const string	IniIncludedRegions="IncludedRegions";
const string	IniInputDirectory="InputDirectory";
const string	IniAvailableRegions="AvailableRegions";
const string	IniExcludedRegions="ExcludedRegions";
const string	ResultSuccessStatus="Success";
const string	ResultFailedStatus="Failed";
const string	ResultDefaultZonePresent="Setting Default zone to";
const string	ResultDefaultZoneAbsent="Default zone";
const string	ResultStartDate="Start Year:";
const string	ResultInputDirectory="Data directory is ";
const string	ResultAbortStatus="Aborting";
const string	DefaultZoneNotSet="not set";
const string	NoNextAvailableZoneId="ERROR: All available numeric ids";

class TestTzUtil
{
public:

	// Utility functions used by the parser
	static void RemoveInitialAndTrailingWhitespaces(string& aLine);

	static bool FindToken(string& aLine, const string& aToken);

	static bool FindValue(string& aLine, const string& aToken, string& aDelimiter, string& aValue);

	static bool FindValue(string& aLine, string& aDelimiter, string& aValue);

	static bool FindB4Value(string& aLine, string& aDelimiter, string& aValue);

	static bool FindB4Value(string& aLine, const string& aToken, string& aDelimiter, string& aValue);

};



#endif // __TESTTZCOMPILERUTIL__H

