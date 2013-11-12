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
// Converter Architecture wrappers for base64 conversion
// 
//

#if !defined(__B64CONV_H__)
#define __B64CONV_H__
#include <conarc.h>
#include <s32std.h>

const TInt KBase64Decoder=0x100011C5;
const TUid KBase64DecoderUid={KBase64Decoder};
const TInt KBase64Encoder=0x10000721;
const TUid KBase64EncoderUid={KBase64Encoder};


// classes referenced
class CBase64CodecBase;

NONSHARABLE_CLASS(CBase64Encoder) : public CConverterBase2
// concrete base64 converter
	{
public: // from CConverterBase2
	void ConvertObjectAL(RReadStream& aReadStream, RWriteStream& aWriteStream, MConverterUiObserver* aObserver=NULL);
	TBool DoConvertL();
	TUid Uid();
	TInt Capabilities();
	void CancelConvert();
	~CBase64Encoder();
private:
	CBase64CodecBase* iCodec;
	RReadStream iReadStream;
	RWriteStream iWriteStream;
	MConverterUiObserver* iObserver;
	};

NONSHARABLE_CLASS(CBase64Decoder) : public CConverterBase2
// concrete base64 converter
	{
public: // from CConverterBase2
	void ConvertObjectAL(RReadStream& aReadStream, RWriteStream& aWriteStream, MConverterUiObserver* aObserver=NULL);
	TBool DoConvertL();
	TUid Uid();
	TInt Capabilities();
	void CancelConvert();
	~CBase64Decoder();
private:
	CBase64CodecBase* iCodec;
	RReadStream iReadStream;
	RWriteStream iWriteStream;
	MConverterUiObserver* iObserver;
	};

#endif
