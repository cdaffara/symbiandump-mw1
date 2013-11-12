// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ASAltRequestQuietPeriodEnd.h"
#include "ASAltClientSession.h"
#include "ASSrvStaticUtils.h"
#include "ASAltAlarmAlertObserver.h"


CASAltRequestQuietPeriodEnd::CASAltRequestQuietPeriodEnd(RASAltClientSession& aSession,MASAltAlarmAlertObserver& aObserver)
:	CActive(CActive::EPriorityHigh), iSession(aSession),iObserver(aObserver)
	{
	CActiveScheduler::Add(this);
	}

CASAltRequestQuietPeriodEnd::~CASAltRequestQuietPeriodEnd()
	{
	Cancel();
	}

void CASAltRequestQuietPeriodEnd::RequestQuietPeriodEnd()
	{
	Cancel();
	Session().TimeWhenQuietPeriodShouldEnd(iStatus,iTimeBuf);
	SetActive();
	}

void CASAltRequestQuietPeriodEnd::ConstructL()
	{
	}

CASAltRequestQuietPeriodEnd* CASAltRequestQuietPeriodEnd::NewL(RASAltClientSession& aSession,MASAltAlarmAlertObserver& aObserver)
	{
	CASAltRequestQuietPeriodEnd* self = new(ELeave) CASAltRequestQuietPeriodEnd(aSession,aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CASAltRequestQuietPeriodEnd::RunL()
	{
	TInt error=iStatus.Int();
	__ASSERT_ALWAYS(error == KErrNone, ASSrvStaticUtils::Fault(ASSrvStaticUtils::EASSrvFaultAlarmAlertServerFault));
	iObserver.ASAltQuietPeriodUntil(iTimeBuf());
	}

void CASAltRequestQuietPeriodEnd::DoCancel()
	{
	//TRequestStatus* status = &iStatus;
	//User::RequestComplete(status, KErrCancel);
	}



