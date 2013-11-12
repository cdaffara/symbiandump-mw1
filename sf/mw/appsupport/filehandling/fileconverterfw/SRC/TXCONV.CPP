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



#include <conarc.h>
#include <s32file.h>
#include <e32std.h>
#include <e32uid.h>

#include "TXCONV.H"

//
// class CEtToTxtCnv
// 

CEtToTxtCnv::~CEtToTxtCnv()
	{
	delete iTextTranFromEra;
	}

void CEtToTxtCnv::ConvertObjectAL(RReadStream& aReadStream, RWriteStream& aWriteStream,MConverterUiObserver* /*aObserver*/)
	{
	iReadStream = &aReadStream;
	iPos=0;
	iLength=aReadStream.Source()->SizeL();
	delete iTextTranFromEra;
	iTextTranFromEra=NULL;
	iTextTranFromEra=TTextTranFromEra::NewL(CPlainText::EOrganiseByParagraph, aWriteStream,0);
	}

const TInt KBufSize=256;

TBool CEtToTxtCnv::DoConvertL()
	{
	__ASSERT_DEBUG(iTextTranFromEra, User::Invariant());
	TBuf<KBufSize> buf;
	TInt read = Min(iLength - iPos, KBufSize);
#if defined _UNICODE
	TBuf8<KBufSize> buf8;
	iReadStream->ReadL(buf8, read);
	for(TInt i = 0; i < (buf8.Size() - 1); i += 2)
		{
		buf.Append(buf8[i] | (buf8[i + 1] << 8));
		}
#else
	iReadStream->ReadL(buf,read);
#endif
	iPos += read;
	iTextTranFromEra->Initialise(buf);
	return iTextTranFromEra->DoTranslateL();
	}


TUid CEtToTxtCnv::Uid()
	{
	return KUidEtextToText;
	}

TInt CEtToTxtCnv::Capabilities()
	{
	return EConvertsObjects;
	}

CEtToTxtCnv::CEtToTxtCnv()
	{}

//
// class CTxtToEtCnv
// 

CTxtToEtCnv::~CTxtToEtCnv()
	{
	delete iTextTranToEra;
	}

void CTxtToEtCnv::ConvertObjectAL(RReadStream& aReadStream, RWriteStream& aWriteStream, MConverterUiObserver* /*aObserver*/)
	{
	iReadStream = &aReadStream;
	iWriteStream = &aWriteStream;
	iLength=iReadStream->Source()->SizeL();
	iPos=0;
	delete iTextTranToEra;
	iTextTranToEra=NULL;
	iTextTranToEra=new(ELeave) TTextTranToEra(CPlainText::EOrganiseByParagraph, iNoTrim);
	}

TBool CTxtToEtCnv::DoConvertL()
	{
	__ASSERT_DEBUG(iTextTranToEra, User::Invariant());
	TBuf<KBufSize> buf;
	TInt read = Min(iLength - iPos, KBufSize);
#if defined _UNICODE
	TBuf8<KBufSize> buf8;
	iReadStream->ReadL(buf8, read);
	TInt i = 0;
	for(; i < (buf8.Size() - 1); i += 2)
		{
		buf.Append(buf8[i] | (buf8[i + 1] << 8));
		}
#else
	iReadStream->ReadL(buf, read);
#endif
	iPos += read;
	iTextTranToEra->Translate(buf);
#if defined _UNICODE
	buf8.Zero();
	for (i = 0; i < buf.Length(); i++)
		{
		buf8.Append(buf[i] & 0x00FF);
		buf8.Append((buf[i] >> 8) & 0x00FF);
		}
	iWriteStream->WriteL(buf8);	
#else
	iWriteStream->WriteL(buf);
#endif
	if (iPos < iLength)
		return ETrue; // more to do
	return EFalse;
	}

TUid CTxtToEtCnv::Uid()
	{
	return KUidTextToEtext;
	}

TInt CTxtToEtCnv::Capabilities()
	{
	return EConvertsObjects;
	}

CTxtToEtCnv::CTxtToEtCnv()
	{}
CTxtToEtCnv::CTxtToEtCnv(TBool aNoTrim) : iNoTrim(aNoTrim)
	{}

TTextTran::TTextTran(CPlainText::TTextOrganisation aTextOrganisation)
// C'tor
//
	: iTransMode(aTextOrganisation)
	{}


void TTextTran::SetTranslationMode(CPlainText::TTextOrganisation aTextOrganisation)
//
	{iTransMode=aTextOrganisation;}


CPlainText::TTextOrganisation TTextTran::TranslationMode()const
//
	{return iTransMode;}
	

TTextTranToEra::TTextTranToEra(CPlainText::TTextOrganisation aTextOrganisation, TBool aNoTrim)
// C'tor
//
	: TTextTran(aTextOrganisation)
	{
	iNoTrim=aNoTrim;
	}


