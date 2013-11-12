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
//

#include <e32std.h>
#include <e32test.h>
#include <conlist.h>
#include <conarc.h>
#include <txtrich.h>
#include <s32file.h>
#include <s32mem.h>
#include <ecom/ecom.h>

const TUid KCrt2HTMLConverterUid = {0x1000071c};

static RTest TheTest(_L("Rich text to HTML converter "));

_LIT(KCnvEPOCSmallDocumentDriveC, "C:\\ConvTest");
_LIT(KTestFile, "Z:\\Test\\RT2HtmlConv.html");
_LIT(KHtmlOutput, "C:\\HtmlOutput.html");
_LIT(KHtmlOutputStream, "C:\\HtmlOutputStream.html");

_LIT(KTextToConvert_ParagraphDelimiterTest, "*********Test  Started:Converting rich text paragraphs to HTML *********");
_LIT(KTextToConvert_Line01, "line1 followed by 1 rich text paragraph delimiter ");
_LIT(KTextToConvert_Line02, "line2 followed by 2 rich text paragraphs delimiter");
_LIT(KTextToConvert_Line04, "line3 followed by 3 rich text paragraphs delimiter");
_LIT(KTextToConvert_Line06, "line4");
_LIT(KEndTest, "*********Test completed*********");

_LIT(KLeftAlignText,   "LEFT");
_LIT(KCenterAlignText, "CENTRE");
_LIT(KRightAlignText,  "RIGHT");
_LIT(KFmtdRTCnvTestFile, "C:\\FmtRTConvTestFile");
_LIT(KFmtdRT2HtmlConvOutputFile, "C:\\FmtdRT2HtmlConvOutputFile.html");
_LIT(KFmtdRT2HtmlConvExpectedFile, "Z:\\Test\\FmtdRT2HtmlConvExpectedFile.html");

_LIT(KItem1, "Item 1");
_LIT(KItem2, "Item 2");
_LIT(KItem3, "Item 3");
_LIT(KBulletConvOutputFile1, "C:\\bulletconvexpectedfile1.html");
_LIT(KBulletConvExpectedFile1, "Z:\\Test\\bulletconvexpectedfile1.html");

_LIT(KTranslateConvOutputFile1, "C:\\translateconvoutputfile1.html");
_LIT(KTranslateConvExpectedFile1, "Z:\\Test\\translateconvexpectedfile1.html");

_LIT(KTextStyleConvOutputFile1, "C:\\textstyleconvoutputfile1.html");
_LIT(KTextStyleConvExpectedFile1, "Z:\\Test\\textstyleconvexpectedfile1.html");

/**


Reads data in from a file and stores the information
into an HBuf pointer. If there is an error the function will return an empty buffer

@return				 HBufC8* - containing the data read from the file
@internalComponent
@param				 aFileName - the filename to read from e.g.C:\test.html'
*/
HBufC8* ReadFileLC(const TDesC& aFileName)
{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);

	RFile myFile;
	CleanupClosePushL(myFile);

	TInt err = myFile.Open(fs, aFileName, EFileRead|EFileShareReadersOnly);
	if (err != KErrNone)
		{
		User::Leave(err);	
		}
	TEntry entry;
	fs.Entry(aFileName, entry);
	TInt streamSize = entry.iSize;				// Find the size of the file
	HBufC8* data = HBufC8::NewLC(streamSize);
	TPtr8 ptrData(data->Des());					// Sets a pointer to the HBufC data
	myFile.Read(0, ptrData);
	
	CleanupStack::Pop(data);
	CleanupStack::PopAndDestroy(&myFile);
	CleanupStack::PopAndDestroy(&fs);
	CleanupStack::PushL(data);
	return data; // Return the buffer
}


