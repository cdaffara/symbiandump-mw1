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

#ifndef __MBACKUPRESTORENOTIFICATIONOBSERVER_H__
#define __MBACKUPRESTORENOTIFICATIONOBSERVER_H__


class MBackupRestoreNotificationObserver
/**	
	The MBackupRestoreNotificationObserver
	This interface is an observer class for CBackupRestoreNotification.
	Use this observer to be notified of a backup/restore operation.

	@internalComponent
	@released
*/
	{
public:
/**	
	The Backup Server has notified that a Backup is about to begin.
	The Application should complete all existing operations on files which are to be backed up.
	It should block write access attempt to the file until BackupCompletedL is called.
*/
	virtual void BackupBeginningL()	= 0;

/**	
	The Backup Server has notified that a Backup operation has completed.
	Write access to application files should be allowed again.
	@pre BackupBeginningL had previously been called.
*/
	virtual void BackupCompletedL()	= 0;

/**	
	The Backup Server has notified that a Restore operation has completed.
	Files being backed up should be closed. Files should not be accessed until 
	RestoreCompletedL is called. 
*/
	virtual void RestoreBeginningL() = 0;

/**	
	The Backup Server has notified that a Restore operation has completed.
	File can now be accessed. After a successful Restore (i.e.the Restore operation 
	completed) file should be reopened.
	@pre	RestoreBeginningL had previously been called.
	@param	aRestoreResult Indicates if the Restore operation was successful.
			KErrAbort - Restore failed to complete for some reason.
			KErrNone - The Restore operation completed successfully.
*/
	virtual void RestoreCompletedL(TInt aRestoreResult) = 0;
	};


#endif // __MBACKUPRESTORENOTIFICATIONOBSERVER_H__

