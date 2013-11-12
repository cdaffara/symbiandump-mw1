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

/**
 @file SetSysTime.cpp
*/
#include "SetSysTime.h"

// Constant Literals
/*@{*/
_LIT(KTime,					"time");
/*@}*/

/**
 * Destructor
 */
CSetSysTime::~CSetSysTime()
	{
	}

/**
 * Constructor
 */
CSetSysTime::CSetSysTime()
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KSetSysTime);
	}

/**
 * Base class pure virtual
 * Sets the system time to a value passed from the ini
 */
TVerdict CSetSysTime::doTestStepL() 
	{
	// Connect to the TZ server and set auto update to on, as needed
	// by the DST tests. ETZAutoDSTUpdateOn enables automatic DST update
	// RPIMTestServer currently does not provide this facility
	RTz	tz;
	CleanupClosePushL(tz);
	User::LeaveIfError(tz.Connect());
	tz.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOn);
	
	TPtrC	timeStr;
	TESTL(GetStringFromConfig(ConfigSection(), KTime, timeStr));
	TTime	time(timeStr);
	
	// Set the time. We already have a conncetion to TZ.
	TInt	err = tz.SetHomeTime(time);
	SetTestStepError(err);
	CleanupStack::PopAndDestroy(&tz);
	
	PrintHomeTimeL();
	
	return TestStepResult();
	}
	
/**
 * Prints the current local time
 @test
 */
void CSetSysTime::PrintHomeTimeL()
	{
	TTime now;
	now.HomeTime();
	PrintTimeL(now);
	}

/**
 * Prints the current local time
 @param aTime Time to be printed in the specified format
 @test
 */
void CSetSysTime::PrintTimeL(const TTime& aTime)
	{
	_LIT(KTimeFmt, 				"%F%Y%M%D-%H:%T:%S");
	_LIT(KTimeLocal, 			"Local time : %s");

	TBuf<32> buf;
	aTime.FormatL(buf, KTimeFmt);
	INFO_PRINTF2(KTimeLocal, buf.PtrZ());
	}

