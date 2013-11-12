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

#ifndef __TESTTZDATA__H
#define __TESTTZDATA__H

#pragma warning(disable:4786)
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

// This class will contain details about the values in ini files and the
// values found in the result.


class TestTzData
{
public:

	TestTzData();

	const string&	DefaultZone();

	const string& StartDate();

	const string& InputDirectory();

	bool IncludeAllRegions();

	vector<string>& IncludedRegions();

	vector<string>& AvailableRegions();

	void SetDefaultZone(string&);
	
	void SetStartDate(string&);

	void SetInputDirectory(string&);

	void SetIncludedRegions(string&);

	void SetAvailableRegions(string&);

	void SetIncludeAllRegions(bool);

private:
	// DefaultZone details present in ini files
	string iDefaultZone;

	// StartDate details in ini files
	string iStartdate;

	// InputDirectory details in ini files
	string iInputDirectory;

	// Bool inidcating if all regions are to be included
	bool  iIncludeAllRegions;

	// Vector for storing all the "IncludedRegions"
	vector<string> iIncludedRegions;

	// Vector for storing all the "AvailableRegions"
	vector<string> iAvailableRegions;

};

#endif // __TESTTZCOMPILERDATA__H