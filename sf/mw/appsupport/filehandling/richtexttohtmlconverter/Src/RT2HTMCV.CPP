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
// ConArc plug-in converter.
// Converts EPOC Rich Text files (i.e. externalised CRichText objects) to HTML files.
// 1. Does not support object->object conversions
// 2. Therefore does not support embedded objects
// 3. No output encoding for UNICODE has beed decided upon
// 
//

#include <e32base.h>
#include <e32def.h>
#include <e32std.h>
#include <s32file.h>
#include <txtuids.h>
#include <txtetext.h>
#include <txtrich.h>
#include <conarc.h>
#include <bautils.h>

#include "RT2HTMCV.H"
#include "HTMLDEFS.H"
#include <ecom/ecom.h>
#include <ecom/implementationproxy.h>


//-------------------------------------------------------------------------------------------------
const TInt  KMaxTagLength		= 256;
const TText KCharacterSpace		= 0x20;
//-------------------------------------------------------------------------------------------------
#ifdef _UNICODE
	HBufC8* ConvertUnicodeToUTF8LC(const TDesC16& uniText);
#endif // _UNICODE


_LIT(KTempFileExt, ".dat");

typedef TPckgBuf<CRichText*> TRTPtrBuffer;

/**
-------------------------------------------------------------------------------------------------
 The required ConArc classes: CCtrToHTMLConverter and CCrtToHTMLLibrary
-------------------------------------------------------------------------------------------------
 Class CCrtToHTMLConverter.
 The CConverterBase derived class which is responsable for performing the conversion operation.
 1. Supports File->File conversions and output to a RWriteStream
 2. For the stream interface a temporary file store is created for reading the rich text object.
    this is deleted once the conversion is complete
 3. The conversion is performed on a paragraph by paragraph basis
 4. iHTMLOut is used to output the resulting HTML. For File->File conversions a new file stream store
    is opened based on the user provided file name. For the streaming interface iHTMLOut is assigned 
    to a RWriteStream& provided by the user
*/
CCrtToHTMLConverter::CCrtToHTMLConverter() : iHTMLOut(iFileStream)
	{ 
	}

void CCrtToHTMLConverter::ConvertL(const TFileName& aSourceFile, const TFileName& aTargetFile, MConverterUiObserver* aObserver /*= NULL*/)
//
// Configures and performs a file -> file conversion synchronously
//
	{
	iObserver = aObserver;
	PrepForConversionL(aSourceFile, aTargetFile);

	while(DoConvertL())
		{
		}
	ASSERT(iParaRemain == 0);
	}

void CCrtToHTMLConverter::ConvertAL(const TFileName& aSourceFile, const TFileName& aTargetFile, MConverterUiObserver* aObserver /*= NULL*/)
//
// Configures the object to perform a file -> file conversion asynchronously
//
	{
	// Cast away the const-ness - for now...
	iObserver = aObserver;
	PrepForConversionL(aSourceFile, aTargetFile);

	if(iObserver)
		{
		iObserver->MaxSteps(iParaRemain, 0);
		}
	}

/**
 To create the RReadStream you package a pointer to the CRichText object and create a RDesReadStream:
 TPckgBuf<CRichText*> buffer(text); //text is the CRichText* to be converted
 RDesReadStream readStream(buffer); //readStream can now be passed to ConvertObjectL/ConvertObjectAL
*/
void CCrtToHTMLConverter::ConvertObjectL(RReadStream& aReadStream, RWriteStream& aWriteStream, MConverterUiObserver* aObserver)
	{
	iObserver = aObserver;
	PrepForConversionL(aReadStream, aWriteStream);

	while(DoConvertL());
	
	ASSERT(iParaRemain == 0);
	}