static CRichText* CreateTestTextL()
	{
	// Variable definitions
	CRichText* richText = NULL;
	CParaFormatLayer* paragraphFormatLayer = NULL;
	CCharFormatLayer* charFormatLayer = NULL;

	// Create Paragraph and Char format layers
	paragraphFormatLayer = CParaFormatLayer::NewL( );
	CleanupStack::PushL( paragraphFormatLayer );
	charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);
	// Create Rich Text object
	richText =  CRichText::NewL(paragraphFormatLayer, charFormatLayer, CEditableText::EFlatStorage);
	CleanupStack::PushL( richText );
	
	// start
	richText->InsertL( richText->DocumentLength(), KTextToConvert_ParagraphDelimiterTest );
	richText->InsertL( richText->DocumentLength(), CEditableText::EParagraphDelimiter );

	richText->InsertL( richText->DocumentLength(), KTextToConvert_Line01 );
	richText->InsertL( richText->DocumentLength(), CEditableText::EParagraphDelimiter );
	richText->InsertL( richText->DocumentLength(), KTextToConvert_Line02 );
	richText->InsertL( richText->DocumentLength(), CEditableText::EParagraphDelimiter );
	richText->InsertL( richText->DocumentLength(), CEditableText::EParagraphDelimiter );
	richText->InsertL( richText->DocumentLength(), KTextToConvert_Line04 );
	richText->InsertL( richText->DocumentLength(), CEditableText::EParagraphDelimiter );
	richText->InsertL( richText->DocumentLength(), CEditableText::EParagraphDelimiter );
	richText->InsertL( richText->DocumentLength(), CEditableText::EParagraphDelimiter );
	richText->InsertL( richText->DocumentLength(), KTextToConvert_Line06 );

	richText->InsertL( richText->DocumentLength(), CEditableText::EParagraphDelimiter );
	richText->InsertL( richText->DocumentLength(), KEndTest );

	CleanupStack::Pop( richText );
	CleanupStack::PopAndDestroy( charFormatLayer );
	CleanupStack::PopAndDestroy( paragraphFormatLayer );
	
	return richText;
	}
	
static void ConstructTestSourceL()
	{
	// Variable definitions
	CDirectFileStore* fileStore = NULL;

	TStreamId streamID;

	CRichText* richText = CreateTestTextL();
	CleanupStack::PushL(richText);
	
	// write this out to a file
	// Grab a file session
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	// Create file store.
	fileStore = CDirectFileStore::ReplaceL(fs, KCnvEPOCSmallDocumentDriveC(), EFileWrite|EFileShareAny);
	CleanupStack::PushL( fileStore );
	fileStore->SetTypeL( KDirectFileStoreLayoutUid );
	streamID = richText->StoreL(*fileStore);
	fileStore->SetRootL(streamID);
	fileStore->CommitL();

	// Start cleanup of stack
	CleanupStack::PopAndDestroy( fileStore );
	CleanupStack::PopAndDestroy(&fs ); 
	CleanupStack::PopAndDestroy( richText );
	}


static CRichText* CreateBulletedAndFormattedTextL()
	{
	// Variable definitions
	CRichText* richText = NULL;
	CParaFormatLayer* paragraphFormatLayer = NULL;
	CCharFormatLayer* charFormatLayer = NULL;

	// Create Paragraph and Char format layers
	CParaFormat paraFormat;
	TParaFormatMask paraFormatMask;
	paragraphFormatLayer = CParaFormatLayer::NewL(&paraFormat,paraFormatMask);
	CleanupStack::PushL( paragraphFormatLayer );
	charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);
	// Create Rich Text object
	richText =  CRichText::NewL(paragraphFormatLayer, charFormatLayer, CEditableText::EFlatStorage);
	CleanupStack::PushL( richText );
	
	// Left Align bulleted text
	paraFormat.iHorizontalAlignment=CParaFormat::ELeftAlign;
	paraFormatMask.SetAttrib(EAttAlignment);
	richText->InsertL( richText->DocumentLength(), CEditableText::EBullet );
	richText->InsertL( richText->DocumentLength(), KLeftAlignText);
	richText->ApplyParaFormatL(&paraFormat, paraFormatMask,richText->DocumentLength(),0);
	richText->InsertL( richText->DocumentLength(), CEditableText::EParagraphDelimiter );
	// Center Align bulleted text
	richText->InsertL( richText->DocumentLength(), CEditableText::EBullet );
	richText->InsertL( richText->DocumentLength(), KCenterAlignText);
	paraFormat.iHorizontalAlignment=CParaFormat::ECenterAlign;
	paraFormatMask.SetAttrib(EAttAlignment);
	richText->ApplyParaFormatL(&paraFormat, paraFormatMask,richText->DocumentLength(),0);
	richText->InsertL( richText->DocumentLength(), CEditableText::EParagraphDelimiter );
	// Right Align bulleted text
	richText->InsertL( richText->DocumentLength(), CEditableText::EBullet );
	richText->InsertL( richText->DocumentLength(), KRightAlignText);
	paraFormat.iHorizontalAlignment=CParaFormat::ERightAlign;
	paraFormatMask.SetAttrib(EAttAlignment);
	richText->ApplyParaFormatL(&paraFormat, paraFormatMask,richText->DocumentLength(),0);

	CleanupStack::Pop( richText );
	CleanupStack::PopAndDestroy( charFormatLayer );
	CleanupStack::PopAndDestroy( paragraphFormatLayer );
	
	return richText;
	}


