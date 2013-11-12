// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// The following test case is used to test the ecom style converter plugin and test the list and converter APIs
// 
//

/**
 @file
 @internalComponent - Internal Symbian test code
*/

#include <e32std.h>
#include <e32cons.h>
#include <s32file.h>
#include <e32test.h>
#include <ecom/ecom.h>

#include <concnf.h>
#include <conlist.h>
#include "TLoadEcomCnvStep.h"
#include "TTestEcomConverter.hrh"

 
/**
  Auxiliary Fn for TestCaseID TLoadEcomCnvStep-ExecuteL
  Construct CCnaConverterList object and call ConvFromListL & ConvToListL
  
*/
void CTLoadEComCnvStep::TestListsL()
	{
	TDataType dataType;
	CCnaConverterList* list=CCnaConverterList::NewLC();
	list->UpdateL();
	CArrayFixFlat<SConverterInfo>* info;
	info= new(ELeave) CArrayFixFlat<SConverterInfo>(5);

	CleanupStack::PushL(info);

	list->ConvFromListL(mimeEngEpoc32CPlaintext,info);
	TInt count =info->Count();
	INFO_PRINTF1(_L("Test from list\n"));
	for (TInt i=0;i<count;i++)
		{
		INFO_PRINTF1((*info)[i].iTranslation);
		INFO_PRINTF2(_L(" uid= %d \n"),(*info)[i].iUid);
		if(list->MimeTypeFrom(dataType,(*info)[i])==KErrNone)
			{
			TPtrC mimeType(dataType.Des());
			INFO_PRINTF2(_L(" mimeType= %S \n\n"),&mimeType);
			}
		else
			{
			INFO_PRINTF2(_L("***** ERROR: MimeTypeFrom returned KErrNotFound\n\n"),i);
			}
		}
	info->Reset();
	list->ConvToListL(mimeFrenchxyzzyFrot,info);
	count =info->Count();
	INFO_PRINTF1(_L("Test to list\n"));
	for (TInt j=0;j<count;j++)
		{
		INFO_PRINTF1((*info)[j].iTranslation);
		INFO_PRINTF2(_L(" uid= %d \n"),(*info)[j].iUid);
		if(list->MimeTypeTo(dataType,(*info)[j])==KErrNone)
			{
			TPtrC mimeType(dataType.Des());
			INFO_PRINTF2(_L(" mimeType= %S \n\n"),&mimeType);
			}
		else
			{
			INFO_PRINTF2(_L("***** ERROR: MimeTypeTo returned KErrNotFound\n\n"),j);
			}
		}
	CleanupStack::PopAndDestroy(2);

	}

 /**
   @SYMTestCaseID		TLoadEcomCnvStep-ExecuteL
  
   @SYMPREQ				REQ2689		
  
   @SYMTestCaseDesc 	Construct CCnaConverterList object and get the converter by uid as well as mime style
  
   @SYMTestPriority 	High
  
   @SYMTestStatus 		Implemented
   
   @SYMTestActions  
   To construct CCnaConverterList object and get the instance of the converter passing the relevant UID\n
   Using the list object its following APIs are tested
   API Calls:\n		
   CCnaConverterList::NewLC();
   CCnaConverterList::NewConverterL(TUid aUid);
   CCnaConverterList::Count();
   CCnaConverterList::ConvFromL(const TDataType& aMimeType);
   CCnaConverterList::ConvToL(const TDataType& aMimeType);
   CCnaConverterList::ConverterL(const TDataType& aFrom,const TDataType& aTo);
   CTestEcomConverter::ConvertL(const TFileName& aSourceFile, const TFileName& aTargetFile, 
								MConverterUiObserver* aObserver=NULL);
   @SYMTestExpectedResults Test should complete without any panic.
   
 */
void CTLoadEComCnvStep::ExecuteL()
	{
	TUid uid;
	uid.iUid=KTestEcomConvImplementationUid;
	CCnaConverterList* list=CCnaConverterList::NewLC();
	list->UpdateL();
	INFO_PRINTF2(_L("Number of converters found %d\n"),list->Count());
	CConverterBase* conv=list->NewConverterL(uid);
	CleanupStack::PushL(conv);
	conv->ConvertL(_L("file1"), _L("File2"));
	INFO_PRINTF1(_L("Converter by uid\n"));
	CleanupStack::PopAndDestroy(); // conv
	TUid fromUid=list->ConvFromL(mimeEngEpoc32CPlaintext);
	INFO_PRINTF2(_L(" ConvFromUid= %d \n"),fromUid);
	TUid toUid=list->ConvToL(mimeFrenchxyzzyFrot);
	INFO_PRINTF2(_L(" ConvToUid= %d \n"),toUid);
	TUid converterUid=list->ConverterL(mimeEngEpoc32CPlaintext,mimeFrenchxyzzyFrot);
	INFO_PRINTF2(_L(" ConvterterUid= %d \n"),converterUid);
	conv=list->NewConverterL(fromUid);
	CleanupStack::PushL(conv);
	conv->ConvertL(_L("file1"), _L("file2"));
	INFO_PRINTF1(_L("Converter by mime type\n"));
	CleanupStack::Pop(); // conv
	delete conv;
	CleanupStack::PopAndDestroy(); // list
	INFO_PRINTF1(_L("Destroyed converter list\n"));
	TestListsL();
	}

CTLoadEComCnvStep::CTLoadEComCnvStep()
/**
   Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTLoadEComCnvStep);
	}

CTLoadEComCnvStep::~CTLoadEComCnvStep()
	{
	}

TVerdict CTLoadEComCnvStep::doTestStepL()
    {
	INFO_PRINTF1(_L("Test Started"));
	__UHEAP_MARK; // mark heap state
	
	mimeEngEpoc32CPlaintext = TDataType(_L8("foo/bar"));
	mimeFrenchxyzzyFrot = TDataType(_L8("xyzzy/frot"));
	

	TRAPD(error,ExecuteL()); // do most stuff under cleanup stack
	TEST(error == KErrNone);
	REComSession::FinalClose(); 
	
	__UHEAP_MARKEND; // check no memory leak
	
	INFO_PRINTF1(_L("Test Finished"));
	
	return TestStepResult();
    }
