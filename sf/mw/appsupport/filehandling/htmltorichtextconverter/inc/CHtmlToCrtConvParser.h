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

#ifndef __CHTMLTOCRTCONVPARSER_H__
#define __CHTMLTOCRTCONVPARSER_H__

#include <e32std.h>  
#include <txtfrmat.h>
#include "CHtmlToCrtConvHash.h"

class CRichText;
class CHtmlToCrtConvBuffer;
class CHtmlToCrtConvHashTable;
class CHtmlToCrtConvActionProcessor;
class MHtmlToCrtConvResourceFile;

const TInt KCharsProcessedInOneStep			= 100;
const TInt KTagBufferLength					= 10;
const TInt KAttributeBufferLength			= 10;
const TInt KEntityBufferLength				= 10;

//========================================================================

class CHtmlToCrtConvAttribute : public CBase
	{
	public:
		static CHtmlToCrtConvAttribute* NewLC(THtmlToCrtConvAttributeType aType, TDesC8& aValue);
		~CHtmlToCrtConvAttribute();

		inline const THtmlToCrtConvAttributeType Type() const {return iType;}
		inline const TDesC8& Value() const {return *iValue;}

	private:
		CHtmlToCrtConvAttribute(THtmlToCrtConvAttributeType aType);
		void ConstructL(TDesC8& aValue);

	private:
		THtmlToCrtConvAttributeType	iType;
		HBufC8*						iValue;
	};

//=========================================================================

class CHtmlToCrtConvParser : public CBase
	{
	public:
		enum TTagState
			{
			ENoTag,
			EOpeningTag,
			EClosingTag
			};

	public:
		static CHtmlToCrtConvParser* NewL(CRichText& aRichText, CHtmlToCrtConvBuffer& aBuffer, MHtmlToCrtConvResourceFile& aResourceFile);
		~CHtmlToCrtConvParser();
		TBool DoOneStepL();

	private:
		CHtmlToCrtConvParser(CHtmlToCrtConvBuffer& aBuffer);
		void ConstructL(CRichText& aRichText, MHtmlToCrtConvResourceFile& aResourceFile);

		TBool InspectCurrentCharacter(TChar aChar);
		void  DoActionL(TChar aChar);

		void  SeeWhiteSpaceCharacterInTag(TBool& aBool);
		void  SeeSpecialCharactersInTag(TChar aChar, TBool& aBool);
		void  SeeOtherCharactersInTag();
		void  SeeOtherCharactersNotInTag(TBool& aBool);

		void  DoTagOperationL();
		void  DoEntityOperationL();
		void  WriteToRichTextL();
		void  WriteToTagBufferL(TChar aChar);
		void  WriteToEntityBufferL(TChar aChar);
		void  WriteToAttributeBufferL(TChar aChar);
		void  WriteToAttributeValueBufferL(TChar aChar);
		void  WriteToAttributeArrayL();

	private:
		enum TParserState
		{
		EInitialState,
		ESeeStartOfTag,
		ESeeEndOfTag,
		ESeeClosingTagIndicator,
		ESeeEquals,
		ESeeExclamationMark,
		ESeeEndOfTagWhileReadingJavascript,
		EReadingOpeningTag,
		EReadingClosingTag,
		EReadingAttribute,
		EReadingAttributeValue,
		EReadingAttributeValueWithinInvCommas,
		EReadingText,
		EReadingJavascript,
		EFinishedReadingTag,
		EFinishedReadingAttribute,
		EFinishedReadingAttributeValue,
		EStartOfCharacterEntity,
		EEndOfCharacterEntity,
		EReadingCharacterEntity
		};

	private:
		TPtrC16 iTextBuffer;
		TBuf16<KTagBufferLength> iTagBuffer;
		TBuf16<KEntityBufferLength> iEntityBuffer;
		TBuf16<KAttributeBufferLength> iAttributeBuffer;
		CBufBase* iAttributeValueBuffer;
		RPointerArray<CHtmlToCrtConvAttribute> iAttributes;
		TBuf16<2> iLastTwoCharacters;
	
		TParserState iParserState;
		TTagState iTagState;
		TBool iInTag;
		TBool iInCharacterEntity;

		TInt iBufferPosition;
		TInt iStartOfTextPosition;
		TInt iEndOfTextPosition;
		TBool iEndOfBufferReached;

		CHtmlToCrtConvBuffer& iBuffer;
		CHtmlToCrtConvHashTable* iHashTable;
		CHtmlToCrtConvActionProcessor* iActionProcessor;
	};

#endif
