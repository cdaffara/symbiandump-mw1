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

#ifndef __TESTTZCHECKER__H
#define __TESTTZCHECKER__H

#pragma warning(disable:4786)
#include "testtzdata.h"

// This class checks the console output generated when TzCompiler.exe is run. 
// The output is validated against the values present in the TzCompiler.ini file.
class TestTzChecker
{
public:

	TestTzChecker(TestTzData* aData);

	~TestTzChecker();

	void		ValidateDefaultZone(string& aDefaultZone);

	void		ValidateStartDate(string& aStartDate);

	void		ValidateIncludedRegions();

	void		ValidateInputDirectory(string& aInputDirectory);

	void		SetFailed();

	bool		Status();
	
private:

	// Pointer to TestTzData object
	TestTzData*	iData;

	// Bool indicating the pass/fail status of the test run.
	bool		iPassed;

};

#endif // __TESTTZCOMPILERCHECKER__H