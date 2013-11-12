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




#include "UtcOffsetChangeEventNotifier.h"
#include <tzupdate.h>

CUtcOffsetChangeEventNotifier* CUtcOffsetChangeEventNotifier::NewL()
	{
	CUtcOffsetChangeEventNotifier* self = new(ELeave)CUtcOffsetChangeEventNotifier;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}


void CUtcOffsetChangeEventNotifier::ConstructL()
	{
	iActiveSchedulerWait = new(ELeave) CActiveSchedulerWait;
	CActiveScheduler::Add(this);
	User::LeaveIfError(iProperty.Attach(NTzUpdate::KPropertyCategory,NTzUpdate::EUtcOffsetChangeNotification));
	iProperty.Subscribe(iStatus);
	SetActive();
	}
	
CUtcOffsetChangeEventNotifier::CUtcOffsetChangeEventNotifier():CActive(EPriorityStandard)
	{}

CUtcOffsetChangeEventNotifier::~CUtcOffsetChangeEventNotifier()
	{
	iProperty.Close();
	Cancel();
	delete iActiveSchedulerWait;
	}
	
	
void CUtcOffsetChangeEventNotifier::WaitForNotification()
	{
	iActiveSchedulerWait->Start();
	}
	
	
void CUtcOffsetChangeEventNotifier::RunL()
	{
	iActiveSchedulerWait->AsyncStop();
	}
	
void CUtcOffsetChangeEventNotifier::DoCancel()	
	{
	iProperty.Cancel();
	}