/**
 To create the RReadStream you package a pointer to the CRichText object and create a RDesReadStream:
 TPckgBuf<CRichText*> buffer(text); //text is the CRichText* to be converted
 RDesReadStream readStream(buffer); //readStream can now be passed to ConvertObjectL/ConvertObjectAL
*/
void CCrtToHTMLConverter::ConvertObjectAL(RReadStream& aReadStream, RWriteStream& aWriteStream, MConverterUiObserver* aObserver)
	{
	iObserver = aObserver;
	PrepForConversionL(aReadStream, aWriteStream);

	if(iObserver)
		{
		iObserver->MaxSteps(iParaRemain, 0);
		}
	}


TUid CCrtToHTMLConverter::Uid()
//
// Returns the UID associated with the conversion operation
//
	{
	return KCrt2HTMLConverterUid;
	}

TInt CCrtToHTMLConverter::Capabilities()
//
// Returns EConvertsFiles. Only File->File conversion is supported.
//
	{
	return EConvertsFiles;
	}

void CCrtToHTMLConverter::CancelConvert()
//
// Clean up. Doesn't remove half finished file.
//
	{
	ResetMembers();
	}

CCrtToHTMLConverter::~CCrtToHTMLConverter()
	{
	ResetMembers();
	iFs.Close();
	}

/**
 Performs a single step of the conversion. Converts a single paragraph from Rich Text -> HTML
 THe final aparagrahp of a RichText doc contains a single end of document marker. When we 
 reach this we can delete the output sink object, which will flush and close the output file.
*/
TBool CCrtToHTMLConverter::DoConvertL()
	{
	TBool closePara = ETrue;

	if(iParaNum == -1)				// We're at the start
		{
		StartDocumentL();			// Write the HTML header

	    iParaNum   = 0;
	    iParaLen   = 0;
		iParaPos   = 0;
		closePara  = EFalse;		// Coz we don't want a </P> tag a the start...
		}	
	
	// Do a single paragraph...
	if((iParaPos = iInputText->CharPosOfParagraph(iParaLen, iParaNum++)) >= 0)
		{
		// Construct a formatting object
		CParaFormat* format = CParaFormat::NewLC();

		TParaFormatMask  mask;

		// Cue, Mr Hack...
		// Knock 1 off the document length as a panic prevention measure, as the iParaLength of
		// the last paragraph is _beyond_ the end of the document. Doh!
		if(iParaNum == iParaCount)
			iParaLen -= 1;

		iInputText->GetParaFormatL(format, mask, iParaPos, iParaLen);

		ASSERT(mask.IsNull());

		ProcessParagraphFormatL(format, closePara);	// Put paragraph level formatting
		ProcessParagraphTextL(iParaPos, iParaLen);	// Process the paragraph itself
		--iParaRemain;

		CleanupStack::PopAndDestroy(); // CParaFormat format
		if(iParaNum == iParaCount)  // We're at the end of the document, so finish up
			{
			ASSERT(iParaPos + iParaLen == iInputText->DocumentLength());
			EndDocumentL();
			// Commit the file
			iHTMLOut.CommitL();
			iHTMLOut.Close();
			return EFalse;
			}
		}
	else
		{
		Panic(EHTMLBeyondLastPara);
		}
	return ETrue;
}

/**
 Both File->File and stream converstions use a file store to read the input CRichText object
 In File->File conversions the user creates the rich text file store prior to calling ConvertL()/ConvertAL()
 In stream conversions CCrtToHTMLConverter creates a temporary file store
*/
void CCrtToHTMLConverter::RestoreInputTextL(const TFileName& aSourceFile)
	{
	delete iInputText;
	iInputText = NULL;
	
	iParaLayer = CParaFormatLayer::NewL();
	iCharLayer = CCharFormatLayer::NewL();
	iInputText = CRichText::NewL(iParaLayer, iCharLayer, CEditableText::EFlatStorage);

	// Get the store and restore from it to the CRichText object
	iStore = CDirectFileStore::OpenL(iFs, aSourceFile, EFileRead | EFileShareReadersOnly);

	if(iStore->Type()[0] != KDirectFileStoreLayoutUid)
          User::Leave(KErrUnknown);     
	
   	iInputText->RestoreL(*iStore, iStore->Root());
	
	iParaRemain = iInputText->ParagraphCount();
	iParaCount  = iParaRemain;
	}

