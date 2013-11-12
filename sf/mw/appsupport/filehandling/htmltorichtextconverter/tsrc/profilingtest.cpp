// Copyright (c) 2003-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// CHtmlToCrtConverter profiling test code
// profiles conversion of files of size 0.1kb - 2kb in 0.1kb increments
// and then 2kb - 20kb in 1kb increments
// The results are written to the file results.csv
// 
//

#include <e32std.h>
#include <e32test.h>
#include <f32file.h>
#include <e32math.h>
#include <e32def_private.h>
#include "CHtmlToCrtConverter.h"
#include "CHtmlToCrtConvActive.h"

_LIT(KTestName,"CHtmlToCrtConverter");

LOCAL_D RTest test(KTestName);

//=================================================================================
//synchronousConversionL
//tests synchronous conversion
//=================================================================================
void synchronousConversionL(TDesC& aSourceFile, TDesC& aTargetFile)
	{
	CHtmlToCrtConverter* converter = new(ELeave) CHtmlToCrtConverter;
	CleanupStack::PushL(converter);
	converter->ConvertL(aSourceFile, aTargetFile);		

	CleanupStack::PopAndDestroy(converter);
	}
//================================================================================
//getPath
//================================================================================
TBool getPath(TDes& aPath, RFs& aFs)
	{
	// Generate the folder spec to search in
	TFileName searchSpec;
	TChar driveLetter = 'C';
	searchSpec.Append(driveLetter);
	searchSpec.Append(aPath);

	CDir* entryList = NULL;
	TFindFile finder(aFs);
	TInt ret = finder.FindWildByPath(searchSpec, NULL, entryList);
	if(ret<KErrNone)
		{
		//try z drive
		driveLetter = 'Z';
		searchSpec.Zero();
		searchSpec.Append(driveLetter);
		searchSpec.Append(aPath);
		delete entryList;
		entryList = NULL;

		ret = finder.FindWildByPath(searchSpec, NULL, entryList);
		if	(ret < KErrNone)
			{
			//path not found
			delete entryList;
			test.Printf(_L("files not found\n"));
			return EFalse;
			}
		}

	//set path
	delete entryList;
	aPath.Copy(searchSpec);
	return ETrue;
	}
//=================================================================================
//doProfilingL
//=================================================================================
void doProfilingL(TDesC& aPath, TDesC& aPathName, RFs& aFs, TDesC& aTargetFile, TDesC& aResultFile)
	{
	TBuf<24> sourceFile;

	CDir* entryList = NULL;
	TInt error = aFs.GetDir(aPath,KEntryAttMatchMask,ESortByName,entryList);
	User::LeaveIfError(error);

	TInt numberOfEntries = entryList->Count();
	TBuf<11>sourceFileName;
	_LIT8(KCSVHeadings,"size,time\r\n");
	HBufC8* temp=HBufC8::NewLC(500);
	TPtr8 resultBuffer(temp->Des());
	resultBuffer.Append(KCSVHeadings);

	for (TInt i=0; i<numberOfEntries; i++)
		{
		//get file name
		sourceFileName = (*entryList)[i].iName;
		sourceFile = aPathName;
		sourceFile.Append(sourceFileName);
		__PROFILE_RESET(1);
		__PROFILE_START(0);
		synchronousConversionL(sourceFile, aTargetFile);
		__PROFILE_END(0);
		__PROFILE_DISPLAY(1);
		}

	//write result to file
	_LIT(KPath,"c:\\");
	_LIT(KExtension,".csv");
	TBuf<256> result(KPath);
	result.Append(aResultFile);
	result.Append(KExtension);

	RFs theFs;
	User::LeaveIfError(theFs.Connect());
	CleanupClosePushL(theFs);
	RFile file;
	TInt err=file.Replace(theFs, result,
	EFileWrite+EFileShareAny+EFileStreamText);
	User::LeaveIfError(err);
	CleanupClosePushL(file);
	file.Write(resultBuffer);
	CleanupStack::PopAndDestroy(3);

	delete entryList;
	}
//=================================================================================
	
/**
@SYMTestCaseID PIM-PROFILINGTEST-0001
*/
void doMainL()
	{
	test.Start(_L("@SYMTestCaseID PIM-PROFILINGTEST-0001 CHtmlToCrtConverter profiling test"));

	RFs		fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);

	TBufC<11> targetFile(_L("target file"));

//path to be profiled
	TBuf<18> path(_L(":\\profiling\\*.*"));

//file to write results to
	_LIT(KResultFile,"results");
	TBufC<7> resultFile(KResultFile);

	if(getPath(path, fs))
		{
		TBuf<18> pathName(path);
		pathName.SetLength(pathName.Length()-3);

		doProfilingL(path, pathName, fs, targetFile, resultFile);
		}

	CleanupStack::PopAndDestroy(&fs);

	test.End();
	test.Close();
	}

GLDEF_C TInt E32Main()
	{	
	__UHEAP_MARK;

	CTrapCleanup* theCleanup=CTrapCleanup::New();
	TRAPD(ret,doMainL());		
	test(ret==KErrNone);
	delete theCleanup;	

	__UHEAP_MARKEND;
	return(KErrNone);
	}
