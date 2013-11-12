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
#include "MHtmlToCrtConvResourceFile.h"
#include <chtmltocrtconverter.rsg>

const TInt KDefaultFontSizeInTwips = 200;

//
//Header Tags
//
void CHtmlToCrtConvActionProcessor::DoHeaderL(CHtmlToCrtConvParser::TTagState aTagState, TInt aFontSizeInTwips)
	{
	DoBlockLevelTagActionL();

	if (aTagState==CHtmlToCrtConvParser::EOpeningTag)
		{
		SetFontStrokeWeightL(EStrokeWeightBold);
		SetFontSizeL(aFontSizeInTwips);
		}
	else if (aTagState==CHtmlToCrtConvParser::EClosingTag)
		{
		SetFontStrokeWeightL(EStrokeWeightNormal);
		SetFontSizeL(KDefaultFontSizeInTwips);
		}
	}
//
//EHtmlTagTitle
//
void CHtmlToCrtConvActionProcessor::DoTitleL(CHtmlToCrtConvParser::TTagState aTagState)
	{
	DoHeaderL(aTagState, KH1SizeInTwips);
	InsertParagraphDelimiterL();
	}
//
//EHtmlAttributeHref and EHtmlAttributeSrc - recognised for EHtmlTagAnchor and EHtmlTagImage
//
void CHtmlToCrtConvActionProcessor::DoShowUrlL(const TDesC8& aValue, CHtmlToCrtConvParser::TTagState aTagState, THtmlToCrtConvTagType aTagType)
	{
	TRgb urlTextColor=KDefaultTextColor;

	if(aTagType==EHtmlTagAnchor)
		{
		urlTextColor=iResourceFile.ReadResourceTRgbLC(R_CNV_COLOUR_ANCHOR_TAG_HREF);
		}
	else if(aTagType==EHtmlTagImage)
		{
		urlTextColor=iResourceFile.ReadResourceTRgbLC(R_CNV_COLOUR_IMAGE_TAG_SRC);
		}

	if (aTagState==CHtmlToCrtConvParser::EOpeningTag)
		{
		SetFontTextColorL(urlTextColor);

		_LIT(KSpaceLeftBracket," (");
		iText.InsertL(iTextPosition, KSpaceLeftBracket);
		iTextPosition += 2;

		HBufC* temp=HBufC::NewLC(aValue.Length());
		TPtr pTemp(temp->Des());
		pTemp.Copy(aValue);
		iText.InsertL(iTextPosition, pTemp);
		CleanupStack::PopAndDestroy(temp);
		iTextPosition=iText.DocumentLength();

		_LIT(KRightBracketSpace,") ");
		iText.InsertL(iTextPosition, KRightBracketSpace);
		iTextPosition += 2;

		SetFontTextColorL(KDefaultTextColor);
		}
	}
//
//EHtmlAttributeAlt - recognised for EHtmlTagImage only
//
void CHtmlToCrtConvActionProcessor::DoAltL(const TDesC8& aValue, CHtmlToCrtConvParser::TTagState aTagState)
	{
	if (aTagState==CHtmlToCrtConvParser::EOpeningTag)
		{
		_LIT(KLeftBracket,"<");
		iText.InsertL(iTextPosition, KLeftBracket);
		iTextPosition += 1;

		HBufC* temp=HBufC::NewLC(aValue.Length());
		TPtr pTemp(temp->Des());
		pTemp.Copy(aValue);
		iText.InsertL(iTextPosition, pTemp);
		CleanupStack::PopAndDestroy(temp);
		iTextPosition=iText.DocumentLength();

		_LIT(KRightBracket,">");
		iText.InsertL(iTextPosition, KRightBracket);
		iTextPosition += 1;
		}
	}
void CHtmlToCrtConvActionProcessor::InsertImgTagResourceL()
	{
	//read image text from the resource file
	HBufC* resourceText=iResourceFile.ReadResourceHBufCLC(R_CNV_IMAGE_TAG_DEFAULT_TEXT);
	iText.InsertL(iTextPosition, *resourceText);
	CleanupStack::PopAndDestroy(resourceText);
	iTextPosition=iText.DocumentLength();
	}