/**
  Configures the object prior to performing a conversion. 
  1. Connects to the file server
  2. Restores the input CRichText object from the given file
  3. Prepares the output context
*/
void CCrtToHTMLConverter::PrepForConversionL(const TFileName& aSourceFile, const TFileName& aTargetFile)
	{
	ResetMembers();
	// ResetMembers() will close the file server session to ensure it's flushed and any temporary files are deleted 
	User::LeaveIfError(iFs.Connect());

	iHTMLOut = iFileStream;
	RestoreInputTextL(aSourceFile);

	// Set up the output context
	User::LeaveIfError(((RFileWriteStream&)iHTMLOut).Replace(iFs, aTargetFile, EFileStream|EFileWrite|EFileShareExclusive));
	}
	
/**
  Configures the object prior to performing a conversion. 
  1. Connects to the file server
  2. Assign the output stream
  3. Copys the read CRichText object to a file store
  4. Restores our copy of the input CRichText

 There's no way to copy a CRichText object without first externalizing it in some way. The normal mechanism for 
 saving and loading CRichText objects is to use Store and Restore. Using Externalize and Internalize is non-trivial
 with CRichText. The stream interface only gives us a RReadStream& making Restore very difficult. The way around 
 this is for the user to just stream a pointer to the actual CRichText object. This way the caller dosn't need to 
 worry about setting up any stores. CCrtToHTMLConverter does the copy.
 To create the RReadStream you package a pointer to the CRichText object and create a RDesReadStream:
 TPckgBuf<CRichText*> buffer(text); //text is the CRichText* to be converted
 RDesReadStream readStream(buffer); //readStream can now be passed to ConvertObjectL/ConvertObjectAL
*/ 
void CCrtToHTMLConverter::PrepForConversionL(RReadStream& aReadStream, RWriteStream& aWriteStream)
	{
	ResetMembers();
	// ResetMembers() will close the file server session to ensure it's flushed and any temporary files are deleted 
	User::LeaveIfError(iFs.Connect());
	
	iHTMLOut = aWriteStream;
	
	// Unpackage the source rich text pointer
	TRTPtrBuffer readBuffer;
	aReadStream.ReadL(readBuffer);
	CRichText* sourceText = readBuffer();

	// The temporary file needs to be unique to allow for concurrent conversions
	// This file is deleted on a call to ResetMembers()
	TFileName newTempFile;
	User::LeaveIfError(iFs.PrivatePath(newTempFile));

	newTempFile.AppendNum((TInt)sourceText);
	newTempFile.Append(KTempFileExt);
	BaflUtils::EnsurePathExistsL(iFs, newTempFile);
	iInternalFile = newTempFile.AllocL();
		
	CDirectFileStore* fileStore = CDirectFileStore::ReplaceL(iFs, *iInternalFile, EFileWrite|EFileShareAny);
	CleanupStack::PushL( fileStore );
	fileStore->SetTypeL( KDirectFileStoreLayoutUid );
	TStreamId streamID = sourceText->StoreL(*fileStore);
	fileStore->SetRootL(streamID);
	fileStore->CommitL();
	CleanupStack::PopAndDestroy( fileStore );
	
	// Restore our copy of the input rich text object from the newly created file store
	RestoreInputTextL(*iInternalFile);
	}

/**
 Ensures that any contained objects are desstroyed and sets all member variable to zero(NULL)
*/
void CCrtToHTMLConverter::ResetMembers()
	{
	iStyleIndex = -1;
	iParaNum    = -1;
	iParaLen    = 0;
	iParaPos    = 0;
	iParaCount  = 0;
	iParaRemain = 0;
	iIndent     = 0;
	iOldFmtCount = 0;

	iHTMLOut.Close();
	delete iInputText;
	iInputText = NULL;
	delete iParaLayer;
	iParaLayer = NULL;
	delete iCharLayer;
	iCharLayer = NULL;
	delete iBullet;
	iBullet = NULL;
	delete iStore;
	iStore = NULL;
	if (iFs.Handle()) 
		{
		if (iInternalFile && BaflUtils::FileExists(iFs,*iInternalFile))
			{
			iFs.Delete(*iInternalFile);
			delete iInternalFile;
			iInternalFile = NULL;			
			}
		// I'm closing the file server to ensure it flushes.
		// Otherwise internal files are not always deleted
		iFs.Close();
		}
	}

