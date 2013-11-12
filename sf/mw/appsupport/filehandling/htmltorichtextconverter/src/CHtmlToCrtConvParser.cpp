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

//========================================================================================
//the html to be parsed looks like:
//<tag attribute=attribute_value attribute = "attribute value">text ... etc
//
//(1)'tag' is written to iTagBuffer
//(2)'attribute' is written to iAttributeBuffer and 'attribute value' is written
//   to iAttributeValueBuffer, and each pair are appended to the array iAttributes
//(3)iStartOfTextPosition is set to the start of 'text'
//
//when < is seen, 'text' is written to the richtext object
//when > is seen, 'iTagBuffer' is recognised, and any required action is performed; similarly for iAttributes
//========================================================================================

#include "CHtmlToCrtConvParser.h"
#include "CHtmlToCrtConvBuffer.h"
#include "CHtmlToCrtConvHashTable.h"
#include "CHtmlToCrtConvHash.h"
#include "CHtmlToCrtConvActionProcessor.h"
#include "MHtmlToCrtConvResourceFile.h"

const TInt KAttributeValueBufferGranularity = 10;
const TInt KDefaultTextPosition				= -1;

CHtmlToCrtConvParser* CHtmlToCrtConvParser::NewL(CRichText& aRichText, CHtmlToCrtConvBuffer& aBuffer, MHtmlToCrtConvResourceFile& aResourceFile)
	{
	CHtmlToCrtConvParser* self=new(ELeave) CHtmlToCrtConvParser(aBuffer);
	CleanupStack::PushL(self);
	self->ConstructL(aRichText, aResourceFile);
	CleanupStack::Pop(self);
	return self;
	}

void CHtmlToCrtConvParser::ConstructL(CRichText& aRichText, MHtmlToCrtConvResourceFile& aResourceFile)
	{
	iAttributeValueBuffer=CBufFlat::NewL(KAttributeValueBufferGranularity);
	iHashTable=CHtmlToCrtConvHashTable::NewL();
	iActionProcessor=CHtmlToCrtConvActionProcessor::NewL(aRichText, aResourceFile);
	}

CHtmlToCrtConvParser::CHtmlToCrtConvParser(CHtmlToCrtConvBuffer& aBuffer)
:iStartOfTextPosition(KDefaultTextPosition)
,iEndOfTextPosition(KDefaultTextPosition)
,iBuffer(aBuffer)
	{
	}

CHtmlToCrtConvParser::~CHtmlToCrtConvParser()
	{
	delete iHashTable;
	delete iActionProcessor;
	iAttributes.ResetAndDestroy();
	iAttributes.Close();
	delete iAttributeValueBuffer;
	}

//=============================================================
//DoOneStepL
//=============================================================
TBool CHtmlToCrtConvParser::DoOneStepL()
	{
	TChar currentCharacter;
	TBool moreProcessingReqd=ETrue;

	for (TInt ii=0; ii<KCharsProcessedInOneStep; ii++)
		{
		moreProcessingReqd=iBuffer.ReadCharacterL(currentCharacter, iBufferPosition, iEndOfBufferReached);

		if (!moreProcessingReqd)
			{
			if (iStartOfTextPosition!=KDefaultTextPosition)
				{
				iBuffer.GetToEndOfBufferL(iTextBuffer, iStartOfTextPosition);
				WriteToRichTextL();
				}
			break;
			}

		if (InspectCurrentCharacter(currentCharacter))
			{
			DoActionL(currentCharacter);
			}

 		if (iEndOfBufferReached && (iStartOfTextPosition != KDefaultTextPosition) && moreProcessingReqd)
			{
			iEndOfTextPosition=iBufferPosition;
			iBuffer.GetPartOfBufferL(iTextBuffer, iStartOfTextPosition, iEndOfTextPosition);
			WriteToRichTextL();
			iParserState=EInitialState;
			iStartOfTextPosition=KDefaultTextPosition;
			iEndOfTextPosition=KDefaultTextPosition;
			}
		}
	return moreProcessingReqd;
	}
