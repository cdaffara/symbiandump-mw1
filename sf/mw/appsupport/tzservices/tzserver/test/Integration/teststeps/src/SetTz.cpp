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
 @file SetTz.cpp
*/
#include "SetTz.h"
#include "DstStep.h"
#include "DstIntUtils.h"
#include <tz.h>

// Constant Literals
/*@{*/
_LIT(KTimeZone,					"timezone");

const TInt KMaxLocationLength = 64;
/*@}*/

/**
 * Destructor
 */
CSetTz::~CSetTz()
	{
	}

/**
 * Constructor
 */
CSetTz::CSetTz()
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KSetTz);
	}

/**
Base class pure virtual
Sets the current timezone to a value passed from the ini
*/
TVerdict CSetTz::doTestStepL()
	{
	RTz	tz;
	CleanupClosePushL(tz);
	User::LeaveIfError(tz.Connect());
		
	TPtrC	timeZone;
	GetStringFromConfig(ConfigSection(), KTimeZone, timeZone);

	TBuf8<KMaxLocationLength> location;
	location.Copy(timeZone);
	
	CTzId* tzId = CTzId::NewL(location);
	
	CleanupStack::PushL(tzId);
	
	// Set the time zone
	TRAPD(err, tz.SetTimeZoneL(*tzId));
    //Added Test code for Defect 142081
	if(err == KErrNotFound)
	    {
	    ERR_PRINTF2(_L("Unable to set the Tz [%S] as it is Invalid"), &timeZone);
	    CleanupStack::PopAndDestroy(tzId);
	    CleanupStack::PopAndDestroy(&tz);
	    User::Leave(err);
	    }

	CTzId* tzId1 = NULL;
	
	TRAP(err,tzId1 = tz.GetTimeZoneIdL());
	TESTL(err == KErrNone);
	CleanupStack::PushL(tzId1);
	
	TESTL(tzId1->TimeZoneNameID() == location);
	
	CleanupStack::PopAndDestroy(tzId1);
	CleanupStack::PopAndDestroy(tzId);
	CleanupStack::PopAndDestroy(&tz);

	PrintHomeTimeL();

	return TestStepResult();
	}
	
/**
 * Prints the current local time
 @test
 */
void CSetTz::PrintHomeTimeL()
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
void CSetTz::PrintTimeL(const TTime& aTime)
	{
	_LIT(KTimeFmt, 				"%F%Y%M%D-%H:%T:%S");
	_LIT(KTimeLocal, 			"Local time : %s");

	TBuf<32> buf;
	aTime.FormatL(buf, KTimeFmt);
	INFO_PRINTF2(KTimeLocal, buf.PtrZ());
	}

