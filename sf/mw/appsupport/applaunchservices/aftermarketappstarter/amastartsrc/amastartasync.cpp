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

/**
 @file
 @internalComponent
*/

#include "amastartasync.h"
#include "ssmdebug.h"

CAmaStartAsync* CAmaStartAsync::NewL()
	{
	CAmaStartAsync* self = NewLC();
	CleanupStack::Pop();	
	return self;
	}

CAmaStartAsync* CAmaStartAsync::NewLC()
	{
	CAmaStartAsync* self = new(ELeave) CAmaStartAsync();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

void CAmaStartAsync::ConstructL()
	{
	iSsmStartSafe = CSsmStartSafe::NewL();
	CActiveScheduler::Add(this);
	}

CAmaStartAsync::CAmaStartAsync()
: CActive( EPriorityStandard )
	{
	}

CAmaStartAsync::~CAmaStartAsync()
	{
	Cancel();
	delete iCurrentDscItem;
	delete iSsmStartSafe;
	iDscStore.Close();
	iSysMon.Close();
	}

/**
 * Initiate AMA processing.
 */	
void CAmaStartAsync::StartL(const TUid& aDscId, TRequestStatus& aStatus)
	{
	if (iRunning)
		{
		DEBUGPRINT1( _L("CAmaStartAsync is already running..."));
		User::Leave(KErrInUse);
		}

	iDscStore.OpenL();
	TBool result = iDscStore.DscExistsL(aDscId);
	if (!result)
		{
		TRequestStatus* status = &aStatus;
		User::RequestComplete(status, KErrArgument);
		DEBUGPRINT1A( "RAmaStartSession::StartDsc> (DSC does not exist)") ;
		return;
		}

	iAmaStatus = &aStatus;
	iRunning = ETrue;
	iDscStore.EnumOpenLC(aDscId);
	CleanupStack::Pop();

	DoStartL();
	}

void CAmaStartAsync::DoStartL()
	{
	iCurrentDscItem = iDscStore.EnumReadNextL();
	if (iCurrentDscItem)
		{
		//if loading of one AMA from the DSC fails we should move to next AMA 
		StartDscItem(*iCurrentDscItem);
		}
	else // No more items in store, so finish here
		{
		iDscStore.EnumClose();
		iDscStore.Close();
		User::RequestComplete(iAmaStatus, KErrNone);
		iAmaStatus = NULL;
		delete iCurrentDscItem;
		iCurrentDscItem = NULL;
		iRunning = EFalse;
		}
	}

TInt CAmaStartAsync::RunError( TInt aError )
	{
	iDscStore.EnumClose();
	iDscStore.Close();
	User::RequestComplete(iAmaStatus, aError);
	iRunning = EFalse;
	return KErrNone;
	}

/**
 * Cancel the current command.
 */
void CAmaStartAsync::DoCancel()
	{
	if (iRunning)
		{
		iSsmStartSafe->StartCancel(iSSCancelIndex);
		iDscStore.EnumClose();
		User::RequestComplete(iAmaStatus, KErrCancel);
		iRunning = EFalse;
		}	
	}

void CAmaStartAsync::StartDscItem(const CDscItem& aDscItem)
	{
	// start the process
	const CSsmStartupProperties& properties = aDscItem.SsmStartupProperties();
	iSsmStartSafe->Start(properties, iCurrentProcess, iStatus, iSSCancelIndex);
	SetActive();
	}

void CAmaStartAsync::MonitorProcessL(const CStartupProperties& aProperties, const RProcess& aProcess)
	{
	// first time monitoring, so connect with the SysMon server
	if (KNullHandle == iSysMon.Handle())
		{
		iSysMon.OpenL();
		}

	// monitor the process	
	iSysMon.MonitorL(aProperties, aProcess);
	}

/**
 * Succsessive calls to StartDscItem, but only one per RunL in order for
 * the scheduler/ server to remain responsive.
 */
void CAmaStartAsync::RunL()
	{
	//monitor the process if indicated
	if (iCurrentDscItem->Monitored() && KErrNone == iStatus.Int())
		{
		const CStartupProperties& properties = iCurrentDscItem->StartupProperties();
		//if the start method is Fire and Forget don't take any action, 
		//monitor will be started by fire and forget itself if required.
		if(properties.StartMethod() != EFireAndForget)
			{
			TRAPD(error, MonitorProcessL(properties, iCurrentProcess));

			//Kill the started process in case of an error as Start and Monitor should be one atomic function.
			if (KErrNone != error)
				{
				iCurrentProcess.Kill(error);
				}
			}
		}

	iCurrentProcess.Close();
	delete iCurrentDscItem;
	iCurrentDscItem = NULL;
	DoStartL();
	}
