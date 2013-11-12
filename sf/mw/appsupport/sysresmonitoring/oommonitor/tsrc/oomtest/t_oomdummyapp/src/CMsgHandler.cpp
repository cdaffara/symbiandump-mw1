/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*
*/




#include "CMsgHandler.h"

CMsgHandler::CMsgHandler(Ct_oomdummyappAppUi& aOwner) :
	CActive(EPriorityStandard), // Standard priority
	iOwner(aOwner)
	{
	}

CMsgHandler* CMsgHandler::NewLC(Ct_oomdummyappAppUi& aOwner)
	{
	CMsgHandler* self = new (ELeave) CMsgHandler(aOwner);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CMsgHandler* CMsgHandler::NewL(Ct_oomdummyappAppUi& aOwner)
	{
	CMsgHandler* self = CMsgHandler::NewLC(aOwner);
	CleanupStack::Pop(); // self;
	return self;
	}

void CMsgHandler::ConstructL()
	{
	User::LeaveIfError(iMsgQueue.Open(15)); // Initialize message queue from process parameters
	CActiveScheduler::Add(this); // Add to scheduler
	StartL();
	}

CMsgHandler::~CMsgHandler()
	{
	Cancel(); // Cancel any request, if outstanding
	iMsgQueue.Close(); // Destroy the RMsgQueue object
	// Delete instance variables if any
	}

void CMsgHandler::DoCancel()
	{
	iMsgQueue.CancelDataAvailable();
	}

void CMsgHandler::StartL()
	{
	Cancel(); // Cancel any request, just to be sure
	iMsgQueue.NotifyDataAvailable(iStatus);
	SetActive(); // Tell scheduler a request is active
	}

void CMsgHandler::RunL()
	{
	TInt command;
	while(KErrNone==iMsgQueue.Receive(command))
		{
		iOwner.HandleHarnessCommandL(command);
		}
	StartL();
	}

TInt CMsgHandler::RunError(TInt aError)
	{
	return aError;
	}
