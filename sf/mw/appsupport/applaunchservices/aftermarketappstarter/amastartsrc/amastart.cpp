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

#include "ssmdebug.h"
#include "amastart.h"
#include <dscitem.h>
#include <startsafe.h>

/**
 * Used to create an instance of CAmaStart class
 * @return An instance of CAmaStart
 */
EXPORT_C CAmaStart* CAmaStart::NewL()
	{
	CAmaStart* ama = new(ELeave) CAmaStart();
	CleanupStack::PushL(ama);
	ama->ConstructL();
	CleanupStack::Pop(ama);
	return ama;
	}

void CAmaStart::ConstructL()
	{
	iAmaStarter = CAmaStartAsync::NewL();
	iStartSafe = CStartSafe::NewL();
	}

CAmaStart::CAmaStart()
	{
	}

/**
 * Destructor for CAmaStart class
 */
EXPORT_C CAmaStart::~CAmaStart()
	{
	delete iStartSafe;
	iDscStore.Close();
	iSysMon.Close();
	delete iAmaStarter;
	}

/**
 * Used to launch AMAs (after market application) from a DSC. 
 * Returns when processing of the DSC is complete. 
 * (Synchronous)
 * @param aDscId Id of the DSC containing AMAs to be started.
 */
EXPORT_C void CAmaStart::StartL(const TUid aDscId)
	{
	if (iClientStatus && *iClientStatus == KRequestPending)
		{
		User::Leave(KErrNotReady);
		}

	// Opens a connection with the DSC database.
	iDscStore.OpenL();

	//Prepares the database to read the given details of the given dsc id
	iDscStore.EnumOpenLC(aDscId) ;

	//Read the details of the current dscid (aDscId) 
	CDscItem* item = iDscStore.EnumReadNextL();
	while (item)
		{
		//if loading of one AMA from the DSC fails we should move to next AMA - so we trap here 
		TRAP_IGNORE(StartDscItemL(*item));
		delete item;
		item = iDscStore.EnumReadNextL();
		}

	//EnumClose()
	//Pop and destroy cleanup rollback operation left on cleanstack by EnumOpenLC()
	CleanupStack::PopAndDestroy();
	}

/**
 * Used to begin launching AMAs (after market application) from a DSC.
 * Returns immediately. The supplied TRequestStatus object is used to signal completion. 
 * When the request is completed it will have one of the following values: 
 * KErrNone - if the request complete successfully
 * KErrCancel - if the request was cancelled using CAmaStart::CancelStart
 * KErrNotReady - if another asynchronous request is outstanding
 * KErrArgument - if a DSC with the supplied UID does not exist
 * or one of the other system-wide error codes.
 * (Asynchronous)
 * @param aDscId Id of the DSC containing AMAs to be started.
 * @param aRequestStatus Status object to signal completion.
 */
EXPORT_C void CAmaStart::Start(const TUid aDscId, TRequestStatus& aRequestStatus)
	{
	aRequestStatus = KRequestPending;
	TRequestStatus* trs = &aRequestStatus;
	if (iClientStatus && *iClientStatus == KRequestPending)
		{
		User::RequestComplete(trs, KErrNotReady);
		return;
		}
	
	iClientStatus = &aRequestStatus;
	TRAPD(err, iAmaStarter->StartL(aDscId, aRequestStatus));
	if (err != KErrNone)
		{
		//Complete the request with the error code returned
		User::RequestComplete(trs, err);
		return;
		}
	}

/**
 * Cancel's a pending asynchronous StarL request.
 * If there is no request pending, calling thisw method has no effect.
 */
EXPORT_C void CAmaStart::CancelStart()
	{
	iAmaStarter->Cancel();
	}

void CAmaStart::StartDscItemL(const CDscItem& aDscItem)
	{
	//A process to be created inside iStartSafe->StartL();
	RProcess process;
	CleanupClosePushL(process);

	//Number of retries made for starting the process.
	TInt tried = 0;

	// start the process
	const CStartupProperties& properties = aDscItem.StartupProperties();
	iStartSafe->StartL(properties, process, tried);
	DEBUGPRINT2A("CAmaStart::StartDscItemL(): %d times has been retried to start the process successfully", tried);

	//monitor the process if indicated
	if (aDscItem.Monitored())
		{
		TRAPD(error, MonitorProcessL(properties, process));

		//Kill the started process in case of an error as Start and Monitor should be one atomic function.
		if (KErrNone != error)
			{
			process.Kill(error);
			}
		}
	CleanupStack::PopAndDestroy(&process);
 	}

void CAmaStart::MonitorProcessL(const CStartupProperties& aProperties, const RProcess& aProcess)
	{
	// first time monitoring, so connect with the SysMon server
	if (KNullHandle == iSysMon.Handle())
		{
		iSysMon.OpenL();
		}

	// monitor the process	
	iSysMon.MonitorL(aProperties, aProcess);
	}