//======================================================================
//InspectCurrentCharacterL - updates iParserState, iTagState and iInTag
//======================================================================
TBool CHtmlToCrtConvParser::InspectCurrentCharacter(TChar aChar)
	{
	TBool processCharacter=ETrue;

	if (iParserState==EReadingJavascript)
		{
		if (aChar=='>')
			iParserState=ESeeEndOfTagWhileReadingJavascript;
		}

	else if (aChar=='<')
		{
		iTagState	=EOpeningTag;
	 	iParserState=ESeeStartOfTag;
		iInTag		=ETrue;
		}
	else if (iInTag)
		{
		if (aChar=='/' || aChar=='=' || aChar=='"' || aChar=='>' || aChar=='!')
			{
			SeeSpecialCharactersInTag(aChar, processCharacter);
			}
		else if (aChar.IsSpace())
			{
			SeeWhiteSpaceCharacterInTag(processCharacter);
			}
		else if (iParserState==ESeeExclamationMark && aChar=='-')
			{
			iParserState=EReadingJavascript;
			processCharacter=EFalse;
			}
		else
			{
			SeeOtherCharactersInTag();
			}
		}
	else if (aChar == '&')
		{
		iParserState = EStartOfCharacterEntity;
		iInCharacterEntity = ETrue;
		processCharacter = EFalse;
		iStartOfTextPosition = KDefaultTextPosition;
		}
	else if (iInCharacterEntity)
		{
		if (aChar == ';')
			{
			iParserState = EEndOfCharacterEntity;
			iInCharacterEntity = EFalse;
			}
 		else
 			{
			iParserState = EReadingCharacterEntity;
			}
		}
	else
		{
		SeeOtherCharactersNotInTag(processCharacter);
		}

	return processCharacter;
	}
//=============================================================
//functions used by InspectCurrentCharacterL
//=============================================================
void CHtmlToCrtConvParser::SeeWhiteSpaceCharacterInTag(TBool& aBool)
	{		
	switch (iParserState)
		{
		case EReadingOpeningTag:
			iParserState=EFinishedReadingTag;
			aBool=EFalse;
			break;
		case EReadingClosingTag:
			iParserState=EFinishedReadingTag;
			aBool=EFalse;
			break;
		case EReadingAttribute:
			iParserState=EFinishedReadingAttribute;
			aBool=EFalse;
			break;
		case EReadingAttributeValue:
			iParserState=EFinishedReadingAttributeValue;
			break;
		case EReadingAttributeValueWithinInvCommas:
			break;
		default:
			break;
		}
	};

void CHtmlToCrtConvParser::SeeSpecialCharactersInTag(TChar aChar, TBool& aBool)
	{
	switch (aChar)
		{
		case '/':
			if (iParserState==EReadingAttributeValue || iParserState==EReadingAttributeValueWithinInvCommas)
				aBool=ETrue;
			else if (iParserState==ESeeStartOfTag)
				{
				iTagState=EClosingTag;
	 			iParserState=ESeeClosingTagIndicator;
				aBool=EFalse;
				}
			break;
		case '=':
			if(iParserState!=EReadingAttributeValue && iParserState!=EReadingAttributeValueWithinInvCommas)
				{
				iParserState=ESeeEquals;
				aBool=EFalse;
				}
			else
				{
				aBool=ETrue;
				}
			break;
		case '"':
			if (iParserState==ESeeEquals)
				{
				iParserState=EReadingAttributeValueWithinInvCommas;
				aBool=EFalse;
				}
			else
				{
				iParserState=EFinishedReadingAttributeValue;
				}
			break;
		case '>':
			iParserState=ESeeEndOfTag;
			iInTag=EFalse;
			break;
		case '!':
			if(iParserState==ESeeStartOfTag)
				{
				iParserState=ESeeExclamationMark;
				aBool=EFalse;
				}
			break;
		default:
			break;
		}	
	};

