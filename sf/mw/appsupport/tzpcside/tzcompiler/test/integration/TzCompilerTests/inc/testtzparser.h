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

#ifndef __TESTTZPARSER__H
#define __TESTTZPARSER__H

// Parser class. Reads the ini files and the result files and
// uploads the results in the data class
#pragma warning(disable:4786)
#include "testtzchecker.h"
#include "testtzutil.h"

class TestTzParser
{
public:

	TestTzParser(TestTzData* aData);

	~TestTzParser();

	void		ReadTzCompilerIniFile();

	void		ReadTzIdentitiesIniFile();

	void		ReadResultFile();

	void		ReadTzRegionLists();

private:
	//Pointer to TestTzData object
	TestTzData*		iData;

	// Pointer to TestTzChecker object
	TestTzChecker*	iChecker;
	

};


#endif //__TESTTZCOMPILERPARSER__H