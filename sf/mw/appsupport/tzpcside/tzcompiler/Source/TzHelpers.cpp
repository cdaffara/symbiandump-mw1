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
#include "TzHelpers.h"
#include "TzTables.h"
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;
//============================================================================
// CTzCpHelpers::GetMonth
//============================================================================
int CTzCpHelpers::GetMonth(string aMonthString)
	{
	int month = -1;
	if		(aMonthString == "Jan")	{month = EJan;}
	else if (aMonthString == "Feb")	{month = EFeb;}
	else if (aMonthString == "Mar")	{month = EMar;}
	else if (aMonthString == "Apr")	{month = EApr;}
	else if (aMonthString == "May")	{month = EMay;}
	else if (aMonthString == "Jun")	{month = EJun;}
	else if (aMonthString == "Jul")	{month = EJul;}
	else if (aMonthString == "Aug")	{month = EAug;}
	else if (aMonthString == "Sep")	{month = ESep;}
	else if (aMonthString == "Oct")	{month = EOct;}
	else if (aMonthString == "Nov")	{month = ENov;}
	else if (aMonthString == "Dec")	{month = EDec;}
	return month;
	}
//============================================================================
// CTzCpHelpers::GetDay
//============================================================================
int CTzCpHelpers::GetDay(string aDayString)
	{
	// aDayString can come in the form "Sun>=7" or as "7"
	// Return 0 if no day of week is contained in the string
	int day = 0;
	// day could be expressed as "Mon" or as "Monday"
	if		(strstr(aDayString.c_str(),"Mon") != NULL) { day = EMon;}
	else if (strstr(aDayString.c_str(),"Tue") != NULL) { day = ETue;}
	else if (strstr(aDayString.c_str(),"Wed") != NULL) { day = EWed;}
	else if (strstr(aDayString.c_str(),"Thu") != NULL) { day = EThu;}
	else if (strstr(aDayString.c_str(),"Fri") != NULL) { day = EFri;}
	else if (strstr(aDayString.c_str(),"Sat") != NULL) { day = ESat;}
	else if (strstr(aDayString.c_str(),"Sun") != NULL) { day = ESun;}
	return day;
	}
//============================================================================
// CTzHelpers::GetTimeOfDayInMinutes
// Expects a string containing a time in the format 'HH:MM' or just the number
// of hours (eg- 'H') and returns the time in minutes. The ':' may not be 
// specified in some rule files if there are no minutes.
//============================================================================
TInt16 CTzCpHelpers::GetTimeOfDayInMinutes(std::string aTimeString)
	{
	bool negativeTime = false;
	string tmpString;
	//Check for negative number
	if (aTimeString[0] == '-')
		{
		negativeTime = true;
		tmpString = (aTimeString.substr(1));
		}
	else
		{
		tmpString = aTimeString;
		}

	int hours	= atoi(tmpString.c_str());
	
	// Get the number of minutes. If the colon isn't specified then there are 
	// 0 minutes.
	int minutes	=0;
	int breakAt	= tmpString.find_first_of(':');
	if (breakAt != string::npos)
		{
		minutes = atoi(tmpString.substr(breakAt+1).c_str());
		}

	int timeInMinutes = (hours*60) + minutes;
	if (negativeTime)
		{
		timeInMinutes *= -1;
		}
	return timeInMinutes;
	}
//============================================================================
// CTzCpHelpers::GetTimeReference
//============================================================================
int CTzCpHelpers::GetTimeReference(std::string aTimeString)
	{
	int timeRefValue = ETzWallTimeReference;
	char timeRef = aTimeString.c_str()[aTimeString.length()-1];
	switch (timeRef)
		{
		case 'u':
			timeRefValue = ETzUtcTimeReference;
		break;
		case 's':
			timeRefValue = ETzStdTimeReference;
		break;
		default:
			timeRefValue = ETzWallTimeReference;
		break;
		}
	return timeRefValue;
	}
//============================================================================
// CTzCpHelpers::GetFileSizeInBytes
//============================================================================
int CTzCpHelpers::GetFileSizeInBytes(std::string aFileName)
	{
	ifstream aFile;
	aFile.open(aFileName.c_str());
	aFile.seekg(0,ifstream::end);
	int fileSize = aFile.tellg();
	aFile.seekg(0);
	aFile.close();
	return fileSize;
	}
//============================================================================
// CTzCpHelpers::PrintStep
//============================================================================
void CTzCpHelpers::PrintStep(std::string aStep)
	{
	cout.width(50);
	cout << aStep;
	}
//============================================================================
// CTzCpHelpers::PrintStepResult
//============================================================================
void CTzCpHelpers::PrintStepResult(int aResult)
	{
	switch (aResult)
		{
		case TzGlobals::ETzNone:
			cerr << "OK" << endl;
			break;
		case TzGlobals::ETzAbortNoInputFiles:
			cerr << "Error - Input files not found." << endl;
			break;
		case TzGlobals::ETzAbortCreateDatabaseFile:
			cerr << "Error - Could not create database file." << endl;
			break;
		case TzGlobals::ETzAbortScannerSyntaxError:
			cerr << "Error - Syntax errors in source files." << endl;
			break;
		case TzGlobals::ETzAbortScannerFileIOError:
			cerr << "Error - Could not open source file for reading." << endl;
			break;
		default:
			cerr << "Error - Unknown" << endl;
			break;
		}
	}

//============================================================================
// End of file
//============================================================================