static void ConstructTestSourceForBulletedAndFormattedTextL()
	{
	// Variable definitions
	CDirectFileStore* fileStore = NULL;

	TStreamId streamID;

	CRichText* richText = CreateBulletedAndFormattedTextL();
	CleanupStack::PushL(richText);
	
	// write this Bulleted and formatted richtext to a file c:\HtmlOutputFile.html
	// Grab a file session
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	// Create file store.
	fileStore = CDirectFileStore::ReplaceL(fs, KFmtdRTCnvTestFile(), EFileWrite|EFileShareAny);
	CleanupStack::PushL( fileStore );
	fileStore->SetTypeL( KDirectFileStoreLayoutUid );
	streamID = richText->StoreL(*fileStore);
	fileStore->SetRootL(streamID);
	fileStore->CommitL();

	// Start cleanup of stack
	CleanupStack::PopAndDestroy( fileStore );
	CleanupStack::PopAndDestroy(&fs ); 
	CleanupStack::PopAndDestroy( richText );
	}

typedef TPckgBuf<CRichText*> TRTPtrBuffer;

static void DoUidAndCapabilitiesTest(CConverterBase& aConv)
	{
	TheTest(aConv.Capabilities() == CConverterBase::EConvertsFiles);
	TheTest(aConv.Uid() == KCrt2HTMLConverterUid);
	}

static void DoStreamingTestL(CConverterBase& aConv)
	{
	CRichText* text = CreateTestTextL();
	CleanupStack::PushL(text);
	TRTPtrBuffer buffer(text);
	RDesReadStream readStream(buffer);
	CleanupClosePushL(readStream);
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	RFileWriteStream writeStream;
	User::LeaveIfError(writeStream.Replace(fs, KHtmlOutputStream(), EFileStream|EFileWrite|EFileShareExclusive));
	CleanupClosePushL(writeStream);
	
	TTime started; started.UniversalTime();
	aConv.ConvertObjectL(readStream, writeStream,NULL);
	HBufC8* htmlOutput = ReadFileLC(KHtmlOutputStream);
	HBufC8* testFile = ReadFileLC(KTestFile);
	TheTest(htmlOutput->Compare(*testFile)==KErrNone);
	TTime finished; finished.UniversalTime();
	TTimeIntervalMicroSeconds interval = finished.MicroSecondsFrom(started);
	TheTest.Printf(_L("\tEPOC => HTML (Streaming) %4dms\n"), interval.Int64());
	
	CleanupStack::PopAndDestroy(testFile);
	CleanupStack::PopAndDestroy(htmlOutput);
	CleanupStack::PopAndDestroy(&writeStream);
	CleanupStack::PopAndDestroy(&fs);
	CleanupStack::PopAndDestroy(&readStream);
	CleanupStack::PopAndDestroy(text);
	}