void TTextTranToEra::Translate(TDes& aBuf)
// Scans the passed buffer, replacing line delimiters with paragraph delimiters
// based on the translation mode that has been set. (Paragraph or line).
// Control characters in the source, that have meaning to ERA text are translated
// into 'blobs'.
// The descriptor aBuf will never be enlarged, only made smaller!
//
// Unicode specifies discontinuous control code regions.!!!
	{

	iReadPtr=CONST_CAST(TText*,aBuf.Ptr());
	iWritePtr=CONST_CAST(TText*,iReadPtr);
	const TText* basePtr=iReadPtr;
	const TInt length=aBuf.Length();
	while (iReadPtr<basePtr+length)
		{
//		__ASSERT_DEBUG(iReadPtr>=iWritePtr,Panic(EDebug));
		if (*iReadPtr<0x20)
			MapControlCode();  // This is a control character
		else
			{
			*iWritePtr=*iReadPtr;
			iWritePtr++;
			}
		iReadPtr++;
		}
	aBuf.SetLength(iWritePtr-basePtr);
	}


void TTextTranToEra::MapControlCode()
// Translate any control codes encountered into something more reasonable.
// CR's are ignored completely, only LF's influence where paragraph delimiters occur.
//
	{
	switch (*iReadPtr)
		{
		case 0x0c:  // FF
			*iWritePtr=CEditableText::EPageBreak;
			break;
		case 0x09:  // CEditableText::ETabCharacter
			*iWritePtr=*iReadPtr;
			break;  // No translation required
		case 0x0d:  // CR - ignore this character.
			iWritePtr--;  // cos its incremented again outside the switch.
			break;
		case 0x0a:  // LF
			{
			if (iTransMode==CPlainText::EOrganiseByParagraph)
				{
				if(!iNoTrim)
					{
					Trim();  // trims trailng white space preceeding this line delimter.
					}
				*iWritePtr=CEditableText::EParagraphDelimiter;  // all line delimiters become paragraph delimiters
				}
			else
				{// only consecutive line delimiters become paragraph delimiters
				if (*(iReadPtr+1)==0x0a || (*(iReadPtr+1)==0x0d && *(iReadPtr+2)==0x0a))
					{
					Trim();  // trims trailng white space preceeding this line delimter.
					*iWritePtr=CEditableText::EParagraphDelimiter;  // Insert paragraph
					iReadPtr+=(*(iReadPtr+1)==0x0a) ? 1 : 2; // consume this delimiter set.
					}
				else
					*iWritePtr=0x20;  // replace with white space.
				}
			break;
			}
		default:
			// CEditableText::EParagraphDelimiter
			// CEditableText::ELineBreak
			// CEditableText::EPageBreak
			// CEditableText::ENonBreakingTab
			// CEditableText::ENonBreakingHyphen
			// CEditableText::EPotentialHyphen
			// CEditableText::ENonBreakingSpace
			// CEditableText::EPictureCharacter
			*iWritePtr=*iReadPtr;
			break;
		}
	iWritePtr++;
	}


void TTextTranToEra::Trim()
// Removes white space that has already been written before the line delimiter was encountered.
//
	{
	while (*(iWritePtr-1)==0x20)
		iWritePtr--;
	}


