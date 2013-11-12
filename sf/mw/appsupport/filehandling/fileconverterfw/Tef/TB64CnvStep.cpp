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


#include <s32file.h>
#include <s32mem.h>

#include <conarc.h>
#include <concnf.h>
#include <conlist.h>
#include <bautils.h>
#include "TB64CnvStep.h"
#include <ecom/ecom.h>
#include <u32hal.h>

_LIT(KLitOriginalTxtFile,						"z:\\resource\\test\\conarc\\release.txt");
_LIT(KLitExpectedEtextFromTxtFile,				"z:\\resource\\test\\Conarc\\release.txt.etext");
_LIT(KLitOriginalDocFile,						"z:\\resource\\test\\Conarc\\w7allchr.doc");
_LIT(KLitExpectedBase64FromDocFile,				"z:\\resource\\test\\Conarc\\w7allchr.doc.b64");
_LIT(KLitExpectedQuotedPrintableFromDocFile,	"z:\\resource\\test\\Conarc\\w7allchr.doc.qp");
_LIT(KLitExpectedQuotedPrintableFromDocFileWithoutSplChar,	"z:\\resource\\test\\Conarc\\w7allchrwosplchr.doc.qp");

_LIT(KLitDirectoryForGeneratedFiles,			"c:\\conarc\\");
_LIT(KLitGeneratedEtextFromTxtFile,				"c:\\conarc\\release.txt.etext");
_LIT(KLitGeneratedTxtFileFromEtext,				"c:\\conarc\\release.txt.etext.txt");
_LIT(KLitGeneratedBase64FromDocFile,			"c:\\conarc\\w7allchr.doc.b64");
_LIT(KLitGeneratedDocFileFromBase64,			"c:\\conarc\\w7allchr.doc.b64.doc");
_LIT(KLitGeneratedQuotedPrintableFromDocFile,	"c:\\conarc\\w7allchr.doc.qp");
_LIT(KLitGeneratedDocFileFromQuotedPrintable,	"c:\\conarc\\w7allchr.doc.qp.doc");
 
TBool CTB64CnvStep::FilesAreIdenticalL(RFs& aFs, const TDesC& aFile1, const TDesC& aFile2)
	{ // static
	RFile file1;
	CleanupClosePushL(file1);
	User::LeaveIfError(file1.Open(aFs, aFile1, EFileShareReadersOnly|EFileStream|EFileRead));
	RFile file2;
	CleanupClosePushL(file2);
	User::LeaveIfError(file2.Open(aFs, aFile2, EFileShareReadersOnly|EFileStream|EFileRead));
	TBuf8<200> buffer1;
	TBuf8<200> buffer2;
	TBool result=EFalse;
	FOREVER
		{
		User::LeaveIfError(file1.Read(buffer1));
		User::LeaveIfError(file2.Read(buffer2));
		if (buffer1!=buffer2) // catches the case where the buffers' contents are different, and/or where they have different lengths, e.g. where one is empty
			{
			result=EFalse;
			break;
			}
		if (buffer1.Length()==0)
			{
			__ASSERT_ALWAYS(buffer2.Length()==0, User::Invariant());
			result=ETrue;
			break;
			}
		}
	CleanupStack::PopAndDestroy(2, &file1);
	return result;
	}

void CTB64CnvStep::ConvertFileL(CCnaConverterList& aList, TUint aConverterUid, const TDesC& aInputFile, const TDesC& aOutputFile)
	{
	CConverterBase* const converter=aList.NewConverterL(TUid::Uid(aConverterUid));
	TEST(converter!=NULL);
	CleanupStack::PushL(converter);
	TEST(converter->Uid().iUid==aConverterUid);
	TEST(converter->Capabilities()&CConverterBase::EConvertsObjects);
	RFileReadStream inputStream;
	CleanupClosePushL(inputStream);
	RFileWriteStream outputStream;
	CleanupClosePushL(outputStream);
	User::LeaveIfError(inputStream.Open(iFs, aInputFile, EFileShareReadersOnly|EFileStream|EFileRead));
	User::LeaveIfError(outputStream.Replace(iFs, aOutputFile, EFileShareExclusive|EFileStream|EFileWrite));
	converter->ConvertObjectL(inputStream, outputStream);
	outputStream.CommitL();
	CleanupStack::PopAndDestroy(3, converter);
 	}
 