void CHtmlToCrtConvParser::SeeOtherCharactersInTag()
	{
	switch (iParserState)
		{
		case ESeeStartOfTag:
			iParserState=EReadingOpeningTag;
			break;
		case ESeeClosingTagIndicator:
			iParserState=EReadingClosingTag;
			break;
		case ESeeEquals:
			iParserState=EReadingAttributeValue;
			break;
		case EFinishedReadingTag:
			iParserState=EReadingAttribute;
			break;
		case EFinishedReadingAttribute:
			iAttributeBuffer.Zero();
			iParserState=EReadingAttribute;
			break;
		case EFinishedReadingAttributeValue:
			iParserState=EReadingAttribute;
			break;
		default:
			break;
		}
	};

void CHtmlToCrtConvParser::SeeOtherCharactersNotInTag(TBool& aBool)
	{
	switch (iParserState)
		{
		case EReadingText:
			aBool=EFalse;
			break;
		case ESeeEndOfTag:
		case EEndOfCharacterEntity:
			iParserState=EReadingText;
			break;
		case EInitialState:
			iParserState=EReadingText;
			break;
		default:
			break;
		}
	};
//=============================================================
//DoActionL
//=============================================================
void CHtmlToCrtConvParser::DoActionL(TChar aChar)
	{
	switch(iParserState)
		{
		case EReadingText:
			iStartOfTextPosition=iBufferPosition;
			break;
		case EReadingOpeningTag:
			WriteToTagBufferL(aChar);
			break;
		case EReadingClosingTag:
			WriteToTagBufferL(aChar);
			break;
		case EEndOfCharacterEntity:
			DoEntityOperationL();
			break;
		case EReadingCharacterEntity:
			WriteToEntityBufferL(aChar);
			break;
		case EReadingAttribute:
			WriteToAttributeBufferL(aChar);
			break;
		case EReadingAttributeValue:
			WriteToAttributeValueBufferL(aChar);
			break;
		case EReadingAttributeValueWithinInvCommas:
			WriteToAttributeValueBufferL(aChar);
			break;
		case EFinishedReadingAttributeValue:
			WriteToAttributeArrayL();
			iAttributeBuffer.Zero();
			iAttributeValueBuffer->Reset();
			break;
		case ESeeStartOfTag:
			if (iStartOfTextPosition!=KDefaultTextPosition)
				{
				iEndOfTextPosition=iBufferPosition - 1;
				iBuffer.GetPartOfBufferL(iTextBuffer, iStartOfTextPosition, iEndOfTextPosition);
				WriteToRichTextL();
				iStartOfTextPosition=KDefaultTextPosition;
				iEndOfTextPosition=KDefaultTextPosition;
				}
			break;				
		case ESeeEndOfTag:
			if(iAttributeValueBuffer->Size())
				{
				WriteToAttributeArrayL();
				iAttributeBuffer.Zero();
				iAttributeValueBuffer->Reset();
				}
			DoTagOperationL();
			break;
		case ESeeEndOfTagWhileReadingJavascript:
			{
			_LIT(KJavascriptEnd, "--");
			if(!(iLastTwoCharacters.Compare(KJavascriptEnd)))
				{
				iParserState=EInitialState;//finished reading javascript
				iInTag=EFalse;
				}
			else
				iParserState=EReadingJavascript;
			break;
			}
		case EReadingJavascript:
			//keep record of last two characters encountered, for comparing with -- when > seen
			if(iLastTwoCharacters.Length()==2)
				iLastTwoCharacters.Copy(iLastTwoCharacters.Right(1));

			iLastTwoCharacters.Append(aChar);
			break;
		default:
			break;
		};
	}