static void DoFmtdStreamingTestL(CConverterBase& aConv)
	{
	CRichText* text = CreateBulletedAndFormattedTextL();
	CleanupStack::PushL(text);
	TRTPtrBuffer buffer(text);
	RDesReadStream readStream(buffer);
	CleanupClosePushL(readStream);
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	RFileWriteStream writeStream;
	User::LeaveIfError(writeStream.Replace(fs, KFmtdRT2HtmlConvOutputFile(), EFileStream|EFileWrite|EFileShareExclusive));
	CleanupClosePushL(writeStream);
	
	TTime started; started.UniversalTime();
	aConv.ConvertObjectL(readStream, writeStream, NULL);
	HBufC8* htmlOutputFile = ReadFileLC(KFmtdRT2HtmlConvOutputFile);
	HBufC8* htmlExpectedFile = ReadFileLC(KFmtdRT2HtmlConvExpectedFile);
	TheTest(htmlOutputFile->Compare(*htmlExpectedFile)==KErrNone);
	TTime finished; finished.UniversalTime();
	TTimeIntervalMicroSeconds interval = finished.MicroSecondsFrom(started);
	TheTest.Printf(_L("\tEPOC Bulleted and Formatted richtext => HTML (Streaming) %4dms\n"), interval.Int64());
	
	CleanupStack::PopAndDestroy(htmlExpectedFile);
	CleanupStack::PopAndDestroy(htmlOutputFile);
	CleanupStack::PopAndDestroy(&writeStream);
	CleanupStack::PopAndDestroy(&fs);
	CleanupStack::PopAndDestroy(&readStream);
	CleanupStack::PopAndDestroy(text);
	}

static CRichText* CreateBulletedTextL()
	{
	// Variable definitions
	CRichText* richText = NULL;
	CParaFormatLayer* paragraphFormatLayer = NULL;
	CCharFormatLayer* charFormatLayer = NULL;
	
	// Create Paragraph and Char format layers
	CParaFormat paraFormat;
	TParaFormatMask paraFormatMask;
	paragraphFormatLayer = CParaFormatLayer::NewL(&paraFormat,paraFormatMask);
	CleanupStack::PushL( paragraphFormatLayer );
	charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);
	// Create Rich Text object
	richText = CRichText::NewL(paragraphFormatLayer, charFormatLayer, CEditableText::EFlatStorage);
	CleanupStack::PushL( richText );
	
	// Left Align bulleted text
	TBullet* bullet = new(ELeave) TBullet();
	bullet->iCharacterCode=0x2022;
	bullet->iHeightInTwips=300;
	paraFormat.iBullet=bullet;
	paraFormatMask.SetAttrib(EAttBullet);
	
	richText->InsertL( richText->DocumentLength(), KItem1);
	richText->ApplyParaFormatL(&paraFormat, paraFormatMask, richText->DocumentLength(), 0);
	richText->InsertL( richText->DocumentLength(), CEditableText::EParagraphDelimiter );
	
	richText->InsertL( richText->DocumentLength(), KItem2);
	richText->ApplyParaFormatL(&paraFormat, paraFormatMask,richText->DocumentLength(),0);
	richText->InsertL( richText->DocumentLength(), CEditableText::EParagraphDelimiter );
	
	richText->InsertL( richText->DocumentLength(), KItem3);
	richText->ApplyParaFormatL(&paraFormat, paraFormatMask, richText->DocumentLength(),0);
	
	CleanupStack::Pop( richText );
	CleanupStack::PopAndDestroy( charFormatLayer );
	CleanupStack::PopAndDestroy( paragraphFormatLayer );
	
	return richText;
	}

static void DoBulletTestL(CConverterBase& aConv)
	{	
	CRichText* text = CreateBulletedTextL();
	CleanupStack::PushL(text);
	TRTPtrBuffer buffer(text);
	RDesReadStream readStream(buffer);
	CleanupClosePushL(readStream);
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	RFileWriteStream writeStream;
	User::LeaveIfError(writeStream.Replace(fs, KBulletConvOutputFile1(), EFileStream|EFileWrite|EFileShareExclusive));
	CleanupClosePushL(writeStream);
	
	TTime started; started.UniversalTime();
	aConv.ConvertObjectL(readStream, writeStream, NULL);
	HBufC8* outputFile = ReadFileLC(KBulletConvOutputFile1);
	HBufC8* expectedFile = ReadFileLC(KBulletConvExpectedFile1);
	TheTest(outputFile->Compare(*expectedFile)==KErrNone);
	TTime finished; finished.UniversalTime();
	TTimeIntervalMicroSeconds interval = finished.MicroSecondsFrom(started);
	TheTest.Printf(_L("\tEPOC Bulleted richtext => HTML (Streaming) %4dms\n"), interval.Int64());
	
	CleanupStack::PopAndDestroy(expectedFile);
	CleanupStack::PopAndDestroy(outputFile);
	CleanupStack::PopAndDestroy(&writeStream);
	CleanupStack::PopAndDestroy(&fs);
	CleanupStack::PopAndDestroy(&readStream);
	CleanupStack::PopAndDestroy(text);
	}

