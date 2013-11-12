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

#include "..\inc\testtzchecker.h"
#include <direct.h>

// Constructor. 
TestTzChecker::TestTzChecker(TestTzData* aData) 
: iData(aData) {}

// Destructor
TestTzChecker::~TestTzChecker() {}


// Check if the Default Zone returned in the output matches with the expected default zone.
// @param  aDefaultZone  String containing the default zone found in the output.
void TestTzChecker::ValidateDefaultZone(string& aDefaultZone)
	{
	// Compare iniString with aDefaultZone. Set iPassed to "false" if the strings do not match.
	if (strcmpi(aDefaultZone.c_str(), iData->DefaultZone().c_str()) == 0)
		{
		cout << endl << "Default Zone validated."<< endl;
		}
	else
		{
		iPassed = false;
		cout << endl << "Default Zone failed validation." << endl;
		}		
		
	cout << "Expected Zone = " << iData->DefaultZone() << endl << "Returned Zone = " << aDefaultZone << endl;
	}


// Check if the Start Date returned in the output matches with the expected value.
// @param  aStartDate  String containing the Start Date found in the output.
void TestTzChecker::ValidateStartDate(string& aStartDate)
	{
	bool rc = true;
	
	// Calculate the length of the EarliestDateOfInterest field of TzCompiler.ini
	string iniString = iData->StartDate();
	int iniStringLength = iniString.length();

	// In case, EarliestDateOfInterest does not have a value, default to "1980"
	if (iniStringLength == 0)
		{
		iniString = "1980";
		}
	else
		{
		// EarliestDateOfInterest field had a value in TzCompiler.ini file.
		// Parse iniString and check if all the digits are numeric.
		for(int i=0; i < iniStringLength; i++)
			{
			// Parse individual characters in iniString and convert them to numeric value.
			string tempString = iniString.substr(i, 1);

			// atoi() function returns "0" for both non-numeric values and "0". Ensure that
			// the 1 character string is a non-numeric character before declaring it as invalid.
			if (atoi(tempString.c_str()) == 0)
				{
				if ((strcmpi(tempString.c_str(), "0")) != 0)
					{
					// Invalid input. The iniString contains a non-numeric character. Break from the
					// loop.
					cout << endl << "Invalid input.." << endl;
					rc = false;
					break;
					}
				}
			}
		}
		
	// If value of rc is "true", compare iniString with aStartDate. If validation fails, set rc to "false".
	if (rc)
		{
		if (strcmpi(aStartDate.c_str(), iniString.c_str()) != 0)
			{
			rc = false;
			}
		}

	// Depending on the value of rc, display an appropriate message.
	if (rc)
		{
		cout << endl << "Start date validated."<< endl;
		}
	else
		{
		iPassed = false;
		cout << endl << "Start date failed validation." << endl;
		}	
						
	cout << "Expected Start Date = " << iniString << endl << "Returned Start Date = " << aStartDate << endl;
	}

// Check if the InputDirectory returned in the output matches with the expected value.
// @param  aInputDirectory  String containing the aInputDirectory found in the output.
void TestTzChecker::ValidateInputDirectory(string& aInputDirectory)
	{
	const int KMaxPathLength = 255;

	// Calculate the length of the InputDirectory field of TzCompiler.ini
	string iniString = iData->InputDirectory();
	int iniStringLength = iniString.length();

	// In case, no value is given in ini file, default to "data" folder present in the current working directory
	if (iniStringLength == 0)
		{
		char buffer[KMaxPathLength];
		getcwd(buffer,KMaxPathLength);
		string tempString(buffer);
		tempString += "\\data\\";		
		iniString = tempString;
		}

	string relativeString;
	relativeString = iniString.substr(0,1);

	if (strcmpi(relativeString.c_str(), ".") == 0)
		{
		// This is reltaive path. Prepend "." before aInputDirectory
		aInputDirectory = relativeString + aInputDirectory;
		}

		
	// Compare iniString with aInputDirectory. Set iPassed to "false" if the strings do not match.
	if (strcmpi(iniString.c_str(), aInputDirectory.c_str()) == 0)
		{
		cout << endl << "Input Directory validated."<< endl;
		}
	else
		{
		iPassed = false;
		cout << endl << "Input Directory failed validation." << endl;
		}		
			
	cout << "Expected Input Directory = " << iniString << endl << "Returned Input Directory = " << aInputDirectory << endl;		
	}


// This procedure checks if the regions present in "AvailableRegions" field are present in "IncludedRegions" field.
// The validation is only for the case when "IncludedRegions" is a comma separated list.
void TestTzChecker::ValidateIncludedRegions()
	{
	bool rc = true;
	bool matched;

	vector<string>::iterator availableRegionIterator;
	vector<string>::iterator includedRegionIterator;

	string tempAvailableString;
	string tempIncludedString;
	

	// If iData->IncludeAllRegions() is set to "true", "IncludedRegions" list is set to include all regions. 
	if (!iData->IncludeAllRegions())
		{
		if (!iData->AvailableRegions().empty())
			{
			// Nested for loop that iterates through the list of regions in "AvailableRegions" and "IncludedRegions". 
			// It checks if the regions listed in "AvailableRegions" is present in "IncludedRegions".
			
			for(availableRegionIterator = iData->AvailableRegions().begin(); availableRegionIterator != iData->AvailableRegions().end(); availableRegionIterator++)
				{
				tempAvailableString = *(availableRegionIterator);
				matched = false;
		
				for (includedRegionIterator = iData->IncludedRegions().begin(); includedRegionIterator != iData->IncludedRegions().end(); includedRegionIterator++)
					{
					tempIncludedString = *(includedRegionIterator);
					if (strcmpi(tempIncludedString.c_str(), tempAvailableString.c_str()) == 0)
						{
						// Set matched boolean to "true" indicating that the available regions is present in the 
						// list of included regions.
						matched = true;
						break;
						}
					}

				// One of the regions in "AvailableRegions" is not a part of "IncludedRegions"
				if (!matched)
					{
					cout << "AvailableRegions: " << tempAvailableString << " was not present in " << "IncludedRegions" << endl;
					iPassed = false;
					// Set rc to "false"
					if (rc)
						{
						rc = false;
						}
					}
						
				}
					
			// if rc indicates "true", all the regions have been validated.
			if (rc)
				{
				cout << "Included Regions validated" << endl;
				}
			else
				{
				cout << "Included Regions Validation failed" << endl;
				}
			}
		else
			{
			cout << "AvailableRegions list is empty" << endl;
			iPassed = false;
			}
		}
	else
		{
		cout << "All regions are included in the database" << endl;
		}
	}


// Sets iPassed to "false".
void TestTzChecker::SetFailed()
	{
	iPassed = false;
	}

// Return the value of iPassed.
bool TestTzChecker::Status()
	{
	return iPassed;
	}