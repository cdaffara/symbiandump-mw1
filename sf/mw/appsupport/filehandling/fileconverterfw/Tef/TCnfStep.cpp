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
//

/**
 @file
 @internalComponent - Internal Symbian test code 
*/
	
#include <e32svr.h>
#include <e32std.h>
#include <ecom/ecom.h>
#include "TCnfStep.h"

void CTCnfStep::ChangeLocaleL(TLanguage aLanguage)
	{
	TBuf<50> libraryName;
#ifdef SYMBIAN_DISTINCT_LOCALE_MODEL 
	libraryName.Format(_L("elocl_lan.%03d"), aLanguage);
	TExtendedLocale myExtendedLocale;
	TInt err = myExtendedLocale.LoadLocaleAspect(libraryName);
	TEST(err == KErrNone);
	User::LeaveIfError(err);
	err = myExtendedLocale.SaveSystemSettings();
	TEST(err == KErrNone);
	User::LeaveIfError(err);
		
#else
	libraryName.Format(_L("ELOCL.%02d"), aLanguage);
	TInt err = UserSvr::ChangeLocale(libraryName);
	User::LeaveIfError(err);
	TEST(err == KErrNone);
#endif
	}
	
void CTCnfStep::CreateConv1L()
	{
	_LIT(KEnglishabc,"english_abc");
	_LIT(KEnglishdef,"english_def");
	_LIT(KFrenchabc,"french_abc");
	_LIT(KFrenchdef,"french_def");

	TInt ret = 0;

	CMimeInfo* mime = CMimeInfo::NewLC(mimeWord_Tcnf);
	mime->AddLanguageL(TLanguageInfo(ELangEnglish,engword_Tcnf));
	mime->AddLanguageL(TLanguageInfo(ELangFrench,frword_Tcnf));

	CleanupStack::PopAndDestroy(); //mime
	
	_LIT(KPath,"z:\\resource\\convert\\10004c41.RSC");
	TFileName resourceFile;
	resourceFile.Copy(KPath);
	CCnaConvInfoFileReader2* readConverter=CCnaConvInfoFileReader2::NewL(resourceFile);
	CleanupStack::PushL(readConverter);
	readConverter->RestoreL();
	CCnaConverter* con2 = readConverter->AtL(0);
	CleanupStack::PushL(con2);
	
	TLanguage originalLanguage = User::Language();
	TRAP(ret,ChangeLocaleL(ELangEnglish));
	TEST(ret == KErrNone);
	TEST(User::Language() == ELangEnglish);
	TDataType dt1(_L8("text/abc"));
	TTranslation trans = con2->MimeFromText(dt1);
	TEST(trans == KEnglishabc);

	TDataType dt2(_L8("text/def"));
	trans = con2->MimeToText(dt2);
	TEST(trans == KEnglishdef);
	
	TRAP(ret,ChangeLocaleL(ELangFrench));
	TEST(ret == KErrNone);
	TEST(User::Language() == ELangFrench);
	if(User::Language() == ELangFrench)
		{
		trans = con2->MimeFromText(dt1);
		TEST(trans == KFrenchabc);
	
		trans = con2->MimeToText(dt2);
		TEST(trans == KFrenchdef);
		}
	TRAP(ret,ChangeLocaleL(originalLanguage));
	TEST(ret == KErrNone);
	
	CleanupStack::PopAndDestroy(2); //con2 and readConverter
	}

void CTCnfStep::testConverterCNFWriterL()
	{
	TUid uid;
	uid.iUid=123;
	CCnaConverter* con1=CCnaConverter::NewL(uid);
	CleanupStack::PushL(con1);
	TEST(con1->Uid()==uid);
	uid.iUid=1234;
	con1->SetUid(uid);
	TEST(con1->Uid()==uid);
	CleanupStack::PopAndDestroy(); //con1

	CreateConv1L();
	}

CTCnfStep::~CTCnfStep()
/**
   Destructor
 */
	{
	}

CTCnfStep::CTCnfStep()
/**
   Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTCnfStep);
	}

TVerdict CTCnfStep::doTestStepL()
	{
	INFO_PRINTF1(_L("Testing the Conarc dll..."));

	// run the testcode (inside an alloc heaven harness)

 	__UHEAP_MARK;
	INFO_PRINTF1(_L("About to test the Converter CNF"));
	
	mimeWord_Tcnf = TDataType(_L8("mime/msword"));
	engword_Tcnf = (_L("englishword"));
	frword_Tcnf = (_L("frenchword"));
	
	TRAPD(r,CTCnfStep::testConverterCNFWriterL());
	TEST(r==KErrNone);
	INFO_PRINTF1(_L("test finished\n"));

	REComSession::FinalClose();
	__UHEAP_MARKEND;

	return TestStepResult();
	}
