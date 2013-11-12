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

#include "CBackupRestoreNotification.h"


/** Default constructor. 
 *	@param aCallback reference to observer class object.
 */
CBackupRestoreNotification::CBackupRestoreNotification(MBackupRestoreNotificationObserver& aCallback)
:	
	CActive(CActive::EPriorityStandard), 
	iObserver(aCallback)
	{
	
	CActiveScheduler::Add(this);
	}


/** Destructor.
 */
EXPORT_C CBackupRestoreNotification::~CBackupRestoreNotification()
	{
	Cancel();
	iBackupNotification.Close();
	}

/** 
 *	A class wishing to be notified of a Backup/Restore shoud be derived from 
 *	MBackupRestoreNotificationObserver and pass a self reference.
 *	@param	aFilename	full filename of the file which needs monitoring.
 *	@param aCallback should usually be a pointer to the class calling this function.
 */
EXPORT_C CBackupRestoreNotification* CBackupRestoreNotification::NewL(const TDesC& aFilename, 
												MBackupRestoreNotificationObserver& aCallback)
	{
	CBackupRestoreNotification* self = new(ELeave) CBackupRestoreNotification(aCallback);
	CleanupStack::PushL(self);
	self->ConstructL(aFilename);
	CleanupStack::Pop(self);
	return self;
	}

/** 
 *	Indicates if a Backup operation is currently taking place.
 *	@return	ETrue if operation in progress else EFalse.
 */
EXPORT_C TBool CBackupRestoreNotification::BackupInProgress() const
	{
	return (iCurrentState & (conn::EBURBackupPartial | conn::EBURBackupFull));
	}

/** 
 *	Indicates if a Restore operation is currently taking place.
 *	@return	ETrue if operation in progress else EFalse.
 */
EXPORT_C TBool CBackupRestoreNotification::RestoreInProgress() const
	{
	return (iCurrentState & (conn::EBURRestorePartial | conn::EBURRestoreFull));
	}



/** 
 *	Initial registration with Publish&Subscribe for KUidBackupRestoreKey.
 *	@param full filename of the file being backed and which needs monitoring. 
 */
void CBackupRestoreNotification::ConstructL(const TDesC& /*aFilename*/)
	{
	// Initial subscribe to the Backup and Restore Flag.
	// The first ID indicates the type of flag being subscribed to.
	// The second ID is the actual Backup/Restore flag.
	iBackupNotification.Attach(KUidSystemCategory, conn::KUidBackupRestoreKey);
	iBackupNotification.Subscribe(iStatus);
	
	// Check to see if a Backup/Restore is currently underway.
	TInt newState=0;
	if (iBackupNotification.Get(newState)!=KErrNotFound)
		{
		ProcessInitialStateL(newState);
		}

	SetActive();
	}


/** 
 *	Have been notifed of a change in the backup/restore falg value.
 *	Re-register for changes to the backup flag. Then evaluate which observer 
 *	function need to be called.
 *	@see CActive
 */
void CBackupRestoreNotification::RunL()
	{
	// Resubscribe before dealing with the current notification.
	iBackupNotification.Subscribe(iStatus);
	SetActive();

	// Flag updated. What to do..
	TInt newState=0;
	if (iBackupNotification.Get(newState)!=KErrNotFound)
		{
		ReceivedNotificationL(newState);
		}
	}


TInt CBackupRestoreNotification::RunError(TInt /* aError */)
	{ // recover after handler for notification left
	return (KErrNone);
	}


/**
 * @see CActive
 */
void CBackupRestoreNotification::DoCancel()
	{
	iBackupNotification.Cancel();
	}


/**
 * Determines what operation is starting or has completed
 * and calls the relevant observer function.
 * @param aNewState	flags indicating what the new backup/restore state is:
 *	ENoBackup or EBackupBase or EBackupIncremetal. Only interesed in BaseBackup.
 *  EBUR (Unset, Normal, BackupFull, BackupPartial, RestoreFull, RestorePartial)
 *
 *	1. No backup/restore is taking place = EBURNormal | ENoBackup		
 *	2. Backup operation starting = (EBURBackupFull || EBURBackupPartial) && EBackupBase
 *  3. Restore operation starting = (EBURRestoreFull || EBURRestorePartial) && EBackupBase
 *								
 */
void CBackupRestoreNotification::ReceivedNotificationL(TInt aNewState)
	{
	// We are starting a a new operation...
	// Was there a previous Restore which failed to complete ?
	if (!(aNewState & (conn::EBURNormal | conn::EBURUnset)) && RestoreInProgress())
		{
		iCurrentState=aNewState;
		iObserver.RestoreCompletedL(KErrAbort);		
		}

	// A Backup or Restore taking place.
	if (aNewState & (conn::EBURBackupPartial | conn::EBURBackupFull) )
		{
		// Backup is taking place (either partial or full)
		iCurrentState=aNewState;
		iObserver.BackupBeginningL();
		}
	else if (aNewState & (conn::EBURRestorePartial | conn::EBURRestoreFull)) 
		{
		// Restore is taking place (either partial or full)
		iCurrentState=aNewState;
		iObserver.RestoreBeginningL();
		}
	else if (aNewState & (conn::EBURNormal | conn::EBURUnset))
		{		
		// The state has changed to no backup/restore
		// Decide which operation has just completed.
		if (BackupInProgress())
			{
			iCurrentState=aNewState;
			iObserver.BackupCompletedL();
			}
		else if (RestoreInProgress())
			{
			iCurrentState=aNewState;
			iObserver.RestoreCompletedL(KErrNone);
			}		
		}
	}

void CBackupRestoreNotification::ProcessInitialStateL(TInt aNewState)
	{
	// A Backup or Restore is taking place.
	if (aNewState & (conn::EBURBackupPartial | conn::EBURBackupFull) )
		{
		// Backup is taking place (either partial or full) 
		iObserver.BackupBeginningL();
		}
	else if (aNewState & (conn::EBURRestorePartial | conn::EBURRestoreFull)) 
		{
		// Restore is taking place (either partial or full)
		iObserver.RestoreBeginningL();
		}	
	iCurrentState=aNewState;
	}