static CRichText* CreateTranslateTextL()
	{
	// Variable definitions
	CRichText* richText = NULL;
	CParaFormatLayer* paragraphFormatLayer = NULL;
	CCharFormatLayer* charFormatLayer = NULL;
	
	// Create Paragraph and Char format layers
	CParaFormat paraFormat;
	TParaFormatMask paraFormatMask;
	paragraphFormatLayer = CParaFormatLayer::NewL(&paraFormat,paraFormatMask);
	CleanupStack::PushL( paragraphFormatLayer );
	charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);
	// Create Rich Text object
	richText = CRichText::NewL(paragraphFormatLayer, charFormatLayer, CEditableText::EFlatStorage);
	CleanupStack::PushL( richText );
	
	// Left Align bulleted text
	_LIT(KSpecialCharactersString, "<>&");
	richText->InsertL( richText->DocumentLength(), KSpecialCharactersString);
	richText->InsertL( richText->DocumentLength(), CEditableText::EPageBreak);
	richText->InsertL( richText->DocumentLength(), CEditableText::EPotentialHyphen);
	richText->InsertL( richText->DocumentLength(), CEditableText::ELineBreak);
	richText->InsertL( richText->DocumentLength(), CEditableText::ENonBreakingSpace);
	richText->InsertL( richText->DocumentLength(), CEditableText::ETabCharacter);	
	richText->InsertL( richText->DocumentLength(), CEditableText::EParagraphDelimiter );
	
	CleanupStack::Pop( richText );
	CleanupStack::PopAndDestroy( charFormatLayer );
	CleanupStack::PopAndDestroy( paragraphFormatLayer );
	
	return richText;
	}

static void DoTranslateTestL(CConverterBase& aConv)
	{	
	CRichText* text = CreateTranslateTextL();
	CleanupStack::PushL(text);
	TRTPtrBuffer buffer(text);
	RDesReadStream readStream(buffer);
	CleanupClosePushL(readStream);
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	RFileWriteStream writeStream;
	User::LeaveIfError(writeStream.Replace(fs, KTranslateConvOutputFile1(), EFileStream|EFileWrite|EFileShareExclusive));
	CleanupClosePushL(writeStream);
	
	TTime started; started.UniversalTime();
	aConv.ConvertObjectL(readStream, writeStream, NULL);
	HBufC8* outputFile = ReadFileLC(KTranslateConvOutputFile1);
	HBufC8* expectedFile = ReadFileLC(KTranslateConvExpectedFile1);
	TheTest(outputFile->Compare(*expectedFile)==KErrNone);
	TTime finished; finished.UniversalTime();
	TTimeIntervalMicroSeconds interval = finished.MicroSecondsFrom(started);
	TheTest.Printf(_L("\tEPOC Bulleted richtext => HTML (Streaming) %4dms\n"), interval.Int64());
	
	CleanupStack::PopAndDestroy(expectedFile);
	CleanupStack::PopAndDestroy(outputFile);
	CleanupStack::PopAndDestroy(&writeStream);
	CleanupStack::PopAndDestroy(&fs);
	CleanupStack::PopAndDestroy(&readStream);
	CleanupStack::PopAndDestroy(text);
	}

