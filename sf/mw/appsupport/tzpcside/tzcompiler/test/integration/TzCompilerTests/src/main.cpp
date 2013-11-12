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
#include "..\inc\testtzchecker.h"

#pragma warning(disable:4786)

// Invokes TzCompiler.exe and stores the console output in a log file.
void RunTzCompiler()
	{
	system("TzCompiler.exe > TestTzCompiler.log 2>&1");
	}

// Reads the ini files, runs TzCompiler.exe and validates the result.
int main(int argc, char* argv[])
	{
	try
		{
		// Create the data object
		TestTzData*		data = new TestTzData();
		if (data == NULL)
			{
			// Exception caused due to memory allocation failure.
			throw "Failed to allocate memory for TestTzData object. Test aborted";
			}

		// Create the parser object
		TestTzParser*	parser = new TestTzParser(data);
		if (parser == NULL)
			{
			// Exception caused due to memory allocation failure.
			throw "Failed to allocate memory for TestTzParser object. Test aborted";
			}

		// Read TzCompiler.ini file
		parser->ReadTzCompilerIniFile();

		// Read TzIdentities.ini file
		parser->ReadTzIdentitiesIniFile();

		// Run TzCompiler.exe
		RunTzCompiler();

		// Read TzCompiler.ini "Available Regions" and validates the regions included.
		parser->ReadTzRegionLists();

		// Read the results file and validate it
		parser->ReadResultFile();

		delete parser;
		parser = NULL;
		
		delete data;
		data = NULL;
		}

	catch (char* str)
		{
		cout << "Exception raised !!" << endl << str << endl;
		}
	
	return (0);
	}