/**
 Scans the passed buffer, replacing special characters in the source with relevant HTML tags
 before sending them to be output
*/
void CCrtToHTMLConverter::TranslateL(const TDesC& aBuf)
	{
	int i = 0;
	while(i < aBuf.Length())
		{
		TText ch = aBuf[i++];
		switch(ch)
			{
			// !! am I picking up all possibles here?
			case CEditableText::EPageBreak:
			case CEditableText::EPotentialHyphen:
			case CEditableText::ENonBreakingHyphen:
				break;  // These characters are not emitted.
			case CEditableText::EPictureCharacter:
				break;
			case CEditableText::ELineBreak:
				WriteTagL(KHtmlLineBreak);
				break;
			case CEditableText::ENonBreakingSpace:
			case CEditableText::ETabCharacter:
				WriteContentL(TPtrC(&KCharacterSpace, 1));
				break;
			case CEditableText::EParagraphDelimiter:
				break;
			case KLessThan:
				WriteTagL(KHtmlLessThan);
				break;
			case KGreaterThan:
				WriteTagL(KHtmlGreaterThan);
				break;
			case KAmpersand:
				WriteTagL(KHtmlAmpersand);
				break;
			default:
				WriteContentL(TPtrC(&ch, 1));
				break;
			}
		}
	}

void CCrtToHTMLConverter::WriteTagL(const TDesC8& aTagText)
	{
	iHTMLOut.WriteL(aTagText);
	}

void CCrtToHTMLConverter::WriteContentL(const TDesC& aText)
	{
#ifdef _UNICODE
	HBufC8* pBuf = ConvertUnicodeToUTF8LC(aText);
	User::LeaveIfNull(pBuf);
	iHTMLOut.WriteL(*pBuf);
	CleanupStack::PopAndDestroy();
#else
	iHTMLOut.WriteL(aText);
#endif
	}

void CCrtToHTMLConverter::ProcessParagraphFormatL(const CParaFormat* aFormat, TBool aClosePara)
//
// Processes any paragraph level formatting (paragraph alignment, list bullets, indentation)
//
	{
	// Close indents
	for( ; iIndent ; iIndent--)
		WriteTagL(KHtmlBlockquoteEnd);

	if(!iBullet && aClosePara)
		{
		if ( iInsertBlankDivClose )
			{
			WriteTagL(KHtmlDivBlankEnd);
			}
		else
			{
			WriteTagL(KHtmlDivEnd);
			}
		}

	// Process unordered (bulleted) lists
	if(iBullet)
		{
		// Previous paragraph was bulleted
		if(!aFormat->iBullet)
			{
			// End of list
			delete iBullet;
			iBullet = NULL;
			WriteTagL(KHtmlBulletListPointEnd);
			WriteTagL(KHtmlBulletListEnd);
			}
		else
			{
			if(*iBullet == *(aFormat->iBullet))
				{
				WriteTagL(KHtmlBulletListPointEnd);
				WriteTagL(KHtmlBulletListPointStart);
				}
			else
				{
				// A _new_ list
				WriteTagL(KHtmlBulletListPointEnd);
				WriteTagL(KHtmlBulletListEnd);
				WriteTagL(KHtmlBulletListStart);
				WriteTagL(KHtmlBulletListPointStart);
				*iBullet = *(aFormat->iBullet);
				}
			}
		}
	else
		{
		// Previous paragraph was _not_ bulleted
		if(aFormat->iBullet)
			{
			// But this one is: start a new list
			iBullet = new (ELeave) TBullet();
			*iBullet = *(aFormat->iBullet);
			WriteTagL(KHtmlBulletListStart);
			WriteTagL(KHtmlBulletListPointStart);
			}
		}

	// Process paragraph alignment
	switch(aFormat->iHorizontalAlignment)
		{
	case CParaFormat::ELeftAlign:   // Paragraph aligned flush with left margin
		WriteTagL(KHtmlDivAlignLeftStart);
		break;
	case CParaFormat::ECenterAlign: // Paragraph center aligned
		WriteTagL(KHtmlDivAlignCentreStart);
		break;
	case CParaFormat::ERightAlign:  // Paragraph aligned flush with right margin
		WriteTagL(KHtmlDivAlignRightStart);
		break;
	case CParaFormat::EJustifiedAlign: // Justified text
		WriteTagL(KHtmlDivAlignJustifyStart);
		break;
	default:
		WriteTagL(KHtmlDivAlignNoneStart);
		break;
		}

	// Open indents
    iIndent = (aFormat->iLeftMarginInTwips) / KTwipsToBlockQuote;
	for(TInt i = 0; i < iIndent; i++)
  		WriteTagL(KHtmlBlockquoteStart);
	}

