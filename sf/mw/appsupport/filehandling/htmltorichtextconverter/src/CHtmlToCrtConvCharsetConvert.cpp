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

#include <s32file.h>

#include "CHtmlToCrtConvCharsetConvert.h"
#include "CHtmlToCrtConvBuffer.h"
#include "CHtmlToCrtConverter.h"
#include "MHtmlToCrtConvResourceFile.h"
#include <chtmltocrtconverter.rsg>

const TInt KSampleBufferSize = 256;
const TInt KMetaTagCharsetValueBufferLength	= 40;
const TInt KMimimumConfidenceLevel = 50;

CHtmlToCrtConvCharsetConvert* CHtmlToCrtConvCharsetConvert::NewL(CHtmlToCrtConvBuffer& aBuffer, RFs& aFsSession, CCnvCharacterSetConverter& aCnvCharacterSetConverter, MHtmlToCrtConvResourceFile& aResourceFile, MConverterUiObserver* aObserver)
	{
	CHtmlToCrtConvCharsetConvert* self=new(ELeave) CHtmlToCrtConvCharsetConvert(aBuffer, aObserver, aFsSession, aCnvCharacterSetConverter, aResourceFile);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CHtmlToCrtConvCharsetConvert::ConstructL()
	{
	iArrayOfCharacterSetsAvailable=CCnvCharacterSetConverter::CreateArrayOfCharacterSetsAvailableL(iFsSession);
	}

CHtmlToCrtConvCharsetConvert::CHtmlToCrtConvCharsetConvert(CHtmlToCrtConvBuffer& aBuffer, MConverterUiObserver* aObserver, RFs& aFsSession, CCnvCharacterSetConverter& aCnvCharacterSetConverter, MHtmlToCrtConvResourceFile& aResourceFile)
:iCnvCharacterSetConverter(aCnvCharacterSetConverter)
,iBuffer(aBuffer)
,iObserver(aObserver)
,iResourceFile(aResourceFile)
,iFsSession(aFsSession)
	{
	}

CHtmlToCrtConvCharsetConvert::~CHtmlToCrtConvCharsetConvert()
	{
	delete iArrayOfCharacterSetsAvailable;
	}

//==================================================================================
//GetCharSetFromPasswordL - uses MConverterUiObserver::QueryPasswordL
//==================================================================================
TBool CHtmlToCrtConvCharsetConvert::GetCharSetFromPasswordL()
	{
	if(iObserver)
		{
		HBufC16* unicodeCharsetName=iObserver->QueryPasswordL(KHtmlToCrtRequestCharset);
				
		if(unicodeCharsetName)
			{
			//convert from unicode
			HBufC8* charsetName=HBufC8::NewLC(unicodeCharsetName->Length());
			ConvertFromUnicodeL(*unicodeCharsetName, *charsetName);		
			//convert standard name of character set to TUint identifier
			iCharacterSetIdentifier=iCnvCharacterSetConverter.ConvertStandardNameOfCharacterSetToIdentifierL(*(charsetName), iFsSession);
			CleanupStack::PopAndDestroy(charsetName);
			}
		}
	return iCharacterSetIdentifier;
	}

//==================================================================================
//GetCharSetFromMetaTagL - searches for META tag, then extracts charset value
//<META http-equiv="Content-Type" content="text/html; charset = us-ascii">
//==================================================================================
TBool CHtmlToCrtConvCharsetConvert::GetCharSetFromMetaTagL()
	{
	HBufC8* temp=HBufC8::NewLC(KSampleBufferSize);
	TPtr8 buffer(temp->Des());
	TInt offset=0;
	iBuffer.GetSampleOfTextFromFileL(buffer, KSampleBufferSize, offset);

	_LIT8(KMeta, "<meta");
	_LIT8(KClosingTagIndicator, ">");
	_LIT8(KCharset, "charset");
	_LIT8(KEquals, "=");

	offset=FindAndDeleteString(buffer, KMeta);
	if(offset>=0)//meta found
		{
		//find >
		TInt offset=buffer.FindF(KClosingTagIndicator);		
		if(offset>=0)//> found
			{
			//delete > onwards - Delete() adjusts length so as not to delete beyond end of buffer
			buffer.Delete(offset, buffer.Length());

			//now buffer only contains META tag
			offset=FindAndDeleteString(buffer, KCharset);
			if(offset>=0)//charset found
				{
				offset=FindAndDeleteString(buffer, KEquals);
				if(offset>=0)//= found
					{
					ParseCharsetValue(buffer);
					//ConvertStandardNameOfCharacterSetToIdentifierL returns the UID of a 
					//character set for a given Internet-standard name
					//zero is returned if none found
					iCharacterSetIdentifier=iCnvCharacterSetConverter.ConvertStandardNameOfCharacterSetToIdentifierL(buffer, iFsSession);
					}
				}
			}
		}
	CleanupStack::PopAndDestroy(temp);
	return iCharacterSetIdentifier;
	}

//==================================================================================
//FindAndDeleteString - deletes up to and including string
//==================================================================================
TInt CHtmlToCrtConvCharsetConvert::FindAndDeleteString(TDes8& aBuffer, const TDesC8& aString)
	{
	TInt offset=aBuffer.FindF(aString);
	TInt length=aString.Length();
	
	if(offset>=0)//string found
		{
		//delete up to and including string
		ASSERT(offset+length <= aBuffer.Length());
		aBuffer.Delete(0, offset+length);
		}
	return offset;
	}
//==================================================================================
//ParseCharsetValue - charset value read until " or white space character seen
//==================================================================================
void CHtmlToCrtConvCharsetConvert::ParseCharsetValue(TDes8& aBuffer)
	{
	aBuffer.TrimLeft();
	TInt length=aBuffer.Length();
	if(length)
		{
		TChar character;
		TBuf8<KMetaTagCharsetValueBufferLength> charsetBuffer;
		TBool finishedReadingValue=EFalse;
		TInt ii=0;
		while(ii<length && !finishedReadingValue)
			{
			character=aBuffer[ii++];
			if(character.IsSpace() || character=='"')
				{
				finishedReadingValue=ETrue;
				}
			else
				{
				if(charsetBuffer.Length()<KMetaTagCharsetValueBufferLength)
					{
					charsetBuffer.Append(character);
					}
				else
					{
					ii=length;
					}
				}
			}
		aBuffer.Copy(charsetBuffer);
		}
	}
//==================================================================================
//GetCharSetUsingAutoDetectL
//==================================================================================
TBool CHtmlToCrtConvCharsetConvert::GetCharSetUsingAutoDetectL()
	{
	HBufC8* temp=HBufC8::NewLC(KSampleBufferSize);
	TPtr8 buffer(temp->Des());
	TInt offset=256;
	//it's likely that the start of the file will contain the header, so offset
	//is used to read from a position within the file
	iBuffer.GetSampleOfTextFromFileL(buffer, KSampleBufferSize, offset);

	TInt confidence=0;

	if(buffer.Length())
		{
		iCnvCharacterSetConverter.AutoDetectCharacterSetL(confidence, iCharacterSetIdentifier, *iArrayOfCharacterSetsAvailable, buffer);
		}

	CleanupStack::PopAndDestroy(temp);

	if(iCharacterSetIdentifier == KCharacterSetIdentifierAscii)
		{
		//In order to support Latin letters, the character set needs to 
		//be widened from ASCII to Code Page 1252
		iCharacterSetIdentifier = KCharacterSetIdentifierCodePage1252;
		}
		
	//confidence in range 0-100
	if(confidence<KMimimumConfidenceLevel)
		{
		//low level of confidence in the chosen character set, so reset
		// - this method of determining character set not successful
		iCharacterSetIdentifier=0;
		return EFalse;
		}
	else
		return ETrue;
	}

//=============================================================
//PrepareForConvertToUnicodeL
//=============================================================
void CHtmlToCrtConvCharsetConvert::PrepareForConvertToUnicodeL()
	{
	if(!iCharacterSetIdentifier)
		{
		//get default character set for locale from resource file
		HBufC* unicodeDefaultCharacterSet=iResourceFile.ReadResourceHBufCLC(R_CNV_DEFAULT_CHARACTER_SET);

		//convert from unicode
		HBufC8* defaultCharacterSet=HBufC8::NewLC(unicodeDefaultCharacterSet->Length());
		ConvertFromUnicodeL(*unicodeDefaultCharacterSet, *defaultCharacterSet);
		
		//convert standard name of character set to TUint identifier
		iCharacterSetIdentifier=iCnvCharacterSetConverter.ConvertStandardNameOfCharacterSetToIdentifierL(*(defaultCharacterSet), iFsSession);
		CleanupStack::PopAndDestroy(2); //defaultCharacterSet, unicodeDefaultCharacterSet
		}

	iCnvCharacterSetConverter.PrepareToConvertToOrFromL(iCharacterSetIdentifier, *iArrayOfCharacterSetsAvailable, iFsSession);
	}
//=============================================================
//ConvertFromUnicodeL
//=============================================================
void CHtmlToCrtConvCharsetConvert::ConvertFromUnicodeL(const TDesC& aUnicode, HBufC8& aNarrow)
	{
	TPtr8 ptrNarrow(aNarrow.Des());	
	iCnvCharacterSetConverter.PrepareToConvertToOrFromL(KCharacterSetIdentifierAscii, *iArrayOfCharacterSetsAvailable, iFsSession);
	iCnvCharacterSetConverter.ConvertFromUnicode(ptrNarrow, aUnicode);
	}