void CTB64CnvStep::DoTextEtextL(CCnaConverterList* aList)
	{
	INFO_PRINTF1(_L("Testing Etext\n"));

	ConvertFileL(*aList, KUidPlainTextToETextConverter, KLitOriginalTxtFile, KLitGeneratedEtextFromTxtFile);
	INFO_PRINTF1(_L("... testing conversion to Etext\n"));
	TEST(FilesAreIdenticalL(iFs, KLitGeneratedEtextFromTxtFile, KLitExpectedEtextFromTxtFile));
 	ConvertFileL(*aList, KUidETextToPlainTextConverter, KLitGeneratedEtextFromTxtFile, KLitGeneratedTxtFileFromEtext);
	INFO_PRINTF1(_L("... and testing conversion back from Etext (round trip)\n"));
	TEST(FilesAreIdenticalL(iFs, KLitGeneratedTxtFileFromEtext, KLitOriginalTxtFile));

	TEST(BaflUtils::DeleteFile(iFs, KLitGeneratedEtextFromTxtFile)==KErrNone);
	TEST(BaflUtils::DeleteFile(iFs, KLitGeneratedTxtFileFromEtext)==KErrNone);
 	}

void CTB64CnvStep::DoTextEtextWithControlCharsL(CCnaConverterList* aList)
	{
	TUid uid={KUidPlainTextToETextConverter};
	INFO_PRINTF1(_L("Testing Etext with Control Characters\n"));
	TBuf8<50> buf=_L8("Testing control characters ");
	buf.Append('\x000');
	TBufC8<20> buf2(_L8("\x006\x007 ah!!!"));
	buf.Append(buf2);
	TBuf8<50> out;
	RDesReadStream read(buf);
	RDesWriteStream write(out);
	CConverterBase* conv=aList->NewConverterL(uid);
	TEST(conv!=NULL);
	CleanupStack::PushL(conv);
	TEST(conv->Uid()==uid);
	TEST(conv->Capabilities()&CConverterBase::EConvertsObjects);
	conv->ConvertObjectL(read,write);
	CleanupStack::PopAndDestroy(conv);
	write.CommitL();
	write.Close();
	read.Close();
	TBuf8<50> resultBuf;
	read.Open(out);
	write.Open(resultBuf);
	uid.iUid=KUidETextToPlainTextConverter;
 	conv=aList->NewConverterL(uid);
	TEST(conv!=NULL);
	CleanupStack::PushL(conv);
	TEST(conv->Uid()==uid);
	TEST(conv->Capabilities()&CConverterBase::EConvertsObjects);
	conv->ConvertObjectL(read,write);
	CleanupStack::PopAndDestroy(conv);
	write.CommitL();
	write.Close();
	read.Close();
	TEST(buf==resultBuf);
	}

/** 
   @SYMTestCaseID	 TB64CNVStep-DoTestBase64L
  
   @SYMPREQ		 REQxxx	
  
   @SYMTestCaseDesc  Base 64 Test Converter
  
   @SYMTestPriority  High
  
   @SYMTestStatus 	 Implemented
   
   @SYMTestActions  	Converts a .b64 file to a .doc file, Converts a .doc file to a .b64 file and compares them.
					Test the following APIs
					CConverterBase2::ConvertObjectL(RReadStream& aReadStream, RWriteStream& aWriteStream, 
					MConverterUiObserver* aObserver=NULL);	
					CConverterBase2::Capabilities();
					CConverterBase2::Uid();
					CCnaConverterList::NewConverterL(TUid aUid)
   
   @SYMTestExpectedResults Compare is succesful  
*/

