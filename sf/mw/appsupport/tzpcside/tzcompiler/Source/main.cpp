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

#include "TZCompiler.h"
#include <direct.h>
#include "TzGlobals.h"
#include "TzHelpers.h"
#include "tzscanner.h"

#pragma warning(disable: 4786)

using namespace std;
//============================================================================
// main
// TZCompiler application entry point.  CTzCompiler does all the real work.
//============================================================================
int main(int argc, char* argv[])
	{
	
	if (argc > 1)
		{
		TzGlobals::iIniFilePath = argv[1];
		}
	else
		{
		//Read the ini file to set up default parameters
		char buffer[KMaxPathLength];
		getcwd(buffer,KMaxPathLength);
		std::string filePath(buffer);
		filePath += "\\TZCompiler.ini";
		ifstream file(filePath.c_str());
		if(!file)
			{
			cerr << "Aborting - Configuration file (TZCompiler.ini) not found" << endl;
			return TzGlobals::ETzAbortNoConfigFile;
			}
		TzGlobals::iIniFilePath = filePath;
		}

	CTzCpScanner* theScanner = new CTzCpScanner();
	CTZDocument* theTzDocument = new CTZDocument(*theScanner);
	theScanner->SetDocument(theTzDocument);
	CTzCompiler* theCompiler = new CTzCompiler(*theTzDocument);
	try
		{
		theCompiler->CompileL();
		// uncomment following line for diagnostic
		//theTzDocument->DisplayData();
		cerr << "Success" << endl;
		}

	//Fatal errors - abort compilation
	catch (TzGlobals::TzFaults aExceptionCode)
		{
		switch (aExceptionCode)
			{
			case TzGlobals::ETzAbortNoInputFiles:
				cerr << "Aborting - Input files not found." << endl;
				break;
			case TzGlobals::ETzAbortCreateDatabaseFile:
				cerr << "Aborting - Could not create database file." << endl;
				break;
			case TzGlobals::ETzAbortScannerSyntaxError:
				cerr << "Aborting - Syntax errors in source files" << endl;
				break;
			case TzGlobals::ETzAbortScannerFileIOError:
				cerr << "Aborting - Could not open source file for reading";
				break;
			default:
				cerr << "Aborting - Fatal Error" << endl;
				break;
			}
		return aExceptionCode;
		}

	delete theCompiler;
	delete theTzDocument;
	return TzGlobals::ETzNone;
	}

//============================================================================
// End of file
//============================================================================

