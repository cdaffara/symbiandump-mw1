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

#ifndef __CHTMLTOCRTCONVCHARSETCONVERT_H__
#define __CHTMLTOCRTCONVCHARSETCONVERT_H__

#include <f32file.h>
#include <charconv.h>

_LIT(KHtmlToCrtRequestCharset, "content-type: charset?");

class CHtmlToCrtConvBuffer;
class MHtmlToCrtConvResourceFile;
class MConverterUiObserver;

class CHtmlToCrtConvCharsetConvert : public CBase
	{
	public:
		static CHtmlToCrtConvCharsetConvert* NewL(CHtmlToCrtConvBuffer& aBuffer, RFs& aFsSession, CCnvCharacterSetConverter& aCnvCharacterSetConverter, MHtmlToCrtConvResourceFile& aResourceFile, MConverterUiObserver* aObserver=NULL);
		~CHtmlToCrtConvCharsetConvert();

	/**
		@fn		TBool GetCharSetFromPasswordL()
		Intended Usage	:	attempts to get the character set from the mime message header
		@return	TBool ETrue if successful, else EFalse
	 */
		TBool GetCharSetFromPasswordL();
	/**
		@fn		TBool GetCharSetFromMetaTagL()
		Intended Usage	:	attempts to get the character set from the html meta tag
		@return	TBool ETrue if successful, else EFalse
	 */	
		TBool GetCharSetFromMetaTagL();
	/**
		@fn		TBool GetCharSetUsingAutoDetectL()
		Intended Usage	:	attempts to determine the character set by reading a sample of the text
							from the file, and passes this to CCnvCharacterSetConverter::AutoDetectCharacterSetL()
		@return	TBool ETrue if successful, else EFalse
	 */
		TBool GetCharSetUsingAutoDetectL();
		void PrepareForConvertToUnicodeL();

	private:
		CHtmlToCrtConvCharsetConvert(CHtmlToCrtConvBuffer& aBuffer, MConverterUiObserver* aObserver, RFs& aFsSession, CCnvCharacterSetConverter& aCnvCharacterSetConverter, MHtmlToCrtConvResourceFile& aResourceFile);
		void ConstructL();

		TInt FindAndDeleteString(TDes8& aBuffer, const TDesC8& aString);
		void ParseCharsetValue(TDes8& aBuffer);
		void ConvertFromUnicodeL(const TDesC& aUnicode, HBufC8& aNarrow);

	private:
		CCnvCharacterSetConverter& iCnvCharacterSetConverter;
		CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* iArrayOfCharacterSetsAvailable;
		
		TUint iCharacterSetIdentifier;
		CHtmlToCrtConvBuffer& iBuffer;
		MConverterUiObserver* iObserver;
		MHtmlToCrtConvResourceFile& iResourceFile;
		RFs& iFsSession;
	};

#endif
