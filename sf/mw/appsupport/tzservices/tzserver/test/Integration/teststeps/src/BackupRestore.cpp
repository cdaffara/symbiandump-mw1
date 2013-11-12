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

#include "BackupRestore.h"
#include "DstStep.h"
#include "DstIntUtils.h"
#include <tz.h>

const TInt KVancouverTZId = 1320;

CBackupRestoreTestStep::CBackupRestoreTestStep()
 	{
 	SetTestStepName( KBackupRestoreTestStepName );
 	}
 	
CBackupRestoreTestStep::~CBackupRestoreTestStep()
	{
	}
	
TVerdict CBackupRestoreTestStep::doTestStepPreambleL()
    {
    return TestStepResult();
	}	

TVerdict CBackupRestoreTestStep::doTestStepL()
	{
    _LIT( KSetTimeZoneCmd,   "SetTimeZone" );
    _LIT( KCheckTimeZoneCmd, "CheckTimeZone" );

    // An ini file section name is specified in the test script to tell this
    // test step what to do. Check which command was specified.
    TDes& sectionName = ConfigSection();
    if (!sectionName.Compare( KSetTimeZoneCmd ))
        {
        SetTimeZoneL();
        }
    else if (!sectionName.Compare( KCheckTimeZoneCmd ))
        {
        CheckTimeZoneL();
        }
    else
        {
        User::Leave( KErrBadName );
        }
    
	return TestStepResult();
	}

/**
* Sets time zone to something other than default (Vancouver). 
*/
void CBackupRestoreTestStep::SetTimeZoneL()
    {
    RTz tz;
   	TInt err = tz.Connect();
   	User::LeaveIfError( err );
	
	CTzId* tzId = CTzId::NewL( KVancouverTZId );
	CleanupStack::PushL( tzId );
	tz.SetTimeZoneL( *tzId );
	tzId->TimeZoneNumericID();	
	CleanupStack::PopAndDestroy( tzId );
	
	tz.Close();
    }

/*
* Checks that the TZ is Vancouver. This is called after the restore has been
* completed to ensure that the .cre file for the TZ server was restored 
* properly.
*/	
void CBackupRestoreTestStep::CheckTimeZoneL()
    {
    // After the restore has completed, the cenrep server should have reloaded
    // the new .cre file and updated the TZ server keys. We wait here to ensure 
    // this has been completed before reading the latest TZ again. If the 
    // cenrep server didn't update the key then when the TZ is read below, the 
    // value would be coming from memory and not from the restored value from
    // the restored .cre file.
    User::After( 3000000 );

    // Ensure .cre file exists.
    INFO_PRINTF1( _L("Checking if cenrep .cre file has been restored." ));
    CheckIfCenrepFileExistsL();

    // Check current TZ.
    RTz tz;
    TInt err = tz.Connect();
   	User::LeaveIfError( err );
    CTzId* tzid = tz.GetTimeZoneIdL();
    TInt currentId = tzid->TimeZoneNumericID();
    delete tzid;
    tz.Close();

    if (currentId != KVancouverTZId)
        {
        INFO_PRINTF1( _L("Test failed: Current TZ should be Vancouver." ) );
        SetTestStepResult( EFail );
        }
    }

void CBackupRestoreTestStep::CheckIfCenrepFileExistsL()
    {
    _LIT( KCenrepFile, "c:\\Private\\10202be9\\persists\\1020383e.cre" );
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    
    RFile file;
    TInt err = file.Open( fs, KCenrepFile, EFileRead );
    file.Close();
    fs.Close();
    User::LeaveIfError( err );
    }

TVerdict CBackupRestoreTestStep::doTestStepPostambleL()
	{
	return TestStepResult();
	}
