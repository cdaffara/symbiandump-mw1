// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "CHtmlToCrtConvActionProcessor.h"
#include "CHtmlToCrtConvActionProcessorCustom.h"
#include "CHtmlToCrtConvHash.h"

CHtmlToCrtConvActionProcessor* CHtmlToCrtConvActionProcessor::NewL(CRichText& aRichText, MHtmlToCrtConvResourceFile& aResourceFile)
	{
	CHtmlToCrtConvActionProcessor* self=new(ELeave) CHtmlToCrtConvActionProcessor(aRichText, aResourceFile);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CHtmlToCrtConvActionProcessor::ConstructL()
	{
	iParaFormat=new(ELeave) CParaFormat;
	}

CHtmlToCrtConvActionProcessor::CHtmlToCrtConvActionProcessor(CRichText& aRichText, MHtmlToCrtConvResourceFile& aResourceFile)
:iText(aRichText)
,iResourceFile(aResourceFile)
	{
	}

CHtmlToCrtConvActionProcessor::~CHtmlToCrtConvActionProcessor()
	{
	delete iParaFormat;
	}

//==========================================================================
//utility functions
//==========================================================================
void CHtmlToCrtConvActionProcessor::DoBlockLevelTagActionL()
	{
	InsertParagraphDelimiterL();
	ApplyAlignmentL();
	}

void CHtmlToCrtConvActionProcessor::ApplyAlignmentL()
	{
 	if (IsFlagSet(EAlignAttribPending))
		{
		iText.ApplyParaFormatL(iParaFormat, iParaFormatMask, iAlignmentStartPosition, (iTextPosition-iAlignmentStartPosition));
		iParaFormatMask.ClearAttrib(EAttAlignment);
		ClearFlag(EAlignAttribPending);
		}
	}
void CHtmlToCrtConvActionProcessor::InsertParagraphDelimiterL()
	{
	if(iTextPosition>1)
		{
		//get last 2 significant characters from the richtext object, ie ignore white space characters except for paragraph delimiter
		TBuf<2> lastTwoCharacters;
		TInt richTextPosition=iTextPosition-1;//position of last character in rich text

		while(lastTwoCharacters.Length()<2 && richTextPosition>=0)
			{
			//get character at position richTextPosition from rich text
			TPtrC ptrCurrentCharacter=iText.Read(richTextPosition--, 1);
			TChar currentCharacter=(ptrCurrentCharacter)[0];

			if(!(currentCharacter.IsSpace() && currentCharacter!=CEditableText::EParagraphDelimiter))
				{
				lastTwoCharacters.Append(currentCharacter);
				}
			}

		TBuf<2> twoParagraphDelimiters;
		twoParagraphDelimiters.Append(CEditableText::EParagraphDelimiter);
		twoParagraphDelimiters.Append(CEditableText::EParagraphDelimiter);
		if(lastTwoCharacters.Compare(twoParagraphDelimiters))
			{
			iText.InsertL(iTextPosition, CEditableText::EParagraphDelimiter);
			iTextPosition+=1;
			}
		}
	else
		{
		iText.InsertL(iTextPosition, CEditableText::EParagraphDelimiter);
		iTextPosition+=1;
		}
	}
//==========================================================================
//DoWriteTextL
//==========================================================================
void CHtmlToCrtConvActionProcessor::DoWriteTextL(const TDesC16& aText)
	{
	if (!IsFlagSet(EIgnoreText))
		{
 		HBufC* temp=HBufC::NewLC(aText.Length());
		TPtr tempPtr(temp->Des());
 		tempPtr.Copy(aText);

		//trim buffer, leaving one space at start and end of the buffer
		TrimBufferL(tempPtr);

 		iText.InsertL(iTextPosition, *temp);
 		CleanupStack::PopAndDestroy(temp);
		iTextPosition=iText.DocumentLength();

		//CancelInsertCharFormat cancels the "insert pending" state set by a call to SetInsertCharFormatL() or DelSetInsertCharFormatL().
		//This removes the restriction on the text insertion position, and has no effect if not applicable.
		iText.CancelInsertCharFormat();
		}
	}
//==========================================================================
//TrimBufferL
//==========================================================================
void CHtmlToCrtConvActionProcessor::TrimBufferL(TPtr& aPtr)
	{
	TInt len=aPtr.Length();
	if	(!len)
		return;

	// Current document length - required so that we don't insert
	// any spaces until we come across a first run of text.
	const TInt documentLength=iText.DocumentLength();
	//check if first and last characters are white space characters
	TBool spaceAtStart=EFalse;
	TBool spaceAtEnd=EFalse;
	
	TChar left=(aPtr)[0];
	TChar right=(aPtr)[len-1];
	if(documentLength>0 && left.IsSpace())
		{
		spaceAtStart=ETrue;
		//if length is 1 we are seeing the same space twice,
		//if length is 2 then only one space is required
		if(len>2 && right.IsSpace())
			{
			spaceAtEnd=ETrue;
			}
		}
	else if(right.IsSpace())
		{
		spaceAtEnd=ETrue;
		}

	//remove leading and trailing space characters, and replace each contiguous set of space characters by one space character
	aPtr.TrimAll();

	//add space character at start/end of buffer, if required
	_LIT(KSpace, " ");
	if(spaceAtStart)
		{
		aPtr.Insert(0, KSpace);
		}
	if(spaceAtEnd)
		{
		aPtr.Append(KSpace);
		}

	// Convert any remaining carriage returns or line feeds to spaces
	const TText KSpaceCharacter=0x20;
	const TText KCarriageReturn=0x0d;
	const TText KLineFeed=0x0a;

	TText* pText=const_cast<TText*>(aPtr.Ptr());
	for(TInt ii=0; ii<len; ii++)
		{
		if	(pText[ii]==KCarriageReturn || pText[ii]==KLineFeed)
			pText[ii]=KSpaceCharacter;
		}
	}
//==========================================================================
//SetFontStrokeWeightL
//==========================================================================
void CHtmlToCrtConvActionProcessor::SetFontStrokeWeightL(TFontStrokeWeight aStrokeWeight)
	{
	if(!(iCharFormat.iFontSpec.iFontStyle.StrokeWeight()==aStrokeWeight))
		{
		iCharFormatMask.SetAttrib(EAttFontStrokeWeight);
		iCharFormat.iFontSpec.iFontStyle.SetStrokeWeight(aStrokeWeight);
		iText.SetInsertCharFormatL(iCharFormat, iCharFormatMask, iTextPosition);
		}
	}
//==========================================================================
//SetFontPostureL
//==========================================================================
void CHtmlToCrtConvActionProcessor::SetFontPostureL(TFontPosture aFontPosture)
	{
	if(!(iCharFormat.iFontSpec.iFontStyle.Posture()==aFontPosture))
		{
		iCharFormatMask.SetAttrib(EAttFontPosture);
		iCharFormat.iFontSpec.iFontStyle.SetPosture(aFontPosture);
		iText.SetInsertCharFormatL(iCharFormat, iCharFormatMask, iTextPosition);
		}
	}
//==========================================================================
//SetFontUnderlineL
//==========================================================================
void CHtmlToCrtConvActionProcessor::SetFontUnderlineL(TFontUnderline aFontUnderline)
	{
	if(!(iCharFormat.iFontPresentation.iUnderline==aFontUnderline))
		{
		iCharFormatMask.SetAttrib(EAttFontUnderline);
		iCharFormat.iFontPresentation.iUnderline=aFontUnderline;
		iText.SetInsertCharFormatL(iCharFormat, iCharFormatMask, iTextPosition);
		}
	}
//==========================================================================
//SetFontPrintPositionL
//==========================================================================
void CHtmlToCrtConvActionProcessor::SetFontPrintPositionL(TFontPrintPosition aFontPrintPosition)
	{
	if(!(iCharFormat.iFontSpec.iFontStyle.PrintPosition()==aFontPrintPosition))
		{
		iCharFormatMask.SetAttrib(EAttFontPrintPos);
		iCharFormat.iFontSpec.iFontStyle.SetPrintPosition(aFontPrintPosition);
		iText.SetInsertCharFormatL(iCharFormat, iCharFormatMask, iTextPosition);
		}
	}
//==========================================================================
//SetFontStrikethroughL
//==========================================================================
void CHtmlToCrtConvActionProcessor::SetFontStrikethroughL(TFontStrikethrough aFontStrikethrough)
	{
	if(!(iCharFormat.iFontPresentation.iStrikethrough==aFontStrikethrough))
		{
		iCharFormatMask.SetAttrib(EAttFontStrikethrough);
		iCharFormat.iFontPresentation.iStrikethrough=aFontStrikethrough;
		iText.SetInsertCharFormatL(iCharFormat, iCharFormatMask, iTextPosition);
		}
	}
//==========================================================================
//SetFontTextColorL
//==========================================================================
void CHtmlToCrtConvActionProcessor::SetFontTextColorL(const TRgb& aFontTextColor)
	{
	if(!(iCharFormat.iFontPresentation.iTextColor==aFontTextColor))
		{
		iCharFormatMask.SetAttrib(EAttColor);
		iCharFormat.iFontPresentation.iTextColor=aFontTextColor;
		iText.SetInsertCharFormatL(iCharFormat, iCharFormatMask, iTextPosition);
		}
	}
//==========================================================================
//SetFontSizeL
//==========================================================================
void CHtmlToCrtConvActionProcessor::SetFontSizeL(TInt aFontSizeInTwips)
	{
	if(!(iCharFormat.iFontSpec.iHeight==aFontSizeInTwips))
		{
		iCharFormatMask.SetAttrib(EAttFontHeight);
		iCharFormat.iFontSpec.iHeight=aFontSizeInTwips;
		iText.SetInsertCharFormatL(iCharFormat, iCharFormatMask, iTextPosition);
		}
	}
//==========================================================================
//DoTagActionL
//==========================================================================

void CHtmlToCrtConvActionProcessor::DoTagActionL(THtmlToCrtConvTagType aTagType, CHtmlToCrtConvParser::TTagState aTagState)
	{
	ASSERT(aTagState!=CHtmlToCrtConvParser::ENoTag);

	switch(aTagType)
		{
//BLOCK LEVEL ELEMENTS
		case EHtmlTagH1:
			DoHeaderL(aTagState, KH1SizeInTwips);
			break;
		case EHtmlTagH2:
			DoHeaderL(aTagState, KH2SizeInTwips);
			break;
		case EHtmlTagH3:
			DoHeaderL(aTagState, KH3SizeInTwips);
			break;
		case EHtmlTagH4:
			DoHeaderL(aTagState, KH4SizeInTwips);
			break;
		case EHtmlTagH5:
			DoHeaderL(aTagState, KH5SizeInTwips);
			break;
		case EHtmlTagH6:
			DoHeaderL(aTagState, KH6SizeInTwips);
			break;
		case EHtmlTagCenter:
			DoCenterL(aTagState);
			break;
		case EHtmlTagBlockquote:
			DoBlockquoteL(aTagState);
			break;
		case EHtmlTagForm:
			DoFormL(aTagState);
			break;
		case EHtmlTagUnorderedList:
			DoUnorderedListL(aTagState);
			break;
		case EHtmlTagOrderedList:
			DoUnorderedListL(aTagState);
			break;
		case EHtmlTagDefinitionList:
			DoUnorderedListL(aTagState);
			break;
		case EHtmlTagParagraph:
			DoParagraphL();
			break;
		case EHtmlTagTable:
			DoTableL();
			break;
		case EHtmlTagDivision:
			DoDivisionL();
			break;
		case EHtmlTagPreformatted:
			DoPreformattedL();
			break;
		case EHtmlTagHorizontalRule:
			DoHorizontalRuleL();
			break;
		case EHtmlTagHtml:
			DoHtmlL(aTagState);
			break;
//TEXT LEVEL ELEMENTS
		case EHtmlTagTitle:
			DoTitleL(aTagState);
			break;
		case EHtmlTagItalic:
			DoItalicL(aTagState);
			break;
		case EHtmlTagBold:
			DoBoldL(aTagState);
			break;
		case EHtmlTagUnderline:
			DoUnderlineL(aTagState);
			break;
		case EHtmlTagEmphasis:
			DoItalicL(aTagState);
			break;
		case EHtmlTagStrong:
			DoBoldL(aTagState);
			break;
		case EHtmlTagDefinition:
			DoItalicL(aTagState);
			break;
		case EHtmlTagVariable:
			DoItalicL(aTagState);
			break;
		case EHtmlTagCitation:
			DoItalicL(aTagState);
			break;
		case EHtmlTagSubscript:
			DoSubscriptL(aTagState);
			break;
		case EHtmlTagSuperscript:
			DoSuperscriptL(aTagState);
			break;
		case EHtmlTagStrikethrough:
			DoStrikethroughL(aTagState);
			break;
		case EHtmlTagBreak:
			DoBreakL(aTagState);
			break;
		case EHtmlTagCaption:
			DoCaptionL(aTagState);
			break;
		case EHtmlTagTableData:
			DoTableDataL(aTagState);
			break;
		case EHtmlTagTableHeader:
			DoTableDataL(aTagState);
			break;
		case EHtmlTagListItem:
			DoListItemL(aTagState);
			break;
		case EHtmlTagDefinitionTerm:
			DoListItemL(aTagState);
			break;
		case EHtmlTagDefinitionDefinition:
			DoListItemL(aTagState);
			break;
		default:
			break;
		}
	}

//=================================================
//DoAttributeActionL
//=================================================

void CHtmlToCrtConvActionProcessor::DoAttributeActionL(THtmlToCrtConvTagType aTagType, CHtmlToCrtConvParser::TTagState aTagState, const THtmlToCrtConvAttributeType& aType, const TDesC8& aValue, TBool& aImgTagResourceReqd)
	{
	switch(aType)
		{
		case EHtmlAttributeAlign:
			DoAlignL(aValue, aTagState);
			break;
		case EHtmlAttributeHref:
			if (aTagType==EHtmlTagAnchor)
				{
				DoShowUrlL(aValue, aTagState, aTagType);
				}
			break;
		case EHtmlAttributeSrc:
			if (aTagType==EHtmlTagImage)
				{
				DoShowUrlL(aValue, aTagState, aTagType);
				if(IsFlagSet(ESeenAltAttribute))
					{
					aImgTagResourceReqd=EFalse;
					ClearFlag(ESeenAltAttribute);
					}
				else
					aImgTagResourceReqd=ETrue;
				}
			break;
		case EHtmlAttributeAlt:
			if (aTagType==EHtmlTagImage)
				{
				DoAltL(aValue, aTagState);
				aImgTagResourceReqd=EFalse;
				SetFlag(ESeenAltAttribute);
				}
			break;
		default:
			break;
		}
	}

//=======================================================================
//=======================================================================
// TAGS - implementation of
//
// DoHeaderL, DoTitleL, DoShowUrlL, DoAltL
//
// in CHtmlToCrtConvActionProcessorCustom.cpp
//=======================================================================
//
//EHtmlTagCenter
//
void CHtmlToCrtConvActionProcessor::DoCenterL(CHtmlToCrtConvParser::TTagState aTagState)
	{
	DoBlockLevelTagActionL();

	if (aTagState==CHtmlToCrtConvParser::EOpeningTag)
		{
		iParaFormatMask.SetAttrib(EAttAlignment);
		iParaFormat->iHorizontalAlignment=CParaFormat::ECenterAlign;
		iAlignmentStartPosition=iTextPosition;
		SetFlag(EAlignAttribPending);
		}
	}
//
//EHtmlTagBlockquote
//
void CHtmlToCrtConvActionProcessor::DoBlockquoteL(CHtmlToCrtConvParser::TTagState aTagState)
	{
	DoBlockLevelTagActionL();

	if (aTagState==CHtmlToCrtConvParser::EOpeningTag)
		{
		iParaFormatMask.SetAttrib(EAttLeftMargin);
		iParaFormat->iLeftMarginInTwips=KBlockquoteIndentSizeInTwips;
		iBlockquoteStartPosition=iTextPosition;
		SetFlag(EBlockquotePending);
		}
	else if (aTagState==CHtmlToCrtConvParser::EClosingTag && IsFlagSet(EBlockquotePending))
		{
		iText.ApplyParaFormatL(iParaFormat, iParaFormatMask, iBlockquoteStartPosition, (iTextPosition-iBlockquoteStartPosition));
		iParaFormatMask.ClearAttrib(EAttLeftMargin);
		ClearFlag(EBlockquotePending);
		}
	}
//
//EHtmlTagForm
//
void CHtmlToCrtConvActionProcessor::DoFormL(CHtmlToCrtConvParser::TTagState aTagState)
	{
	DoBlockLevelTagActionL();

	if (aTagState==CHtmlToCrtConvParser::EOpeningTag)
		{
		SetFlag(EIgnoreText);
		}
	else if (aTagState==CHtmlToCrtConvParser::EClosingTag)
		{
		ClearFlag(EIgnoreText);
		}
	}
//
//EHtmlTagUnorderedList
//
void CHtmlToCrtConvActionProcessor::DoUnorderedListL(CHtmlToCrtConvParser::TTagState aTagState)
	{
	DoBlockLevelTagActionL();

	if (aTagState==CHtmlToCrtConvParser::EClosingTag && IsFlagSet(EListPending))
		{
		iText.ApplyParaFormatL(iParaFormat, iParaFormatMask, iListStartPosition, (iTextPosition-iListStartPosition));
		iParaFormatMask.ClearAttrib(EAttBullet);
		ClearFlag(EListPending);
		}
	}
//
//EHtmlTagParagraph
//
void CHtmlToCrtConvActionProcessor::DoParagraphL()
	{
	DoBlockLevelTagActionL();
	}
//
//EHtmlTagTable
//
void CHtmlToCrtConvActionProcessor::DoTableL()
	{
	DoBlockLevelTagActionL();
	}
//
//EHtmlTagDivision
//
void CHtmlToCrtConvActionProcessor::DoDivisionL()
	{
	DoBlockLevelTagActionL();
	}
//
//EHtmlTagPreformatted
//
void CHtmlToCrtConvActionProcessor::DoPreformattedL()
	{
	DoBlockLevelTagActionL();
	}
//
//EHtmlTagHorizontalRule
//
void CHtmlToCrtConvActionProcessor::DoHorizontalRuleL()
	{
	DoBlockLevelTagActionL();
	}
//
//EHtmlTagHtml - actions any outstanding formatting at the end of the file
//
void CHtmlToCrtConvActionProcessor::DoHtmlL(CHtmlToCrtConvParser::TTagState aTagState)
	{
	if (aTagState==CHtmlToCrtConvParser::EClosingTag && IsFlagSet(EAlignAttribPending))
		{
		DoBlockLevelTagActionL();
		}
	}
//
//EHtmlTagItalic
//
void CHtmlToCrtConvActionProcessor::DoItalicL(CHtmlToCrtConvParser::TTagState aTagState)
	{
	if (aTagState==CHtmlToCrtConvParser::EOpeningTag)
		{
		SetFontPostureL(EPostureItalic);
		}
	else if (aTagState==CHtmlToCrtConvParser::EClosingTag)
		{
		SetFontPostureL(EPostureUpright);
		}
	}
//
//EHtmlTagBold
//
void CHtmlToCrtConvActionProcessor::DoBoldL(CHtmlToCrtConvParser::TTagState aTagState)
	{
	if (aTagState==CHtmlToCrtConvParser::EOpeningTag)
		{
		SetFontStrokeWeightL(EStrokeWeightBold);
		}
	else if (aTagState==CHtmlToCrtConvParser::EClosingTag)
		{
		SetFontStrokeWeightL(EStrokeWeightNormal);
		}
	}
//
//EHtmlTagUnderline
//
void CHtmlToCrtConvActionProcessor::DoUnderlineL(CHtmlToCrtConvParser::TTagState aTagState)
	{
	if (aTagState==CHtmlToCrtConvParser::EOpeningTag)
		{
		SetFontUnderlineL(EUnderlineOn);
		}
	else if (aTagState==CHtmlToCrtConvParser::EClosingTag)
		{
		SetFontUnderlineL(EUnderlineOff);
		}
	}
//
//EHtmlTagSubscript
//
void CHtmlToCrtConvActionProcessor::DoSubscriptL(CHtmlToCrtConvParser::TTagState aTagState)
	{
	if (aTagState==CHtmlToCrtConvParser::EOpeningTag)
		{
		SetFontPrintPositionL(EPrintPosSubscript);
		}
	else if (aTagState==CHtmlToCrtConvParser::EClosingTag)
		{
		SetFontPrintPositionL(EPrintPosNormal);
		}
	}
//
//EHtmlTagSuperscript
//
void CHtmlToCrtConvActionProcessor::DoSuperscriptL(CHtmlToCrtConvParser::TTagState aTagState)
	{
	if (aTagState==CHtmlToCrtConvParser::EOpeningTag)
		{
		SetFontPrintPositionL(EPrintPosSuperscript);
		}
	else if (aTagState==CHtmlToCrtConvParser::EClosingTag)
		{
		SetFontPrintPositionL(EPrintPosNormal);
		}
	}
//
//EHtmlTagStrikethrough
//
void CHtmlToCrtConvActionProcessor::DoStrikethroughL(CHtmlToCrtConvParser::TTagState aTagState)
	{
	if (aTagState==CHtmlToCrtConvParser::EOpeningTag)
		{
		SetFontStrikethroughL(EStrikethroughOn);
		}
	else if (aTagState==CHtmlToCrtConvParser::EClosingTag)
		{
		SetFontStrikethroughL(EStrikethroughOff);
		}
	}
//
//EHtmlTagBreak
//
void CHtmlToCrtConvActionProcessor::DoBreakL(CHtmlToCrtConvParser::TTagState aTagState)
	{
	if (aTagState==CHtmlToCrtConvParser::EOpeningTag)
		{
		iText.InsertL(iTextPosition, CEditableText::ELineBreak);
		iTextPosition += 1;
		}
	}
//
//EHtmlTagCaption
//
void CHtmlToCrtConvActionProcessor::DoCaptionL(CHtmlToCrtConvParser::TTagState aTagState)
	{
	if (aTagState==CHtmlToCrtConvParser::EOpeningTag)
		InsertParagraphDelimiterL();
	}
//
//EHtmlTagTableData
//
void CHtmlToCrtConvActionProcessor::DoTableDataL(CHtmlToCrtConvParser::TTagState aTagState)
	{
	if (aTagState==CHtmlToCrtConvParser::EOpeningTag)
		InsertParagraphDelimiterL();
	}
//
//EHtmlTagListItem
//
void CHtmlToCrtConvActionProcessor::DoListItemL(CHtmlToCrtConvParser::TTagState aTagState)
	{
	if (aTagState==CHtmlToCrtConvParser::EOpeningTag)
		{
		InsertParagraphDelimiterL();

		if (!IsFlagSet(EListPending))
			{
			iParaFormatMask.SetAttrib(EAttBullet);
			if (!(iParaFormat->iBullet))
				{
				TBullet* bullet=new(ELeave) TBullet;
				bullet->iCharacterCode=KBulletCharacterCode;
				bullet->iHeightInTwips=KBulletHeightInTwips;
				iParaFormat->iBullet=bullet;
				}
			iListStartPosition=iTextPosition;
			SetFlag(EListPending);
			}
		}
	}

//=================================================================================
// ATTRIBUTES
//=================================================================================
//
//EHtmlAttributeAlign
//
void CHtmlToCrtConvActionProcessor::DoAlignL(const TDesC8& aValue, CHtmlToCrtConvParser::TTagState aTagState)
	{
	_LIT8(KCenter,"CENTER");
	_LIT8(KRight,"RIGHT");

	if (aTagState==CHtmlToCrtConvParser::EOpeningTag)
		{
		if (!(aValue.CompareF(KCenter)))
			{
			iParaFormatMask.SetAttrib(EAttAlignment);
			iParaFormat->iHorizontalAlignment=CParaFormat::ECenterAlign;
			iAlignmentStartPosition=iTextPosition;
			SetFlag(EAlignAttribPending);
			}
		else if (!(aValue.CompareF(KRight)))
			{
			iParaFormatMask.SetAttrib(EAttAlignment);
			iParaFormat->iHorizontalAlignment=CParaFormat::ERightAlign;
			iAlignmentStartPosition=iTextPosition;
			SetFlag(EAlignAttribPending);
			}
		}
	}
