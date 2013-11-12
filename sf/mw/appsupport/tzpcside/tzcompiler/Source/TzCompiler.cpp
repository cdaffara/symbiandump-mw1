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
#include "TzCompiler.h"
#include "TzDocument.h"
#include "TzHelpers.h"
#include "TzScanner.h"
#include <iostream>
#include <io.h>
#include <direct.h>
#include <algorithm>

using namespace std;

//============================================================================
// CTzCompiler::CTZCompiler
//============================================================================
CTzCompiler::CTzCompiler(CTZDocument& aTzDocument)
:	iTzDocument(aTzDocument)
	{
	}

//============================================================================
// CTzCompiler::Compile
// Directs the compilation process through the document
//============================================================================
void CTzCompiler::CompileL()
	{
	string outputString;
	//Use all files in the data directory
	
	int numFiles = 0;		//Number of files scanned
	int scanSuccess = 0;	//Succesful scans

	struct _finddata_t file;
	long fileHandle;
	string fileName;
	//Find all files in directory
	string inputFilePath = TzGlobals::iInputFilePath + "\\*";
	fileHandle = _findfirst(inputFilePath.c_str(),&file);
	std::vector<std::string>::iterator iter;
	while (_findnext(fileHandle,&file) == 0)
		{
		numFiles++;
		fileName.erase();
		fileName  = file.name;
		if (!(strcmp(file.name, ".") == 0 ||  strcmp(file.name, "..") == 0))
			{
			TzGlobals::iAvailableFiles.push_back(file.name);

			//Check that we don't want to exclude this file
			iter = find(TzGlobals::iExcludedFiles.begin(),TzGlobals::iExcludedFiles.end(),file.name);
			
			if (iter != TzGlobals::iExcludedFiles.end())
				{
				//Don't process this file
				cout << "Ignoring " << file.name << endl;
				}
			else
				{
				outputString.erase();
				outputString = "Scanning ";
				outputString.append(file.name);
				CTzCpHelpers::PrintStep(outputString);
					
				string tmpString = TzGlobals::iInputFilePath;
				tmpString.append(file.name);
				int result = iTzDocument.Scan(tmpString.c_str());
				if (result == TzGlobals::ETzNone)
					{
					scanSuccess++;
					}

				CTzCpHelpers::PrintStepResult(result);
				}
			}

		}

	//Check at least one file was scanned.  If not we have a fatal error
	if (scanSuccess < 1)
		{
		throw TzGlobals::ETzAbortNoInputFiles;
		}

	if (iTzDocument.iErrors.size() > 0)
		{
		cerr << "Scan failed" << endl;
		for (int x = 0; x < iTzDocument.iErrors.size();++x)
			{
			cerr << iTzDocument.iErrors[x] << endl;
			}
		throw TzGlobals::ETzAbortScannerSyntaxError;
		}
	
	//Assemble the parsed data into Symbian native database entities
	iTzDocument.AssembleL();
    CTzCpHelpers::PrintStepResult(TzGlobals::ETzNone);
	//Link the assembled objects 
	iTzDocument.Link();
	CTzCpHelpers::PrintStepResult(TzGlobals::ETzNone);
	//Assign each zone a unique id
	iTzDocument.AssignZoneIds();
	//Remove unwanted entities from the database
	iTzDocument.Exclude();
	//Persist the database model to a binary Symbian file for inclusion in ROM
	iTzDocument.ExternaliseL();
	CTzCpHelpers::PrintStepResult(TzGlobals::ETzNone);
	//Show warnings for missing zones
	iTzDocument.VerifyZoneIds();
	//Move the database file to the required output directory
	iTzDocument.CopyDatabaseFileToOutputDirectory();
	
	cout << "Output files are:\t" << TzGlobals::iOutputFilePath.c_str() << endl;
	}

//============================================================================
// End of file
//============================================================================
	
		
