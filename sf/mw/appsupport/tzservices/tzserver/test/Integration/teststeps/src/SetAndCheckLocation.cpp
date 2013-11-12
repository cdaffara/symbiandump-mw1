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

/**
 @file SetAndCheckLocation.cpp
*/

#include "SetAndCheckLocation.h"
#include "DstIntUtils.h"
#include <tz.h>
#include <tzconverter.h>


CSetAndCheckLocation::~CSetAndCheckLocation()
	{
	iTimeZoneServer.Close();
	}

CSetAndCheckLocation::CSetAndCheckLocation()
	{
	SetTestStepName(KSetAndCheckLocation);
	}

void CSetAndCheckLocation::PrepareTestEnvironmentL()
	{
	// Read parameters and store
	_LIT(KTagExpectedErr, "ExpectedErr");
	_LIT(KTagKErrNotFound, "KErrNotFound");

	TPtrC strPtr;

	GetStringFromConfig(ConfigSection(), KTagLocation, strPtr);
	iLocation.Copy(strPtr);
	GetIntFromConfig(ConfigSection(), KTagLocationId, iLocationId);

	GetStringFromConfig(ConfigSection(), KTagExpectedErr, strPtr);
	iExpectedErr = KErrNone;
	if(strPtr == KTagKErrNotFound)
		iExpectedErr = KErrNotFound;
	}

TInt CSetAndCheckLocation::CreateAndRunTestUnitL()
	{
	User::LeaveIfError(iTimeZoneServer.Connect());

	// Set the timezone to London for a known start condition
	TInt err = CDstIntUtils::SetHomeTimeZoneL(KEuropeLondon, iTimeZoneServer);
	if(err == KErrNone)
		{
		// Set the time zone to the specified one
		err = CDstIntUtils::SetHomeTimeZoneL(iLocation, iTimeZoneServer);
		}

	if(err == KErrNone)
		{
		// Retrieve the current timezone
		err = CDstIntUtils::CheckHomeTimeZoneL(iTimeZoneServer, iLocation);
		}

	return (err == iExpectedErr ? KErrNone : err);
	}


void CSetAndCheckLocation::DestroyTestUnit()
	{
	iTimeZoneServer.Close();
	}