void CCrtToHTMLConverter::ProcessParagraphTextL(TInt aPos, TInt aLength)
//
// Processes a paragraph of text
//
	{
	TInt			pos = aPos;
	TCharFormat		oldFormat,
		            newFormat;

	TCharFormatMask maskChar;
	// Set up initial character formatting
	iInputText->GetSpecificCharFormat(newFormat, maskChar, pos);
	DiffCharFormats(newFormat, oldFormat, maskChar);
	OpenCharFormatL (maskChar, newFormat);
	oldFormat = newFormat;

	// reset blank paragraph flag
	iInsertBlankDivClose = EFalse;
	// Scan the paragraph 1 char at a time...
	while((pos - aPos) < aLength)
		{
		ASSERT(pos < iInputText->DocumentLength());
		TPtrC str = iInputText->Read(pos++, 1);

		if(str[0] == CEditableText::EParagraphDelimiter)
			{
			// only insert a blank div if we have a blank paragraph
			// ie aLength == 1 && it's only contents are CEditableText::EParagraphDelimiter
			iInsertBlankDivClose = ( aLength == 1 );
			continue;
			}

		TCharFormatMask testMask;
		TCharFormat		tstFormat;

		iInputText->GetCharFormat(tstFormat, testMask, pos-1, 2);

		if(!tstFormat.IsEqual(oldFormat))
			{
			// Something has changed...
			DiffCharFormats(tstFormat, oldFormat, maskChar);
			CloseCharFormatL(maskChar, oldFormat);
			OpenCharFormatL (maskChar, tstFormat);
			}
		oldFormat = tstFormat;
		TranslateL(str);
		}
	// End of paragraph, reset formatting to base...
	TCharFormat	closeFormat;
	DiffCharFormats(oldFormat, closeFormat, maskChar);
	CloseCharFormatL(maskChar, oldFormat);
	}

