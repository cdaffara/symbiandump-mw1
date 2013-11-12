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
//

#ifndef __TZPERSISTED_H__
#define __TZPERSISTED_H__

#include <tzdefines.h> // enumerated types used by client, server and compiler

// This file is shared between the Symbian OS TZ component and the ISO C++ TZ Compiler.
// Hence the need to define the Symbian OS types on both platforms:
#if defined(__SYMBIAN32__)

	#include <e32std.h>

#else

	typedef void TAny;
	typedef signed char TInt8;
	typedef unsigned char TUint8;
	typedef short int TInt16;
	typedef unsigned short int TUint16;
	typedef long int TInt32;
	typedef unsigned long int TUint32;
	typedef signed int TInt;
	typedef unsigned int TUint;
	typedef float TReal32;
	typedef double TReal64;
	typedef double TReal;
	typedef unsigned char TText8;
	typedef unsigned short int TText16;
	typedef int TBool;


	const TInt KMaxTInt8=0x7f;
	const TInt KMinTInt8=(-128);
	const TUint KMaxTUint8=0xffu;
	const TInt KMaxTInt16=0x7fff;
	const TInt KMinTInt16=(-32768);
	const TUint KMaxTUint16=0xffffu;
	const TInt KMaxTInt32=0x7fffffff;
	const TInt KMinTInt32=(TInt)0x80000000;
	const TUint KMaxTUint32=0xffffffffu;
	const TInt KMaxTInt=0x7fffffff;
	const TInt KMinTInt=(TInt)0x80000000;
	const TUint KMaxTUint=0xffffffffu;

#endif

//============================================
// Time Zone Database Version Number
const TInt KTzDbVersion = 1;

const TInt KMaxYear = 9999;
const TInt KMinYear = 0;
const TInt KMaxDayOfMonth = 30;
const TInt KMinutesInOneDay = 1440;
const TInt KMaxDstOffset = 240; // 4 hours
//============================================

//==============================================================================
//
// Definitions of persisted entity classes. This is how the TZ entities are 
// stored in the data file. The class definitions are based on the Persisted Data
// Format specification.
//================================================================================

//==============================================================================
//  OFFSET ARRAYS:
//
// When a class must encapsulate a collection of references (offsets) to other 
// objects, this is represented as 1-element arrays.
//
// From the class' point of view there is only a single element in the array.
// In reality, there will be more or less than one element, depending on the value of 
// TTzWhatever::iNumberOfWhateverElements. The actual offsets will be persisted IMMEDIATELY
// AFTER the class, thus becoming an extension of the array. We can then get the i-th
// element by doing TTzWhatever::iOffsetsToWhateverElements[i].
//
// ================================================================================
//
// FILE HEADER
//
class TTzHeader
	{
public:
	TUint8 iReserved1;
	TUint8 iReserved2;
	TUint8 iReserved3;
	TUint8 iVersion;
	TUint16 iStartYear;
	TUint16 iReserved4;

	TUint32 iOffsetToStringTable;
	TUint32 iOffsetToRegionsTable;
	TUint32 iOffsetToRegionalZonesTable;
	TUint32 iOffsetToZones;
	TUint32 iOffsetToZonesTable;
	TUint32 iOffsetToLinksTable;
	TUint32 iOffsetToStdTimeAlignmentsTable;
	TUint32 iOffsetToRuleSetsTable;
	TUint32 iOffsetToRuleUsesTable;
	TUint32 iOffsetToRuleDefinitionsTable;
	TUint32 iOffsetToDefaultZone;
	};


// SIMPLE ENTITIES

class TTzRegion
	{
public:
	TUint16 iOffsetToRegionName;
	TUint16 iOffsetToRegionalZoneIndex;
	};

class TTzRegionalZoneIndex
	{
public:
	TUint8 iNumberOfZones; // no. of zones IN THIS REGION
	TUint16 iOffsetsToZones[1];
	};

class TTzZone
	{
public:
	TUint16 iLocationId;
	TUint16 iOffsetToZoneName;
	TUint16 iOffsetToRegionName;
	TUint16 iNumberOfStdTimeAlignments;
	TUint16 iOffsetsToTimeAlignments[1];
	};

class TTzLink
	{
public:
	TUint16 iOffsetToLinkName;
	TUint16 iOffsetToZone;
	};

class TTzStdTimeAlignment
	{
public:
	TUint16 iUntilYear;
	TUint8 iUntilMonth;
	TUint8 iUntilDayOfMonth;
	TUint16 iUntilTimeInMinutes; // minutes from 0:00
	TUint8 iUntilTimeReference; // TTzTimeReference
	
	TUint8 iNumberOfTimeZones;
	TInt16 iUtcOffset; // in minutes (signed)
	TUint16 iOffsetToRuleSet;
	TUint16 iOffsetToTimeZoneFormatName;
	TUint16 iOffsetsToTimeZones[1];
	};

class TTzRuleSet
	{
public:
	TUint16 iNumberOfRuleUses;
	TUint16 iOffsetsToRuleUses[1];
	};

class TTzRuleUse
	{
public:
	TUint16 iFromYear;
	TUint16 iUntilYear;
	TUint16 iOffsetToRuleDefinition;
	TUint16 iOffsetToRuleLetterString;
	};

class TTzRuleDefinition
	{
public:
	TUint8 iStdTimeOffset;	// in minutes
	TUint8 iMonth;
	TUint8 iDayRule;		// TTzRuleDay
	TUint8 iDayOfMonth;
	TUint8 iDayOfWeek;
	TUint8 iTimeReference;	// TTzTimeReference
	TUint16 iTimeOfChange;	// number of minutes: "02:00" becomes 120 minutes
	};

//
// TABLES
// Persisted tables consist of the number of elements in the table and a
// collection of offsets to those elements
//
class TTzStringsTable
	{
public:
	TUint16 iNumberOfStrings;
	TUint16 iOffsetsToStrings[1];
	};

class TTzRegionsTable
	{
public:
	TUint16 iNumberOfRegions;
	TTzRegion iRegions[1];
	};

class TTzRegionalZonesTable
	{
public:
	TUint8 iNumberOfRegionalZoneIndices;
	TUint16 iOffsetsToRegionalZoneIndices[1];
	};

class TTzZonesTable
	{
public:
	TUint16 iNumberOfZones;
	TUint16 iOffsetsToZones[1];
	};

class TTzLinksTable
	{
public:
	TUint16 iNumberOfLinks;
	TTzLink iLinks[1];
	};

class TTzStdTimeAlignmentsTable
	{
public:
	TUint16 iNumberOfStdTimeAlignments;
	TUint16 iOffsetsToStdTimeAlignments[1];
	};

class TTzRuleSetsTable
	{
public:
	TUint16 iNumberOfRuleSets;
	TUint16 iOffsetsToRuleSets[1];
	};

class TTzRuleUsesTable
	{
public:
	TUint16 iNumberOfRuleUses;
	TUint16 iOffsetsToRuleUses[1];
	};

class TTzRuleDefinitionsTable
	{
public:
	TUint16 iNumberOfRuleDefinitions;
	TUint16 iOffsetsToRuleDefinitions[1];
	};

#endif // __TZPERSISTED_H__
