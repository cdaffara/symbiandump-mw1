// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <f32file.h>

_LIT(KMask, "*.test");

_LIT(KTesterTclFile, "tester.tcl");

//Note: every time when you add a new KTestFileXX constant here, don't forget to add that constant as a new entry into
//"TPtrC fileNames[]" array in DoDeleteTestFilesL() function.
_LIT(KTestFile1, "test1.bt");
_LIT(KTestFile2, "test.bu");
_LIT(KTestFile3, "backup.db");
_LIT(KTestFile4, "bak.db");
_LIT(KTestFile5, "corrupt.db");
_LIT(KTestFile6, "ptf2.db");
_LIT(KTestFile7, "test1.db");
_LIT(KTestFile8, "test2.db");
_LIT(KTestFile9, "test3.db");
_LIT(KTestFile10,"test4.db");
_LIT(KTestFile11,"test.db");
_LIT(KTestFile12,"test.db2");
_LIT(KTestFile13,"bak.db-journal");
_LIT(KTestFile14,"test.db-journal-bu");
_LIT(KTestFile15,"mydir");
_LIT(KTestFile16,"testdb");
_LIT(KTestFile17,"test2-script.tcl");
_LIT(KTestFile18,"test.tcl");
_LIT(KTestFile19,"speed1.txt");
_LIT(KTestFile20,"speed2.txt");
_LIT(KTestFile21,"test.db-bu1");
_LIT(KTestFile22,"test.db-bu2");

_LIT(KTestDir1,"mydir-journal");

#ifdef _DEBUG 

#define PRINT_DELFILE_ERROR(aFileName, aErr) \
	if(aErr != KErrNone && aErr != KErrNotFound)\
		{\
		RDebug::Print(_L("###TCLSQLITE: Failed to delete file \"%S\". Error=%d\r\n"), &aFileName, aErr);\
		}
	
#else

#define PRINT_DELFILE_ERROR(aFileName, aErr)	void(0)
	
#endif

//Connects the file session argument.
//Creates application's private datacage on drive C: (if does not exist).
//Copies the private path as string to the aPrivatePath argument (without the drive name).
//aPrivatePath must point to a big enough place (ideally TFileName object).
static void GetFsAndPrivatePathL(RFs& aFs, TDes& aPrivatePath)
	{
	User::LeaveIfError(aFs.Connect());
	TInt err = aFs.CreatePrivatePath(EDriveC);
	if(!(err == KErrNone || err == KErrAlreadyExists))
		{
		User::Leave(err);
		}

	User::LeaveIfError(aFs.PrivatePath(aPrivatePath));
	}

//The function constructs a full script file path, containing wildcards, in aFullPath argument
//	(from aDriveNumber and aPrivatePath arguments, using KMask constant as a file name).
//aFullPath must point to a big enough place (ideally TFileName object).
static void ConstructFilePathByMask(TDriveNumber aDriveNumber, const TDesC& aPrivatePath, TDes& aFullPath)
	{
	TDriveName srcDriveName = TDriveUnit(aDriveNumber).Name();
	aFullPath.Copy(srcDriveName);
	aFullPath.Append(KMask);
	TParse parse;
	parse.Set(aPrivatePath, &aFullPath, 0);
	aFullPath.Copy(parse.FullName());
	}

//The function constructs a full file path in aFullPath argument (from aDriveNumber, aFileName and aPrivatePath arguments).
//aFullPath must point to a big enough place (ideally TFileName object).
static void ConstructFilePathByName(TDriveNumber aDriveNumber, const TDesC& aFileName, const TDesC& aPrivatePath, TDes& aFullPath)
	{
	TDriveName srcDriveName = TDriveUnit(aDriveNumber).Name();
	aFullPath.Copy(srcDriveName);
	aFullPath.Append(aFileName);
	TParse parse;
	parse.Set(aPrivatePath, &aFullPath, 0);
	aFullPath.Copy(parse.FullName());
	}

//The function constructs a full path in aDestPath argument (from aDriveNumber and aPrivatePath arguments).
//aDestPath must point to a big enough place (ideally TFileName object).
static void ConstructDestPath(TDriveNumber aDriveNumber, const TDesC& aPrivatePath, TDes& aDestPath)
	{
	TDriveName destDriveName = TDriveUnit(aDriveNumber).Name();
	TParse parse;
	parse.Set(aPrivatePath, &destDriveName, 0);
	aDestPath.Copy(parse.FullName());
	}