void CTB64CnvStep::DoTestBase64L(CCnaConverterList* aList)
	{
	INFO_PRINTF1(_L("Testing Base64\n"));
	ConvertFileL(*aList, KUidConverterToBase64, KLitOriginalDocFile, KLitGeneratedBase64FromDocFile);
	INFO_PRINTF1(_L("... testing conversion to Base64\n"));
	TEST(FilesAreIdenticalL(iFs, KLitGeneratedBase64FromDocFile, KLitExpectedBase64FromDocFile));

	ConvertFileL(*aList, KUidConverterFromBase64, KLitGeneratedBase64FromDocFile, KLitGeneratedDocFileFromBase64);
	INFO_PRINTF1(_L("... and testing conversion back from Base64 (round trip)\n"));
	TEST(FilesAreIdenticalL(iFs, KLitGeneratedDocFileFromBase64, KLitOriginalDocFile));

	TEST(BaflUtils::DeleteFile(iFs, KLitGeneratedBase64FromDocFile)==KErrNone);
	TEST(BaflUtils::DeleteFile(iFs, KLitGeneratedDocFileFromBase64)==KErrNone);
 
	}

/** 
   @SYMTestCaseID	 TB64CNVStep-DoTestQPL
  
   @SYMPREQ		REQxxx
  
   @SYMTestCaseDesc  Quoted Printable Test Converter
  
   @SYMTestPriority  High
  
   @SYMTestStatus 	 Implemented
   
   @SYMTestActions  	Converts a .qp file to a .doc file, Converts a .qp.doc file to a .qp file and compares them
					Test the following APIs
					CConverterBase2::ConvertObjectL(RReadStream& aReadStream, RWriteStream& aWriteStream, 
					MConverterUiObserver* aObserver=NULL);	
					CConverterBase2::Capabilities();
					CConverterBase2::Uid();
					CCnaConverterList::NewConverterL(TUid aUid)
   
   @SYMTestExpectedResults Compare is succesful
   
*/	
void CTB64CnvStep::DoTestQPL(CCnaConverterList* aList)
	{
	INFO_PRINTF1(_L("Testing Quoted-printable\n"));
	
	// On ARMV5 platform, this test is automated only with the value 1 (the default value)
	// of ROM patchable constant 'KEnableAllSplCharForQpCnv'. This does the convertion of all the special
	// characters into Quoted Printable format. The special characters are not converted if the patch data
	// value is set to 0 (zero) and it has been tested manually.
	TInt enableAllSplCharForQpCnv = 1;
	
#ifdef __WINS__
	// KEnableAllSplCharForQpCnv is a Rom patchable constant, so patch for emulator builds can be done by
	// adding "patchdata_conarc_dll_KEnableAllSplCharForQpCnv X" to epoc.ini file.
	// When X is set to non zero, the conversion is performed for all the special characters.
	// When X is set to zero, no conversion is performed in case of the special characters.
	// If this patch data value is not given in epoc.ini file, then this test will be executed with
	// the value 1 (the default value).
	
	TInt valueOfKEnableAllSplCharForQpCnv = 0;
	if (UserSvr::HalFunction(EHalGroupEmulator,EEmulatorHalIntProperty,(TAny*)"patchdata_conarc_dll_KEnableAllSplCharForQpCnv",&valueOfKEnableAllSplCharForQpCnv) == KErrNone)
		{
		enableAllSplCharForQpCnv = valueOfKEnableAllSplCharForQpCnv;
		}
#endif
		
	ConvertFileL(*aList, KUidPlainTextToQuotedPrintableConverter, KLitOriginalDocFile, KLitGeneratedQuotedPrintableFromDocFile);
	INFO_PRINTF1(_L("... testing conversion to Quoted-printable\n"));

	if (enableAllSplCharForQpCnv)
		{
		TEST(FilesAreIdenticalL(iFs, KLitGeneratedQuotedPrintableFromDocFile, KLitExpectedQuotedPrintableFromDocFile));
		}
	else
		{
		TEST(FilesAreIdenticalL(iFs, KLitGeneratedQuotedPrintableFromDocFile, KLitExpectedQuotedPrintableFromDocFileWithoutSplChar));
		}

	ConvertFileL(*aList, KUidQuotedPrintableToPlainTextConverter, KLitGeneratedQuotedPrintableFromDocFile, KLitGeneratedDocFileFromQuotedPrintable);
	INFO_PRINTF1(_L("... and testing back from Quoted-printable (round trip)\n"));
	TEST(FilesAreIdenticalL(iFs, KLitGeneratedDocFileFromQuotedPrintable, KLitOriginalDocFile));

	TEST(BaflUtils::DeleteFile(iFs, KLitGeneratedQuotedPrintableFromDocFile)==KErrNone);
	TEST(BaflUtils::DeleteFile(iFs, KLitGeneratedDocFileFromQuotedPrintable)==KErrNone);
 	}

