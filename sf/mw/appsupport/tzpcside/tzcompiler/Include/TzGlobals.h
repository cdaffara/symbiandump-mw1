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

#ifndef __TZGLOBALS_H
#define __TZGLOBALS_H
//============================================================================
// This disables a Visual Studio 6 browser symbol truncation warning which
// appears in debug builds only when using the standard template library.
// This occurs when the template names are expanded and become longer than 255
// characters.  This does not appear in release builds.
//============================================================================
#pragma warning(disable: 4786)
//============================================================================
// Include files
//============================================================================
#include <string>
#include <vector>
#include "..\..\tz\Server\Include\tzpersisted.h"
//============================================================================
// Global variables used by the Tz Database Compiler
//============================================================================

const int KDefaultStartYear = 1900;
const int KMinStartYear = 1;
const int KMaxStartYear = 2100;
const int KMaxZoneNameLength = 255;
const int KMaxPathLength = 255;
const int KMaxFiles = 255;
const int KFirstZoneNumericId = 8;
extern const char KCharOlsonStartOfComment;
extern const char* KOlsonRuleTag;
extern const char* KOlsonZoneTag;
extern const char* KOlsonLinkTag;

class TzGlobals
	{
public:

	enum TzFaults
		{
		ETzNone = 0,
		ETzAbortNoConfigFile,
		ETzAbortNoInputFiles,
		ETzAbortCreateDatabaseFile,
		ETzAbortScannerSyntaxError,
		ETzAbortScannerFileIOError,
		ETzAbortInvalidCompilerState
		};


	static TUint16	iStartYear;
	static char		iDefaultZoneName[KMaxZoneNameLength];
	static TUint16	iRegionMask;
	static TUint16	iNextNumericZoneId;
	static bool		iIncludeAllRegions;
	static std::string iIniFilePath;		//Path to the ini file in use
	static std::string iZoneIdIniFilePath;	//Zone Identity ini file
	static std::string iOutputFilePath;
	static std::string iInputFilePath;
	static std::vector<std::string> iIncludedRegions;
	static std::vector<std::string> iExcludedRegions;
	static std::vector<std::string> iAvailableRegions;
	static std::vector<std::string> iExcludedFiles;
	static std::vector<std::string> iAvailableFiles;
	};

#endif //__TZGLOBALS_H
//============================================================================
// End of file
//============================================================================
