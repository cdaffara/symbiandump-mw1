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

#include "..\inc\testtzdata.h"

// Constructor. 
TestTzData::TestTzData() : iIncludeAllRegions(false) {}

// Get DefaultZone details
//@return			reference to the value of DefaultZone field in TzCompiler.ini file
const string& TestTzData::DefaultZone()
	{
	return iDefaultZone;
	}

// Get Startdate details
//@return			reference to the value of EarliestDateOfInterest field in TzCompiler.ini file
const string& TestTzData::StartDate()
	{
	return iStartdate;
	}

// Get InputDirectory details
//@return			reference to the value of InputDirectory field in TzCompiler.ini file
const string& TestTzData::InputDirectory()
	{
	return iInputDirectory;
	}

// Get the IncludedRegions vector
//@return			reference to the value of IncludedRegions field in TzCompiler.ini file
vector<string>& TestTzData::IncludedRegions()
	{
	return iIncludedRegions;
	}

// Get the Available Regions vector
//@return			reference to the value of AvailableRegions field in TzCompiler.ini file
vector<string>& TestTzData::AvailableRegions()
	{
	return iAvailableRegions;
	}

// Get the All regions included flag
//@return			true if All regions are included in the database. That is, the "IncludedRegions" field 
// in TzCompiler.ini file contains "All" or is empty.
bool TestTzData::IncludeAllRegions()
	{
	return iIncludeAllRegions;
	}

// Set DefaultZone details
//@param			aDefaultZone	string containing the default zone of the database
void TestTzData::SetDefaultZone(string& aDefaultZone)
	{
	iDefaultZone = aDefaultZone;
	}

// Set startDate details
//@param			aStartDate		string containing the value of EarliestDateOfInterest
void TestTzData::SetStartDate(string& aStartDate)
	{
	iStartdate = aStartDate;
	}

// Set inputDirectory details
//@param			aInputDirectory		The directory containing all the Olsen Tz Regions
void TestTzData::SetInputDirectory(string& aInputDirectory)
	{
	iInputDirectory = aInputDirectory;
	}

// set included regions details
//@param			aIncludedRegions	region that is to be included in the database
void TestTzData::SetIncludedRegions(string& aIncludedRegion)
	{
	iIncludedRegions.push_back(aIncludedRegion);
	}

// set available regions details
//@param			aAvailableRegions	region present in the generated database
void TestTzData::SetAvailableRegions(string& aAvailableRegion)
	{
	iAvailableRegions.push_back(aAvailableRegion);
	}

// set includeAllRegions flag
//@param			aAllRegions			boolean indicating if all regions are to be included in the database
void TestTzData::SetIncludeAllRegions(bool aAllRegions)
	{
	iIncludeAllRegions = aAllRegions;
	}