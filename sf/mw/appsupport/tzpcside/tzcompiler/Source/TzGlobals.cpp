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

const char KCharOlsonStartOfComment = '#';
const char* KOlsonRuleTag = "Rule";
const char* KOlsonZoneTag = "Zone";
const char* KOlsonLinkTag = "Link";

TUint16			TzGlobals::iStartYear;
char			TzGlobals::iDefaultZoneName[KMaxZoneNameLength];
TUint16			TzGlobals::iRegionMask = 0x7F; // by default, all regions are parsed
TUint16			TzGlobals::iNextNumericZoneId = 0;
bool			TzGlobals::iIncludeAllRegions = true;
std::string		TzGlobals::iIniFilePath;
std::string		TzGlobals::iZoneIdIniFilePath;
std::string		TzGlobals::iOutputFilePath;
std::string		TzGlobals::iInputFilePath;
std::vector<std::string>	TzGlobals::iIncludedRegions;
std::vector<std::string>	TzGlobals::iExcludedRegions;
std::vector<std::string>	TzGlobals::iAvailableRegions;
std::vector<std::string>	TzGlobals::iExcludedFiles;
std::vector<std::string>	TzGlobals::iAvailableFiles;


//============================================================================
// End of file
//============================================================================