void CTB64CnvStep::DoSpecificL(CCnaConverterList* aList)
	{
	INFO_PRINTF1(_L("Specific tests"));
	INFO_PRINTF1(_L("QP conv 123 test"));
	TBuf8<25> buf = _L8("123");
	TBuf8<25> out;
	RDesReadStream read(buf);
	RDesWriteStream write(out);
	TUid uid={KUidPlainTextToQuotedPrintableConverter};
	CConverterBase* conv=aList->NewConverterL(uid);
	TEST(conv!=NULL);
	conv->ConvertObjectL(read,write);
	read.Close();
	write.CommitL();
	write.Close();
	TEST(buf==out);
	delete conv;
	INFO_PRINTF1(_L("String splitting"));
	TBuf8<256> buf2 = _L8("00 Xxxxxx Xxxxx\x00d\x00aXxxxx\x00d\x00aXxxxxxxxxxxxxxxxxx XX00 0XX\x00d\x00aUnited Kingdom");
	TBuf8<256> out2;
	read.Open(buf2);
	write.Open(out2);
	CConverterBase* conv1=aList->NewConverterL(uid);
	TEST(conv1!=NULL);
	conv1->ConvertObjectL(read,write);
	read.Close();
	write.CommitL();
	write.Close();
	delete conv1;
	TBuf8<256> return2;
	read.Open(out2);
	write.Open(return2);
	TUid uid2={KUidQuotedPrintableToPlainTextConverter};
	CConverterBase* conv2=aList->NewConverterL(uid2);
	TEST(conv2!=NULL);
	conv2->ConvertObjectL(read,write);
	read.Close();
	write.CommitL();
	write.Close();
	delete conv2;
	TEST(return2.Compare(buf2)==0);
	}

void CTB64CnvStep::DoTestL()
	{
	BaflUtils::EnsurePathExistsL(iFs, KLitDirectoryForGeneratedFiles);

	CCnaConverterList* const list=CCnaConverterList::NewLC();
	list->UpdateL();
	INFO_PRINTF2(_L("Created converter list (found %d converters)\n"), list->Count());

	DoTestBase64L(list);
	DoTestQPL(list);
	DoTextEtextL(list);
	// specific cases test code
	DoSpecificL(list);
	DoTextEtextWithControlCharsL(list);
	
	CleanupStack::PopAndDestroy(list);
 	INFO_PRINTF1(_L("Destroyed converter list\n"));
	}


CTB64CnvStep::~CTB64CnvStep()
/**
    Destructor
 */
	{
	}

CTB64CnvStep::CTB64CnvStep()
/**
    Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTB64CnvStep);
	}

TVerdict CTB64CnvStep::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Started"));
	//
	iFs.Connect();
	//
	__UHEAP_MARK;
	INFO_PRINTF1(_L("Testing Base64"));
	TRAPD(r,DoTestL());
	TEST(r==KErrNone);
 
	REComSession::FinalClose(); 
	
	__UHEAP_MARKEND;
	INFO_PRINTF1(_L("Test Finished"));

	return TestStepResult();
	}




