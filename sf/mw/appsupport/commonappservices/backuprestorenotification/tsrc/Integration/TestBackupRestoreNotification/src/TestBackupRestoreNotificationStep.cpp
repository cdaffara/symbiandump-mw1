// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "TestBackupRestoreNotificationStep.h"


CTestBackupRestoreNotificationStep::~CTestBackupRestoreNotificationStep()
/**
 * Destructor
 */
	{
	delete iBackupClient;
	delete iBackupNotification;
	iFs.Close();	
	}

CTestBackupRestoreNotificationStep::CTestBackupRestoreNotificationStep()
/**
 * Constructor
 */
	{
	/* **MUST** call SetTestStepName in the constructor as the controlling
	   framework uses the test step name immediately following construction to set
	   up the step's unique logging ID. */
	SetTestStepName(KTestBackupRestoreNotificationStep);
	}

TVerdict CTestBackupRestoreNotificationStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iScheduler);
	iFs.Connect();
	iDriveList.FillZ();
	User::LeaveIfError(iFs.DriveList(iDriveList));
	iBackupClient = conn::CSBEClient::NewL();
	iBackupNotification = CBackupRestoreNotification::NewL(KContactsIniFileNameAndPath, *this);	
	SetTestStepResult(EPass);
	return TestStepResult();
	}

TVerdict CTestBackupRestoreNotificationStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 */
	{	
	/* Initiate Backup and Validate the same*/
	StartBackupL();
	TBool backupState = iBackupNotification->BackupInProgress();
	Validate(backupState);

	/* End Backup and validate the same */
	EndBackupL();
	backupState = iBackupNotification->BackupInProgress();
	Validate(backupState);


	/* Initiate Restore and Validate the same*/
	StartRestoreL();
	TBool restoreState = iBackupNotification->RestoreInProgress();
	Validate(restoreState);

	/* End Restore and validate the same */
	EndRestoreL();
	restoreState = iBackupNotification->RestoreInProgress();
	Validate(restoreState);

	return TestStepResult();
	}

TVerdict CTestBackupRestoreNotificationStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	CActiveScheduler::Install(NULL);
	delete iScheduler;	
	iScheduler = NULL;
	return TestStepResult();
	}

/* MBackupRestoreNotificationObserver Callback functions */

void CTestBackupRestoreNotificationStep::BackupBeginningL()
	{
	INFO_PRINTF1(KInfoBackupStarted);
	}

void CTestBackupRestoreNotificationStep::BackupCompletedL()
	{
	INFO_PRINTF1(KInfoBackupCompleted);
	}

void CTestBackupRestoreNotificationStep::RestoreBeginningL()
	{
	INFO_PRINTF1(KInfoRestoreStarted);
	}

void CTestBackupRestoreNotificationStep::RestoreCompletedL(TInt aRestoreResult)
	{
	INFO_PRINTF2(KInfoRestoreCompletedWithResult, aRestoreResult);
	}


void CTestBackupRestoreNotificationStep::StartBackupL()
	{
	/* Use the Secure Backup Engine API to "start" a backup. 
	 This changes the P&S property conn::KUidBackupRestoreKey.
	 */
	iBackupClient->SetBURModeL(iDriveList, conn::EBURBackupFull, conn::EBackupBase);
	}


void CTestBackupRestoreNotificationStep::EndBackupL()
	{
	/* Use the Secure Backup Engine API to "end" a backup. 
	 This changes the P&S property conn::KUidBackupRestoreKey.
	 */
	iBackupClient->SetBURModeL(iDriveList, conn::EBURNormal, conn::ENoBackup);
	}


void CTestBackupRestoreNotificationStep::StartRestoreL()
	{
	/* Use the Secure Backup Engine API to "start" a restore. 
	 This changes the P&S property conn::KUidBackupRestoreKey.
	 */
	iBackupClient->SetBURModeL(iDriveList, conn::EBURRestoreFull, conn::EBackupBase);
	}


void CTestBackupRestoreNotificationStep::EndRestoreL()
	{
	/* Use the Secure Backup Engine API to "end" a restore. 
	 This changes the P&S property conn::KUidBackupRestoreKey.
	 */
	iBackupClient->SetBURModeL(iDriveList, conn::EBURNormal, conn::ENoBackup);
	}

void CTestBackupRestoreNotificationStep::Validate(const TBool& aValid)
	{
	if(aValid)
		{
		_LIT(KEvaluationFailureMessage, "Test Evaluation failed");
		ERR_PRINTF1(KEvaluationFailureMessage);
		SetTestStepResult(EFail);
		}
	}


	