TTextTranFromEra* TTextTranFromEra::NewL(CPlainText::TTextOrganisation aTextOrganisation,RWriteStream& aOutStream,TInt aLineWrap)
// Create & initialise a new instance of this class.
//
	{
	TTextTranFromEra* self=new(ELeave) TTextTranFromEra(aTextOrganisation,aOutStream,aLineWrap);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


TTextTranFromEra::TTextTranFromEra(CPlainText::TTextOrganisation aTextOrganisation,RWriteStream& aOutStream,TInt aLineWrap)
// C'tor
//
	: TTextTran(aTextOrganisation),iOutStream(aOutStream),iLineWrap(aLineWrap)

	{
	ResetExportBuffer();
	if (aTextOrganisation==CPlainText::EOrganiseByParagraph)
		iLineWrap=KMaxTInt;
	}


void TTextTranFromEra::ConstructL()
// Initialise this object.
//
	{}


TInt TTextTranFromEra::DoTranslateL()
// Scans the passed buffer, replacing paragraph delimiters with line delimiters (CR/LF)
// based on the translation mode that has been set. (Paragraph or line).
// Special characters in the source, that have meaning to ERA text are translated.
//
	{
	if(iReadPtr<iSBase+iSLen)
		{
		while (iReadPtr<(iSBase+iSLen))
			{
			DoTranslateSourceBufferL();  // returns when export buffer is full or aBuf is empty
			CommitExportBufferToFileL();  // export buffer full
			}
		return ETrue;
		}
	CommitExportBufferToFileL();
	return EFalse;
	}


void TTextTranFromEra::NotifyEndOfSourceL()
// Flush the export buffer.
//
	{CommitExportBufferToFileL();}
	

void TTextTranFromEra::Initialise(const TDesC& aBuf)
//
	{
	iReadPtr=CONST_CAST(TText*,aBuf.Ptr());
	iSBase=iReadPtr;
	iSLen=aBuf.Length();
	}


void TTextTranFromEra::ResetExportBuffer()
//
	{
	iWritePtr=CONST_CAST(TText*,iExportBuffer.Ptr());
	iTBase=iWritePtr;
	}

void TTextTranFromEra::CommitExportBufferToFileL()
//
	{
	iExportBuffer.SetLength(iWritePtr - iTBase);
#if !defined(_UNICODE)
	iOutStream.WriteL(iExportBuffer);
#else // Write wide descriptor to file as byte stream
	TBuf8<KMaxExportBuffer> narrowBuffer;
	TInt i = 0;
	for (; i < (iExportBuffer.Length() / 2); i++)
		{
		narrowBuffer.Append(iExportBuffer[i] & 0x00FF);
		narrowBuffer.Append((iExportBuffer[i] >> 8) & 0x00FF);
		}
	iOutStream.WriteL(narrowBuffer);
	narrowBuffer.Zero();
	for (i = (iExportBuffer.Length() / 2); i < iExportBuffer.Length(); i++)
		{
		narrowBuffer.Append(iExportBuffer[i] & 0x00FF);
		narrowBuffer.Append((iExportBuffer[i] >> 8) & 0x00FF);
		}
	iOutStream.WriteL(narrowBuffer);
#endif
	iExportBuffer.SetLength(0);
	iWritePtr = iTBase;  // Reset the write pointer - fundamental !!!!
	}


void TTextTranFromEra::DoTranslateSourceBufferL()
// Translate the source buffer, writing to the export buffer.
// Returns when either the source has been consumed or the export buffer is full
//
	{
	while (iReadPtr<iSBase+iSLen && iWritePtr<iTBase+KMaxExportBuffer-2*KLineDelimiterLength)
		{
		if ((*iReadPtr<0x20) || (*iReadPtr==CEditableText::EParagraphDelimiter))
			MapEraCodeL();  // May fill export buffer
		else
			{
			*iWritePtr=*iReadPtr;
			iWritePtr++;
			}
		if (iLineWrap!=KMaxTInt && iWritePtr>=iTBase+iLineWrap)
			{
			TText* ptr=iWritePtr;
			TText* end=iWritePtr;
			TText chr(0);
			while ((*(--ptr))!=' ' && ptr>iTBase)		//Allow for more general breaking character
				{}
			if (*ptr!=' ')
				{
				WriteCRLF(KLineDelimiterLength);
				ptr=end;		//So the while below is not executed
				}
			else
				{
				*ptr++=KCharacterCR;
				chr=*ptr;
				*ptr++=KCharacterLF;
				iWritePtr=ptr;
				}
			CommitExportBufferToFileL();
			if (ptr<=end)
				{
				*(iWritePtr++)=chr;
				while (ptr<end)
					{
					*(iWritePtr++)=*(ptr++);
					}
				}
			}
		iReadPtr++;
		}
	}
	

void TTextTranFromEra::MapEraCodeL()
//
	{
	switch (*iReadPtr)
		{
		case CEditableText::EPictureCharacter:
		case CEditableText::ELineBreak:
		case CEditableText::EPageBreak:
		case CEditableText::EPotentialHyphen:
			return;  // These characters are not emitted.
		case CEditableText::ETabCharacter:
			*iWritePtr=KCharacterTab;
			break;
		case CEditableText::ENonBreakingHyphen:
			*iWritePtr=KCharacterHyphen;
			break;
		case CEditableText::ENonBreakingSpace:
			*iWritePtr=KCharacterSpace;
			break;
		case CEditableText::EParagraphDelimiter:
			{
			TInt charsToWrite=KLineDelimiterLength;
			if (iTransMode==CPlainText::EOrganiseByLine)
				charsToWrite=2*KLineDelimiterLength;
			WriteCRLF(charsToWrite);
			if (iTransMode==CPlainText::EOrganiseByLine)
				CommitExportBufferToFileL();  // export buffer
			return;
			}
		default:
			*iWritePtr=*iReadPtr;
			break;
		}
	iWritePtr++;
	}


void TTextTranFromEra::WriteCRLF(TInt aNumCharacters)
//
	{
	while (aNumCharacters>0)
		{
		if (aNumCharacters%2==1)
			*iWritePtr=KCharacterLF;
		else
			*iWritePtr=KCharacterCR;
		aNumCharacters--;
		iWritePtr++;
		}
	}

