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

#include <e32base.h>
#include <e32def.h>

#include "BASE64.H"
#include "BASE64DF.H"

//
// class CBase64CodecBase
// 
// Provides 3 octet encoding and decoding alone
// Returns an error code in the case of invalid source
//

void CBase64CodecBase::Reset()
//
// Reset the internal buffer
	{
	for (TInt i = 0; i < 4; i++)
		iQuart[i] = NULL;
	iQuartCount = 0;
	}


TInt CBase64CodecBase::Encode(const TDesC8& aSource, TDes8& aResult) const
//
// Encode an arbitrary number of octets in aSource append the output to aResult.
// If the number of octets in aSource is not a multiple of 3
// padding will be added terminating the encoding
//
	{
	TInt sourceLength = aSource.Length();
	if (sourceLength == 0)
		return KErrNone;
	__ASSERT_ALWAYS(((sourceLength * 4)/3 + sourceLength%3) <= (aResult.MaxLength()-aResult.Length()), Panic(EBase64Overflow));

	sourceLength -= sourceLength%3;
	TUint8 sixBit=NULL;
	TText8 source;
	TInt i;

	for (i = 0; i < sourceLength; i++)
		{
		source=aSource[i];
		sixBit = STATIC_CAST(TUint8,(source & 0xFC) >> 2);
		aResult.Append(KBase64Alphabet[sixBit]);
		sixBit = NULL;
		sixBit = STATIC_CAST(TUint8,(source & 0x03) << 4);
		source=aSource[++i];
		sixBit |= STATIC_CAST(TUint8,(source & 0xF0) >> 4);
		aResult.Append(KBase64Alphabet[sixBit]);
		sixBit = NULL;
		sixBit = STATIC_CAST(TUint8,(source & 0x0F) << 2);
		source=aSource[++i];
		sixBit |= STATIC_CAST(TUint8,(source & 0xC0) >> 6);
		aResult.Append(KBase64Alphabet[sixBit]);
		sixBit = NULL;
		sixBit = STATIC_CAST(TUint8,(source & 0x3F));
		aResult.Append(KBase64Alphabet[sixBit]);
		}
	switch (aSource.Length() % 3)
		{
	case 2:
		source=aSource[i];
		sixBit = STATIC_CAST(TUint8,(source & 0xFC) >> 2);
		aResult.Append(KBase64Alphabet[sixBit]);
		sixBit = NULL;
		sixBit = STATIC_CAST(TUint8,(source & 0x03) << 4);
		source=aSource[++i];
		sixBit |= STATIC_CAST(TUint8,(source & 0xF0) >> 4);
		aResult.Append(KBase64Alphabet[sixBit]);
		sixBit = NULL;
		sixBit = STATIC_CAST(TUint8,(source & 0x0F) << 2);
		aResult.Append(KBase64Alphabet[sixBit]);
		aResult.Append(KBase64Alphabet[KBase64Pad]);
		break;
	case 1:
		source=aSource[i];
		sixBit = STATIC_CAST(TUint8,(source & 0xFC) >> 2);
		aResult.Append(KBase64Alphabet[sixBit]);
		sixBit = NULL;
		sixBit = STATIC_CAST(TUint8,(source & 0x03) << 4);
		aResult.Append(KBase64Alphabet[sixBit]);
		aResult.Append(KBase64Alphabet[KBase64Pad]);
		aResult.Append(KBase64Alphabet[KBase64Pad]);
		break;
	default:
		break;
		}
	return KErrNone;
	}

TInt CBase64CodecBase::Decode(const TDesC8& aSource, TDes8& aResult)
//
// Decode the base64 stream in aSource append the output to aResult.
// Any non base64 chars are discarded while decoding
// Returns if either an end of stream is found, or all aSource is exhausted
// 
// Octets are only decoded when complete
// 
	{
	__ASSERT_ALWAYS((aSource.Length() * 3)/4 <= (aResult.MaxLength() - aResult.Length()), Panic(EBase64Overflow));
	if (aSource.Length() == 0)
		return KErrNone;
	TText8* readPtr=CONST_CAST(TText8*,aSource.Ptr());
	TText8* writePtr=CONST_CAST(TText8*,aResult.Ptr());
	TText8* writeStart=writePtr;
	writePtr += aResult.Length();
	TText8 result;
	TText8* readEnd=(readPtr + aSource.Length());

	for (; readPtr < readEnd; readPtr++)
		{
		TUint8 base64code = Base64Char(*readPtr);
		switch (base64code)
			{
		case KBase64Pad:
			if (iQuartCount == 0)
				return KErrNone;
			result = STATIC_CAST(TText8,iQuart[0] << 2);
			result |= STATIC_CAST(TText8,((iQuart[1] & 0x30) >> 4));
			*writePtr=result;
			writePtr++;
			if (iQuartCount == 3)
				{
				result = STATIC_CAST(TText8,(iQuart[1] & 0x0F) << 4);
				result |= STATIC_CAST(TText8,((iQuart[2] & 0x3C) >> 2));
				*writePtr=result;
				writePtr++;
				}
			aResult.SetLength(writePtr-writeStart);	
			iQuartCount = 0;
			return KErrNone;
		case KErrInvalidCharacter:
			// Do nothing
			break;
		default:
			{
			iQuart[iQuartCount++] = base64code;
			if (iQuartCount == 4)
				{
				result = STATIC_CAST(TText8,iQuart[0] << 2);
				result |= STATIC_CAST(TText8,((iQuart[1] & 0x30) >> 4));
				*writePtr=result;
				writePtr++;
				result = STATIC_CAST(TText8,(iQuart[1] & 0x0F) << 4);
				result |= STATIC_CAST(TText8,((iQuart[2] & 0x3C) >> 2));
				*writePtr=result;
				writePtr++;
				result = STATIC_CAST(TText8,((iQuart[2] & 0x03) << 6));
				result |= STATIC_CAST(TText8,iQuart[3]);
				*writePtr=result;
				writePtr++;
				iQuartCount = 0;
				}
			}
			}
		}
	aResult.SetLength(writePtr-writeStart);
	return KErrNone;
	}

TUint8 CBase64CodecBase::Base64Char(TText8 aChar) const
//
// Return the code or KErrInvalidCharacter
	{
	for (TUint8 i = 0; i < 65; i++)
		{
		if (aChar == KBase64Alphabet[i])
			return i;
		}
	return KErrInvalidCharacter;
	}


GLDEF_C void Panic(TBase64Panic aPanic)
// Panic the process with Base64 codec as the category.
//
	{
	_LIT(KPanicBase64Codec,"Base64 codec");
	User::Panic(KPanicBase64Codec,aPanic);
	}
