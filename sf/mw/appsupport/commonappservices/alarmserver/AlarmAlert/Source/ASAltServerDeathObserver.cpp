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

#include "ASAltServerDeathObserver.h"

// User includes
#include "ASAltClientSession.h"


//
// ----> CASAltServerDeathObserver (source)
//

//*************************************************************************************
CASAltServerDeathObserver::CASAltServerDeathObserver(RASAltClientSession& aSession)
:	CActive(CActive::EPriorityHigh), iSession(aSession)
	{
	CActiveScheduler::Add(this);
	}


//*************************************************************************************
CASAltServerDeathObserver::~CASAltServerDeathObserver()
	{
	Cancel();
	}


//*************************************************************************************
void CASAltServerDeathObserver::ConstructL()
	{
	// Kill the Alarm Server as soon as the Alarm Alert Server dies
	iSession.NotifyAlertServerDeath(iStatus);
	SetActive();
	}


//*************************************************************************************
CASAltServerDeathObserver* CASAltServerDeathObserver::NewL(RASAltClientSession& aSession)
	{
	CASAltServerDeathObserver* self = new(ELeave) CASAltServerDeathObserver(aSession);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}


//
//
//


//*************************************************************************************
void CASAltServerDeathObserver::RunL()
	{
	// Undertaker - the under taker stops the active scheduler (shuts down the alarm
	// server) if the UIKON server which handles alarm windows fails.
	if	(iStatus != KErrCancel)
		CActiveScheduler::Stop();
	}


//*************************************************************************************
void CASAltServerDeathObserver::DoCancel()
	{
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrCancel);
	}


//
//
//

