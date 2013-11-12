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

#ifndef __CBACKUPRESTORENOTIFICATION_H__
#define __CBACKUPRESTORENOTIFICATION_H__

// System includes
#include <e32base.h>
#include <s32file.h>

#include <e32property.h>
#include <connect/sbdefs.h>

// User includes
#include "MBackupRestoreNotificationObserver.h"

/**	
	The CBackupRestoreNotification
	A "wrapper" class around the Backup & Restore notification mechanisms
	Configured to use Publish & Subscribe Flag

	@internalTechnology
	@released
*/

class CBackupRestoreNotification : 
	public CActive
	{

public:
	IMPORT_C static CBackupRestoreNotification* NewL(const TDesC& aFilename, 
											MBackupRestoreNotificationObserver& aCallback);
	IMPORT_C ~CBackupRestoreNotification();
	IMPORT_C TBool BackupInProgress() const;
	IMPORT_C TBool RestoreInProgress() const;

private:
	CBackupRestoreNotification(MBackupRestoreNotificationObserver& aCallback);
	void ConstructL(const TDesC& aFilename);

	void ReceivedNotificationL(TInt aNewState);
	void ProcessInitialStateL(TInt aNewState);
	void DoCancel();
	void RunL();
	TInt RunError(TInt aError);

private:

	/**
	 * Publish & Subscribe object used to tell when a Backup/Restore event has occured.
	 */
	RProperty	iBackupNotification;

	/**
	 * Current state of Backup Restore. This stores the flags value when they change.
	 * and can be checked the next time they change.
	 */
	TInt iCurrentState;

	/**
	 * Means by which an object can be notified of an operation.
	 */
	MBackupRestoreNotificationObserver&	iObserver;
		
	};

#endif // __CBACKUPRESTORENOTIFICATION_H__
