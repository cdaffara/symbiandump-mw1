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
 @file IsDaylightSavingOn.cpp
*/
 
#include "IsDaylightSavingOn.h"
#include "DstStep.h"
#include "DstIntUtils.h"
#include <tz.h>



CIsDaylightSavingOn::CIsDaylightSavingOn()
 	{
 	SetTestStepName(KIsDaylightSavingOn);
 	}
 	
CIsDaylightSavingOn::~CIsDaylightSavingOn()
	{
	iTimeZoneServer.Close();
	}
	
TVerdict CIsDaylightSavingOn::doTestStepPreambleL()
	{	
	_LIT(KDstLocationTag,"City");
	_LIT(KDstTimeTag, "Time");
	_LIT(KChangedTime, "ChangedTime");
	
	TPtrC ptr;
	
	// read the time zone by name 
	GetStringFromConfig(ConfigSection(), KDstLocationTag, ptr);
	iLocation.Copy(ptr);
	
	// read the time to be set
	GetStringFromConfig(ConfigSection(), KDstTimeTag, ptr);
	iTime = CDstStep::ReadTimeParamStringL(ptr);
	
	GetStringFromConfig(ConfigSection(), KChangedTime, ptr);
	iChangedTime = CDstStep::ReadTimeParamStringL(ptr);

	
	return TestStepResult();
	}	
	
TVerdict CIsDaylightSavingOn::doTestStepL()
	{
	TBool isOn = EFalse;
	SetTestStepResult(EFail);
	// connect to the time zone server
	User::LeaveIfError(iTimeZoneServer.Connect());
	// enable auto update function - it's enabled by default but to make it sure.
	iTimeZoneServer.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOn);

	TInt err(KErrNone);
	// first we set the time zone where we want to test
	CDstIntUtils::SetHomeTimeZoneL(iLocation, iTimeZoneServer);
	
	iTimeZoneServer.SetHomeTime(iTime);
	// get time zone id
	CTzId* id = CTzId::NewL(iLocation);
	CleanupStack::PushL(id);
	err = iTimeZoneServer.IsDaylightSavingOnL(*id);
	isOn = iTimeZoneServer.IsDaylightSavingOnL(*id);
	CleanupStack::PopAndDestroy(id);
	if(isOn != EFalse)
		return TestStepResult();
	if(err != 0)
		return TestStepResult();

	iTimeZoneServer.SetHomeTime(iChangedTime);
	id = NULL;
	id = CTzId::NewL(iLocation);
	CleanupStack::PushL(id);
	err = iTimeZoneServer.IsDaylightSavingOnL(*id);
	isOn = iTimeZoneServer.IsDaylightSavingOnL(*id);

	CleanupStack::PopAndDestroy(id);
	if(isOn == EFalse)
		return TestStepResult();
	if(err != 1)
		return TestStepResult();		
	else
		SetTestStepResult(EPass);			
	
	return TestStepResult();
	}
	
TVerdict CIsDaylightSavingOn::doTestStepPostambleL()
	{
	iTimeZoneServer.Close();
	return TestStepResult();
	}