void CCrtToHTMLConverter::DiffCharFormats(const TCharFormat& aFormatA, const TCharFormat& aFormatB, TCharFormatMask& aMask)
//
// Compare two TCharFormat and set flags in the mask which descrbe the differences
// (Would be quite nice if TCharFormat knew how to do this itself...)
//
	{
	aMask.ClearAll();

	if(aFormatA.iLanguage!=aFormatB.iLanguage)
		aMask.SetAttrib(EAttCharLanguage);

	if(aFormatA.iFontSpec.iHeight != aFormatB.iFontSpec.iHeight)
		aMask.SetAttrib(EAttFontHeight);

	if(!(aFormatA.iFontSpec.iTypeface == aFormatB.iFontSpec.iTypeface))
		aMask.SetAttrib(EAttFontTypeface);

	if(aFormatA.iFontSpec.iFontStyle.Posture() != aFormatB.iFontSpec.iFontStyle.Posture())
		aMask.SetAttrib(EAttFontPosture);

	if(aFormatA.iFontSpec.iFontStyle.StrokeWeight() != aFormatB.iFontSpec.iFontStyle.StrokeWeight())
		aMask.SetAttrib(EAttFontStrokeWeight);

	if(aFormatA.iFontSpec.iFontStyle.PrintPosition() != aFormatB.iFontSpec.iFontStyle.PrintPosition())
		aMask.SetAttrib(EAttFontPrintPos);

	if(aFormatA.iFontPresentation.iUnderline != aFormatB.iFontPresentation.iUnderline)
		aMask.SetAttrib(EAttFontUnderline);

	if (aFormatA.iFontPresentation.iStrikethrough != aFormatB.iFontPresentation.iStrikethrough)
		aMask.SetAttrib(EAttFontStrikethrough);

	if(aFormatA.iFontPresentation.iTextColor != aFormatB.iFontPresentation.iTextColor)
		aMask.SetAttrib(EAttColor);

	if(!(aFormatA.iFontSpec.iTypeface == aFormatB.iFontSpec.iTypeface))
		aMask.SetAttrib(EAttFontTypeface);
	}

void CCrtToHTMLConverter::OpenCharFormatL(const TCharFormatMask& aMask, const TCharFormat& aFormat)
//
//	Open the formating tags as set in aMask using parameters gleened from aFormat
//
	{
	if(aMask.IsNull())
		return;
	// Bold text ?
	if(aMask.AttribIsSet(EAttFontStrokeWeight) && (aFormat.iFontSpec.iFontStyle.StrokeWeight() == EStrokeWeightBold))
		WriteTagL(KHtmlBoldStart);
	// Underlined ?
	if(aMask.AttribIsSet(EAttFontUnderline)  && (aFormat.iFontPresentation.iUnderline == EUnderlineOn))
		WriteTagL(KHtmlUnderlineStart);
	// Italic ?
	if(aMask.AttribIsSet(EAttFontPosture) && (aFormat.iFontSpec.iFontStyle.Posture() == EPostureItalic))
		WriteTagL(KHtmlItalicStart);
	// Strike through ?
	if(aMask.AttribIsSet(EAttFontStrikethrough) && aFormat.iFontPresentation.iStrikethrough)
		WriteTagL(KHtmlStrikeoutStart);
	// Sub/Super-script ?
	if(aMask.AttribIsSet(EAttFontPrintPos))
		{
		switch(aFormat.iFontSpec.iFontStyle.PrintPosition())
			{
		case EPrintPosSuperscript:	WriteTagL(KHtmlSuperscriptStart);
			break;
		case EPrintPosSubscript  :	WriteTagL(KHtmlSubscriptStart);
			break;
		case EPrintPosNormal     :	break;
			}
		}
	// Font typeface
	if(aMask.AttribIsSet(EAttFontTypeface) && !aFormat.iFontSpec.iTypeface.IsProportional())
		WriteTagL(KHtmlTeletypeStart);


	//	if both font height and colour are set.
		if(aMask.AttribIsSet(EAttFontHeight) && aMask.AttribIsSet(EAttColor))
		{
			TInt htmlHeight = ((aFormat.iFontSpec.iHeight - KHtmlTwipsToHeightBaseAdjust) / KHtmlTwipsToHeight) + 1;
			TBuf8<KMaxTagLength > tag;
			if(htmlHeight > KHtmlMaxFontSize)
				htmlHeight = KHtmlMaxFontSize;
			tag.Format(KHtmlFontStartClrNSize, htmlHeight, aFormat.iFontPresentation.iTextColor.Red(),
												 aFormat.iFontPresentation.iTextColor.Green(),
												 aFormat.iFontPresentation.iTextColor.Blue());
			WriteTagL(tag);
			//if(!iOldFmtCount)
			iOldFmtCount = 1;
		}
		else
		{
			// Font height
			if(aMask.AttribIsSet(EAttFontHeight))
			{
				TInt htmlHeight = ((aFormat.iFontSpec.iHeight - KHtmlTwipsToHeightBaseAdjust) / KHtmlTwipsToHeight) + 1;
				TBuf8<KMaxTagLength > tag;
				if(htmlHeight > KHtmlMaxFontSize)
					htmlHeight = KHtmlMaxFontSize;
				tag.Format(KHtmlFontSizeStart, htmlHeight);
				WriteTagL(tag);
				//if(!iOldFmtCount)
				iOldFmtCount = 1;

			}
			// Font colour
			if(aMask.AttribIsSet(EAttColor))
			{
				TBuf8<KMaxTagLength > tag;
				tag.Format(KHtmlFontColourStart, aFormat.iFontPresentation.iTextColor.Red(),
												 aFormat.iFontPresentation.iTextColor.Green(),
												 aFormat.iFontPresentation.iTextColor.Blue());
				WriteTagL(tag);
				//if(!iOldFmtCount)
				iOldFmtCount = 1;
			}
		}



	}

