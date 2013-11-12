// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CHtmlToCrtConverter test code
// (1)convert path
// (2)convert single file synchronously/asynchronously
// (3)synchronous/asynchronous oom test
// the resulting richtext is written to the clipboard
// 
//

#include <e32std.h>
#include <e32test.h>
#include <f32file.h>
#include <e32math.h>

#include <confndr.h>
#include <conlist.h>
#include <concnf.h>
#include "CHtmlToCrtConverter.h"
#include "CHtmlToCrtConvActive.h"


_LIT(KTestName,"CHtmlToCrtConverter");
_LIT(KTest1,"@SYMTestCaseID PIM-CONVERTERTEST-0001 CHtmlToCrtConverter test");

LOCAL_D RTest test(KTestName);

//================================================================================
//asynchronousL
//tests asynchronous conversion
//================================================================================
void asynchronousL(TDesC& aSourceFile, TDesC& aTargetFile)
	{
	CConversionObserver* observer=CConversionObserver::NewLC();
	CTestScheduler* scheduler = new(ELeave) CTestScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);
	CActiveConv* active = CActiveConv::NewL(aSourceFile, aTargetFile, observer);
	CleanupStack::PushL(active);
	active->StartConversionL();
	TInt error = scheduler->GetError();
	User::LeaveIfError(error);

	CleanupStack::PopAndDestroy(active);
	CleanupStack::PopAndDestroy(scheduler);
	CleanupStack::PopAndDestroy(observer);

	_LIT(KString,"%S");
	test.Printf(KString,&(aSourceFile));
	test.Printf(_L(" - asynchronous conversion\n"));
	}
//================================================================================
//oomAsynchronousL
//================================================================================
void oomAsynchronousL(TDesC& aSourceFile, TDesC& aTargetFile)
	{
	test.Printf(_L("starting OOM test\n"));

	//reserve space on cleanup stack
	for(TInt i=0; i<1000; i++)
		{
		CleanupStack::PushL(&i);
		}
	CleanupStack::Pop(1000);

	TInt ret=KErrNoMemory;
	TInt failAt=0;
	while (ret!=KErrNone)
		{
		failAt++;
		__UHEAP_SETFAIL(RHeap::EDeterministic,failAt);
		__UHEAP_MARK;
		TRAP(ret, asynchronousL(aSourceFile, aTargetFile));
		if (ret!=KErrNone)
			{
			__UHEAP_MARKEND;
			}
		__UHEAP_RESET;
		test(ret==KErrNoMemory||ret==KErrNone);
		test.Printf(_L("."));
		}

	test.Printf(_L("asynchronous OOM test complete\n"));
	}
//================================================================================
//synchronousL
//tests synchronous conversion
//================================================================================
void synchronousL(TDesC& aSourceFile, TDesC& aTargetFile)
	{	
	//get converter
	CCnaConverterList* convList=CCnaConverterList::NewLC();
	const TUid KUidCHtmlToCrtConverter = {0x1000a90e};
	CConverterBase* conv = convList->NewConverterL(KUidCHtmlToCrtConverter);
	CleanupStack::PushL(conv);
	if(conv)
		{
		//do conversion
		conv->ConvertL(aSourceFile, aTargetFile);
		}
	CleanupStack::PopAndDestroy(conv);
	convList->Release();
	CleanupStack::PopAndDestroy(convList);
	}
//================================================================================
//oomSynchronousL
//================================================================================
void oomSynchronousL(TDesC& aSourceFile, TDesC& aTargetFile)
	{
	test.Printf(_L("starting OOM test\n"));

	//reserve space on cleanup stack
	for(TInt i=0; i<1000; i++)
		{
		CleanupStack::PushL(&i);
		}
	CleanupStack::Pop(1000);

	TInt ret=KErrNoMemory;
	TInt failAt=0;
	while (ret!=KErrNone)
		{
		failAt++;
		__UHEAP_SETFAIL(RHeap::EDeterministic,failAt);
		__UHEAP_MARK;
		TRAP(ret, synchronousL(aSourceFile, aTargetFile));
		if (ret!=KErrNone)
			{
			__UHEAP_MARKEND;
			}
		__UHEAP_RESET;
		test(ret==KErrNoMemory||ret==KErrNone);
		test.Printf(_L("."));
		}

	test.Printf(_L("\nsynchronous OOM test complete\n"));
	}
//================================================================================
//getPathL
//================================================================================
TBool getPathL(TDes& aPath, RFs& aFs)
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

	delete entryList;
	//set path
	aPath.Copy(searchSpec);
	return ETrue;
	}
//================================================================================
//convertPathL
//================================================================================
void convertPathL(TDesC& aPath, TDesC& aPathName, RFs& aFs, TDesC& aTargetFile)
	{
	aFs.MkDirAll(aPathName);

	CDir* entryList = NULL;
	TInt error = aFs.GetDir(aPath,KEntryAttMatchMask,ESortByName,entryList);
	User::LeaveIfError(error);

	TInt numberOfFiles = entryList->Count();
	for (TInt i=0; i<numberOfFiles; i++)
		{
		//get the source file
		HBufC* temp=HBufC::NewLC(((*entryList)[i].iName).Length());
		TPtr sourceFileName(temp->Des());
		sourceFileName.Copy((*entryList)[i].iName);

		HBufC* temp2=HBufC::NewLC(((*entryList)[i].iName).Length()+aPathName.Length());
		TPtr sourceFile(temp2->Des());
		sourceFile = aPathName;
		sourceFile.Append(sourceFileName);
		//do the conversion
		synchronousL(sourceFile, aTargetFile);
		//output result
		_LIT(KString,"%S");
		test.Printf(KString,&(sourceFileName));
		test.Printf(_L("\n"));
		CleanupStack::PopAndDestroy(2);//temp, temp2
		}

	delete entryList;
	test.Printf(_L("\n%d files converted\n"),numberOfFiles);
	}
//================================================================================
//================================================================================
	
/**
@SYMTestCaseID PIM-CONVERTERTEST-0001
*/	
void doMainL()
	{
	test.Start(KTest1);

	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	TBufC<11> targetFile(_L("target file"));

//========================================================
//path
	TBuf<17> path(_L(":\\testHtml\\*.*"));
//single file
	TBuf<25> sourceFile;
	_LIT(KFileName, "test1.txt");
//========================================================
	if(getPathL(path, fs))
		{
		//CONVERT PATH
		TBuf<17> pathName(path);
		pathName.SetLength(pathName.Length()-3);

		convertPathL(path, pathName, fs, targetFile);

		//CONVERT SINGLE FILE

		sourceFile.Copy(pathName);
		sourceFile.Append(KFileName);

		synchronousL(sourceFile, targetFile);
//		asynchronousL(sourceFile, targetFile);

		//OOM TESTS
//		oomSynchronousL(sourceFile, targetFile);
//		oomAsynchronousL(sourceFile, targetFile);
		}
//========================================================

	CleanupStack::PopAndDestroy(); // fs
	
	REComSession::FinalClose(); //needed, otherwise you will get a memory leak
	
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
