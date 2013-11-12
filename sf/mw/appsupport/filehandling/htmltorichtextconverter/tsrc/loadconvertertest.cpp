// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CHtmlToCrtConverter loading test code
// 
//

#include <e32std.h>
#include <e32test.h>
#include <f32file.h>
#include <e32math.h>

#include <e32cons.h>

#include <ecom/ecom.h>

#include <confndr.h>
#include <conlist.h>
#include <concnf.h>
#include "CHtmlToCrtConverter.h"

_LIT(KTestName,"CHtmlToCrtConverter");
_LIT(KTest1,"@SYMTestCaseID PIM-LOADCONVERTERTEST-0001 CHtmlToCrtConverter test");

LOCAL_D RTest test(KTestName);

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

	//set path
	delete entryList;
	aPath.Copy(searchSpec);
	return ETrue;
	}
//==========================================================================
//loadConverterL
//==========================================================================
void loadConverterL(TDesC& aSourceFile, TDesC& aTargetFile)
	{
	//get converter
	CCnaConverterList* convList=CCnaConverterList::NewLC();
	const TUid KUidCHtmlToCrtConverter = {0x1000a90e};
	CConverterBase* conv = NULL;
	conv = convList->NewConverterL(KUidCHtmlToCrtConverter);

	if (conv)
		{
		CleanupStack::PushL(conv);
		//do conversion
		conv->ConvertL(aSourceFile, aTargetFile);	
		CleanupStack::PopAndDestroy(); //conv
		//output result
		_LIT(KString,"%S");
		test.Printf(KString,&(aSourceFile));
		test.Printf(_L(" converted\n"));
		}
	CleanupStack::PopAndDestroy(convList);
	}
//==========================================================================

/**
@SYMTestCaseID PIM-LOADCONVERTERTEST-0001
*/	
void doMainL()
	{
	test.Start(KTest1);

	RFs		fs;
	fs.Connect();

	TBufC<11> targetFile = (_L("target file"));
	TBuf<100> sourceFile(_L(":\\testHtml\\LoadConverterTest.txt"));

	if(getPathL(sourceFile, fs))
		{
		loadConverterL(sourceFile, targetFile);
		}

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
