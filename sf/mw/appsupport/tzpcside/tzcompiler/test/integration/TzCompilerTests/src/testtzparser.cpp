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

#include "..\inc\testtzparser.h"

// Constructor
TestTzParser::TestTzParser(TestTzData* aData) : iData(aData), iChecker(NULL)
	{
	iChecker = new TestTzChecker(aData);

	// If memory allocation fails, waise an exception.
	
	if (iChecker == NULL)
		{
		cout << "Failed to allocate memory for TestTzChecker object. Aborting the test" << endl;
		}
	}
	

// Destructor
TestTzParser::~TestTzParser()
	{
	if (iChecker != NULL)
		{
		delete iChecker;
		}

	}

// Parses TzCompiler.ini file and populates the data members in TestTzData object
void TestTzParser::ReadTzCompilerIniFile()
	{
	// Create a readStream object and open the TzCompiler.ini file
	ifstream readStream;
	readStream.open("TzCompiler.ini");
	
	if (readStream.is_open())
		{
		// Get a line from the file and check if any token is present there
		string line;
		
		while (!readStream.eof())
			{
			getline(readStream, line);
		
			// The presence of "[" indicates that the line is one of the headers. Ignore it.
			if (line.find_first_of("[") != string::npos)
				{
				continue;
				}

			// Look through the files for proper tokens....If they are there, process them
			string iniString;
			string delimiter;
			
			// Check if the line is about the "DefaultZone" field. If so, read the value of "DefaultZone" field 
			// and store it in TestTzData::iDefaultZone.
			delimiter = "=";
			if (TestTzUtil::FindValue(line, IniDefaultZone, delimiter, iniString) == true)
				{
				iData->SetDefaultZone(iniString);
				continue;
				}

			// Check if the line is about the "EarliestDateOfInterest" field. If so, 
			// read the value of "EarliestDateOfInterest" field and store it in TestTzData::iStartDate.
			if (TestTzUtil::FindValue(line, IniEarliestDateOfInterest, delimiter, iniString) == true)
				{
				iData->SetStartDate(iniString);
				continue;
				}
			
			// Check if the line is about the "InputDirectory" field. If so, 
			// read the value of "InputDirectory" field and store it in TestTzData::iInputDirectory.
			if (TestTzUtil::FindValue(line, IniInputDirectory, delimiter, iniString) == true)
				{
				iData->SetInputDirectory(iniString);
				continue;
				}

			// Check if the line is about the "IncludedRegions" field. If so, 
			// read the value of "IncludedRegions" field and store it in TestTzData::iIncludedRegions.	
			if (TestTzUtil::FindValue(line, IniIncludedRegions, delimiter, iniString) == true)
				{

				// "IncludedRegions" can either contain "All"(indicating that all regions should be used in the
				// database) or a list of regions to be included in the database (the regions should be separated 
				// by comma). The default value is "All".

				if ((strcmpi(iniString.c_str(), "All") == 0) || (iniString.length() == 0))
					{
					// If "IncludedRegions" is set to "All" or does not contain any value, 
					// set TestTzData::iIncludedRegions bool to true.
					iData->SetIncludeAllRegions(true);
					}
				else
					{
					// "IncludedRegions" is a comma separated list of regions. Read the regions one at a time
					// and store them in TestTzData::iIncludedRegions vector.
					string region;
					delimiter = ",";

					while (TestTzUtil::FindB4Value(iniString, delimiter, region) == true)
						{
						iData->SetIncludedRegions(region);
						}
					// Read the region in the list that comes after the last comma
					iData->SetIncludedRegions(iniString);
					}
				}
			}
		
		}
	// Close the file.
	readStream.close();
	}

// TzCompiler.exe updates TzCompiler.ini file during its execution. Parses the updated TzCompiler.ini file 
// and retrieves the AvailableRegions vector.
void TestTzParser::ReadTzRegionLists()
	{
	// Open TzCompiler.ini file for reading.
	ifstream readStream;
	readStream.open("TzCompiler.ini");

	if (readStream.is_open())
		{
		string line;
		string region;
		string delimiter = "=";

		while(!readStream.eof())
			{
			// Extract one line at a time from the file and check if it contains "AvailableRegions" field.
			getline(readStream, line);
						
			// If the line contains information about "AvailableRegions", extract the information is "region" string. 
			// "region" string would contain a comma separated list of regions that are available in the database.
			if (TestTzUtil::FindValue(line, IniAvailableRegions, delimiter, region))
				{
				// Read the regions one at a time and store them in TestTzData::iAvailableRegions vector.
				line = region;
				delimiter = ",";
				while(TestTzUtil::FindB4Value(line, delimiter, region) == true)
					{
					iData->SetAvailableRegions(region);
					}
				// Read the region after the last comma in the list
				iData->SetAvailableRegions(line);
				}
			}
		}
		// Close the file.
		readStream.close();
		

		// Finished reading from the file. Check if all regions present in "AvailableRegions" are 
		// present in "IncludedRegions" list.

		iChecker->ValidateIncludedRegions();
	}


