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
// concrete conversion for a Plain Text to Quoted Printable
// 
//

#if !defined(__TXCONV_H__)
#define __TXCONV_H__
#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif
#if !defined(__TXTETEXT_H__)
#include <txtetext.h>
#endif


const TInt KEtextToText=0x100040C8;
const TUid KUidEtextToText={KEtextToText};
const TInt KTextToEtext=0x100040C7;
const TUid KUidTextToEtext={KTextToEtext};
const TInt KTextToEtextNoTrim=0x10281B4C;
const TUid KUidTextToEtextNoTrim={KTextToEtextNoTrim};

class CFileStore;
class CStreamDictionary;
//
// Classes declared in this file
class TTextTran;
	class TTextTranToEra;
	class TTextTranFromEra;


NONSHARABLE_CLASS(CEtToTxtCnv) : public CConverterBase2
	{
public:
	CEtToTxtCnv();
	~CEtToTxtCnv();
public: // from CConverterBase2
	void ConvertObjectAL(RReadStream& aReadStream, RWriteStream& aWriteStream,MConverterUiObserver* aObserver=NULL);
	TBool DoConvertL();
	TInt Capabilities();
	TUid Uid();
private:
	RReadStream* iReadStream;
	TTextTranFromEra* iTextTranFromEra;
	TInt iPos;
	TInt iLength;
	};

NONSHARABLE_CLASS(CTxtToEtCnv) : public CConverterBase2
	{
public:
	CTxtToEtCnv();
	CTxtToEtCnv(TBool aNoTrim);
	~CTxtToEtCnv();
public: // from CConverterBase2
	void ConvertObjectAL(RReadStream& aReadStream, RWriteStream& aWriteStream, MConverterUiObserver* aObserver=NULL);
	TBool DoConvertL();
	TInt Capabilities();
	TUid Uid();
private:
	RReadStream* iReadStream;
	RWriteStream* iWriteStream;
	TTextTranToEra* iTextTranToEra;
	TInt iLength;
	TInt iPos;
	TBool iNoTrim;
	};


class TTextTran
	{
	// Text converter
	//
public:
//	virtual void Translate(TDes& aBuf)=0;
	void SetTranslationMode(CPlainText::TTextOrganisation aTextOrganisation);
	CPlainText::TTextOrganisation TranslationMode()const;
protected:
	TTextTran(CPlainText::TTextOrganisation aTextOrganisation);
protected:
	CPlainText::TTextOrganisation iTransMode;  // Paragraph or Line
	TText* iReadPtr;
	TText* iWritePtr;
	};


class TTextTranToEra : public TTextTran
	{
	// Converts plain text to ERA Editable Text format.
	//
public:
	TTextTranToEra(CPlainText::TTextOrganisation aTextOrganisation, TBool aNoTrim=EFalse);
	// Text organisation is the one to read from.
	void Translate(TDes& aBuf);
private:
	void MapControlCode();
	void Trim();
private:
	enum {EBlobCharacter=0x000000b9};
	TBool iNoTrim;
	};


const TInt KMaxExportBuffer=0x100;
const TInt KLineDelimiterLength=2;  // CR/LF protocol
//
const TText KCharacterCR=0x0d;
const TText KCharacterLF=0x0a;
const TText KCharacterTab=0x09;
const TText KCharacterHyphen=0x2d;
const TText KCharacterSpace=0x20;


class TTextTranFromEra : public TTextTran
	{
	// Converts from ERA Editable Text format to plain text.
	//
public:
	static TTextTranFromEra* NewL(CPlainText::TTextOrganisation aTextOrganisation,RWriteStream& aOutStream,TInt aLineWrap);
	TInt DoTranslateL();
	void NotifyEndOfSourceL();
	void Initialise(const TDesC& aBuf);
private:
	TTextTranFromEra(CPlainText::TTextOrganisation aTextOrganisation,RWriteStream& aOutStream,TInt aLineWrap);
	void ConstructL();
	void ResetExportBuffer();
	void CommitExportBufferToFileL();
	void DoTranslateSourceBufferL();
	void MapEraCodeL();
	void WriteCRLF(TInt aCharacters);
private:
	TBuf<KMaxExportBuffer> iExportBuffer;
	TText* iSBase;
	TInt iSLen;
	TText* iTBase;
	RWriteStream& iOutStream;
	TInt iLineWrap;
	};
#endif 
