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

#ifndef __CHTMLTOCRTCONVACTIONPROCESSOR_H__
#define __CHTMLTOCRTCONVACTIONPROCESSOR_H__

#include <e32std.h>
#include <e32base.h>
#include <f32file.h>
#include <txtrich.h> 
#include <txtfrmat.h>

#define KDefaultTextColor KRgbBlack

#include "CHtmlToCrtConvParser.h"
#include "MHtmlToCrtConvResourceFile.h"

enum THtmlToCrtConvTagType;
enum THtmlToCrtConvAttributeType;

class CHtmlToCrtConvActionProcessor : public CBase
	{
	enum TParserFlags
		{
		ENull = 0x00,
		EAlignAttribPending = 0x01,
		EBlockquotePending = 0x02,
		EListPending = 0x04,
		EIgnoreText = 0x08,
		ESeenAltAttribute = 0x10
		};

	public:
		static CHtmlToCrtConvActionProcessor* NewL(CRichText& aRichText, MHtmlToCrtConvResourceFile& aResourceFile);
		~CHtmlToCrtConvActionProcessor();

		void DoTagActionL(THtmlToCrtConvTagType aTagType, CHtmlToCrtConvParser::TTagState aTagState);
		void DoAttributeActionL(THtmlToCrtConvTagType aTagType, CHtmlToCrtConvParser::TTagState aTagState, const THtmlToCrtConvAttributeType& aType, const TDesC8& aValue, TBool& aImgTagResourceReqd);
	/**
		@fn				void DoWriteTextL(const TDesC16& aText)
		Intended Usage	: inserts text at the end of the CRichText object
		@param			aText The text to be inserted
		@return			void
	 */
		void DoWriteTextL(const TDesC16& aText);
		void InsertImgTagResourceL();

	private:
		CHtmlToCrtConvActionProcessor(CRichText& aRichText, MHtmlToCrtConvResourceFile& aResourceFile);
		void ConstructL();

		void TrimBufferL(TPtr& aPtr);
	/**
		@fn				void DoBlockLevelTagActionL()
		Intended Usage	: inserts a paragraph delimiter and applies alignment to the CRichText object.
						  Should be called for each block level HTML tag
		@return			void
	 */
		void DoBlockLevelTagActionL();
		void ApplyAlignmentL();
		void InsertParagraphDelimiterL();
		void SetFontStrokeWeightL(TFontStrokeWeight aStrokeWeight);
		void SetFontPostureL(TFontPosture aFontPosture);
		void SetFontUnderlineL(TFontUnderline aFontUnderline);
		void SetFontPrintPositionL(TFontPrintPosition aFontPrintPosition);
		void SetFontStrikethroughL(TFontStrikethrough aFontStrikethrough);
		void SetFontTextColorL(const TRgb& aFontTextColor);
		void SetFontSizeL(TInt aFontSizeInTwips);
//
//tags
//
	/**
		@fn				void DoHeaderL(CHtmlToCrtConvParser::TTagState aTagState, TInt aFontSizeInTwips)
		Intended Usage	: sets font stroke weight and font size in the CRichText object
		@param			aTagState  whether the HTML tag is an opening or closing tag
		@param			aFontSizeInTwips  the font size in twips
		@return			void
	 */
		void DoHeaderL(CHtmlToCrtConvParser::TTagState aTagState, TInt aFontSizeInTwips);	
		void DoCenterL(CHtmlToCrtConvParser::TTagState aTagState);
		void DoBlockquoteL(CHtmlToCrtConvParser::TTagState aTagState);
	/**
		@fn				void DoFormL(CHtmlToCrtConvParser::TTagState aTagState)
		Intended Usage	: updates the flag EIgnoreText
		@param			aTagState  whether the HTML tag is an opening or closing tag
		@return			void
	 */
		void DoFormL(CHtmlToCrtConvParser::TTagState aTagState);
	/**
		@fn				void DoUnorderedListL(CHtmlToCrtConvParser::TTagState aTagState)
		Intended Usage	: applies bulleted list formatting to the CRichText object
		@param			aTagState  whether the HTML tag is an opening or closing tag
		@return			void
	 */
		void DoUnorderedListL(CHtmlToCrtConvParser::TTagState aTagState);
		void DoParagraphL();
		void DoTableL();
		void DoDivisionL();
		void DoPreformattedL();
		void DoHorizontalRuleL();
		void DoItalicL(CHtmlToCrtConvParser::TTagState aTagState);
		void DoBoldL(CHtmlToCrtConvParser::TTagState aTagState);
		void DoUnderlineL(CHtmlToCrtConvParser::TTagState aTagState);
		void DoSubscriptL(CHtmlToCrtConvParser::TTagState aTagState);
		void DoSuperscriptL(CHtmlToCrtConvParser::TTagState aTagState);
		void DoStrikethroughL(CHtmlToCrtConvParser::TTagState aTagState);
		void DoBreakL(CHtmlToCrtConvParser::TTagState aTagState);
	/**
		@fn				void DoTitleL(CHtmlToCrtConvParser::TTagState aTagState)
		Intended Usage	: renders title text as a header (H1) and inserts a paragraph delimiter
		@param			aTagState  whether the HTML tag is an opening or closing tag
		@return			void
	 */
		void DoTitleL(CHtmlToCrtConvParser::TTagState aTagState);
	/**
		@fn				void DoCaptionL(CHtmlToCrtConvParser::TTagState aTagState)
		Intended Usage	: inserts a paragraph delimiter at the end of the CRichText object
		@param			aTagState  whether the HTML tag is an opening or closing tag
		@return			void
	 */
		void DoCaptionL(CHtmlToCrtConvParser::TTagState aTagState);
	/**
		@fn				void DoTableDataL(CHtmlToCrtConvParser::TTagState aTagState)
		Intended Usage	: inserts a paragraph delimiter at the end of the CRichText object
		@param			aTagState  whether the HTML tag is an opening or closing tag
		@return			void
	 */
		void DoTableDataL(CHtmlToCrtConvParser::TTagState aTagState);
	/**
		@fn				void DoListItemL(CHtmlToCrtConvParser::TTagState aTagState)
		Intended Usage	: updates the flag EListPending
		@param			aTagState  whether the HTML tag is an opening or closing tag
		@return			void
	 */
		void DoListItemL(CHtmlToCrtConvParser::TTagState aTagState);
		void DoHtmlL(CHtmlToCrtConvParser::TTagState aTagState);

		//attributes
		void DoAlignL(const TDesC8& aValue, CHtmlToCrtConvParser::TTagState aTagState);
	/**
		@fn				void DoShowUrlL(const TDesC8& aValue, CHtmlToCrtConvParser::TTagState aTagState)
		Intended Usage	: inserts " ( aValue )" at the end of the CRichText object
		@param			aValue     the value of the Href or Src HTML tag attribute
		@param			aTagState  whether the HTML tag is an opening or closing tag
		@return			void
	 */
		void DoShowUrlL(const TDesC8& aValue, CHtmlToCrtConvParser::TTagState aTagState, THtmlToCrtConvTagType aTagType);
	/**
		@fn				void DoAltL(const TDesC8& aValue, CHtmlToCrtConvParser::TTagState aTagState)
		Intended Usage	: inserts "<aValue>" at the end of the CRichText object
		@param			aValue     the value of the Alt HTML tag attribute
		@param			aTagState  whether the HTML tag is an opening or closing tag
		@param			aTagType  the type of HTML tag
		@return			void
	 */
		void DoAltL(const TDesC8& aValue, CHtmlToCrtConvParser::TTagState aTagState);

		//flags
		inline void SetFlag (TParserFlags aFlag);
		inline TBool IsFlagSet (TParserFlags aFlag) const;
		inline void ClearFlag (TParserFlags aFlag);

	private:
		CRichText& iText;//reference to rich text object owned by converter
		TInt iTextPosition;
		TInt iAlignmentStartPosition;
		TInt iBlockquoteStartPosition;
		TInt iListStartPosition;
		TUint iFlags;

		TCharFormatMask iCharFormatMask;
		TCharFormat iCharFormat;
		TParaFormatMask iParaFormatMask;
		CParaFormat* iParaFormat;
		MHtmlToCrtConvResourceFile& iResourceFile;
	};

void CHtmlToCrtConvActionProcessor::SetFlag (TParserFlags aFlag)
	{
	iFlags |= aFlag;
	}

TBool CHtmlToCrtConvActionProcessor::IsFlagSet (TParserFlags aFlag) const
	{
	return (iFlags & aFlag);
	}

void CHtmlToCrtConvActionProcessor::ClearFlag (TParserFlags aFlag)
	{
	iFlags &= ~aFlag;
	}

#endif