//=============================================================
//DoTagOperationL
//=============================================================
void  CHtmlToCrtConvParser::DoTagOperationL()
	{
	const TInt count=iAttributes.Count();
	THtmlToCrtConvTagType tagType=iHashTable->LookupTag(iTagBuffer);

	if (iTagBuffer.Length())
		{
		iActionProcessor->DoTagActionL(tagType, iTagState);
		iTagBuffer.Zero();

		TBool imgTagResourceReqd=EFalse;
		for(TInt ii=0; ii<count; ii++)
			{
			THtmlToCrtConvAttributeType attributeType=(iAttributes)[ii]->Type();
			const TDesC8& attributeValue=(iAttributes)[ii]->Value();
			iActionProcessor->DoAttributeActionL(tagType, iTagState, attributeType, attributeValue, imgTagResourceReqd);
			}
		if(imgTagResourceReqd)
			{
			iActionProcessor->InsertImgTagResourceL();
			}
		iAttributes.ResetAndDestroy();
		}
	}

void CHtmlToCrtConvParser::DoEntityOperationL()
	{
	const TDesC16& entity = iHashTable->LookupEntity(iEntityBuffer);
	if(entity != KHtmlEntityUnknown)
		{
		iTextBuffer.Set(entity);
		WriteToRichTextL();
		}
	iEntityBuffer.Zero();
	}

void  CHtmlToCrtConvParser::WriteToRichTextL()
	{
	ASSERT(iTextBuffer.Length() > 0);
	if (iTextBuffer.Length())
		{
		iActionProcessor->DoWriteTextL(iTextBuffer);
		}
	}

void  CHtmlToCrtConvParser::WriteToTagBufferL(TChar aChar)
	{
	if (iTagBuffer.Length()==KTagBufferLength)
		{
		iTagBuffer.Zero();
		}
	iTagBuffer.Append(aChar);
	}

void  CHtmlToCrtConvParser::WriteToEntityBufferL(TChar aChar)
	{
	if (iEntityBuffer.Length()==KEntityBufferLength)
		iEntityBuffer.Zero();

	iEntityBuffer.Append(aChar);
	}

void  CHtmlToCrtConvParser::WriteToAttributeBufferL(TChar aChar)
	{
	if (iAttributeBuffer.Length()==KAttributeBufferLength)
		{
		iAttributeBuffer.Zero();
		}
	iAttributeBuffer.Append(aChar);
	}

void  CHtmlToCrtConvParser::WriteToAttributeValueBufferL(TChar aChar)
	{
	TInt size=iAttributeValueBuffer->Size();
	TBuf8<1> temp;
	temp.Append(aChar);
	iAttributeValueBuffer->InsertL(size, temp);
	}

void  CHtmlToCrtConvParser::WriteToAttributeArrayL()
	{
	iAttributeValueBuffer->Compress();
	if (iAttributeBuffer.Length() && iAttributeValueBuffer->Size())
		{
		THtmlToCrtConvAttributeType attType=iHashTable->LookupAttribute(iAttributeBuffer);
		TPtr8 pAttributeTag(iAttributeValueBuffer->Ptr(0));
		CHtmlToCrtConvAttribute* attribute=CHtmlToCrtConvAttribute::NewLC(attType, pAttributeTag);

		User::LeaveIfError(iAttributes.Append(attribute));		
		CleanupStack::Pop(attribute);
		}
	}

//=============================================================
//CHtmlToCrtConvAttribute class
//=============================================================
CHtmlToCrtConvAttribute* CHtmlToCrtConvAttribute::NewLC(THtmlToCrtConvAttributeType aType, TDesC8& aValue)
	{
	CHtmlToCrtConvAttribute* self=new(ELeave) CHtmlToCrtConvAttribute(aType);
	CleanupStack::PushL(self);
	self -> ConstructL(aValue);
	return self;
	}

CHtmlToCrtConvAttribute::CHtmlToCrtConvAttribute(THtmlToCrtConvAttributeType aType)
:iType(aType)
	{
	}

void CHtmlToCrtConvAttribute::ConstructL(TDesC8& aValue)
	{
	iValue=aValue.AllocL();
	}

CHtmlToCrtConvAttribute::~CHtmlToCrtConvAttribute()
	{
	delete iValue;
	}
