// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "UnknownZoneTime.h"

CUnknownZoneTime* CUnknownZoneTime::NewL()
	{
	CUnknownZoneTime* self = new CUnknownZoneTime();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CUnknownZoneTime::CUnknownZoneTime()
	{
	SetTestStepName(KUnknownZoneTime);
	}

void CUnknownZoneTime::ConstructL()
	{
	User::LeaveIfError(iTz.Connect());
	}

CUnknownZoneTime::~CUnknownZoneTime()
	{
	iTz.Close();
	}

TVerdict CUnknownZoneTime::doTestStepPreambleL()
	{
	return TestStepResult();
	}

/**
Base class pure virtual.
@return		EPass or EFail indicating the result of the test step.
@test
*/
TVerdict CUnknownZoneTime::doTestStepL()
	{
	TestSetUnknownZoneTimeNoPersistL();
	TestSetUnknownZoneTimePersistL();
				
	return TestStepResult();
	}

TVerdict CUnknownZoneTime::doTestStepPostambleL()
	{
	return TestStepResult();
	}

void CUnknownZoneTime::TestSetUnknownZoneTimeNoPersistL()
	{
	INFO_PRINTF1(_L("Test SetUnknowZoneTimeL without persistence"));
			
	TTime newTime;
	newTime.UniversalTime();
	newTime += TTimeIntervalHours(1);
	TTimeIntervalSeconds oldOffset = User::UTCOffset();
	TInt expectedNewOffset = (oldOffset.Int()+3600) / 60;	// Must be in minutes
	INFO_PRINTF2(_L("Expected new offset = %d minutes"), expectedNewOffset);
	iTz.SetUnknownZoneTimeL(newTime, expectedNewOffset);
	User::After(100);	// Give some time to the server to handle the request
	TTimeIntervalSeconds actualNewOffset = User::UTCOffset();
	INFO_PRINTF2(_L("Actual new offset = %d minutes"), actualNewOffset.Int()/60);
	TESTL(actualNewOffset.Int() == (oldOffset.Int()+3600));
	CTzId* id = iTz.GetTimeZoneIdL();
	CleanupStack::PushL(id);
	const TUint32 KUnknownTZId = 0x0ffffff0;
	TESTL(id->TimeZoneNumericID() == KUnknownTZId);
	CleanupStack::PopAndDestroy(id);
		
	iTz.Close();
	User::After(100000);
	User::LeaveIfError(iTz.Connect());
	id = iTz.GetTimeZoneIdL();
	CleanupStack::PushL(id);
	INFO_PRINTF2(_L("Zone id = %x"), id->TimeZoneNumericID());
	TESTL(id->TimeZoneNumericID() != KUnknownTZId);
	CleanupStack::PopAndDestroy(id);
	}

void CUnknownZoneTime::TestSetUnknownZoneTimePersistL()
	{
	INFO_PRINTF1(_L("Test SetUnknowZoneTimeL with persistence"));
	
	TTime newTime;
	newTime.UniversalTime();
	newTime += TTimeIntervalHours(1);
	TTimeIntervalSeconds oldOffset = User::UTCOffset();
	TInt expectedNewOffset = (oldOffset.Int()+3600) / 60;	// Must be in minutes
	INFO_PRINTF2(_L("Expected new offset = %d minutes"), expectedNewOffset);
	iTz.SetUnknownZoneTimeL(newTime, expectedNewOffset, ETrue);
	User::After(100);	// Give some time to the server to handle the request
	TTimeIntervalSeconds actualNewOffset = User::UTCOffset();
	INFO_PRINTF2(_L("Actual new offset = %d minutes"), actualNewOffset.Int()/60);
	TESTL(actualNewOffset.Int() == (oldOffset.Int()+3600));
	CTzId* id = iTz.GetTimeZoneIdL();
	CleanupStack::PushL(id);
	const TUint32 KUnknownTZId = 0x0ffffff0;
	INFO_PRINTF2(_L("Zone id = %x"), id->TimeZoneNumericID());
	TESTL(id->TimeZoneNumericID() == KUnknownTZId);
	CleanupStack::PopAndDestroy(id);
	
	iTz.Close();
	User::After(100000);
	User::LeaveIfError(iTz.Connect());
	id = iTz.GetTimeZoneIdL();
	CleanupStack::PushL(id);
	TESTL(id->TimeZoneNumericID() == KUnknownTZId);
	CleanupStack::PopAndDestroy(id);
	}