static CRichText* CreateTextStyleTextL()
	{
	// Variable definitions
	CRichText* richText = NULL;
	CParaFormatLayer* paragraphFormatLayer = NULL;
	CCharFormatLayer* charFormatLayer = NULL;
	
	// Create Paragraph and Char format layers
	CParaFormat paraFormat;
	TParaFormatMask paraFormatMask;
	paragraphFormatLayer = CParaFormatLayer::NewL(&paraFormat,paraFormatMask);
	CleanupStack::PushL( paragraphFormatLayer );
	charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);
	// Create Rich Text object
	richText = CRichText::NewL(paragraphFormatLayer, charFormatLayer, CEditableText::EFlatStorage);
	CleanupStack::PushL( richText );
	
	// Left Align bulleted text
	richText->InsertL( richText->DocumentLength(), _L("This bit is italic, this one is bold, and this one is both."));
	TCharFormat format;
	format.iFontSpec.iFontStyle.SetPosture(EPostureItalic);
	TCharFormatMask formatMask;
	formatMask.SetAttrib(EAttFontPosture);
	richText->ApplyCharFormatL(format,formatMask,12,6);
	format.iFontSpec.iFontStyle.SetPosture(EPostureUpright);
	format.iFontSpec.iFontStyle.SetStrokeWeight(EStrokeWeightBold);
	formatMask.SetAttrib(EAttFontStrokeWeight);
	formatMask.SetAttrib(EAttFontPosture);
	richText->ApplyCharFormatL(format,formatMask,32,4);
	format.iFontSpec.iFontStyle.SetPosture(EPostureItalic);
	richText->ApplyCharFormatL(format,formatMask,54,4);
	
	richText->InsertL( richText->DocumentLength(), CEditableText::ELineBreak);
	richText->InsertL( richText->DocumentLength(), _L("This bit is underlined and this one strikethrough."));
	format.iFontPresentation.iUnderline = EUnderlineOn;
	formatMask.ClearAll();
	formatMask.SetAttrib(EAttFontUnderline);
	richText->ApplyCharFormatL(format,formatMask,72,10);
	format.iFontPresentation.iStrikethrough = EStrikethroughOn;
	formatMask.ClearAll();
	formatMask.SetAttrib(EAttFontStrikethrough);
	richText->ApplyCharFormatL(format,formatMask,96,13);
	
	richText->InsertL( richText->DocumentLength(), CEditableText::EParagraphDelimiter );
	
	CleanupStack::Pop( richText );
	CleanupStack::PopAndDestroy( charFormatLayer );
	CleanupStack::PopAndDestroy( paragraphFormatLayer );
	
	return richText;
	}

static void DoTextStyleTestL(CConverterBase& aConv)
	{	
	CRichText* text = CreateTextStyleTextL();
	CleanupStack::PushL(text);
	TRTPtrBuffer buffer(text);
	RDesReadStream readStream(buffer);
	CleanupClosePushL(readStream);
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	RFileWriteStream writeStream;
	User::LeaveIfError(writeStream.Replace(fs, KTextStyleConvOutputFile1(), EFileStream|EFileWrite|EFileShareExclusive));
	CleanupClosePushL(writeStream);
	
	TTime started; started.UniversalTime();
	aConv.ConvertObjectL(readStream, writeStream, NULL);
	HBufC8* outputFile = ReadFileLC(KTextStyleConvOutputFile1);
	HBufC8* expectedFile = ReadFileLC(KTextStyleConvExpectedFile1);
	TheTest(outputFile->Compare(*expectedFile)==KErrNone);
	TTime finished; finished.UniversalTime();
	TTimeIntervalMicroSeconds interval = finished.MicroSecondsFrom(started);
	TheTest.Printf(_L("\tEPOC Bulleted richtext => HTML (Streaming) %4dms\n"), interval.Int64());
	
	CleanupStack::PopAndDestroy(expectedFile);
	CleanupStack::PopAndDestroy(outputFile);
	CleanupStack::PopAndDestroy(&writeStream);
	CleanupStack::PopAndDestroy(&fs);
	CleanupStack::PopAndDestroy(&readStream);
	CleanupStack::PopAndDestroy(text);
	}