// Parses TzIdentities.ini file
void TestTzParser::ReadTzIdentitiesIniFile()
	{}

// Parses the result file (TestTzCompiler.log) containing the console output generated by TzCompiler.exe
void TestTzParser::ReadResultFile()
	{
	// Open TestTzCompiler.log file for reading.
	ifstream readStream;
	readStream.open("TestTzCompiler.log");

	if (readStream.is_open())
		{
		while(!readStream.eof())
			{
			// Extract one line at a time from the file.
			string line;
			getline(readStream, line);
			
			if (TestTzUtil::FindToken(line, ResultAbortStatus) || TestTzUtil::FindToken(line, ResultFailedStatus))
				{
				// The execution of TzCompiler.exe was aborted or failed; this might be required by the test.
				// Display a message stating the same.
				cout << line << endl;
				}

			// Check if the line contains information about "Default Zone". If "DefaultZone" is set to a value in 
			// TzCompiler.ini, the result file should contain the following statement: "Setting Default Zone to <zone>".
			// Otherwise, the result file should contain the statement: "Default Zone not set".
			string delimiter = " ";
			string resultString;
				
			// If "DefaultZone" was set to some value in TzCompiler.ini, length of TestTzData::iDefaultZone
			// would be greater then 0.
			string tempZone = iData->DefaultZone();	
			if (tempZone.length() > 0)
				{
				// Default Zone is present
				if (TestTzUtil::FindValue(line, ResultDefaultZonePresent, delimiter, resultString))
					{
					// validate the default zone
					iChecker->ValidateDefaultZone(resultString);
					continue;
					}
				}
			else
				{
				// No default zone was present in the ini file
				delimiter = " ";
				if (TestTzUtil::FindToken(line, ResultDefaultZoneAbsent))
					{
					if (TestTzUtil::FindToken(line, DefaultZoneNotSet))
						{
						cout << endl << "Default Zone validated" << endl;
						cout << "Expected Zone = " << tempZone << endl << "Returned Zone = " << tempZone << endl;
						}
					else
						{
						cout << endl << "Default Zone validation failed" << endl;
						cout << "Expected Zone = " << tempZone << endl << "Found = " << line << endl;
						iChecker->SetFailed();
						}
					}
				}

			// If the line contains information about "Start Date", validate it.
			delimiter = ".";
			if (TestTzUtil::FindValue(line, ResultStartDate, delimiter, resultString))
				{
				iChecker->ValidateStartDate(resultString);
				}

			// If the line contains information about "InputDirectory", validate it.
			if (TestTzUtil::FindValue(line, ResultInputDirectory, delimiter, resultString))
				{
				iChecker->ValidateInputDirectory(resultString);
				}
			
			// The line may contain information about an ERROR in "NextAvailableZoneId" when the 
			// maximum number of ids has been used. Database will not be created in this case
			delimiter = "ERROR: All available numeric ids";
			if(TestTzUtil::FindValue(line, NoNextAvailableZoneId, delimiter, resultString))
				{	
				cout << endl << "The database could not be created as we ran out of numeric ids" << endl;
				break;
				}
			}
		}
	// Close the file.
	readStream.close();
			
	// Check that the TzDb database is successfully created. Try opening "TzDb.dbz" file. If it 
	// returns failure, the file is not created.
	ifstream tempStream;
	tempStream.open("Tzdb.dbz");
	tempStream.close();
									
	if (tempStream.fail())
		{
		// It is possible that the test might be expected to not create the database.
		cout << endl << "The tzdb.dbz file was not created" << endl;
		}
	else
		{	
		cout << endl << "The tzdb.dbz file was created" << endl;
		}

	// If iChecker->Status() returns true, the validation is successfully completed.
	if (iChecker->Status())
		{
		cout << "The test has passed" << endl;
		}
	else
		{
		cout << "The test has failed" << endl;
		}
	}		