void CCrtToHTMLConverter::CloseCharFormatL(const TCharFormatMask& aMask, const TCharFormat& aFormat)
//
// Open the formating tags as set in aMask using parameters gleened from aFormat
// These tags should be checked in the exact _reverse_ order as when they were opened
// to ensure that they nest properly.
//
	{
	if(aMask.IsNull())
		return;

	//Font color or height
	if(iOldFmtCount && (aMask.AttribIsSet(EAttColor)	|| aMask.AttribIsSet(EAttFontHeight)))
	{
		WriteTagL(KHtmlFontEnd);
		iOldFmtCount = 0;
	}

	// Font typeface
if(aMask.AttribIsSet(EAttFontTypeface) && !aFormat.iFontSpec.iTypeface.IsProportional())
		WriteTagL(KHtmlTeletypeEnd);
	// Sub/Super-script ?
	if(aMask.AttribIsSet(EAttFontPrintPos))
		{
		switch(aFormat.iFontSpec.iFontStyle.PrintPosition())
			{
		case EPrintPosSuperscript:	WriteTagL(KHtmlSuperscriptEnd);
			break;
		case EPrintPosSubscript  :	WriteTagL(KHtmlSubscriptEnd);
			break;
		case EPrintPosNormal     :	break;
			}
		}
	// Strike through ?
	if(aMask.AttribIsSet(EAttFontStrikethrough) && aFormat.iFontPresentation.iStrikethrough)
		WriteTagL(KHtmlStrikeoutEnd);
	// Italic ?
	if(aMask.AttribIsSet(EAttFontPosture) && (aFormat.iFontSpec.iFontStyle.Posture() == EPostureItalic))
		WriteTagL(KHtmlItalicEnd);
	// Underlined ?
	if(aMask.AttribIsSet(EAttFontUnderline)  && (aFormat.iFontPresentation.iUnderline == EUnderlineOn))
		WriteTagL(KHtmlUnderlineEnd);
	// Bold text ?
	if(aMask.AttribIsSet(EAttFontStrokeWeight) && (aFormat.iFontSpec.iFontStyle.StrokeWeight() == EStrokeWeightBold))
		WriteTagL(KHtmlBoldEnd);
	}

