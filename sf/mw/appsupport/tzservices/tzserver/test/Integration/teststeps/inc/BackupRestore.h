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



#ifndef __BACKUP_RESTORE_H_
#define __BACKUP_RESTORE_H_

#include <tz.h>
#include <test/testexecutestepbase.h>
#include <e32property.h>

// String for the server create test step code
_LIT( KBackupRestoreTestStepName, "BackupRestore" );

/**
* Sets and checks the the time zone as part of the backup and restore test
* of the TZ server's central repository .cre file. The TZ is set prior to
* a backup and checked after the restore to ensure the .cre file was
* actually restored properly.
*/
class CBackupRestoreTestStep : public CTestStep
	{
public:
	CBackupRestoreTestStep();
	~CBackupRestoreTestStep();
		
private:
	// From CTestStep.
	TVerdict doTestStepL();
	TVerdict doTestStepPreambleL();
	TVerdict doTestStepPostambleL();

private:
    void SetTimeZoneL();
    void CheckTimeZoneL();
    void CheckIfCenrepFileExistsL();
	};

#endif
