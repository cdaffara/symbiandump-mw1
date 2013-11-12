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
// Contains implementation of CProgressCallBack class
// 
//

/**
 @file
 @internalTechnology 
*/

// User Include
#include "ProgressCallBack.h"

/**
Constructor. Initializes the request status object, and adds itself to 
the active scheduler
@param aRequestStatus The reference to the request status object, on whom 
the caller will wait for completion
@internalTechnology
@test
*/
CProgressCallBack::CProgressCallBack(TRequestStatus& aRequestStatus)
: CActive(EPriorityStandard), iRequestStatus(aRequestStatus)
	{
	iRequestStatus = KRequestPending;
	CActiveScheduler::Add(this);
	}

/**
From MCalProgressCallBack. Does nothing
@internalTechnology
@test
*/
void CProgressCallBack::Progress(TInt /*aPercentageCompleted*/)
	{
	}

/**
From MCalProgressCallBack. Sets this active object to be active and completes
the external request status object. which was passed while constructing us.
Also completes the iStatus of this active object.
@internalTechnology
@test
*/
void CProgressCallBack::Completed(TInt aError)
	{
	SetActive();
	
	// Complete the external request
	TRequestStatus* status = &iRequestStatus;
	User::RequestComplete(status, aError);
	
	// Complete this Active Object
	status = &iStatus;
	User::RequestComplete(status, KErrNone);
	}
	
/**
From MCalProgressCallBack. Does nothing
@internalTechnology
@test
*/
TBool CProgressCallBack::NotifyProgress()
	{
	return ETrue;
	}
	
/**
From CActive. Stops the current nested loop as we are done
@internalTechnology
@test
*/
void CProgressCallBack::RunL()	
	{
	CActiveScheduler::Stop();
	}

/**
From CActive. Completes the external request with a KErrCancel
Stops the current nested loop as we are done
@internalTechnology
@test
*/
void CProgressCallBack::DoCancel()
	{
	TRequestStatus* status = &iRequestStatus;
	User::RequestComplete(status, KErrCancel);
	CActiveScheduler::Stop();
	}

