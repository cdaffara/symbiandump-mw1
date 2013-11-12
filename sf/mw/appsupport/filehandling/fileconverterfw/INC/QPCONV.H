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

#if !defined(__QPCONV_H__)
#define __QPCONV_H__
#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

const TInt KQuotedPrintableToText=0x10001825;
const TUid KUidQuotedPrintableToText={KQuotedPrintableToText};
const TInt KTextToQuotedPrintable=0x10001826;
const TUid KUidTextToQuotedPrintable={KTextToQuotedPrintable};

const TInt KQuotedPrintableConvertAbove=127;

class CFileStore;
class CStreamDictionary;

NONSHARABLE_CLASS(CQpToTxtCnv) : public CConverterBase2
	{
public:
	CQpToTxtCnv();
public: // from CConverterBase2
	void ConvertObjectAL(RReadStream& aReadStream, RWriteStream& aWriteStream, MConverterUiObserver* aObserver=NULL);
	TBool DoConvertL();
	TInt Capabilities();
	TUid Uid();
private:
	RReadStream* iReadStream;
	RWriteStream* iWriteStream;
	TBuf8<3> iBuffer;
	TInt iBufferPos;
	};

NONSHARABLE_CLASS(CTxtToQpCnv) : public CConverterBase2
	{
public:
	CTxtToQpCnv();
public: // from CConverterBase2
	void ConvertObjectAL(RReadStream& aReadStream, RWriteStream& aWriteStream, MConverterUiObserver* aObserver=NULL);
	TBool DoConvertL();
	TInt Capabilities();
	TUid Uid();
	void FormatAndWriteBufferL(TDes8& aChar);
	void WriteBufferL(TDes8& aChar);
private:
	RReadStream* iReadStream;
	RWriteStream* iWriteStream;
	TInt iLineLen;
	TInt iEnableAllSplCharForQpCnv;
	};
#endif