//-------------------------------------------------------------------------------------------------
// Output functions
//
void CCrtToHTMLConverter::StartDocumentL()
//
// Put the <HTML><HEAD>...</HEAD> tags and open the >BODY> tag.
//
	{
	// Do some validity checks ?				// TTD:	Change this to 4.0 xxxx - or skip it!
	WriteTagL(KHTMLDocType32);				// <!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\">
	WriteTagL(KHtmlStartTag);				// <HTML>
	WriteTagL(KHtmlHeadStartTag); 			// <HEAD>

	WriteTagL(KHTMLHeadTitleStartTag);		// <TITLE>
	TranslateL(_L("Converted from Rich Text"));	//   ...
	WriteTagL(KHTMLHeadTitleEndTag);		// </TITLE>
#ifdef _UNICODE
	WriteTagL(KHTMLMetaCharSetUTF8);		// <META HTTP-EQUIV ="Content-Type" CONTENT = "text/html; charset=UTF-8">
#endif //_UNICODE
	WriteTagL(KHTMLHeadMetaGenTag);  		// <META NAME = "generator" CONTENT = "rt2html converter">
	WriteTagL(KHtmlHeadEndTag);	  		// </HEAD>

	// Get default background colour...
	CParaFormat* formatPara = CParaFormat::NewLC();
	iInputText->GlobalParaFormatLayer()->SenseEffectiveL(formatPara);
	TRgb backColour = formatPara->iFillColor;
	CleanupStack::PopAndDestroy(); // CParaFormat formatPara

	// Get default foreground colour...
	TCharFormat formatChar;
	iInputText->GlobalCharFormatLayer()->SenseEffective(formatChar);
	TRgb foreColour = formatChar.iFontPresentation.iTextColor;

	TBuf8<KMaxTagLength > tag;
	tag.Format(KHtmlBodyStartTag, backColour.Red(),
								  backColour.Green(),
								  backColour.Blue(),
								  foreColour.Red(),
								  foreColour.Green(),
								  foreColour.Blue());

	WriteTagL(tag);
	}


void CCrtToHTMLConverter::EndDocumentL()
//
// Close the document (write the </BODY> and </HTML> tags
//
	{
	if(iOldFmtCount)
		{
		WriteTagL(KHtmlFontEnd);
		}

	if(iBullet)
		{
		// End of list
		delete iBullet;
		iBullet = NULL;
		WriteTagL(KHtmlBulletListPointEnd);
		WriteTagL(KHtmlBulletListEnd);
		}
	else if (iInsertBlankDivClose)
		{
		WriteTagL(KHtmlDivBlankEnd);//&nbsp</DIV>
		}
	else
		{
		WriteTagL(KHtmlDivEnd);//</DIV>
		}
	
	WriteTagL(KHtmlBodyEndTag);	// </BODY>
	WriteTagL(KHtmlEndTag);		// </HTML>
	}

CConverterBase2* CCrtToHTMLConverter::NewL()
	{
	CConverterBase2* crtToHtmlConverter=new (ELeave) CCrtToHTMLConverter();
	return crtToHtmlConverter;
	}

const TImplementationProxy ImplementationTable[] =
    {
	IMPLEMENTATION_PROXY_ENTRY(0x1000071c,CCrtToHTMLConverter::NewL)
    };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }
//-------------------------------------------------------------------------------------------------
// Conversion helper
//-------------------------------------------------------------------------------------------------
#ifdef _UNICODE
#include <utf.h>
// Stole most of this from Phil :-)
HBufC8* ConvertUnicodeToUTF8LC(const TDesC16& uniText)
	{
    // Final UTF8 destination buffer.
	TInt    len     = uniText.Length() * sizeof(TText);
	HBufC8* utfText = HBufC8::NewL(len); // Probably be enough...
    CleanupStack::PushL(utfText);

    // Keep going until there are no unconverted characters left.
    FOREVER
		{
        TPtr8 destination = utfText->Des();
		      destination.FillZ();
        TInt  charsLeft   = CnvUtfConverter::ConvertFromUnicodeToUtf8(destination, uniText);

        if(charsLeft < 0)
			User::Leave(KErrCorrupt);       // Conversion error due to input stream.
        else if(0==charsLeft)
			{
			return utfText;
			}
            else
            {
            // There are characters left to convert due to running out of destination buffer space.
			len += charsLeft * sizeof(TText);
			utfText = utfText->ReAlloc(len);
            }
		}
//    return NULL;  PFD - removed unreachable code as no warnings are acceptable.
	}
#endif // _UNICODE

//-------------------------------------------------------------------------------------------------
// Globals
//-------------------------------------------------------------------------------------------------

GLDEF_C void Panic(TTextToHTMLPanic aPanic)
// Panic the process with HTML converter as the category.
//
	{
	User::Panic(_L("CRT2HTML"),aPanic);
	}


