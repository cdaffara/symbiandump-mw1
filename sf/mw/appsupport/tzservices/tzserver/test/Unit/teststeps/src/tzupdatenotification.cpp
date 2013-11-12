// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 

#include "tzupdatenotification.h"
#include <tz.h>
#include <tzupdate.h>
#include <utf.h>

_LIT(KLocationTag1, "Location1");
_LIT(KLocationTag2, "Location2");

CTzPropertyWatcher* CTzPropertyWatcher::NewL()
	{
	CTzPropertyWatcher* self = new (ELeave) CTzPropertyWatcher;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTzPropertyWatcher::CTzPropertyWatcher() : CActive(EPriorityLow)
	{
	CActiveScheduler::Add(this);
	}
	
CTzPropertyWatcher::~CTzPropertyWatcher()
	{
	Cancel();
	}

void CTzPropertyWatcher::ConstructL()
	{
	User::LeaveIfError(iProperty.Attach(NTzUpdate::KPropertyCategory, NTzUpdate::ECurrentTimeZoneId));
	}
	
void CTzPropertyWatcher::Start()
	{
	if (!IsActive())
		{
		iProperty.Subscribe(iStatus);
		SetActive();		
		}
	}

NTzUpdate::TTimeZoneChange CTzPropertyWatcher::GetChangeDetails() const
	{
	TPckgBuf<NTzUpdate::TTimeZoneChange> changeBuf;
	iProperty.Get(NTzUpdate::KPropertyCategory, NTzUpdate::ECurrentTimeZoneId, changeBuf);
	return changeBuf();
	}
	
void CTzPropertyWatcher::RunL()
	{
	CActiveScheduler::Stop();
	}

void CTzPropertyWatcher::DoCancel()
	{
	iProperty.Cancel();
	}
	
//
	
TInt CTzUpdateNotification::GetTimeZoneNumericIdL() const
	{
	TInt id = KErrNotFound;
	
	CTzId* tzId = iTimeZoneServer.GetTimeZoneIdL();
	CleanupStack::PushL(tzId);
	id = tzId->TimeZoneNumericID();
	CleanupStack::PopAndDestroy(tzId);
	
	return id;
	}
	
TBool CTzUpdateNotification::TestChangeL()
	{
	NTzUpdate::TTimeZoneChange change = iPropertyWatcher->GetChangeDetails();
	TBool success = ETrue;
	
	TInt tzId = GetTimeZoneNumericIdL();
	if (change.iNewTimeZoneId != tzId)
		{
		ERR_PRINTF1(_L("New time zone ID doesn't match time zone property"));
		success = EFalse;
		}
	else if (change.iOldTimeZoneId != iOldTimeZoneId)
		{
		ERR_PRINTF1(_L("Old time zone ID doesn't match time zone property"));
		success = EFalse;
		}
	return success;
	}

TInt CTzUpdateNotification::TimeOutCallBack(TAny* aPtr)
	{
	CTzUpdateNotification* self = static_cast<CTzUpdateNotification*>(aPtr);
	if (self)
		{
		self->DoTimeOutCallBack();
		}
	return KErrNone;
	}

void CTzUpdateNotification::DoTimeOutCallBack()
	{
	iTimeOutTimer->Cancel();
	iTimedOut = ETrue;
	CActiveScheduler::Stop();
	}

TInt CTzUpdateNotification::WaitForChangeL()
	{
	const TInt KTimeOutPeriod = 1000000;
	
	iTimedOut = EFalse;
	iTimeOutTimer->Start(KTimeOutPeriod, KTimeOutPeriod, TCallBack(TimeOutCallBack, this));

	CActiveScheduler::Start();
	if (iTimeOutTimer->IsActive())
		{
		iTimeOutTimer->Cancel();
		}

	if (iTimedOut)
		{
		if (iLocation1.Compare(iLocation2) != 0)
			{
			ERR_PRINTF1(_L("No callback from RProperty when time zone changed"));
			return KErrGeneral;
			}
		}
	else
		{
		// check the time zone property is the current time zone id
		if (!TestChangeL())
			{
			return KErrGeneral;
			}
		}
	return KErrNone;
	}

TInt CTzUpdateNotification::CreateAndRunTestUnitL()
	{
	TInt err = KErrNone;
	SetUpTestL();
		
	// start watching the time zone property
	iPropertyWatcher->Start();
	
	iOldTimeZoneId = GetTimeZoneNumericIdL();
	// set the time zone where we want to test
	SetHomeTimeZoneL(iLocation1);
	
	err = WaitForChangeL();
	if (err != KErrNone)
		{
		return err;
		}
	
	// check the second location
	
	// start watching the time zone property
	iPropertyWatcher->Start();
	
	iOldTimeZoneId = GetTimeZoneNumericIdL();
	// set the time zone where we want to test
	SetHomeTimeZoneL(iLocation2);
	
	err = WaitForChangeL();
	if (err != KErrNone)
		{
		return err;
		}
	
	return err;
	}

void CTzUpdateNotification::SetUpTestL()
	{
	TBuf<KMaxLocationLength> location1;
	TPtrC locationPtr1(location1);
	GetStringFromConfig(ConfigSection(),KLocationTag1,locationPtr1);
	iLocation1.Set(CnvUtfConverter::ConvertFromUnicodeToUtf8L(locationPtr1)->Des());
	
	TBuf<KMaxLocationLength> location2;
	TPtrC locationPtr2(location2);
	GetStringFromConfig(ConfigSection(),KLocationTag2,locationPtr2);
	iLocation2.Set(CnvUtfConverter::ConvertFromUnicodeToUtf8L(locationPtr2)->Des());
	
	if (iPropertyWatcher == NULL)
		{
		iPropertyWatcher = CTzPropertyWatcher::NewL();	
		}

	// connect to the time zone server
	User::LeaveIfError(iTimeZoneServer.Connect());	

	// enable the auto update functionality - it's enabled by default but to make it sure
	iTimeZoneServer.SetAutoUpdateBehaviorL(RTz::ETZAutoDSTUpdateOn);
	
	SetHomeTimeZoneL(iLocation2);

	iTimeOutTimer = CPeriodic::NewL(EPriorityNormal);

	User::After(1000000);
	}
	
void CTzUpdateNotification::SetHomeTimeZoneL(const TDesC8& aZoneName)
	{
	CTzId* tzid = CTzId::NewL(aZoneName);
	CleanupStack::PushL(tzid);
	iTimeZoneServer.SetTimeZoneL(*tzid);
	CleanupStack::PopAndDestroy(tzid);
	}
	
CTzUpdateNotification::CTzUpdateNotification(CDstUnitServer& aServer) : CDstUnitStep(aServer)
	{
	SetTestStepName(KTzUpdateNotification);
	}

CTzUpdateNotification::~CTzUpdateNotification()
	{
	DestroyTestUnit(); 
	}
	
void CTzUpdateNotification::DestroyTestUnit()
	{
	iTimeZoneServer.Close();
	if (iPropertyWatcher)
		{
		iPropertyWatcher->Cancel();
		}
	delete iPropertyWatcher;
	iPropertyWatcher = NULL;
	}

