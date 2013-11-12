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
// See 6.7 in http://www.ietf.org/rfc/rfc2045.txt
// 
//

#include <e32svr.h>
#include <conarc.h>
#include <s32file.h>
#include <e32std.h>
#include <e32uid.h>
#include <u32hal.h>

#include "QPCONV.H"

// The ASCII character '#' is not encoded into quoted printable format. If it is present along with
// the phone number, it gets converted to its corresponding character '=23'. This makes a bit difficult
// for the user to identify the exact phone number from a VCard file. Hence the encoding is not performed.

#define KLineFeed 10
#define KCarriageReturn 13
#define KEquals '='
#define KSpace ' '
#define KTab 9
#define KExclamationPoint '!'
#define KTilde 0x7E
#define KDoubleQuotes  0x22
#define KDollarSign    0x24
#define KAtSign    0x40
#define KSquareBracketOpen    0x5B
#define KBackSlash    0x5C
#define KSquareBracketClose    0x5D
#define KCircumflex    0x5E
#define KSingleQuote    0x60
#define KCurlyBracketOpen    0x7B
#define KOrOperatorSign    0x7C
#define KCurlyBracketClose    0x7D
#define KQuotedPrintableNewLine _L8("=\r\n")
#define KQuotedPrintableCarriageReturn _L8("=0D")
#define KQuotedPrintableLineFeed _L8("=0A")

// Patchable const data value defined in conarc.cpp
IMPORT_C extern const TInt KEnableAllSplCharForQpCnv;


//
// class CQpToTxtCnv
// 

void CQpToTxtCnv::ConvertObjectAL(RReadStream& aReadStream, RWriteStream& aWriteStream,MConverterUiObserver* /*aObserver*/)
	{
	iReadStream = &aReadStream;
	iWriteStream = &aWriteStream;

	iBufferPos = 0;
	iBuffer.SetMax();
	}

TBool CQpToTxtCnv::DoConvertL()
	{
	// Check if we need to move out the oldest charcter
	if (iBufferPos == 3)
		{
		iBuffer[0] = iBuffer[1];
		iBuffer[1] = iBuffer[2];
		iBufferPos = 2;
		}

	TBuf8<1> character;
	TRAPD(err, iReadStream->ReadL(character));
	if (err == KErrEof)
		{
		// Write what we have in the buffer, and exit
		iWriteStream->WriteL(iBuffer, iBufferPos);
		return EFalse;
		}
	else if (err == KErrNone)
		{
		iBuffer[iBufferPos] = character[0];
		iBufferPos++;
		if (iBufferPos == 3)
			{
			// Check that we have three characters to compare:
			if (iBuffer[0] == KEquals)
				{
				// Check for special QP codes
				if (!(iBuffer[1] == KCarriageReturn && iBuffer[2] == KLineFeed))
					{
					// act on the code
					TLex8 lex(iBuffer);
					TUint code;
					(void)lex.Get(); // skip the equals
					lex.Val(code,EHex);
					iWriteStream->WriteUint8L(STATIC_CAST(TUint8,code));
					}
				iBufferPos=0;
				}
			else if (iBuffer[1] == KCarriageReturn && iBuffer[2] == KLineFeed)
				{
				// throw away these characters
				iBufferPos=0;
				}
			else
				{
				// Write out the last character in the stream
				iWriteStream->WriteUint8L(iBuffer[0]);
				}
			}
		}

	else
		{
		// Some other error, let's clear up and go home
		return EFalse;
		}
	return ETrue;
	}

TUid CQpToTxtCnv::Uid()
	{
	return KUidQuotedPrintableToText;
	}

TInt CQpToTxtCnv::Capabilities()
	{
	return EConvertsObjects;
	}

CQpToTxtCnv::CQpToTxtCnv()
	{}

//
// class CTxtToQpCnv
// 

void CTxtToQpCnv::ConvertObjectAL(RReadStream& aReadStream, RWriteStream& aWriteStream,MConverterUiObserver* /*aObserver*/)
	{
	iReadStream = &aReadStream;
	iWriteStream = &aWriteStream;
	
	iLineLen = 0;
	}

TBool CTxtToQpCnv::DoConvertL()
	{
	TBuf8<1> character;
	TRAPD(err, iReadStream->ReadL(character));
	if (err == KErrNone)
		{
		switch (character[0])
			{
		case KCarriageReturn:
			iWriteStream->WriteL(KQuotedPrintableCarriageReturn);
			iLineLen += 3;
			break;
		case KLineFeed:
			iWriteStream->WriteL(KQuotedPrintableLineFeed);
			iLineLen += 3;
			break;
		case KEquals:
		case KTab:
		case KSpace:
			{
			FormatAndWriteBufferL(character);
			}
			break;
		case KExclamationPoint:
		case KTilde:
		case KDoubleQuotes:
		case KDollarSign:
		case KAtSign:
		case KSquareBracketOpen:
		case KBackSlash:
		case KSquareBracketClose:
		case KCircumflex:
		case KSingleQuote:
		case KCurlyBracketOpen:
		case KOrOperatorSign:
		case KCurlyBracketClose:
			{
			if (iEnableAllSplCharForQpCnv)
				{
				FormatAndWriteBufferL(character);
				}
			else
				{
				WriteBufferL(character);
				}
			}
			break;
		default:
			if( character[0] > KQuotedPrintableConvertAbove )
				{
				FormatAndWriteBufferL(character);
				}
			else
				{
				WriteBufferL(character);
				}
			break;
			}
				
		// Check to see if the line is getting too long
		if (iLineLen > 70)
			{
			iWriteStream->WriteL(KQuotedPrintableNewLine);
			iLineLen = 0;
			}

		return ETrue;
		}
	else
		{
		// Exit on end of stream
		return EFalse;
		}

	}

void CTxtToQpCnv::FormatAndWriteBufferL(TDes8& aChar)
	{
	TBuf8<3> buf;
	buf.Format(_L8("=%02X"),STATIC_CAST(TUint,aChar[0])); // RFC 2045 states "Uppercase letters must be used; lowercase letters are not allowed", hence the capital 'X' in this format descriptor to the statement
	iWriteStream->WriteL(buf);
	iLineLen+=3;
	}

void CTxtToQpCnv::WriteBufferL(TDes8& aChar)
	{
	iWriteStream->WriteL(aChar);
	iLineLen++;
	}

TUid CTxtToQpCnv::Uid()
	{
	return KUidTextToQuotedPrintable;
	}

TInt CTxtToQpCnv::Capabilities()
	{
	return EConvertsObjects;
	}

CTxtToQpCnv::CTxtToQpCnv():iEnableAllSplCharForQpCnv(KEnableAllSplCharForQpCnv)
	{
#ifdef __WINS__
	// KEnableAllSplCharForQpCnv is a Rom patchable constant, so patch for emulator builds can be done by
	// adding "patchdata_conarc_dll_KEnableAllSplCharForQpCnv X" to epoc.ini file.
	// When X is set to non zero, the conversion is performed for all the special characters.
	// When X is set to zero, no conversion is performed in case of the special characters.
	// Requires licensees to set the patch data value in epoc.ini file.
	
	TInt valueOfKEnableAllSplCharForQpCnv = 0;
	if (UserSvr::HalFunction(EHalGroupEmulator,EEmulatorHalIntProperty,(TAny*)"patchdata_conarc_dll_KEnableAllSplCharForQpCnv",&valueOfKEnableAllSplCharForQpCnv) == KErrNone)
		{
		iEnableAllSplCharForQpCnv = valueOfKEnableAllSplCharForQpCnv;
		}
#endif
	}