static void DoTestsL(CCnaConverterList& aList)
	{
	CConverterBase* conv = NULL;
	ConstructTestSourceL();
	ConstructTestSourceForBulletedAndFormattedTextL();
	//
	// EPOC ==> HTML
	//
	
	TheTest.Next(_L("Converting EPOC rich text to  HTML "));
	conv = aList.NewConverterL(KCrt2HTMLConverterUid);
	TheTest(conv!=NULL);
	CleanupStack::PushL(conv);
	
	TheTest.Next(_L("Testing Uid() and Capabilities() functions"));
	DoUidAndCapabilitiesTest(*conv);
	
	
	TheTest.Next(_L("Converting simple rich text to HTML using the streaming interface"));
	DoStreamingTestL(*conv);
	
	TheTest.Next(_L("Converting formatted rich text to HTML using the streaming interface"));
	DoFmtdStreamingTestL(*conv);
	
	TheTest.Next(_L("Converting simple rich text to HTML using the file-file interface"));
	TTime started; started.UniversalTime();
	conv->ConvertL(KCnvEPOCSmallDocumentDriveC(), KHtmlOutput(),NULL);
	HBufC8* htmlOutput = ReadFileLC(KHtmlOutput);
	HBufC8* testFile = ReadFileLC(KTestFile);
	TheTest(htmlOutput->Compare(*testFile)==KErrNone);
	TTime finished; finished.UniversalTime();
	TTimeIntervalMicroSeconds interval = finished.MicroSecondsFrom(started);
	TheTest.Printf(_L("\tEPOC => HTML (Writing) %4dms\n"), interval.Int64());
	CleanupStack::PopAndDestroy(testFile);
	CleanupStack::PopAndDestroy(htmlOutput);

	//INC042516 - Email, S90_REL_1.1_HK_W02: Wrong format of receipting e-mail 
	// from proto when bullet marks with alignments		
	//
	// EPOC Bulleted and Formatted richtext to HTML Conversion test
	//
	
	
	TheTest.Next(_L("Converting formatted rich text to HTML using the file-file interface"));
	started.UniversalTime();
	conv->ConvertL(KFmtdRTCnvTestFile(), KFmtdRT2HtmlConvOutputFile(),NULL);
	HBufC8* htmlOutputFile = ReadFileLC(KFmtdRT2HtmlConvOutputFile);
	HBufC8* htmlExpectedFile = ReadFileLC(KFmtdRT2HtmlConvExpectedFile);
	TheTest(htmlOutputFile->Compare(*htmlExpectedFile)==KErrNone);
	finished.UniversalTime();
	interval = finished.MicroSecondsFrom(started);
	TheTest.Printf(_L("\tEPOC Bulleted and Formatted richtext => HTML (Writing) %4dms\n"), interval.Int64());
	CleanupStack::PopAndDestroy(htmlExpectedFile);
	CleanupStack::PopAndDestroy(htmlOutputFile);
	
	
	TheTest.Next(_L("Converting bulleted text"));
	DoBulletTestL(*conv);
	
	TheTest.Next(_L("Testing TranslateL function"));
	DoTranslateTestL(*conv);
	
	TheTest.Next(_L("Testing text styles conversion"));
	DoTextStyleTestL(*conv);
			
	CleanupStack::PopAndDestroy(conv);
	}

void doMainL()
	{
	/**
	@SYMTestCaseID PIM-CNVHTMLTEST-0001
	*/	
	TheTest.Start(_L("@SYMTestCaseID PIM-CNVHTMLTEST-0001 Preparing to start..."));
	
	TheTest.Next(_L("Create converter list"));
	CCnaConverterList* convList = CCnaConverterList::NewLC();
	DoTestsL(*convList);
	CleanupStack::PopAndDestroy(); // convList
	REComSession::FinalClose(); //needed, otherwise you will get a memory leak
	TheTest.Printf(_L("\nTests complete\n"));
	TheTest.End();
	TheTest.Close();
	}

GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;

	CTrapCleanup* theCleanup=CTrapCleanup::New();
	TRAPD(ret,doMainL());
	TheTest(ret==KErrNone);
	delete theCleanup;

	__UHEAP_MARKEND;
	return(KErrNone);
	}
