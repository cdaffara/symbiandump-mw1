// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
// To avoid problems with TEF tests looking for test data on
// the C drive after generating a ROM with IBY files, this utility
// test will copy all test data from the Z drive to the C drive
// then pass.
// Anything not specified in the blacklist will be treated as test data
// and the program will attempt to copy it to drive C. Failures to copy
// data are logged from trouble-shooting purposes, but it is considered
// non-fatal.
//



#include <e32std.h>
#include <f32file.h>
#include <e32test.h>
#include <bautils.h>

#include "preparedrivec.h"

// String constants for black-listing.
// - Sub-directories are exluded.
// - The trailing backslash is important for directories.
// - Don't forget to update KNonTestDataBlacklist and KNumberOfExclusions when adding more.
_LIT(KPrivateDir, "z:\\private\\");
_LIT(KResourceDir, "z:\\resource\\");
_LIT(KErrResDir, "z:\\error_resource_dir\\");
_LIT(KRefSsyDir, "z:\\refssy\\");
_LIT(KResFileDir, "z:\\resource_files_dir\\");
_LIT(KSysDir, "z:\\sys\\");
_LIT(KSystemDir, "z:\\system\\");
_LIT(KRefTspIni, "z:\reftsp.ini");
_LIT(KRefNvrCompIni, "z:\\reftsp_nevercomplete.ini");

// Number of string constants.
const TInt KNumberOfExclusions(9);

// Array for blacklisted items.
const TDesC* KNonTestDataBlacklist[KNumberOfExclusions] =
	{
	&KPrivateDir,
	&KResourceDir,
	&KErrResDir,
	&KRefSsyDir,
	&KResFileDir,
	&KSysDir,
	&KSystemDir,
	&KRefTspIni,
	&KRefNvrCompIni
	};

// Global resources
_LIT(KName, "preparedrivec.exe");

RTest test(KName);
RFs fs;

EXPORT_C TInt E32Main()
	{
	CTrapCleanup* cleanupStack = CTrapCleanup::New();
	if (! cleanupStack)
		{
		return KErrNoMemory;
		}
	
	// Start the test
	_LIT(KTestTitle, "Prepare Drive C - Copying all test data to C:\n");
	test.Start(KTestTitle);

	// Do preparation
	TRAPD(err, DoDrivePreparationL());
	test(err == KErrNone);
	
	// End test
	test.End();
	test.Close();
	
	return KErrNone;
	}

void DoDrivePreparationL()
	{
	// Open the file server connection
	CleanupClosePushL(fs);
	User::LeaveIfError(fs.Connect());
	
	_LIT(KTestStepTitle, "Copying content of drive C to drive Z\n");
	test.Printf(KTestStepTitle);
	
	ScanAndCopyL();
	
	CleanupStack::PopAndDestroy(&fs);
	}

/*
 * Check to see if the given path is a specified non-test data entry
 * in the blacklist KNonTestDataBlacklist
 */
TBool IsBlacklisted(TPtrC aPath)
	{
	TBool blackListed(EFalse);
	
	for(TInt i(0); i < KNumberOfExclusions; ++i)
		{
		// If a subdirectory is excluded, then this file should be too
		TInt ptrLen;
		if (aPath.Length() >= KNonTestDataBlacklist[i]->Length())
			{
			ptrLen = KNonTestDataBlacklist[i]->Length();
			}
		else
			{
			continue;
			}
		
		// Setting a substring pointer for at most comparing to the
		// current exlusion item (in case of sub-directories)
		TPtrC substr(aPath.Ptr(), ptrLen);
		
		if (KNonTestDataBlacklist[i]->CompareF(substr) == 0)
			{
			blackListed = ETrue;
			break;
			}
		}
	
	return blackListed;
	}

/*
 * Walk the directory tree on drive Z and copy all non-blacklisted files,
 * preserving directory structure.
 */
void ScanAndCopyL()
	{
	// Start scanning from root of Z:
	_LIT(KScanRoot, "Z:\\");
	
	// Create and setup a directory scanner
	CDirScan* dirScanner = CDirScan::NewL(fs);
	CleanupStack::PushL(dirScanner);
	dirScanner->SetScanDataL(KScanRoot, KEntryAttDir, ESortNone, CDirScan::EScanDownTree);
	
	// Iterate through directories
	CDir* currentDir = NULL;
	dirScanner->NextL(currentDir);
	while (currentDir)
		{
		CleanupStack::PushL(currentDir);
		
		// Iterate through current directory entries
		const TInt KDirCount(currentDir->Count());
		for (TInt i(0); i < KDirCount; ++i)
			{
			// Create full path to file/dir
			TFileName currentFilePath(dirScanner->FullPath());
			currentFilePath.Append((*currentDir)[i].iName);
			
			if ((*currentDir)[i].IsDir())
				{
				// Must add a trailing backslash if one isn't already there
				if (currentFilePath[currentFilePath.Length() - 1] != TChar('\\'))
					{
					currentFilePath.Append(TChar('\\'));
					}
				}
			
			// If this entry is black listed, don't copy it.
			if (IsBlacklisted(currentFilePath))
				{
				_LIT(KBlackListedLogFormat, "[excl] %S\n");
				test.Printf(KBlackListedLogFormat, &currentFilePath);
				
				// Don't copy this entry
				continue;
				}
			
			// Do copy/mkdir
			TRAPD(err, CopyToCDriveL(currentFilePath, (*currentDir)[i].IsDir()));
			if (err != KErrNone)
				{
				_LIT(KErrFmt, "![err] (%d) %S\n");
				test.Printf(KErrFmt, err, &currentFilePath);
				}

			currentFilePath.Zero();
			}
		
		CleanupStack::PopAndDestroy(currentDir);
		dirScanner->NextL(currentDir);		
		}
	
	
	CleanupStack::PopAndDestroy(dirScanner);
	}

/*
 * If aPath is a file, copy it to the C drive in the equivalent location.
 * If aPath is a directory, make the directory on the C drive.
 * 
 * Note that path not found errors are logged as exclusion, as it is assumed
 * that the path components higher up than them in the tree were exluded.
 */
void CopyToCDriveL(TFileName& aPath, TBool aIsDir)
	{
	const TChar KDestDrive('c');
	
	// Create the destination path
	TFileName destination(aPath);
	destination[0] = KDestDrive;
	
	TInt err(KErrNone);
	
	if (aIsDir)
		{
		// Deliberately not using MkDirAll so that we preserve the filtering
		// of directories higher up the tree.
		User::LeaveIfError(fs.MkDir(destination));
		
		_LIT(KMkDirLog, "[mkdr] %S\n");
		test.Printf(KMkDirLog, &destination);
		}
	else
		{
		// Attempt to copy file
		err = BaflUtils::CopyFile(fs, aPath, destination);
		
		User::LeaveIfError(err);
		
		_LIT(KCopyFileLog, "[copy] %S -> %S\n");
		test.Printf(KCopyFileLog, &aPath, &destination);
		}
	}