//The function copies all test script files from Z: to C: drive, in application's private data cage.	
static void DoCopyTestFilesL()
	{
	RDebug::Print(_L("###TCLSQLITE: Construct private data cage on drive C:\r\n"));
	RFs fs;
	CleanupClosePushL(fs);
	TFileName privatePath;
	GetFsAndPrivatePathL(fs, privatePath);

	CFileMan* fm = CFileMan::NewL(fs);
	CleanupStack::PushL(fm);
	
	TFileName srcPath;
	ConstructFilePathByMask(EDriveZ, privatePath, srcPath);
	TFileName destPath;
	ConstructDestPath(EDriveC, privatePath, destPath);
	RDebug::Print(_L("###TCLSQLITE: Copying \"%S\" to \"%S\"\r\n"), &srcPath, &destPath);
	User::LeaveIfError(fm->Copy(srcPath, destPath));

	ConstructFilePathByName(EDriveZ, KTesterTclFile, privatePath, srcPath);
	RDebug::Print(_L("###TCLSQLITE: Copying \"%S\" to \"%S\"\r\n"), &srcPath, &destPath);
	User::LeaveIfError(fm->Copy(srcPath, destPath));
	
	CleanupStack::PopAndDestroy(2);
	}

//The function deletes a file, identified by the aFullPath argument.
//The function leaves if the delete operation error is different than KErrNone and KErrNotFound.
static void DoDeleteTestFileL(CFileMan& aFm, const TDesC& aFullPath)
	{
	TInt err = aFm.Attribs(aFullPath, 0, KEntryAttReadOnly, TTime(0));
	if(err == KErrNone)
		{
		err = aFm.Delete(aFullPath);
		}
	if(err != KErrNone && err != KErrNotFound)
		{
		User::Leave(err);	
		}
	}

//The function deletes a directory, identified by the aFullPath argument.
//The function leaves if the delete operation error is different than KErrNone and KErrNotFound.
static void DoDeleteTestDirL(CFileMan& aFm, const TDesC& aFullPath)
	{
	TInt err = aFm.Attribs(aFullPath, 0, KEntryAttReadOnly, TTime(0));
	if(err == KErrNone)
		{
		err = aFm.RmDir(aFullPath);
		}
	if(err != KErrNone && err != KErrNotFound)
		{
		User::Leave(err);	
		}
	}

//Deletes the test scripts and test output files from C: drive.
static void DoDeleteTestFilesL()
	{
	RFs fs;
	CleanupClosePushL(fs);
	TFileName privatePath;
	GetFsAndPrivatePathL(fs, privatePath);

	CFileMan* fm = CFileMan::NewL(fs);
	CleanupStack::PushL(fm);

	TFileName filePath;
	ConstructFilePathByMask(EDriveC, privatePath, filePath);
	TRAPD(err, DoDeleteTestFileL(*fm, filePath));
	PRINT_DELFILE_ERROR(filePath, err);

	TPtrC fileNames[] = 
		{
		KTesterTclFile(), 
		KTestFile1(), KTestFile2(), KTestFile3(), KTestFile4(), KTestFile5(), 
		KTestFile6(), KTestFile7(), KTestFile8(), KTestFile9(), KTestFile10(),
		KTestFile11(), KTestFile12(), KTestFile13(), KTestFile14(), KTestFile15(), 
		KTestFile16(), KTestFile17(), KTestFile18(), KTestFile19(), KTestFile20(),
		KTestFile21(), KTestFile22()
		};
	for(TInt i=0;i<(sizeof(fileNames)/sizeof(fileNames[0]));++i)
		{
		ConstructFilePathByName(EDriveC, fileNames[i], privatePath, filePath);
		TRAP(err, DoDeleteTestFileL(*fm, filePath));
		PRINT_DELFILE_ERROR(filePath, err);
		}

	ConstructFilePathByName(EDriveC, KTestDir1, privatePath, filePath);
	TRAP(err, DoDeleteTestDirL(*fm, filePath));
	PRINT_DELFILE_ERROR(filePath, err);

	CleanupStack::PopAndDestroy(2);
	}
	
//Deletes the test scripts from C: drive
//Because the TCL SQLITE exe has mutiple exit points, there is no right place in the code where this function
//can be called from.
//The way how the test cleanup is implemented is:
// - a new script function has been definied and registered (tclsqlite.c)- "delete_test_files"
// - the "delete_test_files" function is called from the "finalize_testing" procedure (tester.tcl file),
//	   that is guaranteed to be called at the end of any test script execution
extern "C" int DeleteTestFiles(void)
	{
	RDebug::Print(_L("###TCLSQLITE: Begin \"Delete test files\" operation\r\n"));
	TRAP_IGNORE(DoDeleteTestFilesL());
	RDebug::Print(_L("###TCLSQLITE: \"Delete test files\" operation has completed\r\n"));
	return 0;
	}

//Copies the test scripts from Z: to C: drive	
//This function is called from main() (tclsqlite.c file)
extern "C" TInt CopyTestFiles(void)
	{
    RDebug::Print(_L("###TCLSQLITE: Begin \"Copy test files\" operation\r\n"));
	TRAPD(err, DoCopyTestFilesL());
	if(err != KErrNone)
		{
		RDebug::Print(_L("###TCLSQLITE: \"Copy test files\" operation has failed with error %d\r\n"), err);
		DeleteTestFiles();
		}
	else
		{
		RDebug::Print(_L("###TCLSQLITE: \"Copy test files\" operation has completed successfully\r\n"));
		}
	return err;
	}
