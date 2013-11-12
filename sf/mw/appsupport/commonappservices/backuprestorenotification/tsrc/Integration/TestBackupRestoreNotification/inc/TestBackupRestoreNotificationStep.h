/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __TEST_BACKUP_RESTORE_NOTIFICATION_STEP_H__
#define __TEST_BACKUP_RESTORE_NOTIFICATION_STEP_H__

#include <testexecutestepbase.h>
#include <connect/sbeclient.h>
#include "cbackuprestorenotification.h"

_LIT(KTestBackupRestoreNotificationStep,    "TestBackupRestoreNotificationStep");
_LIT(KContactsIniFileNameAndPath, 		    "C:\\private\\10003A73\\CntModel.ini");
_LIT(KInfoBackupStarted, 		   			"Backup Started");
_LIT(KInfoBackupCompleted, 		   			"Backup Ended");
_LIT(KInfoRestoreStarted, 		   			"Restore Started");
_LIT(KInfoRestoreCompletedWithResult, 		"Restore Ended");
_LIT(KInfoFileBeingBackedup, 		 		"The File being backed up is %S \n");
_LIT(KBackupRestoreFilePath,				"C:\\BackupRestoreNotification\\CntModel.ini");


class CTestBackupRestoreNotificationStep : public CTestStep, public MBackupRestoreNotificationObserver
	{
public:
	CTestBackupRestoreNotificationStep();
	~CTestBackupRestoreNotificationStep();

	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

    /* MBackupRestoreNotificationObserver Callback Functions */
	void BackupBeginningL(void);
	void BackupCompletedL(void);
	void RestoreBeginningL();
	void RestoreCompletedL(TInt aRestoreResult);

	/* General Utility Functions */
	void StartBackupL(void);
	void EndBackupL(void);
	void StartRestoreL(void);
	void EndRestoreL(void);
	void Validate(const TBool& aValid);

private:
	TDriveList iDriveList;
	conn::CSBEClient* iBackupClient;
	CBackupRestoreNotification*	iBackupNotification;
	CActiveScheduler* iScheduler;
	RFs iFs;
	};

#endif
